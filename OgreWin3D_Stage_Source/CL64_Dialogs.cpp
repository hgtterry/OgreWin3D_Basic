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
#include "CL64_Dialogs.h"

CL64_Dialogs::CL64_Dialogs(void)
{
	Message_Text_Header[0] = 0;
	Message_Text_Message[0] = 0;

	MessageString[0] = 0;
	MessageString2[0] = 0;
	MessageString3[0] = 0;

	Canceled = 0;
	TrueFlase = 0;

	Flag_Convert_to_Ogre = 0;

	flag_Mouse_Normal = 1;
	flag_Mouse_Slow = 0;
	flag_Mouse_VerySlow = 0;
	flag_Mouse_Fast = 0;

	flag_Dlg_Int_Active = 0;

	What_Check_Name = Enums::Check_Name_None;

	btext[0] = 0;
	Chr_Text[0] = 0;
	mTextureFile[0] = 0;
	mFile[0] = 0;

	Chr_Int[0] = 0;
	mInt = 0;

	Sel_BaseBitmap =	nullptr;
	RightGroups_Hwnd =	nullptr;
	FileViewer_Hwnd =	nullptr;
	FPSLock_Dlg_hWnd =	nullptr;

}

CL64_Dialogs::~CL64_Dialogs(void)
{
}

// *************************************************************************
// *	  			Message:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Dialogs::Message(char* pString, char* pString2)
{
	Message_Text_Header[0] = 0;
	Message_Text_Message[0] = 0;

	strcpy(Message_Text_Header, " ");
	strcat(Message_Text_Header, pString);
	strcat(Message_Text_Header, " ");

	strcpy(Message_Text_Message, " ");
	strcat(Message_Text_Message, pString2);
	strcat(Message_Text_Message, " ");

	DialogBox(App->hInst, (LPCTSTR)IDD_MESSAGE, App->Fdlg, (DLGPROC)Proc_Message);
}

// *************************************************************************
// *        	Message_Proc:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Message(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STTEXT, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STMESSAGE, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SetDlgItemText(hDlg, IDC_STTEXT, App->CL_Dialogs->Message_Text_Header);
		SetDlgItemText(hDlg, IDC_STMESSAGE, App->CL_Dialogs->Message_Text_Message);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_STTEXT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STMESSAGE) == (HWND)lParam)
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

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	return FALSE;
}

// *************************************************************************
// *	  			PleaseWait:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::PleaseWait()
{
	App->ViewPLeaseWait = CreateDialog(App->hInst, (LPCTSTR)IDD_PLEASEWAIT, App->Fdlg, (DLGPROC)Proc_PleaseWait);
}

// *************************************************************************
// *			Proc_PleaseWait:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_PleaseWait(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		HFONT Font;
		Font = CreateFont(-25, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Aerial Black");
		SendDlgItemMessage(hDlg, IDC_STWAIT, WM_SETFONT, (WPARAM)Font, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Green;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STWAIT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_Green;
		}

		return FALSE;
	}
	}
	return FALSE;
}

// *************************************************************************
// *	  		Start_FPSLock_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Start_FPSLock_Dlg()
{
	if (App->CL_TopDlg->flag_FPS_Dlg_Running == 0)
	{
		FPSLock_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_FPSLOCK, App->Fdlg, (DLGPROC)Proc_FPSLock);
		App->CL_TopDlg->flag_FPS_Dlg_Running = 1;
	}
}

// *************************************************************************
// *			Proc_FPSLock:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_FPSLock(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_FPSSETTINGS, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_FPSNOLOCK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_FPSLIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Dialogs->Fill_Face_Combo(hDlg);

		HWND temp = GetDlgItem(hDlg, IDC_CK_FPSNOLOCK);
		HWND temp1 = GetDlgItem(hDlg, IDC_CB_FPSLIST);

		if (App->CL_Ogre->FPSLock == 0)
		{
			SendMessage(temp, BM_SETCHECK, 1, 0);
			EnableWindow(temp1, false);
		}
		else
		{
			SendMessage(temp, BM_SETCHECK, 0, 0);
			EnableWindow(temp1, true);
		}

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_FPSSETTINGS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_FPSNOLOCK) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		return FALSE;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_CB_FPSLIST)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_FPSLIST);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				int Num = atof(buff);

				float test = static_cast<float>(1) / Num;
				test = test * 1000000;
				App->CL_Ogre->FPSLock = (int)test;

			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_FPSNOLOCK)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_FPSNOLOCK);
			HWND temp1 = GetDlgItem(hDlg, IDC_CB_FPSLIST);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Ogre->FPSLock = 0;
				EnableWindow(temp1, false);
				return 1;
			}
			else
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_FPSLIST);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				int Num = atof(buff);

				float test = static_cast<float>(1) / Num;
				test = test * 1000000;
				App->CL_Ogre->FPSLock = (int)test;

				EnableWindow(temp1, true);
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	return FALSE;
}

// *************************************************************************
// *		  	Fill_Face_Combo:- Terry and Hazel Flanigan 2024	    	   *
// *************************************************************************
void CL64_Dialogs::Fill_Face_Combo(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_CB_FPSLIST, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	SendDlgItemMessage(hDlg, IDC_CB_FPSLIST, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"60");
	SendDlgItemMessage(hDlg, IDC_CB_FPSLIST, CB_ADDSTRING, (WPARAM)0, (LPARAM)"100");
	SendDlgItemMessage(hDlg, IDC_CB_FPSLIST, CB_ADDSTRING, (WPARAM)0, (LPARAM)"120");

	SendDlgItemMessage(hDlg, IDC_CB_FPSLIST, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

}

// *************************************************************************
// *	  Start_Import_Options_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Start_Import_Options_Dlg()
{
	Flag_Convert_to_Ogre = 0;

	DialogBox(App->hInst, (LPCTSTR)IDD_IMPORT_OPTIONS, App->Fdlg, (DLGPROC)Proc_Import_Options_Dlg);
}

// *************************************************************************
// *        Proc_Import_Options_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Import_Options_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_CK_COVERT_TO_OGRE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_CREATEPHYSICS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_EXPORT_TO_OGRE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_CK_EXPORT_TO_OGRE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_COVERT_TO_OGRE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_CREATEPHYSICS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:


		if (LOWORD(wParam) == IDOK)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_COVERT_TO_OGRE);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Dialogs->Flag_Convert_to_Ogre = 1;
			}
			
			temp = GetDlgItem(hDlg, IDC_CK_EXPORT_TO_OGRE);
			test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				EndDialog(hDlg, LOWORD(wParam));
				//App->CL_Exporters->Export_Ogre(1);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  		Show_YesNo_Dlg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::Show_YesNo_Dlg(char* Text, char* Text2, char* Text3)
{
	Canceled = 0;

	MessageString[0] = 0;
	MessageString2[0] = 0;
	MessageString3[0] = 0;

	strcpy(MessageString, Text);
	strcpy(MessageString2, Text2);
	strcpy(MessageString3, Text3);

	DialogBox(App->hInst, (LPCTSTR)IDD_YESNO, App->Fdlg, (DLGPROC)Proc_YesNo);

}

// *************************************************************************
// *			Proc_YesNo:- Terry and Hazel Flanigan 2024		  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_YesNo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BANNER_YN, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXT_YN, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXT_YN3, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_BANNER_YN, App->CL_Dialogs->MessageString);
		SetDlgItemText(hDlg, IDC_STTEXT_YN, App->CL_Dialogs->MessageString2);
		SetDlgItemText(hDlg, IDC_STTEXT_YN3, App->CL_Dialogs->MessageString3);
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_BANNER_YN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTEXT_YN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTEXT_YN3) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}

	return FALSE;
}

// *************************************************************************
// *	  	Start_Speed_Camera_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Start_Speed_Camera_Dlg()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_MOUSESENSITIVITY, App->MainHwnd, (DLGPROC)Proc_Speed_Camera);
}

// *************************************************************************
// *		Proc_Speed_Camera:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Speed_Camera(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STBANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CKNORMAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKSLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKVERYSLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKFAST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STBANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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
	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_CKNORMAL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_Mouse_Normal);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKSLOW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_Mouse_Slow);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKVERYSLOW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_Mouse_VerySlow);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKFAST)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_Mouse_Fast);
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

		if (LOWORD(wParam) == IDC_CKNORMAL)
		{
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 50;
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 50;

			App->CL_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CL_Dialogs->flag_Mouse_Normal = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKSLOW)
		{
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 5;
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 20;
			
			App->CL_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CL_Dialogs->flag_Mouse_Slow = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKVERYSLOW)
		{
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 1;
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 1;
			
			App->CL_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CL_Dialogs->flag_Mouse_VerySlow = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKFAST)
		{
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 250;
			App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 250;

			App->CL_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CL_Dialogs->flag_Mouse_Fast = 1;

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	return FALSE;
}

// *************************************************************************
// *	UnCheck_All_SpeedMouseOption:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void CL64_Dialogs::UnCheck_All_SpeedMouseOption()
{
	flag_Mouse_Normal = 0;
	flag_Mouse_Slow = 0;
	flag_Mouse_VerySlow = 0;
	flag_Mouse_Fast = 0;
}

// *************************************************************************
// *	  			Dialog_Text:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Dialogs::Dialog_Text(int What_Check)
{
	Canceled = 0;
	What_Check_Name = What_Check;

	DialogBox(App->hInst, (LPCTSTR)IDD_TEXT_DIALOG, App->MainHwnd, (DLGPROC)Proc_Dialog_Text);

	return 1;
}

// **************************************************************************
// *				Proc_Dialog_Text:- Terry and Hazel Flanigan 2024		*
// **************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Dialog_Text(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_TITLENAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_EDITTEXT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_TITLENAME, (LPCTSTR)App->CL_Dialogs->btext);
		SetDlgItemText(hDlg, IDC_EDITTEXT, (LPCTSTR)App->CL_Dialogs->Chr_Text);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_TITLENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

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
		if (LOWORD(wParam) == IDOK)
		{
			char buff[255];
			GetDlgItemText(hDlg, IDC_EDITTEXT, (LPTSTR)buff, 255);

			strcpy(App->CL_Dialogs->Chr_Text, buff);

			App->CL_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *	  Start_TextureViewer_Dialog:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Dialogs::Start_TextureViewer_Dialog(char* TextureFile, HWND Owner_hDlg)
{
	strcpy(mTextureFile, TextureFile);
	DialogBox(App->hInst, (LPCTSTR)IDD_TEXTUREVIEWER, Owner_hDlg, (DLGPROC)Proc_TextureViewer);
}

// **************************************************************************
// *		Proc_TextureViewer:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_TextureViewer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_DETAILS, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_VIEWEXPORT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SetWindowLongPtr(GetDlgItem(hDlg, IDC_BASETEXTURE), GWLP_WNDPROC, (LONG_PTR)Proc_ViewerBasePic);

		App->CL_Dialogs->RightGroups_Hwnd = hDlg;
		App->CL_Textures->Texture_To_HBITMP(App->CL_Dialogs->mTextureFile);

		char buf[MAX_PATH]{ 0 };
		
		{
			sprintf(buf, "%i X %i   %s", App->CL_Textures->BasePicWidth
				, App->CL_Textures->BasePicHeight, App->CL_Resources->mSelected_File);
		}

		SetDlgItemText(hDlg, IDC_ST_DETAILS, (LPCTSTR)buf);
		

		App->CL_Ogre->RenderFrame(8);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_DETAILS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		if (some_item->idFrom == IDC_BT_VIEWEXPORT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_VIEWEXPORT));
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
		
		if (LOWORD(wParam) == IDC_BT_VIEWEXPORT)
		{
			App->CL_Resources->Export_File(App->CL_Resources->mSelected_File);
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			remove(App->CL_Dialogs->mTextureFile);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			remove(App->CL_Dialogs->mTextureFile);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *		Proc_ViewerBasePic:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
bool CALLBACK CL64_Dialogs::Proc_ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (msg == WM_PAINT)
	{
		PAINTSTRUCT	ps;
		HDC			hDC;
		RECT		Rect;

		hDC = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &Rect);
		Rect.left--;
		Rect.bottom--;
		FillRect(hDC, &Rect, (HBRUSH)(RGB(0, 255, 0)));

		if (App->CL_Dialogs->Sel_BaseBitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = App->CL_Textures->BasePicHeight;
			Source.right = App->CL_Textures->BasePicWidth;

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CL_Textures->RenderTexture_Blit(hDC, App->CL_Dialogs->Sel_BaseBitmap, &Source, &Dest);
			ReleaseDC(hwnd, hDC);
		}

		EndPaint(hwnd, &ps);

		return 0;
	}

	return 0;
}

// *************************************************************************
// *	  Start_FileViewer_Dialog:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Start_FileViewer_Dialog(char* FFile, HWND Owner_hDlg)
{
	strcpy(mFile, FFile);
	DialogBox(App->hInst, (LPCTSTR)IDD_FILEVIEWER, Owner_hDlg, (DLGPROC)Proc_FileViewer);
}

// **************************************************************************
// *			Proc_FileViewer:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_FileViewer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_DETAILS, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_VIEWEXPORT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LST_FILE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		App->CL_Dialogs->FileViewer_Hwnd = hDlg;
		
		SetDlgItemText(hDlg, IDC_ST_DETAILS, (LPCTSTR)App->CL_Resources->mSelected_File);

		App->CL_Dialogs->Read_File(App->CL_Dialogs->mFile, hDlg);

		//if (App->CL_Scene->GroupCount > 0)
		//{
		//	char Text[MAX_PATH];
		//	strcpy(Text, "material ");
		//	//strcat(Text, App->CL_Scene->Group[App->CL_Props_Textures->Selected_Group]->Ogre_Material);
		//	App->CL_Dialogs->Material_Search((LPSTR)Text);
		//}

		
		App->CL_Ogre->RenderFrame(8);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_DETAILS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		if (some_item->idFrom == IDC_BT_VIEWEXPORT)
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

		if (LOWORD(wParam) == IDC_BT_VIEWEXPORT)
		{
			App->CL_Resources->Export_File(App->CL_Resources->mSelected_File);
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			remove(App->CL_Dialogs->mFile);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			remove(App->CL_Dialogs->mFile);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *		 	Read_File:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Dialogs::Read_File(char* mFile, HWND hDlg)
{
	FILE* fp = NULL;

	char buffer[MAX_PATH]{ 0 };
	char bufTabs[MAX_PATH]{ 0 };
	char bufferOut[MAX_PATH]{ 0 };

	int TabCount = 0;
	int Count = 0;
	int len = 0;

	strcpy(bufTabs, "");

	fp = fopen(mFile, "r");
	if (!fp)
	{
		App->Say("Cant Find File");
		return;
	}

	while (!feof(fp))
	{
		bufTabs[0] = 0;
		buffer[0] = 0;
		bufferOut[0] = 0;

		fgets(buffer, MAX_PATH, fp);

		TabCount = 0;
		Count = 0;
		len = strlen(buffer);

		while (Count < len)
		{
			if (buffer[Count] == '\t')
			{
				TabCount++;
			}

			Count++;
		}

		Count = 0;
		while (Count < TabCount)
		{
			strcat(bufTabs, "    ");
			Count++;
		}

		strcat(bufTabs, buffer);
		strcpy(bufferOut, bufTabs);
	
		SendDlgItemMessage(hDlg, IDC_LST_FILE, LB_ADDSTRING, (WPARAM)0, (LPARAM)bufferOut);
	}

	fclose(fp);

	_unlink(mFile);

}

// *************************************************************************
// *		 	Material_Search:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::Material_Search(char* ItemString)
{
	int Index = SendDlgItemMessage(FileViewer_Hwnd, IDC_LST_FILE, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)ItemString);
	SendDlgItemMessage(FileViewer_Hwnd, IDC_LST_FILE, LB_SETTOPINDEX, (WPARAM)Index, (LPARAM)0);
}

// *************************************************************************
// *	  	Dialog_TrueFlase:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Dialogs::Dialog_TrueFlase(HWND Parent)
{
	DialogBox(App->hInst, (LPCTSTR)IDD_TRUEFALSE, Parent, (DLGPROC)Proc_Dialog_TrueFlase);
	return 1;
}
// *************************************************************************
// *						Dialog_TrueFlase_Proc	  					   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Dialog_TrueFlase(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//App->SetTitleBar(hDlg);
		SendDlgItemMessage(hDlg, IDC_TITLENAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CHECKNO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CHECKYES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_TITLENAME, (LPCTSTR)App->CL_Dialogs->btext);


		if (App->CL_Dialogs->TrueFlase == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CHECKYES);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			temp = GetDlgItem(hDlg, IDC_CHECKNO);
			SendMessage(temp, BM_SETCHECK, 0, 0);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CHECKYES);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			temp = GetDlgItem(hDlg, IDC_CHECKNO);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_TITLENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_CHECKYES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_CHECKNO) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CHECKYES)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CHECKYES);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			temp = GetDlgItem(hDlg, IDC_CHECKNO);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			App->CL_Dialogs->TrueFlase = 1;
		}

		if (LOWORD(wParam) == IDC_CHECKNO)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CHECKNO);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			temp = GetDlgItem(hDlg, IDC_CHECKYES);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			App->CL_Dialogs->TrueFlase = 0;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;
}

// *************************************************************************
// *	  	Dialog_Counter():- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::Dialog_Counter()
{
	Canceled = 0;

	DialogBox(App->hInst, (LPCTSTR)IDD_PROPS_COUNTER, App->Fdlg, (DLGPROC)Proc_Dialog_Counter);

}

// *************************************************************************
// *        Proc_Dialog_Counter:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Dialog_Counter(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//App->SetTitleBar(hDlg);

		SendDlgItemMessage(hDlg, IDC_EDTRIGGERVALUE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
	
		SendDlgItemMessage(hDlg, IDC_STTRIGGERVALUE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STMATHS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_CT_COUNTER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_ENABLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CT_MATHS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CB_COUNTERS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STBANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_CT_MATHS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Dialogs->UpDate_Counter_Dialog(hDlg);
		App->CL_Dialogs->Set_Counter_Dialog_Details(hDlg);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STMATHS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_STTRIGGERVALUE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_CT_MATHS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STBANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_ENABLE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_CT_COUNTER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		if (some_item->idFrom == IDC_BT_CT_MATHS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		//if (LOWORD(wParam) == IDC_BT_CT_MATHS)
		//{
		//	int Index = App->SBC_Properties->Current_Selected_Object;

		//	// Collectables
		//	if (App->SBC_Properties->Edit_Category == Enums::Edit_Collectable)
		//	{
		//		int Counter_Index = App->SBC_Scene->V_Object[Index]->S_Collectable[0]->Counter_ID;

		//		strcpy(App->Cl_Dialogs->btext, "Set Maths Option");

		//		if (App->SBC_Scene->V_Object[Index]->S_Collectable[0]->Maths == 1)
		//		{
		//			strcpy(App->SBC_Dialogs->Chr_DropText, "Add");
		//		}

		//		if (App->SBC_Scene->V_Object[Index]->S_Collectable[0]->Maths == 2)
		//		{
		//			strcpy(App->SBC_Dialogs->Chr_DropText, "Subtract");
		//		}

		//		App->SBC_Dialogs->DropList_Data = Enums::DropDialog_Maths;
		//		App->SBC_Dialogs->Dialog_DropGen();

		//		if (App->SBC_Dialogs->Canceled == 0)
		//		{
		//			int TestChr;

		//			// Add
		//			TestChr = strcmp(App->SBC_Dialogs->Chr_DropText, "Add");
		//			if (TestChr == 0)
		//			{
		//				App->SBC_Scene->V_Object[Index]->S_Collectable[0]->Maths = 1;
		//			}

		//			// Subtract
		//			TestChr = strcmp(App->SBC_Dialogs->Chr_DropText, "Subtract");
		//			if (TestChr == 0)
		//			{
		//				App->SBC_Scene->V_Object[Index]->S_Collectable[0]->Maths = 2;
		//			}

		//			App->SBC_Dialogs->UpDate_Counter_Dialog(hDlg);
		//		}

		//	}

		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDC_CB_COUNTERS)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				int ObjectIndex = App->CL_Properties->Current_Selected_Object;
				
				char buff[MAX_PATH]{ 0 };
				HWND temp = GetDlgItem(hDlg, IDC_CB_COUNTERS);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				strcpy(App->CL_Scene->V_Object[ObjectIndex]->S_Teleport[0]->Counter_Name, buff);
				int CounterIndex = App->CL_Display->GetIndex_By_Name(buff);

				App->CL_Scene->V_Object[ObjectIndex]->S_Teleport[0]->Counter_ID = CounterIndex;
				 
			}
			}

			return TRUE;
		}

		//	if (App->SBC_Properties->Edit_Category == Enums::Edit_Message)
		//	{
		//		int Counter_Index = App->SBC_Scene->V_Object[Index]->S_Message[0]->Counter_ID;

		//		strcpy(App->SBC_Dialogs->Chr_DropText, App->SBC_Scene->B_Counter[Counter_Index]->Panel_Name);
		//		App->SBC_Dialogs->DropList_Data = Enums::DropDialog_Counters;
		//		App->SBC_Dialogs->Dialog_DropGen();

		//		if (App->SBC_Dialogs->Canceled == 0)
		//		{
		//			strcpy(App->SBC_Scene->V_Object[Index]->S_Message[0]->Counter_Name, App->SBC_Dialogs->Chr_DropText);

		//			int CounterIndex = App->SBC_Display->GetIndex_By_Name(App->SBC_Dialogs->Chr_DropText);

		//			App->SBC_Scene->V_Object[Index]->S_Message[0]->Counter_ID = CounterIndex;

		//		}

		//		App->SBC_Dialogs->UpDate_Counter_Dialog(hDlg);
		//	}

		//	// Collectables
		//	if (App->SBC_Properties->Edit_Category == Enums::Edit_Collectable)
		//	{
		//		int Counter_Index = App->SBC_Scene->V_Object[Index]->S_Collectable[0]->Counter_ID;

		//		strcpy(App->SBC_Dialogs->Chr_DropText, App->SBC_Scene->B_Counter[Counter_Index]->Panel_Name);
		//		App->SBC_Dialogs->DropList_Data = Enums::DropDialog_Counters;
		//		App->SBC_Dialogs->Dialog_DropGen();

		//		if (App->SBC_Dialogs->Canceled == 0)
		//		{
		//			strcpy(App->SBC_Scene->V_Object[Index]->S_Collectable[0]->Counter_Name, App->SBC_Dialogs->Chr_DropText);

		//			int CounterIndex = App->SBC_Display->GetIndex_By_Name(App->SBC_Dialogs->Chr_DropText);

		//			App->SBC_Scene->V_Object[Index]->S_Collectable[0]->Counter_ID = CounterIndex;

		//		}

		//		App->SBC_Dialogs->UpDate_Counter_Dialog(hDlg);
		//		return TRUE;
		//	}

		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDC_CK_ENABLE)
		{
			int Index = App->CL_Properties->Current_Selected_Object;

			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Dialogs->Set_Counter_Dialog(hDlg, false);

				// Move Entity
				if (App->CL_Properties->Edit_Category == Enums::Edit_Move_Entity)
				{
					App->CL_Scene->V_Object[Index]->S_MoveType[0]->Counter_Disabled = 1;
					return 1;
				}

				// Messages
				if (App->CL_Properties->Edit_Category == Enums::Edit_Message)
				{
					App->CL_Scene->V_Object[Index]->S_Message[0]->Counter_Disabled = 1;
					return 1;
				}

				// Collectables
				if (App->CL_Properties->Edit_Category == Enums::Edit_Collectable)
				{
					App->CL_Scene->V_Object[Index]->S_Collectable[0]->Counter_Disabled = 1;
					return 1;
				}

				// Teleporters
				if (App->CL_Properties->Edit_Category == Enums::Edit_Teleport)
				{
					App->CL_Scene->V_Object[Index]->S_Teleport[0]->Counter_Disabled = 1;
					return 1;
				}

				return 1;
			}
			else
			{
				// Move Entity
				App->CL_Dialogs->Set_Counter_Dialog(hDlg, true);

				if (App->CL_Properties->Edit_Category == Enums::Edit_Move_Entity)
				{
					App->CL_Scene->V_Object[Index]->S_MoveType[0]->Counter_Disabled = 0;
					return 1;
				}

				// Messages
				if (App->CL_Properties->Edit_Category == Enums::Edit_Message)
				{
					App->CL_Scene->V_Object[Index]->S_Message[0]->Counter_Disabled = 0;
					return 1;
				}

				// Collectables
				if (App->CL_Properties->Edit_Category == Enums::Edit_Collectable)
				{
					App->CL_Scene->V_Object[Index]->S_Collectable[0]->Counter_Disabled = 0;
					return 1;
				}

				// Teleporters
				if (App->CL_Properties->Edit_Category == Enums::Edit_Teleport)
				{
					App->CL_Scene->V_Object[Index]->S_Teleport[0]->Counter_Disabled = 0;
					return 1;
				}

				return 1;
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			char buff[256];
			int result = 0;
			GetDlgItemText(hDlg, IDC_EDTRIGGERVALUE, (LPTSTR)buff, 256);
			strcpy(App->CL_Dialogs->Chr_Int, buff);
			App->CL_Dialogs->mInt = atoi(buff);

			if (App->CL_Properties->Edit_Category == Enums::Edit_Move_Entity)
			{
				int Index = App->CL_Properties->Current_Selected_Object;
				App->CL_Scene->V_Object[Index]->S_MoveType[0]->Trigger_Value = atoi(buff);
			}

			if (App->CL_Properties->Edit_Category == Enums::Edit_Message)
			{
				int Index = App->CL_Properties->Current_Selected_Object;
				App->CL_Scene->V_Object[Index]->S_Message[0]->Trigger_Value = atoi(buff);
			}

			// Collectables
			if (App->CL_Properties->Edit_Category == Enums::Edit_Collectable)
			{
				int Index = App->CL_Properties->Current_Selected_Object;
				App->CL_Scene->V_Object[Index]->S_Collectable[0]->Value = atoi(buff);
			}

			// Teleprters
			if (App->CL_Properties->Edit_Category == Enums::Edit_Teleport)
			{
				int Index = App->CL_Properties->Current_Selected_Object;
				App->CL_Scene->V_Object[Index]->S_Teleport[0]->Trigger_Value = atoi(buff);
			}

			App->CL_Dialogs->Canceled = 0;
			//App->Cl_Dialogs->Active_Dlg_Int = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Dialogs->Canceled = 1;
			//App->Cl_Dialogs->Active_Dlg_Int = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  	UpDate_Counter_Dialog:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Dialogs::UpDate_Counter_Dialog(HWND hDlg)
{
	int Index = App->CL_Properties->Current_Selected_Object;

	// Move Entity
	if (App->CL_Properties->Edit_Category == Enums::Edit_Move_Entity)
	{
		if (App->CL_Scene->V_Object[Index]->S_MoveType[0]->Counter_Disabled == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			Set_Counter_Dialog(hDlg, false);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			Set_Counter_Dialog(hDlg, true);
		}

		char chr_TriggerVal[20];
		_itoa(App->CL_Scene->V_Object[Index]->S_MoveType[0]->Trigger_Value, chr_TriggerVal, 10);
		SetDlgItemText(hDlg, IDC_EDTRIGGERVALUE, (LPCTSTR)chr_TriggerVal);

		return 1;
	}

	// Messages
	if (App->CL_Properties->Edit_Category == Enums::Edit_Message)
	{
		if (App->CL_Scene->V_Object[Index]->S_Message[0]->Counter_Disabled == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			Set_Counter_Dialog(hDlg, false);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			Set_Counter_Dialog(hDlg, true);
		}

		char chr_TriggerVal[20];
		_itoa(App->CL_Scene->V_Object[Index]->S_Message[0]->Trigger_Value, chr_TriggerVal, 10);
		SetDlgItemText(hDlg, IDC_EDTRIGGERVALUE, (LPCTSTR)chr_TriggerVal);

		return 1;
	}

	// Collectables
	if (App->CL_Properties->Edit_Category == Enums::Edit_Collectable)
	{
		if (App->CL_Scene->V_Object[Index]->S_Collectable[0]->Counter_Disabled == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			Set_Counter_Dialog(hDlg, false);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			Set_Counter_Dialog(hDlg, true);
		}

		char chr_TriggerVal[20];
		_itoa(App->CL_Scene->V_Object[Index]->S_Collectable[0]->Value, chr_TriggerVal, 10);
		SetDlgItemText(hDlg, IDC_EDTRIGGERVALUE, (LPCTSTR)chr_TriggerVal);


		if (App->CL_Scene->V_Object[Index]->S_Collectable[0]->Maths == 1)
		{
			SetDlgItemText(hDlg, IDC_STMATHS, (LPCTSTR)"Add");
		}

		if (App->CL_Scene->V_Object[Index]->S_Collectable[0]->Maths == 2)
		{
			SetDlgItemText(hDlg, IDC_STMATHS, "Subtract");
		}

		return 1;
	}

	// Teleporters
	if (App->CL_Properties->Edit_Category == Enums::Edit_Teleport)
	{
		if (App->CL_Scene->V_Object[Index]->S_Teleport[0]->Counter_Disabled == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			Set_Counter_Dialog(hDlg, false);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ENABLE);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			Set_Counter_Dialog(hDlg, true);
		}

		char chr_TriggerVal[20];
		_itoa(App->CL_Scene->V_Object[Index]->S_Teleport[0]->Trigger_Value, chr_TriggerVal, 10);
		SetDlgItemText(hDlg, IDC_EDTRIGGERVALUE, (LPCTSTR)chr_TriggerVal);

		return 1;
	}

	return 1;
}

// *************************************************************************
// *	 Set_Counter_Dialog_Details- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Set_Counter_Dialog_Details(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_CB_COUNTERS, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	int Count = 0;

	while (Count < App->CL_Scene->Counters_Count)
	{
		if (App->CL_Scene->B_Counter[Count]->Deleted == 0)
		{
			SendDlgItemMessage(hDlg, IDC_CB_COUNTERS, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)App->CL_Scene->B_Counter[Count]->Panel_Name);
		}

		Count++;
	}

	SendDlgItemMessage(hDlg, IDC_CB_COUNTERS, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

}

// *************************************************************************
// *	  	Set_Counter_Dialog:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::Set_Counter_Dialog(HWND hDlg, bool Enable)
{
	EnableWindow(GetDlgItem(hDlg, IDC_CB_COUNTERS), Enable);
	
	EnableWindow(GetDlgItem(hDlg, IDC_STMATHS), Enable);
	EnableWindow(GetDlgItem(hDlg, IDC_BT_CT_MATHS), Enable);

	EnableWindow(GetDlgItem(hDlg, IDC_EDTRIGGERVALUE), Enable);
}

// *************************************************************************
// *	  	Dialog_Text_Props:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::Dialog_Text_Props()
{
	Canceled = 0;

	DialogBox(App->hInst, (LPCTSTR)IDD_PROP_TEXT, App->Fdlg, (DLGPROC)Proc_Dialog_Text_Props);

}

// **************************************************************************
// *		Proc_Dialog_Text_Props:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Dialog_Text_Props(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		SendDlgItemMessage(hDlg, IDC_TITLENAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_EDITTEXT, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_TITLENAME, (LPCTSTR)App->CL_Dialogs->btext);

		SetDlgItemText(hDlg, IDC_EDITTEXT, (LPCTSTR)App->CL_Dialogs->Chr_Text);


		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_TITLENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

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
		if (LOWORD(wParam) == IDOK)
		{
			char buff[255];
			GetDlgItemText(hDlg, IDC_EDITTEXT, (LPTSTR)buff, 255);

			strcpy(App->CL_Dialogs->Chr_Text, buff);

			App->CL_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *	  		Dialog_Int:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Dialogs::Dialog_Int()
{
	if (flag_Dlg_Int_Active == 1)
	{
		return;
	}

	flag_Dlg_Int_Active = 1;

	DialogBox(App->hInst, (LPCTSTR)IDD_EDITFLOAT, App->Fdlg, (DLGPROC)Proc_Dialog_Int);

}

// *************************************************************************
// *        Proc_Dialog_Int:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Dialog_Int(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_EDIT1, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_BANNER, (LPCTSTR)App->CL_Dialogs->btext);

		SetDlgItemText(hDlg, IDC_EDIT1, (LPCTSTR)App->CL_Dialogs->Chr_Int);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{

		if (GetDlgItem(hDlg, IDC_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

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
		if (LOWORD(wParam) == IDOK)
		{
			char buff[256];
			int result = 0;
			GetDlgItemText(hDlg, IDC_EDIT1, (LPTSTR)buff, 256);
			strcpy(App->CL_Dialogs->Chr_Int, buff);
			App->CL_Dialogs->mInt = atoi(buff);

			App->CL_Dialogs->Canceled = 0;
			App->CL_Dialogs->flag_Dlg_Int_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Dialogs->Canceled = 1;
			App->CL_Dialogs->flag_Dlg_Int_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	return FALSE;
}
