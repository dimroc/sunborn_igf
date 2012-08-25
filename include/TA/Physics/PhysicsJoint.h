//---------------------------------------------------------------------------------
// File Name: PhysicsJoint.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_PHYSICSJOINT_H
#define TA_PHYSICSJOINT_H

#ifndef TA_MATRIX_H
#include "../Common/Matrix.h"
#endif 

#ifndef lllOlOllOO
#include "../Common/zcta005.h"
#endif 

#ifndef llOlOOlOOOll
#include "../Common/EulerAngles.h"
#endif 

#ifndef lOlOllllOOl
#include "../Physics/zpta012.h"
#endif 



namespace TA
{

class DynamicObject;

class lOlOllllOlO PhysicsJoint
{
public:
	
	//pl.BeginExportClass(PhysicsJoint)
	enum Type
	{
		TYPE_BALL_AND_SOCKET = 0,
		TYPE_LIMITED_BALL_AND_SOCKET = 1,
		TYPE_SQUARE_SOCKET = 2,
		TYPE_EULER_ANGLE_CONSTRAINT = 31,
		TYPE_LINEAR_SLIDER = 4,
		TYPE_ROTATION_CONSTRAINT = 5,
		TYPE_VECTOR_CONSTRAINT = 6,
	};
	PhysicsJoint();
	~PhysicsJoint();

	
	
	void InitialiseEulerConstraint(
		const Vec3& v3Position,
		const Mat33& m33JointOrientation,
		const EulerAngles& minAngles,
		const EulerAngles& maxAngles);
	void InitialiseHinge(
		const Vec3& v3Position,
		const Vec3& v3HingeAxis,
		float fMinAngle,
		float fMaxAngle);
	void InitialiseBallAndSocket(
		const Vec3& v3Position,
		const Vec3& v3LimitCenterDirection,
		float fMaxAngle);
	void InitialiseLimitedBallAndSocket(
		const Vec3& v3Position,
		const Mat33& m33JointOrientation,
		float fMaxAngleY,
		float fMaxAngleXZ);
	void InitialiseSquareSocket(
		const Vec3& v3Position,
		const Mat33& m33JointOrientation,
		float fMinX,
		float fMaxX,
		float fMinY,
		float fMaxY,
		float fMinZ,
		float fMaxZ);
	void InitialiseSlider(
		const Vec3& v3PosOnA,
		const Vec3& v3StartPosOnB,
		const Vec3& v3EndPosPosOnB);
	void InitialiseRotationConstraint(
		const Mat33& m33JointOrientation,
		const EulerAngles& minAngles,
		const EulerAngles& maxAngles);
	void InitialiseVectorConstraint(
		const Vec3& v3LimitCenterDirection,
		float fMaxAngle);
	


	
	
	void InitialiseEulerConstraintEx(
		const Vec3& v3LocalPosOnA,
		const Vec3& v3LocalPosOnB,
		const Mat33& m33JointOrientation,
		const Mat33& m33DefaultRotationA,
		const Mat33& m33DefaultRotationB,
		const EulerAngles& minAngles,
		const EulerAngles& maxAngles);
	void InitialiseHingeEx(
		const Vec3& v3LocalPosOnA,
		const Vec3& v3LocalPosOnB,
		const Vec3& v3HingeAxis, 
		const Vec3& v3HingeCenterDirection, 
		const Mat33& m33DefaultRotationA,
		const Mat33& m33DefaultRotationB,
		float fMinAngle,
		float fMaxAngle);
	void InitialiseBallAndSocketEx(
		const Vec3& v3LocalPosOnA,
		const Vec3& v3LocalPosOnB,
		const Vec3& v3LocalLimitCenterDirectionOnA,	
		const Vec3& v3LocalLimitCenterDirectionOnB,
		float fMaxAngle);
	void InitialiseLimitedBallAndSocketEx(
		const Vec3& v3LocalPosOnA,
		const Vec3& v3LocalPosOnB,
		const Mat33& m33JointOrientation,
		const Mat33& m33DefaultRotationA,
		const Mat33& m33DefaultRotationB,
		float fMaxAngleY,
		float fMaxAngleXZ);
	void InitialiseSquareSocketEx(
		const Vec3& v3LocalPosOnA,
		const Vec3& v3LocalPosOnB,
		const Mat33& m33JointOrientation,
		const Mat33& m33DefaultRotationA,
		const Mat33& m33DefaultRotationB,
		float fMinX,
		float fMaxX,
		float fMinY,
		float fMaxY,
		float fMinZ,
		float fMaxZ);	
	void InitialiseSliderEx(
		const Vec3& v3LocalPosOnA,
		const Vec3& v3LocalStartPosOnB,
		const Vec3& v3LocalEndPosPosOnB);
	void InitialiseRotationConstraintEx(
		const Mat33& m33JointOrientation,
		const Mat33& m33DefaultRotationA,
		const Mat33& m33DefaultRotationB,
		const EulerAngles& minAngles,
		const EulerAngles& maxAngles);
	void InitialiseVectorConstraintEx(
		const Vec3& v3LocalLimitCenterDirectionOnA,
		const Vec3& v3LocalLimitCenterDirectionOnB,
		float fMaxAngle);
	


	
	
	
	
	void Reset(
		const Vec3& v3PosOnA,
		const Vec3& v3PosOnB,
		const Mat33& defaultRotationB,
		const EulerAngles& minAngles,
		const EulerAngles& maxAngles);
	
	
	
	void ResetAsTypeHinge(
		const Vec3& v3PosOnA,
		const Vec3& v3PosOnB,
		const Vec3& v3LocalHingeAxis, 
		const Vec3& v3LocalHingeNormal, 
		float fMinAngle,
		float fMaxAngle);
	
	void ResetAsTypeSocket(
		const Vec3& v3PosOnA,
		const Vec3& v3PosOnB,
		const Vec3& v3LimitNormalOnA,	
		const Vec3& v3LimitNormalOnB,
		float fMaxAngle);

	
	
	void ResetAsTypeLimitedSocket(
		const Vec3& v3PosOnA,
		const Vec3& v3PosOnB,
		const Mat33& defaultRotationB,
		float fMaxAngleY,
		float fMaxAngleXZ);

	
	
	void ResetAsTypeSquareSocket(
		const Vec3& v3PosOnA,
		const Vec3& v3PosOnB,
		const Mat33& defaultRotationB,
		float fMinX,
		float fMaxX,
		float fMinY,
		float fMaxY,
		float fMinZ,
		float fMaxZ);
	
	
	
	void ResetAsTypeSlider(
		const Vec3& v3PosOnA,
		const Vec3& v3StartPosOnB,
		const Vec3& v3EndPosPosOnB);
	
	
	
	void ResetAsTypeRotationConstraint(
		const Mat33& defaultRotationB,
		const EulerAngles& minAngles,
		const EulerAngles& maxAngles);
	
	
	
	void ResetAsTypeVectorConstraint(
		const Vec3& v3LimitNormalOnA,
		const Vec3& v3LimitNormalOnB,
		float fMaxAngle);
	


	
	
	bool IsObjectASolid() const;
	void SetObjectASolid(bool bValue);
	bool IsObjectBSolid() const;
	void SetObjectBSolid(bool bValue);
	bool IsCollisionDisabledBetweenObjects() const;
	void SetCollisionDisabledBetweenObjects(bool bValue);
	

	
	
	float GetBreakForce() const;
	void SetBreakForce(float fBreakForce);
	void SetBreakable(bool bValue);
	bool IsBreakable() const;
	void SetBroken(bool bValue);
	bool IsBroken() const;
	

	
	
	void* GetUserData() const;
	void SetUserData(void* pUserData);
	

	
	
	Type GetType() const;
	DynamicObject* GetObjectA();
	DynamicObject* GetObjectB();
	const Vec3& GetPosOnObjectA() const;
	const Vec3& GetPosOnObjectB() const;
	const Vec3& GetLimitNormalOnObjectA() const;
	const Vec3& GetLimitNormalOnObjectB() const;
	const Vec3& GetCenterDirectionOnObjectA() const;
	const Vec3& GetCenterDirectionOnObjectB() const;
	float GetMaxAngle() const;
	const Mat33& GetJointOrientationA();
	const Mat33& GetJointOrientationB();
	const EulerAngles& GetMinAngles();
	const EulerAngles& GetMaxAngles();
	const Vec3& GetSliderPos0() const;
	const Vec3& GetSliderPos1() const;
	

	//pl.EndExportClass(PhysicsJoint)
	
#ifdef TA_PHYSICS_INTERNAL
	bool llOOOlOlOlOO() const { return lllOllOOOl(lllOlOlllO, FLAG_MARKED); }
	void llOOOlOOlllO(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, FLAG_MARKED, lllOllOOOO); }
	bool llOOOlOlllOl() const { return lllOllOOOl(lllOlOlllO, llOlOOlOOlOO); }
	void llOOOlOlOOOO(bool lllOllOOOO) { lllOlOllOl(lllOlOlllO, llOlOOlOOlOO, lllOllOOOO); }
	const EulerAngles& llOOOOOOOOll() const { return llOlOOOOOOOO; }
	void lOOlllOOOOlO(const EulerAngles& llOlOOlOOlOl) { llOlOOOOOOOO = llOlOOlOOlOl; }
	float llOlOOOllOOO(Vec3& llOlOOOllOOl, Vec3* lOlOllllllOl);
	void Initialise(DynamicObject* lOlOOOOOlOll,	DynamicObject* lOlOOOOOllOO);
	void Finalise();
	void lOllllOllOll(u32 lOOllOOlllO) { llOlOOlOOllO = lOOllOOlllO; }
	u32 lOllllOllOOO() const { return llOlOOlOOllO; }
#endif 

private:
	enum 
	{
		FLAG_OBJECTA_SOLID = 0x1,
		FLAG_OBJECTB_SOLID = 0x2,
		llOlOOlOOlll = 0x4,
		FLAG_MARKED = 0x8,
		llOlOOlOOlOO = 0x10,
		llOlOOlOlOOO = 0x20,
		llOOllllOlOO = 0x40,
	};
	PhysicsJoint(PhysicsJoint&); 
	void lOOllOllllll();

	Vec3 llOOllllOlOl;
	Vec3 llOOllllOllO;
	Vec3 llOOlllllOOl;
	Vec3 llOOlllllOlO;
	Mat33 llOOllllllOO;
	Mat33 llOOllllllOl;
	EulerAngles llOOlllllllO;
	EulerAngles llOOllllllll;
	EulerAngles llOlOOOOOOOO;
	u16 llOlOOOOOlll;
	u16 lllOlOlllO;
	DynamicObject* llOOllllOlll;
	DynamicObject* llOOlllllOOO;
	float llOOlllllOll;
	float llOlOOOOOOOl;
	int llOlOOlOOllO;
	void* lOOllOllOOOl;
};

}

#include "../Physics/zpta009.inl"

#endif 
