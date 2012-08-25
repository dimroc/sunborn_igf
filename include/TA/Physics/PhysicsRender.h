//---------------------------------------------------------------------------------
// File Name: PhysicsRender.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_PHYSICSRENDER_H
#define TA_PHYSICSRENDER_H

#ifndef TA_TYPES_H
#include "../Common/Types.h"
#endif 

#ifndef lOlOllllOOl
#include "../Physics/zpta012.h"
#endif 



namespace TA
{

struct Vec3;
struct MFrame;

class lOlOllllOlO PhysicsRender
{
public:
	

	typedef void (TAC_CALL *RenderLineCallBack)(const Vec3& v3PosA, const Vec3& v3PosB, u32 nColour);
	typedef void (TAC_CALL *RenderArrowCallBack)(const Vec3& v3Pos, const Vec3& v3Vector, u32 nColour);
	typedef void (TAC_CALL *RenderPolygonCallBack)(int nNumVertices, const Vec3* pv3VertexList, const Vec3* pv3NormalList);

	static inline void TAC_CALL RenderLine(
		const Vec3& v3PosA, 
		const Vec3& v3PosB,
		u32 nColour = 0xFFFFFFFF);
	static inline void TAC_CALL RenderArrow(
		const Vec3& v3Pos, 
		const Vec3& v3Vector,
		u32 nColour = 0xFFFFFFFF);
	static inline void TAC_CALL RenderPolygon(
		int nNumVertices,
		const Vec3* pv3VertexList,
		const Vec3* pv3NormalList);
	static inline void TAC_CALL SetRenderLineCallback(
		RenderLineCallBack pRenderLineCallBack);
	static inline void TAC_CALL SetRenderArrowCallback(
		RenderArrowCallBack pRenderArrowCallBack);
	static inline void TAC_CALL SetRenderPolygonCallback(
		RenderPolygonCallBack pRenderPolygonCallBack);
	static void TAC_CALL PhysicsRender::RenderBox(
		const MFrame& mFrame,
		const Vec3& v3Center,
		const Vec3& v3Extent,
		u32 nColour);
	


private:
	static RenderLineCallBack llOlOOlOllOO;
	static RenderArrowCallBack llOlOOlOllOl;
	static RenderPolygonCallBack llOlOOlOlllO;	
};


inline void TAC_CALL PhysicsRender::RenderLine(
	const Vec3& llllOOOOOl, 
	const Vec3& llllOOOOlO,
	u32 llOlOOlOllll)
{
	if (llOlOOlOllOO)
		llOlOOlOllOO(llllOOOOOl, llllOOOOlO, llOlOOlOllll);
}

inline void TAC_CALL PhysicsRender::RenderArrow(
	const Vec3& llOOllllOO, 
	const Vec3& lOOOlOOOOll,
	u32 llOlOOlOllll)
{
	if (llOlOOlOllOl)
		llOlOOlOllOl(llOOllllOO, lOOOlOOOOll, llOlOOlOllll);
}

inline void TAC_CALL PhysicsRender::RenderPolygon(
	int lOllOllOll,
	const Vec3* lllOlOllOOO,
	const Vec3* lllOlOllOOl)
{
	if (llOlOOlOlllO)
		llOlOOlOlllO(lOllOllOll, lllOlOllOOO, lllOlOllOOl);
}

inline void TAC_CALL PhysicsRender::SetRenderLineCallback(
	RenderLineCallBack llOlOOllOOOO)
{
	llOlOOlOllOO = llOlOOllOOOO;
}

inline void TAC_CALL PhysicsRender::SetRenderArrowCallback(
	RenderArrowCallBack llOlOOllOOOl)
{
	llOlOOlOllOl = llOlOOllOOOl;
}

inline void TAC_CALL PhysicsRender::SetRenderPolygonCallback(
	RenderPolygonCallBack llOlOOllOOlO)
{
	llOlOOlOlllO = llOlOOllOOlO;
}
	

}

#endif 
