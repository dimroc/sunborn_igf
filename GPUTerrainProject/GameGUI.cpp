#include "dxstdafx.h"
#include "GameGUI.h"

#include "GUIDefine.h"
#include "GameTables.h"

CGameGUI::CGameGUI(CGameRoot* root){
	m_pGameRoot = root;
	m_pMenu = NULL;
	m_pConsole = NULL;
	m_pHud = NULL;
	m_pServerBrowser = NULL;
	m_pScoreboard = NULL;
	m_pOptions = NULL;
}
CGameGUI::~CGameGUI(){ 
	SAFE_DELETE(m_pConsole);
	SAFE_DELETE(m_pMenu);
	SAFE_DELETE(m_pHud);
	SAFE_DELETE(m_pServerBrowser);
	SAFE_DELETE(m_pScoreboard);
	SAFE_DELETE(m_pOptions);
	m_pGameRoot = NULL;
}
VOID CGameGUI::ClearFocus(){
	for (int i = 0; i < NUM_REGS; i++)
		m_vFocus[i] = false;
}
VOID CGameGUI::OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice){ 
	m_pMenu = new CMenu(pd3dDevice, CMenu::RIGHT, .25f, 1.0f, this, &CGameGUI::ExecuteCommand);
	BuildMenu();
	m_pConsole = new CConsole(pd3dDevice, CScrollingWindow::TOP, .6f, .4f, this, &CGameGUI::ExecuteCommand);
	m_pHud = new CHud(pd3dDevice);
	m_pServerBrowser = new CServerBrowser(CScrollingWindow::NONE, .7f, .4f);
	m_pScoreboard = new CScoreboard(CScrollingWindow::BOTTOM, .55f, .5f);
	m_pOptions = new COptionsPage(CScrollingWindow::NONE, .5f, .55f);

	for (int i = 0; i < NUM_REGS; i++)
		m_vFocus.push_back(false);

	// default to menu has focus
	m_vFocus[MENU] = true;
}
VOID CGameGUI::OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice){
	m_pMenu->OnResetDevice();
	m_pConsole->OnResetDevice();
	m_pHud->OnResetDevice();
	m_pServerBrowser->OnResetDevice();
	m_pScoreboard->OnResetDevice();
	m_pOptions->OnResetDevice();

}
VOID CGameGUI::OnLostDevice(){
	m_pMenu->OnLostDevice();
	m_pConsole->OnLostDevice();
	m_pHud->OnLostDevice();
	m_pServerBrowser->OnLostDevice();
	m_pScoreboard->OnLostDevice();
	m_pOptions->OnLostDevice();
}


BOOL CGameGUI::KeyboardProc( UINT nChar, bool bKeyDown){
	if (bKeyDown && nChar == KEY_TILDE){
		ToggleConsole();
		if (!m_pConsole->IsActive())
			m_vFocus[CONSOLE] = false;
	}

	bool result = false;
	if (m_vFocus[CONSOLE])
		result = result || m_pConsole->KeyboardProc(nChar, bKeyDown);
	if (m_vFocus[BROWSER])
		result = result || m_pServerBrowser->KeyboardProc(nChar, bKeyDown);
	if (m_vFocus[SCOREBOARD])
		result = result || m_pScoreboard->KeyboardProc(nChar, bKeyDown);
	if (m_vFocus[OPTIONS])
		result = result || m_pOptions->KeyboardProc(nChar, bKeyDown);

	return result;
}  
VOID CGameGUI::MouseProc(bool bLeftButtonDown, int xPos, int yPos){
	int oldfocused = -1;
	for (int i = 0; i < NUM_REGS; i++){
		if (m_vFocus[i])
			oldfocused = i;
	}

	if (bLeftButtonDown){
		m_vFocus[CONSOLE] = m_pConsole->MouseProc(xPos, yPos, m_vFocus[CONSOLE]);
		m_vFocus[BROWSER] = m_pServerBrowser->MouseProc(xPos, yPos, m_vFocus[BROWSER]);
		m_vFocus[SCOREBOARD] = m_pScoreboard->MouseProc(xPos, yPos, m_vFocus[SCOREBOARD]);
	}
	m_vFocus[OPTIONS] = m_pOptions->MouseProc(bLeftButtonDown,xPos, yPos, m_vFocus[OPTIONS]);
	m_vFocus[MENU] = m_pMenu->MouseProc(bLeftButtonDown, xPos, yPos, m_vFocus[MENU]);

	int count = 0;
	for (int i = 0; i < NUM_REGS; i++){
		if (m_vFocus[i])
			count++;
	}

	if (count < 2)
		return; // amount of focused windows is ok

	// more than 1 is focused
	if (oldfocused != -1 && m_vFocus[oldfocused]){
		// previously focused window still has focus so all other windows lose focus
		ClearFocus();
		m_vFocus[oldfocused] = true;
	} else {
		// previously focused window lost focus or no window had focus
		// just take the first 
		bool done = false;
		for (int i = 0; i < NUM_REGS; i++){
			if (!done)
				done = m_vFocus[i];
			else
				m_vFocus[i] = false;
		}
	}
}

VOID CGameGUI::UpdateMouseLocation(int xPos, int yPos){
	if (xPos + MENU_TRIGGER_WIDTH >= (int) DXUTGetBackBufferSurfaceDesc()->Width)
		SetVisible(MENU, true);

	int index = -1;
	if (m_pConsole->CheckInBounds(xPos, yPos))
		if (m_vFocus[CONSOLE])
			return;
		else
			index = CONSOLE;
	if (m_pServerBrowser->CheckInBounds(xPos, yPos))
		if (m_vFocus[BROWSER])
			return;
		else
			index = BROWSER;
	if (m_pScoreboard->CheckInBounds(xPos, yPos))
		if (m_vFocus[SCOREBOARD])
			return;
		else
			index = SCOREBOARD;
	if (m_pOptions->CheckInBounds(xPos, yPos))
		if (m_vFocus[OPTIONS])
			return;
		else
			index = OPTIONS;
	if (m_pMenu->CheckInBounds(xPos, yPos))
		if (m_vFocus[MENU])
			return;
		else
			index = MENU;

	if (index != -1){
		ClearFocus();	
		m_vFocus[index] = true;
	}
}
VOID CGameGUI::OnFrameMove(double fTime, float fElapsedTime){
	m_pMenu->Animate();
	m_pConsole->Animate();
	m_pServerBrowser->Animate();
	m_pScoreboard->Animate();
	m_pOptions->Animate();
	m_pHud->OnFrameMove(fElapsedTime);
}
VOID CGameGUI::OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, const CGameEngine::PlayerHashMap &phm, const CGameEngine::SRoundInfo &roundInfo){
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pd3dDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );

	// Draw HUD
	m_pHud->OnFrameRender(pd3dDevice, phm, roundInfo);

	// Draw registered windows
	int focused = -1;
	for (int i = 0; i < NUM_REGS; i++)
		if (!m_vFocus[i])
			DrawWindow(pd3dDevice, i);
		else
			focused = i;
	if (focused != -1) // Window with focus gets drawn last
		DrawWindow(pd3dDevice, focused);
}  
VOID CGameGUI::SetVisible(int index, bool bVis){
	switch(index){
		case MENU: 
			if (m_pMenu->hidden == bVis)
				ToggleMenu();
			break;
		case CONSOLE:
			if (m_pConsole->hidden == bVis)
				ToggleConsole();
			break;
		case BROWSER:
			if (m_pServerBrowser->hidden == bVis)
				ToggleServerBrowser();
			break;
		case SCOREBOARD:
			if (m_pScoreboard->hidden == bVis)
				ToggleScoreboard();
			break;
		case OPTIONS:
			if (m_pOptions->hidden == bVis)
				ToggleOptions();
			break;
	}
}
VOID CGameGUI::AddCommand(UCHAR c, GameCommandPtr p){
	m_hmCommands[c] = p;
}
CGameGUI::GameCommandPtr CGameGUI::GetCommand(UCHAR c) {
	return m_hmCommands[c];
}
VOID CGameGUI::ExecuteCommand(UCHAR c, string& param){
	if (GetCommand(c) == NULL)
		return;
	if (c == GameTable::CMD_CONNECT){
		std::vector<string> row = m_pServerBrowser->GetSelectedText();
		for (int i = 0; i < (int) row.size(); i++){
			param.append(row[i]);
			if (i < (int) row.size() - 1)
				param.append(";");
		}
	}

	(m_pGameRoot->*m_hmCommands[c])(param);
}

VOID CGameGUI::BuildMenu()
{
	using namespace GameTable;
	m_pMenu->SetSpeed(20);

#if defined(DEBUG) || defined(_DEBUG)
	m_pMenu->AddButton(0, "Hack Menu", CMD_SUBMENU, 4);
#endif
	// m_pMenu->AddButton(0, "Single Player Sandbox", NO_COMMAND);   // its automatic, why bother say it.s
	m_pMenu->AddButton(0, "Multiplayer", CMD_SUBMENU, 3);
	m_pMenu->AddButton(0, "Character Select", CMD_SUBMENU, 2);
	m_pMenu->AddButton(0, "Options", CMD_OPTIONS);
	m_pMenu->AddButton(0, "Toggle Scoreboard", CMD_SCOREBOARD);
	m_pMenu->AddButton(0, "Hide Menu", CMD_MENU);
	m_pMenu->AddButton(0, "Quit", CMD_QUIT);

#if defined(DEBUG) || defined(_DEBUG)
	m_pMenu->AddButton(1, "Set Menu Right", CMD_MENU, CMenu::RIGHT);
	m_pMenu->AddButton(1, "Set Menu Left", CMD_MENU, CMenu::LEFT);
	m_pMenu->AddButton(1, "Set Menu Top", CMD_MENU, CMenu::TOP);
	m_pMenu->AddButton(1, "Set Menu Bottom", CMD_MENU, CMenu::BOTTOM);
	m_pMenu->AddButton(1, "Set Menu None", CMD_MENU, CMenu::NONE);
#endif
	m_pMenu->AddButton(1, "Back", CMD_SUBMENU, 0);

#if defined(DEBUG) || defined(_DEBUG)
	m_pMenu->AddButton(2, "Menuitem1", NO_COMMAND);
	m_pMenu->AddButton(2, "WTF", NO_COMMAND);
	m_pMenu->AddButton(2, "Alt C too control camera", NO_COMMAND);
#endif
	m_pMenu->AddButton(2, "Back", CMD_SUBMENU, 0);

	// multiplayer menu
	m_pMenu->AddButton(3, "Toggle Server Browser", CMD_BROWSER);
	m_pMenu->AddButton(3, "Refresh Server List", CMD_REFRESH_SERVER_LIST);
	m_pMenu->AddButton(3, "Refresh LAN Server List", CMD_REFRESH_LAN_LIST);
	m_pMenu->AddButton(3, "Connect to Server", CMD_CONNECT);	
	m_pMenu->AddButton(3, "Host Server", CMD_HOST);
	m_pMenu->AddButton(3, "Disconnect", CMD_DISCONNECT);
	m_pMenu->AddButton(3, "Back", CMD_SUBMENU, 0);

#if defined(DEBUG) || defined(_DEBUG)
	// Hack dev menu
	// m_pMenu->AddButton(4, "Toggle Camera Control", CMD_CAMERA_CONTROL);
	m_pMenu->AddButton(4, "Toggle Physics Render", CMD_PHYSICS_RENDER);
	m_pMenu->AddButton(4, "Toggle Bounding Box", CMD_BOUNDING_BOX);
	m_pMenu->AddButton(4, "Toggle Sun Movement", CMD_SUNSTOP);
	m_pMenu->AddButton(4, "Run Sun Test", CMD_SUNTEST);
	m_pMenu->AddButton(4, "Select Next Animation", CMD_CYCLE_ANIMATION);
	m_pMenu->AddButton(4, "Save Shadow Map", CMD_SAVE_SHADOW_MAP);
	m_pMenu->AddButton(4, "Back", CMD_SUBMENU, 0);
#endif

	ActivateSubMenu(0);
}

VOID CGameGUI::ToggleMenu(){ 
	m_pMenu->ToggleMenu();
	if (!m_pMenu->hidden){
		ClearFocus();
		m_vFocus[MENU] = true;
	}
	else m_vFocus[MENU] = false;
}
BOOL CGameGUI::ActivateSubMenu(int i){
	return m_pMenu->ActivateSubMenu(i);
}	
VOID CGameGUI::SetupMenu(LPDIRECT3DDEVICE9 pd3dDevice, CScrollingWindow::WindowAnchor _anchor, float _xpercent, float _ypercent){
	m_pMenu->SetupMenu(pd3dDevice, _anchor, _xpercent, _ypercent);
	ToggleMenu();
}
VOID CGameGUI::ToggleConsole() { 
	m_pConsole->ToggleConsole(); 
	if (!m_pConsole->hidden) {
		ClearFocus();
		m_vFocus[CONSOLE] = true;
	}
	else m_vFocus[CONSOLE] = false;
}
VOID CGameGUI::PostConsoleMessage(LPCSTR text){
	m_pConsole->PostMessage(text);
}

VOID CGameGUI::SetupHUD(const CGameEngine::SPlayer* player){
	m_pHud->SetupHUD(DXUTGetD3DDevice(), player);
}
VOID CGameGUI::ResetHUDAlpha(){
	m_pHud->ResetAlpha();
}
VOID CGameGUI::PostHUDMessage(LPCSTR text, bool bigFont, float timeToLive){
	CHud::SHudMessage message;
	message.strText.append(text);
	message.fTimeLeft = message.fTimeToLive = timeToLive;
	message.bBigFont = bigFont;

	m_pHud->AddHUDMessage(message);

}
VOID CGameGUI::SetupServerBrowser(const ServerInfoSet &set){
	m_pServerBrowser->SetupBrowser(set);
}
VOID CGameGUI::ToggleServerBrowser(){
	m_pServerBrowser->Toggle();
	if (!m_pServerBrowser->hidden){
		ClearFocus();
		m_vFocus[BROWSER] = true;
	}
	else m_vFocus[BROWSER] = false;
}

VOID CGameGUI::SetupScoreboard(const CGameEngine::PlayerHashMap &phm){
	m_pScoreboard->SetupScoreboard(phm);
}
VOID CGameGUI::ToggleScoreboard(){
	m_pScoreboard->Toggle();
	if (!m_pScoreboard->hidden){
		ClearFocus();
		m_vFocus[SCOREBOARD] = true;
	}
	else m_vFocus[SCOREBOARD] = false;
}

VOID CGameGUI::SetupOptions(CGameSettings* stgs){
	m_pOptions->SetupOptionsPage(stgs);
}
VOID CGameGUI::ToggleOptions(){
	m_pOptions->Toggle();
	if (!m_pOptions->hidden){
		ClearFocus();	
		m_vFocus[OPTIONS] = true;
	}
	else m_vFocus[OPTIONS] = false;
}


VOID CGameGUI::DrawWindow(LPDIRECT3DDEVICE9 pd3dDevice, int index){
	bool hidden = false;
	switch(index){
		case CONSOLE:
			m_pConsole->OnFrameRender(pd3dDevice, m_vFocus[CONSOLE]);
			hidden = m_pConsole->hidden;
			break;
		case MENU:
			m_pMenu->OnFrameRender(pd3dDevice, m_vFocus[MENU]);
			hidden = m_pMenu->hidden;
			break;
		case OPTIONS:
			m_pOptions->OnFrameRender(pd3dDevice, m_vFocus[OPTIONS]);
			hidden = m_pOptions->hidden;
			break;
		case BROWSER:
			m_pServerBrowser->OnFrameRender(pd3dDevice, m_vFocus[BROWSER]);
			hidden = m_pServerBrowser->hidden;
			break;
		case SCOREBOARD:
			m_pScoreboard->OnFrameRender(pd3dDevice, m_vFocus[SCOREBOARD]);
			hidden = m_pScoreboard->hidden;
			break;
		default:
			dprintf("[%s] Invalid argument", __FUNCTION__);
			return;
	}
	if (hidden && m_vFocus[index])
		m_vFocus[index] = false;
}
