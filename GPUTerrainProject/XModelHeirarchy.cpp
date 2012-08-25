#include "dxstdafx.h"
#include ".\xmodelheirarchy.h"
#include "gamedefine.h"
#include "globaldefine.h"
#include "fxdecl.h"


#define         TECHNIQUE                   "CelShade"

bool            CXModelHeirarchy::s_bUseSoftwareVP = false;

//--------------------------------------------------------------------------------------
// struct SXHeirarchy : public ID3DXAllocateHierarchy
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Functions necessary to overload ID3DXAllocateHierarchy for use with X Files and XLoadFromHeirarchy.    
//--------------------------------------------------------------------------------------

HRESULT AllocateName ( LPCSTR Name, LPSTR *pNewName )
{
    UINT cbLength;

    if( Name != NULL )
    {
        cbLength = (UINT)strlen(Name) + 1;
        *pNewName = new CHAR[cbLength];
        if (*pNewName == NULL)
            return E_OUTOFMEMORY;
        memcpy( *pNewName, Name, cbLength*sizeof(CHAR) );
    }
    else
    {
        *pNewName = NULL;
    }

    return S_OK;

}

HRESULT SXHeirarchy::CreateFrame ( LPCSTR Name, LPD3DXFRAME *ppNewFrame )
{
    HRESULT hr = S_OK;
    D3DXFRAME_DERIVED *pFrame;

    *ppNewFrame = NULL;

    pFrame = new D3DXFRAME_DERIVED;
    if (pFrame == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    hr = AllocateName(Name, &pFrame->Name);
    if (FAILED(hr))
        goto e_Exit;

    // initialize other data members of the frame
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;

    *ppNewFrame = pFrame;
    pFrame = NULL;

e_Exit:
    delete pFrame;
    return hr;

}

HRESULT SXHeirarchy::CreateMeshContainer (
                                LPCSTR Name, 
                                CONST D3DXMESHDATA *pMeshData,
                                CONST D3DXMATERIAL *pMaterials, 
                                CONST D3DXEFFECTINSTANCE *pEffectInstances, 
                                DWORD NumMaterials, 
                                CONST DWORD *pAdjacency, 
                                LPD3DXSKININFO pSkinInfo, 
                                LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
    UINT NumFaces;
    UINT iMaterial;
    UINT iBone, cBones;
    LPDIRECT3DDEVICE9 pd3dDevice = NULL;

    LPD3DXMESH pMesh = NULL;

    *ppNewMeshContainer = NULL;

    // this sample does not handle patch meshes, so fail when one is found
    if (pMeshData->Type != D3DXMESHTYPE_MESH)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // get the pMesh interface pointer out of the mesh data structure
    pMesh = pMeshData->pMesh;

    // this sample does not handle non-FVF compatible meshes, so fail when one is found
    if (pMesh->GetFVF() == 0)
    {
        // DIMITRI ROCHE
        // Skipped and done in lower if clause.
        // hr = E_FAIL;
        // goto e_Exit;
    }

    // allocate the overloaded structure to return as a D3DXMESHCONTAINER
    pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    if (pMeshContainer == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

    // make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
    hr = AllocateName(Name, &pMeshContainer->Name);
    if (FAILED(hr))
        goto e_Exit;        

    pMesh->GetDevice(&pd3dDevice);
    NumFaces = pMesh->GetNumFaces();

    // if no normals are in the mesh, add them. OR if no FVF, clone and create.
    // DIMITRI HACK: If pMesh->GetFVF() == 0 this will also enter if() and clone mesh.
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        // clone the mesh to make room for the normals
        if(pMesh->GetFVF()) {
            hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                        pMesh->GetFVF() | D3DFVF_NORMAL, 
                                        pd3dDevice, &pMeshContainer->MeshData.pMesh );
            // now generate the normals for the pmesh
            D3DXComputeNormals( pMesh, NULL );
        }
        else {  // DIMITRI HACK: Get Maya Meshes (with no corresponding FVFs) to work.
            hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                        D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, 
                                        pd3dDevice, &pMeshContainer->MeshData.pMesh );
        }

        if (FAILED(hr))
            goto e_Exit;

        // get the new pMesh pointer back out of the mesh container to use
        // NOTE: we do not release pMesh because we do not have a reference to it yet
        pMesh = pMeshContainer->MeshData.pMesh;        
    }
    else  // if normals, just add a reference to the mesh for the mesh container
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        pMesh->AddRef();
    }
        
    // allocate memory to contain the material information.  This sample uses
    //   the D3D9 materials and texture names instead of the EffectInstance style materials
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    // if materials provided, copy them
    if (NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
        {
            if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
            {
                WCHAR strTexturePath[MAX_PATH];
                WCHAR wszBuf[MAX_PATH];
                MultiByteToWideChar( CP_ACP, 0, pMeshContainer->pMaterials[iMaterial].pTextureFilename, -1, wszBuf, MAX_PATH );
                wszBuf[MAX_PATH - 1] = L'\0';
                DXUTFindDXSDKMediaFileCch( strTexturePath, MAX_PATH, wszBuf );
                if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, strTexturePath,
                                                        &pMeshContainer->ppTextures[iMaterial] ) ) )
                    pMeshContainer->ppTextures[iMaterial] = NULL;

                // don't remember a pointer into the dynamic memory, just forget the name after loading
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
    else // if no materials provided, use a default one
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

    // if there is skinning information, save off the required data and then setup for HW skinning
    if (pSkinInfo != NULL)
    {
        // first save off the SkinInfo and original mesh data
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();

        // Will need an array of offset matrices to move the vertices from the figure space to the bone's space
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
        if (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // get each of the bone offset matrices so that we don't need to get them later
        for (iBone = 0; iBone < cBones; iBone++)
        {
            pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
        }

        // GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
        hr = pMeshContainer->GenerateSkinnedMesh( pd3dDevice );
        if (FAILED(hr))
            goto e_Exit;
    }

    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = NULL;

e_Exit:
    SAFE_RELEASE(pd3dDevice);

    // call Destroy function to properly clean up the memory allocated 
    if (pMeshContainer != NULL)
    {
        DestroyMeshContainer(pMeshContainer);
    }

    return hr;
}

HRESULT SXHeirarchy::DestroyFrame (LPD3DXFRAME pFrameToFree)
{
    SAFE_DELETE_ARRAY( pFrameToFree->Name );
    SAFE_DELETE( pFrameToFree );
    return S_OK; 
}

HRESULT SXHeirarchy::DestroyMeshContainer (LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT iMaterial;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    SAFE_DELETE_ARRAY( pMeshContainer->Name );
    SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
    SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
    SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );

    // release all the allocated textures
    if (pMeshContainer->ppTextures != NULL)
    {
        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
        }
    }

    SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
    SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
    SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
    SAFE_RELEASE( pMeshContainer->pSkinInfo );
    SAFE_RELEASE( pMeshContainer->pOrigMesh );
    SAFE_DELETE_ARRAY( pMeshContainer->pBoneMatrices );
    SAFE_DELETE( pMeshContainer );
    
    return S_OK;
}


//--------------------------------------------------------------------------------------
// PUBLIC CXModelHeirarchy //
//
//! CXModelHeirarchy handles all geometries in scene.
//--------------------------------------------------------------------------------------

CXModelHeirarchy::CXModelHeirarchy( IDirect3DDevice9* pd3dDevice, LPCSTR strFileName, bool bHeirarchy )
{
    memset(this, 0, sizeof(CXModelHeirarchy));
    m_bHeirarchy = bHeirarchy;

    HRESULT hr;

    // shader debugger stuff
    DWORD dwShaderFlags;
    dwShaderFlags = D3DXFX_NOT_CLONEABLE;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    if(bHeirarchy)
    {
        m_wszShaderSource[0] = L"skinmesh1.vsh";
        m_wszShaderSource[1] = L"skinmesh2.vsh";
        m_wszShaderSource[2] = L"skinmesh3.vsh";
        m_wszShaderSource[3] = L"skinmesh4.vsh";        

        // Load GPU skinning shader
        V( D3DXCreateEffectFromFileA( pd3dDevice, HEIRARCHYEFFECTFILE, NULL, NULL, dwShaderFlags, 
                                     NULL, &m_pEffect, NULL ) );

        V( LoadMesh( pd3dDevice, strFileName ) );  
        m_vObjectExtremities.x = m_fObjectRadius; m_vObjectExtremities.y = m_fObjectRadius; m_vObjectExtremities.z = m_fObjectRadius;

        // Obtain the behavior flags
        D3DDEVICE_CREATION_PARAMETERS cp;
        pd3dDevice->GetCreationParameters( &cp );
        m_dwBehaviorFlags = cp.BehaviorFlags;
    }
    else
    {
        DWORD nMaterials;
        V( D3DXLoadMeshFromXA(strFileName, D3DXMESH_MANAGED, pd3dDevice, &m_pAdjacencyBuffer, &m_pMaterialsBuffer, NULL, &nMaterials, &m_pStandardMesh) );
        V( D3DXCreateEffectFromFileA(pd3dDevice, STANDARDEFFECTFILE, NULL, NULL, dwShaderFlags, NULL, &m_pEffect, NULL) ); 
        V( m_pEffect->SetTechnique( TECHNIQUE ) );

        ID3DXMesh* pNewMesh;
        V( m_pStandardMesh->CloneMeshFVF( m_pStandardMesh->GetOptions(), 
                                D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, 
                                pd3dDevice, &pNewMesh ) );

        SAFE_RELEASE(m_pStandardMesh);
        m_pStandardMesh = pNewMesh;

        FindMeshExtremities(&m_vObjectCenter, &m_vObjectExtremities, m_pStandardMesh); //! Checks for the extreme boudning values of the mesh
        m_fObjectRadius = max(m_vObjectExtremities.x, m_vObjectExtremities.y);
        m_fObjectRadius = max(m_fObjectRadius, m_vObjectExtremities.z);

        // get all textures
        D3DXMATERIAL* pMats = (D3DXMATERIAL*)m_pMaterialsBuffer->GetBufferPointer();
        for(DWORD i = 0; i<nMaterials; i++)
        {
            // HACK: stupid x file appends an empty texture material on the end, skip it. (i think its that damn lambert1)
            if(strlen(pMats[i].pTextureFilename) <= 0)    continue;
            LPDIRECT3DTEXTURE9 pTex;
            V( D3DXCreateTextureFromFileA( pd3dDevice, pMats[i].pTextureFilename, &pTex ) );
            m_vpTextures.push_back(pTex);            
        }
    }
    V( m_pEffect->SetTechnique( TECHNIQUE ) );
    m_ShadowMtxHandle = m_pEffect->GetParameterByName( 0, "mShadowMatrix" );
    m_ShadowMapHandle = m_pEffect->GetParameterByName( 0, "ShadowMap" );
}

HRESULT CXModelHeirarchy::LoadMesh( IDirect3DDevice9* pd3dDevice, LPCSTR strFileName )
{
    assert(m_bHeirarchy);
    HRESULT hr;

    SXHeirarchy Alloc;
    V_RETURN( D3DXLoadMeshHierarchyFromXA( strFileName, D3DXMESH_MANAGED, pd3dDevice,
                                          &Alloc, NULL, &m_pBindPoseFrameRoot, &m_pAnimController ) );
    V_RETURN( SetupBoneMatrixPointers( m_pBindPoseFrameRoot, m_pBindPoseFrameRoot ) );
    V_RETURN( D3DXFrameCalculateBoundingSphere( m_pBindPoseFrameRoot, &m_vObjectCenter, &m_fObjectRadius ) );
    return hr;
}

CXModelHeirarchy::~CXModelHeirarchy(void)
{   
    if(m_bHeirarchy)
    {
        SAFE_RELEASE( m_pAnimController );
        ReleaseAttributeTable( m_pBindPoseFrameRoot );
        SXHeirarchy Alloc;
        D3DXFrameDestroy( m_pBindPoseFrameRoot, &Alloc );
    }

    SAFE_RELEASE(m_pMaterialsBuffer);
    SAFE_RELEASE(m_pAdjacencyBuffer);
    SAFE_RELEASE(m_pStandardMesh);

    for(size_t i=0; i<m_vpTextures.size(); i++) {
        SAFE_RELEASE(m_vpTextures[i]);
    }    
    SAFE_RELEASE( m_pEffect );
}

// VOID CXModelHeirarchy::OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice)
VOID CXModelHeirarchy::OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice, float zFarPlane)
{
    HRESULT hr;
    const D3DSURFACE_DESC* pSurfDesc = DXUTGetBackBufferSurfaceDesc(); 

    if( m_pEffect ) {
        V( m_pEffect->OnResetDevice() );
        float pixelOffsets[2] = { 1.f / (pSurfDesc->Width), 1.f / (pSurfDesc->Height) };
        V( m_pEffect->SetFloatArray( "fPixelOffset", pixelOffsets, 2 ) );		
        V( m_pEffect->SetFloat( "fFarPlane", zFarPlane) );
    }

    // load the indexed vertex shaders
    DWORD dwShaderFlags = 0;
    #if defined(DEBUG_VS) || defined(DEBUG_PS)
        dwShaderFlags |= D3DXSHADER_DEBUG|D3DXSHADER_SKIPVALIDATION;
    #endif
    for( DWORD iInfl = 0; m_bHeirarchy && iInfl < 4; ++iInfl )
    {
        LPD3DXBUFFER pCode;

        // Assemble the vertex shader file
        WCHAR str[MAX_PATH];
        DXUTFindDXSDKMediaFileCch( str, MAX_PATH, m_wszShaderSource[iInfl] );
        V(D3DXAssembleShaderFromFile( str, NULL, NULL, dwShaderFlags, &pCode, NULL ) );

        // Create the vertex shader
        V(pd3dDevice->CreateVertexShader( (DWORD*)pCode->GetBufferPointer(),
                                                         &m_pIndexedVertexShader[iInfl] ) );
        pCode->Release();
    }
}

// VOID CXModelHeirarchy::OnLostDevice()
VOID CXModelHeirarchy::OnLostDevice()
{
    if( m_pEffect )
        m_pEffect->OnLostDevice();

    // Release the vertex shaders
    for( DWORD iInfl = 0; iInfl < 4; ++iInfl )
        SAFE_RELEASE( m_pIndexedVertexShader[iInfl] );
}

// ---------------------------------------------------------------------
// ANIMATION
// ---------------------------------------------------------------------

//! Loads the xfile and adds the animationSet to the current geometries controller. Deletes the new xfile.
UINT CXModelHeirarchy::AddAnimation( LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR strFileName )
{
    assert(m_bHeirarchy);
    HRESULT hr;

    // Load temp xFile
    CXModelHeirarchy tempGeom(pd3dDevice, strFileName, true);

    UINT nNewAnims = tempGeom.GetNumberOfAnimationSets();
    UINT nAnimPos = 0;
    if(!m_pAnimController)
    {
        V( tempGeom.m_pAnimController->CloneAnimationController( 
                                                        tempGeom.m_pAnimController->GetMaxNumAnimationOutputs(),
                                                        tempGeom.m_pAnimController->GetNumAnimationSets(),
                                                        2,
                                                        tempGeom.m_pAnimController->GetMaxNumEvents(),
                                                        &m_pAnimController) );
        return nAnimPos; // animation sets are already registered since we straight up cloned the OTHER AnimController.
        
    }
    else if(m_pAnimController->GetNumAnimationSets() + nNewAnims > m_pAnimController->GetMaxNumAnimationSets())
    {   // we don't have enough room to add the anims so CREATE more in CLONE.
        nAnimPos = m_pAnimController->GetNumAnimationSets();
        LPD3DXANIMATIONCONTROLLER newController = NULL;
		V( m_pAnimController->CloneAnimationController(
			                                            m_pAnimController->GetMaxNumAnimationOutputs(),
			                                            m_pAnimController->GetNumAnimationSets() + nNewAnims,
			                                            2,	// number of required tracks
			                                            m_pAnimController->GetMaxNumEvents(),
			                                            &newController) );

		SAFE_RELEASE(m_pAnimController); // release existing controller.
		m_pAnimController = newController;
    }
    // copy animation sets
    LPD3DXANIMATIONSET pAnimSet;
	for (UINT i=0; i<nNewAnims; ++i)
	{		
        pAnimSet = tempGeom.GetAnimationSet(i);

		if (!pAnimSet) {            
            dprintf("No Animation Set to add when loading: %s\n", strFileName);
			break;
        }
        // Output animation name
        dprintf("Animation %s Added with Period: %lf.\n", pAnimSet->GetName(), pAnimSet->GetPeriod());        
        m_vAnimPeriods.push_back(pAnimSet->GetPeriod());

		// Add it to our controller
		V( m_pAnimController->RegisterAnimationSet(pAnimSet) );
        pAnimSet->Release();
	}
    // tempGeom's destructor is called because it's in stack and gets cleaned up.    
    return nAnimPos;
}

LPD3DXANIMATIONSET CXModelHeirarchy::GetAnimationSet(UINT idx)
{
    assert(m_bHeirarchy);
    LPD3DXANIMATIONSET pAnimSet;
    assert(idx >=0 && idx < m_pAnimController->GetNumAnimationSets());
    m_pAnimController->GetAnimationSet(idx, &pAnimSet);
    return pAnimSet;
}

void CXModelHeirarchy::SetShadowMap( IDirect3DDevice9 *pd3dDevice, const D3DXMATRIX* pShadowMtx, LPDIRECT3DTEXTURE9 pShadowMap )
{
    HRESULT hr;
    assert(pd3dDevice && pShadowMtx && pShadowMap);
    V( m_pEffect->SetMatrix( m_ShadowMtxHandle, pShadowMtx ) );
    V( m_pEffect->SetTexture( m_ShadowMapHandle, pShadowMap ) );
}

//! Draw a Frame in the Heirarchy
void CXModelHeirarchy::DrawFrame( IDirect3DDevice9 *pd3dDevice, LPD3DXFRAME pFrame, LPDIRECT3DTEXTURE9 pTex, const D3DXMATRIX* pMatView )
{
    assert(m_bHeirarchy);
    LPD3DXMESHCONTAINER pMeshContainer;

    pMeshContainer = pFrame->pMeshContainer;
    while (pMeshContainer != NULL)
    {
        DrawMeshContainer( pd3dDevice, pMeshContainer, pFrame, pTex, pMatView );

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        DrawFrame( pd3dDevice, pFrame->pFrameSibling, pTex, pMatView );
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        DrawFrame( pd3dDevice, pFrame->pFrameFirstChild, pTex, pMatView );
    }
}

void CXModelHeirarchy::DrawStandard( LPDIRECT3DDEVICE9 pd3dDevice )
{
    HRESULT hr;
    UINT cPass;
    V( m_pEffect->Begin( &cPass, 0 ) );
    for(size_t i=0; i<m_vpTextures.size(); ++i)
    {
        V( m_pEffect->SetTexture("Texture", m_vpTextures[i]) );
        for( UINT iPass = 0; iPass < cPass; iPass++ )
        {
            V( m_pEffect->BeginPass( iPass ) );            
            V( m_pStandardMesh->DrawSubset( (DWORD)i ) );
            V( m_pEffect->EndPass() );
        }
    }
    V( m_pEffect->End() );
}

void CXModelHeirarchy::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix )
{
    assert(m_bHeirarchy);
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

    if (pParentMatrix != NULL)
        D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
    else
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

    if (pFrame->pFrameSibling != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
    }
}

//--------------------------------------------------------------------------------------
// PRIVATE CXModelHeirarchy //
//
//! CXModelHeirarchy handles all geometries in scene.
//--------------------------------------------------------------------------------------

void CXModelHeirarchy::DrawMeshContainer( IDirect3DDevice9 *pd3dDevice, 
                                  LPD3DXMESHCONTAINER pMeshContainerBase, 
                                  LPD3DXFRAME pFrameBase,
                                  LPDIRECT3DTEXTURE9 pTex,
                                  const D3DXMATRIX* pMatView)
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    UINT iMaterial;
    UINT iAttrib;
    LPD3DXBONECOMBINATION pBoneComb;

    UINT iMatrixIndex;
    UINT iPaletteEntry;
    D3DXMATRIXA16 matTemp;
    D3DCAPS9 d3dCaps;
    pd3dDevice->GetDeviceCaps( &d3dCaps );

    // first check for skinning
    if (pMeshContainer->pSkinInfo != NULL)
    {   
        // Perform HLSL Skinning
        
        //
        // BELOW TAKEN FROM MS DIRECTX SKINNING SAMPLE
        //
        if (pMeshContainer->UseSoftwareVP)
        {
            // If hw or pure hw vertex processing is forced, we can't render the
            // mesh, so just exit out.  Typical applications should create
            // a device with appropriate vertex processing capability for this
            // skinning method.
            if( m_dwBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
                return;

            V( pd3dDevice->SetSoftwareVertexProcessing(TRUE) );
        }

        pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
        for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
        { 
            // Sum of all ambient and emissive contribution
            D3DXCOLOR color1(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient);
            D3DXCOLOR color2(.25, .25, .25, 1.0);
            D3DXCOLOR ambEmm;
            D3DXColorModulate(&ambEmm, &color1, &color2);
            ambEmm += D3DXCOLOR(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Emissive);

            // set material color properties 
            // V( m_pEffect->SetVector("MaterialDiffuse", (D3DXVECTOR4*)&(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse)) );
            // V( m_pEffect->SetVector("MaterialAmbient", (D3DXVECTOR4*)&ambEmm) );

            // Hacked in the ability to cycle texture between characters so each player looks different.
            // setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
            // V( m_pEffect->SetTexture( "Texture", pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] ) );            
            if(pTex)
            {
                V( m_pEffect->SetTexture( "Texture", pTex ) );
            }
            else
            {
                V( m_pEffect->SetTexture( "Texture", pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] ) );            
            }

            // Set CurNumBones to select the correct vertex shader for the number of bones
            V( m_pEffect->SetInt( "CurNumBones", pMeshContainer->NumInfl -1) );

            // calculate all the world matrices
            for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
            {
                iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                if (iMatrixIndex != UINT_MAX)
                {
                    D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
                    // D3DXMatrixMultiply(&pMeshContainer->pBoneMatrices[iPaletteEntry], &matTemp, pMatView); // view transformation done in shader.
                    pMeshContainer->pBoneMatrices[iPaletteEntry] = matTemp; // view transformation done in shader.

                }
            }
            V( m_pEffect->SetMatrixArray( "mWorldMatrixArray", pMeshContainer->pBoneMatrices, pMeshContainer->NumPaletteEntries) );                

            // Start the effect now all parameters have been updated
            UINT numPasses;
            V( m_pEffect->Begin( &numPasses, 0 ) );
            for( UINT iPass = 0; iPass < numPasses; iPass++ )
            {
                V( m_pEffect->BeginPass( iPass ) );

                // draw the subset with the current world matrix palette and material state
                V( pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib ) );

                V( m_pEffect->EndPass() );
            }

            V( m_pEffect->End() );
            V( pd3dDevice->SetVertexShader(NULL) );
        }

        // remember to reset back to hw vertex processing if software was required
        if (pMeshContainer->UseSoftwareVP)
        {
            V( pd3dDevice->SetSoftwareVertexProcessing(FALSE) );
        }        
    }
    else  // standard mesh, just draw it after setting material properties
    {
        assert(!("All standard meshes( no heirarchy ) should be loaded with the NOHEIRARCHY FLAG"));
        V( pd3dDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix) );

        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            V( pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D ) );
            V( pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] ) );
            V( pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial) );
        }
    }
}

// makes the heirarchy and makes paramter the root.
HRESULT CXModelHeirarchy::SetupBoneMatrixPointers( LPD3DXFRAME pFrame, LPD3DXFRAME pRootFrame ) 
{
    assert(m_bHeirarchy);
    HRESULT hr;

    if (pFrame->pMeshContainer != NULL)
    {
        hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer, pRootFrame);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        hr = SetupBoneMatrixPointers(pFrame->pFrameSibling, pRootFrame);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild, pRootFrame);
        if (FAILED(hr))
            return hr;
    }

    return S_OK;
}

// helper
HRESULT CXModelHeirarchy::SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pRootFrame ) 
{
    assert(m_bHeirarchy);
    UINT iBone, cBones;
    D3DXFRAME_DERIVED *pFrame;

    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    // if there is a skinmesh, then setup the bone matrices
    if (pMeshContainer->pSkinInfo != NULL)
    {
        cBones = pMeshContainer->pSkinInfo->GetNumBones();

        pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
        if (pMeshContainer->ppBoneMatrixPtrs == NULL)
            return E_OUTOFMEMORY;

        for (iBone = 0; iBone < cBones; iBone++)
        {
            pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind( pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(iBone) );
            if (pFrame == NULL)
                return E_FAIL;

            pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
        }
    }

    return S_OK;
}

void CXModelHeirarchy::ReleaseAttributeTable( LPD3DXFRAME pFrameBase )
{
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer;

    pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pFrame->pMeshContainer;

    while( pMeshContainer != NULL )
    {
        delete[] pMeshContainer->pAttributeTable;

        pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pMeshContainer->pNextMeshContainer;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        ReleaseAttributeTable(pFrame->pFrameSibling);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        ReleaseAttributeTable(pFrame->pFrameFirstChild);
    }
}


// -----------------------------------------------------------------
// STRUCT STUFF
// -----------------------------------------------------------------

HRESULT D3DXMESHCONTAINER_DERIVED::GenerateSkinnedMesh( IDirect3DDevice9 *pd3dDevice )
{
    HRESULT hr = S_OK;
    D3DCAPS9 d3dCaps;
    pd3dDevice->GetDeviceCaps( &d3dCaps );

    if( this->pSkinInfo == NULL )
        return hr;

    CXModelHeirarchy::s_bUseSoftwareVP = false;

    SAFE_RELEASE( this->MeshData.pMesh );
    SAFE_RELEASE( this->pBoneCombinationBuf );

    
    // Generate skins for hardware skinning

    //
    // BELOW TAKEN FROM MS DIRECTX SKINNING SAMPLE
    //

    // Get palette size
    // First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
    // (96 - 9) /3 i.e. Maximum constant count - used constants 
    UINT MaxMatrices = 26; 
    this->NumPaletteEntries = min(MaxMatrices, this->pSkinInfo->GetNumBones());

    DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
    if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
    {
        this->UseSoftwareVP = false;
        Flags |= D3DXMESH_MANAGED;
    }
    else
    {
        this->UseSoftwareVP = true;
        CXModelHeirarchy::s_bUseSoftwareVP = true;
        Flags |= D3DXMESH_SYSTEMMEM;
    }

    SAFE_RELEASE(this->MeshData.pMesh);

    hr = this->pSkinInfo->ConvertToIndexedBlendedMesh
                                            (
                                            this->pOrigMesh,
                                            Flags, 
                                            this->NumPaletteEntries, 
                                            this->pAdjacency, 
                                            NULL, NULL, NULL,             
                                            &this->NumInfl,
                                            &this->NumAttributeGroups, 
                                            &this->pBoneCombinationBuf, 
                                            &this->MeshData.pMesh);
    if (FAILED(hr))
        goto e_Exit;


    // FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
    DWORD NewFVF = (this->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
    if (NewFVF != this->MeshData.pMesh->GetFVF())
    {
        LPD3DXMESH pMesh;
        hr = this->MeshData.pMesh->CloneMeshFVF(this->MeshData.pMesh->GetOptions(), NewFVF, pd3dDevice, &pMesh);
        if (!FAILED(hr))
        {
            this->MeshData.pMesh->Release();
            this->MeshData.pMesh = pMesh;
            pMesh = NULL;
        }
    }

    D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
    LPD3DVERTEXELEMENT9 pDeclCur;
    hr = this->MeshData.pMesh->GetDeclaration(pDecl);
    if (FAILED(hr))
        goto e_Exit;

    // the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
    //   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
    //          this is more of a "cast" operation
    pDeclCur = pDecl;
    while (pDeclCur->Stream != 0xff)
    {
        if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
            pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
        pDeclCur++;
    }

    hr = this->MeshData.pMesh->UpdateSemantics(pDecl);
    if (FAILED(hr))
        goto e_Exit;

    // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
    if( nBoneMatricesMax < this->pSkinInfo->GetNumBones() )
    {
        nBoneMatricesMax = this->pSkinInfo->GetNumBones();

        // Allocate space for blend matrices
        delete[] pBoneMatrices; 
        pBoneMatrices  = new D3DXMATRIXA16[nBoneMatricesMax];
        if( pBoneMatrices == NULL )
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
    }
e_Exit:
    return hr;
}

///////////////////////////////////// GLOBALS ///////////////////////////////////////

HRESULT FindMeshExtremities( D3DXVECTOR3* pCenterOut, D3DXVECTOR3* pExtremitiesOut, ID3DXMesh* pMesh )
{
    assert( pExtremitiesOut && pCenterOut && pMesh && "Don't send in NULLs dumbass" );
    HRESULT hr;
    BoxVertTexColor* pVertices;
    V_RETURN(pMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pVertices ) );

    memset(pCenterOut, 0, sizeof(D3DXVECTOR3));
    memset(pExtremitiesOut, 0, sizeof(D3DXVECTOR3));

    for(DWORD i=0; i<pMesh->GetNumVertices(); i++)
    {
        *pCenterOut += pVertices[i].pos;
        if(pExtremitiesOut->x < pVertices[i].pos.x) pExtremitiesOut->x = pVertices[i].pos.x;
        if(pExtremitiesOut->y < pVertices[i].pos.y) pExtremitiesOut->y = pVertices[i].pos.y;
        if(pExtremitiesOut->z < pVertices[i].pos.z) pExtremitiesOut->z = pVertices[i].pos.z;
    }
    *pCenterOut /= (float)pMesh->GetNumVertices();
    pExtremitiesOut->x -= pCenterOut->x;
    pExtremitiesOut->y -= pCenterOut->y;
    pExtremitiesOut->z -= pCenterOut->z;

    V_RETURN(pMesh->UnlockVertexBuffer());
    return S_OK;
}

