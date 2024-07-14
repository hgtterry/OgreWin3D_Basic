/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Importers.h"

CL64_Importers::CL64_Importers()
{
	Flag_Reload_Ogre_Model = 0;

	Ogre_Loader_Resource_Group = "Ogre_Loader_Resource_Group";
	Ogre_CFG_Resource_Group = "Ogre_CFG_Resource_Group";
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
		App->Say("Failed To Load");

		return 0;
	}

	App->CL_Scene->Model_Type = Enums::LoadedFile_Assimp;

	App->CL_Ogre->Show_Test_Mesh(false);
	App->CL_Camera->Reset_View();

	App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 1;
	App->CL_Scene->Scene_Mode = Enums::Scene_Mode_MeshData;

	App->CL_Scene->flag_Model_Loaded = 1;

	if (UseDialog == 1)
	{
		App->CL_Ogre->RenderFrame();
		App->CL_Ogre->RenderFrame();
		App->CL_Ogre->RenderFrame();

		App->CL_Dialogs->Start_Import_Options_Dlg();

		if (App->CL_Dialogs->Flag_Convert_to_Ogre == 1)
		{
			App->CL_Scene->Reset_Main_Entity();

			App->CL_Scene->Main_Ent = App->CL_Converters->Convert_To_Ogre3D(1);
			
			App->CL_Scene->Main_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
			App->CL_Scene->Main_Node->attachObject(App->CL_Scene->Main_Ent);

			App->CL_Scene->Main_Node->setVisible(true);
			App->CL_Scene->Main_Node->setOrientation(Ogre::Quaternion::IDENTITY);
			App->CL_Scene->Main_Node->setPosition(0, 0, 0);
			App->CL_Scene->Main_Node->setScale(1, 1, 1);

			App->CL_Scene->Model_Type = Enums::LoadedFile_None;

			App->CL_Ogre->Ogre3D_Listener->Ogre_Model_Loaded = 1;
		
		}
		
	}

	return 1;
}

// *************************************************************************
// *			Load_Ogre_Model:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Importers::Load_Ogre_Model(bool Use_File_Dialog)
{
	if (Use_File_Dialog == 1)
	{
		int Result = App->CL_File_IO->Open_File_Model("Ogre3D   *.mesh\0*.mesh\0", "Ogre3D", NULL);
		if (Result == 0)
		{
			return;
		}
	}

	App->CL_Scene->Clear_Model_And_Reset();

	char Model_Path_And_File[MAX_PATH];
	strcpy(Model_Path_And_File, App->CL_File_IO->Get_Model_Path_File_Name().c_str());

	App->CL_Scene->Set_Paths();

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(Ogre_Loader_Resource_Group) == NULL)
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Ogre_Loader_Resource_Group);
	}

	App->CL_Scene->Reset_Main_Entity();
	
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(App->CL_Scene->Texture_FolderPath,
		"FileSystem",Ogre_Loader_Resource_Group);

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();// App->CL_Scene->Texture_FolderPath);
	}
	catch (...)
	{

	}

	App->CL_Scene->Main_Entity_Set_Default();

	App->CL_Ogre->Show_Test_Mesh(false);
	App->CL_Camera->Reset_View();

	if (App->CL_Scene->Main_Ent)
	{
		Ogre::Vector3 vCenter = Ogre::Vector3(0.0f, (App->CL_Scene->Main_Ent->getBoundingBox().getMaximum().y +
			App->CL_Scene->Main_Ent->getBoundingBox().getMinimum().y) * 0.5f,
			0.0f);

		App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0,0, App->CL_Scene->Main_Ent->getBoundingRadius() * 2.8f));
	}

	App->CL_Ogre->Ogre3D_Listener->Ogre_Model_Loaded = 1;
	
	App->CL_Converters->Ogre_To_Mesh_Data(App->CL_Scene->Main_Ent);
	
	App->CL_Scene->Scene_Mode = Enums::Scene_Mode_ExtMesh;
	App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 0;

	App->CL_Scene->flag_Model_Loaded = 1;

	/*Get_BoneNames();

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

// *************************************************************************
// *			Reload_Ogre_Model:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Importers::Reload_Ogre_Model(void)
{
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(App->CL_Scene->Texture_FolderPath,
		"FileSystem", Ogre_Loader_Resource_Group);

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();// App->CL_Scene->Texture_FolderPath);
	}
	catch (...)
	{

	}

	App->CL_Scene->Main_Ent = App->CL_Ogre->mSceneMgr->createEntity("UserMesh", App->CL_Scene->FileName, Ogre_Loader_Resource_Group);
	App->CL_Scene->Main_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	App->CL_Scene->Main_Node->attachObject(App->CL_Scene->Main_Ent);

	App->CL_Scene->Main_Node->setVisible(true);
	App->CL_Scene->Main_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	App->CL_Scene->Main_Node->setPosition(0, 0, 0);
	App->CL_Scene->Main_Node->setScale(1, 1, 1);

	App->CL_Ogre->Show_Test_Mesh(false);
	App->CL_Camera->Reset_View();

	if (App->CL_Scene->Main_Ent)
	{
		Ogre::Vector3 vCenter = Ogre::Vector3(0.0f, (App->CL_Scene->Main_Ent->getBoundingBox().getMaximum().y +
			App->CL_Scene->Main_Ent->getBoundingBox().getMinimum().y) * 0.5f,
			0.0f);

		App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 0, App->CL_Scene->Main_Ent->getBoundingRadius() * 2.8f));
	}

	App->CL_Converters->Ogre_To_Mesh_Data(App->CL_Scene->Main_Ent);

	App->CL_Ogre->Ogre3D_Listener->Ogre_Model_Loaded = 1;

}

// *************************************************************************
// *		Ogre_Resource_CFG_Loader:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Importers::Ogre_Resource_CFG_Loader(char* Extension, char* Extension2)
{
	int Result = App->CL_File_IO->Open_Resource_File((LPSTR)"Ogre Config   *.cfg\0*.cfg\0", (LPSTR)"Ogre Config", NULL);// App->CL_Model_Data->Path_FileName);
	if (Result == 0)
	{
		return;
	}
	
	App->CL_Resources->Load_OgreCFG_Resources(App->CL_File_IO->OgreCFG_Path_FileName);

	if (Flag_Reload_Ogre_Model == 1)
	{
		Reload_Ogre_Model();
	}
}
