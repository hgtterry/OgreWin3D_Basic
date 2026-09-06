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
// *				Add_New_Sound:- Terry and Hazel Flanigan 2026
// *************************************************************************
bool CL64_Com_Sounds::Add_New_Sound()
{
	int New_Object_Index = App->CL_Scene->Object_Count;

	App->CL_Scene->B_Object[New_Object_Index] = new Base_Object();

	auto& New_Sound_Object = App->CL_Scene->B_Object[New_Object_Index];  // Pointer to New Object

	// Set sound file and path
	strcpy(New_Sound_Object->Sound_File, "Welcome.ogg");
	strcpy(New_Sound_Object->Sound_Path, App->CL_SoundMgr->Default_Folder);
	strcat(New_Sound_Object->Sound_Path, "\\Media\\Sounds\\Welcome.ogg");
	
	// Set flags and types
	New_Sound_Object->flag_HasSound = true;
	New_Sound_Object->Type = Enums::Bullet_Type_Static;
	New_Sound_Object->Shape = Enums::Shape_Box;
	New_Sound_Object->This_Object_UniqueID = App->CL_Scene->UniqueID_Object_Counter; // Unique ID

	// Set mesh file name
	strcpy(New_Sound_Object->Mesh_FileName, "SoundEntity_GD.mesh");

	// Generate a unique object name
	char B_Name[MAX_PATH];
	sprintf(B_Name, "%s%i", "Sound_", New_Object_Index);
	strcpy(New_Sound_Object->Object_Name, B_Name);

	// Set the position of the sound object
	New_Sound_Object->Mesh_Pos = App->CL_Com_Objects->GetPlacement(-50);

	// Create sound entity and brush
	Create_Sound_Entity(New_Object_Index);
	App->CL_Entities->Create_Entity_Brush(New_Object_Index);
	App->CL_Brush_X->Move_Brush_By_Name((LPSTR)B_Name, New_Object_Index);

	// Add item to file view and select it
	HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Sounds_Folder, New_Sound_Object->Object_Name, New_Object_Index, true);
	New_Sound_Object->FileViewItem = Temp;
	App->CL_FileView->SelectItem(New_Sound_Object->FileViewItem);

	// Update counters and flags
	App->CL_Scene->UniqueID_Object_Counter++;
	App->CL_Scene->Object_Count++;

	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Sounds_Folder);
	App->CL_Level->flag_Level_is_Modified = true;

	return 1;
}

// *************************************************************************
// *			Create_Sound_Entity:- Terry and Hazel Flanigan 2026
// *************************************************************************
bool CL64_Com_Sounds::Create_Sound_Entity(int Index)
{
	char Mesh_File[MAX_PATH];
	char Ogre_Name[MAX_PATH];

	auto& New_Sound_Object = App->CL_Scene->B_Object[Index]; // Pointer to New Object
	
	strcpy(New_Sound_Object->Entity_Type_Name, "Sound Entity");

	// Construct Ogre name
	snprintf(Ogre_Name, sizeof(Ogre_Name), "GDEnt_%d", Index);
	strcpy(Mesh_File, New_Sound_Object->Mesh_FileName);

	// Create entity and attach to scene node
	New_Sound_Object->Object_Ent = App->CL_Ogre->mSceneMgr->createEntity(Ogre_Name, Mesh_File, App->CL_Ogre->App_Resource_Group);
	New_Sound_Object->Object_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	New_Sound_Object->Object_Node->attachObject(New_Sound_Object->Object_Ent);
	New_Sound_Object->Object_Node->setVisible(true);
	New_Sound_Object->Object_Node->setOrientation(New_Sound_Object->Mesh_Quat);
	New_Sound_Object->Object_Node->setPosition(New_Sound_Object->Mesh_Pos);
	strcpy(New_Sound_Object->Material_File, "Internal");
	
	// Physics setup
	Ogre::Vector3 Centre = New_Sound_Object->Object_Ent->getWorldBoundingBox(true).getCenter();
	New_Sound_Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 0, 1));

	btScalar mass = 0.0f;
	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);
	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMeshBoundingBoxSize(New_Sound_Object->Object_Node);
	New_Sound_Object->Physics_Size = Size * 0.5f; // Halve the size for physics
	
	// Create collision shape
	btCollisionShape* newRigidShape = new btBoxShape(btVector3(Size.x, Size.y, Size.z));
	newRigidShape->calculateLocalInertia(mass, localInertia);
	App->CL_Physics->collisionShapes.push_back(newRigidShape);

	// Create motion state and rigid body
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);
	New_Sound_Object->Phys_Body = new btRigidBody(rbInfo);
	New_Sound_Object->Phys_Body->setRestitution(1.0);
	New_Sound_Object->Phys_Body->setFriction(1.5);
	New_Sound_Object->Phys_Body->setUserPointer(New_Sound_Object->Object_Node);
	New_Sound_Object->Phys_Body->setWorldTransform(startTransform);
	New_Sound_Object->Usage = Enums::Obj_Usage_Sound;
	New_Sound_Object->Phys_Body->setUserIndex(Enums::Obj_Usage_Sound);
	New_Sound_Object->Phys_Body->setUserIndex2(Index);

	// Set collision flags
	int f = New_Sound_Object->Phys_Body->getCollisionFlags();
	New_Sound_Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT
		| btCollisionObject::CF_KINEMATIC_OBJECT
		| btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// Add rigid body to dynamics world
	App->CL_Physics->dynamicsWorld->addRigidBody(New_Sound_Object->Phys_Body);
	New_Sound_Object->flag_Physics_Valid = true;
	App->CL_Physics->Update_Object_Physics(Index);

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
