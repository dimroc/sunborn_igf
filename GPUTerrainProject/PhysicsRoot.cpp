#include "dxstdafx.h"
#include "PhysicsRoot.h"
#include "FXDecl.h"
#include "globaldefine.h"

#include "TA/Physics/DynamicObject.h"
#include "TA/Physics/StaticObject.h"
#include "TA/Physics/CollisionObjectAABBMesh.h"
#include "TA/Physics/CollisionObjectCombo.h"
#include "TA/Physics/CollisionObjectCapsule.h"
#include "TA/Physics/PhysicsRender.h" 

// Inclusions from the Graphics area. Sorry I'm lazy.
#include "World.h"
#include "QuadTree.h"
#include "ITerrain.h"
#include "ModelInstance.h"
#include "XModelHeirarchy.h"

#include "GameSettings.h"

//---------------------------------------------------------------------------------
// PUBLIC SHIZNIT
//---------------------------------------------------------------------------------

CPhysicsRoot::CPhysicsRoot(CWorld* pWorld, CGameSettings* pSettings) :
m_fTimeStep(1.f/30.f), m_fTimeRemainder(0.f)
{
	assert(pWorld && pSettings);

	AssignSettings(pSettings);

	//---------------------------------------------------------------------------------
	// Initialise physics.
	//---------------------------------------------------------------------------------
	TA::Physics::CreateInstance();

	// Make a reference to the physics instance for convienience.
	TA::Physics& physics = TA::Physics::GetInstance();

	// Set the size of the world.
	float fWorldExtent = pWorld->GetWorldExtent();

	TA::AABB aabb;
	aabb.Initialise(
		TA::Vec3(0.0f, 0.0f, 0.0f),                                 // Center
		TA::Vec3(fWorldExtent, fWorldExtent, fWorldExtent));        // Extent
	physics.SetWorldDimensions(aabb);

	physics.SetGravity(TA::Vec3(0.0f, -9.81f, 0.0f)); //! Set gravity. -9.81 is default anyways.    
	physics.SetupSimulation(); //! Initialise the simulation based on the settings set above.


	const EntityHashMap& hmEntities = pWorld->GetEntities();
	if(hmEntities.size() > 0)
	{
		//
		// Add SMeshEntities as ONE static object in physics world.
		//
		TA::StaticObject* pStaticObject = TA::StaticObject::CreateNew();    //! TA static object

		vector<D3DXVECTOR3> vVerts;
		vector<int> vIndices;

		EntityHashMap::const_iterator it = hmEntities.begin();
		for( ; it != hmEntities.end(); it++) 
		{   // pushes valid triangles into vectors to be batch dumped in a collision object.
			ID3DXMesh* pMesh = it->second.pModelInst->GetXModel()->GetStandardMesh();
			GatherVerticesAndIndices( vVerts, vIndices, pMesh, &it->second.mWorld );
		}
		TA::CollisionObjectAABBMesh* pStaticCollisionObject = 
			CreateStaticCollisionObject( vVerts, vIndices );

		//
		// Add the ONE static(collision)Object for all StaticMeshes(obstacles) to physics simulation.
		//        
		pStaticObject->Initialise(pStaticCollisionObject); //! Initialise the static object with the collision object.            
		physics.AddStaticObject(pStaticObject); //! Add the static object to the simulation.

		SAFE_RELEASE(pStaticCollisionObject); //! We no long need the reference.
		m_vpStaticObjects.push_back(pStaticObject);
	}

	//
	// Add each island as a static object in physics world.
	//    
	const vector<CQuadTree*>& vpIslands = pWorld->GetIslands();
	if(vpIslands.size() > 0)
	{   
		TA::StaticObject* pStaticObject = TA::StaticObject::CreateNew();    //! TA static object

		vector<D3DXVECTOR3> vVerts;
		vector<int> vIndices;

		for(size_t i=0; i<vpIslands.size(); i++) 
		{
			GatherStaticIsland( vVerts, vIndices, vpIslands[i]);
		}
		TA::CollisionObjectAABBMesh* pStaticCollisionObject = 
			CreateStaticCollisionObject( vVerts, vIndices );

		pStaticObject->Initialise(pStaticCollisionObject); //! Initialise the static object with the collision object.    
		physics.AddStaticObject(pStaticObject); //! Add the static object to the simulation.

		SAFE_RELEASE(pStaticCollisionObject);
		m_vpStaticObjects.push_back(pStaticObject);
	}

	//
	// Set the render callbacks for use with directx
	//
	TA::PhysicsRender::SetRenderPolygonCallback(RenderPolygon);
	TA::PhysicsRender::SetRenderLineCallback(RenderLine);
	TA::PhysicsRender::SetRenderArrowCallback(RenderArrow);

	// Set up groups for future dynamic objects
	m_nPlayerGroupID = TA::DynamicObject::CreateUserGroup();
	m_nSLEGroupID = TA::DynamicObject::CreateUserGroup();

	physics.SetDynamicVsDynamicCollisionTestCallBack(OnDynamicVsDynamicCollisionTest);
}

CPhysicsRoot::~CPhysicsRoot()
{
	for(size_t i=0; i<m_vpStaticObjects.size(); i++)
	{
		SAFE_RELEASE(m_vpStaticObjects[i]);
	}

	//---------------------------------------------------------------------------------
	// Finalise physics.
	//---------------------------------------------------------------------------------
	TA::Physics::DestroyInstance(); 

	//---------------------------------------------------------------------------------
	// Clean up Objects.
	//---------------------------------------------------------------------------------

	DynamicObjectsHashMap::iterator it = m_hmDynamicObjects.begin();
	for( ; it != m_hmDynamicObjects.end(); it++ )
	{
		SAFE_RELEASE(it->second);
	}
}

VOID CPhysicsRoot::OnFrameMove(float fElapsedTime)
{
	TA::Physics& physics = TA::Physics::GetInstance();
	m_fTimeRemainder += fElapsedTime;

	physics.Update(fElapsedTime);
#if 0    
	while( m_fTimeRemainder > m_fTimeStep )
	{   // iterate through physics simulation.
		physics.Update(m_fTimeStep);
		m_fTimeRemainder -= m_fTimeStep;
	}
#endif
}

VOID CPhysicsRoot::OnFrameRender(const D3DXMATRIX* pProj, const D3DXMATRIX* pView)
{
	HRESULT hr;
	LPDIRECT3DDEVICE9 pd3dDevice = DXUTGetD3DDevice();

	// V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, (DWORD)0xff000000, 1.0f, 0) );

	V( pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE) );
	V( pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE) );
	V( pd3dDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE) );	

	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);
	V( pd3dDevice->SetTransform(D3DTS_PROJECTION, pProj) );
	V( pd3dDevice->SetTransform(D3DTS_VIEW, pView) );
	V( pd3dDevice->SetTransform(D3DTS_WORLD, &mtx) );

	TA::Physics::GetInstance().Render();	// renders dynamic objects.
#if 0
	TA::MFrame mframe(TA::Vec3(0.f, 0.f, 0.f));
	// RENDER THE STATIC OBJECTS
	for(size_t i=0;i<m_vpStaticObjects.size(); i++)
	{
		m_vpStaticObjects[i]->GetCollisionObject().Render(mframe);
	}
#endif    
}

int CPhysicsRoot::AddDynamicCylinder(const D3DXVECTOR3* pvPosition, 
									 const D3DXVECTOR3* pvTopCenter, 
									 const D3DXVECTOR3* pvBottomCenter, 
									 float fRadius)
{
	TA::Physics& physics = TA::Physics::GetInstance();

	//---------------------------------------------------------------------------------
	// Add a dynamic object.
	//---------------------------------------------------------------------------------
	TA::DynamicObject* pDynamicObject = TA::DynamicObject::CreateNew();
	pDynamicObject->InitialiseAsACylinder(Vec3FromD3DXVector(pvTopCenter), 
		Vec3FromD3DXVector(pvBottomCenter),                                           
		fRadius);

	// Place the dynamic object
	pDynamicObject->SetPosition(Vec3FromD3DXVector(pvPosition));
	pDynamicObject->SetRotationDisabled(true);

	pDynamicObject->SetGravityMult(m_fGravMult);
	pDynamicObject->SetMass(m_fMass);
	pDynamicObject->SetFriction(m_fFriction);
	pDynamicObject->SetRestitution(m_fBounce);    

	// Add the dynamic object to the simulation
	physics.AddDynamicObject(pDynamicObject);
	physics.SetDynamicObjectIsMoving(pDynamicObject);

	m_hmDynamicObjects[pDynamicObject->GetID()] = pDynamicObject;

	return pDynamicObject->GetID();
}


int CPhysicsRoot::AddDynamicCapsuleCombo(const D3DXVECTOR3* pvPosition, 
										 const D3DXVECTOR3* pvTopCenter, 
										 const D3DXVECTOR3* pvBottomCenter, 
										 float fRadius, float fGroundOffset)
{
	using namespace TA;

	Physics& physics = Physics::GetInstance();

	//---------------------------------------------------------------------------------
	// Add a dynamic object.
	//---------------------------------------------------------------------------------
	DynamicObject* pDynamicObject = DynamicObject::CreateNew();

	// Create a combo collision object for this dynamic object
	CollisionObjectCombo* pCombo = CollisionObjectCombo::CreateNew();
	pCombo->Initialise();

	// Create body collision
	CollisionObjectCapsule* pCapsule = CollisionObjectCapsule::CreateNew();
	pCapsule->Initialise(Vec3(pvTopCenter->x, pvTopCenter->y, pvTopCenter->z), 
		Vec3(pvBottomCenter->x, pvBottomCenter->y, pvBottomCenter->z),                                           
		fRadius);
	pCombo->AddCollisionObject(pCapsule);
	pCapsule->Release();

	// Create line test collision data to attact to object to simulate legs.
	// It is faster to attach short lines to a collision object then use Physics::TestLineForCollision
	CollisionObjectLineList* pLines = CollisionObjectLineList::CreateNew();
	const float fLinesTop = -1 * fRadius;
	const float fLinesTopRadius = fRadius / -2.f;
	const float fLinesBottomRadius = fRadius / (8.f / -3.f);
	const float fLinesBottom = -fGroundOffset - 0.3f; // the ground pos minus a bit so the lines always go through the ground.
	Vec3 v3LinePoints[8] =
	{
		Vec3(fLinesTopRadius, fLinesTop, fLinesTopRadius),
		Vec3(fLinesBottomRadius, fLinesBottom, fLinesBottomRadius),

		Vec3(fLinesTopRadius, fLinesTop, -fLinesTopRadius),
		Vec3(fLinesBottomRadius, fLinesBottom, -fLinesBottomRadius),

		Vec3(-fLinesTopRadius, fLinesTop, -fLinesTopRadius),
		Vec3(-fLinesBottomRadius, fLinesBottom, -fLinesBottomRadius),

		Vec3(-fLinesTopRadius, fLinesTop, fLinesTopRadius),
		Vec3(-fLinesBottomRadius, fLinesBottom, fLinesBottomRadius),
	};
	pLines->Initialise(v3LinePoints, 4);
	pCombo->AddCollisionObject(pLines);
	pLines->Release();

	// Iniatialise the dynamic object with the combined collision ojbects.
	pDynamicObject->Initialise(pCombo);
	pCombo->Release();

	// Place the dynamic object
	pDynamicObject->SetPosition(Vec3(pvPosition->x, pvPosition->y, pvPosition->z));
	pDynamicObject->SetRotationDisabled(true);

	pDynamicObject->SetGravityMult(m_fGravMult);
	pDynamicObject->SetMass(m_fMass);
	pDynamicObject->SetFriction(m_fFriction);
	pDynamicObject->SetRestitution(m_fBounce);    

	// Add the dynamic object to the simulation
	physics.AddDynamicObject(pDynamicObject);
	physics.SetDynamicObjectIsMoving(pDynamicObject);

	m_hmDynamicObjects[pDynamicObject->GetID()] = pDynamicObject;

	return pDynamicObject->GetID();
}
int CPhysicsRoot::AddDynamicSphere(const D3DXVECTOR3* pvPosition, 
								   float fRadius, float fMass){
									   TA::Physics& physics = TA::Physics::GetInstance();

									   //---------------------------
									   // Add a dynamic object.
									   //---------------------------
									   TA::DynamicObject* pDynamicObject = TA::DynamicObject::CreateNew();
									   pDynamicObject->InitialiseAsASphere(TA::Vec3(0.f,0.f,0.f), 
										   fRadius);

									   // Place the dynamic object
									   pDynamicObject->SetPosition(Vec3FromD3DXVector(pvPosition));
									   pDynamicObject->SetRotationDisabled(true);

									   pDynamicObject->SetGravityMult(0.0);
									   pDynamicObject->SetMass(fMass);
									   pDynamicObject->SetFriction(m_fFriction);
									   pDynamicObject->SetRestitution(m_fBounce);    

									   pDynamicObject->SetCollisionListEnabled(true);

									   // Add the dynamic object to the simulation
									   physics.AddDynamicObject(pDynamicObject);
									   physics.SetDynamicObjectIsMoving(pDynamicObject);

									   m_hmDynamicObjects[pDynamicObject->GetID()] = pDynamicObject;

									   return pDynamicObject->GetID();
}
void CPhysicsRoot::DeleteDynamicObject(int physicsID)
{
	if (!DynamicObjectExists(physicsID))
		return;
	TA::Physics& physics = TA::Physics::GetInstance();
	physics.RemoveDynamicObject(m_hmDynamicObjects[physicsID]);
	SAFE_RELEASE(m_hmDynamicObjects[physicsID]);
	CPhysicsRoot::m_hmDynamicObjects.erase(physicsID);
}


VOID CPhysicsRoot::SetLinearVelocity(int physicsID, const D3DXVECTOR3* pvVelocity)
{
	TA::Physics::GetInstance().SetDynamicObjectIsMoving(m_hmDynamicObjects[physicsID]);
	m_hmDynamicObjects[physicsID]->SetLinearVelocity(TA::Vec3(pvVelocity->x, pvVelocity->y, pvVelocity->z));
}

bool CPhysicsRoot::SetToDesiredVelocity(int physicsID, D3DXVECTOR3* pvVelocity, float fGroundOffset, bool bGrounded, float fDt)
{
	using namespace TA;

	DynamicObject* pDynamicObject =  m_hmDynamicObjects[physicsID];
	Vec3 v3Velocity = Vec3(pvVelocity->x, pvVelocity->y, pvVelocity->z);

	Vec3 v3RayNormal = Vec3(0.f, 1.f, 0.f);
	float fGroundHeight = 0.0f;
	Vec3 v3GroundNormal = Vec3(0.f, 0.f, 0.f);

	bool bFeetOnSurface = false;

	pDynamicObject->SetCollisionDisabled(true); // So that the test ray doesn't intersect with this object

	// Check how our legs are hitting the ground.
	int nColCount = 0;
	for (int i = 0; i < pDynamicObject->GetNumLineCollisions(); i++)
	{
		const DynamicObject::LineIntersection& collision = pDynamicObject->GetLineCollision(i);
		if (collision.GetIntersectionDepth() > 0.0f)
		{
			nColCount++;
			bFeetOnSurface = true;
			fGroundHeight += (collision.GetPosition() * pDynamicObject->GetFrame()).y;
			// Take the maximum normal for the ground.
			// Note: This method will may cause problems if trying to walk
			// through a narow valley with no steep sides and no flat bottom.
			if (v3GroundNormal.y < collision.GetNormal().y)
				v3GroundNormal = collision.GetNormal();
		}
	}
	if (nColCount){
		fGroundHeight /= (float)nColCount;
		Vec3 pos = pDynamicObject->GetPosition();
		float diff = pos.y + fGroundOffset - fGroundHeight;
		pDynamicObject->SetPosition(pos + (diff * Vec3(0.f, 0.f, 0.f)));
	}
	else
		v3GroundNormal = Vec3(0.f, 1.f, 0.f);

	pDynamicObject->SetCollisionDisabled(false);

	if (bFeetOnSurface)
	{
		pDynamicObject->SetGravityMult(0.0f); // turn off gravity if we are on the ground.

		// todo: take into account the movment of any object we might be standing on.
	}
	else
	{
		pDynamicObject->SetGravityMult(m_fGravMult); // turn on gravity if we are in the air.
	}
	// make sure the player doesn't come to rest if not moving.
	Physics::GetInstance().SetDynamicObjectIsMoving(pDynamicObject);

	if (bFeetOnSurface)
	{
		// Find the velocity along the ground plain that we are moving at.
		Vec3 v3HorizontalVelocity = pDynamicObject->GetLinearVelocity();
		Vec3 v3VerticalVelocity = v3GroundNormal * v3HorizontalVelocity.Dot(v3GroundNormal);
		v3HorizontalVelocity -= v3VerticalVelocity;


		// Find the velocity along the ground plain that we want to be moving at.
		Vec3 v3TargetHorizontalVelocity = v3Velocity;
		v3TargetHorizontalVelocity -= v3GroundNormal * v3TargetHorizontalVelocity.Dot(v3GroundNormal);

		//// make running up hill harder then downhill.
		//fMag = v3TargetHorizontalVelocity.GetMagnitude();
		//if (fMag > 1.0f)
		//{
		//	Vec3 v3Normal = v3TargetHorizontalVelocity / fMag;
		//	v3TargetHorizontalVelocity -= v3Normal * v3Normal.y * k_fSlideForce;
		//}

		// Try to move at the velocity we are trying to move at.
		v3HorizontalVelocity += (v3TargetHorizontalVelocity - v3HorizontalVelocity) * (1.0f - Exp(-20.0f * fDt));

		/*
		// Try to limit the travelling speed
		float fHorizontalSpeed = v3HorizontalVelocity.GetMagnitude();
		if (fHorizontalSpeed > k_fClampedHorizontalSpeed)
		{
		v3HorizontalVelocity /= fHorizontalSpeed;
		fHorizontalSpeed = k_fClampedHorizontalSpeed;
		v3HorizontalVelocity *= fHorizontalSpeed;
		}
		*/
		pDynamicObject->SetLinearVelocity(
			v3HorizontalVelocity + v3VerticalVelocity);
	}
	else
	{
		// Player is in the air.
		// Even though it is not physically realistic, it is typically better to give a little control to
		// the player in the air.

		//Had to comment this out cuz i dont have fDt currently, maybe can/should change that
		Vec3 v3HorizontalVelocity = pDynamicObject->GetLinearVelocity();
		Vec3 v3TargetHorizontalVelocity = v3Velocity;
		v3TargetHorizontalVelocity.y = v3HorizontalVelocity.y;
		v3HorizontalVelocity += (v3TargetHorizontalVelocity - v3HorizontalVelocity) * (1.0f - Exp(-2.0f * fDt));
		pDynamicObject->SetLinearVelocity(v3HorizontalVelocity);
	}

	return bFeetOnSurface;
}



bool CPhysicsRoot::UpdatePlayerMovement(int physicsID, D3DXVECTOR3* pvVelocity, float fGroundOffset, bool bJumping, bool bUncontrollable, float fDt){
	using namespace TA;

	DynamicObject* pDynamicObject =  m_hmDynamicObjects[physicsID];
	Vec3 v3Velocity = Vec3FromD3DXVector(pvVelocity);

	// make sure the player doesn't come to rest if not moving.
	Physics::GetInstance().SetDynamicObjectIsMoving(pDynamicObject);

	pDynamicObject->SetFriction(m_fFriction);

	bool bFeetOnSurface = false;
	bool bForwardCollision = false;

	Vec3 v3Position = pDynamicObject->GetPosition();
	const Vec3 v3Up = Vec3(0.f, 1.f, 0.f);

	Collision collision;

	if (bJumping){ //jumping
		// Make sure we didn't just hit the ground during this frame
		if (pDynamicObject->GetLinearVelocity().y <= 0) { // Test only if the player is on his way down
			pDynamicObject->SetCollisionDisabled(true); // So that the test ray doesn't intersect with this object

			// Check if our legs are hitting the ground.
			for (int i = 0; i < pDynamicObject->GetNumLineCollisions(); i++){
				const DynamicObject::LineIntersection& collision = pDynamicObject->GetLineCollision(i);
				if (collision.GetIntersectionDepth() > 0.0f){
					bFeetOnSurface = true;
					break;
				}
			}

			pDynamicObject->SetCollisionDisabled(false);
		}

		/***** HACK FIX BEGIN ****	
		collision = Physics::GetInstance().TestLineForCollision(
		v3Position, // start position
		v3Position + (v3Velocity * 10), // end position
		Physics::FLAG_ALL_NON_DYNAMIC_OBJECTS); // flags



		if (!bFeetOnSurface && !collision.CollisionOccurred()){ //switch this line with the one after the comment
		***** END HACK FIX *****/
		if (!bFeetOnSurface){
			// Player is in the air.
			// Give a little control to the player in the air.
			pDynamicObject->SetGravityMult(m_fGravMult); // turn on gravity if we are in the air.

			Vec3 v3HorizontalVelocity = pDynamicObject->GetLinearVelocity();
			Vec3 v3TargetHorizontalVelocity = v3Velocity;
			//v3TargetHorizontalVelocity.y = v3HorizontalVelocity.y;
			v3HorizontalVelocity += (v3TargetHorizontalVelocity - v3HorizontalVelocity) * (1.0f - Exp(-2.0f * fDt));
			pDynamicObject->SetLinearVelocity(v3HorizontalVelocity);
			return true;
		}
	}

	if (v3Velocity.IsZero()){
		return false; // just standing
	}
	// Test the static objects for collision to find the "top" 
	// of the ground at that x,z point (find y)
	#if 0
	// Find the intended x,z coordinate point
	Vec3 v3Test = Vec3(v3Position.x + (v3Velocity.x * fDt),
		v3Position.y,
		v3Position.z + (v3Velocity.z * fDt));

	collision = Physics::GetInstance().TestLineForCollision(
		v3Test + (v3Up * 200.f), // start position
		v3Test + (v3Up * -200.f), // end position
		Physics::FLAG_ALL_NON_DYNAMIC_OBJECTS); // flags
	#endif
//#if 0
	// Check feet for collision to find normal
	collision = Physics::GetInstance().TestLineForCollision(
		v3Position + (v3Up * 200.f), // start position
		v3Position + (v3Up * -200.f), // end position
		Physics::FLAG_ALL_NON_DYNAMIC_OBJECTS); // flags
	Vec3 normal = collision.GetNormal();
	//normal.Normalise();
	//v3Velocity.Normalise();
	// Find slope of ground
	// slope = n x (v x n)
	Vec3 right = v3Velocity.Cross(normal);
	float m = v3Velocity.GetMagnitude();
	//right.Normalise();
	Vec3 surface = normal.Cross(right);
	surface.Normalise();
	// Project the velocity onto the slope
	float k = v3Velocity.Dot(surface) / surface.Dot(surface);
	Vec3 v3ProjectedVelocity = m * surface;
	// Add velocity * time to the position 
	Vec3 newPos = Vec3(v3Position.x + (v3ProjectedVelocity.x * fDt),
		v3Position.y + (v3ProjectedVelocity.y * fDt),
		v3Position.z + (v3ProjectedVelocity.z * fDt));

	// Test this point for the ground
	collision = Physics::GetInstance().TestLineForCollision(
		newPos + (v3Up * 200.f), // start position
		newPos + (v3Up * -200.f), // end position
		Physics::FLAG_ALL_NON_DYNAMIC_OBJECTS); // flags
//#endif
	if (collision.CollisionOccurred()){
		// Set the players position to that x,y,z coord + a groundoffset
		pDynamicObject->SetGravityMult(0.0f); // turn off gravity if we are on the ground.
		pDynamicObject->SetPosition(collision.GetPosition() + (v3Up * fGroundOffset));
		pDynamicObject->SetLinearVelocity(Vec3(0.f,0.f,0.f));
	} else { 
		// The player walked off a cliff/ very steep slope
		// Just set the velocity to the requested
		pDynamicObject->SetGravityMult(m_fGravMult); // turn on gravity if we are in the air.
		pDynamicObject->SetLinearVelocity(v3Velocity);
		return true; // make us airborne
	}

	return false;
}

//! Returns the physicsIDs of the objects this SLE collided with
void CPhysicsRoot::UpdateSLE(int physicsID, int ownerID, set<int> &sVictims){
	using namespace TA;

	DynamicObject* pDynamicObject = m_hmDynamicObjects[physicsID];
	assert(pDynamicObject);

	// check for last frame's collisions
	DynamicObject::CollisionIterator it = pDynamicObject->GetCollisionIterator();
	while(!it.AtEnd())
	{
		const Collision& collision = *it;	
		// see if it collided with a player
#if 0  // Testin out some stuff to make sure i'm getting the right id
		DynamicObject* objA = collision.GetObjectA();
		DynamicObject* objB = collision.GetObjectB();
		if (objA)
			dprintf("\nobjA ID: %d", objA->GetID());
		if (objB)
			dprintf("    objB ID: %d", objB->GetID());
		dprintf("    my ID: %d\n", pDynamicObject->GetID());
#endif
		//		#if 0
		DynamicObject* obj;
		if (collision.GetObjectA()->GetID() != pDynamicObject->GetID())
			obj = collision.GetObjectA();
		else obj = collision.GetObjectB();

		int id = obj->GetID();
		if (id != ownerID){ // we don't mind if it collides with owner
			//if (obj)
			//	dprintf("    obj ID: %d", obj->GetID());
			//dprintf("    my ID: %d\n", physicsID);
			sVictims.insert(id);
		}
		//#endif
		it++;
	}
}
VOID CPhysicsRoot::ApplyLinearImpulse(int physicsID, const D3DXVECTOR3* pvImpulse){
	TA::Physics::GetInstance().SetDynamicObjectIsMoving(m_hmDynamicObjects[physicsID]);
	m_hmDynamicObjects[physicsID]->ApplyLinearImpulse(TA::Vec3(pvImpulse->x, pvImpulse->y, pvImpulse->z));
}

float CPhysicsRoot::TestLineForCollision(const D3DXVECTOR3* pvStart, const D3DXVECTOR3* pvEnd, int ownerID, int *victimID){
	using namespace TA;

	// Don't let the line collide with the owner
	m_hmDynamicObjects[ownerID]->SetCollisionDisabled(true);
	Collision collision = Physics::GetInstance().TestLineForCollision(
		Vec3FromD3DXVector(pvStart), // start position
		Vec3FromD3DXVector(pvEnd), // end position
		Physics::FLAG_DYNAMIC_OBJECTS); // flags
	m_hmDynamicObjects[ownerID]->SetCollisionDisabled(false);
	if (!collision.CollisionOccurred())
		return -1;

	*victimID = collision.GetObjectA()->GetID();
	return collision.GetTime();

}
bool CPhysicsRoot::GetSurfaceNormal(int physicsID, D3DXVECTOR3* pvOut){
	using namespace TA;

	DynamicObject* pDynamicObject = m_hmDynamicObjects[physicsID];
	if (!pDynamicObject) return false;

	bool success = false;
	pDynamicObject->SetCollisionDisabled(true); // So that the test ray doesn't intersect with this object

	// Check if our legs are hitting the ground.
	for (int i = 0; i < pDynamicObject->GetNumLineCollisions(); i++){
		const DynamicObject::LineIntersection& collision = pDynamicObject->GetLineCollision(i);
		if (collision.GetIntersectionDepth() > 0.0f){
			Vec3 vNormal = collision.GetNormal();
			pvOut->x = vNormal.x;
			pvOut->y = vNormal.y;
			pvOut->z = vNormal.z;
			success = true;
			break;
		}
	}

	pDynamicObject->SetCollisionDisabled(false);

	return success;
}

//---------------------------------------------------------------------------------
// PRIVATE SHIZNIT
//---------------------------------------------------------------------------------

//! Assigns settings to physics world.
VOID CPhysicsRoot::AssignSettings(CGameSettings* pSettings)
{
	m_fGravMult = pSettings->GetGravMult();
	m_fMass = pSettings->GetMass();
	m_fFriction = pSettings->GetFriction();
	m_fBounce = pSettings->GetBounce();
}


//! Stuffs the passed vectors with world space vertices/indices pertaining to an island (heightmap/basin/pillar).
VOID CPhysicsRoot::GatherStaticIsland(vector<D3DXVECTOR3> &vVerts, vector<int> &vIndices, const CQuadTree* pIsland)
{
	int indexOffset = (int)vVerts.size();

	const ITerrain *pt = pIsland->GetTerrain();

	const float* pHeightMap = pt->GetHeightMap();
	const D3DXVECTOR3* vTranslation = pIsland->GetTranslation();

	//
	// Generate Triangles from the heightmap
	//
	for(UINT i=0; i<pt->GetHeight(); i++)
	{   // push back all hmp vertices
		for(UINT j=0; j<pt->GetWidth(); j++)
		{
			D3DXVECTOR3 vec;
			vec.y = pHeightMap[i*pt->GetWidth() + j] * pt->GetHeightSpacing();
			vec.x = (float)(j * pt->GetCellSpacing());
			vec.z = (float)(i * pt->GetCellSpacing());
			vec += *vTranslation;
			vVerts.push_back(vec);
		}
	}
	for(UINT i=0; i<pt->GetHeight() - 1; i++)
	{   // generate triangles via indices.
		for(UINT j=0; j<pt->GetWidth() - 1; j++)
		{
			vIndices.push_back(i*pt->GetWidth() + j + indexOffset);
			vIndices.push_back(i*pt->GetWidth() + j + 1 + indexOffset);
			vIndices.push_back((i+1)*pt->GetWidth() + j + indexOffset);

			vIndices.push_back(i*pt->GetWidth() + j + 1 + indexOffset);
			vIndices.push_back((i+1)*pt->GetWidth() + j + 1 + indexOffset);
			vIndices.push_back((i+1)*pt->GetWidth() + j + indexOffset);
		}
	}

	// TODO: Basin and Pillar
}

//! Gathers the valid triangle vertices from the passed in mesh, transforms them to world space
//! and stuffs them into the passed vectors.
VOID CPhysicsRoot::GatherVerticesAndIndices( vector<D3DXVECTOR3> &vVerts,
											vector<int> &vIndices,
											ID3DXMesh* pMesh, 
											const D3DXMATRIX* pWorld )
{
	HRESULT hr;

	int indexOffset = (int)vVerts.size();

	// get the vertex/index buffers to run thru into the collision object.
	LPDIRECT3DVERTEXBUFFER9 pVB;
	LPDIRECT3DINDEXBUFFER9 pIB;

	pMesh->GetIndexBuffer( &pIB );
	pMesh->GetVertexBuffer( &pVB );    

	UINT vblength = pMesh->GetNumVertices();
	UINT facelength = pMesh->GetNumFaces();
	UINT iblength = facelength * 3;

	//
	// Go through vertices
	//
	D3DXVECTOR4 vec;
	BoxVertTexColor* pVerts;
	V( pVB->Lock( 0, 0, (void**)&pVerts, D3DLOCK_READONLY ) );
	for(UINT i=0; i<vblength; i++)
	{   // Add world space vertices to collision object.
		D3DXVec3Transform(&vec, &pVerts[i].pos, pWorld);
		vVerts.push_back(D3DXVECTOR3(vec.x, vec.y, vec.z));
	}
	V( pVB->Unlock() );

	//
	// Go through indices
	//
	short* pIndices;
	V( pIB->Lock( 0, 0, (void**)&pIndices, D3DLOCK_READONLY ) );    
	for(UINT i=0; i<iblength; i+=3)
	{   // Go through each face as if Triangle List        
		if(pIndices[i] != pIndices[i+1] && pIndices[i] != pIndices[i+2] && pIndices[i+1] != pIndices[i+2] && 
			abs(gdef::TriangleArea(&vVerts[pIndices[i]], &vVerts[pIndices[i+1]], &vVerts[pIndices[i+2]]) ) > 0.f)
		{   // make sure not zero triangle
			vIndices.push_back(pIndices[i] + indexOffset);
			vIndices.push_back(pIndices[i+1] + indexOffset);
			vIndices.push_back(pIndices[i+2] + indexOffset);
		}
	}
	V( pIB->Unlock() );    

	SAFE_RELEASE(pVB);
	SAFE_RELEASE(pIB);
}

TA::CollisionObjectAABBMesh* CPhysicsRoot::CreateStaticCollisionObject( vector<D3DXVECTOR3> &vVerts,
																	   vector<int> &vIndices )
{
	UINT vblength = (UINT)vVerts.size();
	UINT iblength = (UINT)vIndices.size();
	UINT facelength = (UINT)iblength / 3;    

	// Create a collision object to add to the static object.    
	TA::CollisionObjectAABBMesh* pStaticCollisionObject = TA::CollisionObjectAABBMesh::CreateNew();

	pStaticCollisionObject->Initialise(
		vblength,                        // Num vertices.
		facelength,                      // Num polygons.
		iblength);                       // Num polygon indices. 

	//
	// Go through vertices
	//
	for(UINT i=0; i<vblength; i++)
	{   // Add world space vertices to collision object.
		pStaticCollisionObject->AddVertex(TA::Vec3(vVerts[i].x, vVerts[i].y, vVerts[i].z));        
	}    

	//
	// Go through indices
	//
	for(UINT i=0; i<iblength; i+=3)
	{   // Go through each face as if Triangle List
		pStaticCollisionObject->AddPolygon(3, &vIndices[i]);
	}

	pStaticCollisionObject->FinishedAddingGeometry();
	return pStaticCollisionObject;
}



//
// STATIC DEBUG RENDER FUNCTIONS
//
void TAC_CALL RenderLine(const TA::Vec3 &v3PosA, const TA::Vec3 &v3PosB, TA::u32 nColor)
{
	HRESULT hr;
	LPDIRECT3DDEVICE9 pd3dDevice = DXUTGetD3DDevice();

	V( pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE) );

	BoxVertColor verts[2];
	for(int i=0; i<2; i++) {
		verts[i].Color = nColor;
	}

	verts[0].Pos.x = v3PosA.x; verts[0].Pos.y = v3PosA.y; verts[0].Pos.z = v3PosA.z;
	verts[1].Pos.x = v3PosB.x; verts[1].Pos.y = v3PosB.y; verts[1].Pos.z = v3PosB.z;

	V( pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, verts, sizeof(BoxVertColor)) );
}

void TAC_CALL RenderArrow(const TA::Vec3 &v3PosA, const TA::Vec3 &v3PosB, TA::u32 nColor)
{
	HRESULT hr;
	LPDIRECT3DDEVICE9 pd3dDevice = DXUTGetD3DDevice();

	V( pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE) );

	BoxVertColor verts[2];
	for(int i=0; i<2; i++) {
		verts[i].Color = nColor;
	}

	verts[0].Pos.x = v3PosA.x; verts[0].Pos.y = v3PosA.y; verts[0].Pos.z = v3PosA.z;
	verts[1].Pos.x = v3PosB.x; verts[1].Pos.y = v3PosB.y; verts[1].Pos.z = v3PosB.z;

	V( pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, verts, sizeof(BoxVertColor)) );
}

void TAC_CALL RenderPolygon(int nNumVertices, const TA::Vec3 *pv3VertexList, const TA::Vec3 *pv3NormalList)
{
	HRESULT hr;
	LPDIRECT3DDEVICE9 pd3dDevice = DXUTGetD3DDevice();

	V( pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE) );
	V( pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ) );

	BoxVertColor *verts = new BoxVertColor[nNumVertices];
	for(int i=0; i<nNumVertices; i++) {
		verts[i].Pos.x = pv3VertexList[i].x;
		verts[i].Pos.y = pv3VertexList[i].y;
		verts[i].Pos.z = pv3VertexList[i].z;
		verts[i].Color = 0xffffff00;
	}

	V( pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, nNumVertices - 2, verts, sizeof(BoxVertColor)) );
	SAFE_DELETE_ARRAY(verts);

	V( pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ) );
}
bool TA_CALL_BACK OnDynamicVsDynamicCollisionTest(TA::DynamicObject& objectA, TA::DynamicObject& objectB){
	int groupA = objectA.GetUserGroup(), groupB = objectB.GetUserGroup();
	if (groupA == groupB)
		return false;


	//if (objectA.GetUserGroup() )
	return true;
}
