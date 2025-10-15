/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
#include "CL64_Properties_Templates.h"

CL64_Properties_Templates::CL64_Properties_Templates()
{
	TemplatesDlg_Hwnd = nullptr;

	flag_Insert_Enabled = 0;
}

CL64_Properties_Templates::~CL64_Properties_Templates()
{
}

// *************************************************************************
// *	  	Show_TemplatesDialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Templates::Show_TemplatesDialog(bool Show)
{
	ShowWindow(TemplatesDlg_Hwnd, Show);
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Properties_Templates::Init_Bmps_Globals(void)
{
	/*HWND Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BT_HELP);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_Help_Bmp);*/

	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOFADE, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);
	SendMessage(hTooltip_TB_2, TTM_SETMAXTIPWIDTH, 0, 250);

	/*Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BT_HELP);
	TOOLINFO ti1 = { 0 };
	ti1.cbSize = sizeof(ti1);
	ti1.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti1.uId = (UINT_PTR)Temp;
	ti1.lpszText = (LPSTR)"Help / Information.";
	ti1.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti1);*/

	HWND Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_CUBE_PRIMITIVE);
	TOOLINFO ti2 = { 0 };
	ti2.cbSize = sizeof(ti2);
	ti2.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti2.uId = (UINT_PTR)Temp;
	ti2.lpszText = (LPSTR)"Create Room/Cube Shape";
	ti2.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti2);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_STAIRCASE_PRIMITIVE);
	TOOLINFO ti3 = { 0 };
	ti3.cbSize = sizeof(ti3);
	ti3.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti3.uId = (UINT_PTR)Temp;
	ti3.lpszText = (LPSTR)"Create Staircase or Slope Shape";
	ti3.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti3);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_CYCLINDER_PRIMITIVE);
	TOOLINFO ti4 = { 0 };
	ti4.cbSize = sizeof(ti4);
	ti4.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti4.uId = (UINT_PTR)Temp;
	ti4.lpszText = (LPSTR)"Create Cylinder Shape";
	ti4.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti4);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_ARCH_PRIMITIVE);
	TOOLINFO ti5 = { 0 };
	ti5.cbSize = sizeof(ti5);
	ti5.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti5.uId = (UINT_PTR)Temp;
	ti5.lpszText = (LPSTR)"Create Arch Shape";
	ti5.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti5);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_CONE_PRIMITIVE);
	TOOLINFO ti6 = { 0 };
	ti6.cbSize = sizeof(ti6);
	ti6.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti6.uId = (UINT_PTR)Temp;
	ti6.lpszText = (LPSTR)"Create Cone Shape";
	ti6.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti6);

}

// *************************************************************************
// *	  	Start_TemplatesDialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Templates::Start_TemplatesDialog()
{
	TemplatesDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEMPLATES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_Templates);
	
	Set_Icons();
	Init_Bmps_Globals();
}

// *************************************************************************
// *        TemplatesDialog_Proc:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Templates::Proc_Templates(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TEMPLATES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_TEMPLATES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
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

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BRUSH_CUBE_PRIMITIVE)
		{
			App->CL_X_CreateBoxDialog->Start_CreateBox_Dlg();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BRUSH_CYCLINDER_PRIMITIVE)
		{
			App->CL_X_CreateCylDialog->Start_CreateCyl_Dlg();
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BRUSH_CONE_PRIMITIVE)
		{
			App->CL_X_CreateConeDialog->Start_CreateCone_Dlg();
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BRUSH_STAIRCASE_PRIMITIVE)
		{
			App->CL_X_CreateStaircaseDialog->Start_CreateStaircase_Dlg();
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BRUSH_ARCH_PRIMITIVE)
		{
			App->CL_X_CreateArchDialog->Start_CreateArch_Dlg();
			return 1;
		}
		
		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			//App->CL_TextureDialog->f_TextureDlg_Active = 0;
			//EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			//App->CL_TextureDialog->f_TextureDlg_Active = 0;
			//EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}

	return FALSE;
}

// *************************************************************************
// *	  	Insert_Template:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Properties_Templates::Insert_Template()
{
	App->CL_Doc->mModeTool = ID_TOOLS_TEMPLATE;

	if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
	{
		// Check if it's a new scene
		if (App->CL_X_Brush->Get_Brush_Count() == 0) // New Scene
		{
			App->CL_Project->flag_Is_New_Project = true;
			App->CL_Doc->AddBrushToWorld();

			// Create Player if none exists
			if (App->CL_Scene->Player_Count == 0)
			{
				App->CL_Com_Player->Create_New_Player("Main_Player", false);
			}

			// Create the first location if none exists
			if (App->CL_Locations->Location_Count == 0)
			{
				App->CL_Locations->Add_New_Location(true);
			}

			// Create first Envoronment
			App->CL_Com_Environments->Create_Test_Environment();

			App->CL_Gizmos->Show_MarkerBox(false);

			// Reset camera settings
			App->CL_Ogre->Camera_Reset_Zero();
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

			App->CL_Doc->Editor_Set_Dlgs(Enums::Editor_Dlgs_First_Brush);

			App->Set_Title(App->CL_Level->MTF_PathAndFile);
			App->CL_Level->flag_Level_is_Modified = true;

			// Set new level and project names
			strcpy(App->CL_Project->m_Project_Name, "New_Room");
			strcpy(App->CL_Project->m_Level_Name, "New_Level");

			// Update file view with new names
			App->CL_FileView->Change_Level_Name();
			App->CL_FileView->Change_Project_Name();

			App->CL_Editor_Control->Set_Map_Editor_Startup();

			App->CL_Level->flag_File_Been_Saved = false;
		}
		else
		{
			App->CL_Doc->AddBrushToWorld();
			App->CL_Level->flag_Level_is_Modified = true;
		}
	}

	App->CL_Doc->Set_Faces_To_Brush_Name_All();

	App->CL_Doc->Do_General_Select_Dlg(true);

	App->CL_Doc->mCurrentTool = CURTOOL_NONE;
	App->CL_Doc->mModeTool = ID_GENERALSELECT;

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);

	App->CL_Properties_Brushes->Fill_ListBox();

	App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(false);

}

// *************************************************************************
// *	  		Set_Icons:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Properties_Templates::Set_Icons()
{
	HWND Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_CUBE_PRIMITIVE);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)App->Hnd_Box_Icon);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_CYCLINDER_PRIMITIVE);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)App->Hnd_Cyl_Icon);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_CONE_PRIMITIVE);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)App->Hnd_Cone_Icon);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_SPHEROID_PRIMITIVE);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)App->Hnd_Spheriod_Icon);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_STAIRCASE_PRIMITIVE);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)App->Hnd_Stairs_Icon);

	Temp = GetDlgItem(TemplatesDlg_Hwnd, IDC_BRUSH_ARCH_PRIMITIVE);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)App->Hnd_Arch_Icon);

}

