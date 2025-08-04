/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_Prefs.h"

CL64_Prefs::CL64_Prefs(void)
{
	Grid_Fine_Spacing = 8;
	Grid_Spacing = 128;

	Wad_File_Name[0] = 0;
	UserData_Folder[0] = 0;

	Prefs_PathAndFile[0] = 0;
	Prefs_JustFileName[0] = 0;

	flag_OpenLastFile = false;

	WriteData = nullptr;

	WriteRecentFiles = nullptr;
	ReadRecentFiles = nullptr;
}

CL64_Prefs::~CL64_Prefs(void)
{
}

// *************************************************************************
// *			  Start_Options_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Prefs::Start_Options_Dlg()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_OPTIONS, App->MainHwnd, (DLGPROC)Proc_Options_Dlg);
}

// *************************************************************************
// *			Proc_Options_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Prefs::Proc_Options_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_CK_LASTFILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		HWND Temp = GetDlgItem(hDlg, IDC_CK_LASTFILE);
		SendMessage(Temp, BM_SETCHECK, App->CL_Prefs->flag_OpenLastFile, 0);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_CK_LASTFILE) == (HWND)lParam)
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
		if (LOWORD(wParam) == IDC_CK_LASTFILE)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_CK_LASTFILE);

			if (App->CL_Prefs->flag_OpenLastFile == true)
			{
				App->CL_Prefs->flag_OpenLastFile = false;
				SendMessage(Temp, BM_SETCHECK, false, 0);
			}
			else
			{
				App->CL_Prefs->flag_OpenLastFile = true;
				SendMessage(Temp, BM_SETCHECK, true, 0);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Prefs->Save_Config_File();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *			Read_Preferences:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Prefs::Read_Preferences()
{
	char chr_Tag1[MAX_PATH];
	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->RB_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Room_Builder\\Room_Builder.ini");

	App->CL_Ini_File->SetPathName(Preferences_Path);


	Grid_Fine_Spacing = App->CL_Ini_File->GetInt("Grid", "Grid_Fine_Spacing", 0, 10);
	Grid_Spacing = App->CL_Ini_File->GetInt("Grid", "Grid_Spacing", 0, 10);

	App->CL_Editor_Map->GridSize = Grid_Spacing;
	App->CL_Editor_Map->GridSnapSize = Grid_Fine_Spacing;

	App->CL_Ini_File->GetString("Wad", "Wad_File_Name", chr_Tag1, MAX_PATH);
	strcpy(Wad_File_Name, chr_Tag1);

}

// *************************************************************************
// *		Write_Preferences:- Terry and Hazel Flanigan 2025 			   *
// *************************************************************************
bool CL64_Prefs::Write_Preferences()
{
	WriteData = nullptr;

	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->RB_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Room_Builder\\Room_Builder.ini");

	WriteData = fopen(Preferences_Path, "wt");
	if (!WriteData)
	{
		App->Say_Win("Cant Open Preference File");
		return 0;
	}

	fprintf(WriteData, "%s\n", "[Grid]");
	fprintf(WriteData, "%s%i\n", "Grid_Fine_Spacing=", Grid_Fine_Spacing);
	fprintf(WriteData, "%s%i\n", "Grid_Spacing=", Grid_Spacing);

	fprintf(WriteData, "%s\n", "[Wad]");
	fprintf(WriteData, "%s%s\n", "Wad_File_Name=", "Default.txl");

	fclose(WriteData);

	return 1;
}

// *************************************************************************
// *			Init_Configuration:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Prefs::Init_Configuration()
{
	char DirCheck[MAX_PATH];
	strcpy(DirCheck, UserData_Folder);
	strcat(DirCheck, "\\");
	strcat(DirCheck, "OW3D_Dir");

	bool check = 0;
	check = Search_For_Folder(DirCheck);
	if (check == 0)
	{
		CreateDirectory(DirCheck, NULL);

		Config_SetDefaults();
		Save_Config_File();
		Load_Config_File();
	}
	else
	{
		char mCheckFile[MAX_PATH];
		strcpy(mCheckFile, DirCheck);
		strcat(mCheckFile, "\\OW3D_MeshBuilder.cfg");

		bool checkfile = Check_File_Exist(mCheckFile);

		if (checkfile == 1)
		{

		}
		else
		{
			Config_SetDefaults();
			Save_Config_File();
			Load_Config_File();
		}

		Load_Config_File();
	}
}

// *************************************************************************
// *			Config_SetDefaults:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Prefs::Config_SetDefaults()
{

	Save_Config_File();
}

// *************************************************************************
// *			Save_Config_File:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Prefs::Save_Config_File()
{
	WriteRecentFiles = nullptr;

	char buf[MAX_PATH];
	strcpy(buf, UserData_Folder);
	strcat(buf, "\\OW3D_Dir\\OW3D_MeshBuilder.cfg");

	WriteRecentFiles = std::fopen(buf, "wt");

	if (!WriteRecentFiles)
	{
		App->Say("Why Cant Find Recent Files");
		return;
	}

	fprintf(WriteRecentFiles, "%s\n", "[Version_Data]");
	fprintf(WriteRecentFiles, "%s%s\n", "Version=", "V1.0");

	fprintf(WriteRecentFiles, "%s\n", "[Start_Up]");
	fprintf(WriteRecentFiles, "%s%i\n", "Open_Last_File=", flag_OpenLastFile);
	fprintf(WriteRecentFiles, "%s%s\n", "Last_File_Full=", App->CL_Level->MTF_PathAndFile);
	fprintf(WriteRecentFiles, "%s%s\n", "Last_File_Name=", App->CL_Level->MTF_Just_FileName);
	
	std::fclose(WriteRecentFiles);

	return;
}

// *************************************************************************
// *		LoadHistory_Files:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Prefs::Load_Config_File()
{
	char buf[MAX_PATH];

	strcpy(buf, UserData_Folder);
	strcat(buf, "\\OW3D_Dir\\OW3D_MeshBuilder.cfg");

	App->CL_Ini_File->SetPathName(buf);

	flag_OpenLastFile = App->CL_Ini_File->GetInt("Start_Up", "Open_Last_File", 0, 10);

	App->CL_Ini_File->GetString("Start_Up", "Last_File_Full", Prefs_PathAndFile, MAX_PATH);
	App->CL_Ini_File->GetString("Start_Up", "Last_File_Name", Prefs_JustFileName, MAX_PATH);
	return;
}

// *************************************************************************
// *		Search_For_Folder:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
bool CL64_Prefs::Search_For_Folder(char* FolderPath)
{
	char pSearchPath[1024];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(pSearchPath, FolderPath);

	hFind = FindFirstFile(pSearchPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		FindClose(hFind);
		return 1;
	}

	return 0;
}

// *************************************************************************
// *		Check_File_Exist:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
bool CL64_Prefs::Check_File_Exist(char* Full_Path)
{
	char pSearchPath[MAX_PATH];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(pSearchPath, Full_Path);

	hFind = FindFirstFile(pSearchPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		FindClose(hFind);
		return 1;
	}

	return 0;
}
