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
#include "CL64_Properties_Textures.h"

CL64_Properties_Textures::CL64_Properties_Textures(void)
{
	TexturesDlg_Hwnd = nullptr;
}

CL64_Properties_Textures::~CL64_Properties_Textures(void)
{
}

// *************************************************************************
// *	  	Start_Tabs_Textures_Dlg:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Start_Tabs_Textures_Dlg()
{
	TexturesDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEXTURES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_Tabs_Textures_Dlg);
}

// **************************************************************************
// *			Proc_Tabs_Textures_Dlg:- Terry and Hazel Flanigan 2026
// **************************************************************************
LRESULT CALLBACK CL64_Properties_Textures::Proc_Tabs_Textures_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& p_Faces = App->CL_Faces_Control; // Pointer to Faces Control

	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_FACES_NONE2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACES_ALL2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACE_NEXT2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACE_PREV2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_TITLENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;
		LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

		switch (some_item->idFrom)
		{
		
		case IDC_BT_FACES_NONE2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_FACES_NONE2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_No_Faces);
			}

			break;
		}

		case IDC_BT_TT_FACES_ALL2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACES_ALL2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_All_Faces);
			}

			break;
		}

		case IDC_BT_TT_FACE_NEXT2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACE_NEXT2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_Next_Face);
			}

			break;
		}

		case IDC_BT_TT_FACE_PREV2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACE_PREV2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_Prev_Face);
			}

			break;
		}

		default:
			return CDRF_DODEFAULT;
		}
		

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_FACES_NONE2)
		{
			p_Faces->Unselect_All_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACES_ALL2)
		{
			p_Faces->Select_All_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACE_NEXT2)
		{
			p_Faces->Select_Next_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACE_PREV2)
		{
			p_Faces->Select_Prev_Face();
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}
