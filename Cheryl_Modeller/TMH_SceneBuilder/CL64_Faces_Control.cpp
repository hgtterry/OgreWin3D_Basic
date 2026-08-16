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
#include "CL64_Faces_Control.h"

CL64_Faces_Control::CL64_Faces_Control(void)
{
}

CL64_Faces_Control::~CL64_Faces_Control(void)
{
}

// *************************************************************************
// *		Start_Faces_Control_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Faces_Control::Start_Faces_Control_Dlg()
{
	App->CL_Top_Tabs->TopTabs_Faces_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOP_TABS_FACES, App->MainHwnd, (DLGPROC)Proc_Top_Tabs_Faces);
	App->CL_Interface->Show_TopTabs_Faces_Panel(false);
}

// **************************************************************************
// *		Proc_Top_Tabs_Faces:- Terry and Hazel Flanigan 2026			*
// **************************************************************************
LRESULT CALLBACK CL64_Faces_Control::Proc_Top_Tabs_Faces(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACES_ALL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACE_NEXT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACE_PREV, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_FACE_SHOWSELECTEDFACE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_TT_CB_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		/*SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);

		COLORREF transparentColor = RGB(213, 222, 242);
		SetLayeredWindowAttributes(hDlg, transparentColor, 0, LWA_COLORKEY);*/

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	//case WM_NCHITTEST:
	//{
	//	if (DefWindowProc(hDlg, message, wParam, lParam) == HTCLIENT)
	//	{
	//		return HTCAPTION;
	//	}
	//	//return 0;
	//	return DefWindowProc(hDlg, message, wParam, lParam);
	//}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_FACE_SHOWSELECTEDFACE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_FACE_SHOWSELECTEDFACE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				if (App->flag_3D_Started == true)
				{
					App->Custom_Button_Toggle_Tabs(item, App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face);
				}
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_FACES_ALL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACES_ALL));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_All_Faces);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_FACE_NEXT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACE_NEXT));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				//App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Move);
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_FACE_PREV)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACE_PREV));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				//App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Rotate);
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_FACE_SHOWSELECTEDFACE)
		{
			if (App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face == true)
			{
				App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = false;
			}
			else
			{
				App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = true;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACES_ALL)
		{
			App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
			App->CL_Top_Tabs->flag_All_Faces = 1;

			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

			App->CL_Doc->SelectAllFacesInBrushes();
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

			App->CL_Properties_Textures->Enable_FaceProps_Button(true);
			App->CL_Properties_Tabs->Select_Textures_Tab();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACE_NEXT)
		{
			App->CL_X_Face->Selected_Face_Index++;

			if (App->CL_X_Face->Selected_Face_Index == App->CL_Brush_X->Face_Count)
			{
				App->CL_X_Face->Selected_Face_Index = 0;
			}

			App->CL_Top_Tabs->Select_Face();

			if (App->CL_X_Face_Editor->flag_FaceDlg_Active == 1)
			{
				App->CL_X_Face_Editor->Change_Selection();
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACE_PREV)
		{
			App->CL_X_Face->Selected_Face_Index--;

			if (App->CL_X_Face->Selected_Face_Index < 0)
			{
				App->CL_X_Face->Selected_Face_Index = App->CL_Brush_X->Face_Count - 1;
			}

			App->CL_Top_Tabs->Select_Face();

			if (App->CL_X_Face_Editor->flag_FaceDlg_Active == 1)
			{
				App->CL_X_Face_Editor->Change_Selection();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TT_CB_FACES)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				HWND temp = GetDlgItem(hDlg, IDC_TT_CB_FACES);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);

				if (Index == -1)
				{
				}
				else
				{
					App->CL_X_Face->Selected_Face_Index = Index;
					App->CL_Top_Tabs->Select_Face();

					if (App->CL_X_Face_Editor->flag_FaceDlg_Active == 1)
					{
						App->CL_X_Face_Editor->Change_Selection();
					}
				}
			}
			}

			return true;
		}

		break;
	}

	}
	return FALSE;
}
