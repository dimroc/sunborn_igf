#pragma once

#include "TA/Physics/Physics.h"
#include "TA/Physics/CollisionObjectAABBMesh.h"
#include "MeshEntity.h"

using std::vector;
using std::list;
using std::set;
using stdext::hash_map;

using TA::Vec3;

class CWorld;
class CQuadTree;
class CGameSettings;
class TA::CollisionObjectAABBMesh;

//! Handles the physics of the passed in CWorld*.
class CPhysicsRoot
{
private:
    float                                       m_fTimeStep; //! Constant time step for engine.
    float                                       m_fTimeRemainder; //! Reminaing time since last iteration if less tha time step.

    typedef hash_map<int, TA::DynamicObject*>   DynamicObjectsHashMap;
    DynamicObjectsHashMap                       m_hmDynamicObjects;

	vector<TA::StaticObject*>					m_vpStaticObjects;

    //! Physics Dymanic Object World Settings
    float m_fGravMult, m_fMass, m_fFriction, m_fBounce;
	int											m_nPlayerGroupID, m_nSLEGroupID; //! Group IDs 
private:
    //! Assigns the physics variables defined by CGameSettings*
    VOID AssignSettings(CGameSettings* pSettings);

    //! Stuffs the passed vectors with world space vertices/indices pertaining to an island (heightmap/basin/pillar).
    VOID GatherStaticIsland(vector<D3DXVECTOR3> &vVerts, vector<int> &vIndices, const CQuadTree* pIsland);

    //! Gathers the valid triangle vertices from the passed in mesh, transforms them to world space
    //! and stuffs them into the passed vectors.
    VOID GatherVerticesAndIndices(           
                                vector<D3DXVECTOR3> &vVerts,
                                vector<int> &vIndices,
                                ID3DXMesh* pMesh, 
                                const D3DXMATRIX* pWorld );


    //! Stuffs the passed in vertices and indices into a staticCollisionObject and returns pointer.
    TA::CollisionObjectAABBMesh* CreateStaticCollisionObject( vector<D3DXVECTOR3> &vVerts,
                                                              vector<int> &vIndices );	

	TA::Vec3 Vec3FromD3DXVector(const D3DXVECTOR3* vector) { return TA::Vec3(vector->x, vector->y, vector->z); }
public:
    CPhysicsRoot(CWorld* pWorld, CGameSettings* pSettings);   //! Creates static objects based on the passed in CWorld.
    ~CPhysicsRoot();    //! Stops simulation and cleans up.

    VOID OnFrameMove(float fElapsedTime);
    VOID OnFrameRender(const D3DXMATRIX* pProj, const D3DXMATRIX* pView);   //! Renders the meshes in the physics simulation. Used for debugging purposes.

    //! returns the fixed time step for physics engine. 
    //! Used to stop the jittering present when updating at different rates ( between gfx and physics ).
    float GetPhysicsTimeStep()                              { return m_fTimeStep; } 
    

	// -------------------------
	//  DynamicObject Functions
	// -------------------------
    int AddDynamicCylinder(const D3DXVECTOR3* pvPosition, 
                           const D3DXVECTOR3* pvTopCenter, 
                           const D3DXVECTOR3* pvBottomCenter, 
                           float fRadius); //! Adds a dynamic cylinder to the physics simulation. returns unique id.

    int AddDynamicCapsuleCombo(const D3DXVECTOR3* pvPosition, 
                           const D3DXVECTOR3* pvTopCenter, 
                           const D3DXVECTOR3* pvBottomCenter, 
                           float fRadius, float fGroundOffset); //! Adds a custom player shaped dynamic object to physics simulation. returns unique id.
	int AddDynamicSphere(const D3DXVECTOR3* pvPosition, 
		float fRadius, float fMass);//! Adds a dynamic sphere to the physics simulation. returns unique id.
    void DeleteDynamicObject(int physicsID);

    D3DXVECTOR3 GetPosition(int ID) 
    { 
        return D3DXVECTOR3(m_hmDynamicObjects[ID]->GetPosition().x, 
                           m_hmDynamicObjects[ID]->GetPosition().y,
                           m_hmDynamicObjects[ID]->GetPosition().z);
    }
	D3DXVECTOR3 GetLinearVelocity(int ID)
	{	
		return D3DXVECTOR3(m_hmDynamicObjects[ID]->GetLinearVelocity().x,
                           m_hmDynamicObjects[ID]->GetLinearVelocity().y,
                           m_hmDynamicObjects[ID]->GetLinearVelocity().z);
	}
	
	bool DynamicObjectExists(int ID) { 
		if (m_hmDynamicObjects[ID]) 
			return true; 
		return false;
	}
    // ------------------------------------------------------------------------
    // Set Variables to Physics entities
    // ------------------------------------------------------------------------
    VOID SetLinearVelocity(int physicsID, const D3DXVECTOR3* pvVelocity);
    VOID SetPosition(int physicsID, const D3DXVECTOR3* pvPosition){
		m_hmDynamicObjects[physicsID]->SetPosition(Vec3FromD3DXVector(pvPosition));
	}


	bool SetToDesiredVelocity(int physicsID, D3DXVECTOR3* pvVelocity, float fGroundOffset, bool bGrounded, float fDt);

	bool UpdatePlayerMovement(int physicsID, D3DXVECTOR3* pvVelocity, 
				float fGroundOffset, bool bJumping, bool bUncontrollable, float fDt); // Returns true if player is still airborne after this frame
	VOID UpdateSLE(int physicsID,  // input
					int ownerID,  // input
					set<int> &sVictims); // output - set of all the physicsIDs that this SLE collided with
	VOID ApplyLinearImpulse(int physicsID, const D3DXVECTOR3* pvImpulse);
	VOID SetFriction(int physicsID, float fFriction) 
	{
		m_hmDynamicObjects[physicsID]->SetFriction(fFriction); 
	}
	VOID SetGravityMult(int physicsID, float fGrav) 
	{
		m_hmDynamicObjects[physicsID]->SetGravityMult(fGrav); 
	}
	VOID SetGhost(int physicsID, bool bGhost) 
	{
		m_hmDynamicObjects[physicsID]->SetGhost(bGhost); 
	}

	VOID AddToSLEGroup(int physicsID) { m_hmDynamicObjects[physicsID]->SetUserGroup(m_nSLEGroupID); }
	VOID AddToPlayerGroup(int physicsID) { m_hmDynamicObjects[physicsID]->SetUserGroup(m_nPlayerGroupID); }
	float TestLineForCollision(const D3DXVECTOR3* pvStart, 
                           const D3DXVECTOR3* pvEnd, int ownerID, int *victimID); // Returns the time (0,1) of a collision, -1 if none  
	// Populates pvOut with the surface normal of the ground the player with given physicsID
	// is standing on.
	// Returns false if player is not grounded (no collision)
	bool GetSurfaceNormal(int physicsID, D3DXVECTOR3* pvOut); 
};


//
// DEBUG RENDER FUNCTIONS
//
static void TAC_CALL RenderLine(const TA::Vec3 &v3PosA, const TA::Vec3 &v3PosB, TA::u32 nColor);
static void TAC_CALL RenderArrow(const TA::Vec3 &v3PosA, const TA::Vec3 &v3PosB, TA::u32 nColor);
static void TAC_CALL RenderPolygon(int nNumVertices, const TA::Vec3 *pv3VertexList, const TA::Vec3 *pv3NormalList);

bool TA_CALL_BACK OnDynamicVsDynamicCollisionTest(TA::DynamicObject& objectA, TA::DynamicObject& objectB);
