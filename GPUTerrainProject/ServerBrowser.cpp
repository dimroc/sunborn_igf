#include "dxstdafx.h"
#include "ServerBrowser.h"
#include "EditableMultiColumnList.h"

using std::string;

CServerBrowser::CServerBrowser(WindowAnchor _anchor, float _xpercent, float _ypercent) : CScrollingWindow(0xaa000000) {
	m_pList = new CEditableMultiColumnList(0);
	anchor = _anchor;
	xPercent = _xpercent;
	yPercent = _ypercent;
	hidden = true;

	UpdateDimensions();

	// Add titles
	m_pList->AddColumn("Name", false, false, CEditableMultiColumnList::SColumn::STRING);
	m_pList->AddColumn("IP", false, false, CEditableMultiColumnList::SColumn::STRING, .30f);
	m_pList->AddColumn("Players", false, false, CEditableMultiColumnList::SColumn::STRING, .2f);
	m_pList->AddColumn("Ping (ms)", false, false, CEditableMultiColumnList::SColumn::STRING, .15f);

}
CServerBrowser::~CServerBrowser(){
	SAFE_DELETE(m_pList);
}

void CServerBrowser::Animate() {
	CScrollingWindow::Animate();

	m_pList->SetX( m_nX + BORDEROFFSET );
	m_pList->SetY( m_nY + BORDEROFFSET );
}

void CServerBrowser::UpdateDimensions(){
	CScrollingWindow::UpdateDimensions();

	UpdateObjects();
}
void CServerBrowser::OnResetDevice(){
	CScrollingWindow::OnResetDevice();

	UpdateObjects();
}
void CServerBrowser::OnLostDevice(){
	CScrollingWindow::OnLostDevice();
}
void CServerBrowser::OnFrameRender(IDirect3DDevice9 *pd3dDevice, bool bHasFocus){
	if (hidden && GetAnimType() == AnimOb::NONE)
		return;

	CScrollingWindow::OnFrameRender(pd3dDevice, bHasFocus); // Call the superclass function
	if (!m_font){
		D3DXCreateFont( pd3dDevice, 18, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			TEXT("Arial"), &m_font );
	}

	m_pList->OnFrameRender(pd3dDevice, m_font);

}
bool CServerBrowser::KeyboardProc(UINT nChar, bool bKeyDown){ 
	return m_pList->KeyboardProc(nChar, bKeyDown); 
}
bool CServerBrowser::MouseProc(int x, int y, bool hadFocus){
	if (hadFocus)
		m_pList->MouseProc(x, y);

	return CheckInBounds(x, y);
}
void CServerBrowser::Toggle() {
	CScrollingWindow::Toggle();
}


vector<string> CServerBrowser::GetSelectedText() {
	return m_pList->GetSelectedText();
}

void CServerBrowser::SetupBrowser(const ServerInfoSet &set) {
	m_pList->ClearList(false);

	// Add entries
	ServerInfoSet::const_iterator it = set.begin();
	for (; it != set.end(); it++){
		vector<string> row;
		std::stringstream ss;

		// name
		row.push_back(it->strName);

		// ip
		ss.str("");
		ss << it->strIP.c_str();
		ss << ":";
		ss << it->nPort;
		row.push_back(ss.str());


		// players
		ss.str("");
		ss << it->nPlayers << " / " << it->nMaxPlayers;
		row.push_back(ss.str());


		// ping
		ss.str("");
		ss << it->nPing;
		row.push_back(ss.str());

		m_pList->AddRow(row);
	}
}
void CServerBrowser::UpdateObjects(){
	// The max amount of lines in this font size that can fit in our alloted space
	m_pList->SetMaxLines((m_nHeight - (2 * BORDEROFFSET)) / 20);
	m_pList->SetWidth( m_nWidth - (2 * BORDEROFFSET) );
	m_pList->SetHeight( m_nHeight - (2 * BORDEROFFSET) );
}