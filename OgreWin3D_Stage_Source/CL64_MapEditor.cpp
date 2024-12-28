/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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

CL64_MapEditor::CL64_MapEditor()
{
	Main_Dlg_Hwnd = NULL;
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
	flag_Map_Editor_Running = 0;

	flag_Show_Areas = 1;
	flag_Show_Camera = 1;
	flag_Show_Colectables = 1;
	flag_Show_Static_Objects = 1;

	flag_Right_Button_Down = 0;
	flag_Left_Button_Down = 0;

	BackGround_Brush = CreateSolidBrush(RGB(64, 64, 64));

	Pen_CutBrush = CreatePen(PS_SOLID, 0, RGB(255, 155, 0));
	Pen_Camera = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));
	Pen_White = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	Pen_Colectables = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	Pen_Static_Objects = CreatePen(PS_SOLID, 0, RGB(0, 0, 255));

	MemoryhDC = nullptr;

	int Count = 0;
	while (Count < 3)
	{
		VCam[Count] = nullptr;
		Count++;
	}
}

CL64_MapEditor::~CL64_MapEditor()
{
}

// *************************************************************************
// *	  	Start_Map_View_Dlg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Start_Map_View_Dlg()
{
	Map_View_Main_Dlg();
}

// *************************************************************************
// *	  			Init_Views:- Terry and Hazel Flanigan 2023			   *
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
// *	  	Map_View_Main_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MapEditor::Map_View_Main_Dlg()
{
	//if (flag_Map_Editor_Running == 0)
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_MAPEDITOR, App->MainHwnd, (DLGPROC)Proc_Main_Dlg);
		//flag_Map_Editor_Running = 1;
	}
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
		App->CL_MapEditor->mMenu = GetMenu(hDlg);

		App->CL_MapEditor->Init_Views();
		App->CL_MapEditor->Create_Top_Left_Window();
		App->CL_MapEditor->Create_Top_Right_Window();
		App->CL_MapEditor->Create_Bottom_Left_Window();
		App->CL_MapEditor->Create_Bottom_Right_Window();
		App->CL_MapEditor->Resize_Windows(App->CL_MapEditor->Main_Dlg_Hwnd, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);
		
		CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_AREAS, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_CAMERA, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_COLECTABLES, MF_BYCOMMAND | MF_CHECKED);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)App->AppBackground);
		EndPaint(hDlg, &ps);
	}

	case WM_NOTIFY:
	{
		return CDRF_DODEFAULT;
	}

	case WM_SIZE:
	{
		App->CL_MapEditor->Init_Views();
		App->CL_MapEditor->Resize_Windows(hDlg, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);

		GetClientRect(hDlg, &rect);
		RedrawWindow(App->CL_MapEditor->Main_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		if (App->CL_MapEditor->flag_Right_Button_Down == 0)
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
		if (App->CL_MapEditor->flag_Right_Button_Down == 0)
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
		if (App->CL_MapEditor->flag_Right_Button_Down == 0)
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
		if (LOWORD(wParam) == ID_SHOW_AREAS)
		{
			if (App->CL_MapEditor->flag_Show_Areas == 1)
			{
				App->CL_MapEditor->flag_Show_Areas = 0;
				CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_AREAS, MF_BYCOMMAND | MF_UNCHECKED);
			}
			else
			{
				App->CL_MapEditor->flag_Show_Areas = 1;
				CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_AREAS, MF_BYCOMMAND | MF_CHECKED);
			}

			App->CL_MapEditor->Resize_Windows(hDlg, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);
	
			return TRUE;
		}

		if (LOWORD(wParam) == ID_SHOW_CAMERA)
		{
			if (App->CL_MapEditor->flag_Show_Camera == 1)
			{
				App->CL_MapEditor->flag_Show_Camera = 0;
				CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_CAMERA, MF_BYCOMMAND | MF_UNCHECKED);
			}
			else
			{
				App->CL_MapEditor->flag_Show_Camera = 1;
				CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_CAMERA, MF_BYCOMMAND | MF_CHECKED);
			}

			App->CL_MapEditor->Resize_Windows(hDlg, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);

			return TRUE;
		}

		if (LOWORD(wParam) == ID_SHOW_COLECTABLES)
		{
			if (App->CL_MapEditor->flag_Show_Colectables == 1)
			{
				App->CL_MapEditor->flag_Show_Colectables = 0;
				CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_COLECTABLES, MF_BYCOMMAND | MF_UNCHECKED);
			}
			else
			{
				App->CL_MapEditor->flag_Show_Colectables = 1;
				CheckMenuItem(App->CL_MapEditor->mMenu, ID_SHOW_COLECTABLES, MF_BYCOMMAND | MF_CHECKED);
			}

			App->CL_MapEditor->Resize_Windows(hDlg, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_MapEditor->flag_Map_Editor_Running = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			int Count = 0;

			while (Count < 3)
			{
				delete App->CL_MapEditor->VCam[Count];
				Count++;
			}
			
			App->CL_MapEditor->flag_Map_Editor_Running = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *			Resize_Windowns:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
bool CL64_MapEditor::Resize_Windows(HWND hDlg, int NewWidth, int NewDepth)
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

	return 1;
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
		GetClientRect(hDlg,&r);

		App->CL_MapEditor->VCam[V_TL] = new ViewVars;
		strcpy(App->CL_MapEditor->VCam[V_TL]->Name, "TLV");
		App->CL_MapEditor->VCam[V_TL]->ViewType = 8;
		App->CL_MapEditor->VCam[V_TL]->ZoomFactor = 0.3;

		App->CL_MapEditor->VCam[V_TL]->XCenter = 310;
		App->CL_MapEditor->VCam[V_TL]->YCenter = 174;

		App->CL_MapEditor->VCam[V_TL]->Width = 310;
		App->CL_MapEditor->VCam[V_TL]->Height = 174;

		App->CL_MapEditor->VCam[V_TL]->CamPos = App->CL_Ogre->camNode->getPosition();
		
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

	case WM_SETCURSOR:
	{
		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	case WM_MOUSEMOVE:
	{
		

		int			dx, dy;
		POINT		RealCursorPosition;

		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		dx = (RealCursorPosition.x);
		dy = (RealCursorPosition.y);


		if (App->CL_MapEditor->flag_Right_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Zoom_View(hDlg, dx, dy);
		}

		if (App->CL_MapEditor->flag_Left_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Pan_View(hDlg, dx, dy);
		}

		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
		App->CUR = SetCursor(NULL);

		return 1;
	}

	case WM_LBUTTONUP:
	{
		App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 1;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
		App->CUR = SetCursor(NULL);

		return 1;
	}

	case WM_RBUTTONUP:
	{
		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);

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
		App->CL_MapEditor->VCam[V_TR]->ZoomFactor = 0.3;

		App->CL_MapEditor->VCam[V_TR]->CamPos = App->CL_Ogre->camNode->getPosition();
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

	case WM_SETCURSOR:
	{
		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	case WM_MOUSEMOVE:
	{
		int			dx, dy;
		POINT		RealCursorPosition;

		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		dx = (RealCursorPosition.x);
		dy = (RealCursorPosition.y);


		if (App->CL_MapEditor->flag_Right_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Zoom_View(hDlg, dx, dy);
		}

		if (App->CL_MapEditor->flag_Left_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Pan_View(hDlg, dx, dy);
		}

		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
		App->CUR = SetCursor(NULL);

		return 1;
	}

	case WM_LBUTTONUP:
	{
		App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 1;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
		App->CUR = SetCursor(NULL);

		return 1;
	}

	case WM_RBUTTONUP:
	{
		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);

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
		App->CL_MapEditor->VCam[V_BL]->ZoomFactor = 0.3;

		App->CL_MapEditor->VCam[V_BL]->CamPos = App->CL_Ogre->camNode->getPosition();
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

	case WM_SETCURSOR:
	{
		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	case WM_MOUSEMOVE:
	{
		int			dx, dy;
		POINT		RealCursorPosition;

		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		dx = (RealCursorPosition.x);
		dy = (RealCursorPosition.y);


		if (App->CL_MapEditor->flag_Right_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Zoom_View(hDlg, dx, dy);
		}

		if (App->CL_MapEditor->flag_Left_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_MapEditor->Pan_View(hDlg, dx, dy);
		}

		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];
		App->CUR = SetCursor(NULL);

		return 1;
	}

	case WM_LBUTTONUP:
	{
		App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		GetCursorPos(&App->CL_MapEditor->mStartPoint);
		ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

		App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];
		
		App->CUR = SetCursor(NULL);
		return 1;
	}

	case WM_RBUTTONUP:
	{
		App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);

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
// *	  Create_Bottom_Right_Window:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void CL64_MapEditor::Create_Bottom_Right_Window()
{
	Bottom_Right_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_RIGHT, Main_Dlg_Hwnd, (DLGPROC)Bottom_Right_Proc);
}

// *************************************************************************
// *			Bottom_Right_Proc:- Terry and Hazel Flanigan 2023 		   *
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
// *			Zoom_View:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_MapEditor::Zoom_View(HWND hDlg, int Dx, int Dy)
{
	
	if (flag_Right_Button_Down == 1)
	{
		if (Dy < App->CL_MapEditor->mStartPoint.y)
		{
			long test = App->CL_MapEditor->mStartPoint.y - Dy;

			if (test > 0)
			{
				App->CL_MapEditor->Current_View->ZoomFactor = App->CL_MapEditor->Current_View->ZoomFactor + 0.01;
				App->CL_MapEditor->Draw_Screen(hDlg);
			}

			POINT pt = mStartPoint;
			ClientToScreen(hDlg, &pt);
			SetCursorPos(pt.x, pt.y);

		}
		else if (Dy > App->CL_MapEditor->mStartPoint.y)
		{
			long test = Dy - App->CL_MapEditor->mStartPoint.y;
			if (test > 0)
			{
				App->CL_MapEditor->Current_View->ZoomFactor = App->CL_MapEditor->Current_View->ZoomFactor - 0.01;
				App->CL_MapEditor->Draw_Screen(hDlg);

			}

			POINT pt = mStartPoint;
			ClientToScreen(hDlg ,&pt);
			SetCursorPos(pt.x, pt.y);

		}
	}
}

// *************************************************************************
// *			Pan_View:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_MapEditor::Pan_View(HWND hDlg, int Dx, int Dy)
{
	Ogre::Vector3 dv;
	Ogre::Vector3 dcamv;

	switch (Current_View->ViewType)
	{
	case VIEWTOP:  // Top Left
	{
		if (Dx < App->CL_MapEditor->mStartPoint.x)
		{
			long test = App->CL_MapEditor->mStartPoint.x - Dx;
			if (test > 2)
			{
				dv = Ogre::Vector3(-15, 0, 0);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}

		}
		else if (Dx > App->CL_MapEditor->mStartPoint.x)
		{
			long test = Dx - App->CL_MapEditor->mStartPoint.x;
			if (test > 2)
			{
				dv = Ogre::Vector3(15, 0, 0);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}

		if (Dy < App->CL_MapEditor->mStartPoint.y)
		{
			long test = App->CL_MapEditor->mStartPoint.y - Dy;
			if (test > 2)
			{
				dv = Ogre::Vector3(0, 0, -15);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}
		else if (Dy > App->CL_MapEditor->mStartPoint.y)
		{
			long test = Dy - App->CL_MapEditor->mStartPoint.y;
			if (test > 2)
			{
				dv = Ogre::Vector3(0, 0, 15);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}

		break;
	}
	case VIEWFRONT:  // Bottom Left
	{
		if (Dx < App->CL_MapEditor->mStartPoint.x)
		{
			long test = App->CL_MapEditor->mStartPoint.x - Dx;
			if (test > 2)
			{
				dv = Ogre::Vector3(-15, 0, 0);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}

		}
		else if (Dx > App->CL_MapEditor->mStartPoint.x)
		{
			long test = Dx - App->CL_MapEditor->mStartPoint.x;
			if (test > 2)
			{
				dv = Ogre::Vector3(15, 0, 0);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}

		if (Dy < App->CL_MapEditor->mStartPoint.y)
		{
			long test = App->CL_MapEditor->mStartPoint.y - Dy;
			if (test > 2)
			{
				dv = Ogre::Vector3(0, 15, 0);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}
		else if (Dy > App->CL_MapEditor->mStartPoint.y)
		{
			long test = Dy - App->CL_MapEditor->mStartPoint.y;
			if (test > 2)
			{
				dv = Ogre::Vector3(0, -15, 0);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}

		break;
	}
	case VIEWSIDE: // Top Right
	{
		if (Dx < App->CL_MapEditor->mStartPoint.x)
		{
			long test = App->CL_MapEditor->mStartPoint.x - Dx;
			if (test > 2)
			{
				dv = Ogre::Vector3(0, 0, -15);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}

		}
		else if (Dx > App->CL_MapEditor->mStartPoint.x)
		{
			long test = Dx - App->CL_MapEditor->mStartPoint.x;
			if (test > 2)
			{
				dv = Ogre::Vector3(0, 0, 15);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}

		if (Dy < App->CL_MapEditor->mStartPoint.y)
		{
			long test = App->CL_MapEditor->mStartPoint.y - Dy;
			if (test > 2)
			{
				dv = Ogre::Vector3(0, 15, 0);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}
		else if (Dy > App->CL_MapEditor->mStartPoint.y)
		{
			long test = Dy - App->CL_MapEditor->mStartPoint.y;
			if (test > 2)
			{
				dv = Ogre::Vector3(0, -15, 0);

				POINT pt = mStartPoint;
				ClientToScreen(hDlg, &pt);
				SetCursorPos(pt.x, pt.y);
			}
		}

		break;
	}
	default:

		break;
	}

	App->CL_Maths->Vector3_Scale(&dv, -1.0f, &dcamv);
	App->CL_Maths->Vector3_Add(&App->CL_MapEditor->Current_View->CamPos, &dcamv, &App->CL_MapEditor->Current_View->CamPos);
	App->CL_MapEditor->Draw_Screen(hDlg);

}

// *************************************************************************
// *						Draw_Screen Terry Flanigan		  			   *
// *************************************************************************
void CL64_MapEditor::Draw_Screen(HWND hwnd)
{
	
	HDC			RealhDC;
	RECT		Rect;

	float GridSize = 2;
	float GridSnapSize = 2;

	GetClientRect(hwnd, &Rect);
	Rect.left--;
	Rect.bottom--;

	RealhDC = GetDC(hwnd);

	MemoryhDC = CreateCompatibleDC(RealhDC);

	GetClipBox(RealhDC, &Rect);

	HBITMAP OffScreenBitmap;

	OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);

	SelectObject(MemoryhDC, OffScreenBitmap);

	HBRUSH hBrush = CreateSolidBrush(RGB(64, 64, 64));
	FillRect(MemoryhDC, &Rect, (HBRUSH)hBrush); // BackGround
	DeleteObject(hBrush);

	int Center_X, Center_Y;
	int Width, Depth;

	Width = Rect.right;
	Depth = Rect.bottom;

	Center_Y = (Depth / 2);
	Center_X = (Width / 2);

	// ---------------------- Draw Grid Fine

	HPEN pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	if (Current_View->ZoomFactor > 0.1)
	{
		SelectObject(MemoryhDC, pen);
		Draw_Grid(MemoryhDC, 8, Rect); // Snap grid
	}

	// ---------------------- Draw Grid
	HPEN pen2 = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
	if (Current_View->ZoomFactor < 0.1)
	{
		Current_View->ZoomFactor = 0.1;
	}

	SelectObject(MemoryhDC, pen2);
	Draw_Grid(MemoryhDC, 128, Rect); // Big grid
	

	// ---------------------- Draw Areas
	MeshData_Render_Faces(MemoryhDC);
	

	// ---------------------- Draw Camera
	if (flag_Show_Camera == 1)
	{
		SelectObject(MemoryhDC, Pen_Camera);
		Draw_Camera(MemoryhDC);
	}

	/*RECT rect = { 0,0, 400, 20 };
	FillRect(MemoryhDC, &rect, App->AppBackground);
	TextOut(MemoryhDC, 2, 2, TEXT("Test"), 5);*/

	int TopLeft, BottomRight;
	int CrossSize = 16;

	int CamX = 0;
	int CamBottomRight = 0;

	Width = Rect.right;
	Depth = Rect.bottom;

	Center_Y = (Depth / 2);
	Center_X = (Width / 2);

	TopLeft = Center_Y - 4;
	BottomRight = Center_X - 4;

	BitBlt(RealhDC, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, MemoryhDC, 0, 0, SRCCOPY);

	SelectObject(MemoryhDC, &OffScreenBitmap);

	DeleteObject(pen);
	DeleteObject(pen2);
	DeleteObject(hBrush);

	DeleteObject(OffScreenBitmap);
	DeleteDC(MemoryhDC);
}

// *************************************************************************
// *			Draw_Camera:- Terry and Hazel Flanigan 2024	 		   *
// *************************************************************************
void CL64_MapEditor::Draw_Camera(HDC ViewDC)
{
#define ENTITY_SIZE (32.0f)  // 16" across

	Ogre::Vector3 VecOrigin;
	Ogre::Vector3 EntSizeWorld;	// entity size in world space

	POINT EntPosView;
	POINT EntSizeView;
	POINT EntWidthHeight;
	POINT OriginView;

	POINT TopLeft, BottomRight;
	POINT TopRight, BottomLeft;

	static const float COS45 = (float)cos(M_PI / 4.0f);
	static const float SIN45 = (float)sin(M_PI / 4.0f);
	static const float MCOS45 = (float)cos(-(M_PI / 4.0f));
	static const float MSIN45 = (float)sin(-(M_PI / 4.0f));

	// compute entity size in view coordinates
	App->CL_Maths->Vector3_Set(&EntSizeWorld, ENTITY_SIZE, ENTITY_SIZE, ENTITY_SIZE);
	EntSizeView = m_Render_OrthoWorldToView(&EntSizeWorld);
	App->CL_Maths->Vector3_Clear(&VecOrigin);
	OriginView = m_Render_OrthoWorldToView(&VecOrigin);
	// This one is the width and height of the Entity
	EntWidthHeight.x = std::max(OriginView.x, EntSizeView.x) - std::min(OriginView.x, EntSizeView.x);
	EntWidthHeight.y = std::max(OriginView.y, EntSizeView.y) - std::min(OriginView.y, EntSizeView.y);

	// This can have negative numbers
	EntSizeView.x -= OriginView.x;
	EntSizeView.y -= OriginView.y;

	// entity's position in the view
	EntPosView = m_Render_OrthoWorldToView(&(App->CL_Ogre->camNode->getPosition()));

	{
		// Draw an X at the entity's position...
		TopLeft.x = EntPosView.x - EntSizeView.x;
		TopLeft.y = EntPosView.y - EntSizeView.y;
		BottomRight.x = EntPosView.x + EntSizeView.x;
		BottomRight.y = EntPosView.y + EntSizeView.y;
		TopRight.x = BottomRight.x;
		TopRight.y = TopLeft.y;
		BottomLeft.x = TopLeft.x;
		BottomLeft.y = BottomRight.y;

		MoveToEx(ViewDC, TopLeft.x, TopLeft.y, NULL);
		LineTo(ViewDC, BottomRight.x, BottomRight.y);

		MoveToEx(ViewDC, TopRight.x, TopRight.y, NULL);
		LineTo(ViewDC, BottomLeft.x, BottomLeft.y);

	}


}

static POINT plist[64];

// *************************************************************************
// *		MeshData_Render_Faces:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_MapEditor::MeshData_Render_Faces(HDC ViewDC)
{
	int Count = 0;
	int Dont_Draw = 0;
	int Map_Count = App->CL_Scene->Map_Group_Count;

	while (Count < Map_Count)
	{
		if (App->CL_Scene->Map_Group[Count]->Obj_Usage == Enums::Obj_Usage_Room)
		{
			if (flag_Show_Areas == 0)
			{
				Dont_Draw = 1;
			}

			SelectObject(MemoryhDC, Pen_White);
		}

		if (App->CL_Scene->Map_Group[Count]->Obj_Usage == Enums::Obj_Usage_Collectable)
		{
			if (flag_Show_Colectables == 0)
			{
				Dont_Draw = 1;
			}

			SelectObject(MemoryhDC, Pen_Colectables);
		}

		if (App->CL_Scene->Map_Group[Count]->Obj_Usage == Enums::Obj_Usage_Static)
		{
			if (flag_Show_Static_Objects == 0)
			{
				Dont_Draw = 1;
			}

			SelectObject(MemoryhDC, Pen_Static_Objects);
		}

		
		if (Dont_Draw == 0)
		{
			MeshData_Face_Groups(Count, ViewDC);
		}

		Dont_Draw = 0;

		Count++;
	}
}

// *************************************************************************
// *		MeshData_Face_Groups:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MapEditor::MeshData_Face_Groups(int Count, HDC ViewDC)
{

	int Sub_Group_Count = App->CL_Scene->Map_Group[Count]->Sub_Mesh_Count;

	int Index = 0;
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	Ogre::Vector3 Points;

	while (Index < Sub_Group_Count)
	{
		int FaceCount = 0;
		while (FaceCount < App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->Face_Count)
		{
			A = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->Face_Data[FaceCount].a;
			B = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->Face_Data[FaceCount].b;
			C = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->Face_Data[FaceCount].c;

			Points.x = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[A].x;
			Points.y = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[A].y;
			Points.z = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[A].z;

			plist[0] = m_Render_OrthoWorldToView(&Points);
			
			Points.x = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[B].x;
			Points.y = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[B].y;
			Points.z = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[B].z;

			plist[1] = m_Render_OrthoWorldToView(&Points);

			Points.x = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[C].x;
			Points.y = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[C].y;
			Points.z = App->CL_Scene->Map_Group[Count]->B_Sub_Mesh[Index]->vertex_Data[C].z;

			plist[2] = m_Render_OrthoWorldToView(&Points);

			plist[3] = plist[0];
			Polyline(ViewDC, plist, 3 + 1);

			FaceCount++;
	
		}

		Index++;
	}

}

// *************************************************************************
// *	  			m_Render_OrthoWorldToView							   *
// *************************************************************************
POINT CL64_MapEditor::m_Render_OrthoWorldToView(Ogre::Vector3 const* wp)
{

	POINT	sc = { 0, 0 };
	Ogre::Vector3 ptView;
	Ogre::Vector3 Campos;

	switch (Current_View->ViewType)
	{
	case VIEWTOP:
	{
		App->CL_Maths->Vector3_Subtract(wp, &Current_View->CamPos, &ptView);
		App->CL_Maths->Vector3_Scale(&ptView, Current_View->ZoomFactor, &ptView);

		sc.x = (int)(Current_View->XCenter + ptView.x);
		sc.y = (int)(Current_View->YCenter + ptView.z);
		break;
	}
	case VIEWFRONT:
	{
		App->CL_Maths->Vector3_Subtract(wp, &Current_View->CamPos, &ptView);
		App->CL_Maths->Vector3_Scale(&ptView, Current_View->ZoomFactor, &ptView);

		sc.x = (int)(Current_View->XCenter + ptView.x);
		sc.y = (int)(Current_View->YCenter - ptView.y);
		break;
	}
	case VIEWSIDE:
	{
		App->CL_Maths->Vector3_Subtract(wp, &Current_View->CamPos, &ptView);
		App->CL_Maths->Vector3_Scale(&ptView, Current_View->ZoomFactor, &ptView);

		sc.x = (int)(Current_View->XCenter + ptView.z);
		sc.y = (int)(Current_View->YCenter - ptView.y);
		break;
	}
	default:

		break;
	}

	return sc;
}

static const Ogre::Vector3	VecOrigin = { 0.0f, 0.0f, 0.0f };

#define Units_Round(n) ((int)Units_FRound((n)))
#define Units_Trunc(n) ((int)(n))
#define Units_FRound(n)	((float)floor((n)+0.5f))
#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

// *************************************************************************
// *	  			Draw_Grid:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_MapEditor::Draw_Grid(HDC hDC, int Interval, RECT Rect)
{
	Current_View->Width = Rect.right;
	Current_View->Height = Rect.bottom;

	Ogre::Vector3 ystep, xstep, Delt, Delt2;
	int			i, cnt, xaxis, yaxis, inidx;
	static int axidx[3][2] = { 2, 1, 0, 2, 0, 1 };
	float	gsinv;
	Box3d ViewBox;
	POINT		sp;

	inidx = (Current_View->ViewType >> 3) & 0x3;

	xaxis = axidx[inidx][0];
	yaxis = axidx[inidx][1];

	Render_ViewToWorld(Current_View, Units_Round(-Interval), Units_Round(-Interval), &Delt);
	Render_ViewToWorld(Current_View, Units_Round(Current_View->Width + Interval), Units_Round(Current_View->Height + Interval), &Delt2);

	App->CL_Box->Box3d_Set(&ViewBox, Delt.x, Delt.y, Delt.z, Delt2.x, Delt2.y, Delt2.z);

	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	gsinv = 1.0f / (float)Interval;
	for (i = 0; i < 3; i++)
	{
		VectorToSUB(ViewBox.Min, i) = (float)((int)(VectorToSUB(ViewBox.Min, i) * gsinv)) * Interval;
		VectorToSUB(ViewBox.Max, i) = (float)((int)(VectorToSUB(ViewBox.Max, i) * gsinv)) * Interval;
	}

	App->CL_Maths->Vector3_Copy(&VecOrigin, &xstep);
	App->CL_Maths->Vector3_Copy(&VecOrigin, &ystep);
	VectorToSUB(ystep, yaxis) = (float)Interval;
	VectorToSUB(xstep, xaxis) = (float)Interval;

	cnt = Rect.bottom / Interval; // hgtterry Debug Odd

	// horizontal lines
	int Count = 0;
	App->CL_Maths->Vector3_Copy(&ViewBox.Min, &Delt);
	App->CL_Maths->Vector3_Copy(&ViewBox.Min, &Delt2);
	VectorToSUB(Delt2, xaxis) = VectorToSUB(ViewBox.Max, xaxis);
	cnt = Units_Round((VectorToSUB(ViewBox.Max, yaxis) - VectorToSUB(ViewBox.Min, yaxis)) * gsinv);

	while (Count < cnt)
	{
		sp = m_Render_OrthoWorldToView(&Delt);
		MoveToEx(hDC, 0, sp.y, NULL);
		sp = m_Render_OrthoWorldToView(&Delt2);
		LineTo(hDC, Current_View->Width, sp.y);
		App->CL_Maths->Vector3_Add(&Delt, &ystep, &Delt);
		App->CL_Maths->Vector3_Add(&Delt2, &ystep, &Delt2);
		Count++;
	}

	// vertical lines
	Count = 0;
	App->CL_Maths->Vector3_Copy(&ViewBox.Min, &Delt);
	App->CL_Maths->Vector3_Copy(&ViewBox.Min, &Delt2);
	VectorToSUB(Delt2, yaxis) = VectorToSUB(ViewBox.Max, yaxis);
	cnt = Units_Round((VectorToSUB(ViewBox.Max, xaxis) - VectorToSUB(ViewBox.Min, xaxis)) * gsinv);

	while (Count < cnt)
	{
		sp = m_Render_OrthoWorldToView(&Delt);
		MoveToEx(hDC, sp.x, 0, NULL);
		sp = m_Render_OrthoWorldToView(&Delt2);
		LineTo(hDC, sp.x, Current_View->Height);
		App->CL_Maths->Vector3_Add(&Delt, &xstep, &Delt);
		App->CL_Maths->Vector3_Add(&Delt2, &xstep, &Delt2);
		Count++;
	}

	return 1;
}

// *************************************************************************
// *	  	Render_ViewToWorld:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Render_ViewToWorld(const ViewVars* v, const int x, const int y, Ogre::Vector3* wp)
{
	float	ZoomInv = 1.0f / Current_View->ZoomFactor;

	switch (Current_View->ViewType)
	{
	case VIEWTOP:
	{
		App->CL_Maths->Vector3_Set(wp, (x - Current_View->XCenter), 0.0f, (y - Current_View->YCenter));
		App->CL_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_Maths->Vector3_Add(wp, &Current_View->CamPos, wp);
		break;
	}
	case VIEWFRONT:
	{

		App->CL_Maths->Vector3_Set(wp, (x - Current_View->XCenter), -(y - Current_View->YCenter), 0.0f);
		App->CL_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_Maths->Vector3_Add(wp, &Current_View->CamPos, wp);
		break;
	}
	case VIEWSIDE:
	{
		App->CL_Maths->Vector3_Set(wp, 0.0f, -(y - Current_View->YCenter), (x - Current_View->XCenter));
		App->CL_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_Maths->Vector3_Add(wp, &Current_View->CamPos, wp);
		break;
	}
	default:
	{
		App->CL_Maths->Vector3_Set
		(
			wp,
			-(x - Current_View->XCenter) * (Current_View->MaxScreenScaleInv),
			-(y - Current_View->YCenter) * (Current_View->MaxScreenScaleInv),
			1.0f
		);

		App->CL_Maths->Vector3_Normalize(wp);

		break;
	}
	}
}
