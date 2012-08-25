#pragma once

class CScrollingWindow;

#include "AnimOb.h"
#include "dxstdafx.h"

#define BORDEROFFSET 15

class CScrollingWindow : public AnimOb
{
public:
	enum WindowAnchor{ NONE, TOP, RIGHT, BOTTOM, LEFT };
	int						initX, initY;	// Initial screen X,Y coordinates of window's top left
	bool					hidden;			
	DWORD					bgColor;		// Background color
	WindowAnchor			anchor;			// Where is the window docked
	float					xPercent;		// What percentage of the window is taken up by the window (between 0 and 1)
	float					yPercent;

	LPD3DXFONT				m_font;
	LPD3DXSPRITE			m_Sprite;		// our sprite interface
	LPDIRECT3DTEXTURE9		m_Texture;		// our texture


	CScrollingWindow(DWORD _bgColor);
	~CScrollingWindow();

	//Animates the window, Should be called in the OnFrameMove function
	void Animate();
	void OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, bool bHasFocus, bool bSpriteStarted = false);
	void OnLostDevice();
	void OnResetDevice();

	void Toggle();
	void UpdateDimensions();
};