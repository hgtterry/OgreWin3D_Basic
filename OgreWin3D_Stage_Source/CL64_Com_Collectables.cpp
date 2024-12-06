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
#include "CL64_Com_Collectables.h"

CL64_Com_Collectables::CL64_Com_Collectables(void)
{
}

CL64_Com_Collectables::~CL64_Com_Collectables(void)
{
}

// *************************************************************************
// *		Add_New_Collectable:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Com_Collectables::Add_New_Collectable()
{
	char B_Name[MAX_PATH];
	char ConNum[MAX_PATH];

	int Index = App->CL_Scene->Object_Count;

	App->CL_Scene->B_Object[Index] = new Base_Object();
	App->CL_Scene->B_Object[Index]->S_Collectable[0] = new Collectable_type;
	App->CL_Com_Collectables->Set_Collectables_Defaults(Index);

	Base_Object* Object = App->CL_Scene->B_Object[Index];
	Object->This_Object_UniqueID = App->CL_Scene->UniqueID_Object_Counter; // Unique ID

	strcpy(Object->Mesh_Name, App->CL_MeshViewer->Object_Name);
	strcpy(Object->Mesh_FileName, App->CL_MeshViewer->Selected_MeshFile);
	strcpy(Object->Mesh_Resource_Path, App->CL_MeshViewer->m_Resource_Folder_Full);
	//strcpy(Object->Material_File, App->CL_MeshViewer->);

	strcpy_s(B_Name, "Collectable_");
	_itoa(Index, ConNum, 10);
	strcat(B_Name, ConNum);
	strcpy(App->CL_Scene->B_Object[Index]->Mesh_Name, B_Name);

	Ogre::Vector3 Pos = App->CL_Com_Objects->GetPlacement(-50);
	App->CL_Scene->B_Object[Index]->Mesh_Pos = Pos;

	Object->Type = Enums::Bullet_Type_Static;
	Object->Shape = Enums::Shape_Box;


	Create_Collectable_Entity(Index);

	HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Collectables_Folder, App->CL_Scene->B_Object[Index]->Mesh_Name, Index, true);
	App->CL_Scene->B_Object[Index]->FileViewItem = Temp;

	App->CL_FileView->SelectItem(App->CL_Scene->B_Object[Index]->FileViewItem);
	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_EntitiesFolder);

	App->CL_Scene->UniqueID_Object_Counter++; // Unique ID
	App->CL_Scene->Object_Count++;  // Must be last line

	App->CL_Scene->B_Object[Index]->Altered = 1;
	App->CL_Scene->flag_Scene_Modified = 1;

	return 1;
}

// *************************************************************************
// *	Create_Collectable_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Com_Collectables::Create_Collectable_Entity(int Index)
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

	// Get Material Name
	Ogre::String text = Object->Object_Ent->getMesh()->getSubMesh(0)->getMaterialName().c_str();
	Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));
	strcpy(Object->Material_File, Mat->getOrigin().c_str());

	Object->UsageEX = 777;

	Object->Object_Node->setVisible(true);

	Object->Object_Node->setOrientation(Object->Mesh_Quat);

	Object->Object_Node->setPosition(Object->Mesh_Pos);

	App->CL_Scene->flag_Scene_Loaded = 1;

	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 0, 1));

	btScalar mass;

	mass = 0.0f;

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

	Object->Usage = Enums::Stage_Usage_Colectable;
	Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Colectable);
	Object->Phys_Body->setUserIndex2(Index);

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Physics->Set_Physics_New(Index);

	App->CL_Scene->B_Object[Index]->Folder = Enums::Folder_Collectables;

	return 1;
}

// *************************************************************************
// *		Set_Collectables_Defaults:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Com_Collectables::Set_Collectables_Defaults(int Index)
{
	Base_Object* B_Object = App->CL_Scene->B_Object[Index];

	B_Object->Altered = 0;

	strcpy(B_Object->S_Collectable[0]->Sound_File, "footstep.ogg");
	B_Object->S_Collectable[0]->Play = 1;
	B_Object->S_Collectable[0]->SndVolume = 0.5;

	B_Object->S_Collectable[0]->Maths = 1; // Add
	B_Object->S_Collectable[0]->Value = 1;

	B_Object->S_Collectable[0]->Counter_ID = 0;
	strcpy(B_Object->S_Collectable[0]->Counter_Name, "Not_Set");

	B_Object->S_Collectable[0]->Counter_Disabled = 0;
}

// *************************************************************************
// *			Rename_Collectable:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Collectables::Rename_Collectable(int Index)
{
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	strcpy(App->CL_Dialogs->btext, "Change Object Name");
	strcpy(App->CL_Dialogs->Chr_Text, Object->Mesh_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CL_Dialogs->Canceled == 1)
	{
		return;
	}

	strcpy(Object->Mesh_Name, App->CL_Dialogs->Chr_Text);

	Object->Altered = 1;

	App->CL_Scene->flag_Scene_Modified = 1;
	App->CL_FileView->Mark_Altered(Object->FileViewItem);

	App->CL_FileView->Change_Item_Name(Object->FileViewItem, Object->Mesh_Name);
}
