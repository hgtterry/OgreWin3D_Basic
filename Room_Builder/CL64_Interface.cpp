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
#include "CL64_App.h"
#include "CL64_Interface.h"

CL64_Interface::CL64_Interface()
{
	flag_Tab_Templates = true;
	flag_Tab_Templates = false;
	flag_Tab_Group = false;
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
		//if (App->CL_Model->Editor_Setup_Mode == Enums::Editor_Setup_Mode_Create_Model)
		{
			Show_Textures_Dialog(true);
		}
		/*else
		{
			Show_Materials_Dialog(true);
		}*/
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
	//ShowWindow(App->CL_Properties_Materials->Materials_Dlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Show_TemplatesDialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Interface::Show_TemplatesDialog(bool Show)
{
	ShowWindow(App->CL_Properties_Templates->TemplatesDlg_Hwnd, Show);
}
