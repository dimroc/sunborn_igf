#include "dxstdafx.h"
#include "GameSettings.h"
#include "GameTables.h"


#define             SHIFT_ASCII_KEY             16
#define             CTRL_ASCII_KEY              17
#define             ALT_ASCII_KEY               18


CGameSettings::CGameSettings() :
m_cLastModifierKey(0), m_cLastNonModifierKey(0),
m_nKeysDown(0)
{
	ClearControls();
	ClearSettings();

	m_strPlayerAlias.assign("badinfluence pwns me!");   // read from settings file later.

	ReadControls(CURRENT_CONTROLS_FILE);
	ReadSettings(CURRENT_SETTINGS_FILE);
}

CGameSettings::~CGameSettings(void)
{
}

// Loads the .cfg file and populates the arrays of CommandPtr with the appropriate function.
int CGameSettings::ReadControls(LPCSTR settingsFN)
{
	using std::ifstream;
	m_strGameSettingsFile.assign(settingsFN);

	char dir[1000];
	_getcwd(dir, 999);
	_chdir("..\\Media");

	ifstream fin(m_strGameSettingsFile.c_str());

	if(!fin.good()) {
		_chdir(dir);
		fin.close();
		return EXIT_FAILURE;
	}

	char dels[] = " \t\n=";
	char buf[256];
	char *pos;
	while(fin.good())
	{   // read in settings and match up with GameTable::ComandIndexArray
		fin.getline(buf, 255);

		char *token = strtok_s(buf, dels, &pos);
		if(token == NULL || token[0] == '#')            
			continue;   // skip blank lines

		string cmd(token);
		UCHAR cmdIdx = FindActionIndex(cmd);
		if(cmdIdx > GameTable::NO_ACTION)
		{   // assign key to this command

			//
			// Get all tokens in line
			///            
			vector<string> vTokens;
			do
			{
				token = strtok_s(NULL, dels, &pos);
				if(token != NULL)   vTokens.push_back(token);
			} while(token != NULL);

			//
			// use this char, no modifier if only one other token.
			//
			if(vTokens.size() == 1)
			{ 
				UCHAR c = ConvertKeyToVKeyChar(vTokens[0]); // in case user passes in 'space' etc.                                
				m_cKeyBoard[c] = cmdIdx;
			}

			else if(vTokens.size() == 2)
			{
				// make modifier uppercase.
				std::transform(vTokens[0].begin(), vTokens[0].end(), vTokens[0].begin(), toupper);
				UCHAR c = ConvertKeyToVKeyChar(vTokens[1]); // in case user passes in 'space' etc.

				if(vTokens[0].compare("ALT") == 0)
					m_cAltKeyBoard[c] = cmdIdx;
				else if(vTokens[0].compare("CTRL") == 0)
					m_cCtrlKeyBoard[c] = cmdIdx;
				else if(vTokens[0].compare("SHIFT") == 0)
					m_cShiftKeyBoard[c] = cmdIdx;

				else if(vTokens[0].compare("MOUSE") == 0)
				{
					if( isdigit(vTokens[1].at(0)) )
					{
						int mi = vTokens[1].at(0) - 48; // converts ascii # to actual #.
						if(mi > 0 && mi < NUM_MOUSE_BUTTONS + 1)
							m_cMouse[mi-1] = cmdIdx;
					}
				}
			}
		}        
	}
	fin.close();
	_chdir(dir);

	UpdateControlMapping();

	return EXIT_SUCCESS;
}

int CGameSettings::ReadSettings(LPCSTR settingsFN)
{   
	using std::ifstream;
	m_strHostSettingsFile.assign(settingsFN);

	char dir[1000];	// change to media folder.
	_getcwd(dir, 999);
	_chdir("..\\Media");

	ifstream fin(m_strHostSettingsFile.c_str());

	char dels[] = " \t\n=";
	char buf[256];
	char *pos;
	while(fin.good())
	{   // read in settings and match up with GameTable::ComandIndexArray
		fin.getline(buf, 255);

		char *token = strtok_s(buf, dels, &pos);
		if(token == NULL || token[0] == '#')            
			continue;   // skip blank lines

		else if(strncmp(token, "alias", 100) == 0)
		{   // assign player alias
			char *aliasDels = "\"'\n";
			token = strtok_s(NULL, aliasDels, &pos);    // gather the middle rubbish bit (whitespace, =, etc).            
			token = strtok_s(NULL, aliasDels, &pos);    // get name
			m_strPlayerAlias.assign(token);
			continue;
		}

		string setting(token);
		UCHAR setIdx = FindSettingIndex(setting);
		if(setIdx > GameTable::NO_SETTING)
		{            
			token = strtok_s(NULL, dels, &pos);			
			m_hmSettings[setIdx] = (float)atof(token);
		}		
	}

	// close file, return to .exe directory, exit success.
	fin.close();
	_chdir(dir);
	return EXIT_SUCCESS;
}

VOID CGameSettings::RevertGameToDefault()
{
	// ClearControls();
	ReadControls(DEFAULT_CONTROLS_FILE);
}

VOID CGameSettings::RevertSettingsToDefault()
{    
	ReadSettings(DEFAULT_SETTINGS_FILE);    
}

VOID CGameSettings::UpdateControlMapping()
{
	for(int i = 0; i < 256; i++)
	{
		if(m_cKeyBoard[i] != GameTable::NO_ACTION)
			m_hmControls[m_cKeyBoard[i]] = SKeyCombo((UCHAR)i, false, false, false);
		if(m_cShiftKeyBoard[i] != GameTable::NO_ACTION)
			m_hmControls[m_cShiftKeyBoard[i]] = SKeyCombo((UCHAR)i, true, false, false);
		if(m_cCtrlKeyBoard[i] != GameTable::NO_ACTION)
			m_hmControls[m_cCtrlKeyBoard[i]] = SKeyCombo((UCHAR)i, false, true, false);
		if(m_cAltKeyBoard[i] != GameTable::NO_ACTION)
			m_hmControls[m_cAltKeyBoard[i]] = SKeyCombo((UCHAR)i, false, false, true);
	}
}

// ------------------------------------------------------------------
// ASS LOADS OF SETTERS/GETTERS
// ------------------------------------------------------------------

UCHAR CGameSettings::GetKeyboardMappedFunction(UINT nKey, bool bKeyDown)
{
	UCHAR c = GameTable::NO_ACTION;
	if(bKeyDown && !m_bKeyboard[nKey]) 
	{   // key down for first time
		m_nKeysDown++;
		m_bKeyboard[nKey] = bKeyDown;

		if(nKey == SHIFT_ASCII_KEY || nKey == CTRL_ASCII_KEY || nKey == ALT_ASCII_KEY)
			m_cLastModifierKey = (UCHAR)nKey;
		else
			m_cLastNonModifierKey = (UCHAR)nKey; 

		if(m_nKeysDown == 1)
			c = GetVanillaKeyboard(nKey);    // no need to do combo just return this key.        
		else
		{   // check for combo (modifier + key)
			if( m_cLastModifierKey == SHIFT_ASCII_KEY )
				c = GetShiftKeyboard(m_cLastNonModifierKey);
			else if( m_cLastModifierKey == CTRL_ASCII_KEY)
				c = GetCtrlKeyboard(m_cLastNonModifierKey);
			else if( m_cLastModifierKey == ALT_ASCII_KEY)
				c = GetAltKeyboard(m_cLastNonModifierKey);
			if(c == GameTable::NO_ACTION)
				c = GetVanillaKeyboard(nKey);
		}
	}
	else if(!bKeyDown && m_bKeyboard[nKey]) 
	{   // key up                
		m_bKeyboard[nKey] = bKeyDown;

		if(m_nKeysDown > 1)
		{   // release combo
			if( m_cLastModifierKey == SHIFT_ASCII_KEY )
				c = GetShiftKeyboard(m_cLastNonModifierKey);
			else if( m_cLastModifierKey == CTRL_ASCII_KEY)
				c = GetCtrlKeyboard(m_cLastNonModifierKey);
			else if( m_cLastModifierKey == ALT_ASCII_KEY)
				c = GetAltKeyboard(m_cLastNonModifierKey);
			if(c == GameTable::NO_ACTION)
				c = GetVanillaKeyboard(nKey);
		}
		else
		{
			c = GetVanillaKeyboard(nKey);    // no need to do combo just return this key.        
		}

		// Reset m_cLastModifierKey and m_cLastNonModifierKey if nothing's held down.
		if(nKey == SHIFT_ASCII_KEY || nKey == CTRL_ASCII_KEY || nKey == ALT_ASCII_KEY)
			m_cLastModifierKey = 0;
		else
			m_cLastNonModifierKey = 0;

		m_nKeysDown--;
	}
	else
		c = GameTable::NO_ACTION; 

	assert(m_nKeysDown >= 0);
	return c;
}

UCHAR CGameSettings::GetMouseMappedFunction(UINT nMouseButton, bool bKeyDown)
{
	assert(nMouseButton < NUM_MOUSE_BUTTONS);
	if(nMouseButton >= NUM_MOUSE_BUTTONS)
		return GameTable::NO_ACTION;

	if(m_bMouse[nMouseButton] == bKeyDown)
	{   // action already initiated, ie key already down.
		return GameTable::NO_ACTION;        
	}
	else
	{
		m_bMouse[nMouseButton] = bKeyDown;
		return m_cMouse[nMouseButton];
	}
}

// ------------------------------------------------------------------
// PRIVATE SHIZNIT
// ------------------------------------------------------------------

VOID CGameSettings::ClearControls() 
{
	memset(m_cKeyBoard, GameTable::NO_ACTION, sizeof(UCHAR) * 256);
	memset(m_cShiftKeyBoard, GameTable::NO_ACTION, sizeof(UCHAR) * 256);
	memset(m_cCtrlKeyBoard, GameTable::NO_ACTION, sizeof(UCHAR) * 256);
	memset(m_cAltKeyBoard, GameTable::NO_ACTION, sizeof(UCHAR) * 256);
	memset(m_cMouse, GameTable::NO_ACTION, sizeof(UCHAR) * NUM_MOUSE_BUTTONS);

	memset(m_bKeyboard, false, sizeof(bool)*256);
	memset(m_bMouse, false, sizeof(bool)*NUM_MOUSE_BUTTONS);

	m_hmControls.clear();
	//for (UCHAR i = 1; i < GameTable::NUM_ACTIONS; i++)
	//	m_hmControls[i] = SKeyCombo();
}

VOID CGameSettings::ClearSettings()
{
	m_hmSettings.clear();
	RevertSettingsToDefault();	
}

CGameSettings::SKeyCombo::SKeyCombo(UCHAR _cKey, bool _bShift, bool _bCtrl, bool _bAlt)
{
	cKey = _cKey;
	bShift = _bShift;
	bCtrl = _bCtrl;
	bAlt = _bAlt;
}