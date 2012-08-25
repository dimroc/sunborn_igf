//---------------------------------------------------------------------------------
// File Name: Collision.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISION_H
#define TA_COLLISION_H

#ifndef TA_MFRAME_H
#include "../Common/MFrame.h"
#endif 



namespace TA
{

class DynamicObject;
class StaticObject;
class CollisionObject;
class PhysicsJoint;
	
class Collision
{
public:
#ifndef DOXYGEN
	enum Type
	{
		lOllOOllllO =							 0x100000,
		lOllOOlllll =							 0x200000,
		lOllOlOOOOO =						 0x300000,
		TYPE_ROTATION_CONSTRAINT =				 0x400000,
		lOllOlOOOOl =	 0x500000,
		lOllOlOOOlO =				 0x600000,
		lOllOlOOOll =								 0x700000,
		lOllOlOOlOO =								 0x700000,
	};
#endif 

	
	//pl.BeginExportClass(Collision)

	enum Flag
	{
		FLAG_CONSTRAIN_IN_BOTH_DIRECTIONS =		0x1,
		FLAG_FRICTION =							0x2,
		FLAG_FRICTION_GENERATING_IMPULSE =		0x4,
		FLAG_NO_FRICTION =						0x8,
		FLAG_JOINT =							0x10,
		FLAG_JOINT_LIMIT =						0x20,
		FLAG_COLLISION_OCCURRED =				0x40,
		FLAG_DISABLE_PUSHOUT =					0x80,
		FLAG_FRICTION_GENERATING_IMPULSE2 =		0x100,
		FLAG_MARKED =							0x200,
		FLAG_RESTITUTION_CALCULATED =			0x400,
		FLAG_OBJECTA_SOLID = 					0x1000,
		FLAG_OBJECTB_SOLID = 					0x2000,
		FLAG_NEW_COLLISION = 					0x4000,
		FLAG_ROTATION_CONSTRAINT =				0x8000,
		FLAG_OBJECTB_INFINITE_MASS =			0x10000,
		FLAG_DEPTH_CALCULATED_FROM_TIME =		0x20000,
		FLAG_USE_PUSH_OUT_MODIFIER =			0x40000,
#ifndef DOXYGEN
		FLAG_OBJECTA_SOLID_INTERNAL = 			0x80000,
		FLAG_OBJECTB_SOLID_INTERNAL = 			0x100000,
#endif 
	};

#ifndef TA_WRAPPER	
	Collision();
#endif 
	void Clear();
	void Initialise(
		DynamicObject* pNewObjectA,
		DynamicObject* pNewObjectB,
		float fNewTime,
		float fNewIntersectingDepth,
		const Vec3& v3NewPosition,
		const Vec3& v3NewNormal);
	
#ifndef DOXYGEN
#ifndef TA_WRAPPER
	inline void Initialise(
		Type eNewType,
		float fNewTime,
		float fNewIntersectingDepth,
		const Vec3& v3NewPosition,
		const Vec3& v3NewNormal,
		PhysicsJoint* pJoint = 0);
#endif 
#endif 

	const Vec3& GetPosition() const;
	void SetPosition(const Vec3& v3Position);
	const Vec3& GetNormal() const;
	void SetNormal(const Vec3& v3Normal);
	
	DynamicObject* GetObjectA() const;
	DynamicObject* GetObjectB() const;
	void SetObjectA(DynamicObject* pDynamicObject);
	void SetObjectB(DynamicObject* pDynamicObject);

	CollisionObject* GetCollisionObjectA();
	CollisionObject* GetCollisionObjectB();
	const CollisionObject* GetCollisionObjectA() const;
	const CollisionObject* GetCollisionObjectB() const;
	void SetCollisionObjectA(CollisionObject* pCollisionObjectA);
	void SetCollisionObjectB(CollisionObject* pCollisionObjectB);

	int GetFlags() const;
	void SetFlags(u32 nFlags);
	u32& Flags();
	float GetRestitution() const;
	void SetRestitution(float fRestitution);
	float GetImpulse() const;
	void SetImpulse(float fImpulse);
	float GetFrictionCoefficient() const;
	void SetFrictionCoefficient(float fFrictionCoefficient);
	float GetTime() const;
	void SetTime(float fTime);
	float GetIntersectingDepth() const;
	void SetIntersectingDepth(float fIntersectingDepth);
	u32 GetCollisionIdA() const;
	void SetCollisionIdA(u32 nCollisionIdA);
	u32 GetCollisionIdB() const;
	void SetCollisionIdB(u32 nCollisionIdB);
	u32 GetAttributeA() const;
	void SetAttributeA(u32 nAttributeA);
	u32 GetAttributeB() const;
	void SetAttributeB(u32 nAttributeB);

	
	PhysicsJoint* GetJoint();
	const PhysicsJoint* GetJoint() const;

	bool CollisionOccurred() const;
	Collision* GetNext();

	//pl.EndExportClass(Collision)
	
	

#ifdef TA_PHYSICS_INTERNAL

	void lOllOlOOlOl(Collision* lOllOlOOllO) { lOOOOOOllll = lOllOlOOllO; }

	u32 GetType() { return lOlOllOOOl & lOllOlOOlOO; }
	void lOllOlOOlll(u32 lOllOlOlOOO) { lOlOllOOOl &= ~lOllOlOOlOO; lOlOllOOOl |= (lOllOlOlOOO & lOllOlOOlOO); }

	
	
	DynamicObject* Collision::lOllOlOlOOl() 
	{ 
		if ((lOlOllOOOl & (FLAG_OBJECTB_SOLID_INTERNAL | FLAG_OBJECTB_INFINITE_MASS)) == 0)
			return lOllOOOllOO;
		else
			return 0;
	}

	
	
	inline void Collision::lOllOlOlOlO()
	{
		TA_ASSERT(lOllOlOlOOl());
		Swap(lOllOOOlOll, lOllOOOllOO);
		lOOOlOOOOO = -lOOOlOOOOO;
		Swap(lOllOOOllll, lOllOOlOOOO);
		Swap(lOllOOOllOl, lOllOOOlllO);

	#ifdef _DEBUG
		int lOllOlOlOll = lOlOllOOOl;
	#endif
		
		TA_ASSERT((FLAG_OBJECTB_SOLID >> 1) == FLAG_OBJECTA_SOLID);
		TA_ASSERT((FLAG_OBJECTB_SOLID_INTERNAL >> 1) == FLAG_OBJECTA_SOLID_INTERNAL);
		const int lOllOlOllOO = (FLAG_OBJECTA_SOLID | FLAG_OBJECTB_SOLID | FLAG_OBJECTA_SOLID_INTERNAL | FLAG_OBJECTB_SOLID_INTERNAL);
		int lOllOlOllOl = lOlOllOOOl & lOllOlOllOO;
		lOllOlOllOl = (((lOllOlOllOl << 2) + lOllOlOllOl) >> 1) & lOllOlOllOO;
		lOlOllOOOl &= ~lOllOlOllOO;
		lOlOllOOOl |= lOllOlOllOl;

		lOllOOlOllO.Clear();
		lOllOOlOlll.Clear();
		
	#ifdef _DEBUG
		TA_ASSERT(
			((lOllOlOlOll & FLAG_OBJECTB_SOLID) && (lOlOllOOOl & FLAG_OBJECTA_SOLID)) ||
			((!(lOllOlOlOll & FLAG_OBJECTB_SOLID)) && (!(lOlOllOOOl & FLAG_OBJECTA_SOLID))));
		TA_ASSERT(
			((lOllOlOlOll & FLAG_OBJECTB_SOLID) && (lOlOllOOOl & FLAG_OBJECTA_SOLID)) ||
			((!(lOllOlOlOll & FLAG_OBJECTB_SOLID)) && (!(lOlOllOOOl & FLAG_OBJECTA_SOLID))));
		TA_ASSERT(
			((lOllOlOlOll & FLAG_OBJECTB_SOLID_INTERNAL) && (lOlOllOOOl & FLAG_OBJECTA_SOLID_INTERNAL)) ||
			((!(lOllOlOlOll & FLAG_OBJECTB_SOLID_INTERNAL)) && (!(lOlOllOOOl & FLAG_OBJECTA_SOLID_INTERNAL))));
		TA_ASSERT(
			((lOllOlOlOll & FLAG_OBJECTB_SOLID_INTERNAL) && (lOlOllOOOl & FLAG_OBJECTA_SOLID_INTERNAL)) ||
			((!(lOllOlOlOll & FLAG_OBJECTB_SOLID_INTERNAL)) && (!(lOlOllOOOl & FLAG_OBJECTA_SOLID_INTERNAL))));
	#endif
	}
#endif 



	
	friend void lOllOlOlllO(Collision* lOllOlOllll, Collision* lOOOOOOllll);

#ifndef lOllOllOOOO
private:
#endif 

	friend class PreCollision;
	friend class PostCollision;

	Vec3 lOOOOlllll;
	Vec3 lOOOlOOOOO;
	
	Vec4 lOllOOlOllO;
	Vec4 lOllOOlOlll;

	u32 lOlOllOOOl;
	DynamicObject* lOllOOOlOll;
	DynamicObject* lOllOOOllOO;
	Collision* lOOOOOOllll;

	float lOllOOlOOll;
	float lOllOOlOlOl;
	float lOllOOlOOlO;
	float lOllOOlOlOO;

	float lllOllllOO;
	float lOllOOlOOOl;
	u32 lOllOllOOOl;
	u32 lOllOllOOlO;
	
	CollisionObject* lOllOOOllOl;
	CollisionObject* lOllOOOlllO;
	u32 lOllOOOllll;
	u32 lOllOOlOOOO;
	

	Collision* lOllOllOOll() const { return (Collision*)this; }
};

#ifndef DOXYGEN
typedef bool (TAC_CALL *lOllOllOlOO)(Collision& lOllOllOlOl, void* lOOlllOOOO);
struct lOllOllOllO
{
	
	
	lOllOllOlOO lOllOllOlll;
	void* lOllOlllOOO;
	bool lOllOlllOOl;
	bool lOllOlllOlO;
	u32 lOllOlllOll;
	CollisionObject* lOllOOOllOl;
	CollisionObject* lOllOOOlllO;
	MFrame lOllOllllOO;
};
#endif 

#define lOllOllllOl(lOllOlllllO)			\
	void TAC_CALL lOllOlllllO(										\
		const MFrame& lOllOllllll,									\
		const MFrame& lOlllOOOOOO,									\
		lOllOllOllO* lOlllOOOOOl);

#define lOlllOOOOlO(lOllOlllllO)	\
	friend lOllOllllOl(lOllOlllllO)

#define lOlllOOOOll(lOllOlllllO)			\
	void TAC_CALL lOllOlllllO(										\
		const MFrame& lOllOllllll,									\
		const MFrame& lOlllOOOOOO,									\
		lOllOllOllO* lOlllOOOOOl);

#define lOlllOOOlOO(lOllOlllllO)	\
	friend lOllOllllOl(lOllOlllllO)


class PreCollision
{
public:
	const DynamicObject* GetObjectA() const;
	const DynamicObject* GetObjectB() const;
	const CollisionObject* GetCollisionObjectA() const;
	const CollisionObject* GetCollisionObjectB() const;
	const PhysicsJoint* GetJoint() const;

	PreCollision* GetNext();

	const Vec3& GetPosition() const;
	void SetPosition(const Vec3& v3Position);
	const Vec3& GetNormal() const;
	void SetNormal(const Vec3& v3Normal);

	u32 GetFlags() const;
	void SetFlags(u32 nFlags);
	
	float GetRestitution() const;
	void SetRestitution(float fRestitution);
	float GetFrictionCoefficient() const;
	void SetFrictionCoefficient(float fFrictionCoefficient);

	float GetTime() const;
	void SetTime(float fTime);
	float GetIntersectingDepth() const;
	void SetIntersectingDepth(float fIntersectingDepth);

	u32 GetAttributeA() const;
	u32 GetAttributeB() const;

	u32 GetCollisionIdA() const;
	u32 GetCollisionIdB() const;

	
private:
	friend class Physics;
	Collision lOlllOOOlOl;
	PreCollision() {};
	PreCollision(const PreCollision&) {};
};


class PostCollision
{
public:
	DynamicObject* GetObjectA();
	DynamicObject* GetObjectB();
	CollisionObject* GetCollisionObjectA();
	CollisionObject* GetCollisionObjectB();
	PhysicsJoint* GetJoint();


	PostCollision* GetNext();

	const Vec3& GetPosition() const;
	const Vec3& GetNormal() const;

	u32 GetFlags() const;

	float GetImpulse() const;
	float GetTime() const;
	float GetIntersectingDepth() const;

	u32 GetAttributeA() const;
	u32 GetAttributeB() const;

	u32 GetCollisionIdA() const;
	u32 GetCollisionIdB() const;

	

private:
	friend class Physics;
	Collision lOlllOOOlOl;
	PostCollision() {};
	PostCollision(const PostCollision&) {};

};

}

#include "../Physics/zpta001.inl"

#endif 
