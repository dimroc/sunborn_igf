#pragma once

//! Holds constant information for:
//!     - Animation indices ( Run == 1, etc)
//!     - Command Indices ( MoveForward == 2 )
//!
//! Used by CGameSettings and CGameRoot to have consistent constants in one location.

using std::string;

namespace GameTable
{
	struct NameIndexPair 
	{	// Struct matching a string to a UCHAR (really an ENUM).
		LPCSTR str;
		UCHAR cmd;
		NameIndexPair(LPCSTR _str, UCHAR _cmd) : str(_str), cmd(_cmd) {}
	};

	// ----------------------------------------------------------------------------
	// Character Animations that are matched to their proper enum/UCHAR idx.
	// ----------------------------------------------------------------------------

	static enum Animations 
	{   
		ANIM_NO = 0,
		ANIM_IDLE,   // am i gonna have strafe?
		ANIM_IDLE_MIDAIR,
		ANIM_RUN,
		ANIM_BLOCK,

		ANIM_JUMP,
		ANIM_JUMP_START,
		ANIM_JUMP_MID,
		ANIM_JUMP_DOUBLE,
		ANIM_JUMP_END,

		ANIM_DIVE_START,
		ANIM_DIVE_MID,
		ANIM_DIVE_END,

		ANIM_DROP_START,
		ANIM_DROP_MID,
		ANIM_DROP_END,

		ANIM_FALL_START,
		ANIM_FALL_MID,
		ANIM_FALL_END,

		ANIM_HIT_START,
		ANIM_HIT_MID,
		ANIM_HIT_END,

		ANIM_FIREBALL_START,
		ANIM_FIREBALL_MID,
		ANIM_FIREBALL_END,

		ANIM_LIGHTNING_START,
		ANIM_LIGHTNING_MID,
		ANIM_LIGHTNING_END,

		ANIM_ATTACK_START,
		ANIM_ATTACK_MID,
		ANIM_ATTACK_END,

		ANIM_SPIN_START,
		ANIM_SPIN_MID,
		ANIM_SPIN_SPIN,
		ANIM_SPIN_END,

		ANIM_DODGE,
		ANIM_CELEBRATE,

		ANIM_NUM
	};    

	static const NameIndexPair AnimationIndexArray[] = 
	{   
		NameIndexPair( "idle",                   ANIM_IDLE ),
		NameIndexPair( "idle_midair",            ANIM_IDLE_MIDAIR ),

		NameIndexPair( "run",                    ANIM_RUN ),

		NameIndexPair( "jump",                   ANIM_JUMP ),
		NameIndexPair( "jump_double",            ANIM_JUMP_DOUBLE ),

		NameIndexPair( "dive_start",             ANIM_DIVE_START ),
		NameIndexPair( "dive_mid",               ANIM_DIVE_MID ),
		NameIndexPair( "dive_end",               ANIM_DIVE_END ),

		NameIndexPair( "drop_start",             ANIM_DROP_START ),
		NameIndexPair( "drop_mid",               ANIM_DROP_MID ),
		NameIndexPair( "drop_end",               ANIM_DROP_END ),

		NameIndexPair( "fall_start",             ANIM_FALL_START ),
		NameIndexPair( "fall_mid",               ANIM_FALL_MID ),
		NameIndexPair( "fall_end",               ANIM_FALL_END ),

		NameIndexPair( "hit_start",              ANIM_HIT_START ),
		NameIndexPair( "hit_mid",                ANIM_HIT_MID ),
		NameIndexPair( "hit_end",                ANIM_HIT_END ),

		NameIndexPair( "fireball_start",         ANIM_FIREBALL_START ),
		NameIndexPair( "fireball_mid",           ANIM_FIREBALL_MID ),
		NameIndexPair( "fireball_end",           ANIM_FIREBALL_END ),

		NameIndexPair( "lightning_start",        ANIM_LIGHTNING_START ),
		NameIndexPair( "lightning_mid",          ANIM_LIGHTNING_MID ),
		NameIndexPair( "lightning_end",          ANIM_LIGHTNING_END ),

		NameIndexPair( "attack_start",           ANIM_ATTACK_START ),
		NameIndexPair( "attack_mid",             ANIM_ATTACK_MID ),
		NameIndexPair( "attack_end",             ANIM_ATTACK_END ),

		NameIndexPair( "spin_start",             ANIM_SPIN_START ),
		NameIndexPair( "spin_mid",               ANIM_SPIN_MID ),
		NameIndexPair( "spin_spin",              ANIM_SPIN_SPIN ),
		NameIndexPair( "spin_end",               ANIM_SPIN_END ),

		NameIndexPair( "dodge",                  ANIM_DODGE ),

		NameIndexPair( "block",                  ANIM_BLOCK ),

		NameIndexPair( "celeb",            ANIM_CELEBRATE )
	};

	// ----------------------------------------------------------------------------
	// Sprite Textures for Short Life Entites
	// ----------------------------------------------------------------------------

	static enum SpriteAnims 
	{   
		NO_SPRITE = 0,
		RANGE_SPRITE,   // am i gonna have strafe?
		SMASH_SPRITE,
		LIGHTNING_SPRITE,
		SWEEP_SPRITE,
		NUM_SPRITES
	};    

	static const NameIndexPair SpriteIndexArray[] = 
	{   
		NameIndexPair( "range",                   RANGE_SPRITE ),
		NameIndexPair( "smash",                   SMASH_SPRITE ),
		NameIndexPair( "lightning",               LIGHTNING_SPRITE ),
		NameIndexPair( "sweep",                   SWEEP_SPRITE ),
	};

	// ----------------------------------------------------------------------------
	// The Player Actions index by ENUM and paired below in the Action Index Array
	// ----------------------------------------------------------------------------

	static enum Actions 
	{   // hardcoded to match static array below  
		NO_ACTION = 0,
		MOVE_FORWARD_ACTION,
		MOVE_BACKWARD_ACTION,
		MOVE_LEFT_ACTION,
		MOVE_RIGHT_ACTION,

		JUMP_ACTION,
		DESCEND_ACTION,
		BLOCK_ACTION,
		DASH_ACTION,

		ATTACK_SMASH_ACTION,
		ATTACK_VERTICAL_ONE_ACTION,
		ATTACK_VERTICAL_TWO_ACTION,
		ATTACK_RANGE_ACTION,

		CELEBRATE_ACTION,
		ROTATE_CAMERA_LEFT_ACTION,
		ROTATE_CAMERA_RIGHT_ACTION,
		TILT_CAMERA_ACTION,
		RESET_CAMERA_ACTION,

		REQUEST_RESPAWN,
		NUM_ACTIONS
	};

	static const NameIndexPair ActionIndexArray[] = 
	{   
		NameIndexPair( "move_forward",           MOVE_FORWARD_ACTION ),
		NameIndexPair( "move_backward",          MOVE_BACKWARD_ACTION ),
		NameIndexPair( "move_left",              MOVE_LEFT_ACTION ),
		NameIndexPair( "move_right",             MOVE_RIGHT_ACTION ),

		NameIndexPair( "jump",                   JUMP_ACTION ),
		NameIndexPair( "descend",                DESCEND_ACTION ),  
		NameIndexPair( "block",                  BLOCK_ACTION ),
		NameIndexPair( "dash",					 DASH_ACTION ),

		NameIndexPair( "attack_smash",           ATTACK_SMASH_ACTION ),
		NameIndexPair( "attack_vertical",        ATTACK_VERTICAL_ONE_ACTION ),
		NameIndexPair( "attack_spin",			 ATTACK_VERTICAL_TWO_ACTION ),
		NameIndexPair( "attack_range",           ATTACK_RANGE_ACTION ),

		NameIndexPair( "rotate_camera_left",     ROTATE_CAMERA_LEFT_ACTION),
		NameIndexPair( "rotate_camera_right",    ROTATE_CAMERA_RIGHT_ACTION),
		NameIndexPair( "tilt_camera",            TILT_CAMERA_ACTION),
		NameIndexPair( "reset_camera",           RESET_CAMERA_ACTION),

		NameIndexPair( "respawn",                REQUEST_RESPAWN),
		NameIndexPair( "celebrate",              CELEBRATE_ACTION )
	};

	// ----------------------------------------------------------------------------
	// The Server Settings indexed by ENUM and paired below in the Setting Index Array
	// ----------------------------------------------------------------------------

	static enum Settings 
	{   // hardcoded to match static array below  
		NO_SETTING = 0,
		RUN_SETTING,
		//BACKPEDAL_SETTING,
		DODGE_SPEED_SETTING,
		//TURN_SETTING,
		DASH_SETTING,
		JUMP_ACCEL_SETTING,	
		JUMP_COUNT_SETTING,
		START_LIVES_SETTING,

		GRAV_MULT_SETTING,
		MASS_SETTING,
		FRICTION_SETTING,
		BOUNCE_SETTING,

		PROJ_SPEED_SETTING,
		PROJ_MAX_INCR_SETTING,

		//camera
		CAMERA_OFFSET_X_SETTING,
		CAMERA_OFFSET_Y_SETTING,
		CAMERA_OFFSET_Z_SETTING,

		CAMERA_LOOKAT_OFFSET_X_SETTING,
		CAMERA_LOOKAT_OFFSET_Y_SETTING,
		CAMERA_LOOKAT_OFFSET_Z_SETTING,

		CAMERA_SPRING_K_SETTING,        
		CAMERA_SPRING_LOOK_K_SETTING,        

		//Game settings
		ROUND_LENGTH_SETTING,
		ROUND_TIME_BETWEEN_SETTING,

		NUM_SETTINGS
	};

	static const NameIndexPair SettingIndexArray[] = 
	{   
		NameIndexPair( "run_speed",							RUN_SETTING ),        
		NameIndexPair( "dodge_speed",						DODGE_SPEED_SETTING ),        
		NameIndexPair( "dash_speed",			            DASH_SETTING ),
		NameIndexPair( "jump_accel",			            JUMP_ACCEL_SETTING ),		
		NameIndexPair( "jump_count",			            JUMP_COUNT_SETTING ),		
		NameIndexPair( "start_lives",			            START_LIVES_SETTING ),

		NameIndexPair( "grav_mult",							GRAV_MULT_SETTING),
		NameIndexPair( "mass",							    MASS_SETTING),
		NameIndexPair( "friction",							FRICTION_SETTING),
		NameIndexPair( "bounce",							BOUNCE_SETTING),

		NameIndexPair( "projectile_speed",					PROJ_SPEED_SETTING),
		NameIndexPair( "projectile_max_incr",				PROJ_MAX_INCR_SETTING),        

		// camera
		NameIndexPair( "camera_offset_x",					CAMERA_OFFSET_X_SETTING),
		NameIndexPair( "camera_offset_y",					CAMERA_OFFSET_Y_SETTING),
		NameIndexPair( "camera_offset_z",					CAMERA_OFFSET_Z_SETTING),

		NameIndexPair( "camera_lookat_offset_x",			CAMERA_LOOKAT_OFFSET_X_SETTING),
		NameIndexPair( "camera_lookat_offset_y",			CAMERA_LOOKAT_OFFSET_Y_SETTING),
		NameIndexPair( "camera_lookat_offset_z",			CAMERA_LOOKAT_OFFSET_Z_SETTING),

		NameIndexPair( "camera_spring_k",					CAMERA_SPRING_K_SETTING),        
		NameIndexPair( "camera_spring_look_k",				CAMERA_SPRING_LOOK_K_SETTING),        

		// game
		NameIndexPair( "round_length",						ROUND_LENGTH_SETTING),
		NameIndexPair( "round_time_between",				ROUND_TIME_BETWEEN_SETTING)
	};
	// ----------------------------------------------------------------------------
	// The User Commands index by ENUM and paired below in the Command Index Array
	// ----------------------------------------------------------------------------

	static enum Commands 
	{   // hardcoded to match static array below  
		NO_COMMAND = 0,
		CMD_HELP,
		CMD_CONSOLE,
		CMD_MENU,
		CMD_SCOREBOARD,
		CMD_BROWSER,
		CMD_OPTIONS,
		CMD_SUBMENU,
		CMD_QUIT,

		// Setting Commands
		CMD_SET,
		CMD_CAMERA_DIST,
		CMD_SAVE_CONTROLS,
		CMD_SAVE_SETTINGS,

		// Client/Server Commands
		CMD_HOST,
		CMD_CONNECT,
		CMD_DISCONNECT,
		CMD_KICK,
		CMD_BAN,

		// Hack Commands
		CMD_CAMERA_CONTROL,
		CMD_PHYSICS_RENDER,
		CMD_BOUNDING_BOX,
		CMD_SUNTEST,
		CMD_SUNSTOP,
		CMD_SAVE_SHADOW_MAP,
		CMD_CYCLE_ANIMATION,

		// multiplayer commands
		CMD_TOGGLE_SERVER_LIST,
		CMD_REFRESH_SERVER_LIST,
		CMD_REFRESH_LAN_LIST,
		CMD_SAY,

		NUM_COMMANDS
	};

	static const NameIndexPair CommandIndexArray[] = 
	{   
		NameIndexPair( "help",						CMD_HELP),
		NameIndexPair( "console",					CMD_CONSOLE),
		NameIndexPair( "menu",						CMD_MENU),
		NameIndexPair( "scoreboard",				CMD_SCOREBOARD),
		NameIndexPair( "browser",					CMD_BROWSER),
		NameIndexPair( "options",					CMD_OPTIONS),
		NameIndexPair( "submenu",					CMD_SUBMENU),
		NameIndexPair( "quit",						CMD_QUIT),

		// Setting Commands
		NameIndexPair( "set",						CMD_SET),
		NameIndexPair( "camera_dist",				CMD_CAMERA_DIST),
		NameIndexPair( "save_settings",				CMD_SAVE_SETTINGS),
		NameIndexPair( "save_controls",				CMD_SAVE_CONTROLS),

		// Client/Server Commands
		NameIndexPair( "host",					    CMD_HOST),
		NameIndexPair( "connect",					CMD_CONNECT),
		NameIndexPair( "disconnect",				CMD_DISCONNECT),
		NameIndexPair( "kick",						CMD_KICK),
		NameIndexPair( "ban",						CMD_BAN),

		// Hack Commands
		NameIndexPair( "cameracontrol",				CMD_CAMERA_CONTROL),
		NameIndexPair( "physicsrender",				CMD_PHYSICS_RENDER),
		NameIndexPair( "boundingbox",				CMD_BOUNDING_BOX),
		NameIndexPair( "suntest",					CMD_SUNTEST),
		NameIndexPair( "sunstop",					CMD_SUNSTOP),
		NameIndexPair( "saveshadowmap",				CMD_SAVE_SHADOW_MAP),
		NameIndexPair( "cycleanimation",			CMD_CYCLE_ANIMATION),

		// multiplayer commands
		NameIndexPair( "refreshserverlist",         CMD_REFRESH_SERVER_LIST),
		NameIndexPair( "refreshlanlist",            CMD_REFRESH_LAN_LIST),
		NameIndexPair( "say",						CMD_SAY)
	};

	// ----------------------------------------------------------------------------
	// The Sun States definable in setting file and called upon by GameEngine. 
	// TODO: implement sunstates in setting file.
	// ----------------------------------------------------------------------------

	static enum SunStates 
	{   // hardcoded to match static array below  
		SUNSTATE_NO = 0,
		SUNSTATE_DEFAULT,
		SUNSTATE_BLACKMORPH,
		SUNSTATE_WHITEMORPH,
		SUNSTATE_BLUEMORPH,
		SUNSTATE_BLACKSKY,
		SUNSTATE_WHITESKY,
		SUNSTATE_WHITEEDGE,
		SUNSTATE_BLACKEDGE,
		SUNSTATE_NUM
	};

	static const NameIndexPair SunStateIndexArray[] = 
	{   
		NameIndexPair( "default",						SUNSTATE_DEFAULT),
		NameIndexPair( "black_morph",  					SUNSTATE_BLACKMORPH),
		NameIndexPair( "white_morph",  					SUNSTATE_WHITEMORPH),
		NameIndexPair( "blue_morph",  					SUNSTATE_BLUEMORPH),
		NameIndexPair( "black_sky",  					SUNSTATE_BLACKSKY),
		NameIndexPair( "white_sky",  					SUNSTATE_WHITESKY),
		NameIndexPair( "white_edge",  					SUNSTATE_WHITEEDGE),
		NameIndexPair( "black_edge",  					SUNSTATE_BLACKEDGE)
	};

	// ----------------------------------------------------------------------------
	//! A list of Ascii characters that cannot be represented normall in a text file, so is replaced by word.
	// ----------------------------------------------------------------------------
	struct WordCharPair
	{
		LPCSTR word;
		UCHAR c;
		WordCharPair(LPCSTR _word, UCHAR _c) : word(_word), c(_c) {}
	};

	static const int NumWordChars                   =      17;
	static const WordCharPair WordVKeyArray[NumWordChars] =
	{   // NOTE: --- keep uppercase and in numeric order ---
		WordCharPair( "BACKSPACE",              8 ),        
		WordCharPair( "TAB",                    9 ),
		WordCharPair( "ENTER",                  13 ),
		WordCharPair( "SHIFT",                  16 ),
		WordCharPair( "CTRL",                   17 ),
		WordCharPair( "ALT",                    18 ),

		WordCharPair( "SPACE",                  32 ),
		WordCharPair( "PAGEUP",                 33 ),
		WordCharPair( "PAGEDOWN",               34 ),
		WordCharPair( "END",                    35 ),
		WordCharPair( "HOME",                   36 ),
		WordCharPair( "LEFT",                   37 ),        
		WordCharPair( "UP",                     38 ),
		WordCharPair( "RIGHT",                  39 ),
		WordCharPair( "DOWN",                   40 ),
		WordCharPair( "INSERT",                 45 ),
		WordCharPair( "DELETE",                 46 )

	};

	// ----------------------------------------------------------------------------
	//! Global functions testing the above tables for a match.
	// ----------------------------------------------------------------------------
	UCHAR FindActionIndex(const string& cmd);
	UCHAR FindSettingIndex(const string& stg);
	UCHAR FindAnimationIndex(const string& anm);
	UCHAR FindCommandIndex(const string& cmd);
	UCHAR FindSpriteAnimationIndex(const string& spranm);
	UCHAR FindSunStateIndex(const string& sunname);
	UCHAR ConvertKeyToVKeyChar(const string& key);
	std::string FindKeyWordMatch(UCHAR key);
}