#pragma once

class CGameRoot;
class CConsole;
class CUserTextLine;
class CConsoleBuffer;

#include "dxstdafx.h"
#include "ScrollingWindow.h"
#include "ConsoleBuffer.h"
#include "GameGUI.h"

using stdext::hash_map;

class CConsole : public CScrollingWindow
{
private:
	CGameRoot*							m_pGameRoot;
	CConsoleBuffer*						m_pBuffer;
	CUserTextLine*						m_pLine;
	bool								m_bActive; //! Whether the console is currently active
	bool								m_bShift;
	int									m_iFontSize;

	typedef void (CGameGUI::* GUICallback)(UCHAR, string&);
	GUICallback							m_pFunc;		// pointer to command function
	CGameGUI*							m_pGUI;

public:
	CConsole(LPDIRECT3DDEVICE9 pd3dDevice, WindowAnchor _anchor, float _xpercent, float _ypercent, CGameGUI* _m_pGUI, GUICallback _func);
	~CConsole();
	bool IsActive() { return m_bActive; }
	bool KeyboardProc(UINT nChar, bool bKeyDown);
	bool MouseProc(int x, int y, bool hadFocus);
	void OnFrameRender(IDirect3DDevice9 *pd3dDevice, bool bHasFocus);
	void OnLostDevice();
	void OnResetDevice();
	void ParseText(LPCSTR text);
	void PostMessage(LPCSTR text);
	void SetFontSize(int size);
	void SetupConsole(LPDIRECT3DDEVICE9 pd3dDevice, WindowAnchor _anchor, float _xpercent, float _ypercent);
	void ToggleConsole();
	void UpdateDimensions();

};


