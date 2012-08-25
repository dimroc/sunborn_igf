#include "dxstdafx.h"
#include "PolySprite.h"
#include "ImpostorAtlas.h"

// ---------------------------------------------------------------------
// PUBLICS
// ---------------------------------------------------------------------

CImpostorAtlas::CImpostorAtlas(int nAtlasWidth, int nAtlasHeight, int nImpostorWidth, int nImpostorHeight) :
    IImpostorable(nAtlasWidth, nAtlasHeight), 
    m_nAtlasWidth(nAtlasWidth), m_nAtlasHeight(nAtlasHeight),
    m_nImpostorWidth(nImpostorWidth), m_nImpostorHeight(nImpostorHeight),
    m_nMaxImpostors( ( nAtlasWidth * nAtlasHeight ) / ( 2 * nImpostorWidth * nImpostorHeight ) ),

    m_nEntries(0),
    m_nFlaggedToDraw(0)
{
    // Make sure dimensions aren't retarded.
    assert(nAtlasWidth % nImpostorWidth == 0);
    assert(nAtlasHeight % nImpostorHeight == 0);
    assert(nAtlasWidth % 2 == 0 && nAtlasHeight % 2 == 0);
    assert(nAtlasWidth > 0 && nAtlasHeight > 0 && nImpostorHeight > 0 && nImpostorWidth > 0);
    assert(nAtlasWidth > nImpostorWidth && nAtlasHeight > nImpostorHeight);

    // Setup some of the constants in the viewport needed in RenderToAtlas.
    m_ViewPort.X = m_ViewPort.Y = 0;
    m_ViewPort.Width = m_nImpostorWidth;
    m_ViewPort.Height = m_nImpostorHeight;
    m_ViewPort.MinZ = 0.f;
    m_ViewPort.MaxZ = 1.f;

    // Initialize vectors that hold entries, udpates, and Rects to CLEAR
    SAtlasEntry entry;
    memset(&entry, 0, sizeof(SAtlasEntry));
    m_vEntries.resize(m_nMaxImpostors * 2, entry); //! resize uses the 0'd element for all entries.

    SAtlasEntryUpdate update;
    update.bUpdateImpostor = true; 
    update.bUseImpostor = false;
    update.vecPrevLookAt = D3DXVECTOR3(0.f, 0.f, 1.f);
    update.vecPrevWorldUp = D3DXVECTOR3(0.f, 1.f, 0.f);
    update.prevDistance = -1.f;
    m_vUpdates.resize(m_nMaxImpostors, update);

    m_vInvalidRectsToClear.reserve(m_nMaxImpostors);

    // create all the fixed RECTs that traverse the atlas texture.
    RECT rect;
    RECTF rectf;
    int nMaxCols = nAtlasWidth / nImpostorWidth;
    int nMaxRows = nAtlasHeight / nImpostorHeight;
    float hInverse = 1.f / (float)nAtlasHeight;
    float wInverse = 1.f / (float)nAtlasWidth;

    for(int i=0; i<(int)m_vEntries.size(); ++i)
    {   // Go through assigning the coordinates pertaining to the position i.
        int nRow = i / nMaxCols;
        int nCol = i % nMaxRows;

        // Set Texture coordinates for the RECTs and the verts u/v.
        rect.left = nCol * nImpostorWidth;
        rect.right = (nCol+1) * nImpostorWidth;
        rect.top = nRow * nImpostorHeight;
        rect.bottom = (nRow+1) * nImpostorHeight;

        rectf.left = rect.left * wInverse;
        rectf.right = rect.right * wInverse;
        rectf.top = rect.top * hInverse;
        rectf.bottom = rect.bottom * hInverse;

        m_vEntries[i].rect = rect;
        m_vEntries[i].rectf = rectf;

        AssignTriListUVs(m_vEntries[i].verts, &rectf);

        // Assign the swap ids
        if( i < m_nMaxImpostors )
            m_vEntries[i].swapid = i + m_nMaxImpostors;
        else
            m_vEntries[i].swapid = i - m_nMaxImpostors;
    }

    SetPixelOffsets(m_fPixelOffsets);    
}

CImpostorAtlas::~CImpostorAtlas(void)
{   // Inherited IImpostorable destructor called automatically.
}

HRESULT CImpostorAtlas::OnLostDevice() 
{ 
    m_vInvalidRectsToClear.clear();
    for(size_t i=0;i<m_vUpdates.size();++i) 
    {
        m_vUpdates[i].bUpdateImpostor = true;        
    }

    return IImpostorable::OnLostDevice(); 
}

HRESULT CImpostorAtlas::OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice) 
{     
    return IImpostorable::OnResetDevice(pd3dDevice); 
}

bool CImpostorAtlas::CheckToUpdateImpostor(bool &bShouldRenderImpostor, const D3DXVECTOR3* pWorldUp, D3DXVECTOR3* pViewpointDif, float fDistanceScale, int &id, bool bForce) 
{
    // check if spot on atlas
    if(id < 0)
    {   // new reservation on atlas
        assert(m_nEntries < m_nMaxImpostors);  // Make sure the loser doesn't try to pack too much on one atlas.
        id = m_nEntries++;
        bShouldRenderImpostor = true;   // render to impostor on the first one just to flush initially kinda.
        return true;
    }

    bool functionReturn;
    // else we check to see if we should update and render
    SAtlasEntryUpdate& update = m_vUpdates[ClampId(id)];

    if( update.bUpdateImpostor || bForce) 
    { // Refresh impostor on LostDevice() etc.
        update.bUpdateImpostor = false;
        bShouldRenderImpostor = update.bUseImpostor;
        functionReturn = true;
    }
    else 
    {
        float distance = fabs(pViewpointDif->x) + fabs(pViewpointDif->y) + fabs(pViewpointDif->z);
        if(distance < IMPOSTOR_DISTANCE_FACTOR * fDistanceScale) 
        {   // render actual object
            update.bUseImpostor = bShouldRenderImpostor = false;
            functionReturn = false;            
        }
        else
        {
            update.bUseImpostor = bShouldRenderImpostor = true;
            float diffUp, diffAt;
            AngleCheck(pWorldUp, &update.vecPrevWorldUp, pViewpointDif, &update.vecPrevLookAt, diffUp, diffAt);
            
            if(diffUp < IMPOSTOR_DELTA_UP_ANGLE_THRESH || diffAt < IMPOSTOR_DELTA_AT_ANGLE_THRESH ||
                fabs(update.prevDistance - distance) > IMPOSTOR_DELTA_DISTANCE_THRESH)
            {   
                update.prevDistance = distance;
                update.vecPrevWorldUp = *pWorldUp;
                update.vecPrevLookAt = *pViewpointDif; // already normalized        
                functionReturn = true;    // Update Impostor
            }
            else    // Render Old Impostor
                functionReturn = false;
        }
    }
    if(functionReturn)
        id = SwapEntry(id);
    return functionReturn;
}        

//! Renders an Impostor to the Atlas. Returns the id of this entry in the atlas.
VOID CImpostorAtlas::StartRenderToAtlas(LPDIRECT3DDEVICE9 pd3dDevice,  // duh
                                 const D3DXVECTOR3* m_corners,  //! corners of the bounding box as defined in CQuadTree::QTNode
                                 const D3DXVECTOR3* pEyePt, //! Position of Camera
                                 const D3DXVECTOR3* pObjPt, //! Position of object to be rendered to impostor
                                 const D3DXVECTOR3* pWorldUp, //! Camera's world up.
                                 const D3DVIEWPORT9* pViewport, //! Viewport of the d3dDevice.
                                 const D3DXMATRIX* pMatProj, //! Projection matrix of rendering
                                 const D3DXMATRIX* pMatWorld,
                                 int id)    // entry id for where to place impostor in atlas
{
    HRESULT hr;

    // -------
    // Setup projection / view matrices and get world space vertices.
    // -------

    // Use VIEWPOINT NORMAL, NOT lookAt Normal for view matrix.        
    // TODO: Could make by hand and optimize by using previous normlized vecDif
    D3DXMATRIX viewpointView;
    D3DXMatrixLookAtLH(&viewpointView, pEyePt, pObjPt, pWorldUp); 

    GenerateWorldSpaceBBPlaneCoordinates(m_corners, pViewport, pMatProj, &viewpointView, pMatWorld);  

    // Get Matrices        
    D3DXMATRIX newview, newproj;
    D3DXVECTOR3 newcenter;
    GetProjViewMatrices(&newproj, &newview, &newcenter, pEyePt, pWorldUp);

    //
    // Set Matrices and RENDER TO IMPOSTOR location in Atlas!
    //
    V(pd3dDevice->SetTransform(D3DTS_PROJECTION, &newproj) );
    V(pd3dDevice->SetTransform(D3DTS_VIEW, &newview) );

    m_vEntries[id].bValid = true;
    //
    // Create TRILIST vertices instead of strip.
    m_vEntries[id].verts[0].pos = m_Vertices[0].pos;
    m_vEntries[id].verts[1].pos = m_Vertices[1].pos;
    m_vEntries[id].verts[2].pos = m_Vertices[2].pos;
    m_vEntries[id].verts[3].pos = m_Vertices[2].pos;
    m_vEntries[id].verts[4].pos = m_Vertices[1].pos;
    m_vEntries[id].verts[5].pos = m_Vertices[3].pos;

    m_vEntries[id].vCenter = newcenter;
    
    m_ViewPort.X = m_vEntries[id].rect.left;
    m_ViewPort.Y = m_vEntries[id].rect.top;    

    // Setup device to Render to Atlas placing the viewport at the appropriate location on atlas texture.    
    V( m_pRenderToSurface->BeginScene( m_pTextureSurface, &m_ViewPort ) );
}

VOID CImpostorAtlas::StopRenderToAtlas()
{
    HRESULT hr;
    V( m_pRenderToSurface->EndScene( D3DX_FILTER_NONE ) );
}

//! Render everything in the Atlas
VOID CImpostorAtlas::RenderAtlas(LPDIRECT3DDEVICE9 pd3dDevice, 
                                 const D3DXVECTOR3 *pEyePt, 
                                 const D3DXVECTOR3 *pAhead, 
                                 bool bSortBackToFront)
{
    static vector<BoxVertTex> verts(m_nMaxImpostors);
    verts.clear();

    vector<SAtlasEntry*> vpEntries;
    // Populate verts with valid impostors
    for(size_t i=0; i<m_vEntries.size(); ++i) 
    {
        if(m_vEntries[i].bValid && m_vEntries[i].bToDraw)
        {   // store up valid entries
            assert(m_vEntries[m_vEntries[i].swapid].bValid == false);
            vpEntries.push_back(&m_vEntries[i]);
        }        
    }
    if(bSortBackToFront)
    {   // sort the entered *pEntries
        assert(pEyePt && pAhead);
        PtrEntrySort funcptrSort(pEyePt, pAhead, true);
        std::sort(vpEntries.begin(), vpEntries.end(), funcptrSort);
    }

    for(size_t i=0; i<vpEntries.size(); i++)
    {   // stuff in vertices
        for(int j=0;j<6;j++)
            verts.push_back(vpEntries[i]->verts[j]);                        
    }

    if(!verts.empty())
    {   // Send all the shit to the draw primitive
        HRESULT hr;
        IImpostorable::SetDevice(pd3dDevice);

        V( pd3dDevice->SetTexture( 0, m_pDynamicTexture ) );
        V( pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, (UINT)(verts.size() / 3), &verts[0], sizeof(BoxVertTex) ) );

        IImpostorable::ClearDevice(pd3dDevice);
    }
}

//! Renders all valid impostors through the passed in CPolySprite
VOID CImpostorAtlas::RenderAtlasToSprite(CPolySprite *pPolySprite)
{
    // Populate verts with valid impostors
    for(size_t i=0; i<m_vEntries.size(); ++i) 
    {
        if(m_vEntries[i].bValid && m_vEntries[i].bToDraw)
        {   // store up valid entries
            assert(m_vEntries[m_vEntries[i].swapid].bValid == false);
            
            CPolySprite::PolySpriteVertex spriteverts[6];
            for(int j=0; j<6; j++)
            {
                spriteverts[j].pos = m_vEntries[i].verts[j].pos;
                spriteverts[j].u = m_vEntries[i].verts[j].u + m_fPixelOffsets[j*2];
                spriteverts[j].v = m_vEntries[i].verts[j].v + m_fPixelOffsets[j*2 + 1];
                spriteverts[j].color = 0xffffffff;                
            }
            pPolySprite->Draw(m_pDynamicTexture, spriteverts, &m_vEntries[i].vCenter, true);
        }        
    }
}

//! Render all valid impostors in greyscale with each value being the z depth on current rendertarget 0.
VOID CImpostorAtlas::RenderAtlasForShadowMap(LPDIRECT3DDEVICE9 pd3dDevice,
                                             const D3DXVECTOR3* pEyePt,
                                             const D3DXVECTOR3* pEyeAhead,
                                             const D3DXMATRIX* pProj,
                                             const D3DXMATRIX* pView,
                                             const D3DXMATRIX* pWorld)
{
    static vector<BoxVertTex> verts(m_nMaxImpostors);
    verts.clear();

    vector<SAtlasEntry*> vpEntries;
    // Populate verts with valid impostors
    for(size_t i=0; i<m_vEntries.size(); ++i) 
    {
        if(m_vEntries[i].bValid && m_vEntries[i].bToDraw)
        {   // store up valid entries
            assert(m_vEntries[m_vEntries[i].swapid].bValid == false);
            vpEntries.push_back(&m_vEntries[i]);
        }        
    }
    // sort the entered *pEntries
    assert(pEyePt && pEyeAhead);
    PtrEntrySort funcptrSort(pEyePt, pEyeAhead, false);
    std::sort(vpEntries.begin(), vpEntries.end(), funcptrSort); 

    for(size_t i=0; i<vpEntries.size(); i++)
    {   // stuff in vertices
        for(int j=0;j<6;j++)
            verts.push_back(vpEntries[i]->verts[j]);                        
    }

    if(!verts.empty())
    {   // Send all the shit to the draw primitive
        HRESULT hr;
        IImpostorable::SetDevice(pd3dDevice);

        // V( pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ) );
        // V( pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE ) );

        D3DXMATRIX viewproj = *pView * *pProj;
        // setup shadow map effect. Do I NEED VertexDeclarations? hope fvf will do.
        V( m_pShadowMapEffect->SetTexture("Texture", m_pDynamicTexture) );
        V( m_pShadowMapEffect->SetMatrix("mViewProj", &viewproj) );
        V( m_pShadowMapEffect->SetMatrix("mWorld", pWorld) );        

        UINT numPasses;
        V( m_pShadowMapEffect->Begin( &numPasses, 0 ) );
        for( UINT iPass = 0; iPass < numPasses; iPass++ )
        {
            V( m_pShadowMapEffect->BeginPass( iPass ) );
            V( pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, (UINT)(verts.size() / 3), &verts[0], sizeof(BoxVertTex) ) );
            V( m_pShadowMapEffect->EndPass() );
        }
        V( m_pShadowMapEffect->End() );

        // V( pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ) );
        // V( pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE ) );

        IImpostorable::ClearDevice(pd3dDevice);
    }
}

// ---------------------------------------------------------------------
// PRIVATES
// ---------------------------------------------------------------------


//! Calls pd3d->CLEAR to clear all invalid RECTs.
VOID CImpostorAtlas::ClearInvalidEntries(LPDIRECT3DDEVICE9 pd3dDevice)
{
    HRESULT hr;

    if(!m_vInvalidRectsToClear.empty())
    {   // clear rects
        V( m_pRenderToSurface->BeginScene( m_pTextureSurface, NULL ) );     
        V( pd3dDevice->Clear((DWORD)m_vInvalidRectsToClear.size(), (const D3DRECT*)&m_vInvalidRectsToClear[0], D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0) );    
        V( m_pRenderToSurface->EndScene( 0 ) );
        m_vInvalidRectsToClear.clear();
    }
}

//! Invalidates current entry and places it on clear vector then returns its swaps entry.
int CImpostorAtlas::SwapEntry(int id)
{    
    assert( id >= 0 && id < (int)m_vEntries.size() * 2 );
    assert(m_vEntries[id].bValid == true);

    m_vEntries[id].bValid = false;
    m_vInvalidRectsToClear.push_back(m_vEntries[id].rect);

    return m_vEntries[id].swapid;
}

//! Assigns the tex coordinates in rectf to the trilist verts. PREREQ: verts is an array of 6 BoxVertTex.
VOID CImpostorAtlas::AssignTriListUVs(BoxVertTex* verts, const RECTF* rectf)
{
    /*
    // Save coordinates in impostor vertices. Triangle Strip style.
    m_Vertices[0] => 0.0f, 0.0f);
    m_Vertices[1] => 0.0f, 1.f);
    m_Vertices[2] => 1.f, 0.0f);
    m_Vertices[3] => 1.f, 1.f);
    */
#if 0
    verts[0].u = 0;   //0
    verts[0].v = 0;

    verts[1].u = 0;   //1
    verts[1].v = 1;

    verts[2].u = 1;   //2
    verts[2].v = 0;

    verts[3].u = 1;   //2
    verts[3].v = 0;

    verts[4].u = 0;   //1
    verts[4].v = 1;

    verts[5].u = 1;   //3
    verts[5].v = 1;
#endif
//#if 0
    verts[0].u = rectf->left;   //0
    verts[0].v = rectf->top;

    verts[1].u = rectf->left;   //1
    verts[1].v = rectf->bottom;

    verts[2].u = rectf->right;   //2
    verts[2].v = rectf->top;

    verts[3].u = rectf->right;   //2
    verts[3].v = rectf->top;

    verts[4].u = rectf->left;   //1
    verts[4].v = rectf->bottom;

    verts[5].u = rectf->right;   //3
    verts[5].v = rectf->bottom;
//#endif
}

//! PREREQ: 12 slots
VOID CImpostorAtlas::SetPixelOffsets(float *fPixelOffsets)
{
    float fPixelOffsetY = 1.f / m_nAtlasHeight;
    float fPixelOffsetX = 1.f / m_nAtlasWidth;
    m_fPixelOffsets[0] = fPixelOffsetX;
    m_fPixelOffsets[1] = fPixelOffsetY;

    m_fPixelOffsets[2] = fPixelOffsetX;
    m_fPixelOffsets[3] = -fPixelOffsetY;

    m_fPixelOffsets[4] = -fPixelOffsetX;
    m_fPixelOffsets[5] = fPixelOffsetY;

    m_fPixelOffsets[6] = -fPixelOffsetX;
    m_fPixelOffsets[7] = fPixelOffsetY;

    m_fPixelOffsets[8] = fPixelOffsetX;
    m_fPixelOffsets[9] = -fPixelOffsetY;

    m_fPixelOffsets[10] = -fPixelOffsetX;
    m_fPixelOffsets[11] = -fPixelOffsetY;
}