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
	App->CL_Interface->Show_Textures_Com_Dlg(true);

	App->CL_Top_Tabs->Set_Texture_Bmp_On();

	if(App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		App->CL_Interface->Enable_Change_Textures_Button(false);
		
		App->CL_Interface->Menu_Enable_Textures(true);
		App->CL_Interface->Menu_Enable_OgreExport(false);
		App->CL_Interface->Show_file_view(true);
	}

	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		App->CL_Interface->Enable_Change_Textures_Button(true);

		App->CL_Interface->Menu_Enable_Textures(true);
		App->CL_Ogre->OGL_Listener->flag_ShowTextured = true;
		App->CL_Interface->Menu_Enable_OgreExport(true);
	}


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
bool CL64_Importers::Assimp_Loader(bool UseDialog, const LPCWSTR Filetype, const LPCWSTR Extension)
{
	if (UseDialog)
	{
		LPCWSTR fileType = L"Wavefront .obj file";
		LPCWSTR fileExtensions = L"*.obj";

		if (!App->CL_File_IO->Open_File(Filetype, Extension))
		{
			return false;
		}

		strcpy(App->CL_Model->Loaded_PathFileName, App->CL_File_IO->s_Path_And_File.c_str());
		strcpy(App->CL_Model->Loaded_FileName, App->CL_File_IO->s_Just_FileName.c_str());
	}

	App->CL_Model->Clear_Model();

	// Temporary resource management commented out for review
	// App->CL_Resources->Destroy_Resources_Group(App->CL_Resources->Ogre_Loader_Resource_Group);
	// App->CL_Resources->Ogre_ExternalResourceLoaded = 0;

	App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";

	App->CL_Model->Set_Paths();

	if (!App->CL_Assimp->LoadFile(App->CL_Model->Loaded_PathFileName))
	{
		App->Say("Failed To Load");
		return false;
	}

	App->CL_Model->Model_Type = Enums::Model_Type_Assimp;
	App->CL_Model->flag_Model_Loaded = true;
	App->CL_Properties_Textures_Com->Fill_Materials_ListBox();

	App->CL_Camera->Reset_View_and_Zoom();
	App->CL_Interface->Set_Title(false);

	Set_Editor();
	App->CL_File_IO->RecentFileHistory_Update();

	App->CL_Properties_Textures_Com->Selected_Group = 0;
	App->CL_Properties_Textures_Com->Update_Dlg_Bmp_Texture();
	App->CL_Properties_Textures_Com->Get_First_Texture();

	Create_Brush();

	App->CL_3D_TL_View->Set_Zoom();

	return true;
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
	
	//if (Check_Resource_File == false)
	{
		//App->CL_Resources->Unload_OgreCFG_Resources();
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
	App->CL_Properties_Textures_Com->Fill_Materials_ListBox();
	App->CL_Properties_Textures_Com->Get_First_Texture();

	Set_Editor();
	
	////Get_BoneNames();

	App->CL_Interface->Set_Title(false);

	if (App->CL_Resources->flag_Material_File_Loaded == false)
	{
		App->CL_Dialogs->YesNo((LPSTR)"No Material File", (LPSTR)"Do you want to load a Resource File");
		if (App->CL_Dialogs->flag_Dlg_Canceled == false) // Yes
		{
			App->CL_Importers->Load_Ogre_Resource_CFG(true);
		}
	}

	App->CL_File_IO->RecentFileHistory_Update();

	Create_Brush();

	App->CL_3D_TL_View->Set_Zoom();
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

	App->CL_Properties_Textures_Com->Get_First_Texture();
	App->CL_Properties_Textures_Com->Fill_Materials_ListBox();
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
	App->CL_Resources->flag_Material_File_Loaded = true;

	bool All_Correct = true;
	Ogre::String Material;

	for (unsigned int i = 0; i < App->CL_Model->Imported_Ogre_Ent->getNumSubEntities(); ++i)
	{
		Ogre::SubEntity* subEnt = App->CL_Model->Imported_Ogre_Ent->getSubEntity(i);

		Material = subEnt->getMaterialName();

		int Result = strcmp(Material.c_str(), "BaseWhite");
		if (Result == 0)
		{
			All_Correct = false;
			subEnt->setMaterialName("No_Material_Loaded", App->CL_Ogre->App_Resource_Group);
		}
		else
		{

			bool Test = Ogre::MaterialManager::getSingleton().resourceExists(subEnt->getMaterialName(), App->CL_Resources->Ogre_Loader_Resource_Group);
			if (Test == true)
			{

			}
			else
			{
				subEnt->setMaterialName("No_Material_Loaded", App->CL_Ogre->App_Resource_Group);
				All_Correct = false;
			}
		}
	}

	if (All_Correct == false)
	{
		App->CL_Resources->flag_Material_File_Loaded = false;
	}
}

// *************************************************************************
// *			Create_Brush:- Terry and Hazel Flanigan 2026 			   *
// *************************************************************************
void CL64_Importers::Create_Brush()
{
	FaceList* fl;
	Face* f;
	int NumFaces = 0;

	NumFaces = App->CL_Model->FaceCount;
	fl = App->CL_X_FaceList->FaceList_Create(NumFaces);
	if (!fl)
	{
		App->Say("Can not create Faces");

		return;
	}

	int GroupCount = App->CL_Model->GroupCount;
	int Count = 0;

	while (Count < GroupCount)
	{
		//MeshData_Face_Groups(Count);
		auto& group = App->CL_Mesh->Group[Count];
		int faceCount = group->GroupFaceCount;

		// Iterate through each face in the group
		for (int i = 0; i < faceCount; ++i)
		{
			T_Vec3 FaceVerts[3];

			// Retrieve vertex indices for the current face
			int A = group->Face_Data[i].a;
			int B = group->Face_Data[i].b;
			int C = group->Face_Data[i].c;

			// Specify the vertices of the polygon
			FaceVerts[0].x = group->vertex_Data[A].x;
			FaceVerts[0].y = group->vertex_Data[A].y;
			FaceVerts[0].z = group->vertex_Data[A].z;

			FaceVerts[1].x = group->vertex_Data[B].x;
			FaceVerts[1].y = group->vertex_Data[B].y;
			FaceVerts[1].z = group->vertex_Data[B].z;

			FaceVerts[2].x = group->vertex_Data[C].x;
			FaceVerts[2].y = group->vertex_Data[C].y;
			FaceVerts[2].z = group->vertex_Data[C].z;

			f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);

			if (f)
			{
				App->CL_X_FaceList->FaceList_AddFace(fl, f);
			}
		}

		Count++;
	}

	Brush* Test = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, NULL);

	if (Test)
	{
		App->CL_X_Brush->Brush_SetName(Test, "Test_Brush");
		App->CL_Level->Level_AppendBrush(Test);

		//App->Say("Brush Created");
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

		bool test = App->CL_Importers->Assimp_Loader(false, NULL, NULL);
	}

	if (_stricmp(App->CL_Model->Loaded_FileName + strlen(App->CL_Model->Loaded_FileName) - 5, ".ms3d") == 0)
	{
		App->CL_Assimp->Options.SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
		App->CL_Assimp->Options.Model_Type = Enums::Model_Type_Assimp;

		bool test = App->CL_Importers->Assimp_Loader(false, NULL, NULL);
	}
}

