#pragma once

#include "dxstdafx.h"
#include "ScrollingWindow.h"
#include "EditableMultiColumnList.h"
#include "GameEngine.h"


class CScoreboard : public CScrollingWindow
{
private:
	typedef CGameEngine::SPlayer SPlayer;
	typedef CGameEngine::PlayerHashMap PlayerHashMap;

	CEditableMultiColumnList*							m_pList; //! List


public:
	CScoreboard(WindowAnchor _anchor, float _xpercent, float _ypercent);
	~CScoreboard();
	void Animate();
	void UpdateDimensions();
	void OnResetDevice();
	void OnLostDevice();
	void OnFrameRender(IDirect3DDevice9 *pd3dDevice, bool bHasFocus);
	bool KeyboardProc(UINT nChar, bool bKeyDown);
	bool MouseProc(int x, int y, bool hadFocus);
	void Toggle();
	vector<string> GetSelectedText();
	void SetupScoreboard(const PlayerHashMap &hmPlayers);
private:
	void UpdateObjects();
};