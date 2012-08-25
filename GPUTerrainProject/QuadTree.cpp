#include "dxstdafx.h"
#include "quadtree.h"
#include "mipmapterrain.h"
#include "foliage.h"
#include "gamedefine.h"
#include "FXDecl.h"
#include "Pillar.h"
#include "cloudbatch.h"

using gdef::AABBFrustumTest;

#define         SKIRT_DEPTH                         20.0f
#define         LOAD_AREA_DIMENSION                 1025        // pre cellspace dimension        
#define         LOADTIME_GAP                        1.0f        // 1 second


// ---------------------------------------------------------
//! PRIVATE/PROTECTED FUNCTIONS - STRUCT QTNODE AT BOTTOM
// ---------------------------------------------------------

//! Traverses QT setting nodes to AABB_INSIDE, OUTSIDE, or SPANNING and deposits meshes from leafs to vector
VOID CQuadTree::FrustumCullWorker(QTNode *n) {
    if(n == NULL)
        return;    
    UCHAR r = AABBFrustumTest(m_FrustumPlanes, arrCIP, n->corners); // find out where quad lies in frustum

    // Handle results of quad frustum cull
    //
    if(r & AABB_OUTSIDE) {
        return ; // abort descent 
    }

    // dump objects into vector since leaf and inside/spanning
    else if(n->bLeaf) {   
        m_vBoundingBoxes.push_back(n);
        m_vVisibilityList.push_back(n);        
    }

    // If not leaf deal with children
    else if(r & AABB_INSIDE) {
        m_vBoundingBoxes.push_back(n);
        FrustumGatherChildren(n);
    }
    else if(r & AABB_SPANNING) {
        FrustumCullWorker(n->bl);
        FrustumCullWorker(n->br);
        FrustumCullWorker(n->tl);
        FrustumCullWorker(n->tr);
    }
}

VOID CQuadTree::FrustumGatherChildren(QTNode *n) {
    if(n == NULL)
        return ;
    if(n->bLeaf) {  
        // place contents in vVisibilityList
        m_vVisibilityList.push_back(n);
    }
    else {
        FrustumGatherChildren(n->bl);
        FrustumGatherChildren(n->br);
        FrustumGatherChildren(n->tl);
        FrustumGatherChildren(n->tr);
    }
}

VOID CQuadTree::GenerateQuads() {
    SAFE_DELETE(m_root);
    if(m_bInit) {
        // Generate Quads         
        // NOTE: The values are later multiplied by heightSpacing in ExpandQuadTree!
        int maxLength = (m_Width > m_Length) ? m_Width : m_Length;
        // displace the root center by the m_vTranslate that moves entire quadtreemipmap terrain.
        m_root = new QTNode( maxLength/2, 
                             maxLength/2, 
                             maxLength - 1, 
                             0, 
                             m_Height);

        QuadWorker(m_root); // Generates quad tree in perfect square
        m_root = QuadTrim(m_root); // deletes any quads outside of Terrain (if rectangular for example).
		DetermineNeighbours();				
    }
}

VOID CQuadTree::LoadTerrain(ITerrain *pt) {    
    m_bInit = false;    

    m_pTerrain = pt;
    if(m_pTerrain->IsInit()) { 
        m_Width = m_pTerrain->GetWidth();
        m_Length = m_pTerrain->GetHeight(); // 2d heightmap's "height" is actually z length
        m_Height = 256;
        m_bInit = true;
    }
}

BOOL CQuadTree::NodeOutsideBounds(QTNode *n, int min, int width, int height) {
    if(n == NULL)
        return true;
    if(n->midx <= min || n->midz <= min || n->midx >= width || n->midz >= height)
        return true;
    return false;
}

//! Trims from the bottom of quadtree, up.
CQuadTree::QTNode * CQuadTree::QuadTrim(QTNode *n) {
    if(!n->bLeaf) {
        n->bl = QuadTrim(n->bl);
        n->br = QuadTrim(n->br);
        n->tl = QuadTrim(n->tl);
        n->tr = QuadTrim(n->tr);

        // if they are all NULL, SAFE_DELETE(n)
        if(n->bl == NULL && n->br == NULL && n->tl == NULL && n->tr == NULL)
            SAFE_DELETE(n);
    }
    else { // in leaf
        // Trims by terrain area. Perhaps later, we'll actually want empty area, or water!
        int min = (int)MIN_DIMENSION/2 - 1;
        int width = m_Width - 1;
        int height = m_Length - 1;
        
        if(NodeOutsideBounds(n, min, width, height) )
            SAFE_DELETE(n);        
    }   
    return n;
}

//! Recursively generates quads up to a minimum size of tileSize*tileSize.
VOID CQuadTree::QuadWorker(QTNode *n) {
    // stop recursion if quad has a dimension smaller than tileSize
    if(n->length <= (int)MIN_DIMENSION) {
		n->bLeaf = true;
        n->bl = NULL; n->tl = NULL; n->br = NULL; n->tr = NULL;
        return ;
    }

    UINT newlength = n->length/2;
    UINT wIncr = newlength/2;
    UINT hIncr = newlength/2;

    n->bl = new QTNode(n->midx - wIncr, n->midz - hIncr, newlength, n->miny, n->maxy);
    n->br = new QTNode(n->midx + wIncr, n->midz - hIncr, newlength, n->miny, n->maxy);
    n->tl = new QTNode(n->midx - wIncr, n->midz + hIncr, newlength, n->miny, n->maxy);
    n->tr = new QTNode(n->midx + wIncr, n->midz + hIncr, newlength, n->miny, n->maxy);

    QuadWorker(n->bl);
    QuadWorker(n->br);
    QuadWorker(n->tl);
    QuadWorker(n->tr);
}

//! TODO: Fix so it handles intermediate NULL children
VOID CQuadTree::DetermineNeighbours(VOID)
{	m_vTraversalQueue.push_back(m_root);
	
	while(!m_vTraversalQueue.empty())
	{	QTNode *n = m_vTraversalQueue.front();
		m_vTraversalQueue.pop_front();
        if(n == NULL)   continue; 
		if(n->bLeaf)	continue;
        // This hack I have won't traverse nodes with one node child which is wrong.
    //    if(n->bl == NULL || n->tl == NULL || n->br == NULL || n->tr == NULL) continue;
		//determine neighbours of children first inside node n
		//set up square   tl -- tr
		//				   |     |
		//                bl -- br
		if(n->bl)
		{	n->bl->east  = n->br;	//set bl 
			n->bl->north = n->tl;
			n->bl->west  = n->west  ? n->west->br  : NULL;	
			n->bl->south = n->south ? n->south->tl : NULL;
		}
		if(n->br)
		{	n->br->north = n->tr;	//set br
			n->br->west  = n->bl;
			n->br->south = n->south ? n->south->tr : NULL;
			n->br->east  = n->east  ? n->east->bl  : NULL;
		}
		if(n->tl)
		{	n->tl->east  = n->tr;	//set tl
			n->tl->south = n->bl;
			n->tl->west  = n->west  ? n->west->tr  : NULL;
			n->tl->north = n->north ? n->north->bl : NULL;
		}
		if(n->tr)
		{	n->tr->west  = n->tl;	//set tr
			n->tr->south = n->br;
			n->tr->north = n->north ? n->north->br : NULL;
			n->tr->east  = n->east  ? n->east->tl  : NULL;
		}

		//set up neighbours outside node n by using n's neighbours

		m_vTraversalQueue.push_back(n->bl);
		m_vTraversalQueue.push_back(n->br);
		m_vTraversalQueue.push_back(n->tl);
		m_vTraversalQueue.push_back(n->tr);
	}
}

// ---------------------------------------------------------
// PUBLIC FUNCTIONS
// ---------------------------------------------------------

//! Parent constructor loads terrain
CQuadTree::CQuadTree(const SQuadTreeInfo* pinfo, const SPillarInfo* pillarInfo) 
    : MIN_DIMENSION(MIPMAP_TILESIZE), TILEAREA(MIN_DIMENSION*MIN_DIMENSION),       
      m_LODThreshold(LOD_VERTEX_THRESHOLD),
      m_vTranslation(pinfo->vTranslation),
      m_pTerrain(NULL), m_bInit(false), m_bLockLOD(false), m_bDrawTerrain(true), m_pd3dDevice(NULL),
      m_Width(0), m_Length(0), m_Height(0),
      m_bDrawBoundingBoxes(false), m_bDrawPillar(true),
      m_LODDistanceThreshold(pinfo->nCellSpacing * MIPMAP_TILESIZE * LOD_DISTANCE_MULTIPLIER),      
      m_LoadArea( pinfo->nCellSpacing ),
      m_vLightDir(0.f, 1.f, 0.f, 1.f),
      m_pFoliage(NULL), m_pBasinVertices(NULL), m_pBasinIndices(NULL),
      m_root(NULL),
      m_Info(*pinfo),
	  m_pBasinTexture(NULL)
{
    //Load terrain
    m_pTerrain = new CMipmapTerrain(pinfo->strHMP.c_str(), 
                                    pinfo->fTextureTile, 
                                    pinfo->nCellSpacing, 
                                    pinfo->nHeightSpacing,
                                    MIPMAP_TILESIZE);
    LoadTerrain(m_pTerrain);
    if(!m_bInit) {
        SAFE_DELETE(m_pTerrain);
        memset(this, 0, sizeof(CQuadTree));
        return ;
    }    

    //! Check if whether to load entire thing in memory
    int maxDim = (m_pTerrain->GetWidth() > m_pTerrain->GetHeight()) ? m_pTerrain->GetWidth() : m_pTerrain->GetHeight();
    m_LoadArea.CheckFitsInArea(maxDim * m_pTerrain->GetCellSpacing() ); 

    //! Set the world position of ENTIRE QuadTree Mipmapped terrain. Terrain acts as if at 0.    
    D3DXMatrixTranslation(&m_mWorld, m_vTranslation.x, m_vTranslation.y, m_vTranslation.z);

	int cellSpacing = m_pTerrain->GetCellSpacing();
	D3DXMATRIX pillarWorld;
	D3DXMatrixTranslation(&pillarWorld, m_vTranslation.x + (m_Width/2)*cellSpacing, m_vTranslation.y - SKIRT_DEPTH, m_vTranslation.z + (m_Length/2)*cellSpacing);

	// load pillar    
	m_pPillar = new CPillar(pillarInfo, pillarWorld);

    //! Generates Quads based on Terrain mid point
    GenerateQuads(); 
    //! Actual Terrain blocks is loaded on the fly by LoadNewData	
}

VOID CQuadTree::LoadNewTerrainBlock() {
    QTNode *n = m_LoadArea.ReturnNextBlock();
    if(n != NULL)
    {
        // Load terrain block!                
	    int wIncr = n->length >> 1;
	    int hIncr = n->length >> 1;

        static int cellSpace = m_pTerrain->GetCellSpacing();
        int xposmin = (n->midx - wIncr) / cellSpace;
        int zposmin = (n->midz - hIncr) / cellSpace;
        int xposmax = (n->midx + wIncr) / cellSpace;
        int zposmax = (n->midz + hIncr) / cellSpace;
        
        Block *b = NULL;
        // Only make terrain tile if within terrain
        if(xposmin >= 0 && zposmin >= 0 && xposmax < (int)m_Width && zposmax < (int)m_Length) 
        {	    
	        b = new Block(
                            xposmin, 
                            xposmax,
                            zposmin,
                            zposmax,
                            m_pd3dDevice, 
                            m_pTerrain,
                            m_Width,
                            n->length + 1);	    
            // Make leaves of node have their tile point to block.
            m_LoadArea.SetBlockPointers(n, b);
        }
    }
}

//! Loads any new data from RAM to VRAM based on camera position. Spread loading out over time.
UINT CQuadTree::LoadNewData(const D3DXVECTOR3* pEyePt, double fTime) {
    if(m_LoadArea.IsFitsInArea())  return S_OK;

    HRESULT hr;
    static double prevPopulateTime = 0;
    static int nLoads = 0;   

    // Run Populate Area once every LOADTIME_GAP
    if( fTime - prevPopulateTime > LOADTIME_GAP)
    {
        for(int i=0; i<nLoads; i++)     // make sure all terrain blocks from previous check were loaded.
            LoadNewTerrainBlock();
        nLoads = m_LoadArea.PopulateLoadArea(m_root, pEyePt->x, pEyePt->z);   // Returns # of Blocks to Load.        
        V_RETURN( m_LoadArea.UnloadBlocks() ); //! Unload blocks, if any
        prevPopulateTime = fTime;
    }    
    LoadNewTerrainBlock(); // Load a block per frame, if any
    nLoads --;
    return S_OK;
}

CQuadTree::~CQuadTree(void)
{
    SAFE_DELETE(m_pTerrain);    
    // SAFE_DELETE(m_pFoliage);
    SAFE_DELETE(m_root);    // Cleans up QuadTree recursively. This cleans up BLOCKS/TILES
	SAFE_DELETE(m_pPillar);    
}

//! Creates meshes,etc bound to the pd3dDevice sent in parameter. This is stored in object.
UINT CQuadTree::OnCreateDevice( IDirect3DDevice9 * pd3dDevice ) {
    if(pd3dDevice == NULL || m_bInit == FALSE)
        return EXIT_FAILURE;
    m_pd3dDevice = pd3dDevice;    

    // Generate Chunks (Triangle Strips) For each tile.
	m_pTerrain->GenerateData(pd3dDevice,
                            m_Info.strBlendMap.c_str(),
                            m_Info.strRedTexture.c_str(),
                            m_Info.strGreenTexture.c_str(),
                            m_Info.strBlueTexture.c_str(),
                            m_Info.strAlphaTexture.c_str());
    
    // Sets up Tiles and loads Terrain in Video Memory if m_LoadArea.bFitsInArea
    TileWorker(m_root, NULL, m_LoadArea.IsFitsInArea());

    //! Displaces and expands Quad Tree to HMP CellSpacing.
    DisplaceExpandQuadTree(m_root, m_pTerrain->GetCellSpacing(), m_pTerrain->GetHeightSpacing());

    m_Width *= m_pTerrain->GetCellSpacing();
    m_Length *= m_pTerrain->GetCellSpacing();

#if 0   // NOFOLIAGE
    m_pFoliage->PopulateLeafNodesWithFoliage(m_root);
    m_pFoliage->OnCreateDevice(pd3dDevice);
#endif
    return EXIT_SUCCESS;
}

//! Check min/max y of Terrain Tile against top and bottom frustum planes.
bool CQuadTree::CullTerrainTile(QTNode *n) {        
    Tile *t = n->pTile;
    if(t->b == NULL) { // if not in memory yet
        t->bTerrainVisible = false;
        return false;
    }    

    // Sends the aligned quad vertices for top/bottom planes and checks if the miny/maxy of tile is inside.
    t->bTerrainVisible = IsHeightQuadSectionVisible(n->corners[arrCIP[5].i1],   
                                                    n->corners[arrCIP[5].i2],
                                                    n->corners[arrCIP[4].i1],
                                                    n->corners[arrCIP[4].i2],
                                                    t->miny + m_vTranslation.y, t->maxy + m_vTranslation.y,
                                                    0.0f);
    return t->bTerrainVisible;
}

VOID CQuadTree::CreateBasin(VOID)
{	
	HRESULT hr;
	V(	m_pd3dDevice->CreateVertexBuffer(
		4*sizeof(MESHVERT),
		D3DUSAGE_WRITEONLY,
        0, // we use vertex declarations. no fvfs
		D3DPOOL_MANAGED,
		&m_pBasinVertices,
		0) );

	V( m_pd3dDevice->CreateIndexBuffer( 4*sizeof(WORD), 
										D3DUSAGE_WRITEONLY, 
										D3DFMT_INDEX16, D3DPOOL_MANAGED, 
										&m_pBasinIndices, 0)); 
	int cellSpacing = m_pTerrain->GetCellSpacing();

	float right  = (float)(m_Width-cellSpacing);
	float bottom = (float)(m_Length-cellSpacing);
	float height = -SKIRT_DEPTH;

	int count = 0;

	MESHVERT *vertices;
	m_pBasinVertices->Lock(0, 0, (void**)&vertices, 0);
	
	// top left vertex
	vertices[count++] =  MESHVERT(	D3DXVECTOR3(0.f, height, 0.f), // (x, y, z)
                                    D3DXVECTOR3(0.f, -1.f, 0.f), // normals                                            
                                    0.f, 0.f ); // u, v
	// top right vertex
	vertices[count++] =  MESHVERT(	D3DXVECTOR3(right, height, 0.f), // (x, y, z)
                                    D3DXVECTOR3(0.f, -1.f, 0.f), // normals                                            
                                    1.f, 0.f ); // u, v
	// bottom right vertex
	vertices[count++] =  MESHVERT(	D3DXVECTOR3(right, height, bottom), // (x, y, z)
                                    D3DXVECTOR3(0.f, -1.f, 0.f), // normals                                            
                                    1.f, 1.f ); // u, v
	// bottom left vertex
	vertices[count++] =  MESHVERT(	D3DXVECTOR3(0.f, height, bottom), // (x, y, z)
                                    D3DXVECTOR3(0.f, -1.f, 0.f), // normals                                            
                                    0.f, 1.f ); // u, v

	m_pBasinVertices->Unlock();

	WORD *indices;
	
	m_pBasinIndices->Lock(0, 0, (void**)&indices, 0);

	// back, right, front, left, bottom using triangle strip
	indices[0] = 0; indices[1] = 1; indices[2] = 3; indices[3] = 2;	

	m_pBasinIndices->Unlock();

  D3DXCreateTextureFromFileA(m_pd3dDevice, m_Info.strPillarTexture.c_str(), &m_pBasinTexture);
}

VOID CQuadTree::CullCloud(CCloud *pc)
{   
    const D3DXVECTOR3* corners = pc->GetCorners();

    UCHAR r = AABBFrustumTest(m_FrustumPlanes, arrCIP, corners);
    if(r != AABB_OUTSIDE) {   
        pc->SetVisible(true); 
        pc->SetUpdated(false);
    }
    else
        pc->SetVisible(false); 
}

bool CQuadTree::IsHeightQuadSectionVisible(const D3DXVECTOR3 &c11, // aligned quad corners for optimized plane testing
                                           const D3DXVECTOR3 &c12, 
                                           const D3DXVECTOR3 &c21, 
                                           const D3DXVECTOR3 &c22, 
                                           float miny, float maxy, // the min and max y with which to alter the aligned corners
                                           float thresh)
{
    // Checks bottom plane.
    D3DXVECTOR3 v = c11;
    v.y = maxy;
    float dot1 = D3DXPlaneDotCoord(&m_FrustumPlanes[5], &v);

    if(dot1 < thresh) { // since dot1<0 check dot2
        v.x = c12.x;
        v.y = miny;
        v.z = c12.z;
        float dot2 = D3DXPlaneDotCoord(&m_FrustumPlanes[5], &v);
        if(dot2 < thresh) {
            return false;
        }
    }
    // Check top plane
    v.x = c21.x;
    v.y = maxy;
    v.z = c21.z;
    dot1 = D3DXPlaneDotCoord(&m_FrustumPlanes[4], &v);
    if(dot1 < thresh) {
        v.x = c22.x;
        v.y = miny;
        v.z = c22.z;
        float dot2 = D3DXPlaneDotCoord(&m_FrustumPlanes[4], &v);

        if(dot2 < thresh) // cull 
            return false;
    }
    return true;
}
                                            
//! Further ViewFrustum Culls Visible Nodes. 
VOID CQuadTree::CullVisibleNodes() {
    QTNode *n;
    set<CCloud*> setCloudsToTest;
    m_bTileVisible = false; // used to determine if DrawTerrain should even bother setup effect and renderstates.
    
    for(size_t i=0; i<m_vVisibilityList.size(); i++) { // cycle thru visible nodes
        n = m_vVisibilityList[i];
        
        m_bTileVisible |= CullTerrainTile(n); // cull terrain tile

        for(size_t j=0; j<n->vpClouds.size(); j++) // populate a cloud visibily set
            setCloudsToTest.insert(n->vpClouds[j]); // insert in set so you don't check clouds in more than quad more than once.
    } // end for

    set<CCloud*>::iterator it;  // CULL CLOUDS
    for(it = setCloudsToTest.begin(); it!= setCloudsToTest.end(); it++)
        CullCloud(*it); 
}

//! Populate the mesh bins/chunks for DrawVisible 
// PREREQ: Set View Frustum Planes called.

UINT CQuadTree::ViewFrustumCull(const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatProj) {
    // Empty out visibility bins
    m_vBoundingBoxes.clear();
    m_vVisibilityList.clear();    

    // Traverse Quad Tree and populate VisibilityList with visible QTNodes
    FrustumCullWorker(m_root);

    // Go through visible nodes and culls IObjects' OBB (Object Bounding Box) and Clouds. 
    // For Terrain Tiles it checks min/max y and sets bVisible.
    CullVisibleNodes();

    return 0;
}

//! Batch Renders each group of objects possible in a quad.
UINT CQuadTree::DrawObjects(const D3DXVECTOR3* pEyePt, 
                            const D3DXVECTOR3* pUp, 
                            const D3DXVECTOR3* pRight, 
                            const D3DXVECTOR3* pAhead,                             
                            const D3DXMATRIX* pMatView, 
                            const D3DXMATRIX* pMatProj,
                            const D3DXMATRIX* pMatViewProj)
{    
    //
    // Draw Visible Foliage (determined visible if terrain tile is visible. Cheap work around).
    //
#if 0   // TODO: FOLIAGE
    // TODO: Back to front sorting
	set<SGrass*> spGrasses;
    for(size_t i=0;i<m_vVisibilityList.size(); i++)
    {   // Gather Visible Foliage
        QTNode* n = m_vVisibilityList[i];
		if(n->pTile->bTerrainVisible)
		{
			for(size_t j=0; j<n->vpGrass.size(); j++)
				spGrasses.insert(n->vpGrass[j]);
		}
    }	
	m_pFoliage->OnFrameRender(m_pd3dDevice, spGrasses, pMatProj, pMatView);    // Draw Foliage
#endif


    //
    // TODO: I have a feeling Objects will be culled separate from the QuadTree and just done with normal VFC.
    //
#if 0
    for(size_t i=0;i<m_vVisibilityList.size(); i++)
    {   // Draw Actors
        // QTNode* n = m_vVisibilityList[i];

    }
#endif    

    return EXIT_SUCCESS;
}

HRESULT CQuadTree::DrawTerrain(const D3DXMATRIX &matViewProj, const D3DXMATRIX* pMatShadow, LPDIRECT3DTEXTURE9 pShadowMap) 
{
    HRESULT hr;
    UINT cPass;

    if(!m_bTileVisible)
        return EXIT_SUCCESS;
    
    V_RETURN( m_pd3dDevice->SetVertexDeclaration( m_pTerrain->GetTerrainDecl() ) );
    
    ID3DXEffect *effect = m_pTerrain->GetTerrainEffect(); // NOT OWNER SO DONT DELETE()

    // set worldviewproj in shader to transform vertex.     
    V_RETURN( effect->SetMatrix(m_pTerrain->GetViewProjHandle(), &matViewProj) );
    V_RETURN( effect->SetMatrix(m_pTerrain->GetWorldHandle(), &m_mWorld) );

    D3DXMATRIX mtx;
    D3DXMatrixTranspose(&mtx, &m_mWorld);
    D3DXMatrixInverse(&mtx, NULL, &mtx);    
    V_RETURN( effect->SetMatrix(m_pTerrain->GetITWorldHandle(), &mtx) );

    // Set Sun Light
    V_RETURN( effect->SetVector(m_pTerrain->GetLightDirHandle(), &m_vLightDir ) );

    // Set Shadow map info. TODO: Make Handles
    V_RETURN( effect->SetMatrix(m_pTerrain->GetShadowMtxHandle(), pMatShadow ) );
    V_RETURN( effect->SetTexture(m_pTerrain->GetShadowMapHandle(), pShadowMap ) );

    V_RETURN( effect->Begin(&cPass, 0) ); // Render through all passes
    for(UINT p=0; p<cPass; p++) 
    {
        V_RETURN( effect->BeginPass( p ) );
        V_RETURN( DrawTerrainTiles() );
        V_RETURN( effect->EndPass() );
    }
    V_RETURN( effect->End() );

	return EXIT_SUCCESS;
}

VOID CQuadTree::DrawBasin(VOID)
{
	HRESULT hr;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_mWorld);

	// draw basin	
	DWORD oldLightMode;
	V( m_pd3dDevice->SetTexture(0, m_pBasinTexture))		
	V( m_pd3dDevice->GetRenderState(D3DRS_LIGHTING, &oldLightMode) );		
	V( m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE) );	
    V( m_pd3dDevice->SetFVF(MESHVERT::FVF) );
	V( m_pd3dDevice->SetStreamSource(0, m_pBasinVertices, 0, sizeof(MESHVERT)) );
	V( m_pd3dDevice->SetIndices(m_pBasinIndices) );
	V( m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2) );		
	V( m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, oldLightMode) );	
}

HRESULT CQuadTree::DrawTerrainTiles() {	
    HRESULT hr;
    Block *b = NULL;
    Tile *t = NULL;
    int prevLODindex = -1;
    float prevLOD = -1.0f;

	//variables set up to reduce the overhead of calling polymorphic functions multiple times
	vector<IDirect3DIndexBuffer9 *> tileIBlist;
	vector<int> tileSizeList;
	vector<IDirect3DIndexBuffer9 *> skirtIBlist;
	vector<int> skirtSizeList;

	int numLODs = m_pTerrain->GetNumberOfLODs();
	for(int i = 0; i < numLODs; i ++)
	{	tileIBlist.push_back(m_pTerrain->GetIndexBufferAtLOD(i));
		tileSizeList.push_back(m_pTerrain->GetNumTrianglesAtLOD(i));
		skirtSizeList.push_back(0);
		skirtIBlist.push_back(m_pTerrain->GetSkirtIndex(i, skirtSizeList[i]));
	}

    ID3DXEffect *effect = m_pTerrain->GetTerrainEffect(); // NOT OWNER SO DONT DELETE()

    // Cache Tween Handle.
    D3DXHANDLE tweenHandle = m_pTerrain->GetTweenHandle();

    // batch renders terrain via block vertex buffers of 129x129.
    // Render Terrain Tiles
    for(UINT i = 0; i < m_vVisibilityList.size(); i ++) 
    {   QTNode *n = m_vVisibilityList[i];
        t = n->pTile;

        if(t->bTerrainVisible) // This is fals if Terrain is not loaded yet
        {
            int LODindex = (int)t->LOD;

            if(b != t->b) { // batch renders by Vertex Buffer. Fastest way through this.
                b = t->b;
                V(m_pd3dDevice->SetStreamSource(0, b->pVB, 0, sizeof(TERRAINVERT)));
            }                        

            if(m_bDrawTerrain) // draw terrain
		    {	
                //! Set IB
                if(prevLODindex != LODindex) // only get IB if different LOD
					V(m_pd3dDevice->SetIndices(tileIBlist[LODindex]));
                // Set the Tween            
                if(t->LOD != prevLOD) { // commit tween changes if different
                    float tween[2];
                    if(LODindex == 0) {
                        tween[0] = t->LOD - LODindex;
                        tween[1] = 0;                    
                    }
                    else {
                        tween[0] = 0;
                        tween[1] = t->LOD - LODindex;                    
                    }                
                    V( effect->SetFloatArray(tweenHandle, tween, 2) );
                    V( effect->CommitChanges() );
                    prevLOD = t->LOD;
                }
                // Draw IB
			    V(m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, t->offset, 0, TILEAREA, 0, tileSizeList[LODindex]) );
                prevLODindex = LODindex;
		    }       
        }
	}

    //
    // Render Tile Skirts
    //
    // Draw Skirts

    static const int MipmapTilesize2 = MIPMAP_TILESIZE*2;
    int BlockWidth2 = 0;

	int stripSize = 0;
	V(m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

	prevLODindex = -1;
	prevLOD = -1.0f;
	b = NULL;

	for(UINT i = 0; i < m_vVisibilityList.size(); i ++)
	{   QTNode *n = m_vVisibilityList[i];
		t = n->pTile;

        if(t->bTerrainVisible)
        {
		    int LODindex = (int)t->LOD;

		    if(b != t->b) { // batch renders by Vertex Buffer. Fastest way through this.
			    b = t->b;
			    V(m_pd3dDevice->SetStreamSource(0, b->skirtBuffer, 0, sizeof(TERRAINVERT)));
                BlockWidth2 = b->nDimension*2;
		    }

		    //! Set IB
		    if(prevLODindex != LODindex) // only get IB if different LOD
			    V(m_pd3dDevice->SetIndices(skirtIBlist[LODindex]));
		    // Set the Tween            
		    if(t->LOD != prevLOD) { // commit tween changes if different
			    float tween[2];
			    if(LODindex == 0) {
				    tween[0] = t->LOD - LODindex;
				    tween[1] = 0;                    
			    }
			    else {
				    tween[0] = 0;
				    tween[1] = t->LOD - LODindex;                    
			    }                
			    V( effect->SetFloatArray(tweenHandle, tween, 2) );
			    V( effect->CommitChanges() );
			    prevLOD = t->LOD;
		    }
		    // Draw IB
    	
			stripSize = skirtSizeList[LODindex];
		    V(m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, t->vertSkirtOffset, 0, MipmapTilesize2, 0, stripSize-2) );
		    V(m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, t->vertSkirtOffset+BlockWidth2, 0, MipmapTilesize2, 0, stripSize-2) );
			V(m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, t->horzSkirtOffset, 0, MipmapTilesize2, 0, stripSize-2) );
		    V(m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, t->horzSkirtOffset+BlockWidth2, 0, MipmapTilesize2, 0, stripSize-2) );
    	
		    prevLODindex = LODindex;
        }
	}

	V(m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW));
    return EXIT_SUCCESS;
}

//! Updates everything in visible nodes such as clouds and foliage with their impostors and actors.
VOID CQuadTree::UpdateObjects(const float fElapsedtime,
                              const D3DXMATRIX* pMatProj, 
                              const D3DXMATRIX* pMatView, 
                              const D3DVIEWPORT9* pViewport, 
                              const D3DXVECTOR3* pEyePt, 
                              const D3DXVECTOR3* pUp,
                              const D3DXVECTOR3* pRight,
                              const D3DXVECTOR3* pAhead)
{
#if 0   // visibilty test for pillar
    // Determine visibility of Pillar
    if(AABBFrustumTest(m_pPillar->GetCorners() == AABB_OUTSIDE)
        m_bDrawPillar = false;
    else
        m_bDrawPillar = true;
#endif
    

#if 0   // NOFOLIAGE
    // Update Foliage
    m_pFoliage->OnFrameMove(fElapsedtime, m_pCloudSky->GetLightDir(), pMatProj, pMatView);
#endif
}

//! Calculations to be done before Render
VOID CQuadTree::OnFrameMove( 
    const D3DXVECTOR3* pEyePt,
    const D3DXVECTOR3* pUp,
    const D3DXVECTOR3* pRight,
    const D3DXVECTOR3* pAhead,
    const D3DXMATRIX* pMatProj,
    const D3DXMATRIX* pMatView,
    const D3DVIEWPORT9 * pViewport,
    const double fTime,
    const float fElapsedTime)
{
    HRESULT hr;    

    // Load Into Video Memory any New Data.
    V( LoadNewData( pEyePt, fTime ) );

    // View Frustum Culling that creates a list of visible nodes in ISpatialSub    
    V( ViewFrustumCull( pMatView, pMatProj ) );

	// Give every visible quad its LOD based on camera coordinates, and get distance from camera  
    V( DetermineDistanceLOD( pEyePt, pMatView, pMatProj, pViewport) );    

    // Updates Foliage.
    UpdateObjects(fElapsedTime, pMatProj, pMatView, pViewport, pEyePt, pUp, pRight, pAhead);
}

//! Rendering function
HRESULT CQuadTree::OnFrameRender(
    const D3DXVECTOR3* pEyePt,
    const D3DXVECTOR3* pUp,
    const D3DXVECTOR3* pRight,
    const D3DXVECTOR3* pAhead,
    const D3DXMATRIX* pMatProj,
    const D3DXMATRIX* pMatView,
    const D3DXMATRIX* pMatShadow,
    LPDIRECT3DTEXTURE9 pShadowMap,
    UINT param)
{
    HRESULT hr;
    D3DXMATRIX matViewProj;
    matViewProj = (*pMatView) * (*pMatProj);	

    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, pMatProj );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, pMatView );  
    //m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_mIdentity );

    // Batch Render Terrain
    V_RETURN( DrawTerrain(matViewProj, pMatShadow, pShadowMap) );

    // Batch Render Objects ( Foliage/Grass/Trees )
    // DrawObjects(pEyePt, pUp, pRight, pAhead, pMatView, pMatProj, &matViewProj);

    if( m_bDrawPillar )
    {   // be careful, we don't set the view/proj matrix. we assume above its set.
	    DrawBasin();
	    V_RETURN( m_pPillar->OnFrameRender(m_pd3dDevice) );
    }

    if(m_bDrawBoundingBoxes)
        V_RETURN( DrawBoundingBoxes() );	

    return S_OK;
}

//! Used to create any D3DPOOL_DEFAULT Devices. All EFFECTS inside program MUST do this.
HRESULT CQuadTree::OnResetDevice(IDirect3DDevice9* pd3dDevice) {
    HRESULT hr;

    m_pd3dDevice = pd3dDevice;

    V_RETURN( m_pTerrain->OnResetDevice() );    
    // m_pFoliage->OnResetDevice(pd3dDevice);
	  m_pPillar->OnResetDevice(pd3dDevice);
	  CreateBasin();
    return S_OK;
}

//! Used to RECREATE any D3DPOOL_DEFAULT Devices Lost on minimize etc. All EFFECTS inside program MUST do this.
HRESULT CQuadTree::OnLostDevice() {
    HRESULT hr;

    V_RETURN( m_pTerrain->OnLostDevice() );    
    // m_pFoliage->OnLostDevice();
	  V_RETURN( m_pPillar->OnLostDevice() );
	  SAFE_RELEASE(m_pBasinIndices);
	  SAFE_RELEASE(m_pBasinVertices);
	  SAFE_RELEASE(m_pBasinTexture);

    return S_OK;
}

//! Displaces QuadTree by translation and expands Quad Tree by CellSpacing/HeightSpacing amount
VOID CQuadTree::DisplaceExpandQuadTree(QTNode* n, UINT cellSpacing, UINT heightSpacing) {
    if(n == NULL)
        return ;

    // Stretch
    n->midx *= cellSpacing;
    n->midz *= cellSpacing;
    n->length *= cellSpacing;
    n->maxy *= heightSpacing;
    n->midy *= heightSpacing;
    n->miny *= heightSpacing;
    
    for(int i=0;i<8;i++) {
        n->corners[i].x *= (float)cellSpacing;
        n->corners[i].y *= (float)heightSpacing;
        n->corners[i].z *= (float)cellSpacing;
    }

    // Displace
    n->midx += (int)m_vTranslation.x;
    n->midy += (int)m_vTranslation.y;
    n->midz += (int)m_vTranslation.z; 
    
    for(int i=0;i<8;i++) {
        n->corners[i].x += m_vTranslation.x;
        n->corners[i].y += m_vTranslation.y;
        n->corners[i].z += m_vTranslation.z;
    }

    if(n->bLeaf) { // Expand TerrainTile in QTNode n
        if(n->pTile->maxy < 0.f)    n->pTile->maxy = 0.f;
        else    n->pTile->maxy *= heightSpacing;
        if(n->pTile->miny < 0.f)    n->pTile->miny = 0.f;
        else    n->pTile->miny *= heightSpacing;
    }
    else { // expand children
        DisplaceExpandQuadTree(n->bl, cellSpacing, heightSpacing);
        DisplaceExpandQuadTree(n->br, cellSpacing, heightSpacing);
        DisplaceExpandQuadTree(n->tl, cellSpacing, heightSpacing);
        DisplaceExpandQuadTree(n->tr, cellSpacing, heightSpacing);
    }
}

//! Uses the screen space error between the maxErrorVertex and highest LOD to determine tile LOD.
UINT CQuadTree::DetermineDistanceLOD(const D3DXVECTOR3* pEyePt,
                             const D3DXMATRIX* pMatView, 
                             const D3DXMATRIX* pMatProj,
                             const D3DVIEWPORT9* pViewport) {

    UINT nLODs = m_pTerrain->GetNumberOfLODs();
	for(UINT i = 0; i < m_vVisibilityList.size(); i ++) 
    {
        QTNode *n = m_vVisibilityList[i];
		Tile *t = n->pTile;

        // Get 2D manhattan Distance from Camera to quad
        n->distancex = fabs(n->midx - pEyePt->x);
        n->distancez = fabs(n->midz - pEyePt->z);

        if(m_bLockLOD) { // check if LOD is locked, if so set each tiles LOD
            t->LOD = (float)m_LockedLOD;
            continue;
        }

        //
        // Find distance LOD for terrain
        //
        static float inverseThresh = 1 / m_LODDistanceThreshold;
        float distanceLOD = (n->distancex + n->distancez) * inverseThresh;        

        //
        // Find screen space error LOD
        //
        D3DXVECTOR3 VecLOD0, VecLOD1;
        UINT indexLOD = nLODs - 1;

        // Project maxErrorPoint on to screen space. Currently using all axis, viable for optimization.
        D3DXVec3Project(&VecLOD0, &t->maxErrorPoints[0], pViewport, pMatProj, pMatView, &m_mWorld);
        D3DXVec3Project(&VecLOD1, &t->maxErrorPoints[indexLOD], pViewport, pMatProj, pMatView, &m_mWorld);
        // difference between LOD 0 and max LOD.
        float delta = abs(VecLOD1.x - VecLOD0.x) + abs(VecLOD1.y - VecLOD0.y) + abs(VecLOD1.z - VecLOD0.z); // z's delta is negligible
        
        float screenLOD = 0.0f;

        // if within thresh, just go to worst detail.
        if(delta < m_LODThreshold)
            screenLOD = (float)indexLOD;

        else { // set tween and LOD
            for(UINT i = 1; i < nLODs; i++) {
                delta -= m_LODThreshold;
                if(delta < m_LODThreshold) { // within this LOD, so set LOD/tween and break
                    float tween = 1 - delta / m_LODThreshold;
                    screenLOD = (indexLOD - i) + tween;
                    break;
                }
            }
        }  // end else
        t->LOD = (screenLOD < distanceLOD) ? screenLOD : distanceLOD; // assign least one
    }
	return EXIT_SUCCESS;
}

HRESULT CQuadTree::DrawBoundingBoxes()
{
    HRESULT hr;
    ULONG OldStates[6];
    D3DXMATRIX matIdentity;

    D3DXMatrixIdentity(&matIdentity);    

    // Retrieve old states
    V_RETURN( m_pd3dDevice->GetRenderState( D3DRS_ZWRITEENABLE, &OldStates[0] ) );
    V_RETURN( m_pd3dDevice->GetRenderState( D3DRS_ZENABLE, &OldStates[1] ) );
    V_RETURN( m_pd3dDevice->GetRenderState( D3DRS_LIGHTING, &OldStates[2] ) );
    V_RETURN( m_pd3dDevice->GetRenderState( D3DRS_COLORVERTEX, &OldStates[3] ) );
    V_RETURN( m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &OldStates[4] ) );
    V_RETURN( m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &OldStates[5] ) );

    // Setup new states
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE ) ); 
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE ) );
    V_RETURN( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE ) );
    V_RETURN( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 ) );

    V_RETURN( m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) );    

    // draw the stuff //
    V_RETURN( DrawQuadBBs(D3DCOLOR_XRGB(0,0,255)) ); // blue
    V_RETURN( DrawCloudBBs(0xffff0000) ); // red

    // Reset old states
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, OldStates[0] ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, OldStates[1] ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, OldStates[2] ) );
    V_RETURN( m_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, OldStates[3] ) );
    V_RETURN( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, OldStates[4] ) );
    V_RETURN( m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, OldStates[5] ) );

    V_RETURN( m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) );

    // Reset the matrix
    V_RETURN( m_pd3dDevice->SetTransform( D3DTS_WORLD, &matIdentity ) );
    return S_OK;
}

HRESULT CQuadTree::DrawCloudBBs(ULONG color)
{
    HRESULT hr;
    // create array of appropriate size. uses set to stop multiple entries.
    set<CCloud*> setClouds;
    for(size_t i = 0; i < m_vVisibilityList.size(); i++) {
        QTNode *n = m_vVisibilityList[i];
        for(size_t j=0; j<n->vpClouds.size(); j++)
            setClouds.insert(n->vpClouds[j]);
    }
    int nClouds = (int)setClouds.size();
    int numLines = 12;

    if(nClouds <= 0)
        return EXIT_SUCCESS;

    BoxVertColor *verts = new BoxVertColor[nClouds * numLines * 2]; // nClouds with numLines per BB with 2 vertices per line.

    set<CCloud*>::iterator it;
    int count = 0;
    for(it = setClouds.begin(); it != setClouds.end(); it++) {
        int index = count*numLines*2;
        gdef::GenerateBoundingBox(&verts[index], (*it)->GetCorners());        
        for(int j=0;j<numLines*2;j++)
            verts[index+j].Color = color;
        count++;
    }

    // Draw
    V_RETURN( m_pd3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE ) );
    V_RETURN( m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)(nClouds*numLines), verts, sizeof(BoxVertColor)) );

    SAFE_DELETE_ARRAY(verts);
    return S_OK;
}

//! PREREQ: OnCreateDevice called (to set m_pd3dDevice) and ViewFrustumCull called to set visible BBs
HRESULT CQuadTree::DrawQuadBBs(ULONG Color) {
    if(m_vBoundingBoxes.size() == 0)
        return 0;
    HRESULT hr;

    int numLines = 4*3;
    BoxVertColor *verts = new BoxVertColor[m_vBoundingBoxes.size()*numLines*2]; // every box has 4*3 lines which each have 2 vertices

    for(UINT i = 0; i < m_vBoundingBoxes.size(); i++) {
        int index = i*numLines*2;
        gdef::GenerateBoundingBox(&verts[index], m_vBoundingBoxes[i]->corners);
        for(int j=0;j<numLines*2;j++)
            verts[index+j].Color = Color;
    }

    // Draw
    V_RETURN( m_pd3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE ) );
    V_RETURN( m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)(m_vBoundingBoxes.size()*numLines), verts, sizeof(BoxVertColor)) );

    SAFE_DELETE_ARRAY(verts);
    return S_OK;
}

/*************************************** QTNode Functions *********************************/

CQuadTree::QTNode::QTNode()    
{
    memset(this, 0, sizeof(QTNode));
}

CQuadTree::QTNode::QTNode(int _midx, int _midz, int _length, int _bottomheight, int _topheight) :
    tl(NULL), tr(NULL), bl(NULL), br(NULL), north(NULL), south(NULL), east(NULL), west(NULL),
    midx(_midx), midz(_midz), length(_length), maxy(_topheight), miny(_bottomheight), 
    bLeaf(false), bBlock(false), pBlock(NULL), pTile(NULL), distancex(0.0f), distancez(0.0f),
    midy((maxy-miny)/2)
{                
    // Generate Corner Vectors. 
    //  4 --- 5                3 --- 2 - top
    //  |     |                |     |
    //  0 --- 1  - bottom      7 --- 6
    int diff = (length >> 1);

    float fmaxy = (float)maxy;
    float fminy = (float)miny;
    float minx = (float)(midx - diff);
    float minz = (float)(midz - diff);
    float maxx = (float)(midx - diff + length);
    float maxz = (float)(midz - diff + length);

    corners[0] = D3DXVECTOR3(minx, fminy, minz); // front bottom left
    corners[1] = D3DXVECTOR3(maxx, fminy, minz); // front bottom right
    corners[2] = D3DXVECTOR3(maxx, fmaxy, maxz); // back top right
    corners[3] = D3DXVECTOR3(minx, fmaxy, maxz); // back top left
    corners[4] = D3DXVECTOR3(minx, fminy, maxz); // back bottom left
    corners[5] = D3DXVECTOR3(maxx, fminy, maxz); // back bottom right
    corners[6] = D3DXVECTOR3(maxx, fmaxy, minz); // front top right
    corners[7] = D3DXVECTOR3(minx, fmaxy, minz); // front top left
}
CQuadTree::QTNode::~QTNode() {
    SAFE_DELETE( tl );
    SAFE_DELETE( tr );
    SAFE_DELETE( bl );
    SAFE_DELETE( br );
	if(bBlock)	//if this is highest level of where a block is
	{	SAFE_DELETE( pBlock);
	}
	SAFE_DELETE( pTile);    
}

//! Pushes back the nodes that contain or partially contain the center + length
VOID CQuadTree::QTNode::GetLowestNodes(vector<QTNode*> &vNodes, QTNode* top, 
                                       const D3DXVECTOR3 &center, float lengthx, float lengthy, float lengthz)
{
    float halflength = (float)top->length / 2.f;
    float halfheight = (float)(top->maxy-top->miny) / 2.f;

    bool bIntersect = gdef::IsBoxIntersect3D(center, D3DXVECTOR3(lengthx, lengthy, lengthz), // object center/lengths
            D3DXVECTOR3((float)top->midx, (float)top->midy, (float)top->midz), // node center
            D3DXVECTOR3(halflength, halfheight, halflength));   // node lengths

    if(top->bLeaf) 
    {
        if(bIntersect)
            vNodes.push_back(top);
        else
            return ;
    }
    else if(bIntersect)
    {
        if(top->bl)
            GetLowestNodes(vNodes, top->bl, center, lengthx, lengthy, lengthz);
        if(top->br)
            GetLowestNodes(vNodes, top->br, center, lengthx, lengthy, lengthz);
        if(top->tl)
            GetLowestNodes(vNodes, top->tl, center, lengthx, lengthy, lengthz);
        if(top->tr)
            GetLowestNodes(vNodes, top->tr, center, lengthx, lengthy, lengthz);
    }
}

/*************************************** Tiles Functions **********************************/

//! Recursively generates tiles up to a minimum size of tileSize*tileSize. NOTE: Only Leaves have Tiles!
VOID CQuadTree::TileWorker(QTNode *n, Block *b, bool bLoad) {    
    if(n == NULL)
        return ;

    if(n->length + 1 <= CMipmapTerrain::BLOCK_WIDTH && b == NULL) {
        n->bBlock = true;
        if(bLoad) {
            // Load terrain block!                
	        int wIncr = n->length >> 1;
	        int hIncr = n->length >> 1;
            
            int xposmin = (n->midx - wIncr);
            int zposmin = (n->midz - hIncr);
            int xposmax = (n->midx + wIncr);
            int zposmax = (n->midz + hIncr);
            
            // Only make terrain tile if within terrain
            if(xposmin >= 0 && zposmin >= 0 && xposmax < (int)m_Width && zposmax < (int)m_Length) 
            {	    
	            b = new Block(
                            xposmin, 
                            xposmax,
                            zposmin,
                            zposmax,
                            m_pd3dDevice, 
                            m_pTerrain,
                            m_Width,
                            n->length + 1);	  
            }
        }
    }
    n->pBlock = b;

    if(n->bLeaf) {
        int blockWidth = n->pBlock->nDimension;
		// int blockWidth = CMipmapTerrain::BLOCK_WIDTH;
		int absHorzOff = (blockWidth * ((blockWidth-1)/(MIPMAP_TILESIZE-1) + 1))*2;
        int xVal = (n->midx-n->length/2)%(blockWidth-1);	//find xy position relative to its BLOCK
		int yVal = (n->midz-n->length/2)%(blockWidth-1);

		int pillarSize = MIPMAP_TILESIZE*blockWidth;	//find how many "pillars" it has to skip
		int pillarOffset = (xVal/(MIPMAP_TILESIZE-1))*pillarSize;	//number needed to skip pillars
		int yOffset = yVal*(MIPMAP_TILESIZE);					//climb yVal units up the pillar

		//calculate offset for skirts
		int vertSkirtOffset = (yVal + ((xVal + MIPMAP_TILESIZE-1)/MIPMAP_TILESIZE)*blockWidth)*2;	
		int horzSkirtOffset = (xVal + ((yVal + MIPMAP_TILESIZE-1)/MIPMAP_TILESIZE)*blockWidth)*2 + absHorzOff;

		n->pTile = new Tile();
		Tile* t = n->pTile;
		t->offset = pillarOffset+yOffset;							//offset into terrain buffer
		t->horzSkirtOffset = horzSkirtOffset;
		t->vertSkirtOffset = vertSkirtOffset;
		
		m_pTerrain->GetMaxErrorVertexAtTile(n->corners[0], t->maxErrorPoints, t->maxErrorIndex);
        m_pTerrain->GetMinMaxHeight(n->corners[0], t->miny, t->maxy); // Get highest/lowest point in tile.

        t->b = b;
        return ;
    }

    // If Terrain is not loaded here it is loaded in CQuadTree::LoadNewData() as user moves.     

    TileWorker(n->bl, b, bLoad);
    TileWorker(n->br, b, bLoad);
    TileWorker(n->tl, b, bLoad);
    TileWorker(n->tr, b, bLoad);
}

/*************************************** Block Functions **********************************/

CQuadTree::Block::Block(UINT minX, 
                        UINT maxX, 
                        UINT minY, 
                        UINT maxY, 
                        IDirect3DDevice9 *dev, 
                        ITerrain* pTerrain, 
                        UINT Width, 
                        UINT blockWidth)
{	// Grotesque speed hack. statics only initialized once so prevents recalculation.
	static UINT tileWidth = MIPMAP_TILESIZE;
	static UINT tilesAlongBlock = (blockWidth-1)/(tileWidth-1);	

    int cellSpacing = pTerrain->GetCellSpacing();
    int heightSpacing = pTerrain->GetHeightSpacing();

    nDimension = blockWidth;
    UINT numVertices = blockWidth*(blockWidth+tilesAlongBlock-1);
    // this function is just ew.
	
    HRESULT hr;
	pVB = NULL;

	TERRAINVERT *vertices; 
	//create vertex data

	V(dev->CreateVertexBuffer(
		numVertices*sizeof(TERRAINVERT),
		D3DUSAGE_WRITEONLY,
        0, // we use vertex declarations. no fvfs
		D3DPOOL_MANAGED,
		&pVB,
		0) );

	UINT count = 0;
	UINT xOffset = minX;

    // Texture coordinates are calculated in vertex shader to reduce memory requirements.

	V(pVB->Lock(0, 0, (void**)&vertices, 0) );
	for(UINT tileNo = 0; tileNo < tilesAlongBlock; tileNo ++)
	{	for(UINT j = minY; j <= maxY; j ++)
		{	int yPos = j*Width;
				for(UINT i = 0; i < tileWidth; i ++)
				{	UINT xPos = i + xOffset;
                    D3DXVECTOR3 n = pTerrain->GetNormalAt(xPos+yPos);

                    float actualHeight = pTerrain->GetHeightAt(xPos+yPos)*heightSpacing;

					vertices[count++] =  TERRAINVERT(
                                            (float)xPos*cellSpacing, // x
                                            actualHeight, // y
                                            (float)j*cellSpacing, // z                                            
                                            n.x, n.y, n.z, // normals                                            
                                            pTerrain->GetDiffAtLOD(0, xPos+yPos), // w - used to geomorph from low to med detail
                                            pTerrain->GetDiffAtLOD(1, xPos+yPos)
                                            );
				}
		}
		xOffset += tileWidth-1;
	}
	V( pVB->Unlock() );
	assert(count == numVertices);

	//now set up skirt buffer
	numVertices = (blockWidth * ((blockWidth-1)/(MIPMAP_TILESIZE-1) + 1))*4;
	//create vertex data

	V(dev->CreateVertexBuffer(
		numVertices*sizeof(TERRAINVERT),
		D3DUSAGE_WRITEONLY,
        0, // we use vertex declarations. no fvfs
		D3DPOOL_MANAGED,
		&skirtBuffer,
		0) );

	count = 0;
	xOffset = minX;

	V(skirtBuffer->Lock(0, 0, (void**)&vertices, 0) );
	for(UINT i = 0; i < blockWidth; i += MIPMAP_TILESIZE-1)
	{	for(UINT j = minY; j <= maxY; j ++)
		{	UINT yPos = j*Width;
			UINT xPos = i + xOffset;
            D3DXVECTOR3 n = pTerrain->GetNormalAt(xPos+yPos);

            float actualHeight = pTerrain->GetHeightAt(xPos+yPos)*heightSpacing;

			vertices[count++] =  TERRAINVERT(
                                    (float)xPos*cellSpacing, // x
                                    actualHeight, // y
                                    (float)j*cellSpacing, // z                                            
                                    n.x, n.y, n.z, // normals                                    
                                    pTerrain->GetDiffAtLOD(0, xPos+yPos), // w - used to geomorph from low to med detail
                                    pTerrain->GetDiffAtLOD(1, xPos+yPos)
                                    );

			vertices[count++] =  TERRAINVERT(
                                    (float)xPos*cellSpacing, // x
									actualHeight - SKIRT_DEPTH, // y
                                    (float)j*cellSpacing, // z                                            
                                    n.x, n.y, n.z, // normals                                    
                                    0, // w - used to geomorph from low to med detail
                                    0
                                    );
		}
	}
	
	xOffset = minX;

	for(UINT j = minY; j <= maxY; j += MIPMAP_TILESIZE-1)
	{	UINT yPos = j*Width;
		for(UINT i = 0; i < blockWidth; i ++)
		{	UINT xPos = i + xOffset;
            D3DXVECTOR3 n = pTerrain->GetNormalAt(xPos+yPos);

            float actualHeight = pTerrain->GetHeightAt(xPos+yPos)*heightSpacing;

			vertices[count++] =  TERRAINVERT(
                                    (float)xPos*cellSpacing, // x
                                    actualHeight, // y
                                    (float)j*cellSpacing, // z                                            
                                    n.x, n.y, n.z, // normals                                    
                                    pTerrain->GetDiffAtLOD(0, xPos+yPos), // w - used to geomorph from low to med detail
                                    pTerrain->GetDiffAtLOD(1, xPos+yPos)
                                    );

			vertices[count++] =  TERRAINVERT(
                                    (float)xPos*cellSpacing, // x
									actualHeight - SKIRT_DEPTH, // y
                                    (float)j*cellSpacing, // z
                                    n.x, n.y, n.z, // normals                                    
                                    0, // w - used to geomorph from low to med detail
                                    0
                                    );
		}
	}
	V(skirtBuffer->Unlock());
	assert(count == numVertices);
}

////////////////////////////////////// Load Area Functions /////////////////////////////////

CQuadTree::CLoadArea::CLoadArea(UINT cellSpacing) :         
        Load_Dimension(LOAD_AREA_DIMENSION * cellSpacing)
{
}

//! check if HMP can fit in entire load area, then set bFitsInArea = true;
VOID CQuadTree::CLoadArea::CheckFitsInArea(int dim) {
    if(dim <= Load_Dimension)  
        bFitsInArea = true;
    else
        bFitsInArea = false;
}

int CQuadTree::CLoadArea::PopulateLoadArea(QTNode *top, float x, float z)
{
    int count = 0;
    InLoadAreaSet.clear();
    // Traverse quadtree by checking which Block QTNodes are in m_LoadArea
    SetCenter(x,z);
    PopulateWorker(top); // sets bAllLoaded if it all fits.
    
    // Compare InLoadAreaSet with Prev one and fill up ToLoad vector.
    set<QTNode *, QTNodeLessThanFunctor>::iterator it, fit;    
    for(it = InLoadAreaSet.begin(); it != InLoadAreaSet.end(); it++)
    {
        fit = PrevInLoadAreaSet.find(*it);
        if(fit == PrevInLoadAreaSet.end()) { // not found, so load
            ToLoad.push_back(*it);
            count ++; // don't trust STL list.size() function. might iterate thru list.
        }
        // else found, so keep, don't delete.        
    }    
    // Compare PrevInLoadAreaSet with current one and fill up ToUnload vector.
    for(it = PrevInLoadAreaSet.begin(); it != PrevInLoadAreaSet.end(); it++)
    {
        fit = InLoadAreaSet.find(*it);
        if(fit == InLoadAreaSet.end()) // not found in current, so unload.
            ToUnload.push_back(*it);
        // else found, so keep, don't delete.
    }
    PrevInLoadAreaSet = InLoadAreaSet;
    return count;
}

//! Checks the 4 corners against quads in QuadTree
VOID CQuadTree::CLoadArea::PopulateWorker(QTNode *n)
{    
    if(n == NULL) return ;
    float thresh = (float)(Load_Dimension >> 1) ;
    float distancex = abs(n->midx - m_x);
    float distancez = abs(n->midz - m_z);
    if(n->bBlock) { // just check this block and descend no further
        if(distancex < thresh && distancez < thresh) { // Block within load area
            InLoadAreaSet.insert(n);       
        }
        // else not in load area, stop recursion.
    }
    else {        
        float halflen = (float)(n->length >> 1);
        if(distancex < thresh + halflen && distancez < thresh + halflen) 
        { // node inside  or spanning load area
            PopulateWorker(n->bl);
            PopulateWorker(n->br);
            PopulateWorker(n->tl);
            PopulateWorker(n->tr);
        }
    }
}

//! Returns next block QTNode in STL queue.
CQuadTree::QTNode * CQuadTree::CLoadArea::ReturnNextBlock()
{
    if(ToLoad.size() <= 0) return NULL;

    QTNode *n = *ToLoad.begin();
    ToLoad.pop_front();
    assert(n->bBlock == true);

    if(n->pBlock == NULL)
        return n;
    else // already loaded by some ridiculous luck.
        return NULL;
}

UINT CQuadTree::CLoadArea::UnloadBlocks()
{
    list<QTNode *>::iterator it = ToUnload.begin();
    for( ; it != ToUnload.end(); it++) {
        QTNode *n = *it;
        assert(n->bBlock == true);
        SAFE_DELETE( n->pBlock );    
        SetBlockPointers(n, NULL);
    }
    ToUnload.clear();
    return S_OK;
}

//! Sets block pointers at node and beneath to null. Sets leaves tiles' pointers to blocks null.
VOID CQuadTree::CLoadArea::SetBlockPointers(QTNode *n, Block *b) 
{
    if(n == NULL)   return ;

    if(n->bBlock)
        n->pBlock = b;
    if(!n->bLeaf) 
    {
        SetBlockPointers(n->bl, b);
        SetBlockPointers(n->br, b);
        SetBlockPointers(n->tl, b);
        SetBlockPointers(n->tr, b);
    }
    else // leaf
        n->pTile->b = b;
}