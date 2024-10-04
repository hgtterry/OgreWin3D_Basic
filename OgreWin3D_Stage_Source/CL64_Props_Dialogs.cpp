/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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
#include "CL64_App.h"
#include "resource.h"
#include "CL64_Props_Dialogs.h"

CL64_Props_Dialogs::CL64_Props_Dialogs(void)
{
	Details_Goto_Hwnd = nullptr;
}

CL64_Props_Dialogs::~CL64_Props_Dialogs(void)
{
}

// **************************************************************************
// *	  		Start_Props_Dialogs:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Props_Dialogs::Start_Props_Dialogs()
{

	//Start_Dialog_Dimensions();
	//Start_Dialog_PhysicsReset();
	//Start_Dialog_Debug();
	//Start_Panels_Test_Dlg();
	//Start_Area_PropsPanel();
	Start_Details_Goto_Dlg();
	//Start_Materials_PropsPanel();

}

// *************************************************************************
// *	Start_Details_Goto_PropsPanel:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Details_Goto_Dlg(void)
{
	Details_Goto_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_OBJECT, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Details_Goto);

	//Init_Bmps_DetailsGo();
	Hide_Details_Goto_Dlg(1);

}

// *************************************************************************
// *			Proc_Details_Goto:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Proc_Details_Goto(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//SendDlgItemMessage(hDlg, IDC_BT_GOTO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BT_DETAIL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BT_GOTO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_DETAIL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		/*if (LOWORD(wParam) == IDC_BT_DETAIL)
		{
			if (App->CL_Vm_ImGui->Show_Object_Data == 1)
			{
				App->CL_Vm_ImGui->Show_Object_Data = 0;
			}
			else
			{
				App->SBC_LookUps->Update_Types();
				App->CL_Vm_ImGui->Show_Object_Data = 1;
			}
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_GOTO)
		{
			App->CL_Object->Object_Camera_Goto(App->SBC_Properties->Current_Selected_Object);
			return 1;
		}*/

		break;
	}
	return FALSE;
}

// *************************************************************************
// *		Hide_Details_Goto_Dlg:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void CL64_Props_Dialogs::Hide_Details_Goto_Dlg(bool Show)
{
	ShowWindow(Details_Goto_Hwnd, Show);
}