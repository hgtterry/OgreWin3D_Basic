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
#include "CL64_Editor_Map.h"

#define IDM_GRID_SNAP 1
#define IDM_RESET_VIEW 2
#define IDM_CENTRE_ONCAMERA 3
#define IDM_COPY 4
#define IDM_PASTE 5
#define IDM_GOTO 6
#define IDM_PREVIEW 7
#define IDM_MOVE 8
#define IDM_SCALE 9
#define IDM_ROTATE 10
#define IDM_SCENE_EDITOR 11

#define IDM_3D_WIRED 20
#define IDM_3D_TEXTURED 21
#define IDM_3D_PREVIEW 22
#define IDM_3D_SCENE_EDITOR 23
#define IDM_3D_ENVIRONMENT 24

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

CL64_Editor_Map::CL64_Editor_Map()
{
	Main_View_Dlg_Hwnd = NULL;

	GridSize = 128, 
	GridSnapSize = 8;

	Top_Left_Window_Hwnd =		nullptr;
	Top_Right_Window_Hwnd =		nullptr;
	Bottom_Left_Window_Hwnd =	nullptr;
	Bottom_Ogre_Right_Hwnd =	nullptr;

	Top_Left_Banner_Hwnd =		nullptr;
	Top_Right_Banner_Hwnd =		nullptr;
	Bottom_Left_Banner_Hwnd =	nullptr;
	Bottom_Ogre_Banner =		nullptr;

	LEFT_WINDOW_WIDTH = 500;
	nleftWnd_width = 500;
	
	LEFT_WINDOW_DEPTH = 215;
	nleftWnd_Depth = 215;

	TOP_POS_BOTLEFT = 215;

	RIGHT_MINIMUM_SPACE = 1000;
	LEFT_MINIMUM_SPACE = 15;

	Copy_Spliter_Width = 500;
	Copy_Spliter_Depth = 215;

	Selected_Window = Enums::Selected_Map_View_None;

	Do_Width = 0;
	Do_Depth = 0;
	Do_All = 0;

	flag_Left_Button_Down = 0;
	flag_Right_Button_Down = 0;
	flag_Context_Menu_Active = 0;
	flag_Environment_On = true;

	BackGround_Brush = CreateSolidBrush(RGB(60, 60, 60));

	Pen_Fine_Grid = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	Pen_Grid = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
	PenTemplate = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	PenBrushes = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	PenSelected = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	Pen_Camera = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));
	PenSelectedFaces = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	PenCutBrush = CreatePen(PS_SOLID, 1, RGB(255, 155, 0));
	PenEntity = CreatePen(PS_SOLID, 1, RGB(180, 180, 255));


	hcSizeEW = LoadCursor(NULL, IDC_SIZEWE);
	hcSizeNS = LoadCursor(NULL, IDC_SIZENS);
	hcBoth = LoadCursor(NULL, IDC_SIZEALL);

	mStartPoint.x = 0;
	mStartPoint.y = 0;

	Current_View = nullptr;

	int Count = 0;
	while (Count < 3)
	{
		VCam[Count] = nullptr;
		Count++;
	}

	MemoryhDC = nullptr;
}

CL64_Editor_Map::~CL64_Editor_Map()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Editor_Map::Reset_Class()
{
	Reset_Views_All();
}

// *************************************************************************
// *	  		Reset_Views_All:- Terry and Hazel Flanigan 2024	     *
// *************************************************************************
void CL64_Editor_Map::Reset_Views_All()
{
	Init_Views(Enums::Selected_Map_View_None);
	Resize_Windows(Main_View_Dlg_Hwnd, nleftWnd_width, nleftWnd_Depth);

	Save_Splitter_Width_Depth();

	int Count = 0;

	while (Count < 3)
	{
		RECT		Rect;
		GetClientRect(VCam[Count]->hDlg, &Rect);

		VCam[Count]->XCenter = (float)Rect.right / 2;
		VCam[Count]->YCenter = (float)Rect.bottom / 2;

		VCam[Count]->CamPos.x = 0;
		VCam[Count]->CamPos.y = 0;
		VCam[Count]->CamPos.z = 0;

		VCam[Count]->ZoomFactor = 0.3;

		Count++;
	}

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
}

// *************************************************************************
// *	  	Reset_To_Camera:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Editor_Map::Reset_To_Camera()
{
	RECT		Rect;
	GetClientRect(Current_View->hDlg, &Rect);

	Current_View->XCenter = (float)Rect.right / 2;
	Current_View->YCenter = (float)Rect.bottom / 2;

	Ogre::Vector3 Pos;
	Pos = App->CL_Ogre->camNode->getPosition();

	Current_View->CamPos.x = Pos.x;
	Current_View->CamPos.y = Pos.y;
	Current_View->CamPos.z = Pos.z;

	Current_View->ZoomFactor = 0.3;

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

}

// *************************************************************************
// *	  			Init_Views:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Editor_Map::Init_Views(int View)
{
	RECT rect;
	GetClientRect(Main_View_Dlg_Hwnd, &rect);

	if (View == Enums::Selected_Map_View_None)
	{
		LEFT_WINDOW_WIDTH = rect.right / 2;
		nleftWnd_width = rect.right / 2;

		LEFT_WINDOW_DEPTH = rect.bottom / 2;
		TOP_POS_BOTLEFT = rect.bottom / 2;
	}

	if (View == Enums::Selected_Map_View_3D)
	{
		LEFT_WINDOW_WIDTH = 0;
		nleftWnd_width = 0;

		LEFT_WINDOW_DEPTH = 0;
		TOP_POS_BOTLEFT = 0;
	}

	if (View == Enums::Selected_Map_View_TL)
	{
		LEFT_WINDOW_WIDTH = rect.right;
		nleftWnd_width = rect.right;

		LEFT_WINDOW_DEPTH = rect.bottom;
		TOP_POS_BOTLEFT = rect.bottom;
	}

	if (View == Enums::Selected_Map_View_TR)
	{
		LEFT_WINDOW_WIDTH = 0;
		nleftWnd_width = 0;

		LEFT_WINDOW_DEPTH = rect.bottom;
		TOP_POS_BOTLEFT = rect.bottom;
	}

	if (View == Enums::Selected_Map_View_BL)
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
// *			Resize_Windows:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Editor_Map::Resize_Windows(HWND hDlg, int newWidth, int newDepth)
{
	RECT clientRect;
	GetClientRect(hDlg, &clientRect);

	const int adjustedDepth = newDepth - 3;
	const int bannerHeight = 16;

	// Resize Top Left Window
	MoveWindow(Top_Left_Window_Hwnd,
		0,
		0,
		clientRect.left + (newWidth - WIDTH_ADJUST),
		adjustedDepth,
		FALSE);
	MoveWindow(Top_Left_Banner_Hwnd, 0, 0, newWidth - WIDTH_ADJUST, bannerHeight, FALSE);

	// Resize Top Right Window
	MoveWindow(Top_Right_Window_Hwnd,
		newWidth + WIDTH_ADJUST,
		0,
		clientRect.right - (newWidth + WIDTH_ADJUST),
		adjustedDepth,
		FALSE);
	MoveWindow(Top_Right_Banner_Hwnd, 0, 0, clientRect.right - newWidth - WIDTH_ADJUST, bannerHeight, FALSE);

	// Resize Bottom Left Window
	MoveWindow(Bottom_Left_Window_Hwnd,
		0,
		newDepth,
		newWidth - WIDTH_ADJUST,
		clientRect.bottom - (newDepth + BOTTOM_POS_BOTLEFT),
		FALSE);
	MoveWindow(Bottom_Left_Banner_Hwnd, 0, 0, newWidth - WIDTH_ADJUST, bannerHeight, FALSE);

	// Resize Ogre Window
	MoveWindow(Bottom_Ogre_Right_Hwnd,
		newWidth + WIDTH_ADJUST,
		newDepth,
		clientRect.right - (newWidth + WIDTH_ADJUST),
		clientRect.bottom - (newDepth + BOTTOM_POS_BOTLEFT),
		FALSE);
	MoveWindow(Bottom_Ogre_Banner, 0, 0, clientRect.right - newWidth - WIDTH_ADJUST, bannerHeight, FALSE);


	RedrawWindow(Main_View_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	ResizeOgreWindow();
}

// ************************************************************************
// *			Resize_OgreWin:- Terry and Hazel Flanigan 2025			*
// ************************************************************************
void CL64_Editor_Map::ResizeOgreWindow()
{
	RECT clientRect;
	GetClientRect(Bottom_Ogre_Right_Hwnd, &clientRect);

	// Set the position and size of the window
	SetWindowPos(App->ViewGLhWnd, NULL, 0, 17, clientRect.right, clientRect.bottom - 17, SWP_NOZORDER);

	// Check if the Ogre engine has started
	if (App->flag_OgreStarted == 1)
	{
		// Get the updated client rectangle
		RECT updatedRect;
		GetClientRect(Bottom_Ogre_Right_Hwnd, &updatedRect);

		// Ensure the height is valid and the camera is initialized
		if ((updatedRect.bottom - updatedRect.top) != 0 && App->CL_Ogre->mCamera != nullptr)
		{
			App->CL_Ogre->mWindow->windowMovedOrResized();
			App->CL_Ogre->mCamera->setAspectRatio(static_cast<Ogre::Real>(App->CL_Ogre->mWindow->getWidth()) /
				static_cast<Ogre::Real>(App->CL_Ogre->mWindow->getHeight()));

			App->CL_Ogre->camNode->yaw(Radian(0));
		}
	}
}

// *************************************************************************
// *			Init_Map_Views:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Editor_Map::Init_Map_Views()
{
	Main_View_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAPEDITOR, App->MainHwnd, (DLGPROC)Proc_Main_Dlg);

	Create_Top_Left_Window();
	Create_Top_Right_Window();
	Create_Bottom_Left_Window();
	Create_Ogre_Bottom_Right();

	RECT rcl;
	GetClientRect(App->MainHwnd, &rcl);
	MoveWindow(Main_View_Dlg_Hwnd, 0, 50, rcl.right, rcl.bottom - 50, TRUE);
	
	Init_Views(Enums::Selected_Map_View_None);
	RedrawWindow(Main_View_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *			Proc_Main_Dlg:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
LRESULT CALLBACK CL64_Editor_Map::Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static  BOOL        xSizing;
	static  BOOL        ySizing;

	static UINT			nwnd_resizing = 0;

	RECT rect;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_Editor_Map->Main_View_Dlg_Hwnd = hDlg;
		
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
		App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_None);
		App->CL_Editor_Map->Resize_Windows(hDlg, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

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
			xSizing = (xPos > App->CL_Editor_Map->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_Editor_Map->nleftWnd_width + SPLITTER_BAR_WIDTH);
			ySizing = (yPos > App->CL_Editor_Map->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_Editor_Map->nleftWnd_Depth + SPLITTER_BAR_WIDTH);

			if (xSizing && ySizing == 0)
			{
				SetCapture(hDlg);
				if (xSizing)
				{
					SetCursor(App->CL_Editor_Map->hcSizeEW);
				}

			}

			if (ySizing && xSizing == 0)
			{
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(App->CL_Editor_Map->hcSizeNS);
				}

			}

			if (xSizing && ySizing)
			{
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(App->CL_Editor_Map->hcBoth);
				}
			}
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		//if (App->CL_Editor_Map->flag_Right_Button_Down == 0)
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
					SetRect(&focusrect, App->CL_Editor_Map->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
						App->CL_Editor_Map->nleftWnd_width + WIDTH_ADJUST,
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

			App->CL_Editor_Map->Resize_Windows(hDlg, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
			App->CL_Editor_Map->Save_Splitter_Width_Depth();
			
		}

		return 1;
	}

	case WM_MOUSEMOVE:
	{
		//if (App->CL_Editor_Map->flag_Right_Button_Down == 0)
		{
			int   xPos;
			int   yPos;

			// Get the x and y co-ordinates of the mouse
			xPos = (int)LOWORD(lParam);
			yPos = (int)HIWORD(lParam);

			if (xPos < App->CL_Editor_Map->LEFT_MINIMUM_SPACE || xPos > App->CL_Editor_Map->RIGHT_MINIMUM_SPACE)
			{
				return 0;
			}

			// Checks if the left button is pressed during dragging the splitter
			if (wParam == MK_LBUTTON)
			{
				if (xSizing && App->CL_Editor_Map->Do_Width == 1)
				{
					RECT    focusrect;
					HDC     hdc;

					hdc = GetDC(hDlg);
					GetClientRect(hDlg, &rect);

					if (xSizing && App->CL_Editor_Map->Do_Width == 1)
					{
						SetRect(&focusrect, App->CL_Editor_Map->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
							App->CL_Editor_Map->nleftWnd_width + WIDTH_ADJUST,
							rect.bottom - 6);

						DrawFocusRect(hdc, &focusrect);

						App->CL_Editor_Map->nleftWnd_width = xPos;

						SetRect(&focusrect, App->CL_Editor_Map->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
							App->CL_Editor_Map->nleftWnd_width + WIDTH_ADJUST,
							rect.bottom - 6);

						DrawFocusRect(hdc, &focusrect);
					}

					ReleaseDC(hDlg, hdc);
				}

				if (ySizing && App->CL_Editor_Map->Do_Depth == 1)
				{
					RECT    focusrect;
					HDC     hdc;

					hdc = GetDC(hDlg);
					GetClientRect(hDlg, &rect);

					if (ySizing && App->CL_Editor_Map->Do_Depth == 1)
					{
						SetRect(&focusrect, 0, App->CL_Editor_Map->nleftWnd_Depth, rect.right, App->CL_Editor_Map->nleftWnd_Depth + (WIDTH_ADJUST * 2));

						DrawFocusRect(hdc, &focusrect);

						App->CL_Editor_Map->nleftWnd_Depth = yPos;

						SetRect(&focusrect, 0, App->CL_Editor_Map->nleftWnd_Depth, rect.right, App->CL_Editor_Map->nleftWnd_Depth + (WIDTH_ADJUST * 2));

						DrawFocusRect(hdc, &focusrect);
					}

					ReleaseDC(hDlg, hdc);
				}
			}

			if ((xPos > App->CL_Editor_Map->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_Editor_Map->nleftWnd_width + SPLITTER_BAR_WIDTH))
			{
				if (App->CL_Editor_Map->Do_All == 0)
				{
					SetCursor(App->CL_Editor_Map->hcSizeEW);
				}

				App->CL_Editor_Map->Do_Width = 1;
			}
			else
			{
				App->CL_Editor_Map->Do_Width = 0;
			}

			if ((yPos > App->CL_Editor_Map->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_Editor_Map->nleftWnd_Depth + SPLITTER_BAR_WIDTH))
			{
				if (App->CL_Editor_Map->Do_All == 0)
				{
					SetCursor(App->CL_Editor_Map->hcSizeNS);
				}

				App->CL_Editor_Map->Do_Depth = 1;
			}
			else
			{
				App->CL_Editor_Map->Do_Depth = 0;
			}

			if (App->CL_Editor_Map->Do_Width == 1 && App->CL_Editor_Map->Do_Depth == 1)
			{
				SetCursor(App->CL_Editor_Map->hcBoth);
				App->CL_Editor_Map->Do_All = 1;
			}
			else
			{
				App->CL_Editor_Map->Do_All = 0;
			}
		}

		return 1;
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *	  		Set_Views_Defaults:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Editor_Map::Set_Views_Defaults(int Index, Ogre::int32 View, const char* Name)
{
	strcpy(VCam[Index]->Name, Name);
	VCam[Index]->ViewType = View;
	VCam[Index]->ZoomFactor = 0.4;

	VCam[Index]->XCenter = 310;
	VCam[Index]->YCenter = 174;

	VCam[Index]->XScreenScale = 0;
	VCam[Index]->YScreenScale = 0;

	VCam[Index]->Width = 310;
	VCam[Index]->Height = 174;

	App->CL_Maths->Vector3_Set(&VCam[Index]->CamPos,0,0,0);

	VCam[Index]->MaxScreenScaleInv = 100;
}

// *************************************************************************
// *	  	Set_Splitter_WidthDepth:- Terry and Hazel Flanigan 2024		  *
// *************************************************************************
void CL64_Editor_Map::Set_Splitter_WidthDepth(int Width, int Depth)
{
	nleftWnd_width = Width;
	nleftWnd_Depth = Depth;
}

// *************************************************************************
// *	  Save_Splitter_Width_Depth:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Editor_Map::Save_Splitter_Width_Depth()
{
	Copy_Spliter_Depth = nleftWnd_Depth;
	Copy_Spliter_Width = nleftWnd_width;
}

// *************************************************************************
// *	  	Create_Top_Left_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Editor_Map::Create_Top_Left_Window()
{
	VCam[V_TL] = new ViewVars;
	Set_Views_Defaults(V_TL, VIEWTOP, "Top_Left");

	Top_Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, Main_View_Dlg_Hwnd, (DLGPROC)Proc_Top_Left_Window);
	
	VCam[V_TL]->hDlg = Top_Left_Window_Hwnd;
}

// *************************************************************************
// *		Proc_Top_Left_Window:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_Editor_Map::Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TL_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_Editor_Map->Top_Left_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_TL_TITLE);
		return TRUE;
	}

	case WM_COMMAND:
	{
		if (App->CL_Editor_Map->Context_Command(LOWORD(wParam)))
		{
			return TRUE;
		}
	}
	
	case WM_CTLCOLORSTATIC:
	{
		if (App->CL_Editor_Map->Selected_Window == Enums::Selected_Map_View_TL)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_Green;
		}
		else
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_Editor_Map->BackGround_Brush;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_SETCURSOR:
	{
		if (App->CL_Editor_Map->flag_Context_Menu_Active == 1)
		{
			return false;
		}

		if (App->CL_Editor_Map->flag_Right_Button_Down == 1 || App->CL_Editor_Map->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			SetCursor(App->CL_Editor_Map->hcBoth);
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

		App->CL_Editor_Map->On_Mouse_Move(RealCursorPosition,hDlg);
		
		return 1;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		App->CL_Editor_Map->flag_Right_Button_Down = 0;
		App->CL_Editor_Map->flag_Left_Button_Down = 1;

		App->CL_Editor_Map->On_Left_Button_Down(RealCursorPosition,hDlg);

		return 1;
	}

	// Left Mouse Up
	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		App->CL_Editor_Map->flag_Left_Button_Down = 0;
		App->CL_Editor_Map->flag_Right_Button_Down = 0;

		App->CL_Editor_Map->On_Left_Button_Up(RealCursorPosition);

		return 1;
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			GetCursorPos(&App->CL_Editor_Map->mStartPoint);
			ScreenToClient(hDlg, &App->CL_Editor_Map->mStartPoint);

			App->CL_Editor_Map->flag_Right_Button_Down = 1;
			App->CL_Editor_Map->flag_Left_Button_Down = 0;

			App->CUR = SetCursor(NULL);
		}
		
		return 1;
	}

	// Right Mouse Up
	case WM_RBUTTONUP:
	{
		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_Editor_Map->flag_Right_Button_Down = 0;
			App->CL_Editor_Map->flag_Left_Button_Down = 0;

			App->CUR = SetCursor(App->CUR);
		}
		else
		{
			App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
			App->CL_Editor_Map->Context_Menu(hDlg);
		}

		return 1;
	}

	case WM_PAINT:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
		App->CL_Editor_Map->Draw_Screen(hDlg);

		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  	Create_Top_Right_Window:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Editor_Map::Create_Top_Right_Window()
{
	VCam[V_TR] = new ViewVars;
	Set_Views_Defaults(V_TR, VIEWSIDE, "Top_Right");

	Top_Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_RIGHT, Main_View_Dlg_Hwnd, (DLGPROC)Proc_Top_Right_Window);

	VCam[V_TR]->hDlg = Top_Right_Window_Hwnd;
}

// *************************************************************************
// *		Proc_Top_Right_Window:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_Editor_Map::Proc_Top_Right_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TR_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_Editor_Map->Top_Right_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_TR_TITLE);
		return TRUE;
	}

	case WM_COMMAND:
	{
		if (App->CL_Editor_Map->Context_Command(LOWORD(wParam)))
		{
			return TRUE;
		}
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_TR_TITLE) == (HWND)lParam)
		{
			if (App->CL_Editor_Map->Selected_Window == Enums::Selected_Map_View_TR)
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->Brush_Green;
			}
			else
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_Editor_Map->BackGround_Brush;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_SETCURSOR:
	{
		if (App->CL_Editor_Map->flag_Context_Menu_Active == 1)
		{
			return false;
		}

		if (App->CL_Editor_Map->flag_Right_Button_Down == 1 || App->CL_Editor_Map->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			SetCursor(App->CL_Editor_Map->hcBoth);
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

		App->CL_Editor_Map->On_Mouse_Move(RealCursorPosition, hDlg);

		return 1;
	}

	// Left Button Down
	case WM_LBUTTONDOWN:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TR];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TR)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TR);
		}

		App->CL_Editor_Map->flag_Right_Button_Down = 0;
		App->CL_Editor_Map->flag_Left_Button_Down = 1;

		App->CL_Editor_Map->On_Left_Button_Down(RealCursorPosition, hDlg);

		return 1;
	}

	// Left Button Up
	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->flag_Right_Button_Down = 0;
		App->CL_Editor_Map->flag_Left_Button_Down = 0;

		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TR];

		App->CL_Editor_Map->On_Left_Button_Up(RealCursorPosition);

		return 1;
	}

	// Right Button Down
	case WM_RBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TR];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TR)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TR);
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			GetCursorPos(&App->CL_Editor_Map->mStartPoint);
			ScreenToClient(hDlg, &App->CL_Editor_Map->mStartPoint);

			App->CL_Editor_Map->flag_Right_Button_Down = 1;
			App->CL_Editor_Map->flag_Left_Button_Down = 0;

			App->CUR = SetCursor(NULL);
		}
	
		return 1;
	}

	// Right Button Up
	case WM_RBUTTONUP:
	{

		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_Editor_Map->flag_Right_Button_Down = 0;
			App->CL_Editor_Map->flag_Left_Button_Down = 0;

			App->CUR = SetCursor(App->CUR);
		}
		else
		{
			App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TR];
			App->CL_Editor_Map->Context_Menu(hDlg);
		}

		return 1;
	}

	case WM_PAINT:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TR];
		App->CL_Editor_Map->Draw_Screen(hDlg);

		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  Create_Bottom_Left_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Editor_Map::Create_Bottom_Left_Window()
{
	VCam[V_BL] = new ViewVars;
	Set_Views_Defaults(V_BL, VIEWFRONT, "Bottom_Left");

	Bottom_Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_LEFT, Main_View_Dlg_Hwnd, (DLGPROC)Proc_Bottom_Left_Window);

	VCam[V_BL]->hDlg = Bottom_Left_Window_Hwnd;
}

// *************************************************************************
// *		Proc_Bottom_Left_Window:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
LRESULT CALLBACK CL64_Editor_Map::Proc_Bottom_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_BL_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_Editor_Map->Bottom_Left_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_BL_TITLE);
		return TRUE;
	}

	case WM_COMMAND:
	{
		if (App->CL_Editor_Map->Context_Command(LOWORD(wParam)))
		{
			return TRUE;
		}
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BL_TITLE) == (HWND)lParam)
		{
			if (App->CL_Editor_Map->Selected_Window == Enums::Selected_Map_View_BL)
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->Brush_Green;
			}
			else
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_Editor_Map->BackGround_Brush;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_SETCURSOR:
	{
		if (App->CL_Editor_Map->flag_Context_Menu_Active == 1)
		{
			return false;
		}

		if (App->CL_Editor_Map->flag_Right_Button_Down == 1 || App->CL_Editor_Map->flag_Left_Button_Down == 1)
		{
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			SetCursor(App->CL_Editor_Map->hcBoth);
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

		App->CL_Editor_Map->On_Mouse_Move(RealCursorPosition, hDlg);

		return 1;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_BL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_BL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_BL);
		}

		App->CL_Editor_Map->flag_Right_Button_Down = 0;
		App->CL_Editor_Map->flag_Left_Button_Down = 1;

		App->CL_Editor_Map->On_Left_Button_Down(RealCursorPosition, hDlg);

		return 1;
	}

	// Left Mouse Up
	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_BL];

		App->CL_Editor_Map->flag_Left_Button_Down = 0;
		App->CL_Editor_Map->flag_Right_Button_Down = 0;

		App->CL_Editor_Map->On_Left_Button_Up(RealCursorPosition);

		return 1;
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_BL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_BL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_BL);
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			GetCursorPos(&App->CL_Editor_Map->mStartPoint);
			ScreenToClient(hDlg, &App->CL_Editor_Map->mStartPoint);

			App->CL_Editor_Map->flag_Left_Button_Down = 0;
			App->CL_Editor_Map->flag_Right_Button_Down = 1;

			App->CUR = SetCursor(NULL);
		}
		
		return 1;
	}

	// Right Mouse up
	case WM_RBUTTONUP:
	{
		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			App->CL_Editor_Map->flag_Left_Button_Down = 0;
			App->CL_Editor_Map->flag_Right_Button_Down = 0;

		App->CUR = SetCursor(App->CUR);
		}
		else
		{
			App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_BL];
			App->CL_Editor_Map->Context_Menu(hDlg);
		}

		return 1;
	}

	case WM_PAINT:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_BL];
		App->CL_Editor_Map->Draw_Screen(hDlg);
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *		 Create_Ogre_Bottom_Right:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Editor_Map::Create_Ogre_Bottom_Right()
{
	VCam[V_Ogre] = new ViewVars;
	Set_Views_Defaults(V_Ogre, VIEWOGRE, "Ogre_Window");

	Bottom_Ogre_Right_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_RIGHT, Main_View_Dlg_Hwnd, (DLGPROC)ViewerMain_Proc);
	
	VCam[V_Ogre]->hDlg = Bottom_Ogre_Right_Hwnd;

	App->CL_Ogre->RenderHwnd = App->ViewGLhWnd;
}

// *************************************************************************
// *			ViewerMain_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK CL64_Editor_Map::ViewerMain_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_3D_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_Editor_Map->Bottom_Ogre_Banner = GetDlgItem(hDlg, IDC_ST_3D_TITLE);

		App->ViewGLhWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_RENDER_WINDOW, hDlg, (DLGPROC)Proc_Ogre_BR);
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_3D_TITLE) == (HWND)lParam)
		{
			if (App->CL_Editor_Map->Selected_Window == Enums::Selected_Map_View_3D)
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->Brush_Green;
			}
			else
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
		}

		return FALSE;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_Ogre];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_3D)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_3D);
		}
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_Ogre];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_3D)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_3D);
		}
		return 1;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}
	case WM_COMMAND:
	{

	}
	break;
	}
	return FALSE;
}

// *************************************************************************
// *		Proc_Ogre_BR:- Terry and Hazel Flanigan 2024 				   *
// *************************************************************************
LRESULT CALLBACK CL64_Editor_Map::Proc_Ogre_BR(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

	case WM_COMMAND:
	{
		bool isSceneEditorActive = (App->CL_Editor_Control->flag_Scene_Editor_Active == 1);
		bool commandHandled = false;

		if (isSceneEditorActive)
		{
			commandHandled = App->CL_Editor_Scene->Context_Command_Ogre(LOWORD(wParam));
		}
		else
		{
			commandHandled = App->CL_Editor_Map->Context_Command_Ogre(LOWORD(wParam));
		}

		return commandHandled ? TRUE : FALSE;
	}

	case WM_MOUSEWHEEL:
	{
		if (App->CL_Editor_Control->flag_PreviewMode_Active == 1 &&
			App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown == 0)
		{
			int zDelta = static_cast<short>(HIWORD(wParam)); // wheel rotation

			App->CL_Ogre->Ogre3D_Listener->Wheel = (zDelta > 0) ? -1 : (zDelta < 0) ? 1 : App->CL_Ogre->Ogre3D_Listener->Wheel;

			return 1;
		}
	}

	case WM_MOUSEMOVE:
	{
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(App->ViewGLhWnd, &pos);

		if (App->CL_ImGui->flag_Imgui_Initialized == 1)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos.x = static_cast<float>(pos.x);
			io.MousePos.y = static_cast<float>(pos.y);
		}

		SetFocus(App->ViewGLhWnd);
		
		break;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_Ogre];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_3D)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_3D);
		}

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = true;

		if (App->flag_OgreStarted == 1)
		{
			if (App->flag_Block_Mouse_Buttons == false)
			{
				if (!ImGui::GetIO().WantCaptureMouse)
				{
					POINT p;
					GetCursorPos(&p);
					App->CL_Editor_Map->mStartPoint = p;

					GetCursorPos(&p);
					App->CursorPosX = p.x;
					App->CursorPosY = p.y;
					App->CL_Ogre->Ogre3D_Listener->Pl_Cent500X = p.x;
					App->CL_Ogre->Ogre3D_Listener->Pl_Cent500Y = p.y;

					SetCapture(App->ViewGLhWnd);
					SetCursorPos(App->CursorPosX, App->CursorPosY);
					App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
					App->CUR = SetCursor(NULL);

					App->CL_Camera->Camera_Save_Location();

				}
				else
				{
					//App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
				}
			}
		}

		return 1;
	}

	// Left Mouse Up
	case WM_LBUTTONUP:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = false;

		if (App->flag_OgreStarted == 1)
		{
			if (App->flag_Block_Mouse_Buttons == 0)
			{
				ReleaseCapture();
				App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
				SetCursor(App->CUR);
				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

				Ogre::Quaternion cameraRotation = App->CL_Ogre->camNode->getOrientation();

				int cameraComparison = App->CL_Maths->Ogre_Quaternion_Compare(&cameraRotation, &App->CL_Camera->Saved_Rotation, 0);

				// If Mouse has not moved select Brush and Face
				if (cameraComparison == 1)
				{
					if (App->CL_Editor_Control->flag_PreviewMode_Active == 0 && App->CL_Editor_Control->flag_Scene_Editor_Active == 0)
					{
						App->CL_Picking->Mouse_Pick_Entity(false);
					}
				}
				//App->BeepBeep();
				return 1;
			}
		}
		
		return 1;
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_Ogre];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_3D)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_3D);
		}

		if (App->flag_OgreStarted == 1)
		{
			if (App->flag_Block_Mouse_Buttons == 0)
			{
				POINT cursorPosition;
				GetCursorPos(&cursorPosition);

				App->CursorPosX = cursorPosition.x;
				App->CursorPosY = cursorPosition.y;

				auto& listener = App->CL_Ogre->Ogre3D_Listener;
				listener->Pl_Cent500X = cursorPosition.x;
				listener->Pl_Cent500Y = cursorPosition.y;

				SetCapture(App->ViewGLhWnd);
				SetCursorPos(App->CursorPosX, App->CursorPosY);

				listener->flag_RightMouseDown = 1;
				App->CUR = SetCursor(NULL);

				App->CL_Camera->Camera_Save_Location();

				return 1;
			}
		}

		return 1;
	}

	// Right Mouse Up
	case WM_RBUTTONUP:
	{
		if (App->flag_OgreStarted == 1)
		{
			if (App->flag_Block_Mouse_Buttons == 0)
			{
				ReleaseCapture();
				App->CL_Ogre->Ogre3D_Listener->flag_RightMouseDown = 0;
				SetCursor(App->CUR);

				if (GetAsyncKeyState(VK_CONTROL) < 0 && App->CL_Editor_Control->flag_PreviewMode_Active == 0)
				{
					App->CL_Picking->Mouse_Pick_Entity(true);
					int index = App->CL_TXL_Editor->GetIndex_From_FileName(App->CL_Picking->m_Texture_FileName);

					if (index > -1)
					{
						App->CL_Properties_Textures->Select_With_TextureName(App->CL_TXL_Editor->Texture_List[index]->Name);
						if (App->CL_Properties_Textures->Dialog_Textures_Visible == 0)
						{
							App->CL_Properties_Tabs->Select_Textures_Tab();
						}
					}
				}
				else
				{
					bool isSceneEditorActive = App->CL_Editor_Control->flag_Scene_Editor_Active == 1;
					bool isPreviewModeRunning = App->CL_Editor_Control->flag_PreviewMode_Active == 0;

					Ogre::Vector3 cameraPosition = App->CL_Ogre->camNode->getPosition();
					int cameraComparison = App->CL_Maths->Ogre_Vector3_Compare(&cameraPosition, &App->CL_Camera->Saved_Cam_Pos, 0);

					if (cameraComparison == 1)
					{
						App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TR];
						if (isSceneEditorActive)
						{
							App->CL_Editor_Scene->Context_Menu_Ogre(hDlg);
						}
						else if (isPreviewModeRunning)
						{
							App->CL_Editor_Map->Context_Menu_Ogre(hDlg);
						}
					}
				}

				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
				return 1;
			}
		}
	}

	}
	
	return FALSE;
}

// *************************************************************************
// *	  		Set_Selected_View:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Editor_Map::Set_Selected_View(int Selected_View)
{
	Selected_Window = Selected_View;
	RedrawWindow(Top_Left_Banner_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	RedrawWindow(Top_Right_Banner_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	RedrawWindow(Bottom_Left_Banner_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	RedrawWindow(Bottom_Ogre_Banner, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *				Set_View:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Editor_Map::Set_View()
{
	RECT		Rect;
	GetClientRect(Current_View->hDlg, &Rect);

	Current_View->XCenter = (float)Rect.right / 2;
	Current_View->YCenter = (float)Rect.bottom / 2;

	//App->CL_MapEditor->Current_View->CamPos.x = 0;
	//App->CL_MapEditor->Current_View->CamPos.y = 0;
	//App->CL_MapEditor->Current_View->CamPos.z = 0;

	//App->CL_MapEditor->Current_View->ZoomFactor = 0.3;

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
}

// *************************************************************************
// *			Context_Menu:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Editor_Map::Context_Menu(HWND hDlg)
{
	RECT rcTree;
	TVHITTESTINFO htInfo = { 0 };
	POINT pt;
	GetCursorPos(&pt);

	long xPos = pt.x;   // x position from message, in screen coordinates
	long yPos = pt.y;   // y position from message, in screen coordinates 

	GetWindowRect(hDlg, &rcTree);        // get its window coordinates
	htInfo.pt.x = xPos - rcTree.left;    // convert to client coordinates
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

	// Move Scale Rotate
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	if (App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes))
	{
		AppendMenuW(hMenu, MF_STRING, IDM_MOVE, L"&Move Brush");
		AppendMenuW(hMenu, MF_STRING, IDM_SCALE, L"&Scale Brush");
		AppendMenuW(hMenu, MF_STRING, IDM_ROTATE, L"&Rotate Brush");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_MOVE, L"&Move Brush");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCALE, L"&Scale Brush");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_ROTATE, L"&Rotate Brush");
		
	}

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	int BC = App->CL_Brush->Get_Brush_Count();
	if (BC > 0 && App->CL_Editor_Control->flag_PreviewMode_Active == 0)
	{
		AppendMenuW(hMenu, MF_STRING, IDM_PREVIEW, L"&Preview");
		AppendMenuW(hMenu, MF_STRING, IDM_SCENE_EDITOR, L"&Scene Editor");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PREVIEW, L"&Preview");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCENE_EDITOR, L"&Scene Editor");
	}

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	AppendMenuW(hMenu, MF_STRING, IDM_RESET_VIEW, L"&Reset View");
	AppendMenuW(hMenu, MF_STRING, IDM_CENTRE_ONCAMERA, L"&Centre On Camera");
	
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Zoom Ctrl+Right Mouse Button");
	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Pan Ctrl+Left Mouse Button");

	flag_Context_Menu_Active = 1;
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
	flag_Context_Menu_Active = 0;

	DestroyMenu(hMenu);
}

// *************************************************************************
// *			Context_Menu_Ogre:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Editor_Map::Context_Menu_Ogre(HWND hDlg)
{
	RECT rcTree;
	POINT pt;
	GetCursorPos(&pt);

	GetWindowRect(hDlg, &rcTree);
	long xPos = pt.x - rcTree.left; // Convert to client coordinates
	long yPos = pt.y - rcTree.top;

	HMENU hMenu = CreatePopupMenu();

	// Render Textured
	if (App->CL_Ogre->OGL_Listener->flag_Render_Ogre == 0)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_TEXTURED, L"&Textured");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_TEXTURED, L"&Textured");
	}

	// Render Groups
	if (App->CL_Ogre->OGL_Listener->flag_Render_Groups == 0)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_WIRED, L"&Wireframed");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_WIRED, L"&Wireframed");
	}

	// Environment
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	if (flag_Environment_On == false)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_ENVIRONMENT, L"&Environment");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_ENVIRONMENT, L"&Environment");
	}

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	int brushCount = App->CL_Brush->Get_Brush_Count();
	// Append Preview option
	AppendMenuW(hMenu, MF_STRING | (brushCount > 0 && App->CL_Editor_Control->flag_PreviewMode_Active == 0 ? 0 : MF_GRAYED), IDM_3D_PREVIEW, L"&Preview");

	// Append Scene Editor option
	AppendMenuW(hMenu, MF_STRING | (brushCount > 0 ? 0 : MF_GRAYED), IDM_3D_SCENE_EDITOR, L"&Scene Editor");

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Ctrl+Right Mouse Button Pick Texture ");
	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Pan Right Mouse Button");

	flag_Context_Menu_Active = 1;
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
	flag_Context_Menu_Active = 0;

	DestroyMenu(hMenu);
}

// *************************************************************************
// *		Context_Command_Ogre:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Editor_Map::Context_Command_Ogre(WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_3D_TEXTURED:
		App->CL_Camera->Camera_Textured();
		return TRUE;

	case IDM_3D_WIRED:
		App->CL_Camera->Camera_Wired();
		return TRUE;

	case IDM_3D_PREVIEW:
		App->CL_Editor_Control->Start_Preview_Mode();
		return TRUE;

	case IDM_3D_SCENE_EDITOR:
		App->CL_Editor_Control->Start_Editor_Scene();
		return TRUE;

	case IDM_3D_ENVIRONMENT:
	{
		if (flag_Environment_On == true)
		{
			flag_Environment_On = false;
			App->CL_Com_Environments->Set_Environment_By_Index(false, -1);
		}
		else
		{
			flag_Environment_On = true;
			int Index = App->CL_Com_Environments->Get_First_Environ();
			if (Index == -1)
			{

			}
			else
			{
				App->CL_Com_Environments->Set_Environment_By_Index(false, Index);
			}
		}

		return TRUE;
	}
		
	default:
		return FALSE;
	}
}

// *************************************************************************
// *			 Context_Command:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Editor_Map::Context_Command(WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_GRID_SNAP:
		App->CL_Level->flag_UseGrid = !App->CL_Level->flag_UseGrid;
		CheckMenuItem(App->Menu_Map, ID_GRID_GRIDSNAP,
			MF_BYCOMMAND | (App->CL_Level->flag_UseGrid ? MF_CHECKED : MF_UNCHECKED));
		return TRUE;

	case IDM_RESET_VIEW:
		Set_View();
		return TRUE;

	case IDM_CENTRE_ONCAMERA:
		Reset_To_Camera();
		return TRUE;

	case IDM_PREVIEW:
		App->CL_Editor_Control->Start_Preview_Mode();
		return TRUE;

	case IDM_MOVE:
		App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 1);
		return TRUE;

	case IDM_SCALE:
		App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_SCALEBRUSH, 2);
		return TRUE;

	case IDM_ROTATE:
		App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 3);
		return TRUE;

	case IDM_SCENE_EDITOR:
		App->CL_Editor_Control->Start_Editor_Scene();
		return TRUE;

	default:
		return FALSE;
	}
}

// *************************************************************************
// *	  						On_Mouse_Move							   *
// *************************************************************************
void CL64_Editor_Map::On_Mouse_Move(POINT CursorPosition, HWND hDlg)
{
	int	dx, dy;
	T_Vec3 sp, wp, dv;

	dx = (CursorPosition.x - mStartPoint.x);
	dy = (CursorPosition.y - mStartPoint.y);

	if ((dx == 0) && (dy == 0))	// don't do anything if no delta
	{
		return;
	}

	// Zoom
	if (flag_Right_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
	{
		App->CL_Render->Zoom_View(Current_View, mStartPoint.y, mStartPoint.x, CursorPosition.y);
		return;
	}

	// Pan
	if (flag_Left_Button_Down == 1 && GetAsyncKeyState(VK_CONTROL) < 0)
	{
		App->CL_Render->Pan_View(Current_View, mStartPoint.x, mStartPoint.y);
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
				
				App->CL_Doc->RotateSelectedBrushes(Current_View, &dv);

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
		
		if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH) //|| (Tool == ID_TOOLS_BRUSH_MOVESELECTEDBRUSHES))
		{
			SetEditCursor(ID_TOOLS_BRUSH_SCALEBRUSH, &CursorPosition);
		}
	}

}

// *************************************************************************
// *	  						On_Left_Button_Up						   *
// *************************************************************************
void CL64_Editor_Map::On_Left_Button_Up(POINT CursorPosition)
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
			App->CL_Level->flag_Level_is_Modified = true;
		}

		if (App->CL_Top_Tabs->flag_Brush_Rotate == 1)
		{
			App->CL_Doc->UpdateSelected();

			App->CL_Doc->DoneRotate();

			App->CL_Doc->Do_General_Select_Dlg(false);

			int Selected = App->CL_Properties_Brushes->Selected_Index;
			App->CL_Properties_Brushes->Selected_Index = Selected;
			App->CL_Properties_Brushes->OnSelchangeBrushlist(Selected, 0);

			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
			App->CL_Level->flag_Level_is_Modified = true;

			App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_MOVEROTATEBRUSH;

		}

	}

	if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
	{
		App->CL_Level->flag_Level_is_Modified = true;
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
			//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
		}
	}

	App->CUR = SetCursor(App->CUR);
}

// *************************************************************************
// *	  						On_Left_Button_Down						   *
// *************************************************************************
void CL64_Editor_Map::On_Left_Button_Down(POINT CursorPosition, HWND hDlg)
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

	GetCursorPos(&mStartPoint);
	ScreenToClient(hDlg, &mStartPoint);

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

signed int CL64_Editor_Map::fdocShowBrush(Brush const* b,Box3d const* ViewBox)
{
	return 1;// (App->CL_Brush->BrushIsVisible(b) && App->CL_Brush->Brush_TestBoundsIntersect(b, ViewBox));
}

// *************************************************************************
// *	  						BrushDraw								   *
// *************************************************************************
signed int CL64_Editor_Map::BrushDraw(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData = (BrushDrawData*)lParam;
	

	//if (App->CL_Brush->Brush_GetGroupId(pBrush) == pData->GroupId))
	{
		if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
		{
			if (App->CL_Editor_Map->fdocShowBrush(pBrush, pData->pViewBox))
			{
				App->CL_Editor_Map->Render_RenderBrushFacesOrtho(pData->v, pBrush, App->CL_Editor_Map->MemoryhDC);
			}
		}
	}

	return true;
}

#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

static signed int BrushDrawSelFacesOrtho(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData;

	pData = (BrushDrawData*)lParam;

	App->CL_Editor_Map->Render_RenderBrushSelFacesOrtho(pData->v, pBrush, pData->pDC);

	return	GE_TRUE;
}

static POINT plist[64];

// *************************************************************************
// *					Render_RenderBrushSelFacesOrtho		  			   *
// *************************************************************************
void CL64_Editor_Map::Render_RenderBrushSelFacesOrtho(ViewVars* Cam, Brush* b, HDC ViewDC)
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

// *************************************************************************
// *						Draw_Screen Terry Flanigan		  			   *
// *************************************************************************
void CL64_Editor_Map::Draw_Screen(HWND hwnd)
{
	// Exit if preview mode is active
	if (App->CL_Editor_Control->flag_PreviewMode_Active == 1)
	{
		return;
	}

	// Initialize variables
	int			inidx = 0;
	HDC RealhDC = GetDC(hwnd);
	MemoryhDC = CreateCompatibleDC(RealhDC);

	RECT		Rect;
	BrushDrawData	brushDrawData;

	// Get client rectangle and set current view dimensions
	GetClientRect(hwnd, &Rect);
	Rect.left--;
	Rect.bottom--;
	Current_View->Width = Rect.left;
	Current_View->Height = Rect.bottom;
	Current_View->XScreenScale = Rect.left;
	Current_View->YScreenScale = Rect.bottom;

	// Set up view box
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

	// Prepare brush draw data
	brushDrawData.pViewBox = &ViewBox;
	brushDrawData.pDC = MemoryhDC;
	brushDrawData.v = Current_View;
	brushDrawData.pDoc = App->CL_Doc;
	brushDrawData.GroupId = 0;
	brushDrawData.FlagTest = NULL;

	GetClipBox(RealhDC, &Rect);

	// Create off-screen bitmap
	HBITMAP OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);
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

		// Draw Template Brush
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

		// Iterate through all brushes
		int BrushCount = App->CL_Brush->Get_Brush_Count();
		int Count = 0;
		Brush* SB = nullptr;

		while (Count < BrushCount)
		{
			SB = App->CL_Brush->Get_By_Index(Count);

			switch (SB->GroupId) 
			{
			case Enums::Brushs_ID_Area:
				SelectObject(MemoryhDC, PenBrushes);
				break;

				case Enums::Brushs_ID_Evirons:
				SelectObject(MemoryhDC, PenEntity);
				break;

			default:
				break;
			}

			if (App->CL_Brush->Brush_IsSubtract(SB))
			{
				SelectObject(MemoryhDC, PenCutBrush);
			}


			if (App->CL_Brush->Brush_IsMulti(SB))
			{
				App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(SB), &brushDrawData, BrushDraw);
			}
			else
			{
				Render_RenderBrushFacesOrtho(Current_View, SB, MemoryhDC);
			}

			Count++;
		}

		bool Draw_Sel = 0;
		if (Draw_Sel == 0)
		{
			// Draw selected brushes
			SelectObject(MemoryhDC, PenSelected);
			int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			int i = 0;
			for (i = 0; i < NumSelBrushes; i++)
			{
				Brush* pBrush;

				pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
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

		// Draw selected faces
		BrushList* BList = App->CL_Level->Level_Get_Main_Brushes();
		SelectObject(MemoryhDC, PenSelectedFaces);
		App->CL_Brush->BrushList_EnumLeafBrushes(BList, &brushDrawData, BrushDrawSelFacesOrtho);


		// Draw camera if tracking
		if (App->CL_Doc->flag_Track_Camera == true)
		{
			SelectObject(MemoryhDC, Pen_Camera);
			Draw_Camera(MemoryhDC);
		}

	}

	// BitBlt to the real device context
	BitBlt(RealhDC, Rect.left, Rect.top+17, Rect.right - Rect.left, Rect.bottom - Rect.top, MemoryhDC, 0, 0, SRCCOPY);

	// Clean up
	DeleteObject(OffScreenBitmap);
	DeleteDC(MemoryhDC);
	ReleaseDC(hwnd, RealhDC);
}

// *************************************************************************
// *	  			Render_RenderBrushFacesOrtho		Genesis			   *
// *************************************************************************
void CL64_Editor_Map::Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC)
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
void CL64_Editor_Map::Draw_Camera(HDC ViewDC) 
{
	constexpr float ENTITY_SIZE = 32.0f;  // 16" across

	T_Vec3 VecOrigin{};
	T_Vec3 EntSizeWorld{};
	T_Vec3 DummyPos{ 0, 0, 0 };
	T_Vec3 OgrePos{};
	Ogre::Vector3 OgreRot{};
	T_Vec3 Cam_Angles{};

	POINT EntPosView{};
	POINT EntSizeView{};
	POINT EntWidthHeight{};
	POINT OriginView{};

	POINT TopLeft{}, BottomRight{}, TopRight{}, BottomLeft{};

	static const float COS45 = static_cast<float>(cos(M_PI / 4.0f));
	static const float SIN45 = static_cast<float>(sin(M_PI / 4.0f));
	static const float MCOS45 = static_cast<float>(cos(-M_PI / 4.0f));
	static const float MSIN45 = static_cast<float>(sin(-M_PI / 4.0f));

	// Compute entity size in view coordinates
	App->CL_Maths->Vector3_Set(&EntSizeWorld, ENTITY_SIZE, ENTITY_SIZE, ENTITY_SIZE);
	EntSizeView = App->CL_Render->Render_OrthoWorldToView(Current_View, &EntSizeWorld);
	App->CL_Maths->Vector3_Clear(&VecOrigin);
	OriginView = App->CL_Render->Render_OrthoWorldToView(Current_View, &VecOrigin);

	// Calculate width and height of the entity
	EntWidthHeight.x = std::abs(OriginView.x - EntSizeView.x);
	EntWidthHeight.y = std::abs(OriginView.y - EntSizeView.y);

	// Adjust entity size view
	EntSizeView.x -= OriginView.x;
	EntSizeView.y -= OriginView.y;

	// Entity's position in the view
	if (App->flag_OgreStarted == 1) {
		OgreRot.x = App->CL_Ogre->camNode->getOrientation().getPitch().valueRadians();
		OgreRot.y = App->CL_Ogre->camNode->getOrientation().getYaw().valueRadians();
		Cam_Angles = { M_PI - OgreRot.x, -OgreRot.y, 0 };

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
	TopLeft = { EntPosView.x - EntSizeView.x, EntPosView.y - EntSizeView.y };
	BottomRight = { EntPosView.x + EntSizeView.x, EntPosView.y + EntSizeView.y };
	TopRight = { BottomRight.x, TopLeft.y };
	BottomLeft = { TopLeft.x, BottomRight.y };

	MoveToEx(ViewDC, TopLeft.x, TopLeft.y, NULL);
	LineTo(ViewDC, BottomRight.x, BottomRight.y);
	MoveToEx(ViewDC, TopRight.x, TopRight.y, NULL);
	LineTo(ViewDC, BottomLeft.x, BottomLeft.y);

	// Prepare for drawing the direction line
	POINT ptDirSlope{};
	POINT ptRotationPoint{};
	POINT ptRelRotatePoint{};
	POINT ptPlus45{}, ptMinus45{};

	float fRadius = 100.0f;
	bool bUIAvailable = true;

	// Set camera angles
	App->CL_Maths->Vector3_Set(&Cam_Angles, Cam_Angles.z, (-Cam_Angles.y - M_PI / 2.0f), Cam_Angles.x);

	Matrix3d Xfm{};
	T_Vec3 VecTarg{};
	App->CL_Maths->XForm3d_SetEulerAngles(&Xfm, &Cam_Angles);
	App->CL_Maths->Vector3_Set(&VecTarg, fRadius, 0.0f, 0.0f);
	App->CL_Maths->XForm3d_Transform(&Xfm, &VecTarg, &VecTarg);
	App->CL_Maths->Vector3_Add(&OgrePos, &VecTarg, &VecTarg);

	POINT LineEndView = App->CL_Render->Render_OrthoWorldToView(Current_View, &VecTarg);

	// Draw to the end point
	MoveToEx(ViewDC, EntPosView.x, EntPosView.y, NULL);
	LineTo(ViewDC, LineEndView.x, LineEndView.y);
	//Ellipse(ViewDC, LineEndView.x, LineEndView.x, LineEndView.y+50, LineEndView.y+50);
	
	ptDirSlope = { LineEndView.x - EntPosView.x, LineEndView.y - EntPosView.y };
	float fDirLength = sqrt(ptDirSlope.x * ptDirSlope.x + ptDirSlope.y * ptDirSlope.y);
	float fEntityLength = sqrt(EntSizeView.x * EntSizeView.x + EntSizeView.y * EntSizeView.y) * 1; // Arrow 2x entity size
	float fPercentIntoLine = 1.0f - (fEntityLength / fDirLength);

	ptRotationPoint = { static_cast<long>(ptDirSlope.x * fPercentIntoLine + EntPosView.x),
					   static_cast<long>(ptDirSlope.y * fPercentIntoLine + EntPosView.y) };

	ptRelRotatePoint = { ptRotationPoint.x - LineEndView.x, ptRotationPoint.y - LineEndView.y };

	ptPlus45 = { static_cast<long>(ptRelRotatePoint.x * COS45 - ptRelRotatePoint.y * SIN45 + LineEndView.x),
				static_cast<long>(ptRelRotatePoint.y * COS45 + ptRelRotatePoint.x * SIN45 + LineEndView.y) };

	ptMinus45 = { static_cast<long>(ptRelRotatePoint.x * MCOS45 - ptRelRotatePoint.y * MSIN45 + LineEndView.x),
				 static_cast<long>(ptRelRotatePoint.y * MCOS45 + ptRelRotatePoint.x * MSIN45 + LineEndView.y) };

	LineTo(ViewDC, ptPlus45.x, ptPlus45.y);
	LineTo(ViewDC, ptMinus45.x, ptMinus45.y);
	LineTo(ViewDC, LineEndView.x, LineEndView.y);
	
}

// *************************************************************************
// *	  						SetEditCursor							   *
// *************************************************************************
void CL64_Editor_Map::SetEditCursor(int Tool, const POINT* pMousePos)
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
int CL64_Editor_Map::GetCursorBoxPos(const POINT* ptMousePos)
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



