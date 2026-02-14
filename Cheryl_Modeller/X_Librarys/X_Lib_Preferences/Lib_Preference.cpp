/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
#include "Lib_Preference.h"

Lib_Preference::Lib_Preference(void)
{
	Grid_Fine_Spacing = 8;
	Grid_Spacing = 128;

	Wad_File_Name[0] = 0;
	UserData_Folder[0] = 0;

	Prefs_PathAndFile[0] = 0;
	Prefs_JustFileName[0] = 0;

	flag_OpenLastFile = false;
	flag_MapEditor = true;
	flag_SceneEditor = false;

	Defalut_Zoom = 0.3;

	WriteData = nullptr;

	WriteRecentFiles = nullptr;
	ReadRecentFiles = nullptr;

	ListPanel = nullptr;
	Root = nullptr;
	GD_ProjectFolder = nullptr;
}

Lib_Preference::~Lib_Preference(void)
{
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* Lib_Preference::GetVersion()
{
	return (LPSTR)" TMH_Scene_Builder :-- Lib_Preference Good [ 09-09-25 ] Build X ";
}

// *************************************************************************
// *			Init_FileView:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void Lib_Preference::Init_FileView(HWND hDlg)
{
	//InitCommonControls();	    // make our tree control to work

	

	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	TreeView_DeleteAllItems(Temp);

	TreeView_SetBkColor(Temp, (COLORREF)RGB(255, 255, 255));

	AddRootFolder();
	//MoreFolders(); //  Folders under root 
	//ExpandRoot();

}

// *************************************************************************
// *			AddRootFolder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void Lib_Preference::AddRootFolder(void)
{
	tvinsert.hParent = Root;			// top most level no need handle
	tvinsert.hInsertAfter = TVI_LAST; // work as root level
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Startup";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	GD_ProjectFolder = (HTREEITEM)SendDlgItemMessage(ListPanel, IDC_OPTIONS_TREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
}

// *************************************************************************
// *			  Start_Options_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void Lib_Preference::Start_Options_Dlg()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_OPTIONS, App->MainHwnd, (DLGPROC)Proc_Options_Dlg);
}

// *************************************************************************
// *			Proc_Options_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK Lib_Preference::Proc_Options_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_OPTIONS_TREE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_GB_STARTUP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_CK_LASTFILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_MAPEDITOR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_SCENEEDITOR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_ZOOM_TEXT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_ZOOMDEFAULT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		HWND Temp = GetDlgItem(hDlg, IDC_CK_LASTFILE);
		SendMessage(Temp, BM_SETCHECK, App->CL_X_Preference->flag_OpenLastFile, 0);

		Temp = GetDlgItem(hDlg, IDC_CK_MAPEDITOR);
		SendMessage(Temp, BM_SETCHECK, App->CL_X_Preference->flag_MapEditor, 0);
		
		Temp = GetDlgItem(hDlg, IDC_CK_SCENEEDITOR);
		SendMessage(Temp, BM_SETCHECK, App->CL_X_Preference->flag_SceneEditor, 0);

		char buf[MAX_PATH];
		sprintf(buf, "%f", App->CL_X_Preference->Defalut_Zoom);
		SetDlgItemText(hDlg, IDC_ED_ZOOMDEFAULT, (LPTSTR)buf);

		App->CL_X_Preference->ListPanel = hDlg;
		App->CL_X_Preference->Init_FileView(App->CL_X_Preference->ListPanel);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{

		if (GetDlgItem(hDlg, IDC_GB_STARTUP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_LASTFILE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_MAPEDITOR) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_SCENEEDITOR) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_ZOOM_TEXT) == (HWND)lParam)
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

			if (App->CL_X_Preference->flag_OpenLastFile == true)
			{
				App->CL_X_Preference->flag_OpenLastFile = false;
				SendMessage(Temp, BM_SETCHECK, false, 0);
			}
			else
			{
				App->CL_X_Preference->flag_OpenLastFile = true;
				SendMessage(Temp, BM_SETCHECK, true, 0);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_MAPEDITOR)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_CK_MAPEDITOR);
			SendMessage(Temp, BM_SETCHECK, true, 0);

			Temp = GetDlgItem(hDlg, IDC_CK_SCENEEDITOR);
			SendMessage(Temp, BM_SETCHECK, false, 0);

			App->CL_X_Preference->flag_MapEditor = true;
			App->CL_X_Preference->flag_SceneEditor = false;

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_SCENEEDITOR)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_CK_SCENEEDITOR);
			SendMessage(Temp, BM_SETCHECK, true, 0);

			Temp = GetDlgItem(hDlg, IDC_CK_MAPEDITOR);
			SendMessage(Temp, BM_SETCHECK, false, 0);

			App->CL_X_Preference->flag_MapEditor = false;
			App->CL_X_Preference->flag_SceneEditor = true;

			return TRUE;
		}

		
		if (LOWORD(wParam) == IDOK)
		{
			char buff[255];
			GetDlgItemText(hDlg, IDC_ED_ZOOMDEFAULT, (LPTSTR)buff, 255);
			
			float Zoom_Float = atof(buff);

			App->CL_X_Preference->Defalut_Zoom = Zoom_Float;

			App->CL_X_Preference->Save_Config_File();

			App->CL_Editor_Map->Reset_Views_All();

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
void Lib_Preference::Read_Preferences()
{
	char chr_Tag1[MAX_PATH];
	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->RB_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Room_Builder\\Room_Builder.ini");

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(Preferences_Path);


	Grid_Fine_Spacing = Ini_File->GetInt("Grid", "Grid_Fine_Spacing", 0, 10);
	Grid_Spacing = Ini_File->GetInt("Grid", "Grid_Spacing", 0, 10);

	App->CL_Editor_Map->GridSize = Grid_Spacing;
	App->CL_Editor_Map->GridSnapSize = Grid_Fine_Spacing;

	Ini_File->GetString("Wad", "Wad_File_Name", chr_Tag1, MAX_PATH);
	strcpy(Wad_File_Name, chr_Tag1);

}

// *************************************************************************
// *		Write_Preferences:- Terry and Hazel Flanigan 2025 			   *
// *************************************************************************
bool Lib_Preference::Write_Preferences()
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
void Lib_Preference::Init_Configuration()
{
	char DirCheck[MAX_PATH];
	strcpy(DirCheck, UserData_Folder);
	strcat(DirCheck, "\\");
	strcat(DirCheck, "TMH_Dir");

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
		strcat(mCheckFile, "\\TMH_MeshBuilder.cfg");

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
void Lib_Preference::Config_SetDefaults()
{
	strcpy(App->CL_Level->MTF_PathAndFile, "");
	strcpy(App->CL_Level->MTF_PathAndFile, "");

	flag_OpenLastFile = false;
	flag_MapEditor = true;
	flag_SceneEditor = false;
	Defalut_Zoom = 0.5;

	Save_Config_File();
}

// *************************************************************************
// *			Save_Config_File:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void Lib_Preference::Save_Config_File()
{
	WriteRecentFiles = nullptr;

	char buf[MAX_PATH];
	strcpy(buf, UserData_Folder);
	strcat(buf, "\\TMH_Dir\\TMH_MeshBuilder.cfg");

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

	fprintf(WriteRecentFiles, "%s%i\n", "Start_Map_Editor=", flag_MapEditor);
	fprintf(WriteRecentFiles, "%s%i\n", "Start_Scene_Editor=", flag_SceneEditor);

	fprintf(WriteRecentFiles, "%s%f\n", "Default_Zoom=", Defalut_Zoom);

	std::fclose(WriteRecentFiles);

	return;
}

// *************************************************************************
// *		LoadHistory_Files:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void Lib_Preference::Load_Config_File()
{
	char buf[MAX_PATH];

	strcpy(buf, UserData_Folder);
	strcat(buf, "\\TMH_Dir\\TMH_MeshBuilder.cfg");

	auto& Ini_File = App->CL_X_Ini_File; // App->CL_X_Ini_File-> (Pointer)

	Ini_File->SetPathName(buf);

	flag_OpenLastFile = Ini_File->GetInt("Start_Up", "Open_Last_File", 0, 10);

	Ini_File->GetString("Start_Up", "Last_File_Full", Prefs_PathAndFile, MAX_PATH);
	Ini_File->GetString("Start_Up", "Last_File_Name", Prefs_JustFileName, MAX_PATH);
	
	flag_MapEditor = Ini_File->GetInt("Start_Up", "Start_Map_Editor", 1, 10);
	flag_SceneEditor = Ini_File->GetInt("Start_Up", "Start_Scene_Editor", 0, 10);

	Ini_File->GetString("Start_Up", "Default_Zoom", buf, MAX_PATH);
	Defalut_Zoom = atof(buf);

	return;
}

// *************************************************************************
// *		Search_For_Folder:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
bool Lib_Preference::Search_For_Folder(char* FolderPath)
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
bool Lib_Preference::Check_File_Exist(char* Full_Path)
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

