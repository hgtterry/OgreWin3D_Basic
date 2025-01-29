/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
#include "CL64_Export.h"

#include <string>
#include <shobjidl_core.h>

CL64_Export::CL64_Export()
{
	mFolder_Path[0] = 0;
	mDirectory_Name[0] = 0;
	strcpy(mJustName,"World1");

	DeskTop_Folder[0] = 0;

	flag_Build_Edge_List = 1;
}

CL64_Export::~CL64_Export()
{
}

// *************************************************************************
// *	  		Ogre_Export_Dlg:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Export::Ogre_Export_Dlg()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_OGRE_EXPORT, App->MainHwnd, (DLGPROC)Proc_Ogre_Export_Dlg);
}

// *************************************************************************
// *		Proc_Ogre_Export_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Export::Proc_Ogre_Export_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_BANNER, WM_SETFONT, (WPARAM)App->Font_Banner, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_STFILENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STPATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STOGRESUB, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_OGRE_FILENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_OGRE_PATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_OGRE_SUBFOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_OGRE_NAMECHANGE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_OGREBROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_CK_BL_DESKTOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_CREATE_SUBDIR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_BUILDEDGELIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_ST_OGRE_FILENAME, (LPCTSTR)App->CL_Export->mJustName);
		SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);
		
		
		strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
		strcat(App->CL_Export->mDirectory_Name, "_Ogre_All");

		SetDlgItemText(hDlg, IDC_ST_OGRE_SUBFOLDER, (LPCTSTR)App->CL_Export->mDirectory_Name);
		
		HWND Temp = GetDlgItem(hDlg, IDC_CK_CREATE_SUBDIR);
		SendMessage(Temp, BM_SETCHECK, 1, 0);

		Temp = GetDlgItem(hDlg, IDC_CK_BUILDEDGELIST);
		if (App->CL_Export->flag_Build_Edge_List == 1)
		{
			SendMessage(Temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			SendMessage(Temp, BM_SETCHECK, 0, 0);
		}
		
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_ST_OGRE_FILENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_OGRE_PATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_OGRE_SUBFOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_STFILENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STPATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STOGRESUB) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_CK_BL_DESKTOP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_CK_CREATE_SUBDIR) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BUILDEDGELIST) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		return FALSE;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_OGRE_NAMECHANGE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_OGREBROWSE)
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
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_OGRE_NAMECHANGE)
		{
			strcpy(App->CL_Dialogs->btext, "Change File Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Export->mJustName);

			App->CL_Dialogs->Dialog_Text(Enums::Check_Name_None);

			if (App->CL_Dialogs->flag_Dlg_Canceled == 0)
			{
				strcpy(App->CL_Export->mJustName, App->CL_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_OGRE_FILENAME, App->CL_Export->mJustName);
			
			strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
			strcat(App->CL_Export->mDirectory_Name, "_Ogre_All");

			SetDlgItemText(hDlg, IDC_ST_OGRE_SUBFOLDER, (LPCTSTR)App->CL_Export->mDirectory_Name);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_OGREBROWSE)
		{
			App->CL_File_IO->Select_Folder();
			if (App->CL_File_IO->flag_Canceled == 0)
			{
				strcpy(App->CL_Export->mFolder_Path, App->CL_File_IO->szSelectedDir);
				SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);
			}
			
			return TRUE;
		}
	
		if (LOWORD(wParam) == IDC_CK_BL_DESKTOP)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BL_DESKTOP);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				strcpy(App->CL_Export->mFolder_Path, App->CL_Export->DeskTop_Folder);
				SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);
				return 1;
			}
			else
			{
				return 1;
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_CK_BUILDEDGELIST)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BUILDEDGELIST);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Export->flag_Build_Edge_List = 1;
				return 1;
			}
			else
			{
				App->CL_Export->flag_Build_Edge_List = 0;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			// Check Path
			int result = strcmp(App->CL_Export->mFolder_Path, "");
			if (result == 0)
			{
				App->Say("No Path Selected");
				return 1;
			}

			strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
			strcat(App->CL_Export->mDirectory_Name, "_Ogre_All");

			App->CL_Ogre3D->Export_To_Ogre3D();

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	}

	return FALSE;
}

