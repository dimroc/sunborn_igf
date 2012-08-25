#pragma once

class CGameGUI;

#include "GameRoot.h"

#include "Menu.h"
#include "Console.h"
#include "HUD.h"
#include "ServerBrowser.h"
#include "Scoreboard.h"
#include "OptionsPage.h"

#include "GameEngine.h"
#include "ServerInfo.h"

using stdext::hash_map;
using std::vector;

class CGameGUI {

public:
	enum GUIReg { CONSOLE = 0, MENU, BROWSER, SCOREBOARD, OPTIONS, NUM_REGS };

private:
	CMenu*					m_pMenu;		//! Menu, owned
	CServerBrowser*			m_pServerBrowser;	//! Server browser, owned
	CScoreboard*			m_pScoreboard;	//! Scoreboard, owned
	CConsole*				m_pConsole;		//! Console, owned
	CHud*					m_pHud;			//! HUD, owned
	COptionsPage*			m_pOptions;		//! Options
	CGameRoot*				m_pGameRoot;	//! NOT OWNED BY THIS CLASS, SO NOT CLEANED UP ON DESTRUCTION

	vector<bool>			m_vFocus;		//! Stores which window currently has focus

	typedef void (CGameRoot::* GameCommandPtr)(const string&);
	hash_map<UCHAR, GameCommandPtr>		m_hmCommands;

	VOID ClearFocus();
public:
	CGameGUI(CGameRoot* root);
	~CGameGUI();

	VOID OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice);
	VOID OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice);
	VOID OnLostDevice();

	BOOL KeyboardProc(UINT nChar, bool bKeyDown);  
	void MouseProc(bool bLeftButtonDown, int xPos, int yPos);
	void UpdateMouseLocation(int xPos, int yPos);

	VOID OnFrameMove(double fTime, float fElapsedTime);
	VOID OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, const CGameEngine::PlayerHashMap &phm, const CGameEngine::SRoundInfo &roundInfo);   

	VOID SetVisible(int index, bool bVis);

	VOID AddCommand(UCHAR c, GameCommandPtr p);
	GameCommandPtr GetCommand(UCHAR c);
	VOID ExecuteCommand(UCHAR i, string& param);

	VOID ToggleMenu();
	VOID BuildMenu();
	BOOL ActivateSubMenu(int i);
	VOID SetupMenu(LPDIRECT3DDEVICE9 pd3dDevice, CScrollingWindow::WindowAnchor _anchor, float _xpercent, float _ypercent);

	VOID ToggleConsole();
	VOID PostConsoleMessage(LPCSTR text);

	VOID SetupHUD(const CGameEngine::SPlayer* player);
	VOID ResetHUDAlpha();
	VOID PostHUDMessage(LPCSTR text, bool bigFont, float timeToLive = 1.f);

	VOID SetupServerBrowser(const ServerInfoSet &set);
	VOID ToggleServerBrowser();

	VOID SetupScoreboard(const CGameEngine::PlayerHashMap &phm);
	VOID ToggleScoreboard();

	VOID SetupOptions(CGameSettings* stgs);
	VOID ToggleOptions();

private:
	VOID DrawWindow(LPDIRECT3DDEVICE9 pd3dDevice, int index);
};

