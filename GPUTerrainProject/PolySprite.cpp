#include "dxstdafx.h"
#include ".\polysprite.h"

CPolySprite::CPolySprite(IDirect3DDevice9 *pd3dDevice) : 
    m_pVB(NULL), m_Flags(0), m_pd3dDevice(pd3dDevice), 
    m_bSetCalled(false),
    // For WORLD ORIENTED BILLBOARDS
    m_vecDefaultWorldUp(0.f,1.f,0.f),
    m_vecDefaultWorldRight(1.f,0.f,0.f),
    m_vecDefaultWorldAhead(0.f,0.f,1.f)
{   
    memset(&m_OldStates, 0, sizeof(ULONG)*10);
    m_vToDraw.reserve(NUMBER_OF_TODRAW_RESERVE_SPOTS);

    D3DXMatrixIdentity(&m_mtxIdentity);
    m_fullRectf.left = 0.f; m_fullRectf.right = 1.f; m_fullRectf.top = 0.f; m_fullRectf.bottom = 1.f;    

    // Make the quad coordiantes
    // 1 2
    // 0 3
    m_vQuadCoordinates.push_back(D3DXVECTOR3(-1.f, -1.f, 0.f)); // 0
    m_vQuadCoordinates.push_back(D3DXVECTOR3(-1.f, 1.f, 0.f)); // 1 
    m_vQuadCoordinates.push_back(D3DXVECTOR3(1.f, 1.f, 0.f)); // 2 
    m_vQuadCoordinates.push_back(D3DXVECTOR3(1.f, -1.f, 0.f)); // 3
    // vector4 with w = 1 because they are points and needed for translations

    m_vScreenAlignedCoordinates.resize(4);
}

CPolySprite::~CPolySprite(void)
{    
    SAFE_RELEASE(m_pVB);
}

//! Sets renderstates for the batch drawing of textures.
VOID CPolySprite::Begin(UINT flags)
{   
    HRESULT hr;
    m_Flags = flags;    
    
    V( m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_mtxIdentity) );   
    V( m_pd3dDevice->SetFVF(PolySpriteVertex::FVF) );

    // Retrieve old states
    V( m_pd3dDevice->GetRenderState( D3DRS_LIGHTING, &m_OldStates[0] ) );    
    V( m_pd3dDevice->GetRenderState( D3DRS_SRCBLEND, &m_OldStates[1] ) );
    V( m_pd3dDevice->GetRenderState( D3DRS_DESTBLEND, &m_OldStates[2] ) );    

    V( m_pd3dDevice->GetRenderState( D3DRS_CULLMODE, &m_OldStates[3] ) );  
    V( m_pd3dDevice->GetRenderState( D3DRS_LIGHTING, &m_OldStates[4] ) );  
    V( m_pd3dDevice->GetRenderState( D3DRS_ALPHABLENDENABLE, &m_OldStates[5] ) );  
    V( m_pd3dDevice->GetRenderState( D3DRS_ALPHAFUNC, &m_OldStates[6] ) );  

    // Setup new states
    V( m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ) );
    V( m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ) );
    V( m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE) );
    V( m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER) );    

    // mipmap min etc
    V( m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR) );
    V( m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
    V( m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0) );    

    // Separate alpha and color blending
    //V_RETURN( pd3dDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE) );

    // Defines Alpha/Color operations with texture and world
    V( m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) );
    V( m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) );    

    // Texture blending operations within texture
    V( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ) );
    V( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ) );
    V( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE ) );

#if 0
    V( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ) );    
    V( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1) );
#endif
}

//! Add entry to draw batch. Must be called inside BeginDrawBatch-EndDrawBatch.
VOID CPolySprite::Draw( LPDIRECT3DTEXTURE9 pTex, 
                        const RECTF *pRectf /*rect on tex*/, 
                        const D3DXMATRIX *pMtx,
                        bool bWorldSpace,
                        UINT color)
{
    assert(pMtx); // make sure retards dont send in a null mtx.    
    gdef::RECTF rectf;
    if(pRectf)
        rectf = *pRectf;        
    else
        rectf = m_fullRectf;        
    DrawInfo di(pTex, rectf, *pMtx, color);
    di.bWorldSpace = bWorldSpace;
    di.bUsingVerts = false;
    m_vToDraw.push_back(di);
}

VOID CPolySprite::Draw( LPDIRECT3DTEXTURE9 pTex, 
           PolySpriteVertex verts[6],
           const D3DXVECTOR3 *pCenter,
           bool bWorldSpace)
{
    DrawInfo di;
    di.pTex = pTex;
    di.bUsingVerts = true;
    di.bWorldSpace = bWorldSpace;
    
    memset(&di.vCenter, 0, sizeof(D3DXVECTOR3));
    for(int i=0; i<6; i++) {
        di.verts[i] = verts[i];
        di.vCenter += verts[i].pos;
    }
    di.vCenter /= 6;
    m_vToDraw.push_back(di);
}

//! Sends current batch to video card.
VOID CPolySprite::Flush()
{
    PopulateVB();
    DrawSprites();
    m_vToDraw.clear();
    m_vNumTillTexSwap.clear();
}
//! restors render states, etc.
VOID CPolySprite::End()
{
    HRESULT hr;

    Flush();

    // Restore states
    V( m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, m_OldStates[0] ) );    
    V( m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, m_OldStates[1] ) );
    V( m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, m_OldStates[2] ) );    

    V( m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, m_OldStates[3] ) );  
    V( m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, m_OldStates[4] ) );  
    V( m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, m_OldStates[5] ) );  
    V( m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, m_OldStates[6] ) );  
}

//! Sets the left handed world view matrix for call batch. Used to sort/billboard sprites.
VOID CPolySprite::SetCameraSpace(const D3DXVECTOR3 *pCameraPos,
                                const D3DXVECTOR3 *pCameraUp,
                                const D3DXVECTOR3 *pCameraRight,
                                const D3DXVECTOR3 *pCameraAhead)
{
    m_vecCameraAhead = *pCameraAhead;
    m_vecCameraUp = *pCameraUp;
    m_vecCameraRight = *pCameraRight;
    m_vecCameraPos = *pCameraPos;
    m_bSetCalled = true;
}

VOID CPolySprite::OnLostDevice()
{
    SAFE_RELEASE(m_pVB);
}

VOID CPolySprite::OnResetDevice()
{
    HRESULT hr;
    // Create the largest VB possible! Assertions are everywhere to prevent overflowing VB.
    V( m_pd3dDevice->CreateVertexBuffer( 
                                        MAX_VB_SIZE, 
                                        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
                                        PolySpriteVertex::FVF, 
                                        D3DPOOL_DEFAULT, 
                                        &m_pVB, 
                                        NULL) );
}

/////////////////////////// PRIVATE STUFF ////////////////////////////////////

VOID CPolySprite::PopulateVB() {
    HRESULT hr;    

    if(SORTTEXTURE & m_Flags)
        sort(m_vToDraw.begin(), m_vToDraw.end(), TextureSortFunctor());    
    else if(BACKTOFRONT & m_Flags)     // lookAt normal should be normalized
        sort(m_vToDraw.begin(), m_vToDraw.end(), DepthSortFunctor(m_vecCameraAhead, m_vecCameraPos, true) );
    else if(FRONTTOBACK & m_Flags)
        sort(m_vToDraw.begin(), m_vToDraw.end(), DepthSortFunctor(m_vecCameraAhead, m_vecCameraPos, false) );

    if(m_Flags & WORLDORIENTUP)
        CalcWorldOrientUpTransformation();
    else if(m_Flags & WORLDORIENTRIGHT)
        CalcWorldOrientRightTransformation();
    else if(m_Flags & WORLDORIENTAHEAD)
        CalcWorldOrientAheadTransformation();

    // Only multiple flags you can set is Billboard with anything else
    assert( !(SORTTEXTURE & m_Flags && BACKTOFRONT & m_Flags) );
    assert( !(SORTTEXTURE & m_Flags && FRONTTOBACK & m_Flags) );
    assert( !(BACKTOFRONT & m_Flags && FRONTTOBACK & m_Flags) );    

    LPDIRECT3DTEXTURE9 prevTex = NULL;
    int count = 0;

    int size  = (int)(m_vToDraw.size() * 6 * sizeof(PolySpriteVertex));
    assert(size < MAX_VB_SIZE); // have space to lock in our VB. meh, I dunno if that's our real max size.

    // Lock the buffer to gain access to the vertices 
    PolySpriteVertex* pVertices;
    V( m_pVB->Lock(0, 0, (VOID**)&pVertices, D3DLOCK_DISCARD ) );

    vector<DrawInfo>::iterator it = m_vToDraw.begin();
    const vector<DrawInfo>::const_iterator itend = m_vToDraw.end();
    for( ; it != itend; ++it) 
    {   // Check if texture changed
        if(it->pTex != prevTex)
        {
            m_vNumTillTexSwap.push_back(count); // first entry is a bogus 0 entry
            prevTex = it->pTex;
        }

        if(it->bUsingVerts)
        {   // copy verts straight
            for(int i=0; i<6; i++)
                pVertices[count*6 + i] = it->verts[i];
        }
        else
            CreateSprite(&pVertices[count*6], it->mtx, it->rectf, it->color);
        ++count;
    }    
    m_vNumTillTexSwap.push_back(count);

    V( m_pVB->Unlock() );
}

//! PREREQ: Needs allocated 6 spots in vb for trilist.
//! Creates all the vertices in World Space in the Vertex Buffer.
VOID CPolySprite::CreateSprite(PolySpriteVertex *pvb, 
                               const D3DXMATRIX &mtx, 
                               const RECTF &rectf,
                               ULONG color)
{
    D3DXVECTOR3 *pVec0 = &m_vQuadCoordinates[0];
    D3DXVECTOR3 *pVec1 = &m_vQuadCoordinates[1];
    D3DXVECTOR3 *pVec2 = &m_vQuadCoordinates[2];
    D3DXVECTOR3 *pVec3 = &m_vQuadCoordinates[3];

    // Create Vertices for this sprite using its world mtx.
    // 1 2
    // 0 3      Remember this is a trilist. redundant ones at bottom of function.

    // Break up mtx into column vectors.
    D3DXVECTOR3 col1(mtx(0,0), mtx(1,0), mtx(2,0));
    D3DXVECTOR3 col2(mtx(0,1), mtx(1,1), mtx(2,1));
    D3DXVECTOR3 col3(mtx(0,2), mtx(1,2), mtx(2,2));
    D3DXVECTOR3 center(mtx(3,0), mtx(3,1), mtx(3,2));

    pvb[0].pos.x = D3DXVec3Dot(pVec0, &col1);
    pvb[0].pos.y = D3DXVec3Dot(pVec0, &col2);
    pvb[0].pos.z = D3DXVec3Dot(pVec0, &col3);

    // Coordinates are negated cooresponding to original QuadCoordinates (1's, -1's).
    pvb[1].pos.x = D3DXVec3Dot(pVec1, &col1);
    pvb[1].pos.y = D3DXVec3Dot(pVec1, &col2);
    pvb[1].pos.z = D3DXVec3Dot(pVec1, &col3);

    pvb[2].pos.x = D3DXVec3Dot(pVec2, &col1);
    pvb[2].pos.y = D3DXVec3Dot(pVec2, &col2);
    pvb[2].pos.z = D3DXVec3Dot(pVec2, &col3);

    pvb[5].pos.x = D3DXVec3Dot(pVec3, &col1);
    pvb[5].pos.y = D3DXVec3Dot(pVec3, &col2);
    pvb[5].pos.z = D3DXVec3Dot(pVec3, &col3);

    
    if(m_Flags & SCREENALIGN_BILLBOARD)
        ScreenAlignQuad(pvb);
    else if(m_Flags & VIEWPOINTALIGN_BILLBOARD)
        ViewPointAlignQuad(pvb, center); // Need center to get normal from camera pos
    else if(m_Flags & WORLDORIENT)
        WorldOrientAlignQuad(pvb);

    // Add Translation
    pvb[0].pos.x += center.x; pvb[1].pos.x += center.x; pvb[2].pos.x += center.x; pvb[5].pos.x += center.x;
    pvb[0].pos.y += center.y; pvb[1].pos.y += center.y; pvb[2].pos.y += center.y; pvb[5].pos.y += center.y;
    pvb[0].pos.z += center.z; pvb[1].pos.z += center.z; pvb[2].pos.z += center.z; pvb[5].pos.z += center.z;    

    // Assign texture coordiantes
    pvb[0].u = rectf.left; pvb[0].v = rectf.bottom;
    pvb[1].u = rectf.left; pvb[1].v = rectf.top;
    pvb[2].u = rectf.right; pvb[2].v = rectf.top;
    pvb[5].u = rectf.right; pvb[5].v = rectf.bottom;

    // Assign color
    pvb[0].color = pvb[1].color = pvb[2].color = pvb[5].color = color;

    // redundant copies for trilist
    pvb[3] = pvb[0];
    pvb[4] = pvb[2]; 
}

VOID CPolySprite::DrawSprites() 
{
    HRESULT hr;
    
    V( m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(PolySpriteVertex)) );

    // Goes through assigning textures and then drawing the n primitives till next texture
    // given in m_NumTillTexSwap[i].
    for(size_t i=0; i<m_vNumTillTexSwap.size() - 1; i++)
    {
        int pos = m_vNumTillTexSwap[i];
        int end = m_vNumTillTexSwap[i+1];
        V( m_pd3dDevice->SetTexture(0, m_vToDraw[pos].pTex) );

        V( m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,
                                        pos*6, // 6 vertices in trilist quad
                                        (end-pos)*2) ); // 2 triangle primitives in each quad
    }
}

/////////////////////////// BILLBOARDING TECHNIQUES ////////////////////////////////////

VOID CPolySprite::ViewPointAlignQuad(PolySpriteVertex *pvb, const D3DXVECTOR3 &center)
{
    assert(m_bSetCalled);
    //
    // Calculate VIEWPOINT Rotation Matrix
    //
    m_vecCalcWorldAhead = center - m_vecCameraPos;
    D3DXVec3Normalize(&m_vecCalcWorldAhead, &m_vecCalcWorldAhead);

    // Create tranformation axii when up vector is world vector instead of camera up.
    D3DXVec3Cross(&m_vecCalcWorldRight, &m_vecCameraUp, &m_vecCalcWorldAhead);
    D3DXVec3Normalize(&m_vecCalcWorldRight, &m_vecCalcWorldRight);

    D3DXVec3Cross(&m_vecCalcWorldUp, &m_vecCalcWorldAhead, &m_vecCalcWorldRight);
    D3DXVec3Normalize(&m_vecCalcWorldUp, &m_vecCalcWorldUp);

    // Place quad vertices through viewpoint rotation matrix. Kind of a hack here.
    WorldOrientAlignQuad(pvb);
}

VOID CPolySprite::ScreenAlignQuad(PolySpriteVertex *pvb)
{   
    assert(m_bSetCalled);
    D3DXVECTOR3 vec;
    vec.x = pvb[0].pos.x*m_vecCameraRight.x + pvb[0].pos.y*m_vecCameraUp.x + pvb[0].pos.z*m_vecCameraAhead.x;
    vec.y = pvb[0].pos.x*m_vecCameraRight.y + pvb[0].pos.y*m_vecCameraUp.y + pvb[0].pos.z*m_vecCameraAhead.y;
    vec.z = pvb[0].pos.x*m_vecCameraRight.z + pvb[0].pos.y*m_vecCameraUp.z + pvb[0].pos.z*m_vecCameraAhead.z;
    pvb[0].pos = vec;

    vec.x = pvb[1].pos.x*m_vecCameraRight.x + pvb[1].pos.y*m_vecCameraUp.x + pvb[1].pos.z*m_vecCameraAhead.x;
    vec.y = pvb[1].pos.x*m_vecCameraRight.y + pvb[1].pos.y*m_vecCameraUp.y + pvb[1].pos.z*m_vecCameraAhead.y;
    vec.z = pvb[1].pos.x*m_vecCameraRight.z + pvb[1].pos.y*m_vecCameraUp.z + pvb[1].pos.z*m_vecCameraAhead.z;
    pvb[1].pos = vec;

    vec.x = pvb[2].pos.x*m_vecCameraRight.x + pvb[2].pos.y*m_vecCameraUp.x + pvb[2].pos.z*m_vecCameraAhead.x;
    vec.y = pvb[2].pos.x*m_vecCameraRight.y + pvb[2].pos.y*m_vecCameraUp.y + pvb[2].pos.z*m_vecCameraAhead.y;
    vec.z = pvb[2].pos.x*m_vecCameraRight.z + pvb[2].pos.y*m_vecCameraUp.z + pvb[2].pos.z*m_vecCameraAhead.z;
    pvb[2].pos = vec;

    vec.x = pvb[5].pos.x*m_vecCameraRight.x + pvb[5].pos.y*m_vecCameraUp.x + pvb[5].pos.z*m_vecCameraAhead.x;
    vec.y = pvb[5].pos.x*m_vecCameraRight.y + pvb[5].pos.y*m_vecCameraUp.y + pvb[5].pos.z*m_vecCameraAhead.y;
    vec.z = pvb[5].pos.x*m_vecCameraRight.z + pvb[5].pos.y*m_vecCameraUp.z + pvb[5].pos.z*m_vecCameraAhead.z;
    pvb[5].pos = vec;
}

VOID CPolySprite::WorldOrientAlignQuad(PolySpriteVertex *pvb)
{
    assert(m_bSetCalled);
    D3DXVECTOR3 vec;
    vec.x = pvb[0].pos.x*m_vecCalcWorldRight.x + pvb[0].pos.y*m_vecCalcWorldUp.x + pvb[0].pos.z*m_vecCalcWorldAhead.x;
    vec.y = pvb[0].pos.x*m_vecCalcWorldRight.y + pvb[0].pos.y*m_vecCalcWorldUp.y + pvb[0].pos.z*m_vecCalcWorldAhead.y;
    vec.z = pvb[0].pos.x*m_vecCalcWorldRight.z + pvb[0].pos.y*m_vecCalcWorldUp.z + pvb[0].pos.z*m_vecCalcWorldAhead.z;
    pvb[0].pos = vec;

    vec.x = pvb[1].pos.x*m_vecCalcWorldRight.x + pvb[1].pos.y*m_vecCalcWorldUp.x + pvb[1].pos.z*m_vecCalcWorldAhead.x;
    vec.y = pvb[1].pos.x*m_vecCalcWorldRight.y + pvb[1].pos.y*m_vecCalcWorldUp.y + pvb[1].pos.z*m_vecCalcWorldAhead.y;
    vec.z = pvb[1].pos.x*m_vecCalcWorldRight.z + pvb[1].pos.y*m_vecCalcWorldUp.z + pvb[1].pos.z*m_vecCalcWorldAhead.z;
    pvb[1].pos = vec;

    vec.x = pvb[2].pos.x*m_vecCalcWorldRight.x + pvb[2].pos.y*m_vecCalcWorldUp.x + pvb[2].pos.z*m_vecCalcWorldAhead.x;
    vec.y = pvb[2].pos.x*m_vecCalcWorldRight.y + pvb[2].pos.y*m_vecCalcWorldUp.y + pvb[2].pos.z*m_vecCalcWorldAhead.y;
    vec.z = pvb[2].pos.x*m_vecCalcWorldRight.z + pvb[2].pos.y*m_vecCalcWorldUp.z + pvb[2].pos.z*m_vecCalcWorldAhead.z;
    pvb[2].pos = vec;

    vec.x = pvb[5].pos.x*m_vecCalcWorldRight.x + pvb[5].pos.y*m_vecCalcWorldUp.x + pvb[5].pos.z*m_vecCalcWorldAhead.x;
    vec.y = pvb[5].pos.x*m_vecCalcWorldRight.y + pvb[5].pos.y*m_vecCalcWorldUp.y + pvb[5].pos.z*m_vecCalcWorldAhead.y;
    vec.z = pvb[5].pos.x*m_vecCalcWorldRight.z + pvb[5].pos.y*m_vecCalcWorldUp.z + pvb[5].pos.z*m_vecCalcWorldAhead.z;
    pvb[5].pos = vec;
}

/////////////////////////// CALC WORLD ORIENTATION TRANSFORMATIONS ////////////////////////////////////

VOID CPolySprite::CalcWorldOrientUpTransformation()
{
    assert(m_bSetCalled);
    // Create tranformation axii when up vector is world vector instead of camera up.
    D3DXVec3Cross(&m_vecCalcWorldRight, &m_vecDefaultWorldUp, &m_vecCameraAhead);
    D3DXVec3Normalize(&m_vecCalcWorldRight, &m_vecCalcWorldRight);
    
    D3DXVec3Cross(&m_vecCalcWorldAhead, &m_vecCalcWorldRight, &m_vecDefaultWorldUp);
    D3DXVec3Normalize(&m_vecCalcWorldAhead, &m_vecCalcWorldAhead);

    m_vecCalcWorldUp = m_vecDefaultWorldUp;
}
VOID CPolySprite::CalcWorldOrientAheadTransformation()
{
    assert(m_bSetCalled);
    // Create tranformation axii when ahead vector is world vector instead of camera up.
    D3DXVec3Cross(&m_vecCalcWorldRight, &m_vecCameraUp, &m_vecDefaultWorldAhead);
    D3DXVec3Normalize(&m_vecCalcWorldRight, &m_vecCalcWorldRight);

    D3DXVec3Cross(&m_vecCalcWorldUp, &m_vecDefaultWorldAhead, &m_vecCalcWorldRight);
    D3DXVec3Normalize(&m_vecCalcWorldUp, &m_vecCalcWorldUp);

    m_vecCalcWorldAhead = m_vecDefaultWorldAhead;
}
VOID CPolySprite::CalcWorldOrientRightTransformation()
{   
    assert(m_bSetCalled);
    // Create tranformation axii when right vector is world vector instead of camera up.
    D3DXVec3Cross(&m_vecCalcWorldAhead, &m_vecDefaultWorldRight, &m_vecCameraUp);
    D3DXVec3Normalize(&m_vecCalcWorldAhead, &m_vecCalcWorldAhead);

    D3DXVec3Cross(&m_vecCalcWorldUp, &m_vecCameraAhead, &m_vecDefaultWorldRight);
    D3DXVec3Normalize(&m_vecCalcWorldUp, &m_vecCalcWorldUp);

    m_vecCalcWorldRight = m_vecDefaultWorldRight;
}

/////////////////////////// STRUCT / FUNCTOR STUFF ////////////////////////////////////

CPolySprite::DrawInfo::DrawInfo(LPDIRECT3DTEXTURE9 _pTex, const RECTF &_rectf, const D3DXMATRIX &_mtx, UINT _color) :
    pTex(_pTex), rectf(_rectf), mtx(_mtx), color(_color)
{
    vCenter.x = mtx._41;            
    vCenter.y = mtx._42;
    vCenter.z = mtx._43;

    bWorldSpace = false;
}  


bool CPolySprite::TextureSortFunctor::operator() (const DrawInfo &di1, const DrawInfo &di2) const
{
    return di1.pTex < di2.pTex;
}

CPolySprite::DepthSortFunctor::DepthSortFunctor(const D3DXVECTOR3 &lookAtNormal, 
                                                const D3DXVECTOR3 &eyePt, 
                                                bool bBackToFront) :
    m_lookAt(lookAtNormal), m_eyePt(eyePt), m_bBackToFront(bBackToFront)
{
}

bool CPolySprite::DepthSortFunctor::operator () (const DrawInfo &di1, const DrawInfo &di2) const
{   
    // find vector from eye to sprites
    D3DXVECTOR3 vecEyeSprite1, vecEyeSprite2;
    vecEyeSprite1 = di1.vCenter - m_eyePt;
    vecEyeSprite2 = di2.vCenter - m_eyePt;

    // dot it against lookAtNormal    
    float dist1 = D3DXVec3Dot(&vecEyeSprite1, &m_lookAt);
    float dist2 = D3DXVec3Dot(&vecEyeSprite2, &m_lookAt);

    if(m_bBackToFront)
        return (dist1 > dist2);
    else
        return (dist1 < dist2);
}