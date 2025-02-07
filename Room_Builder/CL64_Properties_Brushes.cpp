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
#include "CL64_Properties_Brushes.h"
#include "Structures.cpp"

CL64_Properties_Brushes::CL64_Properties_Brushes()
{
	BrushesDlg_Hwnd = nullptr;
	Selected_Brush = nullptr;

	Selected_Index = 0;

	flag_Brushes_Dlg_Created = 0;
	flag_Dimension_Dlg_Active = 0;

	App->CL_Maths->Vector3_Clear(&FinalScale);
	App->CL_Maths->Vector3_Clear(&FinalRot);
	App->CL_Maths->Vector3_Clear(&CenterOfSelection);
	App->CL_Maths->Vector3_Clear(&Rotation);
	App->CL_Maths->Vector3_Clear(&Size);

	PosX_Delta = 10;
	PosY_Delta = 10;
	PosZ_Delta = 10;

	ScaleX_Delta = 1;
	ScaleY_Delta = 1;
	ScaleZ_Delta = 1;

	RotX_Delta = 45;
	RotY_Delta = 45;
	RotZ_Delta = 45;

	Rotation;
	Size;

	Dimensions_Dlg_hWnd = NULL;
}

CL64_Properties_Brushes::~CL64_Properties_Brushes()
{
}

// *************************************************************************
// *	  	Show_Brushes_Dialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Brushes::Show_Brushes_Dialog(bool Show)
{
	ShowWindow(BrushesDlg_Hwnd, Show);
}

// *************************************************************************
// *	  Enable_Options_Buttons:- Terry and Hazel Flanigan 2025		*
// *************************************************************************
void CL64_Properties_Brushes::Enable_Options_Buttons(bool Enable)
{
	EnableWindow(GetDlgItem(BrushesDlg_Hwnd, IDC_BT_GD_BRUSHPROPERTIES), Enable);
	EnableWindow(GetDlgItem(BrushesDlg_Hwnd, IDC_BT_BRUSH_DIMENSIONS), Enable);
	EnableWindow(GetDlgItem(BrushesDlg_Hwnd, IDC_BT_DELETE_SEL_BRUSH), Enable);

}

// *************************************************************************
// *	  	Start_Brush_Tabs_Dialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Brushes::Start_Brush_Tabs_Dialog()
{
	BrushesDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_BRUSHES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_Brush_Tabs);

	flag_Brushes_Dlg_Created = 1;

	Fill_ListBox();
	//Update_Dlg_Controls();
}

// *************************************************************************
// *			 Proc_Brush_Tabs:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Brushes::Proc_Brush_Tabs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_GD_BRUSHLIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GD_GROUPS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BRUSHCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_GD_BRUSHCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SELECTED_NUM, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_GD_BRUSHPROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_DIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DELETE_SEL_BRUSH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_GD_GROUPS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_BRUSHCOUNT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_GD_BRUSHCOUNT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_SELECTED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_SELECTED_NUM) == (HWND)lParam)
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

		if (some_item->idFrom == IDC_BT_GD_BRUSHPROPERTIES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_GD_BRUSHPROPERTIES));
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

		if (some_item->idFrom == IDC_BT_BRUSH_DIMENSIONS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_DIMENSIONS));
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

		if (some_item->idFrom == IDC_BT_DELETE_SEL_BRUSH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_DELETE_SEL_BRUSH));
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

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_DELETE_SEL_BRUSH)
		{
			App->CL_Dialogs->YesNo("Are you sure", "Do you want to Delete the selected Brushes");

			bool Doit = App->CL_Dialogs->flag_Dlg_Canceled;
			if (Doit == 0)
			{
				App->CL_Doc->DeleteCurrentThing();
				App->CL_Doc->ResetAllSelectedBrushes();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_DIMENSIONS)
		{
			App->CL_Properties_Brushes->Start_Dimensions_Dlg();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_GD_BRUSHLIST)
		{
			if (App->CL_Properties_Brushes->flag_Brushes_Dlg_Created == 1)
			{
				App->CL_Doc->DoGeneralSelect(false);
				App->CL_Properties_Brushes->List_Selection_Changed(1);
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_GD_BRUSHPROPERTIES)
		{
			App->CL_Dialogs->Start_Brush_Properties_Dlg();
			SendDlgItemMessage(hDlg, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)App->CL_Properties_Brushes->Selected_Index, (LPARAM)0);
			return TRUE;
		}

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
// *	  	List_Selection_Changed:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Properties_Brushes::List_Selection_Changed(bool Clear)
{
	int Brush_Count = App->CL_Brush->Get_Brush_Count();

	//if (Brush_Count > 0)
	{
		int Index = SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		if (Index == -1)
		{
			//App->Say("ListBox No Selection Available");
		}
		else
		{
			if (flag_Brushes_Dlg_Created == 1)
			{
				Selected_Index = Index;
				OnSelchangeBrushlist(Index, Clear);
			}
		}
	}
}

// *************************************************************************
// *		OnSelchangeBrushlist:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Brushes::OnSelchangeBrushlist(int Index, bool Clear)
{
	int			c;
	geBoolean	bChanged = FALSE;

	c = App->CL_Brush->Get_Brush_Count();

	if (c > 0)
	{
		if (Clear == 1)
		{
			App->CL_Doc->ResetAllSelections();
			App->CL_Doc->UpdateSelected();
		}

		Selected_Brush = App->CL_Brush->Get_Brush_ByIndex(Index);

		App->CL_SelBrushList->SelBrushList_Add(App->CL_Doc->pSelBrushes, Selected_Brush);

		if (Clear == 1)
		{
			/*Update_Dlg_Controls();
			App->CLSB_TopTabs->Update_Dlg_Controls();*/
		}
		//m_pDoc->DoBrushSelection( Selected_Brush, brushSelToggle) ;
		bChanged = GE_TRUE;
	}


	if (bChanged)
	{
		App->CL_Doc->UpdateSelected();
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		Update_SelectedBrushesCount_Dlg();
	}
}

// *************************************************************************
// *			 Fill_ListBox:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Brushes::Fill_ListBox()
{
	if (flag_Brushes_Dlg_Created == 1)
	{
		SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		Level* pLevel = App->CL_Doc->pLevel;
		BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

		int Count = 0;
		Brush* b;
		b = pList->First;
		while (b != NULL)
		{
			SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)App->CL_Brush->Brush_GetName(b));
			Count++;
			b = b->Next;
		}

		char buff[100];
		SetDlgItemText(BrushesDlg_Hwnd, IDC_BRUSHCOUNT, _itoa(Count, buff, 10));
	}
}

// *************************************************************************
// *	  		Get_Index:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Properties_Brushes::Get_Index(const Brush* b)
{
	Level* pLevel = App->CL_Doc->pLevel;
	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

	int Selected = 0;
	int Count = 0;

	b = pList->First;

	while (b != NULL)
	{
		Selected = App->CL_Doc->BrushIsSelected(b);

		if (Selected == 1)
		{
			SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)Count, (LPARAM)0);
			Selected_Index = Count;
			//Selected_Brush = App->CL_Brush->Get_Brush_ByIndex(Selected_Index);
			List_Selection_Changed(0);
		}

		Count++;
		b = b->Next;
	}

	Update_SelectedBrushesCount_Dlg();
}

// *************************************************************************
// * Update_SelectedBrushesCount_Dlg:- Terry and Hazel Flanigan 2025	*
// *************************************************************************
void CL64_Properties_Brushes::Update_SelectedBrushesCount_Dlg()
{
	char buff[100];
	int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
	SetDlgItemText(BrushesDlg_Hwnd, IDC_ST_SELECTED_NUM, _itoa(NumSelBrushes, buff, 10));
}

// *************************************************************************
// *	  	Start_Dimensions_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Brushes::Start_Dimensions_Dlg()
{
	if (flag_Dimension_Dlg_Active == 0)
	{
		int NumberOfBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
		if (NumberOfBrushes > 0)
		{
			Dimensions_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_BRUSH_DIMENSIONS, App->MainHwnd, (DLGPROC)Proc_Dimensions_Dlg);
			flag_Dimension_Dlg_Active = 1;
		}
		else
		{
			App->Say("No Brush Selected");
		}
	}
}

// *************************************************************************
// *	Proc_Dimensions_Dlg:- Terry and Hazel Flanigan 2025	    	    *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Brushes::Proc_Dimensions_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_DIM_POSITION, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_DIM_ROTATION, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_DIM_SCALE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STPOSX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STPOSY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STPOSZ, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STROTX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STROTY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STROTZ, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STSIZEX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STSIZEY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STSIZEZ, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_POSX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_POSY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_POSZ, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_ROTX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_ROTY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_ROTZ, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_SCALEX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_SCALEY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ED_BRUSH_SCALEZ, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CBPOSXDELTA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBPOSYDELTA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBPOSZDELTA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CBROTXDELTA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBROTYDELTA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBROTZDELTA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_SIZELOCK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Properties_Brushes->Update_Deltas_Dlg(hDlg);

		//// ----------- ScaleLock
		//if (App->CLSB_Brushes->ScaleLock_Flag == 1)
		//{
		//	HWND temp = GetDlgItem(hDlg, IDC_SIZELOCK);
		//	SendMessage(temp, BM_SETCHECK, 1, 0);
		//	App->CLSB_Brushes->EnableScaleControls_Dlg(hDlg, false);
		//}

		App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		// Position
		if (GetDlgItem(hDlg, IDC_STPOSX) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STPOSY) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STPOSZ) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_DIM_POSITION) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_DIM_ROTATION) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_DIM_SCALE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_STROTX) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STROTY) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STROTZ) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STSIZEX) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STSIZEY) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STSIZEZ) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		//if (GetDlgItem(hDlg, IDC_SIZELOCK) == (HWND)lParam)
		//{
		//	SetBkColor((HDC)wParam, RGB(0, 255, 0));
		//	SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//	SetBkMode((HDC)wParam, TRANSPARENT);
		//	return (UINT)App->AppBackground;
		//}

		return FALSE;
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

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_HSCROLL:
	{
		// ------------------------------------------------------------- Position
		// -------- Pos X
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBPOSXH))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Brushes->CenterOfSelection.x += App->CL_Properties_Brushes->PosY_Delta;
				App->CL_Properties_Brushes->Move_Brush();
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Brushes->CenterOfSelection.x -= App->CL_Properties_Brushes->PosX_Delta;
				App->CL_Properties_Brushes->Move_Brush();
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}

		// ------- Pos Y
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBPOSYH))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Brushes->CenterOfSelection.y += App->CL_Properties_Brushes->PosY_Delta;
				App->CL_Properties_Brushes->Move_Brush();
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Brushes->CenterOfSelection.y -= App->CL_Properties_Brushes->PosY_Delta;
				App->CL_Properties_Brushes->Move_Brush();
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}

		// ------- Pos Z
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBPOSZH))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Brushes->CenterOfSelection.z += App->CL_Properties_Brushes->PosZ_Delta;
				App->CL_Properties_Brushes->Move_Brush();
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Brushes->CenterOfSelection.z -= App->CL_Properties_Brushes->PosZ_Delta;
				App->CL_Properties_Brushes->Move_Brush();
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}

		// ------------------------------------------------------------- Rotation
		// -------- Rot X
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBROTX))
		{
			float m_Delta = App->CL_Properties_Brushes->RotX_Delta * GE_PI / 180;

			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{

				App->CL_Properties_Brushes->Rotation.x += +App->CL_Properties_Brushes->RotX_Delta;
				App->CL_Properties_Brushes->Rotate_Brush(m_Delta, 0, 0);
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Brushes->Rotation.x += -App->CL_Properties_Brushes->RotX_Delta;
				App->CL_Properties_Brushes->Rotate_Brush(-m_Delta, 0, 0);
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}

		// ------- Rot Y
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBROTY))
		{
			float m_Delta = App->CL_Properties_Brushes->RotY_Delta * GE_PI / 180;

			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Brushes->Rotation.y += +App->CL_Properties_Brushes->RotY_Delta;
				App->CL_Properties_Brushes->Rotate_Brush(0, m_Delta, 0);
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Brushes->Rotation.y += -App->CL_Properties_Brushes->RotY_Delta;
				App->CL_Properties_Brushes->Rotate_Brush(0, -m_Delta, 0);
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}

		// ------- Rot Z
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBROTZ))
		{
			float m_Delta = App->CL_Properties_Brushes->RotZ_Delta * GE_PI / 180;

			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Brushes->Rotation.z += +App->CL_Properties_Brushes->RotZ_Delta;
				App->CL_Properties_Brushes->Rotate_Brush(0, 0, m_Delta);
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Brushes->Rotation.z += -App->CL_Properties_Brushes->RotZ_Delta;
				App->CL_Properties_Brushes->Rotate_Brush(0, 0, -m_Delta);
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}
		
		// ------------------------------------------------------------- SCALE
		// -------- Scale X
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBSCALEX))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				/*if (App->CL_Properties_Brushes->ScaleLock_Flag == 1)
				{
					App->CL_Properties_Brushes->Scale_Brush_Lock(1);
				}
				else*/
				{
					float Delta = App->CL_Properties_Brushes->ScaleX_Delta;
					float scale = (App->CL_Properties_Brushes->Size.x + Delta) / App->CL_Properties_Brushes->Size.x;

					App->CL_Properties_Brushes->Scale_Brush(scale, 1, 1);
				}

				break;
			}

			case SB_LINELEFT:
			{
				/*if (App->CLSB_Brushes->ScaleLock_Flag == 1)
				{
					App->CLSB_Brushes->Scale_Brush_Lock(0);
				}
				else*/
				{
					if (App->CL_Properties_Brushes->Size.x > 1)
					{
						float Delta = App->CL_Properties_Brushes->ScaleX_Delta;
						float scale = (App->CL_Properties_Brushes->Size.x + -Delta) / App->CL_Properties_Brushes->Size.x;

						App->CL_Properties_Brushes->Scale_Brush(scale, 1, 1);
					}
				}
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}

		// -------- Scale Y
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBSCALEY))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				float scale = (App->CL_Properties_Brushes->Size.y + 1) / App->CL_Properties_Brushes->Size.y;

				App->CL_Properties_Brushes->Scale_Brush(1, scale, 1);
				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Size.y > 1)
				{
					float scale = (App->CL_Properties_Brushes->Size.y + -1) / App->CL_Properties_Brushes->Size.y;

					App->CL_Properties_Brushes->Scale_Brush(1, scale, 1);
				}
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}

		// -------- Scale Z
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBSCALEZ))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				float scale = (App->CL_Properties_Brushes->Size.z + 1) / App->CL_Properties_Brushes->Size.z;

				App->CL_Properties_Brushes->Scale_Brush(1, 1, scale);
				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Size.z > 1)
				{
					float scale = (App->CL_Properties_Brushes->Size.z + -1) / App->CL_Properties_Brushes->Size.z;

					App->CL_Properties_Brushes->Scale_Brush(1, 1, scale);
				}
				break;
			}
			}

			App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);

			return 0;
		}


		return 0;
	}

	case WM_COMMAND:

		/*if (LOWORD(wParam) == IDC_SIZELOCK)
		{
			HWND temp = GetDlgItem(hDlg, IDC_SIZELOCK);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CLSB_Brushes->ScaleLock_Flag = 1;
				App->CLSB_Brushes->EnableScaleControls_Dlg(hDlg, false);
				return 1;
			}
			else
			{
				App->CLSB_Brushes->ScaleLock_Flag = 0;
				App->CLSB_Brushes->EnableScaleControls_Dlg(hDlg, true);
				return 1;
			}

			return TRUE;
		}*/

		if (LOWORD(wParam) == IDC_CBPOSXDELTA)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBPOSXDELTA);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Brushes->PosX_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CBPOSYDELTA)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBPOSYDELTA);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Brushes->PosY_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CBPOSZDELTA)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBPOSZDELTA);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Brushes->PosZ_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CBROTXDELTA)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBROTXDELTA);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Brushes->RotX_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CBROTYDELTA)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBROTYDELTA);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Brushes->RotY_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CBROTZDELTA)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBROTZDELTA);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Brushes->RotZ_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Properties_Brushes->flag_Dimension_Dlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Brushes->flag_Dimension_Dlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;
}

// *************************************************************************
// *			Update_Deltas_Dlg:- Terry and Hazel Flanigan 2023	 	   *
// *************************************************************************
void CL64_Properties_Brushes::Update_Deltas_Dlg(HWND hDlg)
{
	// Pos
	HWND CB_hWnd = GetDlgItem(hDlg, IDC_CBPOSXDELTA);
	Fill_ComboBox_PosDelta(CB_hWnd);

	CB_hWnd = GetDlgItem(hDlg, IDC_CBPOSYDELTA);
	Fill_ComboBox_PosDelta(CB_hWnd);

	CB_hWnd = GetDlgItem(hDlg, IDC_CBPOSZDELTA);
	Fill_ComboBox_PosDelta(CB_hWnd);

	// Rot
	CB_hWnd = GetDlgItem(hDlg, IDC_CBROTXDELTA);
	Fill_ComboBox_RotDelta(CB_hWnd);

	CB_hWnd = GetDlgItem(hDlg, IDC_CBROTYDELTA);
	Fill_ComboBox_RotDelta(CB_hWnd);

	CB_hWnd = GetDlgItem(hDlg, IDC_CBROTZDELTA);
	Fill_ComboBox_RotDelta(CB_hWnd);
}

// *************************************************************************
// *		Fill_ComboBox_PosDelta:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Properties_Brushes::Fill_ComboBox_PosDelta(HWND hDlg)
{
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.1");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.2");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.5");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"1");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"10");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"20");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"100");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"128");

	SendMessage(hDlg, CB_SETCURSEL, 4, 0);
}

// *************************************************************************
// *		Fill_ComboBox_RotDelta:- Terry and Hazel Flanigan 2025	  	   *
// *************************************************************************
void CL64_Properties_Brushes::Fill_ComboBox_RotDelta(HWND hDlg)
{
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"1");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"45");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"90");

	SendMessage(hDlg, CB_SETCURSEL, 1, 0);
}

// *************************************************************************
// *		Update_From_Brush_Dlg:- Terry and Hazel Flanigan 2025	    *
// *************************************************************************
void CL64_Properties_Brushes::Update_From_Brush_Dlg(HWND hDlg)
{
	//Lock_Textures(true);

	Get_Brush();
	App->CL_SelBrushList->SelBrushList_Center(App->CL_Doc->pSelBrushes, &App->CL_Doc->SelectedGeoCenter);
	CenterOfSelection = App->CL_Doc->SelectedGeoCenter;

	char buf[255];

	// Pos
	sprintf(buf, "%.3f", CenterOfSelection.x);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_POSX, buf);

	sprintf(buf, "%.3f", CenterOfSelection.y);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_POSY, buf);

	sprintf(buf, "%.3f", CenterOfSelection.z);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_POSZ, buf);

	// Rotation
	sprintf(buf, "%.2f", Rotation.x);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_ROTX, buf);

	sprintf(buf, "%.2f", Rotation.y);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_ROTY, buf);

	sprintf(buf, "%.2f", Rotation.z);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_ROTZ, buf);

	// Scale
	sprintf(buf, "%.2f", Size.x);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_SCALEX, buf);

	sprintf(buf, "%.2f", Size.y);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_SCALEY, buf);

	sprintf(buf, "%.2f", Size.z);
	SetDlgItemText(hDlg, IDC_ED_BRUSH_SCALEZ, buf);
}

// *************************************************************************
// *			 	Get_Brush:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Brushes::Get_Brush()
{
	int NumberOfBrushes;
	Brush* pBrush = NULL;

	NumberOfBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

	if (NumberOfBrushes)
	{
		pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, (NumberOfBrushes - 1));
	}
	else
	{
		App->Say("Cant Get Brush");
		return;
	}

	Size.x = (fabs(pBrush->BoundingBox.Max.x - pBrush->BoundingBox.Min.x));
	Size.y = (fabs(pBrush->BoundingBox.Max.y - pBrush->BoundingBox.Min.y));
	Size.z = (fabs(pBrush->BoundingBox.Max.z - pBrush->BoundingBox.Min.z));

	T_Vec3 mSize;
	mSize.x = 2;
	mSize.y = 2;
	mSize.z = 2;

	//Brush_Scale3d(pBrush, &mSize);
	//App->Say(pBrush->Name);
}

// *************************************************************************
// *				Move_Brush:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
void CL64_Properties_Brushes::Move_Brush()
{
	App->CL_Maths->Vector3_Subtract(&CenterOfSelection, &App->CL_Doc->SelectedGeoCenter, &CenterOfSelection);

	App->CL_Doc->MoveSelectedBrushList(App->CL_Doc->pSelBrushes, &CenterOfSelection);

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
}

// *************************************************************************
// *				Rotate_Brush:- Terry and Hazel Flanigan 2025	  	   *
// *************************************************************************
void CL64_Properties_Brushes::Rotate_Brush(float SX, float SY, float SZ)
{
	FinalRot.x = SX;
	FinalRot.y = SY;
	FinalRot.z = SZ;

	App->CL_Doc->RotateSelectedBrushList(App->CL_Doc->pSelBrushes, &FinalRot);

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
}

// *************************************************************************
// *				Scale_Brush:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
void CL64_Properties_Brushes::Scale_Brush(float SX, float SY, float SZ)
{
	FinalScale.x = SX;
	FinalScale.y = SY;
	FinalScale.z = SZ;

	App->CL_Doc->ScaleSelectedBrushes(&FinalScale);

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
}
