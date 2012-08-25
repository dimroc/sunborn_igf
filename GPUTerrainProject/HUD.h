#pragma once

class CHud;

#include "dxstdafx.h"
#include "ScrollingWindow.h"
#include "ProgressBar.h"
#include "GameEngine.h"


#define FADETIME 0.1f

using std::list;

class CHud : CScrollingWindow
{
public:
	struct SHudMessage {
		string strText;
		// Current percentage of the HUD alpha that this text line is visible at - timeLeft / timeToLive
		float fTimeLeft; // Time before this messages gets erased
		float fTimeToLive;
		bool bBigFont;	// Use big or small font
	};

private:
	typedef CGameEngine::SPlayer SPlayer;
	typedef CGameEngine::PlayerHashMap PlayerHashMap;

	const SPlayer*					pMyPlayer;		//! not owned don't delete
	int								alpha;			//! Hud visibility
	LPD3DXFONT						m_BigFont;		//! Font for text
	LPD3DXFONT						m_SmallFont;	//! Smaller font for text

	float							timeLeft;		//! Time until next auto lowering of alpha
	list<SHudMessage>				m_lMessages;	//! vector of messages to get printed to middle of the screen

//! Private functions for drawing GUIObjects
private:
	void DrawProgressBar(LPDIRECT3DDEVICE9 pd3dDevice, int x, int y, int height, int width, float percent, SColor c, bool bDrawBack);
	void DrawHUDText(LPD3DXFONT font, LPCSTR text, int x, int y, int height, int width, DWORD format, SColor c, float alphaPercent = 1.f);
	void DrawLiveIcons(int nLives, int x, int y, int segmentWidth);
	void DrawDamageBar(LPDIRECT3DDEVICE9 pd3dDevice, const SPlayer* player, int x, int y, int segmentWidth);
	void DrawMyPlayerInfo(LPDIRECT3DDEVICE9 pd3dDevice);
	void DrawOtherPlayerInfo(LPDIRECT3DDEVICE9 pd3dDevice, const SPlayer* player, int segmentWidth, int segmentHeight, int i);

public:
	CHud(LPDIRECT3DDEVICE9 pd3dDevice);
	~CHud();
	void OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, const PlayerHashMap &hmPlayers, const CGameEngine::SRoundInfo &roundInfo);
	void OnFrameMove(float fElapsedTime);
	void OnLostDevice();
	void OnResetDevice();
	void ResetAlpha();
	void SetupHUD( LPDIRECT3DDEVICE9 pd3dDevice, const SPlayer* _myPlayer);
	void ToggleHUD();
	void UpdateDimensions();

	void AddHUDMessage(SHudMessage message){ 
		m_lMessages.push_back(message); 
		ResetAlpha();
	}
};