#pragma once

#include "dxstdafx.h"
#include "GUIObject.h"

#define BUTTON_HEIGHT 32
#define BUTTON_WIDTH 128

using std::string;

class CButton : public CGUIObject
{
public:
	bool					m_bDepressed;	// Mouse clicked
	string					m_strText;		// Button Text
	int						m_nParam;		// (Optional) parameter that refers to this Button

	UCHAR					m_cCmdIndex;	// Index of the Command this Button performs
	
public:
	CButton ();
	CButton (int x, int y, int h, int w);
	CButton (LPCSTR strText, UCHAR _cmd, int _param);
	~CButton();
	
	void Animate();

	// Tests if the user clicked on this Button
	bool MouseProc(bool bLeftButtonDown, int x, int y);

	//-------------------
	//! Gets and Sets
	//-------------------
	LPCSTR GetText() { return m_strText.c_str(); }

	void SetText(LPCSTR bText){ m_strText.assign(bText); }
	
};
