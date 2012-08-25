//---------------------------------------------------------------------------------
// File Name: CollisionObjectCapsule.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISIONOBJECTCAPSULE_H
#define TA_COLLISIONOBJECTCAPSULE_H

#ifndef TA_COLLISION_H
#include "Collision.h"
#endif 

#ifndef TA_COLLISIONOBJECTSIMPLE_H
#include "CollisionObjectSimple.h"
#endif 



namespace TA
{

struct MFrame;
class CollisionObjectCachedPolyData;

class lOlOllllOlO CollisionObjectCapsule : public CollisionObjectSimple
{
public:
	
	void Initialise(
		const Vec3& v3PointA,
		const Vec3& v3PointB,
		float fRadius);
	void Finalise();
	
	
#ifndef TA_WRAPPER	
	void Serialise(Serialiser& serialiser, const Version& version);
#endif 
	
	bool Serialisable() const { return true; }

	void Render(const MFrame& mFrame);
	AABB CalculateBoundingBox() const;
	void CalculateMass(float fDensity, float& fMass, Vec3& v3CenterOfMass, Mat33& m33Inertia) const;

	const Vec3& GetPointA() const;
	const Vec3& GetPointB() const;
	float GetRadius() const;
	
	virtual bool TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3Normal,
		float fLength,
		Collision& collision) const;

	static CollisionObjectCapsule* TAC_CALL CreateNew();
	

#ifndef DOXYGEN
	lOlllOOOOlO(lllOllOlOll)
	lOlllOOOOlO(llllOOOOOlO)	
	lOlllOOOOlO(lllOlllOlll)
	lOlllOOOOlO(llllOOOOOll)
	lOlllOOOOlO(lllOllllllO)

	lOlllOOOlOO(lllOllllOll)
#endif 


private:
	float lOlOlOOlOOO;
	Vec3 lllOlOlOOOO;
	Vec3 lllOlOlOOOl;

	CollisionObjectCapsule();
	~CollisionObjectCapsule();
	virtual const char* llOllOOlOOO() const { return "TAColObjCapsule"; }
};

inline const Vec3& CollisionObjectCapsule::GetPointA() const { return lllOlOlOOOO; }
inline const Vec3& CollisionObjectCapsule::GetPointB() const { return lllOlOlOOOl; }
inline float CollisionObjectCapsule::GetRadius() const { return lOlOlOOlOOO; }

#ifndef DOXYGEN
lOllOllllOl(lllOllOlOll)
lOllOllllOl(lllOlllOlll)
lOllOllllOl(lllOllllllO)

lOlllOOOOll(lllOllllOll)
#endif 

};

#endif 
