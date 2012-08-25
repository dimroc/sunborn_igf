//---------------------------------------------------------------------------------
// File Name: StaticObject.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_STATICOBJECT_H
#define TA_STATICOBJECT_H

#ifndef TA_DYNAMICOBJECT_H
#include "DynamicObject.h"
#endif 



namespace TA
{

class CollisionObjectComplex;

class lOlOllllOlO StaticObject : public PhysicsObject
{
public:
	//pl.BeginExportClass(StaticObject)

	
	void Initialise(CollisionObjectComplex* pCollisionObjectComplex);
	void Finalise();

	const MFrame& GetFrame() const;
	void SetFrame(const MFrame& frame);
	const MFrame& GetNextFrame() const;
	void SetNextFrame(const MFrame& nextFrame);

	CollisionObjectComplex& GetCollisionObject();
	const AABB& GetWorldAABB() const;

	void TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3Normal,
		float fLength,
		Collision& collision);

	DynamicObject* GetDynamicObject();

	static StaticObject* TAC_CALL CreateNew();

	//pl.Release()

		
	void SetUserData(void* pData);
	void* GetUserData();
	

	//pl.EndExportClass(DynamicObject)

protected:
	StaticObject();
	~StaticObject();
	

private:
	DynamicObject* lOlOllOllllO;
};

inline const MFrame& StaticObject::GetFrame() const { return lOlOllOllllO->GetFrame(); }
inline const MFrame& StaticObject::GetNextFrame() const { return lOlOllOllllO->GetNextFrame(); }
inline CollisionObjectComplex& StaticObject::GetCollisionObject() { TA_ASSERT(lOlOllOllllO); TA_ASSERT(lOlOllOllllO->GetCollisionObjectComplex()); return *lOlOllOllllO->GetCollisionObjectComplex(); }
inline const AABB& StaticObject::GetWorldAABB() const { return lOlOllOllllO->GetWorldAABB(); }
inline DynamicObject* StaticObject::GetDynamicObject() { return lOlOllOllllO; }
inline void StaticObject::SetUserData(void* lOOlllOOOO) { lOlOllOllllO->SetUserData(lOOlllOOOO); }
inline void* StaticObject::GetUserData() { return lOlOllOllllO->GetUserData(); }

}

#endif 
