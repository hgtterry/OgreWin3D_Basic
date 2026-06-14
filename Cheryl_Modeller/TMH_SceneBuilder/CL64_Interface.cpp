/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
	Materials_Dlg_Active = false;
	Motions_Dlg_Active = false;
	Textures_Dlg_Assimp_Active = false;

	flag_Tab_Texture = true;
	flag_Tab_Templates = false;
	flag_Tab_Group = false;


	flag_Grids_Are_Visible = false;
	flag_Properties_Dlg_Active = false;
}

CL64_Interface::~CL64_Interface()
{
}

// *************************************************************************
// *			Reset_All_Dialogs:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Reset_All_Dialogs(void)
{
	ShowWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, false);

	App->CL_Top_Tabs->Reset_Bmps();

	if (App->CL_Dialogs->flag_FileViewer_Active == true)
	{
		App->CL_Dialogs->flag_FileViewer_Active = false;
		EndDialog(App->CL_Dialogs->FileViewer_Dlg_Hwnd, LOWORD(0));
	}

	if (App->CL_Dialogs->flag_General_ListBox_Active == true)
	{
		App->CL_Dialogs->flag_General_ListBox_Active = false;
		EndDialog(App->CL_Dialogs->ListBox_Dlg_Hwnd, LOWORD(0));
	}

}

// *************************************************************************
// *  Deselect_All_Brushes_Update_Dlgs:- Terry and Hazel Flanigan 2025     *
// *************************************************************************
void CL64_Interface::Deselect_All_Brushes_Update_Dlgs(void)
{
	App->CL_Doc->Set_Tool_GeneralSelect();
	App->CL_Doc->ResetAllSelections();
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	//App->CL_Doc->UpdateSelected();

	App->CL_Properties_Brushes->Update_SelectedBrushesCount_Dlg();
	App->CL_Top_Tabs->Enable_TopBar_Brush_Buttons(false, false);
	App->CL_Top_Tabs->Enable_TopBar_Face_Buttons(false);


	App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();

	App->CL_Properties_Textures->Enable_FaceProps_Button(false);

	App->CL_Top_Tabs->Reset_Brush_Buttons();
	App->CL_Top_Tabs->flag_Brush_Select = 1;

	App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(false);

	App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();

}

// *************************************************************************
// *	  Show_Properties_Panel:- Terry Mo and Hazel 2026				   *
// *************************************************************************
void CL64_Interface::Show_Properties_Panel(bool Show)
{
	if (Show == true)
	{
		ShowWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, true);
		flag_Properties_Dlg_Active = true;
		CheckMenuItem(App->Menu_Map, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		ShowWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, false);
		flag_Properties_Dlg_Active = false;
		CheckMenuItem(App->Menu_Map, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// *************************************************************************
// *		Move_FileView_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Interface::Move_FileView_Window(void)
{
	POINT p = { 0 };

	int Diff = MapWindowPoints(App->CL_View_3D->RenderWin3D_hWnd, NULL, &p, 1);

	int PosX = p.x;
	int PosY = p.y;

	SetWindowPos(App->ListPanel, NULL, PosX + 0, PosY + 5,0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

// *************************************************************************
// *			Position_Properties_Dlg:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Interface::Position_Properties_Dlg(void)
{
	/*POINT p = { 0 };

	int Diff = MapWindowPoints(App->ViewGLhWnd, NULL, &p, 1);

	HDWP hdwp;

	RECT rect;
	GetWindowRect(App->ViewGLhWnd, &rect);

	int widthX = rect.right - rect.left;
	int heightY = rect.bottom - rect.top;

	int PosX = p.x;
	int PosY = p.y;

	hdwp = BeginDeferWindowPos(2);

	DeferWindowPos(hdwp, App->CL_Properties_Scene->Properties_Dlg_hWnd, NULL, p.x + widthX - 255, PosY + 5,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);*/

	return 0;// EndDeferWindowPos(hdwp);
}

// *************************************************************************
// *			Position_Tabs_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Interface::Position_Tabs_Dlg(void)
{
	POINT position = { 0 };

	int Diff = MapWindowPoints(App->CL_Views_Com->Main_View_Dlg_Hwnd, NULL, &position, 1);

	RECT rect;
	GetWindowRect(App->CL_Views_Com->Main_View_Dlg_Hwnd, &rect);

	int widthX = rect.right - rect.left;
	int heightY = rect.bottom - rect.top;

	SetWindowPos(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, 
		position.x + widthX - 300, position.y + 25,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

// *************************************************************************
// *		Position_Motions_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Position_Motions_Dlg(void)
{
	POINT position = { 0 };

	// Map the window points to get the current position
	int offset = MapWindowPoints(App->CL_Views_Com->Main_View_Dlg_Hwnd, NULL, &position, 1);

	RECT mainDialogRect;
	GetWindowRect(App->CL_Views_Com->Main_View_Dlg_Hwnd, &mainDialogRect);

	// Calculate the width and height of the main dialog
	int dialogWidth = mainDialogRect.right - mainDialogRect.left;
	int dialogHeight = mainDialogRect.bottom - mainDialogRect.top;

	// Set the position of the Motions dialog
	SetWindowPos(App->CL_Properties_Motions->Motions_Dlg_Hwnd, NULL,
		position.x, dialogHeight,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// *************************************************************************
// *			Resize_FileView:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Interface::Resize_FileView(void)
{
	RECT clientRect;
	HDWP hdwp;

	// Get the dimensions of the client area
	GetClientRect(App->ListPanel, &clientRect);

	// Calculate new dimensions based on the client area
	int widthClient = clientRect.right - clientRect.left - 1010;
	int newWidth = 417 + widthClient + 200;
	int heightClient = clientRect.bottom - clientRect.top;
	int newHeight = heightClient - 150;

	// Retrieve handles for the controls to be resized
	HWND treeControl = GetDlgItem(App->ListPanel, IDC_TREE1);

	// Begin the process of deferring window position changes
	hdwp = BeginDeferWindowPos(2); // Adjusted to 2 since we have two controls

	// Defer the resizing of the tree control
	DeferWindowPos(hdwp, treeControl, NULL, 2, 2,
		newWidth + 388, newHeight + 145, SWP_NOZORDER);

	// Apply the deferred window position changes
	return EndDeferWindowPos(hdwp);
}

// *************************************************************************
// *			Show_Motions_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_Motions_Dlg(bool show)
{
	if (show == true)
	{
		App->CL_Properties_Motions->Show_Motions_Dialog(true);
		Motions_Dlg_Active = true;
		CheckMenuItem(App->Menu_Map, ID_WINDOW_MOTIONS, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		App->CL_Properties_Motions->Show_Motions_Dialog(false);
		Motions_Dlg_Active = false;
		CheckMenuItem(App->Menu_Map, ID_WINDOW_MOTIONS, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// *************************************************************************
// *	Enable_Change_Textures_Button:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Interface::Enable_Change_Textures_Button(bool option)
{
	if (option == true)
	{
		EnableWindow(GetDlgItem(App->CL_Properties_Materials->Materials_Dlg_Hwnd, IDC_BT_AT_CHANGETEXTURE), true);
	}
	else
	{
		EnableWindow(GetDlgItem(App->CL_Properties_Materials->Materials_Dlg_Hwnd, IDC_BT_AT_CHANGETEXTURE), false);
	}
}

// *************************************************************************
// *		Menu_Enable_OgreExport:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Menu_Enable_OgreExport(bool option)
{
	if (option == true)
	{
		EnableMenuItem(App->Menu_Map, ID_EXPORT_OGRE3D, MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		EnableMenuItem(App->Menu_Map, ID_EXPORT_OGRE3D, MF_BYCOMMAND | MF_GRAYED);
	}
}

// **************************************************************************
// *	  		 Show_file_view:- Terry and Hazel Flanigan 2026				*
// **************************************************************************
void CL64_Interface::Show_file_view(bool show)
{
	HWND Temp = GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_TBINFO);

	if (show == true)
	{
		App->CL_ImGui->flag_Show_Model_Data = true;

		SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_ModelInfoOn_Bmp);
		CheckMenuItem(App->Menu_Map, ID_WINDOW_MODELDATA, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		App->CL_ImGui->flag_Show_Model_Data = false;

		SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_ModelInfo_Bmp);
		CheckMenuItem(App->Menu_Map, ID_WINDOW_MODELDATA, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// **************************************************************************
// *	  		 Set_Editor_Startup:- Terry and Hazel Flanigan 2026			*
// **************************************************************************
void CL64_Interface::Set_Editor_Startup()
{
	Show_file_view(true);

	HMENU Men = GetMenu(App->MainHwnd);

	if (App->flag_Release == true)
	{
		EnableMenuItem(Men, 9, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		EnableMenuItem(Men, ID_IMPORT_AUTODESKFBX, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		EnableMenuItem(Men, ID_FILE_NEWMODEL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		EnableMenuItem(Men, ID_FILE_OPEN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		EnableMenuItem(Men, ID_FILE_SAVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		EnableMenuItem(Men, ID_FILE_SAVEAS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	else
	{
		EnableMenuItem(App->Menu_Map, 9, MF_BYPOSITION | MF_ENABLED);
		EnableMenuItem(Men, ID_IMPORT_AUTODESKFBX, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(Men, ID_FILE_NEWMODEL, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(Men, ID_FILE_OPEN, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(Men, ID_FILE_SAVE, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(Men, ID_FILE_SAVEAS, MF_BYCOMMAND | MF_ENABLED);
	}

}

// *************************************************************************
// *				Set_Title:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Interface::Set_Title(bool Clear)
{
	char TitleBar[MAX_PATH];
	strcpy(TitleBar, App->App_Title);
	strcat(TitleBar, "    ");

	if (Clear == false)
	{
		strcat(TitleBar, App->CL_Model->Loaded_PathFileName);
	}

	SetWindowText(App->MainHwnd, TitleBar);
}

// *************************************************************************
// *			Show_Grid_Windows:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_Grid_Windows(bool Show)
{
	if (Show == true)
	{
		ShowWindow(App->CL_View_Top_Left->Top_Left_Window_Hwnd, true);
		ShowWindow(App->CL_View_Top_Right->Top_Right_Window_Hwnd, true);
		ShowWindow(App->CL_View_Bottom_Left->Bottom_Left_Window_Hwnd, true);

		flag_Grids_Are_Visible = true;
	}
	else
	{
		ShowWindow(App->CL_View_Top_Left->Top_Left_Window_Hwnd, false);
		ShowWindow(App->CL_View_Top_Right->Top_Right_Window_Hwnd, false);
		ShowWindow(App->CL_View_Bottom_Left->Bottom_Left_Window_Hwnd, false);

		flag_Grids_Are_Visible = false;
	}
}

// *************************************************************************
// *	Show_TopTabs_Brushes_Panel:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_TopTabs_Brushes_Panel(bool show)
{
	ShowWindow(App->CL_Top_Tabs->TopTabs_Brushes_Dlg_hWnd, show);
}

// *************************************************************************
// *	Enable_TopTabs_Brushes_Buttons:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Interface::Enable_TopTabs_Brushes_Buttons(bool option)
{
	auto& Win_hWnd = App->CL_Top_Tabs->TopTabs_Brushes_Dlg_hWnd;

	// Array of button IDs to enable or disable
	const int buttonIDs[] = 
	{
		IDC_BT_TT_BRUSH_SELECT,
		IDC_BT_TT_BRUSH_MOVE,
		IDC_BT_TT_BRUSH_SCALE,
		IDC_BT_TT_BRUSH_ROTATE,
		IDC_BT_TT_BRUSH_SHEAR
	};

	// Iterate through the button IDs and set their enabled state
	for (int id : buttonIDs)
	{
		EnableWindow(GetDlgItem(Win_hWnd, id), option);
	}
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
		if (App->CL_Model->Editor_Setup_Mode == Enums::Editor_Setup_Mode_Create_Model)
		{
			Show_Textures_Dialog(true);
		}
		else
		{
			Show_Materials_Dialog(true);
		}
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
	App->CL_Interface->Show_Textures_Dialog(false);
	App->CL_Interface->Show_Materials_Dialog(false);
	App->CL_Interface->Show_Brushes_Dialog(false);
	App->CL_Interface->Show_TemplatesDialog(false);

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
// *	  	Show_Textures_Dialog:- Terry Mo and Hazel 2026				   *
// *************************************************************************
void CL64_Interface::Show_Textures_Dialog(bool Show)
{
	ShowWindow(App->CL_Properties_Textures->Textures_Dlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Show_Materials_Dialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_Materials_Dialog(bool Show)
{
	ShowWindow(App->CL_Properties_Materials->Materials_Dlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Show_TemplatesDialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_TemplatesDialog(bool Show)
{
	ShowWindow(App->CL_Properties_Templates->TemplatesDlg_Hwnd, Show);
}




