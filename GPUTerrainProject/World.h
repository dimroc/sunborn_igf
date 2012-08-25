#pragma once

#include "MeshEntity.h"
#include "GameDefine.h"
#include "GlobalDefine.h"
#include "QuadTree.h"
#include "CloudSky.h"
#include "GameTables.h"

using std::vector;
using stdext::hash_map;
using std::string;
using gdef::CornerIndexPair;

class CBubbles;
class CCloud;

class CShadowMap;
class PostProcEffects;

class CXModelHeirarchy;
class CModelInstance;

class CGameSettings;

class CPolySprite;
class CSpriteAnimMgr;


/*! 
Encompasses all the data that describes the world.
This contains:
    -   TODO: GUI
    -   Camera
    -   View Frustum
    -   Islands (quad tree mipmapped terrain. TODO: foliage)
    -   CloudSky
    -   Bubbles
    -   Limbo objects
    -   Players
*/
class CWorld
{
private:
    typedef CQuadTree::QTNode           QTNode;
    typedef CQuadTree::SQuadTreeInfo    SQuadTreeInfo;
	typedef CPillar::SPillarInfo		SPillarInfo;

    ///////////////////////////// PRIVATE STRUCTS /////////////////////////
    //! Struct to hold Bubble filename/constructor info from level/world file.
    struct SBubbleInfo {
        string strTex, strNormalMap;
        D3DXVECTOR3 vCenter; 
        float fRadius;
        float fHorTile, fVerTile;
    };

    //! Struct to hold CloudSky filename/constructor info from level/world file.
    struct SCloudSkyInfo {
        string strSkyToneTex;
        string strCloudAtlasTex;
        string strCloudMap;
        float fSkyRadius;
    };

    //! Populated by the .lml model file read in by fstream that will populate the world 
    //! with static meshes and define player character.
    struct SStaticEntityInfo {
        int geomID;
        D3DXVECTOR3 scale, angle, pos;
        bool operator < (const SStaticEntityInfo& rhs) const {
            return geomID < rhs.geomID;
        }
    };

    struct SCameraMovement {
        gdef::SSpring PositionSpring, LookAtSpring;

        float fRelVelMag, fWorldVelMag, fLookAtVelMagnitude;        
        D3DXVECTOR3 vLastRelativePos, vLastWorldPos, vLastLookAt;
        D3DXVECTOR3 vCameraOffset, vCameraLookAtOffset;
        D3DXMATRIX mTarget; //! Target destination / orientation.
    };

    struct SRunningSpriteAnimation
    {   //! Holds information for a running animation. Gets purged if  < ttl.
        int runningAnimIdx;
        LPDIRECT3DTEXTURE9 pTex;
        gdef::RECTF rectf;
        D3DXMATRIX mWorld;
        float ttl;  // time to live.        
    };

    struct SSpriteAnimationEntry
    {   //! Entry used to load in and populate Sprite Animations.
        int idxTex, idxAnim;
        vector<int> vStills;
        float fSpeed;        
        D3DXMATRIX mWorld;
        CSound* pSound;        
    };

    ///////////////////////////// PRIVATE MEMBER VARIABLES /////////////////////////

    string                              m_strWorldFileName, m_strModelFileName, m_strCharacterFileName, m_strSpriteFileName;      

    bool                                m_bInit, m_bDrawBBs, m_bSaveShadowMap, m_bCameraControl, m_bFirstRun;

    vector<SQuadTreeInfo>               m_vIslandInfo;
    vector<SBubbleInfo>                 m_vBubbleInfo;
	vector<SPillarInfo>                 m_vPillarInfo;
    vector<SStaticEntityInfo>           m_vStaticEntityInfo;
    SCloudSkyInfo                       m_CloudSkyInfo; //! Only one cloudsky Info per world.

    // 
    // Models: Player characters and world entities such as platforms.
    //    
    vector<string>                      m_vGeometryFilenames;    
    vector<CXModelHeirarchy*>           m_vpStaticModels; // the actual geometries of obstacles.
    vector<CXModelHeirarchy*>           m_vpCharacterModels; // the actual geometries of players
    vector<LPDIRECT3DTEXTURE9>          m_vCharacterTextures; // the alternate textures of player characters.

    //! Holds the index of the animation relating to the enum/uchar defined in GameTables.h
    typedef hash_map<UCHAR, UINT>       AnimationKeyIndexHashMap;
    //! The index in this vector matches that of m_vpCharacterModels. Different character models could have different enum/idx pair.
    vector<AnimationKeyIndexHashMap>    m_vhmAnimationKeyIndex; 
    
    EntityHashMap                       m_hmEntities; //! playable characters and obstacles with meshes.
    UINT                                m_nCharacterKey;
    vector<UINT>			    		m_vCharIDs;	//! Holds the created char ids
    vector<SMeshEntity*>                m_vVisibleEntites; //! VISIBLE player characters sorted by geomID used for rendering.     

    //
    // View Frustum Stuff
    //
    //! 0 - near, 1 - far, 2 - left, 3 - right, 4 - top, 5 - bottom
    D3DXPLANE                           m_FrustumPlanes[6]; // Planes that make up the View Frustum
    CornerIndexPair                     m_arrCIP[6]; //! Array of optimized corner index entries for each frustum plane    
    
    //! Sky, Sun, and clouds for this world
    CCloudSky*                          m_pCloudSky;
    //! Used to store a which clouds need their Impostors to be updated. 
    // Therefore can amortize load by doing IMPOSTOR_UPDATES_PER_FRAME per frame.
    deque<CCloud*>                      m_vpCloudImpostorQueue;
    vector<CCloud*>                     m_vpLimboClouds; //! Clouds that do fall within an island and are in 'limbo' to be culled separate.

    CBubbles*                           m_pBubbles;     //! bubbles around islands

    vector<CQuadTree*>                  m_vpIslands;    //! The islands that comprise this world.
    vector<QTNode*>                     m_vVisibilityList;  //! Used to stuff the returned visible qtnodes from the islands.

	//! Sprite Animation Manager Stuff
	typedef hash_map<UCHAR, SSpriteAnimationEntry>	SpriteAnmHashMap;
	SpriteAnmHashMap					m_hmAnmEntries;	//! Holds the sprite animation entries loaded in by .lal file and index in with key from GameTable.h
	
    typedef hash_map<int, SRunningSpriteAnimation> RunningSpriteAnmHashMap;
    RunningSpriteAnmHashMap             m_hmRunningSpriteAnims;

    CSpriteAnimMgr*                     m_pSpriteAnimMgr;   //! Used for player summons and effects
    CPolySprite*                        m_pPolySprite;  //! Render the damn spriteAnimMgr stuff.


    PostProcEffects*        			m_pPPEffects;   //! PP Effects such as bloom and edge detection.
    CShadowMap*                         m_pShadowMap;
    int                                 m_nShadowUpdateLimit, m_nShadowUpdateCount;

    const gdef::COLOR                   m_BLOOMFLAGCOLOR;
    LPDIRECT3DTEXTURE9                  m_pNotBloomMap; //! A RENDERTARGET Used to store what not to bloom.
    LPDIRECT3DSURFACE9                  m_pNotBloomMapSurface; // to render to above.    

    //! Sunstate Hashmap
    typedef hash_map<UCHAR, SSunMorphState> SunStateHashMap;
    //! holds the sunstates loaded in from .lvl file that are called from CGameEngine
    SunStateHashMap                     m_hmSunStates;  

    // Camera stuff
    CFirstPersonCamera                  m_Camera;
    SCameraMovement                     m_CameraMovement;

    // D3D GOODNESS
    LPDIRECT3DDEVICE9                   m_pd3dDevice;

    ID3DXFont*                          m_pFont;          //! Font for drawing text
    ID3DXSprite*                        m_pTextSprite;    //! Sprite for batching draw text calls

    D3DVIEWPORT9                        m_ViewPort;
    D3DXMATRIX                          m_mIdentity;
    gdef::Matrix3x3                     m_mGaussianKernel;	

private:
    ///////////////////////////// PRIVATE MEMBER FUNCTIONS /////////////////////////

    VOID SetupKernels();
    VOID IncrementCharKey();
    
    UINT LoadWorldFile(LPCSTR worldFN); //! Populates info structs with settings from file.
    UINT LoadModelFile(LPCSTR modelFN); //! Loads models sent in by .lml file.

    UINT CreateCharacterGeoms(LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR charFN); //! Loads and creates the player models sent in by .lcl
    HRESULT CreateStaticGeomsEntities(LPDIRECT3DDEVICE9 pd3dDevice); //! Loads the geoms and instances of entities. set in loadmodel file.    

	UINT LoadSpriteAnimationsFile(LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR spriteAnimeFN);	//! Loads the textures and the sprite animations in the file.

    HRESULT UpdateShadowMap(); //! Updates the ShadowMap Surface and the ShadowMapMtx (the lights' world matrix). Renders entities to surface.
    
    VOID UpdateEntities(float fElapsedTime);    //! Updates the animation of the entities if any.
    bool CullEntity(SMeshEntity* pent);    //! Checks entity AABB against frustum and sets bVisible/returns true if visible. Populates m_vVisibleEntites.
    HRESULT RenderEntities(const D3DXMATRIX* pShadowViewProjMtx);   //! Renders the Characters and Static Meshes(Obstacle) Entities.

    VOID UpdateSpriteAnimations(float fElapsedTime);
    VOID DrawSpriteAnimations(CPolySprite* pPolySprite);

    //
    // CAMERA
    //
    inline VOID IncrementCameraVelocity(float *fVelInOut, float fDist, float fAccel, float fElapsedTime);

    VOID FindCameraRelativeStep(D3DXVECTOR3 *pvOut, float fElapsedTime);
    VOID FindCameraWorldStep(D3DXVECTOR3 *pvOut, float fElapsedTime);
    VOID FindCameraLookAtStep(D3DXVECTOR3 *pvOut, float fElapsedTime);

    VOID UpdateCameraOrientation(float fElapsedTime);
    VOID UpdateCameraRestState();

    VOID UpdateClouds(float fElapsedTime);
    VOID RenderBubblesCloudsSprites( bool bClearFlagged = true); //! Renders the bubbles back to front with a render cloud impostoratlas in between.   
    VOID PopulateLimboVectors(); //! Place objects that are in limbo in their appropriate vector.

    bool IsAABBLimbo(const vector<QTNode*>& vpTopNodes, const D3DXVECTOR3 *center, const D3DXVECTOR3 *displace);
    // Remember the (retarded) Corner Vectors (I made). 
    //  4 --- 5                3 --- 2 - top
    //  |     |                |     |
    //  0 --- 1  - bottom      7 --- 6
    //D3DXVECTOR3 corners[8]; //! store corners for faster tree traversal

    HRESULT DrawBoundingBoxes();
    HRESULT DrawLimboCloudBBs(ULONG color);
    HRESULT DrawEntityBBs(ULONG color);

public:
    /*! 
    Loads the entire world settings from the passed .lvl file and the model settings from the .mdl file.
    The world settings consist of:
        - filenames for the skytone, cloudatlas, and cloudmap. This makes the worlds sky.
        - n islands which are have the filenames for n QuadedMipmapTerrains
            + This has the filenames for each islands terrain textures.
            + bubble texture.            
            + Loads static object that are within quadtree bounds.
            + If we do it, has grassmap filename.
        - translations for the n Islands.
    The model settings consist of:
        - filenames for the player characters' and obstacles' x file meshes.
    */
    CWorld(const string& levelFN,
           const string& modelFN,
           const string& charFN,
           const string& spriteFN);
    ~CWorld(void);

    HRESULT OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice);    

    HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice);
    HRESULT OnLostDevice();

    //! Calculations to be done before Render. Also Renders to texture for ImpostorAtlas stff such as those in CloudSky.
    VOID OnFrameMove( const double fTime, const float fElapsedTime );

    //! Rendering function. Camera matrices are passed to members update functions.
    HRESULT OnFrameRender( UINT param );

    ///////////////////////////// CHARACTER ENTITY FUNCTIONS /////////////////////////    
    int CreateCharacter(UINT geomID);  //! Creates a playable character, initially invisible, returns its graphics id.
    LPDIRECT3DTEXTURE9 GetCharacterTexture(int texID);

    VOID SetCharacterAnimation(UINT charID, UCHAR animID, bool bLoopable = true, float fPeriodTime = -1.f);  //! Takes the enum/UCHAR defined in GameTables.h to assign anim.
    VOID SetCharacterWorldMatrix(UINT charID, const D3DXMATRIX* pWorld, bool bRequestVisible = true);

    const D3DXVECTOR3* GetCharacterObjectCenter(UINT charID);
    float GetCharacterRadius(UINT charID);
    const vector<UINT> &GetCharacterIDs()        { return m_vCharIDs; }

    VOID ToggleCharacterVisibility(UINT charID);
    VOID DeleteAllCharacters();
    VOID DeleteCharacter(UINT charID);

    ///////////////////////////// SPRITE ANIMATION FUNCTIONS  /////////////////////////

    UINT AddSpriteAnimation(UCHAR spriteAnimTableID, const D3DXMATRIX* pWorld, float ttl);
    VOID UpdateSpriteAnimation(int runningAnimID, const D3DXMATRIX* pWorld, float fTTL);

    ///////////////////////////// CAMERA FUNCTIONS

    VOID SetCameraOrientationTarget(const D3DXMATRIX* pOrientationTargetMtx);
    VOID SetCameraVariables(float fKCoef, float fLookKCoef);                            
    VOID SetCameraOffset(const D3DXVECTOR3 *pvCameraPosOffset, const D3DXVECTOR3 *pvCameraLookAtOffset);
    VOID SetCameraViewParams(D3DXVECTOR3* pEye, D3DXVECTOR3* pLookAt);

    ///////////////////////////// MISCELLANY DEVELOPER FUNCTIONS
    VOID RenderStats();
    VOID ToggleDrawBoundingBoxes()              { m_bDrawBBs = !m_bDrawBBs; }
    VOID SunStop();
    VOID SunTest(int idx);    
    VOID SaveShadowMap()                        { m_bSaveShadowMap = true; }
	VOID CycleCharactersAnimation();	// USED SOLELY FOR JONATHAN AND DEBUGGING.

    VOID ToggleCameraControl()                  { m_bCameraControl = !m_bCameraControl; }
    VOID SetCameraControl(bool b)               { m_bCameraControl = b; }

    VOID KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown );  //! used to test rendering features.    	
	bool m_bEditPillar;		// make sure this is removed

    ///////////////////////////// SETTER GETTER FUNCTIONS /////////////////////////

    CFirstPersonCamera& GetCamera()             { return m_Camera; }

    //! Returns the bloom map available after rendering scene. Pure Green defines things to bloom.
    LPDIRECT3DTEXTURE9 GetNotBloomMap()         { return m_pNotBloomMap; }
    //! Returns the color signature that defines whether to bloom area.
    gdef::COLOR GetBloomColor()                 { return m_BLOOMFLAGCOLOR; }

    float GetTimeOfDay()                        { return m_pCloudSky->GetTimeOfDay(); }
    VOID SetTimeOfDay(float fClockTime)         { m_pCloudSky->SetTimeOfDay(fClockTime); }

    VOID RequestSunState(GameTable::SunStates eSunState, int SunIdx);
    VOID RequestLightDirByTimeOfDay(float fLightDirByTimeOfDay); //! Repositions the 'sun' light based on the passed in TimeOfDay.

    int GetNumCharacterGeoms(); // returns number of different char meshes
    float GetCharacterAnimationPeriod(int geomID, UCHAR animIdx);

    ///////////////////////////// GETTER FUNCTIONS FOR PHYSICS /////////////////////////

    float GetWorldExtent()                      { return m_CloudSkyInfo.fSkyRadius; }
	VOID GetLivableSphere(D3DXVECTOR3 *vCenterOut, float *fRadiusOut)	
	{ 
		*vCenterOut = m_vBubbleInfo[0].vCenter;
		*fRadiusOut = m_vBubbleInfo[0].fRadius;
	}
    bool IsPointOverIsland(const D3DXVECTOR3* pvec); //! returns true if the point is ABOVE an island.

    const vector<CQuadTree*> &GetIslands()      { return m_vpIslands; }
    const EntityHashMap &GetEntities()          { return m_hmEntities; }

private: // operators intentionally undefined so as to generate compiler warnings if ever called
    CWorld& operator=(const CWorld &rhs); // disallow assignment operator
    CWorld(const CWorld& rhs); // disallow copy constructor
};
