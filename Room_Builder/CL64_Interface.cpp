/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl Scene Builder

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
#include "CL64_Interface.h"

CL64_Interface::CL64_Interface()
{
	flag_Tab_Templates = true;
	flag_Tab_Templates = false;
	flag_Tab_Group = false;
	flag_Faces_Con_Dlg_Active = false;

	flag_FileView_Active = false;
	flag_Properties_Object_Dlg_Active = false;
	flag_Header_MAP_Mode_Visible = false;
}

CL64_Interface::~CL64_Interface()
{
}

// *************************************************************************
// *			Select_Tab:- Terry and Hazel Flanigan 2026				   *
// *************************************************************************
void CL64_Interface::Select_Tab(int Tab_ID)
{
	Hide_Tab_Dialogs(); // Hide all tab dialogs first

	switch (Tab_ID)
	{
	case Enums::Tab_ID_TEXTURES:
		flag_Tab_Texture = true;
		Show_Textures_Dialog(true);
		break;

	case Enums::Tab_ID_TEMPLATES:
		flag_Tab_Templates = true;
		Show_TemplatesDialog(true);
		break;

	case Enums::Tab_ID_GROUPS:
		flag_Tab_Group = true;
		Show_Brushes_Dialog(true);
		App->CL_Properties_Brushes->Fill_ListBox();
		break;

	default:
		// Handle unexpected Tab_ID
		break;
	}

	// Redraw the window
	RedrawWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Hide_Tab_Dialogs:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Interface::Hide_Tab_Dialogs()
{
	// Reset all tab flags to indicate they are hidden
	flag_Tab_Texture = false;
	flag_Tab_Group = false;
	flag_Tab_Templates = false;
	
	// Hide the respective dialogs for textures, brushes, and templates
	Show_Materials_Dialog(false);
	Show_Brushes_Dialog(false);
	Show_TemplatesDialog(false);
	Show_Textures_Dialog(false);

	// Redraw
	RedrawWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Show_Brushes_Dialog:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Interface::Show_Brushes_Dialog(bool Show)
{
	ShowWindow(App->CL_Properties_Brushes->BrushesDlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Show_Textures_Dialog:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Interface::Show_Textures_Dialog(bool Show)
{
	ShowWindow(App->CL_Properties_Textures->TexturesDlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Show_Materials_Dialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_Materials_Dialog(bool Show)
{
	//ShowWindow(App->CL_Properties_Materials->Materials_Dlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Show_TemplatesDialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_TemplatesDialog(bool Show)
{
	ShowWindow(App->CL_Properties_Templates->TemplatesDlg_Hwnd, Show);
}

// *************************************************************************
// *	Show_Faces_Panel_Control:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_Faces_Panel_Control(bool show)
{
	ShowWindow(App->CL_Faces_Control->Faces_Control_Dlg_hWnd, show);
	flag_Faces_Con_Dlg_Active = show;
}

// *************************************************************************
// *		Position_Face_Options_Dlg:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Interface::Position_Face_Options_Dlg(void)
{
	RECT rect;
	GetWindowRect(App->MainHwnd, &rect);

	RECT rect2;
	GetWindowRect(App->CL_Faces_Control->Faces_Control_Dlg_hWnd, &rect2);

	int widthX = rect2.right - rect2.left;

	int Pos_X = (rect.right / 2);

	SetWindowPos(App->CL_Faces_Control->Faces_Control_Dlg_hWnd, NULL,
		Pos_X - widthX / 2, 120,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

// *************************************************************************
// *	Unselect_Brush_And_Set_Dlgs:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Unselect_Brush_And_Set_Dlgs(void)
{
	if (App->CL_X_Face_Editor->flag_FaceDlg_Active == true)
	{
		App->CL_X_Face_Editor->Close_Faces_Dialog();
	}

	App->CL_Doc->Set_Tool_GeneralSelect();
	App->CL_Doc->ResetAllSelections();
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	//App->CL_Doc->UpdateSelected();

	App->CL_Properties_Brushes->Update_SelectedBrushesCount_Dlg();
	
	App->CL_Faces_Control->Reset_Face_Buttons();

	App->CL_Faces_Control->Reset_Brush_Buttons();
	App->CL_Faces_Control->flag_Brush_Select = 1;

	App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(false);

	App->CL_Faces_Control->Reset_Face_Buttons();

	App->CL_Ogre->OGL_Listener->Show_Visuals(false);

	App->CL_Faces_Control->Update_Faces_Dialog();

	//EnableMenuItem(App->Menu_Map, ID_EDIT_DELETE, MF_DISABLED | MF_GRAYED);
}

// *************************************************************************
// *		Enable_Face_Buttons:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Interface::Enable_Face_Buttons(bool option)
{
	auto& Face_Dlg = App->CL_Faces_Control;

	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_FACES_NONE), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_FACES_ALL), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_FACE_PREV), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_FACE_NEXT), option);
	
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_FACE_FACEEDITOR), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_FACE_SHOWSELECTEDFACE), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_TT_CB_FACES), option);

}

// *************************************************************************
// *		Enable_All_Face_Buttons:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Enable_All_Face_Buttons(bool option)
{
	auto& Face_Dlg = App->CL_Faces_Control;

	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_MOVE), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_SCALE), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_ROTATE), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_SHEAR), false);
	
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_FACES_NONE), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_FACES_ALL), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_FACE_PREV), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_TT_FACE_NEXT), option);

	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_FACE_FACEEDITOR), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_BT_FACE_SHOWSELECTEDFACE), option);
	EnableWindow(GetDlgItem(Face_Dlg->Faces_Control_Dlg_hWnd, IDC_TT_CB_FACES), option);

}

// **************************************************************************
// *			Show_FileView:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_Interface::Show_FileView(bool show)
{
	if (show == true)
	{
		flag_FileView_Active = true;
		ShowWindow(App->ListPanel, true);
	}
	else
	{
		flag_FileView_Active = false;
		ShowWindow(App->ListPanel, false);
	}

	RedrawWindow(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// **************************************************************************
// *		Show_Properties_Object_Dlg:- Terry and Hazel Flanigan 2026
// **************************************************************************
void CL64_Interface::Show_Properties_Object_Dlg(bool show)
{
	if (show == true)
	{
		flag_Properties_Object_Dlg_Active = true;
		ShowWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, true);
	}
	else
	{
		flag_Properties_Object_Dlg_Active = false;
		ShowWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, false);
	}

	RedrawWindow(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Enable_Top_Tabs_Buttons:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Interface::Enable_Top_Tabs_Buttons(bool Enable)
{
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_HD_PREVIEW), Enable);
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_HD_SCENEEDITOR), Enable);
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_TT_FILEVIEW), Enable);
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_TT_OBJ_DATA), Enable);
}

// *************************************************************************
// *	  	Enable_Properties_Tabs:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Interface::Enable_Properties_Tabs(bool Enable)
{
	EnableWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, Enable);
}

// **************************************************************************
// *		Show_Top_Header_MAP_Mode:- Terry and Hazel Flanigan 2024		*
// **************************************************************************
void CL64_Interface::Show_Top_Header_MAP_Mode(bool Enable)
{
	if (Enable == 1)
	{
		flag_Header_MAP_Mode_Visible = true;
		ShowWindow(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, true);
	}
	else
	{
		flag_Header_MAP_Mode_Visible = false;
		ShowWindow(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, false);
	}
}
