//---------------------------------------------------------------------------------
// File Name: Vec4.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_VEC4_H
#define TA_VEC4_H

#ifndef TA_MATHS_H
#include "Maths.h"
#endif 



#define TA_VEC4_CALL TA_FAST_CALL

namespace TA
{


TA_ALIGN_16 struct TACOMMON_CLASS Vec4
{	
	
	float x;
	float y;
	float z;
	float w;


	enum Axis
	{
		AXIS_X = 0,
		AXIS_Y,
		AXIS_Z,
		AXIS_W,
	};

	
	
    Vec4() {};
    Vec4(const Vec4& v4Value) { x = v4Value.x; y = v4Value.y; z = v4Value.z;  w = v4Value.w; }
    Vec4(float fX, float fY, float fZ, float fW) { x = fX; y = fY; z = fZ; w = fW; }

	void Initialise(float fX, float fY, float fZ, float fW) { x = fX; y = fY; z = fZ; w = fW; }
	

    
	
    operator float* () { return (float*)&x; }
    operator const float* () const { return (float*)&x; }
	

	
	
    float& operator [] (int nIndex) { return ((float*)&x)[nIndex]; }
    const float& operator [] (int nIndex) const { return ((float*)&x)[nIndex]; }
	

    
	
    Vec4& operator += (const Vec4& v4Value) { x += v4Value.x; y += v4Value.y; z += v4Value.z;  w += v4Value.w; return *this; }
    Vec4& operator -= (const Vec4& v4Value) { x -= v4Value.x; y -= v4Value.y; z -= v4Value.z;  w += v4Value.w; return *this; }
    Vec4& operator *= (float fValue) { x *= fValue; y *= fValue; z *= fValue; w *= fValue; return *this; }
    Vec4& operator /= (float fValue) { float fDiv = 1.0f / fValue; x *= fDiv; y *= fDiv; z *= fDiv; w *= fDiv; return *this; }
	

    
	
    Vec4 operator + () const { return *this; };
    Vec4 operator - () const { Vec4 v4ReturnValue(-x, -y, -z, -w); return v4ReturnValue; };
	

    
	
    Vec4 operator + (const Vec4& v4Value) const { Vec4 v4ReturnValue(x + v4Value.x, y + v4Value.y, z + v4Value.z, w + v4Value.w); return v4ReturnValue; }
    Vec4 operator - (const Vec4& v4Value) const { Vec4 v4ReturnValue(x - v4Value.x, y - v4Value.y, z - v4Value.z, w - v4Value.w); return v4ReturnValue; }
    Vec4 operator * (float fValue) const { Vec4 v4ReturnValue(x * fValue, y * fValue, z * fValue, w * fValue); return v4ReturnValue; }
    Vec4 operator / (float fValue) const { float fDiv = 1.0f / fValue; Vec4 v4ReturnValue(x * fDiv, y * fDiv, z * fDiv, w * fDiv); return v4ReturnValue; }
	

    friend Vec4 TA_VEC4_CALL operator * (float fValue, const Vec4& v4Value);

	
	
    bool operator == (const Vec4& v4Value) const { return x == v4Value.x && y == v4Value.y && z == v4Value.z && w == v4Value.w; }
	bool operator != (const Vec4& v4Value) const { return x != v4Value.x || y != v4Value.y || z != v4Value.z || w != v4Value.w; }
	

	
	static inline Vec4 TAC_CALL Cross(const Vec4& v4A, const Vec4& v4B)
	{
		Vec4 v4ReturnValue;
		v4ReturnValue.x = v4A.y * v4B.z - v4A.z * v4B.y;
		v4ReturnValue.y = v4A.z * v4B.x - v4A.x * v4B.z;
		v4ReturnValue.z = v4A.x * v4B.y - v4A.y * v4B.x;
		v4ReturnValue.w = 1.0f;
		return v4ReturnValue;
	}
	inline Vec4 Cross(const Vec4& v4Value) const { return Cross(*this, v4Value); }

	
	static inline float TA_VEC4_CALL Dot(const Vec4& v4A, const Vec4& v4B)
	{
		return v4A.x * v4B.x + v4A.y * v4B.y + v4A.z * v4B.z + v4A.w * v4B.w;
	}
	inline float Dot(const Vec4& v4Value) const { return Dot(*this, v4Value); }

	static inline float TA_VEC4_CALL GetMagnitude(const Vec4& v4Value) { return v4Value.GetMagnitude(); }
	inline float GetMagnitude() const { return Sqrt(x * x + y * y + z * z + w * w); }
	static inline float TA_VEC4_CALL GetMagnitudeSqrd(const Vec4& v4Value) { return v4Value.GetMagnitudeSqrd(); }
	inline float GetMagnitudeSqrd() const { return x * x + y * y + z * z + w * w; }
	static inline Vec4 TA_VEC4_CALL GetNormal(const Vec4& v4Value) { return v4Value.GetNormal(); }
	inline Vec4 GetNormal() const { return (*this) * ReciprocalSqrt(GetMagnitudeSqrd()); }
	inline void Normalise() { (*this) *= ReciprocalSqrt(GetMagnitudeSqrd()); }
	inline void Clear() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }
	inline bool IsNormalised() const { return IsEqualToOneWithInError(GetMagnitudeSqrd()); }
	inline bool IsZero() const { return GetMagnitudeSqrd() == 0.0f; }
	inline void GetAxisOrder(int pnAxisOrder[4]) const; 
	inline float GetAxis(int nIndex) const { return (*this)[nIndex]; }
	static inline const Vec4& TA_VEC4_CALL GetUnitVector(int nIndex);
	


};


const Vec4 k_v4Zero(0.0f, 0.0f, 0.0f, 0.0f);
const Vec4 k_v4UnitX(1.0f, 0.0f, 0.0f, 0.0f);
const Vec4 k_v4UnitY(0.0f, 1.0f, 0.0f, 0.0f);
const Vec4 k_v4UnitZ(0.0f, 0.0f, 1.0f, 0.0f);
const Vec4 k_v4UnitW(0.0f, 0.0f, 0.0f, 1.0f);


}

#include "../Common/zcta014.inl"

#endif 
