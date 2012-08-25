//---------------------------------------------------------------------------------
// File Name: DynamicObject.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_DYNAMICOBJECT_H
#define TA_DYNAMICOBJECT_H

#ifndef TA_MFRAME_H
#include "../Common/MFrame.h"
#endif 

#ifndef TA_AABB_H
#include "../Common/AABB.h"
#endif 

#ifndef lOOllOllll
#include "../Common/zcta001.h"
#endif 

#ifndef lOOOOOOllOl
#include "../Common/zcta008.h"
#endif 

#ifndef lOlOlOlOlO
#include "../Common/zcta009.h"
#endif 

#ifndef lllOlOllOO
#include "../Common/zcta005.h"
#endif 

#ifndef TA_PHYSICSOBJECT_H
#include "PhysicsObject.h"
#endif 

#ifndef lOlOOOllOlll
#include "../Physics/zpta016.h"
#endif 

#ifndef TA_PHYSICSJOINT_H
#include "PhysicsJoint.h"
#endif 

#ifndef TA_COLLISIONOBJECT_H
#include "CollisionObject.h"
#endif 

#ifndef TA_COLLISION_H
#include "Collision.h"
#endif 

#ifndef TA_DYNAMICOBJECTCOLLISIONITERATOR_H
#include "DynamicObjectCollisionIterator.h"
#endif 



namespace TA
{

class CollisionObjectCombo;
class CollisionObjectComplex;
class DynamicObjectGroup;
struct EulerAngles;
struct llllOllOlOO;


const float JOINT_LIMIT_DISABLED = k_fMaxFloat;



#ifdef TA_PHYSICS_INTERNAL
	


#define lOlOOOlllOOO(lOlOOOlllOOl, lOlOOOlllOlO) \
namespace lOlOOOlllOlO \
{ \
	lOlOOOlllOOl void TAC_CALL lOlOOOlllOll(float lOOlOllOllOl, Collision& llOllllOlOl); \
	lOlOOOlllOOl void TAC_CALL lOlOOOllllOO(float lOOlOllOllOl, float llOllOllll, float lOlOOOllllOl, Collision& llOllllOlOl); \
	lOlOOOlllOOl bool TAC_CALL lOlOOOlllllO(float lOllOOlOOlO, Collision& llOllllOlOl); \
	lOlOOOlllOOl void TAC_CALL lOlOOOllllll(float lOOlOllOllOl, float llOllOllll, Collision& llOllllOlOl); \
	lOlOOOlllOOl float TAC_CALL lOlOOlOOOOOO(float lOOlllOOllOl, Collision& llOllllOlOl); \
}

lOlOOOlllOOO(extern, lOlOOlOOOOOl)
lOlOOOlllOOO(extern, lOlOOlOOOOlO)

#endif 

class lOlOllllOlO DynamicObject : public PhysicsObject, public SpaceDivisionObject
{
public:	
	
	struct LineIntersection
	{
		float GetIntersectionDepth() const;
		DynamicObject* GetDynamicObject() const;
		CollisionObject* GetCollisionObject() const;
		int GetAttribute() const;
		const Vec3& GetPosition() const;
		const Vec3& GetNormal() const;
		

	private:
		friend class DynamicObject;
		friend class DynamicObjectCar;
		float lOllOOlOOOl;
		DynamicObject* lOlllOOOllO;
		CollisionObject* lOlOllOOOOl;
		int llOlllOOllO;
		Vec3 lOOOOlllll;
		Vec3 lOOOlOOOOO;
	};
	


	typedef List<PhysicsJoint>::Iterator JointIterator;
	typedef DynamicObjectCollisionIterator CollisionIterator;

	const static float JOINT_LIMIT_DISABLED;

	//pl.BeginExportClass(DynamicObject)

	enum ReinitFlags
	{
		REINIT_FLAG_UPDATE_MASS = 0x1,
		REINIT_FLAG_UPDATE_INERTIA_TENSOR = 0x2,
		REINIT_FLAG_UPDATE_CENTER_OFFSET = 0x4,
	};

	enum SetMovingPriority
	{
		SET_MOVING_PRIORITY_LOW = 0,
		SET_MOVING_PRIORITY_HIGH = 1
	};

	
	
	void InitialiseFromFile(const Char* szFileName);
	void InitialiseAsABox(const AABB& aabb);
	void InitialiseAsAnOrientedBox(const AABB& aabb, const MFrame& mFrame);
	void InitialiseAsACapsule(const Vec3& v3Start, const Vec3& v3End, float fRadius);
	void InitialiseAsASphere(const Vec3& v3Center, float fRadius);
	void InitialiseAsACylinder(const Vec3& v3Start, const Vec3& v3End, float fRadius);	
	void InitialiseFromPointList(const Vec3* pv3PointList, int nNumPoints);
	void Initialise(CollisionObjectCombo* pCollisionObjectCombo);
	void Initialise(CollisionObjectComplex* pCollisionObjectComplex);
#ifdef TA_PHYSICS_INTERNAL
	void InitialiseAsWorldObject();
#endif 
	void Finalise();
	

	
	
	const MFrame& GetFrame() const;
	void SetFrame(const MFrame& frame);
	const MFrame& GetNextFrame() const;
	const MFrame& GetPreviousFrame() const;
	const MFrame& GetGraphicsFrame() const;
	const Vec3& GetPosition() const;
	void SetPosition(const Vec3& v3Position);
	const Vec3& GetCenterOfMass() const;
	const Vec3& GetNextCenterOfMass() const;
	const Vec3& GetCenterOffset() const;
	void SetCenterOffset(const Vec3& v3CenterOffset);
	
	
	void SetFrameDirect(const MFrame& frame);
	

	
	
	float GetMass() const;
	float GetInverseMass() const;
	void SetMass(float fMass);
	const Mat33& GetInertia() const;
	void SetInertiaFromBox(const AABB& aabb);
	void SetInertiaFromBox(float fX, float fY, float fZ);
	const Mat33& GetInverseInertia() const;
	const Mat33& GetWorldSpaceInertia() const;
	const Mat33& GetWorldSpaceInverseInertia() const;
	

	
	
	void SetFriction(float fFriction);
	float GetFriction() const;
	void SetRestitution(float fRestitution);
	float GetRestitution() const;
	void SetGravityMult(float fGravityMult);
	float GetGravityMult() const;
	void SetRotationDisabled(bool bValue);
	bool RotationDisabled() const;
	void SetMovementDisabled(bool bValue);
	bool MovementDisabled() const;
	void SetForceFastSolverEnabled(bool bValue);
	bool IsForceFastSolverEnabled() const;
	void SetAllDampingDisabled(bool bValue);
	bool IsAllDampingDisabled() const;
	void SetExtraStability(bool bValue);
	bool ExtraStability() const;
	void SetRigidFrictionDisabled(bool bValue);
	bool RigidFrictionDisabled() const;
	void SetCollisionDisabled(bool bValue);
	bool CollisionDisabled() const;
	void SetUpdateDisabled(bool bValue);
	bool UpdateDisabled() const;
	void SetGhost(bool bValue);
	bool Ghost() const;
	void SetInfiniteMass(bool bValue);
	bool InfiniteMass() const;
	void SetFindVelocityFromNextFrame(bool bValue);
	bool GetFindVelocityFromNextFrame() const;
	bool IsWorldObject() const;
	void SetRestTimeMultiplier(float fRestTimeMultiplier);
	float GetRestTimeMultiplier() const;
	void SetID(int nID); 
	int GetID() const;
	Physics* GetPhysics();
	

	
	
	bool IsInMovingList() const;
	void SetToMoving(int nPriority = SET_MOVING_PRIORITY_LOW);
	void SetToResting();
	

	
	
	
	
#ifdef TA_PHYSICS_INTERNAL
	float lOlOOlOOOOll() const { return lOOllOOlllll; }
	void lOlOOlOOOlOO(float lllOllllOO) { lOOllOOlllll = lllOllllOO; }
	void lOlOOlOOOlOl(float lOOlllOOllOl) { lOOllOOlllll += lOOlllOOllOl * lOOllOlOOOOO; lOOlllOOlOOO(false); }
	void lOOlllOOllOO() { lOOllOOlllll = 0.0f; lOOlllOOlOOO(false); }
	void lOOlllOOlOOO(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, lOlOOOllOllO, lllOllOOOO); }
	bool lOlOOOOllllO() const { return lllOllOOOl(lllOlOlllO, lOlOOOllOllO); }
	void lOOllOOllOlO(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, lOlOOlOOOllO, lllOllOOOO); }

	
	int lOlOOlOOOlll() const { return lOOllOlOOOOl; }
	void lOlOOlOOlOOO() { lOOllOlOOOOl++; }
	void lOlOOlOOlOOl() { lOOllOlOOOOl = 0; }	

	
	void lOlOOlOOlOlO(void* lOlOOlOOlOll) { lOOllOlOlOOl = lOlOOlOOlOll; lllOlOllOl(lllOlOlllO, lOlOOlOOllOO, lOOllOlOlOOl != 0); lllOlOllOl(lllOlOlllO, lOlOOOllOllO, lOOllOlOlOOl == 0); lllOlOllOl(lllOlOlllO, lOlOOlOOllOl, lOOllOlOlOOl == 0); }
	void* lOlOOlOOlllO() { return lOOllOlOlOOl; }

	void lOlOOlOOllll(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, lOlOOlOOllOl, lllOllOOOO); }
	bool lOlOOOOlllll() const { return lllOllOOOl(lllOlOlllO, lOlOOlOOllOl); }

	
	void lOlOOlOlOOOO(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, lOlOOlOlOOOl, lllOllOOOO); }
	bool lOlOOlOlOOlO() const { return lllOllOOOl(lllOlOlllO, lOlOOlOlOOOl); }
	void lOlOOlOlOOll(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, lOlOOlOlOlOO, lllOllOOOO); }
	bool lOlOOlOlOlOl() const { return lllOllOOOl(lllOlOlllO, lOlOOlOlOlOO); }
	void lOlOOlOlOllO(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, lOlOOlOlOlll, lllOllOOOO); }
	bool lOlOOlOllOOO() const { return lllOllOOOl(lllOlOlllO, lOlOOlOlOlll); }
	void lOlOOlOllOOl(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, lOlOOlOllOlO, lllOllOOOO); }
	bool lOOlOOOlOll() const { return lllOllOOOl(lllOlOlllO, lOlOOlOllOlO); }	
	void lOlOOlOllOll(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, lOlOOlOlllOO, lllOllOOOO); }
	bool lOlOOlOlllOl() const { return lllOllOOOl(lllOlOlllO, lOlOOlOlllOO); }	
	u32& lOlOOlOllllO() { return lllOlOlllO; }
	u32 GetFlags() const { return lllOlOlllO; }
	
	llllOllOlOO* lOlOOlOlllll() { return lOOllOlllOlO; }
	void lOlOOllOOOOO(llllOllOlOO* lOlOOllOOOOl) { lOOllOlllOlO = lOlOOllOOOOl; }

#define lOlOOllOOOlO(lOlOOOlllOOl, lOlOOOlllOlO) \
	lOlOOOlllOOl void TAC_CALL lOlOOOlllOlO::lOlOOOlllOll(float lOOlOllOllOl, Collision& llOllllOlOl); \
	lOlOOOlllOOl void TAC_CALL lOlOOOlllOlO::lOlOOOllllOO(float lOOlOllOllOl, float llOllOllll, float lOlOOOllllOl, Collision& llOllllOlOl); \
	lOlOOOlllOOl bool TAC_CALL lOlOOOlllOlO::lOlOOOlllllO(float lOllOOlOOlO, Collision& llOllllOlOl); \
	lOlOOOlllOOl void TAC_CALL lOlOOOlllOlO::lOlOOOllllll(float lOOlOllOllOl, float llOllOllll, Collision& llOllllOlOl);
	
	lOlOOllOOOlO(friend, lOlOOlOOOOOl)
	lOlOOllOOOlO(friend, lOlOOlOOOOlO)



#endif 
	
	
	
	
	
	const Vec3& GetLinearVelocity() const;
	void SetLinearVelocity(const Vec3& v3LinearVelocity);
	const Vec3& GetAngularVelocity() const;
	void SetAngularVelocity(const Vec3& v3AngularVelocity);
	void SetVelocitiesToMoveToFrame(const MFrame& mFrame, float fDt);
	Vec3 GetVelocityAtWorldPosition(const Vec3& v3Position) const;
	void ApplyImpulse(const Vec3& v3WorldImpulse, const Vec3& v3WorldPosition);
	void ApplyLinearImpulse(const Vec3& v3WorldLinearImpulse);
	void ApplyAngularImpulse(const Vec3& v3WorldAngularImpulse);
	

	
	
	void AssertForceAndTorqueCleared() const;
	void AccumulateGravity();	
	void AccumulateForceAndTorque(const Vec3& v3WorldForce, const Vec3& v3WorldPosition);
	void AccumulateLinearForce(const Vec3& v3WorldForce);
	void ApplyForceAndTorqueToVelocities(float fDt);
	void ClearForceAndTorque();
	

	
	
	void Clear();
	void UpdateWorldSpaceInertialTensor();
	virtual void Update(float fDt);
	virtual void UpdateGraphics(float fDt, float fInterpolant);
	void Extrapolate(
		const MFrame& frameA,
		const MFrame& frameB,
		const Vec3& v3VelocityA,
		const Vec3& v3VelocityB,
		float fDt,
		float fExtrapolation);
	virtual void ApplyVelocityToNextFrame(float fDt);
	void ApplyNextFrame();
	

	
	
	void Render();
	

	
	
	
	
#ifdef TA_PHYSICS_INTERNAL
	void lOlOOllOOOll(void* lOlOOllOOlOO) { lOOllOlOlOlO = lOlOOllOOlOO; }
	void* lOlOOllOOlOl() { return lOOllOlOlOlO; }
#endif 
	
	

	
	
	void CalculateWorldAABB();
	const AABB& GetWorldAABB();
#ifndef TA_WRAPPER	
	CollisionObject& GetCollisionObject();
#endif 
	CollisionObjectCombo* GetCollisionObjectCombo();
	CollisionObjectComplex* GetCollisionObjectComplex();
	void SetCollisionObject(
		CollisionObjectCombo* pCollisionObjectCombo, 
		int nFlags = (REINIT_FLAG_UPDATE_MASS | REINIT_FLAG_UPDATE_INERTIA_TENSOR | REINIT_FLAG_UPDATE_CENTER_OFFSET));
	void SetCollisionObjectChanged(
		int nFlags = (REINIT_FLAG_UPDATE_MASS | REINIT_FLAG_UPDATE_INERTIA_TENSOR | REINIT_FLAG_UPDATE_CENTER_OFFSET));
	void ReinitCollisionAsABox(
		const AABB& aabb, 
		int nFlags = (REINIT_FLAG_UPDATE_MASS | REINIT_FLAG_UPDATE_INERTIA_TENSOR | REINIT_FLAG_UPDATE_CENTER_OFFSET));
	void ReinitCollisionAsAnOrientedBox(
		const AABB& aabb, 
		const MFrame& mFrame, 
		int nFlags = (REINIT_FLAG_UPDATE_MASS | REINIT_FLAG_UPDATE_INERTIA_TENSOR | REINIT_FLAG_UPDATE_CENTER_OFFSET));
	void ReinitCollisionAsACapsule(
		const Vec3& v3Start, 
		const Vec3& v3End, 
		float fRadius, 
		int nFlags = (REINIT_FLAG_UPDATE_MASS | REINIT_FLAG_UPDATE_INERTIA_TENSOR | REINIT_FLAG_UPDATE_CENTER_OFFSET));
	void ReinitCollisionAsASphere(
		const Vec3& v3Center, 
		float fRadius, 
		int nFlags = (REINIT_FLAG_UPDATE_MASS | REINIT_FLAG_UPDATE_INERTIA_TENSOR | REINIT_FLAG_UPDATE_CENTER_OFFSET));
	void ReinitCollisionAsACylinder(
		const Vec3& v3Start, 
		const Vec3& v3End, 
		float fRadius, 
		int nFlags = (REINIT_FLAG_UPDATE_MASS | REINIT_FLAG_UPDATE_INERTIA_TENSOR | REINIT_FLAG_UPDATE_CENTER_OFFSET));
	
#ifndef TA_WRAPPER
	CollisionIterator GetCollisionIterator();
#ifndef TA_REMOVE_DEPRECATED
	void SetCollisionListEnabled(bool bValue);
	bool CollisionListEnabled() const;
#endif
#endif 
	bool TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3Normal,
		float fLength,
		Collision& collision) const;
	

	
	
	
#ifdef TA_PHYSICS_INTERNAL
	void lOOllllOllOO(
		int lOOllllOllOl,
		DynamicObject* lOlllOOOllO,
		CollisionObject* lOlOllOOOOl,
		u32 llOlllOOllO,
		float lOllOOlOOOl,
		const Vec3& lOOOOlllll,
		const Vec3& lOOOlOOOOO);
#endif 
	
	

	
	
	int GetNumLineCollisions();
	const LineIntersection& GetLineCollision(int nIndex) const;
	

	
	
	PhysicsJoint& AddJoint(
		DynamicObject* pOtherObject);
	
	
	void RemoveAllJoints(); 	
	void RemoveJoint(PhysicsJoint& joint);
	void RemoveJointsToObject(DynamicObject* pDynamicObject);
	

	
	
#ifndef TA_REMOVE_DEPRECATED
	PhysicsJoint& AddJoint(
		DynamicObject* pOtherObject, 
		const Vec3& v3LocalPos, 
		const Vec3& v3LocalPosOnOtherObject,
		const Mat33& m33DefaultRotationOfOtherObject,
		const EulerAngles& minAngles,
		const EulerAngles& maxAngles);
	PhysicsJoint& AddJointTypeHinge(
		DynamicObject* pOtherObject, 
		const Vec3& v3LocalPos,
		const Vec3& v3LocalPosOnOtherObject,
		const Vec3& v3LocalHingeAxis, 
		const Vec3& v3LocalHingeNormal, 
		float fMinAngle,
		float fMaxAngle);
	PhysicsJoint& AddJointTypeSocket(
		DynamicObject* pOtherObject, 
		const Vec3& v3LocalPos, 
		const Vec3& v3LocalPosOnOtherObject,
		const Vec3& v3LimitCenterNormal,
		const Vec3& v3LimitCenterNormalOnOtherObject,
		float fMaxAngle);
	PhysicsJoint& AddJointTypeLimitedSocket(
		DynamicObject* pOtherObject, 
		const Vec3& v3LocalPos, 
		const Vec3& v3LocalPosOnOtherObject,
		const Mat33& m33DefaultRotationOfOtherObject,
		float fMaxAngleY,
		float fMaxAngleXZ);
	PhysicsJoint& AddJointTypeSquareSocket(
		DynamicObject* pOtherObject, 
		const Vec3& v3LocalPos, 
		const Vec3& v3LocalPosOnOtherObject,
		const Mat33& m33DefaultRotationOfOtherObject,
		float fMinX,
		float fMaxX,
		float fMinY,
		float fMaxY,
		float fMinZ,
		float fMaxZ);
	PhysicsJoint& AddJointTypeSlider(
		DynamicObject* pOtherObject, 
		const Vec3& v3LocalPos,
		const Vec3& v3StartPosOnOtherObject,
		const Vec3& v3EndPosPosOnOtherObject);
	PhysicsJoint& AddRotationConstraint(
		DynamicObject* pOtherObject, 
		const Mat33& m33DefaultRotationOfOtherObject,
		const EulerAngles& minAngles,
		const EulerAngles& maxAngles);
	PhysicsJoint& AddVectorConstraint(
		DynamicObject* pOtherObject,
		const Vec3& v3LimitCenterNormal,
		const Vec3& v3LimitCenterNormalOnOtherObject,
		float fMaxAngle);
#endif 
#ifndef TA_WRAPPER
	
#ifdef TA_DYNAMIC_OBJECT_GROUP_ENABLED
	DynamicObjectGroup* GetDynamicObjectGroup() { return m_pDynamicObjectGroup; }
#endif 
#endif 
	
	
#ifndef TA_WRAPPER
	JointIterator GetJointIterator();
#endif 
	

	
	
	
#ifdef TA_PHYSICS_INTERNAL
	List<PhysicsJoint*>::Iterator lOlOOllOOllO() { return lOOllOlllOOO.lOlOOOOlOO(); }
	void lOOllOlllOOl();
	lOlOOOOOOlOl* lOlOOOOOOllO(DynamicObject* llOOlOOlOOl, bool lOlOOOOOOlll);
	lOlOOOOOOlOl* lOlOOOOOllOl(DynamicObject* llOOlOOlOOl);
	lOlOOOOOlOOO* lOlOOllOOlll() { return lOOllOllOOlO; }
	int lOlOOllOlOOO() const { return lOOllOllOlOO; }
	void lOlOOOOlOllO(int lOlOOOOlOlOl) { lOOllOllOlOO = lOlOOOOlOlOl; }
	int lOlOOllOlOOl() const { return lOOllOllOlOl; }
	void lOlOOllOlOlO(int lOlOOllOlOll) { lOOllOllOlOl = lOlOOllOlOll; }
	void lOlOOOOlOlOO(int lOlOOOOlOlOl);
	void lOlOOOOlOlll(int lOlOOOOlOlOl);
	void lOlOOOOllOOO(void (TAC_CALL *lOlOOOOllOOl)(lOlOOOOOOlOl& lOlOOOOllOlO, void* lOOlllOOOO), void* lOOlllOOOO);
	void lOlOOOOllOll();
	void lOlOOOOlllOl();
	void lOlOOOlOOOOl();
	void lOlOOOlOOOlO(lOlOOOOOOlOl& lOlOOOOllOlO);
	void lOlOOllOllOO(int lOlOOllOllOl) { lOOllOlOOOlO = lOlOOllOllOl; }
	int lOlOOllOlllO() const { return lOOllOlOOOlO; }
	void lOlOOOlOOOll();
	void lOlOOOlOOlOO();
	void lOlOOllOllll(float lOOOOOlllO) { lOlOOlllOOOO = lOOOOOlllO; }
	void lOOlOlllllOl(float lOOOOOlllO) { lOlOOlllOOOO += lOOOOOlllO; }
	float lOlOOlllOOOl() const { return lOlOOlllOOOO; }
	const Vec3& lOlOOlllOOlO() const { return lOOllOlOOlll; }
	const Vec3& lOlOOlllOOll() const { return lOOllOlOlOOO; }	
	const void SetNextFrame(const MFrame& lOllOllllOO) { lOOllOlOOllO = lOllOllllOO; lOOllOlOOlOO = lOOllOlOOOll * lOllOllllOO; }
	void lOlOOOlOOlOl(float lOOlllOOllOl, float lOlOOOlOOllO);
	const Vec3& lOlOOlllOlOO() const { return lOOlllOlOllO; }
	const Vec3& lOlOOlllOlOl() const { return lOOlllOlOlll; }
	void lOlOOOlOOlll();
	float lOOllOOOOOlO() const { return lOOllOOllOOO; }	
	float lOlOOlllOllO() const { return lOOllOOllOOl; }	
	void lOlOOlllOlll(float lOlOOOlOlOlO) { lOOllOOllOOl = lOlOOOlOlOlO; }	
	bool lOlOOllllOOO() const { return (lllOlOlllO & (lOOlllOOlOOl | lOOlllOOOlll | lOlOOlOOllOO)) == lOlOOlOOllOO; }
	void lOlOOOlOlOOO();
	void lOlOOllllOOl(DynamicObject* lOlllOOOllO) { lOOllOllOOOO = lOlllOOOllO; }
	DynamicObject* lOlOOllllOlO() { return lOOllOllOOOO; }
	void lOlOOOlOlOOl(DynamicObject* llOOlOOlOOl);
	void lOlOOOlOlOll(float lOlOOOlOllOO, float lOlOOOlOllOl);
	void lOlOOOlOlllO(); 
	void lOlOOOllOOOO(); 
	void lOOllOllllll();
	
#endif 
	
	
	
	
	
	bool IsAllowedToCollideWith(const DynamicObject& otherObject) const;
	void SetUserGroup(int nUserGroupId);
	int GetUserGroup() const;
	void SetUserGroupItemId(int nUserGroupItemId);
	int GetUserGroupItemId() const;
	void DisallowCollisionWithUserGroupItemId(int nUserGroupItemId);
	void AllowCollisionWithUserGroupItemId(int nUserGroupItemId);
#ifndef TA_REMOVE_DEPRECATED
	void SetCollisionUserData(void* pData);
	void* GetCollisionUserData() const;
#endif 
	void SetUserData(void* pData);
	void* GetUserData() const;
	
	
	
	static int TAC_CALL CreateUserGroup();
	

	static DynamicObject* TAC_CALL CreateNew();
	//pl.Release()
	
	//pl.EndExportClass(DynamicObject)
		

protected:	
	DynamicObject();
	virtual ~DynamicObject();
	void InitialiseCommon();
	void ClearLineIntersections();
	

private:
	
#ifdef TA_PHYSICS_INTERNAL
public:
#endif 
	enum Flags
	{
		lOlOOlOOllOO =					0x1,
		lOlOOlOOllOl =				0x2,
		lOlOOllllOll =					0x4,
		lOOlllOOOllO =					0x8,
		lOlOOlllllOO =					0x10,
		lOlOOlllllOl =					0x20,
		lOlOOllllllO =								0x40,
		lOlOOlllllll =					0x80,
		lOOlllOOOlll =					0x100,
		lOlOOlOlOOOl =		0x400,
		lOlOlOOOOOOO =			0x800,
		lOOlllOOlOOl =						0x1000,
		lOlOOOllOllO =					0x2000,
		lOlOOlOlOlOO =							0x4000,
		lOlOOlOlOlll =							0x8000,
		lOlOlOOOOOOl =		0x10000,
		lOlOOlOOOllO =						0x20000,
		lOlOOlOllOlO =							0x40000,
		lOlOOlOlllOO =			0x80000,
		lOlOlOOOOOlO =					0x100000,
		lOlOlOOOOOll =				0x200000,
	};	
#ifdef TA_PHYSICS_INTERNAL
private:
#endif 
	float lOlOlOlOOlO;
	float lOOlOllllOlO;
	float lOOllOOlllOl;
	float lOOllOOllllO;
	float lOOllOOlllll;
	float lOOllOlOOOOO;
	float lOOlllOlOOll;
	float lOlOOlllOOOO;
	float lOOllOOllOOO;
	float lOOllOOllOOl;
	int lOOllOlOOOOl;
	u32 lllOlOlllO;
	int lOOllOlOOOlO;
	int lOlOOOllOlOl;
	void* lOOllOlOlOOl;
	void* lOOllOlOlOlO;
	int lOOllOlOllOO;
	u32 lOOllOlOllOl; 
	u32 lOOllOlOlllO;			
	CollisionObject* lOOllOOlOlOO;
	DynamicObjectGroup* lOOllOlOllll;
	DynamicObject* lOOllOllOOOO; 
	void* lOOllOllOOOl;
	lOlOOOOOlOOO* lOOllOllOOlO;
	int lOOllOllOOll;
	int lOOllOllOlOO;
	int lOOllOllOlOl;
	llllOllOlOO* lOOllOlllOlO;
	lOOOOlOllO<LineIntersection, false> lOOllOllOllO;
	List<PhysicsJoint> lOOllOllOlll;	
	List<PhysicsJoint*> lOOllOlllOOO;	
	Mat33 lOOllOOlOllO; 
	Mat33 lOOllOOlOlll;
	Mat33 lOOllOOllOll; 
	Mat33 lOOllOOlllOO;
	MFrame lOOllOlOOlOl;
	MFrame lOOllOlOOllO;
	MFrame lOOllOllllOO;
	MFrame lOOllOllllOl;
	MFrame lOOllOlllllO;
	Vec3 lOOllOlOOOll;
	Vec3 lOOlOllllOOl;
	Vec3 lOOlOllllOll;
	Vec3 lOOllOlOOlOO;
	Vec3 lOOlOlllOOll;
	Vec3 lOOllOlOOlll;
	Vec3 lOOllOlOlOOO;
	Vec3 lOOlllOlOOlO;
	Vec3 lOOlllOlOlOl;
	Vec3 lOOlllOlOllO;
	Vec3 lOOlllOlOlll;
	AABB lOOllOlOlOll;
#ifndef lOlOlOOOOlOO
	enum { lOlOOOlOllll = 10 * sizeof (uSize) + 5 * sizeof (u32) + 2 * sizeof (uSize) + 5 * sizeof (u32) };
	
	u8* lOOllOlllOll;
#endif 
	
	bool lOOlllOlOOOl() const { return (lllOlOlllO & (lOOlllOOlOOl | lOOlllOOOlll | lOlOOlOOllOO)) == lOlOOlOOllOO; }

	
	virtual const Vec3& GetCollisionGridWorldPos() const { return lOOllOlOlOll.v3Center; }
	virtual const AABB& GetCollisionGridWorldAABB() const { return lOOllOlOlOll; }
	virtual const AABB& GetCollisionGridGetLocalAABB() const { return lOOllOOlOlOO->GetAABB(); }
#ifdef lOlOlOOOOlOO
	virtual void* GetSpaceDivisionNodeMemory() { return 0; }
	virtual int GetSpaceDivisionNodeMemorySize() { return 0; }
#else
	virtual void* GetSpaceDivisionNodeMemory() { return lOOllOlllOll; }
	virtual int GetSpaceDivisionNodeMemorySize() { return lOlOOOlOllll; }
#endif

};

}

#include "../Physics/zpta006.inl"

#endif 
