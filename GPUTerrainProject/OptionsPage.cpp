#include "dxstdafx.h"
#include "OptionsPage.h"
#include "GameTables.h"
#include "ButtonGroup.h"
#include "GUIDefine.h"

using std::string;

COptionsPage::COptionsPage(WindowAnchor _anchor, float _xpercent, float _ypercent) : CScrollingWindow(0xaa000000) {
	m_pList = new CEditableMultiColumnList(0);
	m_pButtonGroup = new CButtonGroup(CButtonGroup::HORIZONTAL);
	m_pButtonGroup->AddButton("Show Settings", 0, 0);
	m_pButtonGroup->AddButton("Apply", 0, 1);
	m_pButtonGroup->AddButton("Revert", 0, 2);
	m_pButtonGroup->AddButton("Hide", 0, 3);

	m_pButtonGroup->SetActive(true);
	m_pButtonGroup->SetAlignment(DT_CENTER | DT_VCENTER);

	anchor = _anchor;
	xPercent = _xpercent;
	yPercent = _ypercent;
	hidden = true;
	m_pGameSettings = NULL;
	m_bControls = true;

	UpdateDimensions();
}
COptionsPage::~COptionsPage(){
	SAFE_DELETE(m_pList);
	SAFE_DELETE(m_pButtonGroup);
	m_pGameSettings = NULL;
}

void COptionsPage::Animate() {
	CScrollingWindow::Animate();

	m_pList->SetX( m_nX + BORDEROFFSET );
	m_pList->SetY( m_nY + BORDEROFFSET );

	m_pButtonGroup->SetCoordinates(m_nX + BORDEROFFSET,
		m_nY + m_nHeight - BORDEROFFSET - m_pButtonGroup->GetHeight());
}

void COptionsPage::UpdateDimensions(){
	CScrollingWindow::UpdateDimensions();
	UpdateObjects();
}
void COptionsPage::OnResetDevice(){
	CScrollingWindow::OnResetDevice();
	
	UpdateObjects();
}
void COptionsPage::OnLostDevice(){
	CScrollingWindow::OnLostDevice();
}
void COptionsPage::OnFrameRender(IDirect3DDevice9 *pd3dDevice, bool bHasFocus){
	if (hidden && GetAnimType() == AnimOb::NONE)
		return;
	if (!m_font){
		D3DXCreateFont( pd3dDevice, 18, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			TEXT("Arial"), &m_font );
	}
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pd3dDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );
	CScrollingWindow::OnFrameRender(pd3dDevice, bHasFocus); // Call the superclass function

	m_pList->OnFrameRender(pd3dDevice, m_font);
	m_pButtonGroup->OnFrameRender(pd3dDevice, m_font);

}
bool COptionsPage::KeyboardProc(UINT nChar, bool bKeyDown){ 
	return m_pList->KeyboardProc(nChar, bKeyDown); 
}
bool COptionsPage::MouseProc(bool bLeftButtonDown, int x, int y, bool hadFocus){
	if (hidden)
		return false;

	if (hadFocus){
		if (bLeftButtonDown)
			m_pList->MouseProc(x, y);
		int bIndex = m_pButtonGroup->MouseProc(bLeftButtonDown, x, y);
		if (bIndex != -1)
			ButtonClick(m_pButtonGroup->GetButton(bIndex).m_nParam);
	}

	if (bLeftButtonDown)
		return CheckInBounds(x, y);
	else
		return hadFocus;
}
void COptionsPage::Toggle() {
	CScrollingWindow::Toggle();
}


vector<string> COptionsPage::GetSelectedText() {
	return m_pList->GetSelectedText();
}

void COptionsPage::SetupOptionsPage(CGameSettings* gameSettings) {
	m_pGameSettings = gameSettings;

	m_pList->ClearList(true);
	if (m_bControls){
		// Add titles
		m_pList->AddColumn("Control", false, false, CEditableMultiColumnList::SColumn::STRING);
		m_pList->AddColumn("Key", true, true, CEditableMultiColumnList::SColumn::KEY);

		// Add entries
		for (int i = 0; i < GameTable::NUM_ACTIONS - 1; i++){
			vector<string> line;

			// Control
			line.push_back(string(GameTable::ActionIndexArray[i].str));

			// Key
			std::stringstream ss;
			CGameSettings::SKeyCombo kc = m_pGameSettings->GetControlMappedKeys(GameTable::ActionIndexArray[i].cmd);
			if (kc.bCtrl)
				ss << "ctrl ";
			if (kc.bAlt)
				ss << "alt ";
			if (kc.bShift)
				ss << "shift ";
			string text = GameTable::FindKeyWordMatch(kc.cKey);
			if (text.empty())
				ss << (char) guidef::GetKeyboardCharacter((UINT) kc.cKey, false);	
			else
				ss << text.c_str();
			line.push_back(ss.str());

			m_pList->AddRow(line);
		}
	}
	else {
		// Add titles
		m_pList->AddColumn("Setting", false, false, CEditableMultiColumnList::SColumn::STRING);
		m_pList->AddColumn("Value", true, false, CEditableMultiColumnList::SColumn::NUMERIC);

		// Add entries
		for (int i = 0; i < GameTable::NUM_SETTINGS - 1; i++){
			vector<string> line;

			// Setting
			line.push_back(string(GameTable::SettingIndexArray[i].str));

			// Value
			std::stringstream ss;
			ss << m_pGameSettings->GetSetting(GameTable::SettingIndexArray[i].cmd);
			line.push_back(ss.str());

			m_pList->AddRow(line);
		}
	}
	m_pList->MoveToTop();
}

void COptionsPage::WriteControls(){
	using namespace std;
	string filename("..\\Media\\");
	filename.append(CURRENT_CONTROLS_FILE);

	ofstream out (filename.c_str());

	if (!out.is_open()){
		dprintf("[COptionsPage::WriteControls] Error opening this file: %s\n", filename.c_str());
		return;
	}

	for (int i = 0; i < m_pList->GetRowCount(); i++){
		vector<string> row = m_pList->GetRowText(i);
		assert(row.size() == 2);
		out << row[0];
		out << " = ";
		out << row[1];
		out << "\n";
	}

	out.close();

	// Reload the new controls
	m_pGameSettings->ClearControls();
	m_pGameSettings->ReadControls(CURRENT_CONTROLS_FILE);
}
void COptionsPage::WriteSettings(){
	using namespace std;
	string filename("..\\Media\\");
	filename.append(CURRENT_SETTINGS_FILE);

	ofstream out (filename.c_str());

	if (!out.is_open()){
		dprintf("[COptionsPage::WriteSettings] Error opening this file: %s\n", filename.c_str());
		return;
	}

	for (int i = 0; i < m_pList->GetRowCount(); i++){
		vector<string> row = m_pList->GetRowText(i);
		assert(row.size() == 2);
		out << row[0];
		out << " = ";
		out << row[1];
		out << "\n";
	}

	out.close();

	// Reload the new controls
	m_pGameSettings->ClearSettings();
	m_pGameSettings->ReadSettings(CURRENT_SETTINGS_FILE);
}
void COptionsPage::ButtonClick(int param){
	switch(param){
	case 0:
		m_bControls = !m_bControls;
		if (m_bControls)
			m_pButtonGroup->GetButton(0).SetText("Show Settings");
		else
			m_pButtonGroup->GetButton(0).SetText("Show Controls");
		SetupOptionsPage(m_pGameSettings);
		break;
	case 1:
		if (m_bControls)
			WriteControls();
		else
			WriteSettings();
		break;
	case 2:
		SetupOptionsPage(m_pGameSettings);
		break;
	case 3:
		Toggle();
		break;
	default:
		dprintf("[COptionsPage::ButtonClick] Invalid parameter\n");
	}
}
void COptionsPage::UpdateObjects(){
	int objWidth = m_nWidth - (2 * BORDEROFFSET);
	int bgHeight = BUTTON_HEIGHT;
	int listHeight = m_nHeight - (BORDEROFFSET * 3) - bgHeight;

	// dimensions of button group
	m_pButtonGroup->SetDimensions(objWidth, bgHeight);
	
	// The max amount of lines in this font size that can fit in our alloted space
	m_pList->SetMaxLines(listHeight / 20 - 1);
	m_pList->SetWidth( objWidth );
	m_pList->SetHeight( listHeight );
}