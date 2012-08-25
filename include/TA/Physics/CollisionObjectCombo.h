//---------------------------------------------------------------------------------
// File Name: CollisionObjectCombo.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISIONOBJECTCOMBO_H
#define TA_COLLISIONOBJECTCOMBO_H

#ifndef TA_REFCOUNT_H
#include "../Common/RefCount.h"
#endif 

#ifndef TA_TYPES_H
#include "../Common/Types.h"
#endif 

#ifndef TA_AABB_H
#include "../Common/AABB.h"
#endif 

#ifndef TA_COLLISION_H
#include "Collision.h"
#endif 

#ifndef lOlOllllOOl
#include "../Physics/zpta012.h"
#endif 

#ifndef TA_COLLISIONOBJECTSIMPLE_H
#include "CollisionObjectSimple.h"
#endif 

#ifndef TA_COLLISIONOBJECTLINELIST_H
#include "CollisionObjectLineList.h"
#endif 



namespace TA
{

struct MFrame;
class CollisionObjectComplex;
class CollisionObject;
struct llllOllOllO;
struct llllOllOlOO;

class lOlOllllOlO CollisionObjectCombo : public CollisionObject
{
public:
	struct Data;
	

	void Initialise();
	void InitialiseAsABox(float fSizeX, float fSizeY, float fSizeZ);
	void InitialiseAsABox(const AABB& aabb);
	void InitialiseAsAnOrientedBox(const AABB& aabb, const MFrame& mFrame);
	void InitialiseAsACapsule(const Vec3& v3Start, const Vec3& v3End, float fRadius);	
	void InitialiseAsASphere(const Vec3& v3Center, float fRadius);
	void InitialiseAsACylinder(const Vec3& v3Start, const Vec3& v3End, float fRadius);	
	bool InitialiseFromPointList(const Vec3* pv3PointList, int nNumPoints);
	void Finalise();
	
	
	#ifndef TA_WRAPPER	
	void Serialise(Serialiser& serialiser, const Version& version);
	#endif 
	

	virtual bool Serialisable() const { return true; }

	void AddCollisionObject(CollisionObjectSimple* pCollisionObject);
	void RemoveCollisionObject(CollisionObjectSimple* pCollisionObject);
	void RemoveCollisionObject(int nIndex);
	int GetNumCollisionObjects() const;
	CollisionObjectSimple* GetCollisionObject(int nIndex);


	void Render(const MFrame& mFrame);

	const AABB& GetAABB() const;
	void CalculateMass(float fDensity, float& fMass, Vec3& v3CenterOfMass, Mat33& m33Inertia) const;

	bool TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3Normal,
		float fLength,
		Collision& collision) const;
	int GetMaxNumLineCollisions() const;

	static CollisionObjectCombo* TAC_CALL CreateNew();
	

#ifndef DOXYGEN
	friend void TAC_CALL llllOllOOll(
		llllOllOlOO& llllOllOlOl,
		CollisionObjectCombo& lllOllOllOO,
		const MFrame& lOllOllllll,
		const MFrame& lOlllOOOOOO,
		CollisionObjectCombo& lllOllOllOl,
		llllOllOllO* llllOllOlll,
		lOllOllOlOO lllOOOOOlll,
		void* lllOllOlOlO);

	friend void TAC_CALL llllOllOOll(
		llllOllOlOO& llllOllOlOl,
		CollisionObjectCombo& lllllOOOOll,
		const MFrame& lOllOllllll,
		const MFrame& lOlllOOOOOO,
		CollisionObjectComplex& lllllOOOllO,
		lOllOllOlOO lllOOOOOlll,
		void* lllOllOlOlO,
		void* llOllllllOO,
		bool lOllOlllOlO);
#endif 

private:
	
	CollisionObjectCombo();
	~CollisionObjectCombo();
	
	
	void llllOlOllOl();
	virtual const char* llOllOOlOOO() const { return "TAColObjCombo"; }

	CollisionObjectLineList* llllOlOOOOl;
	Data* lOlOOlOllO;
	AABB llllOlOOOOO;
};

inline const AABB& CollisionObjectCombo::GetAABB() const { return llllOlOOOOO; }
inline int CollisionObjectCombo::GetMaxNumLineCollisions() const { return llllOlOOOOl ? llllOlOOOOl->GetNumLines() : 0; }

#ifndef DOXYGEN

void TAC_CALL llllOllOOll(
	llllOllOlOO& llllOllOlOl,
	CollisionObject& lllOllOllOO,
	const MFrame& lOllOllllll,
	const MFrame& lOlllOOOOOO,
	CollisionObject& lllOllOllOl,
	llllOllOllO* llllOllOlll,
	lOllOllOlOO lllOOOOOlll,
	void* lllOllOlOlO);

void TAC_CALL llllOllOOll(
	llllOllOlOO& llllOllOlOl,
	CollisionObjectCombo& lllOllOllOO,
	const MFrame& lOllOllllll,
	const MFrame& lOlllOOOOOO,
	CollisionObjectCombo& lllOllOllOl,
	llllOllOllO* llllOllOlll,
	lOllOllOlOO lllOOOOOlll,
	void* lllOllOlOlO);

void TAC_CALL llllOllOOll(
	llllOllOlOO& llllOllOlOl,
	CollisionObjectCombo& lllllOOOOll,
	const MFrame& lOllOllllll,
	const MFrame& lOlllOOOOOO,
	CollisionObjectComplex& lllllOOOlOO,
	lOllOllOlOO lllOOOOOlll,
	void* lllOllOlOlO,
	void* llOllllllOO,
	bool lllllOOOlll);
#endif 

}

#endif 
