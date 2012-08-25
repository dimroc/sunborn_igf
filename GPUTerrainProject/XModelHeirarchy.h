#pragma once

#include "GameDefine.h"

using std::vector;

// The time to change from one animation set to another
// To see how the merging works - increase this time value to slow it down
const float kMergeTransitionTime = 0.3f;

HRESULT AllocateName ( LPCSTR Name, LPSTR *pNewName );

//--------------------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: Structure derived from D3DXFRAME so we can add some app-specific
//       info that will be stored with each frame
//--------------------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
    D3DXMATRIXA16        CombinedTransformationMatrix;
};


//--------------------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//--------------------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9*  ppTextures;       // array of textures, entries are NULL if no texture specified    
                                
    // SkinMesh info             
    LPD3DXMESH           pOrigMesh;
    LPD3DXATTRIBUTERANGE pAttributeTable;
    DWORD                NumAttributeGroups; 
    DWORD                NumInfl;
    LPD3DXBUFFER         pBoneCombinationBuf;
    D3DXMATRIX**         ppBoneMatrixPtrs;
    D3DXMATRIX*          pBoneOffsetMatrices;
    DWORD                NumPaletteEntries;
    bool                 UseSoftwareVP;
    DWORD                iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning

    /*!  Buffer to deal with bone matrices between D3DXMESHCONTAINER_DERIVED and CXModelHeirarchy */
    D3DXMATRIXA16*       pBoneMatrices;
    UINT                 nBoneMatricesMax;

    HRESULT GenerateSkinnedMesh( IDirect3DDevice9 *pd3dDevice );
};

struct SXHeirarchy : public ID3DXAllocateHierarchy
{   

    //--------------------------------------------------------------------------------------
    // Functions necessary to overload ID3DXAllocateHierarchy for use with X Files and XLoadFromHeirarchy.    
    //--------------------------------------------------------------------------------------
    STDMETHOD( CreateFrame ) ( LPCSTR Name, LPD3DXFRAME *ppNewFrame );

    STDMETHOD( CreateMeshContainer ) (
                                    LPCSTR Name, 
                                    CONST D3DXMESHDATA *pMeshData,
                                    CONST D3DXMATERIAL *pMaterials, 
                                    CONST D3DXEFFECTINSTANCE *pEffectInstances, 
                                    DWORD NumMaterials, 
                                    CONST DWORD *pAdjacency, 
                                    LPD3DXSKININFO pSkinInfo, 
                                    LPD3DXMESHCONTAINER *ppNewMeshContainer);

    STDMETHOD( DestroyFrame ) (LPD3DXFRAME pFrameToFree);
    
    STDMETHOD( DestroyMeshContainer ) (LPD3DXMESHCONTAINER pMeshContainerBase);
};


#define                     HEIRARCHYEFFECTFILE      "../GPUTerrainProject/Effects/SkinnedMesh.fx"
#define                     STANDARDEFFECTFILE       "../GPUTerrainProject/Effects/StandardMesh.fx"

//! Holds the actual mesh data for a model. This class utilizes the X files and takes heavily
//! from the SkinnedMesh DirectX October 2005 SDK Sample. The class CModelInstance creates modelInstances
//! of this geometries and is needed to actually render the mesh.
class CXModelHeirarchy
{
friend class CModelInstance;
public:
    static bool                 s_bUseSoftwareVP;    
    
private:
    bool                        m_bHeirarchy;
    // HEIRARCHY MESH VARIABLES
    WCHAR*                      m_wszShaderSource[4];   // holds shader source for assemly shaders.

    ID3DXAnimationController*   m_pAnimController;
    vector<double>              m_vAnimPeriods; //! Vector of the durations of time for one period of an animation.

    LPD3DXFRAME                 m_pBindPoseFrameRoot;

    D3DXVECTOR3                 m_vObjectCenter;        // Center of bounding sphere of object
    FLOAT                       m_fObjectRadius;        // Radius of bounding sphere of object
    
    IDirect3DVertexShader9*     m_pIndexedVertexShader[4];

    DWORD                       m_dwBehaviorFlags;      // Behavior flags of the 3D device

    // STANDARD MESH VARIABLES
    ID3DXMesh*                  m_pStandardMesh;
    LPD3DXBUFFER                m_pAdjacencyBuffer, m_pMaterialsBuffer;
    vector<LPDIRECT3DTEXTURE9>  m_vpTextures;    

    D3DXVECTOR3                 m_vObjectExtremities;

    // effect
    ID3DXEffect*                m_pEffect;       // D3DX effect interface

    D3DXHANDLE                  m_ShadowMtxHandle, m_ShadowMapHandle;

private:
    //! makes the heirarchy and makes paramater the root.
    HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame, LPD3DXFRAME pRootFrame ); 
    HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pRootFrame ); // helper    

    //! Clean up RAM allocation in creation.
    void ReleaseAttributeTable( LPD3DXFRAME pFrameBase );

    //! Called by constructor.
    HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, LPCSTR strFileName );    

    //! Draws the static mesh.
    void DrawStandard( LPDIRECT3DDEVICE9 pd3dDevice );

    //! Draws the CXModelHeirarchy heirarchy
    void DrawFrame( IDirect3DDevice9 *pd3dDevice, LPD3DXFRAME pFrame, LPDIRECT3DTEXTURE9 pTex, const D3DXMATRIX* pMatView );    
    
    //! Draws the frames mesh, helper for DrawMesh below.
    void DrawMeshContainer( 
                            IDirect3DDevice9 *pd3dDevice, 
                            LPD3DXMESHCONTAINER pMeshContainerBase, 
                            LPD3DXFRAME pFrameBase, 
                            LPDIRECT3DTEXTURE9 pTex,
                            const D3DXMATRIX* pMatView );

    //! Updates heirarchy with new combined (world srt) transformation.
    void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix );

    //! Advances animation.
    void AdvanceAnimController( float fElapsedTime, LPD3DXANIMATIONCALLBACKHANDLER pCallBackHandler ) 
    { 
        if( m_pAnimController )  
            m_pAnimController->AdvanceTime( fElapsedTime, pCallBackHandler ); 
    }

public:
    CXModelHeirarchy( IDirect3DDevice9* pd3dDevice, LPCSTR strFileName, bool bHeirarchy = false );
    ~CXModelHeirarchy(void);

    VOID OnResetDevice( LPDIRECT3DDEVICE9 pd3dDevice, float zFarPlane = FARPLANE_DISTANCE );
    VOID OnLostDevice();

    UINT AddAnimation( LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR strFileName );

    UINT GetNumberOfAnimationSets() const { if(m_pAnimController) return m_pAnimController->GetNumAnimationSets(); return 0; }
    LPD3DXANIMATIONSET GetAnimationSet(UINT idx);

    //! there 
    double GetAnimationPeriod(UINT idx) const { 
        idx--;  //! the idx is -1'd because this does not take into consideration the bogus register vertices animation entry.
        assert(idx<m_vAnimPeriods.size());
        return m_vAnimPeriods[idx]; 
    }

    vector<double> GetAnimationPeriods()                { return m_vAnimPeriods; }
    
    //! Sets the light's viewspace and the shadowmap texture to be used in OnFrameRender by the effect/shaders.
    void SetShadowMap( IDirect3DDevice9 *pd3dDevice, const D3DXMATRIX* pShadowMtx, LPDIRECT3DTEXTURE9 pShadowMap );

    bool IsHeirarchy()                                  { return m_bHeirarchy; }
    ID3DXMesh* GetStandardMesh()                        { return m_pStandardMesh; }
};

HRESULT FindMeshExtremities( D3DXVECTOR3* pCenterOut, D3DXVECTOR3* pExtremitiesOut, ID3DXMesh* m_pStandardMesh );
