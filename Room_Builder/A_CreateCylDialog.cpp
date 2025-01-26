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
#include "A_CreateCylDialog.h"

A_CreateCylDialog::A_CreateCylDialog(void)
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
	m_YSize = 512.0;
	m_RingLength = 0.0;
	m_TCut = false;
	m_VerticalStripes = 0;
	m_Thickness = 0.0f;

	m_UseCamPos = 0;

	strcpy(CylinderName,"Cylinder");

	pCylinderTemplate = NULL;

	flag_Solid_Flag_Dlg = 1;
	flag_Hollow_Flag_Dlg = 0;
	flag_Ring_Flag_Dlg = 0;
}

A_CreateCylDialog::~A_CreateCylDialog(void)
{
}

// *************************************************************************
// *	  	Start_CreateCyl_Dlg:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void A_CreateCylDialog::Start_CreateCyl_Dlg()
{
	pCylinderTemplate = App->CL_Level->Level_GetCylinderTemplate (App->CL_Doc->pLevel);

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
	DialogBox(App->hInst, (LPCTSTR)IDD_CREATE_CYLINDER, App->MainHwnd, (DLGPROC)Proc_Create_Cylinder);
}

// *************************************************************************
// *        CreateBox_Proc:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
LRESULT CALLBACK A_CreateCylDialog::Proc_Create_Cylinder(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
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
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_CreateCylDialog->Set_Members();

		App->CL_CreateCylDialog->Set_DLG_Members(hDlg);

		SetDlgItemText(hDlg, IDC_EDITNAME, (LPCTSTR)"Cylinder");

		// ----------- Style Solid Hollow Funnel
		if(App->CL_CreateCylDialog->m_Solid == 0)
		{
			App->CL_CreateCylDialog->flag_Solid_Flag_Dlg = 1;
			App->CL_CreateCylDialog->flag_Hollow_Flag_Dlg = 0;
			App->CL_CreateCylDialog->flag_Ring_Flag_Dlg = 0;
		}

		if(App->CL_CreateCylDialog->m_Solid == 1)
		{
			App->CL_CreateCylDialog->flag_Hollow_Flag_Dlg = 1;
			App->CL_CreateCylDialog->flag_Solid_Flag_Dlg = 0;
			App->CL_CreateCylDialog->flag_Ring_Flag_Dlg = 0;
		}

		if(App->CL_CreateCylDialog->m_Solid == 2)
		{
			App->CL_CreateCylDialog->flag_Ring_Flag_Dlg = 1;
			App->CL_CreateCylDialog->flag_Hollow_Flag_Dlg = 0;
			App->CL_CreateCylDialog->flag_Solid_Flag_Dlg = 0;
		}
		// ----------------------------------------------

		HWND temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
		SendMessage(temp,BM_SETCHECK,1,0);
		App->CL_CreateCylDialog->m_UseCamPos = 0;

		int Count = App->CL_Brush->Get_Brush_Count();
		char Num[32];
		char Name[32];
		_itoa(Count, Num, 10);
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
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateCylDialog->m_TCut);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_CYL_SOLID)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateCylDialog->flag_Solid_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CYL_HOLLOW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateCylDialog->flag_Hollow_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CYL_RING)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_CreateCylDialog->flag_Ring_Flag_Dlg);
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

	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDC_CKWORLDCENTRE)
			{
				HWND temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
				SendMessage(temp,BM_SETCHECK,1,0);

				temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
				SendMessage(temp,BM_SETCHECK,0,0);

				App->CL_CreateCylDialog->m_UseCamPos = 0;
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_CKCAMPOSITION)
			{
				HWND temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
				SendMessage(temp,BM_SETCHECK,1,0);

				temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
				SendMessage(temp,BM_SETCHECK,0,0);

				App->CL_CreateCylDialog->m_UseCamPos = 1;
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_CYL_SOLID)
			{
				App->CL_CreateCylDialog->m_Solid = 0;

				App->CL_CreateCylDialog->flag_Solid_Flag_Dlg = 1;
				App->CL_CreateCylDialog->flag_Hollow_Flag_Dlg = 0;
				App->CL_CreateCylDialog->flag_Ring_Flag_Dlg = 0;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_CYL_HOLLOW)
			{
				App->CL_CreateCylDialog->m_Solid = 1;

				App->CL_CreateCylDialog->flag_Hollow_Flag_Dlg = 1;
				App->CL_CreateCylDialog->flag_Solid_Flag_Dlg = 0;
				App->CL_CreateCylDialog->flag_Ring_Flag_Dlg = 0;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				return TRUE;
			}
			
			if (LOWORD(wParam) == IDC_BT_CYL_RING)
			{
				App->CL_CreateCylDialog->m_Solid = 2;

				App->CL_CreateCylDialog->flag_Ring_Flag_Dlg = 1;
				App->CL_CreateCylDialog->flag_Hollow_Flag_Dlg = 0;
				App->CL_CreateCylDialog->flag_Solid_Flag_Dlg = 0;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_CUTBRUSH)
			{
				if (App->CL_CreateCylDialog->m_TCut == 1)
				{
					App->CL_CreateCylDialog->m_TCut = 0;
				}
				else
				{
					App->CL_CreateCylDialog->m_TCut = 1;
				}

				return TRUE;
			}

			if (LOWORD(wParam) == IDC_CYL_DEFAULTS)
			{
				App->CL_CreateCylDialog->Set_Defaults(hDlg);
				return TRUE;
			}

			// -----------------------------------------------------------------
			if (LOWORD(wParam) == IDOK)
			{
				App->CL_CreateCylDialog->Get_DLG_Members(hDlg);
				App->CL_CreateCylDialog->Set_CylinderTemplate(); 
				App->CL_CreateCylDialog->CreateCylinder();

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
// *		   CreateCylinder:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void A_CreateCylDialog::CreateCylinder() 
{
	App->CL_Doc->OnToolsTemplate();

	Brush *pCylinder;

	pCylinder = App->CL_BrushTemplate->BrushTemplate_CreateCylinder (pCylinderTemplate);
	if (pCylinder != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName,CylinderName);
		CreateNewTemplateBrush(pCylinder);
	}
	else
	{
		App->Say("No pCylinder");
	}
}

// *************************************************************************
// *       CreateNewTemplateBrush:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void A_CreateCylDialog::CreateNewTemplateBrush(Brush *pBrush)
{
	T_Vec3*pTemplatePos;
	T_Vec3 MoveVec;
	T_Vec3 BrushPos;

	assert (pBrush != NULL);

	if (App->CL_Doc->BTemplate != NULL)
	{
		App->CL_Brush->Brush_Destroy (&App->CL_Doc->BTemplate);
	}

	App->CL_Doc->CurBrush = pBrush;

	App->CL_Doc->TempEnt	= FALSE;
	App->CL_Doc->SetDefaultBrushTexInfo (App->CL_Doc->CurBrush);
	App->CL_Brush->Brush_Bound (App->CL_Doc->CurBrush);
	App->CL_Brush->Brush_Center (App->CL_Doc->CurBrush, &BrushPos);

	pTemplatePos = App->CL_Level->Level_GetTemplatePos (App->CL_Doc->pLevel);

	if (m_UseCamPos == 1 && App->flag_OgreStarted == 1)
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

	App->CL_Brush->Brush_Move (App->CL_Doc->CurBrush, &MoveVec);

	App->CL_Doc->UpdateAllViews (Enums::UpdateViews_Grids);

	App->CL_Doc->flag_Is_Modified = 1;
}

// *************************************************************************
// *		 Set_Members:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void A_CreateCylDialog::Set_Members() 
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
void A_CreateCylDialog::Set_DLG_Members(HWND hDlg) 
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


	HWND temp = GetDlgItem(hDlg, IDC_TCUT);
	if (m_TCut == 1)
	{
		SendMessage(temp,BM_SETCHECK,1,0);
	}
	else
	{
		SendMessage(temp,BM_SETCHECK,0,0);
	}
}

// *************************************************************************
// *		 Get_DLG_Members:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void A_CreateCylDialog::Get_DLG_Members(HWND hDlg) 
{
	char buf[MAX_PATH];

	GetDlgItemText(hDlg, IDC_TOPXSIZE, (LPTSTR)buf,MAX_PATH);
	m_TopXSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_TOPZSIZE, (LPTSTR)buf,MAX_PATH);
	m_TopZSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_TOPXOFF, (LPTSTR)buf,MAX_PATH);
	m_TopXOffset = (float)atof(buf);
	
	GetDlgItemText(hDlg, IDC_TOPZOFF, (LPTSTR)buf,MAX_PATH);
	m_TopZOffset = (float)atof(buf);


	GetDlgItemText(hDlg, IDC_BOTXSIZE, (LPTSTR)buf,MAX_PATH);
	m_BotXSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_BOTZSIZE, (LPTSTR)buf,MAX_PATH);
	m_BotZSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_BOTXOFF, (LPTSTR)buf,MAX_PATH);
	m_BotXOffset = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_BOTZOFF, (LPTSTR)buf,MAX_PATH);
	m_BotZOffset = (float)atof(buf);


	GetDlgItemText(hDlg, IDC_YSIZE, (LPTSTR)buf,MAX_PATH);
	m_YSize = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_VERTSTRIPES, (LPTSTR)buf,MAX_PATH);
	m_VerticalStripes = (int)atoi(buf);

	GetDlgItemText(hDlg, IDC_THICKNESS, (LPTSTR)buf,MAX_PATH);
	m_Thickness = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_RINGLENGTH, (LPTSTR)buf,MAX_PATH);
	m_RingLength = (float)atof(buf);

	GetDlgItemText(hDlg,IDC_EDITNAME,(LPTSTR)buf,MAX_PATH);
	strcpy(CylinderName,buf);
}

// *************************************************************************
// *		 Set_CylinderTemplate:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void A_CreateCylDialog::Set_CylinderTemplate() 
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
void A_CreateCylDialog::Set_Defaults(HWND hDlg) 
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
	m_YSize = 512.0;
	m_RingLength = 0.0;
	m_TCut = false;
	m_VerticalStripes = 6;
	m_Thickness = 16.0f;

	Set_DLG_Members(hDlg);

	App->CL_CreateCylDialog->flag_Solid_Flag_Dlg = 1;
	App->CL_CreateCylDialog->flag_Hollow_Flag_Dlg = 0;
	App->CL_CreateCylDialog->flag_Ring_Flag_Dlg = 0;

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
