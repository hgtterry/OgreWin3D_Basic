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
#include "CL64_Com_Messages.h"

CL64_Com_Messages::CL64_Com_Messages(void)
{
}

CL64_Com_Messages::~CL64_Com_Messages(void)
{
}

// *************************************************************************
// *		Set_Message_Defaults:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Com_Messages::Set_Message_Defaults(int Index)
{

	Base_Object* V_Object = App->CL_Scene->V_Object[Index];

	V_Object->S_Message[0]->Trigger_Value = 0;
	V_Object->S_Message[0]->Counter_ID = 0;
	strcpy(V_Object->S_Message[0]->Counter_Name, "None");
	V_Object->S_Message[0]->Counter_Disabled = 1;

	strcpy(V_Object->S_Message[0]->Message_Text, "Welcome");
	V_Object->S_Message[0]->Message_PosX = 250;
	V_Object->S_Message[0]->Message_PosY = 10;

	V_Object->S_Message[0]->PosXCentre_Flag = 0;
	V_Object->S_Message[0]->PosYCentre_Flag = 0;

	V_Object->S_Message[0]->Text_Colour = Ogre::Vector4(0, 0, 0, 255);
	V_Object->S_Message[0]->BackGround_Colour = Ogre::Vector4(239, 239, 239, 255);

	V_Object->S_Message[0]->Show_BackGround = 1;
	return;
}

// **************************************************************************
// *		Create_Message_Entity:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
bool CL64_Com_Messages::Create_Message_Entity(int Index)
{
	char Mesh_File[255];
	char ConNum[256];
	char Ogre_Name[256];

	Base_Object* MObject = App->CL_Scene->V_Object[Index];

	// ----------------- Mesh

	strcpy_s(Ogre_Name, "GDEnt_");
	_itoa(Index, ConNum, 10);
	strcat(Ogre_Name, ConNum);

	strcpy(Mesh_File, MObject->Mesh_FileName);

	MObject->Object_Ent = App->CL_Ogre->mSceneMgr->createEntity(Ogre_Name, Mesh_File, App->CL_Ogre->App_Resource_Group);
	MObject->Object_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	MObject->Object_Node->attachObject(MObject->Object_Ent);

	MObject->Object_Node->setVisible(true);

	MObject->Object_Node->setOrientation(MObject->Mesh_Quat);
	MObject->Object_Node->setPosition(MObject->Mesh_Pos);

	strcpy(MObject->Material_File, "Internal");
	

	// ----------------- Physics

	Ogre::Vector3 Centre = App->CL_Scene->V_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	MObject->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 0, 1));

	btScalar mass;
	mass = 0.0f;

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);
	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Object->GetMesh_BB_Size(MObject->Object_Node);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	MObject->Physics_Size = Ogre::Vector3(sx, sy, sz);

	btCollisionShape* newRigidShape = new btBoxShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CL_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	MObject->Phys_Body = new btRigidBody(rbInfo);
	MObject->Phys_Body->setRestitution(1.0);
	MObject->Phys_Body->setFriction(1.5);
	MObject->Phys_Body->setUserPointer(MObject->Object_Node);
	MObject->Phys_Body->setWorldTransform(startTransform);

	MObject->Usage = Enums::Stage_Usage_Message;

	MObject->Phys_Body->setUserIndex(Enums::Stage_Usage_Message);
	MObject->Phys_Body->setUserIndex2(Index);

	int f = MObject->Phys_Body->getCollisionFlags();

	MObject->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT
		| btCollisionObject::CF_KINEMATIC_OBJECT
		| btCollisionObject::CF_NO_CONTACT_RESPONSE);


	App->CL_Bullet->dynamicsWorld->addRigidBody(MObject->Phys_Body);

	App->CL_Scene->V_Object[Index]->Physics_Valid = 1;

	App->CL_Physics->Set_Physics_New(Index);

	return 1;
}

// *************************************************************************
// *		Rename_Message_Entity:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Com_Messages::Rename_Message_Entity(int Index)
{
	strcpy(App->CL_Dialogs->btext, "Change Entity Name");
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
