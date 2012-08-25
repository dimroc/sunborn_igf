#include "dxstdafx.h"
#include ".\postproceffects.h"

// ---------------------------- ScreenAligned Quad Functions ------------------------------------

HRESULT ScreenAlignedQuad::OnCreateDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{	// Store Width and Height of screen
    m_Width = pBackBufferSurfaceDesc->Width;
    m_Height = pBackBufferSurfaceDesc->Height;

    // Create an orthogonal view projection matrix for projecting the quad
    D3DXMatrixOrthoLH(&m_mProjection, float(m_Width), float(m_Height), 0.0f, 1.0f);

    return S_OK;
}

HRESULT ScreenAlignedQuad::OnResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{	// Store Width and Height of screen

    m_Width = pBackBufferSurfaceDesc->Width;
    m_Height = pBackBufferSurfaceDesc->Height;
    Init(pd3dDevice, m_Width, m_Height);

    return S_OK;
}

HRESULT ScreenAlignedQuad::OnLostDevice(void)
{
    SAFE_RELEASE(m_pVB);
    return S_OK;
}

HRESULT ScreenAlignedQuad::OnFrameRender(IDirect3DDevice9* pd3dDevice)
{	
    HRESULT hr;

    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);

    DWORD oldFillMode;
    V( pd3dDevice->GetRenderState(D3DRS_FILLMODE, &oldFillMode) );

    V( pd3dDevice->SetRenderState( D3DRS_ZENABLE, false) );
    V( pd3dDevice->SetRenderState( D3DRS_LIGHTING, false) );
    V( pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false ) );
    //V( pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID ));

    // set the transformation matrices to place 
    V( pd3dDevice->SetTransform( D3DTS_VIEW,  &I) );
    V( pd3dDevice->SetTransform( D3DTS_WORLD, &I ) );
    V( pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_mProjection ) );

    // Set the correct vertex format, let it know to use our vertex buffer and draw our screen
    V( pd3dDevice->SetFVF( ScreenVertex_FVF ) );
    V( pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(ScreenVertex) ) );
    V_RETURN( pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2) );

    V( pd3dDevice->SetRenderState(D3DRS_ZENABLE, true) );
    V( pd3dDevice->SetRenderState( D3DRS_LIGHTING, true ) );
    V( pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true ) );

    V( pd3dDevice->SetRenderState( D3DRS_FILLMODE, oldFillMode ));

    return S_OK;
}

void ScreenAlignedQuad::Init(IDirect3DDevice9* pd3dDevice, UINT width, UINT height)
{	
    HRESULT hr;
    // Create Vertex Buffer which holds the screen's corners
    V( pd3dDevice->CreateVertexBuffer(4*sizeof(ScreenVertex), D3DUSAGE_WRITEONLY, ScreenVertex_FVF,
        D3DPOOL_DEFAULT, &m_pVB, NULL) );

    m_Width = width;
    m_Height = height;
    FitToShape();
}

void ScreenAlignedQuad::FitToShape()
{
    HRESULT hr;
    // Create an orthogonal view projection matrix for projecting the quad
    D3DXMatrixOrthoLH(&m_mProjection, float(m_Width), float(m_Height), 0.0f, 1.0f);

    // Place the Screen Corners in m_pVB with appropriate texture co-ordinates
    ScreenVertex *vertices = NULL;
    V( m_pVB->Lock(0, 4*sizeof(ScreenVertex), (void**)&vertices, NULL) );

    vertices[0] = ScreenVertex(-float(m_Width/2.0f), -float(m_Height/2.0f), 0.0f, 0.0f, 1.0f); 
    vertices[1] = ScreenVertex(-float(m_Width/2.0f),  float(m_Height/2.0f), 0.0f, 0.0f, 0.0f); 
    vertices[2] = ScreenVertex( float(m_Width/2.0f), -float(m_Height/2.0f), 0.0f, 1.0f, 1.0f); 
    vertices[3] = ScreenVertex( float(m_Width/2.0f),  float(m_Height/2.0f), 0.0f, 1.0f, 0.0f); 

    V( m_pVB->Unlock() );
}
// -------------------------------------------------------------------------------------------

// ---------------------------- PostProcEffects Functions ------------------------------------

PostProcEffects::PostProcEffects(void)
{
    // set all pointers to null so a lot of the bogus entries dont get SAFE_RELEASED.
    memset(this, 0, sizeof(PostProcEffects));   
    m_fExposureLevel = 2.5f;
    m_bDoBloom = true; m_bEnhance = true;
    m_oldFishSpeed = 0;
    m_fishSpeedInc = 5;
}

PostProcEffects::~PostProcEffects(void)
{	
    OnLostDevice();
}

void PostProcEffects::Bloom(void)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 m_pFullTexture;

    m_pFullTexture = m_pInputTexture;

    PrepareRenderTargets(m_pDownSampleSurface);

    // Setup the effect parameters

    V( m_pDownSample->SetTechnique( "Technique0" ) );	
    setEffectParams();

    V( m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,0.0f), 1.0f, 0 ) );

    // V( m_pd3dDevice->BeginScene() );

    UINT uPasses, uPass;
    V( m_pDownSample->Begin( &uPasses, 0 ) );

    for( uPass = 0; uPass < uPasses; ++uPass )
    {
        V( m_pDownSample->BeginPass( uPass ) );
        V( screen.OnFrameRender(m_pd3dDevice) );
        V( m_pDownSample->EndPass() );
    }

    V( m_pDownSample->End() );
    // V( m_pd3dDevice->EndScene() );

    // Begin the scene and render to the BlurX RT

    V( m_pd3dDevice->SetRenderTarget( 0, m_pBlurXSurface ) );

    // Setup the effect parameters

    V( m_pBlurX->SetTechnique( "Technique0" ) );
    setEffectParams();

    V( m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,0.0f), 1.0f, 0 ) );

    // V( m_pd3dDevice->BeginScene() );
    V( m_pBlurX->Begin( &uPasses, 0 ) );

    for( uPass = 0; uPass < uPasses; ++uPass )
    {
        V( m_pBlurX->BeginPass( uPass ) );
        V( screen.OnFrameRender(m_pd3dDevice) );
        V( m_pBlurX->EndPass() );
    }

    V( m_pBlurX->End() );
    // V( m_pd3dDevice->EndScene() );

    // Begin the scene and render to the BlurY RT

    V( m_pd3dDevice->SetRenderTarget( 0, m_pBlurYSurface ) );	

    // Setup the effect parameters

    V( m_pBlurY->SetTechnique( "Technique0" ) );
    setEffectParams();

    V( m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,0.0f), 1.0f, 0 ) );

    // V( m_pd3dDevice->BeginScene() );
    V( m_pBlurY->Begin( &uPasses, 0 ) );

    for( uPass = 0; uPass < uPasses; ++uPass )
    {
        V( m_pBlurY->BeginPass( uPass ) );		
        V( screen.OnFrameRender( m_pd3dDevice ) );
        V( m_pBlurY->EndPass() );
    }

    V( m_pBlurY->End() );
    // V( m_pd3dDevice->EndScene() );

    // Begin the scene and render to the backbuffer

    V( m_pd3dDevice->SetRenderTarget( 0, m_pRenderToSurface ) );

    // Setup the effect parameters

    V( m_pToneMapping->SetTechnique( "Technique0" ) );
    setEffectParams();

    V( m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,0.0f), 1.0f, 0 ) );

    // V( m_pd3dDevice->BeginScene() );

    V( m_pToneMapping->Begin( &uPasses, 0 ) );

    for( uPass = 0; uPass < uPasses; ++uPass )
    {
        V( m_pToneMapping->BeginPass( uPass ) );
        V( screen.OnFrameRender(m_pd3dDevice) );
        V( m_pToneMapping->EndPass() );
    }

    V( m_pToneMapping->End() );

    // V( m_pd3dDevice->EndScene() );

    RevertRenderTargets();
    SwapInputAndRenderTextures();
}

HRESULT PostProcEffects::OnFrameRender(void)
{
    HRESULT hr;			

    DWORD oldFillMode;
    V( m_pd3dDevice->GetRenderState( D3DRS_FILLMODE, &oldFillMode ) );
    V( m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID ) );

    switch(m_Process)
    {
    case 0:
        V( m_pd3dDevice->SetTexture(0, m_pInputTexture) );
        break;
    case 1:
        V( m_pd3dDevice->SetTexture(0, m_pRenderToTexture) );
        break;		
    case 2:
        V( m_pd3dDevice->SetTexture(0, m_pDownSampleTexture) );
        break;					
    case 3:
        V( m_pd3dDevice->SetTexture(0, m_pBlurXTexture) );
        break;			
    case 4:
        V( m_pd3dDevice->SetTexture(0, m_pBlurYTexture) );
        break;
    }

    // Map final texture to screen	
    V( m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0) );

    V_RETURN( screen.OnFrameRender(m_pd3dDevice) );	
    V( m_pd3dDevice->SetTexture(0, NULL) );

    V( m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, oldFillMode ) );

    return S_OK;
}

HRESULT PostProcEffects::BeginScene()
{	
    HRESULT hr;
    V( m_pd3dDevice->GetRenderTarget( 0, &m_pOldRT ) );	
    V( m_pd3dDevice->GetDepthStencilSurface( &m_pOldDepthBuffer ) );

    V( m_pd3dDevice->SetRenderTarget(0, m_pRenderToSurface) );
    V( m_pd3dDevice->SetDepthStencilSurface( m_pRenderToDepthBuffer ) );

    V( m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE) );
    V( m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE) );	

    return S_OK;
}

HRESULT PostProcEffects::EndScene(DWORD mipFilter)
{	
    HRESULT hr;

    V( m_pd3dDevice->SetDepthStencilSurface( m_pOldDepthBuffer ) );
    V( m_pd3dDevice->SetRenderTarget(0, m_pOldRT) );

    SAFE_RELEASE(m_pOldRT);	
    SAFE_RELEASE(m_pOldDepthBuffer);

    SwapInputAndRenderTextures();

    V( m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE) );
    V( m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE) );

    return S_OK;
}

HRESULT PostProcEffects::OnResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{	
    HRESULT hr;

    UINT width = pBackBufferSurfaceDesc->Width;
    UINT height= pBackBufferSurfaceDesc->Height;

    V_RETURN( pd3dDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
        D3DPOOL_DEFAULT, &m_pInputTexture, NULL ) );

    V_RETURN( pd3dDevice->CreateTexture( width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
        D3DPOOL_DEFAULT, &m_pRenderToTexture, NULL ) );

    V_RETURN( m_pInputTexture->GetSurfaceLevel( 0, &m_pInputSurface ) );		

    V_RETURN( m_pRenderToTexture->GetSurfaceLevel( 0, &m_pRenderToSurface ) );	

    V_RETURN( pd3dDevice->GetDepthStencilSurface( &m_pOldDepthBuffer ) );
    D3DSURFACE_DESC ZDesc;
    m_pOldDepthBuffer->GetDesc(&ZDesc);	
    SAFE_RELEASE( m_pOldDepthBuffer );

    V_RETURN( pd3dDevice->CreateDepthStencilSurface( ZDesc.Width, ZDesc.Height, ZDesc.Format, 
        D3DMULTISAMPLE_NONE, 0, true, &m_pRenderToDepthBuffer, NULL) );    

    V_RETURN( screen.OnResetDevice(pd3dDevice, pBackBufferSurfaceDesc) );

    m_pd3dDevice = pd3dDevice;	

    m_Width = width;
    m_Height = height;

    inv_height = 1/float(height);
    inv_width = 1/float(width);

    initRTS();
    initEffects();

    return S_OK;
}

HRESULT PostProcEffects::OnLostDevice(void)
{
    HRESULT hr;

    SAFE_RELEASE(m_pInputTexture);
    SAFE_RELEASE(m_pRenderToTexture);
    SAFE_RELEASE(m_pInputSurface);
    SAFE_RELEASE(m_pRenderToSurface);
    SAFE_RELEASE(m_pRenderToDepthBuffer);
    SAFE_RELEASE(m_pOldRT);
    SAFE_RELEASE(m_pOldDepthBuffer);

    for(UINT i = 0; i < effects.size(); i ++)
    {
        effects[i]->OnLostDevice();
    }
    V_RETURN(screen.OnLostDevice());		

    shutDown();

    return S_OK;
}

void PostProcEffects::processEffects(float speed, float timeOfDay)
{    
    float maxEdgeThresh = 0.35;
    float minEdgeThresh = 0.25;
    if(m_bEnhance) 
        EdgeEnhance(minEdgeThresh, maxEdgeThresh, timeOfDay);

    FishEye(speed);

    /*if(m_bDoBloom)
    Bloom();*/
    /*if(m_bDoGrey)*/
    //GoGrey();
}

// Bloom stuff

void PostProcEffects::initRTS( void )
{
    //
    // Initialize the render targets
    //

    HRESULT hr;

    V( D3DXCreateTexture( m_pd3dDevice, m_Width/2, m_Height/2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT, &m_pDownSampleTexture ) );
    V( m_pDownSampleTexture->GetSurfaceLevel( 0, &m_pDownSampleSurface ) );

    V( D3DXCreateTexture( m_pd3dDevice, m_Width/2, m_Height/2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT, &m_pBlurXTexture ) );
    V( m_pBlurXTexture->GetSurfaceLevel( 0, &m_pBlurXSurface ) );

    V( D3DXCreateTexture( m_pd3dDevice, m_Width/2, m_Height/2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT, &m_pBlurYTexture ) );
    V( m_pBlurYTexture->GetSurfaceLevel( 0, &m_pBlurYSurface ) );
}

void PostProcEffects::initEffects( void )
{
    //
    // Initialize the effects
    //

    WCHAR str[MAX_PATH];
    DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"../Effects/DownSample.fx");

    LPD3DXBUFFER pBuffer;
    HRESULT hr;

    V( D3DXCreateEffectFromFile( m_pd3dDevice, str, NULL, NULL,
        0, NULL, &m_pDownSample, NULL) );

    V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"../Effects/Blur.fx") );
    V( D3DXCreateEffectFromFile( m_pd3dDevice, str, NULL, NULL,
        0, NULL, &m_pBlurX, NULL ) );

    V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"../Effects/Blur.fx") );
    V( D3DXCreateEffectFromFile( m_pd3dDevice, str, NULL, NULL,
        0, NULL, &m_pBlurY, NULL ) );

    V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"../Effects/ToneMapping.fx") );

    V( D3DXCreateEffectFromFile( m_pd3dDevice, str, NULL, NULL,
        0, NULL, &m_pToneMapping, &pBuffer ) );

    V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"../Effects/KernelOps.fx") );

    V( D3DXCreateEffectFromFile( m_pd3dDevice, str, NULL, NULL,
        0, NULL, &m_pKernelOps, &pBuffer ) );

    V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"../Effects/grey.fx") );

    V( D3DXCreateEffectFromFile( m_pd3dDevice, str, NULL, NULL,
        0, NULL, &m_pGrey, &pBuffer ) );

    V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"../Effects/FishEye.fx") );

    V( D3DXCreateEffectFromFile( m_pd3dDevice, str, NULL, NULL,
        0, NULL, &m_pFishEye, &pBuffer ) );

    V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"../Effects/edges.fx") );

    V( D3DXCreateEffectFromFile( m_pd3dDevice, str, NULL, NULL,
        0, NULL, &m_pEdges, &pBuffer ) );

    // set up all the parameters that only need setting once

    // Bloom effects

    D3DXMATRIX matMVP;
    D3DXMatrixIdentity(&matMVP);
    D3DXMatrixTranspose( &matMVP, &matMVP );

    V( m_pDownSample->SetMatrix( "matMVP", &matMVP ) );
    V( m_pDownSample->SetFloat("viewport_inv_width", inv_width*2) );
    V( m_pDownSample->SetFloat("viewport_inv_height", inv_height*2) ); 
    
    float BOffsetHandle[2] = {inv_width*2, 0}; 
    float POffsetHandle[2] = {inv_width*2, inv_height*2};

    V( m_pBlurX->SetMatrix( "matMVP", &matMVP ) );
    V( m_pBlurX->SetFloatArray("BlurOffset", BOffsetHandle, 2) );
    V( m_pBlurX->SetFloatArray("PixelOffset", POffsetHandle, 2) );

    BOffsetHandle[0] = 0;
    BOffsetHandle[1] = inv_height*2;

    V( m_pBlurY->SetMatrix("matMVP", &matMVP) );
    V( m_pBlurY->SetFloatArray("BlurOffset", BOffsetHandle, 2) );
    V( m_pBlurY->SetFloatArray("PixelOffset", POffsetHandle, 2) );

    V( m_pToneMapping->SetMatrix("matMVP", &matMVP) );
    V( m_pToneMapping->SetFloat("fExposureLevel", m_fExposureLevel) );
    V( m_pToneMapping->SetFloat("viewport_inv_width", inv_width) );
    V( m_pToneMapping->SetFloat("viewport_inv_height", inv_height) );

    // Kernel effect
    float POffset[2] = {inv_width, inv_height};
    V( m_pKernelOps->SetFloatArray("PixelOffset", POffset, 2) );

    // fish eye
    m_pFishEye->SetFloatArray("PixelOffset", POffset, 2);
    m_pFishEye->SetFloat("halfTexWidth", m_Width/2);
    m_pFishEye->SetFloat("halfTexHeight", m_Height/2);
    m_pFishEye->SetFloat("circRad", m_Width/3);    

    // edge enhance/gray
    m_pEdges->SetFloatArray("PixelOffset", POffset, 2);
}

void PostProcEffects::setEffectParams( void )
{
    //
    // Setup the changing effect parameters - for bloom
    //
    HRESULT hr;
    
    V( m_pDownSample->SetTexture( "tDownSample", m_pInputTexture ) );

    V( m_pBlurX->SetTexture( "tBlur", m_pDownSampleTexture ) );
    
    V( m_pBlurY->SetTexture("tBlur", m_pBlurXTexture) );    
    
    V( m_pToneMapping->SetTexture("tFull", m_pInputTexture) );
    V( m_pToneMapping->SetTexture("tBlur", m_pBlurYTexture) );    
}

void PostProcEffects::SetUpKernel( const Matrix3x3& m)
{ 	 
    HRESULT hr;    
    V( m_pKernelOps->SetTexture("texImage", m_pInputTexture) );  
    V( m_pKernelOps->SetFloatArray("Kernel", m.vals, 9) );
}

void PostProcEffects::ExecuteKernel( void )
{
    PrepareRenderTargets(m_pRenderToSurface);

    // Setup the effect parameters
    m_pKernelOps->SetTechnique( "Technique0" );	
    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,0.0f), 1.0f, 0 );

    // m_pd3dDevice->BeginScene();

    UINT uPasses;
    m_pKernelOps->Begin( &uPasses, 0 );

    for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
        m_pKernelOps->BeginPass( uPass );
        screen.OnFrameRender(m_pd3dDevice);
        m_pKernelOps->EndPass();
    }

    m_pKernelOps->End();	
    // m_pd3dDevice->EndScene();

    RevertRenderTargets();	
    SwapInputAndRenderTextures();
}

void PostProcEffects::DoEffect(LPD3DXEFFECT& pEff)
{
    PrepareRenderTargets(m_pRenderToSurface);

    // setup piece  
    pEff->SetTechnique("Technique0");

    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,0.0f), 1.0f, 0 );

    // m_pd3dDevice->BeginScene();

    UINT uPasses;
    pEff->Begin( &uPasses, 0 );

    for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
        pEff->BeginPass( uPass );
        screen.OnFrameRender(m_pd3dDevice);
        pEff->EndPass();
    }

    pEff->End();	
    // m_pd3dDevice->EndScene();

    RevertRenderTargets();	
    SwapInputAndRenderTextures();
}

void PostProcEffects::GoGrey(void)
{
    m_pGrey->SetTexture("image", m_pInputTexture);
    DoEffect(m_pGrey);
}

void PostProcEffects::FishEye(float speed)
{
    // modify speed so it makes a nice curvature for fish eye    
    if(speed < m_oldFishSpeed)
    {
        m_oldFishSpeed -= m_fishSpeedInc;
        if(m_oldFishSpeed < speed)
            m_oldFishSpeed = speed;
    }
    if(speed > m_oldFishSpeed)
    {
        m_oldFishSpeed += m_fishSpeedInc;
        if(m_oldFishSpeed > speed)
            m_oldFishSpeed = speed;
    }  

    float curve = 1 - m_oldFishSpeed/10000.0;

    PrepareRenderTargets(m_pRenderToSurface);

    // setup piece
    m_pFishEye->SetTexture("image", m_pInputTexture);    
    m_pFishEye->SetFloat("curve", curve);

    m_pFishEye->SetTechnique("Technique0");

    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,0.0f), 1.0f, 0 );

    // m_pd3dDevice->BeginScene();

    UINT uPasses;
    m_pFishEye->Begin( &uPasses, 0 );

    for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
        m_pFishEye->BeginPass( uPass );
        screen.OnFrameRender(m_pd3dDevice);
        m_pFishEye->EndPass();
    }

    m_pFishEye->End();	
    // m_pd3dDevice->EndScene();

    RevertRenderTargets();	
    SwapInputAndRenderTextures();
}

void PostProcEffects::EdgeEnhance(float minStr, float maxStr, float timeOfDay)
{
    if(timeOfDay > 18)
        timeOfDay -= 24;
    m_color = 1 - abs(6 - timeOfDay)/12; // 1 at noon, 0 at midnight (note: 6 is noon)

    float exp = 3.0;
    float dist = abs(m_color-0.5)*2;
    dist = pow(dist, 1/exp)/2;
    if(m_color > 0.5)
        m_color = 0.5 + dist;
    else
        m_color = 0.5 - dist;

    float thresh = maxStr;
    
    m_pEdges->SetFloat("thresh", thresh);
    m_pEdges->SetTexture("texImage", m_pInputTexture);    
    m_pEdges->SetFloat("color", m_color);

    DoEffect(m_pEdges);
}

void PostProcEffects::shutDown(void)
{	
    SAFE_RELEASE( m_pQuadBuffer	);
    SAFE_RELEASE( m_pHalfBuffer	);
    SAFE_RELEASE( m_pFullBuffer	);

    SAFE_RELEASE( m_pDownSampleTexture );
    SAFE_RELEASE( m_pDownSampleSurface );
    SAFE_RELEASE( m_pBlurXTexture );
    SAFE_RELEASE( m_pBlurXSurface );
    SAFE_RELEASE( m_pBlurYTexture );
    SAFE_RELEASE( m_pBlurYSurface );
    SAFE_RELEASE( m_pKernelOpsTexture );
    SAFE_RELEASE( m_pKernelOpsSurface );

    SAFE_RELEASE( m_pDownSample );
    SAFE_RELEASE( m_pBlurX );
    SAFE_RELEASE( m_pBlurY );
    SAFE_RELEASE( m_pToneMapping );
    SAFE_RELEASE( m_pKernelOps );
    SAFE_RELEASE( m_pGrey );
    SAFE_RELEASE( m_pFishEye );
    SAFE_RELEASE( m_pEdges );
}

void PostProcEffects::PrepareRenderTargets(LPDIRECT3DSURFACE9 pSurface)
{
    HRESULT hr;

    V( m_pd3dDevice->GetRenderTarget( 0, &m_pOldRT ) );
    V( m_pd3dDevice->GetDepthStencilSurface( &m_pOldDepthBuffer ) );

    V( m_pd3dDevice->GetRenderState( D3DRS_FILLMODE, &m_OldFillMode ) );
    V( m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID ));

    V( m_pd3dDevice->SetRenderTarget( 0, pSurface ) );

    //! used to prevent antialiasing from breaking by having the same multisample format as render targets (none).
    V( m_pd3dDevice->SetDepthStencilSurface( m_pRenderToDepthBuffer ) );    
}

void PostProcEffects::RevertRenderTargets()
{
    HRESULT hr;

    V( m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, m_OldFillMode ));
    V( m_pd3dDevice->SetRenderTarget(0, m_pOldRT) );    
    V( m_pd3dDevice->SetDepthStencilSurface( m_pOldDepthBuffer ) );

    SAFE_RELEASE(m_pOldRT);
    SAFE_RELEASE(m_pOldDepthBuffer);
}

void PostProcEffects::SwapInputAndRenderTextures( void )
{
    LPDIRECT3DTEXTURE9      tmpTex;
    LPDIRECT3DSURFACE9      tmpSurf;

    tmpTex = m_pInputTexture;
    tmpSurf = m_pInputSurface;

    m_pInputTexture = m_pRenderToTexture;
    m_pInputSurface = m_pRenderToSurface;

    m_pRenderToTexture = tmpTex;
    m_pRenderToSurface = tmpSurf;
}

void PostProcEffects::ToggleBloom( void )
{
    m_bDoBloom = !m_bDoBloom;
}

void PostProcEffects::ToggleGrey( void )
{
    m_bDoGrey = !m_bDoGrey;
}

void PostProcEffects::ToggleEnhance()
{
    m_bEnhance = !m_bEnhance;
}