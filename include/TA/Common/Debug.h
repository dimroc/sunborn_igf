//---------------------------------------------------------------------------------
// File Name: Debug.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_DEBUG_H
#define TA_DEBUG_H

#ifndef TA_COMMON_H
#include "Common.h"
#endif 



namespace TA
{

#define lllOOOOllO(lllOOOllOl) TA::Debug::Output(lllOOOllOl);

#ifdef _DEBUG
	#ifdef TA_IN_PLACE_ASSERT
		#if defined (TA_GCC_X86_INLINE_ASM)
			#define TA_ASSERT(x)			\
				do							\
				{							\
					if (!(x))				\
					{						\
						asm("int $3")		\
					}						\
				} while (0)
		#elif defined(TA_MSVC_X86_INLINE_ASM)
			#define TA_ASSERT(x)			\
				do							\
				{							\
					if (!(x))				\
					{						\
						__asm { int 3 }		\
					}						\
				} while (0)
		#endif
		#define lOOOOOOllO(x, lllOOOOlOO) TA_ASSERT(x)
	#else
		#define TA_ASSERT(x) do { if (!(x)) TA::Debug::lllOOOlOOO(__FILE__, __LINE__, #x, 0); } while (0)
		#define lOOOOOOllO(x, lllOOOOlOO) do { if (!(x)) TA::Debug::lllOOOlOOO(__FILE__, __LINE__, #x, lllOOOOlOO); } while (0)
		namespace Debug
		{
			extern void TAC_CALL lllOOOlOOO(const char* lllOOOOOOl, int lllOOOOOlO, const char* lllOOOOOll, const char* lllOOOOlOO);
		}
	#endif
	#define TA_VERIFY(x) TA_ASSERT(x)
	#define lllOOOlllO TA_ASSERT(0)
	#define lllOOOllll(x) TA_ASSERT(x)
	#define lOlOOOllll(x, lllOOOOlOO) lOOOOOOllO(x, lllOOOOlOO)
	#define lOlOOlOOOl(lllOOOOlOO) lOOOOOOllO(0, lllOOOOlOO)
	#ifdef TA_WARNING_AS_ASSERT
		#define lOOOlllOlO(x, lllOOOOlOO) lOOOOOOllO(x, lllOOOOlOO)
	#else
		#define lOOOlllOlO(x, lllOOOOlOO) do { if (!(x)) TA::Debug::lllOOOOOOO(__FILE__, __LINE__, #x, lllOOOOlOO); } while (0)
		namespace Debug
		{
			extern void TAC_CALL lllOOOOOOO(const char* lllOOOOOOl, int lllOOOOOlO, const char* lllOOOOOll, const char* lllOOOOlOO);
		}
	#endif  
#else 
	
	#define TA_ASSERT(x)
	#define TA_VERIFY(x) x
	
	#define lOOOOOOllO(x, lllOOOOlOO)
	#ifdef TA_MSVC
		#define lllOOOlllO __assume(0) 
		#define lllOOOllll(x) __assume(x) 
		#define lOlOOOllll(x, lllOOOOlOO) __assume(x)
	#else
		#define lllOOOlllO
		#define lllOOOllll(x)
		#define lOlOOOllll(x, lllOOOOlOO)
	#endif
	#define lOlOOlOOOl(lllOOOOlOO) lOOOOOOllO(0, lllOOOOlOO)
	#define lOOOlllOlO(x, lllOOOOlOO) lOOOOOOllO(x, lllOOOOlOO)
#endif 

#ifdef _DEBUG

namespace Debug
{	
	void TAC_CALL OpenOutputFile(const char* szFileName);
	void TAC_CALL CloseOutputFile();
	void TAC_CALL Output(const char* szText);
	void TAC_CALL SetDebugAssertsEnable(bool bEnabled);
	void TAC_CALL SetExitWitFailureOnAssert(bool bEnabled);	

}
#endif 

}

#endif 

