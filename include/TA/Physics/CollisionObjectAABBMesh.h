//---------------------------------------------------------------------------------
// File Name: CollisionObjectAABBMesh.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISIONOBJECTAABBMESH_H
#define TA_COLLISIONOBJECTAABBMESH_H

#ifndef llOOlOlllll
#include "../Physics/zpta001.h"
#endif 

#ifndef TA_COLLISIONOBJECTCOMPLEX_H
#include "CollisionObjectComplex.h"
#endif 



namespace TA
{

class CollisionObjectConvex;
class Collision;
struct MFrame;
struct AABB;

class lOlOllllOlO CollisionObjectAABBMesh : public CollisionObjectComplex
{
public:
	struct Polygon 
	{
		float llOlllOlOOl;
		float llOlllOlOlO;
		float llOlllOlOll;
		u32 llOlllOllOO;
		int llOlllOOllO;
		int lOllOllOll;	
		int llOllllOOO[lOlllOOlOOl];
	};

	struct Data;
	friend struct Data;

	//pl.BeginExportClass(CollisionObjectAABBMesh : public CollisionObjectComplex)
	//pl.InheritFrom(CollisionObjectComplex)

	
	void Initialise(int nNumVertices, int nNumPolygons, int nNumPolygonIndices);
	void Finalise();
	
	
	
#ifndef TA_WRAPPER
	void Serialise(Serialiser& lOOOlOllOl, const Version& lllOlOOOlO);
#endif 
	

	
	virtual bool Serialisable() const { return true; }

	void Render();

	void AddVertex(const Vec3& v3Pos);
	void AddPolygon(int nNumVertices, const int* pnIndexList, u32 nAttribute = 0x0);
	void FinishedAddingGeometry();

#ifndef TA_WRAPPER
#ifndef TA_DOXYGEN
	const Data* GetData() const;
#endif 
#endif 

	bool TestLineForCollision(
		const Vec3& v3Start,
		const Vec3& v3Normal,
		float fLength,
		Collision& collision) const;
#ifndef TA_WRAPPER
	void CachedLocalPolygons(
		CollisionObjectCachedPolyData& cachedPolyData,
		const AABB& aabb,
		void* pDynamicObjectUserData) const;
#endif 

	virtual const AABB& GetAABB() const;

	static CollisionObjectAABBMesh* TAC_CALL CreateNew();
	//pl.Release()

	//pl.EndExportClass(DynamicObject)

protected:
	CollisionObjectAABBMesh();
	~CollisionObjectAABBMesh();
	

private:
	virtual const Char* llOllOOlOOO() const { return "TAColObjAABBMesh"; }

	Data* lOlOOlOllO;

	void llOlllOllOl(Polygon* llOOOlOOlO);
};

inline const CollisionObjectAABBMesh::Data* CollisionObjectAABBMesh::GetData() const { return lOlOOlOllO; }

}

#endif 
