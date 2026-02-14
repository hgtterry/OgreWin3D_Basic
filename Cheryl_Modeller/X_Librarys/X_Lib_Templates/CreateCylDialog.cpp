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
#include "CreateCylDialog.h"

CreateCylDialog::CreateCylDialog(void)
{
	m_BotXOffset = 0.0;
	m_BotXSize = 128.0;
	m_BotZOffset = 0.0;
	m_BotZSize = 128.0;
	m_Solid = 0;
	m_TopXOffset = 0.0;
	m_TopXSize = 128.0;
	m_TopZOffset = 0.0;
	m_TopZSize = 128.0;
	m_YSize = 200.0;
	m_RingLength = 0.0;
	m_TCut = false;
	m_VerticalStripes = 0;
	m_Thickness = 0.0f;

	m_UseCamPos = 0;

	strcpy(CylinderName, "Cylinder");

	pCylinderTemplate = NULL;

	flag_Solid_Flag_Dlg = 1;
	flag_Hollow_Flag_Dlg = 0;
	flag_Ring_Flag_Dlg = 0;
}

CreateCylDialog::~CreateCylDialog(void)
{
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* CreateCylDialog::GetVersion()
{
	return (LPSTR)"Create Cylinder [ 08-02-26 ] Build 1 ";
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CreateCylDialog::Init_Bmps_Globals(HWND hDlg)
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

LRESULT CALLBACK CreateCylDialog::OwnerEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		switch (wParam)
		{

		case VK_RETURN:
		{
			App->CL_Libs->CL_CreateCylinder->Update();
			return 0;
		}

		}
	}

	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// *************************************************************************
// *		Capture_Edit_Boxes:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CreateCylDialog::Capture_Edit_Boxes(HWND hDlg)
{
	// Array of control IDs to be processed
	const int controlIDs[] = { IDC_TOPXSIZE, IDC_TOPXOFF, IDC_TOPZSIZE, IDC_TOPZOFF , IDC_BOTXSIZE
		,IDC_BOTXOFF ,IDC_BOTZSIZE ,IDC_BOTZOFF
		,IDC_YSIZE ,IDC_VERTSTRIPES, IDC_THICKNESS, IDC_RINGLENGTH };

	// Loop through each control ID and set the window subclass
	for (int id : controlIDs) {
		HWND control = GetDlgItem(hDlg, id); // Retrieve the handle for the control
		SetWindowSubclass(control, OwnerEditProc, 0, 0); // Set the subclass procedure for the control
	}
}

// *************************************************************************
// *		Remove_Edit_Boxes:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CreateCylDialog::Remove_Edit_Boxes(HWND hDlg)
{
	// Array of control IDs to remove the subclass from
	const int controlIDs[] = { IDC_TOPXSIZE, IDC_TOPXOFF, IDC_TOPZSIZE, IDC_TOPZOFF , IDC_BOTXSIZE
		,IDC_BOTXOFF ,IDC_BOTZSIZE ,IDC_BOTZOFF
		,IDC_YSIZE ,IDC_VERTSTRIPES, IDC_THICKNESS, IDC_RINGLENGTH };

	// Iterate through each control ID and remove the subclass
	for (int id : controlIDs)
	{
		HWND control = GetDlgItem(hDlg, id);
		RemoveWindowSubclass(control, OwnerEditProc, 0);
	}
}

// *************************************************************************
// *	  	Start_CreateCyl_Dlg:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateCylDialog::Start_CreateCyl_Dlg()
{
	App->CL_App_Templates->Shape_Dlg_hWnd = nullptr;

	pCylinderTemplate = App->CL_Level->Level_GetCylinderTemplate();

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	CreateDialog(App->hInst, (LPCTSTR)IDD_CREATE_CYLINDER, App->MainHwnd, (DLGPROC)Proc_Create_Cylinder);

	if (App->CL_App_Templates->Shape_Dlg_hWnd)
	{
		App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);
	}

	Get_DLG_Members(App->CL_App_Templates->Shape_Dlg_hWnd);
	Set_CylinderTemplate();
	CreateCylinder_New();

	App->CL_Ogre->OGL_Listener->Show_Visuals(true);
}

// *************************************************************************
// *        Proc_Create_Cylinder:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CreateCylDialog::Proc_Create_Cylinder(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& m_Cylinder = App->CL_Libs->CL_CreateCylinder; // App->CL_App_Templates->CL_CreateCylinder

	switch (message)
	{
	case WM_INITDIALOG:
	{
		m_Cylinder->Capture_Edit_Boxes(hDlg);

		SendDlgItemMessage(hDlg, IDC_CYL_DEFAULTS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_TOPXSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TOPZSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TOPXOFF, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TOPZOFF, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BOTXSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BOTZSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BOTXOFF, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BOTZOFF, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STTOPXSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTOPZSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTOPXOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTOPZOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STBOTXSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STBOTZSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STBOTXOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STBOTZOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STWALLTHICKNESS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STRINGLENGTH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STSTRIPES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STCAMPOS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_YSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_VERTSTRIPES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_THICKNESS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RINGLENGTH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STYSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_EDITNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_CUTBRUSH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CYL_SOLID, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CYL_HOLLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CYL_RING, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CKWORLDCENTRE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKCAMPOSITION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_UPDATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_App_Templates->Shape_Dlg_hWnd = hDlg;

		m_Cylinder->Init_Bmps_Globals(hDlg);

		App->CL_X_Shapes_3D->Start_Zoom = 400;
		App->CL_X_Shapes_3D->Render_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_BOX_3D, hDlg, (DLGPROC)App->CL_X_Shapes_3D->Proc_Box_Viewer_3D);
		
		SetWindowPos(App->CL_X_Shapes_3D->Render_hWnd, NULL, 20, 370, 0, 0, SWP_NOSIZE);

		App->CL_X_Shapes_3D->Set_OgreWindow();

		m_Cylinder->Set_Members();
		m_Cylinder->Set_DLG_Members(hDlg);
		m_Cylinder->Set_Defaults(hDlg);

		SetDlgItemText(hDlg, IDC_EDITNAME, (LPCTSTR)"Cylinder");

		// ----------- Style Solid Hollow Funnel
		if (m_Cylinder->m_Solid == 0)
		{
			m_Cylinder->flag_Solid_Flag_Dlg = 1;
			m_Cylinder->flag_Hollow_Flag_Dlg = 0;
			m_Cylinder->flag_Ring_Flag_Dlg = 0;
		}

		if (m_Cylinder->m_Solid == 1)
		{
			m_Cylinder->flag_Hollow_Flag_Dlg = 1;
			m_Cylinder->flag_Solid_Flag_Dlg = 0;
			m_Cylinder->flag_Ring_Flag_Dlg = 0;
		}

		if (m_Cylinder->m_Solid == 2)
		{
			m_Cylinder->flag_Ring_Flag_Dlg = 1;
			m_Cylinder->flag_Hollow_Flag_Dlg = 0;
			m_Cylinder->flag_Solid_Flag_Dlg = 0;
		}
		// ----------------------------------------------

		HWND temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
		SendMessage(temp, BM_SETCHECK, 1, 0);
		m_Cylinder->m_UseCamPos = 0;

		int Count = App->CL_X_Brush->Get_Brush_Count();
		char Num[32];
		char Name[32];
		_itoa(Count+1, Num, 10);
		strcpy(Name, "Cylinder_");
		strcat(Name, Num);

		SetDlgItemText(hDlg, IDC_EDITNAME, (LPCTSTR)Name);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STTOP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STBOTTOM) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STGENERAL) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTOPXSIZE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTOPZSIZE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTOPXOFFSET) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTOPZOFFSET) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STWALLTHICKNESS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STRINGLENGTH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STSTRIPES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STYSIZE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STBOTXSIZE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STBOTZSIZE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STBOTXOFFSET) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STBOTZOFFSET) == (HWND)lParam)
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

		if (GetDlgItem(hDlg, IDC_STCAMPOS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CKWORLDCENTRE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CKCAMPOSITION) == (HWND)lParam)
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

		if (some_item->idFrom == IDC_CYL_DEFAULTS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CUTBRUSH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, m_Cylinder->m_TCut);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CYL_SOLID)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, m_Cylinder->flag_Solid_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CYL_HOLLOW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, m_Cylinder->flag_Hollow_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CYL_RING)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, m_Cylinder->flag_Ring_Flag_Dlg);
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
			m_Cylinder->Update();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKWORLDCENTRE)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			m_Cylinder->m_UseCamPos = 0;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKCAMPOSITION)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			m_Cylinder->m_UseCamPos = 1;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CYL_SOLID)
		{
			m_Cylinder->m_Solid = 0;

			m_Cylinder->flag_Solid_Flag_Dlg = 1;
			m_Cylinder->flag_Hollow_Flag_Dlg = 0;
			m_Cylinder->flag_Ring_Flag_Dlg = 0;

			HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_THICKNESS);
			EnableWindow(temp, false);

			m_Cylinder->Update();

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CYL_HOLLOW)
		{
			m_Cylinder->m_Solid = 1;

			m_Cylinder->flag_Hollow_Flag_Dlg = 1;
			m_Cylinder->flag_Solid_Flag_Dlg = 0;
			m_Cylinder->flag_Ring_Flag_Dlg = 0;

			HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_THICKNESS);
			EnableWindow(temp, true);

			m_Cylinder->Update();

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CYL_RING)
		{
			m_Cylinder->m_Solid = 2;

			m_Cylinder->flag_Ring_Flag_Dlg = 1;
			m_Cylinder->flag_Hollow_Flag_Dlg = 0;
			m_Cylinder->flag_Solid_Flag_Dlg = 0;

			HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_THICKNESS);
			EnableWindow(temp, true);

			m_Cylinder->Update();

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CUTBRUSH)
		{
			if (m_Cylinder->m_TCut == 1)
			{
				m_Cylinder->m_TCut = 0;
			}
			else
			{
				m_Cylinder->m_TCut = 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CYL_DEFAULTS)
		{
			App->CL_App_Templates->Enable_Shape_Dialog(false);

			App->CL_Dialogs->YesNo("Reset to Defaults", "All Dimensions will be reset");
			if (App->CL_Dialogs->flag_Dlg_Canceled == false)
			{
				m_Cylinder->Set_Defaults(hDlg);

				HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_THICKNESS);
				EnableWindow(temp, false);

				m_Cylinder->Update();
			}

			App->CL_App_Templates->Enable_Shape_Dialog(true);
			App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HELP)
		{
			App->Open_HTML((LPSTR)"Help\\Create_Cylinder.html");
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			m_Cylinder->Get_DLG_Members(hDlg);
			m_Cylinder->Set_CylinderTemplate();
			m_Cylinder->CreateCylinder_New();

			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);

			App->CL_X_Shapes_3D->Close_OgreWindow();

			m_Cylinder->Remove_Edit_Boxes(hDlg);

			strcpy(App->CL_Properties_Templates->LastCreated_ShapeName, m_Cylinder->CylinderName);
			App->CL_Properties_Templates->Insert_Template();

			App->CL_App_Templates->Enable_Map_Editor_Dialogs(true);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);

			App->CL_X_Shapes_3D->Close_OgreWindow();

			m_Cylinder->Remove_Edit_Boxes(hDlg);

			App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();
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
void CreateCylDialog::Update(void)
{
	Get_DLG_Members(App->CL_App_Templates->Shape_Dlg_hWnd);
	Set_CylinderTemplate();
	CreateCylinder_New();
}

// *************************************************************************
// *		   CreateCylinder:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateCylDialog::CreateCylinder_New()
{
	App->CL_Doc->OnToolsTemplate();

	Brush* pCylinder;

	pCylinder = App->CL_X_BrushTemplate->BrushTemplate_CreateCylinder(pCylinderTemplate);
	if (pCylinder != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName, CylinderName);
		CreateNewTemplateBrush(pCylinder);
	}
	else
	{
		App->Say("Can not create cylinder");
	}
}

// *************************************************************************
// *       CreateNewTemplateBrush:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateCylDialog::CreateNewTemplateBrush(Brush* pBrush)
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
void CreateCylDialog::Set_Members()
{
	m_BotXOffset = pCylinderTemplate->BotXOffset;
	m_BotXSize = pCylinderTemplate->BotXSize;
	m_BotZOffset = pCylinderTemplate->BotZOffset;
	m_BotZSize = pCylinderTemplate->BotZSize;
	m_Solid = pCylinderTemplate->Solid;
	m_TopXOffset = pCylinderTemplate->TopXOffset;
	m_TopXSize = pCylinderTemplate->TopXSize;
	m_TopZOffset = pCylinderTemplate->TopZOffset;
	m_TopZSize = pCylinderTemplate->TopZSize;
	m_YSize = pCylinderTemplate->YSize;
	m_RingLength = pCylinderTemplate->RingLength;
	m_TCut = pCylinderTemplate->TCut;
	m_VerticalStripes = pCylinderTemplate->VerticalStripes;
	m_Thickness = pCylinderTemplate->Thickness;
}

// *************************************************************************
// *		 Set_DLG_Members:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateCylDialog::Set_DLG_Members(HWND hDlg)
{
	char buf[MAX_PATH];
	sprintf(buf, "%0.0f", m_TopXSize);
	SetDlgItemText(hDlg, IDC_TOPXSIZE, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_TopZSize);
	SetDlgItemText(hDlg, IDC_TOPZSIZE, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_TopXOffset);
	SetDlgItemText(hDlg, IDC_TOPXOFF, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_TopZOffset);
	SetDlgItemText(hDlg, IDC_TOPZOFF, (LPCTSTR)buf);


	sprintf(buf, "%0.0f", m_BotXSize);
	SetDlgItemText(hDlg, IDC_BOTXSIZE, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_BotZSize);
	SetDlgItemText(hDlg, IDC_BOTZSIZE, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_BotXOffset);
	SetDlgItemText(hDlg, IDC_BOTXOFF, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_BotZOffset);
	SetDlgItemText(hDlg, IDC_BOTZOFF, (LPCTSTR)buf);


	sprintf(buf, "%0.0f", m_YSize);
	SetDlgItemText(hDlg, IDC_YSIZE, (LPCTSTR)buf);

	sprintf(buf, "%i", m_VerticalStripes);
	SetDlgItemText(hDlg, IDC_VERTSTRIPES, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Thickness);
	SetDlgItemText(hDlg, IDC_THICKNESS, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_RingLength);
	SetDlgItemText(hDlg, IDC_RINGLENGTH, (LPCTSTR)buf);


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
void CreateCylDialog::Get_DLG_Members(HWND hDlg)
{
	char buf[MAX_PATH];

	GetDlgItemText(hDlg, IDC_TOPXSIZE, (LPTSTR)buf, MAX_PATH);
	m_TopXSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_TOPZSIZE, (LPTSTR)buf, MAX_PATH);
	m_TopZSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_TOPXOFF, (LPTSTR)buf, MAX_PATH);
	m_TopXOffset = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_TOPZOFF, (LPTSTR)buf, MAX_PATH);
	m_TopZOffset = (float)atof(buf);


	GetDlgItemText(hDlg, IDC_BOTXSIZE, (LPTSTR)buf, MAX_PATH);
	m_BotXSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_BOTZSIZE, (LPTSTR)buf, MAX_PATH);
	m_BotZSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_BOTXOFF, (LPTSTR)buf, MAX_PATH);
	m_BotXOffset = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_BOTZOFF, (LPTSTR)buf, MAX_PATH);
	m_BotZOffset = (float)atof(buf);


	GetDlgItemText(hDlg, IDC_YSIZE, (LPTSTR)buf, MAX_PATH);
	m_YSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_VERTSTRIPES, (LPTSTR)buf, MAX_PATH);
	m_VerticalStripes = (int)atoi(buf);

	GetDlgItemText(hDlg, IDC_THICKNESS, (LPTSTR)buf, MAX_PATH);
	m_Thickness = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_RINGLENGTH, (LPTSTR)buf, MAX_PATH);
	m_RingLength = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_EDITNAME, (LPTSTR)buf, MAX_PATH);
	strcpy(CylinderName, buf);
}

// *************************************************************************
// *		 Set_CylinderTemplate:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateCylDialog::Set_CylinderTemplate()
{
	pCylinderTemplate->BotXOffset = m_BotXOffset;
	pCylinderTemplate->BotXSize = m_BotXSize;
	pCylinderTemplate->BotZOffset = m_BotZOffset;
	pCylinderTemplate->BotZSize = m_BotZSize;
	pCylinderTemplate->Solid = m_Solid;
	pCylinderTemplate->TopXOffset = m_TopXOffset;
	pCylinderTemplate->TopXSize = m_TopXSize;
	pCylinderTemplate->TopZOffset = m_TopZOffset;
	pCylinderTemplate->TopZSize = m_TopZSize;
	pCylinderTemplate->YSize = m_YSize;
	pCylinderTemplate->RingLength = m_RingLength;
	pCylinderTemplate->TCut = m_TCut;
	pCylinderTemplate->VerticalStripes = m_VerticalStripes;
	pCylinderTemplate->Thickness = m_Thickness;
}


// *************************************************************************
// *	    	Set_Defaults:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateCylDialog::Set_Defaults(HWND hDlg)
{
	m_BotXOffset = 0.0;
	m_BotXSize = 128.0;
	m_BotZOffset = 0.0;
	m_BotZSize = 128.0;
	m_Solid = 0;
	m_TopXOffset = 0.0;
	m_TopXSize = 128.0;
	m_TopZOffset = 0.0;
	m_TopZSize = 128.0;
	m_YSize = 200.0;
	m_RingLength = 0.0;
	m_TCut = false;
	m_VerticalStripes = 6;
	m_Thickness = 16.0f;

	Set_DLG_Members(hDlg);

	flag_Solid_Flag_Dlg = 1;
	flag_Hollow_Flag_Dlg = 0;
	flag_Ring_Flag_Dlg = 0;

	HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_THICKNESS);
	EnableWindow(temp, false);

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
