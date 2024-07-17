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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Dialogs.h"

CL64_Dialogs::CL64_Dialogs(void)
{
	Message_Text[0] = 0;
	MessageString[0] = 0;
	MessageString2[0] = 0;

	Canceled = 0;

	Flag_Convert_to_Ogre = 0;

	flag_Mouse_Normal = 1;
	flag_Mouse_Slow = 0;
	flag_Mouse_VerySlow = 0;
	flag_Mouse_Fast = 0;
}

CL64_Dialogs::~CL64_Dialogs(void)
{
}

// *************************************************************************
// *	  			Message:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Dialogs::Message(char* pString)
{
	strcpy(Message_Text, pString);
	DialogBox(App->hInst, (LPCTSTR)IDD_MESSAGE, App->Fdlg, (DLGPROC)Message_Proc);
}

// *************************************************************************
// *        	Message_Proc:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Message_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STTEXT, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SetDlgItemText(hDlg, IDC_STTEXT, App->CL_Dialogs->Message_Text);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_STTEXT) == (HWND)lParam)
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
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	return FALSE;
}

// *************************************************************************
// *	  			PleaseWait:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::PleaseWait()
{
	App->ViewPLeaseWait = CreateDialog(App->hInst, (LPCTSTR)IDD_PLEASEWAIT, App->Fdlg, (DLGPROC)PleaseWait_Proc);
}

// *************************************************************************
// *			PleaseWait_Proc:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::PleaseWait_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		HFONT Font;
		Font = CreateFont(-25, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Aerial Black");
		SendDlgItemMessage(hDlg, IDC_STWAIT, WM_SETFONT, (WPARAM)Font, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Green;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STWAIT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_Green;
		}

		return FALSE;
	}
	}
	return FALSE;
}

// *************************************************************************
// *	  Start_Import_Options_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Start_Import_Options_Dlg()
{
	Flag_Convert_to_Ogre = 0;

	DialogBox(App->hInst, (LPCTSTR)IDD_IMPORT_OPTIONS, App->Fdlg, (DLGPROC)Import_Options_Dlg_Proc);
}

// *************************************************************************
// *        Import_Options_Dlg_Proc:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Import_Options_Dlg_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_CK_COVERT_TO_OGRE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_CREATEPHYSICS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_CK_COVERT_TO_OGRE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_CREATEPHYSICS) == (HWND)lParam)
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

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_COVERT_TO_OGRE);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Dialogs->Flag_Convert_to_Ogre = 1;
			}
			
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
// *	  		Show_YesNo_Dlg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::Show_YesNo_Dlg(char* Text, char* Text2)
{
	Canceled = 0;

	MessageString[0] = 0;
	MessageString2[0] = 0;

	strcpy(MessageString, Text);
	strcpy(MessageString2, Text2);

	DialogBox(App->hInst, (LPCTSTR)IDD_YESNO, App->Fdlg, (DLGPROC)YesNo_Proc);

}

// *************************************************************************
// *			YesNo_Proc_Proc:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::YesNo_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BANNER_YN, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXT_YN, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_BANNER_YN, App->CL_Dialogs->MessageString);
		SetDlgItemText(hDlg, IDC_STTEXT_YN, App->CL_Dialogs->MessageString2);
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_BANNER_YN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_STTEXT_YN) == (HWND)lParam)
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

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}

	return FALSE;
}

// *************************************************************************
// *	  	Start_Speed_Camera_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Start_Speed_Camera_Dlg()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_MOUSESENSITIVITY, App->MainHwnd, (DLGPROC)Speed_Camera_Proc);
}

// *************************************************************************
// *		Speed_Camera_Proc:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Speed_Camera_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STBANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CKNORMAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKSLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKVERYSLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKFAST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STBANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
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

		if (some_item->idFrom == IDC_CKNORMAL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_Mouse_Normal);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKSLOW && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_Mouse_Slow);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKVERYSLOW && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_Mouse_VerySlow);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKFAST && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_Mouse_Fast);
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

		if (LOWORD(wParam) == IDC_CKNORMAL)
		{
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 50;
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 50;

			App->CL_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CL_Dialogs->flag_Mouse_Normal = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKSLOW)
		{
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 5;
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 5;
			
			App->CL_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CL_Dialogs->flag_Mouse_Slow = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKVERYSLOW)
		{
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 1;
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 1;
			
			App->CL_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CL_Dialogs->flag_Mouse_VerySlow = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKFAST)
		{
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 250;
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 250;

			App->CL_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CL_Dialogs->flag_Mouse_Fast = 1;

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
// *	UnCheck_All_SpeedMouseOption:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void CL64_Dialogs::UnCheck_All_SpeedMouseOption()
{
	flag_Mouse_Normal = 0;
	flag_Mouse_Slow = 0;
	flag_Mouse_VerySlow = 0;
	flag_Mouse_Fast = 0;
}
