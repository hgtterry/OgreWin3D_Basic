#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "Test_Lib_X.h"

Test_Lib_X::Test_Lib_X(void)
{
}

Test_Lib_X::~Test_Lib_X(void)
{
}

// *************************************************************************
// *				Try:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void Test_Lib_X::Try(void)
{
	//App->
	MessageBox(NULL, (LPTSTR)"Lib", (LPTSTR)"Lib", MB_OK);
}

// *************************************************************************
// *	  			Message:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void Test_Lib_X::Message(char* pString, char* pString2)
{
	/*Message_Text_Header[0] = 0;
	Message_Text_Message[0] = 0;

	strcpy(Message_Text_Header, " ");
	strcat(Message_Text_Header, pString);
	strcat(Message_Text_Header, " ");

	strcpy(Message_Text_Message, " ");
	strcat(Message_Text_Message, pString2);
	strcat(Message_Text_Message, " ");

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);*/
	//DialogBox(App->hInst, (LPCTSTR)IDD_MESSAGE, App->MainHwnd, (DLGPROC)Proc_Message);
}

// *************************************************************************
// *        	Message_Proc:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK Test_Lib_X::Proc_Message(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		/*SendDlgItemMessage(hDlg, IDC_STTEXT, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STMESSAGE, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_STTEXT, App->CL_Dialogs->Message_Text_Header);
		SetDlgItemText(hDlg, IDC_STMESSAGE, App->CL_Dialogs->Message_Text_Message);*/

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_BANNER) == (HWND)lParam)
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
		}*/

		return FALSE;
	}

	/*case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}*/

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			//App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			//App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	return FALSE;
}
