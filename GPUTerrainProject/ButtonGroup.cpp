#include "dxstdafx.h"
#include "ButtonGroup.h"

#define CURR_WIDTH m_nWidth / ((int)m_vButtons.size())

CButtonGroup::CButtonGroup(ButtonLayout layout) {
	m_Align = (DT_LEFT | DT_VCENTER);
	m_Layout = layout;
	m_bActive = false;
	m_bAnimating = false;
}
CButtonGroup::~CButtonGroup() {

}

void CButtonGroup::AddButton(LPCSTR text, UCHAR cmd, int param){
	CButton button(text, cmd, param);
	m_vButtons.push_back(button);

	UpdateButtons();
}
void CButtonGroup::OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXFONT pFont) {
	assert(IsActive() || IsAnimating());
	for (int i = 0; i < (int) m_vButtons.size(); i++){
		DrawButton(pd3dDevice, pFont, m_vButtons[i]);
	}
}

int CButtonGroup::MouseProc(bool bLeftButtonDown, int xPos, int yPos){
	assert(IsActive());
	for (int i = 0; i < (int) m_vButtons.size(); i++) {
		if (m_vButtons[i].MouseProc(bLeftButtonDown, xPos, yPos)){
			// This button was clicked
			return i;
		}
	}
	return -1;
}
void CButtonGroup::Animate(){
	int dx, dy;

	// Update X coordinate
	dx = GetEndX() - m_nX;
	if (dx > m_speed)
		dx = m_speed;
	if (dx < -1 * m_speed)
		dx = -1 * m_speed;
	m_nX += dx;

	// Update Y coordinate
	dy = GetEndY() - m_nY;
	if (dy > m_speed)
		dy = m_speed;
	if (dy < -1 * m_speed)
		dy = -1 * m_speed;
	m_nY += dy;

	ShiftButtonPositions(dx, dy);

	if (GetEndX() == m_nX && GetEndY() == m_nY){
		SetAnimating(false);
	}
}

//*************
// Setters
//*************
void CButtonGroup::SetCoordinates(int x, int y){
	int dx = x - m_nX;
	int dy = y - m_nY;

	if (dx == 0 && dy == 0)
		return;

	m_nX = x;
	m_nY = y;
	ShiftButtonPositions(dx, dy);
}
void CButtonGroup::SetDimensions(int width, int height){
	m_nHeight = height;
	m_nWidth = width;

	UpdateButtons();
}


//*************
// Private stuff
//*************
void CButtonGroup::DrawButton(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXFONT pFont, CButton &button){
	float left = (float) button.GetX(),
		right = left + button.GetWidth(),
		top = (float) button.GetY(),
		bottom = top + button.GetHeight();
	RECT rct;
	rct.top = (long) top;
	rct.bottom = (long) bottom;
	rct.left = (long) left;
	rct.right = (long) right;

	DWORD color = 0x0;
	// Draw background (maybe) and text
	if (button.m_bDepressed){
		color = 0xffffffff;
		SVertex bgverts[] = 
		{
			{ left,		bottom,		0.0f,	1.0f,	color },
			{ left,		top,		0.0f,	1.0f,	color },
			{ right,	top,		0.0f,	1.0f,	color },
			{ right,	bottom,		0.0f,	1.0f,	color },
			{ left,		bottom,		0.0f,	1.0f,	color }
		};
		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, bgverts, sizeof(SVertex));
		pFont->DrawTextA(NULL, button.m_strText.c_str(), -1, &rct, m_Align, D3DCOLOR_ARGB(255,0,0,0) );
	}
	else {
		pFont->DrawTextA(NULL, button.m_strText.c_str(), -1, &rct, m_Align, D3DCOLOR_ARGB(255,255,255,255) );
	}
}
void CButtonGroup::ShiftButtonPositions(int dx, int dy){
	for (int i = 0; i < (int) m_vButtons.size(); i++){
		m_vButtons[i].SetX(m_vButtons[i].GetX() + dx);
		m_vButtons[i].SetY(m_vButtons[i].GetY() + dy);
	}
}
void CButtonGroup::UpdateButtons(){
	for (int i = 0; i < (int) m_vButtons.size(); i++){
		int x = m_nX, y = m_nY;
		if (m_Layout == VERTICAL){
			y += m_nHeight - (17 + BUTTON_HEIGHT * ((int) m_vButtons.size() - i));
			m_vButtons[i].SetWidth(m_nWidth);
		} 
		else if (m_Layout == HORIZONTAL) {
			x += CURR_WIDTH * i;
			m_vButtons[i].SetWidth(CURR_WIDTH);
		}
		m_vButtons[i].SetX(x);
		m_vButtons[i].SetY(y);
	}

}