#include "dxstdafx.h"
#include ".\SpriteAnimMgr.h"


CSpriteAnimMgr::CSpriteAnimMgr(LPDIRECT3DDEVICE9 pd3dDevice) :
    m_pd3dDevice(pd3dDevice), m_RunningID(0)
{
}

CSpriteAnimMgr::~CSpriteAnimMgr(void)
{
    for(size_t i=0; i<m_vpTexs.size(); ++i) {
        SAFE_RELEASE(m_vpTexs[i].pTex);        
    }
}

//! Updates running animations
VOID CSpriteAnimMgr::OnFrameMove(float fElapsedTime)
{
    list<SAnimStill>::iterator it = m_lRunningAnimIdx.begin();
    list<SAnimStill>::const_iterator itend = m_lRunningAnimIdx.end();

    for( ; it != itend; ++it)
    {   // if fElapsedTime > speed, get next RECT and reset time.
        if(it->fElapsedTime > m_vAnimations[it->idxAnim].speed) {
            it->idxStill++; // increment still number, wrap around if too big.
            if(it->idxStill >= (int)m_vAnimations[it->idxAnim].vStills.size())
                it->idxStill = 0;            

            // Send texture wrapper, STexture, and still numbe to get new rect.
            UpdateRect(m_vpTexs[m_vAnimations[it->idxAnim].texIdx], 
                        m_vAnimations[it->idxAnim].vStills[it->idxStill],
                        it->frect);                        
            it->fElapsedTime = 0.f;            
        }   
        else
            it->fElapsedTime += fElapsedTime;
    }    
    m_runningIt = m_lRunningAnimIdx.begin();
}

//! Adds a texture to be managed by the Animation Manager. 
//! With the number of stills in each row and column passed.
//! RETURNS: texture index
int CSpriteAnimMgr::AddTexture(LPCWSTR texFileName, int nRows, int nCols)
{
    assert(nRows != 0);
    assert(nCols != 0); // prevent division by 0

    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTex = NULL;
    V( D3DXCreateTextureFromFile(m_pd3dDevice, texFileName, &pTex) );

    D3DSURFACE_DESC desc;
    pTex->GetLevelDesc(0, &desc);

    STexture texWrap;
    texWrap.pTex = pTex;
    texWrap.nWidth = desc.Width;
    texWrap.nHeight = desc.Height;
    texWrap.nRows = nRows;
    texWrap.nCols = nCols;    
    texWrap.fHorizInterv = 1.f / nCols;
    texWrap.fVertInterv = 1.f / nRows;

    m_vpTexs.push_back(texWrap);

    return (int)(m_vpTexs.size() - 1);
}

//! Adds a texture to be managed by the Animation Manager. 
//! With the number of stills in each row and column passed.
//! RETURNS: texture index
int CSpriteAnimMgr::AddTexture(LPCSTR texFileName, int nRows, int nCols)
{
    assert(nRows != 0);
    assert(nCols != 0); // prevent division by 0

    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTex = NULL;
    V( D3DXCreateTextureFromFileA(m_pd3dDevice, texFileName, &pTex) );

    D3DSURFACE_DESC desc;
    pTex->GetLevelDesc(0, &desc);

    STexture texWrap;
    texWrap.pTex = pTex;
    texWrap.nWidth = desc.Width;
    texWrap.nHeight = desc.Height;
    texWrap.nRows = nRows;
    texWrap.nCols = nCols;    
    texWrap.fHorizInterv = 1.f / nCols;
    texWrap.fVertInterv = 1.f / nRows;

    m_vpTexs.push_back(texWrap);

    return (int)(m_vpTexs.size() - 1);
}

//! Adds an animation sequence that refers to an added texture
//! RETURNS: the animations index
int CSpriteAnimMgr::AddAnimation(int idxTex, const vector<int> &vStillIndices, float speed, CSound *pSound)
{
    SAnimation anim;
    anim.texIdx = idxTex;
    anim.vStills.assign(vStillIndices.begin(), vStillIndices.end() );
    anim.speed = speed;
    anim.pSound = pSound;
    m_vAnimations.push_back(anim);
    return (int)(m_vAnimations.size() - 1);
}

//! Adds animation to the running animations list and returns its runningAnimIndex.
//! RETURNS: The runningAnimations index
int CSpriteAnimMgr::StartAnim(int idxAnim)
{
    SAnimStill animStill;
    animStill.idxAnim = idxAnim;
    animStill.idxRunning = m_RunningID++;
    animStill.idxStill = 0;
    animStill.fElapsedTime = 0.f;   

    // Play sound on initial Still
    if(m_vAnimations[idxAnim].pSound)
        m_vAnimations[idxAnim].pSound->Play(0, DSBPLAY_LOOPING);

    m_lRunningAnimIdx.push_front(animStill);

    // update to first frame
    UpdateRect(m_vpTexs[m_vAnimations[idxAnim].texIdx], 
                m_vAnimations[idxAnim].vStills[0],
                m_lRunningAnimIdx.begin()->frect); 

    static int maxThresh = gdef::MAX_INT - 2;    
    static int minThresh = gdef::MIN_INT;
    if(m_RunningID >= maxThresh)
        m_RunningID = minThresh;

    list<SAnimStill>::iterator it = m_lRunningAnimIdx.begin();
    m_hmRunningIterators[animStill.idxRunning] = it;
    return animStill.idxRunning;
}

//! Removes a RUNNING animation from the running animations list. 
//! Can have the same animation running more than once.
VOID CSpriteAnimMgr::StopAnim(int runningAnimIdx)
{   
    list<SAnimStill>::iterator it = m_hmRunningIterators[runningAnimIdx];
    m_lRunningAnimIdx.erase(it);
    m_hmRunningIterators.erase(runningAnimIdx);
}

//! Searches through running anim list and returns Info to display current "Still" of animation.
VOID CSpriteAnimMgr::GetAnimStill(int runningAnimIdx, LPDIRECT3DTEXTURE9 *ppTex, RECTF &frect)
{   
    list<SAnimStill>::iterator it = m_hmRunningIterators[runningAnimIdx];    

    if(it != m_lRunningAnimIdx.end()) 
    {
        *ppTex = m_vpTexs[m_vAnimations[it->idxAnim].texIdx].pTex;
        frect = it->frect;
        return;
    }
    ppTex = NULL;
    assert(false); // break if it reaches here, moron tried to get nonexisting animation still.
}

//! Pops off the next animation from anim manager. qucker than searching for specific index.
//! Returns false when there are more animations. Is reset on OnFrameMove().
bool CSpriteAnimMgr::GetNextAnimStill(LPDIRECT3DTEXTURE9 *ppTex, RECTF &frect)
{
    if(m_runningIt == m_lRunningAnimIdx.end())
        return false;

    *ppTex = m_vpTexs[m_vAnimations[m_runningIt->idxAnim].texIdx].pTex;
    frect = m_runningIt->frect;
    
    m_runningIt++;
    return true;
}

//////////////////////////////////////// PRIVATE FUNCTIONS ///////////////////

VOID CSpriteAnimMgr::UpdateRect(const STexture &texWrap, int nStill, RECTF &frect)
{
    int col = nStill % texWrap.nCols;
    int row = nStill / texWrap.nCols;

    assert(nStill < texWrap.nRows * texWrap.nCols);

    frect.left = col * texWrap.fHorizInterv;
    frect.right = (col + 1)* texWrap.fHorizInterv;
    
    frect.top = row * texWrap.fVertInterv;
    frect.bottom = (row + 1) * texWrap.fVertInterv;
}

