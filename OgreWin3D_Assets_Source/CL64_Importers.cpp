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
		char Start_Directory[MAX_PATH];
		strcpy(Start_Directory, "");

		if (App->CL_Preferences->Use_Default_Directories == 1)
		{
			strcpy(Start_Directory, App->GD_Directory_FullPath);

			if (App->CL_Assimp->Options.Model_Type == Enums::Model_Type_Obj)
			{
				strcat(Start_Directory, "\\Models\\Wavefront_Obj_Models");
			}

			if (App->CL_Assimp->Options.Model_Type == Enums::Model_Type_3ds)
			{
				strcat(Start_Directory, "\\Models\\3ds_Models");
			}

			if (App->CL_Assimp->Options.Model_Type == Enums::Model_Type_Milk)
			{
				strcat(Start_Directory, "\\Models\\Milkshape_Models");
			}
		}

		int Result = App->CL_File_IO->Open_File_Model(Extension, Extension2, Start_Directory);
		if (Result == 0)
		{
			return 0;
		}
	}

	App->CL_Scene->Clear_Scene_And_Reset();

	// Needs Looking At Here Temp 
	App->CL_Resources->Destroy_Resources_Group(App->CL_Resources->Ogre_Loader_Resource_Group);
	App->CL_Resources->Ogre_ExternalResourceLoaded = 0;
	App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";

	char Model_Path_And_File[MAX_PATH];
	strcpy(Model_Path_And_File, App->CL_File_IO->Get_Model_Path_File_Name().c_str());

	App->CL_Scene->Set_Paths();

	bool Test = App->CL_Assimp->LoadFile(Model_Path_And_File);
	if (Test == 0)
	{
		App->Say("Failed To Load");

		return 0;
	}

	App->CL_Scene->Scene_Mode = Enums::Scene_Mode_Assimp_Model;
	App->CL_Scene->Set_Scene(App->CL_Scene->Scene_Mode);

	App->CL_Camera->Reset_View();

	App->Set_Title();
	App->CL_Ogre->RenderFrame(3);

	App->CL_ImGui->flag_Open_Textures_List = 1;
	App->CL_Props_Textures->Selected_Group = 0;
	App->CL_Props_Textures->Update_Texture_Assimp();

	App->Enable_Export_Options(true);

	return 1;
}

// *************************************************************************
// *			Load_Ogre_Model:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
bool CL64_Importers::Load_Ogre_Model(bool Use_File_Dialog)
{
	if (Use_File_Dialog == 1)
	{
		char Start_Directory[MAX_PATH];
		strcpy(Start_Directory,"");

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

	
	Scan_Material_Files();
	
	App->CL_Camera->Reset_View();

	if (App->CL_Scene->Imported_Ogre_Ent)
	{
		Ogre::Vector3 vCenter = Ogre::Vector3(0.0f, (App->CL_Scene->Imported_Ogre_Ent->getBoundingBox().getMaximum().y +
			App->CL_Scene->Imported_Ogre_Ent->getBoundingBox().getMinimum().y) * 0.5f,
			0.0f);

		App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0,0, App->CL_Scene->Imported_Ogre_Ent->getBoundingRadius() * 2.8f));
	}
	
	App->CL_Mesh_Manager->Ogre_To_Mesh_Data(App->CL_Scene->Imported_Ogre_Ent);
	
	App->CL_Resources->mSelected_Resource_Group = App->CL_Resources->Ogre_Loader_Resource_Group;

	App->CL_Scene->Scene_Mode = Enums::Scene_Mode_Ogre_Model;
	App->CL_Scene->flag_Model_Loaded = 1;
	App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 0;
	App->CL_Scene->flag_Show_Ogre_Mesh = 1;
	
	//Get_BoneNames();

	App->CL_Motions->Get_Motions(App->CL_Scene->Imported_Ogre_Ent);

	App->CL_Scene->S_OgreMeshData[0]->mFileName_Str = App->CL_Scene->FileName;
	
	App->Set_Title();

	App->CL_ImGui->flag_Open_Textures_List = 1;
	
	App->CL_Props_Textures->Get_First_Texture_Ogre();
	App->CL_Ogre->RenderFrame(3);

	App->CL_Scene->Scene_Mode = Enums::Scene_Mode_Ogre_Model;
	App->Enable_Export_Options(true);

	App->Say("Mesh Imported", (LPSTR)App->CL_Scene->FileName);
	
	//Get_Textures();

	return 1;
}

// *************************************************************************
// *		Scan_Material_Files:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Importers::Scan_Material_Files(void)
{
	Ogre::String Material;

	for (unsigned int i = 0; i < App->CL_Scene->Imported_Ogre_Ent->getNumSubEntities(); ++i)
	{
		Ogre::SubEntity* subEnt = App->CL_Scene->Imported_Ogre_Ent->getSubEntity(i);
		
		Material = subEnt->getMaterialName();
		
		bool Test = Ogre::MaterialManager::getSingleton().resourceExists(subEnt->getMaterialName(), App->CL_Resources->Ogre_Loader_Resource_Group);
		if (Test == 1)
		{

		}
		else
		{
			subEnt->setMaterialName("No_Material_Loaded", App->CL_Ogre->App_Resource_Group);
		}
	}

}

// *************************************************************************
// *			Reload_Ogre_Model:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Importers::Reload_Ogre_Model(void)
{
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(App->CL_Scene->Texture_FolderPath,
		"FileSystem", App->CL_Resources->Ogre_Loader_Resource_Group);

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();// App->CL_Scene->Texture_FolderPath);
	}
	catch (Ogre::Exception& e)
	{
		App->Say(e.getFullDescription().c_str());
	}

	try
	{
		App->CL_Scene->Imported_Ogre_Ent = App->CL_Ogre->mSceneMgr->createEntity("UserMesh", App->CL_Scene->FileName, App->CL_Resources->Ogre_Loader_Resource_Group);
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

	
	Scan_Material_Files();
	
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

	App->CL_Props_Textures->Get_First_Texture_Ogre();
}

// *************************************************************************
// *		Load_Ogre_Resource_CFG:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Importers::Load_Ogre_Resource_CFG(bool Use_File_Dialog)
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

		int Result = App->CL_File_IO->Open_Resource_File((LPSTR)"Ogre Config   *.cfg\0*.cfg\0", (LPSTR)"Ogre Config", Start_Directory);
		if (Result == 0)
		{
			return 0;
		}
	}

	strcpy(App->CL_Resources->Resource_File_Path_And_File, App->CL_File_IO->OgreCFG_Path_FileName);
	strcpy(App->CL_Resources->Resource_File_FileName, App->CL_File_IO->OgreCFG_FileName);


	App->CL_Dialogs->PleaseWait();

	App->CL_Resources->Load_OgreCFG_Resources(App->CL_File_IO->OgreCFG_Path_FileName);

	App->CL_Resources->mSelected_Resource_Group = App->CL_Resources->Ogre_Loader_Resource_Group;

	if (Flag_Reload_Ogre_Model == 1 && App->CL_Scene->Scene_Mode == Enums::Scene_Mode_Ogre_Model)
	{
		Reload_Ogre_Model();
	}

	App->CL_ImGui->Reset_Material_Index();

	App->CL_Ogre->RenderFrame(8);

	EndDialog(App->ViewPLeaseWait, LOWORD(0));

	App->Say("CFG Imported", App->CL_Resources->Resource_File_FileName);
	
	return 1;
}
