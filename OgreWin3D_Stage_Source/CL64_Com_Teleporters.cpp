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
#include "CL64_Com_Teleporters.h"

CL64_Com_Teleporters::CL64_Com_Teleporters()
{
}

CL64_Com_Teleporters::~CL64_Com_Teleporters()
{
}

// *************************************************************************
// *	  Set_Teleports_Defaults:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Com_Teleporters::Set_Teleports_Defaults(int Index)
{
	Ogre::Vector4 V4 = Ogre::Vector4::ZERO;

	Base_Object* V_Object = App->CL_Scene->V_Object[Index];

	V_Object->S_Teleport[0]->Location_ID = 0;

	strcpy(V_Object->S_Teleport[0]->Name, "Start_Location");

	/*strcpy(V_Object->S_Teleport[0]->Sound_File, "magicspell.ogg");
	V_Object->S_Teleport[0]->SndVolume = 0.5;
	V_Object->S_Teleport[0]->Play = 1;*/

	V4.x = App->CL_Scene->B_Player[0]->StartPos.x;
	V4.y = App->CL_Scene->B_Player[0]->StartPos.y;
	V4.z = App->CL_Scene->B_Player[0]->StartPos.z;

	V_Object->S_Teleport[0]->Physics_Position = btVector3(V4.x, V4.y, V4.z);
	V_Object->S_Teleport[0]->Physics_Rotation = App->CL_Scene->B_Player[0]->Physics_Rotation;

	V_Object->S_Teleport[0]->Trigger_Value = 0;
	V_Object->S_Teleport[0]->Counter_ID = 0;
	strcpy(V_Object->S_Teleport[0]->Counter_Name, "None");
	V_Object->S_Teleport[0]->Counter_Disabled = 1;
}

// *************************************************************************
// *		Create_Teleport_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Com_Teleporters::Create_Teleport_Entity(int Index)
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

	Ogre::Vector3 Size = App->CL_Object->GetMesh_BB_Size(Object->Object_Node);
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

	Object->Usage = Enums::Stage_Usage_Teleport;
	Object->Phys_Body->setUserIndex(Enums::Stage_Usage_Teleport);
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
// *	  Rename_Teleport_Entity:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Com_Teleporters::Rename_Teleport_Entity(int Index)
{
	strcpy(App->CL_Dialogs->btext, "Change Teleport Name");
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
