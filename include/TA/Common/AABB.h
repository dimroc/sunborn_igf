//---------------------------------------------------------------------------------
// File Name: AABB.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_AABB_H
#define TA_AABB_H

#ifndef TA_VECTOR_H
#include "Vector.h"
#endif 



namespace TA
{

struct MFrame;

struct TACOMMON_CLASS AABB
{
	
	
	
	
	Vec3 v3Center;
	Vec3 v3Extent;
	

	
	AABB() {}
	AABB(const Vec3& v3NewCenter, const Vec3& v3NewExtent) { Initialise(v3NewCenter, v3NewExtent); }

	
	void Initialise(const Vec3& v3NewCenter, const Vec3& v3NewExtent);
	void InitialiseToFit2Points(const Vec3& v3A, const Vec3& v3B);
	void Clear();

	
	AABB& operator += (const AABB& aabb);
	AABB operator + (const AABB& aabb) const;
	AABB operator * (const MFrame& mFrame) const;
	AABB operator * (float fValue) const;

	
	bool Intersects(const AABB& aabb) const;
	bool Intersects(const Vec3& v3Point) const;
	bool IsInside(const AABB& aabb) const;
	void ExpandToFit(const Vec3& v3Point);
	
};

}

#include "../Common/zcta000.inl"

#endif 
