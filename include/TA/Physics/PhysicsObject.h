//---------------------------------------------------------------------------------
// File Name: PhysicsObject.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_PHYSICSOBJECT_H
#define TA_PHYSICSOBJECT_H

#ifndef TA_REFCOUNT_H
#include "../Common/RefCount.h"
#endif 

#ifndef lOlOllllOOl
#include "../Physics/zpta012.h"
#endif 



namespace TA
{

class Physics;

class lOlOllllOlO PhysicsObject : public RefCount
{
public:
	
	bool AddedToPhysics() const;

protected:
	PhysicsObject();
	void Initialise();
	void Finalise();
	

private:
	friend class Physics;
	void lOlllOlOOOOO(bool lllOllOOOO) { llOlOOlOlOll = lllOllOOOO; }

	bool llOlOOlOlOll;
};

inline bool PhysicsObject::AddedToPhysics() const { return llOlOOlOlOll; }
inline PhysicsObject::PhysicsObject() : RefCount() { llOlOOlOlOll = false; }
inline void PhysicsObject::Initialise() { llOlOOlOlOll = false; }
inline void PhysicsObject::Finalise() { llOlOOlOlOll = false; }

}

#endif 

