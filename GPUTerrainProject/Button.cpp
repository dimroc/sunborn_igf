#include "dxstdafx.h"
#include "Button.h"


CButton::CButton () : CGUIObject(0,0,BUTTON_HEIGHT,BUTTON_WIDTH) { 
	m_cCmdIndex = 0;
	m_nParam = 0;
	m_bDepressed = false;
}
CButton::CButton (LPCSTR strText, UCHAR _cmd, int _param): CGUIObject(0,0,BUTTON_HEIGHT,BUTTON_WIDTH){
	m_strText.assign(strText);
	m_cCmdIndex = _cmd;
	m_nParam = _param;
	m_bDepressed = false;
}
CButton::~CButton() { }

bool CButton::MouseProc(bool bLeftButtonDown, int x, int y){
	if (CheckInBounds(x, y)){
		if (bLeftButtonDown){ // User clicked the mouse button down on this button
			m_bDepressed = true;
			return false;
		}
		else {
			// Release the mouse button on this button while it was down, click
			bool click = m_bDepressed;
			m_bDepressed = false;
			return click;
		}
	}
	else {
		// User's doin some clickin somewhere we don't care
		m_bDepressed = false;	
		return false;
	}
}