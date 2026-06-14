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
#include "CL64_Properties_Tabs.h"

CL64_Properties_Tabs::CL64_Properties_Tabs()
{
	Tabs_Control_Hwnd = nullptr;
}

CL64_Properties_Tabs::~CL64_Properties_Tabs()
{
}

// *************************************************************************
// *			Reset_Class:- Terry Mo and Hazel 2025				 	   *
// *************************************************************************
void CL64_Properties_Tabs::Reset_Class()
{
	Enable_Tabs(false);
	Select_Templates_Tab();
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Properties_Tabs::Init_Bmps_Globals(void)
{
	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOFADE, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);
	SendMessage(hTooltip_TB_2, TTM_SETMAXTIPWIDTH, 0, 250);

	SendMessage(hTooltip_TB_2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

	HWND Temp = GetDlgItem(Tabs_Control_Hwnd, IDC_TBTEXTURES);
	TOOLINFO ti1 = { 0 };
	ti1.cbSize = sizeof(ti1);
	ti1.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti1.uId = (UINT_PTR)Temp;
	ti1.lpszText = (LPSTR)"Alter Textures on selected shape.";
	ti1.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti1);

	Temp = GetDlgItem(Tabs_Control_Hwnd, IDC_TBTEMPLATES);
	TOOLINFO ti2 = { 0 };
	ti2.cbSize = sizeof(ti2);
	ti2.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti2.uId = (UINT_PTR)Temp;
	ti2.lpszText = (LPSTR)"Add a new geometry shape to the Level.";
	ti2.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti2);

	Temp = GetDlgItem(Tabs_Control_Hwnd, IDC_TBGROUPS);
	TOOLINFO ti3 = { 0 };
	ti3.cbSize = sizeof(ti3);
	ti3.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti3.uId = (UINT_PTR)Temp;
	ti3.lpszText = (LPSTR)"Select a geometry shape in the Level.";
	ti3.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti3);
}

// *************************************************************************
// *	  	Start_Tabs_Control_Dlg:- Terry Mo and Hazel 2025			   *
// *************************************************************************
void CL64_Properties_Tabs::Start_Tabs_Control_Dlg()
{
	Tabs_Control_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TABS, App->MainHwnd, (DLGPROC)Proc_Tabs_Control);
	App->CL_Interface->Show_Properties_Panel(false);

	Init_Bmps_Globals();

	// Start the Brushes dialog
	App->CL_Properties_Brushes->Start_Brush_Tabs_Dialog();
	App->CL_Interface->Show_Brushes_Dialog(false);

	// Textures
	App->CL_Properties_Textures->Start_TextureDialog();
	App->CL_Interface->Show_Textures_Dialog(false);
	
	// Materials
	App->CL_Properties_Materials->Start_Props_Materials_Dlg();
	App->CL_Interface->Show_Materials_Dialog(true);

	App->CL_Interface->Position_Properties_Dlg();
	App->CL_Props_Dialogs->Start_Props_Dialogs();

	// Templates/Shapes
	App->CL_Properties_Templates->Start_TemplatesDialog();
	App->CL_Interface->Show_TemplatesDialog(false);


	App->CL_Properties_Motions->Start_Motions_Dialog();

	App->CL_Interface->Position_Tabs_Dlg();
	App->CL_Interface->Position_Motions_Dlg();
}

// *************************************************************************
// *        Tabs_Control_Proc:- Terry Mo and Hazel 2025					   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Tabs::Proc_Tabs_Control(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_TBTEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TBTEMPLATES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TBGROUPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		//SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
		//SetLayeredWindowAttributes(hDlg, RGB(18, 18, 18), 230, LWA_ALPHA);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
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

		// Check which button was notified and handle accordingly
		switch (some_item->idFrom)
		{
		case IDC_TBTEXTURES:
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_TBTEXTURES));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Interface->flag_Tab_Texture);
			}

			break;
		}
		
		case IDC_TBTEMPLATES:
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_TBTEMPLATES));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Interface->flag_Tab_Templates);
			}

			break;
		}

		case IDC_TBGROUPS:
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_TBGROUPS));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Interface->flag_Tab_Group);
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
		if (LOWORD(wParam) == IDC_TBTEXTURES)
		{
			App->CL_Interface->Select_Tab(Enums::Tab_ID_TEXTURES);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TBTEMPLATES)
		{
			App->CL_Interface->Select_Tab(Enums::Tab_ID_TEMPLATES);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TBGROUPS)
		{
			App->CL_Interface->Select_Tab(Enums::Tab_ID_GROUPS);
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Interface->Show_Properties_Panel(false);
			CheckMenuItem(App->Menu_Map, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);
			//EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Interface->Show_Properties_Panel(false);
			CheckMenuItem(App->Menu_Map, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);
			//EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *	  	Select_Brushes_Tab:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Properties_Tabs::Select_Brushes_Tab()
{
	// Check is valid
	if (Tabs_Control_Hwnd && App->CL_Interface->flag_Properties_Dlg_Active == true)
	{
		// Hide any open dialogs
		App->CL_Interface->Hide_Tab_Dialogs();

		// Show the brushes dialog
		App->CL_Interface->Show_Brushes_Dialog(true);
		App->CL_Interface->flag_Tab_Group = true; 

		// Redraw
		RedrawWindow(Tabs_Control_Hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW); // Redraw the tabs control
	}
}

// *************************************************************************
// *	  	Select_Textures_Tab:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Properties_Tabs::Select_Textures_Tab()
{
	if (Tabs_Control_Hwnd && App->CL_Interface->flag_Properties_Dlg_Active == true)
	{
		if (App->CL_Interface->flag_Tab_Texture == false)
		{
			App->CL_Interface->Hide_Tab_Dialogs();

			if (App->CL_Model->Editor_Setup_Mode == Enums::Editor_Setup_Mode_Create_Model)
			{
				App->CL_Interface->Show_Textures_Dialog(true);
			}
			else
			{
				App->CL_Interface->Show_Materials_Dialog(true);
			}
			App->CL_Interface->flag_Tab_Texture = true;

			RedrawWindow(Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		else
		{
			App->CL_Properties_Textures->Get_Selected_Face();
		}
	}
}

// *************************************************************************
// *	  	Select_Templates_Tab:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Properties_Tabs::Select_Templates_Tab()
{
	if (Tabs_Control_Hwnd && App->CL_Interface->flag_Properties_Dlg_Active == true)
	{
		App->CL_Interface->Hide_Tab_Dialogs();
		App->CL_Interface->Show_TemplatesDialog(true);
		App->CL_Interface->flag_Tab_Templates = 1;

		RedrawWindow(Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

// *************************************************************************
// *	  	Enable_Tabs_Dlg:- Terry Mo and Hazel 2025					   *
// *************************************************************************
void CL64_Properties_Tabs::Enable_Tabs_Dlg(bool Enable)
{
	EnableWindow(Tabs_Control_Hwnd, Enable);
}

// *************************************************************************
// *	  	Enable_Tabs:- Terry Mo and Hazel 2025						   *
// *************************************************************************
void CL64_Properties_Tabs::Enable_Tabs(bool Enable)
{
	EnableWindow(GetDlgItem(Tabs_Control_Hwnd, IDC_TBTEXTURES), Enable);
	EnableWindow(GetDlgItem(Tabs_Control_Hwnd, IDC_TBGROUPS), Enable);
	
	
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_HD_PREVIEW), Enable);
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_HD_SCENEEDITOR), Enable);
	
}

