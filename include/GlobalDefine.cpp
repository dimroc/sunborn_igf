#include "dxstdafx.h"
#include "globaldefine.h"
#include "fxdecl.h"

int gdef::debugprintf(const char *fmt, ...)
{
#if defined(DEBUG) || defined(_DEBUG)
    va_list vargs;

    static char buff[256];
    va_start(vargs,fmt);
    int nc = vsprintf_s(buff,255,fmt,vargs);
    va_end(vargs);

    OutputDebugStringA(buff);

    return nc;
#else
    return 0;
#endif
}

UINT gdef::WCharStringToCharString(LPCWSTR fn, LPSTR pszFilename, int MAXLENGTH) {
    UINT nLength;
    StringCchLength(fn, MAXLENGTH, &nLength);  
    WideCharToMultiByte(CP_ACP, NULL, fn, nLength, pszFilename, MAXLENGTH, NULL, NULL);
    pszFilename[nLength] = 0; // null terminate string
    return nLength;
}

float gdef::lerp(float a, float b, float tween)  
{ 
    return a + tween*(b-a); 
}

VOID gdef::lerp(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pA, const D3DXVECTOR4 *pB, float tween) 
{
    assert(pOut && pA && pB);
    pOut->x = pA->x + tween*(pB->x - pA->x);
    pOut->y = pA->y + tween*(pB->y - pA->y);
    pOut->z = pA->z + tween*(pB->z - pA->z);
    pOut->w = pA->w + tween*(pB->w - pA->w);
}




bool gdef::IsBoxIntersect2D(const D3DXVECTOR2 &c, float clx, float clz, 
                          const D3DXVECTOR2 &o, float olx, float olz)
{
    if( (c.x + clx < o.x - olx) || // other box to the right of c box
        (c.x - clx > o.x + olx) || // to the left
        (c.y + clz < o.y - olz) || // o is above
        (c.y - clz > o.y + olz)) // o is below c
    {
        return false;
    }
    return true;
}

bool gdef::IsBoxIntersect3D(const D3DXVECTOR3 &pos1, const D3DXVECTOR3 &length1, 
                          const D3DXVECTOR3 &pos2, const D3DXVECTOR3 &length2)
{
    if(
        // Is x outside of bounds?
        pos1.x + length1.x < pos2.x - length2.x ||
        pos1.x - length1.x > pos2.x + length2.x ||

        // Is y outside of bounds?
        pos1.y + length1.y < pos2.y - length2.y ||
        pos1.y - length1.y > pos2.y + length2.y ||

        // Is z outside of bounds?
        pos1.z + length1.z < pos2.z - length2.z ||
        pos1.z - length1.z > pos2.z + length2.z
        )
    {
        return false;
    }
    return true;
}


//! Returns whether the first passed cube is entirely inside the second passed cube.
bool gdef::IsCubeInsideCube(const D3DXVECTOR3 &pos1, const D3DXVECTOR3 &length1,
                            const D3DXVECTOR3 &pos2, const D3DXVECTOR3 &length2)
{
    if(
        // Is x inside second cube?
        pos1.x + length1.x < pos2.x + length2.x &&
        pos1.x - length1.x > pos2.x - length2.x &&

        // Is y inside second cube?
        pos1.y + length1.y < pos2.y + length2.y &&
        pos1.y - length1.y > pos2.y - length2.y &&

        // Is z inside second cube?
        pos1.z + length1.z < pos2.z + length2.z &&
        pos1.z - length1.z > pos2.z - length2.z
        )
    {
        return true;
    }
    return false;
}


float gdef::GetAbsoluteManhattanDistance(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2)
{
    return fabs(v1->x - v2->x) + fabs(v1->y - v2->y) + fabs(v1->z - v2->z);
}

float gdef::GetCosAngle3D(const D3DXVECTOR3* origin, const D3DXVECTOR3* p1, const D3DXVECTOR3* p2)
{
    D3DXVECTOR3 v1 = (*p1) - (*origin);
    D3DXVECTOR3 v2 = (*p2) - (*origin);

    D3DXVec3Normalize(&v1, &v1);
    D3DXVec3Normalize(&v2, &v2);

    float dotproduct = D3DXVec3Dot(&v1, &v2);
    return acos(dotproduct);    
}

float gdef::GetCosAngle2D(float ox, float oy,
                          float p1x, float p1y,
                          float p2x, float p2y)
{
    D3DXVECTOR2 v1(p1x - ox, p1y - oy);
    D3DXVECTOR2 v2(p2x - ox, p2y - oy);

    D3DXVec2Normalize(&v1, &v1);
    D3DXVec2Normalize(&v2, &v2);

    float dotproduct = D3DXVec2Dot(&v1, &v2);
    return acos(dotproduct);
}


float gdef::TriangleArea(D3DXVECTOR3 *pa, D3DXVECTOR3 *pb, D3DXVECTOR3 *pc)
{   // TODO: URGENT NOT VERY SIGNED IS IT SCHMUCK GJ DIMITRI. ALWAYS POSITIVE.
    D3DXVECTOR3 ab(pb->x - pa->x, pb->y - pa->y, pb->z - pa->z);    
    D3DXVECTOR3 ac(pc->x - pa->x, pc->y - pa->y, pc->z - pa->z);

    return 0.5f * D3DXVec3Length( D3DXVec3Cross(&ab, &ab, &ac) );
}

VOID gdef::NormalizedNegativeReciprocalY(D3DXVECTOR3 *pvOut, const D3DXVECTOR3 *pvIn)
{
    assert(pvOut && pvIn);
    // rise over run
    if(pvIn->y == 0.f)
    {   // horizontal line so make negative reciprocal go straight up.
        pvOut->x = pvOut->z = 0.f;
        pvOut->y = 1.f;
    }
    else
    {
        float xslope = pvIn->y / pvIn->x;
        float zslope = pvIn->y / pvIn->z;

        pvOut->y = 1.f;
        pvOut->x = 1.f / xslope;
        pvOut->z = 1.f / zslope;
        D3DXVec3Normalize(pvOut, pvOut);
    }
}

float gdef::ForceMovingAlongSpring(const SSpring* pspring, const D3DXVECTOR3* pA, const D3DXVECTOR3* pB, float SPRING_TOLERANCE)
{
    D3DXVECTOR3 vec;
    vec.x = pB->x - pA->x;
    vec.y = pB->y - pA->y;
    vec.z = pB->z - pA->z;

    float fLength = D3DXVec3Length(&vec);    
    // dprintf("length of spring = %f\n", fLength);
    if(fLength > 0)
    {        
        fLength += -pspring->fInertialDist;
        if(fLength < -SPRING_TOLERANCE || fLength > SPRING_TOLERANCE)
            return pspring->fKCoef * fLength;
    }
    return 0;    
}

        // Generate Corner Vectors. 
        //  4 --- 5                3 --- 2 - top
        //  |     |                |     |
        //  0 --- 1  - bottom      7 --- 6
//! PREREQ: corners is an array of 8 D3DXVECTORS
VOID gdef::GenerateAABBPosMiddle(D3DXVECTOR3 corners[8], 
                                 const D3DXVECTOR3 &center, 
                                 const D3DXVECTOR3 &displace)
{   // man typing it out probably would have been better.
    static int bottom[4] = { 0, 1, 4, 5};
    static int top[4] = { 7, 6, 3, 2 };
    static int left[4] = { 0, 4, 3, 7 };
    static int right[4] = {5, 1, 2, 6};
    static int back[4] = { 4, 5, 3, 2 };
    static int front[4] = { 0, 1, 7, 6 };

    float bottomy = center.y - displace.y;
    float topy = center.y + displace.y;
    float leftx = center.x - displace.x;
    float rightx = center.x + displace.x;
    float backz = center.z + displace.z;
    float frontz = center.z - displace.z;

    for(int i=0;i<4;i++) {        
        corners[bottom[i]].y = bottomy;
        corners[top[i]].y = topy;
        corners[left[i]].x = leftx;
        corners[right[i]].x = rightx;
        corners[back[i]].z = backz;
        corners[front[i]].z = frontz;
    }
}

VOID gdef::MatrixTranslation(D3DXMATRIX *pmtx, const float x, const float y, const float z)
{   // Row Major matrix translation assignment
    assert(pmtx);
    pmtx->_41 = x;
    pmtx->_42 = y;
    pmtx->_43 = z;
}

gdef::Matrix3x3::Matrix3x3() 
{   // memset(this, 0, sizeof(Matrix3x3)); 
}

gdef::Matrix3x3::Matrix3x3(float _11, float _12, float _13, float _21, float _22, float _23, float _31, float _32, float _33)
{
	vals[0] = _11;
	vals[1] = _12;
	vals[2] = _13;
	vals[3] = _21;
	vals[4] = _22;
	vals[5] = _23;
	vals[6] = _31;
	vals[7] = _32;
	vals[8] = _33;		
}

gdef::Matrix3x3& gdef::Matrix3x3::operator= (const Matrix3x3& m)
{
	for(int i = 0; i < 9; i ++)
	{
		vals[i] = m.vals[i];
	}
	return *this;
}

//***********************************************************************************************
// Taken from Microsoft
//***********************************************************************************************
void gdef::ExtractPlanes( D3DXPLANE * p_planes, const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatProj) {

    D3DXMATRIX mat;
    D3DXMatrixMultiply( &mat, pMatView, pMatProj );
    D3DXMatrixInverse( &mat, NULL, &mat );
    D3DXVECTOR3 vecFrustum[8];

    vecFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // xyz
    vecFrustum[1] = D3DXVECTOR3( 1.0f, -1.0f, 0.0f); // Xyz
    vecFrustum[2] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f); // xYz
    vecFrustum[3] = D3DXVECTOR3( 1.0f, 1.0f, 0.0f); // XYz
    vecFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f); // xyZ
    vecFrustum[5] = D3DXVECTOR3( 1.0f, -1.0f, 1.0f); // XyZ
    vecFrustum[6] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f); // xYZ
    vecFrustum[7] = D3DXVECTOR3( 1.0f, 1.0f, 1.0f); // XYZ

    for( INT i = 0; i < 8; i++ )
    D3DXVec3TransformCoord( &vecFrustum[i], &vecFrustum[i], &mat );

    D3DXPlaneFromPoints( &p_planes[0], &vecFrustum[0],&vecFrustum[1],&vecFrustum[2] ); // Near
    D3DXPlaneFromPoints( &p_planes[1], &vecFrustum[6],&vecFrustum[7],&vecFrustum[5] ); // Far
    D3DXPlaneFromPoints( &p_planes[2], &vecFrustum[2],&vecFrustum[6],&vecFrustum[4] ); // Left
    D3DXPlaneFromPoints( &p_planes[3], &vecFrustum[7],&vecFrustum[3],&vecFrustum[5] ); // Right
    D3DXPlaneFromPoints( &p_planes[4], &vecFrustum[2],&vecFrustum[3],&vecFrustum[6] ); // Top
    D3DXPlaneFromPoints( &p_planes[5], &vecFrustum[1],&vecFrustum[0],&vecFrustum[4] ); // Bottom
}

//! PREREQ: 6 planes and 6 slots in CornerPairIndex *pOut.
VOID gdef::FindAxisAlignedCorners(D3DXPLANE * p_planes, gdef::CornerIndexPair *pOutAlignedCornerPair) 
{
    using gdef::CornerIndexPair;
    // Generate Corner Vectors. 
    //  4 --- 5                3 --- 2 - top
    //  |     |                |     |
    //  0 --- 1  - bottom      7 --- 6

    // Get 0 2, 1 3, 4 6, 5 7 diagonals
    static D3DXVECTOR3 v20(1.f, 1.f, 1.f);
    static D3DXVECTOR3 v31(-1.f, 1.f, 1.f);
    static D3DXVECTOR3 v64(1.f, 1.f, -1.f);
    static D3DXVECTOR3 v75(-1.f, 1.f, -1.f);

    static D3DXVECTOR3 v[4] = { *D3DXVec3Normalize(&v20, &v20), // 2 - 0
                                *D3DXVec3Normalize(&v31, &v31), // 3 - 1
                                *D3DXVec3Normalize(&v64, &v64), // 6 - 4
                                *D3DXVec3Normalize(&v75, &v75)  // 7 - 5
                               };
    

    static CornerIndexPair SetCornerPairs[4] = { CornerIndexPair(2,0),
                                                 CornerIndexPair(3,1),
                                                 CornerIndexPair(6,4),
                                                 CornerIndexPair(7,5)
                                               };
#if 0
    v[0] = corners[arrSetCornerPairs[0].i1] - m_root->corners[arrSetCornerPairs[0].i2];
    v[1] = corners[arrSetCornerPairs[1].i1] - m_root->corners[arrSetCornerPairs[1].i2];
    v[2] = corners[arrSetCornerPairs[2].i1] - m_root->corners[arrSetCornerPairs[2].i2];
    v[3] = corners[arrSetCornerPairs[3].i1] - m_root->corners[arrSetCornerPairs[3].i2];
#endif

    // test the diagonals to plane normals. no need to normalize since same magnitude.
    float dot[4];
    for(int i=0; i<6; i++)  // Check all 6 planes for most aligned corner
    {
        UINT maxIndex = 0;
        for(int j=0; j<4; j++)  // get dot products of all corners (4) against this plane.
        {
            dot[j] = fabs(D3DXPlaneDotNormal(&p_planes[i], &v[j]));
            if(dot[j] > dot[maxIndex])
                maxIndex = j;
        }
        // assign this corner/plane pair for testing.
        pOutAlignedCornerPair[i] = SetCornerPairs[maxIndex];
    }
}

//! Only tests the 2 corners most aligned with the planes Normal, therefore representing whole square.
UCHAR gdef::AABBFrustumTest(D3DXPLANE pFrustumPlanes[6], CornerIndexPair arrCIP[6], const D3DXVECTOR3* corners) 
{
    UCHAR r = AABB_INSIDE;
    float dot1, dot2;
    for(int i=0; i < 6; i++) { // 6 planes in view frustum(duh)
        // Test the 2 optimal corners against plane
        dot1 = D3DXPlaneDotCoord(&pFrustumPlanes[i], &corners[arrCIP[i].i1]);
        dot2 = D3DXPlaneDotCoord(&pFrustumPlanes[i], &corners[arrCIP[i].i2]);
        
        if(dot1 < 0 && dot2 < 0) // positive is inside!
            return AABB_OUTSIDE;
        else if(dot1 < 0 || dot2 < 0)
            r = AABB_SPANNING;
    }
    return r;
}

VOID gdef::GenerateBoundingBox(BoxVertColor* verts, const D3DXVECTOR3* corners)
{
    // bottom face
    verts[0].Pos = corners[0];
    verts[1].Pos = corners[1];

    verts[2].Pos = corners[1];
    verts[3].Pos = corners[5];

    verts[4].Pos = corners[5];
    verts[5].Pos = corners[4];

    verts[6].Pos = corners[4];
    verts[7].Pos = corners[0];

    // top face
    verts[8].Pos = corners[7];
    verts[9].Pos = corners[6];

    verts[10].Pos = corners[6];
    verts[11].Pos = corners[2];

    verts[12].Pos = corners[2];
    verts[13].Pos = corners[3];

    verts[14].Pos = corners[3];
    verts[15].Pos = corners[7];

    // vertical lines
    verts[16].Pos = corners[1];
    verts[17].Pos = corners[6];

    verts[18].Pos = corners[5];
    verts[19].Pos = corners[2];

    verts[20].Pos = corners[4];
    verts[21].Pos = corners[3];

    verts[22].Pos = corners[0];
    verts[23].Pos = corners[7];
}
