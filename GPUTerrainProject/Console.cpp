#include "dxstdafx.h"
#include "Console.h"
#include "GameTables.h"
#include "GUIDefine.h"


CConsole::CConsole(LPDIRECT3DDEVICE9 pd3dDevice, WindowAnchor _anchor, float _xpercent, float _ypercent, CGameGUI* _m_pGUI, GUICallback _func) : 
CScrollingWindow(0xaa000000), m_bActive(false), m_bShift(false), 
m_iFontSize(18), m_pGUI(_m_pGUI), m_pFunc(_func){ 
	m_pBuffer = new CConsoleBuffer(10); 
	m_pLine = new CUserTextLine();
	SetupConsole(pd3dDevice, _anchor, _xpercent, _ypercent);
}
CConsole::~CConsole() { 
	SAFE_DELETE(m_pBuffer);
	SAFE_DELETE(m_pLine);
	m_pGUI = NULL;
	m_pFunc = NULL;
}

bool CConsole::KeyboardProc(UINT nChar, bool bKeyDown) 
{
	if ( !bKeyDown ){
		if (nChar == VK_SHIFT)
			m_bShift = false;
		return m_bActive;
	}
	if (nChar == VK_ESCAPE) {
		if (m_pLine->Length() > 0)
			m_pLine->Clear();
		else
			ToggleConsole();
	}
	else if (nChar == VK_RETURN) {
		// Add a copy of this line to the Buffer
		m_pBuffer->AddLine(m_pLine->GetText());
		ParseText(m_pLine->GetText());
		m_pLine->Clear();
	}
	else if (nChar == VK_SHIFT) {
		m_bShift = true;
	}
	else if (nChar == VK_BACK)
		m_pLine->Backspace();
	else if (nChar == VK_PRIOR)
		m_pBuffer->PageUp();
	else if (nChar == VK_NEXT)
		m_pBuffer->PageDown();
	else {
		UINT c = guidef::GetKeyboardCharacter(nChar, m_bShift);
		if (c != 0)
			m_pLine->AddChar(c);
	}
	return m_bActive;
}
bool CConsole::MouseProc(int x, int y, bool hadFocus){
	if (hadFocus){
		// do something if u want
	}

	return CheckInBounds(x, y);
}

void CConsole::OnFrameRender(IDirect3DDevice9 *pd3dDevice, bool bHasFocus){
	if (!m_bActive && GetAnimType() == AnimOb::NONE)
		return;

	CScrollingWindow::OnFrameRender(pd3dDevice, bHasFocus); // Call the superclass function

	// Draw the buffer
	RECT rct;
	rct.top = m_nY + BORDEROFFSET;
	rct.left = m_nX + BORDEROFFSET;
	rct.right = rct.left + m_nWidth - (BORDEROFFSET*2);
	rct.bottom = rct.top + (m_iFontSize + 2);
	m_pBuffer->OnFrameRender(pd3dDevice, m_font, rct);

	rct.top = m_nY + m_nHeight - m_iFontSize - BORDEROFFSET;
	rct.bottom = rct.top + (m_iFontSize + 2);
	float left = (float) rct.left - 4, 
		right = (float) rct.right + 4, 
		top = (float) rct.top - 4, 
		bottom = (float) rct.bottom + 4;
	DWORD color = 0x11ffffff;
	SVertex bverts[] = 
	{
		{ left,		bottom,	0.0f,	1.0f,	color },
		{ left,		top,	0.0f,	1.0f,	color },
		{ right,	top,	0.0f,	1.0f,	color },
		{ right,	bottom,	0.0f,	1.0f,	color },
		{ left,		bottom,	0.0f,	1.0f,	color }
	};
	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, bverts, sizeof(SVertex));

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

	m_font->DrawTextA(NULL, m_pLine->GetText(), -1, &rct, DT_LEFT, D3DCOLOR_ARGB(255,255,255,255) );
}
void CConsole::OnLostDevice(){
	CScrollingWindow::OnLostDevice();
}
void CConsole::OnResetDevice(){
	CScrollingWindow::OnResetDevice();
	m_pBuffer->SetMaxLines(((m_nHeight - (BORDEROFFSET*2)) / (m_iFontSize + 2)) - 1);
}
void CConsole::ParseText(LPCSTR text) {
	if (text[0] != '/')
		return; // for now we only care about slash commands 

	string input(text);

	// Search the input starting from the 1th character in the string
	string::size_type pos = input.find(' ', 0);
	string cmd = input.substr(1, pos-1);
	string param;
	if (pos != string::npos)
		param = input.substr(pos + 1, string::npos);

	UCHAR cmdIdx = GameTable::FindCommandIndex(cmd);
	if(cmdIdx > GameTable::NO_COMMAND){
		(m_pGUI->*m_pFunc)(cmdIdx, param);
	} else {
		string error(" Command not found: " + cmd);
		PostMessage(error.c_str());
	}
}
void CConsole::PostMessage(LPCSTR text){ 
	m_pBuffer->AddLine(text); 
}
void CConsole::SetFontSize(int size){
	m_iFontSize = size;
}
void CConsole::SetupConsole(LPDIRECT3DDEVICE9 pd3dDevice, WindowAnchor _anchor, float _xpercent, float _ypercent){
	anchor = _anchor;
	xPercent = _xpercent;
	yPercent = _ypercent;

	hidden = true;
	UpdateDimensions();

	SAFE_RELEASE(m_font);
	// Create a D3DX font object
	D3DXCreateFont( pd3dDevice, m_iFontSize, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		TEXT("Arial"), &m_font );

}
void CConsole::ToggleConsole()
{
	m_bActive = !m_bActive;
	Toggle();    
}
void CConsole::UpdateDimensions(){
	CScrollingWindow::UpdateDimensions();

	// The max amount of lines in this font size that can fit in our alloted space
	m_pBuffer->SetMaxLines(((m_nHeight - (BORDEROFFSET*2)) / (m_iFontSize + 2)) - 1);
}