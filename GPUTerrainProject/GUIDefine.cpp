#include "dxstdafx.h"
#include "GUIDefine.h"

UINT guidef::GetKeyboardCharacter(UINT nChar, bool bShift){
	if (KEY_A <= nChar && nChar <= KEY_Z){
		if (bShift) //Uppercase
			return (nChar);
		else //Lowercase
			return (nChar + 32);
	}
	else if (KEY_0 <= nChar && nChar <= KEY_9){
		if (!bShift)
			return (nChar);
		else {
			switch(nChar - 48){
				case 0: return ')';
				case 1: return '!';
				case 2: return '@';
				case 3: return '#';
				case 4: return '$';
				case 5: return '%';
				case 6: return '^';
				case 7: return '&';
				case 8: return '*';
				case 9: return '(';
			}
		}
	}
	else if (nChar == VK_SPACE) {
		return (nChar);
	}
	else if (nChar == KEY_SLASH) {
		if (bShift)
			return('?');
		else
			return('/');
	}
	//else if (nChar == KEY_TILDE) {
	//	if (bShift)
	//		return '~';
	//	else
	//		return '`';
	//}
	else if (nChar == VK_OEM_MINUS) {
		if (bShift)
			return('_');
		else
			return('-');
	}
	else if (nChar == VK_OEM_PERIOD){
		if (bShift)
			return '>';
		else
			return '.';
	}
	else if (nChar == VK_OEM_COMMA){
		if (bShift)
			return '<';
		else
			return ',';
	}
	else if (nChar == VK_OEM_PLUS){
		if (bShift)
			return '=';
		else
			return '+';
	}
	else if (nChar == VK_OEM_1){
		if (bShift)
			return ':';
		else
			return ';';
	}
	else if (nChar == VK_OEM_4){
		if (bShift)
			return '{';
		else
			return '[';
	}	
	else if (nChar == VK_OEM_6){
		if (bShift)
			return '}';
		else
			return ']';
	}
	else if (nChar == VK_OEM_5){
		if (bShift)
			return '|';
		else
			return '\\';
	}
	else if (nChar == VK_OEM_7){
		if (bShift)
			return '\"';
		else
			return '\'';
	}
	return 0;
}

UINT guidef::GetKeyboardNumericCharacter(UINT nChar, bool bShift){
	if (KEY_0 <= nChar && nChar <= KEY_9){
		if (!bShift)
			return (nChar);
	}
	else if (nChar == VK_OEM_MINUS) {
		if (!bShift)
			return('-');
	}
	else if (nChar == VK_OEM_PERIOD){
		if (!bShift)
			return '.';
	}

	return 0;
}
