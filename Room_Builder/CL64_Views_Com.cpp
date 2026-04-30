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
#include "CL64_Views_Com.h"

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
#define IDM_SCENE_MAX_VIEW 12
#define IDM_SCENE_RESTORE_VIEW 13
#define IDM_SCENE_DESELECT 14
#define IDM_SCENE_HELP 15
#define IDM_SCENE_DUPLICATE 16
#define IDM_SCENE_INFO 17
#define IDM_GOTO_PLAYER 18

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

CL64_Views_Com::CL64_Views_Com()
{
	Main_View_Dlg_Hwnd = NULL;

	GridSize = 128, 
	GridSnapSize = 8;

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
	flag_Wheel_Active = false;
	flag_Grids_Are_Visible = false;

	BackGround_Brush = CreateSolidBrush(RGB(60, 60, 60));

	Pen_Fine_Grid = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	Pen_Grid = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
	PenTemplate = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
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

	MemoryhDC = nullptr;
}

CL64_Views_Com::~CL64_Views_Com()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Views_Com::Reset_Class()
{
	Reset_Views_All();
}

// *************************************************************************
// *	  		Reset_Views_All:- Terry and Hazel Flanigan 2024	     *
// *************************************************************************
void CL64_Views_Com::Reset_Views_All()
{
	Init_Views(Enums::Selected_Map_View_None);
	Resize_Windows(Main_View_Dlg_Hwnd, nleftWnd_width, nleftWnd_Depth);

	Save_Splitter_Width_Depth();

	//------------------------ Top Left
	auto& Cam_TL = App->CL_View_Top_Left->VCam_TL;

	RECT		Rect;
	GetClientRect(Cam_TL->hDlg, &Rect);

	Cam_TL->XCenter = (float)Rect.right / 2;
	Cam_TL->YCenter = (float)Rect.bottom / 2;

	Cam_TL->CamPos.x = 0;
	Cam_TL->CamPos.y = 0;
	Cam_TL->CamPos.z = 0;

	Cam_TL->ZoomFactor = App->CL_X_Preference->Defalut_Zoom;

	//------------------------ Top Right
	auto& Cam_TR = App->CL_View_Top_Right->VCam_TR;

	GetClientRect(Cam_TR->hDlg, &Rect);

	Cam_TR->XCenter = (float)Rect.right / 2;
	Cam_TR->YCenter = (float)Rect.bottom / 2;

	Cam_TR->CamPos.x = 0;
	Cam_TR->CamPos.y = 0;
	Cam_TR->CamPos.z = 0;

	Cam_TR->ZoomFactor = App->CL_X_Preference->Defalut_Zoom;

	//------------------------ Bottom Left
	auto& Cam_BL = App->CL_View_Bottom_Left->VCam_BL;

	GetClientRect(Cam_BL->hDlg, &Rect);

	Cam_BL->XCenter = (float)Rect.right / 2;
	Cam_BL->YCenter = (float)Rect.bottom / 2;

	Cam_BL->CamPos.x = 0;
	Cam_BL->CamPos.y = 0;
	Cam_BL->CamPos.z = 0;

	Cam_BL->ZoomFactor = App->CL_X_Preference->Defalut_Zoom;

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
}

// *************************************************************************
// *	  	Reset_To_Camera:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Views_Com::Reset_To_Camera()
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
void CL64_Views_Com::Init_Views(int View)
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
void CL64_Views_Com::Resize_Windows(HWND hDlg, int newWidth, int newDepth)
{
	RECT clientRect;
	GetClientRect(hDlg, &clientRect);

	const int adjustedDepth = newDepth - 3;
	const int bannerHeight = 16;

	// Resize Top Left Window
	MoveWindow(App->CL_View_Top_Left->Top_Left_Window_Hwnd,
		0,
		0,
		clientRect.left + (newWidth - WIDTH_ADJUST),
		adjustedDepth,
		FALSE);
	MoveWindow(App->CL_View_Top_Left->Top_Left_Banner_Hwnd, 0, 0, newWidth - WIDTH_ADJUST, bannerHeight, FALSE);

	// Resize Top Right Window
	MoveWindow(App->CL_View_Top_Right->Top_Right_Window_Hwnd,
		newWidth + WIDTH_ADJUST,
		0,
		clientRect.right - (newWidth + WIDTH_ADJUST),
		adjustedDepth,
		FALSE);
	MoveWindow(App->CL_View_Top_Right->Top_Right_Banner_Hwnd, 0, 0, clientRect.right - newWidth - WIDTH_ADJUST, bannerHeight, FALSE);

	// Resize Bottom Left Window
	MoveWindow(App->CL_View_Bottom_Left->Bottom_Left_Window_Hwnd,
		0,
		newDepth,
		newWidth - WIDTH_ADJUST,
		clientRect.bottom - (newDepth + BOTTOM_POS_BOTLEFT),
		FALSE);
	MoveWindow(App->CL_View_Bottom_Left->Bottom_Left_Banner_Hwnd, 0, 0, newWidth - WIDTH_ADJUST, bannerHeight, FALSE);

	// Resize Ogre Window
	MoveWindow(App->CL_View_3D->Bottom_Right_Window_Hwnd,
		newWidth + WIDTH_ADJUST,
		newDepth,
		clientRect.right - (newWidth + WIDTH_ADJUST),
		clientRect.bottom - (newDepth + BOTTOM_POS_BOTLEFT),
		FALSE);
	MoveWindow(App->CL_View_3D->Bottom_3D_Banner, 0, 0, clientRect.right - newWidth - WIDTH_ADJUST, bannerHeight, FALSE);


	RedrawWindow(Main_View_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	ResizeOgreWindow();
}

// ************************************************************************
// *			Resize_OgreWin:- Terry and Hazel Flanigan 2025			*
// ************************************************************************
void CL64_Views_Com::ResizeOgreWindow()
{
	RECT clientRect;
	GetClientRect(App->CL_View_3D->Bottom_Right_Window_Hwnd, &clientRect);

	// Set the position and size of the window
	SetWindowPos(App->ViewGLhWnd, NULL, 0, 17, clientRect.right, clientRect.bottom - 17, SWP_NOZORDER);

	// Check if the Ogre engine has started
	if (App->flag_3D_Started == 1)
	{
		// Get the updated client rectangle
		RECT updatedRect;
		GetClientRect(App->CL_View_3D->Bottom_Right_Window_Hwnd, &updatedRect);

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
// *			Init_Map_Views:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Views_Com::Init_Map_Views()
{
	Main_View_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAPEDITOR, App->MainHwnd, (DLGPROC)Proc_Main_Dlg);

	Create_Views();
	Show_Grids(false);

	RECT rcl;
	GetClientRect(App->MainHwnd, &rcl);
	MoveWindow(Main_View_Dlg_Hwnd, 0, 50, rcl.right, rcl.bottom - 50, TRUE);
	
	Init_Views(Enums::Selected_Map_View_None);
	RedrawWindow(Main_View_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *			Proc_Main_Dlg:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
LRESULT CALLBACK CL64_Views_Com::Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static  BOOL        xSizing;
	static  BOOL        ySizing;

	static UINT			nwnd_resizing = 0;

	RECT rect;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_Views_Com->Main_View_Dlg_Hwnd = hDlg;
		
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

		if (App->flag_3D_Started == true)
		{
			FillRect(hdc, &ps.rcPaint, (HBRUSH)App->AppBackground);
		}
		else
		{
			FillRect(hdc, &ps.rcPaint, (HBRUSH)App->BlackBrush);
		}

		EndPaint(hDlg, &ps);
		return 0;
	}

	case WM_SIZE:
	{
		App->CL_Views_Com->Init_Views(Enums::Selected_Map_View_None);
		App->CL_Views_Com->Resize_Windows(hDlg, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);

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
			xSizing = (xPos > App->CL_Views_Com->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_Views_Com->nleftWnd_width + SPLITTER_BAR_WIDTH);
			ySizing = (yPos > App->CL_Views_Com->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_Views_Com->nleftWnd_Depth + SPLITTER_BAR_WIDTH);

			if (xSizing && ySizing == 0)
			{
				SetCapture(hDlg);
				if (xSizing)
				{
					SetCursor(App->CL_Views_Com->hcSizeEW);
				}

			}

			if (ySizing && xSizing == 0)
			{
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(App->CL_Views_Com->hcSizeNS);
				}

			}

			if (xSizing && ySizing)
			{
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(App->CL_Views_Com->hcBoth);
				}
			}
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		//if (App->CL_Views_Com->flag_Right_Button_Down == 0)
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
					SetRect(&focusrect, App->CL_Views_Com->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
						App->CL_Views_Com->nleftWnd_width + WIDTH_ADJUST,
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

			App->CL_Views_Com->Resize_Windows(hDlg, App->CL_Views_Com->nleftWnd_width, App->CL_Views_Com->nleftWnd_Depth);
			App->CL_Views_Com->Save_Splitter_Width_Depth();
			
		}

		return 1;
	}

	case WM_MOUSEMOVE:
	{
		//if (App->CL_Views_Com->flag_Right_Button_Down == 0)
		{
			int   xPos;
			int   yPos;

			// Get the x and y co-ordinates of the mouse
			xPos = (int)LOWORD(lParam);
			yPos = (int)HIWORD(lParam);

			if (xPos < App->CL_Views_Com->LEFT_MINIMUM_SPACE || xPos > App->CL_Views_Com->RIGHT_MINIMUM_SPACE)
			{
				return 0;
			}

			// Checks if the left button is pressed during dragging the splitter
			if (wParam == MK_LBUTTON)
			{
				if (xSizing && App->CL_Views_Com->Do_Width == 1)
				{
					RECT    focusrect;
					HDC     hdc;

					hdc = GetDC(hDlg);
					GetClientRect(hDlg, &rect);

					if (xSizing && App->CL_Views_Com->Do_Width == 1)
					{
						SetRect(&focusrect, App->CL_Views_Com->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
							App->CL_Views_Com->nleftWnd_width + WIDTH_ADJUST,
							rect.bottom - 6);

						DrawFocusRect(hdc, &focusrect);

						App->CL_Views_Com->nleftWnd_width = xPos;

						SetRect(&focusrect, App->CL_Views_Com->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
							App->CL_Views_Com->nleftWnd_width + WIDTH_ADJUST,
							rect.bottom - 6);

						DrawFocusRect(hdc, &focusrect);
					}

					ReleaseDC(hDlg, hdc);
				}

				if (ySizing && App->CL_Views_Com->Do_Depth == 1)
				{
					RECT    focusrect;
					HDC     hdc;

					hdc = GetDC(hDlg);
					GetClientRect(hDlg, &rect);

					if (ySizing && App->CL_Views_Com->Do_Depth == 1)
					{
						SetRect(&focusrect, 0, App->CL_Views_Com->nleftWnd_Depth, rect.right, App->CL_Views_Com->nleftWnd_Depth + (WIDTH_ADJUST * 2));

						DrawFocusRect(hdc, &focusrect);

						App->CL_Views_Com->nleftWnd_Depth = yPos;

						SetRect(&focusrect, 0, App->CL_Views_Com->nleftWnd_Depth, rect.right, App->CL_Views_Com->nleftWnd_Depth + (WIDTH_ADJUST * 2));

						DrawFocusRect(hdc, &focusrect);
					}

					ReleaseDC(hDlg, hdc);
				}
			}

			if ((xPos > App->CL_Views_Com->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_Views_Com->nleftWnd_width + SPLITTER_BAR_WIDTH))
			{
				if (App->CL_Views_Com->Do_All == 0)
				{
					SetCursor(App->CL_Views_Com->hcSizeEW);
				}

				App->CL_Views_Com->Do_Width = 1;
			}
			else
			{
				App->CL_Views_Com->Do_Width = 0;
			}

			if ((yPos > App->CL_Views_Com->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_Views_Com->nleftWnd_Depth + SPLITTER_BAR_WIDTH))
			{
				if (App->CL_Views_Com->Do_All == 0)
				{
					SetCursor(App->CL_Views_Com->hcSizeNS);
				}

				App->CL_Views_Com->Do_Depth = 1;
			}
			else
			{
				App->CL_Views_Com->Do_Depth = 0;
			}

			if (App->CL_Views_Com->Do_Width == 1 && App->CL_Views_Com->Do_Depth == 1)
			{
				SetCursor(App->CL_Views_Com->hcBoth);
				App->CL_Views_Com->Do_All = 1;
			}
			else
			{
				App->CL_Views_Com->Do_All = 0;
			}
		}

		return 1;
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *	  	Set_Splitter_WidthDepth:- Terry and Hazel Flanigan 2024		  *
// *************************************************************************
void CL64_Views_Com::Set_Splitter_WidthDepth(int Width, int Depth)
{
	nleftWnd_width = Width;
	nleftWnd_Depth = Depth;
}

// *************************************************************************
// *	  Save_Splitter_Width_Depth:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Views_Com::Save_Splitter_Width_Depth()
{
	Copy_Spliter_Depth = nleftWnd_Depth;
	Copy_Spliter_Width = nleftWnd_width;
}

// *************************************************************************
// *				Create_Views:- Terry Mo and Hazel 2026				   *
// *************************************************************************
void CL64_Views_Com::Create_Views()
{
	App->CL_View_Top_Left->Create_Top_Left_Window();
	App->CL_View_Top_Right->Create_Top_Right_Window();
	App->CL_View_Bottom_Left->Create_Bottom_Left_Window();
	App->CL_View_3D->Create_Ogre_Bottom_Right();
}

// *************************************************************************
// *	  		Set_Selected_View:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Views_Com::Set_Selected_View(int Selected_View)
{
	Selected_Window = Selected_View;
	RedrawWindow(App->CL_View_Top_Left->Top_Left_Banner_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	RedrawWindow(App->CL_View_Top_Right->Top_Right_Banner_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	RedrawWindow(App->CL_View_Bottom_Left->Bottom_Left_Banner_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	RedrawWindow(App->CL_View_3D->Bottom_3D_Banner, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *				Set_View:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Views_Com::Set_View()
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
void CL64_Views_Com::Context_Menu(HWND hDlg)
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
	if (App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes))
	{
		// Enabled
		AppendMenuW(hMenu, MF_STRING, IDM_MOVE, L"&Move Brush");
		AppendMenuW(hMenu, MF_STRING, IDM_SCALE, L"&Scale Brush");
		AppendMenuW(hMenu, MF_STRING, IDM_ROTATE, L"&Rotate Brush");
		AppendMenuW(hMenu, MF_STRING, IDM_SCENE_DESELECT, L"&Deselect   Esc Key");

		// Copy Functions
		AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCENE_DUPLICATE, L"&Duplicate");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
	}
	else
	{
		// Greyed
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_MOVE, L"&Move Brush");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCALE, L"&Scale Brush");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_ROTATE, L"&Rotate Brush");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCENE_DESELECT, L"&Deselect   Esc Key");

		// Copy Functions
		AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCENE_DUPLICATE, L"&Duplicate");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
		
	}

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	AppendMenuW(hMenu, MF_STRING, IDM_GOTO_PLAYER, L"Move Camera to Player");

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	AppendMenuW(hMenu, MF_STRING, IDM_RESET_VIEW, L"&Reset View");
	AppendMenuW(hMenu, MF_STRING, IDM_CENTRE_ONCAMERA, L"&Centre On Camera");
	
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Zoom Ctrl+Right Mouse Button");
	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Pan Ctrl+Left Mouse Button");

	// Info
	if (App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes))
	{
		AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenuW(hMenu, MF_STRING, IDM_SCENE_INFO, L"Brush Info");
	}
	else
	{
		AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCENE_INFO, L"Brush Info");
	}

	flag_Context_Menu_Active = 1;
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
	flag_Context_Menu_Active = 0;

	DestroyMenu(hMenu);
}

// *************************************************************************
// *			Context_Menu_Ogre:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Views_Com::Context_Menu_Ogre(HWND hDlg)
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

	//AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	int brushCount = App->CL_X_Brush->Get_Brush_Count();
	// Append Preview option
	//AppendMenuW(hMenu, MF_STRING | (brushCount > 0 && App->CL_Editor_Control->flag_PreviewMode_Active == 0 ? 0 : MF_GRAYED), IDM_3D_PREVIEW, L"&Preview");

	// Append Scene Editor option
	//AppendMenuW(hMenu, MF_STRING | (brushCount > 0 ? 0 : MF_GRAYED), IDM_3D_SCENE_EDITOR, L"&Scene Editor");

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
bool CL64_Views_Com::Context_Command_Ogre(WPARAM wParam)
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
bool CL64_Views_Com::Context_Command(WPARAM wParam)
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

	case IDM_SCENE_DESELECT:
	{
		App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();
		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
		App->CL_Ogre->OGL_Listener->Show_Visuals(false);
		return TRUE;
	}

	case IDM_SCENE_DUPLICATE:
	{
		App->CL_Brush_X->Duplicate_Brush();
		return TRUE;
	}

	case IDM_SCENE_INFO:
	{
		App->CL_Dialogs->Start_General_ListBox(Enums::ListBox_BrushInfo, App->MainHwnd);
		return TRUE;
	}
	
	case IDM_GOTO_PLAYER:
	{
		auto& m_Player = App->CL_Scene->B_Player[0];

		Ogre::Vector3 Player_Location;
		Player_Location = m_Player->StartPos;

		App->CL_Camera->Move_Camera(Player_Location);
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		return TRUE;
	}

		return FALSE;
	}
}

// *************************************************************************
// *	  						On_Mouse_Move							   *
// *************************************************************************
void CL64_Views_Com::On_Mouse_Move(POINT CursorPosition, HWND hDlg)
{

	// Pan
	if (flag_Right_Button_Down == true)
	{
		App->CL_Render->Pan_View(Current_View, mStartPoint.x, mStartPoint.y);
		return;
	}

	// Zoom
	if (flag_Right_Button_Down == true && GetAsyncKeyState(VK_CONTROL) < 0)
	{
		//App->CL_Render->Zoom_View(Current_View, mStartPoint.y, mStartPoint.x, CursorPosition.y);
		return;
	}

	if (KEYDOWN(VK_CONTROL) == true)
	{
		return;
	}

	int	dx, dy;
	T_Vec3 sp, wp, dv;

	dx = (CursorPosition.x - mStartPoint.x);
	dy = (CursorPosition.y - mStartPoint.y);

	if ((dx == 0) && (dy == 0))	// don't do anything if no delta
	{
		return;
	}

	if (flag_Left_Button_Down == true)
	{
		App->CL_Render->Render_ViewToWorld(Current_View, mStartPoint.x, mStartPoint.y, &sp);
		App->CL_Render->Render_ViewToWorld(Current_View, CursorPosition.x, CursorPosition.y, &wp);
		App->CL_X_Maths->Vector3_Subtract(&wp, &sp, &dv);

		if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			if (App->CL_Top_Tabs->flag_Brush_Move == 1)
			{
				App->CL_Doc->LockAxis(&dv);
				App->CL_Doc->MoveSelectedBrushes(&dv);
				//Draw_Screen(hDlg);

				auto& Views_Com = App->CL_Views_Com;

				switch (Views_Com->Selected_Window)
				{
				case Enums::Selected_Map_View_TL:
					App->CL_View_Top_Left->Redraw_Window_TL();
					break;
				case Enums::Selected_Map_View_TR:
					App->CL_View_Top_Right->Redraw_Window_TR();
					break;
				case Enums::Selected_Map_View_BL:
					App->CL_View_Bottom_Left->Redraw_Window_BL();
					break;
				default:
					break;
				}

			}

			if (App->CL_Top_Tabs->flag_Brush_Rotate == 1)
			{
				App->CL_Doc->LockAxis(&dv);
				App->CL_Render->Render_ViewDeltaToRotation(Current_View, (float)dx, &dv);
				
				App->CL_Doc->RotateSelectedBrushes(Current_View, &dv);
				
				auto& Views_Com = App->CL_Views_Com;

				switch (Views_Com->Selected_Window)
				{
				case Enums::Selected_Map_View_TL:
					App->CL_View_Top_Left->Redraw_Window_TL();
					break;
				case Enums::Selected_Map_View_TR:
					App->CL_View_Top_Right->Redraw_Window_TR();
					break;
				case Enums::Selected_Map_View_BL:
					App->CL_View_Bottom_Left->Redraw_Window_BL();
					break;
				default:
					break;
				}
				
			}
		}

		if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
		{
			//LockAxisView (&dx, &dy);
			App->CL_Doc->ScaleSelected(dx, dy);

			auto& Views_Com = App->CL_Views_Com;

			switch (Views_Com->Selected_Window)
			{
			case Enums::Selected_Map_View_TL:
				App->CL_View_Top_Left->Redraw_Window_TL();
				break;
			case Enums::Selected_Map_View_TR:
				App->CL_View_Top_Right->Redraw_Window_TR();
				break;
			case Enums::Selected_Map_View_BL:
				App->CL_View_Bottom_Left->Redraw_Window_BL();
				break;
			default:
				break;
			}
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
void CL64_Views_Com::On_Left_Button_Up(POINT CursorPosition)
{
	if (App->CL_Doc->mModeTool == ID_GENERALSELECT)
	{
		App->CL_Doc->SelectOrtho(CursorPosition, Current_View);
	}

	if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
	{
		if (App->CL_Top_Tabs->flag_Brush_Move == true)
		{
			App->CL_Doc->DoneMovingBrushes();
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
			App->CL_Level->flag_Level_is_Modified = true;
		}

		if (App->CL_Top_Tabs->flag_Brush_Rotate == true)
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
	}

	App->CUR = SetCursor(App->CUR);
}

// *************************************************************************
// *	  						On_Left_Button_Down						   *
// *************************************************************************
void CL64_Views_Com::On_Left_Button_Down(POINT CursorPosition, HWND hDlg)
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

			App->CL_X_Maths->Vector3_Clear(&App->CL_Doc->FinalPos);
			App->CL_Doc->TempCopySelectedBrushes();
		}

		if (App->CL_Top_Tabs->flag_Brush_Rotate == 1)
		{
			int CursorSide = 0;
			App->CL_Doc->sides = SideLookup[CursorSide];

			App->CL_X_Maths->Vector3_Clear(&App->CL_Doc->FinalPos);
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

		App->CL_X_Maths->Vector3_Set(&App->CL_Doc->FinalScale, 1.0f, 1.0f, 1.0f);
		App->CL_Doc->TempCopySelectedBrushes();
	}
}

#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

static signed int BrushDrawSelFacesOrtho(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData;

	pData = (BrushDrawData*)lParam;

	App->CL_Views_Com->Render_RenderBrushSelFacesOrtho(pData->v, pBrush, pData->pDC);

	return	GE_TRUE;
}

static POINT plist[64];



//// *************************************************************************
//// *						Draw_Screen Terry Flanigan		  			   *
//// *************************************************************************
//void CL64_Views_Com::Draw_Screen(HWND hwnd)
//{
//	return;
//	//// Exit if preview mode is active
//	//if (App->CL_Editor_Control->flag_PreviewMode_Active == 1)
//	//{
//	//	return;
//	//}
//
//	//// Initialize variables
//	//int			inidx = 0;
//	//HDC RealhDC = GetDC(hwnd);
//	//MemoryhDC = CreateCompatibleDC(RealhDC);
//
//	//RECT		Rect;
//	//BrushDrawData	brushDrawData;
//
//	//// Get client rectangle and set current view dimensions
//	//GetClientRect(hwnd, &Rect);
//	//Rect.left--;
//	//Rect.bottom--;
//	//Current_View->Width = Rect.left;
//	//Current_View->Height = Rect.bottom;
//	//Current_View->XScreenScale = Rect.left;
//	//Current_View->YScreenScale = Rect.bottom;
//
//	//// Set up view box
//	//T_Vec3 XTemp;
//	//Box3d ViewBox;
//	//inidx = App->CL_Render->Render_GetInidx(Current_View);
//	//App->CL_X_Box->Box3d_SetBogusBounds(&ViewBox);
//	//App->CL_Render->Render_ViewToWorld(Current_View, 0, 0, &XTemp);
//	//App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
//	//App->CL_Render->Render_ViewToWorld(Current_View, App->CL_Render->Render_GetWidth(Current_View), App->CL_Render->Render_GetHeight(Current_View), &XTemp);
//	//App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
//	//VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
//	//VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;
//
//	//// Prepare brush draw data
//	//brushDrawData.pViewBox = &ViewBox;
//	//brushDrawData.pDC = MemoryhDC;
//	//brushDrawData.v = Current_View;
//	//brushDrawData.pDoc = App->CL_Doc;
//	//brushDrawData.GroupId = 0;
//	//brushDrawData.FlagTest = NULL;
//
//	//GetClipBox(RealhDC, &Rect);
//
//	//// Create off-screen bitmap
//	//HBITMAP OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);
//	//SelectObject(MemoryhDC, OffScreenBitmap);
//	//FillRect(MemoryhDC, &Rect, (HBRUSH)BackGround_Brush); // BackGround
//
//	//// ---------------------- Draw Grid Fine
//	//if (Current_View->ZoomFactor > 0.1)
//	//{
//	//	SelectObject(MemoryhDC, Pen_Fine_Grid);
//	//	App->CL_Render->Render_RenderOrthoGridFromSize(Current_View, int(GridSnapSize), MemoryhDC, Rect);
//	//}
//
//	//// ---------------------- Draw Grid
//	//if (Current_View->ZoomFactor < 0.1)
//	//{
//	//	Current_View->ZoomFactor = 0.1;
//	//}
//
//	//SelectObject(MemoryhDC, Pen_Grid);
//	//App->CL_Render->Render_RenderOrthoGridFromSize(Current_View, int(GridSize), MemoryhDC, Rect);
//	//
//	//bool test = 0;
//	//if (test == 0)
//	//{
//	//	// ------------------------------------------ Draw Brushes
//	//	SelectObject(MemoryhDC, PenBrushes);
//
//	//	// Draw Template Brush
//	//	if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
//	//	{
//	//		SelectObject(MemoryhDC, PenTemplate);
//
//	//		if (App->CL_X_Brush->Brush_IsMulti(App->CL_Doc->CurBrush))
//	//		{
//
//	//			//App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(App->CL_Doc->CurBrush), &brushDrawData, BrushDraw);
//	//		}
//	//		else
//	//		{
//	//			Render_RenderBrushFacesOrtho(Current_View, App->CL_Doc->CurBrush, MemoryhDC);
//
//	//		}
//	//	}
//
//	//	// Iterate through all brushes
//	//	int BrushCount = App->CL_X_Brush->Get_Brush_Count();
//	//	int Count = 0;
//	//	Brush* SB = nullptr;
//
//	//	while (Count < BrushCount)
//	//	{
//	//		SB = App->CL_X_Brush->Get_By_Index(Count);
//
//	//		switch (SB->GroupId) 
//	//		{
//	//		case Enums::Brushs_ID_Area:
//	//			SelectObject(MemoryhDC, PenBrushes);
//	//			break;
//
//	//			case Enums::Brushs_ID_Evirons:
//	//			SelectObject(MemoryhDC, PenEntity);
//	//			break;
//
//	//		default:
//	//			break;
//	//		}
//
//	//		if (App->CL_X_Brush->Brush_IsSubtract(SB))
//	//		{
//	//			SelectObject(MemoryhDC, PenCutBrush);
//	//		}
//
//
//	//		if (App->CL_X_Brush->Brush_IsMulti(SB))
//	//		{
//	//			//App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(SB), &brushDrawData, BrushDraw);
//	//		}
//	//		else
//	//		{
//	//			Render_RenderBrushFacesOrtho(Current_View, SB, MemoryhDC);
//	//		}
//
//	//		Count++;
//	//	}
//
//	//	bool Draw_Sel = 0;
//	//	if (Draw_Sel == 0)
//	//	{
//	//		// Draw selected brushes
//	//		SelectObject(MemoryhDC, PenSelected);
//	//		int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
//
//	//		int i = 0;
//	//		for (i = 0; i < NumSelBrushes; i++)
//	//		{
//	//			Brush* pBrush;
//
//	//			pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
//	//			{
//	//				if (App->CL_X_Brush->Brush_IsMulti(pBrush))
//	//				{
//	//					//App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(pBrush), &brushDrawData, BrushDraw);
//	//				}
//	//				else
//	//				{
//	//					Render_RenderBrushFacesOrtho(Current_View, App->CL_Doc->CurBrush, MemoryhDC);
//	//				}
//	//			}
//	//		}
//	//	}
//
//	//	// Draw selected faces
//	//	BrushList* BList = App->CL_Level->Level_Get_Main_Brushes();
//	//	SelectObject(MemoryhDC, PenSelectedFaces);
//	//	App->CL_X_Brush->BrushList_EnumLeafBrushes(BList, &brushDrawData, BrushDrawSelFacesOrtho);
//
//
//	//	// Draw camera if tracking
//	//	if (App->CL_Doc->flag_Track_Camera == true)
//	//	{
//	//		SelectObject(MemoryhDC, Pen_Camera);
//	//		Draw_Camera(MemoryhDC);
//	//	}
//
//	//}
//
//	//// BitBlt to the real device context
//	//BitBlt(RealhDC, Rect.left, Rect.top+17, Rect.right - Rect.left, Rect.bottom - Rect.top, MemoryhDC, 0, 0, SRCCOPY);
//
//	//// Clean up
//	//DeleteObject(OffScreenBitmap);
//	//DeleteDC(MemoryhDC);
//	//ReleaseDC(hwnd, RealhDC);
//}

//// *************************************************************************
//// *	  			Render_RenderBrushFacesOrtho		Genesis			   *
//// *************************************************************************
//void CL64_Views_Com::Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC)
//{
//	int	i, j;
//
//	for (i = 0; i < App->CL_X_Brush->Brush_GetNumFaces(b); i++)
//	{
//		Face* f = App->CL_X_Brush->Brush_GetFace(b, i);
//		const T_Vec3* pnts = App->CL_X_Face->Face_GetPoints(f);
//
//		for (j = 0; j < App->CL_X_Face->Face_GetNumPoints(f); j++)
//		{
//			plist[j] = App->CL_Render->Render_OrthoWorldToView(Cam, &pnts[j]);
//		}
//
//		plist[j] = plist[0];
//		Polyline(ViewDC, plist, j + 1);
//	}
//}

// *************************************************************************
// *					Render_RenderBrushSelFacesOrtho		  			   *
// *************************************************************************
void CL64_Views_Com::Render_RenderBrushSelFacesOrtho(ViewVars* Cam, Brush* b, HDC ViewDC)
{
	int	i, j;

	if (!b)
		return;

	for (i = 0; i < App->CL_X_Brush->Brush_GetNumFaces(b); i++)
	{
		Face* f = App->CL_X_Brush->Brush_GetFace(b, i);
		const T_Vec3* pnts = App->CL_X_Face->Face_GetPoints(f);

		if (!App->CL_X_Face->Face_IsSelected(f))
			continue;

		for (j = 0; j < App->CL_X_Face->Face_GetNumPoints(f); j++)
		{
			plist[j] = App->CL_Render->Render_OrthoWorldToView(Cam, &pnts[j]);
		}
		plist[j] = App->CL_Render->Render_OrthoWorldToView(Cam, &pnts[0]);
		Polyline(MemoryhDC, plist, j + 1);
	}
}

// *************************************************************************
// *	  						SetEditCursor							   *
// *************************************************************************
void CL64_Views_Com::SetEditCursor(int Tool, const POINT* pMousePos)
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
int CL64_Views_Com::GetCursorBoxPos(const POINT* ptMousePos)
{
	const Box3d* pBrushBox;
	POINT ptMin, ptMax;
	int dx, dy;
	int x, y;
	int horiz, vert;
	int lookup[4] = { 1, 2, 2, 3 };

	//	Box3d BrushBox;
	//	if (pDoc->mModeTool == ID_TOOLS_TEMPLATE)
	pBrushBox = App->CL_X_Brush->Brush_GetBoundingBox(App->CL_Doc->CurBrush);
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

// *************************************************************************
// *	  		Set_3D_FullView:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Views_Com::Set_3D_FullView()
{
	App->CL_Views_Com->Init_Views(Enums::Selected_Map_View_3D);
	App->CL_Views_Com->Resize_Windows(App->CL_Views_Com->Main_View_Dlg_Hwnd, 
		nleftWnd_width, 
		nleftWnd_Depth);
}

// *************************************************************************
// *				Show_Grids:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Views_Com::Show_Grids(bool Show)
{
	if (Show == true)
	{
		ShowWindow(App->CL_View_Top_Left->Top_Left_Window_Hwnd, true);
		ShowWindow(App->CL_View_Top_Right->Top_Right_Window_Hwnd, true);
		ShowWindow(App->CL_View_Bottom_Left->Bottom_Left_Window_Hwnd, true);
		ShowWindow(App->CL_View_3D->Bottom_Right_Window_Hwnd, true);

		flag_Grids_Are_Visible = true;
	}
	else
	{
		ShowWindow(App->CL_View_Top_Left->Top_Left_Window_Hwnd, false);
		ShowWindow(App->CL_View_Top_Right->Top_Right_Window_Hwnd, false);
		ShowWindow(App->CL_View_Bottom_Left->Bottom_Left_Window_Hwnd, false);
		ShowWindow(App->CL_View_3D->Bottom_Right_Window_Hwnd, false);

		flag_Grids_Are_Visible = false;
	}
}



