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
#include "CL64_Properties_Textures.h"

CL64_Properties_Textures::CL64_Properties_Textures()
{
	Textures_Dlg_Hwnd = nullptr;
}

CL64_Properties_Textures::~CL64_Properties_Textures()
{
}

// *************************************************************************
// *	  	Show_Textures_Dialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Textures::Show_Textures_Dialog(bool Show)
{
	//if (App->CL_Properties_Textures->f_TextureDlg_Active == 1)
	{
		ShowWindow(Textures_Dlg_Hwnd, Show);
	}
}

// *************************************************************************
// *	  	Start_TextureDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Start_TextureDialog()
{
	Textures_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEXTURES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_TextureDialog);

	//Set_Txl_FileName();
	Fill_ListBox();
	//Get_BitMap();
}

// *************************************************************************
// *        Proc_TextureDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Textures::Proc_TextureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{

		//SendDlgItemMessage(hDlg, IDC_STTEXTURE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_GD_TEXTURES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_STTDTXLNAME, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LISTTDTEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BTTDAPPLY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BTEDITFILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BTTDFACEPROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_STWIDTHHEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//App->CLSB_TextureDialog->f_TextureDlg_Active = 1;

		//SetWindowLong(GetDlgItem(hDlg, IDC_BASETEXTURE2), GWL_WNDPROC, (LONG)ViewerBasePic);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_GD_TEXTURES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		/*if (GetDlgItem(hDlg, IDC_STTDTXLNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STWIDTHHEIGHT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BTTDAPPLY && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTEDITFILE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTTDFACEPROPERTIES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		//if (LOWORD(wParam) == IDC_BTTDFACEPROPERTIES)
		//{
		//	App->CL_FaceDialog->Start_FaceDialog();
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_BTEDITFILE)
		//{
		//	App->CL_TxlEditor->Start_Texl_Dialog();

		//	App->Get_Current_Document();

		//	Level_SetWadPath(App->CLSB_Doc->pLevel, Level_GetWadPath(App->CLSB_Doc->pLevel));
		//	App->CL_World->Set_Current_TxlPath();
		//	App->CLSB_Doc->UpdateAfterWadChange();

		//	App->CLSB_TextureDialog->Fill_ListBox();
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_LISTTDTEXTURES)
		//{
		//	App->CLSB_TextureDialog->List_Selection_Changed();
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_BTTDAPPLY)
		//{
		//	App->CLSB_TextureDialog->Apply_Texture();
		//	return TRUE;
		//}

		//// -----------------------------------------------------------------
		//if (LOWORD(wParam) == IDOK)
		//{
		//	//App->CL_TextureDialog->f_TextureDlg_Active = 0;
		//	//EndDialog(hDlg, LOWORD(wParam));
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDCANCEL)
		//{
		//	//App->CL_TextureDialog->f_TextureDlg_Active = 0;
		//	//EndDialog(hDlg, LOWORD(wParam));
		//	return TRUE;
		//}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *			Fill_ListBox:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Fill_ListBox()
{
	int LBIndex;

	//if (f_TextureDlg_Active == 1)
	{
		CL64_WadFile* pWad;
		pWad = NULL;

		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		pWad = App->CL_Level->Level_GetWadFile(App->CL_Doc->pLevel);
		if (pWad == NULL)
		{
			App->Say("Error Getting Wad File",(LPSTR)"");
			return;
		}
		
		for (int index = 0; index < pWad->mBitmapCount; index++)
		{
			char mName[MAX_PATH];

			strcpy(mName, pWad->mBitmaps[index].Name);

			LBIndex = SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
		}

		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		
		//Set_Txl_FileName();
		//List_Selection_Changed();
	}
}
