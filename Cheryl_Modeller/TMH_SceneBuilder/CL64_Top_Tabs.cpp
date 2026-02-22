/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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

	flag_Brush_Select = true;

	flag_Brush_Move = false;
	flag_Brush_Rotate = false;

	flag_Brush_Scale = false;

	flag_All_Faces = false;
	flag_Next_Face = false;
	flag_Prev_Face = false;

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
// *			Reset_Class:- Terry Mo and Hazel 2025				 	   *
// *************************************************************************
void CL64_Top_Tabs::Reset_Class()
{
	Enable_TopBar_Brush_Buttons(false, false);
	Enable_TopBar_Face_Buttons(false);

	App->CL_Top_Tabs->flag_Full_View_3D = 0;
	App->CL_Top_Tabs->flag_View_Top_Left = 0;
	App->CL_Top_Tabs->flag_View_Top_Right = 0;
	App->CL_Top_Tabs->flag_View_Bottom_Left = 0;

	App->CL_Interface->Deselect_All_Brushes_Update_Dlgs();

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
}

// *************************************************************************
// *				Reset_Bmps:- Terry Mo and Hazel 2026				   *
// *************************************************************************
void CL64_Top_Tabs::Reset_Bmps(void)
{
	HWND Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBSHOWGRID);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOn_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBSHOWHAIR);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOn_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWNORMALS);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBSHOWFACES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWPOINTS);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWBONES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBBOUNDBOX);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWTEXTURES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOff_Bmp);
}

// *************************************************************************
// *				Set_Texture_Bmp_On:- Terry Mo and Hazel 2026		   *
// *************************************************************************
void CL64_Top_Tabs::Set_Texture_Bmp_On(void)
{
	HWND Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWTEXTURES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);
}


// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Top_Tabs::Init_Bmps_Globals(void)
{
	HWND Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_HELP);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_Help_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBSHOWGRID);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOn_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBSHOWHAIR);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOn_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWTEXTURES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWNORMALS);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBSHOWFACES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWPOINTS);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWBONES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBBOUNDBOX);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);

	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOFADE, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);
	SendMessage(hTooltip_TB_2, TTM_SETMAXTIPWIDTH, 0, 250);

	SendMessage(hTooltip_TB_2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_HELP);
	TOOLINFO ti1 = { 0 };
	ti1.cbSize = sizeof(ti1);
	ti1.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti1.uId = (UINT_PTR)Temp;
	ti1.lpszText = (LPSTR)"Help / Information.";
	ti1.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti1);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_HD_SCENEEDITOR);
	TOOLINFO ti2 = { 0 };
	ti2.cbSize = sizeof(ti2);
	ti2.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti2.uId = (UINT_PTR)Temp;
	ti2.lpszText = (LPSTR)"Add Entities to the Scene.";
	ti2.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti2);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_HD_PREVIEW);
	TOOLINFO ti3 = { 0 };
	ti3.cbSize = sizeof(ti3);
	ti3.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti3.uId = (UINT_PTR)Temp;
	ti3.lpszText = (LPSTR)"Preview Scene as it is so far.";
	ti3.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti3);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBSHOWHAIR);
	TOOLINFO ti4 = { 0 };
	ti4.cbSize = sizeof(ti4);
	ti4.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti4.uId = (UINT_PTR)Temp;
	ti4.lpszText = (LPSTR)"3D View Toggle Main Cross Hair.";
	ti4.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti4);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBSHOWFACES);
	TOOLINFO ti5 = { 0 };
	ti5.cbSize = sizeof(ti5);
	ti5.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti5.uId = (UINT_PTR)Temp;
	ti5.lpszText = (LPSTR)"Toggle Mesh/Faces.";
	ti5.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti5);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_TBBOUNDBOX);
	TOOLINFO ti6 = { 0 };
	ti6.cbSize = sizeof(ti6);
	ti6.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti6.uId = (UINT_PTR)Temp;
	ti6.lpszText = (LPSTR)"Toggle Bounding Box.";
	ti6.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti6);

	Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_BTSHOWBONES);
	TOOLINFO ti7 = { 0 };
	ti7.cbSize = sizeof(ti7);
	ti7.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti7.uId = (UINT_PTR)Temp;
	ti7.lpszText = (LPSTR)"Toggle Bones/Skeleton.";
	ti7.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti7);

	SendMessage(hTooltip_TB_2, TTM_SETTITLE, (WPARAM)TTI_INFO, (LPARAM)"Information");
}

// *************************************************************************
// *	  		Start_Top_Tabs:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Top_Tabs::Start_Top_Tabs()
{
	TopTabs_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOP_TABS_HEADERS, App->MainHwnd, (DLGPROC)Proc_Top_Tabs);
	Init_Bmps_Globals();
	Update_Faces_Combo();

	flag_TopTabs_Active = 1;
}

// *************************************************************************
// *        	Proc_Top_Tabs:- Terry Mo and Hazel 2025					   *
// *************************************************************************
LRESULT CALLBACK CL64_Top_Tabs::Proc_Top_Tabs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
		
		SendDlgItemMessage(hDlg, IDC_BT_3DVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MAPVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_HD_SCENEEDITOR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_HD_PREVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
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

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
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

		if (some_item->idFrom == IDC_BT_HD_SCENEEDITOR)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_HD_SCENEEDITOR));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);// , App->CL_Top_Tabs->flag_View_Bottom_Left);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBSHOWHAIR)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBSHOWGRID)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTSHOWTEXTURES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BTSHOWNORMALS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}
	
		if (some_item->idFrom == IDC_TBSHOWFACES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTSHOWBONES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBBOUNDBOX)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_3DVIEW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_Editor_Control->flag_Mode_3DEditor_View);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_MAPVIEW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_Editor_Control->flag_Mode_Map_View);

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_HD_PREVIEW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_HD_PREVIEW));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);// , App->CL_Top_Tabs->flag_View_Bottom_Left);
			}

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

		if (some_item->idFrom == IDC_BT_HELP)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PREVFACE));
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		// 3D
		if (LOWORD(wParam) == IDC_BT_FULL_3D)
		{
			/*if (App->CL_Top_Tabs->flag_Full_View_3D == 1)
			{
				App->CL_Top_Tabs->flag_Full_View_3D = 0;
				App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
			}
			else
			{
				App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_3D);
				App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_3D);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
			}*/
			
			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
			
			return TRUE;
		}

		// Top Left
		if (LOWORD(wParam) == IDC_BT_TOP_LEFT)
		{
			if (App->CL_Top_Tabs->flag_View_Top_Left == 1)
			{
				App->CL_Top_Tabs->flag_View_Top_Left = 0;
				App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

				App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
				App->CL_Editor_Map->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_TL);
				App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_TL);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
				App->CL_Editor_Map->Set_View();
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
				App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

				App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TR];
				App->CL_Editor_Map->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_TR);
				App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_TR);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
				App->CL_Editor_Map->Set_View();
			}

			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOTTOM_LEFT)
		{
			if (App->CL_Top_Tabs->flag_View_Bottom_Left == 1)
			{
				App->CL_Top_Tabs->flag_View_Bottom_Left = 0;
				App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

				App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_BL];
				App->CL_Editor_Map->Set_View();
			}
			else
			{
				App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_BL);
				App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_BL);
				App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
				App->CL_Editor_Map->Set_View();
			}

			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

			return TRUE;
		}

		//------------------------------------------------

		if (LOWORD(wParam) == IDC_TBSHOWHAIR)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_TBSHOWHAIR);

			if (App->CL_Gizmos->flag_Show_Hair == true)
			{
				App->CL_Gizmos->Hair_SetVisible(false);

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOff_Bmp);
			}
			else
			{
				App->CL_Gizmos->Hair_SetVisible(true);

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOn_Bmp);
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_TBSHOWGRID)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_TBSHOWGRID);

			if (App->CL_Gizmos->flag_ShowGrid == true)
			{
				App->CL_Gizmos->Grid_SetVisible(false);

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOff_Bmp);
			}
			else
			{
				App->CL_Gizmos->Grid_SetVisible(true);

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOn_Bmp);
			}

			return TRUE;
		}

		//-------------------------------------------------------- Show Textures
		if (LOWORD(wParam) == IDC_BTSHOWTEXTURES)
		{
			App->CL_Mesh->Show_Mesh_Textures();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TBSHOWFACES)
		{
			App->CL_Mesh->Show_Mesh_Faces();
			return TRUE;
		}

		//-------------------------------------------------------- Show Normals
		if (LOWORD(wParam) == IDC_BTSHOWNORMALS)
		{
			App->CL_Mesh->Show_Mesh_Normals();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TBSHOWFACES)
		{
			App->CL_Mesh->Show_Mesh_Faces();
			return TRUE;
		}

		//-------------------------------------------------------- Show Points
		if (LOWORD(wParam) == IDC_BTSHOWPOINTS)
		{
			App->CL_Mesh->Show_Mesh_Points();
			return TRUE;
		}

		//-------------------------------------------------------- Show Bounding Box
		if (LOWORD(wParam) == IDC_TBBOUNDBOX)
		{
			App->CL_Mesh->Show_Mesh_BoundBox();
			return TRUE;
		}

		//-------------------------------------------------------- Show Bones
		if (LOWORD(wParam) == IDC_BTSHOWBONES)
		{
			App->CL_Mesh->Show_Mesh_Bones();
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_IMPORT)
		{
			App->CL_Importers->Load_Ogre_Model(true);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_3DVIEW)
		{
			App->CL_Editor_Control->Set_3DEditor_View();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MAPVIEW)
		{
			App->CL_Editor_Control->Set_Map_View();
			return TRUE;
		}

		// ---------------------------------------------------------------------


		if (LOWORD(wParam) == IDC_BT_HD_SCENEEDITOR)
		{
			App->CL_Editor_Control->Start_Editor_Scene();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HD_PREVIEW)
		{
			
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_BRUSH_SELECT)
		{
			App->CL_Interface->Deselect_All_Brushes_Update_Dlgs();

			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

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

		if (LOWORD(wParam) == IDC_BT_ALLFACES)
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

		// ----- Next Face
		if (LOWORD(wParam) == IDC_BT_NEXTFACE)
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

		if (LOWORD(wParam) == IDC_BT_PREVFACE)
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
		
		if (LOWORD(wParam) == IDC_BT_HELP)
		{
			App->Open_HTML((LPSTR)"Help\\Top_Bar_Map.html");
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
// *			Set_View_Buttons:- Terry Mo and Hazel 2025				   *
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
// *			Redraw_TopTabs_Dlg:- Terry Mo and Hazel 2025			   *
// *************************************************************************
void CL64_Top_Tabs::Redraw_TopTabs_Dlg()
{
	RedrawWindow(TopTabs_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Set_Brush_Mode:- Terry Mo and Hazel 2025				   *
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

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

	App->CL_Doc->mCurrentTool = CURTOOL_NONE;
	//App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_SCALEBRUSH;
	App->CL_Doc->mModeTool = Mode;
}

//// *************************************************************************
//// *			Set_Brush_Move:- Terry Mo and Hazel 2025				   *
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
//// *			Set_Brush_Scale:- Terry Mo and Hazel 2025				   *
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
// *		Enable_TopBar_Brush_Buttons:- Terry Mo and Hazel 2025   	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_TopBar_Brush_Buttons(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_BRUSH_MOVE), Enable);
	flag_Brush_Move = Active;

	if (App->CL_Doc->CurBrush->GroupId == Enums::Brushs_ID_Evirons)
	{
		EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_BRUSH_SCALE), false);
		flag_Brush_Scale = false;
	}
	else
	{
		EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_BRUSH_SCALE), Enable);
		flag_Brush_Scale = Active;
	}


	EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_BRUSH_ROTATE), Enable);
	flag_Brush_Scale = Active;
}

// *************************************************************************
// *		Enable_TopBar_Face_Buttons:- Terry Mo and Hazel 2025		   *
// *************************************************************************
void CL64_Top_Tabs::Enable_TopBar_Face_Buttons(bool Enable)
{
	EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_ALLFACES), Enable);
	EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_NEXTFACE), Enable);
	EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_PREVFACE), Enable);
	EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_CB_FACELIST), Enable);
	
}

// *************************************************************************
// *		Enable_Select_Button:- Terry Mo and Hazel 2025			  	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_Select_Button(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_BRUSH_SELECT), Enable);
	flag_Brush_Select = Active;
}

// *************************************************************************
// *		Enable_Move_Button:- Terry Mo and Hazel 2025   			 	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_Move_Button(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(TopTabs_Dlg_hWnd, IDC_BT_BRUSH_MOVE), Enable);
	flag_Brush_Move = Active;
}

// *************************************************************************
// *	  	Reset_Brush_Buttons:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Top_Tabs::Reset_Brush_Buttons()
{
	flag_Brush_Select = 0;
	flag_Brush_Move = 0;
	flag_Brush_Rotate = 0;
	flag_Brush_Scale = 0;

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
}

// *************************************************************************
// *	Deselect_Faces_Dlg_Buttons:- Terry Mo and Hazel 2025			   *
// *************************************************************************
void CL64_Top_Tabs::Deselect_Faces_Dlg_Buttons()
{
	flag_All_Faces = 0;
	flag_Next_Face = 0;
	flag_Prev_Face = 0;

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
}


// *************************************************************************
// *			Select_Face:- Terry Mo and Hazel 2025					   *
// *************************************************************************
void CL64_Top_Tabs::Select_Face()
{
	if (App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces) == 0)
	{
		App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
		App->CL_Top_Tabs->flag_Next_Face = 1;
		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

		App->CL_Doc->SelectAllFacesInBrushes();
		App->CL_X_Face->Select_Face_From_Index(App->CL_X_Face->Selected_Face_Index);
	}
	else
	{
		App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
		App->CL_Top_Tabs->flag_Next_Face = 1;
		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

		App->CL_X_Face->Select_Face_From_Index(App->CL_X_Face->Selected_Face_Index);
	}

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	App->CL_Properties_Textures->Enable_FaceProps_Button(true);
	App->CL_Properties_Tabs->Select_Textures_Tab();

	HWND Temp = GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_CB_FACELIST);
	SendMessage(Temp, CB_SETCURSEL, App->CL_X_Face->Selected_Face_Index, 0);
}

// *************************************************************************
// *		Update_Faces_Combo:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Top_Tabs::Update_Faces_Combo()
{
	//Do_Timer

	HWND Temp = GetDlgItem(TopTabs_Dlg_hWnd, IDC_CB_FACELIST);
	SendMessage(Temp, CB_RESETCONTENT, 0, 0);
	char buff[MAX_PATH];

	int SB = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
	
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

		SendMessage(Temp, CB_SETCURSEL, App->CL_X_Face->Selected_Face_Index, 0);
	}

	//Get_Timer
}

// **************************************************************************
// *				Show_TopTabs:- Terry Mo and Hazel 2025					*
// **************************************************************************
void CL64_Top_Tabs::Show_TopTabs(bool Enable)
{
	if (Enable == 1)
	{
		flag_TopTabs_Active = 1;
		ShowWindow(TopTabs_Dlg_hWnd, 1);
	}
	else
	{
		flag_TopTabs_Active = 0;
		ShowWindow(TopTabs_Dlg_hWnd, 0);
	}
}
