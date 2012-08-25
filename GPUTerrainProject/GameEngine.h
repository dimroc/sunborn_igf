#pragma once

#include "GameSettings.h"
#include "PhysicsRoot.h"

#include "RakNet/NetworkIDGenerator.h"
#include "RakNet/NetworkTypes.h"

#include "ServerInfo.h"

class CWorld;
class CServer;
class CClient;

using stdext::hash_map;
using std::list;
using std::vector;

class CGameEngine : public NetworkIDGenerator
{    
//friend RPC functions
// ------------------------------------------------------------------------
// SERVER RPC FUNCTIONS (called by client). 
// ------------------------------------------------------------------------
friend void ServerPlayerConnect(RPCParameters *pRpcParms); // client sends acknowledgement to server.
friend void ServerPlayerCmdUpdate(RPCParameters *pRpcParms);

// ------------------------------------------------------------------------
// CLIENT RPC FUNCTIONS (called by server). 
// ------------------------------------------------------------------------        
friend void ClientAcknowledge(RPCParameters *pRpcParms); // client receives personal acknowledgement from server with its networkID.
friend void ClientPlayerConnectAnnouncement(RPCParameters *pRpcParms); //! receive announcement of new player connection 
friend void ClientPlayerDisconnectAnnouncement(RPCParameters *pRpcParms);  //! receive announcement of player disconnect

friend void ClientRealityUpdate(RPCParameters *pRpcParms); // reality packets for all players sent by server
friend void ClientSpawnSLE(RPCParameters *pRpcParms); // immediate packet sent from server when an sle has been spawned.
friend void ClientStartRound(RPCParameters *pRpcParms);    // announcement from server when round starts.
friend void ClientEndRound(RPCParameters *pRpcParms);  // announcement with respawn locations for everyone on round end.

public:
    virtual bool IsNetworkIDAuthority(void) const { return m_bHosting;  }
    // --------------------------------------------------------------------------------------
    //! Structs
    // --------------------------------------------------------------------------------------
    struct SAnimRequest
    {
        //! current animation request of player. ANIM_NO does nothing. 
        //! After Request is taken by GameRoot, it is reset to AMIN_NO
        UCHAR cCurAnimRequest; 
        //! time it takes to play one cycle of animation. 
        //! if <= 0, plays the animation at it's default time period.
        float fPeriodTime; 
        bool bLoopAnim; //! whether to loop animation.
    };

    struct SCameraControl
    {   // struct to hold the offsets sent to camera to have the desired third person angle.
        //! The Camera will interpolate to this position target position with the 1,2,3 rows defining xyz, 4th row defining position.
        D3DXMATRIX mCameraTarget;   // for normal movement, just set = to mSpace.
        D3DXVECTOR3 vPosOffSetOriginal, vLookAtOffsetOriginal;  //! The original offsets from gamesettings
        D3DXVECTOR3 vPosOffSetCur, vLookAtOffsetCur;  //! The offsets due to slope / player tilt request.
        float fPosOffsetMag, fLookAtOffsetMag;
        UCHAR cTilt, oTilt;    // the player requested tilt of the camera (looking up in the air, or from above down, or horizontal).
        // old tilt. to stop redundant calculations.
    };
    struct SPlayer 
    {
        int charID, physicsID; // charID unique ID in graphics world.
        PlayerID playerID;  // used to identify player over network.
        UCHAR charType; 

        char strAlias[NUM_BYTES_FOR_NAMES];    //! alias in game. (crackie, bobschmackee, dimroc)

        SAnimRequest animRequest;
		UCHAR cLastAnimation;

        D3DXVECTOR3 vVel, vLastVel;  //! player relative velocity. Not actual world velcotiy (affects such as grav etc).
        D3DXVECTOR3 vLastDirection; //! to render world mtx if player stops moving.
        char cSpaceIdx;    //! index into the SPlayerSpace that defines this player's movement. see struct below (SPlayersSpaces).
		D3DXMATRIX mSpace;	//! this defines the players world space. His forward(run), up and right. 4th vector is position.
        D3DXMATRIX mWorld;  //! this defines the player's world rendering matrix. Specifically, the position and direction facing.        
        SCameraControl CameraControl;   //! the lookAt and Pos offset are then applied to mCameraTarget.

        float fAttackDelay; //! grogginess/delay after attack
		float fGroundOffset;
		float fUncontrollableTimeLeft;

		//! bools for continuous events such as run toggle on key down.
        bool bMovement[4];  // movement directions: forward, backward, left, right.
		bool bDive, bDash, bBlock, bCharge[4];
        bool bCameraTurnLeft, bCameraTurnRight, bResetCamera;
        bool bUncontrollable, bDazed, bCelebrate;	//! Player is in dazed or uncontrollable state.
		bool bJumping;			//! Player is jumping
        bool bRequestVisible;

		int nLastHitBy; //! the ID of the player that last hit this player, for assigning points
		//! Some stats
		float fCurrDamage;
		float fCurrCharge, fMaxCharge;
		int nJumps;
		int nLives;
		int nScore;
    };    

    struct SPlayerSpaces 
    {   // Stored 4 degrees of 'player space'. Where forward is either -z, -x, +z, +x
        enum { POSX = 0, NEGZ, NEGX, POSZ, NUMSPACES };
        D3DXMATRIX mSpaces[4];
        SPlayerSpaces();
        ~SPlayerSpaces() {}
    };

    struct SShortLifeEntity
    {   //! used to represent lightning and other sprite visuals summoned by player or just has short lifespan.
		enum { FIREBALL = 0, LIGHTNING, SWEEP, SMASH, DIVE, NUM_SLES }; 
        static int StaticID;
        int sseID;  // unique ID of this short life entity (sse).
		int physicsID; // ID of the physics representation of this sse
		int ownerID; // ID of the physics object (player) that created this sse
        int nSunAffiliation;    // The idx of the sun player spawned from.
        D3DXVECTOR3 vPos, vScale;
        D3DXMATRIX mWorld;
        float fTimeToLive, fRadius;
		int nData; // integer used for whatever each specific type of sle needs
        float fData;
		UCHAR spriteAnimTableID;    //! ID used to index which texture to use as defined in GameTables.h 
		UCHAR sleType;
        int runningAnimId;  //! the anim running in spriteanimmgr.
 //       D3DXVECTOR3 corners[8]; // AABB stuff for graphics part and for collision test in GameEngine.
        bool bCollidable, bVisualCreated;
    };
	struct SRoundInfo{
		float fRoundTimeLeft; // Game timer
		int nRound; // current round number
		bool bRoundStarted; // if true a round is in progress, false in between rounds
		
		bool bShowScoreboardRequest; // if true show the scoreboard, then set to false
		bool bHideScoreboardRequest; // if true show the scoreboard, then set to false
	};

	struct SHUDMessageRequest{
		string text;
		float fTimeToLive;
		bool bBigFont;	// Use big or small font
	};

    struct SSunStateRequest
    {
        int SunIdx;
        GameTable::SunStates eSunState;
        float fNewLightDirByTimeOfDay;  //! 0 == 24 is dawn.
        SSunStateRequest(GameTable::SunStates _eSunState, int _SunIdx, float _fNewLightDirByTimeOfDay) : 
            SunIdx(_SunIdx), eSunState(_eSunState), fNewLightDirByTimeOfDay(_fNewLightDirByTimeOfDay) {}
    };

    struct SEnvironment
    {   // keeps tabs of CWorld environment variables that get periodically updated by server.        
        float fCurrentClockTime;
        bool bNight;
		D3DXVECTOR3 vLivableSphereCenter;
		float fLivableSphereRadius;
    };

    // Player command function
    typedef void(CGameEngine::* PlayerCommandFuncPtr)(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    
    // --------------------------------------------
    // NETWORK STRUCTS AND TYPES
    // --------------------------------------------
    struct SPlayerNetworkUpdate
    {   //! broadcasted by server to clients to fix entropy every server->BroadcastReality Interval.
        D3DXVECTOR3 vPos, vVel;
        PlayerID playerID;
        bool bMovement[4];
        UCHAR cLastAnimation;
        
        SPlayerNetworkUpdate();
        SPlayerNetworkUpdate(const SPlayer &player);
        SPlayerNetworkUpdate(RakNet::BitStream *pBitStream);
        ~SPlayerNetworkUpdate();

        VOID StuffBitStream(RakNet::BitStream *pBitStream);
    };

    struct SPlayerCmdHistory
    {   //! History of player command requests to be sent to server ever client->UpdateInterval.
        RakNetTime rakTime;
        UCHAR cPlayerCommand;
        bool bKeyDown;
        SPlayerCmdHistory(UCHAR cPlayerCommand, bool _bKeyDown, RakNetTime rakTime);
    };

    typedef hash_map<int, SPlayer>		    PlayerHashMap;
    typedef hash_map<__int64, SPlayer>	    ServerPlayerHashMap;

private:
    const bool                          m_bHosting;
    CGameSettings*                      m_pGameSettings;    //! Not owner dipshit. don't delete.

    //
    //! Server if hosting game. Client if connected to network game ( including self ).
    //
    CClient*                            m_pClient;      //! Network client that makes requests to connected server (if present).
    CServer*                            m_pHost;    //! If hosting game, responds to client requests.

    //
    //! physics top layer
    //
    CPhysicsRoot*                       m_pPhysicsRoot; 

    CWorld*                             m_pWorld;

    //
    //! hashmap of players. SPlayer::charID is key.
    //
    PlayerHashMap			            m_hmPlayers;
    ServerPlayerHashMap                 m_hmServerPlayers;
	SPlayer*							m_pMyPlayer;

    const SPlayerSpaces                 m_PlayerSpaces; //! holds the possible player movement spaces.
    list<SShortLifeEntity>              m_lShortLives;
    vector<SSunStateRequest>            m_vSunStateRequests;
	vector<SHUDMessageRequest>			m_vHudMessageRequests;

    //
    //! PLAYER FUCNTION PTRS
    //    
    typedef hash_map<UCHAR, PlayerCommandFuncPtr> PlayerCommandHashMap;
    PlayerCommandHashMap                m_hmPlayerCommands;
    //! This hashmap is populated with func ptrs that get indexed by the returned GameSettings Keyboard request.

    //
    //! Mouse Location
    //
    int                                 m_nMouseX, m_nMouseY, m_nMouseWheelDelta;

    //
    //! ENGINE ENVIRONMENT VARIABLES
    //
    SEnvironment                        m_Environment;
	
	//
    //! GAME INFO
    //
    SRoundInfo							m_RoundInfo;

    //
    //! NETWORK VARIABLES
    //
    //! The Command history of this player since last update to server.
    vector<SPlayerCmdHistory>           m_vCmdHistory; 

private:

    VOID UpdateShortLifeEntity(SShortLifeEntity* pentity, float fElapsedTime);
    VOID UpdateSkyRequest(float fClockTime); // Send a sun state change request to CWorld based on the enums in GameTables.h
	
	//-------------------------------------------------------------------------
	//  ROUND FUNCTIONS
	//-------------------------------------------------------------------------
	VOID UpdateRoundInfo(float fElapsedTime);	

    // ------------------------------------------------------------------------
    // NETWORK FUNCTIONS.
    // ------------------------------------------------------------------------
    VOID RegisterRPCFunctions();
    VOID GetSettingsFromBitStream(RakNet::BitStream *pBitStream);
    VOID StuffSettingsToBitStream(RakNet::BitStream *pBitStream);

    VOID ServerUpdateClients(float fElapsedTime);  //! broadcast the real positions (or 'reality') to clients.    
    VOID ClientUpdateServer(float fElapsedTime);   //! send action deltas to server

    //! Server functions    
    //! This broadcasts the server's interpretation of the world, aka, 'Reality' as opposed to client prediction.
    VOID BroadcastReality();    //! PREREQ: m_pHost != NULL ( player is server ).
    VOID RoundEnd();    //! Server assigns spawn points to every player and broadcasts to all clients.

    //! Client Update functions
    VOID SendCmdHistory();    

    // ------------------------------------------------------------------------
    // PLAYER MANIPULATION FUNCTIONS.
    // ------------------------------------------------------------------------

    //! Updates clients perception of reality of passed in player with the PlayerNetworkUpdate
    VOID RealityUpdate(SPlayer *pplayer, SPlayerNetworkUpdate *ppnu);

    //! Used when initially creating the player. Returns the current players spawn info and physics collision object info.
    VOID SpawnPhysicsInfo( int charID, 
                       D3DXVECTOR3* pvPos, //! out
                       D3DXVECTOR3* pvTopCenter, //! out
                       D3DXVECTOR3* pvBottomCenter, //! out
                       float* pfRadius, float* pfGroundOffset); //! out

    D3DXVECTOR3* GetSpawnPoint(D3DXVECTOR3* pvOut); // gets the current spawn point dependent on sun position. 
    //returns pointer so it can be used as paramter returning ptr.

    //! Fills out a player's animation request. the fPeriodTime is how long one cycle of an animation period should take.
    //! ie: if you wish FIREBALL_START to take 2 seconds, you send in 2.f and the animation manager will speed it up / slow it down accordingly.
    //
    //! Send in a fPeriodTime <= 0 to let the animation run its normal speed. You can alter the period of a 
    //! loopable animation to make it seem faster such as running faster moves legs faster.
    inline VOID SetPlayerAnimation(SPlayer* pplayer, UCHAR cAnimID, bool bLoopAble, float fPeriodTime = -1.f);

	SPlayer* GetPlayerByPhysicsID( int physicsID );    
    
	VOID ClearCharges(SPlayer* pplayer) { 	memset(&pplayer->bCharge, 0, sizeof(pplayer->bCharge));	}
	bool IsCharged(SPlayer* pplayer) {
		for (int i = 0; i < 4; i++)
			if (pplayer->bCharge[i])
				return true;
		return false;
	}
	bool IsMoving(SPlayer* pplayer) {
		for (int i = 0; i < 4; i++)
			if (pplayer->bMovement[i])
				return true;
		return false;
	}

	D3DXVECTOR3 GetForwardVector(SPlayer* pplayer){
		return -D3DXVECTOR3(*(D3DXVECTOR3*)&pplayer->mWorld._31);
	}
	D3DXVECTOR3 GetUpVector(SPlayer* pplayer){
		return D3DXVECTOR3(*(D3DXVECTOR3*)&pplayer->mWorld._21);
	}
	D3DXVECTOR3 GetRightVector(SPlayer* pplayer){
		return -D3DXVECTOR3(*(D3DXVECTOR3*)&pplayer->mWorld._11);
	}
	D3DXVECTOR3 GetPlayerPosition(SPlayer* pplayer){
		return D3DXVECTOR3(*(D3DXVECTOR3*)&pplayer->mWorld._41);
	}
	
    // ------------------------------------------------------------------------
    // PLAYER CONTROL FUNCTIONS. These are stored in the m_hmPlayerCommands for lookup on the keyboard return.
    // ------------------------------------------------------------------------

    //! This populates the player command func ptr hash map with the settings defined in CGameTables.h
    void PopulatePlayerCommandHashMap();    
    
    void PlayerNothing(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta) {}  // do nothing.

    void PlayerMoveForward(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);    
    void PlayerMoveBackward(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    void PlayerMoveLeft(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    void PlayerMoveRight(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);

    void PlayerJump(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    void PlayerDescend(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    void PlayerBlock(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);  
    void PlayerCelebrate(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);  
    void PlayerDash(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    
    void PlayerAttackSmash(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    void PlayerAttackVertical(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    void PlayerAttackRange(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);
    void PlayerAttackSweep(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);          
    
    void PlayerRotateCameraLeft(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);  
    void PlayerRotateCameraRight(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);  
    void PlayerTiltCamera(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);  
    void PlayerResetCamera(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);  

    void PlayerRequestRespawn(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta);

    // more Player Functions to Come. Watch out, if it's not mapped right it'll crash.

	void RangedAttack(SPlayer* pplayer);
	void VerticalAttack(SPlayer* pplayer);
	void SweepAttack(SPlayer* pplayer);
	void SmashAttack(SPlayer* pplayer);
	void DiveAttack(SPlayer* pplayer);

	// Applies damage and knockback to the victim
	void HitPlayer(SPlayer* pplayer, SPlayer* pVictim, float fDamage, float fDazeTime, D3DXVECTOR3* pvImpulse);
	void Respawn(SPlayer* pplayer, const D3DXVECTOR3 *pvRespawn = NULL);
    __int64 AddServerPlayer(UCHAR charType, PlayerID playerID, LPCSTR pszAlias); //! Spawns player solely for server purposes. Physics entity, etc. No m_pWorld representation.
    VOID GetPlayerSpawnStats(SPlayer *pplayer); //! Gets the players start health/charge/lives/etc.
    // ------------------------------------------------------------------------
    // PLAYER UPDATE FUNCTIONS. Called in OnFrameMove to update player input, orientation, velocity, and action.
    // ------------------------------------------------------------------------
	void UpdatePlayer(SPlayer* pplayer, float fElapsedTime);    //! Updates the player by calling the privates below.
    VOID UpdateCameraPosition(SPlayer* pplayer);    // updates the camera according to player's position and ground slope.
    
    void UpdatePlayerActualVelocity(SPlayer* pplayer);  //! Adds this to physics world. PREREQ: Called after UpdatePlayerInputVelocity. 
    void UpdatePlayerWorldMtx(SPlayer* pplayer);    //! Updates the world render mtx with player face direction, position, etc.
    void UpdatePlayerMoveSpace(SPlayer* pplayer);   //! Updates the player move space if user rotates camera.    
	void UpdatePlayerTimers(SPlayer* pplayer, float fElapsedTime);		//! Updates the player's timers
	void UpdatePlayerAnimation(SPlayer* pplayer, float fElapsedTime);
	void TestForPlayerDeath(SPlayer* pplayer);	

    void DeletePlayer(SPlayer* pplayer);    //! Deletes player from client by removing m_pWorld entity, physics entity, and place in hashmap.
    void DeleteServerPlayer(SPlayer* pplayer);  //! Deletes player from server hash_map and deletes physics entity.

public:
    CGameEngine(bool bHosting = false);
    ~CGameEngine(void);

    VOID OnFrameMove( float fElapsedTime, float fClockTime );

    VOID CreatePhysicsWorld(CWorld* pWorld);
    VOID RenderPhysics(const D3DXMATRIX* pProj, const D3DXMATRIX* pView);

    void KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown );
    void UpdateMouseLocation(int xPos, int yPos);
    void MouseProc(bool bLeftButtonDown, 
                   bool bRightButtonDown, 
                   bool bMiddleButtonDown, 
                   bool bSideButton1Down, 
                   bool bSideButton2Down, 
                   int nMouseWheelDelta, 
                   int xPos, int yPos);

	void ChangeSetting(UCHAR stgID, float fValue){
		m_pGameSettings->ChangeSetting(stgID, fValue);
	}

    int AddPlayer(UCHAR charType, PlayerID playerID, LPCSTR pszAlias, bool bUsersPlayer = false);    

    D3DXVECTOR3 GetPlayerPosition(int charID);
    //! Returns the players world space (right up ahead position).
    D3DXVECTOR3 GetPlayerVelocity(int physicsID)                { return m_pPhysicsRoot->GetLinearVelocity(physicsID); }
    const D3DXMATRIX* GetPlayerWorldSpace(int charID)           { return &m_hmPlayers[charID].mWorld; }
    const SPlayer* GetMyPlayer()                                { return m_pMyPlayer; }
	PlayerHashMap &GetPlayerHashMap()   						{ return m_hmPlayers; }
	CGameSettings* GetGameSettings()							{ return m_pGameSettings; }
	VOID SetMyPlayerResetCamera(bool b)							{ if(m_pMyPlayer) m_pMyPlayer->bResetCamera = b; }

   	SRoundInfo &GetGameInfo() { return m_RoundInfo; }
	const D3DXMATRIX* GetMyPlayerCameraTarget() {
        if(m_pMyPlayer) return &m_pMyPlayer->CameraControl.mCameraTarget; 
        else            return NULL;
    }

    list<SShortLifeEntity> &GetShortLives()                     { return m_lShortLives; }
    vector<SSunStateRequest> &GetSunRequests()                  { return m_vSunStateRequests; }
	vector<SHUDMessageRequest> &GetHUDMessageRequests()			{ return m_vHudMessageRequests; }

    const D3DXVECTOR3* GetCameraPosOffset()                     { if(m_pMyPlayer) return &m_pMyPlayer->CameraControl.vPosOffSetCur; return NULL;}
	const D3DXVECTOR3* GetCameraLookAtOffset()                  { if(m_pMyPlayer) return &m_pMyPlayer->CameraControl.vLookAtOffsetCur; return NULL;}

    float GetCameraSpringPositionCoefficient()                  { return m_pGameSettings->GetCameraSpringK(); }
    float GetCameraSpringLookCoefficient()                      { return m_pGameSettings->GetCameraSpringLookK(); }

    LPCSTR GetAlias()                                           { return m_pGameSettings->GetPlayerAlias(); }


    // ------------------------------------------------------------------------
    // NETWORK FUNCTIONS
    // ------------------------------------------------------------------------
    bool IsConnected();
    bool IsServerActive();

    //! Disconnects from current server and connects to passed in server.
    VOID ConnectToServer(const SServerInfo &serverInfo);
    //! Uses the passed in serverInfo to host a server. (IP entry is ignored).
    VOID HostServer(SServerInfo &serverOptions, const vector<SMasterServerInfo> &m_vMasterServers);

    VOID Disconnect();    

private: // operators intentionally undefined so as to generate compiler warnings if ever called
    CGameEngine& operator=(const CGameEngine &rhs); // disallow assignment operator
    CGameEngine(const CGameEngine& rhs); // disallow copy constructor
};

// deprecated
__int64 GetKeyFromPlayerID(PlayerID playerid);  //! used for the m_hmServerPlayers hashmap
PlayerID GetPlayerIDFromKey(__int64 key);   //! used for the m_hmServerPlayers hashmap


//! type used for RPC callbacks by client and server.
typedef void (*RakNetFuncPtr)(RPCParameters *rpcParms);

// ------------------------------------------------------------------------
// SERVER RPC FUNCTIONS (called by client). 
// ------------------------------------------------------------------------
void ServerPlayerConnect(RPCParameters *pRpcParms); // client sends acknowledgement to server.
void ServerPlayerCmdUpdate(RPCParameters *pRpcParms);

// ------------------------------------------------------------------------
// CLIENT RPC FUNCTIONS (called by server). 
// ------------------------------------------------------------------------        
void ClientAcknowledge(RPCParameters *pRpcParms); // client receives personal acknowledgement from server with its networkID.
void ClientPlayerConnectAnnouncement(RPCParameters *pRpcParms); //! receive announcement of new player connection 
void ClientPlayerDisconnectAnnouncement(RPCParameters *pRpcParms);  //! receive announcement of player disconnect

void ClientRealityUpdate(RPCParameters *pRpcParms); // reality packets for all players sent by server
void ClientSpawnSLE(RPCParameters *pRpcParms); // immediate packet sent from server when an sle has been spawned.
void ClientStartRound(RPCParameters *pRpcParms);    // announcement from server when round starts.
void ClientEndRound(RPCParameters *pRpcParms);  // announcement with respawn locations for everyone on round end.