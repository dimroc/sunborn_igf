#include "dxstdafx.h"
#include "EditableMultiColumnList.h"
#include "GUIDefine.h"
#include "GameTables.h"

CEditableMultiColumnList::CEditableMultiColumnList(int _maxLines) : m_iMaxLines(_maxLines), m_start(0), m_end(0) {
	m_nRowHeight = 20;
	m_nSortBy = -1;

	m_SelectedCell.column = -1;
	m_SelectedCell.row = -1;
	m_SelectedCell.bEditing = false;
	m_SelectedCell.line.Clear();

	std::stringstream ss;
	ss << SEP_CHAR;
	m_delimiter.append(ss.str().c_str());
};
CEditableMultiColumnList::~CEditableMultiColumnList() { };

// Returns false if there weren't enough columns for this row
bool CEditableMultiColumnList::AddRow(vector<string> rowdata){
	if (rowdata.size() != m_vColumns.size())
		return false;

	if (m_nSortBy == -1) {
		for (int i = 0; i < (int) rowdata.size(); i++){
			m_vColumns[i].data.push_back(rowdata[i]);
		}
	} else {

	}
	UpdateBounds();
	return true;
}

void CEditableMultiColumnList::AddColumn(LPCSTR title, bool bEditable, bool bUnique, int nFormat, float fWidth){
	SColumn column;
	column.title.append(title);
	column.bEditable = bEditable;
	column.bUnique = bUnique;
	column.nFormat = nFormat;
	column.fWidth = fWidth;

	assert(nFormat < SColumn::NUM_FORMATS);
	m_vColumns.push_back(column);
}
void CEditableMultiColumnList::ClearList(bool bClearTitles, bool bClearSelected) {
	if (bClearTitles)
		m_vColumns.clear();
	else {
		for (int i = 0; i < (int) m_vColumns.size(); i++){
			m_vColumns[i].data.clear();
		}
	}
	UpdateBounds();

	if (bClearSelected){
		m_SelectedCell.row = -1;
		m_SelectedCell.column = -1;
	}
}
bool CEditableMultiColumnList::KeyboardProc(UINT nChar, bool bKeyDown){
	if (!bKeyDown){ 
		if (nChar == VK_SHIFT)
			m_KeyCombo.bShift = false;
		if (nChar == VK_CONTROL)
			m_KeyCombo.bCtrl = false;
		if (nChar == VK_MENU)
			m_KeyCombo.bAlt = false;

		if (m_SelectedCell.bEditing && m_SelectedCell.column != -1 && m_vColumns[m_SelectedCell.column].nFormat == SColumn::KEY){
			// The user just wanted to set it to one of ctrl, alt or shift
			// Don't allow a combination of just those three

			// biggest string length of only 1 of those buttons is 6 (5+' ')
			// note: smallest string length of 2 buttons is 9 'ctrl alt '
			if (m_SelectedCell.line.Length() <= 6){
				// Apply the text to the selected box
				m_SelectedCell.line.Backspace(); // to get rid of the trailing space
				ApplyTextToCell();
			}
			else {
				m_SelectedCell.line.Clear();
				m_SelectedCell.bEditing = false;
			}
			return true;
		}

		return false;
	}
	if (!m_SelectedCell.bEditing || m_vColumns[m_SelectedCell.column].nFormat != SColumn::KEY){
		if (nChar == VK_PRIOR || nChar == VK_UP){
			PageUp();
			return true;
		}
		else if (nChar == VK_NEXT || nChar == VK_DOWN){
			PageDown();
			return true;
		}
		if (!m_SelectedCell.bEditing)
			return false;
	}

	if (m_SelectedCell.bEditing){
		if (nChar == VK_ESCAPE) { // Escape can't be used as for a keybinding
			m_SelectedCell.line.Clear();
			m_SelectedCell.bEditing = false;
		}

		if (m_vColumns[m_SelectedCell.column].nFormat == SColumn::KEY){
			// Translate their key press into a string and add it to the line
			string text = (GameTable::FindKeyWordMatch((UCHAR) nChar));
			if (text.empty())
				m_SelectedCell.line.AddChar(guidef::GetKeyboardCharacter(nChar, false));	
			else if (nChar == VK_SHIFT && m_KeyCombo.bShift)
				;
			else if (nChar == VK_CONTROL && m_KeyCombo.bCtrl)
				;
			else if (nChar == VK_MENU && m_KeyCombo.bAlt)
				;
			else {
				if (nChar == VK_SHIFT || nChar == VK_CONTROL || nChar == VK_MENU)
					text.append(" ");
				m_SelectedCell.line.Append(text.c_str());
			}

			if (nChar != VK_SHIFT && nChar != VK_CONTROL && nChar != VK_MENU){
				// Apply the text to the selected box
				ApplyTextToCell();
			}

		} else {
			if (nChar == VK_RETURN) {
				// Apply the text to the selected box
				ApplyTextToCell();
			}
			else if (nChar == VK_BACK){
				m_SelectedCell.line.Backspace();
			}
			else {
				UINT c = 0;
				if (m_vColumns[m_SelectedCell.column].nFormat == SColumn::NUMERIC)
					c = guidef::GetKeyboardNumericCharacter(nChar, m_KeyCombo.bShift);
				else if (m_vColumns[m_SelectedCell.column].nFormat == SColumn::STRING)
					c = guidef::GetKeyboardCharacter(nChar, m_KeyCombo.bShift);
				if (c != 0)
					m_SelectedCell.line.AddChar(c);
			}
		}
		if (nChar == VK_SHIFT)
			m_KeyCombo.bShift = true;
		else if (nChar == VK_CONTROL)
			m_KeyCombo.bCtrl = true;
		else if (nChar == VK_MENU)
			m_KeyCombo.bAlt = true;
	}
	return true;
}
void CEditableMultiColumnList::MouseProc(int xPos, int yPos){
	if (!CheckInBounds(xPos, yPos))
		return;

	if (m_vColumns.empty())
		return;

	// Store the old values of the cell coordinates
	int oldRow = m_SelectedCell.row;
	int oldColumn = m_SelectedCell.column;
	bool oldEditing = m_SelectedCell.bEditing;

	// find out which row was clicked on
	int row  = (yPos - m_nY) / m_nRowHeight;
	row += m_start;

	int selected = row - 1;
	if (selected >= (int) m_vColumns[0].data.size())
		selected = -1;

	// find out what cell was clicked on
	m_SelectedCell.row = selected;
	int prevWidth = m_nX;
	for (int i = 0; i < (int) m_vColumns.size(); i++){
		SColumn column = m_vColumns[i];
		int columnWidth;
		if (column.fWidth == 0) // If width isn't defined, default to an even division of the space between columns
			columnWidth = GetDefaultWidth();	
		else
			columnWidth = (int) (column.fWidth * m_nWidth);

		if (prevWidth <= xPos && xPos <= prevWidth + columnWidth) {
			// Found a match
			m_SelectedCell.column = i;
			break;
		}
		prevWidth += columnWidth;
	}

	// Set editing to true if this is at least the second click on this cell
	if (m_SelectedCell.row != -1 && m_SelectedCell.column != -1){
		m_SelectedCell.bEditing = m_vColumns[m_SelectedCell.column].bEditable && oldRow == m_SelectedCell.row && oldColumn == m_SelectedCell.column;
		if (!oldEditing && m_SelectedCell.bEditing)
			if (m_vColumns[m_SelectedCell.column].nFormat == SColumn::KEY)
				m_SelectedCell.line.Clear();
			else
				m_SelectedCell.line.SetText(m_vColumns[m_SelectedCell.column].data[m_SelectedCell.row].c_str());
	}
}
void CEditableMultiColumnList::OnFrameRender(IDirect3DDevice9 *pd3dDevice, LPD3DXFONT m_font){
	float left = (float) m_nX,
		right = left + m_nWidth,
		top = (float) m_nY + m_nRowHeight,
		bottom = top + m_nHeight - m_nRowHeight;
	SVertex backverts[] = 
	{
		{ left,		bottom,		0.0f,	1.0f,	0x11ffffff },
		{ left,		top,		0.0f,	1.0f,	0x11ffffff },
		{ right,	top,		0.0f,	1.0f,	0x11ffffff },
		{ right,	bottom,		0.0f,	1.0f,	0x11ffffff },
		{ left,		bottom,		0.0f,	1.0f,	0x11ffffff }
	};
	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backverts, sizeof(SVertex));
	SVertex backedges[] = 
	{
		{ left,		bottom,		0.0f,	1.0f,	0xccffffff },
		{ left,		top,		0.0f,	1.0f,	0xccffffff },
		{ right,	top,		0.0f,	1.0f,	0xccffffff },
		{ right,	top,		0.0f,	1.0f,	0xcc888888 },
		{ right,	bottom,		0.0f,	1.0f,	0xcc888888 },
		{ left,		bottom,		0.0f,	1.0f,	0xcc888888 }
	};
	pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 5, backedges, sizeof(SVertex));

	// Nothing to draw
	if (m_vColumns.size() == 0)
		return;

	// Highlight the selected row
	if (m_SelectedCell.row != -1 && m_start <= m_SelectedCell.row && m_SelectedCell.row <= m_end){
		float left = (float) m_nX, 
			right = (float) left + m_nWidth, 
			top = (float) m_nY + ((m_SelectedCell.row - m_start + 1) * m_nRowHeight), 
			bottom = (float) top + m_nRowHeight;
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

	long rowHeight = (long) m_nRowHeight;
	long prevColWidth = 0;
	// Draw each column
	for (int i = 0; i < (int) m_vColumns.size(); i++){
		SColumn &column = m_vColumns[i];
		long columnWidth;
		if (column.fWidth == 0){ // If width isn't defined, default to an even division of the space between columns
			columnWidth = (long) GetDefaultWidth();	
			column.fWidth = (float)columnWidth / (float)m_nWidth; // Set fWidth so we don't need to call GetDefaultWidth anymore
		}
		else
			columnWidth = (long) (column.fWidth * m_nWidth);
		RECT rct;
		rct.top = m_nY;
		rct.bottom = rct.top + rowHeight;
		rct.left = m_nX + prevColWidth;
		rct.right = rct.left + columnWidth;
		// Draw title
		m_font->DrawTextA(NULL, column.title.c_str(), -1, &rct, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255,255,255,255) );

		// Draw each row of the column
		for (int r = m_start; r <= (int)m_end; r++){
			rct.top += rowHeight;
			rct.bottom += rowHeight;
			if (m_SelectedCell.bEditing && m_SelectedCell.column == i && m_SelectedCell.row == r){
				// Extra highlight on the selected cell
				float left = (float) rct.left, 
					right = (float) rct.right, 
					top = (float) rct.top, 
					bottom = (float) rct.bottom;
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

				if (column.bEditable){
					SVertex verts[] = 
					{
						{ left,		bottom,	0.0f,	1.0f,	0xccffffff },
						{ left,		top,	0.0f,	1.0f,	0xccffffff },
						{ right,	top,	0.0f,	1.0f,	0xccffffff },
						{ right,	top,	0.0f,	1.0f,	0xcc888888 },
						{ right,	bottom,	0.0f,	1.0f,	0xcc888888 },
						{ left,		bottom,	0.0f,	1.0f,	0xcc888888 }
					};
					pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 5, verts, sizeof(SVertex));

				}

				m_font->DrawTextA(NULL, m_SelectedCell.line.GetText(), -1, &rct, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255,255,255,255) );
			}
			else
				m_font->DrawTextA(NULL, column.data[r].c_str(), -1, &rct, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255,255,255,255) );
		}
		prevColWidth += columnWidth;
	}
}
void CEditableMultiColumnList::PageUp(){
	if (m_start == 0) // can't go any further up
		return;
	m_start--;
	m_end--;
}
void CEditableMultiColumnList::PageDown(){
	if (m_vColumns.empty()) // why u pagin down
		return;
	if (m_end == (int) m_vColumns[0].data.size() - 1) // can't go any further down
		return;
	m_start++;
	m_end++;
}
void CEditableMultiColumnList::MoveToTop(){
	m_start = 0;
	if (m_vColumns.empty())
		m_end = -1;
	else
		m_end = min((int)m_vColumns[0].data.size() - 1, m_iMaxLines - 1);
}
void CEditableMultiColumnList::SetMaxLines(int max){
	m_iMaxLines = max;

	UpdateBounds();
}
int CEditableMultiColumnList::GetRowCount(){
	if (m_vColumns.empty())
		return -1;
	return (int) m_vColumns[0].data.size();
}
vector<string> CEditableMultiColumnList::GetRowText(int row) {
	vector<string> text;
	if (m_vColumns.empty() || row < 0 || row >= (int) m_vColumns[0].data.size())
		return text;

	for (int i = 0; i < (int) m_vColumns.size(); i++) {
		text.push_back(m_vColumns[i].data[row]);
	}
	return text;
}

void CEditableMultiColumnList::UpdateBounds(){
	if (m_vColumns.empty()) {
		m_end = -1;
		m_start = 0;
		return;
	}

	m_end = (int) m_vColumns[0].data.size() - 1;
	if (m_end >= m_iMaxLines)
		m_start = m_end - m_iMaxLines + 1;
	else
		m_start = 0;
}
void CEditableMultiColumnList::ApplyTextToCell(){
	assert(!m_vColumns.empty());

	// Change the value of the currently selected cell
	m_vColumns[m_SelectedCell.column].data[m_SelectedCell.row].assign(m_SelectedCell.line.GetText());
	m_SelectedCell.line.Clear();
	m_SelectedCell.bEditing = false;

	if (m_vColumns[m_SelectedCell.column].bUnique){
		// Make sure no other cell in this column has the same value as the one we just added
		string text = m_vColumns[m_SelectedCell.column].data[m_SelectedCell.row];
		for (int i = 0; i < (int) m_vColumns[m_SelectedCell.column].data.size(); i++){
			if (i != m_SelectedCell.row)
				if (m_vColumns[m_SelectedCell.column].data[i].compare(text) == 0) // This row matches
					m_vColumns[m_SelectedCell.column].data[i].assign(""); // set it to empty string (the user has to reenter it)
		}
	}
}
int CEditableMultiColumnList::GetDefaultWidth(){
	if (m_vColumns.empty())
		return -1;

	float totalSet = 1.f;
	int n = 0;
	for (int i = 0; i < (int)m_vColumns.size(); i++){
		if (m_vColumns[i].fWidth != 0) {
			totalSet -= m_vColumns[i].fWidth;
			n++;
		}
	}
	if ((int)m_vColumns.size() != n)
		return (int)((totalSet * m_nWidth) / ((int)m_vColumns.size() - n));

	return -1;
}