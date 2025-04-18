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
#include "CL64_Dialogs.h"
#include "Structures.cpp"

CL64_Dialogs::CL64_Dialogs(void)
{
	Message_Text_Header[0] = 0;
	Message_Text_Message[0] = 0;

	MessageString[0] = 0;
	MessageString2[0] = 0;

	TextureView_Hwnd = NULL;
	Sel_BaseBitmap = NULL;

	m_ListType = Enums::ListBox_None;

	btext[0] = 0;
	Chr_Text[0] = 0;;
	mTextureFile[0] = 0;
	Face_Index = 0;
	Check_What = 0;

	flag_Dlg_Canceled = 0;
	flag_boolBrush_Properties_Dialog_Active = 0;
}

CL64_Dialogs::~CL64_Dialogs(void)
{
}

// *************************************************************************
// *	  			YesNo:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Dialogs::YesNo(const char* Text, const char* Text2)
{
	flag_Dlg_Canceled = 0;
	MessageString[0] = 0;
	MessageString2[0] = 0;

	strcpy(MessageString, Text);
	strcpy(MessageString2, Text2);

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	DialogBox(App->hInst, (LPCTSTR)IDD_YESNO_DLG, App->MainHwnd, (DLGPROC)Proc_YesNo);
	
}

// *************************************************************************
// *		  Proc_YesNo:- Terry and Hazel Flanigan 2025		 		   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_YesNo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXT, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_BANNER, App->CL_Dialogs->MessageString);
		SetDlgItemText(hDlg, IDC_STTEXT, App->CL_Dialogs->MessageString2);
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
		if (GetDlgItem(hDlg, IDC_STTEXT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		return FALSE;
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

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_Dialogs->flag_Dlg_Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_Dialogs->flag_Dlg_Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}

	return FALSE;
}

// *************************************************************************
// *	  		Dialog_Text:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Dialogs::Dialog_Text(int What_Check)
{
	flag_Dlg_Canceled = 0;
	Check_What = What_Check;
	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
	DialogBox(App->hInst, (LPCTSTR)IDD_TEXT_DIALOG, App->MainHwnd, (DLGPROC)Proc_Dialog_Text);
}

// **************************************************************************
// *			Dialog_Text_Proc:- Terry and Hazel Flanigan 2025			*
// **************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Dialog_Text(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		HFONT Font;
		Font = CreateFont(-20, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
		SendDlgItemMessage(hDlg, IDC_TITLENAME, WM_SETFONT, (WPARAM)Font, MAKELPARAM(TRUE, 0));

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

			if (App->CL_Dialogs->Check_What == Enums::Check_Names_Textures)
			{
				bool test = App->CL_TXL_Editor->Check_if_Name_Exist(buff);
				if (test == 1)
				{
					App->Say("Texture Name Exist");
					return TRUE;
				}
			}

			if (App->CL_Dialogs->Check_What == Enums::Check_Name_Brushes)
			{
				bool test = App->CL_Brush_X->Check_if_Brush_Name_Exist(buff);
				if (test == 1)
				{
					App->Say("Brush Name Exist");
					return TRUE;
				}
			}

			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_Dialogs->flag_Dlg_Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_Dialogs->flag_Dlg_Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
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

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
	DialogBox(App->hInst, (LPCTSTR)IDD_MESSAGE, App->MainHwnd, (DLGPROC)Proc_Message);
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
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	return FALSE;
}

// *************************************************************************
// *	  			PleaseWait:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Dialogs::PleaseWait()
{
	App->ViewPLeaseWait = CreateDialog(App->hInst, (LPCTSTR)IDD_PLEASEWAIT, App->MainHwnd, (DLGPROC)Proc_PleaseWait);
}

// *************************************************************************
// *			Proc_PleaseWait:- Terry and Hazel Flanigan 2025			   *
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
// *	  	Start_Properties_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Dialogs::Start_Brush_Properties_Dlg()
{
	if (flag_boolBrush_Properties_Dialog_Active == 0)
	{
		App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
		DialogBox(App->hInst, (LPCTSTR)IDD_SB_BRUSH_PROPERTIES, App->MainHwnd, (DLGPROC)Proc_Brush_Properties);
	}
}

// *************************************************************************
// *			Properties_Proc:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_Brush_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		App->CL_Dialogs->flag_boolBrush_Properties_Dialog_Active = 1;

		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_SELECTED_BRUSH, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_BP_SELECTEDBRUSHES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));


		App->CL_Dialogs->Fill_Brush_Combo(hDlg);
		App->CL_Dialogs->List_BrushData(hDlg);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BP_SELECTEDBRUSHES) == (HWND)lParam)
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
	{

		if (LOWORD(wParam) == IDC_CB_SELECTED_BRUSH)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_SELECTED_BRUSH);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);

				App->CL_Properties_Brushes->Selected_Index = Index;
				App->CL_Properties_Brushes->Selected_Brush = App->CL_Brush->Get_By_Index(Index);
				App->CL_Dialogs->List_BrushData(hDlg);

				//App->CL_Properties_Brushes->OnSelchangeBrushlist(Index, 1);
			}
			}

			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_Dialogs->flag_boolBrush_Properties_Dialog_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_Dialogs->flag_boolBrush_Properties_Dialog_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}

	return FALSE;
}

// *************************************************************************
// *	  	Fill_Brush_Combo:- Terry and Hazel Flanigan 2025	    	   *
// *************************************************************************
void CL64_Dialogs::Fill_Brush_Combo(HWND hDlg)
{

	SendDlgItemMessage(hDlg, IDC_CB_SELECTED_BRUSH, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	Level* pLevel = App->CL_Doc->Current_Level;
	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->Current_Level);

	int Count = 0;
	Brush* b;
	b = pList->First;
	while (b != NULL)
	{
		SendDlgItemMessage(hDlg, IDC_CB_SELECTED_BRUSH, CB_ADDSTRING, (WPARAM)0, (LPARAM)App->CL_Brush->Brush_GetName(b));
		Count++;
		b = b->Next;
	}

	SendDlgItemMessage(hDlg, IDC_CB_SELECTED_BRUSH, CB_SETCURSEL, (WPARAM)App->CL_Properties_Brushes->Selected_Index, (LPARAM)0);
}

// *************************************************************************
// *	  	   List_BrushData:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Dialogs::List_BrushData(HWND hDlg)
{

	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	Face_Index = 0;
	
	char buf[255];
	int Count = App->CL_Brush->Get_Brush_Count();

	if (Count > 0)
	{
		if (App->CL_Properties_Brushes->Selected_Brush && App->CL_Properties_Brushes->Selected_Index > -1)
		{
			Show_Brush_Info(App->CL_Properties_Brushes->Selected_Brush, hDlg);
		}
		else
		{
			sprintf(buf, "%s", "No Brush Selected");
			SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		}
	}
	else
	{
		sprintf(buf, "%s", "The World has No Brushes");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
}

// *************************************************************************
// *	  	Show_Brush_Info:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_Dialogs::Show_Brush_Info(const Brush* b, HWND hDlg)
{
	char buf[255];

	sprintf(buf, "%s%s", "Brush Name ", b->Name);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	// ----------------------------------- Flags
	if (b->Flags & BRUSH_SUBTRACT)
	{
		sprintf(buf, "%s%d%s", "Type Flags ", b->Flags, "  BRUSH_SUBTRACT");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else if (b->Flags & BRUSH_SOLID)
	{
		sprintf(buf, "%s%d%s", "Type Flags ", b->Flags, "  BRUSH_SOLID");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else if (b->Flags & BRUSH_HOLLOW)
	{
		sprintf(buf, "%s%d%s", "Type Flags ", b->Flags, "  Hollow Brush");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else
	{
		sprintf(buf, "%s%d%s", "Type Flags XX ", b->Flags, "  Unknown");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	// Main Brush test locked
	bool locked = App->CL_Brush->Brush_IsLocked(b);
	if (locked == 1)	
	{
		sprintf(buf, "%s %s", "Brush Locked ","Yes");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else
	{
		sprintf(buf, "%s %s", "Brush Locked ", "No");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	// ----------------------------------- Data
	sprintf(buf, "%s%d", "Model ID ", b->ModelId);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%d", "Group ID ", b->GroupId);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%f", "Hull Size ", b->HullSize);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%i", "Has Been Cut ", b->Has_Been_Cut);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	// ----------------------------------- Type
	if (b->Type == BRUSH_MULTI)
	{
		sprintf(buf, "%s%i%s", "Type ", b->Type, "  - BRUSH_MULTI");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else if (b->Type == BRUSH_LEAF)
	{
		sprintf(buf, "%s%i%s", "Type ", b->Type, "  - BRUSH_LEAF");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else if (b->Type == BRUSH_CSG)
	{
		sprintf(buf, "%s%i%s", "Type ", b->Type, "  - BRUSH_CSG");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else
	{
		sprintf(buf, "%s%i%s", "Type ", b->Type, "Unknown");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	if (b->Type == BRUSH_MULTI)
	{
		return Show_Brush_ListInfo(b->BList, hDlg);
	}
	if (b->Type == BRUSH_LEAF)
	{
		return Show_Brush_Faces_Info(b->Faces, hDlg);
	}
	if (b->Type == BRUSH_CSG)
	{
		App->Say("BRUSH_CSG");
	}

	return 1;
}

// *************************************************************************
// *	  	Show_Brush_ListInfo:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Dialogs::Show_Brush_ListInfo(BrushList* BList, HWND hDlg)
{
	char buf[MAX_PATH];
	Brush* pBrush;
	BrushIterator bi;
	int Count;

	Count = App->CL_Brush->BrushList_Count(BList, (BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF | BRUSH_COUNT_NORECURSE));
	if (Count < 0)
	{
		sprintf(buf, "%s%d", " ===== Sub Brushes ", Count);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		return 0;
	}
	else
	{
		sprintf(buf, "%s%d", " ===== Sub Brushes ", Count - 1);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		Show_Brush_Info(pBrush, hDlg);
		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}

	return 1;
}

// *************************************************************************
// *	  Show_Brush_Faces_Info:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Dialogs::Show_Brush_Faces_Info(const FaceList* pList, HWND hDlg)
{
	int i;
	char buf[MAX_PATH];

	if (pList->NumFaces < 0)
	{
	}
	else
	{
		sprintf(buf, "%s%d", "BrushFaces ", pList->NumFaces);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		for (i = 0; i < pList->NumFaces; i++)
		{
			if (!Show_Face_Data(Face_Index, pList->Faces[i], hDlg)) return 0;
			Face_Index++;
		}

		sprintf(buf, "%s", " ------");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	return 1;
}

// *************************************************************************
// *		  Show_Face_Data:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Dialogs::Show_Face_Data(int Index, const Face* f, HWND hDlg)
{
	char buf[MAX_PATH];

	int  i = 0;
	//int		i, xShift, yShift, Rotate;
	//geFloat xScale, yScale, rot;

	sprintf(buf, "%s %i", " --------------------- Face ", f->Real_Brush_Face_Index);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %s", "Brush: = ", App->CL_Face->Face_GetBrushName(f));
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "Face Index", Index + 1);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "Main_Brush_Face", f->Real_Brush_Face_Index);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "Cut_Brush_Index", f->Cut_Brush_Index);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "Flags", f->Flags);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%i", "Selected ", f->Selected);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%d", "NumPoints", f->NumPoints);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	for (i = 0; i < f->NumPoints; i++)
	{
		sprintf(buf, "XYZ %f %f %f", f->Points[i].x, f->Points[i].y, f->Points[i].z);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}


	const TexInfo_Vectors* TVecs = App->CL_Face->Face_GetTextureVecs(f);
	T_Vec3 uVec, vVec;
	float U, V;

	int txSize, tySize;

	App->CL_Face->Face_GetTextureSize(f, &txSize, &tySize);

	// make sure that the texture size is set correctly (division!)
	if (txSize == 0)
		txSize = 32;
	if (tySize == 0)
		tySize = 32;

	App->CL_Maths->Vector3_Scale(&TVecs->uVec, 1.f / (float)txSize, &uVec);
	App->CL_Maths->Vector3_Scale(&TVecs->vVec, -1.f / (float)tySize, &vVec);

	const T_Vec3* verts = App->CL_Face->Face_GetPoints(f);

	int j = 0;
	for (j = 0; j < f->NumPoints; j++)
	{
		U = App->CL_Maths->Vector3_DotProduct(&(verts[j]), &uVec);
		V = App->CL_Maths->Vector3_DotProduct(&(verts[j]), &vVec);
		U += (TVecs->uOffset / txSize);
		V -= (TVecs->vOffset / tySize);

		sprintf(buf, "UV %.3f %.3f", U, V);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	char buf2[MAX_PATH];
	strcpy(buf, App->CL_Face->Face_GetTextureName(f));
	sprintf(buf2, "%s%s", "Texture: = ", buf);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf2);

	bool faceLocked = App->CL_Face->Face_IsTextureLocked(f);
	if (faceLocked == 1)
	{
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Texture Locked Yes");
	}
	else
	{
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Texture Locked No");
	}

	return 1;
}

// *************************************************************************
// *	  		Dialog_SnapOptions:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Dialogs::Dialog_SnapOptions()
{
	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
	DialogBox(App->hInst, (LPCTSTR)IDD_SNAPOPTIONS, App->MainHwnd, (DLGPROC)Proc_SnapOptions);
}

// **************************************************************************
// *			Proc_SnapOptions:- Terry and Hazel Flanigan 2025			*
// **************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_SnapOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_USESNAP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_GRIDSNAP, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RADIO1, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RADIO2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RADIO4, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RADIO8, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RADIO16, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_RADIO32, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_ROTATIONSNAP, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_DEGREES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_DEGREES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_SNAP15, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SNAP30, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SNAP45, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SNAP60, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SNAP90, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		HWND Temp = GetDlgItem(hDlg, IDC_USESNAP);
		if (App->CL_Level->flag_UseGrid == 1)
		{
			SendMessage(Temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			SendMessage(Temp, BM_SETCHECK, 0, 0);
		}
		

		float fSnapSize = App->CL_Level->Level_GetGridSnapSize(App->CL_Doc->Current_Level);

		if (fSnapSize == 1)
		{
			SendDlgItemMessage(hDlg, IDC_RADIO1, BM_SETCHECK, BST_CHECKED, 0);
		}

		if (fSnapSize == 2)
		{
			SendDlgItemMessage(hDlg, IDC_RADIO2, BM_SETCHECK, BST_CHECKED, 0);
		}

		if (fSnapSize == 4)
		{
			SendDlgItemMessage(hDlg, IDC_RADIO4, BM_SETCHECK, BST_CHECKED, 0);
		}

		if (fSnapSize == 8)
		{
			SendDlgItemMessage(hDlg, IDC_RADIO8, BM_SETCHECK, BST_CHECKED, 0);
		}

		if (fSnapSize == 16)
		{
			SendDlgItemMessage(hDlg, IDC_RADIO16, BM_SETCHECK, BST_CHECKED, 0);
		}

		if (fSnapSize == 32)
		{
			SendDlgItemMessage(hDlg, IDC_RADIO32, BM_SETCHECK, BST_CHECKED, 0);
		}

		char buff[10];

		int fRotation = App->CL_Level->Level_GetRotationSnap(App->CL_Doc->Current_Level);
		SetDlgItemText(hDlg, IDC_ED_DEGREES, (LPCTSTR)_itoa(fRotation,buff,10));
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_GRIDSNAP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_ROTATIONSNAP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_DEGREES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
	
		if (GetDlgItem(hDlg, IDC_USESNAP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_RADIO1) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_RADIO2) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_RADIO4) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_RADIO8) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_RADIO16) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_RADIO32) == (HWND)lParam)
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

		if (some_item->idFrom == IDC_SNAP15)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_SNAP30)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_SNAP45)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_SNAP60)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_SNAP90)
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
		if (LOWORD(wParam) == IDC_USESNAP)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_USESNAP);

			if (App->CL_Level->flag_UseGrid == 1)
			{
				App->CL_Level->flag_UseGrid = 0;
				SendMessage(Temp, BM_SETCHECK, 0, 0);
				CheckMenuItem(App->Menu_Map, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_UNCHECKED);
			}
			else
			{
				App->CL_Level->flag_UseGrid = 1;
				SendMessage(Temp, BM_SETCHECK, 1, 0);
				CheckMenuItem(App->Menu_Map, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_CHECKED);
			}

			return TRUE;
		}
		

		if (LOWORD(wParam) == IDC_RADIO1)
		{
			App->CL_Editor_Map->GridSnapSize = 1;
			App->CL_Doc->Current_Level->GridSettings.TexelSnapSize = 1;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_RADIO2)
		{
			App->CL_Editor_Map->GridSnapSize = 2;
			App->CL_Doc->Current_Level->GridSettings.TexelSnapSize = 2;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_RADIO4)
		{
			App->CL_Editor_Map->GridSnapSize = 4;
			App->CL_Doc->Current_Level->GridSettings.TexelSnapSize = 4;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_RADIO8)
		{
			App->CL_Editor_Map->GridSnapSize = 8;
			App->CL_Doc->Current_Level->GridSettings.TexelSnapSize = 8;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_RADIO16)
		{
			App->CL_Editor_Map->GridSnapSize = 16;
			App->CL_Doc->Current_Level->GridSettings.TexelSnapSize = 16;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_RADIO32)
		{
			App->CL_Editor_Map->GridSnapSize = 32;
			App->CL_Doc->Current_Level->GridSettings.TexelSnapSize = 32;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_SNAP15)
		{
			App->CL_Doc->Current_Level->GridSettings.RotationSnap = 15;

			char buff[10];
			int fRotation = App->CL_Level->Level_GetRotationSnap(App->CL_Doc->Current_Level);
			SetDlgItemText(hDlg, IDC_ED_DEGREES, (LPCTSTR)_itoa(fRotation, buff, 10));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_SNAP30)
		{
			App->CL_Doc->Current_Level->GridSettings.RotationSnap = 30;

			char buff[10];
			int fRotation = App->CL_Level->Level_GetRotationSnap(App->CL_Doc->Current_Level);
			SetDlgItemText(hDlg, IDC_ED_DEGREES, (LPCTSTR)_itoa(fRotation, buff, 10));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_SNAP45)
		{
			App->CL_Doc->Current_Level->GridSettings.RotationSnap = 45;

			char buff[10];
			int fRotation = App->CL_Level->Level_GetRotationSnap(App->CL_Doc->Current_Level);
			SetDlgItemText(hDlg, IDC_ED_DEGREES, (LPCTSTR)_itoa(fRotation, buff, 10));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_SNAP60)
		{
			App->CL_Doc->Current_Level->GridSettings.RotationSnap = 60;

			char buff[10];
			int fRotation = App->CL_Level->Level_GetRotationSnap(App->CL_Doc->Current_Level);
			SetDlgItemText(hDlg, IDC_ED_DEGREES, (LPCTSTR)_itoa(fRotation, buff, 10));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_SNAP90)
		{
			App->CL_Doc->Current_Level->GridSettings.RotationSnap = 90;

			char buff[10];
			int fRotation = App->CL_Level->Level_GetRotationSnap(App->CL_Doc->Current_Level);
			SetDlgItemText(hDlg, IDC_ED_DEGREES, (LPCTSTR)_itoa(fRotation, buff, 10));
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
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
	mTextureFile[0] = 0;
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
		//SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_ST_DETAILS, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BT_VIEWEXPORT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetWindowLongPtr(GetDlgItem(hDlg, IDC_BASETEXTURE), GWLP_WNDPROC, (LONG_PTR)Proc_ViewerBasePic);

		App->CL_Dialogs->TextureView_Hwnd = hDlg;
		App->CL_Textures->Texture_To_HBITMP(App->CL_Dialogs->mTextureFile);

		//char buf[MAX_PATH];
		//if (App->CL_Scene->Scene_Mode == Enums::Scene_Mode_Assimp_Model)
		//{
		//	EnableWindow(GetDlgItem(hDlg, IDC_BT_VIEWEXPORT), false);

		//	sprintf(buf, "%i X %i", App->CL_Textures->BasePicWidth
		//		, App->CL_Textures->BasePicHeight);// , App->CL_Dialogs->mTextureFile);
		//}
		//else
		//{
		//	sprintf(buf, "%i X %i   %s", App->CL_Textures->BasePicWidth
		//		, App->CL_Textures->BasePicHeight, App->CL_Resources->mSelected_File);
		//}

		//SetDlgItemText(hDlg, IDC_ST_DETAILS, (LPCTSTR)buf);

		//App->CL_Ogre->RenderFrame(8);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_ST_DETAILS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		/*if (some_item->idFrom == IDC_BT_VIEWEXPORT)
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
		}*/

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

		/*if (LOWORD(wParam) == IDC_BT_VIEWEXPORT)
		{
			App->CL_Resources->Export_File(App->CL_Resources->mSelected_File);
			return TRUE;
		}*/

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
// *	  		Start_Gen_ListBox:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dialogs::Start_General_ListBox(int ListType)
{
	m_ListType = ListType;
	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
	DialogBox(App->hInst, (LPCTSTR)IDD_LISTDATA, App->MainHwnd, (DLGPROC)Proc_General_ListBox);
}

// *************************************************************************
// *		Proc_General_ListBox:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_Dialogs::Proc_General_ListBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LST_GENERAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		HWND List = GetDlgItem(hDlg, IDC_LST_GENERAL);

		if (App->CL_Dialogs->m_ListType == Enums::ListBox_Used_Textures)
		{
			App->CL_Dialogs->List_Used_Textures(List);
		}

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	break;
	case WM_CTLCOLORSTATIC:
	{

		return FALSE;
	}

	case WM_COMMAND:
	{
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
// *	  	List_Used_Textures:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dialogs::List_Used_Textures(HWND List)
{
	SendMessage(List, LB_RESETCONTENT, 0, 0);
	SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)" ------- Textures Used in Mesh");

	int Count = 0;
	memset(App->CL_Mesh_Mgr->UsedTextures, 0, 500);

	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->Current_Level);

	App->CL_Brush_X->BrushList_GetUsedTextures_X(pList, App->CL_Mesh_Mgr->UsedTextures);

	while (Count < App->CL_TXL_Editor->Texture_Count)
	{
		if (App->CL_Mesh_Mgr->UsedTextures[Count])
		{
			char matname[MAX_PATH];
			strncpy(matname, App->CL_TXL_Editor->Texture_List[Count]->Name, MAX_PATH - 1);
			SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)matname);
		}

		Count++;
	}
	
	SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)" -------  Textures Selected Brush");

	memset(App->CL_Mesh_Mgr->UsedTextures, 0, 500);

	int SB = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

	if (SB > 0)
	{
		BrushList* BList = NULL;
		BrushList* SBList = NULL;
		Brush* pBrush = NULL;
		BrushIterator bi;

		BList = App->CL_Level->Level_GetBrushes(App->CL_Doc->Current_Level);

		SBList = App->CL_Brush->BrushList_Create();
		pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);

		while (pBrush != NULL)
		{
			if (App->CL_SelBrushList->SelBrushList_Find(App->CL_Doc->pSelBrushes, pBrush))
			{
				Brush* pClone = App->CL_Brush->Brush_Clone(pBrush);
				App->CL_Brush->BrushList_Append(SBList, pClone);
			}

			pBrush = App->CL_Brush->BrushList_GetNext(&bi);
		}

		App->CL_Brush_X->BrushList_GetUsedTextures_X(SBList, App->CL_Mesh_Mgr->UsedTextures);
		
		Count = 0;
		while (Count < App->CL_TXL_Editor->Texture_Count)
		{
			if (App->CL_Mesh_Mgr->UsedTextures[Count])
			{
				char matname[MAX_PATH];
				strncpy(matname, App->CL_TXL_Editor->Texture_List[Count]->Name, MAX_PATH - 1);
				SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)matname);
			}

			Count++;
		}

		App->CL_Brush->BrushList_Destroy(&SBList);
	}

}
