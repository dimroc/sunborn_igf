#pragma once

class CModelInstance;

//
//! SHARED BETWEEN CPHYSICSROOT AND CWORLD.
//

//! Holds entity information such as the geometry ID (for batching), Mesh, and world matrix 
//! used for rendering StaticEntities and Player Characters.
struct SMeshEntity {
    int geomID;    
    bool bVisible, bRequestVisible;
    D3DXMATRIX mWorld;        
    CModelInstance* pModelInst;

    LPDIRECT3DTEXTURE9 pTex;    // texture for this entity. used for diff character textures.
    int texID;

    //! For AABB
    D3DXVECTOR3 corners[8];
    D3DXVECTOR3 displace;

    SMeshEntity() : pTex(NULL) {}
};
typedef stdext::hash_map<UINT, SMeshEntity> EntityHashMap;

struct SMeshEntityPtrFunctor {  //! Functor to sort m_vVisibleEntites ( which have SMeshEntity* ).
    bool operator() ( const SMeshEntity* lhs, const SMeshEntity* rhs ) {
        return lhs->geomID < rhs->geomID;
    }
};