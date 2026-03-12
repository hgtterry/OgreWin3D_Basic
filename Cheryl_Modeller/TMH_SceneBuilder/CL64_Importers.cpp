#include "pch.h"
#include "CL64_App.h"
#include "CL64_Importers.h"

CL64_Importers::CL64_Importers(void)
{
	Flag_Reload_Ogre_Model = false;
}

CL64_Importers::~CL64_Importers(void)
{
}

// *************************************************************************
// *			Set_Editor:- Terry and Hazel Flanigan 2026		 		   *
// *************************************************************************
void CL64_Importers::Set_Editor()
{
	if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		App->CL_Interface->Show_Materials_Dlg(true);
		App->CL_Interface->Menu_Enable_Materials(true);
		App->CL_Interface->Menu_Enable_OgreExport(false);
		App->CL_Interface->Show_file_view(true);
	}

	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		App->CL_Ogre->OGL_Listener->flag_ShowTextured = true;
		App->CL_Interface->Show_Textures_Assimp_Dlg(true);
		App->CL_Interface->Menu_Enable_OgreExport(true);
	}

	App->CL_Top_Tabs->Set_Texture_Bmp_On();


	if (App->CL_Model->MotionCount > 0)
	{
		App->CL_Interface->Show_Motions_Dlg(true);
	}
	else
	{
		App->CL_Interface->Show_Motions_Dlg(false);
	}
}

// *************************************************************************
// *			Assimp_Loader:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Importers::Assimp_Loader(bool UseDialog, const char* Extension, const char* Extension2)
{
	if (UseDialog == 1)
	{
		/*char Start_Directory[MAX_PATH];
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
		}*/

		LPCWSTR mType = L"Wavefront .obj file";
		LPCWSTR mExtensions = L"*.obj";

		bool test = App->CL_File_IO->Open_File((LPCWSTR)mType, (LPCWSTR)mExtensions);
		if (test == false)
		{
			return 0;
		}

		strcpy(App->CL_Model->Loaded_PathFileName, App->CL_File_IO->s_Path_And_File.c_str());
		strcpy(App->CL_Model->Loaded_FileName, App->CL_File_IO->s_Just_FileName.c_str());
	}

	App->CL_Model->Clear_Model();

	//// Needs Looking At Here Temp 
	//App->CL_Resources->Destroy_Resources_Group(App->CL_Resources->Ogre_Loader_Resource_Group);
	//App->CL_Resources->Ogre_ExternalResourceLoaded = 0;
	App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";

	//char Model_Path_And_File[MAX_PATH];
	//strcpy(Model_Path_And_File, App->CL_File_IO->Get_Model_Path_File_Name().c_str());

	App->CL_Model->Set_Paths();

	bool Test = App->CL_Assimp->LoadFile(App->CL_Model->Loaded_PathFileName);
	if (Test == 0)
	{
		App->Say("Failed To Load");

		return 0;
	}

	App->CL_Model->Model_Type = Enums::Model_Type_Assimp;
	App->CL_Model->flag_Model_Loaded = true;
	App->CL_Properties_Textures_Assimp->Fill_Materials_ListBox();
	/*App->CL_Scene->Scene_Mode = Enums::Scene_Mode_Assimp_Model;
	App->CL_Scene->Set_Scene(App->CL_Scene->Scene_Mode);*/

	App->CL_Camera->Reset_View_and_Zoom();

	App->CL_Interface->Set_Title(false);

	Set_Editor();

	App->CL_File_IO->RecentFileHistory_Update();

	//App->CL_Ogre->RenderFrame(3);

	//App->CL_ImGui->flag_Open_Textures_List = 1;
	App->CL_Properties_Textures_Assimp->Selected_Group = 0;
	App->CL_Properties_Textures_Assimp->Update_Texture_Assimp();
	App->CL_Properties_Textures_Assimp->Get_First_Texture_Ogre();

	//App->Enable_Export_Options(true);

	return 1;
}

// *************************************************************************
// *			Load_Ogre_Model:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
bool CL64_Importers::Load_Ogre_Model(bool Use_File_Dialog, bool Check_Resource_File)
{
	if (Use_File_Dialog == true)
	{
		char Start_Directory[MAX_PATH];
		strcpy(Start_Directory, "");

		/*if (App->CL_Preferences->Use_Default_Directories == 1)
		{
			strcpy(Start_Directory, App->GD_Directory_FullPath);
			strcat(Start_Directory, "\\Models\\Ogre3D_Models");

			App->CL_Preferences->Use_Default_Directories = 0;
		}*/

		LPCWSTR mType = L"Ogre3D";
		LPCWSTR mExtensions = L"*.mesh";

		bool test = App->CL_File_IO->Open_File((LPCWSTR)mType, (LPCWSTR)mExtensions);
		if (test == false)
		{
			return 0;
		}

		strcpy(App->CL_Model->Loaded_PathFileName, App->CL_File_IO->s_Path_And_File.c_str());
		strcpy(App->CL_Model->Loaded_FileName, App->CL_File_IO->s_Just_FileName.c_str());

	}
	
	App->CL_Model->Clear_Model();

	if (App->CL_Resources->flag_Ogre_CFG_Loaded == true && Check_Resource_File == true)
	{
		App->CL_Dialogs->YesNo((LPSTR)"Use Loaded Resources", (LPSTR)"Yes No");
		if (App->CL_Dialogs->flag_Dlg_Canceled == true) // No
		{
			App->CL_Resources->Unload_OgreCFG_Resources();
		}
	}

	if (Check_Resource_File == false)
	{
		App->CL_Resources->Unload_OgreCFG_Resources();
	}

	App->CL_Model->Set_Paths();

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Resources->Ogre_Loader_Resource_Group) == NULL)
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Resources->Ogre_Loader_Resource_Group);
	}

	if (App->CL_Model->Imported_Ogre_Ent && App->CL_Model->Imported_Ogre_Node)
	{
		App->CL_Model->Imported_Ogre_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Model->Imported_Ogre_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Model->Imported_Ogre_Ent);
		App->CL_Model->Imported_Ogre_Ent = nullptr;
		App->CL_Model->Imported_Ogre_Node = nullptr;
	}

	// Resource FolderPath
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(App->CL_Model->Model_FolderPath,
		"FileSystem", App->CL_Resources->Ogre_Loader_Resource_Group);

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();// App->CL_Scene->Texture_FolderPath);
	}
	catch (Ogre::Exception& e)
	{
		App->Say_Win(e.getFullDescription().c_str());
		return false;
	}

	try
	{
		App->CL_Model->Imported_Ogre_Ent = App->CL_Ogre->mSceneMgr->createEntity("Imported_Entity", App->CL_Model->Loaded_FileName, App->CL_Resources->Ogre_Loader_Resource_Group);
		App->CL_Model->Imported_Ogre_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		App->CL_Model->Imported_Ogre_Node->attachObject(App->CL_Model ->Imported_Ogre_Ent);

		App->CL_Model->Imported_Ogre_Node->setVisible(true);
		App->CL_Model->Imported_Ogre_Node->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Model->Imported_Ogre_Node->setPosition(0, 0, 0);
		App->CL_Model->Imported_Ogre_Node->setScale(1, 1, 1);
	}
	catch (Ogre::Exception& e)
	{
		App->Say_Win(e.getFullDescription().c_str());
		return false;
	}


	Scan_Material_Files();

	App->CL_Camera->Reset_View();

	if (App->CL_Model->Imported_Ogre_Ent)
	{
		Ogre::Vector3 vCenter = Ogre::Vector3(0.0f, (App->CL_Model->Imported_Ogre_Ent->getBoundingBox().getMaximum().y +
			App->CL_Model->Imported_Ogre_Ent->getBoundingBox().getMinimum().y) * 0.5f,
			0.0f);

		App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 0, App->CL_Model->Imported_Ogre_Ent->getBoundingRadius() * 2.8f));
	}

	App->CL_Mesh->Ogre_To_Mesh_Data(App->CL_Model->Imported_Ogre_Ent);

	App->CL_Resources->mSelected_Resource_Group = App->CL_Resources->Ogre_Loader_Resource_Group;

	App->CL_Model->Model_Type = Enums::Model_Type_Ogre3D;
	App->CL_Model->flag_Model_Loaded = true;
	//App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 0;
	App->CL_Ogre->OGL_Listener->flag_ShowFaces = false;

	App->CL_Motions->Get_Motions(App->CL_Model->Imported_Ogre_Ent);

	App->CL_Resources->mSelected_Resource_Group = App->CL_Resources->Ogre_Loader_Resource_Group;

	App->CL_Model->Model_Type = Enums::Model_Type_Ogre3D;
	App->CL_Properties_Materials->Fill_Materials_ListBox();
	App->CL_Properties_Materials->Get_First_Texture_Ogre();

	Set_Editor();
	
	////Get_BoneNames();

	App->CL_Interface->Set_Title(false);

	//App->CL_ImGui->flag_Open_Textures_List = 1;

	//App->CL_Ogre->RenderFrame(3);

	if (App->CL_Resources->flag_Ogre_CFG_Loaded == false && Check_Resource_File == true)
	{
		App->CL_Dialogs->YesNo((LPSTR)"No Resources Loaded", (LPSTR)"Load Resources");
		if (App->CL_Dialogs->flag_Dlg_Canceled == false) // Yes
		{
			App->CL_Importers->Load_Ogre_Resource_CFG(true);
		}
	}

	App->CL_File_IO->RecentFileHistory_Update();

	//App->Enable_Export_Options(true);

	//App->Say("Mesh Imported", (LPSTR)App->CL_Scene->FileName);

	////Get_Textures();

	return 1;
}

// *************************************************************************
// *			Reload_Ogre_Model:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Importers::Reload_Ogre_Model(Ogre::Quaternion Rotation)
{
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(App->CL_Model->Model_FolderPath,
		"FileSystem", App->CL_Resources->Ogre_Loader_Resource_Group);

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();// App->CL_Scene->Texture_FolderPath);
	}
	catch (Ogre::Exception& e)
	{
		App->Say_Win(e.getFullDescription().c_str());
	}

	try
	{
		App->CL_Model->Imported_Ogre_Ent = App->CL_Ogre->mSceneMgr->createEntity("UserMesh", App->CL_Model->Loaded_FileName, App->CL_Resources->Ogre_Loader_Resource_Group);
		App->CL_Model->Imported_Ogre_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		App->CL_Model->Imported_Ogre_Node->attachObject(App->CL_Model->Imported_Ogre_Ent);

		App->CL_Model->Imported_Ogre_Node->setVisible(true);
		App->CL_Model->Imported_Ogre_Node->setOrientation(Rotation);
		App->CL_Model->Imported_Ogre_Node->setPosition(0, 0, 0);
		App->CL_Model->Imported_Ogre_Node->setScale(1, 1, 1);

	}
	catch (Ogre::Exception& e)
	{
		App->Say_Win(e.getFullDescription().c_str());
	}


	Scan_Material_Files();
	App->CL_Mesh->Get_Ogre_Mesh_Data(App->CL_Model->Imported_Ogre_Ent);
	//App->CL_Camera->Reset_View();


	/*if (App->CL_Model->Imported_Ogre_Ent)
	{
		Ogre::Vector3 vCenter = Ogre::Vector3(0.0f, (App->CL_Model->Imported_Ogre_Ent->getBoundingBox().getMaximum().y +
			App->CL_Model->Imported_Ogre_Ent->getBoundingBox().getMinimum().y) * 0.5f,
			0.0f);

		App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 0, App->CL_Model->Imported_Ogre_Ent->getBoundingRadius() * 2.8f));
	}*/

	App->CL_Properties_Materials->Get_First_Texture_Ogre();
	App->CL_Properties_Materials->Fill_Materials_ListBox();
}

// *************************************************************************
// *		Load_Ogre_Resource_CFG:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Importers::Load_Ogre_Resource_CFG(bool Use_File_Dialog)
{
	if (Use_File_Dialog == 1)
	{
		/*char Start_Directory[MAX_PATH];
		strcpy(Start_Directory, "");

		if (App->CL_Preferences->Use_Default_Directories == 1)
		{
			strcpy(Start_Directory, App->GD_Directory_FullPath);
			strcat(Start_Directory, "\\Models\\Ogre3D_Models");

			App->CL_Preferences->Use_Default_Directories = 0;
		}*/

		LPCWSTR mType = L"Ogre Config File";
		LPCWSTR mExtensions = L"*.cfg";

		bool test = App->CL_File_IO->Open_File((LPCWSTR)mType, (LPCWSTR)mExtensions);
		if (test == false)
		{
			return 0;
		}
	}

	strcpy(App->CL_Resources->Resource_File_Path_And_File, App->CL_File_IO->s_Path_And_File.c_str());
	strcpy(App->CL_Resources->Resource_File_FileName, App->CL_File_IO->s_Just_FileName.c_str());


	Ogre::Quaternion Rot = App->CL_Model->Imported_Ogre_Node->getOrientation();


	App->CL_Dialogs->PleaseWait();

	App->CL_Resources->Load_OgreCFG_Resources(App->CL_Resources->Resource_File_Path_And_File);

	App->CL_Resources->mSelected_Resource_Group = App->CL_Resources->Ogre_Loader_Resource_Group;

	if (Flag_Reload_Ogre_Model == true && App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		Reload_Ogre_Model(Rot);
	}

	//App->CL_ImGui->Reset_Material_Index();

	App->CL_Ogre->RenderFrame(8);

	EndDialog(App->ViewPLeaseWait, LOWORD(0));

	App->Say("CFG Imported", App->CL_Resources->Resource_File_FileName);

	return 1;
}

// *************************************************************************
// *		Scan_Material_Files:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Importers::Scan_Material_Files(void)
{
	bool All_Correct = true;

	Ogre::String Material;

	for (unsigned int i = 0; i < App->CL_Model->Imported_Ogre_Ent->getNumSubEntities(); ++i)
	{
		Ogre::SubEntity* subEnt = App->CL_Model->Imported_Ogre_Ent->getSubEntity(i);

		Material = subEnt->getMaterialName();

		bool Test = Ogre::MaterialManager::getSingleton().resourceExists(subEnt->getMaterialName(), App->CL_Resources->Ogre_Loader_Resource_Group);
		if (Test == true)
		{

		}
		else
		{
			subEnt->setMaterialName("No_Material_Loaded", App->CL_Ogre->App_Resource_Group);
			bool All_Correct = false;
		}
	}

	if (All_Correct == true)
	{
		//App->CL_Resources->flag_Ogre_CFG_Loaded = true;
	}
}

// *************************************************************************
// *			Load_Recent_File:- Terry and Hazel Flanigan 2026 		   *
// *************************************************************************
void CL64_Importers::Load_Recent_File(char* FileAndPath)
{
	App->CL_Utilities->Get_FileName_FromPath((LPSTR)FileAndPath, (LPSTR)FileAndPath);
	strcpy(App->CL_Model->Loaded_FileName, App->CL_Utilities->JustFileName);
	strcpy(App->CL_Model->Loaded_PathFileName, FileAndPath);

	if (_stricmp(App->CL_Model->Loaded_FileName + strlen(App->CL_Model->Loaded_FileName) - 5, ".mesh") == 0)
	{
		bool test = App->CL_Importers->Load_Ogre_Model(false, true);
	}

	if (_stricmp(App->CL_Model->Loaded_FileName + strlen(App->CL_Model->Loaded_FileName) - 4, ".obj") == 0)
	{
		App->CL_Assimp->Options.SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
		App->CL_Assimp->Options.Model_Type = Enums::Model_Type_Assimp;

		bool test = App->CL_Importers->Assimp_Loader(false, "", "");
	}
}

