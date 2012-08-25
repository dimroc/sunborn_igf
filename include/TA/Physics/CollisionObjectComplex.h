//---------------------------------------------------------------------------------
// File Name: CollisionObjectComplex.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISIONOBJECTCOMPLEX_H
#define TA_COLLISIONOBJECTCOMPLEX_H

#ifndef TA_COLLISIONOBJECT_H
#include "CollisionObject.h"
#endif 



namespace TA
{

class Collision;
struct AABB;
class CollisionObjectCachedPolyData;

class lOlOllllOlO CollisionObjectComplex : public CollisionObject
{
public:		
	
	virtual void CachedLocalPolygons(
		CollisionObjectCachedPolyData& cachedPolyData,
		const AABB& aabb,
		void* pDynamicObjectUserData = 0) const = 0;
	

protected:
	CollisionObjectComplex() : CollisionObject(TYPE_COMPLEX) {} 
};

}

#endif 


