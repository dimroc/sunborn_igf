//---------------------------------------------------------------------------------
// File Name: DynamicObjectCollisionIterator.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_DYNAMICOBJECTCOLLISIONITERATOR_H
#define TA_DYNAMICOBJECTCOLLISIONITERATOR_H

#ifndef lOlOllOlllOl
#include "../Physics/zpta009.h"
#endif 



namespace TA
{

class DynamicObject;

class DynamicObjectCollisionIterator
{
public:
	
	DynamicObjectCollisionIterator();
	~DynamicObjectCollisionIterator();
	DynamicObjectCollisionIterator(const DynamicObjectCollisionIterator& that);
	void Finalise() ;
	void operator ++();
	DynamicObjectCollisionIterator& operator = (const DynamicObjectCollisionIterator& that);
	bool operator == (const DynamicObjectCollisionIterator& that) const ;
	Collision& operator *();
	bool AtEnd();
	

private:

	friend class DynamicObject;

	DynamicObject* lOlOllOllllO;
	lOlOOOOOlOOO* lOlOllOlllll;
	lOlOOOOOOlOl::CollisionIterator lOlOlllOOOOO;
	
	DynamicObjectCollisionIterator(DynamicObject* lOlllOOOllO, lOlOOOOOlOOO* lOlOlllOOOOl)
	{
		lOlOllOllllO = lOlllOOOllO;
		lOlOllOlllll = lOlOlllOOOOl;
		lOlOlllOOOlO();		
	}

	void lOlOlllOOOlO()
	{
		for (; lOlOllOlllll; lOlOllOlllll = lOlOllOlllll->llOOOOlllll)
		{
			if (!lOlOllOlllll->lOlOOOOOlOlO->lOlOlllOOOll.lOOOOOlOOlO())
			{
				lOlOlllOOOOO = lOlOllOlllll->lOlOOOOOlOlO->lOlOlllOOOll.lOlOOOOlOO();
				break;
			}
		}
	}
};

inline DynamicObjectCollisionIterator::DynamicObjectCollisionIterator() 
{
	lOlOllOllllO = 0;
	lOlOllOlllll = 0;
}
inline DynamicObjectCollisionIterator::~DynamicObjectCollisionIterator() 
{
}
inline DynamicObjectCollisionIterator::DynamicObjectCollisionIterator(const DynamicObjectCollisionIterator& lOOOllllllO) 
{
	lOlOllOllllO = lOOOllllllO.lOlOllOllllO;
	lOlOllOlllll = lOOOllllllO.lOlOllOlllll;
	lOlOlllOOOOO = lOOOllllllO.lOlOlllOOOOO;
}
inline void DynamicObjectCollisionIterator::Finalise() 
{
	lOlOllOllllO = 0;
	lOlOllOlllll = 0;
	lOlOlllOOOOO.Finalise();
}
inline void DynamicObjectCollisionIterator::operator ++() 
{
	if (lOlOllOlllll)
	{
		++lOlOlllOOOOO;
		if (lOlOlllOOOOO.AtEnd())
		{
			lOlOllOlllll = lOlOllOlllll->llOOOOlllll;
			lOlOlllOOOlO();		
		}
	}
}
inline DynamicObjectCollisionIterator& DynamicObjectCollisionIterator::operator = (const DynamicObjectCollisionIterator& lOOOllllllO) 
{
	lOlOllOllllO = lOOOllllllO.lOlOllOllllO;
	lOlOllOlllll = lOOOllllllO.lOlOllOlllll;
	lOlOlllOOOOO = lOOOllllllO.lOlOlllOOOOO;
	return *this;
}
inline bool DynamicObjectCollisionIterator::operator == (const DynamicObjectCollisionIterator& lOOOllllllO) const 
{
	return
		(lOlOllOllllO == lOOOllllllO.lOlOllOllllO &&
		lOlOllOlllll == lOOOllllllO.lOlOllOlllll &&
		lOlOlllOOOOO == lOOOllllllO.lOlOlllOOOOO);
}
inline Collision& DynamicObjectCollisionIterator::operator *() 
{ 
	return *lOlOlllOOOOO; 
}
inline bool DynamicObjectCollisionIterator::AtEnd() 
{ 
	return lOlOllOlllll == 0; 
}

}

#endif 
