//---------------------------------------------------------------------------------
// File Name: Vec2.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_VEC2_H
#define TA_VEC2_H

#ifndef TA_MATHS_H
#include "Maths.h"
#endif 



namespace TA
{


struct TACOMMON_CLASS Vec2
{	
	

	
	float x;
	float y;

	enum Axis
	{
		AXIS_X = 0,
		AXIS_Y,
	};

    
	
    Vec2() {};
  
    Vec2(const Vec2& v2Value) { x = v2Value.x; y = v2Value.y; }
    Vec2(float fX, float fY) { x = fX; y = fY; }

	void Initialise(float fX, float fY) { x = fX; y = fY; }
	

    
	
    operator float* () { return (float*)&x; }
    operator const float* () const { return (float*)&x; }
	

    
	
    float& operator [] (int nIndex) { return ((float*)&x)[nIndex]; }
    const float& operator [] (int nIndex) const { return ((float*)&x)[nIndex]; }
	

    
	
    Vec2& operator += (const Vec2& v2Value) { x += v2Value.x; y += v2Value.y; return *this; }
    Vec2& operator -= (const Vec2& v2Value) { x -= v2Value.x; y -= v2Value.y; return *this; }
    Vec2& operator *= (float fValue) { x *= fValue; y *= fValue; return *this; }
    Vec2& operator /= (float fValue) { float fDiv = 1.0f / fValue; x *= fDiv; y *= fDiv; return *this; }
	

    
	
    Vec2 operator + () const { return *this; };
    Vec2 operator - () const { Vec2 v2ReturnValue(-x, -y); return v2ReturnValue; };

    
	
    Vec2 operator + (const Vec2& v2Value) const { Vec2 v2ReturnValue(x + v2Value.x, y + v2Value.y); return v2ReturnValue; }
    Vec2 operator - (const Vec2& v2Value) const { Vec2 v2ReturnValue(x - v2Value.x, y - v2Value.y); return v2ReturnValue; }
    Vec2 operator * (float fValue) const { Vec2 v2ReturnValue(x * fValue, y * fValue); return v2ReturnValue; }
    Vec2 operator / (float fValue) const { float fDiv = 1.0f / fValue; Vec2 v2ReturnValue(x * fDiv, y * fDiv); return v2ReturnValue; }
	

    friend Vec2 operator * (float fValue, const Vec2& v2Value);

    
	
    bool operator == (const Vec2& v2Value) const { return x == v2Value.x && y == v2Value.y; }
	bool operator != (const Vec2& v2Value) const { return x != v2Value.x || y != v2Value.y; }
	

	
	static inline float TAC_CALL Dot(const Vec2& v2A, const Vec2& v2B)
	{
		return v2A.x * v2B.x + v2A.y * v2B.y;
	}
	inline float Dot(const Vec2& v2Value) const { return Dot(*this, v2Value); }

	static inline float TAC_CALL GetMagnitude(const Vec2& v2Value) { return v2Value.GetMagnitude(); }
	inline float GetMagnitude() const { return Sqrt(x * x + y * y); }
	static inline float TAC_CALL GetMagnitudeSqrd(const Vec2& v2Value) { return v2Value.GetMagnitudeSqrd(); }
	inline float GetMagnitudeSqrd() const { return x * x + y * y; }
	static inline Vec2 TAC_CALL GetNormal(const Vec2& v2Value) { return v2Value.GetNormal(); }
	inline Vec2 GetNormal() const { return (*this) * ReciprocalSqrt(GetMagnitudeSqrd()); }
	inline void Normalise() { (*this) *= ReciprocalSqrt(GetMagnitudeSqrd()); }
	inline void Clear() { x = 0.0f; y = 0.0f; }
	inline bool IsNormalised() const { return IsEqualToOneWithInError(GetMagnitudeSqrd()); }
	inline bool IsZero() const { return GetMagnitudeSqrd() == 0.0f; }
	inline void GetAxisOrder(int pnAxisOrder[2]) const; 
	inline float GetAxis(int nIndex) const { return (*this)[nIndex]; }
	static inline const Vec2& TAC_CALL GetUnitVector(int nIndex);
	
	
	
};


const Vec2 k_v2Zero(0.0f, 0.0f);
const Vec2 k_v2UnitX(1.0f, 0.0f);
const Vec2 k_v2UnitY(0.0f, 1.0f);



}
#include "../Common/zcta010.inl"

#endif 
