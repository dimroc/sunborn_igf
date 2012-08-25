#include "dxstdafx.h"
#include "resource.h"
#include "GameRoot.h"

#include "GameEngine.h" // Game Top
#include "World.h" // graphics top
#include "NetworkDefine.h"
#include "NetworkRoot.h" // network browser top
#include "../mp3player/mp3player.h"  // music top (not soundfx)
#include "GameGUI.h"
#include "GameTables.h"



CGameRoot::CGameRoot(void)  :
    m_bControlCamera(false), m_bRenderPhysics(false),
    m_pWorld(NULL)
{    
    m_pEngine = new CGameEngine();    
	m_pGUI = new CGameGUI(this);
    m_pNetworkRoot = new CNetworkRoot();
    m_pMP3Player = new Cmp3player();    

    //! TODO:
    // Make a splash screen Through WIN32 Magic (since the d3d device isn't made yet) 
    // that appears while loading
}

CGameRoot::~CGameRoot(void)
{
    //! Cleans up items that get recreated in OnCreateDevice().
    OnLostDevice();
    OnDestroyDevice();

    //! Clean up items that are allocated in heap outside of OnCreateDevice().
    SAFE_DELETE(m_pEngine); 
    SAFE_DELETE(m_pNetworkRoot);
	SAFE_DELETE(m_pGUI);
    SAFE_DELETE(m_pMP3Player);
}

// --------------------------------------------------------------------------------------
//! Framework function calls.
// --------------------------------------------------------------------------------------

HRESULT CGameRoot::OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{
    if(m_strWorldFile.empty() || m_strModelFile.empty() || m_strCharacterFile.empty())
        return S_FALSE;

    m_pWorld = new CWorld(m_strWorldFile, m_strModelFile, m_strCharacterFile, m_strSpriteFile);
    m_pWorld->OnCreateDevice(pd3dDevice);
    m_pWorld->SetCameraVariables(m_pEngine->GetCameraSpringPositionCoefficient(),                                    
                                   m_pEngine->GetCameraSpringLookCoefficient());



    m_pEngine->CreatePhysicsWorld(m_pWorld); //! pass in world info.        
	m_pGUI->OnCreateDevice(pd3dDevice);
	PopulateCommandHashMap();

    //! Play theme music on startup
    m_pMP3Player->AddFileToList(L"../Media/sound/theme_track.mp3");       
    m_pMP3Player->AddFileToList(L"../Media/sound/battle_track1.mp3");
    m_pMP3Player->AddFileToList(L"../Media/sound/battle_track2.mp3");
    m_pMP3Player->LoopSong(false);
    m_pMP3Player->LoopList(true);
    m_pMP3Player->PlayFromList(0, DXUTGetHWND(), WM_GRAPHNOTIFY);

    return S_OK;
}
//! Cleans up items that get recreated in OnCreateDevice().
VOID CGameRoot::OnDestroyDevice()
{
    SAFE_DELETE(m_pWorld);
}

HRESULT CGameRoot::OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{
    HRESULT hr; 
    m_pd3dDevice = pd3dDevice;

    m_pWorld->OnResetDevice(pd3dDevice);    
	m_pGUI->OnResetDevice(pd3dDevice);

    V( m_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &m_pVanillaStateBlock) );

    return S_OK;
}

HRESULT CGameRoot::OnLostDevice()
{
    m_pWorld->OnLostDevice();
	m_pGUI->OnLostDevice();

    SAFE_RELEASE(m_pVanillaStateBlock);

    return S_OK;
}

bool CGameRoot::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    bool bReturn = false;
    if(m_bControlCamera && m_pWorld)
        bReturn = (bool)m_pWorld->GetCamera().HandleMessages( hWnd, uMsg, wParam, lParam );

    switch(uMsg)
    {
    case WM_GRAPHNOTIFY: // event by mp3player
        m_pMP3Player->HandleGraphEvent(hWnd, WM_GRAPHNOTIFY);
        break;    
    case WM_MOUSEMOVE:
        // update mouse coordinates when the user moves the mouse.
        m_pEngine->UpdateMouseLocation(LOWORD(lParam), HIWORD(lParam));
        m_pGUI->UpdateMouseLocation(LOWORD(lParam), HIWORD(lParam));
        break;
    }
   
    return bReturn;
}

//! Keys sent to game engine.
void CGameRoot::KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown )
{				
    //! This is just to test out graphics, not keyboard handler for actual game. Turn off in real builds.
    m_pWorld->KeyboardProc( nChar, bKeyDown, bAltDown );    

#if defined(DEBUG) || defined(_DEBUG)
	if(bKeyDown && nChar == 'P' && bAltDown)
		m_bRenderPhysics = !m_bRenderPhysics;
#endif

	// I'm adding the toggle menu button here, if you want it somewhere else just move it -Richard
	if (bKeyDown && nChar == 'M' && bAltDown)
		m_pGUI->ToggleMenu();
	
	if (bKeyDown && nChar == 'O' && bAltDown){
		m_pGUI->SetupScoreboard(m_pEngine->GetPlayerHashMap());
		m_pGUI->ToggleScoreboard();
	}

    if (bKeyDown && nChar == 'C' && bAltDown)
        ToggleCameraControl("");

    if(!m_pGUI->KeyboardProc(nChar, bKeyDown) && !m_bControlCamera)
    {   //! Control Character if the Camera is fixed behind player and not being controlled by user.
        //TODO: ugly hack to compensate for directx samples weird ascii keyboard listening
        m_pEngine->KeyboardProc( nChar, bKeyDown, bAltDown );        
		m_pGUI->ResetHUDAlpha();
    }
}


void CGameRoot::MouseProc(bool bLeftButtonDown, 
                          bool bRightButtonDown, 
                          bool bMiddleButtonDown, 
                          bool bSideButton1Down, 
                          bool bSideButton2Down, 
                          int nMouseWheelDelta, 
                          int xPos, int yPos)
{
	m_pGUI->MouseProc(bLeftButtonDown, xPos, yPos);

    m_pEngine->MouseProc(bLeftButtonDown, 
                         bRightButtonDown, 
                         bMiddleButtonDown, 
                         bSideButton1Down, 
                         bSideButton2Down,
                         nMouseWheelDelta,
                         xPos,
                         yPos);    
}

VOID CGameRoot::OnFrameMove( double fTime, float fElapsedTime )
{
    //
    // HACK: MODEL TEST
    // TODO: REMOVE
    //
    if( m_pEngine->GetMyPlayer() == NULL && m_pEngine->IsConnected() == false && !m_pEngine->IsServerActive())
    {
        SpawnCharacter(0);  // they all spawn at random points.
        SpawnCharacter(0);
        SpawnCharacter(0);
        SpawnCharacter(0);
	}
    //
    // HACK: END
    //

    //
    // TODO: Put in separate thread.
    //
    
    m_pEngine->OnFrameMove(fElapsedTime, m_pWorld->GetTimeOfDay());        
    m_pWorld->OnFrameMove(fTime, fElapsedTime);    
    UpdateWorld(); // Update graphical world with physical location/orientation and sky updates.        

    m_pGUI->SetupServerBrowser(m_pNetworkRoot->GetServerList());
	m_pGUI->OnFrameMove(fTime, fElapsedTime);

    m_pNetworkRoot->ProcessPackets();
}

VOID CGameRoot::OnFrameRender()
{
    HRESULT hr;
   
    V( m_pVanillaStateBlock->Apply() ); // Clear states every frame.
    V( m_pd3dDevice->BeginScene() );
		
    m_pWorld->OnFrameRender( 0 );
    if(m_bRenderPhysics)
        m_pEngine->RenderPhysics(m_pWorld->GetCamera().GetProjMatrix(), m_pWorld->GetCamera().GetViewMatrix());

	m_pGUI->OnFrameRender(m_pd3dDevice, m_pEngine->GetPlayerHashMap(), m_pEngine->GetGameInfo());
    V( m_pd3dDevice->EndScene() );	
}



VOID CGameRoot::LoadMasterServersFile(const string &mastersFN)
{
    m_strMasterServersFile = mastersFN;
    dprintf("LOADING MASTER SERVERS FILE: %s\n", mastersFN.c_str());

    char dir[1000];
    _getcwd(dir, 999);
    _chdir("..\\Media");

    std::ifstream fin(mastersFN.c_str());
    assert(fin.good());

    int nMasterServers = 0;
    if(fin.good())
        fin >> nMasterServers;

    assert(m_pNetworkRoot);
    for(int i=0; i<nMasterServers && fin.good(); i++)
    {   // each line is a master server, populate a struct then push it onto the vector.
        string strIP;
        USHORT nPort;
        fin >> strIP >> nPort;
        m_pNetworkRoot->AddMasterServer(strIP, nPort);        
    }
    fin.close();
    _chdir(dir);    
}


// --------------------------------------------------------------------------------------
//! Private Functions.
// --------------------------------------------------------------------------------------


VOID CGameRoot::SpawnCharacter(UCHAR cCharType)
{   // Create A LOCAL ONLY CHARACTER
    PlayerID playerID;
    playerID.binaryAddress = playerID.port = 0;
    int charID = m_pEngine->AddPlayer(cCharType, playerID, m_pEngine->GetAlias(), true);
	m_pGUI->SetupHUD(m_pEngine->GetMyPlayer());
#if 0
    D3DXMATRIX mtx;
    D3DXMatrixIdentity(&mtx);
    D3DXVECTOR3 pos(m_pEngine->GetPlayerPosition(charID));
    gdef::MatrixTranslation(&mtx, pos.x, pos.y, pos.z); 

    D3DXVECTOR3 cam(pos);
    cam.z -= 100; cam.y += 100;
    m_pWorld->SetCameraViewParams(&cam, &pos);
    m_pWorld->SetCameraOrientationTarget(&mtx);
#endif
}

//! Updates the graphical CWorld with the GameEngine positions
VOID CGameRoot::UpdateWorld()
{    
    typedef CGameEngine::SShortLifeEntity SShortLifeEntity;
    typedef CGameEngine::SSunStateRequest SSunStateRequest;
	typedef CGameEngine::SHUDMessageRequest SHUDMessageRequest;
    typedef CGameEngine::PlayerHashMap PlayerHashMap;

    // update player characters world matrices and animations    
    PlayerHashMap &hmPlayers = m_pEngine->GetPlayerHashMap();
    PlayerHashMap::iterator playerIt = hmPlayers.begin();
    for( ; playerIt != hmPlayers.end(); playerIt++)
    {        
        if(playerIt->second.animRequest.cCurAnimRequest != GameTable::ANIM_NO)
        {   // set new player animation if need be.
            m_pWorld->SetCharacterAnimation(playerIt->second.charID, 
                                            playerIt->second.animRequest.cCurAnimRequest, 
                                            playerIt->second.animRequest.bLoopAnim,
                                            playerIt->second.animRequest.fPeriodTime);
			playerIt->second.cLastAnimation = playerIt->second.animRequest.cCurAnimRequest;
			playerIt->second.animRequest.cCurAnimRequest = GameTable::ANIM_NO;
        }

        //set player world matrix                
        m_pWorld->SetCharacterWorldMatrix(playerIt->second.charID, &playerIt->second.mWorld, playerIt->second.bRequestVisible);
    }

    if(m_pEngine->GetMyPlayer())
    {	    
        // update camera position and offset.
        m_pWorld->SetCameraOffset(m_pEngine->GetCameraPosOffset(), m_pEngine->GetCameraLookAtOffset());
        m_pWorld->SetCameraOrientationTarget(m_pEngine->GetMyPlayerCameraTarget());        
    }


    // update projectiles, collidables, and sprite visuals.
    list<SShortLifeEntity> &shortlives = m_pEngine->GetShortLives();
    list<SShortLifeEntity>::iterator it = shortlives.begin(); 
    for( ; it != shortlives.end(); it++)
    {
        if(!it->bVisualCreated) 
        {   // add to animation.
            it->runningAnimId = m_pWorld->AddSpriteAnimation(it->spriteAnimTableID, &it->mWorld, it->fTimeToLive);
            it->bVisualCreated = true;
        }
        else // update animation with new position.
			m_pWorld->UpdateSpriteAnimation(it->runningAnimId, &it->mWorld, it->fTimeToLive);
    }

    // update sky/sun changes according to game engine.
    vector<SSunStateRequest> &vSunRequests = m_pEngine->GetSunRequests();
    for(size_t iSun=0; iSun<vSunRequests.size(); iSun++)
    {
        assert(vSunRequests[iSun].SunIdx >= 0 && vSunRequests[iSun].SunIdx < 2);
        m_pWorld->RequestSunState(vSunRequests[iSun].eSunState, vSunRequests[iSun].SunIdx);
        m_pWorld->RequestLightDirByTimeOfDay(vSunRequests[iSun].fNewLightDirByTimeOfDay);
    }
    vSunRequests.clear();

	// Pass any HUD messages to the GUI
	vector<SHUDMessageRequest> &vMessages = m_pEngine->GetHUDMessageRequests();
    for(size_t iMsg = 0; iMsg < vMessages.size(); iMsg++)
    {
		if (vMessages[iMsg].text.length() > 0) // Actually have something to say
			m_pGUI->PostHUDMessage(vMessages[iMsg].text.c_str(), vMessages[iMsg].bBigFont, vMessages[iMsg].fTimeToLive);
    }
    vMessages.clear();

	if (m_pEngine->GetGameInfo().bShowScoreboardRequest){
		m_pGUI->SetupScoreboard(m_pEngine->GetPlayerHashMap());
		m_pGUI->SetVisible(CGameGUI::SCOREBOARD, true);
		m_pEngine->GetGameInfo().bShowScoreboardRequest = false;
	}
	if (m_pEngine->GetGameInfo().bHideScoreboardRequest){
		m_pGUI->SetVisible(CGameGUI::SCOREBOARD, false);
		m_pEngine->GetGameInfo().bHideScoreboardRequest = false;
	}
}

VOID CGameRoot::PopulateCommandHashMap()
{
	// Sets every item in the hashmap to the default function
	for (UCHAR i = 1; i < GameTable::NUM_COMMANDS; i++)
		m_pGUI->AddCommand(i, &CGameRoot::DefaultFunc);

	// As specific functions are added they are assigned here
	m_pGUI->AddCommand(GameTable::CMD_HELP, &CGameRoot::HelpCmdFunc);
	m_pGUI->AddCommand(GameTable::CMD_CONSOLE, &CGameRoot::ConsoleCmdFunc);
	m_pGUI->AddCommand(GameTable::CMD_MENU, &CGameRoot::MenuCmdFunc);
	m_pGUI->AddCommand(GameTable::CMD_SCOREBOARD, &CGameRoot::ScoreboardCmdFunc);
	m_pGUI->AddCommand(GameTable::CMD_OPTIONS, &CGameRoot::OptionsCmdFunc);
	m_pGUI->AddCommand(GameTable::CMD_SUBMENU, &CGameRoot::SubMenuCmdFunc);
	m_pGUI->AddCommand(GameTable::CMD_QUIT, &CGameRoot::QuitCmdFunc);
	m_pGUI->AddCommand(GameTable::CMD_CAMERA_CONTROL, &CGameRoot::ToggleCameraControl);
	m_pGUI->AddCommand(GameTable::CMD_PHYSICS_RENDER, &CGameRoot::TogglePhysicsRender);
	m_pGUI->AddCommand(GameTable::CMD_BOUNDING_BOX, &CGameRoot::ToggleBBRender);
	m_pGUI->AddCommand(GameTable::CMD_SUNTEST, &CGameRoot::SunTest);
	m_pGUI->AddCommand(GameTable::CMD_SUNSTOP, &CGameRoot::SunStop);
	m_pGUI->AddCommand(GameTable::CMD_SAVE_SHADOW_MAP, &CGameRoot::SaveShadowMap);
	m_pGUI->AddCommand(GameTable::CMD_CYCLE_ANIMATION, &CGameRoot::CyclePlayerAnimation);
	m_pGUI->AddCommand(GameTable::CMD_SET, &CGameRoot::SetCmdFunc);

    // Multiplayer Commands
    m_pGUI->AddCommand(GameTable::CMD_REFRESH_SERVER_LIST, &CGameRoot::RefreshServerBrowser);
    m_pGUI->AddCommand(GameTable::CMD_REFRESH_LAN_LIST, &CGameRoot::RefreshLANBrowser);
	m_pGUI->AddCommand(GameTable::CMD_BROWSER, &CGameRoot::ToggleServerBrowser);
	m_pGUI->AddCommand(GameTable::CMD_CONNECT, &CGameRoot::ConnectCmdFunc);
    m_pGUI->AddCommand(GameTable::CMD_HOST, &CGameRoot::HostCmdFunc);
	m_pGUI->AddCommand(GameTable::CMD_SAY, &CGameRoot::SayCmdFunc);
    m_pGUI->AddCommand(GameTable::CMD_DISCONNECT, &CGameRoot::DisconnectCmdFunc);

}

//------------------
//! CALLBACK FUNCTIONS
//------------------
VOID CGameRoot::DefaultFunc(const string& s){
	string text(" Entered DefaultFunc with param: " + s);
	m_pGUI->PostConsoleMessage(text.c_str());
}

VOID CGameRoot::ConsoleCmdFunc(const string& s){
	m_pGUI->ToggleConsole();
}

VOID CGameRoot::HelpCmdFunc(const string& s){
	string text(" Active commands:");

	for (int i = 0; i < GameTable::NUM_COMMANDS - 1; i++){
		text.append("  ");
		text.append(GameTable::CommandIndexArray[i].str);
		if ((i + 1) % 5 == 0){ //limit to 5 per line
			m_pGUI->PostConsoleMessage(text.c_str());
			text.clear();
			text.push_back('\t');
		}
	}

	m_pGUI->PostConsoleMessage(text.c_str());
}
VOID CGameRoot::ScoreboardCmdFunc(const string& s){
	m_pGUI->SetupScoreboard(m_pEngine->GetPlayerHashMap());
	m_pGUI->ToggleScoreboard();
}
VOID CGameRoot::OptionsCmdFunc(const string& s){
	m_pGUI->SetupOptions(m_pEngine->GetGameSettings());
	m_pGUI->ToggleOptions();
}

VOID CGameRoot::MenuCmdFunc(const string& s){
	string::size_type pos = s.find(' ', 0);
	string param = s.substr(0, pos-1); //get rid of any tailing spaces just in case

	if(param.compare("") == 0) {
		m_pGUI->ToggleMenu();
	}
	else if (param.compare("right") == 0) {
		m_pGUI->SetupMenu(DXUTGetD3DDevice(), CMenu::RIGHT, .25f, 1.0f);
	}
	else if (param.compare("left") == 0) {
		m_pGUI->SetupMenu(DXUTGetD3DDevice(), CMenu::LEFT, .25f, 1.0f);
	}
	else if (param.compare("top") == 0) {
		m_pGUI->SetupMenu(DXUTGetD3DDevice(), CMenu::TOP, 1.0f, .5f);
	}
	else if (param.compare("bottom") == 0) {
		m_pGUI->SetupMenu(DXUTGetD3DDevice(), CMenu::BOTTOM, 1.0f, .5f);
	}
	else if (param.compare("none") == 0) {
		m_pGUI->SetupMenu(DXUTGetD3DDevice(), CMenu::NONE, .5f, .5f);
	}
	else {
		string error(" Invalid parameter: " + param);
		m_pGUI->PostConsoleMessage(error.c_str());
	}
}

VOID CGameRoot::SubMenuCmdFunc(const string& s){
	string::size_type pos = s.find(' ', 0);
	string param = s.substr(0, pos-1); //get rid of any tailing spaces just in case

	int num = atoi(param.c_str());
	if(!m_pGUI->ActivateSubMenu(num)) {
		string error(" Invalid parameter: " + param);
		m_pGUI->PostConsoleMessage(error.c_str());
	}
}
VOID CGameRoot::QuitCmdFunc(const string& s) { 
	PostQuitMessage(EXIT_SUCCESS); 
}

VOID CGameRoot::SetCmdFunc(const string& s){
	std::stringstream ss(s);
	
	string param;// = s.substr(0, pos-1); //get rid of any tailing spaces just in case
	ss >> param;
	UCHAR stg;
	if ((stg = GameTable::FindSettingIndex(param)) != GameTable::NO_SETTING){
		float fValue = gdef::MIN_FLOAT;
		ss >> fValue;
		if (fValue != gdef::MIN_FLOAT){
			m_pEngine->ChangeSetting(stg, fValue);
		}
		else {
			string error(" Invalid numeric parameter");
			m_pGUI->PostConsoleMessage(error.c_str());
		}
	} else {
		string error(" Invalid parameter: " + param);
		m_pGUI->PostConsoleMessage(error.c_str());
	}
}


VOID CGameRoot::RefreshServerBrowser(const string& s)
{
    PRINT_ENTER_FUNCTION_DEBUG
	m_pGUI->PostHUDMessage("Refreshing Server Browser...", false, 2.5f);
    m_pNetworkRoot->RefreshServerList();
}

VOID CGameRoot::RefreshLANBrowser(const string &s)
{
    PRINT_ENTER_FUNCTION_DEBUG;
    m_pNetworkRoot->RefreshLANList();    
}

VOID CGameRoot::ToggleServerBrowser(const string& s)
{
	//ServerInfoSet set; // = m_pNetworkRoot->GetServerList();
	// bogus info i used to make sure the browser printed up right
	//SServerInfo si;
	//si.nPlayers = 2;
	//si.nMaxPlayers = 8;
	//si.nPing = 140;
	//si.nPort = 4321;
	//si.strIP.append("12.34.56.78");
	//si.strName.append("Server");
	//set.insert(si);
	//m_pGUI->SetupServerBrowser(set);
	m_pGUI->ToggleServerBrowser();
}

VOID CGameRoot::ConnectCmdFunc(const string& s)
{
	ServerInfoSet set = m_pNetworkRoot->GetServerList();

	ServerInfoSet::iterator it = set.begin();
	bool done = false;

    // get server info
    std::stringstream ss(s);
    char pszName[100], pszIP[100], pszPort[100];
    ss.getline(pszName, 100, ';');
    ss.getline(pszIP, 100, ':');
    ss.getline(pszPort, 100, ';');
    int nPort = atoi(pszPort);    

	for (; it != set.end() && !done; it++)
    {
        if (it->strName.compare(pszName) == 0 && it->strIP.compare(pszIP) == 0 && nPort == it->nPort)
        {            
            if(m_pEngine->IsNetworkIDAuthority())
            {
                m_pWorld->DeleteAllCharacters();
                SAFE_DELETE(m_pEngine);                
                m_pEngine = new CGameEngine(false);
                m_pEngine->CreatePhysicsWorld(m_pWorld);
            }
		    m_pEngine->ConnectToServer(*it);
			done = true;
            string str("Connected to server: " + s);
            m_pGUI->PostHUDMessage(s.c_str(), true, 5.f);            
            m_pGUI->PostConsoleMessage(s.c_str());
        }
	}
	if(!done){
		string error("Failed to connected to: " + s);
        m_pGUI->PostHUDMessage(error.c_str(), true, 5.f);
		m_pGUI->PostConsoleMessage(error.c_str());
	}
}

VOID CGameRoot::HostCmdFunc(const string& s)
{
    // HACK: Server name, player count, and port fixed for now
    SServerInfo serverInfo;
    serverInfo.strName = "SunBorn SERVAH!";
    serverInfo.nPort = SERVER_PORT;
    serverInfo.nMaxPlayers = 20;

    // Should I Recreate the CGameEngine and pass in the variable for NetworkIDGenerator
    // Or use the abstract CClient CServer layer for all RPCs.

    if(!m_pEngine->IsNetworkIDAuthority())
    {
        m_pWorld->DeleteAllCharacters();
        SAFE_DELETE(m_pEngine);    
        m_pEngine = new CGameEngine(true);
        m_pEngine->CreatePhysicsWorld(m_pWorld);
    }
    m_pEngine->HostServer(serverInfo, m_pNetworkRoot->GetMasterServers());
    m_pGUI->PostHUDMessage("SERVER STARTED", true, 4.f);   
    m_pGUI->PostConsoleMessage("SERVER STARTED");
}

//! Shuts down server or disconnects client.
VOID CGameRoot::DisconnectCmdFunc(const string& s)
{
    m_pEngine->Disconnect();
}
VOID CGameRoot::SayCmdFunc(const string& s){
	// Send this string over the network
	// TODO: Fill me in!
}
// Callback Function Template 
//(make a copy and leave this down here for future commands)

//VOID CGameRoot::CmdFunc(const string& s){
//	string::size_type pos = s.find(' ', 0);
//	string param = s.substr(0, pos-1); //get rid of any tailing spaces just in case
//
//	if(param.compare("parameter") == 0) {
//
//	}
//	else {
//		string error(" Invalid parameter: " + param);
//		m_pGUI->PostConsoleMessage(error.c_str());
//	}
//}
