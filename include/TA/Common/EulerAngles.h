//---------------------------------------------------------------------------------
// File Name: EulerAngles.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_EULERANGLES_H
#define TA_EULERANGLES_H

#ifndef TA_COMMON_H
#include "Common.h"
#endif 



namespace TA
{

struct Mat33;

struct TACOMMON_CLASS EulerAngles
{
	
	
	float x;
	float y;
	float z;

	EulerAngles() {}
	EulerAngles(const EulerAngles& eulerAngles) { x = eulerAngles.x; y = eulerAngles.y; z = eulerAngles.z; }
	EulerAngles(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
	explicit EulerAngles(Mat33& m33) { Initialise(m33); }

	void Initialise(const Mat33& m33);
	void Initialise(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }

	void UpdateTo(const EulerAngles& eulerAngles);

	void Clear() { x = 0.0f, y = 0.0f, z = 0.0f; }

	Mat33 GetAsMat33();

	void WrapYRotation();
	

};

};

#endif 
