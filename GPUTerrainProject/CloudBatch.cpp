#include "dxstdafx.h"
#include ".\cloudbatch.h"
#include "gamedefine.h"
#include "sprite.h"
#include "globaldefine.h"
#include "impostoratlas.h"
#include "polysprite.h"

using std::endl;

extern bool g_bDrawAlwaysClouds;
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////   CLOUD BATCH FUNCTIONS   ///////////////////////
///////////////////////////////////////////////////////////////////////////////

UINT CCloudBatch::m_UniqueCloudBatchID = 0;

CCloudBatch::CCloudBatch( CPolySprite *pSprite, CImpostorAtlas *pImpostorAtlas ) :
    m_pCloudSprite(pSprite), m_pImpostorAtlas(pImpostorAtlas), m_CloudBatchID(m_UniqueCloudBatchID++), bOwnSprite(false)
{
    assert(pSprite);
    assert(pImpostorAtlas);
}

CCloudBatch::~CCloudBatch(void)
{
    for(size_t i=0;i<m_vpClouds.size(); i++) {
        SAFE_DELETE(m_vpClouds[i]);
    }
}

CCloud* CCloudBatch::AddCloud(const SCloudInfo &ci)
{
    CCloud* pc = new CCloud(m_pCloudSprite, m_pImpostorAtlas, ci, m_CloudBatchID);
    m_vpClouds.push_back(pc);
    return pc;
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////   CLOUD FUNCTIONS   //////////////////////////
///////////////////////////////////////////////////////////////////////////////

CCloud::CCloud(CPolySprite *pSprite, CImpostorAtlas* pImpostorAtlas, const SCloudInfo &ci, UINT batchID) 
    : 
    m_CloudInfo(ci), m_BatchID(batchID), 
    m_bVisible(false), m_bUpdated(false), m_bCheckedForDraw(false),
    m_pSprite(pSprite), m_pImpostorAtlas(pImpostorAtlas), m_nAtlasId(-1),
    m_fTimeSinceLastColorUpdate(0.f), m_fColorTimeThresh(COLOR_TIME_UPDATE), m_bChangedColor(true)

{   // Generate Subclouds based on CloudInfo!
    // have to add in sprite edges outside of bb to box
    float spriteedge = ci.puffScale;
    D3DXVECTOR3 displace(ci.widthhalf+spriteedge, ci.heighthalf+spriteedge, ci.depthhalf+spriteedge);
    gdef::GenerateAABBPosMiddle(m_corners, ci.pos, displace);

    // Find the largest dimension by checking the faces of the AABB.
    // Corner Vectors.  Worst layout ever.
    //  4 --- 5                    3 --- 2 - top
    //  |     |                    |     |
    //  0 --- 1  - bottom face     7 --- 6      
    m_fLargestDim = max( fabs(m_corners[0].x - m_corners[1].x), fabs(m_corners[0].y - m_corners[7].y) );
    m_fLargestDim = max( m_fLargestDim, fabs(m_corners[0].z - m_corners[4].z) );
    assert(m_fLargestDim > 0.f);

    GenerateSubClouds();
}

//! All the pointers belong to the bigger ppl higher up.
CCloud::~CCloud(void)
{
}

VOID CCloud::GenerateSubClouds()
{
    const UINT* cloudPuffs = SCloudInfo::PCTARR_TYPES[m_CloudInfo.CloudType - 1];    

    for(int i=0; i<m_CloudInfo.nPuffs; i++)
    {        
        int npos = cloudPuffs[rand() % 6];
        RECTF rectf = SCloudInfo::GetPuffTexRect(npos);
        D3DXVECTOR3 newpos;
        newpos.x = m_CloudInfo.pos.x + (float)((rand()%100) - 50)/50.0f*m_CloudInfo.widthhalf;
        newpos.y = m_CloudInfo.pos.y + (float)((rand()%100) - 50)/50.0f*m_CloudInfo.heighthalf;
        newpos.z = m_CloudInfo.pos.z + (float)((rand()%100) - 50)/50.0f*m_CloudInfo.depthhalf;
        
        float rotation = (float)((rand()%314)) / 100.0f;
        m_vSubClouds.push_back( (SSubCloud(rectf, newpos, m_CloudInfo.puffScale, rotation, 0.5f)) );

    }
    // Remove Subclouds that are too close together    
    vector<SSubCloud>::iterator it;
    for(it = m_vSubClouds.begin(); it != m_vSubClouds.end(); it++)
    {   // ew my first n^2 pretty much. should sort and then compare. nlogn.
        vector<SSubCloud>::iterator it2 = it;
        it2++;
        while(it2 != m_vSubClouds.end())
        {
            D3DXVECTOR3 vdist = it->pos - it2->pos;            
            if(abs(vdist.x) < SUBCLOUD_DISTANCE_THRESH || abs(vdist.y) < SUBCLOUD_DISTANCE_THRESH || abs(vdist.z) < SUBCLOUD_DISTANCE_THRESH)
                it2 = m_vSubClouds.erase(it2); // returns next element
            else
                it2++;
        }
    }    
}

//! Updates Clouds state (lighting, pos etc.)
VOID CCloud::OnFrameMove(const D3DXVECTOR3* pSunPos, const D3DXVECTOR3* pLightDir, 
                         float fTimeOfDay, const D3DXVECTOR4* pSunColor, float fElapsedTime)
{
#if 0   // We are not doing cloud lighting anymore


    // Update Lighting (Color / Alpha)
    m_fTimeSinceLastColorUpdate += fElapsedTime;
    if(m_fTimeSinceLastColorUpdate > m_fColorTimeThresh) {
        UpdateLighting(pSunPos, pLightDir, fTimeOfDay, pSunColor);
        m_fTimeSinceLastColorUpdate = 0.f;
    }
#endif

    // TODO: Update Position for moving clouds?
}

// More clouds the less quality by taking up less space on tex so it scales.

//! Returns true if it updated Cloud's Impostor.
bool CCloud::UpdateImpostor(IDirect3DDevice9 *pd3dDevice, const D3DXMATRIX* pMatProj, const D3DXMATRIX* pMatView, 
                         const D3DXMATRIX* pMatWorld, const D3DVIEWPORT9* pViewport, 
                         const D3DXVECTOR3* pEyePt, const D3DXVECTOR3* pWorldUp, const D3DXVECTOR3* pRight, const D3DXVECTOR3* pAhead,                          
                         LPDIRECT3DTEXTURE9 pCloudAtlasTex)
{
    //
    // Check to and render impostor if required
    //    
    D3DXVECTOR3 vecDif = m_CloudInfo.pos - *pEyePt; // unnormalized viewpoint normal
    if(m_pImpostorAtlas->CheckToUpdateImpostor(m_bUsingImpostor, pWorldUp, &vecDif, m_fLargestDim, m_nAtlasId, m_bChangedColor))
    {   // Render Updated version to impostor
        HRESULT hr;
        m_bChangedColor = false; // reset color check

        m_pImpostorAtlas->StartRenderToAtlas(pd3dDevice, 
                                             m_corners, // AABB coordinates as defined in our terrible setup at QTNode.
                                             pEyePt, 
                                             &m_CloudInfo.pos, 
                                             pWorldUp, 
                                             pViewport, 
                                             pMatProj, 
                                             pMatWorld,
                                             m_nAtlasId);   // id on atlas that defines location to render to by returning the swap position of the previous.

        //DWORD flag = CPolySprite::BACKTOFRONT | CPolySprite::SCREENALIGN_BILLBOARD; 
        DWORD flag = CPolySprite::SCREENALIGN_BILLBOARD;  // Don't need to sort with the magic of alphas and zwrite
        
        m_pSprite->Begin( flag );
        m_pSprite->SetCameraSpace(pEyePt, pWorldUp, pRight, pAhead);

        //
        // Specific stupid cloud to impostor necessities
        //

        // Testing out ZWRITE with impostors and actual clouds
        V( pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE) );
        V( pd3dDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE) );
        V( pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER) );
        V( pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)CLOUD_ALPHA_CUTOFF));

        // Defines Color/Alpha operations with texture and world
        V( pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE) );
        V( pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO) );

        V( pd3dDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE) );
        V( pd3dDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA) );
        /*
        V( pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR) );
        V( pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO) );

        V( pd3dDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE) );
        V( pd3dDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA) );
        */

        RenderThruSprite(m_pSprite, pCloudAtlasTex);

        m_pSprite->End();
                
        V( pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE) );
        V( pd3dDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE) );

        m_pImpostorAtlas->StopRenderToAtlas();
        return true; // Return true: Rendered Impostor
    }
    return false;
}

HRESULT CCloud::RenderThruSprite(CPolySprite* pSprite, LPDIRECT3DTEXTURE9 pTex)
{
    for(size_t i=0, iend=m_vSubClouds.size(); i<iend; i++)
    {   // Render each subcloud
        pSprite->Draw( pTex, &m_vSubClouds[i].srcRectf, &m_vSubClouds[i].mWorld, false, m_vSubClouds[i].color);
    }
    return S_OK;
}
//! Calculates the lighting oawwwwwwwf all SubClouds inside cloud based on the closest subcloud.
//! TODO: This whole function sucks, scrap the whole cloud color change at night and reserve only for events.
VOID CCloud::UpdateLighting(const D3DXVECTOR3* pSunPos, const D3DXVECTOR3* pLightDir, 
                            float fTimeOfDay, const D3DXVECTOR4* pSunColor)
{
#if 0   // Not doing this stupid shit. Costly and doesn't even look that good without real
        // light scattering models
    D3DXVECTOR3 dif;
    vector<float> vDistance;
    float min = gdef::MAX_FLOAT, max = -1.f;
    // Find subcloud closest to sun
    for(size_t i=0, iend=m_vSubClouds.size(); i<iend; ++i)
    {
        dif = m_vSubClouds[i].pos - *pSunPos;
        vDistance.push_back(D3DXVec3LengthSq(&dif));
        if( vDistance[i] < min )
            min = vDistance[i];
        if( vDistance[i] > max )
            max = vDistance[i];
    }
    float inverse = 1 / (max - min);
#endif

    // Deal with color(shading)
    UINT c = CLOUD_DAY_SHADE;
    UCHAR dif = CLOUD_NIGHT_DIF;
    if(dif) { // if cloud not always white
        static float one_over_six = 1.f/6.f;
        fTimeOfDay -= 12.f;
        if(fTimeOfDay > 0.f)
        {   // Night time
            if( fTimeOfDay > 6.f) // approaching sunrise
                c -= (UCHAR) ( (12.f - fTimeOfDay) * one_over_six * dif );
            else    // leaving sunset
                c -= (UCHAR) ( fTimeOfDay * one_over_six * dif );
            

            gdef::COLOR color(0xff, (UCHAR)c, (UCHAR)c, (UCHAR)c);

            for(size_t i=0, iend=m_vSubClouds.size();i<iend;++i)
            {   // Give shading to subcloud based on square distance
                SSubCloud *psc = &m_vSubClouds[i];                
                memcpy(&psc->color, &color, 4);        
            }
            m_bChangedColor = true;
        }
        else
            m_bChangedColor = false;
    }
    else
        m_bChangedColor = false;        

    // TODO: Alphas on clouds moving in and out of scene?
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////// SUBCLOUD /////////////////////////////////////////

CCloud::SSubCloud::SSubCloud(const RECTF &_srcRectf, const D3DXVECTOR3 &_pos, float _scale, float _rotation, float _vertInterp) :
    srcRectf(_srcRectf), pos(_pos), scale(_scale), rotation(_rotation), vertInterp(_vertInterp), color(0xFFFFFFFF)
{
    D3DXMATRIX mtxTrans, mtxScale, mtxRot;
    D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
    D3DXMatrixScaling(&mtxScale, scale, scale, scale);
    D3DXMatrixRotationZ(&mtxRot, rotation);    

    mWorld = mtxScale * mtxRot * mtxTrans;
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////   CLOUDINFO FUNCTIONS   //////////////////////
///////////////////////////////////////////////////////////////////////////////

SCloudInfo::SCloudInfo() :
    CloudType(0), pos(0.0f,0.0f,0.0f), heighthalf(0), widthhalf(0), depthhalf(0), nPuffs(0), puffScale(0)
{
}

SCloudInfo::SCloudInfo(UINT cloudType, D3DXVECTOR3 _pos, int _puffs, float _pfScale, float _width, float _height, float _depth) :
    CloudType(cloudType), pos(_pos), heighthalf(_height), widthhalf(_width), depthhalf(_depth), nPuffs(_puffs), puffScale(_pfScale)
{
}

const UINT SCloudInfo::CTARR_NIMBOSTRATUS[6]     =   { 0, 1, 2, 0, 6, 2 };
const UINT SCloudInfo::CTARR_STRATOCUMULUS[6]    =   { 0, 1, 2, 3, 7, 1 };
const UINT SCloudInfo::CTARR_FAIRCUMULUS[6]      =   { 10, 11, 12, 13, 14, 15 };
const UINT SCloudInfo::CTARR_CUMULUSNIMBUS[6]    =   { 3, 12, 13, 14, 15, 0 };
const UINT SCloudInfo::CTARR_ALTOCUMULUS[6]      =   { 6, 10, 5, 11, 8, 12 };
const UINT SCloudInfo::CTARR_CIRRUS[6]           =   { 4, 5, 6, 7, 8, 9 };

const UINT* SCloudInfo::PCTARR_TYPES[6]           = { SCloudInfo::CTARR_NIMBOSTRATUS, 
                                                     SCloudInfo::CTARR_STRATOCUMULUS,
                                                     SCloudInfo::CTARR_FAIRCUMULUS,
                                                     SCloudInfo::CTARR_CUMULUSNIMBUS,
                                                     SCloudInfo::CTARR_ALTOCUMULUS,
                                                     SCloudInfo::CTARR_CIRRUS };

//! Returns the srcRect in the texture that holds the pos' texture 0-15 row first.
RECTF SCloudInfo::GetPuffTexRect(int pos)
{       
    static float invCloudBlocks = 1.f / CLOUD_BLOCKS_PER_DIMENSION;

    int col = pos % CLOUD_BLOCKS_PER_DIMENSION;
    int row = pos / CLOUD_BLOCKS_PER_DIMENSION;

    RECTF rectf;
    rectf.left = col * invCloudBlocks;
    rectf.right = (col+1) * invCloudBlocks;

    rectf.top = row * invCloudBlocks;
    rectf.bottom = (row+1) * invCloudBlocks;

    return rectf;
}

SCloudInfo::~SCloudInfo()
{
}

ostream& operator<< (ostream &os, const SCloudInfo& ci) 
{
    os << ci.CloudType << " " << ci.pos.x << " " << ci.pos.y << " " << ci.pos.z << " ";
    os << ci.nPuffs << " " << ci.puffScale << " " << ci.widthhalf << " " << ci.heighthalf << " " << ci.depthhalf << endl;

    return os;    
}

istream& operator>> (istream &is, SCloudInfo& ci)
{
    is >> ci.CloudType >> ci.pos.x >> ci.pos.y >> ci.pos.z >>
        ci.nPuffs >> ci.puffScale >> ci.widthhalf >> ci.heighthalf >> ci.depthhalf;

    return is;
}

