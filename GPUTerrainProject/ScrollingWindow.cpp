#include "dxstdafx.h"
#include "ScrollingWindow.h"

CScrollingWindow::CScrollingWindow(DWORD _bgColor) : AnimOb(-1,-1,-1,-1), 
initX(-1), initY(-1), hidden(true), m_Sprite(NULL), m_Texture(NULL), m_font(NULL), bgColor(_bgColor)
{

}
CScrollingWindow::~CScrollingWindow() {
	SAFE_RELEASE(m_Texture);
	SAFE_RELEASE(m_Sprite);
	SAFE_RELEASE(m_font);
}
void CScrollingWindow::Animate()
{
	if (GetAnimType() == AnimOb::WINDOW){
		if (GetEndX() == m_nX && GetEndY() == m_nY){
			this->SetAnimType(AnimOb::NONE);
			return;
		}
		int speed;

		// Update X coordinate
		speed = GetEndX() - m_nX;
		if (speed > m_speed)
			speed = m_speed;
		if (speed < -1 * m_speed)
			speed = -1 * m_speed;
		m_nX += speed;

		// Update Y coordinate
		speed = GetEndY() - m_nY;
		if (speed > m_speed)
			speed = m_speed;
		if (speed < -1 * m_speed)
			speed = -1 * m_speed;
		m_nY += speed;

	}
}

void CScrollingWindow::OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, bool bHasFocus, bool bSpriteStarted) {
	if (!m_Sprite){
		// Create sprite object
		D3DXCreateSprite(pd3dDevice,&m_Sprite);
	}
	if (!m_Texture){
		// Load a texture for our sprite
		// D3DXCreateTextureFromFileA(pd3dDevice, "..\\Media\\border.bmp", &m_Texture);
	}

	if (hidden && GetAnimType() == AnimOb::NONE)
		return;
	if (bgColor < 0x01000000) // alpha is too low, dont bother drawing
		return;

	pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pd3dDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );

	// Draw background 
	float x = (float) m_nX; //+2;
	float y = (float) m_nY; //+2;
	SVertex verts[] =
	{  //    X			,      Y    ,     Z, rhw,    color
		{ x				, y + m_nHeight	, 0,  1,    bgColor },
		{ x			    , y			    , 0,  1,    bgColor },
		{ x + m_nWidth	, y				, 0,  1,    bgColor },
		{ x + m_nWidth	, y + m_nHeight	, 0,  1,    bgColor },
		{ x				, y + m_nHeight	, 0,  1,    bgColor }
	};
	//	SVertex verts[] =
	//{  //    X			,      Y    ,     Z, rhw,    color
	//	{ x				, y + m_nHeight	-4, 0,  1,    bgColor },
	//	{ x			    , y			    , 0,  1,    bgColor },
	//	{ x + m_nWidth - 4	, y				, 0,  1,    bgColor },
	//	{ x + m_nWidth-4	, y + m_nHeight	-4, 0,  1,    bgColor },
	//	{ x				, y + m_nHeight	-4, 0,  1,    bgColor }
	//};

	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verts, sizeof(SVertex));

	if (bHasFocus){
		SVertex bverts[] = 
		{  //    X			,      Y    ,      Z,   rhw,    color
			{ x				, y + m_nHeight	, 0.f,  1.f, 0x66ffffff },
			{ x			    , y			    , 0.f,  1.f, 0x66ffffff },
			{ x + m_nWidth	, y				, 0.f,  1.f, 0x66ffffff },
			{ x + m_nWidth	, y + m_nHeight	, 0.f,  1.f, 0x66ffffff },
			{ x				, y + m_nHeight	, 0.f,  1.f, 0x66ffffff }
		};
		pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, bverts, sizeof(SVertex));
	}

	// Draw border (when we have one)
	//D3DSURFACE_DESC pDesc;
	//m_Texture->GetLevelDesc(0, &pDesc);

	//D3DXMATRIX mx, mxScale, mxTrans;
	//D3DXMatrixIdentity(&mx);
	//float xscale = (float)m_nWidth / pDesc.Width;
	//float yscale = (float)m_nHeight / pDesc.Height;
	//D3DXMatrixTranslation(&mxTrans, (float)m_nX, (float)m_nY, 0);
	//D3DXMatrixScaling(&mxScale, xscale, yscale, 1.0f);

	//mx = mxScale * mxTrans;
	//m_Sprite->SetTransform(&mx);

	//if (!bSpriteStarted)
	//	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	//m_Sprite->Draw(m_Texture, NULL, NULL,NULL,0xFFFFFFFF);
	//if (!bSpriteStarted)
	//	m_Sprite->End();

}
void CScrollingWindow::Toggle() {
	int dx = 0, dy = 0;
	switch(anchor){
		case CScrollingWindow::TOP:
			dx = 0;
			dy = -1 * m_nHeight;
			break;
		case CScrollingWindow::RIGHT:
			dx = m_nWidth;
			dy = 0;
			break;
		case CScrollingWindow::BOTTOM:
			dx = 0;
			dy = m_nHeight;
			break;
		case CScrollingWindow::LEFT:
			dx = -1 * m_nWidth;
			dy = 0;
			break;
		case CScrollingWindow::NONE:
			dx = 0;
			dy = (int) DXUTGetBackBufferSurfaceDesc()->Height - initY;
			break;
	}
	if (hidden) { // bring window into view
		this->SetEndX(initX);
		this->SetEndY(initY);
	}
	else { // hide the window
		this->SetEndX(initX + dx);
		this->SetEndY(initY + dy);
	}
	SetAnimType(AnimOb::WINDOW);
	hidden = !hidden;
}
void CScrollingWindow::UpdateDimensions(){
	const D3DSURFACE_DESC* desc = DXUTGetBackBufferSurfaceDesc();

	switch (anchor){
		case CScrollingWindow::TOP:
			initX = 0;
			initY = 0;
			m_nHeight = (int) (desc->Height * yPercent);
			m_nWidth = (int) (desc->Width * xPercent);
			m_nX = initX;
			if (hidden)
				m_nY = initY - m_nHeight;
			else
				m_nY = initY;
			break;
		case CScrollingWindow::RIGHT:
			m_nHeight = (int) (desc->Height * yPercent); 
			m_nWidth = (int) (desc->Width * xPercent);
			initX = desc->Width - m_nWidth; 
			initY = 0;
			if (hidden)
				m_nX = initX + m_nWidth;
			else
				m_nX = initX;
			m_nY = initY;
			break;
		case CScrollingWindow::BOTTOM:
			m_nHeight = (int) (desc->Height * yPercent);
			m_nWidth = (int) (desc->Width * xPercent);
			initX = 0;
			initY = desc->Height - m_nHeight;
			m_nX = initX;
			if (hidden)
				m_nY = initY + m_nHeight;
			else
				m_nY = initY;
			break;
		case CScrollingWindow::LEFT:
			m_nHeight = (int) (desc->Height * yPercent); 
			m_nWidth = (int) (desc->Width * xPercent);
			initX = 0; 
			initY = 0;
			if (hidden)
				m_nX = initX - m_nWidth;
			else
				m_nX = initX;
			m_nY = initY;	
			break;
		case CScrollingWindow::NONE: // default this to the center of the screen
			m_nHeight = (int)(desc->Height * yPercent);
			m_nWidth = (int)(desc->Width * xPercent);
			initX = (desc->Width - m_nWidth) / 2;
			initY = (desc->Height - m_nHeight) / 2;
			m_nX = initX;
			if (hidden)
				m_nY = desc->Height;
			else 
				m_nY = initY;	
			break;
	}
	animType = AnimOb::NONE;
}
void CScrollingWindow::OnLostDevice(){
	if (m_Sprite)
		m_Sprite->OnLostDevice();

	if (m_font)
		m_font->OnLostDevice();
}
void CScrollingWindow::OnResetDevice(){
	if (m_Sprite)
		m_Sprite->OnResetDevice();
	if (m_font)
		m_font->OnResetDevice();

	UpdateDimensions();
}
