#include "dxstdafx.h"
#include ".\bubbles.h"
#include "sphere.h"


#define         REFRACT_EFFECT_NAME         L"Effects/Refract.fx"
#define         REFRACT_TECHNIQUE_NAME      "Bubble"


// ------------ STRUCTS ----------------

CBubbles::SBubble::SBubble( const D3DXVECTOR3& v, float fRad, float fHorTile, float fVerTile, LPDIRECT3DTEXTURE9 _pTex, LPDIRECT3DTEXTURE9 _pNormalTex ) 
    : vCenter(v), fRadius(fRad), pTex(_pTex), pNormalTex(_pNormalTex), fRadiusSq(fRad*fRad), fDistSq(0.f)
{
    D3DXMatrixIdentity(&mWorld);
    D3DXMATRIX scale;
    D3DXMatrixScaling(&scale, fRad, fRad, fRad);
    D3DXMatrixTranslation(&mWorld, v.x, v.y, v.z);
    mWorld = scale * mWorld;

    fTiles[0] = fHorTile;
    fTiles[1] = fVerTile;
}


// ----------------------------------------------------------
// PUBLICS
// ----------------------------------------------------------

CBubbles::CBubbles( IDirect3DDevice9* pd3dDevice, int nRings, int nSegments ) :
    m_pSphere(NULL), m_pRefractEffect(NULL)
{
    HRESULT hr;
    assert(nRings > 1 && nSegments > 1);    

    m_pSphere = new CSphere(pd3dDevice, nRings, nSegments, 1.f, 1.f, true, true);

    //
    // Create the shaders, techniques, etc for bubble.
    //

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    V( D3DXCreateEffectFromFile( pd3dDevice, REFRACT_EFFECT_NAME, NULL, NULL, dwShaderFlags, 
                                        NULL, &m_pRefractEffect, NULL ) );

    // Get Handles
    m_RefractTechnique = m_pRefractEffect->GetTechniqueByName( REFRACT_TECHNIQUE_NAME );
    m_WorldViewProjHandle = m_pRefractEffect->GetParameterByName(NULL, "matWorldViewProj");    
    m_WorldViewHandle = m_pRefractEffect->GetParameterByName(NULL, "matWorldView");    
    m_ViewHandle = m_pRefractEffect->GetParameterByName(NULL, "matView");    

    m_LightPosHandle = m_pRefractEffect->GetParameterByName(NULL, "vecLightPos");
    m_LightDirHandle = m_pRefractEffect->GetParameterByName(NULL, "vecLightDir");

    m_TilesHandle = m_pRefractEffect->GetParameterByName(NULL, "fTiles");

    m_NormalMapHandle = m_pRefractEffect->GetParameterByName(NULL, "NormalMap");
    m_TextureHandle = m_pRefractEffect->GetParameterByName(NULL, "Texture");
    
    V( m_pRefractEffect->SetTechnique( m_RefractTechnique ) );
}

CBubbles::~CBubbles(void)
{
    for(size_t i=0;i<m_vBubbles.size();++i) {
        SAFE_RELEASE(m_vBubbles[i].pTex);
        SAFE_RELEASE(m_vBubbles[i].pNormalTex);
    }
    SAFE_RELEASE( m_pRefractEffect );
    SAFE_DELETE( m_pSphere );
}

VOID CBubbles::AddBubble( LPDIRECT3DDEVICE9 pd3dDevice, 
                         const D3DXVECTOR3& pCenter, float fRadius, 
                         float fHorTile, float fVerTile, 
                         LPCSTR texFile, LPCSTR normalTexFile )
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTex, pNormalTex;

    // Create Textures
    V( D3DXCreateTextureFromFileA( pd3dDevice, texFile, &pTex ) );
    V( D3DXCreateTextureFromFileA( pd3dDevice, normalTexFile, &pNormalTex ) );

    m_vBubbles.push_back(SBubble(pCenter, fRadius, fHorTile, fVerTile, pTex, pNormalTex));
}

VOID CBubbles::OnFrameMove(float fElapsedTime, const D3DXVECTOR3* pEyePt)
{   // TODO: Determine which Bubbles are visible inside View Frustum.
    using std::sort;
    for(size_t i=0; i<m_vBubbles.size();++i)
    {
        D3DXVECTOR3 diff = *pEyePt - m_vBubbles[i].vCenter;
        m_vBubbles[i].fDistSq = D3DXVec3LengthSq(&diff);
    }
    // sort bubbles by distance from eye.
    sort(m_vBubbles.begin(), m_vBubbles.end(), SBubbleDistSort());
}

VOID CBubbles::Draw(IDirect3DDevice9* pd3dDevice, 
                    const D3DXVECTOR3* pLightPos,
                    const D3DXVECTOR4* pLightDir,
                    const D3DXMATRIX* pView,
                    const D3DXMATRIX* pViewProj,
                    UCHAR flag)
{
    HRESULT hr;
    UINT cPass;

    // Set StreamSource, RenderStates, effect parameters
    SetupDevice(pd3dDevice, pLightPos, pLightDir, pView);

    V( m_pRefractEffect->Begin(&cPass, 0) ); 
    for(size_t i=0; i<m_vBubbles.size(); ++i)   
    {
        if(flag & DRAWINSIDE) {
            V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW) );
        }
        else if(flag & DRAWOUTSIDE) {
            // Optimization: If we are inside the bubble, no reason to render outside, we can't see it.
            if(m_vBubbles[i].fDistSq < m_vBubbles[i].fRadiusSq)
                continue;
            V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW) );
        }

        //
        // Setup this bubbles properties.
        //
        D3DXMATRIX mtx = m_vBubbles[i].mWorld * (*pViewProj);
        V( m_pRefractEffect->SetMatrix(m_WorldViewProjHandle, &mtx) );
        mtx = m_vBubbles[i].mWorld * (*pView);
        V( m_pRefractEffect->SetMatrix(m_WorldViewHandle, &mtx) );

        V( m_pRefractEffect->SetFloatArray(m_TilesHandle, m_vBubbles[i].fTiles, 2) );                
        V( m_pRefractEffect->SetTexture(m_NormalMapHandle, m_vBubbles[i].pNormalTex) );
        V( m_pRefractEffect->SetTexture(m_TextureHandle, m_vBubbles[i].pTex) );        
    
        for(UINT p=0; p<cPass; p++) 
        {   // Render through all passes
            V( m_pRefractEffect->BeginPass(p) );            
            V( m_pSphere->Draw(pd3dDevice) );
            V( m_pRefractEffect->EndPass() );
        }
    }
    V( m_pRefractEffect->End() );

    RestoreDevice(pd3dDevice);
}

VOID CBubbles::Draw(IDirect3DDevice9* pd3dDevice, 
                    const D3DXVECTOR3* pLightPos,
                    const D3DXVECTOR4* pLightDir,
                    const D3DXMATRIX* pView,
                    const D3DXMATRIX* pViewProj,
                    UCHAR flag,
                    int idxBubble )
{
    HRESULT hr;
    UINT cPass;
    assert(idxBubble < (int)m_vBubbles.size());

    if(flag & DRAWINSIDE) {
        V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW) );
    }
    else if(flag & DRAWOUTSIDE) {
        // Optimization: If we are inside the bubble, no reason to render outside, we can't see it.
        if(m_vBubbles[idxBubble].fDistSq < m_vBubbles[idxBubble].fRadiusSq)
            return ;  // skip if u cant see outside.
        V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW) );
    }

    // Set StreamSource, RenderStates, effect parameters
    SetupDevice(pd3dDevice, pLightPos, pLightDir, pView);

    V( m_pRefractEffect->Begin(&cPass, 0) ); 
    //
    // Setup this bubbles properties.
    //
    D3DXMATRIX mtx = m_vBubbles[idxBubble].mWorld * (*pViewProj);
    V( m_pRefractEffect->SetMatrix(m_WorldViewProjHandle, &mtx) );
    mtx = m_vBubbles[idxBubble].mWorld * (*pView);
    V( m_pRefractEffect->SetMatrix(m_WorldViewHandle, &mtx) );

    V( m_pRefractEffect->SetFloatArray(m_TilesHandle, m_vBubbles[idxBubble].fTiles, 2) );                
    V( m_pRefractEffect->SetTexture(m_NormalMapHandle, m_vBubbles[idxBubble].pNormalTex) );
    V( m_pRefractEffect->SetTexture(m_TextureHandle, m_vBubbles[idxBubble].pTex) );        

    for(UINT p=0; p<cPass; p++) 
    {   // Render through all passes
        V( m_pRefractEffect->BeginPass(p) );            
        V( m_pSphere->Draw(pd3dDevice) );
        V( m_pRefractEffect->EndPass() );
    }
    V( m_pRefractEffect->End() );
    RestoreDevice(pd3dDevice);   
}

VOID CBubbles::OnLostDevice()
{
    if(m_pRefractEffect)
        m_pRefractEffect->OnLostDevice();
}


VOID CBubbles::OnResetDevice(IDirect3DDevice9 *pd3dDevice)
{
    if(m_pRefractEffect)
        m_pRefractEffect->OnResetDevice();
}

// ----------------------------------------------------------
// PRIVATE
// ----------------------------------------------------------

VOID CBubbles::SetupDevice(IDirect3DDevice9* pd3dDevice, 
                           const D3DXVECTOR3* pLightPos,
                           const D3DXVECTOR4* pLightDir,
                           const D3DXMATRIX* pView)
{
    HRESULT hr;

    assert( m_pSphere->IsUsingTangents() );
    V( m_pSphere->SetDevice(pd3dDevice) ); // sets up the streamsource, IB , etc of sphere.
    // V( pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE) );

    //
    // Prepare Effect
    //
    V( m_pRefractEffect->SetVector(m_LightDirHandle, pLightDir) );    
    V( m_pRefractEffect->SetFloatArray(m_LightPosHandle, (const float*)pLightPos, 3) );  
    V( m_pRefractEffect->SetMatrix(m_ViewHandle, pView) );  
}

VOID CBubbles::RestoreDevice(IDirect3DDevice9* pd3dDevice)
{
    HRESULT hr;


    // Restore Render States and TextureStages
    // V( pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE) );


    // V( pd3dDevice->SetVertexDeclaration(NULL) );

    // Turn back on Culling.
    V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW) );
}
