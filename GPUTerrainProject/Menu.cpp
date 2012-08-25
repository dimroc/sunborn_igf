#include "dxstdafx.h"
#include "Menu.h"
#include "ButtonGroup.h"
#include "GameTables.h"

#define LOGO_PERCENT 1.f/4.f
CMenu::CMenu(LPDIRECT3DDEVICE9 pd3dDevice, WindowAnchor _anchor, float _xpercent, float _ypercent, CGameGUI* _gui, GUICallback _func) : 
CScrollingWindow(0xaa000000), m_LogoTexture(NULL), m_pGUI(_gui), m_pFunc(_func)
{
	m_nActiveGroup = -1;
	SetupMenu(pd3dDevice, _anchor, _xpercent, _ypercent);
}
CMenu::~CMenu() 
{ 
	CleanUp();
}


bool CMenu::ActivateSubMenu(int index){
	if (0 > index || index >= (int) m_vButtonGroups.size())
		return false;
	if (index == m_nActiveGroup)
		return true;

	int dx = 0, dy = 0;
	DWORD m_Format = 0x0;
	switch(anchor){
		case CMenu::TOP:
			dx = 0;
			dy = -1 * m_nHeight;
			m_Format = DT_RIGHT;
			break;
		case CMenu::RIGHT:
			dx = m_nWidth;
			dy = 0;
			m_Format = DT_LEFT;
			break;
		case CMenu::BOTTOM:
			dx = 0;
			dy = m_nHeight;
			m_Format = DT_CENTER;
			break;
		case CMenu::LEFT:
			dx = -1 * m_nWidth;
			dy = 0;
			m_Format = DT_RIGHT;
			break;
		case CMenu::NONE:
			dx = m_nWidth;
			dy = m_nHeight;
			m_Format = DT_CENTER;
			break;
	}

	int endX = initX + m_nWidth - m_vButtonGroups[index]->GetWidth() - BORDEROFFSET;
	int endY = initY + m_nHeight - m_vButtonGroups[index]->GetHeight() - BORDEROFFSET;
	if (hidden){
		// Set newly activated group's coordinates so it slides in with menu
		CButtonGroup* bg = m_vButtonGroups[index];
		assert(bg);
		bg->SetAlignment(m_Format | DT_VCENTER);
		bg->SetActive(true);
		bg->SetCoordinates(endX + dx, endY + dy);

		// Deactivate currently active group
		if (m_nActiveGroup != -1){
			bg = m_vButtonGroups[m_nActiveGroup];
			assert(bg);
			bg->SetActive(false);
		}

		// Bring menu and newly activated submenu into view
		this->SetEndX(initX);
		this->SetEndY(initY);
		this->SetAnimType(AnimOb::WINDOW);
		hidden = false;
	}
	else {
		// Set newly activated to move onscreen
		CButtonGroup* bg = m_vButtonGroups[index];
		assert(bg);
		bg->SetAlignment(m_Format | DT_VCENTER);
		bg->SetActive(true);
		bg->SetCoordinates(endX + dx, endY + dy);
		bg->SetEndX(endX);
		bg->SetEndY(endY);
		bg->SetAnimating(true);


		// Move currently activated offscreen
		bg = m_vButtonGroups[m_nActiveGroup];
		assert(bg);
		bg->SetActive(false);
		bg->SetEndX(endX);
		bg->SetEndY(endY + m_nHeight);
		bg->SetAnimating(true);

		this->SetAnimType(AnimOb::BUTTON);
	}

	m_nActiveGroup = index;
	return true;
}
void CMenu::AddButton(int mIndex, LPCSTR text, UCHAR cmd, int param){
	while (mIndex >= (int) m_vButtonGroups.size())
		AddNewButtonGroup();

	m_vButtonGroups[mIndex]->AddButton(text, cmd, param);
}
void CMenu::Animate() {
	int oldX = m_nX;
	int oldY = m_nY;
	CScrollingWindow::Animate();
	if (oldX != m_nX || oldY != m_nY){
		// The window is sliding into or out of view
		// Bring the active group with it
		assert(m_vButtonGroups[m_nActiveGroup]);
		int endX = m_vButtonGroups[m_nActiveGroup]->GetX() + (m_nX - oldX);
		int endY = m_vButtonGroups[m_nActiveGroup]->GetY() + (m_nY - oldY);
		m_vButtonGroups[m_nActiveGroup]->SetCoordinates(endX, endY);
	}
	else if (GetAnimType() == AnimOb::BUTTON){
		// The button groups are moving around
		bool cont = false;		
		for (int i = 0; i < (int) m_vButtonGroups.size(); i++){
			if (m_vButtonGroups[i]->IsAnimating()) {
				m_vButtonGroups[i]->Animate();
				cont = true;
			}
		}
		if (!cont)
			SetAnimType(AnimOb::NONE);
	}
}
void CMenu::ButtonClick(UCHAR c, int param){
	string s;
	if (c == GameTable::CMD_MENU){
		switch (param){
			case CMenu::LEFT:
				s.append("left");
				break;
			case CMenu::RIGHT:
				s.append("right");
				break;
			case CMenu::TOP:
				s.append("top");
				break;
			case CMenu::BOTTOM:
				s.append("bottom");
				break;
			case CMenu::NONE:
				s.append("none");
				break;
		}
	} 
	else if (c == GameTable::CMD_SUBMENU){
		std::stringstream ss;
		ss << param;
		s.append(ss.str().c_str());
	}
	(m_pGUI->*m_pFunc)(c, s);
}
bool CMenu::MouseProc(bool bLeftButtonDown, int x, int y, bool hadFocus){
	if (GetAnimType() == AnimOb::WINDOW)
		return false;

	if (hadFocus){
		assert(m_vButtonGroups[m_nActiveGroup]);
		int bIndex = m_vButtonGroups[m_nActiveGroup]->MouseProc(bLeftButtonDown, x, y);
		if (bIndex != -1){
			CButton button = m_vButtonGroups[m_nActiveGroup]->GetButton(bIndex);
			ButtonClick(button.m_cCmdIndex, button.m_nParam);
		}
	}

	if (bLeftButtonDown)
		return CheckInBounds(x, y);
	else
		return hadFocus;

}
void CMenu::CleanUp() {
	for (int i = 0; i < (int) m_vButtonGroups.size(); i++){
		SAFE_DELETE(m_vButtonGroups[i]);
	}

	SAFE_RELEASE(m_LogoTexture);
	m_pGUI = NULL;
}
CButtonGroup* CMenu::AddNewButtonGroup(){
	CButtonGroup* bg = new CButtonGroup(CButtonGroup::VERTICAL);
	m_vButtonGroups.push_back(bg);
	UpdateButtonGroupDimensions();

	return bg;
}
void CMenu::OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, bool bHasFocus) {
	if (hidden && GetAnimType() == AnimOb::NONE)
		return;

	if (!m_Sprite){
		// Create sprite object
		D3DXCreateSprite(pd3dDevice,&m_Sprite);
	}
	if (!m_LogoTexture){
		// Load a texture for our sprite
		D3DXCreateTextureFromFile(pd3dDevice, MENU_LOGO, &m_LogoTexture);
	}
	if (!m_font){
		D3DXCreateFont( pd3dDevice, 18, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			TEXT("Arial"), &m_font );
	}
	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	CScrollingWindow::OnFrameRender(pd3dDevice, bHasFocus, true);

	// Draw image (logo)
	D3DSURFACE_DESC pDesc;
	m_LogoTexture->GetLevelDesc(0, &pDesc);

	D3DXMATRIX mx, mxScale, mxTrans;
	D3DXMatrixIdentity(&mx);
	float maxHeight = (float) m_nHeight * LOGO_PERCENT;
	float xscale = (float)m_nWidth / pDesc.Width;
	if (xscale * pDesc.Height > maxHeight){
		xscale = maxHeight / pDesc.Height;
		float tempm_nWidth = pDesc.Width * xscale;

		D3DXMatrixTranslation(&mxTrans, (float)m_nX + ((m_nWidth - tempm_nWidth) / 2), (float)m_nY, 0);
	}
	else
		D3DXMatrixTranslation(&mxTrans, (float)m_nX, (float)m_nY, 0);
	D3DXMatrixScaling(&mxScale, xscale, xscale, 1.0f);

	mx = mxScale * mxTrans;
	m_Sprite->SetTransform(&mx);

	m_Sprite->Draw(m_LogoTexture, NULL, NULL,NULL,0xFFFFFFFF);
	m_Sprite->End();

	// Button Groups
	for (int i = 0; i < (int) m_vButtonGroups.size(); i++) {
		// Only a max of 2 should ever really need to be drawn during the same frame
		if (m_vButtonGroups[i]->IsActive() || m_vButtonGroups[i]->IsAnimating())
			m_vButtonGroups[i]->OnFrameRender(pd3dDevice, m_font);
	}
}
void CMenu::OnLostDevice(){
	CScrollingWindow::OnLostDevice();
}
void CMenu::OnResetDevice(){
	CScrollingWindow::OnResetDevice();

	if (!m_vButtonGroups.empty())
		UpdateButtonGroupDimensions();
}
void CMenu::SetCallback(GUICallback funcPtr){
	m_pFunc = funcPtr;

}
void CMenu::SetSpeed(int s) { 
	if (s > 0) 
		m_speed = s;
}

void CMenu::SetupMenu(LPDIRECT3DDEVICE9 pd3dDevice, WindowAnchor _anchor, float _xpercent, float _ypercent)
{
	anchor = _anchor;
	xPercent = _xpercent;
	yPercent = _ypercent;
	hidden = true;
	UpdateDimensions();
}

void CMenu::ToggleMenu() {
	CScrollingWindow::Toggle();
}
void CMenu::UpdateDimensions(){
	CScrollingWindow::UpdateDimensions(); // Call the inherited function
	if (m_vButtonGroups.empty())
		return;
	UpdateButtonGroupDimensions();
}

void CMenu::UpdateButtonGroupDimensions(){
	DWORD m_Format = 0x0;
	int newWidth = 0, newHeight = 0;
	switch (anchor){
		case CMenu::TOP:
			m_Format = DT_RIGHT;
			newWidth = m_nWidth / 2;
			newHeight = m_nHeight;
			break;
		case CMenu::RIGHT:
			m_Format = DT_LEFT;
			newWidth = m_nWidth;
			newHeight = (int) (m_nHeight * (1 - LOGO_PERCENT));
			break;
		case CMenu::BOTTOM:
			m_Format = DT_CENTER;
			newWidth = m_nWidth;
			newHeight = (int) (m_nHeight * (1 - LOGO_PERCENT));
			break;
		case CMenu::LEFT:
			m_Format = DT_RIGHT;
			newWidth = m_nWidth;
			newHeight = (int) (m_nHeight * (1 - LOGO_PERCENT));
			break;
		case CMenu::NONE: // default this to the center of the screen
			m_Format = DT_CENTER;
			newWidth = m_nWidth;
			newHeight = (int) (m_nHeight * (1 - LOGO_PERCENT));
			break;
	}
	newWidth -= (BORDEROFFSET * 2);
	newHeight -= (BORDEROFFSET * 2);
	for (int i = 0; i < (int) m_vButtonGroups.size(); i++){
		m_vButtonGroups[i]->SetAlignment(m_Format | DT_VCENTER);
		m_vButtonGroups[i]->SetDimensions(newWidth, newHeight);

		if (m_vButtonGroups[i]->IsActive()){
			m_vButtonGroups[i]->SetCoordinates(m_nX + m_nWidth - newWidth - BORDEROFFSET, m_nY + m_nHeight - newHeight - BORDEROFFSET);
		}
		else {
			m_vButtonGroups[i]->SetCoordinates(m_vButtonGroups[i]->GetX(), initY + m_nHeight);
		}
	}
}