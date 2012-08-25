//---------------------------------------------------------------------------------
// File Name: Types.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_TYPES_H
#define TA_TYPES_H

#ifndef TA_COMMON_H
#include "Common.h"
#endif 



namespace TA
{

	


#ifdef TA_MSVC



typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned __int64 u64;
typedef size_t uSize;

typedef signed char s8;
typedef short s16;
typedef int s32;
typedef __int64 s64;


typedef char Char;

#endif

#ifdef TA_GCC

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long  u64;
typedef u32 uSize;

typedef signed char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef char Char;

#endif


#define TA_CHAR(x) x




};

#endif 
