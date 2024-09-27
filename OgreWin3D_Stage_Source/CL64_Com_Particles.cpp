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
	//Ogre::SceneNode* mObject_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//mObject_Node->attachObject(Object->Object_Ent);

	// ----------------------------------------------------------------------
	char buf[100];
	char Name[100];
	_itoa(Index, buf, 10);
	strcpy(Name, "TestPart_");
	strcat(Name, buf);


	Object->S_Particle[0]->Particle = App->CL_Ogre->mSceneMgr->createParticleSystem(Name, Object->S_Particle[0]->ParticleScript);
	if (Object->S_Particle[0]->Particle == NULL)
	{
		App->Say("Poop");
	}

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

	/*Ogre::Vector3 Size = App->CL_Object->GetMesh_BB_Size(Object->Object_Node);
	App->SBC_Markers->MarkerBB_Update(Size.x / 2, Size.y / 2, Size.z / 2);

	Ogre::Vector3 Centre = Object->Object_Node->getAttachedObject(0)->getBoundingBox().getCenter();
	Ogre::Vector3 WS = Object->Object_Node->convertLocalToWorldPosition(Centre);

	float x = App->SBC_Markers->BoxManual->getWorldBoundingBox().getSize().x / 4;
	float y = App->SBC_Markers->BoxManual->getWorldBoundingBox().getSize().y / 4;
	float z = App->SBC_Markers->BoxManual->getWorldBoundingBox().getSize().z / 4;

	mObject_Node->setScale(Object->S_Particle[0]->Particle->getBoundingBox().getSize());
	mObject_Node->setOrientation(Object->Mesh_Quat);
	mObject_Node->setPosition(WS);*/
	//PartNode->attachObject(Object->Object_Ent);
}
