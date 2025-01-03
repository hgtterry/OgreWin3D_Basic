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
#include "CL64_Properties_Brushes.h"

struct tag_BrushList
{
	Brush* First;
	Brush* Last;
};

CL64_Properties_Brushes::CL64_Properties_Brushes()
{
	BrushesDlg_Hwnd = nullptr;

	flag_Brushes_Dlg_Created = 0;

}

CL64_Properties_Brushes::~CL64_Properties_Brushes()
{
}

// *************************************************************************
// *	  	Show_GroupsDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Brushes::Show_Brushes_Dialog(bool Show)
{
	ShowWindow(BrushesDlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Start_Brush_Tabs_Dialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Brushes::Start_Brush_Tabs_Dialog()
{
	BrushesDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_BRUSHES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_Brush_Tabs);

	flag_Brushes_Dlg_Created = 1;

	Fill_ListBox();
	//Update_Dlg_Controls();
}

// *************************************************************************
// *			 Proc_Brush_Tabs:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Brushes::Proc_Brush_Tabs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_GD_BRUSHLIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GD_GROUPS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BRUSHCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GD_BRUSHCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		/*SendDlgItemMessage(hDlg, IDC_BT_GD_BRUSHPROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_GD_SELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DELETEBRUSH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_GD_GROUPS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_BRUSHCOUNT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_GD_BRUSHCOUNT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		/*if (GetDlgItem(hDlg, IDC_ST_GD_SELECTED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_SELECTED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BT_GD_BRUSHPROPERTIES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_GD_BRUSHPROPERTIES));
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

		if (some_item->idFrom == IDC_BT_DIMENSIONS && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_DIMENSIONS));
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

		if (some_item->idFrom == IDC_BT_DELETEBRUSH && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_DELETEBRUSH));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		//if (LOWORD(wParam) == IDC_BT_DELETEBRUSH)
		//{
		//	App->CLSB_Dialogs->YesNo("Are you sure", "Do you want to Delete the selected Brushes");

		//	bool Doit = App->CLSB_Dialogs->Canceled;
		//	if (Doit == 0)
		//	{
		//		App->m_pDoc = (CFusionDoc*)App->m_pMainFrame->GetCurrentDoc();
		//		App->CLSB_Doc->DeleteCurrentThing();
		//	}

		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_BT_DIMENSIONS)
		//{
		//	App->CLSB_Brushes->Start_Dimensions_Dlg();
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_GD_BRUSHLIST)
		//{
		//	if (App->CL_TabsGroups_Dlg->Groups_Dlg_Created == 1)
		//	{
		//		App->CL_TabsGroups_Dlg->List_Selection_Changed(1);
		//	}
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_BT_GD_BRUSHPROPERTIES)
		//{
		//	App->CL_TabsGroups_Dlg->Start_Brush_Properties_Dlg();
		//	SendDlgItemMessage(hDlg, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)App->CL_TabsGroups_Dlg->Selected_Index, (LPARAM)0);
		//	return TRUE;
		//}

		//// -----------------------------------------------------------------
		//if (LOWORD(wParam) == IDOK)
		//{
		//	//App->CL_TextureDialog->f_TextureDlg_Active = 0;
		//	//EndDialog(hDlg, LOWORD(wParam));
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDCANCEL)
		//{
		//	//App->CL_TextureDialog->f_TextureDlg_Active = 0;
		//	//EndDialog(hDlg, LOWORD(wParam));
		//	return TRUE;
		//}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *			 Fill_ListBox:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_Properties_Brushes::Fill_ListBox()
{
	if (flag_Brushes_Dlg_Created == 1)
	{
		SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		Level* pLevel = App->CL_Doc->pLevel;
		BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

		int Count = 0;
		Brush* b;
		b = pList->First;
		while (b != NULL)
		{
			SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)App->CL_Brush->Brush_GetName(b));
			Count++;
			b = b->Next;
		}

		char buff[100];
		SetDlgItemText(BrushesDlg_Hwnd, IDC_BRUSHCOUNT, _itoa(Count, buff, 10));
	}
}
