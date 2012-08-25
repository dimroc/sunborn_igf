#include "dxstdafx.h"
#include "sprite.h"
#include "fxdecl.h"

CSprite::CSprite(IDirect3DDevice9* pd3dDevice, float animThresh) :    
    m_animThresh(animThresh), m_nTex(0),
    m_pD3DXSprite(NULL)
{
    HRESULT hr;
    V( D3DXCreateSprite( pd3dDevice, &m_pD3DXSprite ) );
}

CSprite::~CSprite(void)
{
    SAFE_RELEASE(m_pD3DXSprite);
    for(size_t i=0; i<m_vpTex.size(); i++)
        SAFE_RELEASE(m_vpTex[i]);
}

VOID CSprite::AddTexture(IDirect3DDevice9* pd3dDevice, LPCWSTR texFileName)
{
    HRESULT hr;
    
    LPDIRECT3DTEXTURE9 pTex; // Create texture.
    V(D3DXCreateTextureFromFile(pd3dDevice, texFileName, &pTex) );

    m_vpTex.push_back(pTex);
    m_nTex++;
}

HRESULT CSprite::Begin(DWORD flags)
{
    return m_pD3DXSprite->Begin(flags);
}

HRESULT CSprite::End()
{
    return m_pD3DXSprite->End();
}

HRESULT CSprite::Draw(
        const UINT texIndex,
        const RECT * pSrcRect,
        const D3DXVECTOR3 * pCenter,
        const D3DXVECTOR3 * pPosition,
        D3DCOLOR Color )
{
    return m_pD3DXSprite->Draw(m_vpTex[texIndex], pSrcRect, pCenter, pPosition, Color);
}

HRESULT CSprite::Draw(
        LPDIRECT3DTEXTURE9 pTex,
        const RECT * pSrcRect,
        const D3DXVECTOR3 * pCenter,
        const D3DXVECTOR3 * pPosition,
        D3DCOLOR Color )
{
    return m_pD3DXSprite->Draw(pTex, pSrcRect, pCenter, pPosition, Color);
}

HRESULT CSprite::SetTransform(CONST D3DXMATRIX *pTransform)
{
    return m_pD3DXSprite->SetTransform(pTransform);
}

HRESULT CSprite::SetWorldViewLH( CONST D3DXMATRIX* pMatView, CONST D3DXMATRIX* pMatWorld )
{
    return m_pD3DXSprite->SetWorldViewLH(pMatView, pMatWorld);
}

HRESULT CSprite::OnLostDevice()
{
    HRESULT hr;
    V_RETURN( m_pD3DXSprite->OnLostDevice() );
    return S_OK;    
}

HRESULT CSprite::OnResetDevice()
{
    HRESULT hr;
    V_RETURN( m_pD3DXSprite->OnResetDevice() );
    return S_OK;
}
