#pragma once

class CMenu;

#include "GameDefine.h"
#include "ScrollingWindow.h"
#include "GameGUI.h"

class CButtonGroup; 

using std::vector;
using std::string;

class CMenu : public CScrollingWindow {
public: 
	typedef void (*ButtonCallback)(void);
	
private:
	vector<CButtonGroup*>		m_vButtonGroups;		// Vector of buttons
	int							m_nActiveGroup;
	LPDIRECT3DTEXTURE9			m_LogoTexture;		// our texture

	typedef void (CGameGUI::* GUICallback)(UCHAR, string&);
	GUICallback					m_pFunc;		// pointer to command function
	CGameGUI*					m_pGUI;			// pointer to GUI wrapper object


public:
	// Constructor: defaults all integers to -1 and all pointers to NULL
	CMenu(LPDIRECT3DDEVICE9 pd3dDevice, WindowAnchor _anchor, float _xpercent, float _ypercent, CGameGUI* _gui, GUICallback _func);
	~CMenu();
	
	//Activates all Buttons with menu index 'index'
	//All Buttons not in this submenu will become inactive
	bool ActivateSubMenu(int index);

	//Adds a new Button
	// mIndex - sub menu index  Note: all Buttons in the same submenu will be displayed in the order they were added
	// text - Button text
	// cmd - Command that this Button refers to (should match up to GameTable::CommandIndexArray)
	// param - extra parameter that might be needed for certain commands
	void AddButton(int mIndex, LPCSTR text, UCHAR cmd, int param = -1);

	//Animates the menu, Should be called in the OnFrameMove function
	void Animate();
	
	// Sends the command up to the GUI object
	void ButtonClick(UCHAR c, int param);

	//Checks if the coordinates of a click coincide with any of the Menu Buttons
	//Clicks are not accepted while the menu is animating
	//Returns true if the mouse click inside the bounds of menu
	bool MouseProc(bool bLeftButtonDown, int x, int y, bool hadFocus);
	
	//Releases any memory used by the menu
	void CleanUp();
	
	// Creates a new group and passes the pointer up
	CButtonGroup* AddNewButtonGroup();

	//Draws the menu to the screen
	void OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, bool bHasFocus);
	void OnLostDevice();
	void OnResetDevice();
	
	//Sets the function that the menu calls when a Button is clicked
	void SetCallback(GUICallback funcPtr);

	void SetSpeed(int s);

	//Sets up the menu
	// anchor - which border the menu is docked on
	// percent - what percentage of the screen the menu should take up
	void SetupMenu(LPDIRECT3DDEVICE9 pd3dDevice, WindowAnchor _anchor, float _xpercent, float _ypercent);


	//Hides or shows the menu
	//Menu cannot be toggled while a button animation is in progress
	void ToggleMenu();

private:
	//Calculates the values of initX, initY, height, and width
	//based on dimensions of the BackBuffer and the percent value
	void UpdateDimensions();
	void UpdateButtonGroupDimensions();
};
