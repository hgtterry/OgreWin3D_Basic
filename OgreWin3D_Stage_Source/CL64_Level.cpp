/*
Copyright (c) OgreWin3D_Stage 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

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
#include "CL64_Level.h"

CL64_Level::CL64_Level(void)
{
}

CL64_Level::~CL64_Level(void)
{
}

// *************************************************************************
// *				Load_Level:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_Level::Load_Level(bool Use_File_Dialog)
{
	if (Use_File_Dialog == 1)
	{
		char Start_Directory[MAX_PATH];
		strcpy(Start_Directory, "");

		if (App->CL_Preferences->Use_Default_Directories == 1)
		{
			strcpy(Start_Directory, App->GD_Directory_FullPath);
			strcat(Start_Directory, "\\Models\\Ogre3D_Models");

			App->CL_Preferences->Use_Default_Directories = 0;
		}

		int Result = App->CL_File_IO->Open_File_Model("Ogre3D   *.mesh\0*.mesh\0", "Ogre3D", Start_Directory);
		if (Result == 0)
		{
			return 0;
		}
	}

	App->CL_Scene->Clear_Scene_And_Reset();

	char Model_Path_And_File[MAX_PATH];
	strcpy(Model_Path_And_File, App->CL_File_IO->Get_Model_Path_File_Name().c_str());

	App->CL_Scene->Set_Paths();

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Resources->Ogre_Loader_Resource_Group) == NULL)
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Resources->Ogre_Loader_Resource_Group);
	}

	if (App->CL_Scene->Imported_Ogre_Ent && App->CL_Scene->Imported_Ogre_Node)
	{
		App->CL_Scene->Imported_Ogre_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Scene->Imported_Ogre_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Scene->Imported_Ogre_Ent);
		App->CL_Scene->Imported_Ogre_Ent = nullptr;
		App->CL_Scene->Imported_Ogre_Node = nullptr;
	}

	App->CL_Ogre->Ogre3D_Listener->Ogre_Model_Loaded = 0;

	//App->CL_Scene->Reset_Main_Entity();

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(App->CL_Scene->Texture_FolderPath,
		"FileSystem", App->CL_Resources->Ogre_Loader_Resource_Group);

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();// App->CL_Scene->Texture_FolderPath);
	}
	catch (...)
	{

	}

	try
	{
		App->CL_Scene->Imported_Ogre_Ent = App->CL_Ogre->mSceneMgr->createEntity("Imported_Entity", App->CL_Scene->FileName, App->CL_Resources->Ogre_Loader_Resource_Group);
		App->CL_Scene->Imported_Ogre_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		App->CL_Scene->Imported_Ogre_Node->attachObject(App->CL_Scene->Imported_Ogre_Ent);

		App->CL_Scene->Imported_Ogre_Node->setVisible(true);
		App->CL_Scene->Imported_Ogre_Node->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Scene->Imported_Ogre_Node->setPosition(0, 0, 0);
		App->CL_Scene->Imported_Ogre_Node->setScale(1, 1, 1);
	}
	catch (Ogre::Exception& e)
	{
		App->Say(e.getFullDescription().c_str());
	}


	App->CL_Importers->Scan_Material_Files();


	App->CL_Ogre->Show_Test_Mesh(false);
	App->CL_Camera->Reset_View();

	if (App->CL_Scene->Imported_Ogre_Ent)
	{
		Ogre::Vector3 vCenter = Ogre::Vector3(0.0f, (App->CL_Scene->Imported_Ogre_Ent->getBoundingBox().getMaximum().y +
			App->CL_Scene->Imported_Ogre_Ent->getBoundingBox().getMinimum().y) * 0.5f,
			0.0f);

		App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 0, App->CL_Scene->Imported_Ogre_Ent->getBoundingRadius() * 2.8f));
	}

	App->CL_Mesh_Manager->Ogre_To_Mesh_Data(App->CL_Scene->Imported_Ogre_Ent);

	App->CL_Resources->mSelected_Resource_Group = App->CL_Resources->Ogre_Loader_Resource_Group;

	App->CL_Import_Ogre3D->flag_Ogre_Model_Loaded = 1;
	App->CL_Scene->flag_Model_Loaded = 1;

	App->CL_Scene->Set_Scene(Enums::Scene_Mode_Imported_Entity);

	//Get_BoneNames();

	App->CL_Motions->Get_Motions(App->CL_Scene->Imported_Ogre_Ent);

	App->CL_Scene->S_OgreMeshData[0]->mFileName_Str = App->CL_Scene->FileName;

	App->Set_Title();

	App->CL_ImGui->flag_Open_Textures_List = 1;
	App->CL_Props_Textures->Get_First_Texture_Ogre();
	App->CL_Ogre->RenderFrame(3);

	App->CL_ImGui->flag_Show_Model_Data = 0;
	//App->Say("Mesh Imported", (LPSTR)App->CL_Scene->FileName);

	//Get_Textures();
	Set_Scene();

	return 1;
}

// *************************************************************************
// *				Set_Scene:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
void CL64_Level::Set_Scene()
{
	App->CL_Ogre->Enable_Fog(true);

	App->CL_Camera->Reset_View();

	App->CL_Bullet->Create_New_Trimesh(App->CL_Scene->Imported_Ogre_Ent, App->CL_Scene->Imported_Ogre_Node);

	App->CL_Player->Set_Player_Position(Ogre::Vector3(0, 20, 0));
	App->CL_Player->Set_Player_Rotation(btQuaternion(0, 0, 1, 0));
	App->CL_Player->Set_Player_GroundSpeed(70);

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;
	App->CL_Ogre->Ogre3D_Listener->Run_Physics = 1;

	App->CL_ImGui->flag_Show_Demo_Options = 1;
}

//// *************************************************************************
//// *				Reset_View:- Terry and Hazel Flanigan 2024			   *
//// *************************************************************************
//void CL64_Demos::Reset_View(void)
//{
//	if (flag_Demo_1_Running == 1)
//	{
//		App->CL_Camera->Reset_View();
//		App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
//
//		App->CL_Player->Set_Player_Position(Ogre::Vector3(0, 0, 0));
//		App->CL_Player->Set_Player_Rotation(btQuaternion(1, 0, 0, 0));
//
//		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);
//		App->CL_Scene->B_Player[0]->CameraPitch_Node->setOrientation(Ogre::Quaternion::IDENTITY);
//	}
//
//	if (flag_Demo_2_Running == 1)
//	{
//		App->CL_Camera->Set_Camera_Rotation(0, 90, 0);
//		App->CL_Camera->Set_Camera_Position(-237, -120, 800);
//
//		App->CL_Player->Set_Player_Position(Ogre::Vector3(-237, -120, 800));
//		App->CL_Player->Set_Player_Rotation(btQuaternion(0.7071068, 0, 0.7071068, 0));
//	}
//
//}
