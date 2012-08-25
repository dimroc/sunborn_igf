#pragma once
#include "FXDecl.h"
#include "Pillar.h"
#include "globaldefine.h"

//////////////////////////////////////////////////

using std::vector;
using std::list;
using std::set;
using std::deque;
using std::string;
using gdef::CornerIndexPair;

#define LOD_VERTEX_THRESHOLD                    4.0f // screen space error in pixels (i think)
#define LOD_DISTANCE_MULTIPLIER                 2.5f // mulitplier to tile distance from camera LOD tweening

//////////////////////////////////
// forward declaration for pointers to reduce compiler dependency
//////////////////////////////////
class CCloud; 
class CCloudSky;
class CBubbles;

class ITerrain;

class CFoliage;
class CPillar;
struct SGrass;

//
//! Quad tree data structure. Owner of a CMipmapTerrain and a variety of objects.
//
class CQuadTree
{    
    friend class CWorld;
    friend class CCloudSky;
    friend class CFoliage;

public:
	typedef CPillar::SPillarInfo SPillarInfo;
    //! Struct to hold Island filename/constructor info from level/world file.    
    struct SQuadTreeInfo {
        string strHMP;
        string strBlendMap;
        string strRedTexture, strGreenTexture, strBlueTexture, strAlphaTexture;
        string strPillarTexture;

        int nCellSpacing, nHeightSpacing;
        float fTextureTile;
        D3DXVECTOR3 vTranslation;
    };
private:        
    const UINT MIN_DIMENSION;	 
    const UINT TILEAREA;
    const float m_LODThreshold;
    const float m_LODDistanceThreshold;    

    /////////////////////////// STRUCTS USED BY QUADTREE //////////////////////////////////    

	struct Block {
		IDirect3DVertexBuffer9 *pVB;
		IDirect3DVertexBuffer9 *skirtBuffer;
        int nDimension;

		Block() : pVB(NULL), skirtBuffer(NULL){};
		Block(UINT minX, UINT maxX, UINT minY, UINT maxY, IDirect3DDevice9 *dev, ITerrain* pTerrain, UINT Width, UINT blockWidth);
		~Block() { SAFE_RELEASE(pVB); SAFE_RELEASE(skirtBuffer); }
	};

	struct Tile {
		int offset; // offset in VB
		int vertSkirtOffset; //offset for drawing vertical skirt
		int horzSkirtOffset; //offset for drawing horizontal skirt
        int maxErrorIndex;
        bool bTerrainVisible; // although QTNode might be visible, the Terrain Tile might not be.
        float LOD, miny, maxy; // LOD, lowest/highest terrain point
		Block *b;
		vector<D3DXVECTOR3> maxErrorPoints; //! Error Vertex per LOD

		Tile() : LOD(0.0f), offset(0), miny(0.0f), maxy(0.0f), bTerrainVisible(false), b(NULL), vertSkirtOffset(0), horzSkirtOffset(0) {}        
	};

    //
    //! Quad tree node has four children, a mid point, a length, plus pointers to data.
    struct QTNode {
        QTNode *tl, *tr, *bl, *br; //! four children        
		QTNode *north, *south, *east, *west; //! Pointer to neighbors. TODO: Remove since not needed for strips and not made properly.
        int midx, midz, length, maxy, miny, midy;    //! mid point and length of quad
        
        //! NOTE: Only defined in Leaf nodes!
        float distancex, distancez; //! Absolute Distance from camera when last visible. Updated in DetermineDistanceLOD.       

        bool bLeaf, bBlock; //! bBlock is only true if QTNode is of 129x129 AND lies on an area with terrain. (eg: rectangular HMPs have some nodes with nothing on it!)
		
        // Generate Corner Vectors. 
        //  4 --- 5                3 --- 2 - top
        //  |     |                |     |
        //  0 --- 1  - bottom      7 --- 6
        D3DXVECTOR3 corners[8]; //! store corners for faster tree traversal

        Block *pBlock; //! pointer to block struct holding VBs for Tiles
	    Tile *pTile; //! pointer to Tile which holds data for different LODs

        vector<CCloud*> vpClouds;   //! Clouds in the _Leaf_ node.
        vector<SGrass*> vpGrass;    //! Grass objects in leaf node.
        
        // Need to figure out how to batch render objects. Make sure it's inserted coupled in order?
        // Or just go through nEffects times? 
        //****THIS IS WHY I AM SEPARATING INTO OWN VECTORS WITH NO INHERITCANE. BATCH RENDERING.

        QTNode();   //! Constructor. Sets up pointers to null and variables to 0.
        QTNode(int _midx, int _midz, int _length, int _bottomheight, int _topheight); 
        ~QTNode(); //! Cleans up all QTNodes and Tiles/Blocks therein

		//! draws terrain Tile  based on LOD sent in via IB and nTriangles.
        VOID Draw(IDirect3DDevice9 *Device, IDirect3DIndexBuffer9 *pIB, int nTriangles, int tileArea);
        //! Finds the lowest nodes that encompass the point + length
        static VOID GetLowestNodes(vector<QTNode*> &vNodes, QTNode* top, 
            const D3DXVECTOR3 &center, float lengthx, float lengthy, float lengthz);
    };
    
    //! Binary Functor used in CLoadArea in a Set.
    class QTNodeLessThanFunctor  {
    public:
        bool operator()(const QTNode* lhs, const QTNode* rhs) const {
            return (lhs->midx < rhs->midx) || (lhs->midx == rhs->midx && lhs->midz < rhs->midz);
        }
    };
    //! Handles the loading of Blocks from RAM to VRAM as player moves.
    class CLoadArea {        
    private:
        float m_x, m_z;
        bool bFitsInArea;
        const int Load_Dimension;
        set<QTNode *, QTNodeLessThanFunctor> InLoadAreaSet, PrevInLoadAreaSet;
        list<QTNode *> ToLoad, ToUnload;

        VOID SetCenter(float _x, float _z) { m_x = _x; m_z = _z; }        
        VOID PopulateWorker(QTNode *top);
    public:
         //! Sets LoadDimension.
        CLoadArea(UINT cellSpacing);
        ~CLoadArea() {}
        VOID CheckFitsInArea(int dim); //! Sets bFitsInArea if the largest dimension fits in LoadArea

        int PopulateLoadArea(QTNode *n, float x, float z); //! Populates the ToLoad and ToUnload vectors used in Un/LoadArea
        QTNode * ReturnNextBlock(); //! Loads an Area in ToLoad if any.
        UINT UnloadBlocks(); //! Deletes an Area in Unload if any.
        //! Sets block pointers at node and beneath to null. Sets leaves tiles' pointers to blocks null.
        VOID SetBlockPointers(QTNode *n, Block *b);         
        bool IsFitsInArea() { return bFitsInArea; } //! Returns whether entire terrain is loaded in VRAM.        
    private:
        CLoadArea& operator=(const CLoadArea &rhs); // disallow assignment operator and shutup warning
    };
    //////////////////////////// END OF STRUCTS //////////////////////////////



    ///////////////////////////// PRIVATE MEMBERS VARIABLES /////////////////////////

    UINT m_Width, m_Length, m_Height;
    bool m_bInit, m_bDrawBoundingBoxes;

    bool m_bLockLOD, m_bDrawTerrain, m_bDrawPillar;
    UINT m_LockedLOD;

    LPDIRECT3DDEVICE9 m_pd3dDevice; //! NOT THE OWNER OF pd3dDevice
    
    //
    SQuadTreeInfo m_Info;
    //
    D3DXMATRIX m_mWorld;    //! The QuadTree acts as if it's created at 0,0,0 until displaced by this world matrix when rendering and culling.    
    D3DXVECTOR3 m_vTranslation; //! translation for island.

    D3DXVECTOR4 m_vLightDir;    //! light direction to do realtime lighting via normals of terrain.    

    CLoadArea m_LoadArea;    //! Class to handle the loading of terrain blocks!    

    QTNode *m_root; //! Root node of Quad Tree!. The Ultimate private member variable!

    ITerrain* m_pTerrain;       //! Owner

    CFoliage* m_pFoliage;       //! TODO: Pointer to foliage (animated grass meadow)

	CPillar* m_pPillar;

	IDirect3DVertexBuffer9 *m_pBasinVertices;

	IDirect3DIndexBuffer9  *m_pBasinIndices;

	IDirect3DTexture9      *m_pBasinTexture;

    //! Holds the nodes that are currently visible in the ViewFrustum. These nodes are later drawn in OnFrameRender by DrawVisible()
    vector<QTNode *> m_vVisibilityList; 
    bool m_bTileVisible; //! Says if at least one tile is visible

    vector<QTNode *> m_vBoundingBoxes; //! Just holds the QTNodes that are to be drawn in DrawBoundingBoxes

	list<QTNode *> m_vTraversalQueue; //! Used to calculate QTNode's neighbors. TODO: Fix DetermineNeighbors

    //! 0 - near, 1 - far, 2 - left, 3 - right, 4 - top, 5 - bottom
    D3DXPLANE m_FrustumPlanes[6]; // Planes that make up the View Frustum
    CornerIndexPair arrCIP[6]; //! Array of optimized corner index entries for each frustum plane

private: ///////////////////////////// PRIVATE FUNCTIONS /////////////////////////////

    VOID GenerateQuads();   //! Generates quads in quad tree from width,height to MIN_DIMENSION

    VOID QuadWorker(QTNode *n); //! recursive helper function for generate quads. 

    QTNode *QuadTrim(QTNode *n); //! In case terrain isn't perfect square, cleans up NULL members.

    BOOL NodeOutsideBounds(QTNode *n, int min, int width, int height); //! Checks if nodes midpoint is outside width height, inclusive.

	VOID DetermineNeighbours(VOID);	//! recursively lets each node know its neighbours. TODO: Fix so it handles NULLs    

    VOID CullVisibleNodes();  //! PREREQ: VisibilityList is populated. Further Culls the visible QTNodes with objects AABBs.

    bool CullTerrainTile(QTNode *n); //! Culls away a Tile in a Visible QTNode if the min/maxy's are not within top/bottom plane.

    VOID CullCloud(CCloud *pc); //! Culls away clouds by testing against view frustum.

	VOID CreateBasin(VOID); //! Creates the basin underneath the terrain to place on the pillar

    //! Tests if a portion of the quad between height miny and maxy is visible to top/bottom planes.
    bool IsHeightQuadSectionVisible(const D3DXVECTOR3 &c11, // c1 and c2 are the 2 plane aligned vertices to test bottom/top planes
                                    const D3DXVECTOR3 &c12, // miny/maxy is their new y's to test with
                                    const D3DXVECTOR3 &c21,
                                    const D3DXVECTOR3 &c22,
                                    float miny, float maxy,
                                    float thresh); 

    VOID FrustumCullWorker(QTNode *n);  //! recursively culls quads against viewFrustum

    VOID FrustumGatherChildren(QTNode *n); //! recursively makes all child quads of n as visible           

    VOID DisplaceExpandQuadTree(QTNode* n, UINT cellSpacing, UINT heightSpacing); //! Displaces and expands Quad Tree to HMP CellSpacing.

    //! Loads any new data from RAM to VRAM. Call after DetermineDistanceLOD. Spreads loading over time.
    UINT LoadNewData(const D3DXVECTOR3* pEyePt, double fTime);

    //! Populate the mesh bins/chunks for DrawVisible 
    /*! Performed by initially testing if within near and far plane. 
        Then if dot(pt-eye, lookat-eye)(normalized) is within FOV. Does it in 2D only. */    
    UINT ViewFrustumCull(const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatProj);

    VOID LoadNewTerrainBlock(); //! Loads the next Terrain Block returned by m_LoadArea.ReturnNextBlock();

    //! Update Objects in OnFrameMove that are in visible nodes.
    VOID UpdateObjects(
        const float fElapsedTime, 
        const D3DXMATRIX* pMatProj, 
        const D3DXMATRIX* pMatView, 
        const D3DVIEWPORT9 * pViewport, 
        const D3DXVECTOR3* pEyePt, 
        const D3DXVECTOR3* pUp,
        const D3DXVECTOR3* pRight,
        const D3DXVECTOR3* pAhead); 

	//! Determines LOD for each tile in tree
	UINT DetermineDistanceLOD(
        const D3DXVECTOR3* pEyePt,
        const D3DXMATRIX* pMatView,
        const D3DXMATRIX* pMatProj,
        const D3DVIEWPORT9 * pViewport);

    UINT DrawObjects(const D3DXVECTOR3* pEyePt, 
                     const D3DXVECTOR3* pUp, 
                     const D3DXVECTOR3* pRight, 
                     const D3DXVECTOR3* pAhead,                     
                     const D3DXMATRIX* pMatView, 
                     const D3DXMATRIX* pMatProj,
                     const D3DXMATRIX* pMatViewProj);

    //! Draws the bounding boxes of everything(nodes, clouds, foliage, etc.)
    HRESULT DrawQuadBBs(ULONG Color);

    HRESULT DrawCloudBBs(ULONG color);    

    //! Loads Terrain.
    VOID LoadTerrain(ITerrain *pt); 

    //! Draws the Terrain after GenerateData has been loaded
    //! PREREQ: GenerateData and TileWorker called
    HRESULT DrawTerrain(const D3DXMATRIX &matViewProj, const D3DXMATRIX* pMatShadow, LPDIRECT3DTEXTURE9 pShadowMap); 

	VOID DrawBasin(VOID);

    HRESULT DrawTerrainTiles();

    //! Used by Quadtree to Assign Tiles to corresponding quad nodes. MUST BE CALLED before DrawTerrain.
    VOID TileWorker(QTNode *n, Block *b, bool bLoad);

public:   ///////////////////////////// PUBLIC FUNCTIONS /////////////////////////////

    //! Takes over Ownership of the pterrain, therefore is responsible for its memory.
    CQuadTree(const SQuadTreeInfo* pinfo, const SPillarInfo* pillarInfo);
    ~CQuadTree(void);

    /*! 
        Generate the Meshes needed in D3DPOOL_MANAGED on CreateDevice
        Creates meshes,etc bound to the pd3dDevice sent in parameter. This is stored in object. */
    UINT OnCreateDevice(IDirect3DDevice9 * pd3dDevice);

    //! Draw the bounding boxes of the quads
    HRESULT DrawBoundingBoxes();

    VOID SetFrustumPlanesAndCorners(D3DXPLANE *planes, CornerIndexPair *cip) {
        memcpy(m_FrustumPlanes, planes, sizeof(D3DXPLANE)*6);
        memcpy(arrCIP, cip, sizeof(CornerIndexPair)*6);
    }

    //! Calculations to be done before Render. Also Renders to texture for Impostors.
    VOID OnFrameMove( 
        const D3DXVECTOR3* pEyePt,
        const D3DXVECTOR3* pUp,
        const D3DXVECTOR3* pRight,
        const D3DXVECTOR3* pAhead,
        const D3DXMATRIX* pMatProj,
        const D3DXMATRIX* pMatView,        
        const D3DVIEWPORT9 * pViewport,
        const double fTime,
        const float fElapsedTime);

    //! Rendering function
    HRESULT OnFrameRender(
        const D3DXVECTOR3* pEyePt,
        const D3DXVECTOR3* pUp,
        const D3DXVECTOR3* pRight,
        const D3DXVECTOR3* pAhead,
        const D3DXMATRIX* pMatProj,
        const D3DXMATRIX* pMatView,
        const D3DXMATRIX* pMatShadow, 
        LPDIRECT3DTEXTURE9 pShadowMap,
        UINT param);

    //! Used to create any D3DPOOL_DEFAULT Devices.
    HRESULT OnResetDevice(IDirect3DDevice9 * pd3dDevice);

    //! Used to CLEAN UP any D3DPOOL_DEFAULT Devices Lost.
    HRESULT OnLostDevice();

    VOID SetLightDir(const D3DXVECTOR4* pLightDir) { m_vLightDir = *pLightDir; }

    //! Returns the center of this spatial subdivision. TODO: Make sure this works properly for rectangular hmps.
    D3DXVECTOR3 GetCenter() { return D3DXVECTOR3((float)m_root->midx, (float)m_root->midy, (float)m_root->midz); }
    float GetMinY() { return (float)m_root->miny; }
    float GetMaxY() { return (float)m_root->maxy; }
    const D3DXVECTOR3* GetTranslation() const { return &m_Info.vTranslation; }

    const ITerrain* GetTerrain() const { return m_pTerrain; }
    CPillar *GetPillarPointer(void) { return m_pPillar; }

    VOID ToggleDrawTerrain() { m_bDrawTerrain = !m_bDrawTerrain; }

    //! Unlocks the Level of Detail if previously locked 
    VOID UnlockLOD() { m_bLockLOD = false; }

    //! Locks LOD to the parameter level. If not within the current number of LODS, unlocks.
    VOID LockLOD(UINT lod)  { 
        m_bLockLOD = true;
        m_LockedLOD = lod;
    }

    //! Toggles whether or not to Draw Spatial Subdivision's Bounding Boxes
    VOID ToggleBoundingBoxes() { m_bDrawBoundingBoxes = !m_bDrawBoundingBoxes; }

    //! Tells whether ISS is in good standing
    bool IsInit()                { return m_bInit; }

    DIM_GET_ACCESSOR(UINT, Width);
    DIM_GET_ACCESSOR(UINT, Length);

private: // operators intentionally undefined so as to generate compiler warnings if ever called
    CQuadTree& operator=(const CQuadTree &rhs); // disallow assignment operator
    CQuadTree(const CQuadTree& rhs); // disallow copy constructor
};
