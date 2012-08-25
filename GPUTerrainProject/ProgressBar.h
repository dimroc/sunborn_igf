#pragma once

#include "dxstdafx.h"
#include "GUIObject.h"


class CProgressBar : public CGUIObject{
private:
public:
	int m_nCurrent, m_nMax;
	
	CProgressBar() { }
	CProgressBar(int x, int y, int h, int w, int curr, int max) : 
	CGUIObject(x, y, h, w), m_nCurrent(curr), m_nMax(max) { }
	~CProgressBar() { }

	void OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice) {
		float left = (float) m_nX, 
			right = (float) m_nX + m_nWidth,
			top = (float) m_nY, 
			bottom = (float) m_nY + m_nHeight,
			fraction = (m_nCurrent / (float) m_nMax * m_nWidth) + left;

		int alpha = 255;
		pd3dDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );

		DWORD color = D3DCOLOR_ARGB(alpha, 255, 100, 100);
		SVertex bverts[] = 
		{
			{ left,		bottom,	0.0f,	1.0f,	color },
			{ left,		top,	0.0f,	1.0f,	color },
			{ fraction,	top,	0.0f,	1.0f,	color },
			{ fraction,	bottom,	0.0f,	1.0f,	color },
			{ left,		bottom,	0.0f,	1.0f,	color }
		};
		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, bverts, sizeof(SVertex));

		color =  D3DCOLOR_ARGB(alpha / 2, 255, 100, 100);
		SVertex cverts[] = 
		{
			{ fraction,		bottom,	0.0f,	1.0f,	color },
			{ fraction,		top,	0.0f,	1.0f,	color },
			{ right,		top,	0.0f,	1.0f,	color },
			{ right,		bottom,	0.0f,	1.0f,	color },
			{ fraction,		bottom,	0.0f,	1.0f,	color }
		};
		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cverts, sizeof(SVertex));

		DWORD tcolor = D3DCOLOR_ARGB(alpha, 255, 255, 255);
		DWORD bcolor = D3DCOLOR_ARGB(alpha, 136, 136, 136);
		SVertex verts[] = 
		{
			{ left,		bottom,	0.0f,	1.0f,	tcolor },
			{ left,		top,	0.0f,	1.0f,	tcolor },
			{ right,	top,	0.0f,	1.0f,	tcolor },
			{ right,	top,	0.0f,	1.0f,	bcolor },
			{ right,	bottom,	0.0f,	1.0f,	bcolor },
			{ left,		bottom,	0.0f,	1.0f,	bcolor }
		};
		pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 5, verts, sizeof(SVertex));
	}
	void OnLostDevice(){ }
	void OnResetDevice() { }
	void UpdateDimensions(){ }

};