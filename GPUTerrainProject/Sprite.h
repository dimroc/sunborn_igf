#pragma once

using std::vector;

/*!
Wraps around D3DXSPRITE with texture info etc.
*/
class CSprite
{
private:
    LPD3DXSPRITE m_pD3DXSprite;
    vector<LPDIRECT3DTEXTURE9> m_vpTex;

    int m_nTex;    
    float m_animThresh; //! time needed to pass until animating.
    
public:
    CSprite(IDirect3DDevice9* pd3dDevice, float animThresh = 0.0f);
    ~CSprite(void);

    VOID AddTexture(IDirect3DDevice9* pd3dDevice, LPCWSTR texFileName);

    HRESULT Begin(DWORD flags );
    HRESULT End();

    HRESULT Draw(
        const UINT texIndex,
        const RECT * pSrcRect,
        const D3DXVECTOR3 * pCenter,
        const D3DXVECTOR3 * pPosition,
        D3DCOLOR Color );

    HRESULT Draw(
        LPDIRECT3DTEXTURE9 pTex,
        const RECT * pSrcRect,
        const D3DXVECTOR3 * pCenter,
        const D3DXVECTOR3 * pPosition,
        D3DCOLOR Color );

    HRESULT Flush() { return m_pD3DXSprite->Flush(); }

    HRESULT SetTransform( const D3DXMATRIX* pTransform );
    HRESULT SetWorldViewLH( const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatWorld );

    HRESULT OnResetDevice();
    HRESULT OnLostDevice();

    int GetNumTextures() { return m_nTex; }    
};
