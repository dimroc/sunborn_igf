#include "dxstdafx.h"
#include "HUD.h"
#include "GameDefine.h"

CHud::CHud(LPDIRECT3DDEVICE9 pd3dDevice) : CScrollingWindow(0x00000000), alpha(255), pMyPlayer(NULL), m_BigFont(NULL), m_SmallFont(NULL), timeLeft(FADETIME) {
	HRESULT hr;
	// Create a D3DX font object
	V(D3DXCreateFont( pd3dDevice, 40, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		TEXT("Arial"), &m_BigFont ) );

	V(D3DXCreateFont( pd3dDevice, 24, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		TEXT("Arial"), &m_SmallFont ) );

	// Create sprite object
	V( D3DXCreateSprite(pd3dDevice,&m_Sprite) );

	// Load a texture for our sprite
	V( D3DXCreateTextureFromFile(pd3dDevice, LIVE_ICON, &m_Texture) );

	if (hr != S_OK)
		dprintf("Something went wrong in the HUD!!");
	this->anchor = CScrollingWindow::TOP;
	this->hidden = false;
	this->xPercent = 1.f;
	this->yPercent = 1.f;
	UpdateDimensions();
}
CHud::~CHud() { 
	pMyPlayer = NULL;
	SAFE_RELEASE(m_BigFont);
	SAFE_RELEASE(m_SmallFont);
}
void CHud::OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, const PlayerHashMap &hmPlayers, const CGameEngine::SRoundInfo &roundInfo){ 
	if (pMyPlayer == NULL)
		return;

	if (alpha < 10) // don't waste my time with a low alpha
		return;

	// Draw player stuff
	// Divide up the width between each of the players
	int segmentWidth = m_nWidth / max(4, (int)hmPlayers.size());
	int segmentHeight = m_nHeight / 5;
	
	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);

	PlayerHashMap::const_iterator playerit = hmPlayers.begin();
	int index = 0;
	for( ; playerit != hmPlayers.end(); playerit++)	{
		const SPlayer* player = &playerit->second;
		if (player->charID == pMyPlayer->charID)
			DrawMyPlayerInfo(pd3dDevice);
		else
			DrawOtherPlayerInfo(pd3dDevice, player, segmentWidth, segmentHeight, index);
		index++;
	}

	// Draw Hud messages
	list<SHudMessage>::iterator hmIt = m_lMessages.begin();
	int x, y, width, height;
	x = m_nX;
	width = m_nWidth;
	y = m_nY + (m_nHeight / 4);
	height = 22;

	while(hmIt != m_lMessages.end()){
		LPD3DXFONT font;
		if (hmIt->bBigFont) {
			font = m_BigFont;
			height = 42;
		}
		else {
			font = m_SmallFont;
			height = 22;
		}
		DrawHUDText(font, hmIt->strText.c_str(), x, y, height, width, 
				DT_CENTER | DT_VCENTER, SColor(255,0,0), hmIt->fTimeLeft / hmIt->fTimeToLive);
		y += height;
		
		hmIt++;
	}

	// Draw timer
	float fTimer = roundInfo.fRoundTimeLeft;
	int minutes = (int)fTimer / 60, seconds = (int)fTimer % 60;
	std::stringstream ss;
	if (!roundInfo.bRoundStarted)
		ss << "Time Until Next Round:  ";
	else
		ss << "Round " << roundInfo.nRound << ":  ";
	ss << minutes << ":";
	if (seconds < 10)
		ss << "0";
	ss << seconds;
	DrawHUDText(m_SmallFont, ss.str().c_str(), m_nWidth / 2, 0, segmentHeight, m_nWidth / 2, DT_CENTER | DT_VCENTER, SColor(0, 0, 255));
	
	m_Sprite->End();
}
void CHud::OnFrameMove(float fElapsedTime){
	timeLeft -= fElapsedTime;
	if (timeLeft <= 0){
		timeLeft += FADETIME;
		alpha = alpha - (alpha / 10);
	}

	list<SHudMessage>::iterator hmIt = m_lMessages.begin();
	while(hmIt != m_lMessages.end()){
		hmIt->fTimeLeft -= fElapsedTime;
		if (hmIt->fTimeLeft <= 0){
			// remove this message from list
			hmIt = m_lMessages.erase(hmIt);
		}
		else
			hmIt++;
	}

}
void CHud::OnLostDevice(){ 
	CScrollingWindow::OnLostDevice();
	m_BigFont->OnLostDevice();
	m_SmallFont->OnLostDevice();
}
void CHud::OnResetDevice(){
	CScrollingWindow::OnResetDevice();
	m_BigFont->OnResetDevice();
	m_SmallFont->OnResetDevice();
}
void CHud::ResetAlpha() { 
	alpha = 255; 
	timeLeft = 5; 
}
void CHud::SetupHUD( LPDIRECT3DDEVICE9 pd3dDevice, const SPlayer* _myPlayer) {
	pMyPlayer = _myPlayer;
}
void CHud::ToggleHUD(){
	CScrollingWindow::Toggle();
}
void CHud::UpdateDimensions(){
	CScrollingWindow::UpdateDimensions();
}

//!------------------------------------------
//! Private functions for drawing GUIObjects
//!------------------------------------------

void CHud::DrawProgressBar(LPDIRECT3DDEVICE9 pd3dDevice, int x, int y, int height, int width, float percent, SColor c, bool bDrawBack){
	float left = (float) x, 
		right = (float) x + width,
		top = (float) y , 
		bottom = (float) y + height,
		fraction = (percent * width) + left;

	pd3dDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );

	// Draw darkened area
	DWORD color = D3DCOLOR_ARGB(alpha, c.r, c.g, c.b);
	SVertex bverts[] = 
	{
		{ left,		bottom,	0.0f,	1.0f,	color },
		{ left,		top,	0.0f,	1.0f,	color },
		{ fraction,	top,	0.0f,	1.0f,	color },
		{ fraction,	bottom,	0.0f,	1.0f,	color },
		{ left,		bottom,	0.0f,	1.0f,	color }
	};
	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, bverts, sizeof(SVertex));

	if (bDrawBack) {
		// Draw faded area
		color =  D3DCOLOR_ARGB(alpha / 3, c.r, c.g, c.b);
		SVertex cverts[] = 
		{
			{ fraction,		bottom,	0.0f,	1.0f,	color },
			{ fraction,		top,	0.0f,	1.0f,	color },
			{ right,		top,	0.0f,	1.0f,	color },
			{ right,		bottom,	0.0f,	1.0f,	color },
			{ fraction,		bottom,	0.0f,	1.0f,	color }
		};
		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cverts, sizeof(SVertex));
	}
	// Draw border
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
void CHud::DrawHUDText(LPD3DXFONT font, LPCSTR text, int x, int y, int height, int width, DWORD format, SColor c, float alphaPercent){
	if (!font)
		return;
	RECT rct;
	rct.top = y;// + height / 4;
	rct.bottom = y + height; //3 * height / 4;
	rct.left = x;
	rct.right = x + width;
	font->DrawTextA(NULL, text, -1, &rct, format, D3DCOLOR_ARGB((int)(alpha * alphaPercent), c.r, c.g, c.b) );
}

// Draws the icons, does NOT perform a Sprite::Begin or Sprite::End, must be called from calling function
void CHud::DrawLiveIcons(int nLives, int x, int y, int segmentWidth){
	if (nLives <= 0)
		return; 

	D3DSURFACE_DESC pDesc;
	m_Texture->GetLevelDesc(0, &pDesc);
	int num = (segmentWidth * 3 / 4) / (int) pDesc.Width; // how many icons can we fit in this space
	float offset = (float) ((segmentWidth * 3 / 4) % (int) pDesc.Width) / 2; // how much space is left over (for centering)
	D3DXVECTOR3 vPos;
	D3DXMATRIX mx, mxScale, mxTrans;
	D3DXMatrixIdentity(&mx);
	D3DXMatrixScaling(&mxScale, 1.f, 1.f, 1.f);

	vPos.x = x + offset;
	vPos.y = (float) y;
	vPos.z = 0.f;
	for (int l = 0; l < min(num, nLives); l++){
		D3DXMatrixTranslation(&mxTrans, vPos.x, vPos.y, vPos.z);
		mx = mxScale * mxTrans;
		m_Sprite->SetTransform(&mx);
		m_Sprite->Draw(m_Texture, NULL, NULL, NULL, D3DCOLOR_ARGB(alpha, 255, 255, 255));

		vPos.x += pDesc.Width;
	}
}
void CHud::DrawDamageBar(LPDIRECT3DDEVICE9 pd3dDevice, const SPlayer* player, int x, int y, int segmentWidth){
	float dmg = player->fCurrDamage;
	int width = segmentWidth * 3 / 4;
	std::stringstream ss;

	if (player->nLives > 0){
		int div = (int) dmg / 100;
		for (int k = max(0,div-1); k <= div; k++){
			dmg = player->fCurrDamage - 100.f * k;
			bool bDrawBack = (div == 0);
			DrawProgressBar(pd3dDevice, x, y, 16, width, 
				min(dmg, 100.f) / 100.f,  // percent
				SColor(153,max(0,255 - (51 * k)),0), //color 
				bDrawBack);
		}
		ss << (int) player->fCurrDamage << "%";
	}
	else { // Dead
		DrawProgressBar(pd3dDevice, x, y, 16, width, 
			1.f,  // percent
			SColor(103, 103, 103), //color (gray) 
			true);
		ss << "DEAD";
	}

	// Write health amount over bar
	DrawHUDText(m_BigFont, ss.str().c_str(), x, y - 12,
		42, width, DT_CENTER | DT_VCENTER, SColor(255,255,255));
}
void CHud::DrawOtherPlayerInfo(LPDIRECT3DDEVICE9 pd3dDevice, const SPlayer* player, int segmentWidth, int segmentHeight, int i){
	int x = m_nX + segmentWidth / 8 + segmentWidth * i;
	int y = m_nY + m_nHeight - (segmentHeight * 2 / 3);

	// Draw live icons
	DrawLiveIcons(player->nLives, x, y + 18, segmentWidth);

	// Draw Damage bar
	DrawDamageBar(pd3dDevice, player, x, y, segmentWidth);
}
void CHud::DrawMyPlayerInfo(LPDIRECT3DDEVICE9 pd3dDevice){
	int segmentWidth = m_nWidth / 2;
	int segmentHeight = m_nHeight / 5;

	// Draw Damage Bar
	DrawDamageBar(pd3dDevice, pMyPlayer, 
		m_nX + segmentWidth / 8,	// x
		segmentHeight / 4,			// y
		segmentWidth);				// width

	// Draw Charge Bar
	std::stringstream ss;
	ss << "Charge";
	DrawProgressBar(pd3dDevice, 
		m_nX + segmentWidth / 8, segmentHeight / 2, 
		16, segmentWidth * 3 / 4, pMyPlayer->fCurrCharge / pMyPlayer->fMaxCharge, SColor(0,0,255) , true);
	DrawHUDText(m_BigFont, ss.str().c_str(), 
		m_nX + segmentWidth / 8, (segmentHeight / 2) - 12, 
		42, segmentWidth * 3 / 4, DT_CENTER | DT_VCENTER, SColor(255,255,255));

	// Draw Lives
	DrawLiveIcons(pMyPlayer->nLives,
		m_nX + (segmentWidth / 8),	// x
		m_nY + segmentHeight * 3 / 4,		// y
		segmentWidth);
}