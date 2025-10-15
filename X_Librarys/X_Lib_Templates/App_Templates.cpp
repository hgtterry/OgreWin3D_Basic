#include "pch.h"
#include "App_Templates.h"

App_Templates::App_Templates(void)
{
	Shape_Dlg_hWnd = nullptr;
}

App_Templates::~App_Templates(void)
{
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
