#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Properties_Motions.h"

CL64_Properties_Motions::CL64_Properties_Motions()
{
	Motions_Dlg_Hwnd = nullptr;
}

CL64_Properties_Motions::~CL64_Properties_Motions()
{
}

// *************************************************************************
// *	  	Show_Motions_Dialog_Ogre:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Properties_Motions::Show_Motions_Dialog(bool Show)
{
	ShowWindow(Motions_Dlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Start_Motions_Dialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Motions::Start_Motions_Dialog()
{
	Motions_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_MOTIONS, App->MainHwnd, (DLGPROC)Proc_Motions_Dialog);
	Show_Motions_Dialog(true);
}

// *************************************************************************
// *	     Proc_Motions_Dialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Motions::Proc_Motions_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_MOT_PLAY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MOT_STOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_ST_PT_DIMENSIONS) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_PT_NUMTEXTUNITS) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	/*case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
	}*/

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_MOT_PLAY)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Motions->flag_Motion_Playing);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_MOT_STOP)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		/*if (some_item->idFrom == IDC_BT_MOTIONS_PAUSE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Motions->flag_Motion_Paused);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_MOT_PLAY)
		{
			if (App->CL_ImGui->PreviouseMotion_Ogre > -1)
			{
				App->CL_Motions->Play_SelectedMotion();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MOT_STOP)
		{
			App->CL_Motions->Stop_SelectedMotion();
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			//App->CL_Interface->Show_Materials_Dlg(false);
			return TRUE;
		}

		break;
	}

	}
	return FALSE;
}
