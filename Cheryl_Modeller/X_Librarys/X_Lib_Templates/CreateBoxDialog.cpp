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
#include "CreateBoxDialog.h"

CreateBoxDialog::CreateBoxDialog(void)
{
	m_YSize = 128.0f;
	m_Solid = 0;
	m_XSizeBot = 128.0f;
	m_XSizeTop = 128.0f;
	m_ZSizeBot = 128.0f;
	m_ZSizeTop = 128.0f;
	m_TCut = false;
	m_Thickness = 16.0f;
	m_TSheet = false;

	pBoxTemplate = NULL;
	Test_Cube = nullptr;

	m_UseCamPos = 0;

	Solid_Flag = 1;
	Hollow_Flag = 0;
	Cut_Flag = 0;

	flag_Default = 1;
	flag_Room = 0;

	strcpy(BoxName, "Box");

}

CreateBoxDialog::~CreateBoxDialog(void)
{
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* CreateBoxDialog::GetVersion()
{
	return (LPSTR)"Create Box [ 30-12-25 ] Build 1 ";
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CreateBoxDialog::Init_Bmps_Globals(HWND hDlg)
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

LRESULT CALLBACK CreateBoxDialog::OwnerEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		switch (wParam)
		{

		case VK_RETURN:
		{
			App->CL_Libs->CL_CreateBox->Update();
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
void CreateBoxDialog::Capture_Edit_Boxes(HWND hDlg)
{
	// Array of control IDs to be processed
	const int controlIDs[] = { IDC_XSIZETOP, IDC_ZSIZETOP, IDC_XSIZEBOT, IDC_ZSIZEBOT, IDC_YSIZE, IDC_THICKNESS };

	// Loop through each control ID and set the window subclass
	for (int id : controlIDs) {
		HWND control = GetDlgItem(hDlg, id); // Retrieve the handle for the control
		SetWindowSubclass(control, OwnerEditProc, 0, 0); // Set the subclass procedure for the control
	}
}

// *************************************************************************
// *		Remove_Edit_Boxes:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CreateBoxDialog::Remove_Edit_Boxes(HWND hDlg)
{
	// Array of control IDs to remove the subclass from
	const int controlIDs[] = { IDC_XSIZETOP, IDC_ZSIZETOP, IDC_XSIZEBOT, IDC_ZSIZEBOT };

	// Iterate through each control ID and remove the subclass
	for (int id : controlIDs)
	{
		HWND control = GetDlgItem(hDlg, id);
		RemoveWindowSubclass(control, OwnerEditProc, 0);
	}
}

// *************************************************************************
// *	  	Start_CreateBox_Dlg:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateBoxDialog::Start_CreateBox_Dlg()
{
	App->CL_App_Templates->Shape_Dlg_hWnd = nullptr;

	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	CreateDialog(App->hInst, (LPCTSTR)IDD_CREATE_BOX, App->MainHwnd, (DLGPROC)Proc_CreateBox);

	if (App->CL_App_Templates->Shape_Dlg_hWnd)
	{
		App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);
	}

	Get_Dialog_Members(App->CL_App_Templates->Shape_Dlg_hWnd);
	Set_BoxTemplate();
	CreateCube();

	App->CL_Ogre->OGL_Listener->Show_Visuals(true);
}

// *************************************************************************
// *		  Proc_CreateBox:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CreateBoxDialog::Proc_CreateBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& m_Box = App->CL_Libs->CL_CreateBox; // App->CL_App_Templates->CL_CreateBox

	switch (message)
	{

	case WM_INITDIALOG:
	{
		m_Box->Capture_Edit_Boxes(hDlg);
		
		// Array of control IDs to set font to Font_CB15
		const int controlIDs[] =
		{
			IDC_STBOTTOM, IDC_STBOTX, IDC_STBOTZ, IDC_XSIZEBOT, IDC_ZSIZEBOT,
			IDC_STTOP, IDC_STTOPX, IDC_STZTOP, IDC_XSIZETOP, IDC_ZSIZETOP,
			IDC_BT_BOXSOLID, IDC_BT_BOXHOLLOW, IDC_BT_BOXCUTBRUSH,
			IDC_STGENERAL, IDC_STYSIZE, IDC_STWALL, IDC_YSIZE, IDC_THICKNESS,
			IDC_STNAME, IDC_EDITNAME,
			IDC_BOXDEFAULTS, IDC_BT_BOXROOM,
			IDC_STCAMPOS, IDC_CKWORLDCENTRE, IDC_CKCAMPOSITION,
			IDOK, IDCANCEL,IDC_BT_UPDATE
		};

		// Set font for each control
		for (int id : controlIDs) {
			SendDlgItemMessage(hDlg, id, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		}

		App->CL_App_Templates->Shape_Dlg_hWnd = hDlg;

		App->CL_X_Shapes_3D->Start_Zoom = 400;
		App->CL_X_Shapes_3D->Render_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_BOX_3D, hDlg, (DLGPROC)App->CL_X_Shapes_3D->Proc_Box_Viewer_3D);
		App->CL_X_Shapes_3D->Set_OgreWindow();

		// Initialize dialog members
		m_Box->SetMembers();
		m_Box->Set_Dialog_Members(hDlg);
		m_Box->SetDefaults(hDlg);

		// Generate and set the box name
		int Count = App->CL_X_Brush->Get_Brush_Count();
		char Name[32];
		snprintf(Name, sizeof(Name), "Box_%d", Count+1);
		SetDlgItemText(hDlg, IDC_EDITNAME, Name);

		// Set checkbox state
		HWND Temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
		SendMessage(Temp, BM_SETCHECK, 1, 0);
		m_Box->m_UseCamPos = 0;

		m_Box->Init_Bmps_Globals(hDlg);

		if (App->CL_X_Brush->Get_Brush_Count() == 0) // New Scene
		{
			m_Box->flag_Default = 0;
			m_Box->flag_Room = 1;

			EnableWindow(GetDlgItem(hDlg, IDC_BOXDEFAULTS), false);
			EnableWindow(GetDlgItem(hDlg, IDC_BT_BOXCUTBRUSH), false);
			EnableWindow(GetDlgItem(hDlg, IDC_BT_BOXSOLID), false);

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			int brushCount = App->CL_X_Brush->Get_Brush_Count();
			std::string boxName = "Room_" + std::to_string(brushCount);

			SetDlgItemText(hDlg, IDC_EDITNAME, boxName.c_str());

			m_Box->SetRoom(hDlg);
		}

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		const int controlIds[] =
		{
			IDC_STTOP, IDC_STBOTTOM, IDC_STTOPX, IDC_STWALL,
			IDC_STGENERAL, IDC_STZTOP, IDC_STBOTX, IDC_STBOTZ,
			IDC_STYSIZE, IDC_STNAME, IDC_STCAMPOS,
			IDC_CKWORLDCENTRE, IDC_CKCAMPOSITION
		};

		for (const auto& id : controlIds)
		{
			if (GetDlgItem(hDlg, id) == (HWND)lParam)
			{
				SetBkColor((HDC)wParam, RGB(0, 0, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
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
		LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

		switch (some_item->idFrom)
		{
		case IDC_BT_BOXSOLID:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BOXSOLID));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, m_Box->Solid_Flag);
			}
			break;
		}

		case IDC_BT_BOXHOLLOW:
			App->Custom_Button_Toggle_Tabs(item, m_Box->Hollow_Flag);
			break;

		case IDC_BT_BOXCUTBRUSH:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BOXCUTBRUSH));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, m_Box->Cut_Flag);
			}
			break;
		}

		case IDC_BOXDEFAULTS:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BOXDEFAULTS));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, m_Box->flag_Default);
			}
			break;
		}

		case IDC_BT_BOXROOM:
			App->Custom_Button_Toggle_Tabs(item, m_Box->flag_Room);
			break;

		case IDC_BT_UPDATE:
			App->Custom_Button_Normal(item);
			break;
			
		case IDOK:
		case IDCANCEL:
			App->Custom_Button_Normal(item);
			break;

		default:
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
			m_Box->Update();
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_BOXSOLID)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
			SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_SolidBox_Bmp);

			m_Box->Zero_Dlg_Flags(hDlg);
			m_Box->m_Solid = 0;
			m_Box->Solid_Flag = 1;

			HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_THICKNESS);
			EnableWindow(temp, false);

			int Count = App->CL_X_Brush->Get_Brush_Count();
			char Name[32];
			snprintf(Name, sizeof(Name), "Box_%d", Count + 1);

			m_Box->Update();

			SetDlgItemText(hDlg, IDC_EDITNAME, (LPTSTR)Name);

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXHOLLOW)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
			SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HollowBox_Bmp);

			m_Box->Zero_Dlg_Flags(hDlg);
			m_Box->m_Solid = 1;
			m_Box->Hollow_Flag = 1;

			HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_THICKNESS);
			EnableWindow(temp, true);

			int Count = App->CL_X_Brush->Get_Brush_Count();
			char Name[32];
			snprintf(Name, sizeof(Name), "Room_%d", Count + 1);

			m_Box->Update();

			SetDlgItemText(hDlg, IDC_EDITNAME, (LPTSTR)Name);

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXCUTBRUSH)
		{
			m_Box->m_TCut = !m_Box->Cut_Flag;
			m_Box->Cut_Flag = !m_Box->Cut_Flag;

			int Count = App->CL_X_Brush->Get_Brush_Count();
			char Name[32];
			snprintf(Name, sizeof(Name), "Box_%d%s", Count+1, m_Box->Cut_Flag ? "_Cut" : "");

			m_Box->Update();

			SetDlgItemText(hDlg, IDC_EDITNAME, (LPTSTR)Name);
			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}

		if (LOWORD(wParam) == IDC_CKWORLDCENTRE)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			m_Box->m_UseCamPos = 0;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKCAMPOSITION)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			m_Box->m_UseCamPos = 1;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BOXDEFAULTS)
		{
			App->CL_App_Templates->Enable_Shape_Dialog(false);

			App->CL_Dialogs->YesNo("Reset to Defaults", "All Dimensions will be reset");
			if (App->CL_Dialogs->flag_Dlg_Canceled == false)
			{
				m_Box->flag_Default = 1;
				m_Box->flag_Room = 0;
				RedrawWindow(hDlg, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

				int brushCount = App->CL_X_Brush->Get_Brush_Count();
				std::string name = "Box_" + std::to_string(brushCount + 1);

				SetDlgItemText(hDlg, IDC_EDITNAME, name.c_str());

				m_Box->SetDefaults(hDlg);

				m_Box->Update();

				HWND temp = GetDlgItem(App->CL_App_Templates->Shape_Dlg_hWnd, IDC_THICKNESS);
				EnableWindow(temp, false);
			}

			App->CL_App_Templates->Enable_Shape_Dialog(true);
			App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXROOM)
		{
			App->CL_App_Templates->Enable_Shape_Dialog(false);

			App->CL_Dialogs->YesNo("Reset to Defaults", "All Dimensions will be reset");
			if (App->CL_Dialogs->flag_Dlg_Canceled == false)
			{
				m_Box->flag_Default = 0;
				m_Box->flag_Room = 1;
				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				int brushCount = App->CL_X_Brush->Get_Brush_Count();
				std::string boxName = "Box_" + std::to_string(brushCount + 1);

				SetDlgItemText(hDlg, IDC_EDITNAME, boxName.c_str());

				m_Box->SetRoom(hDlg);

				m_Box->Update();
			}

			App->CL_App_Templates->Enable_Shape_Dialog(true);
			App->CL_App_Templates->Enable_Map_Editor_Dialogs(false);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HELP)
		{
			App->Open_HTML((LPSTR)"Help\\Templates_Create_Cube.html");
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			m_Box->Get_Dialog_Members(hDlg);
			m_Box->Set_BoxTemplate();
			m_Box->CreateCube();

			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
		
			App->CL_X_Shapes_3D->Close_OgreWindow();

			m_Box->Remove_Edit_Boxes(hDlg);

			strcpy(App->CL_Properties_Templates->LastCreated_ShapeName, m_Box->BoxName);
			App->CL_Properties_Templates->Insert_Template();

			App->CL_App_Templates->Enable_Map_Editor_Dialogs(true);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;

		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_X_Shapes_3D->Close_OgreWindow();

			m_Box->Remove_Edit_Boxes(hDlg);

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
void CreateBoxDialog::Update(void)
{
	Get_Dialog_Members(App->CL_App_Templates->Shape_Dlg_hWnd);
	Set_BoxTemplate();
	CreateCube();
}		

// *************************************************************************
// *		   CreateCube:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CreateBoxDialog::CreateCube()
{

	App->CL_Doc->OnToolsTemplate();

	Brush* pCube;

	pCube = App->CL_X_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);
	if (pCube != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName, BoxName);
		strcpy(pCube->Name, "Test");
		CreateNewTemplateBrush(pCube);

	}
	else
	{
		App->Say("No pCube");
	}
}

// *************************************************************************
// *       CreateNewTemplateBrush:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateBoxDialog::CreateNewTemplateBrush(Brush* pBrush)
{
	if (App->CL_Doc->BTemplate != nullptr)
	{
		App->CL_X_Brush->Brush_Destroy(&App->CL_Doc->BTemplate);
	}

	App->CL_Doc->CurBrush = pBrush;
	App->CL_Doc->TempEnt = false;
	App->CL_Doc->SetDefaultBrushTexInfo(App->CL_Doc->CurBrush);
	App->CL_X_Brush->Brush_Bound(App->CL_Doc->CurBrush);

	T_Vec3 BrushPos;
	App->CL_X_Brush->Brush_Get_Center(App->CL_Doc->CurBrush, &BrushPos);

	T_Vec3* pTemplatePos = App->CL_Level->Level_GetTemplatePos(App->CL_Doc->Current_Level);
	Ogre::Vector3 Pos;

	if (m_UseCamPos == 1 && App->flag_3D_Started == true)
	{
		Pos = App->CL_Ogre->camNode->getPosition();
		*pTemplatePos = { Pos.x, Pos.y, Pos.z };
	}
	else
	{
		*pTemplatePos = { 0, 0, 0 };
	}

	T_Vec3 MoveVec;
	App->CL_X_Maths->Vector3_Subtract(pTemplatePos, &BrushPos, &MoveVec);
	App->CL_X_Brush->Brush_Move(App->CL_Doc->CurBrush, &MoveVec);

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
	App->CL_Level->flag_Level_is_Modified = true;
}

// *************************************************************************
// *			 SetMembers:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CreateBoxDialog::SetMembers()
{
	m_YSize = pBoxTemplate->YSize;
	m_Solid = pBoxTemplate->Solid;
	m_XSizeBot = pBoxTemplate->XSizeBot;
	m_XSizeTop = pBoxTemplate->XSizeTop;
	m_ZSizeBot = pBoxTemplate->ZSizeBot;
	m_ZSizeTop = pBoxTemplate->ZSizeTop;
	m_TCut = pBoxTemplate->TCut;
	m_Thickness = pBoxTemplate->Thickness;
	m_TSheet = pBoxTemplate->TSheet;
}

// *************************************************************************
// *		 Set_Dialog_Members:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateBoxDialog::Set_Dialog_Members(HWND hDlg)
{
	auto setDlgItemText = [&](int itemId, float value)
		{
			char buf[MAX_PATH];
			snprintf(buf, sizeof(buf), "%0.0f", value);
			SetDlgItemText(hDlg, itemId, buf);
		};

	// Set dialog members
	setDlgItemText(IDC_XSIZETOP, m_XSizeTop);
	setDlgItemText(IDC_ZSIZETOP, m_ZSizeTop);
	setDlgItemText(IDC_XSIZEBOT, m_XSizeBot);
	setDlgItemText(IDC_ZSIZEBOT, m_ZSizeBot);
	setDlgItemText(IDC_YSIZE, m_YSize);
	setDlgItemText(IDC_THICKNESS, m_Thickness);
}

// *************************************************************************
// *		 Get_Dialog_Members:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateBoxDialog::Get_Dialog_Members(HWND hDlg)
{

	char buff[MAX_PATH];

	GetDlgItemText(hDlg, IDC_YSIZE, (LPTSTR)buff, MAX_PATH);
	m_YSize = (float)atof(buff);

	GetDlgItemText(hDlg, IDC_XSIZEBOT, (LPTSTR)buff, MAX_PATH);
	m_XSizeBot = (float)atof(buff);

	GetDlgItemText(hDlg, IDC_XSIZETOP, (LPTSTR)buff, MAX_PATH);
	m_XSizeTop = (float)atof(buff);

	GetDlgItemText(hDlg, IDC_ZSIZEBOT, (LPTSTR)buff, MAX_PATH);
	m_ZSizeBot = (float)atof(buff);

	GetDlgItemText(hDlg, IDC_ZSIZETOP, (LPTSTR)buff, MAX_PATH);
	m_ZSizeTop = (float)atof(buff);

	GetDlgItemText(hDlg, IDC_THICKNESS, (LPTSTR)buff, MAX_PATH);
	m_Thickness = (float)atof(buff);


	GetDlgItemText(hDlg, IDC_EDITNAME, (LPTSTR)buff, MAX_PATH);
	strcpy(BoxName, buff);

}

// *************************************************************************
// *		 Set_BoxTemplate:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateBoxDialog::Set_BoxTemplate()
{
	pBoxTemplate->YSize = m_YSize;
	pBoxTemplate->Solid = m_Solid;
	pBoxTemplate->XSizeBot = m_XSizeBot;
	pBoxTemplate->XSizeTop = m_XSizeTop;
	pBoxTemplate->ZSizeBot = m_ZSizeBot;
	pBoxTemplate->ZSizeTop = m_ZSizeTop;
	pBoxTemplate->TCut = m_TCut;
	pBoxTemplate->Thickness = m_Thickness;
	pBoxTemplate->TSheet = m_TSheet;
}

// *************************************************************************
// *	    	Set_Defaults:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateBoxDialog::SetDefaults(HWND hDlg)
{
	m_YSize = 128.0f;
	m_Solid = 0;
	m_XSizeBot = 128.0f;
	m_XSizeTop = 128.0f;
	m_ZSizeBot = 128.0f;
	m_ZSizeTop = 128.0f;
	m_TCut = false;
	m_Thickness = 16.0f;
	m_TSheet = false;

	Set_Dialog_Members(hDlg);

	Zero_Dlg_Flags(hDlg);
	m_Solid = 0;
	Solid_Flag = 1;

	m_TCut = 0;
	Cut_Flag = 0;

	App->CL_X_Shapes_3D->Set_Camera(300);

	int brushCount = App->CL_X_Brush->Get_Brush_Count();
	std::string boxName = "Box_" + std::to_string(brushCount + 1);
	SetDlgItemText(hDlg, IDC_EDITNAME, boxName.c_str());

	HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
	SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_SolidBox_Bmp);

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	    		SetRoom:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CreateBoxDialog::SetRoom(HWND hDlg)
{
	m_YSize = 256.0f;
	m_Solid = 1;
	m_XSizeBot = 768.0f;
	m_XSizeTop = 768.0f;
	m_ZSizeBot = 512.0f;
	m_ZSizeTop = 512.0f;
	m_TCut = false;
	m_Thickness = 16.0f;
	m_TSheet = false;

	Set_Dialog_Members(hDlg);

	Zero_Dlg_Flags(hDlg);
	m_Solid = 1;
	Solid_Flag = 0;
	Hollow_Flag = 1;

	m_TCut = 0;
	Cut_Flag = 0;

	App->CL_X_Shapes_3D->Set_Camera(800);

	int brushCount = App->CL_X_Brush->Get_Brush_Count();
	std::string boxName = "Room_" + std::to_string(brushCount+1);
	SetDlgItemText(hDlg, IDC_EDITNAME, boxName.c_str());

	HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
	SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HollowBox_Bmp);

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	CreateDefault_TemplateCube:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CreateBoxDialog::CreateDefault_TemplateCube()
{
	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();

	Brush* pCube = NULL;
	pCube = App->CL_X_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);
	if (pCube != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName, BoxName);
		CreateNewTemplateBrush(pCube);
	}
	else
	{
		App->Say("No pCube");
	}
}

// *************************************************************************
// *			Zero_Dlg_Flags:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateBoxDialog::Zero_Dlg_Flags(HWND hDlg)
{

	Solid_Flag = 0;
	Hollow_Flag = 0;

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

