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
#include "A_CreateConeDialog.h"

A_CreateConeDialog::A_CreateConeDialog(void)
{
	m_Style = 0;
	m_Width = 200;
	m_Height = 300;
	m_VerticalStrips = 4;
	m_Thickness = 16;
	m_TCut = false;

	m_UseCamPos = 0;

	pConeTemplate = NULL;

	strcpy(ConeName,"Cone");

	flag_Solid_Flag_Dlg = 1;
	flag_Hollow_Flag_Dlg = 0;
	flag_Funnel_Flag_Dlg = 0;
}

A_CreateConeDialog::~A_CreateConeDialog(void)
{
}

// *************************************************************************
// *	  	Start_CreateCone_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void A_CreateConeDialog::Start_CreateCone_Dlg()
{
	pConeTemplate = App->CL_Level->Level_GetConeTemplate();

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	DialogBox(App->hInst, (LPCTSTR)IDD_CREATE_CONE, App->MainHwnd, (DLGPROC)Proc_CreateCone);
}

// *************************************************************************
// *        Proc_CreateCone:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
LRESULT CALLBACK A_CreateConeDialog::Proc_CreateCone(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
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

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_CreateConeDialog->Set_Members();
		App->CL_CreateConeDialog->Set_DLG_Members(hDlg);

		int Count = App->CL_Brush->Get_Brush_Count();
		char Num[32];
		char Name[32];
		_itoa(Count, Num, 10);
		strcpy(Name, "Cone_");
		strcat(Name, Num);

		SetDlgItemText(hDlg, IDC_ED_CONE_NAME, (LPCTSTR)Name);


		// ----------- Style Solid Hollow Funnel
		if(App->CL_CreateConeDialog->m_Style == 0)
		{
			App->CL_CreateConeDialog->flag_Solid_Flag_Dlg = 1;
			App->CL_CreateConeDialog->flag_Hollow_Flag_Dlg = 0;
			App->CL_CreateConeDialog->flag_Funnel_Flag_Dlg = 0;
		}

		if(App->CL_CreateConeDialog->m_Style == 1)
		{
			App->CL_CreateConeDialog->flag_Solid_Flag_Dlg = 0;
			App->CL_CreateConeDialog->flag_Hollow_Flag_Dlg = 1;
			App->CL_CreateConeDialog->flag_Funnel_Flag_Dlg = 0;
		}

		if(App->CL_CreateConeDialog->m_Style == 2)
		{
			App->CL_CreateConeDialog->flag_Solid_Flag_Dlg = 0;
			App->CL_CreateConeDialog->flag_Hollow_Flag_Dlg = 0;
			App->CL_CreateConeDialog->flag_Funnel_Flag_Dlg = 1;
		}

		// ----------------------------------------------
		HWND temp = GetDlgItem(hDlg, IDC_CK_CONE_WORLDCENTRE);
		SendMessage(temp,BM_SETCHECK,1,0);
		App->CL_CreateConeDialog->m_UseCamPos = 0;

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
			App->Custom_Button_Toggle(item, App->CL_CreateConeDialog->flag_Solid_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CONE_HOLLOW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_CreateConeDialog->flag_Hollow_Flag_Dlg);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CONE_FUNNEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item,App->CL_CreateConeDialog->flag_Funnel_Flag_Dlg);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_CONE_CUTBRUSH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_CreateConeDialog->m_TCut);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CONE_DEFAULTS)
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

	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDC_CK_CONE_WORLDCENTRE)
			{
				HWND temp = GetDlgItem(hDlg, IDC_CK_CONE_WORLDCENTRE);
				SendMessage(temp,BM_SETCHECK,1,0);

				temp = GetDlgItem(hDlg, IDC_CK_CONE_CAMPOSITION);
				SendMessage(temp,BM_SETCHECK,0,0);

				App->CL_CreateConeDialog->m_UseCamPos = 0;
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_CK_CONE_CAMPOSITION)
			{
				HWND temp = GetDlgItem(hDlg, IDC_CK_CONE_CAMPOSITION);
				SendMessage(temp,BM_SETCHECK,1,0);

				temp = GetDlgItem(hDlg, IDC_CK_CONE_WORLDCENTRE);
				SendMessage(temp,BM_SETCHECK,0,0);

				App->CL_CreateConeDialog->m_UseCamPos = 1;
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_CONE_SOLID)
			{
				App->CL_CreateConeDialog->m_Style = 0;

				App->CL_CreateConeDialog->flag_Solid_Flag_Dlg = 1;
				App->CL_CreateConeDialog->flag_Hollow_Flag_Dlg = 0;
				App->CL_CreateConeDialog->flag_Funnel_Flag_Dlg = 0;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_CONE_HOLLOW)
			{
				App->CL_CreateConeDialog->m_Style = 1;

				App->CL_CreateConeDialog->flag_Solid_Flag_Dlg = 0;
				App->CL_CreateConeDialog->flag_Hollow_Flag_Dlg = 1;
				App->CL_CreateConeDialog->flag_Funnel_Flag_Dlg = 0;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_CONE_FUNNEL)
			{
				App->CL_CreateConeDialog->m_Style = 2;

				App->CL_CreateConeDialog->flag_Solid_Flag_Dlg = 0;
				App->CL_CreateConeDialog->flag_Hollow_Flag_Dlg = 0;
				App->CL_CreateConeDialog->flag_Funnel_Flag_Dlg = 1;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_CONE_CUTBRUSH)
			{
				if (App->CL_CreateConeDialog->m_TCut == 1)
				{
					App->CL_CreateConeDialog->m_TCut = 0;
					return 1;
				}
				else
				{
					App->CL_CreateConeDialog->m_TCut = 1;
					return 1;
				}

				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_CONE_DEFAULTS)
			{
				App->CL_CreateConeDialog->Set_Defaults(hDlg);
				return TRUE;
			}

			// -----------------------------------------------------------------
			if (LOWORD(wParam) == IDOK)
			{
				App->CL_CreateConeDialog->Get_DLG_Members(hDlg);
				App->CL_CreateConeDialog->Set_ConeTemplate(); 
				App->CL_CreateConeDialog->CreateCone();

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
// *		   CreateCone:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void A_CreateConeDialog::CreateCone() 
{
	App->CL_Doc->OnToolsTemplate();

	Brush *pCone;
	pCone = App->CL_BrushTemplate->BrushTemplate_CreateCone (pConeTemplate);
	if (pCone != NULL)
	{
		strcpy(App->CL_Doc->LastTemplateTypeName, ConeName);
		CreateNewTemplateBrush (pCone);
	}
	else
	{
		App->Say("No Cone");
	}
}

// *************************************************************************
// *       CreateNewTemplateBrush:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void A_CreateConeDialog::CreateNewTemplateBrush(Brush *pBrush)
{
	T_Vec3 *pTemplatePos;
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

	pTemplatePos = App->CL_Level->Level_GetTemplatePos (App->CL_Doc->Current_Level);

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

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	App->CL_Level->flag_Level_is_Modified = true;
}

// *************************************************************************
// *		 Set_Members:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void A_CreateConeDialog::Set_Members() 
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
void A_CreateConeDialog::Set_DLG_Members(HWND hDlg) 
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
void A_CreateConeDialog::Get_DLG_Members(HWND hDlg) 
{
	char buf[MAX_PATH];

	GetDlgItemText(hDlg, IDC_ED_CONE_1, (LPTSTR)buf,MAX_PATH);
	m_Width = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ED_CONE_2, (LPTSTR)buf,MAX_PATH);
	m_Height = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ED_CONE_3, (LPTSTR)buf,MAX_PATH);
	m_VerticalStrips = (float)atof(buf);
	
	GetDlgItemText(hDlg, IDC_ED_CONE_4, (LPTSTR)buf,MAX_PATH);
	m_Thickness = (float)atof(buf);

	GetDlgItemText(hDlg, IDC_ED_CONE_NAME,(LPTSTR)buf,MAX_PATH);
	strcpy(ConeName,buf);
}

// *************************************************************************
// *		 Set_ConeTemplate:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void A_CreateConeDialog::Set_ConeTemplate() 
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
void A_CreateConeDialog::Set_Defaults(HWND hDlg) 
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
	App->CL_CreateConeDialog->m_UseCamPos = 0;

	App->CL_CreateConeDialog->flag_Solid_Flag_Dlg = 1;
	App->CL_CreateConeDialog->flag_Hollow_Flag_Dlg = 0;
	App->CL_CreateConeDialog->flag_Funnel_Flag_Dlg = 0;

	RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
