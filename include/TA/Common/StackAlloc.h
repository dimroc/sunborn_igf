//---------------------------------------------------------------------------------
// File Name: StackAlloc.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef lOOlllOOOOO
#define lOOlllOOOOO

#ifndef TA_COMMON_H
#include "Common.h"
#endif 

#include <malloc.h>



#ifdef lOlOlOOllO
#define lOOlllOOOOl(Type, lOlOOOOlll, lOOlllOllO) Type lOlOOOOlll[lOOlllOllO]
#else
#define lOOlllOOOOl(Type, lOlOOOOlll, lOOlllOllO) Type* lOlOOOOlll = (Type*)_alloca((lOOlllOllO) * sizeof (Type))
#endif

#endif 

