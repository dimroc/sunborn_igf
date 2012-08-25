//---------------------------------------------------------------------------------
// File Name: Physics.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_PHYSICS_H
#define TA_PHYSICS_H

#ifndef TA_DEBUG_H
#include "../Common/Debug.h"
#endif 

#ifndef lOOOOOOllOl
#include "../Common/zcta008.h"
#endif 

#ifndef lOlOlOlOlO
#include "../Common/zcta009.h"
#endif 

#ifndef TA_VECTOR_H
#include "../Common/Vector.h"
#endif 

#ifndef lOlOllllOOl
#include "../Physics/zpta012.h"
#endif 

#ifndef TA_DYNAMICOBJECT_H
#include "DynamicObject.h"
#endif 

#ifndef TA_STATICOBJECT_H
#include "StaticObject.h"
#endif 

#ifndef TA_SURFACEATTRIBUTE_H
#include "SurfaceAttribute.h"
#endif 



namespace TA
{

class Collision;
class llOOlOlOlll;
class SpaceDivisionObject;
class CollisionObjectCachedPolyData;
class lOllOlOlOOOl;
class SurfaceAttribute;
class lOllOlOllOlO;
class lOlOlllOlllO;
struct lOlOOOOOOlOl;
struct llllOllOlOO;

class lOlOllllOlO Physics
{
public:
	//pl.BeginExportClass(Physics)
	

	enum Flags
	{
		FLAG_DYNAMIC_OBJECTS =			0x1,
		FLAG_STATIC_OBJECTS	=			0x2,

		FLAG_ALL_OBJECTS = FLAG_DYNAMIC_OBJECTS | FLAG_STATIC_OBJECTS, //pl. pascal($1 + $2;)
		FLAG_ALL_NON_DYNAMIC_OBJECTS = FLAG_STATIC_OBJECTS, //pl. pascal($2;)

		FLAG_GHOST_OBJECTS =			0x8,

		FLAG_SPACIAL_DIVISION_MASK =	0x20 + 0x40 + 0x80 + 0x100, 
		FLAG_XY_COLLISION_GRID =		0x20,
		FLAG_XZ_COLLISION_GRID =		0x40,
		FLAG_YZ_COLLISION_GRID =		0x60,
		FLAG_DYNAMIC_OCTREE =			0x80,
		FLAG_CTREE =					0x100
	};

	enum SpacialDivisionType
	{
		SPACIAL_DIVISION_TYPE_DYNAMIC_OCTREE = 0,
		SPACIAL_DIVISION_TYPE_CTREE = 1,
		SPACIAL_DIVISION_TYPE_XY_COLLISION_GRID = 2,
		SPACIAL_DIVISION_TYPE_XZ_COLLISION_GRID = 3,
		SPACIAL_DIVISION_TYPE_YZ_COLLISION_GRID = 4,
	};

	enum SetMovingPriority
	{
		SET_MOVING_PRIORITY_LOW = 0,
		SET_MOVING_PRIORITY_HIGH = 1
	};
	
#ifndef TA_WRAPPER	
	typedef bool (TA_CALL_BACK *DynamicVsDynamicCollisionTestCallBack)(DynamicObject& objectA, DynamicObject& objectB);
	typedef bool (TA_CALL_BACK *PreProcessCollisionCallBack)(PreCollision& collision);
	typedef void (TA_CALL_BACK *PostProcessCollisionCallBack)(PostCollision& collision);
	typedef void (TA_CALL_BACK *OnBrokenJointCallBack)(PhysicsJoint& joint);
	typedef bool (TA_CALL_BACK *CollisionCallBack)(const Collision& collision, void* pUserData);
	typedef bool (TA_CALL_BACK *CollisionCallBack)(const Collision& collision, void* pUserData);
	typedef bool (TA_CALL_BACK *IntersectingObjectCallBack)(DynamicObject& object, void* pCallBackData);
	typedef void (TA_CALL_BACK *OnMovingCallBack)(const DynamicObject& object, void* pCallBackData);
	typedef void (TA_CALL_BACK *OnRestingCallBack)(const DynamicObject& object, void* pCallBackData);
#endif 

	
	
	static void TAC_CALL CreateInstance();
	static void TAC_CALL DestroyInstance();
	static Physics& TAC_CALL GetInstance();
	void SetupSimulation();
	

	
	
	
	void Update(float fDt);
	void Render();
	

	
	
	
	
	void AddStaticObject(StaticObject* pStaticObject);
	void RemoveStaticObject(StaticObject* pStaticObject);
	

	
	
	int AddSurfaceAttribute(SurfaceAttribute* pSurfaceAttribute);
	int AddSurfaceAttribute();
	void RemoveSurfaceAttribute(int nIndex);
	SurfaceAttribute& GetSurfaceAttribute(int nIndex);
	int GetNumSurfaceAttributes();
	void SetDefaultSurfaceAttribute(SurfaceAttribute* pSurfaceAttribute);
	const SurfaceAttribute& GetDefaultSurfaceAttribute();
	

	
	
	void AddDynamicObject(DynamicObject* pDynamicObject, bool bInitiallyMoving = true);
	void RemoveDynamicObject(DynamicObject* pDynamicObject);

	void SetDynamicObjectIsMoving(DynamicObject* pDynamicObject, int nPriority = SET_MOVING_PRIORITY_LOW);
	void SetDynamicObjectAtRest(DynamicObject* pDynamicObject);
	

	
	
	void TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3Normal,
		float fLength,
		Collision& collision,
		u32 nFlags);

	void TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3End,
		Collision& collision,
		u32 nFlags = FLAG_ALL_OBJECTS);

#ifndef TA_WRAPPER	
	Collision TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3Normal,
		float fLength,
		u32 nFlags);

	Collision TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3End,
		u32 nFlags = FLAG_ALL_OBJECTS);
#endif 

	
	
	void ForAllIntersectingObjects(const AABB& aabb, IntersectingObjectCallBack pfnCallBack, void* pCallBackData);
	void WakeUpAllIntersectingObjects(const AABB& aabb);
	

	
	
	const Vec3& GetGravity() const;
	void SetGravity(const Vec3& v3Gravity);
	void SetRenderCollisionsEnabled(bool bValue);
	bool IsRenderCollisionsEnabled() const;
	void SetMovementPropagationEnabled(bool bValue);
	bool IsMovementPropagationEnabled() const;
	

	
	
	void SetWorldDimensions(const AABB& worldAABB);	
	const AABB& GetWorldDimensions();	
	void SetMaxNumCollisions(int nMaxNumCollisions);
	int GetMaxNumCollisions();
	void SetMaxNumMovingObjects(int nMaxNumMovingObjects);
	int GetMaxNumMovingObjects();
	void SetSlowSolverMaxNumCollisions(int nSlowSolverMaxNumCollisions);
	int GetSlowSolverMaxNumCollisions();
	void SetSlowSolverMaxNumObjects(int nSlowSolverMaxNumObjects);
	int GetSlowSolverMaxNumObjects();
	void SetMaxNumObjectPairs(int nMaxNumObjectPairs);
	int GetMaxNumObjectPairs();
#ifdef TA_WRAPPER	
	void SetSpacialDivisionType(int eSpacialDivisionType);
	int GetSpacialDivisionType();
#endif
#ifndef TA_WRAPPER	
	void SetSpacialDivisionType(SpacialDivisionType eSpacialDivisionType);
	SpacialDivisionType GetSpacialDivisionType();
#ifndef DOXYGEN
	void SetSpacialDivisionType(int nSpacialDivisionType);
#endif
#endif
	void SetSpacialDivisionMinSize(float fSpacialDivisionMinSize);
	float GetSpacialDivisionMinSize();
	void SetSpacialDivisionMaxMemory(int nSpacialDivisionMaxMemory);
	int GetSpacialDivisionMaxMemory();
	

	
	
	DynamicVsDynamicCollisionTestCallBack GetDynamicVsDynamicCollisionTestCallBack() const;
	void SetDynamicVsDynamicCollisionTestCallBack(DynamicVsDynamicCollisionTestCallBack pfnDynamicVsDynamicCollisionTest);
	PreProcessCollisionCallBack GetPreProcessCollisionCallBack() const;
	void SetPreProcessCollisionCallBack(PreProcessCollisionCallBack pfnPreProcessCollision);
	PostProcessCollisionCallBack GetPostProcessCollisionCallBack() const;
	void SetPostProcessCollisionCallBack(PostProcessCollisionCallBack pfnPostProcessCollision);
	

	
	
#ifndef TA_WRAPPER	
	OnBrokenJointCallBack GetOnBrokenJointCallBack() const;
	void SetOnBrokenJointCallBack(OnBrokenJointCallBack pfnOnBrokenJoint);
	OnMovingCallBack GetOnMovingCallBack() const;
	void SetOnMovingCallBack(OnMovingCallBack pfnOnMoving, void* pCallBackData);
	OnRestingCallBack GetOnRestingCallBack() const;
	void SetOnRestingCallBack(OnRestingCallBack pfnOnResting, void* pCallBackData);
#endif 
	

#ifdef TA_PHYSICS_INTERNAL	
	
	void lOOlllOOlOll(DynamicObject* lOlllOOOllO, int lOOlllOOlOlO = SET_MOVING_PRIORITY_LOW);
	CollisionObjectCachedPolyData& lllllOOOlOl() { TA_ASSERT(lOlllOOOllOO); return *lOlllOOOllOO; }
	void lOOlllOOOOOO(DynamicObject* lOlllOOOllO);
	lOllOlOllOlO& lOlOOOOOlllO() { TA_ASSERT(lOllOlOllOOl); return *lOllOlOllOOl; }
	lOllOlOllOlO* llOOlOlOOOlO() { return lOllOlOllOOl; }
	llllOllOlOO& lOlOOlOlllll() { TA_ASSERT(lOOllOlllOlO); return *lOOllOlllOlO; }
	bool llOOlOOOlOOl();
	float llOOlOlOOOll() const { return lOlllOOOlllO; }
	void llOOlOOOlOlO(DynamicObject* lOllOOOlOll, DynamicObject* lOllOOOllOO, const MFrame& lOOlllOllOlO, const MFrame& lOOlllOllOll, float lOOlllOOllOl);
	
#endif 

#ifndef TA_REMOVE_DEPRECATED
	
	
	static void TAC_CALL CreateInstance(
		const AABB& worldAABB,
		const Vec3& v3Gravity = Vec3(0.0f, -9.81f, 0.0f),
		u32 nFlags = FLAG_XZ_COLLISION_GRID,
		float fMinSpacialDivision = 10.0f,
		int nSpacialDivisionMemory = 0x40000);
	
#endif 

	//pl.EndExportClass(DynamicObject)


	
private:	
	enum llOOlOlOOlOO
	{
		llOOlOlOOlOl =			0x10000,
		lOllllOOlOlO =		0x20000,
		lOlllOOlOlOO =		0x40000,
		lOllOlOOlOll =		0x80000,
		lOllOlOOllOO =				0x100000,
		lOlllOlOlOOl =		0x200000,
		lOlllOOlOlOl =			0x400000,
		llOOOOllllOO =			0x800000,
	};
	struct lOllOlOOOlOl;

	struct llOOlOlOOllO
	{
		AABB lOllOlOOOlll;
		int lOlOllllOOlO;
		int lOlOllllOOOl;
		int lOlOllllOlOl;
		int lOllOlOOllll;
		int lOllOllOOOll;
		SpacialDivisionType lOllOlOlOOll;
		float lOllOlOlOlOO;
		int lOllOlOlOlOl;
	};

	typedef List<StaticObject*>::Iterator lOlllOlOOOOl;
	typedef List<DynamicObject*>::Iterator lOllOlOlOllO;
	typedef lOlOlllOOO<DynamicObject*>::lOOlOOOllll::Iterator lOlllOlOlOlO;

	Vec3 lOlllOOOllOl;
	float lOlllOOOlllO;	
	float lOlllOOOllll;
	u32 lllOlOlllO;
	u32 lOllOllOOlOO;

	llllOllOlOO* lOOllOlllOlO;
	lOllOlOlOOOl* lOllOlOOllOl;
	llOOlOlOlll* lOllOlOlOOlO;
	lOllOlOllOlO* lOllOlOllOOl;
	DynamicObject* lOlllOOOlOOl;
	DynamicObject* lOlllOOOlOlO;
	DynamicObject* lOlllOOOlOll;
	List<DynamicObject*> lOllOlOlOlll;
	List<StaticObject*> lOlllOlOOOll;
	lOlOlllOOO<DynamicObject*> lOllOlOllOll;
	lOlOlllOOO<DynamicObject*>::lOOlOOOllll lOllOlOlllOO;
	CollisionObjectCachedPolyData* lOlllOOOllOO;
	SurfaceAttribute lOlllOOllllO;
	lOOOOlOllO<SurfaceAttribute> lOlllOOlllll;
	DynamicVsDynamicCollisionTestCallBack lOlllOOllOOO;
	PreProcessCollisionCallBack lOlllOOllOOl;
	PostProcessCollisionCallBack lOlllOOllOlO;
	OnBrokenJointCallBack lOlllOOllOll;
	OnMovingCallBack lOlllOOlllOO;
	void* lOlllOOlllOl;
	OnRestingCallBack lOllOllOOOOl;
	void* lOllOllOOOlO;
	llOOlOlOOllO lOllOlOOlllO;
	llOOlOlOOllO lOllOlOlOOOO;
	
	void (TAC_CALL *lOlllOOlOOOO)(float lOOlOllOllOl, Collision& llOllllOlOl);
	void (TAC_CALL *lOlllOOlOOOl)(float lOOlOllOllOl, float llOllOllll, float lOlOOOllllOl, Collision& llOllllOlOl);
	bool (TAC_CALL *lOlllOOlOOlO)(float lOllOOlOOlO, Collision& llOllllOlOl);
	void (TAC_CALL *lOlllOOlOOll)(float lOOlOllOllOl, float llOllOllll, Collision& llOllllOlOl);
	float (TAC_CALL *lOlllOOlOlll)(float lOOlllOOllOl, Collision& llOllllOlOl);

	static Physics* lOllOlOOOOOl;
	
	Physics();
	~Physics();

	void Initialise();
	void Initialise(
		const AABB& lOllOlOOOlll,
		const Vec3& lOllOlOOlOOO,
		u32 lOlOllOOOl,
		float lOllOlOOlOOl,
		int lOllOlOOlOlO);
	void Finalise();

	void lOlllOlOOlOl();

	void lOllOllOOllO(float lOOlllOOllOl);
	void lOllOllOlOOO(float lOOlllOOllOl, int lOllOllOOlll);
	void lOllllOlOOll(DynamicObject& lOllllOlOlll);
	void lOllllOllOOl(PhysicsJoint& lOOlllOOOOOl);
	void lOllOllOlllO(lOlOlllOlllO& lOllOllOlOlO);
	void llOOOOlOOlOl(float lOOlllOOllOl, Collision& llOllllOlOl);
	void lOllOllOllll(float lOOlllOOllOl, lOlOlllOlllO& lOllOllOlOlO, int lOllOllOOlll);
	float lOlOOlOOOOOO(float lOOlllOOllOl, Collision& llOllllOlOl) { TA_ASSERT(lOlllOOlOlll); return lOlllOOlOlll(lOOlllOOllOl, llOllllOlOl); }
	bool lOllOlllOOOO();
	void llOOOOlOlOll();
	bool lOllOlllOOOl(lOlOlllOlllO& lOllOllOlOlO);
	void lOllOlllOOlO(lOlOlllOlllO& lOllOllOlOlO);
	void lOllOlllllOl(float lOOlllOOllOl, lOlOlllOlllO& lOllOllOlOlO);
	void lOlllOOOOllO(float lOOlllOOllOl, bool llOOOlOOOlOO, lOlOlllOlllO& lOllOllOlOlO);
	void llOOOlOllOOO(PhysicsJoint& lOOlllllOOOO, bool llOOOlOOllll, float llOOOlOlOlll);
	void lOlllOOOOlll(float lOOlllOOllOl, bool llOOOlOOOlOO);

	void lOlllOOOOOll(lOlOlllOlllO&, lOllOlOOOlOl*, int);
	void lOllOllllOll(lOlOlllOlllO&, DynamicObject**, int&, int, Collision**, int&, int);
	void lOllOlllllll(DynamicObject**, int, Collision**,int);
	void lOlllOOOOOOO(lOlOlllOlllO&, DynamicObject**, int, Collision**, int);	
	void lOlllOOOOlOO(lOllOlOOOlOl*, int, int, const Vec3&);
	void lOlllOOOOlOl(lOllOlOOOlOl*, int, int, const Vec3&);
	void lOllOlllllOO(lOlOlllOlllO&, DynamicObject**, int, Collision**, int);	
	void llOOOlllllll(lOllOlOOOlOl*, int, int, const Vec3&);
	void lOlllOOOlOOO();
	
	void lOlOOOlllOll(float lOOlOllOllOl, Collision& llOllllOlOl) { TA_ASSERT(lOlllOOlOOOO); lOlllOOlOOOO(lOOlOllOllOl, llOllllOlOl); }
	void TAC_CALL lOlOOOllllOO(float lOOlOllOllOl, float llOllOllll, float lOlOOOllllOl, Collision& llOllllOlOl) { TA_ASSERT(lOlllOOlOOOl); lOlllOOlOOOl(lOOlOllOllOl, llOllOllll, lOlOOOllllOl, llOllllOlOl); }
	bool TAC_CALL lOlOOOlllllO(float lOllOOlOOlO, Collision& llOllllOlOl) { TA_ASSERT(lOlllOOlOOlO); return lOlllOOlOOlO(lOllOOlOOlO, llOllllOlOl); }
	void TAC_CALL lOlOOOllllll(float lOOlOllOllOl, float llOllOllll, Collision& llOllllOlOl) { TA_ASSERT(lOlllOOlOOll); lOlllOOlOOll(lOOlOllOllOl, llOllOllll, llOllllOlOl); }

	bool llOOlOOOOOlO(DynamicObject* lOlllOOOllO);

	void lOllOllOOlOl();
	void llOOOOOOOlOO();
	
	static bool TAC_CALL lOlllllOOlOl(Collision& lOllOllOlOl, void* lOOlllOOOO);
	static void TAC_CALL lOllllOlOOOl(SpaceDivisionObject& lOllllllllO, void* lOOlllOOOO);
	void lOllllOOlOOl(lOlOOOOOOlOl& lOlOOOOllOlO);
	static void TAC_CALL lOlllOlOlOll(SpaceDivisionObject& lOllllllllO, void* lOOlllOOOO);
	static void TAC_CALL lOlllOlOllOO(SpaceDivisionObject& lOllllllllO, void* lOOlllOOOO);
};

inline Physics& TAC_CALL Physics::GetInstance() { lOOOOOOllO(lOllOlOOOOOl, "Physics::GetInstance. Instance not created."); return *lOllOlOOOOOl; }
inline const Vec3& Physics::GetGravity() const { return lOlllOOOllOl; }
inline void Physics::SetRenderCollisionsEnabled(bool lllOllOOOO) { if (lllOllOOOO) lllOlOlllO |= llOOOOllllOO; else lllOlOlllO &= ~llOOOOllllOO; }
inline bool Physics::IsRenderCollisionsEnabled() const { return (lllOlOlllO & llOOOOllllOO) != 0; }
inline void Physics::SetMovementPropagationEnabled(bool lllOllOOOO) { if (lllOllOOOO) lllOlOlllO |= lOlllOOlOlOl; else lllOlOlllO &= ~lOlllOOlOlOl; }
inline bool Physics::IsMovementPropagationEnabled() const { return (lllOlOlllO & lOlllOOlOlOl) != 0; }
inline Physics::DynamicVsDynamicCollisionTestCallBack Physics::GetDynamicVsDynamicCollisionTestCallBack() const { return lOlllOOllOOO; }
inline void Physics::SetDynamicVsDynamicCollisionTestCallBack(DynamicVsDynamicCollisionTestCallBack llOOlOlOOlll) { lOlllOOllOOO= llOOlOlOOlll; }
inline Physics::PreProcessCollisionCallBack Physics::GetPreProcessCollisionCallBack() const { return lOlllOOllOOl; }
inline void Physics::SetPreProcessCollisionCallBack(PreProcessCollisionCallBack llOOlOlOlOOO) { lOlllOOllOOl = llOOlOlOlOOO; }
inline Physics::PostProcessCollisionCallBack Physics::GetPostProcessCollisionCallBack() const { return lOlllOOllOlO; }
inline void Physics::SetPostProcessCollisionCallBack(PostProcessCollisionCallBack llOOlOlOlOOl) { lOlllOOllOlO = llOOlOlOlOOl; }
inline Physics::OnBrokenJointCallBack Physics::GetOnBrokenJointCallBack() const { return lOlllOOllOll; }
inline void Physics::SetOnBrokenJointCallBack(OnBrokenJointCallBack llOOlOlOlOlO) { lOlllOOllOll = llOOlOlOlOlO; }
inline Physics::OnMovingCallBack Physics::GetOnMovingCallBack() const { return lOlllOOlllOO; }
inline void Physics::SetOnMovingCallBack(OnMovingCallBack llOOlOlOlOll, void* lOOOOllllO) { lOlllOOlllOO = llOOlOlOlOll; lOlllOOlllOl = lOOOOllllO; }
inline Physics::OnRestingCallBack Physics::GetOnRestingCallBack() const { return lOllOllOOOOl; }
inline void Physics::SetOnRestingCallBack(OnRestingCallBack llOOlOlOllOO, void* lOOOOllllO) { lOllOllOOOOl = llOOlOlOllOO; lOllOllOOOlO = lOOOOllllO; }
inline void Physics::SetSpacialDivisionType(int llOOlOlOllOl) { SetSpacialDivisionType((SpacialDivisionType)llOOlOlOllOl); }

void TAC_CALL lOllOlOOOllO(
	const AABB& lOllOlOOOlll,
	const Vec3& lOllOlOOlOOO = Vec3(0.0f, -9.81f, 0.0f),
	u32 lOlOllOOOl = Physics::FLAG_XZ_COLLISION_GRID,
	float lOllOlOOlOOl = 10.0f,
	int lOllOlOOlOlO = 0x40000);

}

#endif 
