//---------------------------------------------------------------------------------
// File Name: zcta014.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOOlOlOlllO
#define lOOlOlOlllO
#ifndef TA_DEBUG_H
#include "Debug.h"
#endif //
#ifndef TA_MEMORYMGR_H
#include "MemoryMgr.h"
#endif //
namespace\

TA\

{
#define lOOllOllllO(Type)						\
public:																\
	static void TAC_CALL DestroyInstance();						\
	static Type& TAC_CALL GetInstance()							\
	{																\
		lOOOOOOllO(s_p##Type, "Singleton("#Type")::GetInstance. Not initialised");	\
		return *s_p##Type;											\
	}																\
	static Type* TAC_CALL lOOlOlOOlOO() {	return s_p##Type; }	\
private:															\
	static Type* s_p##Type;											\
public:
#define lOOlOllllll(Type)										\
public:																\
	static void TAC_CALL CreateInstance();							\
	friend class MemoryMgr;											\
	lOOllOllllO(Type)
#define	lOOllOlllll(Type)					\
Type* Type::s_p##Type;												\
void Type::DestroyInstance()										\
{																	\
	if (!s_p##Type)													\
		return;														\
	lOOlOOOOOO(s_p##Type, Type);							\
	s_p##Type = 0;													\
}
#define	lOOlOOlllll(Type)									\
lOOllOlllll(Type)							\
void Type::CreateInstance()											\
{																	\
	if (s_p##Type)													\
		return;														\
	lOOOOOllOOO(s_p##Type, Type);								\
	s_p##Type->Initialise();										\
}
}
#endif //
