#pragma once

#include "dxstdafx.h"
#include "GUIObject.h"

using std::vector;
using std::string;

#define SEP_CHAR (char) 59

class CMultiColumnList : public CGUIObject
{
private:
	vector<string>						m_vRows; //! Vector of strings
	UINT								m_iMaxLines; //! Max number of lines visible
	UINT								m_start; //! These integers tell which
	UINT								m_end;   //! lines are currently visible
	int									m_selected;	//! Currently selected line
	int									m_nSortBy;	//! which column is this list currently sorted by
	UINT								m_nColumns;
	int									m_nRowHeight;
	LPD3DXFONT							m_font;		//! Font for text
	string								m_delimiter;

public:
	CMultiColumnList(int _maxLines);
	~CMultiColumnList();
	void AddRow(LPCSTR text);
	void AddColumn(LPCSTR title);
	void ClearList(bool bClearTitles);
	bool KeyboardProc(UINT nChar, bool bKeyDown);
	void MouseProc(int xPos, int yPos);
	void OnLostDevice();
	void OnResetDevice();
	void OnFrameRender(IDirect3DDevice9 *pd3dDevice);
	void PageUp();
	void PageDown();
	
	void SetMaxLines(int max);
	void SetSortColumnIndex(int index) { m_nSortBy = index; }
	void SetHeight(int h) { m_nHeight = h; }
	void SetWidth(int w) { m_nWidth = w; }
	void SetX(int x) { m_nX = x; }
	void SetY(int y) { m_nY = y; }
	LPCSTR GetSelectedText();
	LPCSTR GetDelimiter() { return m_delimiter.c_str(); }
	LPCSTR GetNthItemInRow(const string row, int n) {
		std::stringstream ss(row);
		char input[100];
		for (int i = 0; i <= n; i++){
			// get next token
			ss.getline(input, 100, SEP_CHAR);
		}
		string s(input);
		return s.c_str();
	}

private:
	void UpdateBounds();
	void DrawRowText(int r, long rowHeight, long columnWidth, bool bTitles);

};