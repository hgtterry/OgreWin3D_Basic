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
#include "CL64_3D_TL_View.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

CL64_3D_TL_View::CL64_3D_TL_View()
{
	Render_hWnd = nullptr;

	CursorPosX = 0;
	CursorPosY = 0;
}

CL64_3D_TL_View::~CL64_3D_TL_View()
{
}

// *************************************************************************
// *			Create_Top_Left_Window:- Terry Mo and Hazel  2026		   *
// *************************************************************************
void CL64_3D_TL_View::Create_Top_Left_Window()
{
	App->CL_Editor_Map->Top_Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, App->CL_Editor_Map->Main_View_Dlg_Hwnd, (DLGPROC)Proc_Top_Left_Window);

}

// *************************************************************************
// *		  Proc_Top_Left_Window:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_3D_TL_View::Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TL_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_Editor_Map->Top_Left_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_TL_TITLE);

		//App->CL_3D_TL_View->ViewGLhWnd_TL = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_RENDER_WINDOW, hDlg, (DLGPROC)Proc_Ogre_TL);

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

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		return CDRF_DODEFAULT;
	}

	case WM_SETCURSOR:
	{
		if (App->CL_Editor_Map->flag_Context_Menu_Active == 1)
		{
			return false;
		}

		if (App->CL_Editor_Map->flag_Right_Button_Down == 1 || App->CL_Editor_Map->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			SetCursor(App->CL_Editor_Map->hcBoth);
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
		{
			return true;
		}
		else
		{
			return false;
		}

		return false;
	}

	case WM_MOUSEMOVE:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->On_Mouse_Move(RealCursorPosition,hDlg);

		return 1;
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

	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		App->CL_Editor_Map->flag_Left_Button_Down = 0;
		App->CL_Editor_Map->flag_Right_Button_Down = 0;

		App->CL_Editor_Map->On_Left_Button_Up(RealCursorPosition);

		return 1;
	}

	case WM_MOUSEWHEEL:
	{
			if (App->CL_Editor_Map->flag_Left_Button_Down == true)
		{
			return 1;
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0 && App->CL_Editor_Map->Selected_Window == Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->flag_Wheel_Active = true;

			if (App->CL_Editor_Map->flag_Left_Button_Down == false)
			{
				int zDelta = static_cast<short>(HIWORD(wParam)); // wheel rotation

				if (zDelta > 0)
				{
					App->CL_Editor_Map->Current_View->ZoomFactor = App->CL_Editor_Map->Current_View->ZoomFactor + 0.1;
					App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
				}

				if (zDelta < 0)
				{
					App->CL_Editor_Map->Current_View->ZoomFactor = App->CL_Editor_Map->Current_View->ZoomFactor - 0.1;
					App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
				}
			}

			return 1;
		}
		
		App->CL_Editor_Map->flag_Wheel_Active = false;
		int zDelta = (short)HIWORD(wParam);    // wheel rotation

		/*if (zDelta > 0)
		{
			App->CL_3D_TL_View->TL_RenderListener->Wheel_Move = -1;
		}
		else if (zDelta < 0)
		{
			App->CL_3D_TL_View->TL_RenderListener->Wheel_Move = 1;
		}*/

		return 1;
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			GetCursorPos(&App->CL_Editor_Map->mStartPoint);
			ScreenToClient(hDlg, &App->CL_Editor_Map->mStartPoint);

			App->CL_Editor_Map->flag_Right_Button_Down = 1;
			App->CL_Editor_Map->flag_Left_Button_Down = 0;

			App->CUR = SetCursor(NULL);
		}

		return 1;
	}

	// Right Mouse Up
	case WM_RBUTTONUP:
	{
		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_Editor_Map->flag_Right_Button_Down = 0;
			App->CL_Editor_Map->flag_Left_Button_Down = 0;

			App->CUR = SetCursor(App->CUR);
		}
		else
		{
			App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
			App->CL_Editor_Map->Context_Menu(hDlg);
		}

		return 1;
	}

	/*case WM_PAINT:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
		App->CL_Editor_Map->Draw_Screen(hDlg);

		return 0;
	}*/

	case WM_COMMAND:
	{
		break;
	}

	case WM_PAINT:
	{
		if (App->flag_3D_Started == true)
		{
			App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
			App->CL_Editor_Map->Draw_Screen(hDlg);
		}

		return 0;
	}

	}

	return FALSE;
}



