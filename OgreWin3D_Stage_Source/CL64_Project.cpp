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
	m_Level_File_Name[0] = 0;
	m_Level_Folder_Path[0] = 0;

	strcpy(m_Main_Assets_Path, "None");

	strcpy(m_Project_Sub_Folder, App->GD_Directory_FullPath);
	strcat(m_Project_Sub_Folder, "\\");
	strcat(m_Project_Sub_Folder, "Projects");
	strcat(m_Project_Sub_Folder, "\\");
	strcat(m_Project_Sub_Folder, "First_Project");
	strcat(m_Project_Sub_Folder, "_Prj");

	strcpy(m_Project_Name, "First_Project");
	strcpy(m_Level_Name, "First_Level");
	
}

CL64_Project::~CL64_Project()
{
}

// *************************************************************************
// *	  		Set_Paths:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void CL64_Project::Set_Paths()
{
	strcpy(m_Level_File_Name, App->CL_File_IO->Project_File_Name);
	strcpy(m_Project_Sub_Folder, App->CL_File_IO->Project_Path_File_Name);
	strcpy(m_Ini_Path_File_Name, App->CL_File_IO->Project_Path_File_Name);

	strcpy(m_Level_Folder_Path, App->CL_File_IO->Project_Path_File_Name);

	// Get path no file 
	int len1 = strlen(m_Level_File_Name);
	int len2 = strlen(m_Project_Sub_Folder);
	strcpy(m_Project_Sub_Folder, m_Project_Sub_Folder);
	m_Project_Sub_Folder[len2 - (len1 + 1)] = 0;
	
}

// *************************************************************************
// *	  		Load_Project:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Load_Project()
{

	m_Ini_Path_File_Name[0] = 0;

	App->CL_Scene->Clear_Level();
	App->CL_Resources->Create_Project_Resources_Group();
	
	Set_Paths();

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

	App->CL_Ini_File->SetPathName(m_Ini_Path_File_Name);

	App->CL_Ini_File->GetString("Version_Data", "Version", chr_Tag1, 1024);
	
	App->CL_Ini_File->GetString("Files", "Level_Name", m_Level_Name, MAX_PATH);
	App->CL_Ini_File->GetString("Files", "Project_Name", m_Project_Name, MAX_PATH);
	//App->CL_Ini_File->GetString("Files", "Game_Name", App->SBC_Build->GameName, MAX_PATH, "YourGameName");


	Options->Has_Area = App->CL_Ini_File->GetInt("Options", "Areas_Count", 0, 10);
	Options->Has_Player = App->CL_Ini_File->GetInt("Options", "Players_Count", 0, 10);
	Options->Has_Camera = App->CL_Ini_File->GetInt("Options", "Cameras_Count", 0, 10);
	Options->Has_Objects = App->CL_Ini_File->GetInt("Options", "Objects_Count", 0, 10);
	Options->Has_Counters = App->CL_Ini_File->GetInt("Options", "Counters_Count", 0, 10);

	//App->CL_Scene->UniqueID_Object_Counter = App->Cl_Ini->GetInt("Options", "Objects_ID_Count", 0, 10);
	//App->CL_Scene->UniqueID_Counters_Count = App->Cl_Ini->GetInt("Options", "Counters_ID_Count", 0, 10);
	//App->CL_Scene->UniqueID_Area_Count = App->Cl_Ini->GetInt("Options", "Areas_ID_Count", 0, 10);

	//App->SBC_Build->GameOptions->Show_FPS = App->Cl_Ini->GetInt("Config", "Show_FPS", 0, 10);
	//App->SBC_Build->GameOptions->FullScreen = App->Cl_Ini->GetInt("Config", "Game_FullScreen", 1, 10);
	//App->SBC_Build->GameOptions->Zipped_Assets_Flag = App->Cl_Ini->GetInt("Config", "Zipped_Assets", 1, 10);
	//App->SBC_Build->GameOptions->Front_Dialog_Flag = App->Cl_Ini->GetInt("Config", "Use_Front_Dlg", 1, 10);

	////-------------------------------------- Set Resource Path

	Load_Get_Resource_Path();

	// ------------------------------------- Aera
	if (Options->Has_Area > 0)
	{
		bool test = Load_Project_Aera();
		App->CL_Scene->Flag_Area_Added = 1;
	}

	//// ------------------------------------- Player
	if (Options->Has_Player > 0)
	{
		bool test = Load_Project_Player();
		//App->SBC_DCC->Player_CanJump = App->CL_Prefs->Prefs_PlayerCanJump_Flag;
	}

	//// ------------------------------------- Camera
	//if (Options->Has_Camera > 0)
	//{
	//	Load_Project_Camera();
	//	App->CL_Scene->Camera_Added = 1;
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

	//App->CL_Scene->Scene_Modified = 0;

	//int Test = App->SBC_Com_Environments->Get_First_Environ();
	//if (Test == -1)
	//{
	//	App->SBC_Com_Environments->Add_New_Environ_Entity(1);
	//	int mIndex = App->SBC_Com_Environments->Get_First_Environ();
	//	App->SBC_Com_Environments->Set_First_Environment(mIndex);

	//	App->CL_Scene->Scene_Modified = 1;
	//}
	//else
	//{
	//	App->SBC_Com_Environments->Set_First_Environment(Test);
	//}

	//App->CL_Scene->Scene_Loaded = 1;
	//App->SBC_Project->Project_Loaded = 1;

	//delete Options;

	//App->Set_Main_TitleBar(App->SBC_FileIO->Project_Path_File_Name);
	//App->SBC_FileIO->RecentFileHistory_Update();
	//App->CL_Prefs->Update_User_File(App->SBC_FileIO->Project_Path_File_Name);

	//App->SBC_FileView->SelectItem(App->SBC_FileView->FV_LevelFolder);

	//EnableMenuItem(App->mMenu, ID_FILE_SAVEPROJECTALL, MF_GRAYED);

	return 1;
}

// *************************************************************************
// *	  	Load_Get_Resource_Path:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool CL64_Project::Load_Get_Resource_Path()
{
	
	m_Main_Assets_Path[0] = 0;

	strcpy(m_Main_Assets_Path, m_Project_Sub_Folder);
	strcat(m_Main_Assets_Path, "\\");

	strcat(m_Main_Assets_Path, m_Level_Name);
	strcat(m_Main_Assets_Path, "\\");

	strcat(m_Main_Assets_Path, "Assets");
	strcat(m_Main_Assets_Path, "\\");

	App->CL_Resources->Add_Resource_Location_Project(m_Main_Assets_Path);

	return 1;
}

// *************************************************************************
// *	  		Load_Project_Aera:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool CL64_Project::Load_Project_Aera()
{
	char Area_Ini_Path[MAX_PATH];
	char chr_Tag1[MAX_PATH];
	char Area_Name[1024];
	char Mesh_FileName[MAX_PATH];
	char Resource_Location[MAX_PATH];
	int Area_Count = 0;
	int Int_Tag = 0;

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	strcpy(Area_Ini_Path, m_Project_Sub_Folder);
	strcat(Area_Ini_Path, "\\");

	strcat(Area_Ini_Path, m_Level_Name);
	strcat(Area_Ini_Path, "\\");

	strcat(Area_Ini_Path, "Areas");
	strcat(Area_Ini_Path, "\\");

	strcat(Area_Ini_Path, "Areas.aer");

	App->CL_Ini_File->SetPathName(Area_Ini_Path);

	Area_Count = App->CL_Ini_File->GetInt("Counters", "Areas_Count", 0,10);

	int Count = 0;

	while (Count < Area_Count)
	{
		char n_buff[255];
		char buff[255];
		strcpy(buff, "Area_");
		_itoa(Count, n_buff, 10);
		strcat(buff, n_buff);

		App->CL_Scene->B_Area[Count] = new Base_Area();
		App->CL_Com_Area->Set_Area_Defaults(Count);

		Base_Area* Area = App->CL_Scene->B_Area[Count];

		App->CL_Ini_File->GetString(buff, "Area_Name", Area_Name, MAX_PATH);
		App->CL_Ini_File->GetString(buff, "Area_File", Mesh_FileName, MAX_PATH);
		App->CL_Ini_File->GetString(buff, "Area_Resource_Path", Resource_Location, MAX_PATH);

		// ------------ Position
		App->CL_Ini_File->GetString(buff, "Mesh_Pos", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		App->CL_Scene->B_Area[Count]->Mesh_Pos = Ogre::Vector3(x, y, z);

		// ------------ Scale
		int Test = App->CL_Ini_File->GetString(buff, "Mesh_Scale", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
			App->CL_Scene->B_Area[Count]->Mesh_Scale = Ogre::Vector3(x, y, z);
		}
		else
		{
			App->CL_Scene->B_Area[Count]->Mesh_Scale = Ogre::Vector3(1, 1, 1);
		}

		// ------------ Rotation
		Test = App->CL_Ini_File->GetString(buff, "Mesh_Rot", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
			App->CL_Scene->B_Area[Count]->Mesh_Rot = Ogre::Vector3(x, y, z);
		}
		else
		{
			App->CL_Scene->B_Area[Count]->Mesh_Rot = Ogre::Vector3(0, 0, 0);
		}


		// ------------ Mesh_Quat
		Test = App->CL_Ini_File->GetString(buff, "Mesh_Quat", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			int result = sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

			App->CL_Scene->B_Area[Count]->Mesh_Quat.w = w;
			App->CL_Scene->B_Area[Count]->Mesh_Quat.x = x;
			App->CL_Scene->B_Area[Count]->Mesh_Quat.y = y;
			App->CL_Scene->B_Area[Count]->Mesh_Quat.z = z;
		}
		else
		{
			App->CL_Scene->B_Area[Count]->Mesh_Quat.w = 1;
			App->CL_Scene->B_Area[Count]->Mesh_Quat.x = 0;
			App->CL_Scene->B_Area[Count]->Mesh_Quat.y = 0;
			App->CL_Scene->B_Area[Count]->Mesh_Quat.z = 0;
		}

		//App->Cl_Ini->GetString("Area_0", "Material_File", App->CL_Scene->B_Area[Count]->Material_File, MAX_PATH);

		App->CL_Com_Area->Add_Aera_To_Project(Count, Mesh_FileName, m_Main_Assets_Path);

		App->CL_Scene->B_Area[Count]->This_Object_UniqueID = App->CL_Ini_File->GetInt(buff, "Area_Object_ID", 0,10);

		strcpy(App->CL_Scene->B_Area[Count]->Area_Name, Area_Name);

		App->CL_Scene->B_Area[Count]->FileViewItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Areas_Folder, Area_Name, Count, false);

		Count++;

		App->CL_Scene->Area_Count++;
	}

	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Areas_Folder);
	App->CL_FileView->SelectItem(App->CL_Scene->B_Area[0]->FileViewItem);

	return 1;
}

// *************************************************************************
// *	  	Load_Project_Player:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Load_Project_Player()
{
	int Players_Count = 0;
	int Locations_Count = 0;
	char Player_Name[MAX_PATH];
	char Player_Ini_Path[MAX_PATH];
	char chr_Tag1[MAX_PATH];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	strcpy(Player_Ini_Path, m_Project_Sub_Folder);
	strcat(Player_Ini_Path, "\\");

	strcat(Player_Ini_Path, m_Level_Name);
	strcat(Player_Ini_Path, "\\");

	strcat(Player_Ini_Path, "Players");
	strcat(Player_Ini_Path, "\\");

	strcat(Player_Ini_Path, "Players.ply");

	App->CL_Ini_File->SetPathName(Player_Ini_Path);

	Players_Count = App->CL_Ini_File->GetInt("Counters", "Player_Count", 0,10);

	int Count = 0;
	char Cbuff[255];
	char buff[255];
	
	while (Count < Players_Count)
	{
		strcpy(buff, "Player_");
		_itoa(Count, Cbuff, 10);
		strcat(buff, Cbuff);

		
		App->CL_Player->Create_Player_Object();
		
		App->CL_Ini_File->GetString(buff, "Player_Name", Player_Name, MAX_PATH);
		strcpy(App->CL_Scene->B_Player[Count]->Player_Name, Player_Name);


		App->CL_Ini_File->GetString(buff, "Start_Position", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		App->CL_Scene->B_Player[Count]->StartPos = Ogre::Vector3(x, y, z);

		App->CL_Ini_File->GetString(buff, "Start_Rotation", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

		App->CL_Scene->B_Player[Count]->Physics_Rotation.setW(w);
		App->CL_Scene->B_Player[Count]->Physics_Rotation.setX(x);
		App->CL_Scene->B_Player[Count]->Physics_Rotation.setY(y);
		App->CL_Scene->B_Player[Count]->Physics_Rotation.setZ(z);

		App->CL_Ini_File->GetString(buff, "Turn_Rate", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f", &x);
		App->CL_Scene->B_Player[Count]->TurnRate = x;

		App->CL_Ini_File->GetString(buff, "Ground_Speed", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f", &x);
		App->CL_Scene->B_Player[Count]->Ground_speed = x;

		//------------------ Look Up Limit
		int Test = App->CL_Ini_File->GetString(buff, "Limit_Look_Up", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f", &x);
			App->CL_Scene->B_Player[Count]->Limit_Look_Up = x;
		}
		else { App->CL_Scene->B_Player[Count]->Limit_Look_Up = 45; }

		//------------------ Look Down Limit
		Test = App->CL_Ini_File->GetString(buff, "Limit_Look_Down", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f", &x);
			App->CL_Scene->B_Player[Count]->Limit_Look_Down = x;
		}
		else { App->CL_Scene->B_Player[Count]->Limit_Look_Down = -45; }

		//------------------ Capsule Height
		Test = App->CL_Ini_File->GetString(buff, "Height", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f", &x);
			App->CL_Scene->B_Player[Count]->Capsule_Height = x;
		}
		else { App->CL_Scene->B_Player[Count]->Capsule_Height = 17; }

		//------------------ Player Height
		Test = App->CL_Ini_File->GetString(buff, "Player_Height", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f", &x);
			App->CL_Scene->B_Player[Count]->PlayerHeight = x;
		}
		else { App->CL_Scene->B_Player[Count]->PlayerHeight = 16; }

		App->CL_Scene->B_Player[Count]->FileViewItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Players_Folder, Player_Name, Count, false);

		Count++;

	}

	
	// ------------------------------------------ Locations
	int Int_Tag = 0;
	Locations_Count = App->CL_Ini_File->GetInt("Locations", "Locations_Count", 0,10);

	Count = 0;
	while (Count < Locations_Count)
	{
		//if (App->CL_Scene->B_Locations[Count])
		//{
		//	delete App->CL_Scene->B_Locations[Count];
		//	App->CL_Scene->B_Locations[Count] = NULL;
		//}

		//char n_buff[255];
		//char buff[255];
		//strcpy(buff, "Location_");
		//_itoa(Count, n_buff, 10);
		//strcat(buff, n_buff);

		//App->CL_Scene->B_Locations[Count] = new Base_Locations();
		//App->CL_Scene->B_Locations[Count]->Deleted = 0;


		//Int_Tag = App->CL_Ini_File->GetInt(buff, "Locatoin_ID", 0, 10);
		//App->CL_Scene->B_Locations[Count]->This_Object_UniqueID = Int_Tag;

		//App->CL_Ini_File->GetString(buff, "Name", chr_Tag1, MAX_PATH);


		//strcpy(App->CL_Scene->B_Locations[Count]->Name, chr_Tag1);

		//// Mesh_Pos
		//App->CL_Ini_File->GetString(buff, "Mesh_Position", chr_Tag1, MAX_PATH);
		//sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		//App->CL_Scene->B_Locations[Count]->Current_Position.x = x;
		//App->CL_Scene->B_Locations[Count]->Current_Position.y = y;
		//App->CL_Scene->B_Locations[Count]->Current_Position.z = z;

		//App->CL_Ini_File->GetString(buff, "Physics_Position", chr_Tag1, MAX_PATH);
		//sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		//App->CL_Scene->B_Locations[Count]->Physics_Position.setX(x);
		//App->CL_Scene->B_Locations[Count]->Physics_Position.setY(y);
		//App->CL_Scene->B_Locations[Count]->Physics_Position.setZ(z);

		//App->CL_Ini_File->GetString(buff, "Physics_Rotation", chr_Tag1, MAX_PATH);
		//sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);
		//App->CL_Scene->B_Locations[Count]->Physics_Rotation.setW(w);
		//App->CL_Scene->B_Locations[Count]->Physics_Rotation.setX(x);
		//App->CL_Scene->B_Locations[Count]->Physics_Rotation.setY(y);
		//App->CL_Scene->B_Locations[Count]->Physics_Rotation.setZ(z);

		Count++;

	}

	/*App->CL_Scene->Player_Location_Count = Count;

	App->SBC_Physics->Reset_Physics();
	App->SBC_Physics->Enable_Physics(1);*/

	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Players_Folder);
	return 1;
}
