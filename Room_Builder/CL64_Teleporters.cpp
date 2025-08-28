/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D Scene Builder

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
#include "CL64_Teleporters.h"

CL64_Teleporters::CL64_Teleporters(void)
{
	Teleporter_Count = 0;
	UniqueID_Teleporter_Counter = 0;
}

CL64_Teleporters::~CL64_Teleporters(void)
{
}

// *************************************************************************
// *		  Set_Objects_Defaults:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Teleporters::Set_Objects_Defaults(int index)
{
	// Initialize Pointers
	auto& m_Object = App->CL_Scene->B_Object[index];

	m_Object->flag_HasSound = false;
	strcpy(m_Object->Sound_File, "None");
	m_Object->Sound_Path[0] = 0;
	m_Object->flag_Play_Sound = false;
	m_Object->SndVolume = 0.5; // Default Half Volume
	
}

// *************************************************************************
// *	  Set_Teleports_Defaults:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Teleporters::Set_Teleports_Defaults(int index)
{
	// Initialize Pointers
	auto& m_Object = App->CL_Scene->B_Object[index];
	auto& m_Teleport = m_Object->S_Teleport[0];

	m_Teleport->Location_ID = 0;
	strcpy(m_Teleport->Location_Name, "Start_Location");

	// Set the physics position and rotation based on the player's start position
	const auto& playerStartPos = App->CL_Scene->B_Player[0]->StartPos;
	m_Teleport->Physics_Position = btVector3(playerStartPos.x, playerStartPos.y, playerStartPos.z);
	m_Teleport->Physics_Rotation = App->CL_Scene->B_Player[0]->Physics_Quat;

	// Initialize trigger sound and counter values
	m_Teleport->SndFile = nullptr;
	m_Teleport->Trigger_Value = 0;
	m_Teleport->Counter_ID = 0;
	strcpy(m_Teleport->Counter_Name, "None");
	strcpy(m_Teleport->Sound_File, "None");
	m_Teleport->flag_Counter_Disabled = true;
	m_Teleport->SndVolume = 0.5;
}

// *************************************************************************
// *		Add_New_Teleporter:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Teleporters::Add_New_Teleporter()
{
	const int index = App->CL_Scene->Object_Count;

	// Create a new Base_Object and initialize its properties
	auto* newObject = new Base_Object();
	App->CL_Scene->B_Object[index] = newObject;
	Set_Objects_Defaults(index);

	// Initialize Teleport
	newObject->S_Teleport[0] = new Teleport_type;
	Set_Teleports_Defaults(index);
	
	// Initialize Environment
	newObject->S_Environ[0] = new Environ_type;
	App->CL_Com_Environments->V_Set_Environ_Defaults(index);
	newObject->S_Environ[0]->flag_Environ_Enabled = false;

	// Set object type and shape
	newObject->Type = Enums::Bullet_Type_Static;
	newObject->Shape = Enums::Shape_Box;
	newObject->This_Object_UniqueID = App->CL_Scene->UniqueID_Object_Counter; // Unique ID
	
	// Set mesh file name
	strcpy(newObject->Mesh_FileName, "TeleportSend.mesh");
	
	// Generate object name
	std::string objectName = "Teleport_Ent_" + std::to_string(index);
	strcpy(newObject->Object_Name, objectName.c_str());

	// Set Object position
	Ogre::Vector3 Pos = App->CL_Com_Objects->GetPlacement(-50);
	App->CL_Scene->B_Object[index]->Mesh_Pos = Pos;

	// Create teleport Entity Ogre and Physics
	Create_Teleport_Entity(index);

	// Add item to file view and select it
	HTREEITEM tempItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Teleporters_Folder, newObject->Object_Name, index, true);
	newObject->FileViewItem = tempItem;
	App->CL_FileView->SelectItem(newObject->FileViewItem);

	// Update counters
	App->CL_Scene->UniqueID_Object_Counter++;
	App->CL_Scene->Object_Count++;

	// Set active folder
	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Teleporters_Folder);

	return true;
}

// *************************************************************************
// *		Create_Teleport_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Teleporters::Create_Teleport_Entity(int Index)
{
	// Create Pointer
	auto& newObject = App->CL_Scene->B_Object[Index];
	
	// Generate unique Ogre name
	std::string Ogre_Name = "GDEnt_" + std::to_string(Index);
	std::string Mesh_File = newObject->Mesh_FileName;

	// Create entity and attach to scene node
	newObject->Object_Ent = App->CL_Ogre->mSceneMgr->createEntity(Ogre_Name, Mesh_File, App->CL_Ogre->App_Resource_Group);
	newObject->Object_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	newObject->Object_Node->attachObject(newObject->Object_Ent);
	newObject->Object_Node->setVisible(true);
	newObject->Object_Node->setOrientation(newObject->Mesh_Quat);
	newObject->Object_Node->setPosition(newObject->Mesh_Pos);
	strcpy(newObject->Material_File, "Internal");

	// Set up physics
	Ogre::Vector3 Centre = newObject->Object_Ent->getWorldBoundingBox(true).getCenter();
	newObject->Physics_Pos = Centre;

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 0, 1));

	btScalar mass = 0.0f;
	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);
	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMeshBoundingBoxSize(newObject->Object_Node);
	btVector3 halfSize(Size.x / 2, Size.y / 2, Size.z / 2);
	newObject->Physics_Size = Ogre::Vector3(halfSize.x(), halfSize.y(), halfSize.z());

	btCollisionShape* newRigidShape = new btBoxShape(halfSize);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CL_Physics->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	newObject->Phys_Body = new btRigidBody(rbInfo);
	newObject->Phys_Body->setRestitution(1.0);
	newObject->Phys_Body->setFriction(1.5);
	newObject->Phys_Body->setUserPointer(newObject->Object_Node);
	newObject->Phys_Body->setWorldTransform(startTransform);

	newObject->Usage = Enums::Obj_Usage_Teleport;
	newObject->Phys_Body->setUserIndex(Enums::Obj_Usage_Teleport);
	newObject->Phys_Body->setUserIndex2(Index);

	int f = newObject->Phys_Body->getCollisionFlags();

	newObject->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT
		| btCollisionObject::CF_KINEMATIC_OBJECT
		| btCollisionObject::CF_NO_CONTACT_RESPONSE);


	App->CL_Physics->dynamicsWorld->addRigidBody(newObject->Phys_Body);
	newObject->flag_Physics_Valid = true;
	App->CL_Physics->Update_Object_Physics(Index);

	return 1;
}

// *************************************************************************
// *	  Rename_Teleport_Entity:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Teleporters::Rename_Teleport_Entity(int Index)
{
	strcpy(App->CL_Dialogs->btext, "Change Teleport Name");
	strcpy(App->CL_Dialogs->Chr_Text, App->CL_Scene->B_Object[Index]->Object_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CL_Dialogs->flag_Dlg_Canceled == true)
	{
		return;
	}

	strcpy(App->CL_Scene->B_Object[Index]->Object_Name, App->CL_Dialogs->Chr_Text);

	//App->CL_Properties->Mark_As_Altered(Index);

	App->CL_FileView->Change_Item_Name(App->CL_Scene->B_Object[Index]->FileViewItem, App->CL_Dialogs->Chr_Text);
}
