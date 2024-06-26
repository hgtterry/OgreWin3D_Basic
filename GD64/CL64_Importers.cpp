/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Importers.h"

CL64_Importers::CL64_Importers()
{
	OgreModel_Ent = nullptr;
	OgreModel_Node = nullptr;

	Ogre_Loader_Resource_Group = "Ogre_Loader_Resource_Group";
}

CL64_Importers::~CL64_Importers()
{
}

// *************************************************************************
// *			Assimp_Loader:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Importers::Assimp_Loader(bool UseDialog,const char* Extension, const char* Extension2)
{
	if (UseDialog == 1)
	{
		int Result = App->CL_File_IO->Open_File_Model(Extension, Extension2, NULL);
		if (Result == 0)
		{
			return 0;
		}
	}

	App->CL_Scene->Clear_Model_And_Reset();

	char Model_Path_And_File[MAX_PATH];
	strcpy(Model_Path_And_File, App->CL_File_IO->Get_Model_Path_File_Name().c_str());

	App->CL_Scene->Set_Paths();

	bool Test = App->CL_Assimp->LoadFile(Model_Path_And_File);
	if (Test == 0)
	{
		//App->Say_Win("Failed To Load");
		return 0;
	}

	App->CL_Scene->Model_Type = Enums::LoadedFile_Assimp;


	App->CL_Scene->Model_Loaded = 1;

	if (UseDialog == 1)
	{
		App->Say("Imported");
	}

	return 1;
}

// *************************************************************************
// *			Load_Ogre_Model:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Importers::Load_Ogre_Model(void)
{
	int Result = App->CL_File_IO->Open_File_Model("Ogre3D   *.mesh\0*.mesh\0", "Ogre3D", NULL);
	if (Result == 0)
	{
		return;
	}

	//App->CL_Model->Clear_Model_And_Reset();

	char Model_Path_And_File[MAX_PATH];
	strcpy(Model_Path_And_File, App->CL_File_IO->Get_Model_Path_File_Name().c_str());

	App->CL_Scene->Set_Paths();

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(Ogre_Loader_Resource_Group) == NULL)
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Ogre_Loader_Resource_Group);
	}

	//NoTexture = 0;
	//NoMaterialFileFound = 0;

	//AddToScene();

	if (OgreModel_Ent && OgreModel_Node)
	{
		OgreModel_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(OgreModel_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(OgreModel_Ent);
		OgreModel_Ent = nullptr;
		OgreModel_Node = nullptr;
	}

	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(Ogre_Loader_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Ogre_Loader_Resource_Group);

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(App->CL_Scene->Texture_FolderPath,
		"FileSystem",Ogre_Loader_Resource_Group);

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();// App->CL_Scene->Texture_FolderPath);
	}
	catch (...)
	{

	}

	OgreModel_Ent = App->CL_Ogre->mSceneMgr->createEntity("UserMesh", App->CL_Scene->FileName, Ogre_Loader_Resource_Group);
	OgreModel_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	OgreModel_Node->attachObject(OgreModel_Ent);

	OgreModel_Node->setVisible(true);
	OgreModel_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	OgreModel_Node->setPosition(0, 0, 0);
	OgreModel_Node->setScale(1, 1, 1);

	//App->CL_Grid->Grid_SetVisible(1);
	/*Create_MeshGroups();
	Extract_Mesh_Two();*/

	//App->CL_Model_Data->HasMesh = 1;

	/*App->CL_Model->Set_BondingBox_Model(1);

	Get_SkeletonInstance();

	Get_BoneNames();

	Get_Motions();

	Get_Textures();*/

	/*bool SkellAnimation = OgreModel_Ent->hasSkeleton();
	Ogre::SkeletonInstance* skeletonInstance = OgreModel_Ent->getSkeleton();

	if (skeletonInstance && SkellAnimation == 1)
	{
		if (App->CL_Model->MotionCount > 0)
		{
			Ogre::Animation* animation = skeletonInstance->getAnimation(0);
			strcpy(App->CL_Motions->Selected_Motion_Name, animation->getName().c_str());

			strcpy(App->CL_Motions->Decode_MotionByName, App->CL_Motions->Selected_Motion_Name);
			App->CL_Ogre->Ogre_Listener->Animate_State = OgreModel_Ent->getAnimationState(App->CL_Motions->Selected_Motion_Name);
		}
	}*/
}
