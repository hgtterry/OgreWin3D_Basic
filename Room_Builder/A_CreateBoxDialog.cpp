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
#include "A_CreateBoxDialog.h"

A_CreateBoxDialog::A_CreateBoxDialog(void)
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

	m_UseCamPos = 0;

	Solid_Flag = 1;
	Hollow_Flag = 0;
	Cut_Flag = 0;

	flag_Default = 1;
	flag_Room = 0;

	strcpy(BoxName,"Box");
}

A_CreateBoxDialog::~A_CreateBoxDialog(void)
{
}

// *************************************************************************
// *	  	Start_CreateBox_Dlg:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void A_CreateBoxDialog::Start_CreateBox_Dlg()
{

	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate(App->CL_Doc->Current_Level);

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	DialogBox(App->hInst, (LPCTSTR)IDD_CREATE_BOX, App->MainHwnd, (DLGPROC)Proc_CreateBox);
}

// *************************************************************************
// *        CreateBox_Proc:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
LRESULT CALLBACK A_CreateBoxDialog::Proc_CreateBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
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
			IDOK, IDCANCEL
		};

		// Set font for each control
		for (int id : controlIDs) {
			SendDlgItemMessage(hDlg, id, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		}

		// Initialize dialog members
		App->CL_CreateBoxDialog->SetMembers();
		App->CL_CreateBoxDialog->Set_Dialog_Members(hDlg);
		App->CL_CreateBoxDialog->SetDefaults(hDlg);

		// Generate and set the box name
		int Count = App->CL_Brush->Get_Brush_Count();
		char Name[32];
		snprintf(Name, sizeof(Name), "Box_%d", Count);
		SetDlgItemText(hDlg, IDC_EDITNAME, Name);

		// Set checkbox state
		HWND Temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
		SendMessage(Temp, BM_SETCHECK, 1, 0);
		App->CL_CreateBoxDialog->m_UseCamPos = 0;

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
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->Solid_Flag);
			break;

		case IDC_BT_BOXHOLLOW:
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->Hollow_Flag);
			break;

		case IDC_BT_BOXCUTBRUSH:
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->Cut_Flag);
			break;

		case IDC_BOXDEFAULTS:
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->flag_Default);
			break;

		case IDC_BT_BOXROOM:
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->flag_Room);
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

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_BOXSOLID)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
			SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_SolidBox_Bmp);

			App->CL_CreateBoxDialog->Zero_Dlg_Flags(hDlg);
			App->CL_CreateBoxDialog->m_Solid = 0;
			App->CL_CreateBoxDialog->Solid_Flag = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXHOLLOW)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
			SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HollowBox_Bmp);
			
			App->CL_CreateBoxDialog->Zero_Dlg_Flags(hDlg);
			App->CL_CreateBoxDialog->m_Solid = 1;
			App->CL_CreateBoxDialog->Hollow_Flag = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXCUTBRUSH)
		{
			App->CL_CreateBoxDialog->m_TCut = !App->CL_CreateBoxDialog->Cut_Flag;
			App->CL_CreateBoxDialog->Cut_Flag = !App->CL_CreateBoxDialog->Cut_Flag;

			int Count = App->CL_Brush->Get_Brush_Count();
			char Name[32];
			snprintf(Name, sizeof(Name), "Box_%d%s", Count, App->CL_CreateBoxDialog->Cut_Flag ? "_Cut" : "");

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

			App->CL_CreateBoxDialog->m_UseCamPos = 0;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKCAMPOSITION)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CL_CreateBoxDialog->m_UseCamPos = 1;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BOXDEFAULTS) 
		{
			App->CL_CreateBoxDialog->flag_Default = 1;
			App->CL_CreateBoxDialog->flag_Room = 0;
			RedrawWindow(hDlg, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

			int brushCount = App->CL_Brush->Get_Brush_Count();
			std::string name = "Box_" + std::to_string(brushCount);

			SetDlgItemText(hDlg, IDC_EDITNAME, name.c_str());

			App->CL_CreateBoxDialog->SetDefaults(hDlg);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXROOM)
		{
			App->CL_CreateBoxDialog->flag_Default = 0;
			App->CL_CreateBoxDialog->flag_Room = 1;
			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			int brushCount = App->CL_Brush->Get_Brush_Count();
			std::string boxName = "Box_" + std::to_string(brushCount);

			SetDlgItemText(hDlg, IDC_EDITNAME, boxName.c_str());

			App->CL_CreateBoxDialog->SetRoom(hDlg);
			return TRUE;
		}
		
		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_CreateBoxDialog->Get_Dialog_Members(hDlg);
			App->CL_CreateBoxDialog->Set_BoxTemplate();
			App->CL_CreateBoxDialog->CreateCube();
			
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
// *		   CreateCube:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void A_CreateBoxDialog::CreateCube() 
{
	App->CL_Doc->OnToolsTemplate();

	Brush *pCube;
	pCube = App->CL_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);
	if (pCube != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName,BoxName);
		strcpy(pCube->Name,"Test");
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
void A_CreateBoxDialog::CreateNewTemplateBrush(Brush* pBrush)
{
	if (App->CL_Doc->BTemplate != nullptr)
	{
		App->CL_Brush->Brush_Destroy(&App->CL_Doc->BTemplate);
	}

	App->CL_Doc->CurBrush = pBrush;
	App->CL_Doc->TempEnt = false;
	App->CL_Doc->SetDefaultBrushTexInfo(App->CL_Doc->CurBrush);
	App->CL_Brush->Brush_Bound(App->CL_Doc->CurBrush);

	T_Vec3 BrushPos;
	App->CL_Brush->Brush_Center(App->CL_Doc->CurBrush, &BrushPos);

	T_Vec3* pTemplatePos = App->CL_Level->Level_GetTemplatePos(App->CL_Doc->Current_Level);
	Ogre::Vector3 Pos;

	if (m_UseCamPos == 1 && App->flag_OgreStarted == 1)
	{
		Pos = App->CL_Ogre->camNode->getPosition();
		*pTemplatePos = { Pos.x, Pos.y, Pos.z };
	}
	else
	{
		*pTemplatePos = { 0, 0, 0 };
	}

	T_Vec3 MoveVec;
	App->CL_Maths->Vector3_Subtract(pTemplatePos, &BrushPos, &MoveVec);
	App->CL_Brush->Brush_Move(App->CL_Doc->CurBrush, &MoveVec);

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
	App->CL_Doc->flag_Is_Modified = true;
}

// *************************************************************************
// *			 SetMembers:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void A_CreateBoxDialog::SetMembers() 
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
void A_CreateBoxDialog::Set_Dialog_Members(HWND hDlg)
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
void A_CreateBoxDialog::Get_Dialog_Members(HWND hDlg) 
{

	char buff[MAX_PATH];

	GetDlgItemText(hDlg,IDC_YSIZE,(LPTSTR)buff,MAX_PATH);
	m_YSize = (float)atof(buff);

	GetDlgItemText(hDlg,IDC_XSIZEBOT,(LPTSTR)buff,MAX_PATH);
	m_XSizeBot = (float)atof(buff);

	GetDlgItemText(hDlg,IDC_XSIZETOP,(LPTSTR)buff,MAX_PATH);
	m_XSizeTop = (float)atof(buff);

	GetDlgItemText(hDlg,IDC_ZSIZEBOT,(LPTSTR)buff,MAX_PATH);
	m_ZSizeBot = (float)atof(buff);

	GetDlgItemText(hDlg,IDC_ZSIZETOP,(LPTSTR)buff,MAX_PATH);
	m_ZSizeTop = (float)atof(buff);

	GetDlgItemText(hDlg,IDC_THICKNESS,(LPTSTR)buff,MAX_PATH);
	m_Thickness = (float)atof(buff);


	GetDlgItemText(hDlg,IDC_EDITNAME,(LPTSTR)buff,MAX_PATH);
	strcpy(App->CL_CreateBoxDialog->BoxName,buff);

}

// *************************************************************************
// *		 Set_BoxTemplate:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void A_CreateBoxDialog::Set_BoxTemplate() 
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
void A_CreateBoxDialog::SetDefaults(HWND hDlg) 
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

	App->CL_CreateBoxDialog->Zero_Dlg_Flags(hDlg);
	App->CL_CreateBoxDialog->m_Solid = 0;
	App->CL_CreateBoxDialog->Solid_Flag = 1;

	App->CL_CreateBoxDialog->m_TCut = 0;
	App->CL_CreateBoxDialog->Cut_Flag = 0;

	HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
	SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_SolidBox_Bmp);

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	    		SetRoom:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void A_CreateBoxDialog::SetRoom(HWND hDlg)
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

	App->CL_CreateBoxDialog->Zero_Dlg_Flags(hDlg);
	App->CL_CreateBoxDialog->m_Solid = 1;
	App->CL_CreateBoxDialog->Solid_Flag = 0;
	App->CL_CreateBoxDialog->Hollow_Flag = 1;

	App->CL_CreateBoxDialog->m_TCut = 0;
	App->CL_CreateBoxDialog->Cut_Flag = 0;

	HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
	SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HollowBox_Bmp);

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	CreateDefault_TemplateCube:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void A_CreateBoxDialog::CreateDefault_TemplateCube()
{
	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate(App->CL_Doc->Current_Level);

	Brush* pCube = NULL;
	pCube = App->CL_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);
	if (pCube != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName,BoxName);
		CreateNewTemplateBrush(pCube);
		
		App->CL_Properties_Templates->Enable_Insert_Button(true);

		Debug
	}
	else
	{
		App->Say("No pCube");
	}
}

// *************************************************************************
// *			Zero_Dlg_Flags:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void A_CreateBoxDialog::Zero_Dlg_Flags(HWND hDlg)
{

	Solid_Flag = 0;
	Hollow_Flag = 0;

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
