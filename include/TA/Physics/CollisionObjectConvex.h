//---------------------------------------------------------------------------------
// File Name: CollisionObjectConvex.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISIONOBJECTCONVEX_H
#define TA_COLLISIONOBJECTCONVEX_H

#ifndef TA_REFCOUNT_H
#include "../Common/RefCount.h"
#endif 

#ifndef TA_AABB_H
#include "../Common/AABB.h"
#endif 

#ifndef TA_COLLISION_H
#include "Collision.h"
#endif 

#ifndef TA_COLLISIONOBJECTSIMPLE_H
#include "CollisionObjectSimple.h"
#endif 



namespace TA
{


struct MFrame;
struct AABB;
class ConvexHull;
class CollisionObjectCachedPolyData;

class lOlOllllOlO CollisionObjectConvex : public CollisionObjectSimple
{
public:
	struct Data;
	struct lllllOlOlOl;
	struct Polygon;
	struct lllllOlOlll;
	struct Vertex;

	
	void Initialise();
	void InitialiseAsABox(float fSizeX, float fSizeY, float fSizeZ);
	void InitialiseAsABox(const AABB& aabb);
	void InitialiseAsAnOrientedBox(const AABB& aabb, const MFrame& mFrame);
	bool InitialiseFromPointList(const Vec3* pv3PointList, int nNumPoints);
	void InitialiseFromPlaneList(
		const Vec3* pv3PlaneNormalList, 
		const Vec3* pv3PlanePointList, 
		int nNumPlanes);
	void InitialiseFromConvexHull(ConvexHull& convexHull);
	void Finalise();
	
	
	#ifndef TA_WRAPPER	
	void Serialise(Serialiser& serialiser, const Version& version);
	#endif 
	
	bool Serialisable() const { return true; }

	AABB CalculateBoundingBox() const;
	void CalculateMass(float fDensity, float& fMass, Vec3& v3CenterOfMass, Mat33& m33Inertia) const;

	void Transform(const MFrame& frame);
	void Render(const MFrame& mFrame);

	virtual bool TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3Normal,
		float fLength,
		Collision& collision) const;

	static CollisionObjectConvex* TAC_CALL CreateNew();

	

#ifndef DOXYGEN
	lOlllOOOOlO(llllOOlOOlO)
	lOlllOOOOlO(llllOOOOOlO)
	lOlllOOOOlO(llllOOlOlOl)
	lOlllOOOOlO(llllOOlOlOO)	
	lOlllOOOOlO(llllOOlOOll)

	lOlllOOOlOO(lllOlOOllOO)

	const Data* GetData() const { return lOlOOlOllO; }
#endif 



private:
	Data* lOlOOlOllO;
#ifdef lOOOOOOOllOO
	mutable Polygon* lOOOOOOOllOl;
#endif 

	CollisionObjectConvex();
	~CollisionObjectConvex();
	void InitialiseCommon();
	void llllllOOlOO(const AABB& lOOOOOOlll, const MFrame* lOOOOOOlOOlO);

	virtual const char* llOllOOlOOO() const { return "TAColObjConvex"; }
	
	void llllllOllOO();
	void llllllOlOlO();
	
	void lOOOOOOlllOO(
		int lOOOOOOlllOl, 
		const Vec3* lOOOOOOOOlOl,
		int& lOOOOOOllllO,
		int& lOOOOOOlllll,
		float& lOOOOOlOOOOO) const;
	
	void lOOOOOlOOOll(
		int lOOOOOOlllOl,
		const Vec3* lOOOOOlOOlOO,
		const Vec3* lOOOOOlOOlOl,
		Polygon& lOllOOOOll,
		lOllOllOllO* lOlllOOOOOl,
		const MFrame& lOOOOOOOOl,
		float lOOOOOlOOllO) const;
	
	float lOOOOOlOOlll(		
		const Vec3* lOOOOOlOlOOO,
		const CollisionObjectConvex& lllOllOllOl,
		int& lOOOOOlOlOOl,
		int& lOOOOOlOlOlO) const;

	int lOOOOOllOOOl(	
		int lOOOOOOlllll, 
		const Vec3& lOOOOOllOOlO) const;

	int lOOOOOllOllO(const Vec3& lOOOOOllOOlO) const;
};

#ifndef DOXYGEN
lOllOllllOl(llllOOlOOlO)
lOllOllllOl(llllOOOOOlO)
lOllOllllOl(llllOOlOlOl)
lOllOllllOl(lOOOlOOOOlll)
lOllOllllOl(llllOOlOOll)
#endif 

}

#endif 
