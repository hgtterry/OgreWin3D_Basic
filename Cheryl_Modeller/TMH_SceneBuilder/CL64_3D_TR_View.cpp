/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
#include "CL64_3D_TR_View.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

CL64_3D_TR_View::CL64_3D_TR_View()
{
	Render_hWnd = nullptr;
}

CL64_3D_TR_View::~CL64_3D_TR_View()
{
}


// *************************************************************************
// *			Create_Top_Left_Window:- Terry Mo and Hazel  2026		   *
// *************************************************************************
void CL64_3D_TR_View::Create_Top_Left_Window()
{
	//App->CL_Editor_Map->Top_Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, App->CL_Editor_Map->Main_View_Dlg_Hwnd, (DLGPROC)Proc_Top_Left_Window);

}

// *************************************************************************
// *		  Proc_Top_Left_Window:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_3D_TR_View::Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TL_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_Editor_Map->Top_Left_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_TL_TITLE);

		return TRUE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_TL_TITLE) == (HWND)lParam)
		{
			if (App->CL_Editor_Map->Selected_Window == Enums::Selected_Map_View_TL)
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->Brush_Green;
			}
			else
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
		}

		return FALSE;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		return 1;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		return CDRF_DODEFAULT;
	}

	case WM_MOUSEWHEEL:
	{
		//int zDelta = (short)HIWORD(wParam);    // wheel rotation

		//if (zDelta > 0)
		//{
		//	App->CL_3D_TR_View->RenderListener->Wheel_Move = -1;
		//}
		//else if (zDelta < 0)
		//{
		//	App->CL_3D_TR_View->RenderListener->Wheel_Move = 1;
		//}

		return 1;
	}

	case WM_COMMAND:
	{
		break;
	}

	case WM_PAINT:
	{
		//App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
		//App->CL_Editor_Map->Draw_Screen(hDlg);

		return 0;
	}

	}

	return FALSE;
}



