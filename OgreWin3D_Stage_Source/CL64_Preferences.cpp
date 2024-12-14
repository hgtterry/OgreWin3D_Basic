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
// *	Preferences_Editor_ImGui:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Preferences::Preferences_Editor_ImGui()
{

	ImGui::SetNextWindowPos(ImVec2(Preferences_Pos_X, Preferences_Pos_Y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 220), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Environment Editor", &flag_Show_Preferences_Editor, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
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
			ImGui::SetWindowPos("Environment Editor", ImVec2(Preferences_Pos_X, Preferences_Pos_Y));

			flag_Preferences_Start_Pos = 1;
		}

		ImGuiColorEditFlags misc_flags2 = (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(213, 222, 242, 255));

		//ImGui::Text("Environment:  %s", App->CL_Scene->B_Object[Eviron_Index]->Object_Name);
		ImGui::Separator();

		ImGui::Columns(2);
		ImGui::SetColumnWidth(-1, 120);

		if (ImGui::Button("Startup ", ImVec2(100, 0)))
		{
			PropertyEditor_Page = 0;
		}

		if (ImGui::Button("Sound   ", ImVec2(100, 0)))
		{
			//PropertyEditor_Page = 1;
		}

		if (ImGui::Button("Fog   ", ImVec2(100, 0)))
		{
			//PropertyEditor_Page = 2;
		}

		if (ImGui::Button("Sky   ", ImVec2(100, 0)))
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

			// ----------------- Visible
			ImGui::Text("Enable");
			ImGui::SameLine();
			/*int test = ImGui::Checkbox("##Enable", &flag_ClickOnSkyEnabled);
			if (test == 1)
			{
				if (flag_ClickOnSkyEnabled == 1)
				{
					
				}
				else
				{
					
				}

			}*/

		}

		// ---------------------------------------------------------------- Sound
		//if (PropertyEditor_Page == 1)
		//{
		//	ImGui::NextColumn();
		//	ImGui::AlignTextToFramePadding();

		//	ImGui::Selectable("Track:- ", &flag_ClickOnTrack);
		//	ImGui::SameLine();
		//	//ImGui::Text("%s", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Sound_File);

		//	if (flag_ClickOnTrack)
		//	{
		//		ImGui::TextColored(ImVec4(0.f, 1.f, 0.24f, 1.f), "ON");

		//		App->CL_SoundMgr->flag_Accessed = 1;
		//		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Sound_File);

		//		App->CL_Com_Environments->Set_Environment_By_Index(0, Eviron_Index);
		//		App->CL_SoundMgr->Dialog_SoundFile();

		//		if (App->CL_SoundMgr->flag_IsCancelled == 0)
		//		{

		//			strcpy(App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Sound_File, App->CL_SoundMgr->Access_File);
		//			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->SndVolume = App->CL_SoundMgr->SndVolume;

		//			App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);

		//			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		//		}
		//		else
		//		{
		//			App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);
		//		}

		//		flag_ClickOnTrack = 0;
		//	}

		//	// ----------------- Volume
		//	ImGui::Selectable("Volume:- ", &flag_ClickOnVolume);
		//	ImGui::SameLine();
		//	//ImGui::Text("%f", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->SndVolume);
		//	if (flag_ClickOnVolume)
		//	{
		//		App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		//		flag_ClickOnVolume = 0;
		//	}

		//	// ----------------- Play
		//	ImGui::Text("Play:");
		//	ImGui::SameLine();
		//	int test = ImGui::Checkbox("##Play", &flag_ClickOnPlay);
		//	if (test == 1)
		//	{
		//		/*if (flag_ClickOnPlay == 1)
		//		{
		//			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Play = 1;
		//			App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);
		//		}
		//		else
		//		{
		//			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Play = 0;
		//			App->CL_Com_Environments->Set_Environment_By_Index(0, Eviron_Index);
		//		}

		//		App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);*/
		//	}

		//	// ----------------- Loop
		//	ImGui::Text("Loop:");
		//	ImGui::SameLine();
		//	test = ImGui::Checkbox("##Loop", &flag_ClickOnLoop);
		//	if (test == 1)
		//	{
		//		/*if (flag_ClickOnLoop == 1)
		//		{
		//			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Loop = 1;
		//			App->CL_Com_Environments->Set_Environment_By_Index(0, Eviron_Index);
		//			App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);
		//		}
		//		else
		//		{
		//			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Loop = 0;
		//			App->CL_Com_Environments->Set_Environment_By_Index(0, Eviron_Index);
		//			App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);
		//		}

		//		App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);*/
		//	}
		//}

		// ---------------------------------------------------------------- Fog
		//if (PropertyEditor_Page == 2)
		//{
		//	ImGui::NextColumn();
		//	ImGui::AlignTextToFramePadding();

		//	// ----------------- Visible
		//	ImGui::Text("Visible");
		//	ImGui::SameLine();
		//	int test = ImGui::Checkbox("##Visible", &flag_ClickOnFogVisible);
		//	if (test == 1)
		//	{
		//		if (flag_ClickOnFogVisible == 1)
		//		{
		//			/*App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On = 1;
		//			EnableFog(true);*/
		//		}
		//		else
		//		{
		//			/*App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On = 0;
		//			EnableFog(false);*/
		//		}

		//		App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		//	}

		//	// ----------------- Mode
		//	ImGui::Selectable("Mode:- ", &flag_ClickOnFogMode);
		//	ImGui::SameLine();
		//	//ImGui::Text("%i", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_Mode);
		//	if (flag_ClickOnFogMode)
		//	{
		//		flag_ClickOnFogMode = 0;
		//	}

		//	// ----------------- Fog Colour
		//	ImGui::Text("Fog Colour:");
		//	ImGui::SameLine();

		//	if (ImGui::ColorEdit3("", (float*)&Fog_Colour, ImGuiColorEditFlags_NoInputs | misc_flags2))
		//	{
		//		/*Ambient_Int_Red = Fog_Colour.x * 255;
		//		Ambient_Int_Green = Fog_Colour.y * 255;
		//		Ambient_Int_Blue = Fog_Colour.z * 255;*/

		//		/*App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_Colour = Ogre::Vector3(Fog_Colour.x, Fog_Colour.y, Fog_Colour.z);

		//		if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On == 1)
		//		{
		//			
		//		}

		//		App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);*/
		//	}



			//// ----------------- Start
			//ImGui::Text("Start:- ", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_Start);
			//ImGui::SameLine();

			//if (ImGui::InputFloat("##1", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_Start, 1, 0, "%.3f"))
			//{
			//	if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On == 1)
			//	{
			//		
			//	}

			//	App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
			//}

			// ----------------- End
			/*ImGui::Text("End:-   ", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_End);
			ImGui::SameLine();

			if (ImGui::InputFloat("##2", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_End, 1, 0, "%.3f"))
			{
				if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On == 1)
				{

				}

				App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
			}*/

			//}

			// ---------------------------------------------------------------- Sky
			//if (PropertyEditor_Page == 3)
			//{
			//	ImGui::NextColumn();
			//	ImGui::AlignTextToFramePadding();

			//	// ----------------- Visible
			//	ImGui::Text("Enable");
			//	ImGui::SameLine();
			//	int test = ImGui::Checkbox("##Enable", &flag_ClickOnSkyEnabled);
			//	if (test == 1)
			//	{
			//		/*if (flag_ClickOnSkyEnabled == 1)
			//		{
			//			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Enabled = 1;
			//			
			//		}
			//		else
			//		{
			//			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Enabled = 0;
			//			
			//		}

			//		App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);*/
			//	}

			//	// ----------------- Tiling
			//	//ImGui::Text("Tiling:- ", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Tiling);
			//	ImGui::SameLine();

			//	/*if (ImGui::InputFloat("##5", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Tiling, 0.5, 0, "%.3f"))
			//	{
			//		if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Enabled == 1)
			//		{
			//			
			//		}

			//		App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
			//	}*/

			//	// ----------------- Curvature
			//	//ImGui::Text("Curve:- ", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Curvature);
			//	ImGui::SameLine();

			//	/*if (ImGui::InputFloat("##6", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Curvature, 0.5, 0, "%.3f"))
			//	{
			//		if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Enabled == 1)
			//		{
			//			
			//		}

			//		App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
			//	}*/

			//}

		ImGui::PopStyleVar();
		ImGui::Columns(0);

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Close", ImVec2(100, 0)))
		{
			Close_Preferences_Editor();

			ImGui::PopStyleColor();
			PropertyEditor_Page = 0;
			flag_Show_Preferences_Editor = 0;
		}

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

	/*App->Disable_Panels(false);
	App->Show_Panels(true);*/

	/*App->CL_FileView->Show_FileView(true);
	int Index = App->CL_Properties->Current_Selected_Object;
	App->CL_Com_Environments->Set_Environment_By_Index(0, Index);

	Index = App->CL_Com_Environments->Get_First_Environ();
	App->CL_Com_Environments->Set_Environment_By_Index(0, Index);*/
}

// *************************************************************************
// *	  	Start_Preferences_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Preferences::Start_Preferences_Dlg()
{
	flag_Preferences_Start_Pos = 0;
	flag_Show_Preferences_Editor = 1;

	DialogBox(App->hInst, (LPCTSTR)IDD_PREFERENCES, App->MainHwnd, (DLGPROC)Preferences_Dlg_Proc);
}

// *************************************************************************
// *        Preferences_Dlg_Proc:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Preferences::Preferences_Dlg_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_STARTUP, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CK_SU_FULLSCREEN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_SU_DIRECTORIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_SU_LAST_PROJECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_SU_TEST_PROJECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		// Start in Full Screen App
		if (App->CL_Preferences->flag_Start_FullScreen == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULLSCREEN);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULLSCREEN);
			SendMessage(temp, BM_SETCHECK, 0, 0);
		}

		// Use Default Directories
		if (App->CL_Preferences->flag_Use_Default_Directories == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_DIRECTORIES);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_DIRECTORIES);
			SendMessage(temp, BM_SETCHECK, 0, 0);
		}

		// Load last Project
		if (App->CL_Preferences->flag_Load_Last_Project == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_LAST_PROJECT);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_LAST_PROJECT);
			SendMessage(temp, BM_SETCHECK, 0, 0);
		}

		// Load Test Project
		if (App->CL_Preferences->flag_Load_Test_Project == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_TEST_PROJECT);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_TEST_PROJECT);
			SendMessage(temp, BM_SETCHECK, 0, 0);
		}
		
		return TRUE;

	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_CK_SU_FULLSCREEN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_SU_DIRECTORIES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_ST_STARTUP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_SU_LAST_PROJECT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_CK_SU_TEST_PROJECT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_CK_SU_FULLSCREEN)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULLSCREEN);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Preferences->flag_Start_FullScreen = 1;
				return 1;
			}
			else
			{
				App->CL_Preferences->flag_Start_FullScreen = 0;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_SU_DIRECTORIES)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_DIRECTORIES);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Preferences->flag_Use_Default_Directories = 1;
				return 1;
			}
			else
			{
				App->CL_Preferences->flag_Use_Default_Directories = 0;
				return 1;
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_CK_SU_LAST_PROJECT)
		{
			HWND temp1 = GetDlgItem(hDlg, IDC_CK_SU_TEST_PROJECT);
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_LAST_PROJECT);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Preferences->flag_Load_Last_Project = 1;
				App->CL_Preferences->flag_Load_Test_Project = 0;
				SendMessage(temp1, BM_SETCHECK, 0, 0);

				return 1;
			}
			else
			{
				App->CL_Preferences->flag_Load_Last_Project = 0;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_SU_TEST_PROJECT)
		{
			HWND temp1 = GetDlgItem(hDlg, IDC_CK_SU_LAST_PROJECT);
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_TEST_PROJECT);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Preferences->flag_Load_Last_Project = 0;
				App->CL_Preferences->flag_Load_Test_Project = 1;
				SendMessage(temp1, BM_SETCHECK, 0, 0);

				return 1;
			}
			else
			{
				App->CL_Preferences->flag_Load_Test_Project = 0;
				App->CL_Preferences->flag_Load_Last_Project = 0;

				return 1;
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Preferences->Write_Preferences();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}

	return FALSE;
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
