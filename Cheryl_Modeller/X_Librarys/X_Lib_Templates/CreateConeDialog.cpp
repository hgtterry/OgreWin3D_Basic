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
#include "CreateConeDialog.h"

CreateConeDialog::CreateConeDialog(void)
{
	m_Style = 0;
	m_Width = 200;
	m_Height = 300;
	m_VerticalStrips = 4;
	m_Thickness = 16;
	m_TCut = false;

	m_UseCamPos = 0;

	pConeTemplate = NULL;

	strcpy(ConeName, "Cone");

	flag_Solid_Flag_Dlg = 1;
	flag_Hollow_Flag_Dlg = 0;
	flag_Funnel_Flag_Dlg = 0;
}

CreateConeDialog::~CreateConeDialog(void)
{
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* CreateConeDialog::GetVersion()
{
	return (LPSTR)"Create Cone [ 08-10-25 ] Build 1 ";
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CreateConeDialog::Init_Bmps_Globals(HWND hDlg)
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

LRESULT CALLBACK CreateConeDialog::OwnerEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		switch (wParam)
		{

		case VK_RETURN:
		{
			App->CL_Libs->CL_CreateCone->Update();
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
void CreateConeDialog::Capture_Edit_Boxes(HWND hDlg)
{
	// Array of control IDs to be processed
	const int controlIDs[] = { IDC_ED_CONE_1, IDC_ED_CONE_2, IDC_ED_CONE_3, IDC_ED_CONE_4 };

	// Loop through each control ID and set the window subclass
	for (int id : controlIDs) {
		HWND control = GetDlgItem(hDlg, id); // Retrieve the handle for the control
		SetWindowSubclass(control, OwnerEditProc, 0, 0); // Set the subclass procedure for the control
	}
}

// *************************************************************************
// *		Remove_Edit_Boxes:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CreateConeDialog::Remove_Edit_Boxes(HWND hDlg)
{
	// Array of control IDs to remove the subclass from
	const int controlIDs[] = { IDC_ED_CONE_1, IDC_ED_CONE_2, IDC_ED_CONE_3, IDC_ED_CONE_4 };

	// Iterate through each control ID and remove the subclass
	for (int id : controlIDs)
	{
		HWND control = GetDlgItem(hDlg, id);
		RemoveWindowSubclass(control, OwnerEditProc, 0);
	}
}

// *************************************************************************
// *	  	Start_CreateCone_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateConeDialog::Start_CreateCone_Dlg()
{
	App->CL_App_Templates->Shape_Dlg_hWnd = nullptr;

	pConeTemplate = App->CL_Level->Level_GetConeTemplate();

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	CreateDialog(App->hInst, (LPCTSTR)IDD_CREATE_CONE, App->MainHwnd, (DLGPROC)Proc_CreateCone);

	if (App->CL_App_Templates->Shape_Dlg_hWnd)
	{
		App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);
	}

	Get_DLG_Members(App->CL_App_Templates->Shape_Dlg_hWnd);
	Set_ConeTemplate();
	CreateCone();

	App->CL_Ogre->OGL_Listener->Show_Visuals(true);

}

// *************************************************************************
// *        Proc_CreateCone:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
LRESULT CALLBACK CreateConeDialog::Proc_CreateCone(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& m_Cone = App->CL_Libs->CL_CreateCone; // App->CL_App_Templates->CL_CreateCone

	switch (message)
	{
	case WM_INITDIALOG:
	{
		m_Cone->Capture_Edit_Boxes(hDlg);

		SendDlgItemMessage(hDlg, IDC_STDIAMETER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STYSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STVERTSTRIPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STWALLSIZE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ED_CONE_1, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_CONE_2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_CONE_3, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_CONE_4, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_CONE_SOLID, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CONE_HOLLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CONE_FUNNEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CONE_CUTBRUSH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_CONE_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_CONE_POSITION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CK_CONE_WORLDCENTRE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_CONE_CAMPOSITION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ED_CONE_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_CONE_DEFAULTS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_UPDATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_App_Templates->Shape_Dlg_hWnd = hDlg;

		App->CL_X_Shapes_3D->Start_Zoom = 600;
		App->CL_X_Shapes_3D->Render_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_BOX_3D, hDlg, (DLGPROC)App->CL_X_Shapes_3D->Proc_Box_Viewer_3D);
		App->CL_X_Shapes_3D->Set_OgreWindow();

		m_Cone->Set_Members();
		m_Cone->Set_DLG_Members(hDlg);

		int Count = App->CL_X_Brush->Get_Brush_Count();
		char Num[32];
		char Name[32];
		_itoa(Count + 1, Num, 10);
		strcpy(Name, "Cone_");
		strcat(Name, Num);

		SetDlgItemText(hDlg, IDC_ED_CONE_NAME, (LPCTSTR)Name);

		m_Cone->Init_Bmps_Globals(hDlg);

		// ----------- Style Solid Hollow Funnel
		if (m_Cone->m_Style == 0)
		{
			m_Cone->flag_Solid_Flag_Dlg = 1;
			m_Cone->flag_Hollow_Flag_Dlg = 0;
			m_Cone->flag_Funnel_Flag_Dlg = 0;
		}

		if (m_Cone->m_Style == 1)
		{
			m_Cone->flag_Solid_Flag_Dlg = 0;
			m_Cone->flag_Hollow_Flag_Dlg = 1;
			m_Cone->flag_Funnel_Flag_Dlg = 0;
		}

		if (m_Cone->m_Style == 2)
		{
			m_Cone->flag_Solid_Flag_Dlg = 0;
			m_Cone->flag_Hollow_Flag_Dlg = 0;
			m_Cone->flag_Funnel_Flag_Dlg = 1;
		}

		// ----------------------------------------------
		HWND temp = GetDlgItem(hDlg, IDC_CK_CONE_WORLDCENTRE);
		SendMessage(temp, BM_SETCHECK, 1, 0);
		m_Cone->m_UseCamPos = 0;

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

		if (GetDlgItem(hDlg, IDC_STDIAMETER) == (HWND)lParam)
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

		if (GetDlgItem(hDlg, IDC_STVERTSTRIPS) == (HWND)lParam)
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

		// --------------------------------------------------
		if (GetDlgItem(hDlg, IDC_ST_CONE_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_CONE_POSITION) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_CONE_WORLDCENTRE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_CONE_CAMPOSITION) == (HWND)lParam)
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

		if (some_item->idFrom == IDC_BT_CONE_SOLID)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, m_Cone->flag_Solid_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CONE_HOLLOW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, m_Cone->flag_Hollow_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CONE_FUNNEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, m_Cone->flag_Funnel_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CONE_CUTBRUSH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, m_Cone->m_TCut);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CONE_DEFAULTS)
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
			m_Cone->Update();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_CONE_WORLDCENTRE)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_CONE_WORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CK_CONE_CAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			m_Cone->m_UseCamPos = 0;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_CONE_CAMPOSITION)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_CONE_CAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CK_CONE_WORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			m_Cone->m_UseCamPos = 1;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CONE_SOLID)
		{
			m_Cone->m_Style = 0;

			m_Cone->flag_Solid_Flag_Dlg = 1;
			m_Cone->flag_Hollow_Flag_Dlg = 0;
			m_Cone->flag_Funnel_Flag_Dlg = 0;

			m_Cone->Update();

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CONE_HOLLOW)
		{
			m_Cone->m_Style = 1;

			m_Cone->flag_Solid_Flag_Dlg = 0;
			m_Cone->flag_Hollow_Flag_Dlg = 1;
			m_Cone->flag_Funnel_Flag_Dlg = 0;

			m_Cone->Update();

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CONE_FUNNEL)
		{
			m_Cone->m_Style = 2;

			m_Cone->flag_Solid_Flag_Dlg = 0;
			m_Cone->flag_Hollow_Flag_Dlg = 0;
			m_Cone->flag_Funnel_Flag_Dlg = 1;

			m_Cone->Update();

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CONE_CUTBRUSH)
		{
			if (m_Cone->m_TCut == 1)
			{
				m_Cone->m_TCut = 0;
				return 1;
			}
			else
			{
				m_Cone->m_TCut = 1;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_CONE_DEFAULTS)
		{
			App->CL_App_Templates->Enable_Shape_Dialog(false);

			App->CL_Dialogs->YesNo("Reset to Defaults", "All Dimensions will be reset");
			if (App->CL_Dialogs->flag_Dlg_Canceled == false)
			{
				m_Cone->Set_Defaults(hDlg);
				m_Cone->Update();
			}

			App->CL_App_Templates->Enable_Shape_Dialog(true);
			App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HELP)
		{
			App->Open_HTML((LPSTR)"Help\\Create_Cone.html");
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			m_Cone->Get_DLG_Members(hDlg);
			m_Cone->Set_ConeTemplate();
			m_Cone->CreateCone();

			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);

			App->CL_X_Shapes_3D->Close_OgreWindow();

			m_Cone->Remove_Edit_Boxes(hDlg);

			strcpy(App->CL_Properties_Templates->LastCreated_ShapeName, m_Cone->ConeName);
			App->CL_Properties_Templates->Insert_Template();

			App->CL_App_Templates->Enable_Map_Editor_Dialogs(true);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_X_Shapes_3D->Close_OgreWindow();

			m_Cone->Remove_Edit_Boxes(hDlg);

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
void CreateConeDialog::Update(void)
{
	Get_DLG_Members(App->CL_App_Templates->Shape_Dlg_hWnd);
	Set_ConeTemplate();
	CreateCone();
}

// *************************************************************************
// *		   CreateCone:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void CreateConeDialog::CreateCone()
{
	App->CL_Doc->OnToolsTemplate();

	Brush* pCone;
	pCone = App->CL_X_BrushTemplate->BrushTemplate_CreateCone(pConeTemplate);
	if (pCone != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName, ConeName);
		CreateNewTemplateBrush(pCone);
	}
	else
	{
		App->Say("No Cone");
	}
}

// *************************************************************************
// *       CreateNewTemplateBrush:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CreateConeDialog::CreateNewTemplateBrush(Brush* pBrush)
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
void CreateConeDialog::Set_Members()
{
	m_Style = pConeTemplate->Style;
	m_Width = pConeTemplate->Width;
	m_Height = pConeTemplate->Height;
	m_VerticalStrips = pConeTemplate->VerticalStrips;
	m_Thickness = pConeTemplate->Thickness;
	m_TCut = pConeTemplate->TCut;
}

// *************************************************************************
// *		 Set_DLG_Members:- Terry and Hazel Flanigan 205			   *
// *************************************************************************
void CreateConeDialog::Set_DLG_Members(HWND hDlg)
{
	char buf[MAX_PATH];
	sprintf(buf, "%0.0f", m_Width);
	SetDlgItemText(hDlg, IDC_ED_CONE_1, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Height);
	SetDlgItemText(hDlg, IDC_ED_CONE_2, (LPCTSTR)buf);

	sprintf(buf, "%0.0i", m_VerticalStrips);
	SetDlgItemText(hDlg, IDC_ED_CONE_3, (LPCTSTR)buf);

	sprintf(buf, "%0.0f", m_Thickness);
	SetDlgItemText(hDlg, IDC_ED_CONE_4, (LPCTSTR)buf);


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
void CreateConeDialog::Get_DLG_Members(HWND hDlg)
{
	char buf[MAX_PATH];

	GetDlgItemText(hDlg, IDC_ED_CONE_1, (LPTSTR)buf, MAX_PATH);
	m_Width = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ED_CONE_2, (LPTSTR)buf, MAX_PATH);
	m_Height = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ED_CONE_3, (LPTSTR)buf, MAX_PATH);
	m_VerticalStrips = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ED_CONE_4, (LPTSTR)buf, MAX_PATH);
	m_Thickness = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ED_CONE_NAME, (LPTSTR)buf, MAX_PATH);
	strcpy(ConeName, buf);
}

// *************************************************************************
// *		 Set_ConeTemplate:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CreateConeDialog::Set_ConeTemplate()
{
	pConeTemplate->Style = m_Style;
	pConeTemplate->Width = m_Width;
	pConeTemplate->Height = m_Height;
	pConeTemplate->VerticalStrips = m_VerticalStrips;
	pConeTemplate->Thickness = m_Thickness;
	pConeTemplate->TCut = m_TCut;
}

// *************************************************************************
// *	    	Set_Defaults:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CreateConeDialog::Set_Defaults(HWND hDlg)
{
	m_Style = 0;
	m_Width = 200;
	m_Height = 300;
	m_VerticalStrips = 4;
	m_Thickness = 16;
	m_TCut = false;

	Set_DLG_Members(hDlg);

	HWND temp = GetDlgItem(hDlg, IDC_CK_CONE_WORLDCENTRE);
	SendMessage(temp, BM_SETCHECK, 1, 0);
	m_UseCamPos = 0;

	flag_Solid_Flag_Dlg = 1;
	flag_Hollow_Flag_Dlg = 0;
	flag_Funnel_Flag_Dlg = 0;

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
