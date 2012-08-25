#include "dxstdafx.h"
#include "MultiColumnList.h"

CMultiColumnList::CMultiColumnList(int _maxLines) : m_iMaxLines(_maxLines), m_start(1), m_end(1) {
	m_vRows.push_back(string());

	m_nColumns = 0;
	m_nRowHeight = 20;
	m_selected = -1;
	m_nSortBy = -1;
	m_font = NULL;

	std::stringstream ss;
	ss << SEP_CHAR;
	m_delimiter.append(ss.str().c_str());
};
CMultiColumnList::~CMultiColumnList() {
	SAFE_RELEASE(m_font);
};

void CMultiColumnList::AddRow(LPCSTR text){
	if (m_nSortBy == -1) {
		m_vRows.push_back(string(text));
	} else {

		// TODO: TEST THIS, it probably doesn't work
		// put it in to the vector in the right position
		vector<string>::iterator it = m_vRows.begin();
		string item(GetNthItemInRow(string(text), m_nSortBy));
		int row = 0;
		for (; it != m_vRows.end(); it++){
			string rowitem(GetNthItemInRow(m_vRows[row], m_nSortBy));
			if (rowitem.compare(item) > 0) {
				m_vRows.insert(it, string(text));
				break;
			}
			row++;
		}
	}
	UpdateBounds();
}

void CMultiColumnList::AddColumn(LPCSTR title){
	if (m_nColumns > 0)
		m_vRows[0].append(m_delimiter);
	m_vRows[0].append(title);
	m_nColumns++;
}
void CMultiColumnList::ClearList(bool bClearTitles) {
	assert(m_vRows.size() > 0); // vector should never be empty
	vector<string>::iterator it = m_vRows.begin();

	if (bClearTitles){ // Get rid of the titles row
		it = m_vRows.erase(it);
		m_nColumns = 0;
	}
	else 
		it++;

	while (it != m_vRows.end())
		it = m_vRows.erase(it);

	if (m_vRows.empty())
		m_vRows.push_back(string());

	UpdateBounds();
}
bool CMultiColumnList::KeyboardProc(UINT nChar, bool bKeyDown){
	if (!bKeyDown) return false;
	if (nChar == VK_PRIOR)
		PageUp();
	else if (nChar == VK_NEXT)
		PageDown();
	else 
		return false;
	return true;
}
void CMultiColumnList::MouseProc(int xPos, int yPos){
		if (m_nX > xPos || xPos > m_nX + m_nWidth ||
			m_nY > yPos || yPos > m_nY + m_nHeight){

				return;
		}

		// find out which row was clicked on
		UINT row  = (UINT) ((yPos - m_nY) / m_nRowHeight);
		row += m_start;

		m_selected = row - 1;
		if (m_selected == 0 || m_selected >= (int) m_vRows.size())
			m_selected = -1;
}
void CMultiColumnList::OnLostDevice(){ 
	if (m_font)
		m_font->OnLostDevice();
}
void CMultiColumnList::OnResetDevice(){
	if (m_font) 
		m_font->OnResetDevice();
}
void CMultiColumnList::DrawRowText(int r, long rowHeight, long columnWidth, bool bTitles){
	long prevColWidth = 0;
	std::stringstream ss(m_vRows[r]);

	for (UINT c = 0; c < m_nColumns; c++){
		RECT rct;
		if (bTitles)
			rct.top = m_nY;
		else
			rct.top = m_nY + ((r - m_start + 1) * rowHeight);
		rct.bottom = rct.top + rowHeight;
		rct.left = m_nX + prevColWidth;
		rct.right = rct.left + columnWidth;

		// get next token
		char input[100];
		ss.getline(input, 100, SEP_CHAR);

		//draw text in rct
		m_font->DrawTextA(NULL, input, -1, &rct, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255,255,255,255) );


		prevColWidth += columnWidth;
	}
}
void CMultiColumnList::OnFrameRender(IDirect3DDevice9 *pd3dDevice){
	if (!m_font){
		D3DXCreateFont( pd3dDevice, 18, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			TEXT("Arial"), &m_font );
	}

	if (m_nColumns == 0)
		return;
	long rowHeight = (long) 20;
	long columnWidth = (long) m_nWidth / m_nColumns;
	DrawRowText(0, rowHeight, columnWidth, true);
	for (int r = m_start; r <= (int)m_end; r++){
		if (r == m_selected){
			float left = (float) m_nX, 
				right = (float) left + m_nWidth, 
				top = (float) m_nY + ((r - m_start + 1) * rowHeight), 
				bottom = (float) top + 20;
			DWORD color = 0x44ffffff;
			SVertex bverts[] = 
			{
				{ left,		bottom,	0.0f,	1.0f,	color },
				{ left,		top,	0.0f,	1.0f,	color },
				{ right,	top,	0.0f,	1.0f,	color },
				{ right,	bottom,	0.0f,	1.0f,	color },
				{ left,		bottom,	0.0f,	1.0f,	color }
			};
			pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, bverts, sizeof(SVertex));			
		}
		DrawRowText(r, rowHeight, columnWidth, false);
	}
}
void CMultiColumnList::PageUp(){
	if (m_start == 1) // can't go any further up
		return;
	m_start--;
	m_end--;
}
void CMultiColumnList::PageDown(){
	if (m_end == m_vRows.size() - 1) // can't go any further down
		return;
	m_start++;
	m_end++;
}
void CMultiColumnList::SetMaxLines(int max){
	m_iMaxLines = max;

	UpdateBounds();
}

LPCSTR CMultiColumnList::GetSelectedText() {
	if (m_selected == -1)
		return "";
	return m_vRows[m_selected].c_str();
}

void CMultiColumnList::UpdateBounds(){
		m_end =(UINT) m_vRows.size() - 1;
		if (m_end >= m_iMaxLines)
			m_start = m_end - m_iMaxLines + 2;
		else
			m_start = 1;
	}