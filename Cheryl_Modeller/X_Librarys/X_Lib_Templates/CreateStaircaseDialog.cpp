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
#include "CreateStaircaseDialog.h"

CreateStaircaseDialog::CreateStaircaseDialog(void)
{
	m_Height = 128.0;
	m_Length = 128.0;
	m_Width = 64.0;
	m_MakeRamp = false;
	m_TCut = false;
	m_NumberOfStairs = 8;
	m_UseCamPos = 0;

	pStaircaseTemplate = NULL;
	
	strcpy(StaircaseName, "Spheroid");

	flag_Ramp_Flag_Dlg = 0;
	flag_Stairs_Flag_Dlg = 1;
}

CreateStaircaseDialog::~CreateStaircaseDialog(void)
{
}

LRESULT CALLBACK CreateStaircaseDialog::OwnerEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		switch (wParam)
		{

		case VK_RETURN:
		{
			App->CL_X_CreateStaircaseDialog->Update();
			return 0;
		}

		}
	}

	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CreateStaircaseDialog::Init_Bmps_Globals(HWND hDlg)
{
	HWND Temp = GetDlgItem(hDlg, IDC_BT_HELP);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_Help_Bmp);

	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOFADE, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);
	SendMessage(hTooltip_TB_2, TTM_SETMAXTIPWIDTH, 0, 250);

	Temp = GetDlgItem(hDlg, IDC_BT_HELP);
	TOOLINFO ti1 = { 0 };
	ti1.cbSize = sizeof(ti1);
	ti1.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti1.uId = (UINT_PTR)Temp;
	ti1.lpszText = (LPSTR)"Help / Information.";
	ti1.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti1);
}

// *************************************************************************
// *		Capture_Edit_Boxes:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CreateStaircaseDialog::Capture_Edit_Boxes(HWND hDlg)
{
	HWND control = GetDlgItem(hDlg, IDC_ED_STAIRS_HEIGHT);
	SetWindowSubclass(control, OwnerEditProc, 0, 0);

	control = GetDlgItem(hDlg, IDC_ED_STAIRS_WIDTH);
	SetWindowSubclass(control, OwnerEditProc, 0, 0);

	control = GetDlgItem(hDlg, IDC_ED_STAIRS_LENGTH);
	SetWindowSubclass(control, OwnerEditProc, 0, 0);

	control = GetDlgItem(hDlg, IDC_ED_STAIRS_STEPS);
	SetWindowSubclass(control, OwnerEditProc, 0, 0);
}

// *************************************************************************
// *		Remove_Edit_Boxes:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CreateStaircaseDialog::Remove_Edit_Boxes(HWND hDlg)
{
	HWND control = GetDlgItem(hDlg, IDC_ED_STAIRS_HEIGHT);
	RemoveWindowSubclass(control, OwnerEditProc, 0);

	control = GetDlgItem(hDlg, IDC_ED_STAIRS_WIDTH);
	RemoveWindowSubclass(control, OwnerEditProc, 0);

	control = GetDlgItem(hDlg, IDC_ED_STAIRS_LENGTH);
	RemoveWindowSubclass(control, OwnerEditProc, 0);

	control = GetDlgItem(hDlg, IDC_ED_STAIRS_STEPS);
	RemoveWindowSubclass(control, OwnerEditProc, 0);
}

// *************************************************************************
// *	  	Start_CreateArch_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateStaircaseDialog::Start_CreateStaircase_Dlg()
{
	pStaircaseTemplate = App->CL_Level->Level_GetStaircaseTemplate();

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	CreateDialog(App->hInst, (LPCTSTR)IDD_CREATE_STAIRCASE, App->MainHwnd, (DLGPROC)Proc_CreateStaircase);

	if (App->CL_App_Templates->Shape_Dlg_hWnd)
	{
		App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);
	}

	App->CL_X_CreateStaircaseDialog->Get_DLG_Members(App->CL_App_Templates->Shape_Dlg_hWnd);
	App->CL_X_CreateStaircaseDialog->Set_StaircaseTemplate();
	App->CL_X_CreateStaircaseDialog->CreateStaircase();

	App->CL_Ogre->OGL_Listener->Show_Visuals(true);
}

// *************************************************************************
// *        Proc_CreateStaircase:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CreateStaircaseDialog::Proc_CreateStaircase(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_X_CreateStaircaseDialog->Capture_Edit_Boxes(hDlg);

		SendDlgItemMessage(hDlg, IDC_ST_STAIRS_GENERAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_STAIRS_HEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STAIRS_WIDTH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STAIRS_LENGTH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STAIRS_STEPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ED_STAIRS_HEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_STAIRS_WIDTH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_STAIRS_LENGTH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_STAIRS_STEPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_STAIRS_RAMP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_STAIRS_STAIRS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_STAIRS_CUT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_STAIRS_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STAIRS_EDITNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_STAIRS_POSITION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_STAIRS_WORLDCENTRE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_STAIRS_CAMPOSITION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_STAIRS_DEFAULTS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_UPDATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		App->CL_X_CreateStaircaseDialog->Init_Bmps_Globals(hDlg);

		App->CL_App_Templates->Shape_Dlg_hWnd = hDlg;

		App->CL_X_Shapes_3D->Render_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_BOX_3D, hDlg, (DLGPROC)App->CL_X_Shapes_3D->Proc_Box_Viewer_3D);
		App->CL_X_Shapes_3D->Set_OgreWindow();

		// Initialize dialog members

		App->CL_X_CreateStaircaseDialog->Set_Members();
		App->CL_X_CreateStaircaseDialog->Set_DLG_Members(hDlg);
		//App->CL_X_CreateStaircaseDialog->Set_Defaults(hDlg);


		int Count = App->CL_X_Brush->Get_Brush_Count();
		char Num[32];
		char Name[32];
		_itoa(Count+1, Num, 10);
		strcpy(Name, "Staircase_");
		strcat(Name, Num);
		strcpy(App->CL_X_CreateStaircaseDialog->StaircaseName, Name);
		SetDlgItemText(hDlg, IDC_STAIRS_EDITNAME, (LPCTSTR)Name);

		if (App->CL_X_CreateStaircaseDialog->m_MakeRamp == 1)
		{
			App->CL_X_CreateStaircaseDialog->flag_Ramp_Flag_Dlg = 1;
			App->CL_X_CreateStaircaseDialog->flag_Stairs_Flag_Dlg = 0;
			return 1;
		}
		else
		{
			App->CL_X_CreateStaircaseDialog->flag_Ramp_Flag_Dlg = 0;
			App->CL_X_CreateStaircaseDialog->flag_Stairs_Flag_Dlg = 1;
			return 1;
		}

		HWND temp = GetDlgItem(hDlg, IDC_CK_STAIRS_WORLDCENTRE);
		SendMessage(temp, BM_SETCHECK, 1, 0);
		App->CL_X_CreateStaircaseDialog->m_UseCamPos = 0;

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_STAIRS_GENERAL) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STAIRS_HEIGHT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STAIRS_WIDTH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STAIRS_LENGTH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STAIRS_STEPS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		/*if (GetDlgItem(hDlg, IDC_TCUT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		// --------------------------------------------------
		if (GetDlgItem(hDlg, IDC_ST_STAIRS_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STAIRS_POSITION) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_STAIRS_WORLDCENTRE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_STAIRS_CAMPOSITION) == (HWND)lParam)
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

		if (some_item->idFrom == IDC_BT_STAIRS_RAMP)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_X_CreateStaircaseDialog->flag_Ramp_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_STAIRS_STAIRS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_X_CreateStaircaseDialog->flag_Stairs_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_STAIRS_CUT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_X_CreateStaircaseDialog->m_TCut);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_STAIRS_DEFAULTS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_UPDATE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
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

	case WM_MOUSEWHEEL:
	{
		int zDelta = (short)HIWORD(wParam);    // wheel rotation

		if (zDelta > 0)
		{
			App->CL_X_Shapes_3D->RenderListener->Wheel_Move = -1;
		}
		else if (zDelta < 0)
		{
			App->CL_X_Shapes_3D->RenderListener->Wheel_Move = 1;
		}

		return 1;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_UPDATE)
		{
			App->CL_X_CreateStaircaseDialog->Update();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_STAIRS_WORLDCENTRE)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_STAIRS_WORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CK_STAIRS_CAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CL_X_CreateStaircaseDialog->m_UseCamPos = 0;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_STAIRS_CAMPOSITION)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_STAIRS_CAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CK_STAIRS_WORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CL_X_CreateStaircaseDialog->m_UseCamPos = 1;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_STAIRS_CUT)
		{
			if (App->CL_X_CreateStaircaseDialog->m_TCut == 1)
			{
				App->CL_X_CreateStaircaseDialog->m_TCut = 0;
				return 1;
			}
			else
			{
				App->CL_X_CreateStaircaseDialog->m_TCut = 1;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_STAIRS_STAIRS)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_ST_PIC);
			SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_Stairs_Bmp);

			App->CL_X_CreateStaircaseDialog->m_MakeRamp = 0;

			App->CL_X_CreateStaircaseDialog->flag_Stairs_Flag_Dlg = 1;
			App->CL_X_CreateStaircaseDialog->flag_Ramp_Flag_Dlg = 0;

			App->CL_X_CreateStaircaseDialog->Update();

			HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_ED_STAIRS_STEPS);
			EnableWindow(temp, true);

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_STAIRS_RAMP)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_ST_PIC);
			SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_Ramp_Bmp);

			App->CL_X_CreateStaircaseDialog->m_MakeRamp = 1;

			App->CL_X_CreateStaircaseDialog->flag_Stairs_Flag_Dlg = 0;
			App->CL_X_CreateStaircaseDialog->flag_Ramp_Flag_Dlg = 1;

			App->CL_X_CreateStaircaseDialog->Update();

			HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_ED_STAIRS_STEPS);
			EnableWindow(temp, false);

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_STAIRS_DEFAULTS)
		{
			App->CL_App_Templates->Enable_Shape_Dialog(false);

			App->CL_Dialogs->YesNo("Reset to Defaults", "All Dimensions will be reset");
			if (App->CL_Dialogs->flag_Dlg_Canceled == false)
			{
				App->CL_X_CreateStaircaseDialog->Set_Defaults(hDlg);

				App->CL_X_CreateStaircaseDialog->Update();
				HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_ED_STAIRS_STEPS);
				EnableWindow(temp, true);
			}

			App->CL_App_Templates->Enable_Shape_Dialog(true);
			App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HELP)
		{
			App->Open_HTML((LPSTR)"Help\\Create_Staircase_Slope.html");
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_X_CreateStaircaseDialog->Get_DLG_Members(hDlg);
			App->CL_X_CreateStaircaseDialog->Set_StaircaseTemplate();
			App->CL_X_CreateStaircaseDialog->CreateStaircase();

			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);

			strcpy(App->CL_Properties_Templates->LastCreated_ShapeName, App->CL_X_CreateStaircaseDialog->StaircaseName);
			App->CL_Properties_Templates->Insert_Template();

			App->CL_X_Shapes_3D->Close_OgreWindow();

			App->CL_X_CreateStaircaseDialog->Remove_Edit_Boxes(hDlg);

			App->CL_App_Templates->Enable_Map_Editor_Dialogs(true);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_X_Shapes_3D->Close_OgreWindow();

			App->CL_X_CreateStaircaseDialog->Remove_Edit_Boxes(hDlg);

			App->CL_Interface->Deselect_All_Brushes_Update_Dlgs();
			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
			App->CL_Ogre->OGL_Listener->Show_Visuals(false);

			App->CL_App_Templates->Enable_Map_Editor_Dialogs(true);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *			Update:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void CreateStaircaseDialog::Update(void)
{
	App->CL_X_CreateStaircaseDialog->Get_DLG_Members(App->CL_App_Templates->Shape_Dlg_hWnd);
	App->CL_X_CreateStaircaseDialog->Set_StaircaseTemplate();
	App->CL_X_CreateStaircaseDialog->CreateStaircase();
}

// *************************************************************************
// *		   CreateStaircase:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateStaircaseDialog::CreateStaircase()
{
	App->CL_Doc->OnToolsTemplate();

	Brush* pStaircase;

	pStaircase = App->CL_X_BrushTemplate->BrushTemplate_CreateStaircase(pStaircaseTemplate);
	if (pStaircase != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName, StaircaseName);
		CreateNewTemplateBrush(pStaircase);
	}
	else
	{
		App->Say("No Staircase");
	}
}

// *************************************************************************
// *       CreateNewTemplateBrush:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateStaircaseDialog::CreateNewTemplateBrush(Brush* pBrush)
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

	if (m_UseCamPos == 1 && App->flag_3D_Started == true)
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

	App->CL_X_Maths->Vector3_Subtract(pTemplatePos, &BrushPos, &MoveVec);

	App->CL_X_Brush->Brush_Move(App->CL_Doc->CurBrush, &MoveVec);

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	App->CL_Level->flag_Level_is_Modified = true;
}

// *************************************************************************
// *		 Set_Members:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CreateStaircaseDialog::Set_Members()
{
	m_Height = pStaircaseTemplate->Height;
	m_Length = pStaircaseTemplate->Length;
	m_Width = pStaircaseTemplate->Width;
	m_MakeRamp = pStaircaseTemplate->MakeRamp;
	m_TCut = pStaircaseTemplate->TCut;
	m_NumberOfStairs = pStaircaseTemplate->NumberOfStairs;
}

// *************************************************************************
// *		 Set_DLG_Members:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateStaircaseDialog::Set_DLG_Members(HWND hDlg)
{
	char buf[MAX_PATH];
	sprintf(buf, "%0.0f", m_Height);
	SetDlgItemText(hDlg, IDC_ED_STAIRS_HEIGHT, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Width);
	SetDlgItemText(hDlg, IDC_ED_STAIRS_WIDTH, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Length);
	SetDlgItemText(hDlg, IDC_ED_STAIRS_LENGTH, (LPCTSTR)buf);

	sprintf(buf, "%i", m_NumberOfStairs);
	SetDlgItemText(hDlg, IDC_ED_STAIRS_STEPS, (LPCTSTR)buf);


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
// *		 Get_DLG_Members:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateStaircaseDialog::Get_DLG_Members(HWND hDlg)
{
	char buf[MAX_PATH];

	GetDlgItemText(hDlg, IDC_ED_STAIRS_HEIGHT, (LPTSTR)buf, MAX_PATH);
	m_Height = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ED_STAIRS_WIDTH, (LPTSTR)buf, MAX_PATH);
	m_Width = (int)atoi(buf);

	GetDlgItemText(hDlg, IDC_ED_STAIRS_LENGTH, (LPTSTR)buf, MAX_PATH);
	m_Length = (int)atoi(buf);

	GetDlgItemText(hDlg, IDC_ED_STAIRS_STEPS, (LPTSTR)buf, MAX_PATH);
	m_NumberOfStairs = (int)atoi(buf);

	GetDlgItemText(hDlg, IDC_STAIRS_EDITNAME, (LPTSTR)buf, MAX_PATH);
	strcpy(StaircaseName, buf);
}

// *************************************************************************
// *		 Set_StaircaseTemplate:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateStaircaseDialog::Set_StaircaseTemplate()
{
	pStaircaseTemplate->Height = m_Height;
	pStaircaseTemplate->Length = m_Length;
	pStaircaseTemplate->MakeRamp = m_MakeRamp;
	pStaircaseTemplate->NumberOfStairs = m_NumberOfStairs;
	pStaircaseTemplate->TCut = m_TCut;
	pStaircaseTemplate->Width = m_Width;
}

// *************************************************************************
// *	    	Set_Defaults:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateStaircaseDialog::Set_Defaults(HWND hDlg)
{
	m_Height = 128.0;
	m_Length = 128.0;
	m_Width = 64.0;
	m_MakeRamp = false;
	m_TCut = false;
	m_NumberOfStairs = 8;

	Set_DLG_Members(hDlg);


	/*if(m_Solid == 0)
	{
		HWND temp = GetDlgItem(hDlg,IDC_SOLID);
		SendMessage(temp,BM_SETCHECK,1,0);

		temp = GetDlgItem(hDlg,IDC_HOLLOW);
		SendMessage(temp,BM_SETCHECK,0,0);
	}
	else
	{
		HWND temp = GetDlgItem(hDlg,IDC_HOLLOW);
		SendMessage(temp,BM_SETCHECK,1,0);

		temp = GetDlgItem(hDlg,IDC_SOLID);
		SendMessage(temp,BM_SETCHECK,0,0);
	}*/
}
