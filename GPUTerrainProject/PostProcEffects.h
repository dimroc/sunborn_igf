#pragma once

#include "GlobalDefine.h"

using std::vector;
using gdef::Matrix3x3;

class ScreenAlignedQuad
{
public:
    ScreenAlignedQuad(void): m_Width(0), m_Height(0), m_pVB(NULL) {};
    ~ScreenAlignedQuad(void) {	SAFE_RELEASE(m_pVB); };

    HRESULT OnCreateDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
    HRESULT OnResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
    HRESULT OnFrameRender(IDirect3DDevice9* pd3dDevice);
    HRESULT OnLostDevice(void);

    void Init(IDirect3DDevice9* pd3dDevice, UINT width, UINT height);

    struct ScreenVertex
    {	float x, y, z;
    float tu, tv;
    ScreenVertex(float _x, float _y, float _z, float _tu, float _tv) {x=_x; y=_y; z=_z; tu=_tu; tv=_tv;}
    ScreenVertex(void) {x=0; y=0; z=0; tu=0; tv=0;}
    };

    const static DWORD ScreenVertex_FVF = D3DFVF_XYZ | D3DFVF_TEX1;

private:
    UINT m_Width, m_Height;
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    D3DXMATRIX m_mProjection;

    void FitToShape();
};

// ------------------------------- Post Proc Effects ---------------------------

class PostProcEffects
{
public:
    PostProcEffects(void);
    ~PostProcEffects(void);  

    HRESULT BeginScene();
    HRESULT EndScene(DWORD mipFilter = 0);

    HRESULT OnResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
    HRESULT OnFrameRender(void);
    HRESULT OnLostDevice(void);	
    void	  ExecuteKernel(void);
    void    SetUpKernel(const Matrix3x3& m);
    void    processEffects(float speed, float timeOfDay);	

    void	ToggleBloom();
    void	ToggleGrey();
    void  ToggleEnhance();

    int     m_Process;

    float getColor(void) {return m_color;};
private:
    LPDIRECT3DTEXTURE9      m_pInputTexture, m_pRenderToTexture;
    LPDIRECT3DSURFACE9      m_pInputSurface, m_pRenderToSurface, m_pRenderToDepthBuffer;
    LPDIRECT3DSURFACE9		  m_pOldRT, m_pOldDepthBuffer;
    ScreenAlignedQuad		    screen;
    LPDIRECT3DDEVICE9		    m_pd3dDevice;
    vector<ID3DXEffect*>    effects;

    // Bloom Stuff

    LPDIRECT3DVERTEXBUFFER9  m_pQuadBuffer; // Quad vertex buffer
    LPDIRECT3DVERTEXBUFFER9  m_pHalfBuffer; // Half-screen quad vertex buffer
    LPDIRECT3DVERTEXBUFFER9  m_pFullBuffer; // Full-screen quad vertex buffer

    LPDIRECT3DTEXTURE9      m_pQuadTexture; // Light probe texture

#define FORMAT64        D3DFMT_A16B16G16R16F    // Indicates 64-bit format
#define FORMAT128       D3DFMT_A32B32G32R32F    // Indicates 128-bit format

    LPDIRECT3DTEXTURE9      m_pDownSampleTexture; // Downsample texture - used
    // to downsample "FullTexture"
    LPDIRECT3DSURFACE9      m_pDownSampleSurface; // Downsample surface
    LPDIRECT3DTEXTURE9      m_pBlurXTexture; // Texture used for horizontal blurring
    LPDIRECT3DSURFACE9      m_pBlurXSurface; // Corresponding surface
    LPDIRECT3DTEXTURE9      m_pBlurYTexture; // Texture used for vertical blurring
    LPDIRECT3DSURFACE9      m_pBlurYSurface; // Corresponding surface
    LPDIRECT3DTEXTURE9		  m_pKernelOpsTexture;
    LPDIRECT3DSURFACE9      m_pKernelOpsSurface;

    LPD3DXEFFECT            m_pDownSample; // Effect for Downsample.fx
    LPD3DXEFFECT            m_pBlurX; // Effect for BlurX.fx
    LPD3DXEFFECT            m_pBlurY; // Effect for BlurY.fx
    LPD3DXEFFECT            m_pToneMapping; // Effect for Tonemapping.fx
    LPD3DXEFFECT            m_pKernelOps;
    LPD3DXEFFECT			      m_pGrey;
    LPD3DXEFFECT            m_pFishEye;
    LPD3DXEFFECT            m_pEdges;

    // TODO: Create handles for the text references.
    // D3DXHANDLE              m_Technique0Handle;

    float m_oldFishSpeed;       // old speed used for the fisheye
    float m_fishSpeedInc;
    float m_color;
    float inv_width, inv_height;

    bool	m_bVaryExposure;
    float	m_fExposureLevel;

    UINT m_Width, m_Height;
    DWORD m_OldFillMode;

    bool m_bDoBloom, m_bDoGrey, m_bDoFish, m_bEnhance;

    //--------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------

    void PrepareRenderTargets( LPDIRECT3DSURFACE9 pSurface );  //! swaps the current render target with pp effects rendertarget.
    void RevertRenderTargets( void );   //! Reverts back to saved prior render target. PREREQ: called after PrepareRenderTargets.

    void SwapInputAndRenderTextures( void );
    void initRTS( void );
    void initEffects( void );
    void setEffectParams( void );
    void shutDown( void );
    void GoGrey(void);
    void FishEye(float speed);
    void EdgeEnhance(float minStr, float maxStr, float timeOfDay);
    void DoEffect(LPD3DXEFFECT& pEff);  
    void Bloom(void);	
};
