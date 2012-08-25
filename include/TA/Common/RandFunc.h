//---------------------------------------------------------------------------------
// File Name: RandFunc.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_RANDFUNC_H
#define TA_RANDFUNC_H

#ifndef TA_TYPES_H
#include "Types.h"
#endif 

#ifndef TA_COMMON_H
#include "Common.h"
#endif 

#include <stdlib.h>



namespace TA
{



class RandFunc
{
public:

	
	
	
	static inline void TAC_CALL RandSeed(u32 nSeed) { srand(nSeed); }

	
	
	static inline float TAC_CALL RandUnit() { return (float)rand() / (float)RAND_MAX; }

	
	
	static inline float TAC_CALL RandBetweenPosNeg(float fScale) { return (RandUnit() * 2.0f - 1.0f) * fScale; }

	
	
	
	static inline int TAC_CALL RandBetween(int nMin, int nMax) { return (rand() % (nMax - nMin)) + nMin; }

	
	
	static inline float TAC_CALL RandBetween(float fMin, float fMax) { return RandUnit() * (fMax - fMin) + fMin;  }

	
	
	
	
	static void TAC_CALL TaRandSeed(u32 nSeed);

	
	
	static u32 TAC_CALL TaRandInt();

	
	
	static inline float TAC_CALL TaRandUnit() { return (float)TaRandInt() / (float)0xFFFFFFFF; }

	
	
	static inline float TAC_CALL TaRandBetweenPosNeg(float fScale) { return (TaRandUnit() * 2.0f - 1.0f) * fScale; }

	
	
	
	static inline int TAC_CALL TaRandBetween(int nMin, int nMax) { return (TaRandInt() % (nMax - nMin)) + nMin; }

	
	
	static inline float TAC_CALL TaRandBetween(float fMin, float fMax) { return TaRandUnit() * (fMax - fMin) + fMin;  }


};


};

#endif 
