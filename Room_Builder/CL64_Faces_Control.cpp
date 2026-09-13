/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Scene Builder

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
#include "CL64_Faces_Control.h"

CL64_Faces_Control::CL64_Faces_Control(void)
{
	flag_Brush_Select = true;
	flag_Brush_Move = false;
	flag_Brush_Rotate = false;
	flag_Brush_Scale = false;

	Faces_Control_Dlg_hWnd = nullptr;
}

CL64_Faces_Control::~CL64_Faces_Control(void)
{
}

// *************************************************************************
// *		Start_Faces_Control_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Faces_Control::Start_Faces_Control_Dlg()
{
	Faces_Control_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOP_TABS_FACES, App->MainHwnd, (DLGPROC)Proc_Top_Tabs_Faces);
	App->CL_Interface->Show_Faces_Panel_Control(false);
}

// **************************************************************************
// *		Proc_Top_Tabs_Faces:- Terry and Hazel Flanigan 2026				*
// **************************************************************************
LRESULT CALLBACK CL64_Faces_Control::Proc_Top_Tabs_Faces(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& p_Faces = App->CL_Faces_Control; // Pointer to Faces Control

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_BRUSHNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_TT_SELECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_MOVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_ROTATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_SCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_SHEAR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_FACEAMOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BRUSHNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_FACEAMOUNT) == (HWND)lParam)
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
		LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

		switch (some_item->idFrom)
		{
		case IDC_BT_TT_SELECT:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_SELECT));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_Brush_Select);
			}

			break;
		}

		case IDC_BT_TT_MOVE:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_MOVE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_Brush_Move);
			}

			break;
		}

		case IDC_BT_TT_ROTATE:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_ROTATE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_Brush_Rotate);
			}
			break;
		}

		case IDC_BT_TT_SCALE:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_SCALE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_Brush_Scale);
			}

			break;
		}

		case IDC_BT_TT_SHEAR:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_SHEAR));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			break;
		}

		case IDCANCEL:
		{
			App->Custom_Button_Normal(item);
			break;
		}

		default:
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TT_SELECT)
		{
			App->CL_Interface->Unselect_Brush_And_Set_Dlgs();
			App->CL_Interface->Enable_All_Face_Buttons(false);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_MOVE)
		{
			p_Faces->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 1);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_SCALE)
		{
			p_Faces->Set_Brush_Mode(ID_TOOLS_BRUSH_SCALEBRUSH, 2);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_ROTATE)
		{
			p_Faces->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 3);
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Textures->Unselect_All_Face();
			App->CL_Interface->Unselect_Brush_And_Set_Dlgs();

			App->CL_Interface->Show_Faces_Panel_Control(false);
			App->CL_Interface->flag_Faces_Con_Dlg_Active = false;

			if (App->CL_X_Face_Editor->flag_FaceDlg_Active == true)
			{
				App->CL_X_Face_Editor->Close_Faces_Dialog();
			}

			return TRUE;
		}

		break;
	}

	}
	return FALSE;
}

// *************************************************************************
// *	  	Reset_Brush_Buttons:- Terry and Hazel Flanigan 2026			   
// *************************************************************************
void CL64_Faces_Control::Reset_Brush_Buttons()
{
	flag_Brush_Select = false;
	flag_Brush_Move = false;
	flag_Brush_Rotate = false;
	flag_Brush_Scale = false;

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
}

// *************************************************************************
// *			Set_Brush_Mode:- Terry and Hazel Flanigan 2026			   
// *************************************************************************
void CL64_Faces_Control::Set_Brush_Mode(int Mode, int Dlg_Selection)
{
	SetCursor(App->CL_Views_Com->hcBoth);

	App->CL_Doc->ResetAllSelectedFaces();;
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	Reset_Brush_Buttons();

	if (Dlg_Selection == 1)
	{
		flag_Brush_Move = 1;
	}

	if (Dlg_Selection == 2)
	{
		flag_Brush_Scale = 1;
	}

	if (Dlg_Selection == 3)
	{
		flag_Brush_Rotate = 1;
	}

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

	App->CL_Doc->mCurrentTool = CURTOOL_NONE;
	//App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_SCALEBRUSH;
	App->CL_Doc->mModeTool = Mode;
}
