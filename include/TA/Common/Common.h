//---------------------------------------------------------------------------------
// File Name: Common.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COMMON_H
#define TA_COMMON_H

#include "Config.h"



#ifdef __GNUC__ 
#define TA_GCC
#ifndef __MINGW32__
#define lOlOlOOllO
#endif
#endif

#define TA_VERSION 1202

#ifdef TA_GCC
	#ifndef TA_DISABLE_INLINE_ASM
		#define TA_GCC_X86_INLINE_ASM
	#endif
#else
#define TA_MSVC
	#ifndef TA_DISABLE_INLINE_ASM
		#ifndef TA_DISABLE_IN_PLACE_ASSERT
			#define TA_IN_PLACE_ASSERT
		#endif
		#define TA_MSVC_X86_INLINE_ASM
	#endif
	
	
	
#endif

#define TACOMMON_CLASS
#ifdef TA_MSVC
	#ifdef TA_DISABLE_MANUAL_CALLING_CONVENTIONS
		#define TAC_CALL
		#define TA_FAST_CALL
		#define TA_CALL_BACK
	#elif defined(_DEBUG)
		#define TAC_CALL __stdcall
		#define TA_FAST_CALL __stdcall
		#define TA_CALL_BACK __stdcall
	#else
		#define TAC_CALL __fastcall
		#define TA_FAST_CALL __fastcall
		#define TA_CALL_BACK __stdcall
	#endif 
	#ifdef TA_DISABLE_MEMORY_ALIGNMENT
		#define TA_ALIGN_16
	#else
		#define TA_ALIGN_16 __declspec(align(16))
	#endif

	#if (_MSC_VER >= 1400 )
		#define lOlOlOOlll __restrict
		#define lOlOlOlOOO __declspec (noalias)
	#else+
		#define lOlOlOOlll
		#define lOlOlOlOOO
	#endif

	#define lOlOlOlOOl __forceinline
#else
	#define TAC_CALL
	#define TA_FAST_CALL
	#define TA_ALIGN_16
	#define TA_CALL_BACK 

	#define lOlOlOlOOl inline
	#define lOlOlOOlll
	#define lOlOlOlOOO
#endif


#ifdef _DEBUG
namespace TA
{
extern bool lOOlOOOllO;
}
#endif 


#endif 
