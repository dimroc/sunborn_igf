#pragma once
#include "iterrain.h"
#include "FXDecl.h"

using std::vector;


#define TERRAIN_EFFECT_NAME                 L"../Effects/Terrain.fx"
#define TERRAIN_TECHNIQUE_NAME              "AmbientDif_MultiTex"

//! Tiles the HMP file and mipmaps the tiles to different LODs
class CMipmapTerrain :
    public ITerrain
{       

public:
    static const UINT BLOCK_WIDTH           =       129;
    static const UINT DEFAULT_TILESIZE      =       65;

private:   
    // UINT m_TileSize; in iterrain.
    float m_fTextureTileScale;
	bool m_bIndices;    

	vector<int>	m_vTrianglesAtLOD;  // # of triangles at LOD

	vector<IDirect3DIndexBuffer9*> m_vLODIndexBuffers;	// IB at LOD

	vector<LPDIRECT3DINDEXBUFFER9> m_vSkirt;

private:
	//! Generates the tiles and each tiles LOD
    UINT GenerateIndexBuffers(IDirect3DDevice9 * pd3dDevice);

    //! Creates the Triangle Strip Vertex Buffer indexed by IBs
    bool CreateTriangleStrip(IDirect3DIndexBuffer9* IB, int sideLength, int resolution, int sizeBuffer);

	//! Creates a Vertical skirt at the given resolution
	void CreateSkirt(IDirect3DIndexBuffer9* &IB, int res, IDirect3DDevice9 * pd3dDevice);

    //! Returns the IB at the current LOD
	IDirect3DIndexBuffer9 *GetIndexBufferAtLOD(UINT LOD);

    //! Returns the # of triangles (including degenerate) at LOD.
	UINT GetNumTrianglesAtLOD(UINT LOD);

	//! Returns a skirt to hide cracks
	virtual IDirect3DIndexBuffer9 *GetSkirtIndex(int LOD, int &stripSize);

    //! Prereq: m_pd3dDevice != NULL
    UINT GenerateEffect(IDirect3DDevice9 *pd3dDevice,
                          LPCSTR pszBlendMap,
                          LPCSTR pszRedTex,
                          LPCSTR pszGreenTex,
                          LPCSTR pszBlueTex,
                          LPCSTR pszAlphaTex ); 

    //! Prepares terrain effect for drawing
    virtual UINT PrepareEffect();

public:
    //! Constructor Loads the .hmp file and tiles it with squares of length tileSize
    CMipmapTerrain(LPCSTR fn, 
                   float fTextureTileScale,
                   UINT nCellSpacing,
                   UINT nHeightSpacing,
                   UINT tileSize = DEFAULT_TILESIZE);

    virtual ~CMipmapTerrain(void);    

	virtual UINT GenerateData(IDirect3DDevice9 * pd3dDevice,
                              LPCSTR pszBlendMap,
                              LPCSTR pszRedTex,
                              LPCSTR pszGreenTex,
                              LPCSTR pszBlueTex,
                              LPCSTR pszAlphaTex );  

    //! Used to create any D3DPOOL_DEFAULT Devices. Only Effect for now
    virtual HRESULT OnResetDevice();

    //! Used to RECREATE any D3DPOOL_DEFAULT Devices Lost on minimize etc. Only Effect for now
    virtual HRESULT OnLostDevice();

private:
    CMipmapTerrain(const CMipmapTerrain& rhs); // disallow copy constructor
    CMipmapTerrain& operator=(const CMipmapTerrain& rhs); // disallow assignment operator
};
