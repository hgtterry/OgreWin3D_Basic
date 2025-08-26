/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Project.h"

#pragma warning(disable : 4101) // TODO Temp

CL64_Project::CL64_Project()
{
	Project_File_Name[0] = 0;
	Project_Path_File_Name[0] = 0;

	m_Ini_Path_File_Name[0] = 0;
	m_Level_File_Name[0] = 0;
	m_Level_Folder_Path[0] = 0;

	strcpy(m_Main_Assets_Path, "None");
	strcpy(m_Main_TXL_Path, "None");;
	strcpy(m_Last_Main_Assets_Path, "None");

	strcpy(m_Project_Sub_Folder, App->RB_Directory_FullPath);
	strcat(m_Project_Sub_Folder, "\\");
	strcat(m_Project_Sub_Folder, "Projects");
	strcat(m_Project_Sub_Folder, "\\");
	strcat(m_Project_Sub_Folder, "First_Project");
	strcat(m_Project_Sub_Folder, "_Prj");

	strcpy(m_Project_Name, "First_Project");
	strcpy(m_Level_Name, "First_Level");

	// save
	m_Aera_Folder_Path[0] = 0;
	m_Players_Folder_Path[0] = 0;
	m_Cameras_Folder_Path[0] = 0;
	m_Objects_Folder_Path[0] = 0;
	m_Display_Folder_Path[0] = 0;
	m_Locations_Folder_Path[0] = 0;

	flag_Is_New_Project = 0;
	flag_Silence_SaveAll_Dialogs = 0;

	flag_Canceled = 0;
	WriteFile = NULL;
	
}

CL64_Project::~CL64_Project()
{
}

// *************************************************************************
// *	  			Reset_Class:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Reset_Class()
{
	strcpy(m_Project_Name, "First_Project");
	strcpy(m_Level_Name, "First_Level");

	strcpy(m_Main_Assets_Path, "None");
	strcpy(m_Main_TXL_Path, "None");;
	strcpy(m_Last_Main_Assets_Path, "None");

	return 1;
}

// *************************************************************************
// *	  Start_Save_Project_Dialog:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Project::Start_Save_Project_Dialog()
{
	flag_Canceled = 0;
	//DialogBox(App->hInst, (LPCTSTR)IDD_PROJECTSAVE, App->Fdlg, (DLGPROC)Save_Project_Dialog_Proc);
}

// *************************************************************************
// *		Save_Project_Dialog_Proc:- Terry and Hazel Flanigan 2024  	   *
// *************************************************************************
LRESULT CALLBACK CL64_Project::Save_Project_Dialog_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//SendDlgItemMessage(hDlg, IDC_STPJFOLDERPATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_STPROJECTNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_STLEVELNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_STPATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_STPN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_STLN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_BTCHANGE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BTCHANGELEVEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_CK_SP_DESKTOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));


		//SendDlgItemMessage(hDlg, IDC_BTPJBROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_STBANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		///*SendDlgItemMessage(hDlg, IDC_CKQUICKLOAD, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/

		//SetDlgItemText(hDlg, IDC_STPROJECTNAME, (LPCTSTR)App->CL_Project->m_Project_Name);
		//SetDlgItemText(hDlg, IDC_STLEVELNAME, (LPCTSTR)App->CL_Project->m_Level_Name);
		//SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CL_Project->m_Project_Sub_Folder);

		//SetDlgItemText(hDlg, IDC_STBANNER, (LPCTSTR)"Save Project As");

		////HWND temp = GetDlgItem(hDlg, IDC_CKQUICKLOAD);
		////SendMessage(temp, BM_SETCHECK, 1, 0);

		//HWND temp = GetDlgItem(hDlg, IDC_CK_SP_DESKTOP);

		//int test = SendMessage(temp, BM_SETCHECK, 1, 0);

		//strcpy(App->CL_Project->m_Project_Sub_Folder, App->CL_File_IO->DeskTop_Folder);
		//strcat(App->CL_Project->m_Project_Sub_Folder, "\\");
		//strcat(App->CL_Project->m_Project_Sub_Folder, App->CL_Project->m_Project_Name);
		//strcat(App->CL_Project->m_Project_Sub_Folder, "_Prj");

		//SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CL_Project->m_Project_Sub_Folder);

		//EnableWindow(GetDlgItem(hDlg, IDC_BTPJBROWSE), 0);
		//EnableWindow(GetDlgItem(hDlg, IDC_STPJFOLDERPATH), 0);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{

		//if (GetDlgItem(hDlg, IDC_STPJFOLDERPATH) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->Brush_White;
		//}

		//if (GetDlgItem(hDlg, IDC_STPROJECTNAME) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->Brush_White;
		//}

		//if (GetDlgItem(hDlg, IDC_STLEVELNAME) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->Brush_White;
		//}

		//if (GetDlgItem(hDlg, IDC_STPN) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 255));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		//if (GetDlgItem(hDlg, IDC_STLN) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 255));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 255));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		//if (GetDlgItem(hDlg, IDC_STPATH) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 255));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		//if (GetDlgItem(hDlg, IDC_STBANNER) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 255));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		///*if (GetDlgItem(hDlg, IDC_CKQUICKLOAD) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 255));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}*/

		//if (GetDlgItem(hDlg, IDC_CK_SP_DESKTOP) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 255));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BTCHANGE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTCHANGELEVEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTPJBROWSE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

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
		}*/

		return CDRF_DODEFAULT;
	}
	case WM_COMMAND:

		/*if (LOWORD(wParam) == IDC_CK_SP_DESKTOP)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SP_DESKTOP);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				strcpy(App->CL_Project->m_Project_Sub_Folder, App->CL_File_IO->DeskTop_Folder);
				strcat(App->CL_Project->m_Project_Sub_Folder, "\\");
				strcat(App->CL_Project->m_Project_Sub_Folder, App->CL_Project->m_Project_Name);
				strcat(App->CL_Project->m_Project_Sub_Folder, "_Prj");

				SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CL_Project->m_Project_Sub_Folder);

				EnableWindow(GetDlgItem(hDlg, IDC_BTPJBROWSE), 0);
				EnableWindow(GetDlgItem(hDlg, IDC_STPJFOLDERPATH), 0);

			}
			else
			{
				EnableWindow(GetDlgItem(hDlg, IDC_BTPJBROWSE), 1);
				EnableWindow(GetDlgItem(hDlg, IDC_STPJFOLDERPATH), 1);
			}

			return TRUE;
		}*/

		/*if (LOWORD(wParam) == IDC_BTPJBROWSE)
		{
			App->CL_File_IO->Pick_Folder();

			if (App->CL_File_IO->flag_Canceled == 1)
			{
				return TRUE;
			}

			strcpy(App->CL_Project->m_Project_Sub_Folder, App->CL_File_IO->szSelectedDir);
			strcat(App->CL_Project->m_Project_Sub_Folder, "\\");
			strcat(App->CL_Project->m_Project_Sub_Folder, App->CL_Project->m_Project_Name);
			strcat(App->CL_Project->m_Project_Sub_Folder, "_Prj");

			SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CL_Project->m_Project_Sub_Folder);

			return TRUE;
		}*/

		/*if (LOWORD(wParam) == IDC_BTCHANGE)
		{
			strcpy(App->CL_Dialogs->btext, "Change Project Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Project->m_Project_Name);

			App->CL_Dialogs->Dialog_Text(0);

			if (App->CL_Dialogs->flag_Canceled == 1)
			{
				return TRUE;
			}

			int len1 = strlen(App->CL_Project->m_Project_Sub_Folder);
			int len2 = strlen(App->CL_Project->m_Project_Name);
			App->CL_Project->m_Project_Sub_Folder[len1 - (len2 + 5)] = 0;


			strcpy(App->CL_Project->m_Project_Name, App->CL_Dialogs->Chr_Text);

			strcpy(App->CL_Project->m_Project_Sub_Folder, App->CL_Project->m_Project_Sub_Folder);
			strcat(App->CL_Project->m_Project_Sub_Folder, "\\");
			strcat(App->CL_Project->m_Project_Sub_Folder, App->CL_Project->m_Project_Name);
			strcat(App->CL_Project->m_Project_Sub_Folder, "_Prj");

			SetDlgItemText(hDlg, IDC_STPROJECTNAME, (LPCTSTR)App->CL_Project->m_Project_Name);
			SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CL_Project->m_Project_Sub_Folder);

			return TRUE;
		}*/

		/*if (LOWORD(wParam) == IDC_BTCHANGELEVEL)
		{
			strcpy(App->CL_Dialogs->btext, "Change Level Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Project->m_Level_Name);

			App->CL_Dialogs->Dialog_Text(0);
			if (App->CL_Dialogs->flag_Canceled == 1)
			{
				return TRUE;
			}

			strcpy(App->CL_Project->m_Level_Name, App->CL_Dialogs->Chr_Text);
			SetDlgItemText(hDlg, IDC_STLEVELNAME, (LPCTSTR)App->CL_Project->m_Level_Name);

			return TRUE;
		}*/

		/*if (LOWORD(wParam) == IDC_CKQUICKLOAD)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKQUICKLOAD);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->SBC_Project->Set_QuickLoad_Flag = 1;
			}
			else
			{
				App->SBC_Project->Set_QuickLoad_Flag = 0;
			}

			return TRUE;
		}*/

		if (LOWORD(wParam) == IDCANCEL)
		{
			//App->CL_Project->flag_Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			//App->CL_Project->flag_Canceled = 0;
			//App->CL_Project->Save_Project();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;

	}
	return FALSE;
}

// *************************************************************************
// *	  		Save_All:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Project::Save_All(bool Silent)
{
	flag_Silence_SaveAll_Dialogs = Silent;

	bool test = Save_Project();
	if (test == 0)
	{
		return 0;
	}

	if (flag_Silence_SaveAll_Dialogs == 0)
	{
		App->Say("Scene Saved");
	}

	return 1;
}

// *************************************************************************
// *	  		Save_Project:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Project()
{
	char CurDir[MAX_PATH];
	char* a_cwd = _getcwd(CurDir, MAX_PATH);

	strcpy(m_Project_Sub_Folder, App->CL_Level->Prj_Working_Folder);
	strcpy(m_Project_Name, App->CL_Level->MTF_JustName_NoExt);

	strcpy(m_Level_Name, "First_Level");

	bool test = Save_Project_Ini();
	if (test == 0)
	{
		return 0;
	}

	Save_Level_Folder();
	Save_Main_Asset_Folder();
	
	(void)_chdir(m_Level_Folder_Path);

	//if (App->CL_Scene->flag_Area_Added == 1)
	{
		Save_Area_Folder();
	}

	//if (App->CL_Scene->flag_Player_Added == 1)
	{
		Save_Players_Folder();
	}

	Save_Cameras_Folder();
	Save_Objects_Folder();
	Save_Display_Folder();
	Save_Locations_Folder();
	/*App->CL_FileView->Change_Level_Name();
	App->CL_FileView->Change_Project_Name();*/

	//App->CL_Com_Objects->Clear_Modified_Objects(); // Clear Altered FileView Items
	
	//App->SBC_Project->Directory_Changed_Flag = 0;

	//strcpy(Project_Path_File_Name, m_Ini_Path_File_Name);
	//App->Set_Title();

	//App->CL_File_IO->RecentFileHistory_Update();

	//strcpy(App->CL_Preferences->Last_Loaded_File, App->CL_Project->Project_Path_File_Name);
	//App->CL_Preferences->Write_Preferences();

	//if (Set_QuickLoad_Flag == 1)
	//{
	//	strcpy(App->CL_Prefs->QL_User_File, App->SBC_FileIO->Project_Path_File_Name);
	//	App->CL_Prefs->Prefs_QuickLoad_Default_f = 0;
	//	App->CL_Prefs->Write_Preferences();
	//}

	(void)_chdir(CurDir);
	
	return 1;
}

// *************************************************************************
// *	  		Save_Project_Ini:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Save_Project_Ini()
{
	m_Ini_Path_File_Name[0] = 0;

	strcpy(m_Ini_Path_File_Name, m_Project_Sub_Folder);
	strcat(m_Ini_Path_File_Name, m_Project_Name);
	strcat(m_Ini_Path_File_Name, ".ini");

	char Filename[MAX_PATH];
	strcpy(Filename, "\\");
	strcat(Filename, m_Project_Name);
	strcat(Filename, ".ini");
	
	/*if (flag_Silence_SaveAll_Dialogs == 0)
	{
		int test = App->CL_File_IO->SearchFolders(m_Project_Sub_Folder, (LPSTR)Filename);
		if (test == 1)
		{
			App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"File Exsits", (LPSTR)"Do you want to update File", (LPSTR)"");

			bool Doit = App->CL_Dialogs->flag_Canceled;
			if (Doit == 1)
			{
				return 0;
			}
		}
	}*/

	WriteFile = nullptr;

	WriteFile = fopen(m_Ini_Path_File_Name, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Files]");
	fprintf(WriteFile, "%s%s\n", "Project_Name=", m_Project_Name);
	fprintf(WriteFile, "%s%s\n", "Level_Name=", "First_Level");// m_Level_Name);
	
	//fprintf(WriteFile, "%s%s\n", "Game_Name=", App->CL_Build_Game->GameName);


	fprintf(WriteFile, "%s\n", " ");

	//int Adjusted_Areas_Count = App->CL_Com_Area->Get_Adjusted_Areas_Count();

	fprintf(WriteFile, "%s\n", "[Options]");
	//fprintf(WriteFile, "%s%i\n", "Areas_Count=", Adjusted_Areas_Count);
	//fprintf(WriteFile, "%s%i\n", "Areas_ID_Count=", App->CL_Scene->UniqueID_Area_Count);

	fprintf(WriteFile, "%s%i\n", "Players_Count=", App->CL_Scene->Player_Count);
	//fprintf(WriteFile, "%s%i\n", "Cameras_Count=", App->CL_Scene->Camera_Count);
	fprintf(WriteFile, "%s%i\n", "Objects_Count=", App->CL_Scene->Object_Count);
	fprintf(WriteFile, "%s%i\n", "Objects_ID_Count=", App->CL_Scene->UniqueID_Object_Counter);

	fprintf(WriteFile, "%s%i\n", "Locations_Count=", App->CL_Locations->Location_Count);
	fprintf(WriteFile, "%s%i\n", "Locations_ID_Count=", App->CL_Locations->UniqueID_Location_Counter);

	/*int Adjusted_Counters_Count = App->CL_LookUps->Get_Adjusted_Counters_Count();

	fprintf(WriteFile, "%s%i\n", "Counters_Count=", Adjusted_Counters_Count);
	fprintf(WriteFile, "%s%i\n", "Counters_ID_Count=", App->CL_Scene->UniqueID_Counters_Count);

	fprintf(WriteFile, "%s\n", " ");

	App->CL_Build_Game->GameOptions->flag_Show_FPS = App->CL_Build_Game->flag_Show_FPS;
	App->CL_Build_Game->GameOptions->flag_Front_Dialog = App->CL_Build_Game->flag_Use_Front_Dlg;

	fprintf(WriteFile, "%s\n", "[Config]");
	fprintf(WriteFile, "%s%i\n", "Show_FPS=", App->CL_Build_Game->GameOptions->flag_Show_FPS);
	fprintf(WriteFile, "%s%i\n", "Game_FullScreen=", App->CL_Build_Game->GameOptions->flag_FullScreen);
	fprintf(WriteFile, "%s%i\n", "Zipped_Assets=", App->CL_Build_Game->GameOptions->flag_Zipped_Assets);
	fprintf(WriteFile, "%s%i\n", "Use_Front_Dlg=", App->CL_Build_Game->GameOptions->flag_Front_Dialog);*/

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Level_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Level_Folder()
{
	strcpy(m_Level_Folder_Path, m_Project_Sub_Folder);
	strcat(m_Level_Folder_Path, m_Level_Name);

	// First Level Folder
	if (_mkdir(m_Level_Folder_Path) == 0)
	{
		(void)_chdir(m_Level_Folder_Path);
	}
	else
	{
		(void)_chdir(m_Level_Folder_Path);
	}

	return 1;
}

// *************************************************************************
// *	  	Save_Main_Asset_Folder:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Save_Main_Asset_Folder()
{
	char lastFolder[MAX_PATH];
	strcpy(lastFolder, m_Main_Assets_Path);
	
	// Clear the main Assets path
	m_Main_Assets_Path[0] = '\0';

	// Rebuild Assets path
	strcpy(m_Main_Assets_Path, m_Level_Folder_Path);
	strcat(m_Main_Assets_Path, "\\");
	strcat(m_Main_Assets_Path, "Assets");
	strcat(m_Main_Assets_Path, "\\");

	// Attempt to create the directory and change to it
	if (_mkdir(m_Main_Assets_Path) == 0)
	{
		(void)_chdir(m_Main_Assets_Path);
	}
	else
	{
		(void)_chdir(m_Main_Assets_Path);
	}
	
	Save_Assets_Data();

	// Handle project state
	if (flag_Is_New_Project == false) 
	{
		Copy_Assets(lastFolder, m_Main_Assets_Path);
	}
	else 
	{
		Load_Get_Resource_Path();
	}

	App->CL_Project->flag_Is_New_Project = false;

	// Return to Level Folder
	(void)_chdir(m_Level_Folder_Path);

	return true;
}

// *************************************************************************
// *	  		Save_Asset_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Assets_Data()
{
	Ogre::Vector3 Pos;
	char File[MAX_PATH];

	strcpy(File, m_Main_Assets_Path);
	strcat(File, "\\");
	strcat(File, "Assets.dat");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteFile, "%s\n", " ");

	//int Adjusted_Areas_Count = App->CL_Com_Area->Get_Adjusted_Areas_Count();

	fprintf(WriteFile, "%s\n", "[Counters]");
	//fprintf(WriteFile, "%s%i\n", "Areas_Count=", Adjusted_Areas_Count);

	fprintf(WriteFile, "%s\n", " ");

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Area_Folder:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
bool CL64_Project::Save_Area_Folder()
{
	m_Aera_Folder_Path[0] = 0;

	strcpy(m_Aera_Folder_Path, m_Level_Folder_Path);
	strcat(m_Aera_Folder_Path, "\\");
	strcat(m_Aera_Folder_Path, "Areas");


	if (_mkdir(m_Aera_Folder_Path) == 0)
	{
		(void)_chdir(m_Aera_Folder_Path);
	}
	else
	{
		(void)_chdir(m_Aera_Folder_Path);
	}

	Save_Areas_Data();

	(void)_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  		Save_Areas_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Areas_Data()
{
	Ogre::Vector3 Pos;
	char File[MAX_PATH];

	strcpy(File, m_Aera_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Areas.dat");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteFile, "%s\n", " ");

	//int Adjusted_Areas_Count = App->CL_Com_Area->Get_Adjusted_Areas_Count();

	fprintf(WriteFile, "%s\n", "[Counters]");
	//fprintf(WriteFile, "%s%i\n", "Areas_Count=", Adjusted_Areas_Count);

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int Count = 0;
	//while (Count < App->CL_Scene->Area_Count)
	//{
	//	if (App->CL_Scene->B_Area[Count]->flag_Deleted == 0)
	//	{
	//		strcpy(buff, "[Area_");
	//		_itoa(Count, Cbuff, 10);
	//		strcat(buff, Cbuff);
	//		strcat(buff, "]");

	//		fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

	//		fprintf(WriteFile, "%s%s\n", "Area_Name=", App->CL_Scene->B_Area[Count]->Area_Name); // Change

	//		fprintf(WriteFile, "%s%s\n", "Area_File=", App->CL_Scene->B_Area[Count]->Area_FileName);
	//		fprintf(WriteFile, "%s%s\n", "Area_Path_File=", App->CL_Scene->B_Area[Count]->Area_Path_And_FileName);
	//		fprintf(WriteFile, "%s%s\n", "Area_Resource_Path=", App->CL_Scene->B_Area[Count]->Area_Resource_Path);
	//		fprintf(WriteFile, "%s%s\n", "Material_File=", App->CL_Scene->B_Area[Count]->Material_File);
	//		fprintf(WriteFile, "%s%i\n", "Area_Object_ID=", App->CL_Scene->B_Area[Count]->This_Object_UniqueID);

	//		// ------------ Position
	//		x = App->CL_Scene->B_Area[Count]->Area_Node->getPosition().x;
	//		y = App->CL_Scene->B_Area[Count]->Area_Node->getPosition().y;
	//		z = App->CL_Scene->B_Area[Count]->Area_Node->getPosition().z;
	//		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Pos=", x, y, z);

	//		// ------------ Scale
	//		x = App->CL_Scene->B_Area[Count]->Mesh_Scale.x;
	//		y = App->CL_Scene->B_Area[Count]->Mesh_Scale.y;
	//		z = App->CL_Scene->B_Area[Count]->Mesh_Scale.z;
	//		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Scale=", x, y, z);

	//		// ------------ Mesh_Rot
	//		x = App->CL_Scene->B_Area[Count]->Mesh_Rot.x;
	//		y = App->CL_Scene->B_Area[Count]->Mesh_Rot.y;
	//		z = App->CL_Scene->B_Area[Count]->Mesh_Rot.z;
	//		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Rot=", x, y, z);

	//		// ------------ Mesh_Quat
	//		w = App->CL_Scene->B_Area[Count]->Mesh_Quat.w;
	//		x = App->CL_Scene->B_Area[Count]->Mesh_Quat.x;
	//		y = App->CL_Scene->B_Area[Count]->Mesh_Quat.y;
	//		z = App->CL_Scene->B_Area[Count]->Mesh_Quat.z;
	//		fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Mesh_Quat=", w, x, y, z);


	//		fprintf(WriteFile, "%s\n", " ");

	//	}

	//	Count++;
	//}

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Players_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Players_Folder()
{
	m_Players_Folder_Path[0] = 0;

	strcpy(m_Players_Folder_Path, m_Level_Folder_Path);
	strcat(m_Players_Folder_Path, "\\");
	strcat(m_Players_Folder_Path, "Players");


	(void)_mkdir(m_Players_Folder_Path);

	(void)_chdir(m_Players_Folder_Path);

	Save_Player_Data();

	(void)_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  	Save_Player_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Player_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	float W = 0;
	float X = 0;
	float Y = 0;
	float Z = 0;

	strcpy(File, m_Players_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Players.dat");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Counters]");
	fprintf(WriteFile, "%s%i\n", "Player_Count=", App->CL_Scene->Player_Count);

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];
	int Count = 0;

	while (Count < App->CL_Scene->Player_Count)
	{
		strcpy(buff, "[Player_");
		_itoa(Count, Cbuff, 10);
		strcat(buff, Cbuff);
		strcat(buff, "]");

		fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

		fprintf(WriteFile, "%s%s\n", "Player_Name=", App->CL_Scene->B_Player[Count]->Player_Name);

		Pos.x = App->CL_Scene->B_Player[Count]->StartPos.x;
		Pos.y = App->CL_Scene->B_Player[Count]->StartPos.y;
		Pos.z = App->CL_Scene->B_Player[Count]->StartPos.z;
		fprintf(WriteFile, "%s%f,%f,%f\n", "Start_Position=", Pos.x, Pos.y, Pos.z);

		W = App->CL_Scene->B_Player[Count]->Physics_Quat.getW();
		X = App->CL_Scene->B_Player[Count]->Physics_Quat.getX();
		Y = App->CL_Scene->B_Player[Count]->Physics_Quat.getY();
		Z = App->CL_Scene->B_Player[Count]->Physics_Quat.getZ();

		fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Start_Rotation=", W, X, Y, Z);

		fprintf(WriteFile, "%s%s\n", "Shape=", "Capsule");
		fprintf(WriteFile, "%s%f\n", "Mass=", App->CL_Scene->B_Player[Count]->Capsule_Mass);
		fprintf(WriteFile, "%s%f\n", "Radius=", App->CL_Scene->B_Player[Count]->Capsule_Radius);
		fprintf(WriteFile, "%s%f\n", "Height=", App->CL_Scene->B_Player[Count]->Capsule_Height);
		fprintf(WriteFile, "%s%f\n", "Ground_Speed=", App->CL_Scene->B_Player[Count]->Ground_speed);
		fprintf(WriteFile, "%s%f\n", "Cam_Height=", App->CL_Scene->B_Player[Count]->PlayerHeight);
		fprintf(WriteFile, "%s%f\n", "Turn_Rate=", App->CL_Scene->B_Player[Count]->TurnRate);
		fprintf(WriteFile, "%s%f\n", "Limit_Look_Up=", App->CL_Scene->B_Player[Count]->Limit_Look_Up);
		fprintf(WriteFile, "%s%f\n", "Limit_Look_Down=", App->CL_Scene->B_Player[Count]->Limit_Look_Down);
		fprintf(WriteFile, "%s%f\n", "Player_Height=", App->CL_Scene->B_Player[Count]->PlayerHeight);

		Count++;
	}

	// ---------------------------------------- Player Locations

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	fprintf(WriteFile, "%s\n", " ");
	fprintf(WriteFile, "%s\n", "[Locations]");
	
	//int RealCount = App->CL_LookUps->Player_Location_GetCount(); // Get The real Count Minus Deleted Files

	//fprintf(WriteFile, "%s%i\n", "Locations_Count=", RealCount);

	int Location = 0; // Correct for Deleted Files
	
	Count = 0;
	/*while (Count < App->CL_Scene->Player_Location_Count)
	{
		if (App->CL_Scene->B_Locations[Count]->flag_Deleted == 0)
		{
			fprintf(WriteFile, "%s\n", " ");

			char Cbuff[255];
			char buff[255];
			strcpy(buff, "[Location_");
			_itoa(Location, Cbuff, 10);
			strcat(buff, Cbuff);
			strcat(buff, "]");
			fprintf(WriteFile, "%s\n", buff);

			fprintf(WriteFile, "%s%i\n", "Locatoin_ID=", App->CL_Scene->B_Locations[Count]->This_Object_UniqueID);
			fprintf(WriteFile, "%s%s\n", "Name=", App->CL_Scene->B_Locations[Count]->Name);

			x = App->CL_Scene->B_Locations[Count]->Current_Position.x;
			y = App->CL_Scene->B_Locations[Count]->Current_Position.y;
			z = App->CL_Scene->B_Locations[Count]->Current_Position.z;
			fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Position=", x, y, z);

			x = App->CL_Scene->B_Locations[Count]->Physics_Position.getX();
			y = App->CL_Scene->B_Locations[Count]->Physics_Position.getY();
			z = App->CL_Scene->B_Locations[Count]->Physics_Position.getZ();
			fprintf(WriteFile, "%s%f,%f,%f\n", "Physics_Position=", x, y, z);

			w = App->CL_Scene->B_Locations[Count]->Physics_Rotation.getW();
			x = App->CL_Scene->B_Locations[Count]->Physics_Rotation.getX();
			y = App->CL_Scene->B_Locations[Count]->Physics_Rotation.getY();
			z = App->CL_Scene->B_Locations[Count]->Physics_Rotation.getZ();
			fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Physics_Rotation=", w, x, y, z);
			Location++;
		}

		Count++;
	}*/

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Cameras_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Cameras_Folder()
{
	m_Cameras_Folder_Path[0] = 0;

	strcpy(m_Cameras_Folder_Path, m_Level_Folder_Path);
	strcat(m_Cameras_Folder_Path, "\\");
	strcat(m_Cameras_Folder_Path, "Cameras");

	if (_mkdir(m_Cameras_Folder_Path) == 0)
	{
		(void)_chdir(m_Cameras_Folder_Path);
	}
	else
	{
		(void)_chdir(m_Cameras_Folder_Path);
	}

	Save_Cameras_Data();

	(void)_chdir(m_Level_Folder_Path); // Return to Level Folder

	return 1;
}

// *************************************************************************
// *	  		Save_Cameras_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Save_Cameras_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Cameras_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Cameras.dat");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Counters]");
	//fprintf(WriteFile, "%s%i\n", "Cameras_Count=", App->CL_Scene->Camera_Count);

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int Count = 0;
	//while (Count < App->CL_Scene->Camera_Count)
	//{
	//	strcpy(buff, "[Camera_");
	//	_itoa(Count, Cbuff, 10);
	//	strcat(buff, Cbuff);
	//	strcat(buff, "]");

	//	fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

	//	fprintf(WriteFile, "%s%s\n", "Camera_Name=", App->CL_Scene->B_Camera[Count]->Camera_Name); // Change

	//	// ---------------------------------- Camera Pos
	//	x = App->CL_Scene->B_Camera[Count]->CamPos.x;
	//	y = App->CL_Scene->B_Camera[Count]->CamPos.y;
	//	z = App->CL_Scene->B_Camera[Count]->CamPos.z;

	//	fprintf(WriteFile, "%s%f,%f,%f\n", "Camera_Pos=", x, y, z);

	//	// ---------------------------------- Camera Look At
	//	x = App->CL_Scene->B_Camera[Count]->LookAt.x;
	//	y = App->CL_Scene->B_Camera[Count]->LookAt.y;
	//	z = App->CL_Scene->B_Camera[Count]->LookAt.z;

	//	fprintf(WriteFile, "%s%f,%f,%f\n", "LookAt=", x, y, z);

	//	// ---------------------------------- Camera Quaternion
	//	w = App->CL_Scene->B_Camera[Count]->Cam_Quat.w;
	//	x = App->CL_Scene->B_Camera[Count]->Cam_Quat.x;
	//	y = App->CL_Scene->B_Camera[Count]->Cam_Quat.y;
	//	z = App->CL_Scene->B_Camera[Count]->Cam_Quat.z;

	//	fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Camera_Quat=", w, x, y, z);

	//	Count++;
	//}

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Objects_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Objects_Folder()
{
	m_Objects_Folder_Path[0] = 0;

	strcpy(m_Objects_Folder_Path, m_Level_Folder_Path);
	strcat(m_Objects_Folder_Path, "\\");
	strcat(m_Objects_Folder_Path, "Objects");

	(void) _mkdir(m_Objects_Folder_Path);
	(void) _chdir(m_Objects_Folder_Path);

	Save_Objects_Data();

	(void) _chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  		Save_Objects_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Save_Objects_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Objects_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Objects.dat");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int new_Count = 0;

	int Count = 0;
	while (Count < App->CL_Scene->Object_Count)
	{
		if (App->CL_Scene->B_Object[Count]->flag_Deleted == 0)
		{
			strcpy(buff, "[Object_");
			_itoa(new_Count, Cbuff, 10);
			strcat(buff, Cbuff);
			strcat(buff, "]");

			fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

			fprintf(WriteFile, "%s%s\n", "Mesh_Name=", App->CL_Scene->B_Object[Count]->Object_Name); // Change

			fprintf(WriteFile, "%s%s\n", "Mesh_File=", App->CL_Scene->B_Object[Count]->Mesh_FileName);
			fprintf(WriteFile, "%s%s\n", "Mesh_Resource_Path=", App->CL_Scene->B_Object[Count]->Mesh_Resource_Path);
			fprintf(WriteFile, "%s%s\n", "Material_File=", App->CL_Scene->B_Object[Count]->Material_File);
			fprintf(WriteFile, "%s%i\n", "Object_ID=", App->CL_Scene->B_Object[Count]->This_Object_UniqueID);
			fprintf(WriteFile, "%s%i\n", "Object_Type=", App->CL_Scene->B_Object[Count]->Type);
			fprintf(WriteFile, "%s%i\n", "Object_Shape=", App->CL_Scene->B_Object[Count]->Shape);
			fprintf(WriteFile, "%s%i\n", "Object_Usage=", App->CL_Scene->B_Object[Count]->Usage);

			// Position
			x = App->CL_Scene->B_Object[Count]->Mesh_Pos.x;
			y = App->CL_Scene->B_Object[Count]->Mesh_Pos.y;
			z = App->CL_Scene->B_Object[Count]->Mesh_Pos.z;
			fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Pos=", x, y, z);

			// Mesh_Scale
			x = App->CL_Scene->B_Object[Count]->Mesh_Scale.x;
			y = App->CL_Scene->B_Object[Count]->Mesh_Scale.y;
			z = App->CL_Scene->B_Object[Count]->Mesh_Scale.z;
			fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Scale=", x, y, z);

			// Mesh_Rot
			x = App->CL_Scene->B_Object[Count]->Mesh_Rot.x;
			y = App->CL_Scene->B_Object[Count]->Mesh_Rot.y;
			z = App->CL_Scene->B_Object[Count]->Mesh_Rot.z;
			fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Rot=", x, y, z);

			// Mesh_Quat
			w = App->CL_Scene->B_Object[Count]->Mesh_Quat.w;
			x = App->CL_Scene->B_Object[Count]->Mesh_Quat.x;
			y = App->CL_Scene->B_Object[Count]->Mesh_Quat.y;
			z = App->CL_Scene->B_Object[Count]->Mesh_Quat.z;
			fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Mesh_Quat=", w, x, y, z);

			// Physics_Quat
			w = App->CL_Scene->B_Object[Count]->Physics_Quat.w;
			x = App->CL_Scene->B_Object[Count]->Physics_Quat.x;
			y = App->CL_Scene->B_Object[Count]->Physics_Quat.y;
			z = App->CL_Scene->B_Object[Count]->Physics_Quat.z;
			fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Physics_Quat=", w, x, y, z);

			fprintf(WriteFile, "%s%i\n", "Dimensions_Lock=", App->CL_Scene->B_Object[Count]->flag_Dimensions_Locked);

			//---------------------------------------------------------------------------------- Message Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Obj_Usage_Message)
			{
				fprintf(WriteFile, "%s\n", "-- Message");
				fprintf(WriteFile, "%s%s\n", "Message_Text=", App->CL_Scene->B_Object[Count]->S_Message[0]->Message_Text);

				x = App->CL_Scene->B_Object[Count]->S_Message[0]->Message_PosX;
				y = App->CL_Scene->B_Object[Count]->S_Message[0]->Message_PosY;
				fprintf(WriteFile, "%s%f,%f\n", "Message_Pos=", x, y);

				// Message Counter
				fprintf(WriteFile, "%s%i\n", "Message_Counter_ID=", App->CL_Scene->B_Object[Count]->S_Message[0]->Counter_ID);
				fprintf(WriteFile, "%s%i\n", "Message_Trigger_Value=", App->CL_Scene->B_Object[Count]->S_Message[0]->Trigger_Value);
				fprintf(WriteFile, "%s%i\n", "Message_Counter_Disabled=", App->CL_Scene->B_Object[Count]->S_Message[0]->flag_Counter_Disabled);

				fprintf(WriteFile, "%s%i\n", "Message_CentreX=", App->CL_Scene->B_Object[Count]->S_Message[0]->flag_PosXCentre_Flag);
				fprintf(WriteFile, "%s%i\n", "Message_CentreY=", App->CL_Scene->B_Object[Count]->S_Message[0]->flag_PosYCentre_Flag);

				x = App->CL_Scene->B_Object[Count]->S_Message[0]->Text_Colour.x;
				y = App->CL_Scene->B_Object[Count]->S_Message[0]->Text_Colour.y;
				z = App->CL_Scene->B_Object[Count]->S_Message[0]->Text_Colour.z;
				w = App->CL_Scene->B_Object[Count]->S_Message[0]->Text_Colour.w;
				fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Message_Text_Colour=", x, y, z, w);

				x = App->CL_Scene->B_Object[Count]->S_Message[0]->BackGround_Colour.x;
				y = App->CL_Scene->B_Object[Count]->S_Message[0]->BackGround_Colour.y;
				z = App->CL_Scene->B_Object[Count]->S_Message[0]->BackGround_Colour.z;
				w = App->CL_Scene->B_Object[Count]->S_Message[0]->BackGround_Colour.w;
				fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Message_BackGround_Colour=", x, y, z, w);

				fprintf(WriteFile, "%s%i\n", "Message_Show_BackGround=", App->CL_Scene->B_Object[Count]->S_Message[0]->flag_Show_BackGround);

			}

			//---------------------------------------------------------------------------------- Sound Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Obj_Usage_Sound)
			{
				fprintf(WriteFile, "%s%s\n", "Sound_File=", App->CL_Scene->B_Object[Count]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Sound_Volume=", App->CL_Scene->B_Object[Count]->SndVolume);
			}

			//---------------------------------------------------------------------------------- Colectable Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Obj_Usage_Collectable)
			{
				fprintf(WriteFile, "%s\n", "-- Colectable");
				fprintf(WriteFile, "%s%s\n", "Col_Sound_File=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Col_Sound_Volume=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->SndVolume);
				fprintf(WriteFile, "%s%i\n", "Col_Play=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->flag_Play);
				fprintf(WriteFile, "%s%s\n", "Col_Counter_Name=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Counter_Name);
				fprintf(WriteFile, "%s%i\n", "Col_Counter_ID=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Counter_ID);
				fprintf(WriteFile, "%s%i\n", "Col_Maths=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Maths);
				fprintf(WriteFile, "%s%i\n", "Col_Value=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Value);
				fprintf(WriteFile, "%s%i\n", "Col_Disabled=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->flag_Counter_Disabled);
			}

			//---------------------------------------------------------------------------------- Move Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Obj_Usage_Move)
			{
				fprintf(WriteFile, "%s\n", "-- Move Entity");
				fprintf(WriteFile, "%s%f\n", "Move_Distance=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Move_Distance);
				fprintf(WriteFile, "%s%i\n", "Move_IsNegative=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->flag_IsNegative);

				fprintf(WriteFile, "%s%f\n", "Move_NewPos=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Newpos);
				fprintf(WriteFile, "%s%i\n", "Move_ObjectID=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Object_To_Move_Index);
				fprintf(WriteFile, "%s%s\n", "Move_ObjectName=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Object_Name);
				fprintf(WriteFile, "%s%i\n", "Move_Re_Trigger=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->flag_Re_Trigger);
				fprintf(WriteFile, "%s%f\n", "Move_Speed=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Speed);
				fprintf(WriteFile, "%s%i\n", "Move_Triggered=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->flag_Triggered);
				fprintf(WriteFile, "%s%i\n", "Move_WhatDirection=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->WhatDirection);

				// Move Sound
				fprintf(WriteFile, "%s%s\n", "Move_Sound=", App->CL_Scene->B_Object[Count]->Sound_File);
				fprintf(WriteFile, "%s%i\n", "Move_Play_Sound=", App->CL_Scene->B_Object[Count]->flag_Play_Sound);
				fprintf(WriteFile, "%s%f\n", "Move_Volume=", App->CL_Scene->B_Object[Count]->SndVolume);

				// Move Counter
				fprintf(WriteFile, "%s%i\n", "Move_Counter_ID=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Counter_ID);
				fprintf(WriteFile, "%s%i\n", "Move_Trigger_Value=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Trigger_Value);
				fprintf(WriteFile, "%s%i\n", "Move_Counter_Disabled=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->flag_Counter_Disabled);
			}

			//---------------------------------------------------------------------------------- Teleport Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Obj_Usage_Teleport)
			{
				fprintf(WriteFile, "%s\n", "-- Teleport");
				fprintf(WriteFile, "%s%s\n", "Tele_Goto=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Location_Name);

				fprintf(WriteFile, "%s%i\n", "Tele_ID=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Location_ID);

				fprintf(WriteFile, "%s%s\n", "Tele_Sound=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Tele_Volume=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->SndVolume);
				fprintf(WriteFile, "%s%i\n", "Tele_Play=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->flag_Play);

				x = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Player_Position.x;
				y = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Player_Position.y;
				z = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Player_Position.z;
				fprintf(WriteFile, "%s%f,%f,%f\n", "Tele_Mesh_Position=", x, y, z);

				x = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Physics_Position.getX();
				y = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Physics_Position.getY();
				z = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Physics_Position.getZ();
				fprintf(WriteFile, "%s%f,%f,%f\n", "Tele_Physics_Position=", x, y, z);

				w = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Physics_Rotation.getW();
				x = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Physics_Rotation.getX();
				y = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Physics_Rotation.getY();
				z = App->CL_Scene->B_Object[Count]->S_Teleport[0]->Physics_Rotation.getZ();
				fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Tele_Physics_Rotation=", w, x, y, z);

				// Teleport Counter
				fprintf(WriteFile, "%s%i\n", "Tele_Counter_ID=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Counter_ID);
				fprintf(WriteFile, "%s%i\n", "Tele_Trigger_Value=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Trigger_Value);
				fprintf(WriteFile, "%s%i\n", "Tele_Counter_Disabled=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->flag_Counter_Disabled);

				//--------------- Environment
				fprintf(WriteFile, "%s\n", "------------------------------------------------------------------------------ Teleporter Environ");
				fprintf(WriteFile, "%s%i\n", "Environ_Enabled=", App->CL_Scene->B_Object[Count]->S_Environ[0]->flag_Environ_Enabled);
				fprintf(WriteFile, "%s%s\n", "Environment_Name=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environment_Name);
				fprintf(WriteFile, "%s%i\n", "Environment_ID=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environment_ID);

				//--------------- Sound
				fprintf(WriteFile, "%s%s\n", "Sound_File=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Snd_Volume=", App->CL_Scene->B_Object[Count]->S_Environ[0]->SndVolume);

				fprintf(WriteFile, "%s%i\n", "Sound_Play=", App->CL_Scene->B_Object[Count]->S_Environ[0]->flag_Play);
				fprintf(WriteFile, "%s%i\n", "Sound_Loop=", App->CL_Scene->B_Object[Count]->S_Environ[0]->flag_Loop);

				//--------------- Light

				x = App->CL_Scene->B_Object[Count]->S_Environ[0]->AmbientColour.x;
				y = App->CL_Scene->B_Object[Count]->S_Environ[0]->AmbientColour.y;
				z = App->CL_Scene->B_Object[Count]->S_Environ[0]->AmbientColour.z;
				fprintf(WriteFile, "%s%f,%f,%f\n", "Ambient_Colour=", x, y, z);

				x = App->CL_Scene->B_Object[Count]->S_Environ[0]->Light_Position.x;
				y = App->CL_Scene->B_Object[Count]->S_Environ[0]->Light_Position.y;
				z = App->CL_Scene->B_Object[Count]->S_Environ[0]->Light_Position.z;
				fprintf(WriteFile, "%s%f,%f,%f\n", "Light_Position=", x, y, z);

				//--------------- Sky
				fprintf(WriteFile, "%s%i\n", "Sky_Enable=", App->CL_Scene->B_Object[Count]->S_Environ[0]->flag_Enabled);
				fprintf(WriteFile, "%s%i\n", "Sky_Type=", App->CL_Scene->B_Object[Count]->S_Environ[0]->type);
				fprintf(WriteFile, "%s%s\n", "Sky_Material=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Material);
				fprintf(WriteFile, "%s%f\n", "Sky_Curvature=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Curvature);
				fprintf(WriteFile, "%s%f\n", "Sky_Tiling=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Tiling);
				fprintf(WriteFile, "%s%f\n", "Sky_Distance=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Distance);

				//--------------- Fog
				fprintf(WriteFile, "%s%i\n", "Fog_On=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_On);
				fprintf(WriteFile, "%s%i\n", "Fog_Mode=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Mode);

				x = App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Colour.x;
				y = App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Colour.y;
				z = App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Colour.z;
				fprintf(WriteFile, "%s%f,%f,%f\n", "Fog_Colour=", x, y, z);

				fprintf(WriteFile, "%s%f\n", "Fog_Start=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Start);
				fprintf(WriteFile, "%s%f\n", "Fog_End=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_End);
				fprintf(WriteFile, "%s%f\n", "Fog_Density=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Density);

			}

			//---------------------------------------------------------------------------------- Environ Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Obj_Usage_EnvironEntity)
			{
				fprintf(WriteFile, "%s\n", "--------------------------------------------------------------------------------- EnvironEntity");
				fprintf(WriteFile, "%s%s\n", "Environment_Name=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environment_Name);
				fprintf(WriteFile, "%s%i\n", "Environment_ID=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environment_ID);

				//--------------- Sound
				fprintf(WriteFile, "%s%s\n", "Sound_File=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Snd_Volume=", App->CL_Scene->B_Object[Count]->S_Environ[0]->SndVolume);

				fprintf(WriteFile, "%s%i\n", "Sound_Play=", App->CL_Scene->B_Object[Count]->S_Environ[0]->flag_Play);
				fprintf(WriteFile, "%s%i\n", "Sound_Loop=", App->CL_Scene->B_Object[Count]->S_Environ[0]->flag_Loop);

				//--------------- Light

				x = App->CL_Scene->B_Object[Count]->S_Environ[0]->AmbientColour.x;
				y = App->CL_Scene->B_Object[Count]->S_Environ[0]->AmbientColour.y;
				z = App->CL_Scene->B_Object[Count]->S_Environ[0]->AmbientColour.z;
				fprintf(WriteFile, "%s%f,%f,%f\n", "Ambient_Colour=", x, y, z);

				x = App->CL_Scene->B_Object[Count]->S_Environ[0]->Light_Position.x;
				y = App->CL_Scene->B_Object[Count]->S_Environ[0]->Light_Position.y;
				z = App->CL_Scene->B_Object[Count]->S_Environ[0]->Light_Position.z;
				fprintf(WriteFile, "%s%f,%f,%f\n", "Light_Position=", x, y, z);

				//--------------- Sky
				fprintf(WriteFile, "%s%i\n", "Sky_Enable=", App->CL_Scene->B_Object[Count]->S_Environ[0]->flag_Enabled);
				fprintf(WriteFile, "%s%i\n", "Sky_Type=", App->CL_Scene->B_Object[Count]->S_Environ[0]->type);
				fprintf(WriteFile, "%s%s\n", "Sky_Material=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Material);
				fprintf(WriteFile, "%s%f\n", "Sky_Curvature=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Curvature);
				fprintf(WriteFile, "%s%f\n", "Sky_Tiling=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Tiling);
				fprintf(WriteFile, "%s%f\n", "Sky_Distance=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Distance);

				//--------------- Fog
				fprintf(WriteFile, "%s%i\n", "Fog_On=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_On);
				fprintf(WriteFile, "%s%i\n", "Fog_Mode=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Mode);

				x = App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Colour.x;
				y = App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Colour.y;
				z = App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Colour.z;
				fprintf(WriteFile, "%s%f,%f,%f\n", "Fog_Colour=", x, y, z);

				fprintf(WriteFile, "%s%f\n", "Fog_Start=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Start);
				fprintf(WriteFile, "%s%f\n", "Fog_End=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_End);
				fprintf(WriteFile, "%s%f\n", "Fog_Density=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Fog_Density);

			}

			//---------------------------------------------------------------------------------- Particle
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Obj_Usage_Particle)
			{
				fprintf(WriteFile, "%s\n", "-- Particle");
				fprintf(WriteFile, "%s%s\n", "Particle_Script=", App->CL_Scene->B_Object[Count]->S_Particle[0]->ParticleScript);
				fprintf(WriteFile, "%s%f\n", "Particle_SpeedFactor=", App->CL_Scene->B_Object[Count]->S_Particle[0]->SpeedFactor);

			}

			fprintf(WriteFile, "%s\n", " ");
			fprintf(WriteFile, "%s\n", "*************************************************************************************");

			new_Count++;
		}

		Count++;
	}

	fprintf(WriteFile, "%s\n", "[Counters]");
	fprintf(WriteFile, "%s%i\n", "Objects_Count=", new_Count);

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Display_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Display_Folder()
{
	m_Display_Folder_Path[0] = 0;

	strcpy(m_Display_Folder_Path, m_Level_Folder_Path);
	strcat(m_Display_Folder_Path, "\\");
	strcat(m_Display_Folder_Path, "Display");

	(void)_mkdir(m_Display_Folder_Path);
	(void)_chdir(m_Display_Folder_Path);

	Save_Display_Data();

	(void)_chdir(m_Level_Folder_Path); // Return to Level Folder

	return 1;
}

// *************************************************************************
// *	  		Save_Display_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Save_Display_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Display_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Counters.dat");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int new_Count = 0;

	int Count = 0;
	//while (Count < App->CL_Scene->Counters_Count)
	//{
	//	if (App->CL_Scene->B_Counter[Count]->flag_Deleted == 0)
	//	{
	//		strcpy(buff, "[Counter_");
	//		_itoa(new_Count, Cbuff, 10);
	//		strcat(buff, Cbuff);
	//		strcat(buff, "]");

	//		fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

	//		fprintf(WriteFile, "%s%s\n", "Counter_Name=", App->CL_Scene->B_Counter[Count]->Panel_Name); // Change
	//		fprintf(WriteFile, "%s%i\n", "Counter_ID=", App->CL_Scene->B_Counter[Count]->Unique_ID);

	//		x = App->CL_Scene->B_Counter[Count]->PosX;
	//		y = App->CL_Scene->B_Counter[Count]->PosY;
	//		fprintf(WriteFile, "%s%f,%f\n", "Counter_Pos=", x, y);

	//		fprintf(WriteFile, "%s%s\n", "Counter_Text=", App->CL_Scene->B_Counter[Count]->Text);
	//		fprintf(WriteFile, "%s%i\n", "Counter_Display=", App->CL_Scene->B_Counter[Count]->flag_Show_Panel_Flag);
	//		fprintf(WriteFile, "%s%i\n", "Counter_Start=", App->CL_Scene->B_Counter[Count]->Start_Value);

	//		fprintf(WriteFile, "%s\n", " ");
	//		new_Count++;
	//	}

	//	Count++;
	//}

	fprintf(WriteFile, "%s\n", "[Counters]");
	fprintf(WriteFile, "%s%i\n", "Counters_Count=", new_Count);

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Locations_Folder:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Project::Save_Locations_Folder()
{
	m_Locations_Folder_Path[0] = 0;

	strcpy(m_Locations_Folder_Path, m_Level_Folder_Path);
	strcat(m_Locations_Folder_Path, "\\Locations");
	
	(void)_mkdir(m_Locations_Folder_Path);
	(void)_chdir(m_Locations_Folder_Path);

	Save_Locations_Data();

	(void)_chdir(m_Level_Folder_Path); // Return to Level Folder

	return 1;
}

// *************************************************************************
// *	  	Save_Locations_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Save_Locations_Data()
{
	std::string filePath = m_Locations_Folder_Path + std::string("\\Locations.dat");

	WriteFile = nullptr;

	WriteFile = fopen(filePath.c_str(), "wt");

	if (!WriteFile)
	{
		App->Say("Cannot create file");
		App->Say_Win(filePath.c_str());
		return false;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float W = 0;
	float X = 0;
	float Y = 0;
	float Z = 0;

	int new_Count = 0;

	int Count = 0;
	while (Count < App->CL_Locations->Location_Count)
	{
		if (App->CL_Locations->B_Location[Count]->flag_Deleted == false)
		{
			auto& m_Location = App->CL_Locations->B_Location[Count];

			strcpy(buff, "[Location_");
			_itoa(new_Count, Cbuff, 10);
			strcat(buff, Cbuff);
			strcat(buff, "]");

			fprintf(WriteFile, "%s\n", buff);

			fprintf(WriteFile, "%s%s\n", "Location_Name=", m_Location->Location_Name);
			fprintf(WriteFile, "%s%i\n", "Location UniqueID=", m_Location->Location_UniqueID);

			fprintf(WriteFile, "%s%f\n", "Camera_Pitch=", m_Location->Camera_Pitch);

			fprintf(WriteFile, "%s%f,%f,%f\n", "Location_Pos=", m_Location->Physics_Pos.x, m_Location->Physics_Pos.y, m_Location->Physics_Pos.z);
			
			W = m_Location->Physics_Quat.getW();
			X = m_Location->Physics_Quat.getX();
			Y = m_Location->Physics_Quat.getY();
			Z = m_Location->Physics_Quat.getZ();

			fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Location_Rotation=", W, X, Y, Z);

			fprintf(WriteFile, "%s\n", " ");

			new_Count++;
		}

		Count++;
	}

	fprintf(WriteFile, "%s\n", "[Locations]");
	fprintf(WriteFile, "%s%i\n", "Locations_Count=", new_Count);

	fclose(WriteFile);

	return true;
}

// *************************************************************************
// *	  		Set_Paths:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Project::Set_Paths() 
{
	std::string levelFileName = Project_File_Name;
	std::string projectSubFolder = Project_Path_File_Name;
	std::string iniPathFileName = Project_Path_File_Name;

	// Set the level folder path
	std::string levelFolderPath = Project_Path_File_Name;

	// Get path without file
	size_t len1 = levelFileName.length();
	size_t len2 = projectSubFolder.length();

	// Check
	if (len2 > (len1 + 1)) 
	{
		projectSubFolder.resize(len2 - (len1 + 1));
	}
	else 
	{
		projectSubFolder.clear(); // Clear if the condition is not met
	}

	// Update
	strcpy(m_Level_File_Name, levelFileName.c_str());
	strcpy(m_Project_Sub_Folder, projectSubFolder.c_str());
	strcpy(m_Ini_Path_File_Name, iniPathFileName.c_str());
	strcpy(m_Level_Folder_Path, levelFolderPath.c_str());
}

// *************************************************************************
// *	  		Load_Last_Project:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Load_Last_Project(char* Path_And_File)
{
	bool test = Reload_From_ResentFiles(Path_And_File);

	if (test == 1)
	{
		//App->CL_Scene->Set_Scene();
		App->CL_Ogre->RenderFrame(8);
		return 1;
	}

	return 0;
}

// *************************************************************************
// *	Reload_FromResentFiles:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Reload_From_ResentFiles(char* ResentPathAndFile)
{

	// Check Recent File Exsists
	//bool Result = App->CL_File_IO->Check_File_Exist(ResentPathAndFile);
	//if (Result == 0)
	//{
	//	App->Say(" Can Not Find File", (LPSTR)"This may be due to it has been deleted or renamed.");
	//	return 0;
	//}

	//strcpy(App->CL_Project->Project_Path_File_Name, ResentPathAndFile);

	//std::string mJustFileName = App->CL_Utilities->Get_FileName_From_Path(ResentPathAndFile);

	//strcpy(App->CL_Project->Project_File_Name, mJustFileName.c_str());

	//bool test = App->CL_Project->Load_Project();

	////App->CL_Scene->Set_Scene();
	//App->CL_Ogre->RenderFrame(8);

	////App->SBC_TopTabs->Project_Loaded_Reset();

	//if (test == 1)
	//{
	//	App->Say("Project Loaded", App->CL_Project->Project_File_Name);
	//}
	//else
	//{
	//	App->Say("Project Failed to Load");
	//}

	return 1;
}

// *************************************************************************
// *	  		Load_Project:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Load_Project()
{
	strcpy(m_Project_Sub_Folder, App->CL_Level->Prj_Working_Folder);
	
	m_Ini_Path_File_Name[0] = 0;

	strcpy(m_Ini_Path_File_Name, m_Project_Sub_Folder);
	strcat(m_Ini_Path_File_Name, App->CL_Level->MTF_JustName_NoExt);
	strcat(m_Ini_Path_File_Name, ".ini");

	//App->CL_Scene->Clear_Level();
	
	//App->CL_Resources->Create_Project_Resources_Group();*/
	
	//Set_Paths();

	// ------------------------------------------------------------------- 
	Load_Options* Options = new Load_Options;

	Options->Has_Area = 0;
	Options->Has_Player = 0;
	Options->Has_Camera = 0;
	Options->Has_Objects = 0;
	Options->Has_Counters = 0;
	Options->Has_Locations = 0;

	int Int1 = 0;
	char chr_Tag1[MAX_PATH];
	char chr_Tag2[MAX_PATH];

	chr_Tag1[0] = 0;
	chr_Tag2[0] = 0;

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(m_Ini_Path_File_Name);

	Ini_File->GetString("Version_Data", "Version", chr_Tag1, MAX_PATH);
	
	Ini_File->GetString("Files", "Level_Name", m_Level_Name, MAX_PATH);
	Ini_File->GetString("Files", "Project_Name", m_Project_Name, MAX_PATH);
	
	strcpy(m_Level_Name, "First_Level");

	//App->CL_Ini_File->GetString("Files", "Game_Name", App->CL_Build_Game->GameName, MAX_PATH, "YourGameName");


	Options->Has_Area = Ini_File->GetInt("Options", "Areas_Count", 0, 10);
	Options->Has_Player = Ini_File->GetInt("Options", "Players_Count", 0, 10);
	Options->Has_Camera = Ini_File->GetInt("Options", "Cameras_Count", 0, 10);
	Options->Has_Objects = Ini_File->GetInt("Options", "Objects_Count", 0, 10);
	Options->Has_Counters = Ini_File->GetInt("Options", "Counters_Count", 0, 10);
	Options->Has_Locations = Ini_File->GetInt("Options", "Locations_Count", 0, 10);

	App->CL_Scene->UniqueID_Object_Counter = Ini_File->GetInt("Options", "Objects_ID_Count", 0, 10);
	App->CL_Locations->UniqueID_Location_Counter = Ini_File->GetInt("Options", "Locations_ID_Count", 0, 10);
	
	
	/*App->CL_Scene->UniqueID_Counters_Count = App->CL_Ini_File->GetInt("Options", "Counters_ID_Count", 0, 10);
	App->CL_Scene->UniqueID_Area_Count = App->CL_Ini_File->GetInt("Options", "Areas_ID_Count", 0, 10);

	App->CL_Build_Game->GameOptions->flag_Show_FPS = App->CL_Ini_File->GetInt("Config", "Show_FPS", 0, 10);
	App->CL_Build_Game->GameOptions->flag_FullScreen = App->CL_Ini_File->GetInt("Config", "Game_FullScreen", 1, 10);
	App->CL_Build_Game->GameOptions->flag_Zipped_Assets = App->CL_Ini_File->GetInt("Config", "Zipped_Assets", 1, 10);
	App->CL_Build_Game->GameOptions->flag_Front_Dialog = App->CL_Ini_File->GetInt("Config", "Use_Front_Dlg", 1, 10);

	App->CL_Build_Game->flag_Show_FPS = App->CL_Build_Game->GameOptions->flag_Show_FPS;
	App->CL_Build_Game->flag_Use_Front_Dlg = App->CL_Build_Game->GameOptions->flag_Front_Dialog;*/

	//-------------------------------------- Set Resource Path
	Load_Get_Resource_Path();

	// ------------------------------------- Aera
	if (Options->Has_Area > 0)
	{
		bool test = Load_Project_Aera();
		//App->CL_Scene->flag_Area_Added = 1;
	}

	//// ------------------------------------- Player
	if (Options->Has_Player > 0)
	{
		if (App->CL_Scene->Player_Count == 0)
		{
			App->CL_Com_Player->Create_New_Player("Dummy_Player", true);
		}

		bool test = Load_Project_Player();
		App->CL_Com_Player->Reset_Player();
	}

	// ------------------------------------- Camera
	if (Options->Has_Camera > 0)
	{
		Load_Project_Camera();
		//App->CL_Scene->flag_Camera_Added = 1;
		//App->CL_Com_Cameras->Set_Camera(0);
		//App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Cameras_Folder);
	}

	// ------------------------------------- Objects
	if (Options->Has_Objects > 0)
	{
		Load_Project_Objects();
		App->CL_Objects_Create->Add_Objects_From_File();
	}

	// ------------------------------------- Counters
	if (Options->Has_Counters > 0)
	{
		Load_Project_Counters();
		//App->CL_Com_Counters->Add_Counters_From_File();
	}

	// ------------------------------------- Locations
	if (Options->Has_Locations > 0)
	{
		Load_Project_Locations();
		App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Locations_Folder);
	}
	else
	{
		// Add first location Player start
		App->CL_Locations->Add_New_Location(true);
	}


	App->CL_FileView->Change_Level_Name();
	App->CL_FileView->Change_Project_Name();

	App->CL_FileView->Enable_Environment_Button(true);

	delete Options;

	HTREEITEM Temp = App->CL_Scene->B_Player[0]->FileViewItem;
	App->CL_FileView->SelectItem(Temp);

	return 1;

	//App->CL_FileView->Redraw_FileView();

	//int Test = App->CL_Com_Environments->Get_First_Environ();
	//if (Test == -1)
	//{
	//	/*App->CL_Com_Environments->Add_New_Environ_Entity(1);
	//	int mIndex = App->CL_Com_Environments->Get_First_Environ();
	//	App->CL_Com_Environments->Set_First_Environment(mIndex);

	//	App->CL_Scene->flag_Scene_Modified = 1;*/
	//	Debug
	//}
	//else
	//{
	//	App->CL_Com_Environments->Set_First_Environment(Test);
	//}

	//App->CL_Scene->flag_Scene_Modified = 0;
	//App->CL_Scene->flag_Scene_Loaded = 1;
	
	

	//App->Set_Title();

	//App->CL_Com_MoveEntity->Adjust_Object_To_Move();

	//strcpy(App->CL_Preferences->Last_Loaded_File, App->CL_Project->Project_Path_File_Name);
	//App->CL_Preferences->Write_Preferences();

	//App->CL_Project->Project_Path_File_Name
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
	// Initialize the main assets path
	m_Main_Assets_Path[0] = '\0';

	// Construct the main assets path
	snprintf(m_Main_Assets_Path, sizeof(m_Main_Assets_Path), "%s%s\\Assets\\", m_Project_Sub_Folder, m_Level_Name);

	// Construct the texture Zip File path
	snprintf(m_Main_TXL_Path, sizeof(m_Main_TXL_Path), "%sTXL_Texture.Zip", m_Main_Assets_Path);

	// Add the resource location to the project
	App->CL_Resources->Add_Resource_Location_Project(m_Main_Assets_Path);

	return true;
}

// *************************************************************************
// *	  	Load_Project_Counters:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Load_Project_Counters()
{
	char Object_Ini_Path[MAX_PATH];
	char chr_Tag1[MAX_PATH];
	int Counters_Count = 0;

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	strcpy(Object_Ini_Path, m_Project_Sub_Folder);
	strcat(Object_Ini_Path, "\\");

	strcat(Object_Ini_Path, m_Level_Name);
	strcat(Object_Ini_Path, "\\");

	strcat(Object_Ini_Path, "Display");
	strcat(Object_Ini_Path, "\\");

	//---------------------------------------------------

	strcat(Object_Ini_Path, "Counters.edf");

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(Object_Ini_Path);

	Counters_Count = Ini_File->GetInt("Counters", "Counters_Count", 0, 10);

	int Count = 0;

	//while (Count < Counters_Count)
	//{
	//	App->CL_Scene->B_Counter[Count] = new Base_Counter();
	//	//App->SBC_Display->Set_Counter_Defaults(Count);

	//	char n_buff[255];
	//	char buff[255];
	//	strcpy(buff, "Counter_");
	//	_itoa(Count, n_buff, 10);
	//	strcat(buff, n_buff);

	//	App->CL_Ini_File->GetString(buff, "Counter_Name", chr_Tag1, MAX_PATH);
	//	strcpy(App->CL_Scene->B_Counter[Count]->Panel_Name, chr_Tag1);


	//	App->CL_Scene->B_Counter[Count]->Unique_ID = App->CL_Ini_File->GetInt(buff, "Counter_ID", 0, 10);

	//	App->CL_Ini_File->GetString(buff, "Counter_Pos", chr_Tag1, MAX_PATH);
	//	(void)sscanf(chr_Tag1, "%f,%f", &x, &y);
	//	App->CL_Scene->B_Counter[Count]->PosX = x;
	//	App->CL_Scene->B_Counter[Count]->PosY = y;

	//	App->CL_Ini_File->GetString(buff, "Counter_Text", chr_Tag1, MAX_PATH);
	//	strcpy(App->CL_Scene->B_Counter[Count]->Text, chr_Tag1);

	//	App->CL_Scene->B_Counter[Count]->Set_ImGui_Panel_Name();


	//	App->CL_Scene->B_Counter[Count]->flag_Show_Panel_Flag = App->CL_Ini_File->GetInt(buff, "Counter_Display", 0, 10);

	//	App->CL_Scene->B_Counter[Count]->Start_Value = App->CL_Ini_File->GetInt(buff, "Counter_Start", 0, 10);

	//	Count++;
	//}

	//App->CL_Scene->Counters_Count = Count;

	return 1;
}

// *************************************************************************
// *	  	Load_Project_Locations:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Load_Project_Locations()
{
	char Object_Ini_Path[MAX_PATH];
	char chr_Tag1[MAX_PATH];
	char mSection[MAX_PATH];
	int Locations_Count = 0;

	// Initialize position and rotation variables
	float w = 0, x = 0, y = 0, z = 0;
	
	strcpy(Object_Ini_Path, m_Project_Sub_Folder);
	strcat(Object_Ini_Path, "\\");

	strcat(Object_Ini_Path, m_Level_Name);
	strcat(Object_Ini_Path, "\\Locations\\Locations.dat");
	
	//---------------------------------------------------
	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(Object_Ini_Path);

	Locations_Count = Ini_File->GetInt("Locations", "Locations_Count", 0, 10);

	int count = 0;

	while (count < Locations_Count)
	{
		// Create location and set defaults
		App->CL_Locations->B_Location[count] = new Base_Location();
		App->CL_Locations->Set_Location_Defaults(count);

		auto& m_Location = App->CL_Locations->B_Location[count];

		snprintf(mSection, sizeof(mSection), "Location_%d", count);

		// Load location name
		Ini_File->GetString(mSection, "Location_Name", chr_Tag1, MAX_PATH);
		strcpy(m_Location->Location_Name, chr_Tag1);

		// Load location UniqueID
		m_Location->Location_UniqueID = Ini_File->GetInt(mSection, "Location UniqueID", 0, 10);
		
		// Load Camera Pitch
		Ini_File->GetString(mSection, "Camera_Pitch", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f", &x);
		m_Location->Camera_Pitch = x;

		// Load position
		Ini_File->GetString(mSection, "Location_Pos", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		m_Location->Physics_Pos = Ogre::Vector3(x, y, z);

		// Load rotation
		Ini_File->GetString(mSection, "Location_Rotation", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);
		m_Location->Physics_Quat.setW(w);
		m_Location->Physics_Quat.setX(x);
		m_Location->Physics_Quat.setY(y);
		m_Location->Physics_Quat.setZ(z);

		// Add item to the file view
		HTREEITEM tempItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Locations_Folder, m_Location->Location_Name, count, false);
		m_Location->FileViewItem = tempItem;

		count++;
	}

	// Update the total count of locations
	App->CL_Locations->Location_Count = count;

	return true;
}

// *************************************************************************
// *	  	Load_Project_Objects:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project::Load_Project_Objects()
{
	int Int_Tag = 0;
	char Object_Ini_Path[MAX_PATH];
	char chr_Tag1[MAX_PATH];
	int Object_Count = 0;

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	strcpy(Object_Ini_Path, m_Project_Sub_Folder);
	strcat(Object_Ini_Path, "\\");

	strcat(Object_Ini_Path, m_Level_Name);
	strcat(Object_Ini_Path, "\\");

	strcat(Object_Ini_Path, "Objects");
	strcat(Object_Ini_Path, "\\");

	//App->Say_Win(Object_Ini_Path);
	//---------------------------------------------------

	strcat(Object_Ini_Path, "Objects.dat");

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(Object_Ini_Path);

	Object_Count = Ini_File->GetInt("Counters", "Objects_Count", 0, 10);

	int Count = 0;
	while (Count < Object_Count)
	{
		char mNumChr[MAX_PATH] = { 0 };
		char mSection[MAX_PATH] = { 0 };

		strcpy(mSection, "Object_");
		_itoa(Count, mNumChr, 10);
		strcat(mSection, mNumChr);

		// Create Object 
		App->CL_Scene->B_Object.push_back(new Base_Object());
		Base_Object* B_Object = App->CL_Scene->B_Object[Count];

		Ini_File->GetString(mSection, "Mesh_Name", B_Object->Object_Name, MAX_PATH);
		Ini_File->GetString(mSection, "Mesh_File", B_Object->Mesh_FileName, MAX_PATH);
		Ini_File->GetString(mSection, "Mesh_Resource_Path", B_Object->Mesh_Resource_Path, MAX_PATH);
		Ini_File->GetString(mSection, "Material_File", B_Object->Material_File, MAX_PATH);

		// ------------- Types
		B_Object->This_Object_UniqueID = Ini_File->GetInt(mSection, "Object_ID", 0, 10);
		B_Object->Type = Ini_File->GetInt(mSection, "Object_Type", 0, 10);
		B_Object->Shape = Ini_File->GetInt(mSection, "Object_Shape", 0, 10);
		B_Object->Usage = Ini_File->GetInt(mSection, "Object_Usage", 0, 10);

		// ------------- Pos
		Ini_File->GetString(mSection, "Mesh_Pos", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		B_Object->Mesh_Pos = Ogre::Vector3(x, y, z);

		// ------------- Scale
		Ini_File->GetString(mSection, "Mesh_Scale", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		B_Object->Mesh_Scale = Ogre::Vector3(x, y, z);

		// ------------- Rotation
		Ini_File->GetString(mSection, "Mesh_Rot", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		B_Object->Mesh_Rot = Ogre::Vector3(x, y, z);

		// ------------- Mesh_Quat
		Ini_File->GetString(mSection, "Mesh_Quat", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

		B_Object->Mesh_Quat.w = w;
		B_Object->Mesh_Quat.x = x;
		B_Object->Mesh_Quat.y = y;
		B_Object->Mesh_Quat.z = z;

		// ------------- Physics_Quat
		Ini_File->GetString(mSection, "Physics_Quat", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

		B_Object->Physics_Quat.w = w;
		B_Object->Physics_Quat.x = x;
		B_Object->Physics_Quat.y = y;
		B_Object->Physics_Quat.z = z;

		B_Object->flag_Dimensions_Locked = Ini_File->GetInt(mSection, "Dimensions_Lock", 0,10);

		// Deal with Object Sections
		switch (B_Object->Usage) 
		{
		case Enums::Obj_Usage_Message:
			// Read_Message(count, section);
			break;
		case Enums::Obj_Usage_Sound:
			Ini_File->GetString(mSection, "Sound_File", B_Object->Sound_File, MAX_PATH);

			Ini_File->GetString(mSection, "Sound_Volume", chr_Tag1, MAX_PATH);
			(void)sscanf(chr_Tag1, "%f", &x);
			B_Object->SndVolume = x;
			break;
		case Enums::Obj_Usage_Collectable:
			// Read_Collectable(count, section);
			break;
		case Enums::Obj_Usage_Move:
			// Read_MoveEntity(count, section);
			break;
		case Enums::Obj_Usage_Teleport:
			 Read_Teleport(Count, mSection);
			break;
		case Enums::Obj_Usage_EnvironEntity:
			Read_EnvironEntity(Count, mSection);
			break;
		case Enums::Obj_Usage_Particle:
			// Read_Particle(count, section);
			break;
		default:
			break;
		}

		Count++;

	}

	App->CL_Scene->Object_Count = Count;
	
	return 1;
}

// *************************************************************************
// *	  	Read_EnvironEntity:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Read_EnvironEntity(int Index, char* Section)
{
	int Int_Tag = 0;
	char chr_Tag1[MAX_PATH] = { 0 };
	Ogre::Vector4 V4 = Ogre::Vector4::ZERO;

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	// Create Object and Reference
	Base_Object* B_Object = App->CL_Scene->B_Object[Index];
	B_Object->S_Environ[0] = new Environ_type;
	App->CL_Com_Environments->V_Set_Environ_Defaults(Index);

	// Read Environment Name
	Ini_File->GetString(Section, "Environment_Name", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Environ[0]->Environment_Name, chr_Tag1);

	// Read Environment ID
	Int_Tag = Ini_File->GetInt(Section, "Environment_ID", 0, 10);
	B_Object->S_Environ[0]->Environment_ID = Int_Tag;

	// Read Sound settings
	Ini_File->GetString(Section, "Sound_File", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Environ[0]->Sound_File, chr_Tag1);

	Ini_File->GetString(Section, "Snd_Volume", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->SndVolume = V4.x;

	Int_Tag = Ini_File->GetInt(Section, "Sound_Play", 0, 10);
	B_Object->S_Environ[0]->flag_Play = Int_Tag;

	Int_Tag = Ini_File->GetInt(Section, "Sound_Loop", 0, 10);
	B_Object->S_Environ[0]->flag_Loop = Int_Tag;

	// Read Light settings
	Ini_File->GetString(Section, "Ambient_Colour", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);
	B_Object->S_Environ[0]->AmbientColour = Ogre::Vector3(V4.x, V4.y, V4.z);

	Ini_File->GetString(Section, "Light_Position", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);
	B_Object->S_Environ[0]->Light_Position = Ogre::Vector3(V4.x, V4.y, V4.z);

	// Read Sky settings
	Int_Tag = Ini_File->GetInt(Section, "Sky_Enable", 0, 10);
	B_Object->S_Environ[0]->flag_Enabled = Int_Tag;

	Int_Tag = Ini_File->GetInt(Section, "Sky_Type", 0, 10);
	B_Object->S_Environ[0]->type = Int_Tag;

	Ini_File->GetString(Section, "Sky_Material", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Environ[0]->Material, chr_Tag1);

	Ini_File->GetString(Section, "Sky_Curvature", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Curvature = V4.x;

	Ini_File->GetString(Section, "Sky_Tiling", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Tiling = V4.x;

	Ini_File->GetString(Section, "Sky_Distance", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Distance = V4.x;

	// Read Fog settings
	Int_Tag = Ini_File->GetInt(Section, "Fog_On", 0, 10);
	B_Object->S_Environ[0]->Fog_On = Int_Tag;

	Int_Tag = Ini_File->GetInt(Section, "Fog_Mode", 0, 10);
	B_Object->S_Environ[0]->Fog_Mode = Int_Tag;

	Ini_File->GetString(Section, "Fog_Colour", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);
	B_Object->S_Environ[0]->Fog_Colour = Ogre::Vector3(V4.x, V4.y, V4.z);

	Ini_File->GetString(Section, "Fog_Start", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Fog_Start = V4.x;

	Ini_File->GetString(Section, "Fog_End", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Fog_End = V4.x;

	Ini_File->GetString(Section, "Fog_Density", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Fog_Density = V4.x;

	return 1;
}

// *************************************************************************
// *	  		Read_Message:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Read_Message(int Index, char* Section)
{
	char chr_Tag1[MAX_PATH] = { 0 };
	Ogre::Vector4 V4 = Ogre::Vector4::ZERO;

	Base_Object* B_Object = App->CL_Scene->B_Object[Index];


	B_Object->S_Message[0] = new Message_type;
	//App->CL_Com_Messages->Set_Message_Defaults(Index);

	// --------------------------------
	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->GetString(Section, "Message_Text", B_Object->S_Message[0]->Message_Text, MAX_PATH);

	Ini_File->GetString(Section, "Message_Pos", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f", &V4.x, &V4.y);

	B_Object->S_Message[0]->Message_PosX = V4.x;
	B_Object->S_Message[0]->Message_PosY = V4.y;

	// Message Counter
	B_Object->S_Message[0]->Counter_ID = Ini_File->GetInt(Section, "Message_Counter_ID", 0, 10);
	B_Object->S_Message[0]->Trigger_Value = Ini_File->GetInt(Section, "Message_Trigger_Value", 0, 10);
	B_Object->S_Message[0]->flag_Counter_Disabled = Ini_File->GetInt(Section, "Message_Counter_Disabled", 1, 10);

	B_Object->S_Message[0]->flag_PosXCentre_Flag = Ini_File->GetInt(Section, "Message_CentreX", 0, 10);
	B_Object->S_Message[0]->flag_PosYCentre_Flag = Ini_File->GetInt(Section, "Message_CentreY", 0, 10);

	int Test = Ini_File->GetString(Section, "Message_Text_Colour", chr_Tag1, MAX_PATH);
	if (Test > 0)
	{
		(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &V4.x, &V4.y, &V4.z, &V4.w);

		B_Object->S_Message[0]->Text_Colour = Ogre::Vector4(V4.x, V4.y, V4.z, V4.w);
	}
	else
	{
		B_Object->S_Message[0]->Text_Colour = Ogre::Vector4(0, 1, 0, 255);
	}

	Test = Ini_File->GetString(Section, "Message_BackGround_Colour", chr_Tag1, MAX_PATH);
	if (Test > 0)
	{
		(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &V4.x, &V4.y, &V4.z, &V4.w);

		B_Object->S_Message[0]->BackGround_Colour = Ogre::Vector4(V4.x, V4.y, V4.z, V4.w);

	}
	else
	{
		B_Object->S_Message[0]->BackGround_Colour = Ogre::Vector4(239, 239, 239, 255);
	}

	B_Object->S_Message[0]->flag_Show_BackGround = Ini_File->GetInt(Section, "Message_Show_BackGround", 1, 10);

	return 1;
}

// *************************************************************************
// *	  		Read_MoveEntity:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Read_MoveEntity(int Index, char* Section)
{
	char chr_Tag1[MAX_PATH] = { 0 };
	Ogre::Vector4 V4 = Ogre::Vector4::ZERO;

	Base_Object* B_Object = App->CL_Scene->B_Object[Index];

	B_Object->S_MoveType[0] = new Move_Type;
	//App->CL_Com_MoveEntity->Set_Move_Defaults(Index); // Check

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	//  Distance
	Ini_File->GetString(Section, "Move_Distance", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_MoveType[0]->Move_Distance = V4.x;

	B_Object->S_MoveType[0]->flag_IsNegative = Ini_File->GetInt(Section, "Move_IsNegative", 0, 10);

	//  Speed
	Ini_File->GetString(Section, "Move_Speed", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_MoveType[0]->Speed = V4.x;

	//  Name
	Ini_File->GetString(Section, "Move_ObjectName", B_Object->S_MoveType[0]->Object_Name, MAX_PATH);
	
	B_Object->S_MoveType[0]->Object_To_Move_Index = Ini_File->GetInt(Section, "Move_ObjectID", 0, 10);
	B_Object->S_MoveType[0]->WhatDirection = Ini_File->GetInt(Section, "Move_WhatDirection", 0, 10);

	// Move Sound
	Ini_File->GetString(Section, "Move_Sound", B_Object->Sound_File, MAX_PATH);
	B_Object->flag_Play_Sound = Ini_File->GetInt(Section, "Move_Play_Sound", 0, 10);

	Ini_File->GetString(Section, "Move_Volume", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	//B_Object->SndVolume = V4.x;

	// Move Counter
	B_Object->S_MoveType[0]->Counter_ID = Ini_File->GetInt(Section, "Move_Counter_ID", 0, 10);
	B_Object->S_MoveType[0]->Trigger_Value = Ini_File->GetInt(Section, "Move_Trigger_Value", 0, 10);
	B_Object->S_MoveType[0]->flag_Counter_Disabled = Ini_File->GetInt(Section, "Move_Counter_Disabled", 1, 10);

	return 1;
}

// *************************************************************************
// *	  		Read_Particle:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Read_Particle(int Index, char* Section)
{
	char chr_Tag1[MAX_PATH] = { 0 };
	Ogre::Vector4 V4 = Ogre::Vector4::ZERO;

	Base_Object* B_Object = App->CL_Scene->B_Object[Index];

	B_Object->S_Particle[0] = new Particle_type;
	//App->CL_Com_Particles->Set_Particle_Defaults(Index);

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	int Test = Ini_File->GetString(Section, "Particle_Script", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Particle[0]->ParticleScript, chr_Tag1);

	Ini_File->GetString(Section, "Particle_SpeedFactor", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Particle[0]->SpeedFactor = V4.x;

	return 1;
}

// *************************************************************************
// *	  		Read_Collectable:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool CL64_Project::Read_Collectable(int Index, char* Section)
{
	char chr_Tag1[MAX_PATH] = { 0 };
	Ogre::Vector4 V4 = Ogre::Vector4::ZERO;

	Base_Object* B_Object = App->CL_Scene->B_Object[Index];

	B_Object->S_Collectable[0] = new Collectable_type;
	//App->CL_Com_Collectables->Set_Collectables_Defaults(Index);

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->GetString(Section, "Col_Sound_File", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Collectable[0]->Sound_File, chr_Tag1);

	Ini_File->GetString(Section, "Col_Sound_Volume", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Collectable[0]->SndVolume = V4.x;

	B_Object->S_Collectable[0]->flag_Play = Ini_File->GetInt(Section, "Col_Play", 0, 10);

	Ini_File->GetString(Section, "Col_Counter_Name", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Collectable[0]->Counter_Name, chr_Tag1);

	B_Object->S_Collectable[0]->Counter_ID = Ini_File->GetInt(Section, "Col_Counter_ID", 0, 10);
	B_Object->S_Collectable[0]->Maths = Ini_File->GetInt(Section, "Col_Maths", 0, 10);
	B_Object->S_Collectable[0]->Value = Ini_File->GetInt(Section, "Col_Value", 0, 10);
	B_Object->S_Collectable[0]->flag_Counter_Disabled = Ini_File->GetInt(Section, "Col_Disabled", 0, 10);

	return 1;
}

// *************************************************************************
// *	  		Read_Teleport:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Read_Teleport(int Index, char* Section)
{
	Debug
	int Int_Tag = 0;
	char chr_Tag1[MAX_PATH] = { 0 };
	Ogre::Vector4 V4 = Ogre::Vector4::ZERO;

	Base_Object* B_Object = App->CL_Scene->B_Object[Index];

	B_Object->S_Teleport[0] = new Teleport_type;
	App->CL_Teleporters->Set_Teleports_Defaults(Index);

	B_Object->S_Environ[0] = new Environ_type;
	App->CL_Com_Environments->V_Set_Environ_Defaults(Index);

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->GetString(Section, "Tele_Goto", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Teleport[0]->Location_Name, chr_Tag1);

	B_Object->S_Teleport[0]->Location_ID = Ini_File->GetInt(Section, "Tele_ID", 0, 10);

	// Sound
	Ini_File->GetString(Section, "Tele_Sound", B_Object->S_Teleport[0]->Sound_File, MAX_PATH);

	Ini_File->GetString(Section, "Tele_Volume", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Teleport[0]->SndVolume = V4.x;

	B_Object->S_Teleport[0]->flag_Play = Ini_File->GetInt(Section, "Tele_Play", 0, 10);

	// Mesh_Pos
	Ini_File->GetString(Section, "Tele_Mesh_Position", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);
	B_Object->S_Teleport[0]->Player_Position = Ogre::Vector3(V4.x, V4.y, V4.z);

	//Player_Pos
	Ini_File->GetString(Section, "Tele_Physics_Position", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);

	B_Object->S_Teleport[0]->Physics_Position.setX(V4.x);
	B_Object->S_Teleport[0]->Physics_Position.setY(V4.y);
	B_Object->S_Teleport[0]->Physics_Position.setZ(V4.z);

	//Player_Rotation
	Ini_File->GetString(Section, "Tele_Physics_Rotation", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &V4.w, &V4.x, &V4.y, &V4.z);

	B_Object->S_Teleport[0]->Physics_Rotation.setW(V4.w);
	B_Object->S_Teleport[0]->Physics_Rotation.setX(V4.x);
	B_Object->S_Teleport[0]->Physics_Rotation.setY(V4.y);
	B_Object->S_Teleport[0]->Physics_Rotation.setZ(V4.z);

	// Teleport Counter
	B_Object->S_Teleport[0]->Counter_ID = Ini_File->GetInt(Section, "Tele_Counter_ID", 0, 10);
	B_Object->S_Teleport[0]->Trigger_Value = Ini_File->GetInt(Section, "Tele_Trigger_Value", 0, 10);
	B_Object->S_Teleport[0]->flag_Counter_Disabled = Ini_File->GetInt(Section, "Tele_Counter_Disabled", 1, 10);

	// Environment
	Int_Tag = Ini_File->GetInt(Section, "Environ_Enabled", 0, 10);
	B_Object->S_Environ[0]->flag_Environ_Enabled = Int_Tag;

	Ini_File->GetString(Section, "Environment_Name", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Environ[0]->Environment_Name, chr_Tag1);

	Int_Tag = Ini_File->GetInt(Section, "Environment_ID", 0, 10);
	B_Object->S_Environ[0]->Environment_ID = Int_Tag;

	//--------------- Sound
	Ini_File->GetString(Section, "Sound_File", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Environ[0]->Sound_File, chr_Tag1);

	Ini_File->GetString(Section, "Snd_Volume", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->SndVolume = V4.x;

	Int_Tag = Ini_File->GetInt(Section, "Sound_Play", 0, 10);
	B_Object->S_Environ[0]->flag_Play = Int_Tag;

	Int_Tag = Ini_File->GetInt(Section, "Sound_Loop", 0, 10);
	B_Object->S_Environ[0]->flag_Loop = Int_Tag;

	//--------------- Light
	Ini_File->GetString(Section, "Ambient_Colour", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);
	B_Object->S_Environ[0]->AmbientColour = Ogre::Vector3(V4.x, V4.y, V4.z);

	Ini_File->GetString(Section, "Light_Position", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);
	B_Object->S_Environ[0]->Light_Position = Ogre::Vector3(V4.x, V4.y, V4.z);

	//--------------- Sky
	Int_Tag = Ini_File->GetInt(Section, "Sky_Enable", 0, 10);
	B_Object->S_Environ[0]->flag_Enabled = Int_Tag;

	Int_Tag = Ini_File->GetInt(Section, "Sky_Type", 0, 10);
	B_Object->S_Environ[0]->type = Int_Tag;

	Ini_File->GetString(Section, "Sky_Material", chr_Tag1, MAX_PATH);
	strcpy(B_Object->S_Environ[0]->Material, chr_Tag1);

	Ini_File->GetString(Section, "Sky_Curvature", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Curvature = V4.x;

	Ini_File->GetString(Section, "Sky_Tiling", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Tiling = V4.x;

	Ini_File->GetString(Section, "Sky_Distance", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Distance = V4.x;

	//--------------- Fog
	Int_Tag = Ini_File->GetInt(Section, "Fog_On", 0, 10);
	B_Object->S_Environ[0]->Fog_On = Int_Tag;

	Int_Tag = Ini_File->GetInt(Section, "Fog_Mode", 0, 10);
	B_Object->S_Environ[0]->Fog_Mode = Int_Tag;

	Ini_File->GetString(Section, "Fog_Colour", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);
	B_Object->S_Environ[0]->Fog_Colour = Ogre::Vector3(V4.x, V4.y, V4.z);

	Ini_File->GetString(Section, "Fog_Start", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Fog_Start = V4.x;

	Ini_File->GetString(Section, "Fog_End", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Fog_End = V4.x;

	Ini_File->GetString(Section, "Fog_Density", chr_Tag1, MAX_PATH);
	(void)sscanf(chr_Tag1, "%f", &V4.x);
	B_Object->S_Environ[0]->Fog_Density = V4.x;

	return 1;
}

//float ReadFloatFromIni(const char* Section, const char* Key)
//{
//	char chr_Tag1[MAX_PATH] = { 0 };
//	App->CL_Ini_File->GetString(Section, Key, chr_Tag1, MAX_PATH);
//	float value;
//	sscanf(chr_Tag1, "%f", &value);
//	return value;
//}
//
//Ogre::Vector3 ReadVector3FromIni(const char* Section, const char* Key)
//{
//	char chr_Tag1[MAX_PATH] = { 0 };
//	App->CL_Ini_File->GetString(Section, Key, chr_Tag1, MAX_PATH);
//	Ogre::Vector4 V4;
//	sscanf(chr_Tag1, "%f,%f,%f", &V4.x, &V4.y, &V4.z);
//	return Ogre::Vector3(V4.x, V4.y, V4.z);
//}

// *************************************************************************
// *	  		Load_Project_Aera:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool CL64_Project::Load_Project_Aera()
{
	char Area_Ini_Path[MAX_PATH];
	char chr_Tag1[MAX_PATH];
	char Area_Name[MAX_PATH];
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

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(Area_Ini_Path);

	Area_Count = Ini_File->GetInt("Counters", "Areas_Count", 0,10);

	int Count = 0;

	//while (Count < Area_Count)
	//{
	//	char n_buff[255];
	//	char buff[255];
	//	strcpy(buff, "Area_");
	//	_itoa(Count, n_buff, 10);
	//	strcat(buff, n_buff);

	//	App->CL_Scene->B_Area[Count] = new Base_Area();
	//	App->CL_Com_Area->Set_Area_Defaults(Count);

	//	Base_Area* Area = App->CL_Scene->B_Area[Count];

	//	App->CL_Ini_File->GetString(buff, "Area_Name", Area_Name, MAX_PATH);
	//	App->CL_Ini_File->GetString(buff, "Area_File", Mesh_FileName, MAX_PATH);
	//	App->CL_Ini_File->GetString(buff, "Area_Resource_Path", Resource_Location, MAX_PATH);

	//	// ------------ Position
	//	App->CL_Ini_File->GetString(buff, "Mesh_Pos", chr_Tag1, MAX_PATH);
	//	(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
	//	App->CL_Scene->B_Area[Count]->Mesh_Pos = Ogre::Vector3(x, y, z);

	//	// ------------ Scale
	//	int Test = App->CL_Ini_File->GetString(buff, "Mesh_Scale", chr_Tag1, MAX_PATH);
	//	if (Test > 0)
	//	{
	//		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
	//		App->CL_Scene->B_Area[Count]->Mesh_Scale = Ogre::Vector3(x, y, z);
	//	}
	//	else
	//	{
	//		App->CL_Scene->B_Area[Count]->Mesh_Scale = Ogre::Vector3(1, 1, 1);
	//	}

	//	// ------------ Rotation
	//	Test = App->CL_Ini_File->GetString(buff, "Mesh_Rot", chr_Tag1, MAX_PATH);
	//	if (Test > 0)
	//	{
	//		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
	//		App->CL_Scene->B_Area[Count]->Mesh_Rot = Ogre::Vector3(x, y, z);
	//	}
	//	else
	//	{
	//		App->CL_Scene->B_Area[Count]->Mesh_Rot = Ogre::Vector3(0, 0, 0);
	//	}


	//	// ------------ Mesh_Quat
	//	Test = App->CL_Ini_File->GetString(buff, "Mesh_Quat", chr_Tag1, MAX_PATH);
	//	if (Test > 0)
	//	{
	//		int result = sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

	//		App->CL_Scene->B_Area[Count]->Mesh_Quat.w = w;
	//		App->CL_Scene->B_Area[Count]->Mesh_Quat.x = x;
	//		App->CL_Scene->B_Area[Count]->Mesh_Quat.y = y;
	//		App->CL_Scene->B_Area[Count]->Mesh_Quat.z = z;
	//	}
	//	else
	//	{
	//		App->CL_Scene->B_Area[Count]->Mesh_Quat.w = 1;
	//		App->CL_Scene->B_Area[Count]->Mesh_Quat.x = 0;
	//		App->CL_Scene->B_Area[Count]->Mesh_Quat.y = 0;
	//		App->CL_Scene->B_Area[Count]->Mesh_Quat.z = 0;
	//	}

	//	//App->Cl_Ini->GetString("Area_0", "Material_File", App->CL_Scene->B_Area[Count]->Material_File, MAX_PATH);
	//	
	//	char Test_For_Mesh[MAX_PATH];
	//	strcpy(Test_For_Mesh, App->CL_Project->m_Main_Assets_Path);
	//	strcat(Test_For_Mesh, Mesh_FileName);
	//	Test = App->CL_File_IO->Check_File_Exist(Test_For_Mesh);
	//	if (Test == 0)
	//	{
	//		App->Say("Can not find", Mesh_FileName);
	//	}
	//	else
	//	{
	//		App->CL_Com_Area->Add_Aera_To_Project(Count, Mesh_FileName, m_Main_Assets_Path);

	//		App->CL_Scene->B_Area[Count]->This_Object_UniqueID = App->CL_Ini_File->GetInt(buff, "Area_Object_ID", 0, 10);

	//		strcpy(App->CL_Scene->B_Area[Count]->Area_Name, Area_Name);

	//		App->CL_Scene->B_Area[Count]->FileViewItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Areas_Folder, Area_Name, Count, false);

	//		App->CL_Scene->Area_Count++;
	//	}

	//	Count++;
	//}

	/*if (App->CL_Scene->Area_Count > 0)
	{
		App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Areas_Folder);
		App->CL_FileView->SelectItem(App->CL_Scene->B_Area[0]->FileViewItem);
	}*/

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

	strcat(Player_Ini_Path, "Players.dat");

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(Player_Ini_Path);

	Players_Count = Ini_File->GetInt("Counters", "Player_Count", 0,10);

	int Count = 0;
	char Cbuff[255];
	char buff[255];
	
	while (Count < Players_Count)
	{
		strcpy(buff, "Player_");
		_itoa(Count, Cbuff, 10);
		strcat(buff, Cbuff);

		Ini_File->GetString(buff, "Player_Name", Player_Name, MAX_PATH);
		strcpy(App->CL_Scene->B_Player[Count]->Player_Name, Player_Name);


		Ini_File->GetString(buff, "Start_Position", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		App->CL_Scene->B_Player[Count]->StartPos = Ogre::Vector3(x, y, z);

		Ini_File->GetString(buff, "Start_Rotation", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

		App->CL_Scene->B_Player[Count]->Physics_Quat.setW(w);
		App->CL_Scene->B_Player[Count]->Physics_Quat.setX(x);
		App->CL_Scene->B_Player[Count]->Physics_Quat.setY(y);
		App->CL_Scene->B_Player[Count]->Physics_Quat.setZ(z);

		Ini_File->GetString(buff, "Turn_Rate", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f", &x);
		App->CL_Scene->B_Player[Count]->TurnRate = x;

		Ini_File->GetString(buff, "Ground_Speed", chr_Tag1, MAX_PATH);
		(void)sscanf(chr_Tag1, "%f", &x);
		App->CL_Scene->B_Player[Count]->Ground_speed = x;

		//------------------ Look Up Limit
		int Test = Ini_File->GetString(buff, "Limit_Look_Up", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f", &x);
			App->CL_Scene->B_Player[Count]->Limit_Look_Up = x;
		}
		else { App->CL_Scene->B_Player[Count]->Limit_Look_Up = 45; }

		//------------------ Look Down Limit
		Test = Ini_File->GetString(buff, "Limit_Look_Down", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f", &x);
			App->CL_Scene->B_Player[Count]->Limit_Look_Down = x;
		}
		else { App->CL_Scene->B_Player[Count]->Limit_Look_Down = -45; }

		//------------------ Capsule Radius
		Test = Ini_File->GetString(buff, "Radius", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f", &x);
			App->CL_Scene->B_Player[Count]->Capsule_Radius = x;
		}
		else { App->CL_Scene->B_Player[Count]->Capsule_Radius = 4.4; }

		//------------------ Capsule Height
		Test = Ini_File->GetString(buff, "Height", chr_Tag1, MAX_PATH);
		if (Test > 0)
		{
			(void)sscanf(chr_Tag1, "%f", &x);
			App->CL_Scene->B_Player[Count]->Capsule_Height = x;
		}
		else { App->CL_Scene->B_Player[Count]->Capsule_Height = 17; }

		//------------------ Player Height
		Test = Ini_File->GetString(buff, "Player_Height", chr_Tag1, MAX_PATH);
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
	Locations_Count = Ini_File->GetInt("Locations", "Locations_Count", 0,10);

	Count = 0;
	//while (Count < Locations_Count)
	//{
	//	if (App->CL_Scene->B_Locations[Count])
	//	{
	//		delete App->CL_Scene->B_Locations[Count];
	//		App->CL_Scene->B_Locations[Count] = NULL;
	//	}

	//	char n_buff[255];
	//	char buff[255];
	//	strcpy(buff, "Location_");
	//	_itoa(Count, n_buff, 10);
	//	strcat(buff, n_buff);

	//	App->CL_Scene->B_Locations[Count] = new Base_Locations();
	//	App->CL_Scene->B_Locations[Count]->flag_Deleted = 0;


	//	Int_Tag = App->CL_Ini_File->GetInt(buff, "Locatoin_ID", 0, 10);
	//	App->CL_Scene->B_Locations[Count]->This_Object_UniqueID = Int_Tag;

	//	App->CL_Ini_File->GetString(buff, "Name", chr_Tag1, MAX_PATH);


	//	strcpy(App->CL_Scene->B_Locations[Count]->Name, chr_Tag1);

	//	// Mesh_Pos
	//	App->CL_Ini_File->GetString(buff, "Mesh_Position", chr_Tag1, MAX_PATH);
	//	(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
	//	App->CL_Scene->B_Locations[Count]->Current_Position.x = x;
	//	App->CL_Scene->B_Locations[Count]->Current_Position.y = y;
	//	App->CL_Scene->B_Locations[Count]->Current_Position.z = z;

	//	App->CL_Ini_File->GetString(buff, "Physics_Position", chr_Tag1, MAX_PATH);
	//	(void)sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
	//	App->CL_Scene->B_Locations[Count]->Physics_Position.setX(x);
	//	App->CL_Scene->B_Locations[Count]->Physics_Position.setY(y);
	//	App->CL_Scene->B_Locations[Count]->Physics_Position.setZ(z);

	//	App->CL_Ini_File->GetString(buff, "Physics_Rotation", chr_Tag1, MAX_PATH);
	//	(void)sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);
	//	App->CL_Scene->B_Locations[Count]->Physics_Rotation.setW(w);
	//	App->CL_Scene->B_Locations[Count]->Physics_Rotation.setX(x);
	//	App->CL_Scene->B_Locations[Count]->Physics_Rotation.setY(y);
	//	App->CL_Scene->B_Locations[Count]->Physics_Rotation.setZ(z);

	//	Count++;

	//}

	//App->CL_Scene->Player_Location_Count = Count;

	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Players_Folder);

	return 1;
}

//// Helper function to retrieve float values from INI file
//float GetFloatFromIni(const char* buff, const char* key, float defaultValue) {
//	char chr_Tag1[MAX_PATH];
//	if (Ini_File->GetString(buff, key, chr_Tag1, MAX_PATH) > 0) {
//		float value;
//		sscanf(chr_Tag1, "%f", &value);
//		return value;
//	}
//	return defaultValue;
//}

// *************************************************************************
// *	  	Load_Project_Camera:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Project::Load_Project_Camera()
{
	char Cameras_Ini_Path[MAX_PATH];
	char chr_Tag1[MAX_PATH];
	char Camera_Name[MAX_PATH];
	int Camera_Count = 0;

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	strcpy(Cameras_Ini_Path, m_Project_Sub_Folder);
	strcat(Cameras_Ini_Path, "\\");

	strcat(Cameras_Ini_Path, m_Level_Name);
	strcat(Cameras_Ini_Path, "\\");

	strcat(Cameras_Ini_Path, "Cameras");
	strcat(Cameras_Ini_Path, "\\");

	strcat(Cameras_Ini_Path, "Cameras.epf");

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(Cameras_Ini_Path);

	Camera_Count = Ini_File->GetInt("Counters", "Cameras_Count", 0, 10);

	int Count = 0;

	/*while (Count < Camera_Count)
	{
		char n_buff[255];
		char buff[255];
		strcpy(buff, "Camera_");
		_itoa(Count, n_buff, 10);
		strcat(buff, n_buff);*/

	//	App->CL_Scene->B_Camera[Count] = new Base_Camera();

	//	App->CL_Ini_File->GetString(buff, "Camera_Name", Camera_Name, MAX_PATH);
	//	strcpy(App->CL_Scene->B_Camera[Count]->Camera_Name, Camera_Name);

	//	//---------------------------------- Camera Pos
	//	App->CL_Ini_File->GetString(buff, "Camera_Pos", chr_Tag1, MAX_PATH);
	//	(void) sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);

	//	App->CL_Scene->B_Camera[Count]->CamPos.x = x;
	//	App->CL_Scene->B_Camera[Count]->CamPos.y = y;
	//	App->CL_Scene->B_Camera[Count]->CamPos.z = z;

	//	//---------------------------------- Camera Look At
	//	App->CL_Ini_File->GetString(buff, "LookAt", chr_Tag1, MAX_PATH);
	//	(void) sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);

	//	App->CL_Scene->B_Camera[Count]->LookAt.x = x;
	//	App->CL_Scene->B_Camera[Count]->LookAt.y = y;
	//	App->CL_Scene->B_Camera[Count]->LookAt.z = z;

	//	//---------------------------------- Camera Quaternion
	//	App->CL_Ini_File->GetString(buff, "Camera_Quat", chr_Tag1, MAX_PATH);
	//	(void) sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

	//	App->CL_Scene->B_Camera[Count]->Cam_Quat.w = w;
	//	App->CL_Scene->B_Camera[Count]->Cam_Quat.x = x;
	//	App->CL_Scene->B_Camera[Count]->Cam_Quat.y = y;
	//	App->CL_Scene->B_Camera[Count]->Cam_Quat.z = z;


	//	App->CL_Scene->B_Camera[Count]->FileViewItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Cameras_Folder,
	//		App->CL_Scene->B_Camera[Count]->Camera_Name, Count, false);

	//	App->CL_Scene->Camera_Count++;

	//	Count++;
	//}

	return 1;
}

// *************************************************************************
// *	  		Copy_Assets:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Project::Copy_Assets(char* SourceFolder, char* DestinationFolder)
{
	char SourceFile[MAX_PATH];
	char DestinationFile[MAX_PATH];

	char Path[MAX_PATH];
	strcpy(Path, SourceFolder);
	strcat(Path, "*.*");

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(Path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {

			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{

				strcpy(SourceFile, SourceFolder);
				strcat(SourceFile, fd.cFileName);

				strcpy(DestinationFile, DestinationFolder);
				strcat(DestinationFile, fd.cFileName);

				CopyFile(SourceFile, DestinationFile, false);
			}

		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	return 1;
}
