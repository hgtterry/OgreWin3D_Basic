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
#include "CL64_MapEditor.h"

CL64_MapEditor::CL64_MapEditor()
{
	Left_Window_Hwnd = NULL;
	Right_Window_Hwnd = NULL;

	Bottom_Left_Hwnd = NULL;
	Bottom_Right_Hwnd = NULL;

	LEFT_WINDOW_WIDTH = 500;

	LEFT_WINDOW_DEPTH = 215;
	TOP_POS_BOTLEFT = 215;

	RIGHT_MINIMUM_SPACE = 1000;
	LEFT_MINIMUM_SPACE = 15;

	nleftWnd_width = LEFT_WINDOW_WIDTH;
	nleftWnd_Depth = LEFT_WINDOW_DEPTH;

	Do_Width = 0;
	Do_Depth = 0;
	Do_All = 0;

	BackGround_Brush = CreateSolidBrush(RGB(64, 64, 64));

}

CL64_MapEditor::~CL64_MapEditor()
{
}

// *************************************************************************
// *			Init_Map_Views:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Init_Map_Views()
{
	App->CL_MapEditor->Init_Views();
	App->CL_MapEditor->Create_Top_Left_Window();
	App->CL_MapEditor->Create_Top_Right_Window();
	App->CL_MapEditor->Create_Bottom_Left_Window();
	App->CL_MapEditor->Create_Bottom_Right_Window();

	App->CL_MapEditor->Resize_Windows(App->Fdlg, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);
}

// *************************************************************************
// *	  			Init_Views:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Init_Views()
{
	RECT rect;
	GetClientRect(App->Fdlg, &rect);

	Left_Window_Hwnd = NULL;
	Right_Window_Hwnd = NULL;

	Bottom_Left_Hwnd = NULL;
	Bottom_Right_Hwnd = NULL;

	LEFT_WINDOW_WIDTH = rect.right / 2;
	nleftWnd_width = rect.right / 2;

	LEFT_WINDOW_DEPTH = rect.bottom / 2;
	TOP_POS_BOTLEFT = rect.bottom / 2;
	nleftWnd_Depth = LEFT_WINDOW_DEPTH;

	RIGHT_MINIMUM_SPACE = rect.right - 15;
	LEFT_MINIMUM_SPACE = rect.left + 15;
}

// *************************************************************************
// *			Resize_Windowns:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Resize_Windows(HWND hDlg, int NewWidth, int NewDepth)
{
	RECT rect;
	GetClientRect(hDlg, &rect);

	int Top_Windows_Top_Y = 8;
	int Left_Windows_Start_X = 0;

	int NewDepth_Depth = NewDepth - 11;

	MoveWindow(Left_Window_Hwnd,
		Left_Windows_Start_X,
		Top_Windows_Top_Y,
		rect.left + (NewWidth - WIDTH_ADJUST),
		NewDepth_Depth,
		FALSE);

	MoveWindow(Right_Window_Hwnd,
		Left_Windows_Start_X + NewWidth + WIDTH_ADJUST,
		Top_Windows_Top_Y,
		rect.right - (NewWidth + WIDTH_ADJUST),
		NewDepth_Depth,
		FALSE);

	// Bottom Windows
	MoveWindow(Bottom_Left_Hwnd,
		Left_Windows_Start_X,
		rect.top + NewDepth,
		Left_Windows_Start_X + (NewWidth - WIDTH_ADJUST),
		rect.bottom - (NewDepth + BOTTOM_POS_BOTLEFT),
		FALSE);

	MoveWindow(Bottom_Right_Hwnd,
		Left_Windows_Start_X + NewWidth + WIDTH_ADJUST,
		rect.top + NewDepth,
		rect.right - (NewWidth + WIDTH_ADJUST),
		rect.bottom - (NewDepth + BOTTOM_POS_BOTLEFT),
		FALSE);

	RedrawWindow(App->Fdlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *	  	Create_Top_Left_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MapEditor::Create_Top_Left_Window()
{
	Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, App->Fdlg, (DLGPROC)Proc_Top_Left_Window);
}

// *************************************************************************
// *		Proc_Top_Left_Window:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		RECT r;
		GetClientRect(hDlg, &r);

		/*App->CL_MapEditor->VCam[V_TL] = new ViewVars;
		strcpy(App->CL_MapEditor->VCam[V_TL]->Name, "TLV");
		App->CL_MapEditor->VCam[V_TL]->ViewType = 8;
		App->CL_MapEditor->VCam[V_TL]->ZoomFactor = 0.3;

		App->CL_MapEditor->VCam[V_TL]->XCenter = 310;
		App->CL_MapEditor->VCam[V_TL]->YCenter = 174;

		App->CL_MapEditor->VCam[V_TL]->Width = 310;
		App->CL_MapEditor->VCam[V_TL]->Height = 174;

		App->CL_MapEditor->VCam[V_TL]->CamPos = App->CL_Ogre->camNode->getPosition();*/

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;
	}

	/*case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}*/

	/*case WM_SETCURSOR:
	{
		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}*/

	case WM_MOUSEMOVE:
	{


		int			dx, dy;
		POINT		RealCursorPosition;

		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		dx = (RealCursorPosition.x);
		dy = (RealCursorPosition.y);


		/*if (App->CL_MapEditor->flag_Right_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Zoom_View(hDlg, dx, dy);
		}

		if (App->CL_MapEditor->flag_Left_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Pan_View(hDlg, dx, dy);
		}*/

		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		/*GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
		App->CUR = SetCursor(NULL);*/

		return 1;
	}

	case WM_LBUTTONUP:
	{
		/*App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		/*GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 1;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
		App->CUR = SetCursor(NULL);*/

		return 1;
	}

	case WM_RBUTTONUP:
	{
		/*App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_PAINT:
	{
		/*App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
		App->CL_MapEditor->Draw_Screen(hDlg);*/

		return 0;
	}

	}

	return FALSE;
}



// *************************************************************************
// *	  	Create_Top_Right_Window:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Create_Top_Right_Window()
{
	Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_RIGHT, App->Fdlg, (DLGPROC)Proc_Top_Right_Window);
}

// *************************************************************************
// *		Proc_Top_Right_Window:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Proc_Top_Right_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		/*App->CL_MapEditor->VCam[V_TR] = new ViewVars;
		strcpy(App->CL_MapEditor->VCam[V_TR]->Name, "TRV");
		App->CL_MapEditor->VCam[V_TR]->ViewType = 32;
		App->CL_MapEditor->VCam[V_TR]->ZoomFactor = 0.3;

		App->CL_MapEditor->VCam[V_TR]->CamPos = App->CL_Ogre->camNode->getPosition();*/
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;
	}

	/*case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}*/

	/*case WM_SETCURSOR:
	{
		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}*/

	case WM_MOUSEMOVE:
	{
		int			dx, dy;
		POINT		RealCursorPosition;

		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		dx = (RealCursorPosition.x);
		dy = (RealCursorPosition.y);


		/*if (App->CL_MapEditor->flag_Right_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Zoom_View(hDlg, dx, dy);
		}

		if (App->CL_MapEditor->flag_Left_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Pan_View(hDlg, dx, dy);
		}*/

		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		/*GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
		App->CUR = SetCursor(NULL);*/

		return 1;
	}

	case WM_LBUTTONUP:
	{
		/*App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		/*GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 1;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
		App->CUR = SetCursor(NULL);*/

		return 1;
	}

	case WM_RBUTTONUP:
	{
		/*App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_PAINT:
	{
		/*App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
		App->CL_MapEditor->Draw_Screen(hDlg);*/

		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  Create_Bottom_Left_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MapEditor::Create_Bottom_Left_Window()
{
	Bottom_Left_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_LEFT, App->Fdlg, (DLGPROC)Proc_Bottom_Left_Window);
}

// *************************************************************************
// *		Proc_Bottom_Left_Window:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Proc_Bottom_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		/*App->CL_MapEditor->VCam[V_BL] = new ViewVars;
		strcpy(App->CL_MapEditor->VCam[2]->Name, "BLV");
		App->CL_MapEditor->VCam[V_BL]->ViewType = 16;
		App->CL_MapEditor->VCam[V_BL]->ZoomFactor = 0.3;

		App->CL_MapEditor->VCam[V_BL]->CamPos = App->CL_Ogre->camNode->getPosition();*/
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;
	}

	/*case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}*/

	/*case WM_SETCURSOR:
	{
		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}*/

	case WM_MOUSEMOVE:
	{
		int			dx, dy;
		POINT		RealCursorPosition;

		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		dx = (RealCursorPosition.x);
		dy = (RealCursorPosition.y);


		/*if (App->CL_MapEditor->flag_Right_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Zoom_View(hDlg, dx, dy);
		}

		if (App->CL_MapEditor->flag_Left_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Pan_View(hDlg, dx, dy);
		}*/

		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		/*GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];
		App->CUR = SetCursor(NULL);*/

		return 1;
	}

	case WM_LBUTTONUP:
	{
		/*App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		/*GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];

		App->CUR = SetCursor(NULL);*/
		return 1;
	}

	case WM_RBUTTONUP:
	{
		/*App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_PAINT:
	{
		/*App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];
		App->CL_MapEditor->Draw_Screen(hDlg);*/
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  Create_Bottom_Right_Window:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_MapEditor::Create_Bottom_Right_Window()
{
	Bottom_Right_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_RIGHT, App->Fdlg, (DLGPROC)Bottom_Right_Proc);
}

// *************************************************************************
// *			Bottom_Right_Proc:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Bottom_Right_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_LBUTTONDOWN:
	{
		//App->Say("Bottom Right");
		return 1;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;
	}
	}

	return FALSE;
}
