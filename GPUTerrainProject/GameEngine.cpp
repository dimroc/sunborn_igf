#include "dxstdafx.h"
#include "globaldefine.h"
#include "GameEngine.h"
#include "GameSettings.h"
#include "GameTables.h"

#include "PhysicsRoot.h"

#include "Server.h"
#include "Client.h"

using std::list;
using std::set;


#define                 MAX_NUM_TILTS                   3   // number of vertical tilts for camera


// GLobal pointer used by global RPC functions to call member functions.
CGameEngine* g_pEngine                  =               NULL;


CGameEngine::CGameEngine(bool bHosting) :
m_bHosting(bHosting),
m_pPhysicsRoot(NULL),
m_pHost(NULL),
m_pMyPlayer(NULL)
{
	m_pGameSettings = new CGameSettings();
	// default character always 0 for now. TODO: get from setting file.
    m_pClient = new CClient(GetNetworkID(), m_pGameSettings->GetPlayerAlias(), 0);	
    m_pHost = new CServer(GetNetworkID());

	PopulatePlayerCommandHashMap();
    RegisterRPCFunctions();
	memset(&m_Environment, 0, sizeof(SEnvironment));
	memset(&m_RoundInfo, 0, sizeof(SRoundInfo));
	m_RoundInfo.fRoundTimeLeft = m_pGameSettings->GetTimeBetweenRounds();

    srand((UINT)time(NULL));

    g_pEngine = this;
}

CGameEngine::~CGameEngine(void)
{
	SAFE_DELETE(m_pPhysicsRoot);
	SAFE_DELETE(m_pGameSettings);
    SAFE_DELETE(m_pHost);
    SAFE_DELETE(m_pClient);
}


void CGameEngine::OnFrameMove( float fElapsedTime, float fClockTime )
{
	assert(m_pPhysicsRoot);
	
	m_pPhysicsRoot->OnFrameMove(fElapsedTime);

	// Apply and set positions to all players velocities to all players
	PlayerHashMap::iterator playerit = m_hmPlayers.begin();
	for( ; playerit != m_hmPlayers.end(); playerit++)
	{
		// updates player velocities and states if continuous flags are set.(ie bRun == true so keep linear vel at something)
		SPlayer* player = &playerit->second;
		if (player->nLives <= 0)
			continue;

		UpdatePlayer(player, fElapsedTime);
		// update physics variables
		//m_pPhysicsRoot->SetLinearVelocity(playerit->second.physicsID, &playerit->second.vVel);
		player->bJumping = m_pPhysicsRoot->UpdatePlayerMovement(player->physicsID, &player->vVel, 
			player->fGroundOffset, player->bJumping, player->bUncontrollable, fElapsedTime);
	}

	//
	// go thru shortlifeentities and reduce ttl.
	//
	list<SShortLifeEntity>::iterator shortlifeit = m_lShortLives.begin();
	while(shortlifeit != m_lShortLives.end())
	{
		shortlifeit->fTimeToLive -= fElapsedTime;
		if(shortlifeit->fTimeToLive > 0){
			// update shortlifeentity position with the velocity and all that goodness.
			//if (shortlifeit->sleType == SShortLifeEntity::FIREBALL){
			//	D3DXVECTOR3 v = m_pPhysicsRoot->GetLinearVelocity(shortlifeit->physicsID);
			//	m_pPhysicsRoot->UpdatePlayerMovement(shortlifeit->physicsID, &v, 30, false, false, fElapsedTime);
			//}
			UpdateShortLifeEntity(&(*shortlifeit), fElapsedTime);
			shortlifeit++;
		}
		else
		{   
			m_pPhysicsRoot->DeleteDynamicObject(shortlifeit->physicsID);
			shortlifeit = m_lShortLives.erase(shortlifeit);   // erase returns next element after erased.

		}
	}

	// Update sky/sun according to summoned powers and time of day
	UpdateSkyRequest(fClockTime);
	m_Environment.fCurrentClockTime = fClockTime;
    UpdateRoundInfo(fElapsedTime);

    //
    // networking and getting packets etc.
    //
    if(m_pClient) m_pClient->ProcessPackets();
    if(m_pHost) m_pHost->ProcessPackets();
    // Network updates that check set intervals for network update.
    ServerUpdateClients(fElapsedTime);  // broadcast the real positions (or 'reality') to clients (IF SERVER).
    ClientUpdateServer(fElapsedTime);   // send action deltas to server (IF CONNECTED)
}


int CGameEngine::AddPlayer(UCHAR charType, PlayerID playerID, LPCSTR pszAlias, bool bUsersPlayer)
{
	SPlayer player;
	memset(&player, 0, sizeof(SPlayer));

    player.charID = m_pWorld->CreateCharacter(charType);
    m_pWorld->ToggleCharacterVisibility(player.charID);

	player.charType = charType;
	player.bRequestVisible = true;

    strncpy_s(player.strAlias, NUM_BYTES_FOR_NAMES, pszAlias, NUM_BYTES_FOR_NAMES);

	player.bJumping = true;
	player.nJumps = m_pGameSettings->GetPlayerJumpCount();

	// Get spawn position
	float fRadius;
	D3DXVECTOR3 vPos, vTopCenter, vBottomCenter;
	SpawnPhysicsInfo(player.charID, &vPos, &vTopCenter, &vBottomCenter, &fRadius, &(player.fGroundOffset));

	// Create the player's physics representation
	player.physicsID = m_pPhysicsRoot->AddDynamicCapsuleCombo( &vPos, 
		&vTopCenter, 
		&vBottomCenter, 
		fRadius, player.fGroundOffset);
	m_pPhysicsRoot->AddToPlayerGroup(player.physicsID);

	player.mSpace = m_PlayerSpaces.mSpaces[0];
	player.cSpaceIdx = 0; 
	player.vLastDirection = *(D3DXVECTOR3*)&player.mSpace._31;

    D3DXMatrixScaling(&player.mWorld, CHARACTER_SCALE, CHARACTER_SCALE, CHARACTER_SCALE);
	gdef::MatrixTranslation(&player.mWorld, vPos.x, vPos.y, vPos.z); // initial orientation/position for drawing.
	gdef::MatrixTranslation(&player.mSpace, vPos.x, vPos.y, vPos.z);

    // SET UP CAMERA OFFSETS FROM SETTINGS
    player.CameraControl.vPosOffSetOriginal = D3DXVECTOR3(m_pGameSettings->GetCameraOffsetX(), 
                                                   m_pGameSettings->GetCameraOffsetY(), 
                                                   m_pGameSettings->GetCameraOffsetZ());
    player.CameraControl.vLookAtOffsetOriginal = D3DXVECTOR3(m_pGameSettings->GetCameraLookAtOffsetX(), 
                                                    m_pGameSettings->GetCameraLookAtOffsetY(), 
                                                    m_pGameSettings->GetCameraLookAtOffsetZ());

    player.CameraControl.fPosOffsetMag = D3DXVec3Length(&player.CameraControl.vPosOffSetOriginal);
    player.CameraControl.fLookAtOffsetMag = D3DXVec3Length(&player.CameraControl.vLookAtOffsetOriginal);

    player.CameraControl.vPosOffSetCur = player.CameraControl.vPosOffSetOriginal;
    player.CameraControl.vLookAtOffsetCur = player.CameraControl.vLookAtOffsetOriginal;

    player.CameraControl.cTilt = 1;
    player.CameraControl.oTilt = MAX_NUM_TILTS + 1; // old tilt. to stop redundant calculations.

	player.playerID = playerID;
    GetPlayerSpawnStats(&player);

	m_hmPlayers[player.charID] = player;
	if(bUsersPlayer)
		m_pMyPlayer = &m_hmPlayers[player.charID];
    return player.charID;
}

//! Spawns player solely for server purposes. Physics entity, etc. No m_pWorld representation.
__int64 CGameEngine::AddServerPlayer(UCHAR charType, PlayerID playerID, LPCSTR pszAlias)
{
	SPlayer player;
	memset(&player, 0, sizeof(SPlayer));

	player.charType = charType;
    strncpy_s(player.strAlias, NUM_BYTES_FOR_NAMES, pszAlias, NUM_BYTES_FOR_NAMES);

	player.bJumping = true;
	player.nJumps = m_pGameSettings->GetPlayerJumpCount();

	// Get spawn position
	float fRadius;
	D3DXVECTOR3 vPos, vTopCenter, vBottomCenter;
	SpawnPhysicsInfo(player.charID, &vPos, &vTopCenter, &vBottomCenter, &fRadius, &(player.fGroundOffset));

	// Create the player's physics representation
	player.physicsID = m_pPhysicsRoot->AddDynamicCapsuleCombo( &vPos, 
		&vTopCenter, 
		&vBottomCenter, 
		fRadius, player.fGroundOffset);
	m_pPhysicsRoot->AddToPlayerGroup(player.physicsID);

    player.cSpaceIdx = 0; 
	player.mSpace = m_PlayerSpaces.mSpaces[player.cSpaceIdx];	
	gdef::MatrixTranslation(&player.mSpace, vPos.x, vPos.y, vPos.z);

	player.playerID = playerID;
    GetPlayerSpawnStats(&player);
    
    __int64 key = GetKeyFromPlayerID(playerID);
	m_hmServerPlayers[key] = player;
    return key;
}

//! Gets the players start health/charge/lives/etc.
VOID CGameEngine::GetPlayerSpawnStats(SPlayer *pplayer)
{
    // TODO: Generate these stats / or get from settings, for now hardcoded.
	pplayer->fCurrDamage = 0.f;
	pplayer->nLives = m_pGameSettings->GetPlayerStartLives();

	pplayer->fMaxCharge = 3.f;
	pplayer->fCurrCharge = 0.f;
}

CGameEngine::SPlayer* CGameEngine::GetPlayerByPhysicsID( int physicsID )
{
	PlayerHashMap::iterator playerit = m_hmPlayers.begin();
	for( ; playerit != m_hmPlayers.end(); playerit++)
	{
		if (playerit->second.physicsID == physicsID)
			return &playerit->second;
	}
	return NULL;
}

D3DXVECTOR3 CGameEngine::GetPlayerPosition(int charID)
{
	return m_pPhysicsRoot->GetPosition(m_hmPlayers[charID].physicsID);
}

VOID CGameEngine::CreatePhysicsWorld(CWorld* pWorld)
{
	SAFE_DELETE(m_pPhysicsRoot);
	m_pPhysicsRoot = new CPhysicsRoot(pWorld, m_pGameSettings);

	pWorld->GetLivableSphere(&m_Environment.vLivableSphereCenter, &m_Environment.fLivableSphereRadius); 

    m_pWorld = pWorld;
}

VOID CGameEngine::RenderPhysics(const D3DXMATRIX* pProj, const D3DXMATRIX* pView)
{ 
	if(m_pPhysicsRoot) 
		m_pPhysicsRoot->OnFrameRender(pProj, pView);
}

// ----------------------------------------------------------------------------
// INPUT LISTENAs SHIZNIT
// ----------------------------------------------------------------------------
void CGameEngine::KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown )
{
	UCHAR c = m_pGameSettings->GetKeyboardMappedFunction(nChar, bKeyDown); //! Get mapped function index
	// NOTE: the above stores if modifiers are being held down.    
	(this->*m_hmPlayerCommands[c])(m_pMyPlayer, bKeyDown, m_nMouseX, m_nMouseY, m_nMouseWheelDelta);    // call mapped function!
}

void CGameEngine::MouseProc(bool bLeftButtonDown, 
							bool bRightButtonDown, 
							bool bMiddleButtonDown, 
							bool bSideButton1Down, 
							bool bSideButton2Down, 
							int nMouseWheelDelta, 
							int xPos, int yPos)
{   
	UCHAR c;

	m_nMouseX = xPos;
	m_nMouseY = yPos;
	m_nMouseWheelDelta += nMouseWheelDelta;

	c = m_pGameSettings->GetMouseMappedFunction(0, bLeftButtonDown);
	(this->*m_hmPlayerCommands[c])(m_pMyPlayer, bLeftButtonDown, m_nMouseX, m_nMouseY, nMouseWheelDelta);

	c = m_pGameSettings->GetMouseMappedFunction(1, bRightButtonDown);
	(this->*m_hmPlayerCommands[c])(m_pMyPlayer, bRightButtonDown, m_nMouseX, m_nMouseY, nMouseWheelDelta);

	c = m_pGameSettings->GetMouseMappedFunction(2, bMiddleButtonDown);
	(this->*m_hmPlayerCommands[c])(m_pMyPlayer, bMiddleButtonDown, m_nMouseX, m_nMouseY, nMouseWheelDelta);

	c = m_pGameSettings->GetMouseMappedFunction(3, bSideButton1Down);
	(this->*m_hmPlayerCommands[c])(m_pMyPlayer, bSideButton1Down, m_nMouseX, m_nMouseY, nMouseWheelDelta);

	c = m_pGameSettings->GetMouseMappedFunction(4, bSideButton2Down);
	(this->*m_hmPlayerCommands[c])(m_pMyPlayer, bSideButton2Down, m_nMouseX, m_nMouseY, nMouseWheelDelta);
}

void CGameEngine::UpdateMouseLocation(int xPos, int yPos)
{
	m_nMouseX = xPos;
	m_nMouseY = yPos;
	// dprintf("MOUSE LOCATION: x %d y %d\n\n", xPos, yPos);
}


// ------------------------------------------------------------------------
// PUBLIC NETWORK FUNCTIONS
// ------------------------------------------------------------------------
bool CGameEngine::IsConnected()
{ 
    if(m_pClient) return m_pClient->IsConnected(); 
    return false;
}

bool CGameEngine::IsServerActive()
{ 
    return m_pHost->IsInitialized();  
}

//! Disconnects from current server and connects to passed in server.
VOID CGameEngine::ConnectToServer(const SServerInfo &serverInfo)         
{ 
    assert(m_pClient); 
    PRINT_ENTER_FUNCTION_DEBUG;
    m_pClient->ConnectToServer(serverInfo);     
}
//! Uses the passed in serverInfo to host a server. (IP entry is ignored).
VOID CGameEngine::HostServer(SServerInfo &serverOptions, const vector<SMasterServerInfo> &m_vMasterServers)           
{     
    PRINT_ENTER_FUNCTION_DEBUG;
    assert(serverOptions.nPort < 65536);
    m_pClient->Disconnect();
    if(m_pHost->StartServer(serverOptions, m_vMasterServers))
    {        
        SServerInfo localServer = serverOptions;
        localServer.strIP.assign("127.0.0.1");
        ConnectToServer(localServer);
    }
}

VOID CGameEngine::Disconnect()
{
    m_pClient->Disconnect();
    m_pHost->Disconnect();
}


// ----------------------------------------------------------------------------
// PRIVATE SHIZNIT
// ----------------------------------------------------------------------------

VOID CGameEngine::UpdateSkyRequest(float fClockTime)
{   // initiate sun morph state on sun 2 on ITS sunrise (sunset for other sun) and reset on ITS sunset.
	// this is done so the second sun is more visible during the 'nights of edge'    
	if(fClockTime > 12.f && m_Environment.bNight == false)
	{   // swap suns and set them to 'edge' state.                
		m_vSunStateRequests.push_back(SSunStateRequest(GameTable::SUNSTATE_WHITEEDGE, 1, 0));
		m_Environment.bNight = true;
	}
	else if(fClockTime < 12.f && m_Environment.bNight == true)
	{   // unswap suns to normal day time state                
		m_vSunStateRequests.push_back(SSunStateRequest(GameTable::SUNSTATE_BLACKEDGE, 1, 0));
		m_Environment.bNight = false;
	}
}

//-------------------------------------------------------------------------
//  GAME FUNCTIONS
//-------------------------------------------------------------------------
// Updates the data in the SGameInfo struct
// Checks if round is over based on time or kills and starts new rounds when necessary
VOID CGameEngine::UpdateRoundInfo(float fElapsedTime){
	m_RoundInfo.fRoundTimeLeft -= fElapsedTime;
    if(m_pHost->IsInitialized())   // only run as host
    {
	    if (m_RoundInfo.fRoundTimeLeft <= 0){
		    if (m_RoundInfo.bRoundStarted){
			    // Trigger an End of Round sequence
			    RoundEnd();
		    } else {
			    // Start the next round
                m_pHost->RPC("ClientStartRound", NULL);
		    }
	    } else {
			// Don't bother with the rest if its local, or we don't have a certain amount of players
			if (!IsConnected() || m_hmPlayers.size() <= 1)
			    return;

			int nAlive = 0;
		    PlayerHashMap::iterator playerit = m_hmPlayers.begin();
		    for( ; playerit != m_hmPlayers.end(); playerit++)
		    {   // count how many players are alive
			    if (playerit->second.nLives > 0)
				    nAlive++;
		    }

		    if (nAlive <= 1)
			    RoundEnd();
	    }
    }
    else
    {   // we're in sandbox mode, restart this bad boy.
        // I just never decrement time.
        m_RoundInfo.bRoundStarted = true;
        m_RoundInfo.fRoundTimeLeft = 0.f;
    }
}

// ------------------------------------------------------------------------
// NETWORK FUNCTIONS.
// ------------------------------------------------------------------------

VOID CGameEngine::RegisterRPCFunctions()
{
    //
    // server RPC functions (called by clients)
    //
    m_pHost->RegisterRPCFunction(&ServerPlayerConnect);    
    m_pHost->RegisterRPCFunction(&ServerPlayerCmdUpdate);

    //
    // client RPC functions (called by server)
    //
    m_pClient->RegisterRPCFunction(&ClientAcknowledge);
    m_pClient->RegisterRPCFunction(&ClientPlayerConnectAnnouncement);
    m_pClient->RegisterRPCFunction(&ClientPlayerDisconnectAnnouncement);

    m_pClient->RegisterRPCFunction(&ClientRealityUpdate);
    m_pClient->RegisterRPCFunction(&ClientSpawnSLE);
    m_pClient->RegisterRPCFunction(&ClientStartRound);
    m_pClient->RegisterRPCFunction(&ClientEndRound);
}

//! broadcast the real positions (or 'reality') to clients.    
VOID CGameEngine::ServerUpdateClients(float fElapsedTime)
{
    if(m_pHost && m_pHost->IsInitialized() && m_pHost->IsTimeForRealityBroadcast(fElapsedTime))
    {   // sends to client the server's interpretation of the world
        BroadcastReality();
    }
}

//! send action deltas to server
VOID CGameEngine::ClientUpdateServer(float fElapsedTime)
{
    if(m_pClient && m_pClient->IsConnected() && m_pClient->IsTimeForUpdate(fElapsedTime))
    {   // sends the command differences from the last ClientUpdateServer or RealityUpdate call.
        SendCmdHistory();
        m_vCmdHistory.clear();
    }
}

//! This broadcasts the server's interpretation of the world, aka, 'Reality' as opposed to client prediction.
VOID CGameEngine::BroadcastReality()    //! PREREQ: m_pHost != NULL ( player is server ).
{
    assert(m_pHost && m_pHost->IsInitialized());

    // Gather all player's positions, velocities, and current toggled commands (playermoveforward,etc.)
    RakNet::BitStream bitStream;
    bitStream.Write((UCHAR)m_hmPlayers.size()); // typecast it to uchar to take up 1 byte.

    // iterate thru players and populate the bitstream to ship off to clients
    PlayerHashMap::iterator it = m_hmPlayers.begin();
    for( ; it != m_hmPlayers.end(); it++)
    {
        SPlayerNetworkUpdate pnu(it->second);
        pnu.StuffBitStream(&bitStream);        
    }
    m_pHost->RPC("ClientRealityUpdate", &bitStream);
}

//! Server assigns spawn points to every player and broadcasts to all clients.
VOID CGameEngine::RoundEnd()
{
    RakNet::BitStream bitStream;

    UCHAR cNum = (UCHAR)m_hmPlayers.size();
    bitStream.Write((char*)&cNum, 1);

    PlayerHashMap::iterator it = m_hmPlayers.begin();
    for( ; it != m_hmPlayers.end(); it++)
    {   // Stuff bitstream with playerids and spawn points.            
        D3DXVECTOR3 spawn;
        GetSpawnPoint(&spawn);

        // assign spawn point to server player
        it->second.nLives = m_pGameSettings->GetPlayerStartLives();
        Respawn(&it->second, &spawn);

        // Stuff new spawn point for playerID into bitstream to broadcast to clients.        
        bitStream.Write((char*)&it->second.playerID, sizeof(PlayerID));
        bitStream.Write((char*)&it->second.nLives, 1);
        bitStream.WriteVector(spawn.x, spawn.y, spawn.z);
    }
    // send away
    m_pHost->RPC("ClientEndRound", &bitStream, MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true, true, GetNetworkID(), 0);
}

//! Client Update functions
VOID CGameEngine::SendCmdHistory()
{
    assert(m_pClient && m_pClient->IsConnected());

    if(m_vCmdHistory.size() > 0)    // only send if there are even cmds to send.
    {   // Create bitstream based off of a serialized m_vCmdHistory.
        RakNet::BitStream bitStream;
        //bitStream.Write(m_vCmdHistory);        
        bitStream.Write((char)m_vCmdHistory.size());
        for(size_t i = 0;i<m_vCmdHistory.size(); i++)
        {
            bitStream.Write((char*)&m_vCmdHistory[i].cPlayerCommand, 1);
            if(m_vCmdHistory[i].bKeyDown) bitStream.Write1();
            else    bitStream.Write0();
            bitStream.Write((char*)&m_vCmdHistory[i].rakTime, sizeof(RakNetTime));
        }
        m_pClient->RPC("ServerPlayerCmdUpdate", &bitStream);
    }
}

VOID CGameEngine::GetSettingsFromBitStream(RakNet::BitStream *pBitStream)
{
    assert(pBitStream);
    int nLives;
    float fRoundTime;
    pBitStream->Read(nLives);
    pBitStream->Read(fRoundTime);

    // TODO: complete the passing of all settings.
    m_pGameSettings->SetPlayerStartLives(nLives);
    m_pGameSettings->SetRoundLength(fRoundTime);
}

VOID CGameEngine::StuffSettingsToBitStream(RakNet::BitStream *pBitStream)
{
    assert(pBitStream);
    int nLives = m_pGameSettings->GetPlayerStartLives();
    float fRoundTime = m_pGameSettings->GetRoundLength();

    pBitStream->Write(nLives);
    pBitStream->Write(fRoundTime);
}


// ------------------------------------------------------------------------
// PLAYER MANIPULATION FUNCTIONS.
// ------------------------------------------------------------------------

//! Updates clients perception of reality of passed in player with the PlayerNetworkUpdate
VOID CGameEngine::RealityUpdate(SPlayer *pplayer, SPlayerNetworkUpdate *ppnu)
{
    for(int i=0;i<4;i++)
        pplayer->bMovement[i] = ppnu->bMovement[i];
    m_pPhysicsRoot->SetLinearVelocity(pplayer->physicsID, &ppnu->vVel);
    m_pPhysicsRoot->SetPosition(pplayer->physicsID, &ppnu->vPos);    
}

VOID CGameEngine::UpdateCameraPosition(SPlayer* pplayer)
{
    assert(pplayer);
    if(pplayer == NULL)
        return; // failsafe
    //
    // set camera target. 
    // lookatoffset and cameraPositionOffset are later applied to this. can get offset from pSettings.
    //
    if(pplayer->nLives <= 0)
    {   // if character is dead position camera at top of livable sphere.        
        pplayer->CameraControl.mCameraTarget = pplayer->mSpace;   // normal camera movement. 
		pplayer->CameraControl.vPosOffSetCur.y += m_Environment.fLivableSphereRadius*1.f;
		pplayer->CameraControl.vLookAtOffsetCur.y -= m_Environment.fLivableSphereRadius;
    }
    else
    {   // PLAYER LIVES
        pplayer->CameraControl.mCameraTarget = pplayer->mSpace;   // normal camera movement. 

        //
        // UPDATE OFFSETS BASED ON TILT
        //
        int nMaxTilts = MAX_NUM_TILTS;
        assert(MAX_NUM_TILTS%2 == 1);   // make sure its odd
        if(MAX_NUM_TILTS%2 == 0)
            nMaxTilts ++;
        
        assert(pplayer->CameraControl.cTilt < nMaxTilts);
        // 0 tilt is top, MAX_NUM_TILTS is from beneath.

        // I find the current angle alpha. 
        // for the upper tilts, I multiply that angle by a positive coefficient, in the case of MAX_NUM_TILTS = 3, = 2.
        // for the lower tiltes I multiply it by a negative coeffecient to place it beneath the xz plane. (-1 * alpha).
        // I then find the coordinates of any triangle with that alpha, normalize, then multiply by the old offsets magnitude.
        int nAngleMult = 1; // default behind the player pos.
                
        UCHAR cTilt = pplayer->CameraControl.cTilt; // get player requested tilt

#if 0
        // get the tilt based on player velocity when jump, look underneath when jumping up, above when descending.    
        if(pplayer->bJumping && pplayer->vVel.y > 0)    // this way kinda sucks i wont lie.
            cTilt = 2;
        else if(pplayer->bJumping && pplayer->vVel.y < 0)
            cTilt = 0;
        else
            cTilt = 1;
#endif

        if(pplayer->bDive)
            cTilt = 0;

        if(cTilt != pplayer->CameraControl.oTilt)
        {
            // magic to get the right angle multiplier. If MAX_NUM_TILTS ==3, I want either 2, 1, -1 as coefficients.
            if(cTilt <= nMaxTilts / 2)
                nAngleMult = nMaxTilts / 2 + 1 - cTilt;
            else
                nAngleMult = nMaxTilts / 2 - cTilt;

            // get the amount of tilt, find new dir, normalize, mult by magnitude, set new offset.
            float alpha = 0;
            if(pplayer->CameraControl.vPosOffSetOriginal.y != 0 && pplayer->CameraControl.vPosOffSetOriginal.z != 0)
                alpha = atan(abs(pplayer->CameraControl.vPosOffSetOriginal.y) / abs(pplayer->CameraControl.vPosOffSetOriginal.z));
            float o = tan(nAngleMult * alpha);  
            float z = 1.f;
            if(pplayer->CameraControl.vPosOffSetOriginal.z < 0)
                z = -z;
            D3DXVECTOR3 vNewOffsetDir(0.f, o, z);
            D3DXVec3Normalize(&vNewOffsetDir, &vNewOffsetDir);
#if 0
            dprintf("[UpdateCameraPosition] new direction: %f %f %f new angle mult: %d new angle: %f o: %f\n", 
                vNewOffsetDir.x, vNewOffsetDir.y, vNewOffsetDir.z, nAngleMult, alpha, o);
#endif // #if 0

            pplayer->CameraControl.vPosOffSetCur = vNewOffsetDir * pplayer->CameraControl.fPosOffsetMag;
            pplayer->CameraControl.oTilt = cTilt;   // store old tilt
        }
    }
}

VOID CGameEngine::SetPlayerAnimation(SPlayer* pplayer, UCHAR cAnimID, bool bLoopAble, float fPeriodTime)
{
	assert(pplayer);

	assert(cAnimID != GameTable::ANIM_NO);

	pplayer->animRequest.cCurAnimRequest = cAnimID;
	pplayer->animRequest.bLoopAnim = bLoopAble;
	if (!bLoopAble && fPeriodTime == -1)
		pplayer->animRequest.fPeriodTime = m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, cAnimID);
	else
		pplayer->animRequest.fPeriodTime = fPeriodTime;
}


void CGameEngine::HitPlayer(SPlayer* pplayer, SPlayer* pVictim, float fDamage, float fDazeTime, D3DXVECTOR3* pvImpulse){
	assert(pVictim);

	if (!m_RoundInfo.bRoundStarted)
		return;

	if (pVictim->bBlock){ // Reduce effect if victim is blocking
		fDamage /= 8;
		(*pvImpulse) /= 8;
		fDazeTime /= 8;
	}

	pVictim->bJumping = true; // Make player dazed
	pVictim->bDazed = true;
	pVictim->fUncontrollableTimeLeft = fDazeTime;
	ClearCharges(pVictim);
	pVictim->fCurrCharge = 0;

	pVictim->fCurrDamage += fDamage; // Apply damage
	pVictim->nLastHitBy = pplayer->charID;

	// Apply impulse
	(*pvImpulse) *= (pVictim->fCurrDamage / 100.f);
	m_pPhysicsRoot->SetFriction(pVictim->physicsID, m_pGameSettings->GetFriction() / 5);
	m_pPhysicsRoot->ApplyLinearImpulse(pVictim->physicsID, pvImpulse);

	// Update animation
	SetPlayerAnimation(pVictim, GameTable::ANIM_HIT_START, false);	

}
VOID CGameEngine::UpdateShortLifeEntity(SShortLifeEntity* pentity, float fElapsedTime)
{
	assert(pentity);

	set<int> victims;
	bool bDie = false;
	SPlayer* pplayer = GetPlayerByPhysicsID(pentity->ownerID);
	UCHAR cAnim = GameTable::ANIM_NO;

	if (pentity->physicsID != -1)
		m_pPhysicsRoot->UpdateSLE(pentity->physicsID, pentity->ownerID, victims);

	if (pentity->sleType == SShortLifeEntity::FIREBALL)
	{
		pentity->vPos = m_pPhysicsRoot->GetPosition(pentity->physicsID);
		gdef::MatrixTranslation(&pentity->mWorld, pentity->vPos.x, pentity->vPos.y, pentity->vPos.z);
		set<int>::iterator it;
		for (it = victims.begin(); it != victims.end(); it++){
			int victimID = *it;
			SPlayer* pVictim;
			if ((pVictim = GetPlayerByPhysicsID(victimID)) != NULL) {
				D3DXVECTOR3 vImp = (m_pPhysicsRoot->GetLinearVelocity(pentity->physicsID) + D3DXVECTOR3(0.f, pentity->fRadius, 0.f)) // fireballs velocity plus a bit of up velocity to get the player off the ground
					* m_pGameSettings->GetMass(); // more damage equals stronger force of impulse
				HitPlayer(pplayer, pVictim, pentity->fRadius / 2.f, .5f, &vImp);
			}
		}

		// Make the fireball move towards an enemy player
		PlayerHashMap::iterator playerit = m_hmPlayers.begin();
		for( ; playerit != m_hmPlayers.end(); playerit++)
		{
			SPlayer* pVictim = &playerit->second;
			if (pVictim->charID == pplayer->charID)
				continue;
			D3DXVECTOR3 vSep = GetPlayerPosition(pVictim) - pentity->vPos;
			float origSpeed = D3DXVec3Length(&pentity->vPos);
			float dist = D3DXVec3Length(&vSep);
			if (dist < pentity->fData){
				D3DXVec3Normalize(&vSep, &vSep);
				D3DXVECTOR3 vImp = (vSep) * origSpeed / 100.f
					* m_pGameSettings->GetMass();
				m_pPhysicsRoot->ApplyLinearImpulse(pentity->physicsID, &vImp);
				break;
			}
		}

	} 
	else if (pentity->sleType == SShortLifeEntity::LIGHTNING)
	{
		pentity->vPos = m_pPhysicsRoot->GetPosition(pentity->physicsID);
		gdef::MatrixTranslation(&pentity->mWorld, pentity->vPos.x, pentity->vPos.y, pentity->vPos.z);
		set<int>::iterator it;
		for (it = victims.begin(); it != victims.end(); it++){
			int victimID = *it;
			SPlayer* pVictim;
			if ((pVictim = GetPlayerByPhysicsID(victimID)) != NULL) {
				D3DXVECTOR3 vPPos = GetPlayerPosition(pVictim) - pentity->vPos;
				D3DXVECTOR3 vSep;
				D3DXVec3Normalize(&vSep, &vPPos);
				D3DXVECTOR3 vImp = vSep * 1000.f + D3DXVECTOR3(0.f, 3.f * pentity->fRadius, 0.f)
					* m_pGameSettings->GetMass();
				HitPlayer(pplayer, pVictim, pentity->fRadius / 2.f, 1.f, &vImp);
			}
		}

		//if (pentity->vPos.y < min(0, GetPlayerPosition(GetPlayerByPhysicsID(pentity->ownerID)).y))
		//	bDie = true; // Lightning has gone past the floor, its done
	}
	else if (pentity->sleType == SShortLifeEntity::SWEEP)
	{
		// Rotate the slentity around the player
		float deg = (pentity->fTimeToLive / pentity->fData) * 360;
		deg *= D3DX_PI / 180.f;
		D3DXVECTOR3 vPPos = m_pPhysicsRoot->GetPosition(pentity->ownerID);
		pentity->vPos.x = cos(deg) * pentity->fRadius;
		pentity->vPos.y = 0;
		pentity->vPos.z = sin(deg) * pentity->fRadius;
		pentity->vPos += vPPos;
		gdef::MatrixTranslation(&pentity->mWorld, pentity->vPos.x, pentity->vPos.y, pentity->vPos.z);
		m_pPhysicsRoot->SetPosition(pentity->physicsID, &vPPos);

		set<int>::iterator it;
		for (it = victims.begin(); it != victims.end(); it++){
			int victimID = *it;

			SPlayer* pVictim;
			if ((pVictim = GetPlayerByPhysicsID(victimID)) != NULL) {
				D3DXVECTOR3 vSep = GetPlayerPosition(pVictim) - GetPlayerPosition(pplayer);
				D3DXVECTOR3 vImp = vSep * 20.f + D3DXVECTOR3(0.f, 10.f, 0.f)
					* m_pGameSettings->GetMass();
				HitPlayer(pplayer, pVictim, pentity->fRadius / 40.f, .25f, &vImp);
			}
		}

	}
	else if (pentity->sleType == SShortLifeEntity::SMASH)
	{
		// Follow the 'X' slash path
		D3DXVECTOR3 vStart, vGoal;
		float fTime;
		float fBase = pentity->fRadius * (sqrt(2.f) / 4.f);
		float f55f = pentity->fData * .55f;
		float f45f = pentity->fData * .45f;
		float f10f = pentity->fData * .1f;

		if (pentity->fTimeToLive > f55f){ //top right to bottom left
			vStart = GetPlayerPosition(pplayer) + GetForwardVector(pplayer) * pentity->fRadius
				+ fBase * GetRightVector(pplayer)
				+ fBase * GetUpVector(pplayer);
			vGoal = GetPlayerPosition(pplayer) + GetForwardVector(pplayer) * pentity->fRadius
				- fBase * GetRightVector(pplayer)
				- fBase * GetUpVector(pplayer);
			fTime = 1.f - (pentity->fTimeToLive - f55f) / f45f;
		}
		else if (pentity->fTimeToLive > f45f){ // bottom left to top left
			vStart = GetPlayerPosition(pplayer) + GetForwardVector(pplayer) * pentity->fRadius
				- fBase * GetRightVector(pplayer)
				- fBase * GetUpVector(pplayer);
			vGoal = GetPlayerPosition(pplayer) + GetForwardVector(pplayer) * pentity->fRadius
				- fBase * GetRightVector(pplayer)
				+ fBase * GetUpVector(pplayer);
			fTime = 1.f - (pentity->fTimeToLive - f45f) / f10f;
		}
		else { // top left to bottom right
			vStart = GetPlayerPosition(pplayer) + GetForwardVector(pplayer) * pentity->fRadius
				- fBase * GetRightVector(pplayer)
				+ fBase * GetUpVector(pplayer);
			vGoal = GetPlayerPosition(pplayer) + GetForwardVector(pplayer) * pentity->fRadius
				+ fBase * GetRightVector(pplayer)
				- fBase * GetUpVector(pplayer);
			fTime = 1.f - pentity->fTimeToLive / f45f;
		}


		pentity->vPos = vStart + (vGoal - vStart) * fTime;
		gdef::MatrixTranslation(&pentity->mWorld, pentity->vPos.x, pentity->vPos.y, pentity->vPos.z);

		int victimID = -1;
		D3DXVECTOR3 vPPos = m_pPhysicsRoot->GetPosition(pentity->ownerID);
		m_pPhysicsRoot->TestLineForCollision(&pentity->vPos, &vPPos, pentity->ownerID, &victimID);
		if (victimID != -1){
			SPlayer* pVictim;
			if ((pVictim = GetPlayerByPhysicsID(victimID)) != NULL){
				D3DXVECTOR3 vSep = GetPlayerPosition(pVictim) - vPPos;
				D3DXVECTOR3 vImp = vSep * 20.f + D3DXVECTOR3(0.f, 2.f , 0.f)
					* m_pGameSettings->GetMass();
				HitPlayer(pplayer, pVictim, .6f, .25f, &vImp);
			}
		}

	}
	else if (pentity->sleType == SShortLifeEntity::DIVE){
		cAnim = GameTable::ANIM_DIVE_END;
		if (!pplayer->bDive) // not diving anymore, kill the sle
			bDie = true;
		else {
			if (!pplayer->bJumping) { // Player landed during a dive
				// Shockwave test
				// TODO: Possibly find the normal of the ground player is standing on and rotate the disc
				D3DXVECTOR3 vPPos = GetPlayerPosition(pplayer);
				PlayerHashMap::iterator playerit = m_hmPlayers.begin();
				for( ; playerit != m_hmPlayers.end(); playerit++)
				{
					SPlayer* pVictim = &playerit->second;
					if (pVictim->charID == pplayer->charID)
						continue;
					D3DXVECTOR3 vSep = GetPlayerPosition(pVictim) - vPPos;
					float dist = D3DXVec3Length(&vSep);
					if (dist < pentity->fRadius && !pVictim->bJumping){
						vSep.y = 0;
						D3DXVec3Normalize(&vSep, &vSep);
						D3DXVECTOR3 vImp = (vSep + D3DXVECTOR3(0.f, .15f, 0.f)) * pentity->fRadius
							* m_pGameSettings->GetMass();
						float bonusDmg = (pentity->fRadius - pentity->fData);
						HitPlayer(pplayer, pVictim, bonusDmg, .25f, &vImp);
					}
				}
				bDie = true; // Only cause damage once, die after this frame
				pplayer->bDive = false;
			}
			else {
				// Make the radius wider 
				pentity->fRadius += fElapsedTime * 15;
			}
		}
	}
	else
	{
		//wtf howd u get in here
		assert(false);
	}


	if ((pentity->bCollidable && !victims.empty()) || bDie){ // (collidable and collided) or force kill
		pentity->fTimeToLive = 0; // This sle should die next frame
		m_pPhysicsRoot->DeleteDynamicObject(pentity->physicsID);
		pentity->physicsID = -1;

		// Update animation
		if (cAnim != GameTable::ANIM_NO)
			SetPlayerAnimation(pplayer, cAnim, false);
	}
}

// 
D3DXVECTOR3* CGameEngine::GetSpawnPoint(D3DXVECTOR3 *pvOut)
{   // returns a random spot above terrain
    assert(pvOut);
    static const float SPHERE_RADIUS_MINIMIZER = 0.975f;

    //! default to center of sphere if th e bottom for fails.
    *pvOut = m_Environment.vLivableSphereCenter + D3DXVECTOR3(0.f, SPHERE_RADIUS_MINIMIZER, 0.f) * m_Environment.fLivableSphereRadius;	

    // get width and length of a random island. Then find a random point with that island to spawn. 
    const vector<CQuadTree*> &vpIslands = m_pWorld->GetIslands();
    size_t nIslands = vpIslands.size();
    UINT iIsland = (UINT)(rand()%nIslands);

    D3DXVECTOR3 position;
    D3DXVECTOR3 translation(*vpIslands[iIsland]->GetTranslation());

    const UINT nSpawnBuffer = 100;
    position.x = (float)(rand()%(vpIslands[iIsland]->GetWidth() - nSpawnBuffer));
    position.z = (float)(rand()%(vpIslands[iIsland]->GetLength() - nSpawnBuffer));
    position.y = 0.f;

    position += translation;
    // find appropriate height since it's a sphere and curves from center ( don't spawn outside sphere.
    // use the sin of the normalized distance (normalized with respect to bubble radius).
    float maxdist = max(abs(position.x - m_Environment.vLivableSphereCenter.x), 
                        abs(position.z - m_Environment.vLivableSphereCenter.z));
    assert(maxdist < m_Environment.fLivableSphereRadius);
    float fNormalizedLength = maxdist / m_Environment.fLivableSphereRadius;
    float newy = sin( (1-fNormalizedLength) * D3DX_PI / 2.f);
    newy *= m_Environment.fLivableSphereRadius * SPHERE_RADIUS_MINIMIZER;
    position.y = newy;

    *pvOut = position;            
    return pvOut;
}

//! Returns the current players spawn info.
VOID CGameEngine::SpawnPhysicsInfo( int charID,
							   D3DXVECTOR3* pvPos, 
							   D3DXVECTOR3* pvTopCenter, 
							   D3DXVECTOR3* pvBottomCenter, 
							   float* pfRadius, float* pfGroundOffset )
{
    GetSpawnPoint(pvPos);

	pvTopCenter->x = 0.f;
	pvTopCenter->y = 5.f * CHARACTER_SCALE;
	pvTopCenter->z = 0.f;

	pvBottomCenter->x = 0.f;
	pvBottomCenter->y = -5.f;
	pvBottomCenter->z = 0.f;

	*pfRadius = 5.f * CHARACTER_SCALE;
    *pfGroundOffset = 30.f;
}

// --------------------------------------------------------------------------------------
//! PLAYER Functions.
// --------------------------------------------------------------------------------------

//! This populates the player command func ptr hash map with the settings defined in CGameTables.h
void CGameEngine::PopulatePlayerCommandHashMap()
{
	using namespace GameTable;

	m_hmPlayerCommands[NO_ACTION] = &CGameEngine::PlayerNothing;

	// basic movement    
	m_hmPlayerCommands[MOVE_FORWARD_ACTION] = &CGameEngine::PlayerMoveForward;
	m_hmPlayerCommands[MOVE_BACKWARD_ACTION] = &CGameEngine::PlayerMoveBackward;    
	m_hmPlayerCommands[MOVE_LEFT_ACTION] = &CGameEngine::PlayerMoveLeft;
	m_hmPlayerCommands[MOVE_RIGHT_ACTION] = &CGameEngine::PlayerMoveRight;

	// special movement
	m_hmPlayerCommands[JUMP_ACTION] = &CGameEngine::PlayerJump;
	m_hmPlayerCommands[DESCEND_ACTION] = &CGameEngine::PlayerDescend;
	m_hmPlayerCommands[BLOCK_ACTION] = &CGameEngine::PlayerBlock;
	m_hmPlayerCommands[CELEBRATE_ACTION] = &CGameEngine::PlayerCelebrate;
	m_hmPlayerCommands[DASH_ACTION] = &CGameEngine::PlayerDash;

	// combat    
	m_hmPlayerCommands[ATTACK_SMASH_ACTION] = &CGameEngine::PlayerAttackSmash;
	m_hmPlayerCommands[ATTACK_VERTICAL_ONE_ACTION] = &CGameEngine::PlayerAttackVertical;
	m_hmPlayerCommands[ATTACK_VERTICAL_TWO_ACTION] = &CGameEngine::PlayerAttackSweep;
	m_hmPlayerCommands[ATTACK_RANGE_ACTION] = &CGameEngine::PlayerAttackRange;

	// camera
	m_hmPlayerCommands[ROTATE_CAMERA_LEFT_ACTION] = &CGameEngine::PlayerRotateCameraLeft;
	m_hmPlayerCommands[ROTATE_CAMERA_RIGHT_ACTION] = &CGameEngine::PlayerRotateCameraRight;
    m_hmPlayerCommands[TILT_CAMERA_ACTION] = &CGameEngine::PlayerTiltCamera;
	m_hmPlayerCommands[RESET_CAMERA_ACTION] = &CGameEngine::PlayerResetCamera;

    // respawn
    m_hmPlayerCommands[REQUEST_RESPAWN] = &CGameEngine::PlayerRequestRespawn;
}

void CGameEngine::UpdatePlayer(SPlayer* pplayer, float fElapsedTime)
{
	assert(pplayer);

	// update player position
	D3DXVECTOR3 vPlayerPos(m_pPhysicsRoot->GetPosition(pplayer->physicsID));
	gdef::MatrixTranslation(&pplayer->mSpace, vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);    

	TestForPlayerDeath(pplayer);
	
	UpdatePlayerWorldMtx(pplayer);  // update player world rendering direction with xz plane velocity request direction.    

	if(m_pMyPlayer == pplayer)  // Rotate player move space
		UpdatePlayerMoveSpace(pplayer); // done before ActualVelocity below in case use rotates the camera we can change relative vel.

	UpdatePlayerActualVelocity(pplayer);    // add requested vel to current vel.    

	UpdatePlayerTimers(pplayer, fElapsedTime);
	UpdatePlayerAnimation(pplayer, fElapsedTime);
	if (!pplayer->bJumping) {
		pplayer->nJumps = m_pGameSettings->GetPlayerJumpCount();
		pplayer->nLastHitBy = -1;
	}

    if(m_pMyPlayer == pplayer)
        UpdateCameraPosition(m_pMyPlayer);
}

void CGameEngine::UpdatePlayerActualVelocity(SPlayer* pplayer)
{    
	D3DXVECTOR3 vCurVel(m_pPhysicsRoot->GetLinearVelocity(pplayer->physicsID));
	D3DXVECTOR3 forwardVec(&pplayer->mSpace._31);	// movement direction, normalized already
	D3DXVECTOR3 rightVec(&pplayer->mSpace._11);	// movement direction, normalized already
	float fRunVel;
	if (pplayer->bDash)
		fRunVel = m_pGameSettings->GetPlayerDashSpeed();
	else 
		fRunVel = m_pGameSettings->GetPlayerRunSpeed();    

	// get player request vels
	D3DXVECTOR3 vNewMoveRequest(0.f, 0.f, 0.f);
	if (!IsCharged(pplayer) && pplayer->fUncontrollableTimeLeft <= 0){
		// foward/backward movement
		if(pplayer->bMovement[0] && !pplayer->bMovement[1])
			vNewMoveRequest += forwardVec * fRunVel;
		else if(!pplayer->bMovement[0] && pplayer->bMovement[1])
			vNewMoveRequest -= forwardVec * fRunVel;
		// left right movement
		if(pplayer->bMovement[3] && !pplayer->bMovement[2])
			vNewMoveRequest += rightVec * fRunVel;
		else if(!pplayer->bMovement[3] && pplayer->bMovement[2])
			vNewMoveRequest -= rightVec * fRunVel;
	}

	vNewMoveRequest.y = vCurVel.y;
	if (pplayer->bDive && pplayer->bJumping) {
		D3DXVECTOR3 vJumpVel(&pplayer->mSpace._21);
		vJumpVel *= -m_pGameSettings->GetPlayerJumpAccel();
		if (vNewMoveRequest.y > 0)
			vNewMoveRequest.y = 0;
		vNewMoveRequest += vJumpVel;

	}

	// add it to player's current vel
	pplayer->vVel.x = vNewMoveRequest.x;
	pplayer->vVel.y = vNewMoveRequest.y;
	pplayer->vVel.z = vNewMoveRequest.z;
}

void CGameEngine::UpdatePlayerWorldMtx(SPlayer* pplayer)
{   // TODO: Deal with rotating player if he's running ninja gaiden style along horizontal wall
	float fYDot = D3DXVec3Dot(&pplayer->vVel, (D3DXVECTOR3*)&pplayer->mSpace._21);
	D3DXVECTOR3 vDirection = pplayer->vVel - ( fYDot * *(D3DXVECTOR3*)&pplayer->mSpace._21);

	float fDistSq = D3DXVec3LengthSq(&vDirection);
	if(fDistSq > gdef::EPSILON)
	{
		D3DXVec3Normalize(&vDirection, &vDirection);
		vDirection = -vDirection; // when we import from maya the schmuck faces -z instead of +z.
		pplayer->vLastDirection = vDirection;
	}
	else
		vDirection = pplayer->vLastDirection;

	D3DXVECTOR3 vRight;    
	D3DXVec3Cross(&vRight, (D3DXVECTOR3*)&pplayer->mSpace._21, &vDirection);

    vRight *= CHARACTER_SCALE;
    vDirection *= CHARACTER_SCALE;
	memcpy(&pplayer->mWorld._11, &vRight, sizeof(float)*3);
	memcpy(&pplayer->mWorld._31, &vDirection, sizeof(float)*3);
	gdef::MatrixTranslation(&pplayer->mWorld, pplayer->mSpace._41, pplayer->mSpace._42, pplayer->mSpace._43);
}

void CGameEngine::UpdatePlayerMoveSpace(SPlayer* pplayer)
{
	if(pplayer->bResetCamera)
	{
		static char DEFAULT_SPACE_IDX = 0;
		pplayer->cSpaceIdx = DEFAULT_SPACE_IDX; // POSX

		D3DXVECTOR4 oldPos(&pplayer->mSpace._41);
		pplayer->mSpace = m_PlayerSpaces.mSpaces[DEFAULT_SPACE_IDX];

		memcpy(&pplayer->mSpace._41, &oldPos, sizeof(float)*4);
		pplayer->bCameraTurnLeft = pplayer->bCameraTurnRight = pplayer->bResetCamera = false;		
        pplayer->CameraControl.cTilt = MAX_NUM_TILTS / 2;   // 1 * alpha always gives default offset.
	}

	else if(pplayer->bCameraTurnLeft || pplayer->bCameraTurnRight)
	{   // turn the player you control
		// rotate players x and z axii accordingly        
		if(pplayer->bCameraTurnLeft)                pplayer->cSpaceIdx++;
		else                                        pplayer->cSpaceIdx--;

		if(pplayer->cSpaceIdx >= SPlayerSpaces::NUMSPACES)
			pplayer->cSpaceIdx -= SPlayerSpaces::NUMSPACES; // allow wrap around if it's like 2 under.
		else if(pplayer->cSpaceIdx < 0)
			pplayer->cSpaceIdx = (char)(SPlayerSpaces::NUMSPACES + pplayer->cSpaceIdx);        

		D3DXVECTOR4 oldPos(&pplayer->mSpace._41);        
		pplayer->mSpace = m_PlayerSpaces.mSpaces[pplayer->cSpaceIdx];
		memcpy(&pplayer->mSpace._41, &oldPos, sizeof(float)*4);

		pplayer->bCameraTurnLeft = pplayer->bCameraTurnRight = false;        
	}
}


void CGameEngine::UpdatePlayerTimers(SPlayer* pplayer, float fElapsedTime){
	if (IsCharged(pplayer)) { // increment the charge meter of a ranged attack, fire as soon as it hits max
		pplayer->fCurrCharge = min(pplayer->fCurrCharge + fElapsedTime, pplayer->fMaxCharge);
		if (pplayer->fCurrCharge == pplayer->fMaxCharge){
			if (pplayer->bCharge[SShortLifeEntity::FIREBALL])
				RangedAttack(pplayer);
			if (pplayer->bCharge[SShortLifeEntity::LIGHTNING])
				VerticalAttack(pplayer);
			if (pplayer->bCharge[SShortLifeEntity::SWEEP])
				SweepAttack(pplayer);
			if (pplayer->bCharge[SShortLifeEntity::SMASH])
				SmashAttack(pplayer);
			ClearCharges(pplayer);
			pplayer->fCurrCharge = 0;
		}
	}
	if (pplayer->bUncontrollable || pplayer->bDazed) {
		pplayer->fUncontrollableTimeLeft -= fElapsedTime;
		if (pplayer->fUncontrollableTimeLeft < 0){
			pplayer->fUncontrollableTimeLeft = 0;
			pplayer->bUncontrollable = false;
			pplayer->bDazed = false;
			//// Update this player's animation
			//if (pplayer->cLastAnimation == GameTable::ANIM_HIT_START ||
			//	pplayer->cLastAnimation == GameTable::ANIM_HIT_MID ||
			//	pplayer->cLastAnimation == GameTable::ANIM_HIT_END){
			//		if (pplayer->bJumping)
			//			SetPlayerAnimation(pplayer, GameTable::ANIM_IDLE_MIDAIR, true);				
			//		else
			//			SetPlayerAnimation(pplayer, GameTable::ANIM_IDLE, true);
			//}
		}
	}

	pplayer->fAttackDelay = max(0, pplayer->fAttackDelay - fElapsedTime);

}
void CGameEngine::UpdatePlayerAnimation(SPlayer* pplayer, float fElapsedTime){
	pplayer->animRequest.fPeriodTime -= fElapsedTime;

	/*
	Animations grouped by priority, higher number means higher priority
	Group 1 - idle, run, jump, dive
	Group 2 - lightning, fireball, sweep, smash
	Group 3 - hit, fall (these should have precedence over any other animation)
	*/

	if (!pplayer->animRequest.bLoopAnim && pplayer->animRequest.fPeriodTime > 0.05f)
		return; // Let the current animation finish (added a small buffer time so this animiation doesn't end before next frame)

	UCHAR cAnim = GameTable::ANIM_NO;
	bool bLoop = true;
	float fTime = -1;
	float fAnimTime = 1.f; // or however long start/end animations take


	// Figure out which animation needs to be shown this frame
	if (IsMoving(pplayer)){ // group 1
		if (pplayer->bJumping){
			// aerial movement
			cAnim = GameTable::ANIM_IDLE_MIDAIR;
		} else {
			cAnim = GameTable::ANIM_RUN;
		}
	} else { // not doin anthing or mid jump
		if (pplayer->bJumping){
			// aerial movement
			cAnim = GameTable::ANIM_IDLE_MIDAIR;
		} else {
			cAnim = GameTable::ANIM_IDLE;
		}
	}

	if (pplayer->bDive && pplayer->bJumping) {
		// Diving
		cAnim = GameTable::ANIM_DIVE_MID;
	}

	if (IsCharged(pplayer)) { // group 2
		if (pplayer->bCharge[SShortLifeEntity::FIREBALL])
			cAnim = GameTable::ANIM_FIREBALL_MID;
		if (pplayer->bCharge[SShortLifeEntity::LIGHTNING])
			cAnim = GameTable::ANIM_LIGHTNING_MID;
		if (pplayer->bCharge[SShortLifeEntity::SWEEP])
			cAnim = GameTable::ANIM_SPIN_MID;
		// Smash attack now is just the start
		//if (pplayer->bCharge[SShortLifeEntity::SMASH])
		//	cAnim = GameTable::ANIM_ATTACK_MID;
	}


	// group 3 - i'm hit
	if (pplayer->bDazed) {
		if (pplayer->fUncontrollableTimeLeft > fAnimTime){ // or however long the end animation lasts
			bLoop = true;
			fTime = -1;			
			if (pplayer->bJumping)
				cAnim = GameTable::ANIM_FALL_MID;
			else 
				cAnim = GameTable::ANIM_HIT_MID;

		} else {
			bLoop = false;
			if (pplayer->bJumping)
				cAnim = GameTable::ANIM_FALL_END;
			else
				cAnim = GameTable::ANIM_HIT_END;
			fTime = m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, cAnim) / 3;

		}
	}

	if (pplayer->cLastAnimation == cAnim || cAnim == GameTable::ANIM_NO)
		return; // We're already doin this animation

	// else set it
	SetPlayerAnimation(pplayer, cAnim, bLoop, fTime);
}

void CGameEngine::TestForPlayerDeath(SPlayer* pplayer){
	// Check if outside livesphere
	D3DXVECTOR3 vDistToCenter = m_Environment.vLivableSphereCenter - GetPlayerPosition(pplayer);
	float dist = D3DXVec3Length(&vDistToCenter);
	if (dist < m_Environment.fLivableSphereRadius)
		return; // we're inside the livesphere

	// Dead
	if(m_pClient->IsConnected()) {
		pplayer->nLives--;


		// Award a player a point for killing me (bastard)
		PlayerHashMap::iterator playerit = m_hmPlayers.find(pplayer->nLastHitBy);
		if (playerit != m_hmPlayers.end())
			playerit->second.nScore++;

		pplayer->nScore--;
	}

    if(pplayer->nLives > 0)
    {
        // player keeps dropping until server Reality update repositions client if playing online.
        if (m_pClient->IsConnected() == false || m_pHost->IsInitialized())
		    Respawn(pplayer); // else in singleplayer sandbox mode just respawn him.
    }

    // else its game over for this player. His camera position is updated in UpdateCameraPosition.	
}

//! Deletes player from client by removing m_pWorld entity, physics entity, and place in hashmap.
void CGameEngine::DeletePlayer(SPlayer* pplayer)
{
    assert(pplayer);
    
    m_pWorld->DeleteCharacter(pplayer->charID);
    m_pPhysicsRoot->DeleteDynamicObject(pplayer->physicsID);
    m_hmPlayers.erase(pplayer->charID);
}

//! Deletes player from server hash_map and deletes physics entity.
void CGameEngine::DeleteServerPlayer(SPlayer* pplayer)
{
    assert(pplayer);

    m_pPhysicsRoot->DeleteDynamicObject(pplayer->physicsID);
    m_hmServerPlayers.erase(GetKeyFromPlayerID(pplayer->playerID));
}

void CGameEngine::Respawn(SPlayer* pplayer, const D3DXVECTOR3 *pvRespawnPoint)
{
	assert(pplayer);

    D3DXVECTOR3 vec;
    const D3DXVECTOR3 *pvRespawn;
    if(pvRespawnPoint == NULL)  // get it from GetSpawnPoint()
        pvRespawn = GetSpawnPoint(&vec);        
    else
        pvRespawn = pvRespawnPoint;

	pplayer->fCurrDamage = 0;
	// D3DXVECTOR3 vRespawn = RequestRespawnPoint();
	D3DXVECTOR3 vNewVel(0.f, -1.f, 0.f);
	m_pPhysicsRoot->SetPosition(pplayer->physicsID, pvRespawn);
	m_pPhysicsRoot->SetLinearVelocity(pplayer->physicsID, &vNewVel);
	gdef::MatrixTranslation(&pplayer->mSpace, pvRespawn->x, pvRespawn->y, pvRespawn->z);    

	ClearCharges(pplayer);
	pplayer->fCurrCharge = 0.f;
	pplayer->bDive = false;
}

#define     PUSH_CMD_IN_HISTORY(a, b)     m_vCmdHistory.push_back(SPlayerCmdHistory(a, b, RakNet::GetTime()));

// basic movement
void CGameEngine::PlayerMoveForward(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer)
		return;	
	pplayer->bMovement[0] = bKeyDown;

	// Update this player's animation
	if (!pplayer->bJumping && pplayer->fUncontrollableTimeLeft <= 0){
		if(bKeyDown)
			SetPlayerAnimation(pplayer, GameTable::ANIM_RUN, true);			
		else if (!IsMoving(pplayer))
			SetPlayerAnimation(pplayer, GameTable::ANIM_IDLE, true);
	}

    PUSH_CMD_IN_HISTORY(GameTable::MOVE_FORWARD_ACTION, bKeyDown)
}

void CGameEngine::PlayerMoveBackward(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer)
		return;
	pplayer->bMovement[1] = bKeyDown;

	// Update this player's animation
	if (!pplayer->bJumping && pplayer->fUncontrollableTimeLeft <= 0){
		if(bKeyDown)
			SetPlayerAnimation(pplayer, GameTable::ANIM_RUN, true);			
		else if (!IsMoving(pplayer))
			SetPlayerAnimation(pplayer, GameTable::ANIM_IDLE, true);
	}
    PUSH_CMD_IN_HISTORY(GameTable::MOVE_BACKWARD_ACTION, bKeyDown)
}

void CGameEngine::PlayerMoveLeft(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer)
		return;
	pplayer->bMovement[2] = bKeyDown;

	// Update this player's animation
	if (!pplayer->bJumping && pplayer->fUncontrollableTimeLeft <= 0){
		if(bKeyDown)
			SetPlayerAnimation(pplayer, GameTable::ANIM_RUN, true);			
		else if (!IsMoving(pplayer))
			SetPlayerAnimation(pplayer, GameTable::ANIM_IDLE, true);
	}
    PUSH_CMD_IN_HISTORY(GameTable::MOVE_LEFT_ACTION, bKeyDown)
}

void CGameEngine::PlayerMoveRight(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer)
		return;
	pplayer->bMovement[3] = bKeyDown;

	// Update this player's animation
	if (!pplayer->bJumping && pplayer->fUncontrollableTimeLeft <= 0){
		if(bKeyDown)
			SetPlayerAnimation(pplayer, GameTable::ANIM_RUN, true);			
		else if (!IsMoving(pplayer))
			SetPlayerAnimation(pplayer, GameTable::ANIM_IDLE, true);
	}
    PUSH_CMD_IN_HISTORY(GameTable::MOVE_RIGHT_ACTION, bKeyDown)
}

// special / vertical movement
void CGameEngine::PlayerJump(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer || pplayer->nJumps <= 0 || pplayer->fUncontrollableTimeLeft > 0)
		return;

	// Update this player's animation
	if(bKeyDown) {
		UCHAR cAnim = 0;
		if (pplayer->bJumping)
			cAnim = GameTable::ANIM_JUMP_DOUBLE;
		else
			cAnim = GameTable::ANIM_JUMP;
		SetPlayerAnimation(pplayer, cAnim, false, m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, cAnim) / 3);
	}

	if (bKeyDown){
		pplayer->nJumps--;
		pplayer->bJumping = true;
		D3DXVECTOR3 vJumpVel(&pplayer->mSpace._21);
		vJumpVel *= m_pGameSettings->GetPlayerJumpAccel();
		D3DXVECTOR3 vVel = m_pPhysicsRoot->GetLinearVelocity(pplayer->physicsID);
		if (vVel.y < 0) vVel.y = 0.f;
		vJumpVel += vVel;
		m_pPhysicsRoot->SetLinearVelocity(pplayer->physicsID, &vJumpVel);
	}

    PUSH_CMD_IN_HISTORY(GameTable::JUMP_ACTION, bKeyDown)
}

void CGameEngine::PlayerDescend(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer || pplayer->fUncontrollableTimeLeft > 0)
		return;
	
	if (pplayer->bJumping){
		pplayer->bDive = bKeyDown;
	
		// Update animation
		UCHAR cAnim = 0;
		if (bKeyDown)
			cAnim = GameTable::ANIM_DIVE_START;
		else // make sure the last animation was a dive (since the dive can end before the player lets go of the dive button)
			if (pplayer->cLastAnimation == GameTable::ANIM_DIVE_MID || pplayer->cLastAnimation == GameTable::ANIM_DIVE_START)
				cAnim = GameTable::ANIM_DIVE_END;
		if (cAnim != GameTable::ANIM_NO)
			SetPlayerAnimation(pplayer, cAnim, false, m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, cAnim) / 3);

		if (bKeyDown && m_RoundInfo.bRoundStarted)
			DiveAttack(pplayer);
	}
	else 
		pplayer->bDive = false;



    PUSH_CMD_IN_HISTORY(GameTable::DESCEND_ACTION, bKeyDown)
}

void CGameEngine::PlayerBlock(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer)
		return;
	pplayer->bBlock = bKeyDown;

    PUSH_CMD_IN_HISTORY(GameTable::BLOCK_ACTION, bKeyDown)
}

void CGameEngine::PlayerCelebrate(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
    if(!pplayer || pplayer->fUncontrollableTimeLeft > 0)
		return;
	pplayer->bCelebrate = bKeyDown;

	if (bKeyDown) {
		SetPlayerAnimation(pplayer, GameTable::ANIM_CELEBRATE, false);
		pplayer->fUncontrollableTimeLeft = pplayer->animRequest.fPeriodTime;
		pplayer->bUncontrollable = true;
	}


    PUSH_CMD_IN_HISTORY(GameTable::CELEBRATE_ACTION, bKeyDown)
}

void CGameEngine::PlayerDash(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer || pplayer->fUncontrollableTimeLeft > 0)
		return;
	
	pplayer->bDash = bKeyDown;


    PUSH_CMD_IN_HISTORY(GameTable::DASH_ACTION, bKeyDown)
}

// attacks
void CGameEngine::PlayerAttackSmash(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if (!m_RoundInfo.bRoundStarted)
		return;
	if(!pplayer || pplayer->fAttackDelay > 0.f || pplayer->fUncontrollableTimeLeft > 0)
		return;

	ClearCharges(pplayer);
	// Update this player's animation
	if (!bKeyDown) {
		SmashAttack(pplayer);
	}		
	pplayer->bCharge[SShortLifeEntity::SMASH] = bKeyDown;

    // the server calls a client RPC to spawn the actual SLE.

    PUSH_CMD_IN_HISTORY(GameTable::ATTACK_SMASH_ACTION, bKeyDown)
}

void CGameEngine::PlayerAttackVertical(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if (!m_RoundInfo.bRoundStarted)
		return;
	if(!pplayer || pplayer->fAttackDelay > 0.f || pplayer->fUncontrollableTimeLeft > 0)
		return;

	// Update this player's animation
	if (bKeyDown){
		SetPlayerAnimation(pplayer, GameTable::ANIM_LIGHTNING_START, false, m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, GameTable::ANIM_LIGHTNING_START) / 3);
		ClearCharges(pplayer);
	} else {
		if (pplayer->bCharge[SShortLifeEntity::LIGHTNING])
			VerticalAttack(pplayer);
	}
	pplayer->bCharge[SShortLifeEntity::LIGHTNING] = bKeyDown;

    PUSH_CMD_IN_HISTORY(GameTable::ATTACK_VERTICAL_ONE_ACTION, bKeyDown)
}

void CGameEngine::PlayerAttackSweep(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if (!m_RoundInfo.bRoundStarted)
		return;
	if(!pplayer || pplayer->fAttackDelay > 0.f || pplayer->fUncontrollableTimeLeft > 0)
		return;

	// Update this player's animation
	if (bKeyDown){
		SetPlayerAnimation(pplayer, GameTable::ANIM_SPIN_START, false, m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, GameTable::ANIM_SPIN_START) / 3);
		ClearCharges(pplayer);
	} else {
		if (pplayer->bCharge[SShortLifeEntity::SWEEP])
			SweepAttack(pplayer);
	}
	pplayer->bCharge[SShortLifeEntity::SWEEP] = bKeyDown;

    PUSH_CMD_IN_HISTORY(GameTable::ATTACK_VERTICAL_TWO_ACTION, bKeyDown)
}

void CGameEngine::PlayerAttackRange(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if (!m_RoundInfo.bRoundStarted)
		return;
	if(!pplayer || pplayer->fAttackDelay > 0.f || pplayer->fUncontrollableTimeLeft > 0)
		return;

	// Update this player's animation
	if (bKeyDown){
		SetPlayerAnimation(pplayer, GameTable::ANIM_FIREBALL_START, false, m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, GameTable::ANIM_FIREBALL_START) / 3);
		ClearCharges(pplayer);
	} else {
		if (pplayer->bCharge[SShortLifeEntity::FIREBALL])
			RangedAttack(pplayer);
	}
	pplayer->bCharge[SShortLifeEntity::FIREBALL] = bKeyDown;

	PUSH_CMD_IN_HISTORY(GameTable::ATTACK_RANGE_ACTION, bKeyDown)
}

void CGameEngine::RangedAttack(SPlayer* pplayer){
	if (!m_RoundInfo.bRoundStarted)
		return;

	// deal with rendering visuals
	SShortLifeEntity slentity;
	memset(&slentity, 0, sizeof(SShortLifeEntity));
	slentity.bCollidable = true;
	slentity.fTimeToLive = 10.f;
	slentity.spriteAnimTableID = GameTable::RANGE_SPRITE;
	slentity.sleType = SShortLifeEntity::FIREBALL;

	slentity.vPos = GetPlayerPosition(pplayer);    


	// Had to make the scale a bit smaller because for some reason the physics engine would
	// crash when I used a radius of over about 40, somethin I need to look into
	slentity.fRadius = slentity.vScale.x = slentity.vScale.y = slentity.vScale.z = (10.f * (pplayer->fCurrCharge + 1));
	slentity.fData = 200.f;

	D3DXMatrixScaling(&slentity.mWorld, slentity.vScale.x, slentity.vScale.y, slentity.vScale.z);    
	gdef::MatrixTranslation(&slentity.mWorld, slentity.vPos.x, slentity.vPos.y, slentity.vPos.z);
	slentity.sseID = SShortLifeEntity::StaticID++;
	slentity.physicsID = 
		m_pPhysicsRoot->AddDynamicSphere((D3DXVECTOR3*)&pplayer->mWorld._41,
		slentity.fRadius,
		10);
	slentity.ownerID = pplayer->physicsID;
	D3DXVECTOR3 vVel = GetForwardVector(pplayer) * (m_pGameSettings->GetPlayerProjectileSpeed() * (pplayer->fCurrCharge + 1));   
	m_pPhysicsRoot->SetLinearVelocity(slentity.physicsID, &vVel);
	slentity.runningAnimId = 0;

	m_lShortLives.push_back(slentity);

	//Update player info
	float animPeriod = m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, GameTable::ANIM_FIREBALL_END) / 3;
	SetPlayerAnimation(pplayer, GameTable::ANIM_FIREBALL_END, false, animPeriod);
	pplayer->bCharge[SShortLifeEntity::FIREBALL] = false;
	pplayer->fAttackDelay = animPeriod + .2f; // set attack delay
	pplayer->fCurrCharge = 0; // reset charge
}

void CGameEngine::VerticalAttack(SPlayer* pplayer){
	if (!m_RoundInfo.bRoundStarted)
		return;
	// aka lightning
	SShortLifeEntity slentity;
	memset(&slentity, 0, sizeof(SShortLifeEntity));

	slentity.fTimeToLive = 3.f;
	slentity.spriteAnimTableID = GameTable::LIGHTNING_SPRITE;
	slentity.sleType = SShortLifeEntity::LIGHTNING;
	slentity.bCollidable = false;

	D3DXVECTOR3 vUp(0.f, 1.f, 0.f);
	slentity.vPos = (GetPlayerPosition(pplayer)) + (vUp * 500);
	slentity.vScale.x = slentity.vScale.y = slentity.vScale.z = (10.f * (pplayer->fCurrCharge + 1));

	D3DXMatrixScaling(&slentity.mWorld, slentity.vScale.x, slentity.vScale.y, slentity.vScale.z);    
	gdef::MatrixTranslation(&slentity.mWorld, slentity.vPos.x, slentity.vPos.y, slentity.vPos.z);

	slentity.fRadius = 10 * (pplayer->fCurrCharge + 1);
	slentity.sseID = SShortLifeEntity::StaticID++;
	D3DXVECTOR3 vDown = -vUp;
	slentity.physicsID = 
		m_pPhysicsRoot->AddDynamicCylinder(&slentity.vPos,
		&vUp,
		&vDown, // just make a disc
		slentity.fRadius);
	slentity.ownerID = pplayer->physicsID; // So collisions don't occur with the player that makes it
	D3DXVECTOR3 vVel = vUp * -1000.f;   
	m_pPhysicsRoot->SetLinearVelocity(slentity.physicsID, &vVel);
	m_pPhysicsRoot->SetGhost(slentity.physicsID, true);
	//	m_pPhysicsRoot->AddToSLEGroup(slentity.physicsID);

	slentity.runningAnimId = 0;

	m_lShortLives.push_back(slentity);

	//Update player info
	float animPeriod = m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, GameTable::ANIM_LIGHTNING_END) / 3;
	SetPlayerAnimation(pplayer, GameTable::ANIM_LIGHTNING_END, false, animPeriod);
	pplayer->bCharge[SShortLifeEntity::LIGHTNING] = false;
	pplayer->fAttackDelay = animPeriod + .5f; // set attack delay
	pplayer->fCurrCharge = 0; // reset charge

}
void CGameEngine::SweepAttack(SPlayer* pplayer){
	if (!m_RoundInfo.bRoundStarted)
		return;
	// This function finds the radius and position for a sweep test

	SShortLifeEntity slentity;
	memset(&slentity, 0, sizeof(SShortLifeEntity));

	slentity.sleType = SShortLifeEntity::SWEEP;
	pplayer->bUncontrollable = true;
	pplayer->fUncontrollableTimeLeft = slentity.fTimeToLive = m_pWorld->GetCharacterAnimationPeriod(pplayer->charType, GameTable::ANIM_SPIN_END) / 3;
	slentity.fData = slentity.fTimeToLive;
	slentity.fRadius = 40.f + 20.f * (pplayer->fCurrCharge + 1);

	slentity.spriteAnimTableID = GameTable::SWEEP_SPRITE;
	slentity.vPos = GetPlayerPosition(pplayer);
	slentity.vScale.x = slentity.vScale.y = slentity.vScale.z = 10.f;

	D3DXMatrixScaling(&slentity.mWorld, slentity.vScale.x, slentity.vScale.y, slentity.vScale.z);    
	gdef::MatrixTranslation(&slentity.mWorld, slentity.vPos.x, slentity.vPos.y, slentity.vPos.z);

	slentity.sseID = SShortLifeEntity::StaticID++;
	slentity.ownerID = pplayer->physicsID; // So collisions don't occur with the player that makes it
	D3DXVECTOR3 vUp(0.f, .1f, 0.f);
	D3DXVECTOR3 vDown = -vUp;
	slentity.physicsID = 
		m_pPhysicsRoot->AddDynamicCylinder(&slentity.vPos,
		&vUp,
		&vDown , // just make a disc
		slentity.fRadius);
	m_pPhysicsRoot->SetGravityMult(slentity.physicsID, 0.f); // we want the disc to stay exactly where it is
	m_pPhysicsRoot->SetGhost(slentity.physicsID, true); // don't want anything to collide physically with it

	slentity.runningAnimId = 0;

	m_lShortLives.push_back(slentity);

	//Update player info
	SetPlayerAnimation(pplayer, GameTable::ANIM_SPIN_END, false, slentity.fTimeToLive);
	pplayer->bCharge[SShortLifeEntity::SWEEP] = false;
	pplayer->fAttackDelay = slentity.fTimeToLive + .2f; // set attack delay
	pplayer->fCurrCharge = 0; // reset charge

}
void CGameEngine::DiveAttack(SPlayer* pplayer){
	if (!m_RoundInfo.bRoundStarted)
		return;
	SShortLifeEntity slentity;
	memset(&slentity, 0, sizeof(SShortLifeEntity));

	slentity.sseID = SShortLifeEntity::StaticID++;
	slentity.ownerID = pplayer->physicsID; // So collisions don't occur with the player that makes it
	slentity.sleType = SShortLifeEntity::DIVE;
	slentity.spriteAnimTableID = GameTable::NO_SPRITE;
	slentity.runningAnimId = 0;
	slentity.fTimeToLive = 99999.f;

	slentity.fData = slentity.fRadius = 60.f; // Base radius, will be increased the longer we dive
	slentity.physicsID = -1; // Don't have one

	m_lShortLives.push_back(slentity);
}
void CGameEngine::SmashAttack(SPlayer* pplayer){
	if (!m_RoundInfo.bRoundStarted)
		return;

	SShortLifeEntity slentity;
	memset(&slentity, 0, sizeof(SShortLifeEntity));

	slentity.fData = slentity.fTimeToLive = 1.f;
	slentity.spriteAnimTableID = GameTable::SMASH_SPRITE;
	slentity.sleType = SShortLifeEntity::SMASH;

	float fRadius = slentity.fRadius = 30.f;// + 5.f * (pplayer->fCurrCharge + 1);

	slentity.vPos = GetPlayerPosition(pplayer) + GetForwardVector(pplayer) * fRadius;
	float off = fRadius * (sqrt(2.f) / 4.f);
	slentity.vPos -= off * GetRightVector(pplayer);
	slentity.vPos += off * GetUpVector(pplayer);
	slentity.vScale.x = slentity.vScale.y = slentity.vScale.z = 5.f;

	D3DXMatrixScaling(&slentity.mWorld, slentity.vScale.x, slentity.vScale.y, slentity.vScale.z);    
	gdef::MatrixTranslation(&slentity.mWorld, slentity.vPos.x, slentity.vPos.y, slentity.vPos.z);

	slentity.sseID = SShortLifeEntity::StaticID++;
	slentity.ownerID = pplayer->physicsID; // So collisions don't occur with the player that makes it
	slentity.physicsID = -1;

	slentity.runningAnimId = 0;

	m_lShortLives.push_back(slentity);

	// Update player
	SetPlayerAnimation(pplayer, GameTable::ANIM_ATTACK_START, false, slentity.fTimeToLive);
	pplayer->bCharge[SShortLifeEntity::SMASH] = false;
	pplayer->fAttackDelay = slentity.fTimeToLive; // set attack delay
	pplayer->fCurrCharge = 0; // reset charge

}

void CGameEngine::PlayerRotateCameraLeft(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer || !bKeyDown)
		return;
	pplayer->bCameraTurnLeft = true;

    PUSH_CMD_IN_HISTORY(GameTable::ROTATE_CAMERA_LEFT_ACTION, bKeyDown)
}

void CGameEngine::PlayerRotateCameraRight(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer || !bKeyDown)
		return;
	pplayer->bCameraTurnRight = true;

    PUSH_CMD_IN_HISTORY(GameTable::ROTATE_CAMERA_RIGHT_ACTION, bKeyDown)
}

void CGameEngine::PlayerTiltCamera(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer || !bKeyDown)
		return;    
	pplayer->CameraControl.cTilt++;
    if(pplayer->CameraControl.cTilt >= MAX_NUM_TILTS)
        pplayer->CameraControl.cTilt = 0;    

    // PUSH_CMD_IN_HISTORY(GameTable::ROTATE_CAMERA_RIGHT_ACTION, bKeyDown) // does not affect controls etc so dont send to server.
}

void CGameEngine::PlayerResetCamera(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!pplayer || !bKeyDown)
		return;
	pplayer->bResetCamera = true;

    PUSH_CMD_IN_HISTORY(GameTable::RESET_CAMERA_ACTION, bKeyDown)
}

void CGameEngine::PlayerRequestRespawn(SPlayer* pplayer, bool bKeyDown, int xPos, int yPos, int nMouseWheelDelta)
{
	if(!bKeyDown)
		return;

    // pplayer->charType = GameTable::RED_NINJA // todo set character type.

    if(m_pClient->IsConnected())
    {   // Ask server for respawn (if u have lives).
        PUSH_CMD_IN_HISTORY(GameTable::REQUEST_RESPAWN, bKeyDown)
    }
    else
    {   // If have player respawn when in sandbox mode
        if(pplayer) 
        {            
		    Respawn(pplayer);
        }
    }
}


// --------------------------------------------------------------------------------------
//! STRUCT SHIZNIT Functions.
// --------------------------------------------------------------------------------------

// SPlayerSpaces
CGameEngine::SPlayerSpaces::SPlayerSpaces()
{
	float rads = 2 * D3DX_PI / NUMSPACES;
	for(int i=0; i<NUMSPACES; i++)
		D3DXMatrixRotationY(&mSpaces[i], rads * i);
}

int CGameEngine::SShortLifeEntity::StaticID = 0;

// SPlayerNetworkUpdate
CGameEngine::SPlayerNetworkUpdate::SPlayerNetworkUpdate(const SPlayer &player)
{
    memcpy(this->bMovement, player.bMovement, sizeof(bool)*4);
    this->playerID = player.playerID;
    this->cLastAnimation = player.cLastAnimation;
    this->vPos = *(D3DXVECTOR3*)&player.mWorld._41;
    this->vVel = player.vVel;
}

CGameEngine::SPlayerNetworkUpdate::SPlayerNetworkUpdate(RakNet::BitStream *pBitStream)
{
    assert(pBitStream);

    pBitStream->Read((char*)&this->playerID, sizeof(PlayerID));
    pBitStream->ReadVector(this->vPos.x, this->vPos.y, this->vPos.z);
    pBitStream->ReadVector(this->vVel.x, this->vVel.y, this->vVel.z);

    for(int i=0; i<4; i++)
        this->bMovement[i] = pBitStream->ReadBit();

     pBitStream->Read(this->cLastAnimation);
}

CGameEngine::SPlayerNetworkUpdate::SPlayerNetworkUpdate()
{
}
CGameEngine::SPlayerNetworkUpdate::~SPlayerNetworkUpdate()
{
}

VOID CGameEngine::SPlayerNetworkUpdate::StuffBitStream(RakNet::BitStream *pBitStream)
{
    pBitStream->Write((char*)&playerID, sizeof(PlayerID));
    pBitStream->WriteVector(this->vPos.x, this->vPos.y, this->vPos.z);
    pBitStream->WriteVector(this->vVel.x, this->vVel.y, this->vVel.z);

    for(int i=0; i<4; i++)
        pBitStream->Write(this->bMovement[i]);  // writing a bool takes 1 bit.

    pBitStream->Write(this->cLastAnimation);    
}

// SPlayerCmdHistory
CGameEngine::SPlayerCmdHistory::SPlayerCmdHistory(UCHAR _cPlayerCommand, bool _bKeyDown, RakNetTime _rakTime)
{
    this->cPlayerCommand = _cPlayerCommand;
    this->bKeyDown = _bKeyDown;
    this->rakTime = _rakTime;
}

//! used for the m_hmServerPlayers hashmap
__int64 GetKeyFromPlayerID(PlayerID playerid)
{
    assert(sizeof(__int64) >= sizeof(PlayerID));
    __int64 temp = 0;
    memcpy(&temp, &playerid, sizeof(PlayerID));
    return temp;
}

//! used for the m_hmServerPlayers hashmap
PlayerID GetPlayerIDFromKey(__int64 key)
{
    assert(sizeof(__int64) >= sizeof(PlayerID));
    PlayerID playerid;
    memcpy(&playerid, &key, sizeof(PlayerID));
    return playerid;
}



// ------------------------------------------------------------------------
// GLOBAL RPC FUNCTIONS
// ------------------------------------------------------------------------

// Server RPCs.
void ServerPlayerCmdUpdate(RPCParameters *pRpcParms)
{   // deal with the command history sent in from the client
    RakNet::BitStream bitStream(pRpcParms->input, pRpcParms->numberOfBitsOfData / 8, false);

    char nCmds;
    bitStream.Read(nCmds);
    assert(nCmds > 0);

    for(char i=0; i<nCmds; i++)
    {
        char cCommand;
        bool bKeyDown;
        RakNetTime time;
        bitStream.Read(&cCommand, 1);
        bKeyDown = bitStream.ReadBit();
        bitStream.Read((char*)&time, sizeof(RakNetTime));

        // find player based on playerID.
        CGameEngine::SPlayer *pplayer = NULL;
        CGameEngine::PlayerHashMap::iterator it = g_pEngine->m_hmPlayers.begin();
        for( ; it != g_pEngine->m_hmPlayers.end(); it++)
        {
            if(it->second.playerID == pRpcParms->sender)
            {
                pplayer = &it->second;
                break;
            }
        }

        assert(pplayer);
        if(pplayer && pplayer->playerID != g_pEngine->m_pClient->GetPlayerID() )
        {
            // perform operation for x time for this player. This player is not server
            (g_pEngine->*g_pEngine->m_hmPlayerCommands[cCommand])(pplayer, bKeyDown, 0, 0, 0);
        }
        else if(!pplayer)
            dprintf("Could not find player for cmd history(probably local player. PlayerID: %s\n", pRpcParms->sender.ToString(true));
    }         
}

void ServerPlayerConnect(RPCParameters *pRpcParms)
{
    assert(g_pEngine->m_pHost);
    RakNet::BitStream bitStreamIn(pRpcParms->input, pRpcParms->numberOfBitsOfData / 8, false);    // skip packet identifier

    UCHAR charType; 
    char pszAlias[NUM_BYTES_FOR_NAMES];		
    bitStreamIn.Read(pszAlias, NUM_BYTES_FOR_NAMES);   // got player name
    bitStreamIn.Read(charType); // get player character type
    pszAlias[NUM_BYTES_FOR_NAMES-1] = NULL;

    // send personal acknowledgement to client with his playerID
    RakNet::BitStream bitStreamOut;
    bitStreamOut.Write((char*)&pRpcParms->sender, sizeof(PlayerID));    // write player id

    // write host settings
    g_pEngine->StuffSettingsToBitStream(&bitStreamOut);
    
    g_pEngine->m_pHost->RPC("ClientAcknowledge", &bitStreamOut, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, pRpcParms->sender, false, true, UNASSIGNED_NETWORK_ID, 0);
    
    //announce new player to all clients
    bitStreamOut.Reset();
    bitStreamOut.Write((char*)&pRpcParms->sender, sizeof(PlayerID));    // write player id
    bitStreamOut.Write(pszAlias, NUM_BYTES_FOR_NAMES);  // write name
	bitStreamOut.Write((char*)&charType, 1);   // write player type

    dprintf("Player \"%s\" has joined the server.\n", pszAlias);
    g_pEngine->m_pHost->RPC("ClientPlayerConnectAnnouncement", &bitStreamOut, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true, true, UNASSIGNED_NETWORK_ID, 0);


    // add player to server's player list.
    if(strncmp("127.0.0.1", pRpcParms->sender.ToString(false), 15) == 0)
        g_pEngine->AddPlayer(charType, pRpcParms->sender, pszAlias, true);    
    else
        g_pEngine->AddPlayer(charType, pRpcParms->sender, pszAlias, false);


    // first player to join server so start round sequence with end round to spawn players during limbo time.
    if(g_pEngine->m_hmPlayers.size() == 1)    
        g_pEngine->RoundEnd();
}

// client receives personal acknowledgement from server with its networkID and server settings.
void ClientAcknowledge(RPCParameters *pRpcParms)
{
    RakNet::BitStream bitStream(pRpcParms->input, pRpcParms->numberOfBitsOfData / 8, false);
    
    PlayerID playerID;
    bitStream.Read((char*)&playerID, sizeof(PlayerID)); // read clients playerID
    
	dprintf("Server has acknowledged our connection with the ip: %s.\n", playerID.ToString(true));
    g_pEngine->m_pClient->SetPlayerID(playerID);

    // read server settings
    g_pEngine->GetSettingsFromBitStream(&bitStream);    
}

//! receive announcement of new player connection 
void ClientPlayerConnectAnnouncement(RPCParameters *pRpcParms)
{   
    RakNet::BitStream bitStream(pRpcParms->input, pRpcParms->numberOfBitsOfData / 8, false);    

    UCHAR charType;
    PlayerID playerID;
    char pszAlias[NUM_BYTES_FOR_NAMES];
    bitStream.Read((char*)&playerID, sizeof(PlayerID));    
    bitStream.Read(pszAlias, NUM_BYTES_FOR_NAMES);  // get player name
    pszAlias[NUM_BYTES_FOR_NAMES-1]=NULL;   // stopbufferoverflow.
	bitStream.Read(charType);

    // announce player joins
    CGameEngine::SHUDMessageRequest msg;
    msg.text.assign(pszAlias);
    msg.text.append(" has joined the server.\n");
    msg.fTimeToLive = 4.f;
    msg.bBigFont = false;
    g_pEngine->m_vHudMessageRequests.push_back(msg);

    if(g_pEngine->m_pHost->IsInitialized() == false)
    {   //make sure u are not server

        // add player but make him dead by default
        int nCharID;
        if(playerID == g_pEngine->m_pClient->GetPlayerID())
            nCharID = g_pEngine->AddPlayer(charType, playerID, pszAlias, true);
        else
            nCharID = g_pEngine->AddPlayer(charType, playerID, pszAlias, false);

	    CGameEngine::SPlayer *pplayer = &g_pEngine->m_hmPlayers[nCharID];
        pplayer->playerID = playerID;

	    // make player dead
	    pplayer->nLives = 0;
        pplayer->bRequestVisible = false;
        if(pplayer->playerID == g_pEngine->m_pClient->GetPlayerID())
            g_pEngine->UpdateCameraPosition(pplayer);
    }
    else
    {   // you are server so you already added player.
    }
}

//! announce all connected of departure.
void ClientPlayerDisconnectAnnouncement(RPCParameters *pRpcParms)
{
	RakNet::BitStream bitStream(pRpcParms->input, pRpcParms->numberOfBitsOfData / 8, false);
	
	PlayerID playerID;
	bitStream.Read((char*)&playerID, sizeof(PlayerID));
	CGameEngine::PlayerHashMap::iterator itPlayers = g_pEngine->m_hmPlayers.begin();
    for( ; itPlayers != g_pEngine->m_hmPlayers.end(); itPlayers++)
    {   
		if(itPlayers->second.playerID == playerID)
		{
            g_pEngine->DeletePlayer(&itPlayers->second);

			// display msg.
			CGameEngine::SHUDMessageRequest msg;
			msg.text.assign(itPlayers->second.strAlias);
			msg.text.append(" has left the server.");
			msg.fTimeToLive = 6.f;
			msg.bBigFont = false;
			g_pEngine->m_vHudMessageRequests.push_back(msg);
		}
		else
			dprintf("WTF I couldn't find player to kick with ip: %s\n", playerID.ToString(true));
	}
}

void ClientSpawnSLE(RPCParameters *pRpcParms)
{   // spawn an SLE
    PRINT_ENTER_FUNCTION_DEBUG

}

void ClientRealityUpdate(RPCParameters *pRpcParms)
{   // the 'reality check'
    // PRINT_ENTER_FUNCTION_DEBUG

    RakNet::BitStream bitStream(pRpcParms->input, pRpcParms->numberOfBitsOfData / 8, false);
    UCHAR nPlayers;
    bitStream.Read(nPlayers);

    // Place PlayerNetworkUpdates in an array.
    list<CGameEngine::SPlayerNetworkUpdate> lPNU;
    for(UCHAR i=0; i<nPlayers; i++)
    {
        CGameEngine::SPlayerNetworkUpdate pnu(&bitStream);
        lPNU.push_back(pnu);
    }

    // iterate through players updating existence.
    list<CGameEngine::SPlayerNetworkUpdate>::iterator itPNU;
    CGameEngine::PlayerHashMap::iterator itPlayers = g_pEngine->m_hmPlayers.begin();
    for( ; itPlayers != g_pEngine->m_hmPlayers.end(); itPlayers++)
    {   // eww i think i got an n^2 going.
        itPNU = lPNU.begin();
        while(itPNU != lPNU.end())
        {
            if(itPlayers->second.playerID == itPNU->playerID)
            {
                g_pEngine->RealityUpdate(&itPlayers->second, &(*itPNU));
                itPNU = lPNU.erase(itPNU);
            }
            else
                itPNU++;
        }
    }

    // wtf nonexistent players!@#! perhaps reality update happened before add player spawn wtever.
    itPNU = lPNU.begin();
    for( ; itPNU != lPNU.end(); itPNU++)
    {   // bleh i dont think im gonna add players this way
        dprintf("WTF UPDATE FOR NON EXISTENT PLAYER: %s\n", itPNU->playerID.ToString(true));        
    }
}

// only server can start/end round
void ClientStartRound(RPCParameters *pRpcParms)
{    
	if (g_pEngine->m_RoundInfo.bRoundStarted)
		return;

	g_pEngine->m_RoundInfo.fRoundTimeLeft = g_pEngine->m_pGameSettings->GetRoundLength();
	g_pEngine->m_RoundInfo.bRoundStarted = true;
	g_pEngine->m_RoundInfo.nRound++;

	std::stringstream ss;
	ss << "ROUND " << g_pEngine->m_RoundInfo.nRound << " BEGIN!";
	CGameEngine::SHUDMessageRequest rqst;
	rqst.bBigFont = true;
	rqst.fTimeToLive = 3.f;
	rqst.text.append(ss.str().c_str());
	g_pEngine->m_vHudMessageRequests.push_back(rqst);

	if (!g_pEngine->IsConnected())
		return;
	
	g_pEngine->m_RoundInfo.bHideScoreboardRequest = true;
}

void ClientEndRound(RPCParameters *pRpcParms)
{
    assert(g_pEngine->IsConnected());
#if 0
	if (!m_RoundInfo.bRoundStarted)
		return;
#endif

    RakNet::BitStream bitStream(pRpcParms->input, pRpcParms->numberOfBitsOfData / 8, false);

	g_pEngine->m_RoundInfo.fRoundTimeLeft = g_pEngine->m_pGameSettings->GetTimeBetweenRounds();
	g_pEngine->m_RoundInfo.bRoundStarted = false;

	std::stringstream ss;
	ss << "ROUND " << g_pEngine->m_RoundInfo.nRound << " OVER!";
	CGameEngine::SHUDMessageRequest rqst;
	rqst.bBigFont = true;
	rqst.fTimeToLive = 3.f;
	rqst.text.append(ss.str().c_str());
	g_pEngine->m_vHudMessageRequests.push_back(rqst);
	ss.str("");

    UCHAR cNumPlayers;
    bitStream.Read((char*)&cNumPlayers, 1);

    for(int i = 0; i<cNumPlayers; i++)
    {   // eww n^2. its just of players tho so wtever.
        D3DXVECTOR3 spawn;
        PlayerID playerID;
        int nLives = 0;
        bitStream.Read((char*)&playerID, sizeof(PlayerID));
        bitStream.Read((char*)&nLives, 1);
        bitStream.ReadVector(spawn.x, spawn.y, spawn.z);
	    CGameEngine::PlayerHashMap::iterator playerit = g_pEngine->m_hmPlayers.begin();
	    for( ; playerit != g_pEngine->m_hmPlayers.end(); playerit++)
	    {
            if(playerit->second.playerID == playerID)
            {
                // Clear all character's charges
		        g_pEngine->ClearCharges(&playerit->second);
		        (&playerit->second)->fCurrCharge = 0.f;
                (&playerit->second)->nLives = nLives;

                // Respawn character with points sent by server.
                g_pEngine->Respawn(&playerit->second, &spawn);
                break;
            }
	    }
    }

	// Determine a winner
	//ss << "Winner: Player " << 
    
	// Show the scoreboard
	g_pEngine->m_RoundInfo.bShowScoreboardRequest = true;
}