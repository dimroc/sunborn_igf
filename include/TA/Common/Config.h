#ifndef TA_OBFUSCATED
#define TA_OBFUSCATED
#endif
#ifndef TA_CONFIG_H
#define TA_CONFIG_H

#ifndef TA_OBFUSCATED
#define 
#define 
#define 
#define 
#define 
#define 
#endif 




//---------------------------------------------------------------------------------
// File Name: Config.h
// Description:
//! \file 
//! \brief Portability Configuration
//!
//! This file contains various defines that may be uncommented to help with
//! porting and optimising the True Axis Physics SDK. See the code comments
//! for documentation on the usage.
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia. 
// All Rights Reserved.
//
// History:
//		Created File.
//---------------------------------------------------------------------------------


//#define _DEBUG
// _DEBUG turns on debugging. In debug mode, the library will give asserts and 
// warnings.

//#define TA_DISABLE_INLINE_ASM
// TA_DISABLE_INLINE_ASM turns off all inline assembly replacing it with standard C.
// TrueAxis currently only supports 0x86 assembler, so this can be use full for 
// porting to other machines.

//#define TA_DISABLE_IN_PLACE_ASSERT
// When _DEBUG is used, the library will create asserts and warnings. When using
// msvc++, asserts trigger a break point interrupt inside a macro. This way
// the program brakes on the line of the assert. To instead make the assert
// print an error to file, or to the output pain of msvc++, you can define
// TA_DISABLE_IN_PLACE_ASSERT.

//#define TA_WARNING_AS_ASSERT
// TA_WARNING_AS_ASSERT causes warnings to trigger an assert, rather then just
// writing text to the debug file.

//#define TA_DISABLE_MEMORY_ALIGNMENT
// TA_DISABLE_MEMORY_ALIGNMENT turns off memory alignment by __declspec(align(16))
// using msvc++.

//#define TA_DISABLE_MANUAL_CALLING_CONVENTIONS
// TA_DISABLE_MANUAL_CALLING_CONVENTIONS disables the manual setting of __stdcall,
// __fastcall and __cdecl under msvc++. May be helpful for porting.

//#define TA_ENABLE_PRE_FETCH
// TA_ENABLE_PRE_FETCH turns on cache pre-fetching. This my offer a significant
// performance on systems that support pre-fetching.

#define TA_MAX_ANGULAR_VELOCITY 50.0f

#define TA_MAX_LINEAR_VELOCITY 1000.0f

#endif // TA_CONFIG_H
