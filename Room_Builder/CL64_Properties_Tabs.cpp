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
#include "CL64_Properties_Tabs.h"

CL64_Properties_Tabs::CL64_Properties_Tabs()
{
	Tabs_Control_Hwnd = nullptr;

	flag_Tab_Templates = 1;
	flag_Tab_Texture = 0;
	flag_Tab_Group = 0;
	flag_Tab_3DSettings = 0;
	flag_Tabs_Dlg_Active = 0;
}

CL64_Properties_Tabs::~CL64_Properties_Tabs()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Properties_Tabs::Reset_Class()
{
	Enable_Tabs(false);
	Select_Templates_Tab();
}

// *************************************************************************
// *	  Show_Tabs_Control_Dlg:- Terry and Hazel Flanigan 2025		*
// *************************************************************************
void CL64_Properties_Tabs::Show_Tabs_Control_Dlg(bool Show)
{
	ShowWindow(Tabs_Control_Hwnd, Show);
}

// *************************************************************************
// *	  	Start_Tabs_Control_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Tabs::Start_Tabs_Control_Dlg()
{
	Tabs_Control_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TABSDIALOG, App->MainHwnd, (DLGPROC)Proc_Tabs_Control);

	flag_Tabs_Dlg_Active = 1;
	
	App->CL_Properties_Brushes->Start_Brush_Tabs_Dialog();
	App->CL_Properties_Brushes->Show_Brushes_Dialog(false);

	App->CL_Properties_Textures->Start_TextureDialog();
	App->CL_Properties_Textures->Show_Textures_Dialog(false);

	App->CL_Properties_Templates->Start_TemplatesDialog();
	App->CL_Properties_Templates->Show_TemplatesDialog(true);

	App->CL_Panels->Position_Tabs_Dlg();
	ShowWindow(Tabs_Control_Hwnd, true);

	CheckMenuItem(App->Menu_Map, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_CHECKED);
}

// *************************************************************************
// *        Tabs_Control_Proc:- Terry and Hazel Flanigan 2025			   *
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
		
		SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hDlg, RGB(213, 222, 242), 230, LWA_ALPHA);

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

		if (some_item->idFrom == IDC_TBTEXTURES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_TBTEXTURES));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Tabs->flag_Tab_Texture);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBTEMPLATES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Tabs->flag_Tab_Templates);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBGROUPS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_TBGROUPS));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Tabs->flag_Tab_Group);
			}
			
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_TBTEXTURES)
		{
			App->CL_Properties_Tabs->Hide_Dialogs();
			App->CL_Properties_Tabs->flag_Tab_Texture = 1;
			App->CL_Properties_Textures->Show_Textures_Dialog(true);

			RedrawWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TBTEMPLATES)
		{
			App->CL_Properties_Tabs->Hide_Dialogs();
			App->CL_Properties_Tabs->flag_Tab_Templates = 1;
			App->CL_Properties_Templates->Show_TemplatesDialog(true);

			RedrawWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TBGROUPS)
		{
			App->CL_Properties_Tabs->Hide_Dialogs();
			App->CL_Properties_Tabs->flag_Tab_Group = 1;
			App->CL_Properties_Brushes->Show_Brushes_Dialog(true);

			//App->CL_Properties_Brushes->Fill_ListBox();

			RedrawWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 0;
			App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(false);
			CheckMenuItem(App->Menu_Map, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);
			//EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 0;
			App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(false);
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
// *	  	Hide_Dialogs:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Properties_Tabs::Hide_Dialogs()
{
	// Reset all tab flags to indicate they are hidden
	flag_Tab_Texture = 0;
	flag_Tab_Group = 0;
	flag_Tab_Templates = 0;
	flag_Tab_3DSettings = 0;

	// Hide the respective dialogs for textures, brushes, and templates
	App->CL_Properties_Textures->Show_Textures_Dialog(false);
	App->CL_Properties_Brushes->Show_Brushes_Dialog(false);
	App->CL_Properties_Templates->Show_TemplatesDialog(false);

	// Redraw
	RedrawWindow(Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Select_Brushes_Tab:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Tabs::Select_Brushes_Tab()
{
	// Check is valid
	if (Tabs_Control_Hwnd && flag_Tabs_Dlg_Active == true)
	{
		// Hide any open dialogs
		Hide_Dialogs();

		// Show the brushes dialog
		App->CL_Properties_Brushes->Show_Brushes_Dialog(true);
		flag_Tab_Group = true; 

		// Redraw
		RedrawWindow(Tabs_Control_Hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW); // Redraw the tabs control
	}
}

// *************************************************************************
// *	  	Select_Textures_Tab:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Tabs::Select_Textures_Tab()
{
	if (Tabs_Control_Hwnd && flag_Tabs_Dlg_Active == 1)
	{
		if (flag_Tab_Texture == false)
		{
			Hide_Dialogs();
			App->CL_Properties_Textures->Show_Textures_Dialog(true);
			flag_Tab_Texture = 1;

			RedrawWindow(Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		else
		{
			App->CL_Properties_Textures->Get_Selected_Face();
		}
	}
}

// *************************************************************************
// *	  	Select_Templates_Tab:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Tabs::Select_Templates_Tab()
{
	if (Tabs_Control_Hwnd && flag_Tabs_Dlg_Active == 1)
	{
		Hide_Dialogs();
		App->CL_Properties_Templates->Show_TemplatesDialog(true);
		flag_Tab_Templates = 1;

		RedrawWindow(Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

// *************************************************************************
// *	  	Enable_Tabs_Dlg:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Properties_Tabs::Enable_Tabs_Dlg(bool Enable)
{
	EnableWindow(Tabs_Control_Hwnd, Enable);
}

// *************************************************************************
// *	  	Enable_Tabs:- Terry and Hazel Flanigan 2025					   *
// *************************************************************************
void CL64_Properties_Tabs::Enable_Tabs(bool Enable)
{
	EnableWindow(GetDlgItem(Tabs_Control_Hwnd, IDC_TBTEXTURES), Enable);
	EnableWindow(GetDlgItem(Tabs_Control_Hwnd, IDC_TBGROUPS), Enable);
	
	
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_HD_PREVIEW), Enable);
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_HD_SCENEEDITOR), Enable);
	
}

