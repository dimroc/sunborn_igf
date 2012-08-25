#include "dxstdafx.h"
#include ".\iimpostorable.h"
#include "gamedefine.h"

#define                         SHADOWMAP_SHADER        "../GPUTerrainProject/Effects/GenericShadowMap.fx"

// ---------------------------------------------------------------------
// PUBLICS
// ---------------------------------------------------------------------

IImpostorable::IImpostorable(int _SurfaceWidth, int _SurfaceHeight) :
    m_PrevDistance(-1.0f), m_bUseImpostor(false), m_bUpdateImpostor(true), 
    m_pDynamicTexture(NULL), m_pTextureSurface(NULL), m_pRenderToSurface(NULL), m_vecPrevWorldUp(0,1,0),
    SurfaceWidth(_SurfaceWidth), SurfaceHeight(_SurfaceHeight), 
    SurfaceCenterVector(SurfaceWidth/2.0f, SurfaceHeight/2.0f, 0.0f),
    m_vecPrevLookAt( 0.f, 0.f, 1.f ),
    m_pShadowMapEffect(NULL)
{   
}

IImpostorable::~IImpostorable(void)
{
    SAFE_RELEASE(m_pRenderToSurface);
    SAFE_RELEASE(m_pDynamicTexture);
    SAFE_RELEASE(m_pTextureSurface);
    SAFE_RELEASE(m_pShadowMapEffect);
}

HRESULT IImpostorable::RenderImpostor(IDirect3DDevice9 *pd3dDevice)
{
    HRESULT hr;

    V( pd3dDevice->SetTexture(0, m_pDynamicTexture) );    
    V( pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertices, sizeof(BoxVertTex)) );

    return S_OK;
}

HRESULT IImpostorable::SetDevice(IDirect3DDevice9 *pd3dDevice)
{   // Remember the impostor m_Center and vertices are already in world space coordinates
    HRESULT hr;

    //
    // Retrieve old states
    //
    V_RETURN( pd3dDevice->GetRenderState( D3DRS_LIGHTING, &s_OldStates[0] ) );    
    V_RETURN( pd3dDevice->GetRenderState( D3DRS_SRCBLEND, &s_OldStates[1] ) );
    V_RETURN( pd3dDevice->GetRenderState( D3DRS_DESTBLEND, &s_OldStates[2] ) );

    V_RETURN( pd3dDevice->GetRenderState( D3DRS_ALPHABLENDENABLE, &s_OldStates[3] ) );    
    V_RETURN( pd3dDevice->GetRenderState( D3DRS_ALPHAFUNC, &s_OldStates[4] ) );
    V_RETURN( pd3dDevice->GetRenderState( D3DRS_CULLMODE, &s_OldStates[5] ) );
    //V_RETURN( pd3dDevice->GetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, &s_OldStates[6]) );


    //
    // Setup new states
    //

    V_RETURN( pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ) );

    // Set What is default anyways
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    V_RETURN( pd3dDevice->SetTransform( D3DTS_WORLD, &identity ) );
    V_RETURN( pd3dDevice->SetFVF(BoxVertTex::FVF) );

    V_RETURN( pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE) );
    V_RETURN( pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER) );    

    // mipmap min etc
    V_RETURN( pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR) );
    V_RETURN( pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
    V_RETURN( pd3dDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0) );    

    // Separate alpha and color blending
    //V_RETURN( pd3dDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE) );

    // Defines Alpha/Color operations with texture and world
    V_RETURN( pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) );
    V_RETURN( pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) );

    //V_RETURN( pd3dDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE) );
    //V_RETURN( pd3dDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA) );

    // Texture blending operations within texture
    V_RETURN( pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ) );
    // V_RETURN( pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ) );
    V_RETURN( pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 ) );

//#if 0
    V_RETURN( pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ) );    
    V_RETURN( pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1) );
//#endif

    // Cull Mode for triangle strip
    V_RETURN( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE) );    

    return S_OK;
}

HRESULT IImpostorable::ClearDevice(IDirect3DDevice9 *pd3dDevice)
{
    HRESULT hr;
    // Reset old states    
    V_RETURN( pd3dDevice->SetRenderState( D3DRS_LIGHTING, s_OldStates[0] ) );
    V_RETURN( pd3dDevice->SetRenderState( D3DRS_SRCBLEND, s_OldStates[1] ) );
    V_RETURN( pd3dDevice->SetRenderState( D3DRS_DESTBLEND, s_OldStates[2] ) );

    V_RETURN( pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, s_OldStates[3]) );
    V_RETURN( pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, s_OldStates[4]) );
    V_RETURN( pd3dDevice->SetRenderState(D3DRS_CULLMODE, s_OldStates[5]) );    

    return S_OK;
}

bool IImpostorable::CheckToUpdateImpostor(bool &bRenderImpostor, const D3DXVECTOR3* pWorldUp, D3DXVECTOR3* pViewpointDif, float fDistanceScale, bool bForce)
{
    if( (m_bUpdateImpostor && m_bUseImpostor) || bForce) { // Refresh impostor on LostDevice() etc.
        m_bUpdateImpostor = false;
        bRenderImpostor = m_bUseImpostor;
        return true;
    }

    float distance = fabs(pViewpointDif->x) + fabs(pViewpointDif->y) + fabs(pViewpointDif->z);
    if(distance < IMPOSTOR_DISTANCE_FACTOR * fDistanceScale) 
    {   // render actual object
        m_bUseImpostor = bRenderImpostor = false;
        return false;            
    }    
    m_bUseImpostor = bRenderImpostor = true;
    float diffUp, diffAt;
    AngleCheck(pWorldUp, &m_vecPrevWorldUp, pViewpointDif, &m_vecPrevLookAt, diffUp, diffAt);
    
    if(diffUp < IMPOSTOR_DELTA_UP_ANGLE_THRESH || diffAt < IMPOSTOR_DELTA_AT_ANGLE_THRESH ||
        fabs(m_PrevDistance - distance) > IMPOSTOR_DELTA_DISTANCE_THRESH)
    {   
        m_PrevDistance = distance;
        m_vecPrevWorldUp = *pWorldUp;
        m_vecPrevLookAt = *pViewpointDif; // already normalized        
        return true;    // Update Impostor
    }
    else    // Render Old Impostor
        return false;
}

HRESULT IImpostorable::OnCreateDevice(IDirect3DDevice9* pd3dDevice)
{
        // shader debugger stuff
    DWORD dwShaderFlags;
    dwShaderFlags = D3DXFX_NOT_CLONEABLE;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    HRESULT hr;
    V_RETURN( D3DXCreateEffectFromFileA(pd3dDevice, SHADOWMAP_SHADER, NULL, NULL, dwShaderFlags, NULL,
                                        &m_pShadowMapEffect, NULL) );
    return hr;
}


HRESULT IImpostorable::OnLostDevice()
{
    if(m_pRenderToSurface)
        m_pRenderToSurface->OnLostDevice();
    SAFE_RELEASE(m_pDynamicTexture);
    SAFE_RELEASE(m_pTextureSurface);

    if(m_pShadowMapEffect)
        m_pShadowMapEffect->OnLostDevice();

    return S_OK;    
}

HRESULT IImpostorable::OnResetDevice(IDirect3DDevice9 *pd3dDevice)
{
    HRESULT hr;

    InitRenderToSurface(pd3dDevice);    
    
	V( D3DXCreateTexture( pd3dDevice, 
		                SurfaceWidth, 
		                SurfaceHeight, 
		                1, 
                        D3DUSAGE_RENDERTARGET, 
						SurfaceFormat, 
						D3DPOOL_DEFAULT, 
						&m_pDynamicTexture ) );
    
    m_pDynamicTexture->GetSurfaceLevel( 0, &m_pTextureSurface );
    m_bUpdateImpostor = true;
    m_bUseImpostor = true;

    // clear the surface alpha to 0 so that it does not bleed into a "blurry" background
    //   this is possible because of the avoidance of blurring in a non-blurred texel
    if(SUCCEEDED(m_pRenderToSurface->BeginScene(m_pTextureSurface, NULL)))
    {
        pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);
        m_pRenderToSurface->EndScene( 0 );
    }

#if 0   // no longer haveing shadows for clouds
    if(m_pShadowMapEffect) {
        m_pShadowMapEffect->OnResetDevice();
        V( m_pShadowMapEffect->SetFloat("fFarPlane", FARPLANE_DISTANCE) );
        V( m_pShadowMapEffect->SetTechnique("Shadow") );
    }
#endif
    return S_OK;    
}


// ---------------------------------------------------------------------
// PRIVATES
// ---------------------------------------------------------------------


//! Dots previous world up/viewpoint vectors with current one to determine movement difference
void IImpostorable::AngleCheck(const D3DXVECTOR3* pWorldUp, D3DXVECTOR3* pPrevWorldUp,
                               D3DXVECTOR3* pViewpointDif, D3DXVECTOR3* pPrevLookAt,
                               float &diffUp, float &diffAt)
{
    D3DXVec3Normalize(pViewpointDif, pViewpointDif);
    
    diffUp = max( 0.f, D3DXVec3Dot(pPrevWorldUp, pWorldUp) );
    diffAt = max( 0.f, D3DXVec3Dot(pPrevLookAt, pViewpointDif) );
}

//! Prepares the projection and view matrices on the device for rendering object to impostor.
VOID IImpostorable::PrepareRenderToImpostor(LPDIRECT3DDEVICE9 pd3dDevice,
                                 const D3DXVECTOR3* m_corners, 
                                 const D3DXVECTOR3* pEyePt, 
                                 const D3DXVECTOR3* pObjPt, 
                                 const D3DXVECTOR3* pWorldUp, 
                                 const D3DVIEWPORT9* pViewport, 
                                 const D3DXMATRIX* pMatProj, 
                                 const D3DXMATRIX* pMatWorld)
{
    HRESULT hr;
    // Use VIEWPOINT NORMAL, NOT lookAt Normal for view matrix (ie: normal of direction from camera to object).
    D3DXMATRIX viewpointView;
    D3DXMatrixLookAtLH(&viewpointView, pEyePt, pObjPt, pWorldUp); 

    GenerateWorldSpaceBBPlaneCoordinates(m_corners, pViewport, pMatProj, &viewpointView, pMatWorld);  

    // Get Matrices        
    D3DXMATRIX newview, newproj;
    D3DXVECTOR3 newcenter;
    GetProjViewMatrices(&newproj, &newview, &newcenter, pEyePt, pWorldUp);        

    //
    // Set Matrices and RENDER TO IMPOSTOR!
    //
    V(pd3dDevice->SetTransform(D3DTS_PROJECTION, &newproj) );
    V(pd3dDevice->SetTransform(D3DTS_VIEW, &newview) );        

    // begin scene
    V( m_pRenderToSurface->BeginScene( m_pTextureSurface, NULL ) );

    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 
                     D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0) );

}

//! Restores device from the function above. Basically just calls m_pRenderToTarget->EndScene().
VOID IImpostorable::RestoreRenderToImpostor()
{
    HRESULT hr;
    V( m_pRenderToSurface->EndScene( 0 ) );
}


ULONG IImpostorable::s_OldStates[10] = {0};

VOID IImpostorable::InitRenderToSurface(IDirect3DDevice9 *d3dDevice)
{   
    if(!m_pRenderToSurface) {
        HRESULT hr;
        V( D3DXCreateRenderToSurface( d3dDevice,
                                SurfaceWidth,
                                SurfaceHeight,
                                SurfaceFormat,
                                true, 
		                        D3DFMT_D16,
		                        &m_pRenderToSurface ) );
    }
    m_pRenderToSurface->OnResetDevice();
}


VOID IImpostorable::GenerateWorldSpaceBBPlaneCoordinates(const D3DXVECTOR3* corners, 
                                         const D3DVIEWPORT9* pViewport, const D3DXMATRIX* pMatProj, 
                                         const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatWorld)
{
    // Project the bounding box vertices to screen space
    int stride = sizeof(D3DXVECTOR3);
    D3DXVECTOR3 screenVerts[8]; // 8 bounding box vertices

    // Create VIEWPOINT view matrix

    D3DXVec3ProjectArray(screenVerts, stride, corners, stride, 
                         pViewport, pMatProj, pMatView, pMatWorld, 8);

    // Had help referencing a gamasutra article on impostors by Ashley Davis
	// Determine the smallest screen-space quad that encompasses the bounding box.

	float minX = screenVerts[0].x;
	float maxX = screenVerts[0].x;
	float minY = screenVerts[0].y;
	float maxY = screenVerts[0].y;
	float minZ = screenVerts[0].z;
	for (int i = 1; i < 8; i++)
	{
		minX = min(screenVerts[i].x, minX);
		maxX = max(screenVerts[i].x, maxX);
		minY = min(screenVerts[i].y, minY);
		maxY = max(screenVerts[i].y, maxY);
		minZ = min(screenVerts[i].z, minZ);
	}

	// Add extra vertices here to compute the near plane and far plane positions.	
	D3DXVECTOR3 screenSpaceVerts[4] =
	{
		D3DXVECTOR3(minX, minY, minZ),
		D3DXVECTOR3(maxX, minY, minZ),
		D3DXVECTOR3(maxX, maxY, minZ),
		D3DXVECTOR3(minX, maxY, minZ)
	};
    	
	// Unproject the screen-space quad into world-space.
    D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	
    //
    // Get World coordinates of a plane encompassing BB.
    //
    // 3 - 2
    // 0 - 1
    D3DXVECTOR3 worldSpaceVerts[4];
	D3DXVec3UnprojectArray(worldSpaceVerts, stride, screenSpaceVerts, stride, 
                           pViewport, pMatProj, pMatView, &identity, 4); 

    // Save coordinates in impostor vertices. Triangle Strip style.
    m_Vertices[0] = BoxVertTex(worldSpaceVerts[0], 0.0f, 0.0f);
    m_Vertices[1] = BoxVertTex(worldSpaceVerts[3], 0.0f, 1.f);
    m_Vertices[2] = BoxVertTex(worldSpaceVerts[1], 1.f, 0.0f);
    m_Vertices[3] = BoxVertTex(worldSpaceVerts[2], 1.f, 1.f);
}

VOID IImpostorable::GetProjViewMatrices(D3DXMATRIX* pproj, D3DXMATRIX* pview, D3DXVECTOR3* pnewcenter,
                    const D3DXVECTOR3* pEyePt, const D3DXVECTOR3* pWorldUp)
{
    m_Center = (m_Vertices[0].pos + m_Vertices[1].pos + m_Vertices[2].pos + m_Vertices[3].pos) * 0.25f;

    D3DXVECTOR3 widthVec = m_Vertices[2].pos - m_Vertices[0].pos;
	D3DXVECTOR3 heightVec = m_Vertices[1].pos - m_Vertices[0].pos;
    float viewWidth = D3DXVec3Length(&widthVec);
	float viewHeight = D3DXVec3Length(&heightVec);

    D3DXVECTOR3 nearPlaneVec = m_Center - (*pEyePt);
	float imposterNearPlane = D3DXVec3Length(&nearPlaneVec);
	//float imposterFarPlane = imposterNearPlane + (boundingRadius * 2.0f);
    float imposterFarPlane = imposterNearPlane + 99999.0f;

    //
    // Create Matrices
    //
    D3DXMatrixLookAtLH(pview, pEyePt, &m_Center, pWorldUp);        
    D3DXMatrixPerspectiveLH(pproj, viewWidth, viewHeight, 
				            imposterNearPlane, imposterFarPlane);
    *pnewcenter = m_Center;
}