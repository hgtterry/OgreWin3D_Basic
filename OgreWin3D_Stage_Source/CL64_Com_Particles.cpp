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
	Base_Object* V_Object = App->CL_Scene->V_Object[Index];

	V_Object->Phys_Body = NULL;
	V_Object->Physics_Valid = 0;
	V_Object->Usage = Enums::Stage_Usage_Particle;

	V_Object->S_Particle[0]->Particle = NULL;
	V_Object->S_Particle[0]->SpeedFactor = 0.5;
	strcpy(V_Object->S_Particle[0]->ParticleScript, "GD_Smoke1");
}

// *************************************************************************
// *		Create_Particle_Entity:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Com_Particles::Create_Particle_Entity(int Index)
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
