/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
// *		Move_FileView_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Interface::Move_FileView_Window(void)
{
	POINT p = { 0 };

	int Diff = MapWindowPoints(App->ViewGLhWnd, NULL, &p, 1);

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

	int Diff = MapWindowPoints(App->CL_Editor_Map->Main_View_Dlg_Hwnd, NULL, &position, 1);

	RECT rect;
	GetWindowRect(App->CL_Editor_Map->Main_View_Dlg_Hwnd, &rect);

	int widthX = rect.right - rect.left;
	int heightY = rect.bottom - rect.top;

	SetWindowPos(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, 
		position.x + widthX - 290, position.y + 25,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

// *************************************************************************
// *		Position_Materials_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Position_Materials_Dlg(void)
{
	POINT position = { 0 };

	// Map the window points to get the current position
	int offset = MapWindowPoints(App->CL_Editor_Map->Main_View_Dlg_Hwnd, NULL, &position, 1);

	RECT mainDialogRect;
	GetWindowRect(App->CL_Editor_Map->Main_View_Dlg_Hwnd, &mainDialogRect);

	// Calculate the width and height of the main dialog
	int dialogWidth = mainDialogRect.right - mainDialogRect.left;
	int dialogHeight = mainDialogRect.bottom - mainDialogRect.top;

	// Set the position of the Textures dialog
	SetWindowPos(App->CL_Properties_Materials->Textures_Dlg_Hwnd_Ogre, NULL,
		position.x + dialogWidth - 300, position.y + 25,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// *************************************************************************
// *		Position_Motions_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Position_Motions_Dlg(void)
{
	POINT position = { 0 };

	// Map the window points to get the current position
	int offset = MapWindowPoints(App->CL_Editor_Map->Main_View_Dlg_Hwnd, NULL, &position, 1);

	RECT mainDialogRect;
	GetWindowRect(App->CL_Editor_Map->Main_View_Dlg_Hwnd, &mainDialogRect);

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
// *			Show_Materials_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_Materials_Dlg(bool show)
{
	if (show == true)
	{
		App->CL_Properties_Materials->Show_Materials_Dialog_Ogre(true);
		Materials_Dlg_Active = true;
		CheckMenuItem(App->Menu_Map, ID_WINDOW_MATERIALS, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		App->CL_Properties_Materials->Show_Materials_Dialog_Ogre(false);
		Materials_Dlg_Active = false;
		CheckMenuItem(App->Menu_Map, ID_WINDOW_MATERIALS, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// *************************************************************************
// *		Menu_Enable_Materials:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Menu_Enable_Materials(bool option)
{
	if (option == true)
	{
		EnableMenuItem(App->Menu_Map, ID_WINDOW_MATERIALS, MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		EnableMenuItem(App->Menu_Map, ID_WINDOW_MATERIALS, MF_BYCOMMAND | MF_GRAYED);
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
// *	  		 Show_file_view:- Terry and Hazel Flanigan 2026				*
// **************************************************************************
void CL64_Interface::Enable_Debug_Menu(bool option)
{
	if (option == true)
	{
		EnableMenuItem(App->Menu_Map, 7, MF_BYPOSITION | MF_ENABLED);
	}
	else
	{
		EnableMenuItem(App->Menu_Map, 7, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}
}

// **************************************************************************
// *	  		 Set_Editor_Startup:- Terry and Hazel Flanigan 2026			*
// **************************************************************************
void CL64_Interface::Set_Editor_Startup()
{
	Show_file_view(true);

	if (App->flag_Release == true)
	{
		Enable_Debug_Menu(false);
	}
	else
	{
		Enable_Debug_Menu(true);
	}
}




