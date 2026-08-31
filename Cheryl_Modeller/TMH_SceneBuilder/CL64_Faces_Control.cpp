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
	Selected_Face_Index = 0;

	flag_No_Faces = true;
	flag_All_Faces = false;
	flag_Next_Face = false;
	flag_Prev_Face = false;

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
// *		Proc_Top_Tabs_Faces:- Terry and Hazel Flanigan 2026			*
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
		
		SendDlgItemMessage(hDlg, IDC_BT_FACES_NONE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACES_ALL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACE_NEXT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACE_PREV, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_FACE_FACEEDITOR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_FACE_SHOWSELECTEDFACE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_TT_CB_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
		/*COLORREF transparentColor = RGB(213, 222, 242);
		SetLayeredWindowAttributes(hDlg, transparentColor, 255, LWA_COLORKEY);*/

		//p_Faces->Unselect_All_Face();

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
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Select);
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
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Move);
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
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Rotate);
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
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Scale);
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
			
		
		case IDC_BT_FACES_NONE:
		{
			App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_No_Faces);
			break;
		}

		case IDC_BT_TT_FACES_ALL:
		{
			App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_All_Faces);
			break;
		}

		case IDC_BT_TT_FACE_NEXT:
		{
			App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_Next_Face);
			break;
		}

		case IDC_BT_TT_FACE_PREV:
		{
			App->Custom_Button_Toggle_Tabs(item, p_Faces->flag_Prev_Face);
			break;
		}

		case IDC_BT_FACE_SHOWSELECTEDFACE:
		{
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

			break;
		}

		case IDC_BT_FACE_FACEEDITOR:
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
			//App->CL_Interface->Show_Faces_Panel_Control(false);
			//App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_TT_MOVE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 1);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_SCALE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_SCALEBRUSH, 2);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_ROTATE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 3);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_FACE_FACEEDITOR)
		{
			int SF = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
			if (SF > 0)
			{
				App->CL_X_Face_Editor->Start_FaceDialog();
			}
			else
			{
				App->Say("No Face Selected");
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_FACES_NONE)
		{
			p_Faces->Unselect_All_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACES_ALL)
		{
			p_Faces->Select_All_Face();
			return TRUE;
		}

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

		if (LOWORD(wParam) == IDC_BT_TT_FACE_NEXT)
		{
			p_Faces->Select_Next_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACE_PREV)
		{
			p_Faces->Select_Prev_Face();
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
					p_Faces->Selected_Face_Index = Index;
					p_Faces->Select_Face();

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


// *************************************************************************
// *		Update_Faces_Dialog:- Terry and Hazel Flanigan 2026			  
// *************************************************************************
void CL64_Faces_Control::Update_Faces_Dialog()
{
	//Do_Timer

	HWND Temp = GetDlgItem(Faces_Control_Dlg_hWnd, IDC_TT_CB_FACES);
	SendMessage(Temp, CB_RESETCONTENT, 0, 0);
	char buff[MAX_PATH];

	int SB = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

	if (SB > 0)
	{
		int Count = 0;
		int Face_Count = App->CL_Brush_X->Get_Brush_All_Faces_Count();

		while (Count < Face_Count)
		{
			sprintf(buff, "%s %i", "Face:-", Count + 1);
			SendMessage(Temp, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buff);
			Count++;
		}

		SendMessage(Temp, CB_SETCURSEL, Selected_Face_Index, 0);

		Brush* pBrush;
		pBrush = App->CL_Doc->CurBrush;

		char Brush_Name[MAX_PATH];
		strcpy(Brush_Name, "Brush:-  ");
		strcat(Brush_Name, pBrush->Name);
		SetDlgItemText(Faces_Control_Dlg_hWnd, IDC_ST_BRUSHNAME, (LPCTSTR)Brush_Name);

		char sFace_Count[MAX_PATH];
		sprintf(sFace_Count, "%s %i", "Face Count:-", Face_Count);
		SetDlgItemText(Faces_Control_Dlg_hWnd, IDC_ST_FACEAMOUNT, (LPCTSTR)sFace_Count);
		
		
	}

	//Get_Timer
}

// *************************************************************************
// *			Unselect_All_Face:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Faces_Control::Unselect_All_Face()
{
	App->CL_Doc->ResetAllSelectedFaces();
	App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = false;
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	Reset_Flags();
	flag_No_Faces = true;
	

	RedrawWindow(Faces_Control_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Select_All_Face:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Faces_Control::Select_All_Face()
{
	Reset_Flags();
	flag_All_Faces = true;
	
	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

	App->CL_Doc->SelectAllFacesInBrushes();
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	App->CL_Properties_Textures->Enable_FaceProps_Button(true);
	App->CL_Properties_Tabs->Select_Textures_Tab();

	App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = true;

	RedrawWindow(Faces_Control_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Select_Next_Face:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Faces_Control::Select_Next_Face()
{
	Reset_Flags();
	flag_Next_Face = true;
	
	Selected_Face_Index++;

	if (Selected_Face_Index == App->CL_Brush_X->Face_Count)
	{
		Selected_Face_Index = 0;
	}

	Select_Face();

	if (App->CL_X_Face_Editor->flag_FaceDlg_Active == 1)
	{
		App->CL_X_Face_Editor->Change_Selection();
	}

	App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = true;

	RedrawWindow(Faces_Control_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Select_Prev_Face:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Faces_Control::Select_Prev_Face()
{
	Reset_Flags();
	flag_Prev_Face = true;
	
	Selected_Face_Index--;

	if (Selected_Face_Index < 0)
	{
		Selected_Face_Index = App->CL_Brush_X->Face_Count - 1;
	}

	App->CL_Faces_Control->Select_Face();

	if (App->CL_X_Face_Editor->flag_FaceDlg_Active == 1)
	{
		App->CL_X_Face_Editor->Change_Selection();
	}

	App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = true;

	RedrawWindow(Faces_Control_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Select_Face:- Terry and Hazel Flanigan 2026				   *
// *************************************************************************
void CL64_Faces_Control::Select_Face()
{
	if (App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces) == 0)
	{
		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

		App->CL_Doc->SelectAllFacesInBrushes();
		App->CL_X_Face->Select_Face_From_Index(Selected_Face_Index);
	}
	else
	{
		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

		App->CL_X_Face->Select_Face_From_Index(Selected_Face_Index);
	}

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	App->CL_Properties_Textures->Enable_FaceProps_Button(true);
	App->CL_Properties_Tabs->Select_Textures_Tab();

	HWND Temp = GetDlgItem(Faces_Control_Dlg_hWnd, IDC_TT_CB_FACES);
	SendMessage(Temp, CB_SETCURSEL, Selected_Face_Index, 0);
}

// *************************************************************************
// *				Reset_Flags:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Faces_Control::Reset_Flags()
{
	flag_No_Faces = false;
	flag_All_Faces = false;
	flag_Next_Face = false;
	flag_Prev_Face = false;

	RedrawWindow(Faces_Control_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
