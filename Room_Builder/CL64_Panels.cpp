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
#include "CL64_Panels.h"

CL64_Panels::CL64_Panels()
{
}

CL64_Panels::~CL64_Panels()
{
}

// *************************************************************************
// *  Deselect_All_Brushes_Update_Dlgs:- Terry and Hazel Flanigan 2025     *
// *************************************************************************
void CL64_Panels::Deselect_All_Brushes_Update_Dlgs(void)
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
void CL64_Panels::Move_FileView_Window(void)
{
	POINT p = { 0 };

	int Diff = MapWindowPoints(App->ViewGLhWnd, NULL, &p, 1);

	int PosX = p.x;
	int PosY = p.y;

	SetWindowPos(App->ListPanel, NULL, PosX + 0, PosY + 5,0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

// *************************************************************************
// *			Place_Properties_Dlg:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Panels::Place_Properties_Dlg(void)
{
	POINT p = { 0 };

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
		0, 0, SWP_NOSIZE | SWP_NOZORDER);

	return EndDeferWindowPos(hdwp);
}

// *************************************************************************
// *			Resize_FileView:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Panels::Resize_FileView(void)
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
	HWND buttonControl = GetDlgItem(App->ListPanel, IDC_BT_MAINENVIRONMENT);

	// Begin the process of deferring window position changes
	hdwp = BeginDeferWindowPos(2); // Adjusted to 2 since we have two controls

	// Defer the resizing of the tree control
	DeferWindowPos(hdwp, treeControl, NULL, 2, 2,
		newWidth + 388, newHeight + 100, SWP_NOZORDER);

	// Defer the positioning of the button control
	DeferWindowPos(hdwp, buttonControl, NULL, 12, newHeight + 110,
		0, 7, SWP_NOSIZE | SWP_NOZORDER);

	// Apply the deferred window position changes
	return EndDeferWindowPos(hdwp);
}


// *************************************************************************
// *	Enable_Scene_Editor_Dialogs:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Panels::Enable_Scene_Editor_Dialogs(bool Enable)
{
	if (Enable == true)
	{
		EnableWindow(App->CL_Editor_Scene->Scene_Headers_hWnd, true);
		EnableWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, true);
		EnableWindow(App->ListPanel, true);

		HMENU Hmenu = GetMenu(App->MainHwnd);

		EnableMenuItem(GetSystemMenu(App->MainHwnd, FALSE), SC_CLOSE,MF_BYCOMMAND | MF_ENABLED);

		for (int i = 0; i < GetMenuItemCount(Hmenu); ++i)
		{
			EnableMenuItem(Hmenu, i, MF_BYPOSITION | MF_ENABLED);
		}

		DrawMenuBar(App->MainHwnd);

	}
	else
	{
		EnableWindow(App->CL_Editor_Scene->Scene_Headers_hWnd, false);
		EnableWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, false);
		EnableWindow(App->ListPanel, false);

		EnableMenuItem(GetSystemMenu(App->MainHwnd, FALSE), SC_CLOSE,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		
		HMENU Hmenu = GetMenu(App->MainHwnd);

		for (int i = 0; i < GetMenuItemCount(Hmenu); ++i)
		{
			EnableMenuItem(Hmenu, i, MF_BYPOSITION | MF_GRAYED);
		}

		DrawMenuBar(App->MainHwnd);

	}
}

// *************************************************************************
// *			Position_Tabs_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Panels::Position_Tabs_Dlg(void)
{
	POINT p = { 0 };

	int Diff = MapWindowPoints(App->CL_Editor_Map->Main_View_Dlg_Hwnd, NULL, &p, 1);

	HDWP hdwp;

	RECT rect;
	GetWindowRect(App->CL_Editor_Map->Main_View_Dlg_Hwnd, &rect);

	int widthX = rect.right - rect.left;
	int heightY = rect.bottom - rect.top;

	int PosX = p.x;
	int PosY = p.y;

	hdwp = BeginDeferWindowPos(2);

	DeferWindowPos(hdwp, App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, p.x + widthX - 265, PosY + 25,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);

	return EndDeferWindowPos(hdwp);
}
