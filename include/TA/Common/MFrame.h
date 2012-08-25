//---------------------------------------------------------------------------------
// File Name: MFrame.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_MFRAME_H
#define TA_MFRAME_H

#ifndef TA_MATRIX_H
#include "Matrix.h"
#endif 



struct D3DXMATRIX;

namespace TA
{

struct TACOMMON_CLASS MFrame
{
public:
	
	
	Mat33 m33Rotation;
	Vec3 v3Translation;
	

	
	
	
	MFrame();
	
	MFrame(const MFrame& mFrame);
	MFrame(const Vec3& v3NewTranslation);
	MFrame(const Mat33& m33NewRotation);
	MFrame(
	const Vec3& v3NewTranslation,
	const Mat33& m33NewRotation);
	
	void Clear();

	
	void Initialise(D3DXMATRIX* pD3DMatrix);
	void GetAsD3DMatrix(D3DXMATRIX* pD3DMatrix) const;
	

	
	void InitialiseFromOpenGlMatrix(const float* pfMatrix);
	void GetOpenGlMatrix(float* pfMatrix) const;
	

	void Interpolate(MFrame& a, MFrame& b, float fT);

    
	
	MFrame& operator *= (const MFrame& mFrame);
	MFrame& operator *= (const Mat33& m33Value);
	MFrame& operator /= (const MFrame& mFrameB);
	

	
	
	MFrame operator * (const MFrame& mFrame) const;
	MFrame operator * (const Mat33& m33Value) const;
	MFrame operator / (const MFrame& mFrameB) const;
	

	friend Vec3 operator * (const Vec3& v3A, const MFrame& mFrameB);
	friend Vec3 operator / (const Vec3& v3A, const MFrame& mFrameB);
	friend Vec3& operator *= (Vec3& v3A, const MFrame& m33B);
	friend Vec3& operator /= (Vec3& v3A, const MFrame& m33B);

	
	
	bool operator == (const MFrame& mFrame) const;
	bool operator != (const MFrame& mFrame) const;
	
	
	
	void Invert();
	static MFrame TAC_CALL GetInverse(const MFrame& mFrame);
	MFrame GetInverse() const;

	
	Vec3 GetTransform(const Vec3& v3Value);
	Vec3 GetInverseTransform(const Vec3& v3Value);

	void Mirror(const Vec3& v3DirectionToMirror, const Vec3& v3PointToMirrorAbout);
	
};


const MFrame k_mFrameIdentity(k_v3Zero, k_m33Identity);


}

#include "../Common/zcta005.inl"


#endif 
