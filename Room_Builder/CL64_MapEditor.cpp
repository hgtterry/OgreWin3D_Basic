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

#define IDM_GRID_SNAP 1
#define IDM_RESET_VIEW 2
#define IDM_FILE_RENAME 3
#define IDM_COPY 4
#define IDM_PASTE 5
#define IDM_GOTO 6

#define	M_PI		((float)3.14159265358979323846f)
#define	TOP_POS					8
#define	BOTTOM_POS				400
#define	SPLITTER_BAR_WIDTH		5
#define WIDTH_ADJUST			2

#define BOTTOM_POS_BOTLEFT		5

enum LastBrushAction
{
	BRUSH_MOVE,
	BRUSH_ROTATE,
	BRUSH_SCALE,
	BRUSH_SHEAR,
	BRUSH_RESET,
	BRUSH_DIALOG
};

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
	nleftWnd_width = 500;

	LEFT_WINDOW_DEPTH = 215;
	nleftWnd_Depth = 215;
	TOP_POS_BOTLEFT = 215;

	RIGHT_MINIMUM_SPACE = 1000;
	LEFT_MINIMUM_SPACE = 15;

	Do_Width = 0;
	Do_Depth = 0;
	Do_All = 0;

	flag_Left_Button_Down = 0;
	flag_Right_Button_Down = 0;
	flag_Context_Menu_Active = 0;

	BackGround_Brush = CreateSolidBrush(RGB(64, 64, 64));

	Pen_Fine_Grid = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	Pen_Grid = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
	PenTemplate = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	PenBrushes = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	PenSelected = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	Pen_Camera = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));
	PenSelectedFaces = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	PenCutBrush = CreatePen(PS_SOLID, 1, RGB(255, 155, 0));

	hcSizeEW = LoadCursor(NULL, IDC_SIZEWE);
	hcSizeNS = LoadCursor(NULL, IDC_SIZENS);
	hcBoth = LoadCursor(NULL, IDC_SIZEALL);

	mStartPoint.x = 0;
	mStartPoint.y = 0;

	Current_View = NULL;

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
// *	  		Reset_Views_All:- Terry and Hazel Flanigan 2024	     *
// *************************************************************************
void CL64_MapEditor::Reset_Views_All()
{
	App->CL_MapEditor->Init_Views(Enums::Selected_View_None);
	App->CL_MapEditor->Resize_Windows(Main_Dlg_Hwnd, nleftWnd_width, nleftWnd_Depth);

	int Count = 0;

	while (Count < 3)
	{
		RECT		Rect;
		GetClientRect(App->CL_MapEditor->VCam[Count]->hDlg, &Rect);

		App->CL_MapEditor->VCam[Count]->XCenter = (float)Rect.right / 2;
		App->CL_MapEditor->VCam[Count]->YCenter = (float)Rect.bottom / 2;

		App->CL_MapEditor->VCam[Count]->CamPos.x = 0;
		App->CL_MapEditor->VCam[Count]->CamPos.y = 0;
		App->CL_MapEditor->VCam[Count]->CamPos.z = 0;

		App->CL_MapEditor->VCam[Count]->ZoomFactor = 0.3;

		Count++;
	}

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

}

// *************************************************************************
// *	  			Init_Views:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MapEditor::Init_Views(int View)
{
	RECT rect;
	GetClientRect(Main_Dlg_Hwnd, &rect);

	if (View == Enums::Selected_View_None)
	{
		LEFT_WINDOW_WIDTH = rect.right / 2;
		nleftWnd_width = rect.right / 2;

		LEFT_WINDOW_DEPTH = rect.bottom / 2;
		TOP_POS_BOTLEFT = rect.bottom / 2;
	}

	if (View == Enums::Selected_View_3D)
	{
		LEFT_WINDOW_WIDTH = 0;
		nleftWnd_width = 0;

		LEFT_WINDOW_DEPTH = 0;
		TOP_POS_BOTLEFT = 0;
	}

	if (View == Enums::Selected_View_TL)
	{
		LEFT_WINDOW_WIDTH = rect.right;
		nleftWnd_width = rect.right;

		LEFT_WINDOW_DEPTH = rect.bottom;
		TOP_POS_BOTLEFT = rect.bottom;
	}

	if (View == Enums::Selected_View_TR)
	{
		LEFT_WINDOW_WIDTH = 0;
		nleftWnd_width = 0;

		LEFT_WINDOW_DEPTH = rect.bottom;
		TOP_POS_BOTLEFT = rect.bottom;
	}

	if (View == Enums::Selected_View_BL)
	{
		LEFT_WINDOW_WIDTH = rect.right;
		nleftWnd_width = rect.right;

		LEFT_WINDOW_DEPTH = 0;
		TOP_POS_BOTLEFT = 0;
	}

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
	
	App->CL_Panels->Resize_OgreWin();
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
	App->CL_MapEditor->Create_Bottom_Right_Ogre();

	RECT rcl;
	GetClientRect(App->MainHwnd, &rcl);
	MoveWindow(App->CL_MapEditor->Main_Dlg_Hwnd, 0, 50, rcl.right, rcl.bottom - 50, TRUE);
	
	App->CL_MapEditor->Init_Views(Enums::Selected_View_None);
	RedrawWindow(App->CL_MapEditor->Main_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *			Proc_Main_Dlg:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static  BOOL        xSizing;
	static  BOOL        ySizing;

	static UINT			nwnd_resizing = 0;

	RECT rect;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_MapEditor->Main_Dlg_Hwnd = hDlg;
		
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

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
		App->CL_MapEditor->Init_Views(Enums::Selected_View_None);
		App->CL_MapEditor->Resize_Windows(hDlg, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);

		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		if (App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown == 0)
		{
			int zDelta = (short)HIWORD(wParam);    // wheel rotation

			if (zDelta > 0)
			{
				App->CL_Ogre->Ogre3D_Listener->Wheel = -1;
			}
			else if (zDelta < 0)
			{
				App->CL_Ogre->Ogre3D_Listener->Wheel = 1;
			}

			return 1;
		}
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
					SetCursor(App->CL_MapEditor->hcSizeEW);
				}

			}

			if (ySizing && xSizing == 0)
			{
				// Api to capture mouse input
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(App->CL_MapEditor->hcSizeNS);
				}

			}

			if (xSizing && ySizing)
			{
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(App->CL_MapEditor->hcBoth);
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
					SetCursor(App->CL_MapEditor->hcSizeEW);
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
					SetCursor(App->CL_MapEditor->hcSizeNS);
				}

				App->CL_MapEditor->Do_Depth = 1;
			}
			else
			{
				App->CL_MapEditor->Do_Depth = 0;
			}

			if (App->CL_MapEditor->Do_Width == 1 && App->CL_MapEditor->Do_Depth == 1)
			{
				SetCursor(App->CL_MapEditor->hcBoth);
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
// *	  		Set_Views_Defaults:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MapEditor::Set_Views_Defaults(int Index, Ogre::int32 View, const char* Name)
{
	strcpy(App->CL_MapEditor->VCam[Index]->Name, Name);
	App->CL_MapEditor->VCam[Index]->ViewType = View;
	App->CL_MapEditor->VCam[Index]->ZoomFactor = 0.4;

	App->CL_MapEditor->VCam[Index]->XCenter = 310;
	App->CL_MapEditor->VCam[Index]->YCenter = 174;

	App->CL_MapEditor->VCam[Index]->XScreenScale = 0;
	App->CL_MapEditor->VCam[Index]->YScreenScale = 0;

	App->CL_MapEditor->VCam[Index]->Width = 310;
	App->CL_MapEditor->VCam[Index]->Height = 174;

	App->CL_Maths->Vector3_Set(&App->CL_MapEditor->VCam[Index]->CamPos,0,0,0);

	App->CL_MapEditor->VCam[Index]->MaxScreenScaleInv = 100;
}

// *************************************************************************
// *	  	Create_Top_Left_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MapEditor::Create_Top_Left_Window()
{
	Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, Main_Dlg_Hwnd, (DLGPROC)Proc_Top_Left_Window);
	App->CL_MapEditor->VCam[V_TL]->hDlg = Left_Window_Hwnd;
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
		App->CL_MapEditor->Set_Views_Defaults(V_TL,VIEWTOP,"TLV");

		return TRUE;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDM_GRID_SNAP)
		{
			if (App->CL_Level->flag_UseGrid == 1)
			{
				App->CL_Level->flag_UseGrid = 0;
				CheckMenuItem(App->mMenu, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_UNCHECKED);
			}
			else
			{
				App->CL_Level->flag_UseGrid = 1;
				CheckMenuItem(App->mMenu, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_CHECKED);
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDM_RESET_VIEW)
		{
			RECT		Rect;
			GetClientRect(App->CL_MapEditor->Current_View->hDlg, &Rect);
			
			App->CL_MapEditor->Current_View->XCenter = (float)Rect.right / 2;
			App->CL_MapEditor->Current_View->YCenter = (float)Rect.bottom / 2;

			App->CL_MapEditor->Current_View->CamPos.x = 0;
			App->CL_MapEditor->Current_View->CamPos.y = 0;
			App->CL_MapEditor->Current_View->CamPos.z = 0;

			App->CL_MapEditor->Current_View->ZoomFactor = 0.3;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

			return TRUE;
		}

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
		if (App->CL_MapEditor->flag_Context_Menu_Active == 1)
		{
			return false;
		}

		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			SetCursor(App->CL_MapEditor->hcBoth);
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
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

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
		App->CL_MapEditor->On_Mouse_Move(RealCursorPosition,hDlg);
		
		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
		
		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->On_Left_Button_Down(RealCursorPosition,hDlg);

		return 1;
	}

	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];

		App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->On_Left_Button_Up(RealCursorPosition);

		/*App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];

		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			GetCursorPos(&App->CL_MapEditor->mStartPoint);
			ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

			App->CL_MapEditor->flag_Right_Button_Down = 1;
			App->CL_MapEditor->flag_Left_Button_Down = 0;

			App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
			App->CUR = SetCursor(NULL);
		}
		else
		{
			App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TL];
			App->CL_MapEditor->Context_Menu(hDlg);
		}

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
// *			Context_Menu:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_MapEditor::Context_Menu(HWND hDlg)
{
	RECT rcTree;
	TVHITTESTINFO htInfo = { 0 };
	POINT pt;
	GetCursorPos(&pt);

	long xPos = pt.x;   // x position from message, in screen coordinates
	long yPos = pt.y;   // y position from message, in screen coordinates 

	GetWindowRect(hDlg, &rcTree);        // get its window coordinates
	htInfo.pt.x = xPos - rcTree.left;      // convert to client coordinates
	htInfo.pt.y = yPos - rcTree.top;

	hMenu = CreatePopupMenu();

	if (App->CL_Level->flag_UseGrid == 1)
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_GRID_SNAP, L"&Grid Snap");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_GRID_SNAP, L"&Grid Snap");
	}
	
	AppendMenuW(hMenu, MF_STRING , IDM_RESET_VIEW, L"&Reset View");
	//AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
	//AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	//AppendMenuW(hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");

	flag_Context_Menu_Active = 1;
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
	flag_Context_Menu_Active = 0;
	
	DestroyMenu(hMenu);

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

		App->CL_MapEditor->VCam[V_TR]->CamPos.x = 0;// App->CL_Ogre->camNode->getPosition();
		App->CL_MapEditor->VCam[V_TR]->CamPos.y;
		App->CL_MapEditor->VCam[V_TR]->CamPos.z;
		
		App->CL_MapEditor->VCam[V_TR]->hDlg = hDlg;

		return TRUE;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDM_GRID_SNAP)
		{
			if (App->CL_Level->flag_UseGrid == 1)
			{
				App->CL_Level->flag_UseGrid = 0;
				CheckMenuItem(App->mMenu, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_UNCHECKED);
			}
			else
			{
				App->CL_Level->flag_UseGrid = 1;
				CheckMenuItem(App->mMenu, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_CHECKED);
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDM_RESET_VIEW)
		{
			RECT		Rect;
			GetClientRect(App->CL_MapEditor->Current_View->hDlg, &Rect);

			App->CL_MapEditor->Current_View->XCenter = (float)Rect.right / 2;
			App->CL_MapEditor->Current_View->YCenter = (float)Rect.bottom / 2;

			App->CL_MapEditor->Current_View->CamPos.x = 0;
			App->CL_MapEditor->Current_View->CamPos.y = 0;
			App->CL_MapEditor->Current_View->CamPos.z = 0;

			App->CL_MapEditor->Current_View->ZoomFactor = 0.3;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

			return TRUE;
		}

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
		if (App->CL_MapEditor->flag_Context_Menu_Active == 1)
		{
			return false;
		}

		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			SetCursor(App->CL_MapEditor->hcBoth);
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
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

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
		App->CL_MapEditor->On_Mouse_Move(RealCursorPosition, hDlg);

		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
		
		App->CL_MapEditor->On_Left_Button_Down(RealCursorPosition, hDlg);

		return 1;
	}

	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 0;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];

		App->CL_MapEditor->On_Left_Button_Up(RealCursorPosition);

		/*App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			GetCursorPos(&App->CL_MapEditor->mStartPoint);
			ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

			App->CL_MapEditor->flag_Right_Button_Down = 1;
			App->CL_MapEditor->flag_Left_Button_Down = 0;

			App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
			App->CUR = SetCursor(NULL);
		}
		else
		{
			App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
			App->CL_MapEditor->Context_Menu(hDlg);
		}
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
		App->CL_MapEditor->VCam[V_BL]->ZoomFactor = 0.4;

		App->CL_MapEditor->VCam[V_BL]->CamPos.x = 0;//App->CL_Ogre->camNode->getPosition();
		App->CL_MapEditor->VCam[V_BL]->CamPos.y = 0;
		App->CL_MapEditor->VCam[V_BL]->CamPos.z = 0;
		
		App->CL_MapEditor->VCam[V_BL]->hDlg = hDlg;

		return TRUE;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDM_GRID_SNAP)
		{
			if (App->CL_Level->flag_UseGrid == 1)
			{
				App->CL_Level->flag_UseGrid = 0;
				CheckMenuItem(App->mMenu, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_UNCHECKED);
			}
			else
			{
				App->CL_Level->flag_UseGrid = 1;
				CheckMenuItem(App->mMenu, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_CHECKED);
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDM_RESET_VIEW)
		{
			RECT		Rect;
			GetClientRect(App->CL_MapEditor->Current_View->hDlg, &Rect);

			App->CL_MapEditor->Current_View->XCenter = (float)Rect.right / 2;
			App->CL_MapEditor->Current_View->YCenter = (float)Rect.bottom / 2;

			App->CL_MapEditor->Current_View->CamPos.x = 0;
			App->CL_MapEditor->Current_View->CamPos.y = 0;
			App->CL_MapEditor->Current_View->CamPos.z = 0;

			App->CL_MapEditor->Current_View->ZoomFactor = 0.3;

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

			return TRUE;
		}
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
		if (App->CL_MapEditor->flag_Context_Menu_Active == 1)
		{
			return false;
		}

		if (App->CL_MapEditor->flag_Right_Button_Down == 1 || App->CL_MapEditor->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			SetCursor(App->CL_MapEditor->hcBoth);
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
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

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];
		App->CL_MapEditor->On_Mouse_Move(RealCursorPosition, hDlg);

		return 1;
	}

	case WM_LBUTTONDOWN:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_MapEditor->flag_Right_Button_Down = 0;
		App->CL_MapEditor->flag_Left_Button_Down = 1;

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];

		App->CL_MapEditor->On_Left_Button_Down(RealCursorPosition, hDlg);

		return 1;
	}

	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];

		App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CL_MapEditor->On_Left_Button_Up(RealCursorPosition);

		/*App->CL_MapEditor->flag_Left_Button_Down = 0;
		App->CL_MapEditor->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);*/

		return 1;
	}

	case WM_RBUTTONDOWN:
	{
		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			GetCursorPos(&App->CL_MapEditor->mStartPoint);
			ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

			App->CL_MapEditor->flag_Left_Button_Down = 0;
			App->CL_MapEditor->flag_Right_Button_Down = 1;

			App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];

			App->CUR = SetCursor(NULL);
		}
		else
		{
			App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_BL];
			App->CL_MapEditor->Context_Menu(hDlg);
		}

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
// *	  Create_Bottom_Right_Window:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_MapEditor::Create_Bottom_Right_Ogre()
{
	Bottom_Right_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_RIGHT, Main_Dlg_Hwnd, (DLGPROC)Proc_Bottom_Right_Ogre);
	App->CL_Ogre->RenderHwnd = Bottom_Right_Hwnd;
}

// *************************************************************************
// *		Proc_Bottom_Right_Ogre:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Proc_Bottom_Right_Ogre(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		if (App->flag_OgreStarted == 0)
		{
			return (LONG)App->BlackBrush;
		}
		break;
	}

	/*case WM_CONTEXTMENU:
	{
		Debug
		return 0;
	}*/

	case WM_MOUSEMOVE:
	{
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(App->CL_MapEditor->Bottom_Right_Hwnd, &pos);

		if (App->CL_ImGui->flag_Imgui_Initialized == 1)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos.x = static_cast<float>(pos.x);
			io.MousePos.y = static_cast<float>(pos.y);
		}

		if (GetCursorPos(&pos) && App->flag_OgreStarted == 1)// && App->CL10_Dimensions->Mouse_Move_Mode == Enums::Edit_Mouse_None)
		{
			if (ScreenToClient(App->CL_MapEditor->Bottom_Right_Hwnd, &pos))
			{
				RECT rc;
				GetClientRect(App->CL_MapEditor->Bottom_Right_Hwnd, &rc);
				int width = rc.right - rc.left;
				int height = rc.bottom - rc.top;

				float tx = ((float)width / 2) - (float)pos.x;
			}
		}

		SetFocus(App->CL_MapEditor->Bottom_Right_Hwnd);
		
		break;
	}

	// Left Mouse Button
	case WM_LBUTTONDOWN:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = true;

		if (App->flag_OgreStarted == 1)
		{
			if (!ImGui::GetIO().WantCaptureMouse)
			{
				POINT p;
				GetCursorPos(&p);
				App->CL_MapEditor->mStartPoint = p;

				GetCursorPos(&p);
				App->CursorPosX = p.x;
				App->CursorPosY = p.y;
				App->CL_Ogre->Ogre3D_Listener->Pl_Cent500X = p.x;
				App->CL_Ogre->Ogre3D_Listener->Pl_Cent500Y = p.y;

				SetCapture(App->CL_MapEditor->Bottom_Right_Hwnd);
				SetCursorPos(App->CursorPosX, App->CursorPosY);
				App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
				App->CUR = SetCursor(NULL);
			}
			else
			{
				//App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
			}
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = false;

		if (App->flag_OgreStarted == 1)
		{
			ReleaseCapture();
			App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
			SetCursor(App->CUR);
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

			/*if (GetAsyncKeyState(VK_CONTROL) < 0)
			{
				App->CL_Picking->Mouse_Pick_Entity();

				int BI = App->CL_Model->Group[App->CL_Picking->m_SubMesh]->Face_Data[App->CL_Picking->Local_Face].Brush_Index;
				Brush* b;
				b = App->CL_Brush->Get_By_Index(BI);
				App->CL_Doc->DoBrushSelection(b, brushSelAlways);
			}*/

			return 1;
		}

		return 1;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		if (App->flag_OgreStarted == 1)
		{
			POINT p;
			GetCursorPos(&p);
			App->CursorPosX = p.x;
			App->CursorPosY = p.y;
			App->CL_Ogre->Ogre3D_Listener->Pl_Cent500X = p.x;
			App->CL_Ogre->Ogre3D_Listener->Pl_Cent500Y = p.y;
			SetCapture(App->CL_MapEditor->Bottom_Right_Hwnd);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
			App->CL_Ogre->Ogre3D_Listener->flag_RightMouseDown = 1;
			App->CUR = SetCursor(NULL);
			return 1;
		}
		else
		{
			App->CL_MapEditor->Current_View = App->CL_MapEditor->VCam[V_TR];
			App->CL_MapEditor->Context_Menu(hDlg);
		}


		return 1;
	}

	case WM_RBUTTONUP:
	{
		if (App->flag_OgreStarted == 1)
		{
			ReleaseCapture();
			App->CL_Ogre->Ogre3D_Listener->flag_RightMouseDown = 0;
			SetCursor(App->CUR);

			if (GetAsyncKeyState(VK_CONTROL) < 0)
			{
				App->CL_Picking->Mouse_Pick_Entity();

				int Index = App->CL_TXL_Editor->GetIndex_From_FileName(App->CL_Picking->m_Texture_FileName);

				if (Index > -1)
				{
					App->CL_Properties_Textures->Select_With_TextureName(App->CL_TXL_Editor->Texture_List[Index]->Name);

					if (App->CL_Properties_Textures->Dialog_Textures_Visible == 0)
					{
						App->CL_Properties_Tabs->Select_Textures_Tab();
					}
				}
			}

			return 1;
		}
	}

	}
	
	return FALSE;
}

// *************************************************************************
// *	  						On_Mouse_Move							   *
// *************************************************************************
void CL64_MapEditor::On_Mouse_Move(POINT CursorPosition, HWND hDlg)
{
	int	dx, dy;
	T_Vec3 sp, wp, dv;

	dx = (CursorPosition.x - mStartPoint.x);
	dy = (CursorPosition.y - mStartPoint.y);

	if ((dx == 0) && (dy == 0))	// don't do anything if no delta
	{
		return;
	}

	if (App->CL_MapEditor->flag_Right_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
	{
		dx = (CursorPosition.x);
		dy = (CursorPosition.y);

		App->CL_MapEditor->Zoom_View(Current_View->hDlg, dx, dy);
		return;
	}

	if (App->CL_MapEditor->flag_Left_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
	{
		dx = (CursorPosition.x);
		dy = (CursorPosition.y);

		App->CL_MapEditor->Pan_View(hDlg, dx, dy);
		return;
	}

	if (flag_Left_Button_Down == 1)
	{
		App->CL_Render->Render_ViewToWorld(Current_View, mStartPoint.x, mStartPoint.y, &sp);
		App->CL_Render->Render_ViewToWorld(Current_View, CursorPosition.x, CursorPosition.y, &wp);
		App->CL_Maths->Vector3_Subtract(&wp, &sp, &dv);

		if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			if (App->CL_Top_Tabs->flag_Brush_Move == 1)
			{
				App->CL_Doc->LockAxis(&dv);
				App->CL_Doc->MoveSelectedBrushes(&dv);
				Draw_Screen(hDlg);
			}

			if (App->CL_Top_Tabs->flag_Brush_Rotate == 1)
			{
				App->CL_Doc->LockAxis(&dv);
				App->CL_Render->Render_ViewDeltaToRotation(Current_View, (float)dx, &dv);
				App->CL_Doc->RotateSelectedBrushes(&dv);
				Draw_Screen(hDlg);
			}
		}

		if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
		{
			//LockAxisView (&dx, &dy);
			App->CL_Doc->ScaleSelected(dx, dy);

			Draw_Screen(hDlg);
		}


		POINT pt = mStartPoint;	// The position works on the delta mStartPoint...
		ClientToScreen(hDlg, &pt);
		SetCursorPos(pt.x, pt.y);
	}
	else
	{
		//int Tool;

		if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH) //|| (Tool == ID_TOOLS_BRUSH_MOVESELECTEDBRUSHES))
		{
			SetEditCursor(ID_TOOLS_BRUSH_SCALEBRUSH, &CursorPosition);

			//POINT pt = mStartPoint;	// The position works on the delta mStartPoint...
			//ClientToScreen(hDlg, &pt);
			//SetCursorPos(pt.x, pt.y);

		}
	}

}

// *************************************************************************
// *	  						On_Left_Button_Up						   *
// *************************************************************************
void CL64_MapEditor::On_Left_Button_Up(POINT CursorPosition)
{
	if (App->CL_Doc->mModeTool == ID_GENERALSELECT)
	{
		App->CL_Doc->SelectOrtho(CursorPosition, Current_View);
	}

	if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
	{
		if (App->CL_Top_Tabs->flag_Brush_Move == 1)
		{
			App->CL_Doc->DoneMovingBrushes();
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
			App->CL_Doc->flag_Is_Modified = 1;
		}

		if (App->CL_Top_Tabs->flag_Brush_Rotate == 1)
		{
			App->CL_Doc->UpdateSelected();
			App->CL_Doc->DoneRotate();

			App->CL_Doc->DoGeneralSelect(false);

			int Selected = App->CL_Properties_Brushes->Selected_Index;
			App->CL_Properties_Brushes->Selected_Index = Selected;
			App->CL_Properties_Brushes->OnSelchangeBrushlist(Selected, 0);

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
			App->CL_Doc->flag_Is_Modified = 1;

			App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_MOVEROTATEBRUSH;

		}

	}

	if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
	{
		App->CL_Doc->flag_Is_Modified = 1;
		//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		App->CL_Doc->SnapScaleNearest(App->CL_Doc->sides, App->CL_Render->Render_GetInidx(Current_View), Current_View);

		App->CL_Doc->DoneResize(App->CL_Doc->sides, App->CL_Render->Render_GetInidx(Current_View));
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
	
		//App->CLSB_Doc->UpdateSelected();
		//if ((ModeTool == ID_TOOLS_TEMPLATE) ||
		//	((App->CLSB_Doc->GetSelState() & ANYENTITY) && (!(App->CLSB_Doc->GetSelState() & ANYBRUSH))))
		//{
		//	App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
		//}
		//else
		{
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
		}
	}

	App->CUR = SetCursor(App->CUR);
}

// *************************************************************************
// *	  						On_Left_Button_Down						   *
// *************************************************************************
void CL64_MapEditor::On_Left_Button_Down(POINT CursorPosition, HWND hDlg)
{
	static const int SideLookup[25] =
	{
		5,	5,	4,	6,	6,
		5,	5,	4,	6,	6,
		1,	1,	0,	2,	2,
		9,	9,	8,	10,	10,
		9,	9,	8,	10,	10
	};

	App->CUR = SetCursor(NULL);

	GetCursorPos(&App->CL_MapEditor->mStartPoint);
	ScreenToClient(hDlg, &App->CL_MapEditor->mStartPoint);

	// ---------------------- Move Brush
	if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH) //|| (Tool == ID_TOOLS_BRUSH_MOVESELECTEDBRUSHES))
	{
		if (App->CL_Top_Tabs->flag_Brush_Move == 1)
		{
			int CursorSide = 0;
			App->CL_Doc->sides = SideLookup[CursorSide];

			App->CL_Maths->Vector3_Clear(&App->CL_Doc->FinalPos);
			App->CL_Doc->TempCopySelectedBrushes();
		}

		if (App->CL_Top_Tabs->flag_Brush_Rotate == 1)
		{
			int CursorSide = 0;
			App->CL_Doc->sides = SideLookup[CursorSide];

			App->CL_Maths->Vector3_Clear(&App->CL_Doc->FinalPos);
			App->CL_Doc->TempCopySelectedBrushes();
		}
	}

	// ---------------------- Scale Brush
	if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
	{
		int CursorSide = 0;
		CursorSide = GetCursorBoxPos(&CursorPosition);
		App->CL_Doc->sides = SideLookup[CursorSide];

		App->CL_Doc->ScaleNum = 0;

		App->CL_Maths->Vector3_Set(&App->CL_Doc->FinalScale, 1.0f, 1.0f, 1.0f);
		App->CL_Doc->TempCopySelectedBrushes();
	}
}

signed int CL64_MapEditor::fdocShowBrush(Brush const* b,Box3d const* ViewBox)
{
	return 1;// (App->CL_Brush->BrushIsVisible(b) && App->CL_Brush->Brush_TestBoundsIntersect(b, ViewBox));
}

// *************************************************************************
// *	  						BrushDraw								   *
// *************************************************************************
signed int CL64_MapEditor::BrushDraw(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData = (BrushDrawData*)lParam;
	

	//if ((pData->GroupId == fdoc_SHOW_ALL_GROUPS) || (Brush_GetGroupId(pBrush) == pData->GroupId))
	{
		if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
		{
			if (App->CL_MapEditor->fdocShowBrush(pBrush, pData->pViewBox))
			{
				App->CL_MapEditor->Render_RenderBrushFacesOrtho(pData->v, pBrush, App->CL_MapEditor->MemoryhDC);
			}
		}
	}

	return true;
}

#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

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
			ClientToScreen(hDlg, &pt);
			SetCursorPos(pt.x, pt.y);

		}
	}
}

// *************************************************************************
// *			Pan_View:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_MapEditor::Pan_View(HWND hDlg, int Dx, int Dy)
{
	T_Vec3 sp, wp, dv;;
	T_Vec3 dcamv;
	int Tolerence = 20;
	int	dx, dy;
	POINT RealCursorPosition;

	GetCursorPos(&RealCursorPosition);
	ScreenToClient(hDlg,&RealCursorPosition);

	dx = (RealCursorPosition.x - mStartPoint.x);
	dy = (RealCursorPosition.y - mStartPoint.y);

	if ((dx == 0) && (dy == 0))
	{
		return;
	}

	App->CL_Render->Render_ViewToWorld(Current_View, mStartPoint.x, mStartPoint.y, &sp);
	App->CL_Render->Render_ViewToWorld(Current_View, RealCursorPosition.x, RealCursorPosition.y, &wp);
	App->CL_Maths->Vector3_Subtract(&wp, &sp, &dv);	// delta in world space

	App->CL_Maths->Vector3_Scale(&dv, -1.0f, &dcamv);


	switch (Current_View->ViewType)
	{
	case VIEWTOP:  // Top Left
	{
		App->CL_Render->Render_MoveCamPosOrtho(Current_View, &dcamv);

		POINT pt = mStartPoint;
		ClientToScreen(hDlg, &pt);
		SetCursorPos(pt.x, pt.y);

		break;
	}

	case VIEWFRONT:  // Bottom Left
	{
		App->CL_Render->Render_MoveCamPosOrtho(Current_View, &dcamv);

		POINT pt = mStartPoint;
		ClientToScreen(hDlg, &pt);
		SetCursorPos(pt.x, pt.y);

		break;
	}
	case VIEWSIDE: // Top Right
	{
		App->CL_Render->Render_MoveCamPosOrtho(Current_View, &dcamv);

		POINT pt = mStartPoint;
		ClientToScreen(hDlg, &pt);
		SetCursorPos(pt.x, pt.y);

		break;
	}
	default:

		break;
	}

	App->CL_MapEditor->Draw_Screen(hDlg);
}

static signed int BrushDrawSelFacesOrtho(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData;

	pData = (BrushDrawData*)lParam;

	App->CL_MapEditor->Render_RenderBrushSelFacesOrtho(pData->v, pBrush, pData->pDC);

	return	GE_TRUE;
}

static POINT plist[64];

// *************************************************************************
// *					Render_RenderBrushSelFacesOrtho		  			   *
// *************************************************************************
void CL64_MapEditor::Render_RenderBrushSelFacesOrtho(ViewVars* Cam, Brush* b, HDC ViewDC)
{
	int	i, j;

	if (!b)
		return;

	for (i = 0; i < App->CL_Brush->Brush_GetNumFaces(b); i++)
	{
		Face* f = App->CL_Brush->Brush_GetFace(b, i);
		const T_Vec3* pnts = App->CL_Face->Face_GetPoints(f);

		if (!App->CL_Face->Face_IsSelected(f))
			continue;

		for (j = 0; j < App->CL_Face->Face_GetNumPoints(f); j++)
		{
			plist[j] = App->CL_Render->Render_OrthoWorldToView(Cam, &pnts[j]);
		}
		plist[j] = App->CL_Render->Render_OrthoWorldToView(Cam, &pnts[0]);
		Polyline(MemoryhDC, plist, j + 1);
	}
}

static signed int fdocBrushNotDetail(const Brush* b)
{
	return !App->CL_Brush->Brush_IsDetail(b);
}


static signed int fdocBrushIsSubtract(const Brush* b)
{
	return (App->CL_Brush->Brush_IsSubtract(b) && !App->CL_Brush->Brush_IsHollowCut(b));
}

// *************************************************************************
// *						Draw_Screen Terry Flanigan		  			   *
// *************************************************************************
void CL64_MapEditor::Draw_Screen(HWND hwnd)
{
	//Do_Timer
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

	T_Vec3 XTemp;
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
	
	bool test = 0;
	if (test == 0)
	{
		// ------------------------------------------ Draw Brushes
		SelectObject(MemoryhDC, PenBrushes);
		int BrushCount = App->CL_Brush->Get_Brush_Count();
		int Count = 0;
		
		brushDrawData.FlagTest = fdocBrushNotDetail;
		SelectObject(MemoryhDC, PenBrushes);
		App->CL_Level->Level_EnumLeafBrushes(App->CL_Doc->pLevel, &brushDrawData, BrushDraw);

		SelectObject(MemoryhDC, PenCutBrush);
		brushDrawData.FlagTest = fdocBrushIsSubtract;
		App->CL_Level->Level_EnumLeafBrushes(App->CL_Doc->pLevel, &brushDrawData, BrushDraw);
		
		brushDrawData.FlagTest = fdocBrushNotDetail;

		//if (Brush_TestBoundsIntersect(App->CLSB_Doc->CurBrush, &ViewBox))
		//if (App->CL_Brush->Get_Brush_Count() > 0)
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

		// ------------------------------------------ Draw Selected Brushes
		bool Draw_Sel = 0;
		if (Draw_Sel == 0)
		{
			
			SelectObject(MemoryhDC, PenSelected);
			int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
			
			int i = 0;
			for (i = 0; i < NumSelBrushes; i++)
			{
				Brush* pBrush;

				pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
				//if (m_pDoc->fdocShowBrush(pBrush, &ViewBox))
				{
					if (App->CL_Brush->Brush_IsMulti(pBrush))
					{
						App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(pBrush), &brushDrawData, BrushDraw);
					}
					else
					{
						Render_RenderBrushFacesOrtho(Current_View, App->CL_Doc->CurBrush, MemoryhDC);
					}
				}
			}

		}

		BrushList* BList;
		BList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

		SelectObject(MemoryhDC, PenSelectedFaces);
		App->CL_Brush->BrushList_EnumLeafBrushes(BList, &brushDrawData, BrushDrawSelFacesOrtho);

		if (App->CL_Doc->flag_Track_Camera == 1)
		{
			SelectObject(MemoryhDC, Pen_Camera);
			Draw_Camera(MemoryhDC);
		}

	}

	BitBlt(RealhDC, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, MemoryhDC, 0, 0, SRCCOPY);

	DeleteObject(OffScreenBitmap);
	DeleteDC(MemoryhDC);
	ReleaseDC(hwnd, RealhDC);
	//flag_IsDrawing = 0;

	//Get_Timer
}

// *************************************************************************
// *	  			Render_RenderBrushFacesOrtho		Genesis			   *
// *************************************************************************
void CL64_MapEditor::Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC)
{
	int	i, j;

	for (i = 0; i < App->CL_Brush->Brush_GetNumFaces(b); i++)
	{
		Face* f = App->CL_Brush->Brush_GetFace(b, i);
		const T_Vec3* pnts = App->CL_Face->Face_GetPoints(f);

		for (j = 0; j < App->CL_Face->Face_GetNumPoints(f); j++)
		{
			plist[j] = App->CL_Render->Render_OrthoWorldToView(Cam, &pnts[j]);
		}

		plist[j] = plist[0];
		Polyline(ViewDC, plist, j + 1);
	}
}

// *************************************************************************
// *			Draw_Camera:- Terry and Hazel Flanigan 2024	 		   *
// *************************************************************************
void CL64_MapEditor::Draw_Camera(HDC ViewDC)
{
#define ENTITY_SIZE (32.0f)  // 16" across
	
	T_Vec3 VecOrigin;
	T_Vec3 EntSizeWorld;	// entity size in world space
	T_Vec3 DummyPos;

	DummyPos.x = 0;
	DummyPos.y = 0;
	DummyPos.z = 0;

	T_Vec3 OgrePos;
	Ogre::Vector3 OgreRot;
	T_Vec3 Cam_Angles;

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
	EntSizeView = App->CL_Render->Render_OrthoWorldToView(Current_View ,&EntSizeWorld);
	App->CL_Maths->Vector3_Clear(&VecOrigin);
	OriginView = App->CL_Render->Render_OrthoWorldToView(Current_View ,&VecOrigin);
	// This one is the width and height of the Entity
	EntWidthHeight.x = std::max(OriginView.x, EntSizeView.x) - std::min(OriginView.x, EntSizeView.x);
	EntWidthHeight.y = std::max(OriginView.y, EntSizeView.y) - std::min(OriginView.y, EntSizeView.y);

	// This can have negative numbers
	EntSizeView.x -= OriginView.x;
	EntSizeView.y -= OriginView.y;
	
	// entity's position in the view

	if (App->flag_OgreStarted == 1)
	{
		OgreRot.x = App->CL_Ogre->camNode->getOrientation().getPitch().valueRadians();
		OgreRot.y = App->CL_Ogre->camNode->getOrientation().getYaw().valueRadians();
		OgreRot.z = 0;

		Cam_Angles.x = 3.141593 - OgreRot.x;
		Cam_Angles.y = -OgreRot.y;
		Cam_Angles.z = 0;

		OgrePos.x = App->CL_Ogre->camNode->getPosition().x;
		OgrePos.y = App->CL_Ogre->camNode->getPosition().y;
		OgrePos.z = App->CL_Ogre->camNode->getPosition().z;

		EntPosView = App->CL_Render->Render_OrthoWorldToView(Current_View, &OgrePos);
	}
	else
	{
		EntPosView = App->CL_Render->Render_OrthoWorldToView(Current_View, &DummyPos);
	}

	// Draw an X at the Camera position
	{
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

	// ------------------------------------------------------
	POINT		ptDirSlope;		// Slope of the "Direction" line
	POINT		ptRotationPoint;	// Point near end of "Direction" line we rotate to get arrowhead points
	POINT		ptRelRotatePoint;	// Rotation points about zero
	POINT		ptPlus45;			// Final Arrowhead point
	POINT		ptMinus45;			// Final Arrowhead point

	float		fPercentIntoLine;	// Distance into Direction line for rotation point
	float		fDirLength;		// Direction line length
	float		fEntityLength;		// Entity length
	float		fRadius;
	
	Matrix3d	Xfm;
	T_Vec3		VecTarg;
	
	POINT		LineEndView{ 0 };
	bool	bUIAvailable;

	// Get the Radius and the Angle  ONE of these must be present to show UI
	bUIAvailable = GE_FALSE;
	//if (pEnt->GetRadius(&fRadius, pEntityDefs) == GE_FALSE)
		fRadius = 100.0f;
	//else
		bUIAvailable = GE_TRUE;

	//if (pEnt->GetAngles(&Angles, pEntityDefs) == GE_FALSE)
		//App->CL_Maths->Vector3_Clear(&Angles);
	//else
		//bUIAvailable = GE_TRUE;

	//if (bUIAvailable == GE_FALSE)
		//return;

	// The camera angles are given in camera coordinates rather than
	// world coordinates (don't ask).
	// So we convert them here.
	/*if (pEnt->IsCamera())*/
	{
		App->CL_Maths->Vector3_Set(&Cam_Angles, Cam_Angles.z, (-Cam_Angles.y - M_PI / 2.0f), Cam_Angles.x);
	}

	
	App->CL_Maths->XForm3d_SetEulerAngles(&Xfm, &Cam_Angles);
	App->CL_Maths->Vector3_Set(&VecTarg, fRadius, 0.0f, 0.0f);
	App->CL_Maths->XForm3d_Transform(&Xfm, &VecTarg, &VecTarg);
	App->CL_Maths->Vector3_Add(&(OgrePos), &VecTarg, &VecTarg);

	LineEndView = App->CL_Render->Render_OrthoWorldToView(Current_View, &VecTarg);

	// Draw to the end point
	MoveToEx(ViewDC, EntPosView.x, EntPosView.y, NULL);
	LineTo(ViewDC, LineEndView.x, LineEndView.y);

	ptDirSlope.x = LineEndView.x - EntPosView.x;	// Slope of Direction line
	ptDirSlope.y = LineEndView.y - EntPosView.y;

	fDirLength = sqrt((float)(ptDirSlope.x * ptDirSlope.x) + (ptDirSlope.y * ptDirSlope.y));	// Length of Direction line
	fEntityLength = sqrt((float)(EntSizeView.x * EntSizeView.x) + (EntSizeView.y * EntSizeView.y));
	fEntityLength *= 1;	// Arrow 2x entity size
	fPercentIntoLine = 1.0f - (fEntityLength / fDirLength);
	ptRotationPoint.x = (long)(ptDirSlope.x * fPercentIntoLine);
	ptRotationPoint.y = (long)(ptDirSlope.y * fPercentIntoLine);
	ptRotationPoint.x += EntPosView.x;
	ptRotationPoint.y += EntPosView.y;

	ptRelRotatePoint.x = ptRotationPoint.x - LineEndView.x;
	ptRelRotatePoint.y = ptRotationPoint.y - LineEndView.y;

	ptPlus45.x = (long)(ptRelRotatePoint.x * COS45 - ptRelRotatePoint.y * SIN45);
	ptPlus45.y = (long)(ptRelRotatePoint.y * COS45 + ptRelRotatePoint.x * SIN45);
	ptMinus45.x = (long)(ptRelRotatePoint.x * MCOS45 - ptRelRotatePoint.y * MSIN45);
	ptMinus45.y = (long)(ptRelRotatePoint.y * MCOS45 + ptRelRotatePoint.x * MSIN45);

	ptPlus45.x += LineEndView.x;
	ptPlus45.y += LineEndView.y;
	ptMinus45.x += LineEndView.x;
	ptMinus45.y += LineEndView.y;

	LineTo(ViewDC, ptPlus45.x, ptPlus45.y);
	LineTo(ViewDC, ptMinus45.x, ptMinus45.y);
	LineTo(ViewDC, LineEndView.x, LineEndView.y);

}

// *************************************************************************
// *	  						SetEditCursor							   *
// *************************************************************************
void CL64_MapEditor::SetEditCursor(int Tool, const POINT* pMousePos)
{
	//for sizing stuff
	static const char* SizeCursors[25] =
	{
		IDC_SIZENWSE,	IDC_SIZENWSE,	IDC_SIZENS,		IDC_SIZENESW,	IDC_SIZENESW,
		IDC_SIZENWSE,	IDC_SIZENWSE,	IDC_SIZENS,		IDC_SIZENESW,	IDC_SIZENESW,
		IDC_SIZEWE,		IDC_SIZEWE,		IDC_NO,			IDC_SIZEWE,		IDC_SIZEWE,
		IDC_SIZENESW,	IDC_SIZENESW,	IDC_SIZENS,		IDC_SIZENWSE,	IDC_SIZENWSE,
		IDC_SIZENESW,	IDC_SIZENESW,	IDC_SIZENS,		IDC_SIZENWSE,	IDC_SIZENWSE
	};

	static const char* ShearCursors[25] =
	{
		IDC_NO,			IDC_SIZEWE,		IDC_SIZEWE,		IDC_SIZEWE,		IDC_NO,
		IDC_SIZENS,		IDC_NO,			IDC_SIZEWE,		IDC_NO,			IDC_SIZENS,
		IDC_SIZENS,		IDC_SIZENS,		IDC_NO,			IDC_SIZENS,		IDC_SIZENS,
		IDC_SIZENS,		IDC_NO,			IDC_SIZEWE,		IDC_NO,			IDC_SIZENS,
		IDC_NO,			IDC_SIZEWE,		IDC_SIZEWE,		IDC_SIZEWE,		IDC_NO
	};

	const char* WhichCursor = NULL;
	int CursorIndex;

	assert((Tool == ID_TOOLS_BRUSH_SCALEBRUSH) || (Tool == ID_TOOLS_BRUSH_SHEARBRUSH));

	// Determine where the cursor is on the box surrounding the selected brush,
	// and set the appropriate cursor.
	if (pMousePos->x < 0 || pMousePos->y < 0)
	{
		return;
	}

	CursorIndex = GetCursorBoxPos(pMousePos);

	switch (Tool)
	{
	case ID_TOOLS_BRUSH_SCALEBRUSH:
	{
		// Scaling it's just a simple lookup
		WhichCursor = SizeCursors[CursorIndex];
		break;
	}

	/*case ID_TOOLS_BRUSH_SHEARBRUSH:
		WhichCursor = ShearCursors[CursorIndex];
		break;*/
	default:
		assert(0);
		break;
	}

	HCURSOR	Test;
	Test = LoadCursor(NULL, SizeCursors[CursorIndex]);
	SetCursor(Test);

}

// *************************************************************************
// *	  						GetCursorBoxPos							   *
// *************************************************************************
int CL64_MapEditor::GetCursorBoxPos(const POINT* ptMousePos)
{
	const Box3d* pBrushBox;
	POINT ptMin, ptMax;
	int dx, dy;
	int x, y;
	int horiz, vert;
	int lookup[4] = { 1, 2, 2, 3 };

	//	Box3d BrushBox;
	//	if (pDoc->mModeTool == ID_TOOLS_TEMPLATE)
	pBrushBox = App->CL_Brush->Brush_GetBoundingBox(App->CL_Doc->CurBrush);
	//	else
	//	{
	//		SelBrushList_GetBoundingBox(pDoc->pSelBrushes, &BrushBox);
	//		pBrushBox = &BrushBox;
	//	}

		// obtain screen coordinates for bounding box min and max points
	ptMin = App->CL_Render->Render_OrthoWorldToView(Current_View, &pBrushBox->Min);
	ptMax = App->CL_Render->Render_OrthoWorldToView(Current_View, &pBrushBox->Max);

	// make sure the min and max points are correct...
	if (ptMin.x > ptMax.x)
	{
		int temp;

		temp = ptMin.x;
		ptMin.x = ptMax.x;
		ptMax.x = temp;
	}
	if (ptMin.y > ptMax.y)
	{
		int temp;

		temp = ptMin.y;
		ptMin.y = ptMax.y;
		ptMax.y = temp;
	}

	// compute horizontal first
	x = ptMousePos->x - ptMin.x;
	dx = (ptMax.x - ptMin.x);
	if (dx == 0) horiz = 0; else horiz = (4 * x) / dx;
	if (horiz < 0) horiz = 0;
	else if (horiz > 3) horiz = 4;
	else horiz = lookup[horiz];

	// and vertical
	y = ptMousePos->y - ptMin.y;
	dy = (ptMax.y - ptMin.y);
	if (dy == 0) vert = 0; else vert = (4 * y) / dy;
	if (vert < 0) vert = 0;
	else if (vert > 3) vert = 3;
	else vert = lookup[vert];

	// return index...
	return (vert * 5) + horiz;
}



