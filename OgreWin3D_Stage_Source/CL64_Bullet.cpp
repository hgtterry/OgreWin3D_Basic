/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Bullet.h"

CL64_Bullet::CL64_Bullet(void)
{
	collisionConfiguration = NULL;
	dispatcher = NULL;
	overlappingPairCache = NULL;
	solver = NULL;
	dynamicsWorld = NULL;

	mShape = NULL;
	myMotionState = NULL;
	Phys_Body = NULL;
	triMesh = NULL;

	GD_Physics_On = 0;
	Physics_Dlg_Active = 0;
	flag_TriMesh_Created = 0;
	flag_Debug_All = 0;
}

CL64_Bullet::~CL64_Bullet(void)
{
}

// *************************************************************************
// *			Init_Bullet:- Terry and Hazel Flanigan 2024   	 	 	   *
// *************************************************************************
void CL64_Bullet::Init_Bullet()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -30, 0));
	dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = 0.00001f;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;

}

// *************************************************************************
// *			ShutDown_Bullet:- Terry and Hazel Flanigan 2024   	  	   *
// *************************************************************************
void CL64_Bullet::ShutDown_Bullet()
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
bool CL64_Bullet::Create_New_Trimesh(Ogre::Entity* Entity, Ogre::SceneNode* Node)
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

	Phys_Body = new btRigidBody(rigidBodyCI);
	Phys_Body->clearForces();
	Phys_Body->setLinearVelocity(btVector3(0, 0, 0));
	Phys_Body->setAngularVelocity(btVector3(0, 0, 0));
	Phys_Body->setWorldTransform(startTransform);

	int f = Phys_Body->getCollisionFlags();
	Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	dynamicsWorld->addRigidBody(Phys_Body);

	flag_TriMesh_Created = 1;

	return 1;
}

// *************************************************************************
// *			Clear_Trimesh:- Terry and Hazel Flanigan 2024   	  	   *
// *************************************************************************
void CL64_Bullet::Clear_Trimesh()
{
	if (mShape && flag_TriMesh_Created == 1)
	{
		delete mShape;
		mShape = NULL;

		delete myMotionState;
		myMotionState = NULL;

		dynamicsWorld->removeCollisionObject(Phys_Body);

		delete triMesh;
		triMesh = NULL;

		flag_TriMesh_Created = 0;
	}
}

// *************************************************************************
// *			Show_Debug_Area:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Bullet::Show_Debug_Area(bool Show)
{
	int f = Phys_Body->getCollisionFlags();

	if (Show == 1)
	{
		Phys_Body->setCollisionFlags(f & (~(1 << 5)));
	}
	else
	{
		Phys_Body->setCollisionFlags(f | (1 << 5));
	}

	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
	App->CL_Ogre->RenderFrame(1);
	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;
}

// *************************************************************************
// *			Show_Debug_Objects:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Bullet::Show_Debug_Objects(bool Show)
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

	Count = 0;
	while (Count < App->CL_Scene->Player_Count)
	{

		if (App->CL_Scene->B_Player[Count]->Phys_Body)
		{
			int f = App->CL_Scene->B_Player[Count]->Phys_Body->getCollisionFlags();

			if (Show == true)
			{
				App->CL_Scene->B_Player[Count]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
			}
			else
			{
				App->CL_Scene->B_Player[Count]->Phys_Body->setCollisionFlags(f | (1 << 5));
			}
		}

		Count++;
	}

	Count = 0;
	while (Count < App->CL_Scene->Area_Count)
	{

		if (App->CL_Scene->B_Area[Count]->Phys_Body)
		{
			int f = App->CL_Scene->B_Area[Count]->Phys_Body->getCollisionFlags();

			if (Show == true)
			{
				App->CL_Scene->B_Area[Count]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
			}
			else
			{
				App->CL_Scene->B_Area[Count]->Phys_Body->setCollisionFlags(f | (1 << 5));
			}
		}

		Count++;
	}

	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
	App->CL_Ogre->RenderFrame(1);
	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;
}