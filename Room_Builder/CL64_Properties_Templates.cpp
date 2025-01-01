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
		
		SendDlgItemMessage(hDlg, IDC_BTINSERT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BTINSERT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BTINSERT)
		{
			if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
			{
				if (App->CL_Brush->Get_Brush_Count() == 0)
				{
					App->CL_Doc->AddBrushToWorld();
					App->CL_Doc->flag_Is_Modified = 1;
				}
				else
				{
					App->CL_Doc->AddBrushToWorld();
					App->CL_Doc->flag_Is_Modified = 1;
				}
			}

			App->CL_Doc->DoGeneralSelect();
			App->CL_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);

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

