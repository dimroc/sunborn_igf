#include "dxstdafx.h"
#include ".\modelinstance.h"
#include "gamedefine.h"

#define         TECHNIQUE                   "CelShade"
#define         SHADOWTECHNIQUE             "Shadow"

CModelInstance::CModelInstance( CXModelHeirarchy* pXmodel ) :
    m_pAnimController(NULL), m_pFrameRoot(NULL),
    m_nCurrentAnimationSet(0), m_nCurrentTrack(0), m_fCurrentTime(0.f),    
    m_fShades(9.f), m_fThreshold(0.3f), m_vEdgeColor(0,0,0), m_fMinShade(6.f)
{
    HRESULT hr;
    assert(pXmodel);
    m_pXmodel = pXmodel;

    if(m_pXmodel->IsHeirarchy())
    {
        // Create Frame Heirarchy.
        m_pFrameRoot = CopyFrameHeirarchy( pXmodel->m_pBindPoseFrameRoot );

        // sets up the ppBoneMatrixPtrs for this heirarchy.
        pXmodel->SetupBoneMatrixPointers( m_pFrameRoot, m_pFrameRoot );

        // Create Animation Controller from Original pXModel.
        if( pXmodel->m_pAnimController )
        {
            V( pXmodel->m_pAnimController->CloneAnimationController(
                                pXmodel->m_pAnimController->GetMaxNumAnimationOutputs(),
                                pXmodel->m_pAnimController->GetMaxNumAnimationSets(),
                                pXmodel->m_pAnimController->GetMaxNumTracks(),
                                pXmodel->m_pAnimController->GetMaxNumEvents(),
                                &m_pAnimController) );
            
            // Assign to this model instances heirarchy.
            V( D3DXFrameRegisterNamedMatrices(m_pFrameRoot, m_pAnimController) );
        }  
        m_vfSpeedAdjust.resize(pXmodel->GetNumberOfAnimationSets(), 1.f);
    }

    //setup handles
    m_ShadesHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "fShades" );
    m_MinShadeHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "fMinShade" );    
    m_ThresholdHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "fThreshold" );
    m_EdgeColorHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "vEdgeColor" );

    m_ShadowMtxHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "mShadowMatrix" );
    m_ShadowMapHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "ShadowMap" );

    m_LightDirHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "vecLightDir" );
    m_CameraPosHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "vecCameraPos" );

    m_ViewProjHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "mViewProj" );
    m_WorldHandle = m_pXmodel->m_pEffect->GetParameterByName( 0, "mWorld" );
}

CModelInstance::~CModelInstance(void)
{
    // Cleans up the copied Frame Heirarchy. Lot of cleaning to do.
    if(m_pFrameRoot)
        CleanUpCopyHeirarchy(m_pFrameRoot);
    SAFE_RELEASE(m_pAnimController);
}

VOID CModelInstance::OnFrameMove( float fElapsedTime, const D3DXMATRIX* mWorld )
{
    if(m_pXmodel->IsHeirarchy())
    {
        fElapsedTime *= m_vfSpeedAdjust[m_nCurrentAnimationSet];
        AdvanceAnimController(fElapsedTime, NULL);        
        m_pXmodel->UpdateFrameMatrices( m_pFrameRoot, mWorld );    // Updates frames
    }
    m_fCurrentTime += fElapsedTime;
}

void CModelInstance::OnFrameRender( IDirect3DDevice9 *pd3dDevice,
        const D3DXMATRIX* pMatProj, const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatWorld,
        const D3DXVECTOR3* pEyePt, const D3DXVECTOR4* pLightDir,
        LPDIRECT3DTEXTURE9 pTex)
{
    HRESULT hr;    

    // Setup render state
#if 0
    pd3dDevice->SetRenderState( D3DRS_LIGHTING,         TRUE );
    pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,     TRUE );
    pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );    
    pd3dDevice->SetRenderState( D3DRS_AMBIENT,          0x33333333 );

    // pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    // V( pd3dDevice->SetVertexShaderConstantF(1, (float*)&vLightDir, 1) );
#endif

    V( pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ) ); 
    // take alpha from alpha channel
    V( pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE) );
    V( pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1) );

    V( pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) );
    V( pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) );

    //V( pd3dDevice->SetFVF( m_pFrameRoot->pMeshContainer->MeshData.pMesh->GetFVF() ));    
    
    // Setup Toon / Silhouette stuff
    V( m_pXmodel->m_pEffect->SetFloat( m_ShadesHandle, m_fShades ) );    // number of color shades
    V( m_pXmodel->m_pEffect->SetFloat( m_MinShadeHandle, m_fMinShade ) );    // lowest shade clamp    
    V( m_pXmodel->m_pEffect->SetFloat( m_ThresholdHandle, m_fThreshold ) ); // hard edge color
    V( m_pXmodel->m_pEffect->SetFloatArray( m_EdgeColorHandle, (const float*)&m_vEdgeColor, 3 ) ); // dot product threshold for determining if silhouette.

    // Set Light for vertex shader
    D3DXVECTOR4 vLightDir( -*pLightDir);
    //D3DXVec4Normalize( &vLightDir, &vLightDir );    
    V( m_pXmodel->m_pEffect->SetVector( m_LightDirHandle, &vLightDir) );

    // Set Camera Pos
    V( m_pXmodel->m_pEffect->SetFloatArray( m_CameraPosHandle, (const float*)pEyePt, 3) );

    D3DXMATRIX viewProj = *pMatView * *pMatProj;
    V( m_pXmodel->m_pEffect->SetMatrix( m_ViewProjHandle, &viewProj ) );  

    if(m_pXmodel->IsHeirarchy())
    {
        //! Draw Mesh!    
        m_pXmodel->DrawFrame(pd3dDevice, m_pFrameRoot, pTex, pMatView);
    }
    else
    {
        V( m_pXmodel->m_pEffect->SetMatrix( m_WorldHandle, pMatWorld ) );
        m_pXmodel->DrawStandard(pd3dDevice);
        V( pd3dDevice->SetVertexShader(NULL) );
    }
}

void CModelInstance::SetShadowMap( IDirect3DDevice9 *pd3dDevice, const D3DXMATRIX* pShadowMtx, LPDIRECT3DTEXTURE9 pShadowMap )
{
    m_pXmodel->SetShadowMap( pd3dDevice, pShadowMtx, pShadowMap );
}

void CModelInstance::RenderShadowMap( IDirect3DDevice9 *pd3dDevice, 
                        const D3DXMATRIX* pMatProj, const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatWorld,
                        const D3DXVECTOR3* pLightPos, const D3DXVECTOR4* pLightDir )
{
    HRESULT hr;

    V( m_pXmodel->m_pEffect->SetTechnique( SHADOWTECHNIQUE ) );
    V( m_pXmodel->m_pEffect->SetFloat("fFarPlane", FARPLANE_DISTANCE) );

    OnFrameRender( pd3dDevice, pMatProj, pMatView, pMatWorld, pLightPos, pLightDir, NULL );

    V( m_pXmodel->m_pEffect->SetTechnique( TECHNIQUE ) );
}

void CModelInstance::SelectNextAnimationSet()
{
    if(m_pXmodel->IsHeirarchy())
    {
        ++m_nCurrentAnimationSet;

        if (m_nCurrentAnimationSet >= GetNumberOfAnimationSets())
	        m_nCurrentAnimationSet = 0;

        SetAnimationSet(m_nCurrentAnimationSet);
    }
}

float CModelInstance::PlayAnimationSetOnce(UINT idx)
{
    assert( idx < GetNumberOfAnimationSets() );
    if(m_pXmodel->IsHeirarchy() && idx < GetNumberOfAnimationSets() )
    {
        LPD3DXANIMATIONSET pAnimSet;
        m_pAnimController->GetAnimationSet(idx, &pAnimSet);
        double dAnimTime = pAnimSet->GetPeriod();        
        
        SetAnimationSet(idx);                
        //  replace set animation set and see if still broke
        //m_pAnimController->SetTrackAnimationSet( m_nCurrentTrack, pAnimSet );
        //
        // m_pAnimController->SetTrackPosition(m_nCurrentTrack, 0);
        m_pAnimController->KeyTrackEnable(m_nCurrentTrack, FALSE, m_pAnimController->GetTime() + dAnimTime);

        SAFE_RELEASE(pAnimSet);
        return (float)dAnimTime;
    }
    return 0.f;
}


// Sets new animation by interpolating between the current track and new track.
// MODIFIED FROM: Keith Ditchburn 18/12/2003 CXfileEntity
void CModelInstance::SetAnimationSet(UINT idx)
{
    if( !m_pXmodel->IsHeirarchy() )  return;

    if (idx >= GetNumberOfAnimationSets())
		return;

	m_nCurrentAnimationSet = idx;

	// Get the animation set
	LPD3DXANIMATIONSET set;
	m_pAnimController->GetAnimationSet(m_nCurrentAnimationSet, &set );

	DWORD newTrack = ( m_nCurrentTrack == 0 ? 1 : 0 );

	// Assign to our track
	m_pAnimController->SetTrackAnimationSet( newTrack, set );
    m_pAnimController->SetTrackPosition( newTrack, 0);  // restart the next animation
    set->Release();		

	// Note: for a smooth transition between animation sets we can use two tracks and assign the new set to the track
	// not currently playing then insert Keys into the KeyTrack to do the transition between the tracks
	// tracks can be mixed together so we can gradually change into the new animation

	// Clear any track events currently assigned to our two tracks
	m_pAnimController->UnkeyAllTrackEvents( m_nCurrentTrack );
    m_pAnimController->UnkeyAllTrackEvents( newTrack );

	// Add an event key to disable the currently playing track kMergeTransitionTime seconds in the future
    m_pAnimController->KeyTrackEnable( m_nCurrentTrack, FALSE, m_fCurrentTime + kMergeTransitionTime );
	// Add an event key to change the speed right away so the animation completes in kMergeTransitionTime seconds
    m_pAnimController->KeyTrackSpeed( m_nCurrentTrack, 0.0f, m_fCurrentTime, kMergeTransitionTime, D3DXTRANSITION_LINEAR );
	// Add an event to change the weighting of the current track (the effect it has blended with the secon track)
    m_pAnimController->KeyTrackWeight( m_nCurrentTrack, 0.0f, m_fCurrentTime, kMergeTransitionTime, D3DXTRANSITION_LINEAR );

	// Enable the new track
    m_pAnimController->SetTrackEnable( newTrack, TRUE );
	// Add an event key to set the speed of the track
    m_pAnimController->KeyTrackSpeed( newTrack, 1.0f, m_fCurrentTime, kMergeTransitionTime, D3DXTRANSITION_LINEAR );
	// Add an event to change the weighting of the current track (the effect it has blended with the first track)
	// As you can see this will go from 0 effect to total effect(1.0f) in kMergeTransitionTime seconds and the first track goes from 
	// total to 0.0f in the same time.
    m_pAnimController->KeyTrackWeight( newTrack, 1.0f, m_fCurrentTime, kMergeTransitionTime, D3DXTRANSITION_LINEAR );

    m_nCurrentTrack = newTrack;
}

// ------------------------------------------------------------
// CLEAN UP CODE
// ------------------------------------------------------------

VOID CModelInstance::CleanUpCopyHeirarchy(LPD3DXFRAME pFrame, bool bCopyMeshContainer/*true*/)
{   // Cleans up the copied Frame Heirarchy.
    assert(pFrame);
    
    if(pFrame->pFrameFirstChild)
        CleanUpCopyHeirarchy(pFrame->pFrameFirstChild);
    if(pFrame->pFrameSibling)
        CleanUpCopyHeirarchy(pFrame->pFrameSibling);
    
    if(bCopyMeshContainer)
    {
        D3DXMESHCONTAINER_DERIVED* pMeshDerived = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;
        if(pMeshDerived)
        {
            SAFE_DELETE_ARRAY(pMeshDerived->ppBoneMatrixPtrs);
            SAFE_DELETE(pMeshDerived);
        }
    }
    SAFE_DELETE(pFrame);
}

LPD3DXFRAME CModelInstance::CopyFrameHeirarchy( LPD3DXFRAME pOtherRoot, bool bCopyMeshContainer /*true*/ )
{
    D3DXFRAME_DERIVED* pNewRoot = NULL;
    CopyFrameHelper(&pNewRoot, (D3DXFRAME_DERIVED*)pOtherRoot, bCopyMeshContainer);
    return pNewRoot;
}
/*
typedef struct _D3DXFRAME
{
    LPSTR                   Name;
    D3DXMATRIX              TransformationMatrix;

    LPD3DXMESHCONTAINER     pMeshContainer;

    struct _D3DXFRAME       *pFrameSibling;
    struct _D3DXFRAME       *pFrameFirstChild;
} D3DXFRAME, *LPD3DXFRAME;
*/
VOID CModelInstance::CopyFrameHelper( D3DXFRAME_DERIVED** ppReturn, D3DXFRAME_DERIVED* pOther, bool bCopyMeshContainer)
{
    if(pOther == NULL) {
        *ppReturn = NULL;
        return;
    }

    D3DXFRAME_DERIVED* pNew = new D3DXFRAME_DERIVED(*pOther);
    pNew->pFrameSibling = NULL;
    pNew->pFrameFirstChild = NULL;

    // copy pMeshContainer and have your own. remember it still points to old mesh/stuff.
    if(bCopyMeshContainer && pOther->pMeshContainer) {
        pNew->pMeshContainer = new D3DXMESHCONTAINER_DERIVED(*(D3DXMESHCONTAINER_DERIVED*)pOther->pMeshContainer);
        D3DXMESHCONTAINER_DERIVED* pNewMesh = (D3DXMESHCONTAINER_DERIVED*)pNew->pMeshContainer;
        pNewMesh->ppBoneMatrixPtrs = NULL;
    }
    else
        pNew->pMeshContainer = pOther->pMeshContainer;

    if(pOther->pFrameSibling) {
        CopyFrameHelper( (D3DXFRAME_DERIVED**)&pNew->pFrameSibling, 
                         (D3DXFRAME_DERIVED*)pOther->pFrameSibling, 
                         bCopyMeshContainer);
    }
    if(pOther->pFrameFirstChild) {
        CopyFrameHelper( (D3DXFRAME_DERIVED**)&pNew->pFrameFirstChild, 
                         (D3DXFRAME_DERIVED*)pOther->pFrameFirstChild, 
                         bCopyMeshContainer);
    }

    *ppReturn = pNew;
}



