//---------------------------------------------------------------------------------
// File Name: Matrix.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_MATRIX_H
#define TA_MATRIX_H

#ifndef TA_VECTOR_H
#include "Vector.h"
#endif 



struct D3DXMATRIX;

namespace TA
{

struct EulerAngles;

struct TACOMMON_CLASS Mat33
{
public:


	
	Vec3 v3X;
	Vec3 v3Y;
	Vec3 v3Z;	




	enum RotationAxis
	{
		ROTATION_AXIS_X = 0,
		ROTATION_AXIS_Y,
		ROTATION_AXIS_Z,
	};

	
	
    Mat33();
    
    Mat33(const Mat33& m33Value);
    Mat33(
		float f11, float f12, float f13,
		float f21, float f22, float f23,
		float f31, float f32, float f33);
	explicit Mat33(const EulerAngles& eulerAngles) { Initialise(eulerAngles); }
	Mat33(const Vec3& v3Axis, float fAngle) { Initialise(v3Axis, fAngle); }
	Mat33(RotationAxis eRotationAxis, float fAngle) { Initialise(eRotationAxis, fAngle); }

	void Initialise(const EulerAngles& eulerAngles);
	void Initialise(const Vec3& v3Axis, float fAngle);
	void Initialise(RotationAxis eRotationAxis, float fAngle);
	void SetToLookDownVector(const Vec3& v3Vector);
	void SetToLookAt(const Vec3& v3Vector, const Vec3& v3UpVector);
	void Interpolate(const Mat33& m33A, const Mat33& m33B, float fT);

	void Initialise(D3DXMATRIX* pD3DMatrix);
	void GetAsD3DMatrix(D3DXMATRIX* pD3DMatrix) const;
	

    
	
    float& operator () (int nRow, int nCol);
    float  operator () (int nRow, int nCol) const;
	Vec3& operator [] (int nRow);
	const Vec3&  operator [] (int nRow) const;
	

    


    
	
    Mat33& operator *= (const Mat33& m33Value);
    Mat33& operator /= (const Mat33& m33Value);
    Mat33& operator += (const Mat33& m33Value);
    Mat33& operator -= (const Mat33& m33Value);
    Mat33& operator *= (float fValue);
    Mat33& operator /= (float fValue);
	

    
	
    const Mat33& operator + () const;
    Mat33 operator - () const;
	

    
	
    Mat33 operator * (const Mat33& m33Value) const;
    Mat33 operator / (const Mat33& m33Value) const;
    Mat33 operator + (const Mat33& m33Value) const;
    Mat33 operator - (const Mat33& m33Value) const;
    Mat33 operator * (float fValue) const;
    Mat33 operator / (float fValue) const;
	

    friend Mat33 operator * (float fValue, const Mat33& m33Value);
    friend Vec3 TA_FAST_CALL operator * (const Vec3& v3A, const Mat33& m33B);
    friend Vec3 operator / (const Vec3& v3A, const Mat33& m33B);
    friend Vec3& operator *= (Vec3& v3A, const Mat33& m33B);
    friend Vec3& operator /= (Vec3& v3A, const Mat33& m33B);

    
	
    bool operator == (const Mat33& m33Value) const;
    bool operator != (const Mat33& m33Value) const;
	

	float& M11() { return v3X.x; }
	float& M12() { return v3X.y; }
	float& M13() { return v3X.z; }

	float& M21() { return v3Y.x; }
	float& M22() { return v3Y.y; }
	float& M23() { return v3Y.z; }

	float& M31() { return v3Z.x; }
	float& M32() { return v3Z.y; }
	float& M33() { return v3Z.z; }

	const float& M11() const { return v3X.x; }
	const float& M12() const { return v3X.y; }
	const float& M13() const { return v3X.z; }

	const float& M21() const { return v3Y.x; }
	const float& M22() const { return v3Y.y; }
	const float& M23() const { return v3Y.z; }

	const float& M31() const { return v3Z.x; }
	const float& M32() const { return v3Z.y; }
	const float& M33() const { return v3Z.z; }

	
	static float TAC_CALL GetDeterminant(const Mat33& m33Value);
	float GetDeterminant() const;

	
	void Invert();
	static Mat33 TAC_CALL GetInverse(const Mat33& m33Value);
	Mat33 GetInverse() const;

	
	void Orthogonalise();
	bool IsOrthogonal() const;

	
	void SetToIdentity();
	static Mat33 TAC_CALL GetIdentity();

	
	void Transpose();
	static Mat33 TAC_CALL GetTranspose(const Mat33& m33Value);
	Mat33 GetTranspose() const;

	
	void TransposeAsInvert();
	static Mat33 TAC_CALL GetTransposeAsInverse(const Mat33& m33Value);
	Mat33 GetTransposeAsInverse() const;
	
	
	void GetRotationAxisAndMagnitude(Vec3& v3Axis, float& fAngle) const;

	void PreRotate(RotationAxis eRotationAxis, float fAngle);

	static void TA_FAST_CALL Mul(const Mat33& m33A, const Mat33& m33B, Mat33& m33Dst);
	static void TA_FAST_CALL MulInPlace(Mat33& m33A, const Mat33& m33B);
	static void TA_FAST_CALL MulByTranspose(const Mat33& m33A, const Mat33& m33B, Mat33& m33Dst);
	static void TA_FAST_CALL MulByTransposeInPlace(Mat33& m33A, const Mat33& m33B);
    static void TA_FAST_CALL MulByTranspose(const Vec3& v3A, const Mat33& m33B, Vec3& v3Dst);


};


const Mat33 k_m33Identity(
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f);


};

#include "../Common/zcta004.inl"


#endif 

