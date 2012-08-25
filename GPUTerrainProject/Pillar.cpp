#include "dxstdafx.h"
#include ".\pillar.h"
#include "FXDecl.h"
#include "gamedefine.h"
#include "string"

#define PI acos(-1.f)

#define LENGTH	1000.0f
#define	RAD1	150.0f
#define	RAD2	205.0f

#define	STACKS	10
#define	SLICES	10

#define SPLINEANGLE 90
#define SPLINEHEIGHT 200
#define CURVESTACKS 10

#define MINDOUBLE -0x7FFFFFFFFFFFFFFF

CPillar::CPillar(LPCSTR texFN) : m_Length(LENGTH), m_cylinderRadius(RAD1),
					   m_splineRadius(RAD2), m_numStacks(STACKS), m_numSlices(SLICES),
					   m_splineAngle(SPLINEANGLE), m_curveStacks(CURVESTACKS), m_pTexture(NULL),
					   m_curveHeight(SPLINEHEIGHT), m_numIndices(0), m_numVertices(0),
                       m_strTexName(texFN), maxY(MINDOUBLE), m_textureWrapLength(LENGTH)
{	
	m_bInteractive = false;
	m_bChanged = true;
	fNum = 0;
}

CPillar::CPillar(LPCSTR texFN, D3DXMATRIX mWorld) : m_Length(LENGTH), m_cylinderRadius(RAD1),
					   m_splineRadius(RAD2), m_numStacks(STACKS), m_numSlices(SLICES),
					   m_splineAngle(SPLINEANGLE), m_curveStacks(CURVESTACKS), m_pTexture(NULL),
					   m_curveHeight(SPLINEHEIGHT), m_numIndices(0), m_numVertices(0),
                       m_strTexName(texFN), m_mWorld(mWorld), maxY(MINDOUBLE), m_textureWrapLength(LENGTH)
{
	m_bInteractive = false;
	m_bChanged = true;
	fNum = 0;
}

CPillar::CPillar(LPCSTR texFN, D3DXMATRIX mWorld, float cylRad, float splineRad, float length,
		float splineAngle, float numSlices, float numStacks, UINT curveStacks, UINT curveHeight) :
				m_Length(length), m_cylinderRadius(cylRad),
				m_splineRadius(splineRad), m_numStacks(numStacks), m_numSlices(numSlices),
				m_splineAngle(splineAngle), m_curveStacks(curveStacks), m_pTexture(NULL),
				m_curveHeight(curveHeight), m_numIndices(0), m_numVertices(0),
				m_strTexName(texFN), m_mWorld(mWorld), maxY(MINDOUBLE), m_textureWrapLength(LENGTH)
{	
	m_bInteractive = false;
	m_bChanged = true;
	fNum = 0;
}

CPillar::CPillar(const SPillarInfo* pillInfo, D3DXMATRIX mWorld) : 
				m_Length(pillInfo->Length), m_cylinderRadius(pillInfo->cylinderRadius),
				m_splineRadius(pillInfo->splineRadius), m_numStacks(pillInfo->numStacks), m_numSlices(pillInfo->numSlices),
				m_splineAngle(pillInfo->splineAngle), m_curveStacks(pillInfo->curveStacks), m_pTexture(NULL),
				m_curveHeight(pillInfo->curveHeight), m_numIndices(0), m_numVertices(0),
				m_strTexName(pillInfo->strTexName), m_mWorld(mWorld), maxY(MINDOUBLE), m_textureWrapLength(pillInfo->texWrapLength)
{
	m_bInteractive = false;
	m_bChanged = true;
	fNum = 0;
}

CPillar::~CPillar( void )
{
}

HRESULT CPillar::OnResetDevice( IDirect3DDevice9 *pd3dDevice )
{
	HRESULT hr;
	UINT totalStacks = (m_curveStacks + m_numStacks);
	m_numVertices = totalStacks*m_numSlices;
	m_numIndices  = m_numVertices*2 + m_numSlices*2;	// each vertex indexed twice in triangle strip, 2 degen vertices per slice
	float totalHeight = m_curveHeight + m_Length;
	maxY = MINDOUBLE;

	// Create vertex buffer
	V(pd3dDevice->CreateVertexBuffer(
		m_numVertices*sizeof(TERRAINVERT),
		D3DUSAGE_WRITEONLY,
        0, // we use vertex declarations. no fvfs
		D3DPOOL_MANAGED,
		&m_pVB,
		0) );		

	// set up vertices so that can index in one triangle strip
	TERRAINVERT *vertices; 
	V( m_pVB->Lock(0, 0, (void**)&vertices, 0) );
	UINT count = 0;
	
	for(UINT slice = 0; slice < m_numSlices; ++slice)
	{
		float x, y, z;
		
		getCircleXZCoord(x, z, slice);
		y = -totalHeight;
		// first do just the cylinder part
		for(UINT stack = 0; stack < m_numStacks; ++stack)
		{			
			vertices[count++] = TERRAINVERT(x, y, z, x, 0, z, slice/float(m_numSlices), -y/m_textureWrapLength);
			y += m_Length/float(m_numStacks);
		}

		float nx, ny, nz;
		float lastV = -y/m_textureWrapLength;
		// now do the curvey part
		findCircleCenter(slice);
		for(UINT stack = 0; stack < m_curveStacks; ++stack)
		{
			getSplineCoordsAndNormals(x, y, z, nx, ny, nz, stack, slice);
			vertices[count++] = TERRAINVERT(x, y, z, nx, ny, nz, slice/float(m_numSlices), lastV + stack/(float(m_curveStacks)));
		}
	}


	V( m_pVB->Unlock() );

	V( pd3dDevice->CreateIndexBuffer( 
        m_numIndices*sizeof(WORD), 
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
        D3DPOOL_MANAGED, &m_pIB, NULL ) );   

	//// create the index buffer	     

	WORD *pIndices;
    V( m_pIB->Lock( 0, 0, (VOID **)&pIndices, 0 ) );
	setIndices(pIndices, totalStacks);	
    V( m_pIB->Unlock() );

	m_pd3dDevice = pd3dDevice;

    D3DXCreateTextureFromFileA( pd3dDevice, m_strTexName.c_str(), &m_pTexture);		
	maxY += 2.0;	// deal with Z-fighting

	return hr;	
}

HRESULT CPillar::OnLostDevice( void )
{	
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pTexture);

	return S_OK;
}

HRESULT CPillar::OnFrameRender( IDirect3DDevice9 *pd3dDevice )
{
	HRESULT hr;	
	DWORD oldLightMode;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX shiftMatrix;
	D3DXMatrixTranslation(&shiftMatrix, 0, -maxY, 0);
	D3DXMatrixMultiply(&worldMatrix, &m_mWorld, &shiftMatrix);

	// setup
	V( pd3dDevice->SetTexture(0, m_pTexture));
	V( pd3dDevice->GetRenderState(D3DRS_LIGHTING, &oldLightMode) );	
	V( pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE) );
	V( pd3dDevice->SetTransform(D3DTS_WORLD, &worldMatrix) );    
	
	//// draw your ubr pillar
	V( pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(TERRAINVERT)) );
	V( pd3dDevice->SetIndices(m_pIB) );
	V( pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_numVertices, 0, m_numIndices - 2) );

	// cleanup	
	V( pd3dDevice->SetRenderState(D3DRS_LIGHTING, oldLightMode) );

	return S_OK;
}


// -------------------------------------------------------------------------------------------
// Utility functions to modify geometry of the pillar
// Should be removed once pillar's shape has been decided on
// -------------------------------------------------------------------------------------------

void CPillar::incSplineRadius(float delta)
{
	OnLostDevice();	
	m_splineRadius += delta;
	if(m_splineRadius <= 0)
		m_splineRadius = 0.1;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::incSplineAngle(float delta)
{
	OnLostDevice();
	m_splineAngle += delta;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::incSplineStacks(int delta)
{
	OnLostDevice();
	m_curveStacks += delta;
	if(m_curveStacks <= 0)
		m_curveStacks = 1;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::incSplineHeight(float delta)
{
	OnLostDevice();
	m_curveHeight += delta;
	if(m_curveHeight <= 0)
		m_curveHeight = 1;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::incCylStacks(int delta)
{
	OnLostDevice();
	m_numStacks += delta;
	if(m_numStacks <= 0)
		m_numStacks = 1;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::incCylRadius(float delta)
{
	OnLostDevice();
	m_cylinderRadius += delta;
	if(m_cylinderRadius <= 0)
		m_cylinderRadius = 1;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::incCylLength(float delta)
{
	OnLostDevice();
	m_Length += delta;
	if(m_Length <= 0)
		m_Length = 1;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::incSlices(int delta)
{
	OnLostDevice();
	m_numSlices += delta;
	if(m_numSlices <= 2)
		m_numSlices = 3;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::incTexWrap(int delta)
{
	OnLostDevice();
	m_textureWrapLength += delta;
	if(m_textureWrapLength <= 1)
		m_textureWrapLength = 2;
	OnResetDevice(m_pd3dDevice);
}

void CPillar::toggleInteractiveMode(void)
{
	m_bInteractive = !m_bInteractive;
}

bool CPillar::isInteractive(void)
{
	return m_bInteractive;
}

UINT CPillar::HandleInput(UINT nchar, bool bAltDown)
{	
	switch(nchar)
	{
	case 'V':
		m_bChanged = true;
		if(bAltDown)	incCylLength(-100.f);
		else			incCylLength(100.f);
		break;
	case 'B':
		m_bChanged = true;
		if(bAltDown)	incCylRadius(-0.5f);
		else			incCylRadius(0.5f);
		break;
	case 'N':
		m_bChanged = true;
		if(bAltDown)	incTexWrap(-100);
		else			incTexWrap(100);
	case 'F':
		m_bChanged = true;
		if(bAltDown)	incSplineAngle(-1.f/180.f);
		else			incSplineAngle(1.f/180.f);
		break;
	case 'G':
		m_bChanged = true;
		if(bAltDown)	incSplineHeight(-1.f);
		else			incSplineHeight(1.f);
		break;
	case 'H':
		m_bChanged = true;
		if(bAltDown)	incSplineRadius(-1.f);
		else			incSplineRadius(1.f);
		break;
	case 'R':
		m_bChanged = true;
		if(bAltDown)	incCylStacks(-1);
		else			incCylStacks(1);
		break;
	case 'T':
		m_bChanged = true;
		if(bAltDown)	incSlices(-1);
		else			incSlices(1);
		break;
	case 'Y':
		m_bChanged = true;
		if(bAltDown)	incSplineStacks(-1);
		else			incSplineStacks(1);
		break;
	case 'X':
		writeValsToFile();
		break;
	default:
		return nchar;
		break;
	}

	return 0;
}

void CPillar::writeValsToFile(void)
{
	if(m_bChanged)
	{
		char fileName[100];	
		strcpy(fileName, "pillar");
		char tmp[20];
		ltoa(fNum, tmp, 10);
		strcat(fileName, tmp);
		strcat(fileName, ".txt");
		
		FILE *f = fopen(fileName, "w");
		fprintf(f, "\t%s\n\t%.2lf\n\t%.2lf\n\t%.2lf\n\t%.2lf\n\t%d\n\t%d\n\t%d\n\t%d\n\t%d\n", 
			m_strTexName.c_str(), m_cylinderRadius, m_splineRadius, m_Length, m_splineAngle, 
			(int)m_numSlices, (int)m_numStacks, (int)m_curveStacks, (int)m_curveHeight, 
			(int)m_textureWrapLength);
		fclose(f);

		m_bChanged = false;
		fNum ++;		
	}
}
// -------------------------------------------------------------------------------------------
// Private utility functions
// -------------------------------------------------------------------------------------------

D3DXVECTOR4 CPillar::mul(D3DXMATRIX m, D3DXVECTOR4 v)
{
	D3DXVECTOR4 res;

	res.x = m._11*v.x + m._12*v.y + m._13*v.z + m._14*v.w;
	res.y = m._21*v.x + m._22*v.y + m._23*v.z + m._24*v.w;
	res.z = m._31*v.x + m._32*v.y + m._33*v.z + m._34*v.w;
	res.w = m._41*v.x + m._42*v.y + m._43*v.z + m._44*v.w;

	return res;
}

void CPillar::findCircleCenter(UINT slice)
{	
	// angles making up the isosceles triangle for the sector of the spline
	float alpha = m_splineAngle;	// unique angle in triangle
	float beta = (180 - m_splineAngle)/2; // angle that appears twice in triangle
	float h = float(m_curveHeight);
	float x, y, z;

	// distance from first point on spline to last point on spline
	float dist = abs((m_splineRadius*sin(alpha*PI/180.f))/sin(beta*PI/180.f));

	// the point where the slice attaches to the terrain is on a line normal to the cylindrical slice with y = 0
	
	y = -h;
	// distance from edge of cylinder to point on xz plane
	float d2 = sqrt(dist*dist - h*h);
	
	// bunch of geometry which is pretty impossible to understand without diagram
	float tmpAng1 = asin(d2/dist); 
	tmpAng1 *= 180.f/PI;	// convert to degrees
	float tmpAng2 = 90 - tmpAng1;

	float ang1 = 180 - tmpAng2 - beta;
	float ang2 = 90 - ang1;	

	ang1 *= PI/180.f;	//back to radians
	ang2 *= PI/180.f;	//back to radians

	float yDiff = m_splineRadius*sin(ang2);
	float xDiff = m_splineRadius*sin(ang1);	
		
	m_theta = ang2;
	m_circleCenter = D3DXVECTOR3(-xDiff, -yDiff - h, 0);
}

void CPillar::getCircleXZCoord(float &x, float &z, UINT slice)
{
	double angle = slice*2*PI/double(m_numSlices);

	x = float(m_cylinderRadius*cos(angle));
	z = float(m_cylinderRadius*sin(angle));
}

void CPillar::getSplineCoordsAndNormals(float &x, float &y, float &z, float &nx, float &ny, float &nz, UINT stack, UINT slice)
{	
	// build up from the top of the cylinder to the top of the spline
	float angle = stack*(m_splineAngle*PI/180.f)/float(m_curveStacks-1) + m_theta;
	// figure out point and normal as if they were 2D
	D3DXVECTOR4 point = D3DXVECTOR4(m_splineRadius*cos(angle), m_splineRadius*sin(angle), 0, 1);
	
	point.x = m_cylinderRadius - m_circleCenter.x - point.x ;	// flip it horizontally cuz my math was screwy
	point.y = m_circleCenter.y + point.y;				

	D3DXVECTOR4 normal = point - D3DXVECTOR4(m_circleCenter, 0);
	normal.w = 0;

	// rotate point and normal to get them where they really need to be
	D3DXMATRIX rot;	
	D3DXMatrixRotationY(&rot, 2*acos(-1.f)*slice/float(m_numSlices));

	point = mul(rot, point);
	normal = mul(rot, normal);

	float mag = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

	x = point.x;
	y = point.y;
	z = point.z;

	if(y > maxY)
		maxY = y;

	nx = normal.x/mag;
	ny = normal.y/mag;
	nz = normal.z/mag;
}

void CPillar::setIndices(WORD *indices, UINT totalStacks)
{	
	int count = 0;

	for(int slice = 0; slice < int(m_numSlices); slice ++)
	{
		WORD trow = WORD((slice%m_numSlices)*totalStacks);			// first slice
		WORD brow = WORD(((slice+1)%m_numSlices)*totalStacks);		// second slice

		// interlace the indices between the two slices
		for(int stack = 0; stack < int(totalStacks); stack ++)
		{
			indices[count++] = brow++;
			indices[count++] = trow++;
		}

		// add the two extra points to make degenerate triangle
		indices[count++] = trow-1;
		indices[count++] = brow%m_numVertices;
	}

	assert(m_numIndices == count);
}