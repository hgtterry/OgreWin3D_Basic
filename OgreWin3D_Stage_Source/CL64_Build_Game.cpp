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
#include "resource.h"
#include "CL64_Build_Game.h"
#include "zip.h"

CL64_Build_Game::CL64_Build_Game(void)
{
	flag_Use_Front_Dlg = 0;
	flag_Show_FPS = 0;
	flag_Saved_Show_FPS = 0;

	strcpy(GameName, "YourGameName");
	StartFolder[0] = 0;

	ProjectFolder[0] = 0;
	Sub_ProjectFolder[0] = 0;

	MediaFolder[0] = 0;

	CoreDataFolder[0] = 0;
	SoundFolder[0] = 0;
	ParticleFolder[0] = 0;

	m_Build_Sub_Folder[0] = 0;

	Directory_Altered = 0;
	//---------------------------------
	Sub_Build_Folder[0] = 0;
	m_Ini_Path_File_Name[0] = 0;
	m_Level_Folder_Path[0] = 0;
	m_Main_Assets_Path[0] = 0;
	m_Aera_Folder_Path[0] = 0;
	m_Players_Folder_Path[0] = 0;
	m_Cameras_Folder_Path[0] = 0;
	m_Objects_Folder_Path[0] = 0;
	m_Display_Folder_Path[0] = 0;
	m_Enviromnet_Folder_Path[0] = 0;

	Banner = nullptr;
	DlgHwnd = nullptr;
	WriteFile = nullptr;

	Init_Build_Game_Class();

}

CL64_Build_Game::~CL64_Build_Game(void)
{
}

// *************************************************************************
// *	  	Init_Build_Game_Class:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Build_Game::Init_Build_Game_Class()
{
	//HZIP hz;

	//hz = CreateZip(_T("Test.zip"), 0);
	//ZipAdd(hz, _T("Barrel_B2.dds"), _T("Barrel_B2.dds"));
	//ZipAdd(hz, _T("znsimple.txt"), _T("simple.txt"));
	//CloseZip(hz);

	GameOptions = new Game_Options;
	GameOptions->flag_Show_FPS = 1;
	GameOptions->flag_FullScreen = 1;
	GameOptions->flag_Zipped_Assets = 1;
	GameOptions->flag_Front_Dialog = 1;
}

// *************************************************************************
// *	  	Start_Project_Build:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Build_Game::Start_Project_Build()
{
	DlgHwnd = nullptr;

	DialogBox(App->hInst, (LPCTSTR)IDD_BUILD_DIALOG, App->Fdlg, (DLGPROC)Proc_Project_Build);
}

// *************************************************************************
// *		Project_Build_Proc:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Build_Game::Proc_Project_Build(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_Build_Game->DlgHwnd = hDlg;

		SendDlgItemMessage(hDlg, IDC_EDGAMENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STGAMENAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STPATH, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_BANNER, WM_SETFONT, (WPARAM)App->Font_Banner, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STLOCATION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTBROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_BL_DESKTOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_BT_BUILDOPTIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CK_BO_SHOWFPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_FULLSCREEN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_BO_ZIPFILES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_BO_FRONTDLG, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));


		SetDlgItemText(hDlg, IDC_EDGAMENAME, (LPCTSTR)App->CL_Build_Game->GameName);

		App->CL_Build_Game->Banner = hDlg;

		if (App->CL_Build_Game->Directory_Altered == 0)
		{
			strcpy(App->CL_Build_Game->Desktop, App->CL_File_IO->DeskTop_Folder);
			strcat(App->CL_Build_Game->Desktop, "\\");
			strcpy(App->CL_Build_Game->StartFolder, App->CL_Build_Game->Desktop);
		}

		if (App->CL_Build_Game->GameOptions->flag_Show_FPS == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BO_SHOWFPS);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}

		if (App->CL_Build_Game->GameOptions->flag_FullScreen == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_FULLSCREEN);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}

		if (App->CL_Build_Game->GameOptions->flag_Front_Dialog == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BO_FRONTDLG);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}

		if (App->CL_Build_Game->GameOptions->flag_Zipped_Assets == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BO_ZIPFILES);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}


		SetDlgItemText(hDlg, IDC_STLOCATION, (LPCTSTR)App->CL_Build_Game->StartFolder);


		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_EDGAMENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_STLOCATION) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_CK_FULLSCREEN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STGAMENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STPATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BL_DESKTOP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BO_SHOWFPS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_FULLSCREEN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BO_ZIPFILES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BO_FRONTDLG) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
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

		if (some_item->idFrom == IDC_BTBROWSE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_CK_BL_DESKTOP)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BL_DESKTOP);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				char Desktop[MAX_PATH];
				strcpy(Desktop, App->CL_File_IO->DeskTop_Folder);
				strcat(Desktop, "\\");

				strcpy(App->CL_File_IO->szSelectedDir, Desktop);
				strcpy(App->CL_Build_Game->StartFolder, App->CL_File_IO->szSelectedDir);
				SetDlgItemText(hDlg, IDC_STLOCATION, (LPCTSTR)App->CL_Build_Game->StartFolder);

				App->CL_Build_Game->Directory_Altered = 1;

				EnableWindow(GetDlgItem(hDlg, IDC_BTBROWSE), 0);
				EnableWindow(GetDlgItem(hDlg, IDC_STLOCATION), 0);

				return 1;
			}
			else
			{
				EnableWindow(GetDlgItem(hDlg, IDC_BTBROWSE), 1);
				EnableWindow(GetDlgItem(hDlg, IDC_STLOCATION), 1);

				App->CL_Build_Game->Directory_Altered = 1;
				return 1;
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTBROWSE)
		{
			App->CL_File_IO->Pick_Folder();

			if (App->CL_File_IO->Canceled == 1)
			{
				return TRUE;
			}

			strcpy(App->CL_Build_Game->StartFolder, App->CL_File_IO->szSelectedDir);

			SetDlgItemText(hDlg, IDC_STLOCATION, (LPCTSTR)App->CL_File_IO->szSelectedDir);

			App->CL_Build_Game->Directory_Altered = 1;

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_BO_SHOWFPS)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BO_SHOWFPS);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Build_Game->GameOptions->flag_Show_FPS = 1;
			}
			else
			{
				App->CL_Build_Game->GameOptions->flag_Show_FPS = 0;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_FULLSCREEN)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_FULLSCREEN);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Build_Game->GameOptions->flag_FullScreen = 1;
			}
			else
			{
				App->CL_Build_Game->GameOptions->flag_FullScreen = 0;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_BO_FRONTDLG)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BO_FRONTDLG);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Build_Game->GameOptions->flag_Front_Dialog = 1;
			}
			else
			{
				App->CL_Build_Game->GameOptions->flag_Front_Dialog = 0;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_BO_ZIPFILES)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BO_ZIPFILES);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Build_Game->GameOptions->flag_Zipped_Assets = 1;
			}
			else
			{
				App->CL_Build_Game->GameOptions->flag_Zipped_Assets = 0;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			char GameName[255];
			char PathName[255];
			GetDlgItemText(hDlg, IDC_EDGAMENAME, (LPTSTR)GameName, 255);
			GetDlgItemText(hDlg, IDC_STLOCATION, (LPTSTR)PathName, 255);
			int result = 1;

			// Check Name Entered
			result = strcmp(GameName, "");
			if (result == 0)
			{
				App->Say("No Game Name Specified");
				return 1;
			}

			// Check Path Selected
			result = strcmp(PathName, "");
			if (result == 0)
			{
				App->Say("No Path Specified");
				return 1;
			}

			result = strcmp(GameName, App->CL_Build_Game->GameName);
			if (result < 0 || result > 0)
			{
				App->CL_Scene->flag_Scene_Modified = 1;
			}

			strcpy(App->CL_File_IO->szSelectedDir, PathName);
			strcpy(App->CL_Build_Game->StartFolder, PathName);

			strcpy(App->CL_Build_Game->GameName, GameName);

			App->CL_Build_Game->Create_ProjectFolder();

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
	return FALSE;
}

// *************************************************************************
// *		Create_ProjectFolder:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Build_Game::Create_ProjectFolder(void)
{
	char TestFile[MAX_PATH];
	strcpy(TestFile, App->CL_File_IO->szSelectedDir);
	strcat(TestFile, GameName);
	strcat(TestFile, "_Project");
	strcat(TestFile, "\\");
	strcat(TestFile, "Game");
	strcat(TestFile, "\\");
	
	//App->Say_Win(TestFile);

	int test = App->CL_File_IO->SearchFolders(TestFile, (LPSTR)"\\Game.gdat");
	if (test == 1)
	{
		App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"File Exsits", (LPSTR)"Do you want to update File", (LPSTR)"");

		bool Doit = App->CL_Dialogs->Canceled;
		if (Doit == 1)
		{
			return;
		}
	}

	ProjectFolder[0] = 0;
	Sub_ProjectFolder[0] = 0;

	MediaFolder[0] = 0;

	CoreDataFolder[0] = 0;
	SoundFolder[0] = 0;

	strcpy(ProjectFolder, App->CL_File_IO->szSelectedDir);
	strcat(ProjectFolder, GameName);
	strcat(ProjectFolder, "_Project");
	CreateDirectory(ProjectFolder, NULL);

	strcpy(MediaFolder, ProjectFolder);
	strcat(MediaFolder, "\\");
	strcat(MediaFolder, "Media");
	CreateDirectory(MediaFolder, NULL);

	strcpy(CoreDataFolder, MediaFolder);
	strcat(CoreDataFolder, "\\");
	strcat(CoreDataFolder, "Core_Data");
	CreateDirectory(CoreDataFolder, NULL);

	strcpy(SoundFolder, MediaFolder);
	strcat(SoundFolder, "\\");
	strcat(SoundFolder, "Sounds");
	CreateDirectory(SoundFolder, NULL);

	strcpy(ParticleFolder, MediaFolder);
	strcat(ParticleFolder, "\\");
	strcat(ParticleFolder, "New_Particles");
	CreateDirectory(ParticleFolder, NULL);

	Copy_SystemFiles();

	strcpy(m_Build_Sub_Folder, ProjectFolder);
	strcat(m_Build_Sub_Folder, "\\");
	strcat(m_Build_Sub_Folder, "Game");
	Build_Project();


	Copy_ZipFiles();
	Copy_Sound_Files();
	Copy_Particle_Files();

	App->CL_PB->Stop_Progress_Bar((LPSTR)"Build Completed");

}

// *************************************************************************
// *		Copy_SystemFiles:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Build_Game::Copy_SystemFiles(void)
{

	//----------------------------- OgreMain.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\OgreMain.dll");

	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\OgreMain.dll");

	CopyFile(SourceFile, DestinationFile, false);

	//----------------------------- RenderSystem_GL.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\RenderSystem_GL.dll");

	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\RenderSystem_GL.dll");

	CopyFile(SourceFile, DestinationFile, false);

	//----------------------------- OgreOverlay.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\OgreOverlay.dll");

	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\OgreOverlay.dll");

	CopyFile(SourceFile, DestinationFile, false);

	//----------------------------- vcruntime140.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\vcruntime140_1.dll");

	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\vcruntime140_1.dll");

	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- irrKlang.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\irrKlang.dll");

	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\irrKlang.dll");

	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- Game File
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\Shell.gex");

	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\");
	strcat(DestinationFile, GameName);
	strcat(DestinationFile, ".exe");
	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- Game FIle
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\plugins.cfg");
	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\plugins.cfg");
	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- Plugin_ParticleFX.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\Plugin_ParticleFX.dll");
	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\Plugin_ParticleFX.dll");
	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- SDL2.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\SDL2.dll");
	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\SDL2.dll");
	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- Codec_RsImage.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\Codec_RsImage.dll");
	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\Codec_RsImage.dll");
	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- OgreBites.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\OgreBites.dll");
	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\OgreBites.dll");
	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- OgreRTShaderSystem.dll
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\OgreRTShaderSystem.dll");
	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\OgreRTShaderSystem.dll");
	CopyFile(SourceFile, DestinationFile, false);
	
	//----------------------------- Roboto-Medium.ttf
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\Roboto-Medium.ttf");
	strcpy(DestinationFile, ProjectFolder);
	strcat(DestinationFile, "\\Roboto-Medium.ttf");
	CopyFile(SourceFile, DestinationFile, false);
	
}

// *************************************************************************
// *			Copy_ZipFiles:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Build_Game::Copy_ZipFiles(void)
{
	//----------------------------- GDCore.zip
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\Media\\Core_Data\\GDCore.zip");
	strcpy(DestinationFile, CoreDataFolder);
	strcat(DestinationFile, "\\GDCore.zip");
	CopyFile(SourceFile, DestinationFile, false);
 
	//----------------------------- Entitys.zip
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\Media\\Core_Data\\Entitys.zip");
	strcpy(DestinationFile, CoreDataFolder);
	strcat(DestinationFile, "\\Entitys.zip");
	CopyFile(SourceFile, DestinationFile, false);
	 
	//----------------------------- Sinbad.zip
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\Media\\Core_Data\\Sinbad.zip");
	strcpy(DestinationFile, CoreDataFolder);
	strcat(DestinationFile, "\\Sinbad.zip");
	CopyFile(SourceFile, DestinationFile, false);

	//----------------------------- SdkTrays.zip
	strcpy(SourceFile, App->GD_Directory_FullPath);
	strcat(SourceFile, "\\Media\\Core_Data\\SdkTrays.zip");
	strcpy(DestinationFile, CoreDataFolder);
	strcat(DestinationFile, "\\SdkTrays.zip");
	CopyFile(SourceFile, DestinationFile, false);
	
}


// *************************************************************************
// *		Copy_Sound_Files:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Build_Game::Copy_Sound_Files(void)
{
	char StartFolder[MAX_PATH];
	strcpy(StartFolder, App->GD_Directory_FullPath);
	strcat(StartFolder, "\\");
	strcat(StartFolder, "Media\\Sounds\\");

	char Destination[MAX_PATH];
	strcpy(Destination, SoundFolder);
	strcat(Destination, "\\");

	Copy_Assets(StartFolder, Destination);
}

// *************************************************************************
// *		Copy_Particle_Files:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Build_Game::Copy_Particle_Files(void)
{
	char StartFolder[MAX_PATH];
	strcpy(StartFolder, App->GD_Directory_FullPath);
	strcat(StartFolder, "\\");
	strcat(StartFolder, "Media\\Core_Data\\Files\\New_Particles\\");

	char Destination[MAX_PATH];
	strcpy(Destination, ParticleFolder);
	strcat(Destination, "\\");

	Copy_Assets(StartFolder, Destination);
}

// *************************************************************************
// *			Read_From_Config:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Build_Game::Read_From_Config(void)
{
	//char chr_Tag1[1024];
	char StartFile[1024];
	strcpy(StartFile, App->GD_Directory_FullPath);
	strcat(StartFile, "\\");
	strcat(StartFile, "Data\\StartUp.gcf");

}

// *************************************************************************
// *	  		Build_Project:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Build_Game::Build_Project()
{
	ShowWindow(Banner, SW_HIDE);

	App->CL_PB->Start_ProgressBar();
	App->CL_PB->Set_Progress((LPSTR)"Building Scene/Game", 10);

	App->CL_PB->Nudge((LPSTR)"Creating Sub Folder");

	if (_mkdir(m_Build_Sub_Folder) == 0)
	{
		(void) _chdir(m_Build_Sub_Folder);
	}
	else
	{
		(void) _chdir(m_Build_Sub_Folder);
	}

	App->CL_PB->Nudge((LPSTR)"Creating Ini File");
	bool test = Build_Project_Ini();
	if (test == 0)
	{
		return 0;
	}

	App->CL_PB->Nudge((LPSTR)"Creating Level Folder");
	Build_Level_Folder();

	App->CL_PB->Nudge((LPSTR)"Creating Assets Folder");
	Build_Main_Asset_Folder();

	(void) _chdir(m_Level_Folder_Path);

	App->CL_PB->Nudge((LPSTR)"Creating Area Folder");
	if (App->CL_Scene->flag_Area_Added == 1)
	{
		Build_Area_Folder();
	}

	App->CL_PB->Nudge((LPSTR)"Creating Player Folder");
	if (App->CL_Scene->flag_Player_Added == 1)
	{
		Build_Players_Folder();
	}

	App->CL_PB->Nudge((LPSTR)"Creating Camera Folder");
	Build_Cameras_Folder();

	App->CL_PB->Nudge((LPSTR)"Creating Objects Folder");
	Build_Objects_Folder();

	App->CL_PB->Nudge((LPSTR)"Creating Display Folder");
	Build_Display_Folder();

	App->CL_PB->Nudge((LPSTR)"Finished");

	return 1;
}


// *************************************************************************
// *	  		Build_Project_Ini:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Build_Game::Build_Project_Ini()
{
	m_Ini_Path_File_Name[0] = 0;

	strcpy(m_Ini_Path_File_Name, m_Build_Sub_Folder);
	strcat(m_Ini_Path_File_Name, "\\");
	strcat(m_Ini_Path_File_Name, "Game.gdat");

	/*int test = App->CL_File_IO->SearchFolders(m_Build_Sub_Folder, (LPSTR)"\\Game.gdat");
	if (test == 1)
	{
		App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"File Exsits", (LPSTR)"Do you want to update File", (LPSTR)"");

		bool Doit = App->CL_Dialogs->Canceled;
		if (Doit == 1)
		{
			return 0;
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
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.2");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Files]");
	fprintf(WriteFile, "%s%s\n", "Project_Name=", GameName);
	fprintf(WriteFile, "%s%s\n", "Level_Name=", GameName);

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Options]");
	fprintf(WriteFile, "%s%i\n", "Areas_Count=", App->CL_Scene->Area_Count);
	fprintf(WriteFile, "%s%i\n", "Players_Count=", App->CL_Scene->Player_Count);
	fprintf(WriteFile, "%s%i\n", "Cameras_Count=", App->CL_Scene->Camera_Count);
	fprintf(WriteFile, "%s%i\n", "Objects_Count=", App->CL_Scene->Object_Count);
	fprintf(WriteFile, "%s%i\n", "Objects_ID_Count=", App->CL_Scene->UniqueID_Object_Counter);

	int Adjusted = App->CL_LookUps->Get_Adjusted_Counters_Count();
	fprintf(WriteFile, "%s%i\n", "Counters_Count=", Adjusted);
	fprintf(WriteFile, "%s%i\n", "Counters_ID_Count=", App->CL_Scene->UniqueID_Counters_Count);

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Config]");
	fprintf(WriteFile, "%s%i\n", "Show_FPS=", GameOptions->flag_Show_FPS);
	fprintf(WriteFile, "%s%i\n", "Game_FullScreen=", GameOptions->flag_FullScreen);
	fprintf(WriteFile, "%s%i\n", "Zipped_Assets=", GameOptions->flag_Zipped_Assets);
	fprintf(WriteFile, "%s%i\n", "Use_Front_Dlg=", GameOptions->flag_Front_Dialog);
	//fprintf(WriteFile, "%s%i\n", "Player_CanJump=", flag_Prefs_PlayerCanJump);

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Build_Level_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Build_Game::Build_Level_Folder()
{
	strcpy(m_Level_Folder_Path, m_Build_Sub_Folder);
	strcat(m_Level_Folder_Path, "\\");
	strcat(m_Level_Folder_Path, GameName);

	// First Level Folder
	if (_mkdir(m_Level_Folder_Path) == 0)
	{
		(void) _chdir(m_Level_Folder_Path);
	}
	else
	{
		(void) _chdir(m_Level_Folder_Path);
	}

	return 1;
}

// *************************************************************************
// *	  	Build_Main_Asset_Folder:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Build_Game::Build_Main_Asset_Folder()
{
	char LastFolder[MAX_PATH];

	//	if (Directory_Changed_Flag == 1)
	{
		strcpy(LastFolder, App->CL_Project->m_Main_Assets_Path);
	}

	m_Main_Assets_Path[0] = 0;

	strcpy(m_Main_Assets_Path, m_Level_Folder_Path);
	strcat(m_Main_Assets_Path, "\\");
	strcat(m_Main_Assets_Path, "Assets");
	strcat(m_Main_Assets_Path, "\\");

	if (_mkdir(m_Main_Assets_Path) == 0)
	{
		(void) _chdir(m_Main_Assets_Path);
	}
	else
	{
		(void) _chdir(m_Main_Assets_Path);
	}

	if (GameOptions->flag_Zipped_Assets == 1)
	{
		Zip_Assets(LastFolder, m_Main_Assets_Path);
	}
	else
	{
		Copy_Assets(LastFolder, m_Main_Assets_Path);
	}

	//Directory_Changed_Flag = 0;

	(void) _chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  	Save_Area_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Build_Game::Build_Area_Folder()
{
	m_Aera_Folder_Path[0] = 0;

	strcpy(m_Aera_Folder_Path, m_Level_Folder_Path);
	strcat(m_Aera_Folder_Path, "\\");
	strcat(m_Aera_Folder_Path, "Areas");


	if (_mkdir(m_Aera_Folder_Path) == 0)
	{
		(void) _chdir(m_Aera_Folder_Path);
	}
	else
	{
		(void) _chdir(m_Aera_Folder_Path);
	}

	Build_Areas_Data();

	(void) _chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  		Save_Areas_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Build_Game::Build_Areas_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Aera_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Areas.aer");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.2");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Counters]");
	fprintf(WriteFile, "%s%i\n", "Areas_Count=", App->CL_Scene->Area_Count);

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int Count = 0;
	while (Count < App->CL_Scene->Area_Count)
	{
		strcpy(buff, "[Area_");
		_itoa(Count, Cbuff, 10);
		strcat(buff, Cbuff);
		strcat(buff, "]");

		fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

		fprintf(WriteFile, "%s%s\n", "Area_Name=", App->CL_Scene->B_Area[Count]->Area_Name); // Change

		fprintf(WriteFile, "%s%s\n", "Area_File=", App->CL_Scene->B_Area[Count]->Area_FileName);
		fprintf(WriteFile, "%s%s\n", "Area_Path_File=", App->CL_Scene->B_Area[Count]->Area_Path_And_FileName);
		fprintf(WriteFile, "%s%s\n", "Area_Resource_Path=", App->CL_Scene->B_Area[Count]->Area_Resource_Path);
		fprintf(WriteFile, "%s%s\n", "Material_File=", App->CL_Scene->B_Area[Count]->Material_File);
		fprintf(WriteFile, "%s%i\n", "Area_Object_ID=", App->CL_Scene->B_Area[Count]->This_Object_UniqueID);

		// ------------ Position
		x = App->CL_Scene->B_Area[Count]->Area_Node->getPosition().x;
		y = App->CL_Scene->B_Area[Count]->Area_Node->getPosition().y;
		z = App->CL_Scene->B_Area[Count]->Area_Node->getPosition().z;
		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Pos=", x, y, z);

		// ------------ Scale
		x = App->CL_Scene->B_Area[Count]->Mesh_Scale.x;
		y = App->CL_Scene->B_Area[Count]->Mesh_Scale.y;
		z = App->CL_Scene->B_Area[Count]->Mesh_Scale.z;
		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Scale=", x, y, z);

		// ------------ Mesh_Rot
		x = App->CL_Scene->B_Area[Count]->Mesh_Rot.x;
		y = App->CL_Scene->B_Area[Count]->Mesh_Rot.y;
		z = App->CL_Scene->B_Area[Count]->Mesh_Rot.z;
		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Rot=", x, y, z);

		// ------------ Mesh_Quat
		w = App->CL_Scene->B_Area[Count]->Mesh_Quat.w;
		x = App->CL_Scene->B_Area[Count]->Mesh_Quat.x;
		y = App->CL_Scene->B_Area[Count]->Mesh_Quat.y;
		z = App->CL_Scene->B_Area[Count]->Mesh_Quat.z;
		fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Mesh_Quat=", w, x, y, z);


		fprintf(WriteFile, "%s\n", " ");
		Count++;
	}

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Build_Players_Folder:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Build_Game::Build_Players_Folder()
{
	m_Players_Folder_Path[0] = 0;

	strcpy(m_Players_Folder_Path, m_Level_Folder_Path);
	strcat(m_Players_Folder_Path, "\\");
	strcat(m_Players_Folder_Path, "Players");


	(void) _mkdir(m_Players_Folder_Path);

	(void)_chdir(m_Players_Folder_Path);

	Build_Player_Data();

	(void)_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  	Build_Player_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Build_Game::Build_Player_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	float W = 0;
	float X = 0;
	float Y = 0;
	float Z = 0;

	strcpy(File, m_Players_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Players.ply");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.2");

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

		W = App->CL_Scene->B_Player[Count]->Physics_Rotation.getW();
		X = App->CL_Scene->B_Player[Count]->Physics_Rotation.getX();
		Y = App->CL_Scene->B_Player[Count]->Physics_Rotation.getY();
		Z = App->CL_Scene->B_Player[Count]->Physics_Rotation.getZ();

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

	int RealCount = App->CL_LookUps->Player_Location_GetCount(); // Get The real Count Minus Deleted Files

	fprintf(WriteFile, "%s%i\n", "Locations_Count=", RealCount);

	int Location = 0; // Correct for Deleted Files

	Count = 0;
	while (Count < App->CL_Scene->Player_Location_Count)
	{
		if (App->CL_Scene->B_Locations[Count]->Deleted == 0)
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
	}

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Build_Cameras_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Build_Game::Build_Cameras_Folder()
{
	m_Cameras_Folder_Path[0] = 0;

	strcpy(m_Cameras_Folder_Path, m_Level_Folder_Path);
	strcat(m_Cameras_Folder_Path, "\\");
	strcat(m_Cameras_Folder_Path, "Cameras");

	if (_mkdir(m_Cameras_Folder_Path) == 0)
	{
		(void) _chdir(m_Cameras_Folder_Path);
	}
	else
	{
		(void) _chdir(m_Cameras_Folder_Path);
	}

	Build_Cameras_Data();

	(void) _chdir(m_Level_Folder_Path); // Return to Level Folder

	return 1;
}

// *************************************************************************
// *	  		Build_Cameras_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Build_Game::Build_Cameras_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Cameras_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Cameras.epf");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.2");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Counters]");
	fprintf(WriteFile, "%s%i\n", "Cameras_Count=", App->CL_Scene->Camera_Count);

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int Count = 0;
	while (Count < App->CL_Scene->Camera_Count)
	{
		strcpy(buff, "[Camera_");
		_itoa(Count, Cbuff, 10);
		strcat(buff, Cbuff);
		strcat(buff, "]");

		fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

		//fprintf(WriteFile, "%s%s\n", "Camera_Name=", App->CL_Scene->B_Camera[Count]->Camera_Name); // Change

		//---------------------------------- Camera Pos
		//x = App->CL_Scene->B_Camera[Count]->CamPos.x;
		//y = App->CL_Scene->B_Camera[Count]->CamPos.y;
		//z = App->CL_Scene->B_Camera[Count]->CamPos.z;

		fprintf(WriteFile, "%s%f,%f,%f\n", "Camera_Pos=", x, y, z);

		//---------------------------------- Camera Look At
		//x = App->CL_Scene->B_Camera[Count]->LookAt.x;
		//y = App->CL_Scene->B_Camera[Count]->LookAt.y;
		//z = App->CL_Scene->B_Camera[Count]->LookAt.z;

		fprintf(WriteFile, "%s%f,%f,%f\n", "LookAt=", x, y, z);

		//---------------------------------- Camera Quaternion
		//w = App->CL_Scene->B_Camera[Count]->Cam_Quat.w;
		//x = App->CL_Scene->B_Camera[Count]->Cam_Quat.x;
		//y = App->CL_Scene->B_Camera[Count]->Cam_Quat.y;
		//z = App->CL_Scene->B_Camera[Count]->Cam_Quat.z;

		fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Camera_Quat=", w, x, y, z);

		Count++;
	}

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Build_Objects_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Build_Game::Build_Objects_Folder()
{
	m_Objects_Folder_Path[0] = 0;

	strcpy(m_Objects_Folder_Path, m_Level_Folder_Path);
	strcat(m_Objects_Folder_Path, "\\");
	strcat(m_Objects_Folder_Path, "Objects");

	_mkdir(m_Objects_Folder_Path);
	_chdir(m_Objects_Folder_Path);

	Build_Objects_Data();

	_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  		Build_Objects_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Build_Game::Build_Objects_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Objects_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Objects.efd");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.2");

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
		if (App->CL_Scene->B_Object[Count]->Deleted == 0)
		{
			strcpy(buff, "[Object_");
			_itoa(new_Count, Cbuff, 10);
			strcat(buff, Cbuff);
			strcat(buff, "]");

			fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

			fprintf(WriteFile, "%s%s\n", "Mesh_Name=", App->CL_Scene->B_Object[Count]->Mesh_Name); // Change

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
			//---------------------------------------------------------------------------------- Message Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Message)
			{
				fprintf(WriteFile, "%s\n", "------------------- Message");
				fprintf(WriteFile, "%s%s\n", "Message_Text=", App->CL_Scene->B_Object[Count]->S_Message[0]->Message_Text);

				x = App->CL_Scene->B_Object[Count]->S_Message[0]->Message_PosX;
				y = App->CL_Scene->B_Object[Count]->S_Message[0]->Message_PosY;
				fprintf(WriteFile, "%s%f,%f\n", "Message_Pos=", x, y);

				// Message Counter
				fprintf(WriteFile, "%s%i\n", "Message_Counter_ID=", App->CL_Scene->B_Object[Count]->S_Message[0]->Counter_ID);
				fprintf(WriteFile, "%s%i\n", "Message_Trigger_Value=", App->CL_Scene->B_Object[Count]->S_Message[0]->Trigger_Value);
				fprintf(WriteFile, "%s%i\n", "Message_Counter_Disabled=", App->CL_Scene->B_Object[Count]->S_Message[0]->Counter_Disabled);

				fprintf(WriteFile, "%s%i\n", "Message_CentreX=", App->CL_Scene->B_Object[Count]->S_Message[0]->PosXCentre_Flag);
				fprintf(WriteFile, "%s%i\n", "Message_CentreY=", App->CL_Scene->B_Object[Count]->S_Message[0]->PosYCentre_Flag);

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

				fprintf(WriteFile, "%s%i\n", "Message_Show_BackGround=", App->CL_Scene->B_Object[Count]->S_Message[0]->Show_BackGround);
			}

			//---------------------------------------------------------------------------------- Sound Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Sound)
			{
				fprintf(WriteFile, "%s%s\n", "Sound_File=", App->CL_Scene->B_Object[Count]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Sound_Volume=", App->CL_Scene->B_Object[Count]->SndVolume);
			}

			//---------------------------------------------------------------------------------- Colectable Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Colectable)
			{
				fprintf(WriteFile, "%s%s\n", "Col_Sound_File=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Col_Sound_Volume=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->SndVolume);
				fprintf(WriteFile, "%s%i\n", "Col_Play=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Play);
				fprintf(WriteFile, "%s%s\n", "Col_Counter_Name=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Counter_Name);
				fprintf(WriteFile, "%s%i\n", "Col_Counter_ID=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Counter_ID);
				fprintf(WriteFile, "%s%i\n", "Col_Maths=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Maths);
				fprintf(WriteFile, "%s%i\n", "Col_Value=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Value);
				fprintf(WriteFile, "%s%i\n", "Col_Disabled=", App->CL_Scene->B_Object[Count]->S_Collectable[0]->Counter_Disabled);

			}

			//---------------------------------------------------------------------------------- Move Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Move)
			{
				fprintf(WriteFile, "%s%f\n", "Move_Distance=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Move_Distance);
				fprintf(WriteFile, "%s%i\n", "Move_IsNegative=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->IsNegative);
				//	fprintf(WriteFile, "%s%s\n", "Move_MeshPos=", App->CL_Scene->B_Object[Count]->S_MoveType->MeshPos);
				fprintf(WriteFile, "%s%f\n", "Move_NewPos=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Newpos);
				fprintf(WriteFile, "%s%i\n", "Move_ObjectID=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Object_To_Move_Index);
				fprintf(WriteFile, "%s%s\n", "Move_ObjectName=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Object_Name);
				//	fprintf(WriteFile, "%s%s\n", "Move_PhysicsPos=", App->CL_Scene->B_Object[Count]->S_MoveType->PhysicsPos);
				fprintf(WriteFile, "%s%i\n", "Move_Re_Trigger=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Re_Trigger);
				fprintf(WriteFile, "%s%f\n", "Move_Speed=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Speed);
				fprintf(WriteFile, "%s%i\n", "Move_Triggered=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Triggered);
				fprintf(WriteFile, "%s%i\n", "Move_WhatDirection=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->WhatDirection);

				// Move Sound
				fprintf(WriteFile, "%s%s\n", "Move_Sound=", App->CL_Scene->B_Object[Count]->Sound_File);
				fprintf(WriteFile, "%s%i\n", "Move_Play_Sound=", App->CL_Scene->B_Object[Count]->Play_Sound);
				fprintf(WriteFile, "%s%f\n", "Move_Volume=", App->CL_Scene->B_Object[Count]->SndVolume);

				// Move Counter
				fprintf(WriteFile, "%s%i\n", "Move_Counter_ID=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Counter_ID);
				fprintf(WriteFile, "%s%i\n", "Move_Trigger_Value=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Trigger_Value);
				fprintf(WriteFile, "%s%i\n", "Move_Counter_Disabled=", App->CL_Scene->B_Object[Count]->S_MoveType[0]->Counter_Disabled);

			}

			//---------------------------------------------------------------------------------- Teleport Entity
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Teleport)
			{
				fprintf(WriteFile, "%s%s\n", "Tele_Goto=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Location_Name);

				fprintf(WriteFile, "%s%i\n", "Tele_ID=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Location_ID);

				fprintf(WriteFile, "%s%s\n", "Tele_Sound=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Tele_Volume=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->SndVolume);
				fprintf(WriteFile, "%s%i\n", "Tele_Play=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Play);

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
				fprintf(WriteFile, "%s%i\n", "Tele_Counter_Disabled=", App->CL_Scene->B_Object[Count]->S_Teleport[0]->Counter_Disabled);

				//--------------- Environment
				fprintf(WriteFile, "%s\n", "------------------------------------------------------------------------------ Teleporter Environ");
				fprintf(WriteFile, "%s%i\n", "Environ_Enabled=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environ_Enabled);
				fprintf(WriteFile, "%s%s\n", "Environment_Name=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environment_Name);
				fprintf(WriteFile, "%s%i\n", "Environment_ID=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environment_ID);

				//--------------- Sound
				fprintf(WriteFile, "%s%s\n", "Sound_File=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Snd_Volume=", App->CL_Scene->B_Object[Count]->S_Environ[0]->SndVolume);

				fprintf(WriteFile, "%s%i\n", "Sound_Play=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Play);
				fprintf(WriteFile, "%s%i\n", "Sound_Loop=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Loop);

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
				fprintf(WriteFile, "%s%i\n", "Sky_Enable=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Enabled);
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
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_EnvironEntity)
			{
				fprintf(WriteFile, "%s\n", "------------------- EnvironEntity");
				fprintf(WriteFile, "%s%s\n", "Environment_Name=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environment_Name);
				fprintf(WriteFile, "%s%i\n", "Environment_ID=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Environment_ID);

				//--------------- Sound
				fprintf(WriteFile, "%s%s\n", "Sound_File=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Snd_Volume=", App->CL_Scene->B_Object[Count]->S_Environ[0]->SndVolume);

				fprintf(WriteFile, "%s%i\n", "Sound_Play=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Play);
				fprintf(WriteFile, "%s%i\n", "Sound_Loop=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Loop);

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
				fprintf(WriteFile, "%s%i\n", "Sky_Enable=", App->CL_Scene->B_Object[Count]->S_Environ[0]->Enabled);
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
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Particle)
			{
				fprintf(WriteFile, "%s\n", "-- Particle");
				fprintf(WriteFile, "%s%s\n", "Particle_Script=", App->CL_Scene->B_Object[Count]->S_Particle[0]->ParticleScript);
				fprintf(WriteFile, "%s%f\n", "Particle_SpeedFactor=", App->CL_Scene->B_Object[Count]->S_Particle[0]->SpeedFactor);

			}

			fprintf(WriteFile, "%s\n", " ");
			fprintf(WriteFile, "%s\n", "***************************************************************************************");

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
// *	  	Build_Display_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Build_Game::Build_Display_Folder()
{
	m_Display_Folder_Path[0] = 0;

	strcpy(m_Display_Folder_Path, m_Level_Folder_Path);
	strcat(m_Display_Folder_Path, "\\");
	strcat(m_Display_Folder_Path, "Display");

	_mkdir(m_Display_Folder_Path);
	_chdir(m_Display_Folder_Path);

	Build_Display_Data();

	_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  		Build_Display_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Build_Game::Build_Display_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Display_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Counters.edf");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say_Win(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.2");

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
	while (Count < App->CL_Scene->Counters_Count)
	{
		if (App->CL_Scene->B_Counter[Count]->Deleted == 0)
		{
			strcpy(buff, "[Counter_");
			_itoa(new_Count, Cbuff, 10);
			strcat(buff, Cbuff);
			strcat(buff, "]");

			fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

			fprintf(WriteFile, "%s%s\n", "Counter_Name=", App->CL_Scene->B_Counter[Count]->Panel_Name); // Change
			fprintf(WriteFile, "%s%i\n", "Counter_ID=", App->CL_Scene->B_Counter[Count]->Unique_ID);

			x = App->CL_Scene->B_Counter[Count]->PosX;
			y = App->CL_Scene->B_Counter[Count]->PosY;
			fprintf(WriteFile, "%s%f,%f\n", "Counter_Pos=", x, y);

			fprintf(WriteFile, "%s%s\n", "Counter_Text=", App->CL_Scene->B_Counter[Count]->Text);
			fprintf(WriteFile, "%s%i\n", "Counter_Display=", App->CL_Scene->B_Counter[Count]->Show_Panel_Flag);
			fprintf(WriteFile, "%s%i\n", "Counter_Start=", App->CL_Scene->B_Counter[Count]->Start_Value);

			fprintf(WriteFile, "%s\n", " ");
			new_Count++;
		}

		Count++;
	}

	fprintf(WriteFile, "%s\n", "[Counters]");
	fprintf(WriteFile, "%s%i\n", "Counters_Count=", new_Count);

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  		Copy_Assets:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Build_Game::Copy_Assets(char* SourceFolder, char* DestinationFolder)
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

// *************************************************************************
// *	  		Zip_Assets:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Build_Game::Zip_Assets(char* SourceFolder, char* DestinationFolder)
{
	HZIP hz;

	hz = CreateZip(_T("Assets.zip"), 0);

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

				//CopyFile(SourceFile, DestinationFile, false);
				ZipAdd(hz, _T(fd.cFileName), _T(SourceFile));
			}

		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	CloseZip(hz);

	return 1;
}
