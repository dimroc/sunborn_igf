//---------------------------------------------------------------------------------
// File Name: RefCount.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_REFCOUNT_H
#define TA_REFCOUNT_H

#ifndef TA_DEBUG_H
#include "Debug.h"
#endif 

#ifndef TA_MEMORYMGR_H
#include "MemoryMgr.h"
#endif 



namespace TA
{

class TACOMMON_CLASS RefCount
{
public:

	RefCount();
	virtual ~RefCount();

	void AddRef() const;
	void Release() const;

	int GetRefCount() const;


private:
	mutable int lOOllOOllll;
};

inline RefCount::RefCount() { lOOllOOllll = 1; }
inline RefCount::~RefCount() { lOOOOOOllO(lOOllOOllll == 0, "RefCount::~RefCount. The reference count is non zero"); }
inline void RefCount::AddRef() const { lOOllOOllll++; }
inline int RefCount::GetRefCount() const { return lOOllOOllll; }

};

#endif 
