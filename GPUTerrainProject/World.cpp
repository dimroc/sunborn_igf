#include "dxstdafx.h"
#include "World.h"

#include "bubbles.h"
#include "cloudbatch.h"
#include "cloudsky.h"
#include "mipmapterrain.h"
#include "pillar.h"
#include "postproceffects.h"
#include "shadowmap.h"

#include "XModelHeirarchy.h"
#include "ModelInstance.h"

#include "GameTables.h"
#include "gamedefine.h"

#include "PolySprite.h"
#include "SpriteAnimMgr.h"

#include "SplashScreen.h"

using std::stringstream;

#define             SPRING_ENERGY_LOSS                              0.5f 

#define             BUBBLE_RESOLUTION                               30

#define             SHADOW_UPDATE_LIMIT                             0

#define             FISHEYE_MULT                                    4.f

//! Number of impostors to update per frame inside QuadTree
#define             CLOUD_IMPOSTOR_UPDATES_PER_FRAME                8 
//! Quality of updates depends on number of impostors in scene. Could cause flickering if too low.
//! Higher FPS you get, smoother it looks, test in RELEASE.

CWorld::CWorld(const string& levelFN, const string& modelFN, const string& charFN, const string& spriteFN) :
    m_BLOOMFLAGCOLOR(0, 0, 255, 0), // pure green
    m_strWorldFileName(levelFN), m_strModelFileName(modelFN), m_strCharacterFileName(charFN), m_strSpriteFileName(spriteFN),
    m_nCharacterKey(0),
    m_bDrawBBs(false), m_bSaveShadowMap(false), 
    m_nShadowUpdateLimit(SHADOW_UPDATE_LIMIT), m_nShadowUpdateCount(SHADOW_UPDATE_LIMIT),
    m_bCameraControl(false), m_bFirstRun(true),
    m_pSpriteAnimMgr(NULL), m_pPolySprite(NULL)
{    
    D3DXMatrixIdentity(&m_mIdentity);
    m_pPPEffects = new PostProcEffects();
    // m_pPPEffects->ToggleBloom();    // makes bloom false
    // m_pPPEffects->ToggleGrey();

    SetupKernels();    

    m_pShadowMap = new CShadowMap();

    // Populate Info arrays/structs with world info from file.
    int success = LoadWorldFile(levelFN.c_str());
    if(success == EXIT_FAILURE)
    {
        SAFE_DELETE(m_pPPEffects);
        memset(this, 0, sizeof(CWorld));
        dprintf("CWorld constuctor took a shit in LoadWorldFile\n");
        return ;
    }
    success = LoadModelFile(modelFN.c_str());
    if(success == EXIT_FAILURE)
    {
        SAFE_DELETE(m_pPPEffects);
        memset(this, 0, sizeof(CWorld));
        dprintf("CWorld constuctor took a shit in LoadModelFile\n");
        return ;
    }

    D3DXVECTOR3 vSkyCenter(0.f, 0.f, 0.f);
    m_pCloudSky = new CCloudSky(m_CloudSkyInfo.strCloudMap.c_str(), 
        m_CloudSkyInfo.strCloudAtlasTex.c_str(),
        m_CloudSkyInfo.strSkyToneTex.c_str(),
        &vSkyCenter,
        m_CloudSkyInfo.fSkyRadius);

    // Initialize camera parameters        
    m_Camera.SetEnableYAxisMovement( true );
    m_Camera.SetRotateButtons( true, false, false);
    m_Camera.SetScalers( 0.005f, GROUND_CAMERA_SPEED );

    memset(&m_CameraMovement, 0, sizeof(SCameraMovement));

    D3DXVECTOR3 vecEye(0.0f, 6000.0f, 0.0f);    
    D3DXVECTOR3 vecAt (0.f, 0.f, 0.f);
    
    m_CameraMovement.vLastWorldPos = vecEye;
    m_CameraMovement.vLastLookAt = vecAt;    

    m_Camera.SetViewParams( &vecEye, &vecAt );     

    m_bInit = true;
    m_bEditPillar  = false;
}

CWorld::~CWorld(void)
{
    SAFE_DELETE(m_pCloudSky);
    SAFE_DELETE(m_pBubbles);
    for(size_t i=0; i<m_vpIslands.size(); ++i) {
        SAFE_DELETE(m_vpIslands[i]);
    }
    for(size_t i=0; i<m_vpCharacterModels.size(); ++i) {
        SAFE_DELETE(m_vpCharacterModels[i]);
    }
    for(size_t i=0; i<m_vpStaticModels.size(); ++i) {
        SAFE_DELETE(m_vpStaticModels[i]);
    }
    EntityHashMap::iterator it = m_hmEntities.begin();
    for( ; it != m_hmEntities.end(); it++) {
        SAFE_DELETE(it->second.pModelInst);        
    }
    for(size_t i=0; i<m_vCharacterTextures.size(); i++) {
        SAFE_RELEASE(m_vCharacterTextures[i]);
    }

    SAFE_DELETE(m_pPolySprite);
    SAFE_DELETE(m_pSpriteAnimMgr);

    SAFE_DELETE(m_pShadowMap);
    SAFE_DELETE(m_pPPEffects);

    SAFE_RELEASE(m_pNotBloomMapSurface);
    SAFE_RELEASE(m_pNotBloomMap);

    SAFE_RELEASE( m_pFont );
}

// TODO: Try a 5x5 gaussian kernel in PPEffects.
/*
And here's weighted 3x3 smoothing kernel also known as Gaussian Blur:

0 1 0
1 4 1
0 1 0

a 5x5 gaussian smoothing kernel:

0 1 2 1 0
1 4 8 4 1
2 8 16 8 2
1 4 8 4 1
0 1 2 1 0
*/
VOID CWorld::SetupKernels() 
{
    // Set up ghetto 3x3 gaussian kernel for ppeffect
    m_mGaussianKernel.vals[0] = 0.f;
    m_mGaussianKernel.vals[1] = 0.125f;
    m_mGaussianKernel.vals[2] = 0.f;

    m_mGaussianKernel.vals[3] = 0.125f;
    m_mGaussianKernel.vals[4] = 0.55f;
    m_mGaussianKernel.vals[5] = 0.125f;

    m_mGaussianKernel.vals[6] = 0.f;
    m_mGaussianKernel.vals[7] = 0.125f;
    m_mGaussianKernel.vals[8] = 0.f;
}

//////////////////////////// FILE LOAD FUNCTIONS ////////////////////////

UINT CWorld::LoadWorldFile(LPCSTR pszFN) 
{   //! Load LEVEL
    char dir[1000];
    _getcwd(dir, 999);
    _chdir("..\\Media");
    dprintf("LOADING LEVEL: %s%s\n", dir, pszFN);

    std::ifstream fin(pszFN);
    assert(fin.good());
    if(!fin.good()) {
        fin.close();
        _chdir(dir);
        return EXIT_FAILURE;
    }

    // Load CloudSkyInfo
    fin >> m_CloudSkyInfo.strCloudMap >> m_CloudSkyInfo.strCloudAtlasTex >> m_CloudSkyInfo.strSkyToneTex >> m_CloudSkyInfo.fSkyRadius;

    // Load BubbleInfo
    int nLength = 1;    // HACK: ASSUMING THERE IS ALWAYS ONE BUBBLE PER LEVEL FOR NOW.
    for(int i=0;i<nLength;++i) {
        SBubbleInfo bi;
        fin >> bi.strTex 
            >> bi.strNormalMap             
            >> bi.fHorTile
            >> bi.fVerTile
            >> bi.fRadius 
            >> bi.vCenter.x >> bi.vCenter.y >> bi.vCenter.z;
        m_vBubbleInfo.push_back(bi);
    }
    if(nLength < 1)
        return EXIT_FAILURE;

    // Load IslandInfo
    nLength = 0;
    fin >> nLength;
    for(int i=0;i<nLength;++i) {
        SQuadTreeInfo island;
        fin >> island.strHMP 
            >> island.strBlendMap 
            >> island.strRedTexture
            >> island.strGreenTexture
            >> island.strBlueTexture
            >> island.strAlphaTexture            
            >> island.fTextureTile
            >> island.nCellSpacing
            >> island.nHeightSpacing            
            >> island.vTranslation.x >> island.vTranslation.y >> island.vTranslation.z;    

        // Pillar time
        SPillarInfo pillar;
        fin >> pillar.strTexName
            >> pillar.cylinderRadius
            >> pillar.splineRadius
            >> pillar.Length
            >> pillar.splineAngle
            >> pillar.numSlices
            >> pillar.numStacks
            >> pillar.curveStacks
            >> pillar.curveHeight
            >> pillar.texWrapLength;

        island.strPillarTexture = pillar.strTexName;

        m_vIslandInfo.push_back(island);
        m_vPillarInfo.push_back(pillar);    
    }

    //
    // Load Sunstates time
    //
    char buf[256];        
    SSunMorphState sun;
    while(fin.good())
    {   
        fin.getline(buf, 255);
        stringstream ss(buf);
        string strName;
        ss >> strName;
        if(strName.size() == 0)
            continue;        
        if(strName.compare("#") == 0)
            break;
        ss >> sun.color.x >> sun.color.y >> sun.color.z >> sun.color.w;
        ss >> sun.fMorphTime >> sun.vspower >> sun.pspower >> sun.vsmult;

        UCHAR sunKey = GameTable::FindSunStateIndex(strName);
        if(sunKey != GameTable::SUNSTATE_NO)
            m_hmSunStates[sunKey] = sun;
    }

    fin.close();
    _chdir(dir);
    if(nLength < 1)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

UINT CWorld::LoadModelFile(LPCSTR pszFN) 
{   //! Load LEVEL
    char dir[1000];
    _getcwd(dir, 999);
    _chdir("..\\Media");
    dprintf("LOADING MODEL SET: %s%s\n", dir, pszFN);

    std::ifstream fin(pszFN);
    assert(fin.good());
    if(!fin.good()) {
        fin.close();
        _chdir(dir);
        return EXIT_FAILURE;
    }

    // Get geometries to load
    int n = 0;
    string strModel;
    fin >> n;
    for(int i=0;i<n && fin.good();i++) {
        fin >> strModel;
        m_vGeometryFilenames.push_back(strModel);
    }

    // Setup instances to be created.
    // geom index(int) - scale(d3dxvector3) - angle(d3dxvector3) - position(d3dxvector3)
    fin >> n;
    SStaticEntityInfo ei;
    for(int i=0;i<n && fin.good(); i++)
    {
        fin >> ei.geomID
            >> ei.scale.x >> ei.scale.y >> ei.scale.z
            >> ei.angle.x >> ei.angle.y >> ei.angle.z            
            >> ei.pos.x >> ei.pos.y >> ei.pos.z;
        if(ei.geomID >= 0 && ei.geomID < (int)m_vGeometryFilenames.size())
            m_vStaticEntityInfo.push_back(ei);
        else {
            DebugBreak();
            fin.close();
            return EXIT_FAILURE;
        }
    }
    // sort static entities by geomID so we create and batch render later when going thru vector 0->n.
    std::sort(m_vStaticEntityInfo.begin(), m_vStaticEntityInfo.end());
    fin.close();
    _chdir(dir);
    return EXIT_SUCCESS;
}

//! Loads the textures and the sprite animations in the file.
UINT CWorld::LoadSpriteAnimationsFile(LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR pszFN)
{
    using std::stringstream;

    char dir[1000];
    _getcwd(dir, 999);
    _chdir("..\\Media");
    dprintf("LOADING SPRITE ANIMATION SET: %s%s\n", dir, pszFN);

    std::ifstream fin(pszFN);
    assert(fin.good());
    if(!fin.good()) {
        fin.close();
        _chdir(dir);
        return EXIT_FAILURE;
    }

    // 
    // Get Textures
    //
    int nTexs;	
    fin >> nTexs;	
    for(int i=0; i<nTexs && fin.good(); i++)
    {
        string strTex;
        int nRows, nCols;

        fin >> strTex >> nRows >> nCols;
        // load texture
        m_pSpriteAnimMgr->AddTexture(strTex.c_str(), nRows, nCols);		
    }

    //
    // Get and load Sprite Animations
    //
    int nAnims;
    fin >> nAnims;
    for(int i=0; i<nAnims && fin.good(); i++)
    {
        SSpriteAnimationEntry sentry;
        string entryTag;

        fin >> entryTag;
        fin >> sentry.idxTex;

        int nStills;
        fin >> nStills;
        for(int j=0; j<nStills; j++)
        {
            int still;
            fin >> still;
            sentry.vStills.push_back(still);
        }
        fin >> sentry.fSpeed;

        sentry.pSound = NULL;
        D3DXMatrixIdentity(&sentry.mWorld);

        UCHAR entryID = GameTable::FindSpriteAnimationIndex(entryTag);
        // add to sprite manager
        sentry.idxAnim = m_pSpriteAnimMgr->AddAnimation(sentry.idxTex, sentry.vStills, sentry.fSpeed, sentry.pSound);

        m_hmAnmEntries[entryID] = sentry;	// populate hash map of animation entries.
    }

    fin.close();
    _chdir(dir);
    return EXIT_SUCCESS;
}

// loads n characters with m animations
UINT CWorld::CreateCharacterGeoms( LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR charFN ) 
{
    using namespace GameTable;    

    char dir[1000];
    _getcwd(dir, 999);
    _chdir("..\\Media");
    dprintf("LOADING/CREATING CHARACTER SET: %s%s\n", dir, charFN);

    std::ifstream fin(charFN);
    assert(fin.good());
    if(!fin.good()) {
        fin.close();
        _chdir(dir);
        return EXIT_FAILURE;
    }

    int nChars, nAnims;
    char dels[] = " ,\t\n=";
    char buf[256];
    char *pos;

    fin >> nChars;
    for(int i=0;i<nChars && fin.good(); i++)
    {
        // get mesh and registered vertex animation.
        string mesh, registermesh;
        fin >> mesh;
        fin >> registermesh;

        CXModelHeirarchy* pm = new CXModelHeirarchy(pd3dDevice, mesh.c_str(), true);

        AnimationKeyIndexHashMap hmAnim;
        hmAnim[ANIM_NO] = pm->AddAnimation(pd3dDevice, registermesh.c_str());

        fin >> nAnims;

        for(int j=0; j<nAnims; j++)
        {
            fin.getline(buf, 255);

            char *token = strtok_s(buf, dels, &pos);
            if(token == NULL)
            {
                j--;
                continue;   // skip blank lines
            }

            string anim(token);
            UCHAR animIdx = FindAnimationIndex(anim);

            if(animIdx > GameTable::ANIM_NO)
            {   // if animation index is void (ie valid tag for animation like 'run'). See GameTables.h.
                token = strtok_s(NULL, dels, &pos);
                hmAnim[animIdx] = pm->AddAnimation(pd3dDevice, token);
            }
        }
        m_vpCharacterModels.push_back(pm);
        m_vhmAnimationKeyIndex.push_back(hmAnim);
    }

    // get alternate character textures
    int nAltTexs;
    fin >> nAltTexs;
    for(int i=0; i<nAltTexs; i++)
    {
        LPDIRECT3DTEXTURE9 pTex;
        string fn;
        fin >> fn;
        D3DXCreateTextureFromFileA(pd3dDevice, fn.c_str(), &pTex);
        m_vCharacterTextures.push_back(pTex);
    }

    fin.close();
    _chdir(dir);
    return EXIT_SUCCESS;
}

HRESULT CWorld::CreateStaticGeomsEntities( LPDIRECT3DDEVICE9 pd3dDevice )
{
    char buf[1000];
    _getcwd(buf, 999);
    _chdir("..\\Media");
    dprintf("CREATING STATIC ENTITIES\n");

    // load the geoms
    for(size_t i=0;i<m_vGeometryFilenames.size(); i++)
    {
        CXModelHeirarchy* pm = new CXModelHeirarchy(pd3dDevice, m_vGeometryFilenames[i].c_str(), false);
        m_vpStaticModels.push_back(pm);
    }

    // create the entity instances
    for(size_t i=0;i<m_vStaticEntityInfo.size();i++)
    {
        SMeshEntity ent;
        ent.bVisible = ent.bRequestVisible = true;
        ent.geomID = m_vStaticEntityInfo[i].geomID;
        assert(ent.geomID < (int)m_vpStaticModels.size());
        ent.pModelInst = new CModelInstance(m_vpStaticModels[ent.geomID]);		
        ent.pTex = NULL;

        // Change the toon shading properties.
        ent.pModelInst->SetMinShade(ent.pModelInst->GetMinShade() - 2);
        // ent.pModelInst->SetNumShades(ent.pModelInst->GetNumShades() + 1);
        ent.pModelInst->SetSilhouetteThresh(0.1f);

        // create the world matrix from info. SRT.		
        D3DXMatrixScaling(&ent.mWorld, m_vStaticEntityInfo[i].scale.x, m_vStaticEntityInfo[i].scale.y, m_vStaticEntityInfo[i].scale.z);

        D3DXMATRIX mtx, mtx2, mtx3;
        D3DXMatrixRotationX(&mtx, m_vStaticEntityInfo[i].angle.x);
        D3DXMatrixRotationY(&mtx2, m_vStaticEntityInfo[i].angle.y);
        D3DXMatrixRotationZ(&mtx3, m_vStaticEntityInfo[i].angle.z);
        ent.mWorld *= mtx * mtx2 * mtx3;

        D3DXMatrixTranslation(&mtx, m_vStaticEntityInfo[i].pos.x, m_vStaticEntityInfo[i].pos.y, m_vStaticEntityInfo[i].pos.z);
        ent.mWorld *= mtx;

        // Create the AABB        
        const D3DXVECTOR3* pDisplace = ent.pModelInst->GetObjectExtremities();
        const D3DXVECTOR3* pCenter = ent.pModelInst->GetObjectCenter();

        ent.displace.x = m_vStaticEntityInfo[i].scale.x * pDisplace->x;
        ent.displace.y = m_vStaticEntityInfo[i].scale.y * pDisplace->y;
        ent.displace.z = m_vStaticEntityInfo[i].scale.z * pDisplace->z;

        D3DXVECTOR3 newCenter = m_vStaticEntityInfo[i].pos + *pCenter;
        gdef::GenerateAABBPosMiddle(ent.corners, newCenter, ent.displace);		

        m_hmEntities[m_nCharacterKey] = ent;
        IncrementCharKey();
    }
    _chdir(buf);
    return S_OK;
}

///////////////////////////// CHARACTER ENTITY FUNCTIONS /////////////////////////    

//! Creates a playable character, initially invisible, returns its graphics id.
int CWorld::CreateCharacter(UINT geomID)
{    
    using namespace GameTable;

    if(geomID >= (UINT)m_vpCharacterModels.size()) {
        DebugBreak();
        return 0;
    }
    AnimationKeyIndexHashMap &hmAnimKey = m_vhmAnimationKeyIndex[geomID];

    SMeshEntity ent;
    ent.bVisible = false;
    ent.bRequestVisible = true;
    ent.geomID = geomID;
    ent.pModelInst = new CModelInstance( m_vpCharacterModels[geomID]);
    
    ent.pModelInst->SetAnimationSet(hmAnimKey[GameTable::ANIM_IDLE]);

    D3DXMatrixIdentity(&ent.mWorld);
    float s = ent.pModelInst->GetObjectRadius();
    ent.displace.x = s; ent.displace.y = s; ent.displace.z = s;

    // cycle thru the max number of textures so player can differentiate between pplz.
    ent.texID = m_nCharacterKey % m_vCharacterTextures.size(); 
    ent.pTex = GetCharacterTexture(ent.texID);

    m_hmEntities[m_nCharacterKey] = ent;
    m_vCharIDs.push_back(m_nCharacterKey);

    int nReturn = m_nCharacterKey;
    IncrementCharKey(); // increments character key to the next available int value in m_vCharIDs.
    return nReturn;
}

VOID CWorld::SetCharacterAnimation(UINT charID, UCHAR animID, bool bLoopAble, float fPeriodTime)
{
    SMeshEntity* pent = &m_hmEntities[charID];
    AnimationKeyIndexHashMap &hmAnim = m_vhmAnimationKeyIndex[pent->geomID];
    // check if animID exists
    AnimationKeyIndexHashMap::iterator it = hmAnim.find(animID);
    if(it != hmAnim.end())
    {   // found

        // figure out speed of animation to match fPeriodTime.
        if(fPeriodTime > 0.f)
            pent->pModelInst->AdjustAnimationSpeed(it->second, (float)pent->pModelInst->GetAnimationPeriod(it->second) / fPeriodTime);
        else
            pent->pModelInst->AdjustAnimationSpeed(it->second, 1.f);

        // play looped or just once.
        if(bLoopAble)
            pent->pModelInst->SetAnimationSet(it->second);
        else
            pent->pModelInst->PlayAnimationSetOnce(it->second);
    }
    else
    {   // otherwise set to the bogus NO_ANIM.        
        pent->pModelInst->SetAnimationSet(hmAnim[GameTable::ANIM_NO]);        
    }
}

VOID CWorld::SetCharacterWorldMatrix(UINT charID, const D3DXMATRIX* pWorld, bool bRequestVisible)
{   // TODO: Update Scale of ent.scale if world matrix scales.
    SMeshEntity* pent = &m_hmEntities[charID];
    D3DXVECTOR3 loc(pWorld->_41, pWorld->_42, pWorld->_43);
    gdef::GenerateAABBPosMiddle(pent->corners, loc, pent->displace);
    pent->mWorld = *pWorld;
    pent->bRequestVisible = bRequestVisible;
}

const D3DXVECTOR3* CWorld::GetCharacterObjectCenter(UINT charID) 
{ 
    return m_hmEntities[charID].pModelInst->GetObjectCenter(); 
}

float CWorld::GetCharacterRadius(UINT charID) 
{ 
    return m_hmEntities[charID].pModelInst->GetObjectRadius(); 
}


VOID CWorld::ToggleCharacterVisibility(UINT charID)
{
    SMeshEntity* pent = &m_hmEntities[charID];
    pent->bRequestVisible = !pent->bVisible;
}

VOID CWorld::DeleteCharacter(UINT charID)
{
    SMeshEntity* pent = &m_hmEntities[charID];
    SAFE_DELETE(pent->pModelInst);    

    m_vCharIDs.erase(std::find(m_vCharIDs.begin(), m_vCharIDs.end(), charID));
    m_hmEntities.erase(charID);
}

VOID CWorld::DeleteAllCharacters()
{
    for(size_t i=0; i<m_vCharIDs.size(); i++)
    {
        SMeshEntity* pent = &m_hmEntities[m_vCharIDs[i]];
        SAFE_DELETE(pent->pModelInst);
        m_hmEntities.erase(m_vCharIDs[i]);
    }
    m_vCharIDs.clear();
}

LPDIRECT3DTEXTURE9 CWorld::GetCharacterTexture(int texID)
{
    assert(texID >= 0 && texID < m_vCharacterTextures.size());
    return m_vCharacterTextures[texID];
}

//! pushes the sun request onto the queue in m_pCloudSky.
VOID CWorld::RequestSunState(GameTable::SunStates eSunState, int SunIdx)
{
    assert(SunIdx >= 0 && SunIdx < 2);
    m_pCloudSky->PushBackSunState(m_hmSunStates[(UCHAR)eSunState], SunIdx);
}

//! Repositions the 'sun' light based on the passed in TimeOfDay.
VOID CWorld::RequestLightDirByTimeOfDay(float fLightDirByTimeOfDay)
{   
    m_pCloudSky->SetLightDirByTimeOfDay(fLightDirByTimeOfDay);
}
///////////////////////////// D3D FUNCTIONS /////////////////////////    

HRESULT CWorld::OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{    
    SplashScreen *loadScr = new SplashScreen(300, 400);

    HRESULT hr; 

    if(!m_bInit)
        return S_FALSE;
    m_pd3dDevice = pd3dDevice;

    m_pSpriteAnimMgr = new CSpriteAnimMgr(pd3dDevice);
    m_pPolySprite = new CPolySprite(pd3dDevice);

    // Create Font
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_EXTRABOLD, 1, FALSE, DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
        L"Verdana", &m_pFont ) );

    // Create Islands.
    for(size_t i=0; i<m_vIslandInfo.size(); i++) {        
        m_vpIslands.push_back( new CQuadTree(&m_vIslandInfo[i], &m_vPillarInfo[i]) );
        m_vpIslands[i]->OnCreateDevice(pd3dDevice);
    }

    // Add Bubbles to Islands.
    m_pBubbles = new CBubbles( pd3dDevice, BUBBLE_RESOLUTION, BUBBLE_RESOLUTION );

    for(size_t i=0;i<m_vBubbleInfo.size();++i)
    {
        m_pBubbles->AddBubble( pd3dDevice, m_vBubbleInfo[i].vCenter, 
            m_vBubbleInfo[i].fRadius, m_vBubbleInfo[i].fHorTile, m_vBubbleInfo[i].fVerTile, 
            m_vBubbleInfo[i].strTex.c_str(), m_vBubbleInfo[i].strNormalMap.c_str() );
    }

    //! Load up Clouds and populate the QTNodes with clouds in its space. This gets spaced out with terrain.
    m_pCloudSky->OnCreateDevice(pd3dDevice);

    for(size_t i=0; i<m_vpIslands.size(); ++i) {
        m_pCloudSky->PopulateLeafNodesWithClouds(m_vpIslands[i]->m_root);        
    }

    //
    // Load up all x file models and create the static entities
    //
    CreateCharacterGeoms(pd3dDevice, m_strCharacterFileName.c_str());
    CreateStaticGeomsEntities( pd3dDevice );

    //
    // Load up all the sprite animations
    //
    LoadSpriteAnimationsFile(pd3dDevice, m_strSpriteFileName.c_str());

    //
    // Place objects that are in limbo in their appropriate vector.
    //
    PopulateLimboVectors();
    
    delete loadScr;

    return S_OK;
}

//! Calculations to be done before Render. Also Renders to texture for ImpostorAtlas stff such as those in CloudSky.
VOID CWorld::OnFrameMove( const double fTime, const float fElapsedTime )
{
    using std::deque;
    assert(m_pBubbles->GetNumBubbles() > 0);

    // updates all the sprite animations
    UpdateSpriteAnimations(fElapsedTime);

    // Update the camera's position based on user input            
    UpdateCameraOrientation(fElapsedTime);
    m_Camera.FrameMove( fElapsedTime );

    // Get View Frustum Planes from Camera    
    gdef::ExtractPlanes(m_FrustumPlanes, m_Camera.GetViewMatrix(), m_Camera.GetProjMatrix());
    gdef::FindAxisAlignedCorners(m_FrustumPlanes, m_arrCIP);

    //! Updates everything BUT CLOUDS! Clouds update themselves once determined visible after VFC.
    D3DXMATRIX mViewProj = (*m_Camera.GetViewMatrix()) * (*m_Camera.GetProjMatrix());
    m_pCloudSky->OnFrameMove(m_pd3dDevice, fElapsedTime, &mViewProj, m_Camera.GetEyePt());
    
    //! update the sun's light frustum (after cloudsky is updated).
    m_pShadowMap->SetShadowViewMatrix(m_pCloudSky->GetLightPos(), 
        (D3DXVECTOR3*)m_pCloudSky->GetLightDir(), 
        m_pCloudSky->GetLightUp(),
        m_pBubbles->GetBubbleCenter(0),
        m_pBubbles->GetBubbleRadius(0));    // HACK: Should always get largest bubble.
    
    m_pBubbles->OnFrameMove(fElapsedTime, m_Camera.GetEyePt());

    // Stuff visibility list by checking each island.
    m_vVisibilityList.clear();
    for(size_t i=0; i<m_vpIslands.size(); ++i)
    {
        m_vpIslands[i]->SetFrustumPlanesAndCorners(m_FrustumPlanes, m_arrCIP);
        m_vpIslands[i]->OnFrameMove(m_Camera.GetEyePt(),
            m_Camera.GetWorldUp(),
            m_Camera.GetWorldRight(),
            m_Camera.GetWorldAhead(),
            m_Camera.GetProjMatrix(), 
            m_Camera.GetViewMatrix(),                                         
            &m_ViewPort, 
            fTime, 
            fElapsedTime);

        m_vVisibilityList.insert(m_vVisibilityList.end(),
            m_vpIslands[i]->m_vVisibilityList.begin(), 
            m_vpIslands[i]->m_vVisibilityList.end());
    }

    //
    // Cull limbo clouds    
    //
    for(size_t i=0; i<m_vpLimboClouds.size(); ++i) 
    {   // TODO: figure out a way to skip limbo test if same cloud is deemed visible earlier in visibility list test.        
        UCHAR r = AABBFrustumTest(m_FrustumPlanes, m_arrCIP, m_vpLimboClouds[i]->GetCorners());
        if(r != AABB_OUTSIDE) {   
            m_vpLimboClouds[i]->SetVisible(true); 
            m_vpLimboClouds[i]->SetUpdated(false);
        }
        else
            m_vpLimboClouds[i]->SetVisible(false); 
    }

    UpdateClouds(fElapsedTime);	//! Update the clouds impostor etc.    
    UpdateEntities(fElapsedTime); //! Culls SMeshEntities (includes characters and obstacles) and updates Animations (if any).                
}

//! Rendering function
HRESULT CWorld::OnFrameRender( UINT param )
{
    HRESULT hr;

    if(m_nShadowUpdateCount > m_nShadowUpdateLimit)
    {
        UpdateShadowMap(); //! Update ShadowMap and ShadowMtx for use by Entities and Islands' Terrain.
        m_nShadowUpdateCount = 0;
    }
    else
        m_nShadowUpdateCount ++;

    if( SUCCEEDED( m_pPPEffects->BeginScene() ) )
    {
        // V_RETURN( m_pd3dDevice->SetRenderTarget( 1, m_pNotBloomMapSurface ) );
        V_RETURN( m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, (DWORD)m_BLOOMFLAGCOLOR, 1.0f, 0) );
        // V_RETURN( m_pd3dDevice->SetRenderTarget( 1, NULL ) );                

        // Draw Sky/Sun 
        m_pCloudSky->RenderSky(m_pd3dDevice);

        D3DXMATRIX shadowViewProjMtx;
        m_pShadowMap->GetShadowViewProjMatrix(&shadowViewProjMtx, m_Camera.GetProjMatrix());

        // Draw Islands.
        for(size_t i=0; i<m_vpIslands.size(); ++i)
        {
            m_vpIslands[i]->SetLightDir(m_pCloudSky->GetLightDir());
            m_vpIslands[i]->OnFrameRender( m_Camera.GetEyePt(),
                m_Camera.GetWorldUp(),
                m_Camera.GetWorldRight(),
                m_Camera.GetWorldAhead(),
                m_Camera.GetProjMatrix(),
                m_Camera.GetViewMatrix(),
                &shadowViewProjMtx,
                m_pShadowMap->GetShadowMap(),                                           
                param );
        }

        // Draw MeshEntities ( Players and Objects )
        RenderEntities(&shadowViewProjMtx);        

        // Draw Clouds and Bubbles
        RenderBubblesCloudsSprites(true);  //! Renders inside of a bubble, then cloud impostor atlas, then bubble outside.

#if 0 // Slow Not Bloom map way
        // -------------------------------------
        // Things to render twice to NOTBLOOMMAP.
        // -------------------------------------
        LPDIRECT3DSURFACE9 pOldRT;
        V_RETURN( m_pd3dDevice->GetRenderTarget(0, &pOldRT) );
        V_RETURN( m_pd3dDevice->SetRenderTarget(0, m_pNotBloomMapSurface) );// Render the clouds etc to the NOTBloomMap. (stores what not to bloom)

        RenderBubblesClouds(true);  // ABOVE CALL MUST BE FALSE IF USING NOTBLOOMMAP.

        V_RETURN( m_pd3dDevice->SetRenderTarget(0, pOldRT) );
        SAFE_RELEASE(pOldRT);
#endif // #if 0        

        if(m_bDrawBBs)
            DrawBoundingBoxes();

        //
        // END SCENE AND RENDER PP QUAD
        //
        m_vVisibleEntites.clear();
        V_RETURN( m_pPPEffects->EndScene() );

        float fFishEyeSpeed = max(m_CameraMovement.fWorldVelMag, m_CameraMovement.fRelVelMag);
        m_pPPEffects->processEffects(fFishEyeSpeed * FISHEYE_MULT, m_pCloudSky->GetTimeOfDay());
        //m_pPPEffects->SetUpKernel(m_mGaussianKernel);
        //m_pPPEffects->ExecuteKernel();

        m_pPPEffects->OnFrameRender();

        RenderStats();		
    }
    else
        return S_FALSE;
    m_bFirstRun = false;
    return hr;
}

HRESULT CWorld::OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{
    HRESULT hr;
    assert(pd3dDevice);

    m_pd3dDevice = pd3dDevice;

    m_pPolySprite->OnResetDevice();

    V_RETURN(m_pd3dDevice->GetViewport(&m_ViewPort) );

    for(size_t i=0; i<m_vpIslands.size(); ++i) {
        m_vpIslands[i]->OnResetDevice(pd3dDevice);
    }
    for(size_t i=0; i<m_vpCharacterModels.size(); ++i) {
        m_vpCharacterModels[i]->OnResetDevice(pd3dDevice, m_pBubbles->GetBubbleRadius(0)*2);
    }
    for(size_t i=0; i<m_vpStaticModels.size(); ++i) {
        m_vpStaticModels[i]->OnResetDevice(pd3dDevice, m_pBubbles->GetBubbleRadius(0)*2);
    }

    V_RETURN( m_pCloudSky->OnResetDevice(pd3dDevice) );
    V_RETURN( m_pShadowMap->OnResetDevice(pd3dDevice) );
    m_pBubbles->OnResetDevice(pd3dDevice);    

    const D3DSURFACE_DESC* pSurfDesc = DXUTGetBackBufferSurfaceDesc();

    V_RETURN( m_pPPEffects->OnResetDevice(pd3dDevice, pSurfDesc) );
    // setup gaussian kernel
    m_pPPEffects->SetUpKernel(m_mGaussianKernel);

    // Setup the camera's projection parameters
    float fAspectRatio = pSurfDesc->Width / (FLOAT)pSurfDesc->Height;
    m_Camera.SetProjParams( D3DX_PI/FOV_DIVISOR, fAspectRatio, 0.1f, FARPLANE_DISTANCE );  

    if( DXUTIsWindowed() ) {
        m_Camera.SetResetCursorAfterMove( false );            
    }
    else {
        m_Camera.SetResetCursorAfterMove( true );        
    }    

    // TODO: I have a feeling this is where the shit gets all screwy when I go full screen and then vsync off.    
    D3DXCreateTexture( pd3dDevice, pSurfDesc->Width, pSurfDesc->Height,
        1, D3DUSAGE_RENDERTARGET, pSurfDesc->Format,
        D3DPOOL_DEFAULT, &m_pNotBloomMap ); 
    V_RETURN( m_pNotBloomMap->GetSurfaceLevel( 0, &m_pNotBloomMapSurface ) );

    if( m_pFont )
        V( m_pFont->OnResetDevice() );
    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( pd3dDevice, &m_pTextSprite ) );

    return S_OK;
}

HRESULT CWorld::OnLostDevice()
{
    HRESULT hr;    

    m_pPolySprite->OnLostDevice();

    for(size_t i=0; i<m_vpIslands.size(); ++i) {
        m_vpIslands[i]->OnLostDevice();
    }
    for(size_t i=0; i<m_vpCharacterModels.size(); ++i) {
        m_vpCharacterModels[i]->OnLostDevice();
    }
    for(size_t i=0; i<m_vpStaticModels.size(); ++i) {
        m_vpStaticModels[i]->OnLostDevice();
    }

    V_RETURN( m_pCloudSky->OnLostDevice() );
    V_RETURN( m_pShadowMap->OnLostDevice() );

    m_pBubbles->OnLostDevice();

    V_RETURN( m_pPPEffects->OnLostDevice() );

    SAFE_RELEASE(m_pNotBloomMapSurface);
    SAFE_RELEASE(m_pNotBloomMap);

    // text rendering stuff
    if( m_pFont )
        m_pFont->OnLostDevice();
    SAFE_RELEASE(m_pTextSprite);

    return S_OK;
}


/////////////////////////////// PRIVATE FUNCTIONS ///////////////////////////

VOID CWorld::IncrementCameraVelocity(float *fVelInOut, float fDist, float fAccel, float fElapsedTime)
{
    *fVelInOut += fAccel * fElapsedTime * ( fDist / fElapsedTime - *fVelInOut );
}

VOID CWorld::FindCameraRelativeStep(D3DXVECTOR3 *pvOut, float fElapsedTime)
{
    assert(pvOut);

    D3DXVECTOR3 vDesiredRelativeDest = m_CameraMovement.vCameraOffset.z * *(D3DXVECTOR3*)&m_CameraMovement.mTarget._31;
    vDesiredRelativeDest += m_CameraMovement.vCameraOffset.y * *(D3DXVECTOR3*)&m_CameraMovement.mTarget._21;
    vDesiredRelativeDest += m_CameraMovement.vCameraOffset.x * *(D3DXVECTOR3*)&m_CameraMovement.mTarget._11;
    
    D3DXVECTOR3 vDiff = vDesiredRelativeDest - m_CameraMovement.vLastRelativePos;
    float fDist = D3DXVec3Length(&vDiff);
    IncrementCameraVelocity(&m_CameraMovement.fRelVelMag, fDist, m_CameraMovement.PositionSpring.fKCoef, fElapsedTime);
    
    D3DXVec3Normalize(&vDiff, &vDiff);
    *pvOut = vDiff * m_CameraMovement.fRelVelMag * fElapsedTime;

    m_CameraMovement.fRelVelMag *= SPRING_ENERGY_LOSS;
}

VOID CWorld::FindCameraWorldStep(D3DXVECTOR3 *pvOut, float fElapsedTime)
{
    assert(pvOut);
    D3DXVECTOR3 vLastTargetPos = m_CameraMovement.vLastWorldPos - m_CameraMovement.vLastRelativePos;
    D3DXVECTOR3 vDesiredTargetPos = *(D3DXVECTOR3*)&m_CameraMovement.mTarget._41;

    D3DXVECTOR3 vDiff = vDesiredTargetPos - vLastTargetPos;
    float fDist = D3DXVec3Length(&vDiff);
    IncrementCameraVelocity(&m_CameraMovement.fWorldVelMag, fDist, m_CameraMovement.PositionSpring.fKCoef, fElapsedTime);
    
    D3DXVec3Normalize(&vDiff, &vDiff);
    *pvOut = vDiff * m_CameraMovement.fWorldVelMag * fElapsedTime;

    m_CameraMovement.fWorldVelMag *= SPRING_ENERGY_LOSS;
}

VOID CWorld::FindCameraLookAtStep(D3DXVECTOR3 *pvOut, float fElapsedTime)
{
    assert(pvOut);
    D3DXVECTOR3 offsetLookAt;
    offsetLookAt.x = -D3DXVec3Dot(&m_CameraMovement.vCameraLookAtOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._11);
    offsetLookAt.y = D3DXVec3Dot(&m_CameraMovement.vCameraLookAtOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._21);
    offsetLookAt.z = D3DXVec3Dot(&m_CameraMovement.vCameraLookAtOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._31);
    offsetLookAt += *(D3DXVECTOR3*)&m_CameraMovement.mTarget._41;

    D3DXVECTOR3 vDiff = offsetLookAt - m_CameraMovement.vLastLookAt;
    float fDist = D3DXVec3Length(&vDiff);
    IncrementCameraVelocity(&m_CameraMovement.fLookAtVelMagnitude, fDist, m_CameraMovement.LookAtSpring.fKCoef, fElapsedTime);

    D3DXVec3Normalize(&vDiff, &vDiff);
    *pvOut = vDiff * m_CameraMovement.fLookAtVelMagnitude * fElapsedTime;

    m_CameraMovement.fLookAtVelMagnitude *= SPRING_ENERGY_LOSS;
}

VOID CWorld::UpdateCameraOrientation(const float fElapsedTime)
{
    if(fElapsedTime <= 0.f)
        return;
    if(m_bCameraControl)
        UpdateCameraRestState();

    else
    {
        // Camera movement broken into 2 parts. 
        // 1) Target - Camera Relative movement. 
        //          This handles the movement of the camera relative to player (zoom in out,rotate,etc)
        // 2) World - Camera Movement.
        //          This handles the movement of the camera that is set by vTargetVel in m_CameraMovement.
        
        D3DXVECTOR3 vRelativeStep, vWorldStep;

        FindCameraRelativeStep(&vRelativeStep, fElapsedTime);        
        FindCameraWorldStep(&vWorldStep, fElapsedTime);

        m_CameraMovement.vLastRelativePos += vRelativeStep;        
        m_CameraMovement.vLastWorldPos = vWorldStep + vRelativeStep + m_CameraMovement.vLastWorldPos;
        
        // dprintf("camera speed:%f\n", m_CameraMovement.fWorldVelMag);

        // Camera LookAt target movement        
        D3DXVECTOR3 vLookAtStep;
        FindCameraLookAtStep(&vLookAtStep, fElapsedTime);
        m_CameraMovement.vLastLookAt += vLookAtStep;

        m_Camera.SetViewParams(&m_CameraMovement.vLastWorldPos, &m_CameraMovement.vLastLookAt);

        //
        // CAMERA DEBUG
        //   
#if 0
        dprintf("CAMERA POS: %f %f %f \t\tCAMERA LOOKAT: %f %f %f\n", 
            m_CameraMovement.vLastWorldPos.x,
            m_CameraMovement.vLastWorldPos.y,
            m_CameraMovement.vLastWorldPos.z,
            m_CameraMovement.vLastLookAt.x,
            m_CameraMovement.vLastLookAt.y,
            m_CameraMovement.vLastLookAt.z);
#endif
    }
}

//! Updates m_CameraMovement to where the player last positioned the camera in camera position mode.
VOID CWorld::UpdateCameraRestState()
{
    // Calculate offsets
    // m_CameraMovement.vLastLookAt = *m_Camera.GetLookAtPt();
    m_CameraMovement.vLastWorldPos = *m_Camera.GetEyePt();
    // m_CameraMovement.vLastRelativePos = m_CameraMovement.vLastWorldPos - *(D3DXVECTOR3*)&m_CameraMovement.mTarget._41;        
  
    // calculate camera offset
    D3DXVECTOR3 vRotatedOffset = m_CameraMovement.vLastWorldPos - *(D3DXVECTOR3*)&m_CameraMovement.mTarget._41;    
    // Check mSpace index and offset so vCameraOffset is always as if behind idx 0 (POSX).
    // It will then get rotated later in UpdateCameraOrientation to the proper rotation.
    D3DXVECTOR3 vDesiredOffset;
	vDesiredOffset.x = D3DXVec3Dot(&vRotatedOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._11);
	vDesiredOffset.y = D3DXVec3Dot(&vRotatedOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._21);
	vDesiredOffset.z = D3DXVec3Dot(&vRotatedOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._31);	

    m_CameraMovement.vCameraOffset = vDesiredOffset;

#if 0   // the damn look at pt is pretty bogus in relation to the player ( could be a gazillion units deep down relative z axis, etc).
    // calculate camera look at offset.
    D3DXVECTOR3 vRotatedLookAtOffset = m_CameraMovement.vLastLookAt - *(D3DXVECTOR3*)&m_CameraMovement.mTarget._41;

    D3DXVECTOR3 vDesiredLookAtOffset;
	vDesiredLookAtOffset.x = D3DXVec3Dot(&vRotatedLookAtOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._11);
	vDesiredLookAtOffset.y = D3DXVec3Dot(&vRotatedLookAtOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._21);
	vDesiredLookAtOffset.z = D3DXVec3Dot(&vRotatedLookAtOffset, (D3DXVECTOR3*)&m_CameraMovement.mTarget._31);	

    m_CameraMovement.vCameraLookAtOffset = vDesiredLookAtOffset;
#endif  // #if 0
}


//! Finds next available int.
VOID CWorld::IncrementCharKey()
{   
    for(UINT i=1; i<gdef::MAX_UINT; i++)
    {   // i want to overflow
        int nNext = m_nCharacterKey+i;
        EntityHashMap::iterator it = m_hmEntities.find(nNext);
        if(it == m_hmEntities.end())
        {
            m_nCharacterKey = nNext;
            return;
        }
    }
    exit(EXIT_FAILURE); // crash if the entire 4 billion shits are taken.
}

/////////////////////////////// ENTITY FUNCTIONS ///////////////////////////

//! Updates the animation of the entities if any. For now this is only characters since these are the only ones with animations.
VOID CWorld::UpdateEntities(float fElapsedTime)
{
    EntityHashMap::iterator it = m_hmEntities.begin();
    for( ; it != m_hmEntities.end(); it++) 
    {   // Go through an update Character animations and Cull characters outside of view Frustum.
        it->second.pModelInst->OnFrameMove(fElapsedTime, &it->second.mWorld);
        if(it->second.bRequestVisible && CullEntity(&it->second)) {   // visible
            m_vVisibleEntites.push_back(&it->second);
        }
    }
    // Sort by geomID to sort of batch render because of redundant Set* Calls.
    std::sort(m_vVisibleEntites.begin(), m_vVisibleEntites.end(), SMeshEntityPtrFunctor());
}

//! Checks entity AABB against frustum and sets bVisible/returns true if visible. Populates m_vVisibleEntites.
bool CWorld::CullEntity(SMeshEntity* pent)
{
    if(gdef::AABBFrustumTest(m_FrustumPlanes, m_arrCIP, pent->corners) == AABB_OUTSIDE)
    {   
        pent->bVisible = false;
        return false;
    }
    pent->bVisible = true;
    return true;
}

//! Renders the Characters and Static Meshes(Obstacle) Entities.
HRESULT CWorld::RenderEntities(const D3DXMATRIX* pShadowViewProjMtx)
{
    // Update ShadowMaps in Geoms
    for(size_t i=0; i<m_vpCharacterModels.size(); i++)
        m_vpCharacterModels[i]->SetShadowMap( m_pd3dDevice, pShadowViewProjMtx, m_pShadowMap->GetShadowMap() );
    for(size_t i=0; i<m_vpStaticModels.size(); i++)
        m_vpStaticModels[i]->SetShadowMap( m_pd3dDevice, pShadowViewProjMtx, m_pShadowMap->GetShadowMap() );

    for(size_t i=0; i<m_vVisibleEntites.size(); i++)
    {   //! Already tested for visiblity earlier which is why in this list.
        m_vVisibleEntites[i]->pModelInst->OnFrameRender(m_pd3dDevice, 
            m_Camera.GetProjMatrix(),
            m_Camera.GetViewMatrix(),
            &m_vVisibleEntites[i]->mWorld,
            m_Camera.GetEyePt(),
            m_pCloudSky->GetLightDir(),
            m_vVisibleEntites[i]->pTex);
    }    
    return S_OK;
}

////////////////////////////// SKY/CLOUD FUNCTIONS //////////////////////

VOID CWorld::UpdateClouds(float fElapsedTime)
{
    // -------------------------------------------
    // UPDATE RENDERABLE CLOUDS BY CHECKING VIEW FRUSTUM
    // -------------------------------------------

    // Update clouds in limbo and in visibility list.
    for(size_t i=0; i<m_pCloudSky->GetNumCloudBatches(); i++)
    {   // Check for necessary updates to cloud impostors and populate priority queue.
        const vector<CCloud*>& vpClouds = m_pCloudSky->GetCloudBatch((int)i)->GetClouds();
        for(size_t j=0, jend=vpClouds.size(); j < jend; ++j) 
        {   // Update Clouds            
            CCloud *pcloud = vpClouds[j];
            if(pcloud->IsVisible() && !pcloud->IsUpdated())     
            {   // determined by CullVisibleNodes from islands and Cull Limbo clouds
                // Update Cloud properties
                pcloud->OnFrameMove(m_pCloudSky->GetLightPos(), (const D3DXVECTOR3*)m_pCloudSky->GetLightDir(), 
                    m_pCloudSky->GetTimeOfDay(), m_pCloudSky->GetSunColor(0), fElapsedTime);

                // Update Cloud Impostor FIFO/Queue for later cloud impostor updates
                bool bNotFound = true;
                deque<CCloud*>::iterator it = m_vpCloudImpostorQueue.begin();                
                for( ; it!=m_vpCloudImpostorQueue.end(); ++it) 
                {   // go through n entries and make sure not already in queue, then put at end.
                    if( (*it) == pcloud ) {
                        bNotFound = false;
                        break;
                    }
                }
                if(bNotFound)
                    m_vpCloudImpostorQueue.push_back(pcloud);
                pcloud->SetUpdated(true);
            }
        }
    }

    // 
    // Render IMPOSTOR_UPDATES_PER_FRAME number of cloud impostors per frame
    //
    for( int count = 0; !m_vpCloudImpostorQueue.empty() && count < CLOUD_IMPOSTOR_UPDATES_PER_FRAME; )
    {   // check for IMPOSTOR_UPDATES_PER_FRAME OnFrameMove true returns.
        deque<CCloud*>::reference ref = m_vpCloudImpostorQueue.front();
        if( !ref->IsVisible() ) 
            ;   // Make sure it's still visible by the time we try to update impostor, otherwise run over it.
        else if( ref->UpdateImpostor(m_pd3dDevice, 
            m_Camera.GetProjMatrix(), 
            m_Camera.GetViewMatrix(), 
            &m_mIdentity, 
            &m_ViewPort, 
            m_Camera.GetEyePt(), 
            m_Camera.GetWorldUp(), 
            m_Camera.GetWorldRight(), 
            m_Camera.GetWorldAhead(),
            m_pCloudSky->GetCloudTex()) )
        {
            ++count;
        }        
        m_vpCloudImpostorQueue.pop_front();        
    }

    // -------------------------------------------
    // UPDATE SHADOW CLOUDS BY CHECKING SUN FRUSTUM. (For now function assumes all clouds are visible to sun).
    // -------------------------------------------
    // m_pCloudSky->UpdateShadowClouds(m_pd3dDevice, m_Camera.GetProjMatrix(), m_pShadowMap->GetShadowViewMatrix(), &m_ViewPort);
    // HAVE PRIORITY QUEUE IN UpdateShadowClouds and SORT THEM CAUSE THE THINGS OVERLAP IN Z TEST.
}

VOID CWorld::RenderBubblesCloudsSprites( bool bClearFlagged )
{
    HRESULT hr;

    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, m_Camera.GetProjMatrix() );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, m_Camera.GetViewMatrix() );  
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_mIdentity );  

    // Draw clouds and bubbles.    
    D3DXMATRIX mViewProj = (*m_Camera.GetViewMatrix()) * (*m_Camera.GetProjMatrix());
    m_pCloudSky->SplitClouds();

    int nBubbles = (int)m_pBubbles->GetNumBubbles();

    // Draw bubbles back to front from camera location.
    for(int i=0; i<nBubbles; ++i)
    {
        m_pBubbles->Draw(m_pd3dDevice, m_pCloudSky->GetLightPos(), m_pCloudSky->GetLightDir(), m_Camera.GetViewMatrix(), &mViewProj, DRAWINSIDE, i);                

        // m_pCloudSky->RenderClouds(m_pd3dDevice, m_pPolySprite, m_Camera.GetEyePt(), m_Camera.GetWorldAhead(), ONLYIMPOSTORS);

        m_pPolySprite->SetCameraSpace(m_Camera.GetEyePt(), m_Camera.GetWorldUp(), m_Camera.GetWorldRight(), m_Camera.GetWorldAhead());
        m_pPolySprite->Begin( CPolySprite::SCREENALIGN_BILLBOARD | CPolySprite::BACKTOFRONT );

        // cut out half transparent pixels to speed up rendering
        V( m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)CLOUD_ALPHA_CUTOFF));        
        V( m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE) );

        // Draw Clouds. Note the current ZWRITE=false hack.
        m_pCloudSky->RenderClouds(m_pd3dDevice, m_pPolySprite, m_Camera.GetEyePt(), m_Camera.GetWorldAhead(), 0);

        // Draw Sprite Animations
        DrawSpriteAnimations(m_pPolySprite);

        m_pPolySprite->End();        

        // Draw Bubble around Island LAST because it uses alpha heavily and you have to see through it.    
        m_pBubbles->Draw(m_pd3dDevice, m_pCloudSky->GetLightPos(), m_pCloudSky->GetLightDir(), m_Camera.GetViewMatrix(), &mViewProj, DRAWOUTSIDE, i);
    }
    V( m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW) );

    assert(nBubbles);
    if(!nBubbles)
    {   // Draw Clouds even if there are no bubbles
        exit(EXIT_FAILURE);        
    }

    if(bClearFlagged)
        m_pCloudSky->ClearAtlasFlags();
}

/////////////////////////////// SHADOW MAP FUNCTIONS //////////////////////////

HRESULT CWorld::UpdateShadowMap()
{
    using std::find;
    HRESULT hr;        

    V_RETURN ( m_pShadowMap->StartShadowMapRender(m_pd3dDevice) );

    // Get view planes of light frustum.
    D3DXPLANE planes[6];
    gdef::CornerIndexPair cip[6];
    gdef::ExtractPlanes(planes, m_pShadowMap->GetShadowViewMatrix(), m_Camera.GetProjMatrix());
    gdef::FindAxisAlignedCorners(planes, cip);

    // Render Entities.
    EntityHashMap::iterator it = m_hmEntities.begin();
    for( ; it != m_hmEntities.end(); it++)    
    {   // test every entity against shadow(sun) view frustum
        if(it->second.bRequestVisible && gdef::AABBFrustumTest(planes, cip, it->second.corners) != AABB_OUTSIDE)
        {   // if in in frustum, render to shadow map            
            if(find(m_vCharIDs.begin(), m_vCharIDs.end(), it->first) != m_vCharIDs.end())
            {   // if a character double the scale to combat aliasing artifacts.
                D3DXMATRIX mDblScale;
                D3DXMatrixScaling(&mDblScale, 4.f, 4.f, 4.f);
                mDblScale *= it->second.mWorld;

                it->second.pModelInst->RenderShadowMap( m_pd3dDevice,
                    m_Camera.GetProjMatrix(),
                    m_pShadowMap->GetShadowViewMatrix(),
                    &mDblScale,
                    m_pCloudSky->GetLightPos(),
                    m_pCloudSky->GetLightDir() );
            }
            else
            {   // else render normal
                it->second.pModelInst->RenderShadowMap( m_pd3dDevice,
                    m_Camera.GetProjMatrix(),
                    m_pShadowMap->GetShadowViewMatrix(),
                    &it->second.mWorld,
                    m_pCloudSky->GetLightPos(),
                    m_pCloudSky->GetLightDir() );
            }
        }
    }
#if 0
    // Render cloud impostors from light direction. cloud vertex coordinates are in world space.
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ) );   // cull FRONT faces so if anything is <= the back face its shadow.
    m_pCloudSky->RenderCloudsToShadowMap(m_pd3dDevice, 
        m_Camera.GetEyePt(),
        m_Camera.GetWorldAhead(),
        m_Camera.GetProjMatrix(), 
        m_pShadowMap->GetShadowViewMatrix());
#endif

    V_RETURN ( m_pShadowMap->EndShadowMapRender(m_pd3dDevice) );    

    if(m_bSaveShadowMap) {
        V_RETURN( D3DXSaveTextureToFileA("ShadowMap.bmp", D3DXIFF_BMP, m_pShadowMap->GetShadowMap(), NULL) );
        m_bSaveShadowMap = false;
    }
    return S_OK;
}

/////////////////////////////// CAMERA FUNCTIONS //////////////////////////

VOID CWorld::SetCameraOrientationTarget(const D3DXMATRIX* pOrientationTargetMtx)
{
    if(!pOrientationTargetMtx)
        return;
    m_CameraMovement.mTarget = *pOrientationTargetMtx;
    // m_CameraMovement.vLastRelativePos = m_CameraMovement.vLastWorldPos - *(D3DXVECTOR3*)&pOrientationTargetMtx->_41;
}

VOID CWorld::SetCameraVariables(float fKCoef, float fLookKCoef)                                
{
    m_CameraMovement.PositionSpring.fInertialDist = 0;    
    m_CameraMovement.PositionSpring.fKCoef = fKCoef;
    m_CameraMovement.LookAtSpring.fInertialDist = 0;    
    m_CameraMovement.LookAtSpring.fKCoef = fLookKCoef;
}

VOID CWorld::SetCameraOffset(const D3DXVECTOR3 *pvCameraPosOffset, const D3DXVECTOR3 *pvCameraLookAtOffset)
{
    if(pvCameraPosOffset)
        m_CameraMovement.vCameraOffset = *pvCameraPosOffset;
    if(pvCameraLookAtOffset)
	    m_CameraMovement.vCameraLookAtOffset = *pvCameraLookAtOffset;
}

VOID CWorld::SetCameraViewParams(D3DXVECTOR3* pEye, D3DXVECTOR3* pLookAt)
{
    // m_CameraMovement.vLastLookAt = *pLookAt;
    m_CameraMovement.vLastWorldPos = *pEye;
    m_Camera.SetViewParams(pEye, pLookAt);    
}

///////////////////////////// SPRITE ANIMATION FUNCTIONS  /////////////////////////

VOID CWorld::DrawSpriteAnimations(CPolySprite* pPolySprite)
{
    RunningSpriteAnmHashMap::iterator it = m_hmRunningSpriteAnims.begin();    
    for( ; it != m_hmRunningSpriteAnims.end(); it++)
    {
        SRunningSpriteAnimation &curRunning = it->second;
        m_pSpriteAnimMgr->GetAnimStill(curRunning.runningAnimIdx, &curRunning.pTex, curRunning.rectf);
        pPolySprite->Draw(curRunning.pTex, &curRunning.rectf, &curRunning.mWorld);
    }    
}

VOID CWorld::UpdateSpriteAnimations(float fElapsedTime)
{
    RunningSpriteAnmHashMap::iterator it = m_hmRunningSpriteAnims.begin();
    while(it != m_hmRunningSpriteAnims.end())
    {
        it->second.ttl -= fElapsedTime;
        if(it->second.ttl < 0) 
        {
            m_pSpriteAnimMgr->StopAnim(it->second.runningAnimIdx);
            it = m_hmRunningSpriteAnims.erase(it);
        }
        else
            it++;
    }
    m_pSpriteAnimMgr->OnFrameMove(fElapsedTime);
}

//! Adds a sprite animation to the spriteAnimMgr for shortlife entities.
UINT CWorld::AddSpriteAnimation(UCHAR spriteAnimTableID, const D3DXMATRIX* pWorld, float ttl)
{
    SRunningSpriteAnimation runningAnim;
    runningAnim.runningAnimIdx = m_pSpriteAnimMgr->StartAnim(m_hmAnmEntries[spriteAnimTableID].idxAnim);
    runningAnim.mWorld = *pWorld;
    runningAnim.ttl = ttl;

    m_hmRunningSpriteAnims[runningAnim.runningAnimIdx] = runningAnim;
    return runningAnim.runningAnimIdx;
}

VOID CWorld::UpdateSpriteAnimation(int runningAnimID, const D3DXMATRIX* pWorld, float fTTL)
{
    m_hmRunningSpriteAnims[runningAnimID].mWorld = *pWorld;
	m_hmRunningSpriteAnims[runningAnimID].ttl = fTTL;
}

/////////////////////////////// MISCELLANY FUNCTIONS //////////////////////////

//! Render Engine Test Commands.
VOID CWorld::KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown )
{	
    // TODO: make sure we take out this pillar crap before release
    // jump into pillar interact mode
    if(bKeyDown && nChar == 'Z')
    {   m_bEditPillar = !m_bEditPillar;
        m_pPPEffects->ToggleEnhance();
    }

    if(!m_bEditPillar)
    {
        if(bKeyDown && bAltDown && nChar == 'B') {
            ToggleDrawBoundingBoxes();
        }
        if(bKeyDown && bAltDown && nChar == 'S') {
            SunTest(1);
        }
        if(bKeyDown && bAltDown && nChar == 'D') {
            SunStop();
        }
        if(bKeyDown && bAltDown && nChar == 'N') {
            for(size_t i=0; i<m_vCharIDs.size(); i++) {
                m_hmEntities[m_vCharIDs[i]].pModelInst->SelectNextAnimationSet();
            }
        }
        if(bKeyDown && bAltDown && nChar == 'H') {
            m_bSaveShadowMap = true;
        }
    }
    else
        m_vpIslands[0]->GetPillarPointer()->HandleInput(nChar, bAltDown);

}

VOID CWorld::CycleCharactersAnimation() 
{	
    for(size_t i=0; i<m_vCharIDs.size(); i++)
    {
        m_hmEntities[m_vCharIDs[i]].pModelInst->SelectNextAnimationSet();
    }
}

//! Renders screen stats
void CWorld::RenderStats() 
{
    CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );
    // const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetBackBufferSurfaceDesc();

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );    
    txtHelper.DrawTextLine( DXUTGetFrameStats(true) );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );

    txtHelper.End();
}

VOID CWorld::SunTest(int idx)
{
    m_pCloudSky->PushBackSunState(SunStates::BlackMorph, idx);
    m_pCloudSky->PushBackSunState(SunStates::BlackSkyMorph, idx);
    m_pCloudSky->PushBackSunState(SunStates::BlueMorph, idx);
    m_pCloudSky->PushBackSunState(SunStates::WhiteMorph, idx);
    m_pCloudSky->PushBackSunState(SunStates::WhiteSkyMorph, idx);
    m_pCloudSky->PushBackSunState(SunStates::DefaultSun, idx);
}

VOID CWorld::SunStop()
{
    m_pCloudSky->ToggleDayMovement();
}

//! places in a limbo vector what doesn't fall into the terrain quadtrees (islands)
VOID CWorld::PopulateLimboVectors()
{
    vector<CQuadTree::QTNode*> vTopNodes;
    for(size_t i=0; i<m_vpIslands.size(); ++i) {        
        vTopNodes.push_back(m_vpIslands[i]->m_root);    //! Used to check if object / cloud is in limbo.
    }

    // populate limbo clouds 
    for(size_t i=0; i<m_pCloudSky->GetNumCloudBatches(); ++i)
    {
        const CCloudBatch* pcb = m_pCloudSky->GetCloudBatch((int) i);
        const vector<CCloud*>& vc = pcb->GetClouds();
        for(size_t j=0; j<vc.size(); ++j)
        {
            D3DXVECTOR3 displace(vc[j]->GetCloudInfo().widthhalf, vc[j]->GetCloudInfo().heighthalf, vc[j]->GetCloudInfo().depthhalf);
            if(IsAABBLimbo(vTopNodes, &vc[j]->GetCloudInfo().pos, &displace) )
                m_vpLimboClouds.push_back(vc[j]);
        }
    }
}

bool CWorld::IsAABBLimbo(const vector<QTNode*>& vpTopNodes, const D3DXVECTOR3 *center, const D3DXVECTOR3 *displace)
{
    for(size_t i=0; i<vpTopNodes.size(); i++)
    {
        QTNode* n = vpTopNodes[i];

        float halflength = (float)n->length / 2.f;
        float halfheight = (float)(n->maxy-n->miny) / 2.f;

        // you need the passed center/displace to be ENTIRELY inside one of the nodes
        if(gdef::IsCubeInsideCube(*center, *displace, 
            D3DXVECTOR3((float)n->midx, (float)n->midy, (float)n->midz), // node center
            D3DXVECTOR3(halflength, halfheight, halflength)) )   // node lengths
        {
            return false;

        }
    }
    return true;
}

HRESULT CWorld::DrawBoundingBoxes()
{
    HRESULT hr;
    ULONG OldStates[6];
    D3DXMATRIX matIdentity;

    V_RETURN( m_pd3dDevice->SetTransform( D3DTS_PROJECTION, m_Camera.GetProjMatrix() ) );
    V_RETURN( m_pd3dDevice->SetTransform( D3DTS_VIEW, m_Camera.GetViewMatrix() ) );  
    V_RETURN( m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_mIdentity ) );  

    D3DXMatrixIdentity(&matIdentity);    

    // Retrieve old states
    V_RETURN( m_pd3dDevice->GetRenderState( D3DRS_ZWRITEENABLE, &OldStates[0] ) );
    V_RETURN( m_pd3dDevice->GetRenderState( D3DRS_ZENABLE, &OldStates[1] ) );
    V_RETURN( m_pd3dDevice->GetRenderState( D3DRS_LIGHTING, &OldStates[2] ) );
    V_RETURN( m_pd3dDevice->GetRenderState( D3DRS_COLORVERTEX, &OldStates[3] ) );
    V_RETURN( m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &OldStates[4] ) );
    V_RETURN( m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &OldStates[5] ) );

    // Setup new states
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE ) ); 
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE ) );
    V_RETURN( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE ) );
    V_RETURN( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 ) );

    V_RETURN( m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) );    

    // draw the stuff //
    for(size_t i=0; i<m_vpIslands.size(); i++)
    {
        m_vpIslands[i]->DrawQuadBBs(D3DCOLOR_XRGB(0,0,255)); // blue
        m_vpIslands[i]->DrawCloudBBs(0xffff0000); // red
    }
    DrawLimboCloudBBs(0xffff0000); // green
    DrawEntityBBs(0xff00ff00); // purple?

    // Reset old states
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, OldStates[0] ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, OldStates[1] ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, OldStates[2] ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, OldStates[3] ) );
    V_RETURN( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, OldStates[4] ) );
    V_RETURN( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, OldStates[5] ) );

    V_RETURN( m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) );

    // Reset the matrix
    V_RETURN( m_pd3dDevice->SetTransform( D3DTS_WORLD, &matIdentity ) );
    return EXIT_SUCCESS;
}

HRESULT CWorld::DrawLimboCloudBBs(ULONG color)
{
    if(m_vpLimboClouds.size() <=0 )     return S_OK;
    HRESULT hr;
    int numLines = 12;
    BoxVertColor *verts = new BoxVertColor[(int)m_vpLimboClouds.size() * numLines * 2]; // nClouds with numLines per BB with 2 vertices per line.
    for(size_t i=0; i<m_vpLimboClouds.size(); i++)
    {
        int index = (int)i*numLines*2;
        gdef::GenerateBoundingBox(&verts[index], m_vpLimboClouds[i]->GetCorners());        
        for(int j=0;j<numLines*2;j++)
            verts[index+j].Color = color;
    }    
    // Draw
    V_RETURN( m_pd3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE ) );
    V_RETURN( m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)(m_vpLimboClouds.size()*numLines), verts, sizeof(BoxVertColor)) );

    SAFE_DELETE_ARRAY(verts);
    return S_OK;
}

HRESULT CWorld::DrawEntityBBs(ULONG color)
{
    if(m_vVisibleEntites.size() <= 0)   return S_OK;
    HRESULT hr;
    int numLines = 12;
    BoxVertColor *verts = new BoxVertColor[(int)m_vVisibleEntites.size() * numLines * 2]; // nClouds with numLines per BB with 2 vertices per line.
    for(size_t i=0; i<m_vVisibleEntites.size(); i++)
    {
        int index = (int)i*numLines*2;
        gdef::GenerateBoundingBox(&verts[index], m_vVisibleEntites[i]->corners);
        for(int j=0;j<numLines*2;j++)
            verts[index+j].Color = color;
    }
    // Draw
    V_RETURN( m_pd3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE ) );
    V_RETURN( m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)(m_vVisibleEntites.size()*numLines), verts, sizeof(BoxVertColor)) );

    SAFE_DELETE_ARRAY(verts);
    return S_OK;
}

//! returns true if the point is ABOVE an island.
bool CWorld::IsPointOverIsland(const D3DXVECTOR3* pvec)
{
    for(size_t i=0; i<m_vpIslands.size(); i++)
    {
        D3DXVECTOR3 translation(*m_vpIslands[i]->GetTranslation());
        translation = *pvec - translation;
        if(translation.y >= 0.f && translation.x >= 0.f && translation.z >= 0.f)
        {
            int nWidth = m_vpIslands[i]->GetTerrain()->GetWidth() * m_vpIslands[i]->GetTerrain()->GetCellSpacing();
            int nLength = m_vpIslands[i]->GetTerrain()->GetHeight() * m_vpIslands[i]->GetTerrain()->GetCellSpacing();
            if(translation.x < nWidth && translation.z < nLength)
                return true;
        }
    }
    return false;
}

// returns number of different char meshes
int CWorld::GetNumCharacterGeoms()                  
{ 
    return (int)m_vpCharacterModels.size(); 
}  

float CWorld::GetCharacterAnimationPeriod(int geomID, UCHAR animIdx)  
{ 
    AnimationKeyIndexHashMap &animHM = m_vhmAnimationKeyIndex[geomID];
    return (float) m_vpCharacterModels[geomID]->GetAnimationPeriod(animHM[animIdx]);
}

