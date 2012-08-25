//---------------------------------------------------------------------------------
// File Name: Geometry.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_GEOMETRY_H
#define TA_GEOMETRY_H

#ifndef TA_COMMON_H
#include "Common.h"
#endif 



namespace TA
{

struct Vec2;
struct Vec3;



class TACOMMON_CLASS Geometry
{
public:
	
	
	
	static bool TAC_CALL LineIntersectsBox(
		const Vec2& v2A,			
		const Vec2& v2B,			
		const Vec2& v2Center,		
		const Vec2& v2Extent);		

	
	
	static float TAC_CALL CubicInterpolate(float fPosA, float fPosB, float fVelA, float fVelB, float fTime);

	
	
	static Vec3 TAC_CALL CubicInterpolate(const Vec3& v3PosA, const Vec3& v3PosB, const Vec3& v3VelA, const Vec3& v3VelB, float fTime);

	
	
	static float TAC_CALL CubicInterpolateGradient(float fPosA, float fPosB, float fVelA, float fVelB, float fTime);

	
	
	static bool TAC_CALL Geometry::TestEdgeMovementAgainstEdge(
		const Vec3& v3A0,
		const Vec3& v3A1,
		const Vec3& v3B0,
		const Vec3& v3B1,
		const Vec3& v3C0,
		const Vec3& v3C1,
		float& fResultTime,
		Vec3& v3ResultPos,
		Vec3& v3ResultNormal);
	
	
	
	static bool TAC_CALL Geometry::TestLineMovementAgainstCapsule(
		const Vec3& v3A0,
		const Vec3& v3A1,
		const Vec3& v3B0,
		const Vec3& v3B1,
		const Vec3& v3C0,
		const Vec3& v3C1,
		float fRadius,
		float& fResultTime,
		Vec3& v3ResultPos,
		Vec3& v3ResultNormal);

	
	
	static void TAC_CALL Geometry::FindClosestPointOnLineToLine(
		const Vec3& v3A0,
		const Vec3& v3A1,
		const Vec3& v3B0,
		const Vec3& v3B1,
		Vec3& v3ResultPosOnA,
		Vec3& v3ResultPosOnB);
	
	
	
	
	static bool TAC_CALL Geometry::FindClosestPointOnLineToLineUnClamped(
		const Vec3& v3A0,
		const Vec3& v3A1,
		const Vec3& v3B0,
		const Vec3& v3B1,
		Vec3& v3ResultPosOnA,
		Vec3& v3ResultPosOnB);

	
	
	static bool TAC_CALL Geometry::TestPointMovementAgainstSphere(
		const Vec3& v3A0,
		const Vec3& v3A1,
		const Vec3& v3B,
		float fRadius,
		float& fResultTime,
		Vec3& v3ResultPos,
		Vec3& v3ResultNormal);

	
	
	static bool TAC_CALL Geometry::TestSphereMovementAgainstLine(
		const Vec3& v3A0,
		const Vec3& v3A1,
		const Vec3& v3B0,
		const Vec3& v3B1,
		float fRadius,
		float& fResultTime,
		Vec3& v3ResultPos,
		Vec3& v3ResultNormal);

	
	
	static bool TAC_CALL Geometry::TestLineMovementAgainstSphere(
		const Vec3& v3A0,
		const Vec3& v3A1,
		const Vec3& v3B0,
		const Vec3& v3B1,
		const Vec3& v3C,
		float fRadius,
		float& fResultTime,
		Vec3& v3ResultPos,
		Vec3& v3ResultNormal);

	
	
	static Vec3 TAC_CALL Geometry::FindClosestPointOnLine(
		const Vec3& v3Point,
		const Vec3& v3LineA,
		const Vec3& v3LineB);

	
	
	static float TAC_CALL Geometry::FindShortestDistanceToLineSqrd(
		const Vec3& v3Point,
		const Vec3& v3LineA,
		const Vec3& v3LineB);

	
	
	static float TAC_CALL Geometry::FindShortestDistanceToLine(
		const Vec3& v3Point,
		const Vec3& v3LineA,
		const Vec3& v3LineB);

	
	
	static bool TAC_CALL Geometry::FindClosestPointOnLineUnClamped(
		const Vec3& v3Point,
		const Vec3& v3LineA,
		const Vec3& v3LineB,
		Vec3& v3ResultPos);	

	
	
	static bool TAC_CALL Geometry::TestPointAgainstCone(
		const Vec3& v3Point,
		const Vec3& v3LineA,
		const Vec3& v3LineB,
		float fRadius);

};

};

#endif 
