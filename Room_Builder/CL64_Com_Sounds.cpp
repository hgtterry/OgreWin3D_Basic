/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

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
#include "CL64_Com_Sounds.h"

CL64_Com_Sounds::CL64_Com_Sounds(void)
{
}

CL64_Com_Sounds::~CL64_Com_Sounds(void)
{
}

// *************************************************************************
// *				Add_New_Sound:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Com_Sounds::Add_New_Sound()
{
	char B_Name[MAX_PATH];
	char ConNum[MAX_PATH];

	int Index = App->CL_Scene->Object_Count;

	App->CL_Scene->B_Object[Index] = new Base_Object();

	// Set sound file and path
	strcpy(App->CL_Scene->B_Object[Index]->Sound_File, "Welcome.ogg");
	strcpy(App->CL_Scene->B_Object[Index]->Sound_Path, App->CL_SoundMgr->Default_Folder);
	strcat(App->CL_Scene->B_Object[Index]->Sound_Path, "\\Media\\Sounds\\Welcome.ogg");
	
	// Set flags and types
	App->CL_Scene->B_Object[Index]->flag_HasSound = true;
	App->CL_Scene->B_Object[Index]->Type = Enums::Bullet_Type_Static;
	App->CL_Scene->B_Object[Index]->Shape = Enums::Shape_Box;
	App->CL_Scene->B_Object[Index]->This_Object_UniqueID = App->CL_Scene->UniqueID_Object_Counter; // Unique ID

	// Set mesh file name
	strcpy(App->CL_Scene->B_Object[Index]->Mesh_FileName, "SoundEntity_GD.mesh");

	// Generate a unique object name
	strcpy_s(B_Name, "Sound_");
	_itoa(Index, ConNum, 10);
	strcat(B_Name, ConNum);
	strcpy(App->CL_Scene->B_Object[Index]->Object_Name, B_Name);

	// Set the position of the sound object
	App->CL_Scene->B_Object[Index]->Mesh_Pos = App->CL_Com_Objects->GetPlacement(-50);

	// Create sound entity and brush
	Create_Sound_Entity(Index);
	App->CL_Entities->Create_Entity_Brush(Index);
	App->CL_Brush_X->Move_Brush_By_Name((LPSTR)B_Name, Index);

	// Add item to file view and select it
	HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Sounds_Folder, App->CL_Scene->B_Object[Index]->Object_Name, Index, true);
	App->CL_Scene->B_Object[Index]->FileViewItem = Temp;
	App->CL_FileView->SelectItem(App->CL_Scene->B_Object[Index]->FileViewItem);

	// Update counters and flags
	App->CL_Scene->UniqueID_Object_Counter++;
	App->CL_Scene->Object_Count++;
	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Sounds_Folder);
	App->CL_Level->flag_Level_is_Modified = true;

	return 1;
}

// *************************************************************************
// *			Create_Sound_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Com_Sounds::Create_Sound_Entity(int Index)
{
	char Mesh_File[255];
	char ConNum[256];
	char Ogre_Name[256];

	Base_Object* Object = App->CL_Scene->B_Object[Index];

	// ----------------- Mesh

	strcpy_s(Ogre_Name, "GDEnt_");
	_itoa(Index, ConNum, 10);
	strcat(Ogre_Name, ConNum);

	strcpy(Mesh_File, Object->Mesh_FileName);

	Object->Object_Ent = App->CL_Ogre->mSceneMgr->createEntity(Ogre_Name, Mesh_File, App->CL_Ogre->App_Resource_Group);
	Object->Object_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Object->Object_Node->attachObject(Object->Object_Ent);

	Object->Object_Node->setVisible(true);

	Object->Object_Node->setOrientation(Object->Mesh_Quat);
	Object->Object_Node->setPosition(Object->Mesh_Pos);

	strcpy(Object->Material_File, "Internal");
	
	// ----------------- Physics

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

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMeshBoundingBoxSize(Object->Object_Node);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	Object->Physics_Size = Ogre::Vector3(sx, sy, sz);

	btCollisionShape* newRigidShape = new btBoxShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CL_Physics->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Object->Phys_Body = new btRigidBody(rbInfo);
	Object->Phys_Body->setRestitution(1.0);
	Object->Phys_Body->setFriction(1.5);
	Object->Phys_Body->setUserPointer(Object->Object_Node);
	Object->Phys_Body->setWorldTransform(startTransform);

	Object->Usage = Enums::Obj_Usage_Sound;
	Object->Phys_Body->setUserIndex(Enums::Obj_Usage_Sound);
	Object->Phys_Body->setUserIndex2(Index);


	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT
		| btCollisionObject::CF_KINEMATIC_OBJECT
		| btCollisionObject::CF_NO_CONTACT_RESPONSE);


	App->CL_Physics->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Scene->B_Object[Index]->flag_Physics_Valid = 1;

	App->CL_Physics->Set_Physics_Dimensions(Index);

	return 1;
}

// *************************************************************************
// *			Rename_Sound:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Sounds::Rename_Sound(int Index)
{
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	strcpy(App->CL_Dialogs->btext, "Change Object Name");
	strcpy(App->CL_Dialogs->Chr_Text, Object->Object_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CL_Dialogs->flag_Dlg_Canceled == 1)
	{
		return;
	}

	strcpy(Object->Object_Name, App->CL_Dialogs->Chr_Text);

	Object->flag_Altered = 1;

	App->CL_Level->flag_Level_is_Modified = true;
	//App->CL_Properties_Scene->Mark_As_Altered(Index);

	App->CL_FileView->Change_Item_Name(Object->FileViewItem, Object->Object_Name);
}
