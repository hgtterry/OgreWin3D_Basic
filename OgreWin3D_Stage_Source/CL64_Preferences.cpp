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
#include "resource.h"
#include "CL64_Preferences.h"

CL64_Preferences::CL64_Preferences(void)
{
	flag_Show_Preferences_Editor = 0;
	PropertyEditor_Page = 0;
	flag_Preferences_Start_Pos = 0;
	Preferences_Pos_X = 500;
	Preferences_Pos_Y = 500;

	flag_Start_FullScreen = 0;
	flag_Start_Full_3DWin = 0;
	flag_Use_Default_Directories = 1;
	flag_Load_Last_Project = 1;
	flag_Load_Test_Project = 0;

	strcpy(Last_Loaded_File,"No File");

	WriteData = nullptr;
}

CL64_Preferences::~CL64_Preferences(void)
{
}

// *************************************************************************
// *	  		Clean_Up:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Preferences::Clean_Up(void)
{
	
}

// *************************************************************************
// *	  	Start_Preferences_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Preferences::Start_Preferences_Dlg()
{
	App->CL_Panels->Disable_Panels(true);
	App->CL_ImGui_Dialogs->flag_Disable_Physics_Console = 1;
	App->CL_Ogre->RenderFrame(8);

	flag_Preferences_Start_Pos = 0;
	flag_Show_Preferences_Editor = 1;
}

// *************************************************************************
// *	Preferences_Editor_ImGui:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Preferences::Preferences_Editor_ImGui()
{

	ImGui::SetNextWindowPos(ImVec2(Preferences_Pos_X, Preferences_Pos_Y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 220), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Preferences Editor", &flag_Show_Preferences_Editor, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		if (flag_Preferences_Start_Pos == 0)
		{
			ImVec2 Size = ImGui::GetWindowSize();

			Preferences_Pos_X = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (350 / 2);
			Preferences_Pos_Y = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (220 / 2);
			ImGui::SetWindowPos("Preferences Editor", ImVec2(Preferences_Pos_X, Preferences_Pos_Y));

			flag_Preferences_Start_Pos = 1;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		
		ImGui::Text("Preferences");

		ImGui::Separator();

		ImGui::Columns(2);
		ImGui::SetColumnWidth(-1, 120);

		if (ImGui::Button("Startup ", ImVec2(100, 0)))
		{
			PropertyEditor_Page = 0;
		}

		if (ImGui::Button("##b1   ", ImVec2(100, 0)))
		{
			//PropertyEditor_Page = 1;
		}

		if (ImGui::Button("##b2   ", ImVec2(100, 0)))
		{
			//PropertyEditor_Page = 2;
		}

		if (ImGui::Button("##b3   ", ImVec2(100, 0)))
		{
			//PropertyEditor_Page = 3;
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// ---------------------------------------------------------------- Main Light
		if (PropertyEditor_Page == 0)
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();

			// ----------------- Full Screen
			int test = ImGui::Checkbox("Full Screen", &flag_Start_FullScreen);
			if (test == 1)
			{
				if (flag_Start_FullScreen == 1)
				{
					flag_Start_FullScreen = 1;
				}
				else
				{
					flag_Start_FullScreen = 0;
				}
			}

			// ----------------- Default Directories
			test = ImGui::Checkbox("Default Directories", &flag_Use_Default_Directories);
			if (test == 1)
			{
				if (flag_Use_Default_Directories == 1)
				{
					flag_Use_Default_Directories = 1;
				}
				else
				{
					flag_Use_Default_Directories = 0;
				}
			}

			// ----------------- Load last Project
			test = ImGui::Checkbox("Last Project", &flag_Load_Last_Project);
			if (test == 1)
			{
				if (flag_Load_Last_Project == 1)
				{
					flag_Load_Last_Project = 1;
					flag_Load_Test_Project = 0;
				}
				else
				{
					flag_Load_Last_Project = 0;
				}
			}

			ImGui::Text(" ");

			// ----------------- Test/Demo Project
			test = ImGui::Checkbox("Test/Demo Project", &flag_Load_Test_Project);
			if (test == 1)
			{
				if (flag_Load_Test_Project == 1)
				{
					flag_Load_Last_Project = 0;
					flag_Load_Test_Project = 1;

				}
				else
				{
					flag_Load_Test_Project = 0;
					flag_Load_Last_Project = 0;
				}
			}

		}

		// ---------------------------------------------------------------- 
		if (PropertyEditor_Page == 1)
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
		}

		// ---------------------------------------------------------------- 
		if (PropertyEditor_Page == 2)
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
		}

		// ---------------------------------------------------------------- 
		if (PropertyEditor_Page == 3)
		{

		}

		ImGui::PopStyleVar();
		ImGui::Columns(0);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGuiStyle& style = ImGui::GetStyle();

		float size = ImGui::CalcTextSize("Close").x + style.FramePadding.x * 2.0f;
		float avail = ImGui::GetContentRegionAvail().x;

		float off = (avail - size) * 0.5f;
		if (off > 0.0f)
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
		}

		if (ImGui::Button("Close", ImVec2(100, 0)))
		{
			Close_Preferences_Editor();

			ImGui::PopStyleColor();
			PropertyEditor_Page = 0;
			flag_Show_Preferences_Editor = 0;
		}

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *	 Close_Environment_Editor:- Terry and Hazel Flanigan 2024  		   *
// *************************************************************************
void CL64_Preferences::Close_Preferences_Editor()
{
	flag_Show_Preferences_Editor = 0;
	flag_Preferences_Start_Pos = 0;

	App->CL_Preferences->Write_Preferences();

	App->CL_Panels->Disable_Panels(false);
	App->CL_ImGui_Dialogs->flag_Disable_Physics_Console = 0;

}

// *************************************************************************
// *			Read_Preferences:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Preferences::Read_Preferences()
{
	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->GD_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Preferences.ini");
	
	App->CL_Ini_File->SetPathName(Preferences_Path);

	flag_Start_FullScreen = App->CL_Ini_File->GetInt("Startup", "Full_Screen", 0, 10);
	flag_Start_Full_3DWin = App->CL_Ini_File->GetInt("Startup", "Full_3DWin", 0, 10);
	flag_Use_Default_Directories = App->CL_Ini_File->GetInt("Startup", "Default_Directories", 0, 10);
	flag_Load_Last_Project = App->CL_Ini_File->GetInt("Startup", "Load_Last_Project", 0, 10);
	flag_Load_Test_Project = App->CL_Ini_File->GetInt("Startup", "Load_Test_Project", 0, 10);

	char Path_File[MAX_PATH];
	App->CL_Ini_File->GetString("Startup", "Last_Prj_Dir", Path_File, MAX_PATH);
	strcpy(Last_Loaded_File, Path_File);
}

// *************************************************************************
// *		Write_Preferences:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_Preferences::Write_Preferences()
{
	WriteData = nullptr;

	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->GD_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Preferences.ini");
	
	WriteData = fopen(Preferences_Path, "wt");
	if (!WriteData)
	{
		App->Say("Cant Open Preference File");
		return 0;
	}

	fprintf(WriteData, "%s\n", "[Startup]");
	fprintf(WriteData, "%s%i\n", "Full_Screen=", flag_Start_FullScreen);
	fprintf(WriteData, "%s%i\n", "Full_3DWin=", flag_Start_Full_3DWin);
	fprintf(WriteData, "%s%i\n", "Default_Directories=", flag_Use_Default_Directories);
	fprintf(WriteData, "%s%i\n", "Load_Last_Project=", flag_Load_Last_Project);
	fprintf(WriteData, "%s%i\n", "Load_Test_Project=", flag_Load_Test_Project);
	
	fprintf(WriteData, "%s%s\n", "Last_Prj_Dir=", Last_Loaded_File);

	fclose(WriteData);

	return 1;
}
