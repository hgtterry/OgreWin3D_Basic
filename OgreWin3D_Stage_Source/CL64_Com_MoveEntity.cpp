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
#include "CL64_Com_MoveEntity.h"

CL64_Com_MoveEntity::CL64_Com_MoveEntity(void)
{
}

CL64_Com_MoveEntity::~CL64_Com_MoveEntity(void)
{
}

// *************************************************************************
// *		Add_New_Move_Entity:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Com_MoveEntity::Add_New_Move_Entity()
{
	char B_Name[MAX_PATH];
	char ConNum[MAX_PATH];

	int Index = App->CL_Scene->Object_Count;

	App->CL_Scene->V_Object[Index] = new Base_Object();

	App->CL_Scene->V_Object[Index]->S_MoveType[0] = new Move_Type;
	Set_Move_Defaults(Index); // Check

	App->CL_Scene->V_Object[Index]->Type = Enums::Bullet_Type_Static;
	App->CL_Scene->V_Object[Index]->Shape = Enums::Shape_Box;
	App->CL_Scene->V_Object[Index]->This_Object_UniqueID = App->CL_Scene->UniqueID_Object_Counter; // Unique ID

	strcpy(App->CL_Scene->V_Object[Index]->Mesh_FileName, "DoorEntity_GD.mesh");

	strcpy_s(B_Name, "MoveEnt_");
	_itoa(Index, ConNum, 10);
	strcat(B_Name, ConNum);
	strcpy(App->CL_Scene->V_Object[Index]->Mesh_Name, B_Name);

	Ogre::Vector3 Pos = App->CL_Com_Objects->GetPlacement(-50);
	App->CL_Scene->V_Object[Index]->Mesh_Pos = Pos;

	Create_Move_Entity(Index);

	HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Move_Folder, App->CL_Scene->V_Object[Index]->Mesh_Name, Index, true);
	App->CL_Scene->V_Object[Index]->FileViewItem = Temp;

	App->CL_FileView->SelectItem(App->CL_Scene->V_Object[Index]->FileViewItem);

	App->CL_Scene->UniqueID_Object_Counter++;
	App->CL_Scene->Object_Count++;


	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Move_Folder);
	return 1;
}

// *************************************************************************
// *		Set_Move_Defaults:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_Com_MoveEntity::Set_Move_Defaults(int Index)
{
	Base_Object* V_Object = App->CL_Scene->V_Object[Index];

	V_Object->S_MoveType[0]->IsNegative = 1;
	V_Object->S_MoveType[0]->Move_Distance = -50;
	V_Object->S_MoveType[0]->Newpos = 0;
	V_Object->S_MoveType[0]->Speed = 10.0;
	V_Object->S_MoveType[0]->WhatDirection = Enums::Axis_x;
	V_Object->S_MoveType[0]->Object_To_Move_Index = 0;
	V_Object->S_MoveType[0]->Triggered = 0;
	V_Object->S_MoveType[0]->Re_Trigger = 0;
	strcpy(V_Object->S_MoveType[0]->Object_Name, "None");

	V_Object->S_MoveType[0]->Trigger_Value = 0;
	V_Object->S_MoveType[0]->Counter_ID = 0;
	strcpy(V_Object->S_MoveType[0]->Counter_Name, "None");
	V_Object->S_MoveType[0]->Counter_Disabled = 1;

	return;
}

// **************************************************************************
// *			Create_Move_Entity:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
bool CL64_Com_MoveEntity::Create_Move_Entity(int Index)
{
	char Mesh_File[255];
	char ConNum[256];
	char Ogre_Name[256];

	Base_Object* Object = App->CL_Scene->V_Object[Index];

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

	Ogre::Vector3 Centre = App->CL_Scene->V_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
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

	Object->Usage = Enums::Stage_Usage_Move;
	Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Move);
	Object->Phys_Body->setUserIndex2(Index);

	int f = Object->Phys_Body->getCollisionFlags();

	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT
		| btCollisionObject::CF_KINEMATIC_OBJECT
		| btCollisionObject::CF_NO_CONTACT_RESPONSE);


	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Scene->V_Object[Index]->Physics_Valid = 1;

	App->CL_Physics->Set_Physics_New(Index);

	return 1;
}

// *************************************************************************
// *		Rename_Move_Entity:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_Com_MoveEntity::Rename_Move_Entity(int Index)
{
	strcpy(App->CL_Dialogs->btext, "Change Object Name");
	strcpy(App->CL_Dialogs->Chr_Text, App->CL_Scene->V_Object[Index]->Mesh_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CL_Dialogs->Canceled == 1)
	{
		return;
	}

	strcpy(App->CL_Scene->V_Object[Index]->Mesh_Name, App->CL_Dialogs->Chr_Text);

	App->CL_Properties->Mark_As_Altered(Index);

	App->CL_FileView->Change_Item_Name(App->CL_Scene->V_Object[Index]->FileViewItem, App->CL_Dialogs->Chr_Text);

}

// *************************************************************************
// *		Reset_Move_Entity:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_Com_MoveEntity::Reset_Move_Entity(int Index)
{
	App->CL_Collision->DoMove = 0;
	App->CL_SoundMgr->SoundEngine->stopAllSounds();

	Ogre::Vector3 M_Pos;
	Ogre::Vector3 P_Pos;

	int ObjectToMove = App->CL_Scene->V_Object[Index]->S_MoveType[0]->Object_To_Move_Index;

	M_Pos = App->CL_Scene->V_Object[ObjectToMove]->Mesh_Pos;
	P_Pos = App->CL_Scene->V_Object[ObjectToMove]->Physics_Pos;

	App->CL_Scene->V_Object[Index]->S_MoveType[0]->MeshPos = Ogre::Vector3(M_Pos);
	App->CL_Scene->V_Object[Index]->S_MoveType[0]->PhysicsPos = Ogre::Vector3(P_Pos);

	App->CL_Scene->V_Object[ObjectToMove]->Object_Node->setPosition(M_Pos);
	App->CL_Scene->V_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(P_Pos.x, P_Pos.y, P_Pos.z));

	App->CL_Scene->V_Object[Index]->Triggered = 0;
}

// *************************************************************************
// *		Test_Move_Entity:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_Com_MoveEntity::Test_Move_Entity(int Index)
{
	App->CL_Collision->Set_Move_Entity(Index);
}

// *************************************************************************
// *		Adjust_Object_To_Move:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Com_MoveEntity::Adjust_Object_To_Move()
{
	int Count = 0;
	int Total = App->CL_Scene->Object_Count;

	while (Count < Total)
	{
		if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Move)
		{
			char MoveName[MAX_PATH];
			strcpy(MoveName, App->CL_Scene->V_Object[Count]->S_MoveType[0]->Object_Name);

			int AdjustedIndex = App->CL_Com_Objects->GetIndex_By_Name(MoveName);

			App->CL_Scene->V_Object[Count]->S_MoveType[0]->Object_To_Move_Index = AdjustedIndex;
		}

		Count++;
	}
}
