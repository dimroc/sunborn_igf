//---------------------------------------------------------------------------------
// File Name: MemoryMgr.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_MEMORYMGR_H
#define TA_MEMORYMGR_H

#ifndef TA_COMMON_H
#include "Common.h"
#endif 



#include <new>

#ifdef _DEBUG

#define lOOOllllOO(lOOOlOllOlO, Type, lOOOlOllOll)										\
{																							\
	TA::lOOOlOlllOO = __LINE__;														\
	TA::lOOOlOlllOl = __FILE__;													\
	(lOOOlOllOlO) = (Type*)(new TA::lOOOlOllllO< Type >[lOOOlOllOll]);								\
	TA::lOOOlOlllOO = 0;																\
	TA::lOOOlOlllOl = 0;														\
}

#define lOlOOOlllO(lOOOlOllOlO, Type)												\
{																							\
	TA::lOOOlOlllOO = __LINE__;														\
	TA::lOOOlOlllOl = __FILE__;													\
	delete []((TA::lOOOlOllllO< Type >*)lOOOlOllOlO);											\
	TA::lOOOlOlllOO = 0;																\
	TA::lOOOlOlllOl = 0;														\
}

#define lOOOOOllOOO(lOOOlOllOlO, Type)														\
{																							\
	(lOOOlOllOlO) = (Type*)TA::MemoryMgr::DbgAlloc(sizeof (Type), 16, __FILE__, __LINE__);			\
	new (lOOOlOllOlO) Type;																		\
}

#define lOOlOOOOOO(lOOOlOllOlO, Type)													\
{																							\
	(lOOOlOllOlO)->~Type();																		\
	TA::MemoryMgr::DbgFree((lOOOlOllOlO), __FILE__, __LINE__);										\
}

#else

#define lOOOllllOO(lOOOlOllOlO, Type, lOOOlOllOll)										\
{																							\
	(lOOOlOllOlO) = (Type*)(new TA::lOOOlOllllO< Type >[lOOOlOllOll]);								\
}

#define lOlOOOlllO(lOOOlOllOlO, Type)												\
{																							\
	delete []((TA::lOOOlOllllO< Type >*)lOOOlOllOlO);											\
}

#define lOOOOOllOOO(lOOOlOllOlO, Type)														\
{																							\
	(lOOOlOllOlO) = (Type*)TA::MemoryMgr::Alloc(sizeof (Type), 16);								\
	new (lOOOlOllOlO) Type;																		\
}

#define lOOlOOOOOO(lOOOlOllOlO, Type)													\
{																							\
	(lOOOlOllOlO)->~Type();																		\
	TA::MemoryMgr::Free((lOOOlOllOlO));															\
}

#endif

namespace TA
{


class TACOMMON_CLASS MemoryMgr
{
public:
	typedef void* (TAC_CALL *FnAllocCallBack)(unsigned int nSize, unsigned int nAlignment);
	typedef void (TAC_CALL *FnFreeCallBack)(void* pData);
	typedef void* (TAC_CALL *FnDbgAllocCallBack)(unsigned int nSize, unsigned int nAlignment, const char * szFileName, int nLine);
	typedef void (TAC_CALL *FnDbgFreeCallBack)(void* pData, const char * szFileName, int nLine);
	
	static bool TAC_CALL SetAlloc(FnAllocCallBack pAllocCallBack);
	static bool TAC_CALL SetFree(FnFreeCallBack pFreeCallBack);
	static bool TAC_CALL SetDbgAlloc(FnDbgAllocCallBack pDbgAllocCallBack);
	static bool TAC_CALL SetDbgFree(FnDbgFreeCallBack pDbgFreeCallBack);

	static void* TAC_CALL DbgAlloc(unsigned int nSize, unsigned int nAlignment, const char * szFileName, int nLine);
	static void TAC_CALL DbgFree(void* pPtr, const char * szFileName, int nLine);
	static void* TAC_CALL Alloc(unsigned int nSize, unsigned int nAlignment);
	static void TAC_CALL Free(void* pPtr);

	static int TAC_CALL GetNumAllocationsUnFreed();


#ifdef lOOOlOlllll
	static void TAC_CALL lOOOllOOOOO();
	static void TAC_CALL lOOOllOOOOl();
#endif 

};

#ifndef DOXYGEN
#ifdef _DEBUG
	extern int lOOOlOlllOO;
	extern char* lOOOlOlllOl;
#endif 

template <class Type>
struct lOOOlOllllO
{
public:
	Type lOOOllOOOlO;
	void * TAC_CALL operator new[](unsigned int lOOlllOllO)
	{
#ifdef _DEBUG
		return TA::MemoryMgr::DbgAlloc(lOOlllOllO, 16, lOOOlOlllOl, lOOOlOlllOO);
#else
		return TA::MemoryMgr::Alloc(lOOlllOllO, 16);
#endif
	}
	void TAC_CALL operator delete[](void* lOOlllOOOO)
	{
#ifdef _DEBUG
		TA::MemoryMgr::DbgFree(lOOlllOOOO, lOOOlOlllOl, lOOOlOlllOO);
#else
		TA::MemoryMgr::Free(lOOlllOOOO);
#endif
	}
};

#endif 

}

#endif 
