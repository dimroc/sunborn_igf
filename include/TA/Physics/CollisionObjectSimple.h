//---------------------------------------------------------------------------------
// File Name: CollisionObjectSimple.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISIONOBJECTSIMPLE_H
#define TA_COLLISIONOBJECTSIMPLE_H

#ifndef TA_COLLISIONOBJECT_H
#include "CollisionObject.h"
#endif 



namespace TA
{

class Collision;
struct Mat33;

class lOlOllllOlO CollisionObjectSimple : public CollisionObject
{
public:
	
	virtual AABB CalculateBoundingBox() const = 0;
	
	virtual const AABB& GetAABB() const { return NoGetAABB(); }

	virtual void CalculateMass(float fDensity, float& fMass, Vec3& v3CenterOfMass, Mat33& m33Inertia) const = 0;

protected:
	CollisionObjectSimple(int nType) : CollisionObject(nType) {}
	
};

}

#endif 
