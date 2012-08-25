#pragma once
using std::string;

class CPillar
{
public:
	struct SPillarInfo {
        string strTexName;
		float cylinderRadius, splineRadius, Length;
		float splineAngle;
		UINT numSlices, numStacks;
		UINT curveStacks, curveHeight;
		UINT texWrapLength;
    };

	CPillar(LPCSTR texFN);
    CPillar(LPCSTR texFN, D3DXMATRIX mWorld);
	// constructor which takes in all parameters for fully customized pillar
	CPillar(LPCSTR texFN, D3DXMATRIX mWorld, float cylRad, float splineRad, float length,
			float splineAngle, float numSlices, float numStacks, UINT curveStacks, UINT curveHeight);	
	CPillar(const SPillarInfo* pillInfo, D3DXMATRIX mWorld);
	~CPillar(void);	
	

	// D3D Functions
	HRESULT OnResetDevice(IDirect3DDevice9 *pd3dDevice);
	HRESULT OnLostDevice(void);
	HRESULT OnFrameRender(IDirect3DDevice9 *pd3dDevice);

	// Temp Util Functions

	void incSplineRadius(float delta);
	void incSplineAngle(float delta);
	void incSplineStacks(int delta);
	void incSplineHeight(float delta);
	void incCylStacks(int delta);
	void incCylRadius(float delta);
	void incCylLength(float delta);
	void incSlices(int delta);
	void incTexWrap(int delta);

	void toggleInteractiveMode(void);
	bool isInteractive(void);
	UINT HandleInput(UINT nchar, bool bAltDown);
	bool m_bInteractive;

	void writeValsToFile(void);

	bool m_bChanged;
	int fNum;

private:
	D3DXMATRIX	m_mWorld;
	IDirect3DVertexBuffer9 *m_pVB;
	IDirect3DIndexBuffer9  *m_pIB;
	
	IDirect3DDevice9 *m_pd3dDevice;		// this should only be used in the modify functions and taken out 
										// when the final geometry has been chosen
	IDirect3DTexture9 *m_pTexture;
	string m_strTexName;
	float m_cylinderRadius, m_splineRadius, m_Length;
	float m_splineAngle;
	UINT m_numSlices, m_numStacks;
	UINT m_curveStacks, m_curveHeight;
	UINT m_numVertices, m_numIndices;
	UINT m_textureWrapLength;
	// these variables to speed up calculations when creating pillar
	D3DVECTOR m_circleCenter;
	float m_theta;
	double maxY;
    
    // TODO: Generate Corner Vectors. 
    // NOTE: gdef:: GenerateAABBPosMiddle will spit out corners given a center/displacement vector
    //  4 --- 5                     3 --- 2 - top
    //  |     |                     |     |
    //  0 --- 1  - bottom face      7 --- 6
    D3DXVECTOR3 corners[8]; //! AABB corners to test for visibility. Done ala QTNode.

	// Private utility functions
	D3DXVECTOR4 mul(D3DXMATRIX m, D3DXVECTOR4 v);
	void findCircleCenter(UINT slice);
	void getCircleXZCoord(float &x, float &z, UINT slice);
	void getSplineCoordsAndNormals(float &x, float &y, float &z, float &nx, float &ny, float &nz, UINT stack, UINT slice);
	void setIndices(WORD *indices, UINT totalStacks);
};
