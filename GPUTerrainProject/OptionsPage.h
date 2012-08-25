#pragma once

#include "dxstdafx.h"
#include "ScrollingWindow.h"
#include "EditableMultiColumnList.h"
#include "GameSettings.h"

class CButtonGroup;

class COptionsPage : public CScrollingWindow
{
private:
	CEditableMultiColumnList*							m_pList; //! List
	CButtonGroup*										m_pButtonGroup;
	CGameSettings*										m_pGameSettings; //! settings, not owned
	bool												m_bControls;	//! if true, controls are being shown, else settings

public:
	COptionsPage(WindowAnchor _anchor, float _xpercent, float _ypercent);
	~COptionsPage();
	void Animate();
	void UpdateDimensions();
	void OnResetDevice();
	void OnLostDevice();
	void OnFrameRender(IDirect3DDevice9 *pd3dDevice, bool bHasFocus);
	bool KeyboardProc(UINT nChar, bool bKeyDown);
	bool MouseProc(bool bLeftButtonDown, int x, int y, bool hadFocus);
	void Toggle();
	vector<string> GetSelectedText();
	void SetupOptionsPage( CGameSettings* gameSettings);
	
	void WriteControls();
	void WriteSettings();

private:
	void ButtonClick(int param);
	void UpdateObjects();
};