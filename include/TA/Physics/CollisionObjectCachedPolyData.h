//---------------------------------------------------------------------------------
// File Name: CollisionObjectCachedPolyData.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_COLLISIONOBJECTCACHEDPOLYDATA_H
#define TA_COLLISIONOBJECTCACHEDPOLYDATA_H

#ifndef lOOllOllll
#include "../Common/zcta001.h"
#endif 

#ifndef lOOllOlllO
#include "../Common/zcta000.h" 
#endif 

#ifndef llOOlOlllll
#include "../Physics/zpta001.h"
#endif 

#ifndef TA_COLLISION_H
#include "Collision.h"
#endif 

#ifndef TA_COLLISIONOBJECT_H
#include "CollisionObject.h"
#endif 



namespace TA
{

struct AABB;
class CollisionObjectSimple;

class CollisionObjectCachedPolyData : public CollisionObject
{
public:
	
	struct Vertex
	{
		const Vec3& GetPosition();
		void SetPosition(const Vec3& v3Position);
		const Vec3& GetEdgeNormal();
		void SetEdgeNormal(const Vec3& v3EdgeNormal);
		
#ifndef lOllOllOOOO
	private:
#endif 
		Vec3 lOOOOlllll;
		Vec3 lOlllOlOlOl;
	};
	
	struct Polygon 
	{
		const Vec3& GetNormal();
		void SetNormal(const Vec3& v3Normal);
		int GetAttribute() const;
		void SetAttribute(int nAttribute);
		int GetNumVertices() const;
		void SetCollisionID(u32 nCollisionID);
		u32 GetCollisionID() const;
		
		Vertex& GetVertex(int nIndex);
		const Vertex& GetVertex(int nIndex) const;
		void GenerateEdgeNormals();
		
#ifndef lOllOllOOOO
	private:
#endif 
		Vec3 lOOOlOOOOO;
		int llOlllOOllO;
		int lOllOllOll;
		u32 llOlllOllOO;
		u32 lllOlOOOllO;
		Vertex llOllllOOO[lOlllOOlOOl];
	};

	void Initialise();
	void Finalise();

	
	void Clear();
	Polygon* GetNewPolygon(int nNumVertices);
	
	

#ifndef DOXYGEN
	const u8* GetData() const { return &lOOOllOOOlO[0]; }
	const u8* lllOlOOOlll(int lOOOlOOlll) const { return &lOOOllOOOlO[lOOOlOOlll]; }
	int lllOlOOlOOO() const { return lOOOllOOOlO.lOOOlllOOl(); }
	static int TAC_CALL lllOOlOllll(int lOllOllOll) { return sizeof (Vec3) + sizeof (int) * 4 + sizeof (Vertex) * lOllOllOll; }
#endif 

#ifndef DOXYGEN
	virtual bool TestLineForCollision(
		const Vec3& lOOlOlOlll,
		const Vec3& lOOOlOOOOO,
		float lOOlOllOOO,
		Collision& llOllllOlOl) const { return false; }

	CollisionObject* lllOlOOlOOl() { return lllOOlOllOl; }
	void lllOlOOlOlO(CollisionObject* lllOlOOlOll) { lllOOlOllOl = lllOlOOlOll; }

	lOlllOOOlOO(lllOlOOllOO)		

#endif 

	static CollisionObjectCachedPolyData* TAC_CALL CreateNew();

protected:
	CollisionObjectCachedPolyData();
	~CollisionObjectCachedPolyData();	

private:
	lOOOOlOllO<u8, false> lOOOllOOOlO;

	virtual const AABB& GetAABB() const { return NoGetAABB(); }

	static void TAC_CALL lllOlOOllOl(lOOOOllOll::lOOllOOOOO* lOOlllOOOl, void* lOOlllOOOO);
	CollisionObject* lllOOlOllOl;
};

inline const Vec3& CollisionObjectCachedPolyData::Vertex::GetPosition() { return lOOOOlllll; }
inline void CollisionObjectCachedPolyData::Vertex::SetPosition(const Vec3& lOOOOlllll) { this->lOOOOlllll = lOOOOlllll; }
inline const Vec3& CollisionObjectCachedPolyData::Vertex::GetEdgeNormal() { return lOlllOlOlOl; }
inline void CollisionObjectCachedPolyData::Vertex::SetEdgeNormal(const Vec3& lOlllOlOlOl) { this->lOlllOlOlOl = lOlllOlOlOl; }
inline const Vec3& CollisionObjectCachedPolyData::Polygon::GetNormal() { return lOOOlOOOOO; }
inline void CollisionObjectCachedPolyData::Polygon::SetNormal(const Vec3& lOOOlOOOOO) { this->lOOOlOOOOO = lOOOlOOOOO; }
inline int CollisionObjectCachedPolyData::Polygon::GetAttribute() const { return llOlllOOllO; }
inline void CollisionObjectCachedPolyData::Polygon::SetCollisionID(u32 llOlllOllOO) { this->llOlllOllOO = llOlllOllOO; }
inline u32 CollisionObjectCachedPolyData::Polygon::GetCollisionID() const { return llOlllOllOO; }
inline void CollisionObjectCachedPolyData::Polygon::SetAttribute(int llOlllOOllO) { this->llOlllOOllO = llOlllOOllO; }
inline int CollisionObjectCachedPolyData::Polygon::GetNumVertices() const { return lOllOllOll; }
inline CollisionObjectCachedPolyData::Vertex& CollisionObjectCachedPolyData::Polygon::GetVertex(int lOOOlOOlll) { TA_ASSERT(lOOOlOOlll >= 0); TA_ASSERT(lOOOlOOlll < lOllOllOll); return llOllllOOO[lOOOlOOlll]; }
inline const CollisionObjectCachedPolyData::Vertex& CollisionObjectCachedPolyData::Polygon::GetVertex(int lOOOlOOlll) const { return llOllllOOO[lOOOlOOlll]; }
inline void CollisionObjectCachedPolyData::Clear() { lOOOllOOOlO.Clear(); }

#ifndef DOXYGEN

void TAC_CALL lllOOllOOOO();

void TAC_CALL lllOOllOOOl(lOllOllOllO* lOlllOOOOOl);

void TAC_CALL lllOOlllOOO(
	float lllOllllOO,
	float lOllOOlOOOl,
	const CollisionObjectCachedPolyData::Polygon* llOOOlOOlO,
	int lOlllOOlll,
	bool lllOOlllOOl,
	const Vec3& lllOOOllOlO,
	const Vec3& lllOOlllOlO,
	const Vec3& lOOOlOOOOO);

void TAC_CALL lllOOlllllO(
	const Vec3& lllOlOOOOll,
	const Vec3& lllOOOlllOl);

enum 
{ 
	lllOOllllll = 0,
	lllOlOOOOlO,
};

void TAC_CALL lllOOlllllO(
	const Vec3& lllOlOOOOll,
	int lOllOlOlOOO);

void TAC_CALL lllOlOOOlOO(
	float lllOllllOO,
	float lOllOOlOOOl,
	u32 llOlllOOllO,
	const Vec3& lOllllOOlOO,
	const Vec3& lOlllOllOOO,
	const Vec3& lllOOlllOlO,
	const Vec3& lOOOlOOOOO,
	int lOllOlOlOOO);

void TAC_CALL lllOlOOOlOl(
	const Vec3& lOllllOOlOO,
	const Vec3& lOlllOllOOO,
	int lOllOlOlOOO);

#endif 

};


#endif 
