#pragma once

using std::vector;

class CSphere;


//! Draw Bubble Flags
#define                 DRAWINSIDE                  0x01
#define                 DRAWOUTSIDE                 0x10


class CBubbles
{
private:
    //! Hold Specific Bubble location information
    struct SBubble {
        D3DXMATRIX mWorld;
        D3DXVECTOR3 vCenter;
        float fRadius, fRadiusSq, fDistSq;
        float fTiles[2];
        LPDIRECT3DTEXTURE9 pTex, pNormalTex;

        SBubble() : pTex(NULL), pNormalTex(NULL) {}
        SBubble( const D3DXVECTOR3& v, float fRad, float fHorTile, float fVerTile, LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DTEXTURE9 pNormalTex );
    };
    struct SBubbleDistSort {
        bool operator() (const SBubble& lhs, const SBubble& rhs) const {
            return lhs.fDistSq > rhs.fDistSq;
        }
    };


private:
    CSphere*                        m_pSphere;

    ID3DXEffect*                    m_pRefractEffect; //! D3DX effect interface            

    D3DXHANDLE                      m_RefractTechnique,  //! Cache handles of effect
                                    m_WorldViewProjHandle,
                                    m_WorldViewHandle,
                                    m_ViewHandle,

                                    m_LightDirHandle,
                                    m_LightPosHandle,

                                    m_TilesHandle,
                                    m_NormalMapHandle,
                                    m_TextureHandle;    

    
    //! Hold all the SBubbles in this particular Bubbles. 
    //! Bubbles could need to differ by NormalTex or Shader.
    vector<SBubble>                 m_vBubbles;     
    
private:
    VOID SetupDevice(IDirect3DDevice9* pd3dDevice, 
                       const D3DXVECTOR3* pLightPos,
                       const D3DXVECTOR4* pLightDir,
                       const D3DXMATRIX* pView);
    VOID RestoreDevice(IDirect3DDevice9* pd3dDevice);

public:    
    CBubbles( IDirect3DDevice9* pd3dDevice, int nRings = 15, int nSegments = 15 );
    ~CBubbles();

    //! Add Bubbles to this Bubbles Handler setting.
    VOID AddBubble( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR3& pCenter, float fRadius, float fHorTile, float fVerTile, 
                    LPCSTR texFilename, LPCSTR normalTexFilename );

    //! Assign viewproj matrix to be used in shader. Can enlarge/translate bubble with time.
    VOID OnFrameMove(float fElapsedTime, const D3DXVECTOR3* pEyePt);

    //! Draw All Bubbles in this Bubbles. Bool determines CCW/CW culling 
    VOID Draw( IDirect3DDevice9* pd3dDevice,
               const D3DXVECTOR3* pLightPos,
               const D3DXVECTOR4* pLightDir,
               const D3DXMATRIX* pView,
               const D3DXMATRIX* pViewProj,
               UCHAR flag );

    //! Draw Specific Bubble in this Bubbles given by index. Bool determines CCW/CW culling 
    VOID Draw( IDirect3DDevice9* pd3dDevice,
               const D3DXVECTOR3* pLightPos,
               const D3DXVECTOR4* pLightDir,
               const D3DXMATRIX* pView,
               const D3DXMATRIX* pViewProj,
               UCHAR flag,
               int idxBubble );

    VOID OnLostDevice();
    VOID OnResetDevice( IDirect3DDevice9 *pd3dDevice );

    size_t GetNumBubbles() { return m_vBubbles.size(); }
    float GetBubbleRadius(int idx) { assert(GetNumBubbles() > 0); return m_vBubbles[idx].fRadius; }
    const D3DXVECTOR3* GetBubbleCenter(int idx) { assert(GetNumBubbles() > 0); return &m_vBubbles[idx].vCenter; }
};
