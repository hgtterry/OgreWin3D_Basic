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
#include "CL64_Exporters.h"

CL64_Exporters::CL64_Exporters(void)
{
	mJustName[0] = 0;
	mDirectory_Name[0] = 0;
	mFolder_Path[0] = 0;
	Is_Canceled = 0;
}

CL64_Exporters::~CL64_Exporters(void)
{
}

// *************************************************************************
// *			Start_Export:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_Exporters::Export_Ogre(bool Use_FileDialog)
{
	if (Use_FileDialog == 1)
	{
		strcpy(App->CL_File_IO->BrowserMessage, "Select Folder To Place Ogre3D Files a sub folder will be created");
		int Test = App->CL_File_IO->StartBrowser((LPSTR)"");

		if (Test == 0)
		{
			return 1;
		}
	}

	Start_Ogre_Export_Dlg();

	if (Is_Canceled == 1)
	{
		return 1;
	}

	App->CL_Exp_Ogre->Export_To_Ogre3D(1);
	App->Say("Exported");

	return 1;
}

// *************************************************************************
// *	  	Start_Ogre_Export_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Exporters::Start_Ogre_Export_Dlg()
{
	Is_Canceled = 0;

	DialogBox(App->hInst, (LPCTSTR)IDD_EXPORT_OGRE_OPTIONS, App->MainHwnd, (DLGPROC)Export_Ogre_Dlg_Proc);
}

// *************************************************************************
// *        Export_Dlg_Proc:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
LRESULT CALLBACK CL64_Exporters::Export_Ogre_Dlg_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_FOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CHANGE_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_SUBFOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SUBFOLDER_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_EDGE_LIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_FOLDER_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_FLD, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_FN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Exporters->Set_Dialog_Data(hDlg);

		HWND Temp = GetDlgItem(hDlg, IDC_CK_SUBFOLDER);
		SendMessage(Temp, BM_SETCHECK, BST_CHECKED, 0);

		Temp = GetDlgItem(hDlg, IDC_CK_EDGE_LIST);
		SendMessage(Temp, BM_SETCHECK, BST_CHECKED, 0);
		App->CL_Exp_Ogre->flag_Create_Edge_List = 1;

		// Needs Looking At
		EnableWindow(GetDlgItem(hDlg, IDC_ST_SUBFOLDER_NAME), 0);
		EnableWindow(GetDlgItem(hDlg, IDC_BT_FOLDER_NAME), 0);
		EnableWindow(GetDlgItem(hDlg, IDC_CK_SUBFOLDER), 0);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_FOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_SUBFOLDER_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_CK_SUBFOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_EDGE_LIST) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_FLD) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_FN) == (HWND)lParam)
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


		if (some_item->idFrom == IDC_BT_FOLDER_NAME && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CHANGE_NAME && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_BROWSE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_CK_SUBFOLDER)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SUBFOLDER);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				EnableWindow(GetDlgItem(hDlg, IDC_ST_SUBFOLDER_NAME), 1);
				EnableWindow(GetDlgItem(hDlg, IDC_BT_FOLDER_NAME), 1);
				return 1;
			}
			else
			{
				EnableWindow(GetDlgItem(hDlg, IDC_ST_SUBFOLDER_NAME), 0);
				EnableWindow(GetDlgItem(hDlg, IDC_BT_FOLDER_NAME), 0);
				return 1;
			}

			return 1;
		}

		if (LOWORD(wParam) == IDC_CK_EDGE_LIST)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_EDGE_LIST);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Exp_Ogre->flag_Create_Edge_List = 1;
				return 1;
			}
			else
			{
				App->CL_Exp_Ogre->flag_Create_Edge_List = 0;
				return 1;
			}

			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BT_CHANGE_NAME)
		{
			strcpy(App->CL_Dialogs->btext, "Change File Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Exporters->mJustName);

			App->CL_Dialogs->Dialog_Text(Enums::Check_Name_None);

			if (App->CL_Dialogs->Canceled == 0)
			{
				strcpy(App->CL_Exporters->mJustName, App->CL_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_NAME, App->CL_Exporters->mJustName);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_FOLDER_NAME)
		{
			/*strcpy(App->CL_Dialogs->btext, "Change Folder Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Export_Ogre3D->Directory_Name);

			App->CL_Dialogs->Dialog_Text();

			if (App->CL_Dialogs->Is_Canceled == 0)
			{
				strcpy(App->CLSB_Export_Ogre3D->Directory_Name, App->CL_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_SUBFOLDER_NAME, App->CL_Export_Ogre3D->Directory_Name);*/

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BROWSE)
		{
			strcpy(App->CL_File_IO->BrowserMessage, "Select Folder To Place Ogre Files");
			int Test = App->CL_File_IO->StartBrowser((LPSTR)"");
			if (Test == 0) { return 1; }

			SetDlgItemText(hDlg, IDC_ST_FOLDER, App->CL_File_IO->szSelectedDir);

			strcpy(App->CL_Exporters->mFolder_Path, App->CL_File_IO->szSelectedDir);

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Exporters->Is_Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Exporters->Is_Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	}

	return FALSE;
}

// *************************************************************************
// *			Set_Dialog_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Exporters::Set_Dialog_Data(HWND m_hDlg)
{
	// Just Name
	char buf[MAX_PATH];
	strcpy(buf, App->CL_Scene->FileName);
	int Len = strlen(buf);
	buf[Len - 4] = 0;
	strcpy(mJustName, buf);
	SetDlgItemText(m_hDlg, IDC_ST_NAME, buf);

	// Folder Path
	strcpy(mFolder_Path, App->CL_File_IO->szSelectedDir);
	SetDlgItemText(m_hDlg, IDC_ST_FOLDER, App->CL_File_IO->szSelectedDir);

	// Directory Name
	strcpy(mDirectory_Name, App->CL_Exporters->mJustName);
	strcat(mDirectory_Name, "_Ogre_All");
	SetDlgItemText(m_hDlg, IDC_ST_SUBFOLDER_NAME, App->CL_Exporters->mDirectory_Name);

}

// *************************************************************************
// *			Object_Model:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Exporters::Object_Model(void)
{
	if (App->CL_Scene->flag_Model_Loaded == 0)
	{
		App->Say("No Model Loaded to Export");
		return;
	}

	bool test = App->CL_Exp_Obj->Create_ObjectFile();

	if (test == 1)
	{
		App->Say("Wavefront Object file Created successfully");
	}
}
