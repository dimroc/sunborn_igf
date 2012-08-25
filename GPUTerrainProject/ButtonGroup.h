#pragma once

#include "dxstdafx.h"
#include "ScrollingWindow.h"
#include "Button.h"

using std::vector;

class CButtonGroup : public AnimOb {
public:
	/*
	* VERTICAL - the button group will try to fill its area going up y and then across x
	* HORIZONTAL - the button group will try to fill its area going across x first then down y
	*/
	enum ButtonLayout{ VERTICAL, HORIZONTAL };
private:
	vector<CButton>			m_vButtons;		// Button vector
	DWORD					m_Align;		// Alignment for button text
	ButtonLayout			m_Layout;
	bool					m_bActive;
	bool					m_bAnimating;

public:
	CButtonGroup(ButtonLayout layout);
	~CButtonGroup();
	void AddButton(LPCSTR text, UCHAR cmd, int param = -1);
	void OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXFONT pFont);
	// Returns the index of the button in this group that was clicked, -1 if none
	int MouseProc(bool bLeftButtonDown, int xPos, int yPos);
	void Animate();


	//*************
	// Getters
	//*************
	bool IsActive() { return m_bActive; }
	bool IsAnimating() { return m_bAnimating; }
	CButton &GetButton(int index) { return m_vButtons[index]; }

	//*************
	// Setters
	//*************

	void SetActive(bool value) { m_bActive = value; }
	void SetAlignment(DWORD align) { m_Align = align; }
	// Sets the x,y coordinates of this button group and updates each of the button's positions
	void SetCoordinates(int x, int y);
	void SetAnimating(bool value) { m_bAnimating = value; }
	// Sets the height and width and updates the buttons if necessary
	void SetDimensions(int width = -1, int height = -1);
private:
	void DrawButton(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXFONT pFont, CButton &button);
	void ShiftButtonPositions(int dx, int dy);
	void UpdateButtons();
};
