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
	TopTabs_Dlg_hWnd = nullptr;

	flag_Full_View_3D = false;
	flag_View_Top_Left = false;
	flag_View_Top_Right = false;
	flag_View_Bottom_Left = false;

	flag_TopTabs_Active = false;
}

CL64_Top_Tabs::~CL64_Top_Tabs(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Top_Tabs::Reset_Class()
{
	App->CL_Top_Tabs->flag_Full_View_3D = 0;
	App->CL_Top_Tabs->flag_View_Top_Left = 0;
	App->CL_Top_Tabs->flag_View_Top_Right = 0;
	App->CL_Top_Tabs->flag_View_Bottom_Left = 0;

	App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
}

// *************************************************************************
// *	  		Start_Top_Tabs():- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Top_Tabs::Start_Top_Tabs()
{
	TopTabs_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOP_TABS_HEADERS, App->MainHwnd, (DLGPROC)Proc_Top_Tabs);
	App->CL_Faces_Control->Update_Faces_Dialog();

	flag_TopTabs_Active = true;

	App->CL_Faces_Control->Start_Faces_Control_Dlg();
}

// *************************************************************************
// *        	Proc_Top_Tabs:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
LRESULT CALLBACK CL64_Top_Tabs::Proc_Top_Tabs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_FULL_3D, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TOP_LEFT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TOP_RIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BOTTOM_LEFT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_HD_SCENEEDITOR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_HD_PREVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_TT_FILEVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_OBJ_DATA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;
		LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

		switch (some_item->idFrom)
		{
		case IDC_BT_FULL_3D:
		{
			App->Custom_Button_Toggle(item, App->CL_Top_Tabs->flag_Full_View_3D);
			break;
		}

		case IDC_BT_TOP_LEFT:
		{
			App->Custom_Button_Toggle(item, App->CL_Top_Tabs->flag_View_Top_Left);
			break;
		}

		case IDC_BT_TOP_RIGHT:
		{
			App->Custom_Button_Toggle(item, App->CL_Top_Tabs->flag_View_Top_Right);
			break;
		}

		case IDC_BT_BOTTOM_LEFT:
		{
			App->Custom_Button_Toggle(item, App->CL_Top_Tabs->flag_View_Bottom_Left);
			break;
		}

		case IDC_BT_HD_SCENEEDITOR:
		{
			if (IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_HD_SCENEEDITOR)) == false)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);// , App->CL_Top_Tabs->flag_View_Bottom_Left);
			}
			break;
		}

		case IDC_BT_HD_PREVIEW:
		{
			if (IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_HD_PREVIEW)) == false)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);// , App->CL_Top_Tabs->flag_View_Bottom_Left);
			}
			break;
		}
		
		case IDC_BT_TT_FILEVIEW:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FILEVIEW));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_Interface->flag_FileView_Active);
			}
			break;
		}

		case IDC_BT_TT_OBJ_DATA:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_OBJ_DATA));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_Interface->flag_Properties_Object_Dlg_Active);
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
		// 3D
		if (LOWORD(wParam) == IDC_BT_FULL_3D)
		{
			if (App->CL_Top_Tabs->flag_Full_View_3D == 1)
			{
				App->CL_Top_Tabs->flag_Full_View_3D = 0;
				App->CL_Views_Com->Set_Splitter_WidthDepth(App->CL_Views_Com->Copy_Spliter_Width, App->CL_Views_Com->Copy_Spliter_Depth);
				App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);
			}
			else
			{
				App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_3D);
				App->CL_Views_Com->Init_Views(Enums::Selected_Map_View_3D);
				App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);
			}
			
			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
			
			return TRUE;
		}

		// Top Left
		if (LOWORD(wParam) == IDC_BT_TOP_LEFT)
		{
			if (App->CL_Top_Tabs->flag_View_Top_Left == 1)
			{
				App->CL_Top_Tabs->flag_View_Top_Left = 0;
				App->CL_Views_Com->Set_Splitter_WidthDepth(App->CL_Views_Com->Copy_Spliter_Width, App->CL_Views_Com->Copy_Spliter_Depth);
				App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);

				App->CL_Views_Com->Current_View = App->CL_View_Top_Left->VCam_TL;
				App->CL_Views_Com->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_TL);
				App->CL_Views_Com->Init_Views(Enums::Selected_Map_View_TL);
				App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);
				App->CL_Views_Com->Set_View();
			}

			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
			
			return TRUE;
		}
		
		// Top Right
		if (LOWORD(wParam) == IDC_BT_TOP_RIGHT)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_BT_TOP_RIGHT);

			if (App->CL_Top_Tabs->flag_View_Top_Right == 1)
			{
				App->CL_Top_Tabs->flag_View_Top_Right = 0;
				App->CL_Views_Com->Set_Splitter_WidthDepth(App->CL_Views_Com->Copy_Spliter_Width, App->CL_Views_Com->Copy_Spliter_Depth);
				App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);

				App->CL_Views_Com->Current_View = App->CL_View_Top_Right->VCam_TR;
				App->CL_Views_Com->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_TR);
				App->CL_Views_Com->Init_Views(Enums::Selected_Map_View_TR);
				App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);
				App->CL_Views_Com->Set_View();
			}

			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOTTOM_LEFT)
		{
			if (App->CL_Top_Tabs->flag_View_Bottom_Left == 1)
			{
				App->CL_Top_Tabs->flag_View_Bottom_Left = 0;
				App->CL_Views_Com->Set_Splitter_WidthDepth(App->CL_Views_Com->Copy_Spliter_Width, App->CL_Views_Com->Copy_Spliter_Depth);
				App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);

				App->CL_Views_Com->Current_View = App->CL_View_Bottom_Left->VCam_BL;
				App->CL_Views_Com->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_BL);
				App->CL_Views_Com->Init_Views(Enums::Selected_Map_View_BL);
				App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);
				App->CL_Views_Com->Set_View();
			}

			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HD_SCENEEDITOR)
		{
			App->CL_Editor_Control->Start_Editor_Scene();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HD_PREVIEW)
		{
			App->CL_Editor_Control->Start_Preview_Mode();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FILEVIEW)
		{
			if (App->CL_Interface->flag_FileView_Active == true)
			{
				App->CL_Interface->Show_FileView(false);
			}
			else
			{
				App->CL_Interface->Show_FileView(true);
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_TT_OBJ_DATA)
		{
			if (App->CL_Interface->flag_Properties_Object_Dlg_Active == true)
			{
				App->CL_Interface->Show_Properties_Object_Dlg(false);
			}
			else
			{
				App->CL_Interface->Show_Properties_Object_Dlg(true);
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
// *		Set_View_Buttons:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Top_Tabs::Set_View_Buttons(int Selected_View)
{
	// Reset all view flags to false
	flag_View_Top_Left = false;
	flag_View_Top_Right = false;
	flag_View_Bottom_Left = false;
	flag_Full_View_3D = false;

	// Set the appropriate flag based on the selected view
	switch (Selected_View)
	{
	case Enums::Selected_Map_View_TL:
		flag_View_Top_Left = true;
		break;
	case Enums::Selected_Map_View_TR:
		flag_View_Top_Right = true;
		break;
	case Enums::Selected_Map_View_BL:
		flag_View_Bottom_Left = true;
		break;
	case Enums::Selected_Map_View_3D:
		flag_Full_View_3D = true;
		break;
	default:
		// Handle unexpected view selection if necessary
		break;
	}

	// Redraw the window to reflect the changes
	Redraw_TopTabs_Dlg();
}

// *************************************************************************
// *	  	Redraw_TopTabs_Dlg:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Top_Tabs::Redraw_TopTabs_Dlg()
{
	RedrawWindow(TopTabs_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

//// *************************************************************************
//// *			Set_Brush_Move:- Terry and Hazel Flanigan 2024			   *
//// *************************************************************************
//void CL64_Top_Tabs::Set_Brush_Move(void)
//{
//	SetCursor(App->CL_Views_Com->hcBoth);
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
//	SetCursor(App->CL_Views_Com->hcBoth);
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
	HWND Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_TOP_RIGHT);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TR_Off_Bmp);
}

