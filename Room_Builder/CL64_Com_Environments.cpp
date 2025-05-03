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
// *		Create_Test_Environment:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Environments::Create_Test_Environment()
{
	App->CL_Com_Environments->Add_New_Environ_Entity(true);
	int mIndex = App->CL_Com_Environments->Get_First_Environ();
	App->CL_Com_Environments->Set_First_Environment(mIndex);

	App->CL_FileView->Enable_Environment_Button(true);
	////App->CL_Scene->flag_Scene_Modified = 1;

	App->CL_Editor_Scene->flag_Environment_Available = 1;
}

// *************************************************************************
// *		Add_New_Environ_Entiry:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Com_Environments::Add_New_Environ_Entity(bool FirstOne)
{
	char B_Name[MAX_PATH];
	char ConNum[MAX_PATH];

	int Index = App->CL_Editor_Com->Object_Count;
	
	App->CL_Editor_Com->B_Object[Index] = new Base_Object();

	App->CL_Editor_Com->B_Object[Index]->S_Environ[0] = new Environ_type;
	V_Set_Environ_Defaults(Index);
	
	App->CL_Editor_Com->B_Object[Index]->Type = Enums::Bullet_Type_Static;
	App->CL_Editor_Com->B_Object[Index]->Shape = Enums::Shape_Box;
	App->CL_Editor_Com->B_Object[Index]->This_Object_UniqueID = App->CL_Editor_Com->UniqueID_Object_Counter; // Unique ID

	strcpy(App->CL_Editor_Com->B_Object[Index]->Mesh_FileName, "EnvironmentEntity_GD.mesh");

	strcpy_s(B_Name, "Environ_");
	_itoa(Index, ConNum, 10);
	strcat(B_Name, ConNum);
	strcpy(App->CL_Editor_Com->B_Object[Index]->Object_Name, B_Name);

	if (FirstOne == 0)
	{
		Ogre::Vector3 Pos = App->CL_Com_Objects->GetPlacement(-50);
		App->CL_Editor_Com->B_Object[Index]->Mesh_Pos = Pos;
	}
	else
	{
		Ogre::Vector3 Pos = Ogre::Vector3(0, 0, 0);
	}

	Create_Environ_Entity(Index);

	HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Evirons_Folder, App->CL_Editor_Com->B_Object[Index]->Object_Name, Index, true);
	App->CL_Editor_Com->B_Object[Index]->FileViewItem = Temp;

	App->CL_Editor_Com->B_Object[Index]->flag_Altered = 1;

	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Evirons_Folder);
	App->CL_FileView->SelectItem(App->CL_Editor_Com->B_Object[Index]->FileViewItem);

	App->CL_Editor_Com->UniqueID_Object_Counter++;
	App->CL_Editor_Com->Object_Count++;

	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Evirons_Folder);
	
	return 1;
}

// *************************************************************************
// *		Create_Environ_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Com_Environments::Create_Environ_Entity(int index) 
{
	const int bufferSize = MAX_PATH;
	char meshFile[bufferSize];
	char ogreName[bufferSize];

	Base_Object* object = App->CL_Editor_Com->B_Object[index];

	// Generate Ogre Name
	snprintf(ogreName, sizeof(ogreName), "GDEnt_%d", index);
	strcpy(meshFile, object->Mesh_FileName);

	// Create Entity and Node
	object->Object_Ent = App->CL_Ogre->mSceneMgr->createEntity(ogreName, meshFile, App->CL_Ogre->App_Resource_Group);
	object->Object_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	object->Object_Node->attachObject(object->Object_Ent);
	object->Object_Node->setVisible(true);
	object->Object_Node->setOrientation(object->Mesh_Quat);
	object->Object_Node->setPosition(object->Mesh_Pos);
	strcpy(object->Material_File, "Internal");

	// Physics Setup
	Ogre::Vector3 center = object->Object_Ent->getWorldBoundingBox(true).getCenter();
	object->Physics_Pos = center;

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(center.x, center.y, center.z));

	btScalar mass = 0.0f;
	btVector3 localInertia(0, 0, 0);
	Ogre::Vector3 size = App->CL_Com_Objects->GetMesh_BB_Size(object->Object_Node);
	btVector3 halfSize(size.x / 2, size.y / 2, size.z / 2);

	object->Physics_Size = Ogre::Vector3(halfSize.x(), halfSize.y(), halfSize.z());
	btCollisionShape* newRigidShape = new btBoxShape(halfSize);
	newRigidShape->calculateLocalInertia(mass, localInertia);
	App->CL_Physics->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	object->Phys_Body = new btRigidBody(rbInfo);
	object->Phys_Body->setRestitution(1.0);
	object->Phys_Body->setFriction(1.5);
	object->Phys_Body->setUserPointer(object->Object_Node);
	object->Phys_Body->setWorldTransform(startTransform);
	object->Usage = Enums::Obj_Usage_EnvironEntity;
	object->Phys_Body->setUserIndex(Enums::Obj_Usage_EnvironEntity);
	object->Phys_Body->setUserIndex2(index);

	int collisionFlags = object->Phys_Body->getCollisionFlags();
	object->Phys_Body->setCollisionFlags(collisionFlags | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT
		| btCollisionObject::CF_KINEMATIC_OBJECT
		| btCollisionObject::CF_NO_CONTACT_RESPONSE);

	App->CL_Physics->dynamicsWorld->addRigidBody(object->Phys_Body);
	App->CL_Physics->Set_Physics_New(index);

	App->CL_Entities->Create_Environment_Brush(index);

	//object->Phys_Body->setCollisionFlags(collisionFlags & (~(1 << 5)));

	return true;
}

// *************************************************************************
// *		V_Set_Environ_Defaults:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Environments::V_Set_Environ_Defaults(int index)
{
	Base_Object* bObject = App->CL_Editor_Com->B_Object[index];

	bObject->flag_Altered = true;

	auto& environment = bObject->S_Environ[0];
	environment->Environment_ID = 0;
	strcpy(environment->Environment_Name, "Not_Set");
	environment->flag_Environ_Enabled = true;

	// Sound Configuration
	strcpy(environment->Sound_File, "The_Sun.ogg");
	environment->flag_Play = false;
	environment->flag_Loop = true;
	environment->SndVolume = 0.5f;
	environment->SndFile = nullptr;

	// Light Configuration
	environment->AmbientColour = { 1.0f, 1.0f, 1.0f };
	environment->Light_Position = { 0.0f, 0.0f, 0.0f };

	// Sky Configuration
	environment->Curvature = 15;
	environment->Distance = 4000;
	environment->flag_Enabled = false;
	strcpy(environment->Material, "Examples/CloudySky");
	environment->Tiling = 15;
	environment->type = 1;

	// Fog Configuration
	environment->Fog_On = false;
	environment->Fog_Mode = FOG_LINEAR;
	environment->Fog_Density = 0.001f;
	environment->Fog_Start = 100;
	environment->Fog_End = 700;
	environment->Fog_Colour = Ogre::Vector3(1.0f, 1.0f, 1.0f);
}

// *************************************************************************
// *		Set_Environment_By_Index:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
int CL64_Com_Environments::Set_Environment_By_Index(bool PlayMusic, int Index)
{
	if (Index == -1)
	{
		App->CL_Ogre->mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));
		App->CL_Ogre->mSceneMgr->setFog(FOG_NONE, ColourValue(1, 1, 1), 0, 100, 1000);
		App->CL_Ogre->mSceneMgr->setSkyDome(false, "OW3D/CloudySky");

		App->CL_SoundMgr->SoundEngine->stopAllSounds();	
	}
	else
	{
		// -------- Ambient Light
		float x = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->AmbientColour.x;
		float y = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->AmbientColour.y;
		float z = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->AmbientColour.z;
		App->CL_Ogre->mSceneMgr->setAmbientLight(ColourValue(x, y, z));

		// -------- Fog
		if (App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_On == 1)
		{
			float Start = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Start;
			float End = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_End;
			float Density = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Density;

			float x = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Colour.x;
			float y = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Colour.y;
			float z = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Colour.z;

			App->CL_Ogre->mSceneMgr->setFog(FOG_LINEAR, ColourValue(x, y, z), Density, (Ogre::Real)Start, (Ogre::Real)End);
		}
		else
		{
			App->CL_Ogre->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7, 0.7, 0.8), 0, 100, 1000);
		}

		// -------- Sky
		bool Enable = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->flag_Enabled;
		App->CL_Ogre->mSceneMgr->setSkyDome(Enable, "OW3D/CloudySky", App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Curvature, App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Tiling, App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Distance);

		// -------- Music
		if (PlayMusic == 1)
		{
			char buff[MAX_PATH];
			strcpy(buff, App->CL_SoundMgr->Default_Folder);
			strcat(buff, "\\Media\\Sounds\\");

			if (App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->flag_Play == 1)
			{
				strcat(buff, App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Sound_File);

				App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->SndFile = App->CL_SoundMgr->SoundEngine->play2D(buff, App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->flag_Loop, true, true);

				App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->SndFile->setVolume(App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->SndVolume);
				App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->SndFile->setIsPaused(false);
			}
		}
		else
		{
			if (App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->SndFile == nullptr)
			{
			}
			else
			{
				App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->SndFile->setIsPaused(true);
				App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->SndFile->drop();
				App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->SndFile = nullptr;
			}
		}

	}

	return 1;
}

// *************************************************************************
// *	Mark_As_Altered_Environ:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Com_Environments::Mark_As_Altered_Environ(int Index)
{
	App->CL_Editor_Com->B_Object[Index]->flag_Altered = 1;

	//App->CL_Editor->flag_Scene_Modified = 1;

	//App->CL_FileView->Mark_Altered(App->CL_Editor->B_Object[Index]->FileViewItem);
}

// *************************************************************************
// *		Get_First_Environ:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
int CL64_Com_Environments::Get_First_Environ() 
{
	for (int count = 0; count < App->CL_Editor_Com->Object_Count; ++count) 
	{
		if (App->CL_Editor_Com->B_Object[count]->Usage == Enums::Obj_Usage_EnvironEntity) 
		{
			return count;
		}
	}

	return -1;
}

// *************************************************************************
// *		Set_First_Environment:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Environments::Set_First_Environment(int Index)
{
	float x = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->AmbientColour.x;
	float y = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->AmbientColour.y;
	float z = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->AmbientColour.z;

	App->CL_Ogre->mSceneMgr->setAmbientLight(ColourValue(x, y, z));

	if (App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_On == 1)
	{
		float Start = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Start;
		float End = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_End;
		float Density = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Density;

		float x = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Colour.x;
		float y = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Colour.y;
		float z = App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Fog_Colour.z;

		App->CL_Ogre->mSceneMgr->setFog(FOG_LINEAR, ColourValue(x, y, z), Density, (Ogre::Real)Start, (Ogre::Real)End);
	}
	else
	{
		App->CL_Ogre->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7, 0.7, 0.8), 0, 100, 1000);
	}

	if (App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->flag_Enabled == 1)
	{
	App->CL_Ogre->mSceneMgr->setSkyDome(true, "OW3D/CloudySky", 
		App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Curvature, 
		App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Tiling, 
		App->CL_Editor_Com->B_Object[Index]->S_Environ[0]->Distance);
		
	}
	else
	{

	}

}

// *************************************************************************
// *		Set_Environment_GameMode:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Com_Environments::Set_Environment_GameMode(bool Turn_On)
{
	/*int First_Environ = Get_First_Environ();

	if (Turn_On == 1)
	{
		char buff[1024];
		strcpy(buff, App->CL_SoundMgr->Default_Folder);
		strcat(buff, "\\Media\\Sounds\\");

		strcat(buff, App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->Sound_File);

		App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->SndFile = App->CL_SoundMgr->SoundEngine->play2D(buff, App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->flag_Loop, true, true);

		App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->SndFile->setVolume(App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->SndVolume);
		App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->SndFile->setIsPaused(false);

		App->CL_Collision->Old_Sound_Index = First_Environ;
	}
	else
	{
		if (App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->SndFile == NULL)
		{
		}
		else
		{
			App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->SndFile->setIsPaused(true);
			App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->SndFile->drop();
			App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->SndFile = NULL;
		}
	}

	float x = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->AmbientColour.x;
	float y = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->AmbientColour.y;
	float z = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->AmbientColour.z;

	App->CL_Ogre->mSceneMgr->setAmbientLight(ColourValue(x, y, z));

	if (App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->Fog_On == 1)
	{
		float Start = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->Fog_Start;
		float End = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->Fog_End;
		float Density = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->Fog_Density;

		float x = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->Fog_Colour.x;
		float y = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->Fog_Colour.y;
		float z = App->CL_Editor->B_Object[First_Environ]->S_Environ[0]->Fog_Colour.z;

		App->CL_Ogre->mSceneMgr->setFog(FOG_LINEAR, ColourValue(x, y, z), Density, (Ogre::Real)Start, (Ogre::Real)End);
	}
	else
	{
		App->CL_Ogre->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7, 0.7, 0.8), 0, 100, 1000);
	}*/
}

// *************************************************************************
// *		Rename_Environ_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Environments::Rename_Environ_Entity(int Index)
{
	strcpy(App->CL_Dialogs->btext, "Change Environ Name");
	strcpy(App->CL_Dialogs->Chr_Text, App->CL_Editor_Com->B_Object[Index]->Object_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	//if (App->CL_Dialogs->flag_Canceled == 1)
	{
		return;
	}

	strcpy(App->CL_Editor_Com->B_Object[Index]->Object_Name, App->CL_Dialogs->Chr_Text);

	//App->CL_FileView->Change_Item_Name(App->CL_Editor->B_Object[Index]->FileViewItem, App->CL_Dialogs->Chr_Text);

	Mark_As_Altered_Environ(Index);

}
