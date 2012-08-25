#pragma once

/*! Renders from a defined light source using a new ZBuffer AND a RenderTarget.
    This is done to allow the proper shadow map of sprites which are billboards with varying alphas.
    On a regular Z Buffer the sprite would simply be a boring quad, while now in the RenderTarget we take alpha into account.
*/
class CShadowMap
{
private:
    LPDIRECT3DTEXTURE9                  m_pShadowMapTex;   // where we render the shadowmap (duh)
    LPDIRECT3DSURFACE9                  m_pShadowMapSurface;    //  surface of above.
    
    LPDIRECT3DSURFACE9                  m_pShadowZBufferSurface;

    LPDIRECT3DSURFACE9                  m_pOldRt;       // stores old render target to be restored when EndShadowMap
    LPDIRECT3DSURFACE9                  m_pOldZBuffer;  //! stores old z buffer
    
    D3DXVECTOR3                         m_vLookAt, m_vLightPos, m_vLightUp; //! The light's space.
    D3DXMATRIX                          m_mShadowView;

    // Each object is going to end up having their own shadow map effect though.

public:
    CShadowMap(void);
    ~CShadowMap(void);

    // Renders to shadow map
    HRESULT StartShadowMapRender(LPDIRECT3DDEVICE9 pd3dDevice);
    HRESULT EndShadowMapRender(LPDIRECT3DDEVICE9 pd3dDevice);

    HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice);
    HRESULT OnLostDevice();

    // -------------------------- SETTERS / GETTERS -------------------

    //! Sets the camera to light position. pLightDir must be normalized.
    VOID SetShadowViewMatrix(const D3DXVECTOR3* pLightPos, 
                             const D3DXVECTOR3* pLightDir, 
                             const D3DXVECTOR3* pLightUp, 

                             //! used if caller wants to 'truncate' the view distance to that of a radius from worldCenter
                             const D3DXVECTOR3* pBubbleCenter = NULL,    
                             float fRadius = 0.f);

    D3DXMATRIX* GetShadowViewMatrix() { return &m_mShadowView; }

    //! Helper to return ViewProj matrix of Light/ShadowMap. PREREQ: SetShadowViewMatrix called.
    VOID GetShadowViewProjMatrix(D3DXMATRIX* pOut, const D3DXMATRIX* pProj);  // light projection (same as camera)

    LPDIRECT3DTEXTURE9 GetShadowMap() { return m_pShadowMapTex; }
};
