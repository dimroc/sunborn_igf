//---------------------------------------------------------------------------------
// File Name: ConvexHull.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_CONVEXHULL_H
#define TA_CONVEXHULL_H

#ifndef lOOllOllll
#include "../Common/zcta001.h"
#endif 

#ifndef TA_VECTOR_H
#include "Vector.h"
#endif 



namespace TA
{

struct Vec3;

class TACOMMON_CLASS ConvexHull
{
public:
	
	struct Polygon
	{
		
		int GetNumVertices() const;
		int GetVertexPositionIndex(int nVertexIndex) const;
		int GetVertexAdjacentPolygonIndex(int nVertexIndex) const;
		

		struct Vertex
		{
			int llOlllOllO;
			int llOlllOlll;
		};
		int lOllOllOll;
		Vertex llOllllOOO[16]; 
	};

	

	ConvexHull();
	~ConvexHull();

	bool Initialise(const Vec3* v3PointList, int nNumPoints, int nMaxNumVerticesInPolygon);
	bool Initialise(	
		const Vec3* pv3PlaneNormalList, 
		const Vec3* pv3PlanePointList, 
		int nNumPlanes);

	void Finalise();

	int GetNumPoints() const;
	const Vec3& GetPoint(int nIndex) const;

	int GetNumPolygons() const;
	const Polygon& GetPolygon(int nIndex) const;

	
	
private:
	struct lOlllllOll;
	class lOlOlOllOO;
	
	bool llOOOOllOl;
	bool llOOlOOOlO;
	float llOllllOOl;
	lOOOOlOllO<int> llOOlOlOOO;
	lOOOOlOllO<int> llOOlOlOOl;
	lOOOOlOllO<Vec3, false> llOOlOlOlO;
	
	
	

	bool Initialise(lOlOlOllOO& llOOlOOllO);
	bool llOOlOOOOl(const Vec3* llOOOllOll, int llOOOlllOO, int llOOOlllOl, float llOOOllllO, float llOOOlllll);
};

inline int ConvexHull::Polygon::GetNumVertices() const { return lOllOllOll; }
inline int ConvexHull::Polygon::GetVertexPositionIndex(int llOllllOlO) const { return llOllllOOO[llOllllOlO].llOlllOllO; }
inline int ConvexHull::Polygon::GetVertexAdjacentPolygonIndex(int llOllllOlO) const { return llOllllOOO[llOllllOlO].llOlllOlll; }
inline int ConvexHull::GetNumPoints() const { return llOOlOlOlO.lOOOlllOOl(); }
inline const Vec3& ConvexHull::GetPoint(int lOOOlOOlll) const { return llOOlOlOlO[lOOOlOOlll]; }
inline int ConvexHull::GetNumPolygons() const { return llOOlOlOOl.lOOOlllOOl(); }
inline const ConvexHull::Polygon& ConvexHull::GetPolygon(int lOOOlOOlll) const { return *(Polygon*)&llOOlOlOOO[llOOlOlOOl[lOOOlOOlll]]; }

}
#endif 
