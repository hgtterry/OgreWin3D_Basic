#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Brushes_Control.h"

CL64_Brushes_Control::CL64_Brushes_Control(void)
{
	Brush_Control_Dlg_hWnd = nullptr;
}

CL64_Brushes_Control::~CL64_Brushes_Control(void)
{
}

// *************************************************************************
// *		Start_Brushes_Control_Dlg:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Brushes_Control::Start_Brushes_Control_Dlg()
{
	Brush_Control_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOP_TABS_BRUSHES, App->MainHwnd, (DLGPROC)Proc_Top_Tabs_Brushes);
	App->CL_Interface->Show_Faces_Panel_Control(false);
}

// **************************************************************************
// *		Proc_Top_Tabs_Brushes:- Terry and Hazel Flanigan 2026			*
// **************************************************************************
LRESULT CALLBACK CL64_Brushes_Control::Proc_Top_Tabs_Brushes(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TT_BRUSH_SELECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_BRUSH_MOVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_BRUSH_ROTATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_BRUSH_SCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_BRUSH_SHEAR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TT_BRUSH_SELECT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_BRUSH_SELECT));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Select);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_BRUSH_MOVE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_BRUSH_MOVE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Move);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_BRUSH_ROTATE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_BRUSH_ROTATE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Rotate);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_BRUSH_SCALE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_BRUSH_SCALE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Scale);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_BRUSH_SHEAR)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_BRUSH_SHEAR));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TT_BRUSH_SELECT)
		{
			App->CL_Interface->Unselect_Brush_And_Set_Dlgs();
			App->CL_Interface->Show_Faces_Panel_Control(false);
			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_BRUSH_MOVE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 1);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_BRUSH_SCALE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_SCALEBRUSH, 2);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_BRUSH_ROTATE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 3);
			return TRUE;
		}

		break;
	}

	}

	return FALSE;
}
