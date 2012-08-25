#include "dxstdafx.h"
#include "ShadowMap.h"


CShadowMap::CShadowMap(void) :
    m_pShadowMapTex(NULL), m_pShadowMapSurface(NULL), m_pShadowZBufferSurface(NULL)
{
}

CShadowMap::~CShadowMap(void)
{    
}

HRESULT CShadowMap::OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{
    HRESULT hr;

    const D3DSURFACE_DESC* pSurfDesc = DXUTGetBackBufferSurfaceDesc();    

    // Set up render target
    V_RETURN( D3DXCreateTexture( pd3dDevice, pSurfDesc->Width, pSurfDesc->Height, 1, 
        D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowMapTex ) ); 

    V_RETURN( m_pShadowMapTex->GetSurfaceLevel( 0, &m_pShadowMapSurface ) );

	V_RETURN( pd3dDevice->GetDepthStencilSurface( &m_pOldZBuffer ) );
	D3DSURFACE_DESC ZDesc;
	m_pOldZBuffer->GetDesc(&ZDesc);	
	SAFE_RELEASE( m_pOldZBuffer );

    // Set up new ZBuffer
	V_RETURN( pd3dDevice->CreateDepthStencilSurface( ZDesc.Width, ZDesc.Height, ZDesc.Format, 
			D3DMULTISAMPLE_NONE, 0, true, &m_pShadowZBufferSurface, NULL) );    

    return S_OK;
}

HRESULT CShadowMap::OnLostDevice()
{
    SAFE_RELEASE(m_pShadowMapSurface);
    SAFE_RELEASE(m_pShadowMapTex);

    SAFE_RELEASE(m_pShadowZBufferSurface);    

    return S_OK;
}

HRESULT CShadowMap::StartShadowMapRender(LPDIRECT3DDEVICE9 pd3dDevice)
{
    HRESULT hr;

    // get old stuff
    V_RETURN( pd3dDevice->GetRenderTarget( 0, &m_pOldRt ) );
    V_RETURN( pd3dDevice->GetDepthStencilSurface( &m_pOldZBuffer ) );

    // set new stuff
    V_RETURN( pd3dDevice->SetRenderTarget( 0, m_pShadowMapSurface ) );
    V_RETURN( pd3dDevice->SetDepthStencilSurface( m_pShadowZBufferSurface ) );

    V_RETURN( pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW ) );   // cull FRONT faces so if anything is <= the back face its shadow.

    // V_RETURN( pd3dDevice->BeginScene() );

    V_RETURN( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,255,255,255), 1.0f, 0) );

    return S_OK;
}


HRESULT CShadowMap::EndShadowMapRender(LPDIRECT3DDEVICE9 pd3dDevice)
{
    HRESULT hr;

    // V_RETURN( pd3dDevice->EndScene() );

    V_RETURN( pd3dDevice->SetRenderTarget( 0, m_pOldRt ) );  
    V_RETURN( pd3dDevice->SetDepthStencilSurface( m_pOldZBuffer ) );
    SAFE_RELEASE(m_pOldRt);
    SAFE_RELEASE(m_pOldZBuffer);

    V_RETURN( pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ) );    

    return S_OK;
}

//! Sets the camera to light position. pLightDir must be normalized.
VOID CShadowMap::SetShadowViewMatrix(const D3DXVECTOR3* pLightPos, 
                                     const D3DXVECTOR3* pLightDir, 
                                     const D3DXVECTOR3* pLightUp,
                                     const D3DXVECTOR3* pBubbleCenter,
                                     float fRadius)
{
    if(pBubbleCenter == NULL)
    {
        m_vLightPos = *pLightPos;
        m_vLookAt = *pLightPos + *pLightDir;
        m_vLightUp = *pLightUp;

        D3DXMatrixLookAtLH(&m_mShadowView, &m_vLightPos, &m_vLookAt, &m_vLightUp);
    }
    else
    {   // reposition pLightPos to be a distance from bubble center such that
        // the entire bubble can fit in the view frustum given the fov.
        // This is to combat shadowmap antialiasing.
        static float fFov = sin(M_PI_4);
        float fMinDist = fRadius / fFov;

        m_vLightPos = *pLightPos - *pBubbleCenter;
        D3DXVec3Normalize(&m_vLightPos, &m_vLightPos);        
        m_vLightPos *= fMinDist;
        m_vLightPos += *pBubbleCenter;
        m_vLookAt = *pBubbleCenter;
        m_vLightUp = *pLightUp;

        D3DXMatrixLookAtLH(&m_mShadowView, &m_vLightPos, &m_vLookAt, &m_vLightUp);
    }
}

//! Returns the transformation matrix to go from camera view space to light clip space.
VOID CShadowMap::GetShadowViewProjMatrix(D3DXMATRIX* pOut, const D3DXMATRIX* pProj)  // light projection (same as camera)
{
    *pOut = m_mShadowView * *pProj; // row major order ( camera-1 * lview * lproj )
    // *pOut = *pOut * *pLightView * *pLightProj; // row major order ( camera-1 * lview * lproj )
    // *pOut = *pLightProj * *pLightView * *pOut; // column major order
}