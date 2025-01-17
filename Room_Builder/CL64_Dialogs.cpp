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

typedef struct TexInfoTag
{
	Ogre::Vector3 VecNormal;
	float xScale, yScale;
	int xShift, yShift;
	float	Rotate;			// texture rotation angle in degrees
	TexInfo_Vectors TVecs;
	int Dib;				// index into the wad
	char Name[16];
	signed int DirtyFlag;
	Ogre::Vector3 Pos;
	int txSize, tySize;		// texture size (not currently used)
	Matrix3d XfmFaceAngle;	// face rotation angle
} TexInfo;

typedef struct FaceTag
{
	int			NumPoints;
	int			Flags;
	Plane		Face_Plane;
	int			LightIntensity;
	geFloat		Reflectivity;
	geFloat		Translucency;
	geFloat		MipMapBias;
	geFloat		LightXScale, LightYScale;
	TexInfo		Tex;
	Ogre::Vector3* Points;
} Face;

struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	geBoolean Dirty;
	Box3d Bounds;
};

struct tag_BrushList
{
	Brush* First;
	Brush* Last;
};

enum BrushFlags
{
	BRUSH_SOLID = 0x0001,
	BRUSH_WINDOW = 0x0002,
	BRUSH_WAVY = 0x0004,
	BRUSH_DETAIL = 0x0008,	//not included in vis calculations
	BRUSH_HOLLOWCUT = 0x0010,
	BRUSH_TRANSLUCENT = 0x0020,
	BRUSH_EMPTY = 0x0040,
	BRUSH_SUBTRACT = 0x0080,
	BRUSH_CLIP = 0x0100,
	BRUSH_FLOCKING = 0x0200,
	BRUSH_HOLLOW = 0x0400,
	BRUSH_SHEET = 0x0800,
	BRUSH_HIDDEN = 0x1000,
	BRUSH_LOCKED = 0x2000,
	BRUSH_HINT = 0x4000,
	BRUSH_AREA = 0x8000
};

CL64_Dialogs::CL64_Dialogs(void)
{
	Message_Text_Header[0] = 0;
	Message_Text_Message[0] = 0;

	MessageString[0] = 0;
	MessageString2[0] = 0;

	btext[0] = 0;
	Chr_Text[0] = 0;;

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
			App->CL_Dialogs->flag_Dlg_Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
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
	//What_Check_Name = What_Check;

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
		//App->SetTitleBar(hDlg);

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

			/*if (App->CL_Dialogs->What_Check_Name == Enums::Check_Name_Brushes)
			{
				bool test = App->CL_Brush->Check_if_Name_Exist(buff);
				if (test == 1)
				{
					App->Say("Brush Name Exist");
					return TRUE;
				}
			}*/

			
			App->CL_Dialogs->flag_Dlg_Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
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
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_RENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

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

		if (some_item->idFrom == IDC_BT_BRUSH_RENAME)
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

		if (LOWORD(wParam) == IDC_BT_BRUSH_RENAME)
		{
			char Name[MAX_PATH];

			strcpy(App->CL_Dialogs->btext, "Change File Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Properties_Brushes->Selected_Brush->Name);

			App->CL_Dialogs->Dialog_Text(Enums::Check_Name_Brushes);

			if (App->CL_Dialogs->flag_Dlg_Canceled == 0)
			{
				strcpy(Name, App->CL_Dialogs->Chr_Text);
			}
			else
			{
				return TRUE;
			}

			App->CL_Brush->Brush_SetName(App->CL_Properties_Brushes->Selected_Brush, Name);

			App->CL_Dialogs->Fill_Brush_Combo(hDlg);
			App->CL_Dialogs->List_BrushData(hDlg);
			App->CL_Properties_Brushes->Fill_ListBox();

			//App->m_pDoc->SetModifiedFlag(TRUE);

			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Dialogs->flag_boolBrush_Properties_Dialog_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
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

	Level* pLevel = App->CL_Doc->pLevel;
	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

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

	// ----------------------------------- Data
	sprintf(buf, "%s%d", "Model ID ", b->ModelId);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%d", "Group ID ", b->GroupId);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%f", "Hull Size ", b->HullSize);
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

	//// Bounding Box
	//sprintf(buf, "Max X = %f Y = %f Z = %f", b->BoundingBox.Max.X, b->BoundingBox.Max.Y, b->BoundingBox.Max.Z);
	//SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	//sprintf(buf, "Min X = %f Y = %f Z = %f", b->BoundingBox.Min.X, b->BoundingBox.Min.Y, b->BoundingBox.Min.Z);
	//SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
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
		Debug
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
			if (!Show_Face_Data(i, pList->Faces[i], hDlg)) return 0;
		}

		sprintf(buf, "%s", " -------------------------------------------");
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

	sprintf(buf, "%s", " -------------------------------------------");
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "Face Index", Index + 1);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%i", "Flags ", f->Flags);
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
	geFloat U, V;

	int txSize, tySize;

	App->CL_Face->Face_GetTextureSize(f, &txSize, &tySize);

	// make sure that the texture size is set correctly (division!)
	if (txSize == 0)
		txSize = 32;
	if (tySize == 0)
		tySize = 32;

	App->CL_Maths->Vector3_Scale(&TVecs->uVec, 1.f / (geFloat)txSize, &uVec);
	App->CL_Maths->Vector3_Scale(&TVecs->vVec, -1.f / (geFloat)tySize, &vVec);

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

	return 1;
}
