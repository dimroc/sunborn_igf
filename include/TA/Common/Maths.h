//---------------------------------------------------------------------------------
// File Name: Maths.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_MATHS_H
#define TA_MATHS_H

#ifndef TA_TYPES_H
#include "Types.h"
#endif 

#ifndef TA_DEBUG_H
#include "Debug.h"
#endif 

#include <math.h>
#include <float.h>



namespace TA
{
	



const float k_fPi = 3.14159f;
const float k_fMaxFloat = FLT_MAX;
const float k_fMinFloat = FLT_MIN;



inline float TAC_CALL Fabs(float fValue) { return (float)fabs((double)fValue); }

inline int TAC_CALL Abs(int nValue) 
{ 	
#ifdef TA_GCC
	return (nValue ^ (nValue >> (31))) + ((unsigned) nValue >> (31));
#else
	return abs(nValue);
#endif
}

inline float TAC_CALL Sign(float fValue) 
{ 
	
	u32 nSign = (((u32&)fValue) & 0x80000000) + 0x3f800000;
	return (float&)nSign;
} 



inline bool TAC_CALL SignsEqual(float fA, float fB)
{
	return (((u32&)fA) & 0x80000000) == (((u32&)fB) & 0x80000000);
}

inline float TAC_CALL Sqrt(float fValue) { return (float)sqrtf(fValue); }

inline float TAC_CALL ReciprocalSqrt(float fValue) { return 1.0f / (float)sqrtf(fValue); }

inline float TAC_CALL Pow(float fValue, float fExponent) { return (float)powf(fValue, fExponent); }

inline float Pow2(float fX) { return fX * fX; }	

inline float TAC_CALL Sin(float fAngle) { return (float)sinf(fAngle); }

inline float TAC_CALL Cos(float fAngle) { return (float)cosf(fAngle); }

inline float TAC_CALL Tan(float fAngle) { return (float)tanf(fAngle); }

inline void TAC_CALL SinAndCos(float fAngle, float& fSin, float& fCos)
{
#ifdef TA_MSVC_X86_INLINE_ASM

	float fSin2_reserved; 
	float fCos2_reserved;
	float fAngle_reserved;

	fAngle_reserved = fAngle;
	__asm
	{
		fld fAngle_reserved
		fsincos
		fstp fCos2_reserved
		fstp fSin2_reserved
	}
	fCos = fCos2_reserved;
	fSin = fSin2_reserved; 
#else
	fSin = Sin(fAngle);
	fCos = Cos(fAngle);
#endif
}

inline float TAC_CALL ATan2(float fY, float fX) { return (float)atan2f(fY, fX); }

inline float TAC_CALL ACos(float fValue) { return (float)acosf(fValue); }

inline float TAC_CALL FMod(float fValue, float fMod) { return (float)fmodf(fValue, fMod); }

inline float TAC_CALL Exp(float fValue) { return (float)expf(fValue); }

inline float TAC_CALL Log(float fValue) { return (float)logf(fValue); }

#if defined (TA_MSVC_X86_INLINE_ASM)
#define TA_ASM_LOG2
inline u32 TAC_CALL Log2(u32 nValue) 
{ 
	__asm
	{
		bsr EAX, nValue
	}
}
#endif

inline bool TAC_CALL IsEqualToOneWithInError(float fValue) { return Fabs(1.0f - fValue) < 0.001f; }

inline void TAC_CALL Clamp(float& fValue, float const fMin, const float fMax)
{
	if (fValue > fMax)
		fValue = fMax;
	else if (fValue < fMin)
		fValue = fMin;
}

inline void TAC_CALL Clamp(int& nValue, const int nMin, int const nMax)
{
	if (nValue > nMax)
		nValue = nMax;
	else if (nValue < nMin)
		nValue = nMin;
}

inline void TAC_CALL ClampPosNeg(float& fValue, float fPositive)
{
	if (fValue > fPositive)
		fValue = fPositive;
	else if (fValue < -fPositive)
		fValue = -fPositive;
}

inline void TAC_CALL WrapAngle(float& fValue)
{
	if (fValue > k_fPi)
		fValue -= k_fPi * 2.0f;
	else if (fValue < -k_fPi)
		fValue += k_fPi * 2.0f;
}

inline int TAC_CALL FastFloatToInt(float fValue)
{
	
	int nReturnVal_reserved;
	float fValue_reserved;
	
	fValue_reserved = fValue;
#if defined (TA_GCC_X86_INLINE_ASM)
    return (int)fValue;
    
#elif defined (TA_MSVC_X86_INLINE_ASM)
	__asm
	{
		fld fValue_reserved
		fistp nReturnVal_reserved
	}
#else
	nReturnVal_reserved = (int)fValue_reserved;
#endif
	return nReturnVal_reserved;
}

inline bool TAC_CALL FloatIsOK(float fValue) { return ((*(int*)&fValue) & 0x7F800000) != 0x7F800000; }

inline float TAC_CALL FastMin(float fA, float fB) { return (fA + fB - Fabs(fB - fA)) * 0.5f; }

inline float TAC_CALL FastMax(float fA, float fB) { return (fA + fB + Fabs(fB - fA)) * 0.5f; }

inline float TAC_CALL Min(float fA, float fB) { return (fA < fB) ? fA : fB; }

inline float TAC_CALL Max(float fA, float fB) { return (fA > fB) ? fA : fB; }

inline void TAC_CALL ClampIfLessThen(float& fValue, float fClampTo) { if (fValue < fClampTo) fValue = fClampTo; }

inline void TAC_CALL ClampIfGreaterThen(float& fValue, float fClampTo) { if (fValue > fClampTo) fValue = fClampTo; }

template<class Type> 
inline void TAC_CALL Swap(Type& a, Type& b) { Type tmp = a; a = b; b = tmp;  }




};

#endif 

