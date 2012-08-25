#include "dxstdafx.h"
#include "QuadTree.h"
#include ".\cloudsky.h"
#include "gamedefine.h"
#include "polysprite.h"
#include "CloudBatch.h"
#include "impostoratlas.h"

using std::ifstream;
using std::ofstream;
using std::endl;

using std::multimap;
using std::vector;
using std::make_pair;

#define                 IMPOSTORATLAS_DIMENSION         1024    // went to 2048 instead of 1024, screw it lets gobble 16mb of vram.
#define                 IMPOSTOR_DIMENSION              128     // 64 impostor slots available ( need 2 per cloud for swap)

#define                 IMPOSTORATLASSHADOW_DIMENSION   1024    // no longer used. Maybe I can now that I sort.
#define                 IMPOSTORSHADOW_DIMENSION        128     // 64 slots


#define                 CLOUD_SAVEFILE                  "../Media/cloudmap-SAVE.cmp"
#define                 CLOUD_LOADFILE                  "../Media/cloudmap-LOAD.cmp"

#define                 NOTHING_TEX                     L"../Media/nothing.bmp"

//
// NOTE: There are 256 Impostor slots available.
//


//! Loads Cloud Map. Cloud maps holds cloud positions and texture fn for cloudbatch
CCloudSky::CCloudSky(LPCSTR pszCloudMap, LPCSTR pszCloudAtlasTex, LPCSTR pszSkyToneTex, const D3DXVECTOR3 *pvCenter, float fSkyRadius) : 
    m_strCloudMap(pszCloudMap), m_strCloudAtlasTex(pszCloudAtlasTex), m_strSkyToneTex(pszSkyToneTex),
    m_pCloudAtlas(NULL), m_pImpostorAtlas(NULL),
    m_pSky(NULL), m_pPolySprite(NULL), m_pPolySpriteShadow(NULL)
{
	// Loads the Sky based on the cell/height spacing of the Terrain. Also determines center.
    m_pSky = new CSky(pvCenter, fSkyRadius);

    //! Creates a new Impostor Atlas of the dimensions to hold all cloud impostors. D3D_DEFAULT stuff created in OnResetDevice.
    m_pImpostorAtlas = new CImpostorAtlas(IMPOSTORATLAS_DIMENSION, 
                                          IMPOSTORATLAS_DIMENSION, 
                                          IMPOSTOR_DIMENSION, 
                                          IMPOSTOR_DIMENSION);       

    m_pImpostorAtlasShadow = new CImpostorAtlas(IMPOSTORATLASSHADOW_DIMENSION, 
                                                IMPOSTORATLASSHADOW_DIMENSION, 
                                                IMPOSTORSHADOW_DIMENSION, 
                                                IMPOSTORSHADOW_DIMENSION);
}

CCloudSky::~CCloudSky(void)
{
    SAFE_DELETE(m_pPolySprite);
    SAFE_DELETE(m_pPolySpriteShadow);

    for(size_t i=0;i<m_vpCloudBatches.size(); i++) 
    {
        SAFE_DELETE(m_vpCloudBatchesShadow[i]);
        SAFE_DELETE(m_vpCloudBatches[i]);
    }

    SAFE_DELETE(m_pSky);    
    SAFE_RELEASE(m_pCloudAtlas);
    SAFE_DELETE(m_pImpostorAtlas);
    SAFE_DELETE(m_pImpostorAtlasShadow);
}

VOID CCloudSky::OnCreateDevice(IDirect3DDevice9* pd3dDevice)
{
    HRESULT hr; 

    m_pPolySprite = new CPolySprite(pd3dDevice);
    m_pPolySpriteShadow = new CPolySprite(pd3dDevice);

    m_pImpostorAtlas->OnCreateDevice(pd3dDevice);
    m_pImpostorAtlasShadow->OnCreateDevice(pd3dDevice);

    // Load Cloud File
    LoadClouds(m_strCloudMap.c_str());

    m_pSky->OnCreateDevice(pd3dDevice, m_strSkyToneTex.c_str());
    V( D3DXCreateTextureFromFileA(pd3dDevice, m_strCloudAtlasTex.c_str(), &m_pCloudAtlas) );

    // Add Sun, Stars, etc.
    m_pSky->AddBigStar(pd3dDevice, NOTHING_TEX, 1.0f, 0.0f, 0.0f); // 0 this is the actual sun!! don't remove
    // m_pSky->AddBigStar(pd3dDevice, MOON_TEX, 1.0f, 0.0f, 0.5f);	
	// m_pSky->AddBigStar(pd3dDevice, STAR_TEX, 0.1f, 1.0f, 0.25f);
	// m_pSky->AddBigStar(pd3dDevice, GALAXY_TEX, 0.0f, 0.3f, 0.3f);
}

//! Places clouds in quadtree. Shadow clouds are never in a quadtree but remain in limbo.
VOID CCloudSky::PopulateLeafNodesWithClouds(CQuadTree::QTNode* proot)
{	
    // populate clouds
    vector<CQuadTree::QTNode*> vNodes;

    for(size_t i =0; i<m_vpCloudBatches.size(); i++)
    {
        const vector<CCloud*> &vpClouds = m_vpCloudBatches[i]->GetClouds();

        for(size_t k =0; k<vpClouds.size(); k++)
        {
            CCloud* pc = vpClouds[k];
            const SCloudInfo &ci = pc->GetCloudInfo();

            vNodes.clear();
            CQuadTree::QTNode::GetLowestNodes(vNodes, 
                                                proot, 
                                                ci.pos,
                                                ci.widthhalf+ci.puffScale, 
                                                ci.heighthalf+ci.puffScale,
                                                ci.depthhalf+ci.puffScale);

            for(size_t j=0;j<vNodes.size(); j++)
            {   // Populate Nodes with the clouds they contain.
                vNodes[j]->vpClouds.push_back(pc);
            }
        }
    }
}


VOID CCloudSky::OnFrameMove(IDirect3DDevice9* pd3dDevice, float fElapsedTime, const D3DXMATRIX* pmViewProj, const D3DXVECTOR3* pEyePt)
{
    m_pSky->OnFrameMove(fElapsedTime, pmViewProj); 

    m_pImpostorAtlas->ClearInvalidEntries(pd3dDevice);
    m_pImpostorAtlasShadow->ClearInvalidEntries(pd3dDevice);

    // reset the ccloud visibility flag.
    for(size_t i=0; i<m_vpCloudBatches.size(); ++i)
    {
        const vector<CCloud*>& vpClouds = m_vpCloudBatches[i]->GetClouds();
        for(size_t j=0; j<vpClouds.size(); ++j)
        {
            vpClouds[j]->SetVisible(false);
        }
    }

    // Lighting hack. Lighting resets back to sunrise after passing sunset. 
    // So lighting's never really under islands and nights of 'edge' get their own cycle.
    // TODO:    
}

UINT CCloudSky::RenderSky(IDirect3DDevice9* pd3dDevice, bool bZTest)
{
	HRESULT hr;

    if(!bZTest)
    { // Turn off depth testing
	    V( pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE) );
	    V( pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE) );
    }
    
    // Render Sky
    V( m_pSky->OnFrameRender(pd3dDevice) );

    if(!bZTest)
    {
        V( pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE) );
        V( pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE) );
    }

	return EXIT_SUCCESS;
}

VOID CCloudSky::ClearAtlasFlags()
{
    m_pImpostorAtlas->ClearFlags();
    m_pImpostorAtlasShadow->ClearFlags();
}

// Splits renderable clouds (as opposed to shadow clouds).
VOID CCloudSky::SplitClouds()
{
    //
    // Split Clouds to draw between Impostors and Actual Cloud Sprites
    //
    m_setActualClouds.clear();
    for(size_t iCB = 0; iCB< m_vpCloudBatches.size(); iCB++)
    {
        const vector<CCloud*>& vpClouds = m_vpCloudBatches[iCB]->GetClouds();
        for(size_t i=0;i<vpClouds.size(); ++i)
        {   // Separate Clouds
            CCloud* pc = vpClouds[i];
            if(pc->IsVisible() && pc->IsUsingImpostor()) {  // flag to be drawn with impostor atlas
                m_pImpostorAtlas->FlagToDraw(pc->GetAtlasId());
            }
            else if(pc->IsVisible() /* && !pc->IsUsingImpostor()*/)
            {   // send cloud to actual cloud vector            
                m_setActualClouds.insert(pc);                       
            }
        }
    }
}

// Renders renderable clouds (as opposed to shadow clouds).
UINT CCloudSky::RenderClouds(IDirect3DDevice9* pd3dDevice, 
                             const D3DXVECTOR3* pEyePt, 
                             const D3DXVECTOR3* pUp,
                             const D3DXVECTOR3* pRight,
                             const D3DXVECTOR3* pAhead,
                             UINT flag)
{
    HRESULT hr;       

    // Testing out ZWRITE with impostors and actual clouds
    V( pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE) );

    if(!(flag & ONLYACTUALCLOUDS) )
    {
        //
        // Draw UNSORTED IMPOSTOR Clouds Before Actual Clouds. All our stuff is actually unsorted due to alpha adding magic.
        //
        // cut out half transparent pixels to speed up rendering
        V( pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)CLOUD_ALPHA_CUTOFF));        
        m_pImpostorAtlas->RenderAtlas(pd3dDevice);
    }

    if(!m_setActualClouds.empty() && !(flag & ONLYIMPOSTORS) )
    {

        //
        // Draw Actual Clouds VIA SPRITE
        //

        // I do SCREENALIGN so that the player can go THROUGH the clouds 
        // as opposed to the parting of the red sea bug with VIEWPOINT.
        // DWORD flag = CPolySprite::BACKTOFRONT | CPolySprite::SCREENALIGN_BILLBOARD;
        DWORD flag = CPolySprite::SCREENALIGN_BILLBOARD;  // Don't need to sort with the magic of alphas and zwrite

        m_pPolySprite->SetCameraSpace( pEyePt, pUp, pRight, pAhead);
        m_pPolySprite->Begin( flag );     
        
        set<CCloud*>::iterator it = m_setActualClouds.begin();
        for( ; it != m_setActualClouds.end(); it++)
        {   // Draw ACTUAL Clouds                
            CCloud* pc = *it;
            pc->RenderThruSprite(m_pPolySprite, m_pCloudAtlas);
        } 
        m_pPolySprite->End(); // Draw all ACTUAL CLOUDS
    }
    V( pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE) );
    
    return EXIT_SUCCESS;
}

// Renders renderable clouds (as opposed to shadow clouds).
UINT CCloudSky::RenderClouds(IDirect3DDevice9* pd3dDevice, 
                             CPolySprite* pPolySprite, 
                             const D3DXVECTOR3* pEyePt,
                             const D3DXVECTOR3* pAhead,
                             UINT flag)
{   
    if(!(flag & ONLYACTUALCLOUDS) )
        m_pImpostorAtlas->RenderAtlasToSprite(pPolySprite);

    // Testing out ZWRITE with impostors and actual clouds    
    if(!m_setActualClouds.empty() && !(flag & ONLYIMPOSTORS) )
    {
        //
        // Draw Actual Clouds VIA SPRITE
        //
        set<CCloud*>::iterator it = m_setActualClouds.begin();
        for( ; it != m_setActualClouds.end(); it++)
        {   // Draw ACTUAL Clouds                
            CCloud* pc = *it;
            pc->RenderThruSprite(pPolySprite, m_pCloudAtlas);
        }         
    }    
    
    return EXIT_SUCCESS;
}

UINT CCloudSky::RenderCloudsToShadowMap(IDirect3DDevice9* pd3dDevice,
                                        const D3DXVECTOR3* pEyePt,
                                        const D3DXVECTOR3* pEyeAhead,
                                        const D3DXMATRIX* pProj,
                                        const D3DXMATRIX* pView)
{
    D3DXMATRIX mIdentity;
    D3DXMatrixIdentity(&mIdentity);
    m_pImpostorAtlasShadow->RenderAtlasForShadowMap(pd3dDevice, pEyePt, pEyeAhead, pProj, pView, &mIdentity);
    return EXIT_SUCCESS;
}

UINT CCloudSky::UpdateShadowClouds(IDirect3DDevice9* pd3dDevice,
                                 const D3DXMATRIX* pProj,
                                 const D3DXMATRIX* pView,
                                 const D3DVIEWPORT9* pViewPort)
{
    // determine visible clouds from sun's light frustum (or should i assume they are all visible?)
    // FOR NOW ASSUMING ALL CLOUDS VISIBLE TO SUN


    D3DXMATRIX mIdentity;
    D3DXMatrixIdentity(&mIdentity);

    // TODO: DETERMINE IN A PRIORITY QUEUE WHICH CLOUDS SHOULD AHVE THEIR IMPOSTOR UPDATED!
    // TODO: HAVE THE ANGLE/DISTANCE THRESHOLDS DONE ON AN ATLAS BY ATLAS BASIS AS OPPOSED TO #defines.

    // render clouds to impostor slots that require updating
    for(size_t i=0;i<m_vpCloudBatchesShadow.size();i++)
    {
        const vector<CCloud*> &vpClouds = m_vpCloudBatchesShadow[i]->GetClouds();
        for(size_t i=0; i<vpClouds.size(); i++)
        {
            vpClouds[i]->UpdateImpostor(pd3dDevice,
                                        pProj,
                                        pView,
                                        &mIdentity,
                                        pViewPort, 
                                        m_pSky->GetLightPos(),
                                        m_pSky->GetLightUp(),
                                        m_pSky->GetLightRight(),
                                        (D3DXVECTOR3*)m_pSky->GetLightDir(),
                                        m_pCloudAtlas);                                        
            m_pImpostorAtlasShadow->FlagToDraw( vpClouds[i]->GetAtlasId() );  // flag to draw all clouds for now.
        }
    }
    return EXIT_SUCCESS;
}

/*! Saves clouds to disk: 
<nbatches <nclouds in first batch <clouds ...n>> <nclouds in second batch <clouds ...n>> .. nbatches>*/
VOID CCloudSky::SaveClouds(LPCSTR cloudFilename)
{
    ofstream fout;
    fout.open(cloudFilename);

    fout << (UINT)m_vpCloudBatches.size() << endl;    // nBatches

    for(size_t i=0; i<m_vpCloudBatches.size(); i++)
    {   // Go thru each batch and render nClouds per batch and each cloud
        const vector<CCloud*> &vpClouds = m_vpCloudBatches[i]->GetClouds();
        fout << (UINT)vpClouds.size() << endl;
        for(size_t j=0; j<vpClouds.size(); j++)
        {   // Go thru each cloud and just write out its CloudInfo for now.
            fout << vpClouds[j]->GetCloudInfo();
        }
    }
    fout.close();
}

//! Loads clouds from CloudMap. Loads a copy for normal rendering and shadow rendering.
VOID CCloudSky::LoadClouds(LPCSTR cloudFilename)
{
    ifstream fin(cloudFilename);
    
    assert(fin.good());
    if(!fin.good())
        return ;

    UINT nBatches, nClouds;
    fin >> nBatches;

    for(size_t i=0; i<nBatches && fin.good(); i++)
    {
        CCloudBatch* pcb = new CCloudBatch(m_pPolySprite, m_pImpostorAtlas);
        CCloudBatch* pcbs = new CCloudBatch(m_pPolySpriteShadow, m_pImpostorAtlasShadow);
        fin >> nClouds;
        for(size_t j = 0; j<nClouds; j++)
        {
            SCloudInfo ci;
            fin >> ci;
            // Level Designer deals with cell/height spacing disparities.
            pcb->AddCloud(ci);
            pcbs->AddCloud(ci);
        }
        m_vpCloudBatches.push_back(pcb);
        m_vpCloudBatchesShadow.push_back(pcbs);
    }
    fin.close();
}

HRESULT CCloudSky::OnResetDevice(IDirect3DDevice9* pd3dDevice)
{ 
    HRESULT hr;
    V_RETURN( m_pSky->OnResetDevice(pd3dDevice) );

    if(m_pPolySprite)
        m_pPolySprite->OnResetDevice();
    if(m_pPolySpriteShadow)
        m_pPolySpriteShadow->OnResetDevice();

    if(m_pImpostorAtlas)
        V_RETURN( m_pImpostorAtlas->OnResetDevice(pd3dDevice) );    
    if(m_pImpostorAtlasShadow)
        V_RETURN( m_pImpostorAtlasShadow->OnResetDevice(pd3dDevice) );

    return S_OK;
}

HRESULT CCloudSky::OnLostDevice()
{   
    HRESULT hr;
    V_RETURN( m_pSky->OnLostDevice() );

    if(m_pPolySprite)
        m_pPolySprite->OnLostDevice();
    if(m_pPolySpriteShadow)
        m_pPolySpriteShadow->OnLostDevice();

    if(m_pImpostorAtlas)
        V_RETURN( m_pImpostorAtlas->OnLostDevice() );
    if(m_pImpostorAtlasShadow)
        V_RETURN( m_pImpostorAtlasShadow->OnLostDevice() );

    return S_OK;
}
