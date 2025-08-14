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

	PosX_Delta = 1;
	PosY_Delta = 1;
	PosZ_Delta = 1;

	ScaleX_Delta = 0.1;
	ScaleY_Delta = 0.1;
	ScaleZ_Delta = 0.1;

	RotX_Delta = 1;
	RotY_Delta = 1;
	RotZ_Delta = 1;

	Rotation;
	Size;

	Dimensions_Dlg_hWnd = NULL;
}

CL64_Properties_Brushes::~CL64_Properties_Brushes()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Properties_Brushes::Reset_Class()
{
	App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(false);
}

// *************************************************************************
// *	  	Show_Brushes_Dialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Brushes::Show_Brushes_Dialog(bool Show)
{
	ShowWindow(BrushesDlg_Hwnd, Show);
}

// *************************************************************************
// *		 Set_Dlg_Options_Buttons:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Properties_Brushes::Set_Dlg_Brush_Options_Buttons(bool Enable)
{
	EnableWindow(GetDlgItem(BrushesDlg_Hwnd, IDC_BT_GD_BRUSHPROPERTIES), Enable);
	EnableWindow(GetDlgItem(BrushesDlg_Hwnd, IDC_BT_BRUSH_DIMENSIONS), Enable);
	EnableWindow(GetDlgItem(BrushesDlg_Hwnd, IDC_BT_DELETE_SEL_BRUSH), Enable);
	EnableWindow(GetDlgItem(BrushesDlg_Hwnd, IDC_BT_BRUSH_RENAME), Enable);
}

// *************************************************************************
// *	  	Start_Brush_Tabs_Dialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Brushes::Start_Brush_Tabs_Dialog()
{
	BrushesDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_BRUSHES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_Brush_Dlg);

	flag_Brushes_Dlg_Created = 1;

	Fill_ListBox();
	//Update_Dlg_Controls();
}

// *************************************************************************
// *			 Proc_Brush_Tabs:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Brushes::Proc_Brush_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_CB_BRUSH_CAT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_GD_BRUSHLIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GD_GROUPS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BRUSHCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_GD_BRUSHCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SELECTED_NUM, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_GD_BRUSHPROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_DIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_RENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DELETE_SEL_BRUSH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_CB_BRUSH_CAT, CB_ADDSTRING, (WPARAM)0, (LPARAM)"All");
		SendDlgItemMessage(hDlg, IDC_CB_BRUSH_CAT, CB_SETCURSEL, (WPARAM)App->CL_Properties_Brushes->Selected_Index, (LPARAM)0);
		
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

		if (some_item->idFrom == IDC_BT_BRUSH_RENAME && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_RENAME));
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
		if (LOWORD(wParam) == IDC_BT_GD_BRUSHPROPERTIES)
		{
			int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			if (NumSelBrushes > 0)
			{
				App->CL_Dialogs->Start_Brush_Properties_Dlg();
				SendDlgItemMessage(hDlg, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)App->CL_Properties_Brushes->Selected_Index, (LPARAM)0);
			}
			else
			{
				App->Say("No Brush Selected");
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_DIMENSIONS)
		{
			int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			if (NumSelBrushes > 0)
			{
				App->CL_Properties_Brushes->StartDimensionsDialog();
			}
			else
			{
				App->Say("No Brush Selected");
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_RENAME) 
		{
			const char* brushName = App->CL_Doc->CurBrush->Name;

			// Array of non- Renameable brush names
			const char* nonDeletableBrushes[] = { "Player_Main", "Environ_0" };

			// Check if the current brush name is in the non-Renameable list
			for (const char* nonDeletable : nonDeletableBrushes)
			{
				if (strcmp(brushName, nonDeletable) == 0) 
				{
					App->Say("This Brush cannot be Renamed");
					return true;
				}
			}

			App->CL_Entities->Rename_Brush();

			return TRUE;
		}


		if (LOWORD(wParam) == IDC_BT_DELETE_SEL_BRUSH)
		{
			const char* brushName = App->CL_Doc->CurBrush->Name;

			// Array of non-deletable brush names
			const char* nonDeletableBrushes[] = { "Player_Main", "Environ_0" };

			// Check if the current brush name is in the non-deletable list
			for (const char* nonDeletable : nonDeletableBrushes) 
			{
				if (strcmp(brushName, nonDeletable) == 0) 
				{
					App->Say("This Brush cannot be Deleted");
					return true;
				}
			}

			int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			if (NumSelBrushes > 0)
			{
				App->CL_Dialogs->YesNo("Are you sure", "Do you want to Delete the selected Brushes");

				if (App->CL_Dialogs->flag_Dlg_Canceled == false)
				{
					App->CL_Ogre->OGL_Listener->Show_Visuals(false);
					App->CL_Entities->Delete_Brush();
				}
			}
			else
			{
				App->Say("No Brush Selected");
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_GD_BRUSHLIST)
		{
			if (App->CL_Properties_Brushes->flag_Brushes_Dlg_Created == 1)
			{
				App->CL_Doc->Do_General_Select_Dlg(false);

				App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(true);

				App->CL_Properties_Brushes->List_Selection_Changed(true);

			}
			return TRUE;
		}

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
// *		 	Select_From_List:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Properties_Brushes::Select_From_List(int index)
{
	SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)index, (LPARAM)0);
	List_Selection_Changed(true);
}

// *************************************************************************
// *		OnSelchangeBrushlist:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Brushes::OnSelchangeBrushlist(int index, bool clear)
{
	int brushCount = App->CL_Brush->Get_Brush_Count();
	bool hasChanged = false;

	if (brushCount > 0)
	{
		if (clear)
		{
			App->CL_Doc->ResetAllSelections();
			App->CL_Doc->UpdateSelected();
		}

		Selected_Brush = App->CL_Brush->Get_Brush_ByIndex(index);
		App->CL_SelBrushList->SelBrushList_Add(App->CL_Doc->pSelBrushes, Selected_Brush);

		// Select Object in Scene Editor
		if (Selected_Brush->GroupId > Enums::Brushs_ID_Players)
		{
			// TODO 
			//int entityIndex = App->CL_Entities->GetIndex_By_Name(Selected_Brush->Name);
			//App->CL_FileView->SelectItem(App->CL_Scene->B_Object[entityIndex]->FileViewItem);
		}

		if (clear)
		{
			// Update_Dlg_Controls(); // Uncomment if needed
			// App->CLSB_TopTabs->Update_Dlg_Controls(); // Uncomment if needed
		}

		hasChanged = true;
	}

	if (hasChanged)
	{
		App->CL_Doc->UpdateSelected();
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		Update_SelectedBrushesCount_Dlg();
		App->CL_Top_Tabs->Update_Faces_Combo();
		App->CL_Ogre->OGL_Listener->Show_Visuals(true);
	}
}


// *************************************************************************
// *			 Fill_ListBox:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Brushes::Fill_ListBox()
{
	// Check if the Brush Dialog has been created
	if (flag_Brushes_Dlg_Created != true) return;

	// Reset the content of the brush list box
	SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_RESETCONTENT, 0, 0);

	// Retrieve the main brushes from the level
	BrushList* pList = App->CL_Level->Level_Get_Main_Brushes();
	int count = 0;

	// Iterate through the brushes and add them to the list box
	for (Brush* b = pList->First; b != nullptr; b = b->Next)
	{
		// Add the brush name to the list box
		SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_ADDSTRING, 0, (LPARAM)App->CL_Brush->Brush_GetName(b));
		count++;
	}

	// Update the brush count display
	char buff[100];
	SetDlgItemText(BrushesDlg_Hwnd, IDC_BRUSHCOUNT, _itoa(count, buff, 10));
}

// *************************************************************************
// *	  	Select_in_BrushList_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
int CL64_Properties_Brushes::Select_in_BrushList_Dlg(const Brush* b)
{
	// Get all Brushes into a BrushList
	BrushList* pList = App->CL_Level->Level_Get_Main_Brushes();

	int selected = 0;
	int Count = 0;

	b = pList->First;

	// Search all Brushes
	while (b != NULL)
	{
		// Check if Brush is selected
		selected = App->CL_Doc->BrushIsSelected(b);

		// If selected select in Brush list Dialog
		if (selected == 1)
		{
			SendDlgItemMessage(BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)Count, (LPARAM)0);
			Selected_Index = Count;
			List_Selection_Changed(false);
			Update_SelectedBrushesCount_Dlg();
			return Count;
		}

		Count++;
		b = b->Next;
	}

	return -1;
}

// *************************************************************************
// *	Update_SelectedBrushesCount_Dlg:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Properties_Brushes::Update_SelectedBrushesCount_Dlg()
{
	char buff[100];
	int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
	SetDlgItemText(BrushesDlg_Hwnd, IDC_ST_SELECTED_NUM, _itoa(NumSelBrushes, buff, 10));
}

// *************************************************************************
// *	  	StartDimensionsDialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Brushes::StartDimensionsDialog()
{
	if (!flag_Dimension_Dlg_Active)
	{
		int numberOfBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
		if (numberOfBrushes > 0)
		{
			//App->Say_Int(App->CL_Properties_Brushes->Selected_Brush->GroupId);

			if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Players)
			{
				App->Say("Player");
				return;
			}

			Dimensions_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_BRUSH_DIMENSIONS, App->MainHwnd, (DLGPROC)Proc_Dimensions_Dlg);
			flag_Dimension_Dlg_Active = true;
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
		SendDlgItemMessage(hDlg, IDC_ST_BRUSH_NAME, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
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

		SendDlgItemMessage(hDlg, IDC_CBSCALEZDELTAX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBSCALEZDELTAY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBSCALEZDELTAZ, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_SIZELOCK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Properties_Brushes->Update_Deltas_Dlg(hDlg);

		SetDlgItemText(hDlg, IDC_ST_BRUSH_NAME, App->CL_Properties_Brushes->Selected_Brush->Name);
		
		// Check if the Brush is an Entity

		if (App->CL_Properties_Brushes->Selected_Brush->GroupId > Enums::Brushs_ID_Area)
		{
			int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);

			App->CL_Properties_Brushes->Rotation.x = App->CL_Scene->B_Object[Index]->Mesh_Rot.x;
			App->CL_Properties_Brushes->Rotation.y = App->CL_Scene->B_Object[Index]->Mesh_Rot.y;
			App->CL_Properties_Brushes->Rotation.z = App->CL_Scene->B_Object[Index]->Mesh_Rot.z;
		}

		if (App->CL_Properties_Brushes->Selected_Brush->GroupId > Enums::Brushs_ID_Area)
		{
			int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);

			App->CL_Properties_Brushes->Rotation.x = App->CL_Scene->B_Object[Index]->Mesh_Rot.x;
			App->CL_Properties_Brushes->Rotation.y = App->CL_Scene->B_Object[Index]->Mesh_Rot.y;
			App->CL_Properties_Brushes->Rotation.z = App->CL_Scene->B_Object[Index]->Mesh_Rot.z;
		}

		//// ----------- ScaleLock
		//if (App->CLSB_Brushes->ScaleLock_Flag == 1)
		//{
		//	HWND temp = GetDlgItem(hDlg, IDC_SIZELOCK);
		//	SendMessage(temp, BM_SETCHECK, 1, 0);
		//	App->CLSB_Brushes->EnableScaleControls_Dlg(hDlg, false);
		//}

		App->CL_Properties_Brushes->Update_From_Brush_Dlg(hDlg);


		//App->CL_Properties_Brushes->Rotation.x

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BRUSH_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

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
		// -------- Pos X
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBPOSXH)) {
			int delta = App->CL_Properties_Brushes->PosX_Delta;

			switch ((int)LOWORD(wParam)) 
			{
			case SB_LINERIGHT:
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->CenterOfSelection.x += delta;
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Pos = m_object->Object_Node->getPosition();
						Old_Pos.x += delta;
						m_object->Object_Node->setPosition(Old_Pos);
						m_object->Mesh_Pos = Old_Pos;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);
						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);
					}
				}
				break;

			case SB_LINELEFT:
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->CenterOfSelection.x -= delta;
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Pos = m_object->Object_Node->getPosition();
						Old_Pos.x -= delta;
						m_object->Object_Node->setPosition(Old_Pos);

						m_object->Mesh_Pos = Old_Pos;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);
						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);
					}
				}
				break;
			}

			if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
			{
				App->CL_Properties_Brushes->Move_Brush();
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
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->CenterOfSelection.y += App->CL_Properties_Brushes->PosY_Delta;
					App->CL_Properties_Brushes->Move_Brush();
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Pos = m_object->Object_Node->getPosition();
						Old_Pos.y += App->CL_Properties_Brushes->PosY_Delta;
						m_object->Object_Node->setPosition(Old_Pos);

						m_object->Mesh_Pos = Old_Pos;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);
						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);
					}
				}

				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->CenterOfSelection.y -= App->CL_Properties_Brushes->PosY_Delta;
					App->CL_Properties_Brushes->Move_Brush();
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Pos = m_object->Object_Node->getPosition();
						Old_Pos.y -= App->CL_Properties_Brushes->PosY_Delta;
						m_object->Object_Node->setPosition(Old_Pos);

						m_object->Mesh_Pos = Old_Pos;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);
						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);
					}
				}

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
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->CenterOfSelection.z += App->CL_Properties_Brushes->PosZ_Delta;
					App->CL_Properties_Brushes->Move_Brush();
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Pos = m_object->Object_Node->getPosition();
						Old_Pos.z += App->CL_Properties_Brushes->PosZ_Delta;
						m_object->Object_Node->setPosition(Old_Pos);

						m_object->Mesh_Pos = Old_Pos;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);
						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);
					}
				}

				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->CenterOfSelection.z -= App->CL_Properties_Brushes->PosZ_Delta;
					App->CL_Properties_Brushes->Move_Brush();
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Pos = m_object->Object_Node->getPosition();
						Old_Pos.z -= App->CL_Properties_Brushes->PosZ_Delta;
						m_object->Object_Node->setPosition(Old_Pos);

						m_object->Mesh_Pos = Old_Pos;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);
						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);
					}
				}

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
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->Rotation.x += +App->CL_Properties_Brushes->RotX_Delta;
					App->CL_Properties_Brushes->Rotate_Brush(m_Delta, 0, 0);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];
					
					if (m_object)
					{
						Ogre::Vector3 Old_Rot = m_object->Mesh_Rot;
						Old_Rot.x -= App->CL_Properties_Brushes->RotX_Delta;

						m_object->Object_Node->resetOrientation();
						m_object->Object_Node->pitch(((Ogre::Degree)Old_Rot.x), Ogre::Node::TS_WORLD);
						m_object->Object_Node->yaw(((Ogre::Degree)Old_Rot.y), Ogre::Node::TS_WORLD);
						m_object->Object_Node->roll(((Ogre::Degree)Old_Rot.z), Ogre::Node::TS_WORLD);

						m_object->Mesh_Rot = Old_Rot;
						m_object->Mesh_Quat.w = m_object->Object_Node->getOrientation().w;
						m_object->Mesh_Quat.x = m_object->Object_Node->getOrientation().x;
						m_object->Mesh_Quat.y = m_object->Object_Node->getOrientation().y;
						m_object->Mesh_Quat.z = m_object->Object_Node->getOrientation().z;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);

					}
				}

				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->Rotation.x += -App->CL_Properties_Brushes->RotX_Delta;
					App->CL_Properties_Brushes->Rotate_Brush(-m_Delta, 0, 0);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];
					if (m_object)
					{
						Ogre::Vector3 Old_Rot = m_object->Mesh_Rot;
						Old_Rot.x += App->CL_Properties_Brushes->RotX_Delta;

						m_object->Object_Node->resetOrientation();
						m_object->Object_Node->pitch(((Ogre::Degree)Old_Rot.x), Ogre::Node::TS_WORLD);
						m_object->Object_Node->yaw(((Ogre::Degree)Old_Rot.y), Ogre::Node::TS_WORLD);
						m_object->Object_Node->roll(((Ogre::Degree)Old_Rot.z), Ogre::Node::TS_WORLD);

						m_object->Mesh_Rot = Old_Rot;
						m_object->Mesh_Quat.w = m_object->Object_Node->getOrientation().w;
						m_object->Mesh_Quat.x = m_object->Object_Node->getOrientation().x;
						m_object->Mesh_Quat.y = m_object->Object_Node->getOrientation().y;
						m_object->Mesh_Quat.z = m_object->Object_Node->getOrientation().z;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
				}

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
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->Rotation.y += +App->CL_Properties_Brushes->RotY_Delta;
					App->CL_Properties_Brushes->Rotate_Brush(0, m_Delta, 0);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Rot = m_object->Mesh_Rot;
						Old_Rot.y += App->CL_Properties_Brushes->RotY_Delta;

						m_object->Object_Node->resetOrientation();
						m_object->Object_Node->pitch(((Ogre::Degree)Old_Rot.x), Ogre::Node::TS_WORLD);
						m_object->Object_Node->yaw(((Ogre::Degree)Old_Rot.y), Ogre::Node::TS_WORLD);
						m_object->Object_Node->roll(((Ogre::Degree)Old_Rot.z), Ogre::Node::TS_WORLD);

						m_object->Mesh_Rot = Old_Rot;
						m_object->Mesh_Quat.w = m_object->Object_Node->getOrientation().w;
						m_object->Mesh_Quat.x = m_object->Object_Node->getOrientation().x;
						m_object->Mesh_Quat.y = m_object->Object_Node->getOrientation().y;
						m_object->Mesh_Quat.z = m_object->Object_Node->getOrientation().z;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
				}

				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->Rotation.y += -App->CL_Properties_Brushes->RotY_Delta;
					App->CL_Properties_Brushes->Rotate_Brush(0, -m_Delta, 0);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Rot = m_object->Mesh_Rot;
						Old_Rot.y -= App->CL_Properties_Brushes->RotY_Delta;

						m_object->Object_Node->resetOrientation();
						m_object->Object_Node->pitch(((Ogre::Degree)Old_Rot.x), Ogre::Node::TS_WORLD);
						m_object->Object_Node->yaw(((Ogre::Degree)Old_Rot.y), Ogre::Node::TS_WORLD);
						m_object->Object_Node->roll(((Ogre::Degree)Old_Rot.z), Ogre::Node::TS_WORLD);

						m_object->Mesh_Rot = Old_Rot;
						m_object->Mesh_Quat.w = m_object->Object_Node->getOrientation().w;
						m_object->Mesh_Quat.x = m_object->Object_Node->getOrientation().x;
						m_object->Mesh_Quat.y = m_object->Object_Node->getOrientation().y;
						m_object->Mesh_Quat.z = m_object->Object_Node->getOrientation().z;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
				}

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
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->Rotation.z += +App->CL_Properties_Brushes->RotZ_Delta;
					App->CL_Properties_Brushes->Rotate_Brush(0, 0, m_Delta);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Rot = m_object->Mesh_Rot;
						Old_Rot.z += App->CL_Properties_Brushes->RotZ_Delta;

						m_object->Object_Node->resetOrientation();
						m_object->Object_Node->pitch(((Ogre::Degree)Old_Rot.x), Ogre::Node::TS_WORLD);
						m_object->Object_Node->yaw(((Ogre::Degree)Old_Rot.y), Ogre::Node::TS_WORLD);
						m_object->Object_Node->roll(((Ogre::Degree)Old_Rot.z), Ogre::Node::TS_WORLD);

						m_object->Mesh_Rot = Old_Rot;
						m_object->Mesh_Quat.w = m_object->Object_Node->getOrientation().w;
						m_object->Mesh_Quat.x = m_object->Object_Node->getOrientation().x;
						m_object->Mesh_Quat.y = m_object->Object_Node->getOrientation().y;
						m_object->Mesh_Quat.z = m_object->Object_Node->getOrientation().z;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
				}

				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					App->CL_Properties_Brushes->Rotation.z += -App->CL_Properties_Brushes->RotZ_Delta;
					App->CL_Properties_Brushes->Rotate_Brush(0, 0, -m_Delta);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Rot = m_object->Mesh_Rot;
						Old_Rot.z -= App->CL_Properties_Brushes->RotZ_Delta;

						m_object->Object_Node->resetOrientation();
						m_object->Object_Node->pitch(((Ogre::Degree)Old_Rot.x), Ogre::Node::TS_WORLD);
						m_object->Object_Node->yaw(((Ogre::Degree)Old_Rot.y), Ogre::Node::TS_WORLD);
						m_object->Object_Node->roll(((Ogre::Degree)Old_Rot.z), Ogre::Node::TS_WORLD);

						m_object->Mesh_Rot = Old_Rot;
						m_object->Mesh_Quat.w = m_object->Object_Node->getOrientation().w;
						m_object->Mesh_Quat.x = m_object->Object_Node->getOrientation().x;
						m_object->Mesh_Quat.y = m_object->Object_Node->getOrientation().y;
						m_object->Mesh_Quat.z = m_object->Object_Node->getOrientation().z;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
				}

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

				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					float Delta = App->CL_Properties_Brushes->ScaleX_Delta;
					float scale = (App->CL_Properties_Brushes->Size.x + Delta) / App->CL_Properties_Brushes->Size.x;

					App->CL_Properties_Brushes->Scale_Brush(scale, 1, 1);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Scale = m_object->Mesh_Scale;
						Old_Scale.x += App->CL_Properties_Brushes->ScaleX_Delta;

						m_object->Object_Node->setScale(Old_Scale);
						m_object->Mesh_Scale = Old_Scale;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
				}

				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					if (App->CL_Properties_Brushes->Size.x > 1)
					{
						float Delta = App->CL_Properties_Brushes->ScaleX_Delta;
						float scale = (App->CL_Properties_Brushes->Size.x + -Delta) / App->CL_Properties_Brushes->Size.x;

						App->CL_Properties_Brushes->Scale_Brush(scale, 1, 1);
					}
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Scale = m_object->Mesh_Scale;
						Old_Scale.x -= App->CL_Properties_Brushes->ScaleX_Delta;

						m_object->Object_Node->setScale(Old_Scale);
						m_object->Mesh_Scale = Old_Scale;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
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
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					float scale = (App->CL_Properties_Brushes->Size.y + 1) / App->CL_Properties_Brushes->Size.y;
					App->CL_Properties_Brushes->Scale_Brush(1, scale, 1);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Scale = m_object->Mesh_Scale;
						Old_Scale.y += App->CL_Properties_Brushes->ScaleY_Delta;

						m_object->Object_Node->setScale(Old_Scale);
						m_object->Mesh_Scale = Old_Scale;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
				}

				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					if (App->CL_Properties_Brushes->Size.y > 1)
					{
						float scale = (App->CL_Properties_Brushes->Size.y + -1) / App->CL_Properties_Brushes->Size.y;

						App->CL_Properties_Brushes->Scale_Brush(1, scale, 1);
					}
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Scale = m_object->Mesh_Scale;
						Old_Scale.y -= App->CL_Properties_Brushes->ScaleY_Delta;

						m_object->Object_Node->setScale(Old_Scale);
						m_object->Mesh_Scale = Old_Scale;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
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
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					float scale = (App->CL_Properties_Brushes->Size.z + 1) / App->CL_Properties_Brushes->Size.z;

					App->CL_Properties_Brushes->Scale_Brush(1, 1, scale);
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Scale = m_object->Mesh_Scale;
						Old_Scale.z += App->CL_Properties_Brushes->ScaleY_Delta;

						m_object->Object_Node->setScale(Old_Scale);
						m_object->Mesh_Scale = Old_Scale;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
				}

				break;
			}

			case SB_LINELEFT:
			{
				if (App->CL_Properties_Brushes->Selected_Brush->GroupId == Enums::Brushs_ID_Area)
				{
					if (App->CL_Properties_Brushes->Size.z > 1)
					{
						float scale = (App->CL_Properties_Brushes->Size.z + -1) / App->CL_Properties_Brushes->Size.z;

						App->CL_Properties_Brushes->Scale_Brush(1, 1, scale);
					}
				}
				else
				{
					int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
					auto& m_object = App->CL_Scene->B_Object[Index];

					if (m_object)
					{
						Ogre::Vector3 Old_Scale = m_object->Mesh_Scale;
						Old_Scale.z -= App->CL_Properties_Brushes->ScaleY_Delta;

						m_object->Object_Node->setScale(Old_Scale);
						m_object->Mesh_Scale = Old_Scale;

						App->CL_Physics->Update_Object_Physics(Index);

						App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

						App->CL_Gizmos->MarkerBox_Adjust(Index);
						App->CL_Ogre->RenderFrame(2);

						m_object->flag_Altered = 1;
						App->CL_Level->flag_Level_is_Modified = true;
						App->CL_FileView->Mark_Altered(m_object->FileViewItem);
					}
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

	// Scale
	CB_hWnd = GetDlgItem(hDlg, IDC_CBSCALEZDELTAX);
	Fill_ComboBox_ScaleDelta(CB_hWnd);

	CB_hWnd = GetDlgItem(hDlg, IDC_CBSCALEZDELTAY);
	Fill_ComboBox_ScaleDelta(CB_hWnd);

	CB_hWnd = GetDlgItem(hDlg, IDC_CBSCALEZDELTAZ);
	Fill_ComboBox_ScaleDelta(CB_hWnd);
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

	SendMessage(hDlg, CB_SETCURSEL, 3, 0);
}

// *************************************************************************
// *		Fill_ComboBox_RotDelta:- Terry and Hazel Flanigan 2025	  	   *
// *************************************************************************
void CL64_Properties_Brushes::Fill_ComboBox_RotDelta(HWND hDlg)
{
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"1");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"45");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"90");

	SendMessage(hDlg, CB_SETCURSEL, 0, 0);
}

// *************************************************************************
// *		Fill_ComboBox_ScaleDelta:- Terry and Hazel Flanigan 2025 	   *
// *************************************************************************
void CL64_Properties_Brushes::Fill_ComboBox_ScaleDelta(HWND hDlg)
{
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.1");
	//SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"45");
	//SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"90");

	SendMessage(hDlg, CB_SETCURSEL, 0, 0);
}

// *************************************************************************
// *			Update_From_Brush_Dlg:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Properties_Brushes::Update_From_Brush_Dlg(HWND hDlg)
{
	Get_Brush();

	if (Selected_Brush->GroupId == Enums::Brushs_ID_Area)
	{
		App->CL_SelBrushList->SelBrushList_Center(App->CL_Doc->pSelBrushes, &App->CL_Doc->SelectedGeoCenter);
		CenterOfSelection = App->CL_Doc->SelectedGeoCenter;

		UpdateDialogItem(hDlg, IDC_ED_BRUSH_POSX, CenterOfSelection.x, "%.3f");
		UpdateDialogItem(hDlg, IDC_ED_BRUSH_POSY, CenterOfSelection.y, "%.3f");
		UpdateDialogItem(hDlg, IDC_ED_BRUSH_POSZ, CenterOfSelection.z, "%.3f");

		UpdateDialogItem(hDlg, IDC_ED_BRUSH_ROTX, Rotation.x, "%.2f");
		UpdateDialogItem(hDlg, IDC_ED_BRUSH_ROTY, Rotation.y, "%.2f");
		UpdateDialogItem(hDlg, IDC_ED_BRUSH_ROTZ, Rotation.z, "%.2f");

		UpdateDialogItem(hDlg, IDC_ED_BRUSH_SCALEX, Size.x, "%.2f");
		UpdateDialogItem(hDlg, IDC_ED_BRUSH_SCALEY, Size.y, "%.2f");
		UpdateDialogItem(hDlg, IDC_ED_BRUSH_SCALEZ, Size.z, "%.2f");
	}
	else
	{
		int Index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);

		if (App->CL_Scene->B_Object[Index])
		{
			Ogre::Vector3 Pos = App->CL_Scene->B_Object[Index]->Mesh_Pos;
			Ogre::Vector3 Scale = App->CL_Scene->B_Object[Index]->Mesh_Scale;
			Ogre::Vector3 Rot = App->CL_Scene->B_Object[Index]->Mesh_Rot;

			UpdateDialogItem(hDlg, IDC_ED_BRUSH_POSX, Pos.x, "%.2f");
			UpdateDialogItem(hDlg, IDC_ED_BRUSH_POSY, Pos.y, "%.2f");
			UpdateDialogItem(hDlg, IDC_ED_BRUSH_POSZ, Pos.z, "%.2f");

			UpdateDialogItem(hDlg, IDC_ED_BRUSH_ROTX, Rot.x, "%.2f");
			UpdateDialogItem(hDlg, IDC_ED_BRUSH_ROTY, Rot.y, "%.2f");
			UpdateDialogItem(hDlg, IDC_ED_BRUSH_ROTZ, Rot.z, "%.2f");

			UpdateDialogItem(hDlg, IDC_ED_BRUSH_SCALEX, Scale.x, "%.2f");
			UpdateDialogItem(hDlg, IDC_ED_BRUSH_SCALEY, Scale.y, "%.2f");
			UpdateDialogItem(hDlg, IDC_ED_BRUSH_SCALEZ, Scale.z, "%.2f");
		}
	}



}

// *************************************************************************
// *			UpdateDialogItem:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Brushes::UpdateDialogItem(HWND hDlg, int itemId, float value, const char* format)
{
	char buf[255];
	sprintf(buf, format, value);
	SetDlgItemText(hDlg, itemId, buf);
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

	//if (pBrush->GroupId == Enums::Brushs_ID_Area)
	//{
		Size.x = (fabs(pBrush->BoundingBox.Max.x - pBrush->BoundingBox.Min.x));
		Size.y = (fabs(pBrush->BoundingBox.Max.y - pBrush->BoundingBox.Min.y));
		Size.z = (fabs(pBrush->BoundingBox.Max.z - pBrush->BoundingBox.Min.z));
	//}
	//else
	//{
	//	Size.x = 777;// (fabs(pBrush->BoundingBox.Max.x - pBrush->BoundingBox.Min.x));
	//	Size.y = 777;//(fabs(pBrush->BoundingBox.Max.y - pBrush->BoundingBox.Min.y));
	//	Size.z = 777;//(fabs(pBrush->BoundingBox.Max.z - pBrush->BoundingBox.Min.z));
	//}

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

	//App->CL_Doc->RotateSelectedBrushList(App->CL_Doc->pSelBrushes, &FinalRot);


	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
}

// *************************************************************************
// *				Rotate_Entity:- Terry and Hazel Flanigan 2025	  	   *
// *************************************************************************
void CL64_Properties_Brushes::Rotate_Entity(float SX, float SY, float SZ)
{
	
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
