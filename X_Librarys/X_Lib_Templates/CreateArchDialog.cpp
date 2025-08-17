/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
#include "CreateArchDialog.h"

CreateArchDialog::CreateArchDialog(void)
{
	m_NumSlits = 3;
	m_Thickness = 150;
	m_Width = 100;
	m_Radius = 200;
	m_WallSize = 16;
	m_Style = 0;
	m_EndAngle = 180.0f;
	m_StartAngle = 0.0f;
	m_TCut = FALSE;
	m_Height = 0.0f;
	m_Radius2 = 64.0f;
	m_Massive = false;
	m_Shape = 0;
	m_Sides = 3;
	m_Steps = false;
	m_CW = 0;

	m_UseCamPos = 0;

	flag_Solid_Flag = 0;
	flag_Hollow_Flag = 0;
	flag_Ring_Flag = 0;
	flag_Cut_Flag = 0;
	flag_Rectangle_Flag = 0;
	flag_Round_Flag = 0;

	strcpy(ArchName, "Arch");

	pArchTemplate = NULL;
}

CreateArchDialog::~CreateArchDialog(void)
{
}

// *************************************************************************
// *	  	Start_CreateArch_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateArchDialog::Start_CreateArch_Dlg()
{
	pArchTemplate = App->CL_Level->Level_GetArchTemplate();

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
	DialogBox(App->hInst, (LPCTSTR)IDD_CREATE_ARCH, App->MainHwnd, (DLGPROC)CreateArch_Proc);
}

// *************************************************************************
// *        CreateBox_Proc:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
LRESULT CALLBACK CreateArchDialog::CreateArch_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STTHICKNESS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STWIDTH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STRADIUS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STSIDES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STSTARTANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STENDANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STINNERRADIUS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STWALLSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STNUMSEGS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STHEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STARTANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ENDANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RADIUS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_WALLSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_NUMSLITS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_HEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_THICKNESS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_WIDTH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RADIUS2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SIDES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_ARCHSOLID, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ARCHHOLLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ARCHRING, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ARCHCUT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_ARCHRECTANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ARCHROUND, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_EDITNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_X_CreateArchDialog->Set_Members();
		App->CL_X_CreateArchDialog->Set_DLG_Members(hDlg);
		App->CL_X_CreateArchDialog->Set_Defaults(hDlg);

		int Count = App->CL_X_Brush->Get_Brush_Count();
		char Num[32];
		char Name[32];
		_itoa(Count, Num, 10);
		strcpy(Name, "Arch_");
		strcat(Name, Num);

		SetDlgItemText(hDlg, IDC_EDITNAME, (LPCTSTR)Name);

		//// ----------- Massive
		//if(App->CL_CreateArchDialog->m_Massive == 1)
		//{
		//	HWND temp = GetDlgItem(hDlg, IDC_MASSIVE);
		//	SendMessage(temp, BM_SETCHECK, 1, 0);
		//}


		//// ----------- CW & CCW
		//if(App->CL_CreateArchDialog->m_CW == 1)
		//{
		//	HWND temp = GetDlgItem(hDlg, IDC_CW);
		//	SendMessage(temp, BM_SETCHECK, 1, 0);
		//}

		//if(App->CL_CreateArchDialog->m_CW == 0)
		//{
		//	HWND temp = GetDlgItem(hDlg, IDC_CCW);
		//	SendMessage(temp, BM_SETCHECK, 1, 0);
		/*}


		HWND temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
		SendMessage(temp,BM_SETCHECK,1,0);
		App->CL_CreateArchDialog->m_UseCamPos = 0;*/

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{

		if (GetDlgItem(hDlg, IDC_STGENERAL) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STSTARTANGLE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STENDANGLE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STINNERRADIUS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STWALLSIZE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STNUMSEGS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STHEIGHT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		//if (GetDlgItem(hDlg, IDC_CCW) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		//if (GetDlgItem(hDlg, IDC_CW) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		if (GetDlgItem(hDlg, IDC_STSHAPE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTHICKNESS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STWIDTH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STRADIUS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STSIDES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		//if (GetDlgItem(hDlg, IDC_STEPS) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		//if (GetDlgItem(hDlg, IDC_MASSIVE) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		if (GetDlgItem(hDlg, IDC_STSTYLE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		// --------------------------------------------------
		if (GetDlgItem(hDlg, IDC_STNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		//if (GetDlgItem(hDlg, IDC_STCAMPOS) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		//if (GetDlgItem(hDlg, IDC_CKWORLDCENTRE) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		//if (GetDlgItem(hDlg, IDC_CKCAMPOSITION) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 0, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_ARCHSOLID)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_X_CreateArchDialog->flag_Solid_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ARCHHOLLOW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_X_CreateArchDialog->flag_Hollow_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ARCHRING)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_X_CreateArchDialog->flag_Ring_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ARCHCUT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_X_CreateArchDialog->flag_Cut_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ARCHRECTANGLE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_X_CreateArchDialog->flag_Rectangle_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ARCHROUND)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_X_CreateArchDialog->flag_Round_Flag);
			return CDRF_DODEFAULT;
		}

		/*if (some_item->idFrom == IDC_BOXDEFAULTS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_BOXROOM)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{

		/*if (LOWORD(wParam) == IDC_CKWORLDCENTRE)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp,BM_SETCHECK,1,0);

			temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp,BM_SETCHECK,0,0);

			App->CL_CreateArchDialog->m_UseCamPos = 0;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKCAMPOSITION)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp,BM_SETCHECK,1,0);

			temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp,BM_SETCHECK,0,0);

			App->CL_CreateArchDialog->m_UseCamPos = 1;
			return TRUE;
		}*/

		if (LOWORD(wParam) == IDC_BT_ARCHSOLID)
		{
			App->CL_X_CreateArchDialog->Zero_Dlg_Flags(hDlg);
			App->CL_X_CreateArchDialog->m_Style = 0;
			App->CL_X_CreateArchDialog->flag_Solid_Flag = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_ARCHHOLLOW)
		{
			App->CL_X_CreateArchDialog->Zero_Dlg_Flags(hDlg);
			App->CL_X_CreateArchDialog->m_Style = 1;
			App->CL_X_CreateArchDialog->flag_Hollow_Flag = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_ARCHRING)
		{
			App->CL_X_CreateArchDialog->Zero_Dlg_Flags(hDlg);
			App->CL_X_CreateArchDialog->m_Style = 2;
			App->CL_X_CreateArchDialog->flag_Ring_Flag = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_ARCHCUT)
		{
			if (App->CL_X_CreateArchDialog->flag_Cut_Flag == 0)
			{
				App->CL_X_CreateArchDialog->m_TCut = 1;
				App->CL_X_CreateArchDialog->flag_Cut_Flag = 1;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				return 1;
			}
			else
			{
				App->CL_X_CreateArchDialog->m_TCut = 0;
				App->CL_X_CreateArchDialog->flag_Cut_Flag = 0;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_ARCHRECTANGLE)
		{
			App->CL_X_CreateArchDialog->m_Shape = 0;
			App->CL_X_CreateArchDialog->flag_Rectangle_Flag = 1;
			App->CL_X_CreateArchDialog->flag_Round_Flag = 0;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_ARCHROUND)
		{
			App->CL_X_CreateArchDialog->m_Shape = 1;
			App->CL_X_CreateArchDialog->flag_Round_Flag = 1;
			App->CL_X_CreateArchDialog->flag_Rectangle_Flag = 0;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		/*if (LOWORD(wParam) == IDC_CW)
		{
			App->CL_X_CreateArchDialog->m_CW = 1;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CCW)
		{
			App->CL_X_CreateArchDialog->m_CW = 0;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_STEPS)
		{
			HWND temp = GetDlgItem(hDlg, IDC_STEPS);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_X_CreateArchDialog->m_Steps = 1;
				return 1;
			}
			else
			{
				App->CL_X_CreateArchDialog->m_Steps = 0;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_MASSIVE)
		{
			HWND temp = GetDlgItem(hDlg, IDC_MASSIVE);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_X_CreateArchDialog->m_Massive = 1;
				return 1;
			}
			else
			{
				App->CL_X_CreateArchDialog->m_Massive = 0;
				return 1;
			}

			return TRUE;
		}


		if (LOWORD(wParam) == ID_DEFAULTS)
		{
			App->CL_X_CreateArchDialog->Set_Defaults(hDlg);
			return TRUE;
		}*/

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_X_CreateArchDialog->Get_DLG_Members(hDlg);
			App->CL_X_CreateArchDialog->Set_ArchTemplate();
			App->CL_X_CreateArchDialog->CreateArch();

			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_Properties_Templates->Enable_Insert_Button(true);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *			Zero_Dlg_Flags:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateArchDialog::Zero_Dlg_Flags(HWND hDlg)
{
	flag_Solid_Flag = 0;
	flag_Hollow_Flag = 0;
	flag_Ring_Flag = 0;

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *		   CreateArch:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CreateArchDialog::CreateArch()
{
	App->CL_Doc->OnToolsTemplate();

	Brush* pArch;

	pArch = App->CL_X_BrushTemplate->BrushTemplate_CreateArch(pArchTemplate);
	if (pArch != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName, ArchName);
		strcpy(pArch->Name, "Test"); // TODO Why
		CreateNewTemplateBrush(pArch);
	}
	else
	{
		App->Say("No Arch");
	}
}

// *************************************************************************
// *       CreateNewTemplateBrush:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CreateArchDialog::CreateNewTemplateBrush(Brush* pBrush)
{
	T_Vec3* pTemplatePos;
	T_Vec3 MoveVec;
	T_Vec3 BrushPos;

	assert(pBrush != NULL);

	if (App->CL_Doc->BTemplate != NULL)
	{
		App->CL_X_Brush->Brush_Destroy(&App->CL_Doc->BTemplate);
	}

	App->CL_Doc->CurBrush = pBrush;

	App->CL_Doc->TempEnt = FALSE;
	App->CL_Doc->SetDefaultBrushTexInfo(App->CL_Doc->CurBrush);
	App->CL_X_Brush->Brush_Bound(App->CL_Doc->CurBrush);
	App->CL_X_Brush->Brush_Get_Center(App->CL_Doc->CurBrush, &BrushPos);

	pTemplatePos = App->CL_Level->Level_GetTemplatePos(App->CL_Doc->Current_Level);

	if (m_UseCamPos == 1)
	{
		Ogre::Vector3 Pos;

		Pos = App->CL_Ogre->camNode->getPosition();

		pTemplatePos->x = Pos.x;
		pTemplatePos->y = Pos.y;
		pTemplatePos->z = Pos.z;
	}
	else
	{
		pTemplatePos->x = 0;
		pTemplatePos->y = 0;
		pTemplatePos->z = 0;
	}

	App->CL_Maths->Vector3_Subtract(pTemplatePos, &BrushPos, &MoveVec);

	App->CL_X_Brush->Brush_Move(App->CL_Doc->CurBrush, &MoveVec);

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	App->CL_Level->flag_Level_is_Modified = 1;
}

// *************************************************************************
// *		 Set_Members:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void CreateArchDialog::Set_Members()
{
	m_NumSlits = pArchTemplate->NumSlits;
	m_Thickness = pArchTemplate->Thickness;
	m_Width = pArchTemplate->Width;
	m_Radius = pArchTemplate->Radius;
	m_WallSize = pArchTemplate->WallSize;
	m_Style = pArchTemplate->Style;
	m_EndAngle = pArchTemplate->EndAngle;
	m_StartAngle = pArchTemplate->StartAngle;
	m_TCut = pArchTemplate->TCut;
	m_Height = pArchTemplate->Height;
	m_Radius2 = pArchTemplate->Radius2;
	m_Massive = pArchTemplate->Massive;
	m_Shape = pArchTemplate->Shape;
	m_Sides = pArchTemplate->Sides;
	m_Steps = pArchTemplate->Steps;
	m_CW = pArchTemplate->CW;
}

// *************************************************************************
// *		 Set_DLG_Members:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateArchDialog::Set_DLG_Members(HWND hDlg)
{
	char buf[MAX_PATH];
	sprintf(buf, "%0.0f", m_StartAngle);
	SetDlgItemText(hDlg, IDC_STARTANGLE, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_EndAngle);
	SetDlgItemText(hDlg, IDC_ENDANGLE, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Radius);
	SetDlgItemText(hDlg, IDC_RADIUS, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_WallSize);
	SetDlgItemText(hDlg, IDC_WALLSIZE, (LPCTSTR)buf);

	sprintf(buf, "%i", m_NumSlits);
	SetDlgItemText(hDlg, IDC_NUMSLITS, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Height);
	SetDlgItemText(hDlg, IDC_HEIGHT, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Thickness);
	SetDlgItemText(hDlg, IDC_THICKNESS, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Width);
	SetDlgItemText(hDlg, IDC_WIDTH, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Radius2);
	SetDlgItemText(hDlg, IDC_RADIUS2, (LPCTSTR)buf);

	sprintf(buf, "%i", m_Sides);
	SetDlgItemText(hDlg, IDC_SIDES, (LPCTSTR)buf);

	/*HWND temp = GetDlgItem(hDlg, IDC_TCUT);
	if (m_TCut == 1)
	{
		SendMessage(temp,BM_SETCHECK,1,0);
	}
	else
	{
		SendMessage(temp,BM_SETCHECK,0,0);
	}*/

}

// *************************************************************************
// *		 Get_DLG_Members:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CreateArchDialog::Get_DLG_Members(HWND hDlg)
{
	char buf[MAX_PATH];

	GetDlgItemText(hDlg, IDC_STARTANGLE, (LPTSTR)buf, MAX_PATH);
	m_StartAngle = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ENDANGLE, (LPTSTR)buf, MAX_PATH);
	m_EndAngle = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_RADIUS, (LPTSTR)buf, MAX_PATH);
	m_Radius = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_WALLSIZE, (LPTSTR)buf, MAX_PATH);
	m_WallSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_NUMSLITS, (LPTSTR)buf, MAX_PATH);
	m_NumSlits = (int)atoi(buf);

	GetDlgItemText(hDlg, IDC_HEIGHT, (LPTSTR)buf, MAX_PATH);
	m_Height = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_THICKNESS, (LPTSTR)buf, MAX_PATH);
	m_Thickness = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_WIDTH, (LPTSTR)buf, MAX_PATH);
	m_Width = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_RADIUS2, (LPTSTR)buf, MAX_PATH);
	m_Radius2 = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_SIDES, (LPTSTR)buf, MAX_PATH);
	m_Sides = (int)atoi(buf);

	GetDlgItemText(hDlg, IDC_EDITNAME, (LPTSTR)buf, MAX_PATH);
	strcpy(ArchName, buf);

}

// *************************************************************************
// *		 Set_ArchTemplate:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateArchDialog::Set_ArchTemplate()
{
	pArchTemplate->CW = m_CW;
	pArchTemplate->EndAngle = m_EndAngle;
	pArchTemplate->Height = m_Height;
	pArchTemplate->Massive = m_Massive;
	pArchTemplate->NumSlits = m_NumSlits;
	pArchTemplate->Radius = m_Radius;
	pArchTemplate->Radius2 = m_Radius2;
	pArchTemplate->Shape = m_Shape;
	pArchTemplate->Sides = m_Sides;
	pArchTemplate->StartAngle = m_StartAngle;
	pArchTemplate->Steps = m_Steps;
	pArchTemplate->Style = m_Style;
	pArchTemplate->TCut = m_TCut;
	pArchTemplate->Thickness = m_Thickness;
	pArchTemplate->WallSize = m_WallSize;
	pArchTemplate->Width = m_Width;
}

// *************************************************************************
// *	    	Set_Defaults:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CreateArchDialog::Set_Defaults(HWND hDlg)
{
	m_NumSlits = 3;
	m_Thickness = 150;
	m_Width = 100;
	m_Radius = 200;
	m_WallSize = 16;
	m_Style = 0;
	m_EndAngle = 180.0f;
	m_StartAngle = 0.0f;
	m_TCut = FALSE;
	m_Height = 0.0f;
	m_Radius2 = 64.0f;
	m_Massive = false;
	m_Shape = 0;
	m_Sides = 3;
	m_Steps = false;
	m_CW = 0;

	Set_DLG_Members(hDlg);

	App->CL_X_CreateArchDialog->Zero_Dlg_Flags(hDlg);
	App->CL_X_CreateArchDialog->m_Style = 0;
	App->CL_X_CreateArchDialog->flag_Solid_Flag = 1;

	App->CL_X_CreateArchDialog->m_Shape = 0;
	App->CL_X_CreateArchDialog->flag_Rectangle_Flag = 1;
	App->CL_X_CreateArchDialog->flag_Round_Flag = 0;

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	// ----------- Style Solid Hollow Funnel
	//if(m_Style == 0)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_SOLID);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);

	//	temp = GetDlgItem(hDlg, IDC_HOLLOW);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);

	//	temp = GetDlgItem(hDlg, IDC_RING);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}

	//if(m_Style == 1)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_HOLLOW);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);

	//	temp = GetDlgItem(hDlg, IDC_SOLID);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);

	//	temp = GetDlgItem(hDlg, IDC_RING);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}

	//if(m_Style == 2)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_RING);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);

	//	temp = GetDlgItem(hDlg, IDC_SOLID);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);

	//	temp = GetDlgItem(hDlg, IDC_HOLLOW);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}

	//// ----------- Shape
	//if(m_Shape == 0)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_RECTANGULAR);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);

	//	temp = GetDlgItem(hDlg, IDC_ROUND);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}

	//if(m_Shape == 1)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_ROUND);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);

	//	temp = GetDlgItem(hDlg, IDC_RECTANGULAR);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}

	//// ----------- Steps
	//if(m_Steps == 1)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_STEPS);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);
	//}
	//else
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_STEPS);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}

	//// ----------- Massive
	//if(m_Massive == 1)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_MASSIVE);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);
	//}
	//else
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_MASSIVE);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}


	//// ----------- CW & CCW
	//if(m_CW == 1)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_CW);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);

	//	temp = GetDlgItem(hDlg, IDC_CCW);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}

	//if(m_CW == 0)
	//{
	//	HWND temp = GetDlgItem(hDlg, IDC_CCW);
	//	SendMessage(temp, BM_SETCHECK, 1, 0);

	//	temp = GetDlgItem(hDlg, IDC_CW);
	//	SendMessage(temp, BM_SETCHECK, 0, 0);
	//}


}

