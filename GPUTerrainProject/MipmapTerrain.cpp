#include "dxstdafx.h"
#include ".\mipmapterrain.h"
#include "gamedefine.h"


CMipmapTerrain::CMipmapTerrain(LPCSTR fn, 
                               float fTextureTileScale, 
                               UINT cellspacing, 
                               UINT heightspacing, 
                               UINT tileSize) :
    m_fTextureTileScale(fTextureTileScale)
{ 
    LOG_CREATE(L"GPUTerrain");
    m_bInit = false;
	m_bIndices = false;
    // Load in HMP
    UINT r = LoadHMP(fn); 

    // Generate Tile Size
    if(r == EXIT_SUCCESS) {            
        UINT squareLength = (m_Width < m_Height) ? m_Width : m_Height;
        // Tile Size must be (a power of 2) + 1 and less than width or height. if not select default.        
        m_TileSize = DEFAULT_TILESIZE;
        for(UINT i = squareLength-1; i > 0; i /= 2) {
            if( (tileSize-1) == i) {
                m_TileSize = tileSize;
                break;
            }
        }            
        m_bInit = true;
    }

    m_CellSpacing = cellspacing;
    m_HeightSpacing = heightspacing;

    LOG_DEBUG("Requested TileSize: " << tileSize);
    LOG_DEBUG("Given TileSize: " << m_TileSize);
}

CMipmapTerrain::~CMipmapTerrain() {
	for(UINT i = 0; i < m_vLODIndexBuffers.size(); i ++)
	{	SAFE_RELEASE(m_vLODIndexBuffers[i]);
		SAFE_RELEASE(m_vSkirt[i]);
	}
    SAFE_RELEASE(m_BlendTexture);

    SAFE_RELEASE(m_RedTexture);
    SAFE_RELEASE(m_GreenTexture);
    SAFE_RELEASE(m_BlueTexture);
    SAFE_RELEASE(m_AlphaTexture);

    SAFE_RELEASE(m_TerrainEffect);
    SAFE_RELEASE(m_TerrainDecl);
}

UINT CMipmapTerrain::GenerateData(IDirect3DDevice9 * pd3dDevice,
                                  LPCSTR pszBlendMap,
                                  LPCSTR pszRedTex,
                                  LPCSTR pszGreenTex,
                                  LPCSTR pszBlueTex,
                                  LPCSTR pszAlphaTex ) 
{    
    HRESULT hr;

    V( GenerateIndexBuffers(pd3dDevice) );
    V( GenerateEffect(pd3dDevice,
                      pszBlendMap,
                      pszRedTex,
                      pszGreenTex,
                      pszBlueTex,
                      pszAlphaTex) );    
    V( PrepareEffect() );

    return EXIT_SUCCESS;
}

//! Used to create any D3DPOOL_DEFAULT Devices. Only Effect for now
HRESULT CMipmapTerrain::OnResetDevice() 
{
    HRESULT hr;
    if(m_TerrainEffect)
    {
        const D3DSURFACE_DESC* pSurfDesc = DXUTGetBackBufferSurfaceDesc(); 

        float pixelOffsets[2] = { 1.f / (pSurfDesc->Width), 1.f / (pSurfDesc->Height) };
        V( m_TerrainEffect->SetFloatArray( "fPixelOffset", pixelOffsets, 2 ) );
        V( m_TerrainEffect->SetFloat( "fFarPlane", FARPLANE_DISTANCE ) );

        return m_TerrainEffect->OnResetDevice();
    }
    return S_OK;
}

//! Used to RECREATE any D3DPOOL_DEFAULT Devices Lost on minimize etc. Only Effect for now
HRESULT CMipmapTerrain::OnLostDevice() 
{
    if(m_TerrainEffect)
        return m_TerrainEffect->OnLostDevice();
    return S_OK;
}

/************************************ Private Functions *****************************************/

UINT CMipmapTerrain::GenerateEffect(IDirect3DDevice9 *pd3dDevice,
                                      LPCSTR pszBlendMap,
                                      LPCSTR pszRedTex,
                                      LPCSTR pszGreenTex,
                                      LPCSTR pszBlueTex,
                                      LPCSTR pszAlphaTex) 
{
    HRESULT hr;

    V(D3DXCreateTextureFromFileA(pd3dDevice, pszBlendMap, &m_BlendTexture) );
    V(D3DXCreateTextureFromFileA(pd3dDevice, pszRedTex, &m_RedTexture) );
    V(D3DXCreateTextureFromFileA(pd3dDevice, pszGreenTex, &m_GreenTexture) );
    V(D3DXCreateTextureFromFileA(pd3dDevice, pszBlueTex, &m_BlueTexture) );
    V(D3DXCreateTextureFromFileA(pd3dDevice, pszAlphaTex, &m_AlphaTexture) );

   // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, TERRAIN_EFFECT_NAME ) );

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    // If this fails, there should be debug output as to why
    // the .fx file failed to compile
    V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags, 
                                        NULL, &m_TerrainEffect, NULL ) );

    m_TerrainTechnique = m_TerrainEffect->GetTechniqueByName( TERRAIN_TECHNIQUE_NAME ); 
    m_WorldHandle = m_TerrainEffect->GetParameterByName(NULL, "mWorld" );
    m_ViewProjHandle = m_TerrainEffect->GetParameterByName(NULL, "mViewProj" );
    m_ITWorldHandle = m_TerrainEffect->GetParameterByName(NULL, "mITWorld" );
    m_ShadowMtxHandle = m_TerrainEffect->GetParameterByName(NULL, "mShadowMatrix" );
    m_LightDirHandle = m_TerrainEffect->GetParameterByName(NULL, "vecLightDir" );
    m_LightColorHandle = m_TerrainEffect->GetParameterByName(NULL, "vecLightColor" );
    m_TweenHandle = m_TerrainEffect->GetParameterByName(NULL, "Tween" );
    m_ShadowMapHandle = m_TerrainEffect->GetParameterByName(NULL, "ShadowMap");

    V_RETURN(pd3dDevice->CreateVertexDeclaration( TERRAINVERT::Decl, &m_TerrainDecl ));    

    return EXIT_SUCCESS;
}

//! Prereq: m_TerrainEffect != NULL
UINT CMipmapTerrain::PrepareEffect()
{
    HRESULT hr;
    // Get Technique Handle
    V( m_TerrainEffect->SetTechnique(m_TerrainTechnique) );

    // Set TerrainWidthLength for texture coordinate calculation in shader.
    float twh[2];
    twh[0] = (float)(m_Width * m_CellSpacing);
    twh[1] = (float)(m_Height * m_CellSpacing);
    V( m_TerrainEffect->SetFloatArray("TWidthLength", twh, 2) );

    // Set Textures
    V( m_TerrainEffect->SetTexture("BlendMap", m_BlendTexture) );

    V( m_TerrainEffect->SetTexture("RedTexture", m_RedTexture) );
    V( m_TerrainEffect->SetTexture("GreenTexture", m_GreenTexture) );
    V( m_TerrainEffect->SetTexture("BlueTexture", m_BlueTexture) );    
    V( m_TerrainEffect->SetTexture("AlphaTexture", m_AlphaTexture) ); 

    // Set scale by which to tile textures across rgb
    V( m_TerrainEffect->SetFloat("TextureScale", m_fTextureTileScale) );
    return S_OK;
}

//! Load the Tiles in VRAM! Or perhaps at least the ones around camera.
/*! Stores everything in VRAM on device Create in D3DPOOL_MANAGED */
UINT CMipmapTerrain::GenerateIndexBuffers(IDirect3DDevice9 * pd3dDevice) {
	if(m_bIndices == true) return EXIT_SUCCESS;

    HRESULT hr;
	
	UINT TileSide = m_TileSize;
	for(UINT i = 0; i < m_LOD; i ++)
	{	m_vTrianglesAtLOD.push_back((TileSide+1)*2*(TileSide-1)-2);
		TileSide = TileSide/2 + 1;
	}

	TileSide = m_TileSize;
	WORD res = 1;
	for(UINT i = 0; i < m_LOD; i ++)
	{	m_vLODIndexBuffers.push_back(NULL);
		
		//Create Index buffer for tile
		V(pd3dDevice->CreateIndexBuffer(
		(m_vTrianglesAtLOD[i]+2)*sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_vLODIndexBuffers[i],
		0) );

		if(!CreateTriangleStrip(m_vLODIndexBuffers[i], TileSide, res, m_vTrianglesAtLOD[i]+2))
			return EXIT_FAILURE;		

		res = res * 2;		
	}

	for(UINT i = 0, res = 1; i < m_LOD; i ++, res *= 2)
	{	m_vSkirt.push_back(NULL);
		CreateSkirt(m_vSkirt[i], res, pd3dDevice);
	}

	m_bIndices = true;
	return EXIT_SUCCESS;
}

void CMipmapTerrain::CreateSkirt(IDirect3DIndexBuffer9* &IB, int res, IDirect3DDevice9 * pd3dDevice)
{	WORD *indices;
    HRESULT hr;
	int blockWidth = BLOCK_WIDTH;

	int skip = 2*res;

	int numIndices = res == 1 ? blockWidth*2 : (blockWidth/res + 1)*2;
	
	V(pd3dDevice->CreateIndexBuffer(
		(numIndices)*sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB,
		0) );

	V(IB->Lock(0, 0, (void**)&indices, 0));

	int count = 0;
	for(WORD i = 0; i < blockWidth*2; i = (WORD)(i+skip) ) // getting rid of warning
	{	indices[count++] = i;
		indices[count++] = i+1;
	}
	assert(count == numIndices);
	V(IB->Unlock());
}

IDirect3DIndexBuffer9 *CMipmapTerrain::GetSkirtIndex(int LOD, int &stripSize)
{	if(LOD >= (int)m_LOD)
		LOD = m_LOD - 1;
	stripSize = LOD == 0? MIPMAP_TILESIZE*2 : (MIPMAP_TILESIZE/(int)pow(2.f, LOD) + 1)*2;
	return m_vSkirt[LOD];
}

bool CMipmapTerrain::CreateTriangleStrip(IDirect3DIndexBuffer9* IB, int sideLength, int resolution, int sizeBuffer)
{	WORD *indices;
    HRESULT hr;
	
	int indexSide = (resolution <= 1) ? sideLength : sideLength/resolution + 1;
	int numIndices = (indexSide+1)*2*(indexSide-1)-2;

	if(sizeBuffer < numIndices) 
		return false;	//not enough space to make strip

    int count = 0;
	int tInit = 0;
	int bInit = sideLength*resolution;

	int tRow = tInit;
	int bRow = bInit;
	int mul = resolution;

	V(IB->Lock(0, 0, (void**)&indices, 0));
	for(int i = 0; count < numIndices; i ++)
	{	if(i%2)
		{	indices[count++] = (WORD)bRow;
			bRow += resolution;
		}
		else
		{	indices[count++] = (WORD)tRow;
			tRow += resolution;
		}
		
		if(i+1 == indexSide*2)
		{	tRow = tInit + sideLength*mul;
			bRow = bInit + sideLength*mul;
			indices[count++] = indices[count-1];
			indices[count++] = (WORD)tRow;
			i = -1;
			mul += resolution;
		}
	}

	V(IB->Unlock());
	return true;
}

IDirect3DIndexBuffer9 *CMipmapTerrain::GetIndexBufferAtLOD(UINT LOD)
{	
	return m_vLODIndexBuffers[LOD];
}

UINT CMipmapTerrain::GetNumTrianglesAtLOD(UINT LOD)
{	if(!m_bIndices) return 0;
	if(LOD > m_LOD-1)
		LOD = m_LOD-1;

	UINT numT = m_vTrianglesAtLOD[LOD];
	return numT;
}
