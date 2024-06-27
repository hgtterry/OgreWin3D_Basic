/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Dialogs.h"

CL64_Dialogs::CL64_Dialogs(void)
{
	Message_Text[0] = 0;
	Canceled = 0;
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
		//App->SetTitleBar(hDlg);

		HFONT Font;
		Font = CreateFont(-20, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
		SendDlgItemMessage(hDlg, IDC_STTEXT, WM_SETFONT, (WPARAM)Font, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)Font, MAKELPARAM(TRUE, 0));

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

		if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
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
// *	  Start_Import_Options_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Start_Import_Options_Dlg()
{
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
		/*LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_COVERT_TO_OGRE);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Converters->Convert_ToOgre3D(1);
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
