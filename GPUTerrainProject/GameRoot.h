#pragma once

#include "World.h"

class CGameEngine;
class CGameGUI;
class CNetworkRoot;
class Cmp3player;

using std::string;
using std::vector;


/*!
    The top level of all heirarchies. Uses CWorld to load graphical level from file.

    GRAPHICS
    -   GUI
    -   Has a list of islands (pillared quadtree mipmapped terrain with bubbles).
    -   Takes care of the world's sky and sun.
    -   Loads all the clouds and populates the island quadtree with them.
    -   Has a limbo list for objects not in the quad tree of an island.
    -   Deals with the player list.

    ENGINE
    -   Drives the Game
    -   Deals with Physics
    -   Handles interpolation, and connection to host. (single player local host connection)
*/

class CGameRoot
{
private:
    LPDIRECT3DDEVICE9                   m_pd3dDevice;   //!   the d3d device    
    LPDIRECT3DSTATEBLOCK9               m_pVanillaStateBlock;   //! original state block that d3d starts in.

    //! World to load OnCreateDevice. stored incase of 3d Environment RECREATE.
    string                              m_strWorldFile, m_strModelFile, m_strCharacterFile, m_strSpriteFile, m_strMasterServersFile; 

	CGameGUI*							m_pGUI;		//! GUI wrapper object (contains menu and console)
    CWorld*                             m_pWorld;   //! Top graphical layer
    CGameEngine*                        m_pEngine;  //! Listens to player, drives character, and handles collision events.    
    CNetworkRoot*                       m_pNetworkRoot; //! Top network layer that finds internet and lan servers.
    Cmp3player*                         m_pMP3Player;   //! umm for music, duh.    
    
    bool                                m_bControlCamera;   //! whether to turn camera control on or off.    
    bool                                m_bRenderPhysics;

private:
    VOID SpawnCharacter(UCHAR cCharType);  // Spawns a PC in the graphical and game engine world.
    VOID UpdateWorld(); //! Updates the graphical CWorld with the GameEngine positions
	VOID PopulateCommandHashMap();
    VOID CleanCharsFromWorld();

public:
    CGameRoot(void);
    ~CGameRoot(void);

#if 0   // Needed to properly load a CWorld if not done before d3ddevice created.
    if(!bFirstRun) 
    {  //! If it's not first run, recreate and reset everything on new map.
        V( g_pGameRoot->OnCreateDevice(DXUTGetD3DDevice()) );
        V( DXUTReset3DEnvironment() );
    }
#endif
    VOID AssignWorldFile(LPCSTR levelFN) { m_strWorldFile.assign(levelFN); }
    VOID AssignModelSetFile(LPCSTR modelFN) { m_strModelFile.assign(modelFN); }    
    VOID AssignCharacterSetFile(LPCSTR charFN) { m_strCharacterFile.assign(charFN); }
    VOID AssignSpriteAnimationSetFile(LPCSTR spriteFN) { m_strSpriteFile.assign(spriteFN); }

    VOID LoadMasterServersFile(const string &mastersFN);

    // --------------------------------------------------------------------------------------
    //! Framework function calls.
    // --------------------------------------------------------------------------------------
    HRESULT OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice);
    VOID OnDestroyDevice();

    HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice);
    HRESULT OnLostDevice();

    bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    //! Keys sent to Game engine, a few keys used to test rendering are sent to CWorld.
    void KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown );  
    void MouseProc(bool bLeftButtonDown, 
                   bool bRightButtonDown, 
                   bool bMiddleButtonDown, 
                   bool bSideButton1Down, 
                   bool bSideButton2Down, 
                   int nMouseWheelDelta, 
                   int xPos, int yPos);

    VOID OnFrameMove( double fTime, float fElapsedTime);
    VOID OnFrameRender();

	
	//
    // DEBUG STUFF
    //
    VOID ToggleCameraControl(const string& s) { m_bControlCamera = !m_bControlCamera; m_pWorld->SetCameraControl(m_bControlCamera); }
    VOID TogglePhysicsRender(const string& s) { m_bRenderPhysics = !m_bRenderPhysics; }
    VOID ToggleBBRender(const string& s) { m_pWorld->ToggleDrawBoundingBoxes(); }
    VOID SunTest(const string& s) { m_pWorld->SunTest(0); }
    VOID SunStop(const string& s) { m_pWorld->SunStop(); }
    VOID SaveShadowMap(const string& s) { m_pWorld->SaveShadowMap(); }
	VOID CyclePlayerAnimation(const string& s) { m_pWorld->CycleCharactersAnimation(); }    

    VOID SetCameraControl(bool b) { m_bControlCamera = b; m_pWorld->SetCameraControl(b); }

private:
	//------------------
	//! CALLBACK FUNCTIONS
	//------------------
	VOID DefaultFunc(const string& s);
	VOID ConsoleCmdFunc(const string& s);
	VOID HelpCmdFunc(const string& s);
	VOID MenuCmdFunc(const string& s);
	VOID ScoreboardCmdFunc(const string& s);
	VOID OptionsCmdFunc(const string& s);
	VOID SubMenuCmdFunc(const string& s);
	VOID QuitCmdFunc(const string& s);
	VOID SetCmdFunc(const string& s);


    VOID RefreshServerBrowser(const string& s);
    VOID RefreshLANBrowser(const string &s);
	VOID ToggleServerBrowser(const string& s);
	VOID ConnectCmdFunc(const string& s);
    VOID HostCmdFunc(const string& s);
    VOID DisconnectCmdFunc(const string& s);
	VOID SayCmdFunc(const string& s);

};
