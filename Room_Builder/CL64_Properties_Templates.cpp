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
// *	  	Enable_Insert_Button:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Templates::Enable_Insert_Button(bool Enable)
{
	EnableWindow(GetDlgItem(TemplatesDlg_Hwnd, IDC_BTINSERT), Enable);

	flag_Insert_Enabled = Enable;

	RedrawWindow(TemplatesDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Show_TemplatesDialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Templates::Show_TemplatesDialog(bool Show)
{
	ShowWindow(TemplatesDlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Start_TemplatesDialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Templates::Start_TemplatesDialog()
{

	TemplatesDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEMPLATES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_Templates);
	Set_Icons();

	Enable_Insert_Button(true);

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
		SendDlgItemMessage(hDlg, IDC_BTINSERT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
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

		if (some_item->idFrom == IDC_BTINSERT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BTINSERT));
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

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BTINSERT)
		{
			App->CL_Doc->mModeTool = ID_TOOLS_TEMPLATE;

			if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
			{

				if (App->CL_Brush->Get_Brush_Count() == 0) // New Scene
				{
					App->CL_Project->flag_Is_New_Project = true;
					App->CL_Doc->AddBrushToWorld();
					
					// Scene
					App->CL_Entities->Create_Player_Brush();
					
					//if (App->CL_Level->Level_Version == 1.0)
					{

						App->CL_Com_Environments->Create_Test_Environment();
						App->CL_Com_Objects->Show_Entities(false);
						App->CL_Gizmos->Show_MarkerBox(false);
					}
					
					// ----------------

					App->CL_Ogre->Camera_Reset_Zero();
					App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
					
					App->CL_Doc->Editor_Set_Dlgs(Enums::Editor_Dlgs_First_Brush);
					
					App->Set_Title(App->CL_Level->MTF_PathAndFile);
					App->CL_Level->flag_Level_is_Modified = true;

					// Set for new level
					strcpy(App->CL_Project->m_Project_Name, "New_Room");
					strcpy(App->CL_Project->m_Level_Name, "New_Level");

					App->CL_FileView->Change_Level_Name();
					App->CL_FileView->Change_Project_Name();

					App->CL_Level->flag_File_Been_Saved = 0;
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

			App->CL_Properties_Templates->Enable_Insert_Button(false);
			App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(false);
			return 1;
		}

		if (LOWORD(wParam) == IDC_BRUSH_CUBE_PRIMITIVE)
		{
			App->CL_CreateBoxDialog->Start_CreateBox_Dlg();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BRUSH_CYCLINDER_PRIMITIVE)
		{
			App->CL_CreateCylDialog->Start_CreateCyl_Dlg();
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BRUSH_CONE_PRIMITIVE)
		{
			App->CL_CreateConeDialog->Start_CreateCone_Dlg();
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BRUSH_STAIRCASE_PRIMITIVE)
		{
			App->CL_CreateStaircaseDialog->Start_CreateStaircase_Dlg();
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BRUSH_ARCH_PRIMITIVE)
		{
			App->CL_CreateArchDialog->Start_CreateArch_Dlg();
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

