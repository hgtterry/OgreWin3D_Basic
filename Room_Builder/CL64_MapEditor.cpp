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

#define	M_PI		((float)3.14159265358979323846f)
#define	TOP_POS					8
#define	BOTTOM_POS				400
#define	SPLITTER_BAR_WIDTH		5
#define WIDTH_ADJUST			2

#define BOTTOM_POS_BOTLEFT		5

typedef struct tagBrushDrawData
{
	const Box3d*	pViewBox;
	HDC 			pDC;
	ViewVars*		v;
	int				GroupId;
	CL64_Doc*		pDoc;
	BrushFlagTest	FlagTest;
	Ogre::uint32	Color;
} BrushDrawData;

CL64_MapEditor::CL64_MapEditor()
{
	Main_Dlg_Hwnd = NULL;

	GridSize = 128, 
	GridSnapSize = 8;

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

	Pen_Fine_Grid = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	Pen_Grid = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
	PenTemplate = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	PenBrushes = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	int Count = 0;
	while (Count < 3)
	{
		VCam[Count] = nullptr;
		Count++;
	}

	MemoryhDC = nullptr;
}

CL64_MapEditor::~CL64_MapEditor()
{
}

// *************************************************************************
// *	  			Init_Views:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Init_Views()
{
	RECT rect;
	GetClientRect(Main_Dlg_Hwnd, &rect);

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

	RedrawWindow(Main_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *			Init_Map_Views:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Init_Map_Views()
{
	Main_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAPEDITOR, App->MainHwnd, (DLGPROC)Proc_Main_Dlg);

	App->CL_MapEditor->Create_Top_Left_Window();
	App->CL_MapEditor->Create_Top_Right_Window();
	App->CL_MapEditor->Create_Bottom_Left_Window();
	App->CL_MapEditor->Create_Bottom_Right_Window();

	RECT rcl;
	GetClientRect(App->MainHwnd, &rcl);
	MoveWindow(App->CL_MapEditor->Main_Dlg_Hwnd, 0, 50, rcl.right, rcl.bottom - 50, TRUE);
	App->CL_MapEditor->Init_Views();
	RedrawWindow(App->CL_MapEditor->Main_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *			Proc_Main_Dlg:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR	hcSizeEW = NULL;
	static HCURSOR	hcSizeNS = NULL;
	static HCURSOR	hcBoth = NULL;

	static  BOOL        xSizing;
	static  BOOL        ySizing;

	static UINT			nwnd_resizing = 0;

	RECT rect;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		hcSizeEW = LoadCursor(NULL, IDC_SIZEWE);
		hcSizeNS = LoadCursor(NULL, IDC_SIZENS);
		hcBoth = LoadCursor(NULL, IDC_SIZEALL);

		App->CL_MapEditor->Main_Dlg_Hwnd = hDlg;
		
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	/*case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}*/

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)App->AppBackground);
		EndPaint(hDlg, &ps);
		return 0;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_SIZE:
	{
		App->CL_MapEditor->Init_Views();
		App->CL_MapEditor->Resize_Windows(hDlg, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);

		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		//if (App->CL_MapEditor->flag_Right_Button_Down == 0)
		{
			int                 xPos;
			int                 yPos;

			// Varible used to get the mouse cursor x and y co-ordinates
			xPos = (int)LOWORD(lParam);
			yPos = (int)HIWORD(lParam);

			// Checks whether the mouse is over the splitter window
			xSizing = (xPos > App->CL_MapEditor->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_MapEditor->nleftWnd_width + SPLITTER_BAR_WIDTH);
			ySizing = (yPos > App->CL_MapEditor->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_MapEditor->nleftWnd_Depth + SPLITTER_BAR_WIDTH);

			if (xSizing && ySizing == 0)
			{
				// Api to capture mouse input
				SetCapture(hDlg);
				if (xSizing)
				{
					SetCursor(hcSizeEW);
				}

			}

			if (ySizing && xSizing == 0)
			{
				// Api to capture mouse input
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(hcSizeNS);
				}

			}

			if (xSizing && ySizing)
			{
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(hcBoth);;
				}
			}
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		//if (App->CL_MapEditor->flag_Right_Button_Down == 0)
		{
			if (xSizing)
			{
				RECT    focusrect;
				HDC     hdc;

				ReleaseCapture();

				hdc = GetDC(hDlg);
				GetClientRect(hDlg, &rect);

				if (xSizing)
				{
					SetRect(&focusrect, App->CL_MapEditor->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
						App->CL_MapEditor->nleftWnd_width + WIDTH_ADJUST,
						rect.bottom - 80);

					DrawFocusRect(hdc, &focusrect);

					xSizing = FALSE;
				}

				ReleaseDC(hDlg, hdc);
			}

			if (ySizing)
			{
				HDC     hdc;

				ReleaseCapture();

				hdc = GetDC(hDlg);
				GetClientRect(hDlg, &rect);

				if (ySizing)
				{
					ySizing = FALSE;
				}

				ReleaseDC(hDlg, hdc);
			}

			App->CL_MapEditor->Resize_Windows(hDlg, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);
		}

		return 1;
	}

	case WM_MOUSEMOVE:
	{
		//if (App->CL_MapEditor->flag_Right_Button_Down == 0)
		{
			int   xPos;
			int   yPos;

			// Get the x and y co-ordinates of the mouse
			xPos = (int)LOWORD(lParam);
			yPos = (int)HIWORD(lParam);

			if (xPos < App->CL_MapEditor->LEFT_MINIMUM_SPACE || xPos > App->CL_MapEditor->RIGHT_MINIMUM_SPACE)
			{
				return 0;
			}

			// Checks if the left button is pressed during dragging the splitter
			if (wParam == MK_LBUTTON)
			{

				if (xSizing && App->CL_MapEditor->Do_Width == 1)
				{
					RECT    focusrect;
					HDC     hdc;

					hdc = GetDC(hDlg);
					GetClientRect(hDlg, &rect);

					if (xSizing && App->CL_MapEditor->Do_Width == 1)
					{
						SetRect(&focusrect, App->CL_MapEditor->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
							App->CL_MapEditor->nleftWnd_width + WIDTH_ADJUST,
							rect.bottom - 6);

						DrawFocusRect(hdc, &focusrect);

						App->CL_MapEditor->nleftWnd_width = xPos;

						SetRect(&focusrect, App->CL_MapEditor->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
							App->CL_MapEditor->nleftWnd_width + WIDTH_ADJUST,
							rect.bottom - 6);

						DrawFocusRect(hdc, &focusrect);
					}

					ReleaseDC(hDlg, hdc);
				}

				if (ySizing && App->CL_MapEditor->Do_Depth == 1)
				{
					RECT    focusrect;
					HDC     hdc;

					hdc = GetDC(hDlg);
					GetClientRect(hDlg, &rect);

					if (ySizing && App->CL_MapEditor->Do_Depth == 1)
					{
						SetRect(&focusrect, 0, App->CL_MapEditor->nleftWnd_Depth, rect.right, App->CL_MapEditor->nleftWnd_Depth + (WIDTH_ADJUST * 2));

						DrawFocusRect(hdc, &focusrect);

						App->CL_MapEditor->nleftWnd_Depth = yPos;

						SetRect(&focusrect, 0, App->CL_MapEditor->nleftWnd_Depth, rect.right, App->CL_MapEditor->nleftWnd_Depth + (WIDTH_ADJUST * 2));

						DrawFocusRect(hdc, &focusrect);
					}

					ReleaseDC(hDlg, hdc);
				}

			}

			if ((xPos > App->CL_MapEditor->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_MapEditor->nleftWnd_width + SPLITTER_BAR_WIDTH))
			{
				if (App->CL_MapEditor->Do_All == 0)
				{
					SetCursor(hcSizeEW);
				}

				App->CL_MapEditor->Do_Width = 1;
			}
			else
			{
				App->CL_MapEditor->Do_Width = 0;
			}

			if ((yPos > App->CL_MapEditor->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_MapEditor->nleftWnd_Depth + SPLITTER_BAR_WIDTH))
			{
				if (App->CL_MapEditor->Do_All == 0)
				{
					SetCursor(hcSizeNS);
				}

				App->CL_MapEditor->Do_Depth = 1;
			}
			else
			{
				App->CL_MapEditor->Do_Depth = 0;
			}

			if (App->CL_MapEditor->Do_Width == 1 && App->CL_MapEditor->Do_Depth == 1)
			{
				SetCursor(hcBoth);
				App->CL_MapEditor->Do_All = 1;
			}
			else
			{
				App->CL_MapEditor->Do_All = 0;
			}
		}

		return 1;
	}


	case WM_COMMAND:
	{
		
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *	  	Create_Top_Left_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MapEditor::Create_Top_Left_Window()
{
	Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, Main_Dlg_Hwnd, (DLGPROC)Proc_Top_Left_Window);
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

		App->CL_MapEditor->VCam[V_TL] = new ViewVars;
		strcpy(App->CL_MapEditor->VCam[V_TL]->Name, "TLV");
		App->CL_MapEditor->VCam[V_TL]->ViewType = 8;
		App->CL_MapEditor->VCam[V_TL]->ZoomFactor = 0.4;

		App->CL_MapEditor->VCam[V_TL]->XCenter = 310;
		App->CL_MapEditor->VCam[V_TL]->YCenter = 174;

		App->CL_MapEditor->VCam[V_TL]->Width = 310;
		App->CL_MapEditor->VCam[V_TL]->Height = 174;

		App->CL_MapEditor->VCam[V_TL]->CamPos = Ogre::Vector3(0, 0, 0);// App->CL_Ogre->camNode->getPosition();

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

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
		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
		App->CL_MapEditor->Draw_Screen(hDlg);

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
	Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_RIGHT, Main_Dlg_Hwnd, (DLGPROC)Proc_Top_Right_Window);
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
		App->CL_MapEditor->VCam[V_TR] = new ViewVars;
		strcpy(App->CL_MapEditor->VCam[V_TR]->Name, "TRV");
		App->CL_MapEditor->VCam[V_TR]->ViewType = 32;
		App->CL_MapEditor->VCam[V_TR]->ZoomFactor = 0.4;

		App->CL_MapEditor->VCam[V_TR]->CamPos =  Ogre::Vector3(0, 0, 0);// App->CL_Ogre->camNode->getPosition();
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

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
		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
		App->CL_MapEditor->Draw_Screen(hDlg);

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
	Bottom_Left_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_LEFT, Main_Dlg_Hwnd, (DLGPROC)Proc_Bottom_Left_Window);
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
		App->CL_MapEditor->VCam[V_BL] = new ViewVars;
		strcpy(App->CL_MapEditor->VCam[2]->Name, "BLV");
		App->CL_MapEditor->VCam[V_BL]->ViewType = 16;
		App->CL_MapEditor->VCam[V_BL]->ZoomFactor = 0.4;

		App->CL_MapEditor->VCam[V_BL]->CamPos =  Ogre::Vector3(0, 0, 0);//App->CL_Ogre->camNode->getPosition();
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

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
		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];
		App->CL_MapEditor->Draw_Screen(hDlg);
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
	Bottom_Right_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_RIGHT, Main_Dlg_Hwnd, (DLGPROC)Bottom_Right_Proc);
	App->CL_Ogre->RenderHwnd = Bottom_Right_Hwnd;
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

// *************************************************************************
// *	  						BrushDraw								   *
// *************************************************************************
signed int CL64_MapEditor::BrushDraw(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData = (BrushDrawData*)lParam;
	

	//if ((pData->GroupId == fdoc_SHOW_ALL_GROUPS) || (Brush_GetGroupId(pBrush) == pData->GroupId))
	{
		//if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
		{
			//if (pDoc->fdocShowBrush(pBrush, pData->pViewBox))
			{
				App->CL_MapEditor->Render_RenderBrushFacesOrtho(pData->v, pBrush, App->CL_MapEditor->MemoryhDC);
			}
		}
	}

	return true;
}

#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

// *************************************************************************
// *						Draw_Screen Terry Flanigan		  			   *
// *************************************************************************
void CL64_MapEditor::Draw_Screen(HWND hwnd)
{
	//flag_IsDrawing = 1;

	int			inidx = 0;
	HDC			RealhDC;
	RECT		Rect;
	BrushDrawData	brushDrawData;

	GetClientRect(hwnd, &Rect);
	Rect.left--;
	Rect.bottom--;

	Current_View->Width = Rect.left;
	Current_View->Height = Rect.bottom;

	Current_View->XScreenScale = Rect.left;
	Current_View->YScreenScale = Rect.bottom;

	/*Current_View->XCenter = Rect.left/2;
	Current_View->YCenter = Rect.bottom/2;*/

	Ogre::Vector3 XTemp;
	Box3d ViewBox;
	inidx = App->CL_Render->Render_GetInidx(Current_View);

	App->CL_Box->Box3d_SetBogusBounds(&ViewBox);
	App->CL_Render->Render_ViewToWorld(Current_View, 0, 0, &XTemp);
	App->CL_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);

	App->CL_Render->Render_ViewToWorld(Current_View, App->CL_Render->Render_GetWidth(Current_View), App->CL_Render->Render_GetHeight(Current_View), &XTemp);
	App->CL_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);

	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	brushDrawData.pViewBox = &ViewBox;
	brushDrawData.pDC = MemoryhDC;
	brushDrawData.v = Current_View;
	brushDrawData.pDoc = App->CL_Doc;
	brushDrawData.GroupId = 0;
	brushDrawData.FlagTest = NULL;

	RealhDC = GetDC(hwnd);

	MemoryhDC = CreateCompatibleDC(RealhDC);

	GetClipBox(RealhDC, &Rect);

	HBITMAP OffScreenBitmap;

	OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);

	SelectObject(MemoryhDC, OffScreenBitmap);

	FillRect(MemoryhDC, &Rect, (HBRUSH)BackGround_Brush); // BackGround


	// ---------------------- Draw Grid Fine
	if (Current_View->ZoomFactor > 0.1)
	{
		SelectObject(MemoryhDC, Pen_Fine_Grid);
		App->CL_Render->Render_RenderOrthoGridFromSize(Current_View, int(GridSnapSize), MemoryhDC, Rect);
	}

	// ---------------------- Draw Grid
	if (Current_View->ZoomFactor < 0.1)
	{
		Current_View->ZoomFactor = 0.1;
	}

	SelectObject(MemoryhDC, Pen_Grid);
	App->CL_Render->Render_RenderOrthoGridFromSize(Current_View, int(GridSize), MemoryhDC, Rect);

	SelectObject(MemoryhDC, PenBrushes);
	int BrushCount = App->CL_Brush->Get_Brush_Count();
	int Count = 0;
	while (Count < BrushCount)
	{
		App->CL_Level->Level_EnumLeafBrushes(App->CL_Doc->pLevel, &brushDrawData, BrushDraw);
		Count++;
	}

	//if (Brush_TestBoundsIntersect(App->CLSB_Doc->CurBrush, &ViewBox))
	{
		if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
		{
			SelectObject(MemoryhDC, PenTemplate);

			if (App->CL_Brush->Brush_IsMulti(App->CL_Doc->CurBrush))
			{

				App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(App->CL_Doc->CurBrush), &brushDrawData, BrushDraw);
			}
			else
			{
				Render_RenderBrushFacesOrtho(Current_View, App->CL_Doc->CurBrush, MemoryhDC);

			}
		}
	}

	BitBlt(RealhDC, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, MemoryhDC, 0, 0, SRCCOPY);

	DeleteObject(OffScreenBitmap);
	DeleteDC(MemoryhDC);

	//flag_IsDrawing = 0;
}


static POINT plist[64];

// *************************************************************************
// *	  			Render_RenderBrushFacesOrtho		Genesis			   *
// *************************************************************************
void CL64_MapEditor::Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC)
{
	int	i, j;

	for (i = 0; i < App->CL_Brush->Brush_GetNumFaces(b); i++)
	{
		Face* f = App->CL_Brush->Brush_GetFace(b, i);
		const Ogre::Vector3* pnts = App->CL_Face->Face_GetPoints(f);

		for (j = 0; j < App->CL_Face->Face_GetNumPoints(f); j++)
		{
			plist[j] = App->CL_Render->Render_OrthoWorldToView(Cam, &pnts[j]);
		}

		plist[j] = plist[0];
		Polyline(ViewDC, plist, j + 1);
	}
}



