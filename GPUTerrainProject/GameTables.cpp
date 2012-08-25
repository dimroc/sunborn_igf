#include "dxstdafx.h"
#include "GameTables.h"

// ------------------------------------------------------------------
// GLOBAL SHIZNIT
// ------------------------------------------------------------------

UCHAR GameTable::FindActionIndex(const string &cmd)
{
    for(int i=0; i<GameTable::NUM_ACTIONS; i++)
    {
        if(cmd.compare(GameTable::ActionIndexArray[i].str) == 0)
            return GameTable::ActionIndexArray[i].cmd;
    }
    return GameTable::NO_ACTION;  // not found
}

UCHAR GameTable::FindSettingIndex(const string& stg)
{
    for(int i=0; i<GameTable::NUM_SETTINGS; i++)
    {
		if(stg.compare(GameTable::SettingIndexArray[i].str) == 0)
            return GameTable::SettingIndexArray[i].cmd;
    }
    return GameTable::NO_SETTING;  // not found
}

UCHAR GameTable::FindAnimationIndex(const string& anm)
{
    for(int i=0; i<GameTable::ANIM_NUM; i++)
    {
		if(anm.compare(GameTable::AnimationIndexArray[i].str) == 0)
            return GameTable::AnimationIndexArray[i].cmd;
    }
    return GameTable::ANIM_NO;  // not found
}
UCHAR GameTable::FindCommandIndex(const string &cmd)
{
	for(int i=GameTable::NO_COMMAND; i < GameTable::NUM_COMMANDS; i++)
    {
        if(cmd.compare(GameTable::CommandIndexArray[i].str) == 0)
            return GameTable::CommandIndexArray[i].cmd;
    }
    return GameTable::NO_COMMAND;  // not found
}
UCHAR GameTable::FindSpriteAnimationIndex(const string& spranm)
{
	for(int i=GameTable::NO_SPRITE; i < GameTable::NUM_SPRITES; i++)
    {
        if(spranm.compare(GameTable::SpriteIndexArray[i].str) == 0)
            return GameTable::SpriteIndexArray[i].cmd;
    }
    return GameTable::NO_SPRITE;  // not found
	
}

UCHAR GameTable::FindSunStateIndex(const string& sunname)
{
    for(int i=GameTable::SUNSTATE_NO; i < GameTable::SUNSTATE_NUM; i++)
    {
        if(sunname.compare(GameTable::SunStateIndexArray[i].str) == 0)
            return GameTable::SunStateIndexArray[i].cmd;
    }
    return GameTable::SUNSTATE_NO;  // not found
	
}

UCHAR GameTable::ConvertKeyToVKeyChar(const string& key)
{
    if(key.size() == 1)
        return (UCHAR)VkKeyScanA( (UCHAR)tolower(key.at(0)) );
    string temp(key);
    std::transform(key.begin(), key.end(), temp.begin(), toupper);
    for(int i=0; i<GameTable::NumWordChars; i++)
    {
        if(temp.compare(GameTable::WordVKeyArray[i].word) == 0)
            return (UCHAR)tolower(GameTable::WordVKeyArray[i].c);
    }
    return NULL;
}
// Binary searches the array to find the matching string
// That's why the array needs to stay in numeric order
std::string GameTable::FindKeyWordMatch(UCHAR key){
	int i = NumWordChars / 2;
	int nLeft = i / 2;
	int n = 0;

	while (n != NumWordChars){ // this is just a safety check for an infinite loop, (though this should never happen)
		if (GameTable::WordVKeyArray[i].c == key){
			string text(GameTable::WordVKeyArray[i].word);
		    std::transform(text.begin(), text.end(), text.begin(), tolower);
			return text;
		}

		// not a match, change i
		if (GameTable::WordVKeyArray[i].c > key) // too high, move down
			i -= nLeft;
		else // too low, move up
			i += nLeft;
		
		if (nLeft != 1)
			nLeft /= 2;
		n++;
	}
	return string();
}
