#pragma once


#include "ITerrain.h"
#include "quadtree.h"

#define GRASS_EFFECT_NAME                 L"Effects/Grass.fx"
#define GRASS_TECHNIQUE_NAME              "GrassMeadowRender"

// struct CQuadTree::QTNode; // Reducing compiler dependency with forward declarations.

using std::vector;
using std::string;
using std::set;

//! Struct to hold grass location
struct SGrass {
    D3DXMATRIX mWorld;
    D3DXVECTOR3 vPos; // position of grass object center.
	UINT nGrassTex;	// index of the grass texture
    D3DXVECTOR4 vClusterTranslation;
    float fScale;
}; // TODO: Have grass cluster have its own wind speed and direction to promote local chaos

//! Vertex used for grass star sprite
struct GRASSVERT {
    D3DXVECTOR3 pos; // position
    D3DXVECTOR3 norm;
    float tu, tv; // tex coordinate    

    const static D3DVERTEXELEMENT9 Decl[4];
};

/*! Renders grass as sprites but as a diamond sprite , like 3 rotated around a center. */
class CFoliage
{
private:
    LPDIRECT3DVERTEXBUFFER9         m_pVB;
    LPDIRECT3DINDEXBUFFER9          m_pIB;

    IDirect3DVertexDeclaration9*    m_pGrassDecl; //! Vertex decl for sky rendering
    ID3DXEffect*                    m_pGrassEffect; //! D3DX effect interface

    //! Cache handles of effect
    D3DXHANDLE                      m_GrassTechnique, 
                                    m_GrassTextureHandle,
                                    m_LightDirHandle, 
                                    m_WorldViewProjHandle,
                                    m_ObjectHeightHandle,
                                    m_ClusterTranslationHandle;

	vector<string>					m_vsGrassTexFN;
	vector<LPDIRECT3DTEXTURE9>      m_vpGrassTex;
    vector<SGrass*>                 m_vpGrass; // inherently sorted by grasstex because of loadgrassmap.

	UINT							m_nMaxWidth, m_nMaxLength, m_nCellSpacing, m_nHeightSpacing;    

private:
    //! Creates a cluster of star sprites, 3 quads rotated about the same center.
    VOID CreateGrassCluster(IDirect3DDevice9* pd3dDevice);

    /*! Creates a world matrix that is:
        translated to pos
        rotated to line up with the normal
        scaled to scale. */
    VOID CreateWorldMatrix(D3DXMATRIX* mWorld, const D3DXVECTOR3& pos, const D3DXVECTOR3& norm, float scale);

    /*! Loads the grassmap by populating the vector<SGrass*>.
        100 byte header then raw 1-bit data. first 2 fields in header are width, height. */
    VOID LoadGrassMap(LPCSTR mapFN, UINT texID, const ITerrain* pt);

    VOID UpdateGrassClusterTranslation(SGrass* pgrass); //! Updates the cluster translation based on wind etc.
public:
    CFoliage(); //! sets terrain variables
    ~CFoliage(void);

    //! loads grasses into m_vpGrass and adds texture to be loaded
	int AddGrassMap(LPCSTR mapFN, LPCSTR texFN, const ITerrain* pt); 

    /*! Goes through m_vpGrass and fills leaf nodes vector<SGrass*> 
    with grass clusters bounded by the particular leaf QTNode. */
    VOID PopulateLeafNodesWithFoliage(CQuadTree::QTNode *proot);

    VOID OnCreateDevice(IDirect3DDevice9* pd3dDevice); //! Create the textures and VB

    VOID OnFrameMove(       //! Sets the shader constants such as fTimeStamp.
        float fElapsedTime,
        const D3DXVECTOR4* lightDir,
        const D3DXMATRIX* pMatProj,
        const D3DXMATRIX* pMatView
        ); 

	//! Renders grass through shader
    VOID OnFrameRender(
        IDirect3DDevice9 * pd3dDevice, 
        set<SGrass*> spGrasses,
        const D3DXMATRIX* pMatProj,
        const D3DXMATRIX* pMatView);

    VOID OnResetDevice(IDirect3DDevice9 * pd3dDevice);
    VOID OnLostDevice();
};