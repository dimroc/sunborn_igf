//---------------------------------------------------------------------------------
// File Name: Vec3.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_VEC3_H
#define TA_VEC3_H

#ifndef TA_MATHS_H
#include "Maths.h"
#endif 



#define TA_VEC3_CALL TA_FAST_CALL


#ifndef _DEBUG
#endif


#if defined(TA_SSE) || defined(TA_SSE2)

#ifndef lOOlllllOll
#include "../Common/zcta020.h"
#endif 

#define lOOllllllOO

#elif defined(lOOllllllOl)

#ifndef lOOlllllllO
#include "../Common/zcta019.h"
#endif 

#else

#define TA_VEC3_ZERO_PADDING() lOOllllllll = 0

namespace TA
{



TA_ALIGN_16 struct TACOMMON_CLASS Vec3
{	
	
	float x;
	float y;
	float z;
	float lOOllllllll; 


	enum Axis
	{
		AXIS_X = 0,
		AXIS_Y,
		AXIS_Z,
	};

	
	
    Vec3() { TA_VEC3_ZERO_PADDING(); };
    Vec3(int x) { TA_VEC3_ZERO_PADDING(); };
    Vec3(const Vec3& v3Value) { x = v3Value.x; y = v3Value.y; z = v3Value.z; TA_VEC3_ZERO_PADDING(); }
    Vec3(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; TA_VEC3_ZERO_PADDING(); }

	void Initialise(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; TA_VEC3_ZERO_PADDING(); }
	

    
	
    operator float* () { return (float*)&x; }
    operator const float* () const { return (float*)&x; }
	

	
	
    float& operator [] (int nIndex) { return ((float*)&x)[nIndex]; }
    const float& operator [] (int nIndex) const { return ((float*)&x)[nIndex]; }
	

    
	
    Vec3& operator += (const Vec3& v3Value) { x += v3Value.x; y += v3Value.y; z += v3Value.z; return *this; }
    Vec3& operator -= (const Vec3& v3Value) { x -= v3Value.x; y -= v3Value.y; z -= v3Value.z; return *this; }
    Vec3& operator *= (float fValue) { x *= fValue; y *= fValue; z *= fValue; return *this; }
    Vec3& operator /= (float fValue) { float fDiv = 1.0f / fValue; x *= fDiv; y *= fDiv; z *= fDiv; return *this; }
	

    
	
    Vec3 operator + () const { return *this; };
    Vec3 operator - () const { Vec3 v3ReturnValue(-x, -y, -z); return v3ReturnValue; };
	

    
	
    Vec3 operator + (const Vec3& v3Value) const { Vec3 v3ReturnValue(x + v3Value.x, y + v3Value.y, z + v3Value.z); return v3ReturnValue; }
    Vec3 operator - (const Vec3& v3Value) const { Vec3 v3ReturnValue(x - v3Value.x, y - v3Value.y, z - v3Value.z); return v3ReturnValue; }
    Vec3 operator * (float fValue) const { Vec3 v3ReturnValue(x * fValue, y * fValue, z * fValue); return v3ReturnValue; }
    Vec3 operator / (float fValue) const { float fDiv = 1.0f / fValue; Vec3 v3ReturnValue(x * fDiv, y * fDiv, z * fDiv); return v3ReturnValue; }
	

    friend Vec3 operator * (float fValue, const Vec3& v3Value);

	
	
    bool operator == (const Vec3& v3Value) const { return x == v3Value.x && y == v3Value.y && z == v3Value.z; }
	bool operator != (const Vec3& v3Value) const { return x != v3Value.x || y != v3Value.y || z != v3Value.z; }
	

	
	static inline Vec3 TA_VEC3_CALL Cross(const Vec3& v3A, const Vec3& v3B)
	{
		Vec3 v3ReturnValue;
		v3ReturnValue.x = v3A.y * v3B.z - v3A.z * v3B.y;
		v3ReturnValue.y = v3A.z * v3B.x - v3A.x * v3B.z;
		v3ReturnValue.z = v3A.x * v3B.y - v3A.y * v3B.x;
		return v3ReturnValue;
	}
	inline Vec3 Cross(const Vec3& v3Value) const { return Cross(*this, v3Value); }
	static inline Vec3 TA_VEC3_CALL CrossWithUnitX(const Vec3& v3Value) { return Vec3(0.0f, v3Value.z, -v3Value.y); }
	inline Vec3 CrossWithUnitX() const { return CrossWithUnitX(*this); }
	static inline Vec3 TA_VEC3_CALL CrossWithUnitY(const Vec3& v3Value) { return Vec3(-v3Value.z, 0.0f, v3Value.x); }
	inline Vec3 CrossWithUnitY() const { return CrossWithUnitY(*this); }
	static inline Vec3 TA_VEC3_CALL CrossWithUnitZ(const Vec3& v3Value) { return Vec3(v3Value.y, -v3Value.x, 0.0f); }
	inline Vec3 CrossWithUnitZ() const { return CrossWithUnitZ(*this); }
	static inline float TA_VEC3_CALL CrossX(const Vec3& v3A, const Vec3& v3B) { return v3A.y * v3B.z - v3A.z * v3B.y; }
	inline float CrossX(const Vec3& v3Value) const { return CrossX(*this, v3Value); }
	static inline float TA_VEC3_CALL CrossY(const Vec3& v3A, const Vec3& v3B) { return v3A.z * v3B.x - v3A.x * v3B.z; }
	inline float CrossY(const Vec3& v3Value) const { return CrossY(*this, v3Value); }
	static inline float TA_VEC3_CALL CrossZ(const Vec3& v3A, const Vec3& v3B) { return v3A.x * v3B.y - v3A.y * v3B.x; }
	inline float CrossZ(const Vec3& v3Value) const { return CrossZ(*this, v3Value); }


	
	static inline float TAC_CALL Dot(const Vec3& v3A, const Vec3& v3B)
	{
		return v3A.x * v3B.x + v3A.y * v3B.y + v3A.z * v3B.z;
	}
	inline float Dot(const Vec3& v3Value) const { return Dot(*this, v3Value); }

	
	static inline Vec3 TA_VEC3_CALL Min(const Vec3& v3A, const Vec3& v3B)
	{
		Vec3 v3Result;
		v3Result.x = v3A.x < v3B.x ? v3A.x : v3B.x;
		v3Result.y = v3A.y < v3B.y ? v3A.y : v3B.y;
		v3Result.z = v3A.z < v3B.z ? v3A.z : v3B.z;
		return v3Result;
	}
	static inline Vec3 TA_VEC3_CALL Max(const Vec3& v3A, const Vec3& v3B)
	{
		Vec3 v3Result;
		v3Result.x = v3A.x > v3B.x ? v3A.x : v3B.x;
		v3Result.y = v3A.y > v3B.y ? v3A.y : v3B.y;
		v3Result.z = v3A.z > v3B.z ? v3A.z : v3B.z;
		return v3Result;
	}

	static inline float TA_VEC3_CALL GetMagnitude(const Vec3& v3Value) { return Sqrt(v3Value.x * v3Value.x + v3Value.y * v3Value.y + v3Value.z * v3Value.z); }
	inline float GetMagnitude() const { return Sqrt(x * x + y * y + z * z); }
	static inline float TA_VEC3_CALL GetMagnitudeSqrd(const Vec3& v3Value) { return v3Value.GetMagnitudeSqrd(); }
	inline float GetMagnitudeSqrd() const { return x * x + y * y + z * z; }
	static inline Vec3 TA_VEC3_CALL GetNormal(const Vec3& v3Value) { return v3Value.GetNormal(); }
	inline Vec3 GetNormal() const { return (*this) * ReciprocalSqrt(GetMagnitudeSqrd()); }
	inline void Normalise() { (*this) *= ReciprocalSqrt(GetMagnitudeSqrd()); }
	inline void Clear() { x = 0.0f; y = 0.0f; z = 0.0f; TA_VEC3_ZERO_PADDING(); }
	inline bool IsNormalised() const { return IsEqualToOneWithInError(GetMagnitudeSqrd()); }
	inline bool IsZero() const { return GetMagnitudeSqrd() == 0.0f; }
	inline int GetGreatestAxis() const;
	inline void GetAxisOrder(int pnAxisOrder[3]) const; 
	inline float GetAxis(int nIndex) const { return (*this)[nIndex]; }
	inline float GetMax() const { return TA::Max(TA::Max(x, y), z); }
	inline float GetMin() const { return TA::Min(TA::Min(x, y), z); }
	static inline const Vec3& TA_VEC3_CALL GetUnitVector(int nIndex);
	inline bool IsEqualWithInError(const Vec3& v3Value, float fError) const;
	inline bool IsValid() const { return FloatIsOK(x) && FloatIsOK(y) && FloatIsOK(z); }
	
	
};


const Vec3 k_v3Zero(0.0f, 0.0f, 0.0f);
const Vec3 k_v3UnitX(1.0f, 0.0f, 0.0f);
const Vec3 k_v3UnitY(0.0f, 1.0f, 0.0f);
const Vec3 k_v3UnitZ(0.0f, 0.0f, 1.0f);



}

#include "../Common/zcta011.inl"

#endif

#endif 
