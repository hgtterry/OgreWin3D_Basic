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
#include "CL64_Com_Particles.h"

CL64_Com_Particles::CL64_Com_Particles(void)
{
}

CL64_Com_Particles::~CL64_Com_Particles(void)
{
}

// *************************************************************************
// *		Set_Particle_Defaults:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void CL64_Com_Particles::Set_Particle_Defaults(int Index)
{
	Base_Object* B_Object = App->CL_Scene->B_Object[Index];

	B_Object->Phys_Body = NULL;
	B_Object->Physics_Valid = 0;
	B_Object->Usage = Enums::Stage_Usage_Particle;

	B_Object->S_Particle[0]->Particle = NULL;
	B_Object->S_Particle[0]->SpeedFactor = 0.5;
	strcpy(B_Object->S_Particle[0]->ParticleScript, "GD_Smoke1");
}

// *************************************************************************
// *		Create_Particle_Entity:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Com_Particles::Create_Particle_Entity(int Index)
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
	
	// ----------------------------------------------------------------------
	char buf[100];
	char Name[100];
	_itoa(Index, buf, 10);
	strcpy(Name, "TestPart_");
	strcat(Name, buf);

	//ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout

	Object->S_Particle[0]->Particle = App->CL_Ogre->mSceneMgr->createParticleSystem(Name, Object->S_Particle[0]->ParticleScript);
	if (Object->S_Particle[0]->Particle == NULL)
	{
		App->Say("Can not create Particle");
	}

	strcpy(Object->Material_File, Object->S_Particle[0]->ParticleScript);

	Object->S_Particle[0]->Particle->setKeepParticlesInLocalSpace(true);

	Ogre::SceneNode* PartNode = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();


	PartNode->attachObject(Object->S_Particle[0]->Particle);

	PartNode->setPosition(Object->Mesh_Pos);

	PartNode->setScale(Object->Mesh_Scale);

	Object->Object_Node = PartNode;

	Object->Object_Node->setScale(Object->Mesh_Scale);
	Object->Object_Node->setOrientation(Object->Mesh_Quat);
	Object->Object_Node->setPosition(Object->Mesh_Pos);

	Object->S_Particle[0]->Particle->setSpeedFactor(Object->S_Particle[0]->SpeedFactor);

}

// *************************************************************************
// *		Rename_Particle_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Particles::Rename_Particle_Entity(int Index)
{
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	strcpy(App->CL_Dialogs->btext, "Change Object Name");
	strcpy(App->CL_Dialogs->Chr_Text, Object->Object_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CL_Dialogs->Canceled == 1)
	{
		return;
	}

	strcpy(Object->Object_Name, App->CL_Dialogs->Chr_Text);

	Object->Altered = 1;

	App->CL_Scene->flag_Scene_Modified = 1;
	App->CL_FileView->Mark_Altered(Object->FileViewItem);

	App->CL_FileView->Change_Item_Name(Object->FileViewItem, Object->Object_Name);
}

// *************************************************************************
// *			Add_New_Particle:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Particles::Add_New_Particle(char* Script)
{
	char B_Name[MAX_PATH];
	char ConNum[MAX_PATH];

	int Index = App->CL_Scene->Object_Count;

	App->CL_Scene->B_Object[Index] = new Base_Object();
	App->CL_Scene->B_Object[Index]->S_Particle[0] = new Particle_type;
	App->CL_Com_Particles->Set_Particle_Defaults(Index);

	strcpy(App->CL_Scene->B_Object[Index]->S_Particle[0]->ParticleScript, Script);

	App->CL_Scene->B_Object[Index]->Type = Enums::Bullet_Type_None;
	App->CL_Scene->B_Object[Index]->Shape = Enums::Shape_None;
	App->CL_Scene->B_Object[Index]->This_Object_UniqueID = App->CL_Scene->UniqueID_Object_Counter; // Unique ID

	strcpy(App->CL_Scene->B_Object[Index]->Mesh_FileName, "DoorEntity_GD.mesh");

	strcpy_s(B_Name, "Particle_");
	_itoa(Index, ConNum, 10);
	strcat(B_Name, ConNum);
	strcpy(App->CL_Scene->B_Object[Index]->Object_Name, B_Name);

	Ogre::Vector3 Pos = App->CL_Com_Objects->GetPlacement(-50);
	App->CL_Scene->B_Object[Index]->Mesh_Pos = Pos;
	App->CL_Scene->B_Object[Index]->Mesh_Scale = Ogre::Vector3(1, 1, 1);

	Create_Particle_Entity(Index);

	HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Particles_Folder, App->CL_Scene->B_Object[Index]->Object_Name, Index, true);
	App->CL_Scene->B_Object[Index]->FileViewItem = Temp;

	App->CL_FileView->SelectItem(App->CL_Scene->B_Object[Index]->FileViewItem);

	App->CL_Scene->UniqueID_Object_Counter++;
	App->CL_Scene->Object_Count++;

}
