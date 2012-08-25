#include "dxstdafx.h"
#include ".\foliage.h"
#include "globaldefine.h"
#include "QuadTree.h"

using std::ifstream;
using std::ios;

// --------------------------------------------------------------------------------------
// Public Functions
// --------------------------------------------------------------------------------------

//! Loads grass from Grass map!
CFoliage::CFoliage() :    
    m_pVB(NULL), 
    m_pIB(NULL),
	m_pGrassEffect(NULL), 
	m_pGrassDecl(NULL)
{
}

CFoliage::~CFoliage(void)
{
    for(size_t i=0;i<m_vpGrass.size();i++) {
        SAFE_DELETE(m_vpGrass[i]);
    }
	for(size_t i=0; i<m_vpGrassTex.size(); i++) {
		SAFE_RELEASE(m_vpGrassTex[i]);
	}
	SAFE_RELEASE(m_pVB);
    SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pGrassDecl);
	SAFE_RELEASE(m_pGrassEffect);

	m_vsGrassTexFN.clear();
}

int CFoliage::AddGrassMap(LPCSTR mapFN, LPCSTR texFN, const ITerrain* pt)
{
	// Load mapFN and create entries in m_vpGrass
    LoadGrassMap(mapFN, (UINT)m_vsGrassTexFN.size(), pt);	

    // Add texture to load later in OnCreateDevice
	m_vsGrassTexFN.push_back(string(texFN));
	
	return EXIT_SUCCESS;
}

VOID CFoliage::OnCreateDevice(IDirect3DDevice9* pd3dDevice)
{
    HRESULT hr;

	LPDIRECT3DTEXTURE9 pGrassTex;
	for(size_t i=0; i<m_vsGrassTexFN.size(); i++) 
	{	// Load the textures
		V( D3DXCreateTextureFromFileA( pd3dDevice, m_vsGrassTexFN[i].c_str(), &pGrassTex ) );
		m_vpGrassTex.push_back(pGrassTex);
	}

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    V( D3DXCreateEffectFromFile( pd3dDevice, GRASS_EFFECT_NAME, NULL, NULL, dwShaderFlags, 
                                    NULL, &m_pGrassEffect, NULL ) );

    //
    // Get Handles
    //
    m_GrassTechnique = m_pGrassEffect->GetTechniqueByName( GRASS_TECHNIQUE_NAME );

    m_GrassTextureHandle = m_pGrassEffect->GetParameterByName(NULL, "GrassTexture");
    m_WorldViewProjHandle = m_pGrassEffect->GetParameterByName(NULL, "matWorldViewProj");
    m_LightDirHandle = m_pGrassEffect->GetParameterByName(NULL, "vecLightDir");
    m_ClusterTranslationHandle = m_pGrassEffect->GetParameterByName(NULL, "vecClusterTranslation");    
    m_ObjectHeightHandle = m_pGrassEffect->GetParameterByName(NULL, "fObjectHeight");

    V( pd3dDevice->CreateVertexDeclaration( GRASSVERT::Decl, &m_pGrassDecl ) );

    CreateGrassCluster(pd3dDevice); // Helper function to create star sprite in m_pVB
}

/*! Goes through m_vpGrass and fills leaf nodes vector<SGrass*> 
    with grass clusters bounded by the particular leaf QTNode. */
VOID CFoliage::PopulateLeafNodesWithFoliage(CQuadTree::QTNode* proot)
{   // TODO:


}

VOID CFoliage::OnFrameMove(
        float fElapsedTime,
        const D3DXVECTOR4* pLightDir,
        const D3DXMATRIX* pMatProj,
        const D3DXMATRIX* pMatView)
{
    HRESULT hr;
    // set shader constants

    V( m_pGrassEffect->SetVector(m_LightDirHandle, pLightDir) );
}

/*
TODO: Populate QTNodes somewhere before this. OnCreateDevice?
This shit is wrong. Have to draw grass CLUSTERS in one draw call, not one grass star sprite.
*/
VOID CFoliage::OnFrameRender(IDirect3DDevice9* pd3dDevice, 
                             set<SGrass*> spGrasses, 
                             const D3DXMATRIX* pMatProj,
                             const D3DXMATRIX* pMatView)
{
    HRESULT hr; 
    UINT cPass;
    D3DXMATRIX mtx;

    // Set up device to draw grass. Set effect, streamsource etc.
    V( pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(GRASSVERT)) );
    V( pd3dDevice->SetIndices(m_pIB) );

    V( pd3dDevice->SetVertexDeclaration( m_pGrassDecl ) );    

    V( m_pGrassEffect->SetTechnique(m_GrassTechnique) );    

    // turn off culling
    V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE) );  

    //
    // Render
    //

    V( m_pGrassEffect->Begin(&cPass, 0) );           

    for(UINT i=0;i<(UINT)m_vpGrassTex.size();i++)
    {   // Render through all textures
        V( m_pGrassEffect->SetTexture(m_GrassTextureHandle, m_vpGrassTex[i]) );

        for(UINT p=0; p<cPass; p++) 
        {   // Render through all passes
            V( m_pGrassEffect->BeginPass( p ) );

            set<SGrass*>::iterator it;
            for(it = spGrasses.begin(); it != spGrasses.end(); it++)
            {       
                SGrass* pgrass = *it;
                if( pgrass->nGrassTex == i)
                {   // If grass cluster uses this texture, draw
                    
                    UpdateGrassClusterTranslation(pgrass);
                    V( m_pGrassEffect->SetVector(m_ClusterTranslationHandle, &pgrass->vClusterTranslation) );

                    mtx = pgrass->mWorld * (*pMatView) * (*pMatProj);
                    V( m_pGrassEffect->SetMatrix(m_WorldViewProjHandle, &mtx) );
                    V( m_pGrassEffect->SetFloat(m_ObjectHeightHandle, pgrass->fScale) );                    

                    V( m_pGrassEffect->CommitChanges() );

                    // TODO: Fix so it draws a cluster, not one.
                    V( pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2) ); 
                }
            }
            V( m_pGrassEffect->EndPass() );
        }
    }
    V( m_pGrassEffect->End() );

    //
    // clean up device a little
    //
    V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW) ); // reset cull mode
}

VOID CFoliage::OnResetDevice(IDirect3DDevice9* pd3dDevice)
{
    HRESULT hr; 
    if(m_pGrassEffect)
        V( m_pGrassEffect->OnResetDevice() );
}

VOID CFoliage::OnLostDevice()
{
    HRESULT hr;
    if(m_pGrassEffect)
        V( m_pGrassEffect->OnLostDevice() );
}

// --------------------------------------------------------------------------------------
// Private Functions
// --------------------------------------------------------------------------------------

/*!
        \   /
         \ /
     ----  -----
         / \
        /   \
   For top and bottom of one star sprite. 
   
   TODO: This shit is wrong. Have to draw grass CLUSTERS in one draw call, not one grass star sprite.
   */
VOID CFoliage::CreateGrassCluster(IDirect3DDevice9* pd3dDevice)
{
    HRESULT hr;
    // set vertex count and index count 

    DWORD dwVertices = 3 * 4 ;  // 4 vertices per plane. (top/bottom)
    DWORD dwIndices = 6 * 3;    // in TRIANGLE_LIST, 6 indices per plane.

    // Create the vertex buffer

    V( pd3dDevice->CreateVertexBuffer( 
        dwVertices * sizeof ( GRASSVERT ), 
        D3DUSAGE_WRITEONLY, 0,  // using declarations 
        D3DPOOL_MANAGED, &m_pVB, NULL ) );

    // Create the index buffer

    V( pd3dDevice->CreateIndexBuffer( 
        dwIndices, 
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
        D3DPOOL_MANAGED, &m_pIB, NULL ) );        

    //
    // Lock the vertex buffer
    //

    GRASSVERT *pVertex ; 
    V( m_pVB->Lock( 0, 0, (VOID**)&pVertex , 0 ) );


    V( m_pVB->Unlock() );

    //
    // Lock the index buffer 
    //

    WORD *pIndices;
    V( m_pIB->Lock( 0, 0, (VOID **)&pIndices, 0 ) );

    V( m_pIB->Unlock() );
}
/*! Creates a world matrix that is:
        translated to pos
        rotated to line up with the normal
        scaled to scale. */
// TODO:
VOID CFoliage::CreateWorldMatrix(D3DXMATRIX* mWorld, const D3DXVECTOR3& pos, const D3DXVECTOR3& norm, float scale)
{
    D3DXMatrixIdentity(mWorld);
}

// This shit is wrong. Have to draw grass CLUSTERS in one draw call, not one grass star sprite.
VOID CFoliage::LoadGrassMap(LPCSTR mapFN, UINT texID, const ITerrain* pt)
{   // 100 byte header contains width, height then nothing.    
    LOG_CREATE(L"GPUTerrain");
    LOG_INFO("Loading GrassMap: " << mapFN);

    ifstream fin;
    fin.open(mapFN, ios::in | ios::binary);
    if(!fin.is_open()) {
        LOG_DEBUG("Error opening file! " << mapFN);
        return ;
    }    

    int width, length;
    fin.read((char*)&width, sizeof(int));
    fin.read((char*)&length, sizeof(int));
    fin.seekg(92, ios::cur); // move to 100th position, skips reserved bytes.

    UINT zmult = pt->GetHeight() / length ;
    UINT xmult = pt->GetWidth() / width ;    

    // read in raw data and populate m_vpGrass
    UCHAR scale;
    for(int z=0; z<length; z++)
    {
        for(int x=0; x<width; x++)
        {
            fin.read((char*)&scale, 1);
            if(scale)
            {   // Create a new SGrass* entry in m_vpGrass if not 0.
                int pos = (z*zmult)*pt->GetWidth() + (x*xmult);
                float y = pt->GetHeightAt(pos);

                SGrass* pgrass = new SGrass();
                pgrass->fScale = (float)scale;

                pgrass->vPos.x = (float)(x * xmult * pt->GetCellSpacing());
                pgrass->vPos.z = (float)(z * zmult * pt->GetCellSpacing());
                pgrass->vPos.y = y * pt->GetHeightSpacing();
                
                CreateWorldMatrix(&pgrass->mWorld, pgrass->vPos, pt->GetNormalAt(pos), scale);

                pgrass->nGrassTex = texID;
                m_vpGrass.push_back(pgrass);
            }
        }
    }    
    fin.close(); // done
}

// TODO: Have grass cluster have its own wind speed and direction to promote local chaos
VOID CFoliage::UpdateGrassClusterTranslation(SGrass* pgrass)
{    
    pgrass->vClusterTranslation.x = 0.0f;
    pgrass->vClusterTranslation.y = 0.0f;
    pgrass->vClusterTranslation.z = 0.0f;
    pgrass->vClusterTranslation.w = 0.0f;
}

// --------------------------------------------------------------------------------------
// Grass Vertex Declaration
// --------------------------------------------------------------------------------------
const D3DVERTEXELEMENT9 GRASSVERT::Decl[] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },    
    D3DDECL_END()
};
