#pragma once

namespace guidef{
#define		KEY_TILDE		0xC0
#define		KEY_SLASH		0xBF
#define		KEY_0			0x30
#define		KEY_9			0x39
#define		KEY_A			0x41
#define		KEY_Z			0x5A

	UINT GetKeyboardCharacter(UINT nChar, bool bShift);
	UINT GetKeyboardNumericCharacter(UINT nChar, bool bShift);

}

#define                         MENU_TRIGGER_WIDTH                  20  // width from menu edge to trigger mouse.
