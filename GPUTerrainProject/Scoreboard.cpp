#include "dxstdafx.h"
#include "Scoreboard.h"

using std::string;

CScoreboard::CScoreboard(WindowAnchor _anchor, float _xpercent, float _ypercent) : CScrollingWindow(0xaa000000) {
	m_pList = new CEditableMultiColumnList(0);
	anchor = _anchor;
	xPercent = _xpercent;
	yPercent = _ypercent;
	hidden = true;

	UpdateDimensions();
}
CScoreboard::~CScoreboard(){
	SAFE_DELETE(m_pList);
}

void CScoreboard::Animate() {
	CScrollingWindow::Animate();

	m_pList->SetX( m_nX + BORDEROFFSET );
	m_pList->SetY( m_nY + BORDEROFFSET );
}

void CScoreboard::UpdateDimensions(){
	CScrollingWindow::UpdateDimensions();

	UpdateObjects();
}
void CScoreboard::OnResetDevice(){
	CScrollingWindow::OnResetDevice();

	UpdateObjects();
}
void CScoreboard::OnLostDevice(){
	CScrollingWindow::OnLostDevice();
}
void CScoreboard::OnFrameRender(IDirect3DDevice9 *pd3dDevice, bool bHasFocus){
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
bool CScoreboard::KeyboardProc(UINT nChar, bool bKeyDown){ 
	return m_pList->KeyboardProc(nChar, bKeyDown); 
}
bool CScoreboard::MouseProc(int x, int y, bool hadFocus){
	if (hadFocus)
		m_pList->MouseProc(x, y);

	return CheckInBounds(x, y);
}
void CScoreboard::Toggle() {
	CScrollingWindow::Toggle();
}


vector<string> CScoreboard::GetSelectedText() {
	return m_pList->GetSelectedText();
}

void CScoreboard::SetupScoreboard(const PlayerHashMap &hmPlayers) {
	m_pList->ClearList(true);

	// Add titles
	m_pList->AddColumn("Name", false, false, CEditableMultiColumnList::SColumn::STRING);
	m_pList->AddColumn("Score", false, false, CEditableMultiColumnList::SColumn::NUMERIC);

	// Add entries
	PlayerHashMap::const_iterator playerit = hmPlayers.begin();
	for( ; playerit != hmPlayers.end(); playerit++)
	{
		vector<string> line;
		const SPlayer* player = &playerit->second;

		// Name
		line.push_back(string(player->strAlias));

		// Score
		std::stringstream ss;
		ss << player->nScore;
		line.push_back(ss.str());

		m_pList->AddRow(line);
	}
}

void CScoreboard::UpdateObjects(){
	// The max amount of lines in this font size that can fit in our alloted space
	m_pList->SetMaxLines((m_nHeight - (2 * BORDEROFFSET)) / 20);
	m_pList->SetWidth( m_nWidth - (2 * BORDEROFFSET) );
	m_pList->SetHeight( m_nHeight - (2 * BORDEROFFSET) );
}