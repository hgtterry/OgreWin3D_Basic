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
#include "CL64_Objects_Create.h"

CL64_Objects_Create::CL64_Objects_Create(void)
{
}

CL64_Objects_Create::~CL64_Objects_Create(void)
{
}

// *************************************************************************
//		Add_Objects_From_MeshViewer:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Objects_Create::Add_Objects_From_MeshViewer()
{

	if (App->CL_MeshViewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Collectables) // Collectables
	{
		App->CL_Com_Collectables->Add_New_Collectable();
		return;
	}


	int Index = App->CL_Scene->Object_Count;

	App->CL_Scene->B_Object[Index] = new Base_Object();

	Base_Object* Object = App->CL_Scene->B_Object[Index];
	Object->This_Object_UniqueID = App->CL_Scene->UniqueID_Object_Counter; // Unique ID


	strcpy(Object->Object_Name, App->CL_MeshViewer->Object_Name);
	strcpy(Object->Mesh_FileName, App->CL_MeshViewer->Selected_MeshFile);
	//strcpy(Object->Mesh_Resource_Path, m_ResourcePath);
	//strcpy(Object->Material_File, App->CL_MeshViewer->m_Material_File);

	Object->Type = App->CL_MeshViewer->Physics_Type;
	Object->Shape = App->CL_MeshViewer->Physics_Shape;


	App->CL_Objects_Create->Dispatch_MeshViewer();

	App->CL_FileView->SelectItem(App->CL_Scene->B_Object[Index]->FileViewItem);


	App->CL_Scene->UniqueID_Object_Counter++; // Unique ID
	App->CL_Scene->Object_Count++;  // Must be last line

	App->CL_Scene->flag_Scene_Modified = 1;

}

// *************************************************************************
//			Dispatch_MeshViewer:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Objects_Create::Dispatch_MeshViewer()
{
	int Index = App->CL_Scene->Object_Count;

	if (App->CL_MeshViewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area) // Area
	{
		App->CL_Com_Area->Add_Aera_To_Project(0, App->CL_MeshViewer->Selected_MeshFile, (LPSTR)App->CL_Resources->Project_Resource_Group.c_str());
		App->Say("Dispatch_MeshViewer");
	}
	else
	{
		Add_New_Object(Index, true);
		App->CL_Scene->B_Object[Index]->flag_Altered = 1;
		App->CL_Scene->B_Object[Index]->Folder = Enums::Folder_Objects;
		App->CL_Scene->B_Object[Index]->FileViewItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Objects_Folder,
		App->CL_Scene->B_Object[Index]->Object_Name, Index, true);

	}

	return 1;
}

// *************************************************************************
//				Add_New_Object:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Objects_Create::Add_New_Object(int Index, bool From_MeshViewer)
{
	char Mesh_File[255];
	char ConNum[256];
	char Ogre_Name[256];

	Base_Object* Object = App->CL_Scene->B_Object[Index];


	strcpy_s(Ogre_Name, "GDEnt_");
	_itoa(Index, ConNum, 10);
	strcat(Ogre_Name, ConNum);

	strcpy(Mesh_File, Object->Mesh_FileName);

	Object->Object_Ent = App->CL_Ogre->mSceneMgr->createEntity(Ogre_Name, Mesh_File, App->CL_Resources->Project_Resource_Group);
	

	Object->Object_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Object->Object_Node->attachObject(Object->Object_Ent);
	Object->Object_Node->setVisible(true);

	Object->Object_Node->setOrientation(Object->Mesh_Quat);

	// Get Material Name
	Ogre::String text = Object->Object_Ent->getMesh()->getSubMesh(0)->getMaterialName().c_str();
	Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));
	strcpy(Object->Material_File, Mat->getOrigin().c_str());
	Object->UsageEX = 777;

	// If from MeshViewer Get Placement Method
	if (From_MeshViewer == 1) //&& App->CL_MeshViewer->Placement_Camera == 1)
	{
		Ogre::Vector3 Pos = App->CL_Com_Objects->GetPlacement();
		Object->Mesh_Pos = Pos;
		Object->Object_Node->setPosition(Pos);
	}
	else
	{
		Object->Object_Node->setPosition(Object->Mesh_Pos);
	}


	App->CL_Scene->flag_Scene_Loaded = 1;


	//---------------------- Static
	if (Object->Type == Enums::Bullet_Type_Static)
	{
		if (Object->Shape == Enums::Shape_Box)
		{
			Add_Physics_Box(false, Index);
		}

		if (Object->Shape == Enums::Shape_Sphere)
		{
			Add_Physics_Sphere(false, Index);
		}

		if (Object->Shape == Enums::Shape_Capsule)
		{
			Add_Physics_Capsule(false, Index);
		}

		if (Object->Shape == Enums::Shape_Cylinder)
		{
			Add_Physics_Cylinder(false, Index);
		}

		if (Object->Shape == Enums::Shape_Cone)
		{
			Add_Physics_Cone(false, Index);
		}
	}

	//---------------------- Dynamic
	if (Object->Type == Enums::Bullet_Type_Dynamic)
	{
		if (Object->Shape == Enums::Shape_Box)
		{
			Add_Physics_Box(true, Index);
		}

		if (Object->Shape == Enums::Shape_Sphere)
		{
			Add_Physics_Sphere(true, Index);
		}

		if (Object->Shape == Enums::Shape_Capsule)
		{
			Add_Physics_Capsule(true, Index);
		}

		if (Object->Shape == Enums::Shape_Cylinder)
		{
			Add_Physics_Cylinder(true, Index);
		}

		if (Object->Shape == Enums::Shape_Cone)
		{
			Add_Physics_Cone(true, Index);
		}
	}

	//---------------------- Tri_Mesh
	if (Object->Type == Enums::Bullet_Type_TriMesh)
	{
		create_New_Trimesh(Index);
	}


	//if (Object->Usage == Enums::Usage_Room) // Rooms
	{
		//App->SBC_Scene->Area_Added = 1;
	}

	ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);

	if (From_MeshViewer == 1) //&& App->CL_MeshViewer->Placement_Camera == 1)
	{
		int Count = 0;
		int TexureCount = 0;
		//char TexureName(MAX_PATH);
		Ogre::String TexureName;

		Ogre::MaterialPtr MatCurent;

		TexureCount = Object->Object_Ent->getMesh()->getNumSubMeshes();

		while (Count < TexureCount)
		{
			Ogre::String text = Object->Object_Ent->getMesh()->getSubMesh(Count)->getMaterialName().c_str();
			MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));
			
			TexureName = MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str();
			//App->Say(TexureName.c_str());
			
			MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(TexureName);
			MatCurent->getTechnique(0)->getPass(0)->setAmbient(1, 1, 1);

			Count++;
		}

	}

	return 1;
}

// *************************************************************************
//			Add_Objects_From_File:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Objects_Create::Add_Objects_From_File() // From File
{
	int Object_Count = App->CL_Scene->Object_Count;
	int Count = 0;

	while (Count < Object_Count)
	{
		char Test_For_Mesh[MAX_PATH];
		strcpy(Test_For_Mesh, App->CL_Project->m_Main_Assets_Path);
		strcat(Test_For_Mesh, App->CL_Scene->B_Object[Count]->Mesh_FileName);
		
		if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Sound)
		{
			App->CL_Com_Sounds->Create_Sound_Entity(Count);

			HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Sounds_Folder, App->CL_Scene->B_Object[Count]->Object_Name, Count, false);
			App->CL_Scene->B_Object[Count]->FileViewItem = Temp;

			App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Sounds_Folder);
		}
		else if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Message)
		{
			App->CL_Com_Messages->Create_Message_Entity(Count);
			App->CL_Scene->B_Object[Count]->Set_ImGui_Panel_Name();

			HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Message_Trigger_Folder, App->CL_Scene->B_Object[Count]->Object_Name, Count, false);
			App->CL_Scene->B_Object[Count]->FileViewItem = Temp;

			App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Message_Trigger_Folder);
		}
		else if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Move)
		{
			App->CL_Com_MoveEntity->Create_Move_Entity(Count);

			HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Move_Folder, App->CL_Scene->B_Object[Count]->Object_Name, Count, false);
			App->CL_Scene->B_Object[Count]->FileViewItem = Temp;

			App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Move_Folder);

		}
		else if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Teleport)
		{
			App->CL_Com_Teleporters->Create_Teleport_Entity(Count);

			HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Teleporters_Folder, App->CL_Scene->B_Object[Count]->Object_Name, Count, false);
			App->CL_Scene->B_Object[Count]->FileViewItem = Temp;

			App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Teleporters_Folder);

		}
		else if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Colectable)
		{
			bool Test = App->CL_File_IO->Check_File_Exist(Test_For_Mesh);
			if (Test == 0)
			{
				App->Say("Can not find", App->CL_Scene->B_Object[Count]->Mesh_FileName);
			}
			else
			{
				App->CL_Com_Collectables->Create_Collectable_Entity(Count);

				HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Collectables_Folder, App->CL_Scene->B_Object[Count]->Object_Name, Count, false);
				App->CL_Scene->B_Object[Count]->FileViewItem = Temp;

				App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Collectables_Folder);
				App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_EntitiesFolder);
			}

		}
		else if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_EnvironEntity)
		{
			App->CL_Com_Environments->Create_Environ_Entity(Count);

			HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Evirons_Folder, App->CL_Scene->B_Object[Count]->Object_Name, Count, false);
			App->CL_Scene->B_Object[Count]->FileViewItem = Temp;

			App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Evirons_Folder);

		} // -------------------------------------------------------------------------- Particles
		else if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Particle)
		{
			App->CL_Com_Particles->Create_Particle_Entity(Count);

			HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Particles_Folder, App->CL_Scene->B_Object[Count]->Object_Name, Count, false);
			App->CL_Scene->B_Object[Count]->FileViewItem = Temp;

			App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Particles_Folder);

		}
		else
		{
			bool Test = App->CL_File_IO->Check_File_Exist(Test_For_Mesh);
			if (Test == 0)
			{
				App->Say("Can not find", App->CL_Scene->B_Object[Count]->Mesh_FileName);	
			}
			else
			{
				App->CL_Objects_Create->Add_New_Object(Count, 0);
				App->CL_Scene->B_Object[Count]->flag_Altered = 0;
				App->CL_Scene->B_Object[Count]->Folder = Enums::Folder_Objects;
				App->CL_Scene->B_Object[Count]->FileViewItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Objects_Folder, App->CL_Scene->B_Object[Count]->Object_Name, Count, false);

				if (Object_Count > 0)
				{
					App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Objects_Folder);
					ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
					App->CL_FileView->SelectItem(App->CL_Scene->B_Object[0]->FileViewItem);
				}
			}
		}

		Count++;
	}

	return 1;
}

// *************************************************************************
//				Add_Physics_Box:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Objects_Create::Add_Physics_Box(bool Dynamic, int Index)
{

	Base_Object* Object = App->CL_Scene->B_Object[Index];

	if (Dynamic == 1)
	{
		Object->Type = Enums::Bullet_Type_Dynamic;
		Object->Shape = Enums::Shape_Box;

	}
	else
	{
		Object->Type = Enums::Bullet_Type_Static;
		Object->Shape = Enums::Shape_Box;
	}

	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 0, 1));

	btScalar mass;
	if (Dynamic == 1)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 0.0f;
	}


	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);
	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Object->Object_Node);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	Object->Physics_Size = Ogre::Vector3(sx, sy, sz);

	btCollisionShape* newRigidShape = new btBoxShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CL_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Object->Phys_Body = new btRigidBody(rbInfo);
	Object->Phys_Body->setRestitution(1.0);
	Object->Phys_Body->setFriction(1.5);
	Object->Phys_Body->setUserPointer(Object->Object_Node);
	Object->Phys_Body->setWorldTransform(startTransform);

	if (Dynamic == 1)
	{
		Object->Usage = Enums::Stage_Usage_Dynamic;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Dynamic);
		Object->Phys_Body->setUserIndex2(Index);
	}
	else
	{
		Object->Usage = Enums::Stage_Usage_Static;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Static);
		Object->Phys_Body->setUserIndex2(Index);
	}

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Scene->B_Object[Index]->flag_Physics_Valid = 1;

	App->CL_Physics->Set_Physics_New(Index);

}

// *************************************************************************
//			Add_Physics_Sphere:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Objects_Create::Add_Physics_Sphere(bool Dynamic, int Index)
{
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	if (Dynamic == 1)
	{
		Object->Type = Enums::Bullet_Type_Dynamic;
		Object->Shape = Enums::Shape_Sphere;

	}
	else
	{
		Object->Type = Enums::Bullet_Type_Static;
		Object->Shape = Enums::Shape_Sphere;
	}

	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;

	if (Dynamic == 1)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 0.0f;
	}


	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	float Radius = App->CL_Com_Objects->GetMesh_BB_Radius(Object->Object_Node);
	Object->Physics_Size = Ogre::Vector3(Radius, 0, 0);

	btCollisionShape* newRigidShape = new btSphereShape(Radius);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CL_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Object->Phys_Body = new btRigidBody(rbInfo);
	Object->Phys_Body->setRestitution(1.0);
	Object->Phys_Body->setFriction(1.5);
	Object->Phys_Body->setUserPointer(Object->Object_Node);
	Object->Phys_Body->setWorldTransform(startTransform);

	if (Dynamic == 1)
	{
		Object->Usage = Enums::Stage_Usage_Dynamic;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Dynamic);
		Object->Phys_Body->setUserIndex2(Index);
	}
	else
	{
		Object->Usage = Enums::Stage_Usage_Static;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Static);
		Object->Phys_Body->setUserIndex2(Index);
	}

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Scene->B_Object[Index]->flag_Physics_Valid = 1;

	App->CL_Physics->Set_Physics_New(Index);
}

// *************************************************************************
//			Add_Physics_Capsule:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void CL64_Objects_Create::Add_Physics_Capsule(bool Dynamic, int Index)
{
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	if (Dynamic == 1)
	{
		Object->Type = Enums::Bullet_Type_Dynamic;
		Object->Shape = Enums::Shape_Capsule;

	}
	else
	{
		Object->Type = Enums::Bullet_Type_Static;
		Object->Shape = Enums::Shape_Capsule;
	}

	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	if (Dynamic == 1)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 0.0f;
	}

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Object->Object_Node);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	float Radius = App->CL_Com_Objects->GetMesh_BB_Radius(Object->Object_Node);
	Object->Physics_Size = Ogre::Vector3(Radius, sy, 0);

	btCollisionShape* newRigidShape = new btCapsuleShape(Radius, sy);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CL_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Object->Phys_Body = new btRigidBody(rbInfo);
	Object->Phys_Body->setRestitution(1.0);
	Object->Phys_Body->setFriction(1.5);
	Object->Phys_Body->setUserPointer(Object->Object_Node);
	Object->Phys_Body->setWorldTransform(startTransform);

	if (Dynamic == 1)
	{
		Object->Usage = Enums::Stage_Usage_Dynamic;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Dynamic);
		Object->Phys_Body->setUserIndex2(Index);
	}
	else
	{
		Object->Usage = Enums::Stage_Usage_Static;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Static);
		Object->Phys_Body->setUserIndex2(Index);
	}

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Scene->B_Object[Index]->flag_Physics_Valid = 1;

	App->CL_Physics->Set_Physics_New(Index);
}

// *************************************************************************
//						Add_Physics_Cylinder Terry Flanigan				   *
// *************************************************************************
void CL64_Objects_Create::Add_Physics_Cylinder(bool Dynamic, int Index)
{
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	if (Dynamic == 1)
	{
		Object->Type = Enums::Bullet_Type_Dynamic;
		Object->Shape = Enums::Shape_Cylinder;
	}
	else
	{
		Object->Type = Enums::Bullet_Type_Static;
		Object->Shape = Enums::Shape_Cylinder;
	}

	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	if (Dynamic == 1)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 0.0f;
	}

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Object->Object_Node);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	float Radius = App->CL_Com_Objects->GetMesh_BB_Radius(Object->Object_Node);
	Object->Physics_Size = Ogre::Vector3(sx, sy, sz);

	btCollisionShape* newRigidShape = new btCylinderShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CL_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Object->Phys_Body = new btRigidBody(rbInfo);
	Object->Phys_Body->setRestitution(1.0);
	Object->Phys_Body->setFriction(1.5);
	Object->Phys_Body->setUserPointer(Object->Object_Node);
	Object->Phys_Body->setWorldTransform(startTransform);

	if (Dynamic == 1)
	{
		Object->Usage = Enums::Stage_Usage_Dynamic;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Dynamic);
		Object->Phys_Body->setUserIndex2(Index);
	}
	else
	{
		Object->Usage = Enums::Stage_Usage_Static;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Static);
		Object->Phys_Body->setUserIndex2(Index);
	}

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Scene->B_Object[Index]->flag_Physics_Valid = 1;

	App->CL_Physics->Set_Physics_New(Index);
}

// *************************************************************************
//					Add_PhysicsNew__Static_Cone Terry Bernie			   *
// *************************************************************************
void CL64_Objects_Create::Add_Physics_Cone(bool Dynamic, int Index)
{
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	if (Dynamic == 1)
	{
		Object->Type = Enums::Bullet_Type_Dynamic;
		Object->Shape = Enums::Shape_Cone;
	}
	else
	{
		Object->Type = Enums::Bullet_Type_Static;
		Object->Shape = Enums::Shape_Cone;
	}

	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	if (Dynamic == 1)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 0.0f;
	}

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Object->Object_Node);
	float sx = Size.x / 2;
	float sy = Size.y;// / 2;
	float sz = Size.z / 2;

	float Radius = App->CL_Com_Objects->GetMesh_BB_Radius(Object->Object_Node);
	Object->Physics_Size = Ogre::Vector3(Radius, sy, 0);

	btCollisionShape* newRigidShape = new btConeShape(Radius, sy);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CL_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Object->Phys_Body = new btRigidBody(rbInfo);
	Object->Phys_Body->setRestitution(1.0);
	Object->Phys_Body->setFriction(1.5);
	Object->Phys_Body->setUserPointer(Object->Object_Node);
	Object->Phys_Body->setWorldTransform(startTransform);

	if (Dynamic == 1)
	{
		Object->Usage = Enums::Stage_Usage_Dynamic;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Dynamic);
		Object->Phys_Body->setUserIndex2(Index);
	}
	else
	{
		Object->Usage = Enums::Stage_Usage_Static;
		Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Static);
		Object->Phys_Body->setUserIndex2(Index);
	}

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Scene->B_Object[Index]->flag_Physics_Valid = 1;

	App->CL_Physics->Set_Physics_New(Index);
}

// *************************************************************************
//			Create_New_Trimesh:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
btBvhTriangleMeshShape* CL64_Objects_Create::create_New_Trimesh(int Index)
{
#pragma warning(disable : 4996) // Nightmare why
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	// Get the mesh from the entity
	Ogre::MeshPtr myMesh = Object->Object_Ent->getMesh();
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

		//App->Say("here");
	}

	const bool useQuantizedAABB = true;
	btBvhTriangleMeshShape* mShape = new btBvhTriangleMeshShape(triMesh, false, true);
	//mShape->buildOptimizedBvh();

	float x = Object->Object_Node->getPosition().x;
	float y = Object->Object_Node->getPosition().y;
	float z = Object->Object_Node->getPosition().z;

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


	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI
	(
		0,  // mass
		myMotionState,// initial position
		mShape,      // collision shape of body
		inertia   // local inertia
	);

	Object->Phys_Body = new btRigidBody(rigidBodyCI);
	Object->Phys_Body->clearForces();
	Object->Phys_Body->setLinearVelocity(btVector3(0, 0, 0));
	Object->Phys_Body->setAngularVelocity(btVector3(0, 0, 0));
	Object->Phys_Body->setWorldTransform(startTransform);

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	Object->Type = Enums::Bullet_Type_TriMesh;
	Object->Shape = Enums::Shape_TriMesh;


	Object->Phys_Body->setUserIndex(123);
	Object->Phys_Body->setUserIndex2(Index);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	Object->flag_Physics_Valid = 1;

	App->CL_Physics->Set_Physics_New(Index);

	return mShape;
}
