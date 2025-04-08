/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_Top_Tabs.h"

CL64_Top_Tabs::CL64_Top_Tabs(void)
{
	Headers_hWnd = nullptr;

	Copy_Spliter_Width = 500;
	Copy_Spliter_Depth = 215;
	
	flag_Brush_Select = 1;

	flag_Brush_Move = 0;
	flag_Brush_Rotate = 0;

	flag_Brush_Scale = 0;

	flag_All_Faces = 0;
	flag_Next_Face = 0;
	flag_Prev_Face = 0;

	flag_Full_View_3D = 0;
	flag_View_Top_Left = 0;
	flag_View_Top_Right = 0;
	flag_View_Bottom_Left = 0;

	flag_TopTabs_Active = 0;
}

CL64_Top_Tabs::~CL64_Top_Tabs(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Top_Tabs::Reset_Class()
{
	Enable_Brush_Options_Buttons(false, false);

	App->CL_Top_Tabs->flag_Full_View_3D = 0;
	App->CL_Top_Tabs->flag_View_Top_Left = 0;
	App->CL_Top_Tabs->flag_View_Top_Right = 0;
	App->CL_Top_Tabs->flag_View_Bottom_Left = 0;

	App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();

	RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  			Message:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Top_Tabs::Start_Headers()
{
	Headers_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOP_TABS_HEADERS, App->MainHwnd, (DLGPROC)Proc_Headers);
	Update_Faces_Combo();

	flag_TopTabs_Active = 1;
}

// *************************************************************************
// *        	Message_Proc:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_Top_Tabs::Proc_Headers(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_HEADER_BRUSHES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_SELECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_FULL_3D, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TOP_LEFT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TOP_RIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BOTTOM_LEFT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_MOVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_ROTATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_SCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_SHEAR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_HEADER_FACES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ALLFACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_NEXTFACE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PREVFACE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_FACELIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_HEADER_BRUSHES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_HEADER_FACES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
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

		if (some_item->idFrom == IDC_BT_FULL_3D)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Top_Tabs->flag_Full_View_3D);
			
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TOP_LEFT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Top_Tabs->flag_View_Top_Left);

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TOP_RIGHT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item,App->CL_Top_Tabs->flag_View_Top_Right);

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_BOTTOM_LEFT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Top_Tabs->flag_View_Bottom_Left);

			return CDRF_DODEFAULT;
		}

		
		if (some_item->idFrom == IDC_BT_BRUSH_SELECT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_SELECT));
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

		if (some_item->idFrom == IDC_BT_BRUSH_MOVE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_MOVE));
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

		if (some_item->idFrom == IDC_BT_BRUSH_ROTATE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_ROTATE));
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
		
		if (some_item->idFrom == IDC_BT_BRUSH_SCALE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_SCALE));
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

		if (some_item->idFrom == IDC_BT_BRUSH_SHEAR)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_SHEAR));
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

		if (some_item->idFrom == IDC_BT_ALLFACES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_ALLFACES));
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

		if (some_item->idFrom == IDC_BT_NEXTFACE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_NEXTFACE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Next_Face);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_PREVFACE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PREVFACE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Prev_Face);
			}

			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		// 3D
		if (LOWORD(wParam) == IDC_BT_FULL_3D)
		{
			if (App->CL_Top_Tabs->flag_Full_View_3D == 1)
			{
				App->CL_Top_Tabs->flag_Full_View_3D = 0;

				App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Top_Tabs->Copy_Spliter_Width, App->CL_Top_Tabs->Copy_Spliter_Depth);

				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
			}
			else
			{
				App->CL_Top_Tabs->flag_Full_View_3D = 1;
				App->CL_Top_Tabs->flag_View_Top_Left = 0;
				App->CL_Top_Tabs->flag_View_Top_Right = 0;
				App->CL_Top_Tabs->flag_View_Bottom_Left = 0;

				App->CL_Editor_Map->Init_Views(Enums::Selected_View_3D);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
			}
			
			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			
			return TRUE;
		}

		// Top Left
		if (LOWORD(wParam) == IDC_BT_TOP_LEFT)
		{
			if (App->CL_Top_Tabs->flag_View_Top_Left == 1)
			{
				App->CL_Top_Tabs->flag_View_Top_Left = 0;

				App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Top_Tabs->Copy_Spliter_Width, App->CL_Top_Tabs->Copy_Spliter_Depth);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

				App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
				App->CL_Editor_Map->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->flag_View_Top_Left = 1;
				App->CL_Top_Tabs->flag_Full_View_3D = 0;
				App->CL_Top_Tabs->flag_View_Top_Right = 0;
				App->CL_Top_Tabs->flag_View_Bottom_Left = 0;

				App->CL_Editor_Map->Init_Views(Enums::Selected_View_TL);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
				App->CL_Editor_Map->Set_View();
			}

			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			
			return TRUE;
		}
		
		// Top Right
		if (LOWORD(wParam) == IDC_BT_TOP_RIGHT)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_BT_TOP_RIGHT);

			if (App->CL_Top_Tabs->flag_View_Top_Right == 1)
			{
				App->CL_Top_Tabs->flag_View_Top_Right = 0;
				App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Top_Tabs->Copy_Spliter_Width, App->CL_Top_Tabs->Copy_Spliter_Depth);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

				App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TR];
				App->CL_Editor_Map->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->flag_View_Top_Right = 1;
				App->CL_Top_Tabs->flag_Full_View_3D = 0;
				App->CL_Top_Tabs->flag_View_Top_Left = 0;
				App->CL_Top_Tabs->flag_View_Bottom_Left = 0;

				App->CL_Editor_Map->Init_Views(Enums::Selected_View_TR);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
				App->CL_Editor_Map->Set_View();
			}

			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOTTOM_LEFT)
		{
			if (App->CL_Top_Tabs->flag_View_Bottom_Left == 1)
			{
				App->CL_Top_Tabs->flag_View_Bottom_Left = 0;
				App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Top_Tabs->Copy_Spliter_Width, App->CL_Top_Tabs->Copy_Spliter_Depth);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

				App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_BL];
				App->CL_Editor_Map->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->flag_View_Bottom_Left = 1;
				App->CL_Top_Tabs->flag_Full_View_3D = 0;
				App->CL_Top_Tabs->flag_View_Top_Left = 0;
				App->CL_Top_Tabs->flag_View_Top_Right = 0;

				App->CL_Editor_Map->Init_Views(Enums::Selected_View_BL);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
				App->CL_Editor_Map->Set_View();
			}

			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_SELECT)
		{
			App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();

			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_MOVE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH,1);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_SCALE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_SCALEBRUSH, 2);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_ROTATE)
		{
			App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 3);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CB_FACELIST)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				HWND temp = GetDlgItem(hDlg, IDC_CB_FACELIST);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);

				if (Index == -1)
				{ 
				}
				else
				{
					App->CL_Face->Selected_Face_Index = Index;
					App->CL_Top_Tabs->Select_Face();

					if (App->CL_Properties_Faces->flag_FaceDlg_Active == 1)
					{
						App->CL_Properties_Faces->Change_Selection();
					}
				}
			}
			}

			return true;
		}

		if (LOWORD(wParam) == IDC_BT_ALLFACES)
		{
			App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
			App->CL_Top_Tabs->flag_All_Faces = 1;

			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_Doc->SelectAllFacesInBrushes();
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

			App->CL_Properties_Textures->Enable_FaceProps_Button(true);
			App->CL_Properties_Tabs->Select_Textures_Tab();
		
			return TRUE;
		}

		// ----- Next Face
		if (LOWORD(wParam) == IDC_BT_NEXTFACE)
		{
			App->CL_Face->Selected_Face_Index++;

			if (App->CL_Face->Selected_Face_Index == App->CL_Brush_X->Face_Count)
			{
				App->CL_Face->Selected_Face_Index = 0;
			}

			App->CL_Top_Tabs->Select_Face();

			if (App->CL_Properties_Faces->flag_FaceDlg_Active == 1)
			{
				App->CL_Properties_Faces->Change_Selection();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PREVFACE)
		{
			App->CL_Face->Selected_Face_Index--;

			if (App->CL_Face->Selected_Face_Index < 0)
			{
				App->CL_Face->Selected_Face_Index = App->CL_Brush_X->Face_Count - 1;
			}

			App->CL_Top_Tabs->Select_Face();

			if (App->CL_Properties_Faces->flag_FaceDlg_Active == 1)
			{
				App->CL_Properties_Faces->Change_Selection();
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	}

	return FALSE;
}

// *************************************************************************
// *			Set_Brush_Mode:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Top_Tabs::Set_Brush_Mode(int Mode, int Dlg_Selection)
{
	SetCursor(App->CL_Editor_Map->hcBoth);

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
	
	Deselect_Faces_Dlg_Buttons();

	RedrawWindow(Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	App->CL_Doc->mCurrentTool = CURTOOL_NONE;
	//App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_SCALEBRUSH;
	App->CL_Doc->mModeTool = Mode;
}

//// *************************************************************************
//// *			Set_Brush_Move:- Terry and Hazel Flanigan 2024			   *
//// *************************************************************************
//void CL64_Top_Tabs::Set_Brush_Move(void)
//{
//	SetCursor(App->CL_Editor_Map->hcBoth);
//
//	App->CL_Doc->ResetAllSelectedFaces();;
//	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
//
//	Reset_Brush_Buttons();
//	flag_Brush_Move = 1;
//
//	Deselect_Faces_Dlg_Buttons();
//
//	RedrawWindow(Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
//
//	App->CL_Doc->mCurrentTool = CURTOOL_NONE;
//	App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_MOVEROTATEBRUSH;
//
//}
//
//// *************************************************************************
//// *			Set_Brush_Scale:- Terry and Hazel Flanigan 2024			   *
//// *************************************************************************
//void CL64_Top_Tabs::Set_Brush_Scale(void)
//{
//	SetCursor(App->CL_Editor_Map->hcBoth);
//
//	App->CL_Doc->ResetAllSelectedFaces();;
//	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
//
//	App->CL_Top_Tabs->Reset_Brush_Buttons();
//	App->CL_Top_Tabs->flag_Brush_Scale = 1;
//
//	App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
//
//	RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
//
//	App->CL_Doc->mCurrentTool = CURTOOL_NONE;
//	App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_SCALEBRUSH;
//}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Top_Tabs::Init_Bmps_Globals(void)
{
	HWND Temp = GetDlgItem(Headers_hWnd, IDC_BT_TOP_RIGHT);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TR_Off_Bmp);
}

// *************************************************************************
// *	Enable_Brush_Options_Buttons:- Terry and Hazel Flanigan 2025   	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_Brush_Options_Buttons(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_MOVE), Enable);
	flag_Brush_Move = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_SCALE), Enable);
	flag_Brush_Scale = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_ROTATE), Enable);
	flag_Brush_Scale = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_ALLFACES), Enable);
	//flag_Brush_Scale = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_NEXTFACE), Enable);
	//flag_Brush_Scale = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_PREVFACE), Enable);
	//flag_Brush_Scale = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_CB_FACELIST), Enable);
	//flag_Brush_Scale = Active;
}

// *************************************************************************
// *		Enable_Select_Button:- Terry and Hazel Flanigan 2025   	  	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_Select_Button(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_SELECT), Enable);
	flag_Brush_Select = Active;
}

// *************************************************************************
// *		Enable_Move_Button:- Terry and Hazel Flanigan 2025   	  	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_Move_Button(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_MOVE), Enable);
	flag_Brush_Move = Active;
}

// *************************************************************************
// *	  	Reset_Brush_Buttons:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Top_Tabs::Reset_Brush_Buttons()
{
	flag_Brush_Select = 0;
	flag_Brush_Move = 0;
	flag_Brush_Rotate = 0;
	flag_Brush_Scale = 0;

	RedrawWindow(Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	Deselect_Faces_Dlg_Buttons:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Top_Tabs::Deselect_Faces_Dlg_Buttons()
{
	flag_All_Faces = 0;
	flag_Next_Face = 0;
	flag_Prev_Face = 0;

	RedrawWindow(Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}


// *************************************************************************
// *			Select_Face:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Top_Tabs::Select_Face()
{
	if (App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces) == 0)
	{
		App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
		App->CL_Top_Tabs->flag_Next_Face = 1;
		RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		App->CL_Doc->SelectAllFacesInBrushes();
		App->CL_Face->Select_Face_From_Index(App->CL_Face->Selected_Face_Index);
	}
	else
	{
		App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
		App->CL_Top_Tabs->flag_Next_Face = 1;
		RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		App->CL_Face->Select_Face_From_Index(App->CL_Face->Selected_Face_Index);
	}

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	App->CL_Properties_Textures->Enable_FaceProps_Button(true);
	App->CL_Properties_Tabs->Select_Textures_Tab();

	HWND Temp = GetDlgItem(App->CL_Top_Tabs->Headers_hWnd, IDC_CB_FACELIST);
	SendMessage(Temp, CB_SETCURSEL, App->CL_Face->Selected_Face_Index, 0);
}

// *************************************************************************
// *		Update_Faces_Combo:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Top_Tabs::Update_Faces_Combo()
{
	//App->Flash_Window();

	//Do_Timer

	HWND Temp = GetDlgItem(Headers_hWnd, IDC_CB_FACELIST);
	SendMessage(Temp, CB_RESETCONTENT, 0, 0);
	char buff[MAX_PATH];

	int SB = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
	
	if (SB > 0)
	{
		int Count = 0;
		int Face_Count = App->CL_Brush_X->Get_Brush_All_Faces_Count();

		while (Count < Face_Count)
		{
			sprintf(buff, "%s %i", "Face:-", Count+1);
			SendMessage(Temp, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buff);
			Count++;
		}

		SendMessage(Temp, CB_SETCURSEL, App->CL_Face->Selected_Face_Index, 0);
	}

	//Get_Timer
}

// **************************************************************************
// *				Show_TopTabs:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Top_Tabs::Show_TopTabs(bool Enable)
{
	if (Enable == 1)
	{
		flag_TopTabs_Active = 1;
		ShowWindow(Headers_hWnd, 1);
	}
	else
	{
		flag_TopTabs_Active = 0;
		ShowWindow(Headers_hWnd, 0);
	}
}
