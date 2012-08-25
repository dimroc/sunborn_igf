#pragma once

#include "dxstdafx.h"
#include "ScrollingWindow.h"
#include "ServerInfo.h"

class CEditableMultiColumnList;

using std::vector;

class CServerBrowser : public CScrollingWindow
{
private:
	CEditableMultiColumnList*					m_pList;

public:
	CServerBrowser(WindowAnchor _anchor, float _xpercent, float _ypercent);
	~CServerBrowser();
	void Animate();
	void UpdateDimensions();
	void OnResetDevice();
	void OnLostDevice();
	void OnFrameRender(IDirect3DDevice9 *pd3dDevice, bool bHasFocus);
	bool KeyboardProc(UINT nChar, bool bKeyDown);
	bool MouseProc(int x, int y, bool hadFocus);
	void Toggle();
	vector<string> GetSelectedText();
	void SetupBrowser(const ServerInfoSet &set);
private:
	void UpdateObjects();
};