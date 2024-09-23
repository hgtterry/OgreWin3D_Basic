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
#include "CL64_Project.h"

CL64_Project::CL64_Project()
{
	m_Ini_Path_File_Name[0] = 0;
}

CL64_Project::~CL64_Project()
{
}

// *************************************************************************
// *	  		Load_Project:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Load_Project()
{

	m_Ini_Path_File_Name[0] = 0;

	App->CL_Scene->Clear_Level();
	//App->SBC_Scene->Create_Resources_Group();

	//Set_Paths();

	// ------------------------------------------------------------------- 
	Load_Options* Options = new Load_Options;

	Options->Has_Area = 0;
	Options->Has_Player = 0;
	Options->Has_Camera = 0;
	Options->Has_Objects = 0;
	Options->Has_Counters = 0;

	int Int1 = 0;
	char chr_Tag1[1024];
	char chr_Tag2[1024];

	chr_Tag1[0] = 0;
	chr_Tag2[0] = 0;

	//App->Cl_Ini->SetPathName(m_Ini_Path_File_Name);

	//App->Cl_Ini->GetString("Version_Data", "Version", chr_Tag1, 1024);

	//App->Cl_Ini->GetString("Files", "Level_Name", m_Level_Name, MAX_PATH);
	//App->Cl_Ini->GetString("Files", "Project_Name", m_Project_Name, MAX_PATH);
	//App->Cl_Ini->GetString("Files", "Game_Name", App->SBC_Build->GameName, MAX_PATH, "YourGameName");


	//Options->Has_Area = App->Cl_Ini->GetInt("Options", "Areas_Count", 0, 10);
	//Options->Has_Player = App->Cl_Ini->GetInt("Options", "Players_Count", 0, 10);
	//Options->Has_Camera = App->Cl_Ini->GetInt("Options", "Cameras_Count", 0, 10);
	//Options->Has_Objects = App->Cl_Ini->GetInt("Options", "Objects_Count", 0, 10);
	//Options->Has_Counters = App->Cl_Ini->GetInt("Options", "Counters_Count", 0, 10);

	//App->SBC_Scene->UniqueID_Object_Counter = App->Cl_Ini->GetInt("Options", "Objects_ID_Count", 0, 10);
	//App->SBC_Scene->UniqueID_Counters_Count = App->Cl_Ini->GetInt("Options", "Counters_ID_Count", 0, 10);
	//App->SBC_Scene->UniqueID_Area_Count = App->Cl_Ini->GetInt("Options", "Areas_ID_Count", 0, 10);

	//App->SBC_Build->GameOptions->Show_FPS = App->Cl_Ini->GetInt("Config", "Show_FPS", 0, 10);
	//App->SBC_Build->GameOptions->FullScreen = App->Cl_Ini->GetInt("Config", "Game_FullScreen", 1, 10);
	//App->SBC_Build->GameOptions->Zipped_Assets_Flag = App->Cl_Ini->GetInt("Config", "Zipped_Assets", 1, 10);
	//App->SBC_Build->GameOptions->Front_Dialog_Flag = App->Cl_Ini->GetInt("Config", "Use_Front_Dlg", 1, 10);

	////-------------------------------------- Set Resource Path

	//Load_Get_Resource_Path();

	//// ------------------------------------- Aera
	//if (Options->Has_Area > 0)
	//{
	//	bool test = Load_Project_Aera();
	//	App->SBC_Scene->Area_Added = 1;
	//}

	//// ------------------------------------- Player
	//if (Options->Has_Player > 0)
	//{
	//	bool test = Load_Project_Player();
	//	App->SBC_DCC->Player_CanJump = App->CL_Prefs->Prefs_PlayerCanJump_Flag;
	//}

	//// ------------------------------------- Camera
	//if (Options->Has_Camera > 0)
	//{
	//	Load_Project_Camera();
	//	App->SBC_Scene->Camera_Added = 1;
	//	App->SBC_Com_Camera->Set_Camera(0);
	//	App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Cameras_Folder);
	//}

	//// ------------------------------------- Objects
	//if (Options->Has_Objects > 0)
	//{
	//	V_Load_Project_Objects();
	//	App->SBC_Objects_Create->Add_Objects_From_File();
	//}

	//// ------------------------------------- Counters
	//if (Options->Has_Counters > 0)
	//{
	//	Load_Project_Counters();
	//	App->SBC_Display->Add_Counters_From_File();
	//}


	//App->CL_Ogre->OgreListener->GD_CameraMode = Enums::CamDetached;

	//App->SBC_FileView->Change_Level_Name();
	//App->SBC_FileView->Change_Project_Name();
	//App->SBC_FileView->Redraw_FileView();

	//App->SBC_Scene->Scene_Modified = 0;

	//int Test = App->SBC_Com_Environments->Get_First_Environ();
	//if (Test == -1)
	//{
	//	App->SBC_Com_Environments->Add_New_Environ_Entity(1);
	//	int mIndex = App->SBC_Com_Environments->Get_First_Environ();
	//	App->SBC_Com_Environments->Set_First_Environment(mIndex);

	//	App->SBC_Scene->Scene_Modified = 1;
	//}
	//else
	//{
	//	App->SBC_Com_Environments->Set_First_Environment(Test);
	//}

	//App->SBC_Scene->Scene_Loaded = 1;
	//App->SBC_Project->Project_Loaded = 1;

	//delete Options;

	//App->Set_Main_TitleBar(App->SBC_FileIO->Project_Path_File_Name);
	//App->SBC_FileIO->RecentFileHistory_Update();
	//App->CL_Prefs->Update_User_File(App->SBC_FileIO->Project_Path_File_Name);

	//App->SBC_FileView->SelectItem(App->SBC_FileView->FV_LevelFolder);

	//EnableMenuItem(App->mMenu, ID_FILE_SAVEPROJECTALL, MF_GRAYED);

	return 1;
}
