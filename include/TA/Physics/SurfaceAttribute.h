//---------------------------------------------------------------------------------
// File Name: SurfaceAttribute.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_SURFACEATTRIBUTE_H
#define TA_SURFACEATTRIBUTE_H

#ifndef TA_TYPES_H
#include "../Common/Types.h"
#endif 

#ifndef TA_MATHS_H
#include "../Common/Maths.h"
#endif 

#ifndef lOlOllllOOl
#include "../Physics/zpta012.h"
#endif 



namespace TA
{

class lOlOllllOlO SurfaceAttribute
{
public:
	
	SurfaceAttribute();
	SurfaceAttribute(const SurfaceAttribute& that);
	~SurfaceAttribute() {}

	void Clear();

	void SetCollisionFriction(float fCollisionFriction);
	float GetCollisionFriction() const;

	void SetRestitution(float fRestitution);
	float GetRestitution() const;

	void SetTireFriction(float fTireFriction);
	float GetTireFriction() const;

	void SetTireFrictionFallOff(float fTireFrictionFallOff);
	float GetTireFrictionFallOff() const;

	void SetUserData(u32 nUserData);
	u32 GetUserData() const;
	

private:
	float lllOOOlOlOll;
	float lOOllOOllllO;
	float lllOOOlOllOO;
	float lllOOOlOllOl;
	u32 llOllOOllll;
};

inline void SurfaceAttribute::SetCollisionFriction(float lllOOOlOlllO) { lllOOOlOlOll = lllOOOlOlllO; }
inline float SurfaceAttribute::GetCollisionFriction() const { return lllOOOlOlOll; }
inline void SurfaceAttribute::SetRestitution(float lOllOOlOOll) { lOOllOOllllO = lOllOOlOOll; Clamp(lOOllOOllllO, -1.0f, 1.0f); }
inline float SurfaceAttribute::GetRestitution() const { return lOOllOOllllO; }
inline void SurfaceAttribute::SetTireFriction(float lllOOOlOllll) { lllOOOlOllOO = lllOOOlOllll; }
inline float SurfaceAttribute::GetTireFriction() const { return lllOOOlOllOO; }
inline void SurfaceAttribute::SetTireFrictionFallOff(float lllOOOllOOOO) { lllOOOlOllOl = lllOOOllOOOO; }
inline float SurfaceAttribute::GetTireFrictionFallOff() const { return lllOOOlOllOl; }
inline void SurfaceAttribute::SetUserData(u32 llOllOlOOOO) { llOllOOllll = llOllOlOOOO; }
inline u32 SurfaceAttribute::GetUserData() const { return llOllOOllll; }

}

#endif 
