#include "UserTextLine.h"
#include "dxstdafx.h"

using std::vector;

class CConsoleBuffer
{
private:
	vector<CUserTextLine*>				m_vLines; //! Vector of CLines
	UINT								m_iMaxLines; //! Max number of lines visible
	UINT								m_start; //! These integers tell which
	UINT								m_end;   //! lines are currently visible
public:
	CConsoleBuffer(int _maxLines);
	~CConsoleBuffer();

	//Adds this line of text to the end of the buffer
	//Removes the first line in the buffer if the max # of lines is reached
	void AddLine(LPCSTR text);
	void OnFrameRender(IDirect3DDevice9 *pd3dDevice, LPD3DXFONT font, RECT rct);
	void PageUp();
	void PageDown();
	void SetMaxLines(int max);

private:
	void UpdateBounds();
};