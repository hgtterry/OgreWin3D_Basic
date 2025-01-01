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
}

CL64_Properties_Tabs::~CL64_Properties_Tabs()
{
}

// *************************************************************************
// *	  	Start_Tabs_Control_Dlg:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Properties_Tabs::Start_Tabs_Control_Dlg()
{

	Tabs_Control_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TABSDIALOG, App->MainHwnd, (DLGPROC)Proc_Tabs_Control);

	App->CL_Properties_Brushes->Start_Brush_Tabs_Dialog();
	App->CL_Properties_Brushes->Show_Brushes_Dialog(false);

	App->CL_Properties_Templates->Start_TemplatesDialog();
	App->CL_Properties_Templates->Show_TemplatesDialog(true);

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
		SendDlgItemMessage(hDlg, IDC_BT_3DSETTINGS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_TBTEXTURES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Tabs->flag_Tab_Texture);
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
			App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Tabs->flag_Tab_Group);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_3DSETTINGS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_3DSETTINGS));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Tabs->flag_Tab_3DSettings);
			}

			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		/*if (LOWORD(wParam) == IDC_TBTEXTURES)
		{
			App->CLSB_TabsControl->Hide_Dialogs();
			App->CLSB_TabsControl->Tab_Texture_Flag = 1;
			App->CLSB_TextureDialog->Show_Dialog(true);

			RedrawWindow(App->CLSB_TabsControl->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}*/

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

			App->CL_Properties_Brushes->Fill_ListBox();

			RedrawWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		//if (LOWORD(wParam) == IDC_BT_3DSETTINGS)
		//{
		//	if (App->CLSB_Tabs_True3D_Dlg->GameEditor_Active_Flag == 1)
		//	{

		//		App->CLSB_Tabs_True3D_Dlg->GameEditor_Active_Flag = 0;
		//		RedrawWindow(App->CLSB_Tabs_True3D_Dlg->Game_Tab_hDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		//		App->CLSB_Equity->Equity_Render_Mode = Enums::EQ_Mode_GameDirector;

		//		App->CLSB_Game_Editor->Hide_Game_Editor_Dialog();
		//		RedrawWindow(App->CLSB_Tabs_True3D_Dlg->Game_Tab_hDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		//	}
		//	else
		//	{
		//		/*if (App->BR_True3D_Mode_Active == 1)
		//		{
		//			App->CLSB_BR_Render->Exit_BR_3D_Mode();
		//			App->BR_True3D_Mode_Active = 0;
		//			App->CLSB_ViewMgrDlg->Was_BR_True3D_Mode_Active = 1;
		//		}*/

		//		App->CLSB_Tabs_True3D_Dlg->GameEditor_Active_Flag = 1;
		//		App->CLSB_Game_Editor->Go_Game_Editor();
		//	}
		//	/*App->CLSB_TabsControl->Hide_Dialogs();
		//	App->CLSB_TabsControl->Tab_3DSettings_Flag = 1;
		//	App->CLSB_Tabs_True3D_Dlg->Show_Game_Dlg(1);*/

		//	//RedrawWindow(App->CLSB_TabsControl->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		//	return TRUE;
		//}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			/*App->CLSB_TabsControl->Command_Panel_Started = 0;
			App->CLSB_TabsControl->f_TabsDlg_Active = 0;*/
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			/*App->CLSB_TabsControl->Command_Panel_Started = 0;
			App->CLSB_TabsControl->f_TabsDlg_Active = 0;*/
			EndDialog(hDlg, LOWORD(wParam));
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
	flag_Tab_Texture = 0;
	flag_Tab_Group = 0;
	flag_Tab_Templates = 0;
	flag_Tab_3DSettings = 0;

	//App->CLSB_TextureDialog->Show_Dialog(false);
	App->CL_Properties_Brushes->Show_Brushes_Dialog(false);
	App->CL_Properties_Templates->Show_TemplatesDialog(false);
	//App->CLSB_Tabs_True3D_Dlg->Show_Game_Dlg(0);

	RedrawWindow(Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
