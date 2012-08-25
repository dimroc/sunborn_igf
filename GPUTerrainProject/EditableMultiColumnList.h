#pragma once

#include "dxstdafx.h"
#include "GUIObject.h"
#include "UserTextLine.h"

using std::vector;
using std::string;

#define SEP_CHAR (char) 59

class CEditableMultiColumnList : public CGUIObject
{
public:
	struct SColumn {
		enum { STRING = 0, NUMERIC, KEY, NUM_FORMATS };	//! Format of the data in this column

		string title;				//! Title of this column
		vector<string> data;		//! list (in order) of data
		float fWidth;				//! width of this column (percent of list width), 0 = default
		bool bEditable;				//! is this a column that the user will be able to edit
		int nFormat;				//! one of the items in the above enum
		bool bUnique;			//! are the values in this column unique
	};

	struct SCell {
		int row, column;
		bool bEditing;
		CUserTextLine line;
	};

	struct SKeyCombo
	{
		UCHAR cKey;
		bool bShift, bCtrl, bAlt;
		SKeyCombo() : cKey(0), bShift(false), bCtrl(false), bAlt(false) { }
		SKeyCombo(UCHAR _cKey, bool _bShift, bool _bCtrl, bool _bAlt) : cKey(_cKey), bShift(_bShift), bCtrl(_bCtrl), bAlt(_bAlt) { }
	};
private:
	vector<SColumn>						m_vColumns; //! Vector of strings
	int									m_iMaxLines; //! Max number of lines visible
	int									m_start; //! These integers tell which
	int									m_end;   //! lines are currently visible
	int									m_nSortBy;	//! which column is this list currently sorted by
	int									m_nRowHeight;
	string								m_delimiter;
	SCell								m_SelectedCell; //! Currently selected line
	SKeyCombo							m_KeyCombo;

public:
	CEditableMultiColumnList(int _maxLines);
	~CEditableMultiColumnList();

	bool AddRow(vector<string> rowdata);
	void AddColumn(LPCSTR title, bool bEditable, bool bUnique, int nFormat, float fWidth = 0.f);
	void ClearList(bool bClearTitles, bool bClearSelected = false);

	bool KeyboardProc(UINT nChar, bool bKeyDown);
	void MouseProc(int xPos, int yPos);
	void OnFrameRender(IDirect3DDevice9 *pd3dDevice, LPD3DXFONT m_font);

	void PageUp();
	void PageDown();
	void MoveToTop();

	void SetMaxLines(int max);
	void SetSortColumnIndex(int index) { m_nSortBy = index; }


	int GetRowCount();
	vector<string> GetSelectedText(){ return GetRowText(m_SelectedCell.row);	}
	vector<string> GetRowText(int row);
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
	void ApplyTextToCell();
	int GetDefaultWidth();
};