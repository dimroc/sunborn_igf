#pragma once

#include "GameRoot.h"
#include "GameTables.h"

#define             NUM_MOUSE_BUTTONS					5

#define             DEFAULT_CONTROLS_FILE               "defaultcontrols.cfg"
#define             CURRENT_CONTROLS_FILE               "controls.cfg"

#define             DEFAULT_SETTINGS_FILE               "defaultsettings.cfg"
#define             CURRENT_SETTINGS_FILE               "settings.cfg"

using std::string;
using stdext::hash_map;

using namespace GameTable;

//! Holds game / server variables such as player acceleration, gravity, etc.
class CGameSettings
{
public:
	struct SKeyCombo
	{
		UCHAR cKey;
		bool bShift, bCtrl, bAlt;
		SKeyCombo() : cKey(0), bShift(false), bCtrl(false), bAlt(false) {}
		SKeyCombo(UCHAR _cKey, bool _bShift, bool _bCtrl, bool _bAlt);
	};
private:
    string m_strGameSettingsFile, m_strHostSettingsFile;
    string m_strPlayerAlias;

    // ------------------------------------------------------------------------
    // HOST VARIABLES
    // ------------------------------------------------------------------------
	typedef hash_map<UCHAR, float> SettingsHashMap;
	SettingsHashMap m_hmSettings;	// uses the enums in GameTable.h as keys to the setting.

    // ------------------------------------------------------------------------
    // KEYBOARD SETTINGS
    // ------------------------------------------------------------------------    

    //! keep count of keys down in case u need to do combo 
    //  (if user hits in wrong order to move anyway: w then shift).
    int m_nKeysDown;    
    bool m_bKeyboard[256];  //! Entry is true if the key is down.        
    UCHAR m_cLastModifierKey, m_cLastNonModifierKey;

    // Mapped Functions.
    UCHAR m_cKeyBoard[256];    
    UCHAR m_cShiftKeyBoard[256];
    UCHAR m_cCtrlKeyBoard[256];
    UCHAR m_cAltKeyBoard[256];

	// Store what commands are triggered by what keys for lookup purposes.
	typedef hash_map<UCHAR, SKeyCombo> KeyboardMappingHashMap;
	KeyboardMappingHashMap m_hmControls;	//! stores the keys to trigger a command

    // ------------------------------------------------------------------------
    // MOUSE SETTINGS
    // ------------------------------------------------------------------------    
    bool m_bMouse[NUM_MOUSE_BUTTONS];   //! Entry is true if the key is down.        
    UCHAR m_cMouse[NUM_MOUSE_BUTTONS]; //! 3 mouse buttons!

private:
	VOID UpdateControlMapping();

    // --------------------------------
    // SETTERS/GETTERS FOR KEYS/MOUSE
    // --------------------------------

    UCHAR GetVanillaKeyboard(UINT i)                    { assert(i < 256); return m_cKeyBoard[i]; }        
    UCHAR GetShiftKeyboard(UINT i)                      { assert(i < 256); return m_cShiftKeyBoard[i]; }        
    UCHAR GetCtrlKeyboard(UINT i)                       { assert(i < 256); return m_cCtrlKeyBoard[i]; }        
    UCHAR GetAltKeyboard(UINT i)                        { assert(i < 256); return m_cAltKeyBoard[i]; }
    UCHAR GetMouse(UINT i)                              { assert(i < NUM_MOUSE_BUTTONS); return m_cMouse[i]; } 


public:
    CGameSettings(void);    // Loads default settings in constructor.
    ~CGameSettings(void);

    VOID RevertGameToDefault();  //! Loads default controls.
	VOID RevertSettingsToDefault();

    //! Reads the users game settings (controls, etc).
    int ReadControls(LPCSTR settingsFN);
    // int WriteControls(LPCSTR settingsFN);	// TODO
    VOID ClearControls(); // sets all listeners to 0 (no action).

    //! Reads the settings file that describe the game world (player accel, etc).
    int ReadSettings(LPCSTR settingsFN);    
    // int WriteSettings(LPCSTR settingsFN);    // TODO
    VOID ClearSettings();

	void ChangeSetting(UCHAR id, float fValue)			{ assert(id > 0); m_hmSettings[id] = fValue; }
    // --------------------------------
    // GETTERS FOR MAPPED FUNCTIONS
    // --------------------------------

    UCHAR GetKeyboardMappedFunction(UINT nKey, bool bKeyDown); //! Returns the proper UCHAR index for the mapped function.    
    UCHAR GetMouseMappedFunction(UINT nMouseButton, bool bKeyDown); //! Returns the mouse action
	SKeyCombo GetControlMappedKeys(UCHAR cAction)		{ return m_hmControls[cAction]; }
	float GetSetting(UCHAR cStg)						{ return m_hmSettings[cStg]; }

	// --------------------------------
    // GETTERS FOR PLAYER VARIABLES
    // --------------------------------

    LPCSTR GetPlayerAlias()                             { return m_strPlayerAlias.c_str(); }  //! returns player name

	float GetPlayerRunSpeed()							{ return m_hmSettings[RUN_SETTING]; }
//    float GetPlayerBackpedalSpeed() 					{ return m_hmSettings[BACKPEDAL_SETTING]; }
//   float GetPlayerTurnSpeed()                          { return m_hmSettings[TURN_SETTING]; }
	float GetPlayerDashSpeed()							{ return m_hmSettings[DASH_SETTING]; }
	float GetPlayerJumpAccel()							{ return m_hmSettings[JUMP_ACCEL_SETTING]; }	
	int   GetPlayerJumpCount()							{ return (int) m_hmSettings[JUMP_COUNT_SETTING]; }	
	float GetPlayerDodgeSpeed()							{ return m_hmSettings[DODGE_SPEED_SETTING]; }	

	int   GetPlayerStartLives()							{ return (int) m_hmSettings[START_LIVES_SETTING]; }

    float GetPlayerProjectileSpeed()					{ return m_hmSettings[PROJ_SPEED_SETTING]; }
    float GetPlayerProjectileMaxIncr()					{ return m_hmSettings[PROJ_MAX_INCR_SETTING]; }

    // --------------------------------
    // GETTERS FOR HOST VARIABLES
    // --------------------------------
    float GetGravMult()							        { return m_hmSettings[GRAV_MULT_SETTING]; }
    float GetMass()							            { return m_hmSettings[MASS_SETTING]; }
    float GetFriction()							        { return m_hmSettings[FRICTION_SETTING]; }
    float GetBounce()							        { return m_hmSettings[BOUNCE_SETTING]; }   

    // --------------------------------
    // GETTERS FOR CAMERA VARIABLES
    // --------------------------------
    float GetCameraOffsetX()                            { return m_hmSettings[CAMERA_OFFSET_X_SETTING]; }
    float GetCameraOffsetY()                            { return m_hmSettings[CAMERA_OFFSET_Y_SETTING]; }
    float GetCameraOffsetZ()                            { return m_hmSettings[CAMERA_OFFSET_Z_SETTING]; }

	float GetCameraLookAtOffsetX()                      { return m_hmSettings[CAMERA_LOOKAT_OFFSET_X_SETTING]; }
    float GetCameraLookAtOffsetY()                      { return m_hmSettings[CAMERA_LOOKAT_OFFSET_Y_SETTING]; }
    float GetCameraLookAtOffsetZ()                      { return m_hmSettings[CAMERA_LOOKAT_OFFSET_Z_SETTING]; }

    float GetCameraSpringK()                            { return m_hmSettings[CAMERA_SPRING_K_SETTING]; }    
    float GetCameraSpringLookK()                        { return m_hmSettings[CAMERA_SPRING_LOOK_K_SETTING]; }    

    // --------------------------------
    // GETTERS FOR GAME VARIABLES
    // --------------------------------

	float GetRoundLength()	    						{ return m_hmSettings[ROUND_LENGTH_SETTING]; }
	float GetTimeBetweenRounds()						{ return m_hmSettings[ROUND_TIME_BETWEEN_SETTING]; }

    // --------------------------------
    // SETTERS ----- FOR GAME VARIABLES
    // --------------------------------

    void SetPlayerAlias(LPCSTR pszAlias)                { assert(pszAlias); m_strPlayerAlias.assign(pszAlias); }

	void SetPlayerRunSpeed(float f)						{ m_hmSettings[RUN_SETTING] = f; }
//    void SetPlayerBackpedalSpeed(float f) 				{ m_hmSettings[BACKPEDAL_SETTING] = f; }
//    void SetPlayerTurnSpeed(float f)                    { m_hmSettings[TURN_SETTING] = f; }
	void SetPlayerDashSpeed(float f)					{ m_hmSettings[DASH_SETTING] = f; }
	void SetPlayerJumpAccel(float f)					{ m_hmSettings[JUMP_ACCEL_SETTING] = f; }	
	void SetPlayerJumpCount(int i)						{ m_hmSettings[JUMP_COUNT_SETTING] = (float)i; }	
	void SetPlayerDodgeSpeed(float f)					{ m_hmSettings[DODGE_SPEED_SETTING] = f; }	

	void SetPlayerStartLives(int i)						{ m_hmSettings[START_LIVES_SETTING] = (float)i; }

    void SetPlayerProjectileSpeed(float f)				{ m_hmSettings[PROJ_SPEED_SETTING] = f; }
    void SetPlayerProjectileMaxIncr(float f)			{ m_hmSettings[PROJ_MAX_INCR_SETTING] = f; }

    // --------------------------------
    // SETTERS FOR HOST VARIABLES
    // --------------------------------
    void SetGravMult(float f)							{ m_hmSettings[GRAV_MULT_SETTING] = f; }
    void SetMass(float f)							    { m_hmSettings[MASS_SETTING] = f; }
    void SetFriction(float f)							{ m_hmSettings[FRICTION_SETTING] = f; }
    void SetBounce(float f)							    { m_hmSettings[BOUNCE_SETTING] = f; }

    // --------------------------------
    // SETTERS FOR GAME VARIABLES
    // --------------------------------

	void SetRoundLength(float f)						{ m_hmSettings[ROUND_LENGTH_SETTING] = f; }
	void SetTimeBetweenRounds(float f)					{ m_hmSettings[ROUND_TIME_BETWEEN_SETTING] = f; }

};
