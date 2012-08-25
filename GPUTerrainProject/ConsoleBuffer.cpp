#include "dxstdafx.h"
#include "ConsoleBuffer.h"
#include "AnimOb.h"

CConsoleBuffer::CConsoleBuffer(int _maxLines) : m_iMaxLines(_maxLines), m_start(0), m_end(0) { };
CConsoleBuffer::~CConsoleBuffer() {
	for (UINT i = 0; i < m_vLines.size(); i++){
		SAFE_DELETE(m_vLines[i]);
	}
};

//Adds this line of text to the end of the buffer
//Removes the first line in the buffer if the max # of lines is reached
void CConsoleBuffer::AddLine(LPCSTR text){
	if (m_vLines.size() == m_iMaxLines * 3){ 
		SAFE_DELETE(m_vLines[0]);
		m_vLines.erase(m_vLines.begin());
	}
	m_vLines.push_back(new CUserTextLine(text));
	UpdateBounds();
}


void CConsoleBuffer::OnFrameRender(IDirect3DDevice9 *pd3dDevice, LPD3DXFONT font, RECT rct){
	int height = rct.bottom - rct.top; // The height of one line in the buffer
	float left = (float) rct.left - 4, 
		right = (float) rct.right + 4, 
		top = (float) rct.top - 4;
	SVertex bverts[] = 
	{
		{ left,		top + (height * (int)m_iMaxLines) + 8,	0.0f,	1.0f,	0x11ffffff },
		{ left,		top,									0.0f,	1.0f,	0x11ffffff },
		{ right,	top,									0.0f,	1.0f,	0x11ffffff },
		{ right,	top + (height * (int)m_iMaxLines) + 8,	0.0f,	1.0f,	0x11ffffff },
		{ left,		top + (height * (int)m_iMaxLines) + 8,	0.0f,	1.0f,	0x11ffffff }
	};
	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, bverts, sizeof(SVertex));

	SVertex verts[] = 
	{
		{ left,		top + (height * (int)m_iMaxLines) + 8,	0.0f,	1.0f,	0xccffffff },
		{ left,		top,									0.0f,	1.0f,	0xccffffff },
		{ right,	top,									0.0f,	1.0f,	0xccffffff },
		{ right,	top,									0.0f,	1.0f,	0xcc888888 },
		{ right,	top + (height * (int)m_iMaxLines) + 8,	0.0f,	1.0f,	0xcc888888 },
		{ left,		top + (height * (int)m_iMaxLines) + 8,	0.0f,	1.0f,	0xcc888888 }
	};
	pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 5, verts, sizeof(SVertex));
	
	if (m_vLines.empty())
		return;

	for (UINT i = m_start; i <= m_end; i++){
		font->DrawTextA(NULL, m_vLines[i]->GetText(), -1, &rct, DT_LEFT, D3DCOLOR_ARGB(255,255,255,255) );
		rct.top = rct.top + height;
		rct.bottom = rct.top + height;
	}
}
void CConsoleBuffer::PageUp() {
	if (m_start == 0) // can't go any further up
		return;
	m_start--;
	m_end--;
}
void CConsoleBuffer::PageDown(){
	if (m_end == m_vLines.size() - 1) // can't go any further down
		return;
	m_start++;
	m_end++;
}

void CConsoleBuffer::SetMaxLines(int max) {
	m_iMaxLines = max;
	while (	m_vLines.size() > m_iMaxLines * 3){
		SAFE_DELETE(m_vLines[0]);
		m_vLines.erase(m_vLines.begin());
	}
	UpdateBounds();
}

void CConsoleBuffer::UpdateBounds() {
	m_end =(UINT) m_vLines.size() - 1;
	if (m_end >= m_iMaxLines)
		m_start = m_end - m_iMaxLines + 1;
	else
		m_start = 0;
}
