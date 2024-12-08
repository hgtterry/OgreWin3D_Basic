/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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
#include "CL64_Com_Area.h"

CL64_Com_Area::CL64_Com_Area()
{
}

CL64_Com_Area::~CL64_Com_Area()
{
}

// *************************************************************************
// *	  		Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Com_Area::Reset_Class(void)
{
	int Count = 0;

	while (Count < App->CL_Scene->Area_Count) // Remove Ogre Objects
	{
		if (App->CL_Scene->B_Area[Count])
		{
			if (App->CL_Scene->B_Area[Count]->Area_Ent && App->CL_Scene->B_Area[Count]->Area_Node)
			{
				App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Scene->B_Area[Count]->Area_Node);
				App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Scene->B_Area[Count]->Area_Ent);
				App->CL_Scene->B_Area[Count]->Area_Ent = nullptr;
				App->CL_Scene->B_Area[Count]->Area_Node = nullptr;
			}

			delete App->CL_Scene->B_Area[Count];
			App->CL_Scene->B_Area[Count] = nullptr;
		}

		Count++;
	}

	App->CL_Scene->Area_Count = 0;
	App->CL_Scene->flag_Area_Added = 0;

}

// *************************************************************************
// *		Set_Area_Defaults:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_Com_Area::Set_Area_Defaults(int Index)
{
	// Actor Scale ------------------------------------------------------
	App->CL_Scene->B_Area[Index]->Mesh_Scale.x = 1;
	App->CL_Scene->B_Area[Index]->Mesh_Scale.y = 1;
	App->CL_Scene->B_Area[Index]->Mesh_Scale.z = 1;

	// Actor Pos
	App->CL_Scene->B_Area[Index]->Mesh_Pos.x = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Pos.y = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Pos.z = 0;

	// Mesh Rotation
	App->CL_Scene->B_Area[Index]->Mesh_Rot.x = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Rot.y = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Rot.z = 0;

	// Mesh Quat
	App->CL_Scene->B_Area[Index]->Mesh_Quat.w = 1;
	App->CL_Scene->B_Area[Index]->Mesh_Quat.x = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Quat.y = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Quat.z = 0;

	// Physics Scale ------------------------------------------------------
	App->CL_Scene->B_Area[Index]->Physics_Scale.x = 1;
	App->CL_Scene->B_Area[Index]->Physics_Scale.y = 1;
	App->CL_Scene->B_Area[Index]->Physics_Scale.z = 1;

	// Physics Pos
	App->CL_Scene->B_Area[Index]->Physics_Pos.x = 0;
	App->CL_Scene->B_Area[Index]->Physics_Pos.y = 0;
	App->CL_Scene->B_Area[Index]->Physics_Pos.z = 0;

	// Physics Rotation
	App->CL_Scene->B_Area[Index]->Physics_Rot.x = 0;
	App->CL_Scene->B_Area[Index]->Physics_Rot.y = 0;
	App->CL_Scene->B_Area[Index]->Physics_Rot.z = 0;

	// Physics Quat
	App->CL_Scene->B_Area[Index]->Physics_Quat.w = 1;
	App->CL_Scene->B_Area[Index]->Physics_Quat.x = 0;
	App->CL_Scene->B_Area[Index]->Physics_Quat.y = 0;
	App->CL_Scene->B_Area[Index]->Physics_Quat.z = 0;
	App->CL_Scene->B_Area[Index]->Physics_Valid = 0;
	
}

// *************************************************************************
//				Add_New_Area:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Com_Area::Add_New_Area()
{
	int Index = App->CL_Scene->Area_Count;

	App->CL_Scene->B_Area[Index] = new Base_Area();
	Set_Area_Defaults(Index);

	Base_Area* Area = App->CL_Scene->B_Area[Index];

	Area->This_Object_UniqueID = App->CL_Scene->UniqueID_Area_Count;

	Ogre::Vector3 Pos = App->CL_Com_Objects->GetPlacement(-50);
	Area->Mesh_Pos = Pos;

	Add_Aera_To_Project(Index, App->CL_MeshViewer->Selected_MeshFile, App->CL_MeshViewer->m_Resource_Folder_Full);

	HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Areas_Folder, Area->Area_Name, Index, true);
	Area->FileViewItem = Temp;

	App->CL_FileView->SelectItem(Area->FileViewItem);

	App->CL_Scene->UniqueID_Area_Count++;
	App->CL_Scene->Area_Count++;

	App->CL_Scene->flag_Scene_Modified = 1;

	return 1;
}

// *************************************************************************
// *		Add_Aera_To_Project:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Com_Area::Add_Aera_To_Project(int Index, char* FileName, char* Resource_Location)
{
	char B_Name[MAX_PATH];
	char ConNum[MAX_PATH];

	char Mesh_File[MAX_PATH];
	strcpy(Mesh_File, FileName);

	Base_Area* Area = App->CL_Scene->B_Area[Index];

	strcpy_s(B_Name, "Area_");
	_itoa(Index, ConNum, 10);
	strcat(B_Name, ConNum);
	strcpy(App->CL_Scene->B_Area[Index]->Area_Name, B_Name);

	strcpy(Area->Area_FileName, FileName);
	strcpy(Area->Area_Resource_Path, Resource_Location);

	Area->Area_Ent = App->CL_Ogre->mSceneMgr->createEntity(App->CL_Scene->B_Area[Index]->Area_Name, Mesh_File, App->CL_Resources->Project_Resource_Group);
	Area->Area_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Area->Area_Node->attachObject(Area->Area_Ent);

	// Get Material Name
	Ogre::String text = Area->Area_Ent->getMesh()->getSubMesh(0)->getMaterialName().c_str();
	Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));
	strcpy(Area->Material_File, Mat->getOrigin().c_str());

	Area->Area_Node->setVisible(true);
	Area->Area_Node->setPosition(Area->Mesh_Pos);
	Area->Area_Node->setScale(Area->Mesh_Scale);
	Area->Area_Node->setOrientation(Area->Mesh_Quat);

	Create_Area_Trimesh(Index, Area);

	Area->Usage = Enums::Stage_Usage_Room;

	App->CL_Scene->flag_Area_Added = 1;
}

// *************************************************************************
// *		Create_Aera_Trimesh:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
btBvhTriangleMeshShape* CL64_Com_Area::Create_Area_Trimesh(int Index, Base_Area* Object)
{
#pragma warning(disable : 4996) // Nightmare why
	// Get the mesh from the entity
	Ogre::MeshPtr myMesh = Object->Area_Ent->getMesh();
	Ogre::Mesh::SubMeshIterator SubMeshIter = myMesh->getSubMeshIterator();

	// Create the triangle mesh
	btTriangleMesh* triMesh = NULL;
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
	btBvhTriangleMeshShape* mShape = new btBvhTriangleMeshShape(triMesh, false, true);
	//mShape->buildOptimizedBvh();

	float x = Object->Area_Node->getPosition().x;
	float y = Object->Area_Node->getPosition().y;
	float z = Object->Area_Node->getPosition().z;

	Object->Physics_Pos = Ogre::Vector3(x, y, z);
	Object->Physics_Rot = Ogre::Vector3(0, 0, 0);
	Object->Physics_Quat = Ogre::Quaternion(1, 0, 0, 0);

	btVector3 inertia(0, 0, 0);
	mShape->calculateLocalInertia(0.0, inertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
	btVector3 initialPosition(x, y, z);
	startTransform.setOrigin(initialPosition);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	//myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI
	(
		0,				// mass
		myMotionState,	// initial position
		mShape,			// collision shape of body
		inertia			// local inertia
	);

	Object->Phys_Body = new btRigidBody(rigidBodyCI);
	Object->Phys_Body->clearForces();
	Object->Phys_Body->setLinearVelocity(btVector3(0, 0, 0));
	Object->Phys_Body->setAngularVelocity(btVector3(0, 0, 0));
	Object->Phys_Body->setWorldTransform(startTransform);

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	Object->Type = Enums::Bullet_Type_Static;
	Object->Shape = Enums::Shape_TriMesh;


	Object->Phys_Body->setUserIndex(123);
	Object->Phys_Body->setUserIndex2(Index);

	Object->Collect_Object_Data();

	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	Set_Physics(Index);

	Object->Physics_Valid = 1;
	return mShape;
}

// *************************************************************************
//				Set_Physics:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Com_Area::Set_Physics(int Index)
{
	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	// ----------- Position
	x = App->CL_Scene->B_Area[Index]->Mesh_Pos.x;
	y = App->CL_Scene->B_Area[Index]->Mesh_Pos.y;
	z = App->CL_Scene->B_Area[Index]->Mesh_Pos.z;

	App->CL_Scene->B_Area[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(x, y, z));
	App->CL_Scene->B_Area[Index]->Physics_Pos = Ogre::Vector3(x, y, z);

	// ----------- Rotation
	App->CL_Scene->B_Area[Index]->Physics_Quat = App->CL_Scene->B_Area[Index]->Area_Node->getOrientation();

	w = App->CL_Scene->B_Area[Index]->Physics_Quat.w;
	x = App->CL_Scene->B_Area[Index]->Physics_Quat.x;
	y = App->CL_Scene->B_Area[Index]->Physics_Quat.y;
	z = App->CL_Scene->B_Area[Index]->Physics_Quat.z;

	App->CL_Scene->B_Area[Index]->Phys_Body->getWorldTransform().setRotation(btQuaternion(x, y, z, w));

	// ----------- Scale
	Ogre::Vector3 Scale = App->CL_Scene->B_Area[Index]->Area_Node->getScale();
	App->CL_Scene->B_Area[Index]->Phys_Body->getCollisionShape()->setLocalScaling(btVector3(Scale.x, Scale.y, Scale.z));

	App->CL_Scene->B_Area[Index]->Physics_Valid = 1;
}

// *************************************************************************
// *			Rename_Area:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Com_Area::Rename_Area(int Index)
{
	Base_Area* Area = App->CL_Scene->B_Area[Index];

	strcpy(App->CL_Dialogs->btext, "Change Area Name");
	strcpy(App->CL_Dialogs->Chr_Text, Area->Area_Name);

	App->CL_Dialogs->Dialog_Text(1);

	if (App->CL_Dialogs->Canceled == 1)
	{
		return;
	}

	// Needs Duplicate Name test 
	strcpy(Area->Area_Name, App->CL_Dialogs->Chr_Text);

	Area->Altered = 1;
	App->CL_Scene->flag_Scene_Modified = 1;
	App->CL_FileView->Mark_Altered(Area->FileViewItem);

	App->CL_FileView->Change_Item_Name(Area->FileViewItem, Area->Area_Name);

}
