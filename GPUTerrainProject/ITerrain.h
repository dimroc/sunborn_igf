#pragma once

#include "gamedefine.h"
#include "globaldefine.h"

using std::vector;

#define MAX_FILENAME_LENGTH         100
#define MIPMAP_TILESIZE             65

//! Base Class for any Terrain Class. Loads HMP files.
class ITerrain 
{
protected:
    bool m_bInit;
    UINT m_Width, m_Height;
    UINT m_LOD; //! unfortunately, this value, because of our GeoMorphing, has become fixed at 3.
    UINT m_TileSize;

    int m_CellSpacing;
    int m_HeightSpacing;    

    float *m_HeightMap;
	float *m_DiffMap[2];
    D3DXVECTOR3 *m_NormalMap;    

    WCHAR m_TextureFileName[MAX_FILENAME_LENGTH];

    LPDIRECT3DTEXTURE9              m_BlendTexture;

    LPDIRECT3DTEXTURE9              m_RedTexture;
    LPDIRECT3DTEXTURE9              m_GreenTexture;
    LPDIRECT3DTEXTURE9              m_BlueTexture;
    LPDIRECT3DTEXTURE9              m_AlphaTexture;

    IDirect3DVertexDeclaration9*    m_TerrainDecl; //! Vertex decl for terrain rendering
    ID3DXEffect*                    m_TerrainEffect;        //! D3DX effect interface

    D3DXHANDLE                      m_TerrainTechnique, 
                                    m_WorldHandle,
                                    m_ViewProjHandle,
                                    m_ITWorldHandle,
                                    m_ShadowMtxHandle,
                                    m_LightDirHandle,
                                    m_LightColorHandle,
                                    m_TweenHandle,
                                    m_ShadowMapHandle; 

    static const UINT MIN_DIMENSION     =       65; // smallest hmp dimension
    static const UINT MAX_DIMENSION     =       4097; // Largest Dimension    

    void ZeroEdges();
    void SmoothHMP(); //! Called by LoadHMP to smooth hmp values with a 3x3 kernel.
    D3DXVECTOR3 GetSquareNormal(int i, int j); //! Returns the normal of the ijth square.

public:
    ITerrain(void) : 
      m_bInit(false), 

      m_BlendTexture(NULL),

      m_RedTexture(NULL),
      m_GreenTexture(NULL),
      m_BlueTexture(NULL),
      m_AlphaTexture(NULL),

      m_TerrainDecl(NULL),
      m_TerrainEffect(NULL),

      m_HeightMap(NULL), 
      m_NormalMap(NULL),            

      m_Width(0), 
      m_Height(0), 
      m_CellSpacing(2),
      m_HeightSpacing(2),
      m_LOD(3) //! fixed at 3. Will probably crash otherwise. Had to fixed for diff map.
      {
        m_DiffMap[0] = NULL;
        m_DiffMap[1] = NULL;
      }    
    
      virtual ~ITerrain(void) { 
          SAFE_DELETE_ARRAY(m_HeightMap);
          SAFE_DELETE_ARRAY(m_NormalMap);
          SAFE_DELETE_ARRAY(m_DiffMap[0]);
          SAFE_DELETE_ARRAY(m_DiffMap[1]);
      }

    //! Load .hmp file
    virtual UINT LoadHMP(LPCSTR fn); 

	//! Create map of where vertices morph to
	virtual UINT GenerateDiffMap(void);

    /*! Generate normals via Normal Averaging
        PREREQ: LoadHMP called successfully. Is Dependent on m_CellSpacing, be sure to call LoadNormals if changing spacing */
    virtual VOID LoadNormals();

    //! Load tiles information on video card
    virtual UINT GenerateData(IDirect3DDevice9 * pd3dDevice,
                              LPCSTR pszBlendMap,
                              LPCSTR pszRedTex,
                              LPCSTR pszGreenTex,
                              LPCSTR pszBlueTex,
                              LPCSTR pszAlphaTex )                          =       0;  

	//! Set up maxErrorVertices for a tile
	virtual void GetMaxErrorVertexAtTile(D3DXVECTOR3 v, vector<D3DXVECTOR3> &maxPoints, int &maxErrorIndex);

    //! Get the highest and lowest y value in a tile
    virtual void GetMinMaxHeight(D3DXVECTOR3 v, float &min, float &max);

    virtual UINT GetTileSize() { return m_TileSize; }

    //! Returns the IB at LOD
	virtual IDirect3DIndexBuffer9 *GetIndexBufferAtLOD(UINT LOD)			=		0;

    //! Returns the # of triangles (including degenerate) at lod
	virtual UINT GetNumTrianglesAtLOD(UINT LOD)								=		0;

	virtual UINT GetNumberOfLODs(void);

	virtual IDirect3DIndexBuffer9 *GetSkirtIndex(int LOD, int &stripSize)   =       0;

    //! Used to create any D3DPOOL_DEFAULT Devices. Only Effect for now
    virtual HRESULT OnResetDevice()                              =   0;

    //! Used to RECREATE any D3DPOOL_DEFAULT Devices Lost on minimize etc. Only Effect for now
    virtual HRESULT OnLostDevice()                               =   0;

    //! Check if everything is good with object.
    inline bool IsInit()                { return m_bInit; }    

	//! Return pointer to height map
	inline const float *GetHeightMap(void) const {return m_HeightMap;}
	
	//! Return pointer to diff map
	float *GetDiffMap(UINT LOD) {
        if(LOD >= m_LOD)
            LOD = m_LOD - 1;
        return m_DiffMap[LOD];
    }

	//! Get Height at a heightmap location
	inline float GetHeightAt(int pos) const { return m_HeightMap[pos]; }

    //! Get the difference between lower res height and highest res height
    float GetDiffAtLOD(UINT LOD, UINT pos)     { 
        if(LOD >= m_LOD)
            LOD = m_LOD-1;
        return m_DiffMap[LOD][pos];
    }

    //! Get Normal at pos
    inline D3DXVECTOR3 GetNormalAt(int pos) const { return m_NormalMap[pos]; }
    

    DIM_GET_ACCESSOR(UINT, Height);
    DIM_GET_ACCESSOR(UINT, Width);
    DIM_GET_ACCESSOR(UINT, LOD);

    DIM_GET_ACCESSOR(int, CellSpacing);
    DIM_GET_ACCESSOR(int, HeightSpacing);

    DIM_GET_ACCESSOR(LPDIRECT3DTEXTURE9, BlendTexture);

    DIM_GET_ACCESSOR(LPDIRECT3DTEXTURE9, RedTexture);
    DIM_GET_ACCESSOR(LPDIRECT3DTEXTURE9, GreenTexture);
    DIM_GET_ACCESSOR(LPDIRECT3DTEXTURE9, BlueTexture);
    DIM_GET_ACCESSOR(LPDIRECT3DTEXTURE9, AlphaTexture);

    DIM_GET_ACCESSOR(IDirect3DVertexDeclaration9*, TerrainDecl);
    DIM_GET_ACCESSOR(ID3DXEffect*, TerrainEffect);
    DIM_GET_ACCESSOR(D3DXHANDLE, TerrainTechnique);
    DIM_GET_ACCESSOR(D3DXHANDLE, WorldHandle);
    DIM_GET_ACCESSOR(D3DXHANDLE, ViewProjHandle);
    DIM_GET_ACCESSOR(D3DXHANDLE, ITWorldHandle);    
    DIM_GET_ACCESSOR(D3DXHANDLE, LightDirHandle);
    DIM_GET_ACCESSOR(D3DXHANDLE, LightColorHandle);
    DIM_GET_ACCESSOR(D3DXHANDLE, TweenHandle);
    DIM_GET_ACCESSOR(D3DXHANDLE, ShadowMtxHandle);
    DIM_GET_ACCESSOR(D3DXHANDLE, ShadowMapHandle);
};
