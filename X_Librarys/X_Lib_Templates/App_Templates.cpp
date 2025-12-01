#include "pch.h"
#include "CL64_App.h"
#include "App_Templates.h"

App_Templates::App_Templates(void)
{
	CL_CreateBox =			nullptr;
	CL_CreateCylinder =		nullptr;
	CL_CreateArch =			nullptr;
	CL_CreateCone =			nullptr;
	CL_CreateStaircase =	nullptr;

	Shape_Dlg_hWnd =		nullptr;
}

App_Templates::~App_Templates(void)
{
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* App_Templates::GetVersion()
{
	return (LPSTR)" TMH_Scene_Builder :-- Templates [ 01-12-25 ] Build 1 ";
}

// *************************************************************************
// *			Init_Templates:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void App_Templates::Init_Templates(void)
{
	CL_CreateBox =			new CreateBoxDialog();
	CL_CreateCylinder =		new CreateCylDialog();
	CL_CreateArch =			new CreateArchDialog();
	CL_CreateCone =			new CreateConeDialog();
	CL_CreateStaircase =	new CreateStaircaseDialog();
}

// *************************************************************************
// *	Enable_Map_Editor_Dialogs:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void App_Templates::Enable_Map_Editor_Dialogs(bool Enable)
{
	if (Enable == true)
	{
		EnableWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, true);
		EnableWindow(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, true);

		HMENU Hmenu = GetMenu(App->MainHwnd);

		EnableMenuItem(GetSystemMenu(App->MainHwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);

		for (int i = 0; i < GetMenuItemCount(Hmenu); ++i)
		{
			EnableMenuItem(Hmenu, i, MF_BYPOSITION | MF_ENABLED);
		}

		DrawMenuBar(App->MainHwnd);

	}
	else
	{
		EnableWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, false);
		EnableWindow(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, false);

		EnableMenuItem(GetSystemMenu(App->MainHwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

		HMENU Hmenu = GetMenu(App->MainHwnd);

		for (int i = 0; i < GetMenuItemCount(Hmenu); ++i)
		{
			EnableMenuItem(Hmenu, i, MF_BYPOSITION | MF_GRAYED);
		}

		DrawMenuBar(App->MainHwnd);

	}
}

// *************************************************************************
// *		Enable_Shape_Dialog:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void App_Templates::Enable_Shape_Dialog(bool Enable)
{
	if (Enable == true)
	{
		EnableWindow(Shape_Dlg_hWnd, true);
	}
	else
	{
		EnableWindow(Shape_Dlg_hWnd, false);
	}
}
