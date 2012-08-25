//---------------------------------------------------------------------------------
// File Name: CollisionObjectCylinder.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISIONOBJECTCYLINDER_H
#define TA_COLLISIONOBJECTCYLINDER_H

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

class lOlOllllOlO CollisionObjectCylinder : public CollisionObjectSimple
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

	static CollisionObjectCylinder* TAC_CALL CreateNew();

	

#ifndef DOXYGEN
	friend void TAC_CALL llllOOllOlO(
		const CollisionObjectSimple& lOOOllllOlOO,
		const MFrame& lOllOllllll,
		const MFrame& lOlllOOOOOO,
		bool lOllOlllOOl,
		const CollisionObjectSimple& lOOOllllOlOl,
		lOllOllOllO* lOlllOOOOOl);

	lOlllOOOOlO(llllOOllOlO)
	lOlllOOOOlO(llllOOlOlOl)
	lOlllOOOOlO(llllOOllOOl)
	lOlllOOOOlO(llllOOOOOll)

	lOlllOOOlOO(llllOOlllll)

	lOlllOOOOlO(llllOOlOlll)
	
#endif 

private:
	float lOlOlOOlOOO;
	Vec3 lllOlOlOOOO;
	Vec3 lllOlOlOOOl;

	CollisionObjectCylinder();
	~CollisionObjectCylinder();
	virtual const char* llOllOOlOOO() const { return "TAColObjCylinder"; }
};

inline const Vec3& CollisionObjectCylinder::GetPointA() const { return lllOlOlOOOO; }
inline const Vec3& CollisionObjectCylinder::GetPointB() const { return lllOlOlOOOl; }
inline float CollisionObjectCylinder::GetRadius() const { return lOlOlOOlOOO; }

#ifndef DOXYGEN
lOllOllllOl(llllOOllOlO)
lOllOllllOl(llllOOllOOl)
lOllOllllOl(llllOOOOOll)

lOlllOOOOll(llllOOlllll)

lOllOllllOl(llllOOlOlll)
#endif 

}


#endif 
