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
#include "CL64_Com_Environments.h"

CL64_Com_Environments::CL64_Com_Environments()
{
}

CL64_Com_Environments::~CL64_Com_Environments()
{
}

// *************************************************************************
// *		V_Set_Environ_Defaults:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Environments::V_Set_Environ_Defaults(int Index)
{
	Base_Object* V_Object = App->CL_Scene->V_Object[Index];

	V_Object->Altered = 0;

	V_Object->S_Environ[0]->Environment_ID = 0;
	strcpy(V_Object->S_Environ[0]->Environment_Name, "Not_Set");

	V_Object->S_Environ[0]->Environ_Enabled = 1;

	//----------------------- Sound
	strcpy(V_Object->S_Environ[0]->Sound_File, "The_Sun.ogg");
	V_Object->S_Environ[0]->SndFile = NULL;
	V_Object->S_Environ[0]->Play = 0;
	V_Object->S_Environ[0]->Loop = 1;
	V_Object->S_Environ[0]->SndVolume = 0.5;

	//----------------------- Light
	V_Object->S_Environ[0]->AmbientColour.x = 1;
	V_Object->S_Environ[0]->AmbientColour.y = 1;
	V_Object->S_Environ[0]->AmbientColour.z = 1;

	V_Object->S_Environ[0]->Light_Position.x = 0;
	V_Object->S_Environ[0]->Light_Position.y = 0;
	V_Object->S_Environ[0]->Light_Position.z = 0;

	// Sky
	V_Object->S_Environ[0]->Curvature = 15;
	V_Object->S_Environ[0]->Distance = 4000;
	V_Object->S_Environ[0]->Enabled = 0;
	strcpy(V_Object->S_Environ[0]->Material, "Examples/CloudySky");
	V_Object->S_Environ[0]->Tiling = 15;
	V_Object->S_Environ[0]->type = 1;

	// Fog
	V_Object->S_Environ[0]->Fog_On = 0;
	V_Object->S_Environ[0]->Fog_Mode = FOG_LINEAR;
	V_Object->S_Environ[0]->Fog_Density = 0.001000;
	V_Object->S_Environ[0]->Fog_Start = 50;
	V_Object->S_Environ[0]->Fog_End = 300;
	V_Object->S_Environ[0]->Fog_Colour = Ogre::Vector3(1, 1, 1);
}

// *************************************************************************
// *		Create_Environ_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Com_Environments::Create_Environ_Entity(int Index)
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

	App->CL_Scene->Scene_Loaded = 1;

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

	Object->Usage = Enums::Stage_Usage_EnvironEntity;
	Object->Phys_Body->setUserIndex(Enums::Stage_Usage_EnvironEntity);
	Object->Phys_Body->setUserIndex2(Index);

	int f = Object->Phys_Body->getCollisionFlags();

	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT
		| btCollisionObject::CF_KINEMATIC_OBJECT
		| btCollisionObject::CF_NO_CONTACT_RESPONSE);


	App->CL_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CL_Physics->Set_Physics(Index);

	return 1;
}

// *************************************************************************
// *		Set_Environment_By_Index:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
int CL64_Com_Environments::Set_Environment_By_Index(bool PlayMusic, int Index)
{

	float x = App->CL_Scene->V_Object[Index]->S_Environ[0]->AmbientColour.x;
	float y = App->CL_Scene->V_Object[Index]->S_Environ[0]->AmbientColour.y;
	float z = App->CL_Scene->V_Object[Index]->S_Environ[0]->AmbientColour.z;
	App->CL_Ogre->mSceneMgr->setAmbientLight(ColourValue(x, y, z));


	// Fog
	if (App->CL_Scene->V_Object[Index]->S_Environ[0]->Fog_On == 1)
	{
		float Start = App->CL_Scene->V_Object[Index]->S_Environ[0]->Fog_Start;
		float End = App->CL_Scene->V_Object[Index]->S_Environ[0]->Fog_End;
		float Density = App->CL_Scene->V_Object[Index]->S_Environ[0]->Fog_Density;

		float x = App->CL_Scene->V_Object[Index]->S_Environ[0]->Fog_Colour.x;
		float y = App->CL_Scene->V_Object[Index]->S_Environ[0]->Fog_Colour.y;
		float z = App->CL_Scene->V_Object[Index]->S_Environ[0]->Fog_Colour.z;

		App->CL_Ogre->mSceneMgr->setFog(FOG_LINEAR, ColourValue(x, y, z), Density, (Ogre::Real)Start, (Ogre::Real)End);
	}
	else
	{
		App->CL_Ogre->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7, 0.7, 0.8), 0, 100, 1000);
	}

	if (PlayMusic == 1)
	{
		char buff[1024];
		strcpy(buff, App->CL_SoundMgr->Default_Folder);
		strcat(buff, "\\Media\\Sounds\\");

		if (App->CL_Scene->V_Object[Index]->S_Environ[0]->Play == 1)
		{
			strcat(buff, App->CL_Scene->V_Object[Index]->S_Environ[0]->Sound_File);

			App->CL_Scene->V_Object[Index]->S_Environ[0]->SndFile = App->CL_SoundMgr->SoundEngine->play2D(buff, App->CL_Scene->V_Object[Index]->S_Environ[0]->Loop, true, true);

			App->CL_Scene->V_Object[Index]->S_Environ[0]->SndFile->setVolume(App->CL_Scene->V_Object[Index]->S_Environ[0]->SndVolume);
			App->CL_Scene->V_Object[Index]->S_Environ[0]->SndFile->setIsPaused(false);
		}
	}
	else
	{
		if (App->CL_Scene->V_Object[Index]->S_Environ[0]->SndFile == NULL)
		{
		}
		else
		{
			App->CL_Scene->V_Object[Index]->S_Environ[0]->SndFile->setIsPaused(true);
			App->CL_Scene->V_Object[Index]->S_Environ[0]->SndFile->drop();
			App->CL_Scene->V_Object[Index]->S_Environ[0]->SndFile = NULL;
		}
	}

	return 1;
}