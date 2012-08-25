//---------------------------------------------------------------------------------
// File Name: DynamicObjectCar.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_DYNAMICOBJECTCAR_H
#define TA_DYNAMICOBJECTCAR_H

#ifndef TA_DYNAMICOBJECT_H
#include "DynamicObject.h"
#endif 



namespace TA
{

class CollisionObjectCombo;
class CarTemplate;
class SurfaceAttribute;

class lOlOllllOlO DynamicObjectCar : public DynamicObject
{
public:
	enum lOlOllOlOllO
	{
		lOlOlOOlOlOO =				0x1,
		lOlOlOOlOlOl =				0x2,
		lOlOlOOlOllO =	0x4,
		lOlOlOllOOOl =			0x8,
	};
	
	struct Wheel
	{
	public:
		typedef float (TAC_CALL *FnSlipRatioToNormalisedTraction)(float fSlipRatio, const SurfaceAttribute& surfaceAttribute);
		typedef float (TAC_CALL *FnSlipAngleToLateralForce)(float fSlipAngle, float fLongitudinalSlipRatio, const SurfaceAttribute& surfaceAttribute);

		const MFrame& GetLocalGraphicsFrame() const;
		
		void SetRadius(float fRadius);
		float GetRadius() const;

		void SetInvInertia(float fInvInertia);
		float GetInvInertia() const;

		void SetBrakeTorque(float fBrakeTorque);
		float GetBrakeTorque() const;

		void SetRollResistance(float fRollResistance);
		float GetRollResistance() const;

		void SetSuspensionSpringConstant(float fSuspensionSpringConstant);
		float GetSuspensionSpringConstant() const;

		void SetSuspensionDampingConstant(float fSuspensionDampingConstant);
		float GetSuspensionDampingConstant() const;
		
		void SetSlipRatioToNormalisedTractionCallBack(FnSlipRatioToNormalisedTraction pCallBack);
		FnSlipRatioToNormalisedTraction GetSlipRatioToNormalisedTractionCallBack() const;
	
		void SetSlipAngleToLateralForceCallBack(FnSlipAngleToLateralForce pCallBack);
		FnSlipAngleToLateralForce GetSlipAngleToLateralForceCallBack() const;

		void SetSteeringFlag(bool bSteering);
		bool GetSteeringFlag() const;

		void SetDrivingFlag(bool bDriving);
		bool GetDrivingFlag() const;

		void SetHandBrakeFlag(bool bHandBrake);
		bool GetHandBrakeFlag() const;

		void SetYaw(float fYaw);
		float GetYaw() const;

		void SetPitch(float fPitch);
		float GetPitch() const;

		void SetPitchVelocity(float fPitchVelocity);
        float GetPitchVelocity() const;

		void SetCamberAngle(float fCamberAngle);
		float GetCamberAngle() const;

		

	private:
		friend class DynamicObjectCar;

		MFrame lOlOlOOlOlll;
		Mat33 lOlOlOOllOOO;
		Mat33 lOlOlOOllOOl;		
		Vec3 lOlOlOllOOOO;
		float lOlOlOOlOOO;
		float lOlOlOOllOlO;
		float lOlOlOOllOll;
		float lOlOlOOlllOO;
		float lOlOlOOlllOl;
		float lOlOlOOllllO;
		float lOlOlOOlOll;
		float lOlOlOOlOlO;
		float lOlOlOOlllll;
		float lOlOlOOllOO;
		float lOlOlOOllOl;
		float (TAC_CALL *lOlOlOOlllO)(float lOlOllOlOlO, const SurfaceAttribute& lOlOllOlOll);
		float (TAC_CALL *lOlOlOOllll)(float lOlOlllOllO, float lOlOlllOlll, const SurfaceAttribute& lOlOllOlOll);
		u32 lllOlOlllO;
	};

	
	
	
	void Initialise(CarTemplate* pCarTemplate);
	void Initialise(const Char* szFileName);
	void Finalise();
	

	
	
	virtual void Update(float fDt);

	void ClearControls();
	void UpdateControls(
		float fSteeringAngle,
		float fAccelerator,
		float fBrake,
		float fHandBrake,
		float fSteeringHelp,
		float fTractionControl,
		int nGear = -1);
	


	
	
	float GetRPM() const;
	int GetGear() const;
	int GetNumGears() const;
	float GetCarSpeedFromWheels();
	
	void SetTorqueMultiplier(float fTorqueMultiplier);
	float GetTorqueMultiplier() const;

	void SetAntiRollFudge(float fAntiRollFudge);
	float GetAntiRollFudge() const;
	const Wheel& GetWheel(int nIndex) const;
	Wheel& GetWheel(int nIndex);
	int GetNumWheels() const;

	void SetOrientation(const Vec3& v3Forward, const Vec3& v3Up);
	const Vec3& GetForward() const;
	const Vec3& GetUp() const;
	

	static DynamicObjectCar* TAC_CALL CreateNew();

protected:
	DynamicObjectCar();
	~DynamicObjectCar();

	

private:
	struct lOlOllOlOlll
	{
		float lOlOlOlOlllO;
		float lOlOlOllllOl;
		float lOlOlOllllOO;
		float lOlOllOlOlOO;
		float lOlOlOlOllOl;
		float lOlOllOOlOlO;
		int lOlOlOlOOOlO;
	};

	lOlOllOlOlll lOlOlOlOllOO;
	Vec3 lOlOllOOlOO;
	Vec3 lOlOllOOlOl;
	lOOOOlOllO<Wheel, false> lOlOlOOlOOOl;

	int lOlOlOllOllO;

	float lOlOlOlOOOll;
	float lOlOlOlOOlOO;

	lOOOOlOllO<float, false> lOlOlOlOOOOl;
	float lOlOlOlOlll;
	float lOlOlOlOlOl;
	float lOlOlOlOOll;
	float lOlOlOllOlO;
	float (TAC_CALL *lOlOlOlOlOO)(float lOlOllOlOOO);

	float lOlOlOllOOO;
	float lOlOlOllOOl;

	void InitialiseCommon();
	void lOlOlOlOOOOO(Wheel& lOlOlOllllO);
};

}

#include "../Physics/zpta007.inl"

#endif 
