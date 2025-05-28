/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Physics.h"

CL64_Physics::CL64_Physics(void)
{
	collisionConfiguration = NULL;
	dispatcher = NULL;
	overlappingPairCache = NULL;
	solver = NULL;
	dynamicsWorld = NULL;

	mShape = NULL;
	myMotionState = NULL;
	Area_Phys_Body = NULL;
	triMesh = NULL;

	flag_GD_Physics_On = 0;
	flag_Physics_Dlg_Active = 0;
	flag_TriMesh_Created = 0;
	flag_Debug_All = 0;
}

CL64_Physics::~CL64_Physics(void)
{
}

// *************************************************************************
// *			Init_Bullet:- Terry and Hazel Flanigan 2024   	 	 	   *
// *************************************************************************
void CL64_Physics::Init_Bullet()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();

	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	overlappingPairCache = new btDbvtBroadphase();

	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -30, 0));
	dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = 0.00001f;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;

}

// *************************************************************************
// *			ShutDown_Bullet:- Terry and Hazel Flanigan 2024   	  	   *
// *************************************************************************
void CL64_Physics::ShutDown_Bullet()
{
	int i = 0;

	//remove the rigidbodies from the dynamics world and delete them
	for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		dynamicsWorld->removeCollisionObject(obj);

		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;

	delete collisionConfiguration;

	collisionShapes.clear();
}

// *************************************************************************
//			Create_New_Trimesh:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Physics::Create_New_Trimesh(Ogre::Entity* Entity, Ogre::SceneNode* Node)
{
#pragma warning(disable : 4996) // Nightmare why

	// Get the mesh from the entity
	Ogre::MeshPtr myMesh = Entity->getMesh();
	Ogre::Mesh::SubMeshIterator SubMeshIter = myMesh->getSubMeshIterator();

	// Create the triangle mesh
	btVector3 vert0, vert1, vert2;
	int i = 0;

	while (SubMeshIter.hasMoreElements())
	{
		i = 0;
		Ogre::SubMesh* subMesh = SubMeshIter.getNext();
		Ogre::IndexData* indexData = subMesh->indexData;
		Ogre::VertexData* vertexData = subMesh->vertexData;

		// -------------------------------------------------------
		// Get the position element
		const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		// Get a pointer to the vertex buffer
		Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
		// Get a pointer to the index buffer
		Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;

		// -------------------------------------------------------
		// The vertices and indices used to create the triangle mesh
		std::vector<Ogre::Vector3> vertices;
		vertices.reserve(vertexData->vertexCount);
		std::vector<unsigned long> indices;
		indices.reserve(indexData->indexCount);

		// -------------------------------------------------------
		// Lock the Vertex Buffer (READ ONLY)
		unsigned char* vertex = static_cast<unsigned char*> (vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		float* pReal = NULL;

		for (size_t j = 0; j < vertexData->vertexCount; ++j, vertex += vBuffer->getVertexSize()) {
			posElem->baseVertexPointerToElement(vertex, &pReal);
			Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

			vertices.push_back(pt);
		}
		vBuffer->unlock();
		// -------------------------------------------------------
		bool use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		// -------------------------------------------------------
		// Lock the Index Buffer (READ ONLY)
		unsigned long* pLong = static_cast<unsigned long*> (iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*> (pLong);

		if (use32bitindexes) {
			for (size_t k = 0; k < indexData->indexCount; ++k) {
				//
				indices.push_back(pLong[k]);
			}
		}
		else {
			for (size_t k = 0; k < indexData->indexCount; ++k) {
				//
				indices.push_back(static_cast<unsigned long> (pShort[k]));
			}
		}
		iBuffer->unlock();

		// -------------------------------------------------------
		// We now have vertices and indices ready to go
		// ----

		if (triMesh == nullptr)
		{
			triMesh = new btTriangleMesh(use32bitindexes);
		}

		for (size_t y = 0; y < indexData->indexCount / 3; y++) {
			// Set each vertex
			vert0.setValue(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
			vert1.setValue(vertices[indices[i + 1]].x, vertices[indices[i + 1]].y, vertices[indices[i + 1]].z);
			vert2.setValue(vertices[indices[i + 2]].x, vertices[indices[i + 2]].y, vertices[indices[i + 2]].z);

			// Add the triangle into the triangle mesh
			triMesh->addTriangle(vert0, vert1, vert2);

			// Increase index count
			i += 3;
		}
	}

	const bool useQuantizedAABB = true;
	mShape = new btBvhTriangleMeshShape(triMesh, false, true);
	//collisionShapes.push_back(mShape); Not Needed in this Application

	float x = Node->getPosition().x;
	float y = Node->getPosition().y;
	float z = Node->getPosition().z;

	btVector3 inertia(0, 0, 0);
	mShape->calculateLocalInertia(0.0, inertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
	btVector3 initialPosition(x, y, z);
	startTransform.setOrigin(initialPosition);

	myMotionState = new btDefaultMotionState(startTransform);


	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI
	(
		0,				// mass
		myMotionState,	// initial position
		mShape,			// collision shape of body
		inertia			// local inertia
	);

	Area_Phys_Body = new btRigidBody(rigidBodyCI);
	Area_Phys_Body->clearForces();
	Area_Phys_Body->setLinearVelocity(btVector3(0, 0, 0));
	Area_Phys_Body->setAngularVelocity(btVector3(0, 0, 0));
	Area_Phys_Body->setWorldTransform(startTransform);

	int f = Area_Phys_Body->getCollisionFlags();
	Area_Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	dynamicsWorld->addRigidBody(Area_Phys_Body);

	flag_TriMesh_Created = 1;

	return 1;
}

// *************************************************************************
// *			Clear_Trimesh:- Terry and Hazel Flanigan 2024   	  	   *
// *************************************************************************
void CL64_Physics::Clear_Trimesh()
{
	if (mShape && flag_TriMesh_Created == 1)
	{
		delete mShape;
		mShape = NULL;

		delete myMotionState;
		myMotionState = NULL;

		dynamicsWorld->removeCollisionObject(Area_Phys_Body);

		delete triMesh;
		triMesh = NULL;

		flag_TriMesh_Created = 0;
	}
}

// *************************************************************************
// *			Show_Debug_Area:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Physics::Show_Debug_Area(bool Show)
{
	int f = Area_Phys_Body->getCollisionFlags();

	if (Show == 1)
	{
		Area_Phys_Body->setCollisionFlags(f & (~(1 << 5)));
	}
	else
	{
		Area_Phys_Body->setCollisionFlags(f | (1 << 5));
	}

	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;
	App->CL_Ogre->Bullet_Debug_Listener->Clear_Debug_Render();
	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 1;
}

// *************************************************************************
// *	  		Reset_Physics:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Physics::Reset_Physics(void)
{
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;

	float w = 1;
	float x = 0;
	float y = 0;
	float z = 0;

	if (App->CL_Scene->flag_Player_Added == 1)// && GD_Reset_Player == 1)
	{
		btVector3 zeroVector(0, 0, 0);

		x = App->CL_Scene->B_Player[0]->StartPos.x;
		y = App->CL_Scene->B_Player[0]->StartPos.y;
		z = App->CL_Scene->B_Player[0]->StartPos.z;

		btVector3 initialPosition(x, y, z);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setRotation(btQuaternion(App->CL_Scene->B_Player[0]->Physics_Rotation));
		startTransform.setOrigin(initialPosition);

		App->CL_Scene->B_Player[0]->Phys_Body->clearForces();
		App->CL_Scene->B_Player[0]->Phys_Body->setLinearVelocity(zeroVector);
		App->CL_Scene->B_Player[0]->Phys_Body->setAngularVelocity(zeroVector);

		App->CL_Scene->B_Player[0]->Phys_Body->setWorldTransform(startTransform);
		App->CL_Scene->B_Player[0]->Phys_Body->getMotionState()->setWorldTransform(startTransform);
		App->CL_Scene->B_Player[0]->Phys_Body->activate(true);

		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);

		App->CL_Com_Player->Set_Player_Physics_Position(0);
	}
}

// *************************************************************************
// *	  		Reset_Player:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Physics::Reset_Player(void)
{
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;

	float w = 1;
	float x = 0;
	float y = 0;
	float z = 0;

	if (App->CL_Scene->flag_Player_Added == 1)// && GD_Reset_Player == 1)
	{
		btVector3 zeroVector(0, 0, 0);

		x = App->CL_Scene->B_Player[0]->StartPos.x;
		y = App->CL_Scene->B_Player[0]->StartPos.y;
		z = App->CL_Scene->B_Player[0]->StartPos.z;

		btVector3 initialPosition(x, y, z);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setRotation(btQuaternion(App->CL_Scene->B_Player[0]->Physics_Rotation));
		startTransform.setOrigin(initialPosition);

		App->CL_Scene->B_Player[0]->Phys_Body->clearForces();
		App->CL_Scene->B_Player[0]->Phys_Body->setLinearVelocity(zeroVector);
		App->CL_Scene->B_Player[0]->Phys_Body->setAngularVelocity(zeroVector);

		App->CL_Scene->B_Player[0]->Phys_Body->setWorldTransform(startTransform);
		App->CL_Scene->B_Player[0]->Phys_Body->getMotionState()->setWorldTransform(startTransform);
		App->CL_Scene->B_Player[0]->Phys_Body->activate(true);

		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);

		App->CL_Com_Player->Set_Player_Physics_Position(0);
	}
}
// *************************************************************************
// *	  		Reset_Scene:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Physics::Reset_Scene(bool resetPlayer)
{
	// Reset player settings if required
	if (resetPlayer)
	{
		App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
		App->CL_Physics->Reset_Player();
	}

	// Initialize a zero vector for resetting velocities
	btVector3 zeroVector(0, 0, 0);

	// Iterate through all objects in the scene
	for (int count = 0; count < App->CL_Scene->Object_Count; ++count)
	{
		// Check if the object is dynamic
		if (App->CL_Scene->B_Object[count]->Usage == Enums::Obj_Usage_Dynamic)
		{
			// Retrieve the object's position and quaternion
			const auto& object = App->CL_Scene->B_Object[count];
			btVector3 initialPosition(object->Physics_Pos.x, object->Physics_Pos.y, object->Physics_Pos.z);
			btQuaternion initialRotation(object->Physics_Quat.x, object->Physics_Quat.y, object->Physics_Quat.z, object->Physics_Quat.w);

			// Set up the transformation for the physics body
			btTransform startTransform;
			startTransform.setIdentity();
			startTransform.setRotation(initialRotation);
			startTransform.setOrigin(initialPosition);

			// Reset physics body properties
			object->Phys_Body->clearForces();
			object->Phys_Body->setLinearVelocity(zeroVector);
			object->Phys_Body->setAngularVelocity(zeroVector);
			object->Phys_Body->setWorldTransform(startTransform);
			object->Phys_Body->getMotionState()->setWorldTransform(startTransform);
			object->Phys_Body->activate(true);

			// Update the object's visual representation
			object->Object_Node->setPosition(object->Mesh_Pos);
			object->Object_Node->setOrientation(object->Mesh_Quat);
		}
	}
}


// *************************************************************************
// *			Show_Debug_Objects:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Physics::Show_Debug_Objects(bool Show)
{
	int Count = 0;
	while (Count < App->CL_Scene->Object_Count)
	{
		if (App->CL_Scene->B_Object[Count]->Phys_Body)
		{
			int f = App->CL_Scene->B_Object[Count]->Phys_Body->getCollisionFlags();

			if (Show == true)
			{
				App->CL_Scene->B_Object[Count]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
			}
			else
			{
				App->CL_Scene->B_Object[Count]->Phys_Body->setCollisionFlags(f | (1 << 5));
			}
		}

		Count++;
	}

	/*Count = 0;
	while (Count < App->CL_Editor->Player_Count)
	{

		if (App->CL_Editor->B_Player[Count]->Phys_Body)
		{
			int f = App->CL_Editor->B_Player[Count]->Phys_Body->getCollisionFlags();

			if (Show == true)
			{
				App->CL_Editor->B_Player[Count]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
			}
			else
			{
				App->CL_Editor->B_Player[Count]->Phys_Body->setCollisionFlags(f | (1 << 5));
			}
		}

		Count++;
	}*/

	/*Count = 0;
	while (Count < App->CL_Editor->Area_Count)
	{

		if (App->CL_Editor->B_Area[Count]->Phys_Body)
		{
			int f = App->CL_Editor->B_Area[Count]->Phys_Body->getCollisionFlags();

			if (Show == true)
			{
				App->CL_Editor->B_Area[Count]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
			}
			else
			{
				App->CL_Editor->B_Area[Count]->Phys_Body->setCollisionFlags(f | (1 << 5));
			}
		}

		Count++;
	}*/

	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;
	App->CL_Ogre->RenderFrame(1);
	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 1;
}

// *************************************************************************
//				Set_Physics_New:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Physics::Set_Physics_New(int Index)
{
	// Rotation
	App->CL_Scene->B_Object[Index]->Physics_Quat = App->CL_Scene->B_Object[Index]->Object_Node->getOrientation();

	float w = App->CL_Scene->B_Object[Index]->Physics_Quat.w;
	float x = App->CL_Scene->B_Object[Index]->Physics_Quat.x;
	float y = App->CL_Scene->B_Object[Index]->Physics_Quat.y;
	float z = App->CL_Scene->B_Object[Index]->Physics_Quat.z;

	App->CL_Scene->B_Object[Index]->Phys_Body->getWorldTransform().setRotation(btQuaternion(x, y, z, w));

	// Scale
	App->CL_Scene->B_Object[Index]->Object_Node->setScale(App->CL_Scene->B_Object[Index]->Mesh_Scale);

	Ogre::Vector3 Scale = App->CL_Scene->B_Object[Index]->Object_Node->getScale();
	App->CL_Scene->B_Object[Index]->Phys_Body->getCollisionShape()->setLocalScaling(btVector3(Scale.x, Scale.y, Scale.z));

	// Position
	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	App->CL_Scene->B_Object[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));
	App->CL_Scene->B_Object[Index]->Physics_Pos = Centre;

	// All Good
	App->CL_Scene->B_Object[Index]->flag_Physics_Valid = 1;
}