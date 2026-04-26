/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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

#define IDM_3D_WIRED 20
#define IDM_3D_TEXTURED 21
#define IDM_3D_PREVIEW 22
#define IDM_3D_SCENE_EDITOR 23
#define IDM_3D_ENVIRONMENT 24
#define IDM_3D_RESET_CAMERA 25
#define IDM_3D_HELP 26
#define IDM_3D_BOUNDING_BOX 27
#define IDM_3D_NORMALS 28
#define IDM_3D_CAMERASPEED 29
#define IDM_3D_POINTS 30
#define IDM_3D_BONES 31

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

	flag_Left_Button_Down = false;
	flag_Right_Button_Down = false;
	flag_Context_Menu_Active = false;
	flag_Environment_On = true;
	flag_Wheel_Active = false;

	Stock_Brush = (HBRUSH)::GetStockObject(DC_BRUSH);
	BackGround_Brush = CreateSolidBrush(RGB(60, 60, 60));

	PenTemplate = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	PenSelected = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	Pen_Camera = CreatePen(PS_SOLID, 0, RGB(0, 255, 255));
	PenSelectedFaces = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	PenCutBrush = CreatePen(PS_SOLID, 1, RGB(255, 155, 0));
	PenEntity = CreatePen(PS_SOLID, 1, RGB(180, 180, 255));
	
	Background_Colour = { 60,60,60 };

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
// *				Reset_Class:- Terry Mo and Hazel 2025			 	   *
// *************************************************************************
void CL64_Views_Com::Reset_Class()
{
	Reset_Views_All();
}

// *************************************************************************
// *	  		Reset_Views_All:- Terry Mo and Hazel 2026				   *
// *************************************************************************
void CL64_Views_Com::Reset_Views_All()
{
	Init_Views(Enums::Selected_Map_View_None);
	Resize_Windows(Main_View_Dlg_Hwnd, nleftWnd_width, nleftWnd_Depth);

	Save_Splitter_Width_Depth();
	
	auto& Cam_TL = App->CL_View_Top_Left->VCam_TL;

	RECT		Rect;
	GetClientRect(Cam_TL->hDlg, &Rect);
	
	Cam_TL->XCenter = (float)Rect.right / 2;
	Cam_TL->YCenter = (float)Rect.bottom / 2;

	Cam_TL->CamPos.x = 0;
	Cam_TL->CamPos.y = 0;
	Cam_TL->CamPos.z = 0;

	Cam_TL->ZoomFactor = App->CL_Libs->CL_Preference->Defalut_Zoom;

	auto& Cam_TR = App->CL_View_Top_Right->VCam_TR;

	GetClientRect(Cam_TR->hDlg, &Rect);

	Cam_TR->XCenter = (float)Rect.right / 2;
	Cam_TR->YCenter = (float)Rect.bottom / 2;

	Cam_TR->CamPos.x = 0;
	Cam_TR->CamPos.y = 0;
	Cam_TR->CamPos.z = 0;

	Cam_TR->ZoomFactor = App->CL_Libs->CL_Preference->Defalut_Zoom;

	
	auto& Cam_BL = App->CL_View_Bottom_Left->VCam_BL;

	GetClientRect(Cam_BL->hDlg, &Rect);

	Cam_BL->XCenter = (float)Rect.right / 2;
	Cam_BL->YCenter = (float)Rect.bottom / 2;

	Cam_BL->CamPos.x = 0;
	Cam_BL->CamPos.y = 0;
	Cam_BL->CamPos.z = 0;

	Cam_BL->ZoomFactor = App->CL_Libs->CL_Preference->Defalut_Zoom;

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);


}

// *************************************************************************
// *			Reset_To_Camera:- Terry Mo and Hazel 2024				   *
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
// *	  			Init_Views:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Views_Com::Init_Views(int View)
{
	RECT rect;
	GetClientRect(Main_View_Dlg_Hwnd, &rect);

	if (View == Enums::Selected_Map_View_None)
	{
		/*if (App->flag_Start_3DEditor_Mode == true)
		{
			LEFT_WINDOW_WIDTH = 0;
			nleftWnd_width = 0;

			LEFT_WINDOW_DEPTH = 0;
			TOP_POS_BOTLEFT = 0;

		}
		else*/
		{
			LEFT_WINDOW_WIDTH = rect.right / 2;
			nleftWnd_width = rect.right / 2;

			LEFT_WINDOW_DEPTH = rect.bottom / 2;
			TOP_POS_BOTLEFT = rect.bottom / 2;
		}
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
// *			Resize_Windows:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Views_Com::Resize_Windows(HWND hDlg, int newWidth, int newDepth)
{
	RECT clientRect;
	GetClientRect(hDlg, &clientRect);

	const int adjustedDepth = newDepth - 3;
	const int bannerHeight = 16;

	//                                                     Resize Top Left Window
	MoveWindow(App->CL_View_Top_Left->Top_Left_Window_Hwnd,
		0,
		0,
		clientRect.left + (newWidth - WIDTH_ADJUST),
		adjustedDepth,
		FALSE);
	MoveWindow(App->CL_View_Top_Left->Top_Left_Banner_Hwnd, 0, 0, newWidth - WIDTH_ADJUST, bannerHeight, FALSE);



	//                                                     Resize Top Right Window
	MoveWindow(App->CL_View_Top_Right->Top_Right_Window_Hwnd,
		newWidth + WIDTH_ADJUST,
		0,
		clientRect.right - (newWidth + WIDTH_ADJUST),
		adjustedDepth,
		FALSE);
	MoveWindow(App->CL_View_Top_Right->Top_Right_Banner_Hwnd, 0, 0, clientRect.right - newWidth - WIDTH_ADJUST, bannerHeight, FALSE);



	//                                                      Resize Bottom Left Window
	MoveWindow(App->CL_View_Bottom_Left->Bottom_Left_Window_Hwnd,
		0,
		newDepth,
		newWidth - WIDTH_ADJUST,
		clientRect.bottom - (newDepth + BOTTOM_POS_BOTLEFT),
		FALSE);
	MoveWindow(App->CL_View_Bottom_Left->Bottom_Left_Banner_Hwnd, 0, 0, newWidth - WIDTH_ADJUST, bannerHeight, FALSE);



	//                                                                     Resize Ogre Window
	MoveWindow(App->CL_View_3D->Bottom_Right_Window_Hwnd,
		newWidth + WIDTH_ADJUST,
		newDepth,
		clientRect.right - (newWidth + WIDTH_ADJUST),
		clientRect.bottom - (newDepth + BOTTOM_POS_BOTLEFT),
		FALSE);
	MoveWindow(App->CL_View_3D->Bottom_3D_Banner, 0, 0, clientRect.right - newWidth - WIDTH_ADJUST, bannerHeight, FALSE);


	RedrawWindow(Main_View_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	ResizeOgreWindow();
	//App->CL_3D_TL_View->ResizeOgreWindow_TL();
}

// ************************************************************************
// *			Resize_OgreWin:- Terry Mo and Hazel 2025				  *
// ************************************************************************
void CL64_Views_Com::ResizeOgreWindow()
{
	RECT clientRect;
	GetClientRect(App->CL_View_3D->Bottom_Right_Window_Hwnd, &clientRect);

	// Set the position and size of the window
	SetWindowPos(App->CL_View_3D->RenderWin3D_hWnd, NULL, 0, 17, clientRect.right, clientRect.bottom - 17, SWP_NOZORDER);

	// Check if the 3D engine has started
	if (App->flag_3D_Started == true)
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
// *			Init_Map_Views:- Terry Mo and Hazel 2026				   *
// *************************************************************************
void CL64_Views_Com::Init_Map_Views()
{
	Main_View_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAPEDITOR, App->MainHwnd, (DLGPROC)Proc_Main_Dlg);

	Create_Views();

	App->CL_Interface->Show_Grids(false);

	RECT rcl;
	GetClientRect(App->MainHwnd, &rcl);
	MoveWindow(Main_View_Dlg_Hwnd, 0, 75, rcl.right, rcl.bottom - 75, TRUE);
	
	Init_Views(Enums::Selected_Map_View_None);
	RedrawWindow(Main_View_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *			Proc_Main_Dlg:- Terry Mo and Hazel 2025		 			   *
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
		//if (App->CL_Ogre->Listener_3D->flag_LeftMouseDown == 0)
		//{
		//	int zDelta = (short)HIWORD(wParam);    // wheel rotation

		//	if (zDelta > 0)
		//	{
		//		App->CL_Ogre->Listener_3D->Wheel = -1;
		//	}
		//	else if (zDelta < 0)
		//	{
		//		App->CL_Ogre->Listener_3D->Wheel = 1;
		//	}

		//	return 1;
		//}
	}

	case WM_LBUTTONDOWN:
	{
		auto& Views_Com = App->CL_Views_Com;
		//if (App->CL_MapEditor->flag_Right_Button_Down == 0)
		{
			int                 xPos;
			int                 yPos;

			// Varible used to get the mouse cursor x and y co-ordinates
			xPos = (int)LOWORD(lParam);
			yPos = (int)HIWORD(lParam);

			// Checks whether the mouse is over the splitter window
			xSizing = (xPos > Views_Com->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < Views_Com->nleftWnd_width + SPLITTER_BAR_WIDTH);
			ySizing = (yPos > Views_Com->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < Views_Com->nleftWnd_Depth + SPLITTER_BAR_WIDTH);

			if (xSizing && ySizing == 0)
			{
				SetCapture(hDlg);
				if (xSizing)
				{
					SetCursor(Views_Com->hcSizeEW);
				}

			}

			if (ySizing && xSizing == 0)
			{
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(Views_Com->hcSizeNS);
				}

			}

			if (xSizing && ySizing)
			{
				SetCapture(hDlg);
				if (ySizing)
				{
					SetCursor(Views_Com->hcBoth);
				}
			}
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		auto& Views_Com = App->CL_Views_Com;
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
					SetRect(&focusrect, Views_Com->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
						Views_Com->nleftWnd_width + WIDTH_ADJUST,
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

			Views_Com->Resize_Windows(hDlg, Views_Com->nleftWnd_width, Views_Com->nleftWnd_Depth);
			Views_Com->Save_Splitter_Width_Depth();
			
		}

		return 1;
	}

	case WM_MOUSEMOVE:
	{
		auto& Views_Com = App->CL_Views_Com;
		//if (App->CL_Editor_Map->flag_Right_Button_Down == 0)
		{
			int   xPos;
			int   yPos;

			// Get the x and y co-ordinates of the mouse
			xPos = (int)LOWORD(lParam);
			yPos = (int)HIWORD(lParam);

			if (xPos < Views_Com->LEFT_MINIMUM_SPACE || xPos > Views_Com->RIGHT_MINIMUM_SPACE)
			{
				return 0;
			}

			// Checks if the left button is pressed during dragging the splitter
			if (wParam == MK_LBUTTON)
			{
				if (xSizing && Views_Com->Do_Width == 1)
				{
					RECT    focusrect;
					HDC     hdc;

					hdc = GetDC(hDlg);
					GetClientRect(hDlg, &rect);

					if (xSizing && Views_Com->Do_Width == 1)
					{
						SetRect(&focusrect, Views_Com->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
							Views_Com->nleftWnd_width + WIDTH_ADJUST,
							rect.bottom - 6);

						DrawFocusRect(hdc, &focusrect);

						Views_Com->nleftWnd_width = xPos;

						SetRect(&focusrect, Views_Com->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
							Views_Com->nleftWnd_width + WIDTH_ADJUST,
							rect.bottom - 6);

						DrawFocusRect(hdc, &focusrect);
					}

					ReleaseDC(hDlg, hdc);
				}

				if (ySizing && Views_Com->Do_Depth == 1)
				{
					RECT    focusrect;
					HDC     hdc;

					hdc = GetDC(hDlg);
					GetClientRect(hDlg, &rect);

					if (ySizing && Views_Com->Do_Depth == 1)
					{
						SetRect(&focusrect, 0, Views_Com->nleftWnd_Depth, rect.right, Views_Com->nleftWnd_Depth + (WIDTH_ADJUST * 2));

						DrawFocusRect(hdc, &focusrect);

						Views_Com->nleftWnd_Depth = yPos;

						SetRect(&focusrect, 0, Views_Com->nleftWnd_Depth, rect.right, Views_Com->nleftWnd_Depth + (WIDTH_ADJUST * 2));

						DrawFocusRect(hdc, &focusrect);
					}

					ReleaseDC(hDlg, hdc);
				}
			}

			if ((xPos > Views_Com->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < Views_Com->nleftWnd_width + SPLITTER_BAR_WIDTH))
			{
				if (Views_Com->Do_All == 0)
				{
					SetCursor(Views_Com->hcSizeEW);
				}

				Views_Com->Do_Width = 1;
			}
			else
			{
				Views_Com->Do_Width = 0;
			}

			if ((yPos > Views_Com->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < Views_Com->nleftWnd_Depth + SPLITTER_BAR_WIDTH))
			{
				if (Views_Com->Do_All == 0)
				{
					SetCursor(Views_Com->hcSizeNS);
				}

				Views_Com->Do_Depth = 1;
			}
			else
			{
				Views_Com->Do_Depth = 0;
			}

			if (Views_Com->Do_Width == 1 && Views_Com->Do_Depth == 1)
			{
				SetCursor(Views_Com->hcBoth);
				Views_Com->Do_All = 1;
			}
			else
			{
				Views_Com->Do_All = 0;
			}
		}

		return 1;
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *	  	Set_Splitter_WidthDepth:- Terry Mo and Hazel 2025			  *
// *************************************************************************
void CL64_Views_Com::Set_Splitter_WidthDepth(int Width, int Depth)
{
	nleftWnd_width = Width;
	nleftWnd_Depth = Depth;
}

// *************************************************************************
// *	  Save_Splitter_Width_Depth:- Terry Mo and Hazel 2025			   *
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
// *	  		Set_Selected_View:- Terry Mo and Hazel 2025				   *
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
// *				Set_View:- Terry Mo and Hazel 2025				 	   *
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
// *			Context_Menu_Ogre:- Terry Mo and Hazel 2025			 	   *
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
	/*if (App->CL_Ogre->OGL_Listener->flag_Render_Ogre == 0)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_TEXTURED, L"&Textured");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_TEXTURED, L"&Textured");
	}

	 Render Groups
	if (App->CL_Ogre->OGL_Listener->flag_ShowFaces == false)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_WIRED, L"&Faces");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_WIRED, L"&Faces");
	}

	 Render Points
	if (App->CL_Ogre->OGL_Listener->flag_ShowPoints == false)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_POINTS, L"&Vertice Points");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_POINTS, L"&Vertice Points");
	}

	 Render Bones Skeleton
	if (App->CL_Ogre->OGL_Listener->flag_ShowBones == false)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_BONES, L"&Bones Skeleton");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_BONES, L"&Bones Skeleton");
	}

	 Bounding Box
	if (App->CL_Ogre->OGL_Listener->flag_ShowBoundingBox == false)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_BOUNDING_BOX, L"&Bounding Box");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_BOUNDING_BOX, L"&Bounding Box");
	}

	 Normals
	if (App->CL_Ogre->OGL_Listener->flag_ShowNormals == false)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_NORMALS, L"&Normals");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_NORMALS, L"&Normals");
	}*/

	AppendMenuW(hMenu, MF_STRING , IDM_3D_RESET_CAMERA, L"&Reset View");
	
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_3D_CAMERASPEED, L"&Quick Options");

	// Panel View
	/*AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_SCENE_MAX_VIEW, L"&Full View");
	AppendMenuW(hMenu, MF_STRING, IDM_SCENE_RESTORE_VIEW, L"&All Views");*/


	/*AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	if (App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes))
	{
		AppendMenuW(hMenu, MF_STRING, IDM_SCENE_DESELECT, L"&Deselect");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING, IDM_SCENE_DESELECT, L"&Deselect");
	}*/

	// Environment
	/*AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	if (flag_Environment_On == false)
	{
		AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_3D_ENVIRONMENT, L"&Environment");
	}
	else
	{
		AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_3D_ENVIRONMENT, L"&Environment");
	}*/

	//int brushCount = App->CL_X_Brush->Get_Brush_Count();
	//// Append Preview option
	//AppendMenuW(hMenu, MF_STRING | (brushCount > 0 && App->CL_Editor_Control->flag_PreviewMode_Active == 0 ? 0 : MF_GRAYED), IDM_3D_PREVIEW, L"&Preview");

	//// Append Scene Editor option
	//AppendMenuW(hMenu, MF_STRING | (brushCount > 0 ? 0 : MF_GRAYED), IDM_3D_SCENE_EDITOR, L"&Scene Editor");

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Ctrl+Right Mouse Button Pick Texture ");
	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Pan Right Mouse Button");

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_3D_HELP, L"&Help");

	flag_Context_Menu_Active = 1;
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
	flag_Context_Menu_Active = 0;

	DestroyMenu(hMenu);
}

// *************************************************************************
// *		Context_Command_Ogre:- Terry Mo and Hazel 2024		   *
// *************************************************************************
bool CL64_Views_Com::Context_Command_Ogre(WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_3D_TEXTURED:
	{
		App->CL_Camera->Camera_Textured();
		return TRUE;
	}

	case IDM_3D_POINTS:
	{
		App->CL_Mesh->Show_Mesh_Points();
		return TRUE;
	}
	
	case IDM_3D_WIRED:
	{
		App->CL_Mesh->Show_Mesh_Faces();
		return TRUE;
	}

	case IDM_3D_BONES:
	{
		App->CL_Mesh->Show_Mesh_Bones();
		return TRUE;
	}

	case IDM_3D_NORMALS:
	{
		App->CL_Mesh->Show_Mesh_Normals();
		return TRUE;
	}
	
	case IDM_3D_BOUNDING_BOX:
	{
		App->CL_Mesh->Show_Mesh_BoundBox();
		return TRUE;
	}

	case IDM_3D_CAMERASPEED:
	{
		App->CL_Libs->CL_Preference->Start_Quick_Options_Dlg();
		return TRUE;
	}

	case IDM_3D_RESET_CAMERA:
	{
		//if (App->CL_X_Brush->Get_Brush_Count() > 0)
		{
			App->CL_Dialogs->YesNo("Reset View", "");
			if (App->CL_Dialogs->flag_Dlg_Canceled == false)
			{
				App->CL_Camera->Reset_View_and_Zoom();
				//App->CL_Camera->Camera_Reset_Zero();
			}
		}
		return TRUE;
	}
		
	case IDM_SCENE_MAX_VIEW:
	{
		auto& Views_Com = App->CL_Views_Com;

		if (Views_Com->Selected_Window == Enums::Selected_Map_View_3D)
		{
			Views_Com->Init_Views(Enums::Selected_Map_View_3D);
			Views_Com->Resize_Windows(Views_Com->Main_View_Dlg_Hwnd, Views_Com->nleftWnd_width, Views_Com->nleftWnd_Depth);
			App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_3D);
		}

		return TRUE;
	}

	case IDM_SCENE_RESTORE_VIEW:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Set_Splitter_WidthDepth(Views_Com->Copy_Spliter_Width, Views_Com->Copy_Spliter_Depth);
		Views_Com->Resize_Windows(Views_Com->Main_View_Dlg_Hwnd, Views_Com->nleftWnd_width, Views_Com->nleftWnd_Depth);
		App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_None);
		return TRUE;
	}

	//case IDM_3D_PREVIEW:
	//	//App->CL_Editor_Control->Start_Preview_Mode();
	//	return TRUE;

	//case IDM_3D_SCENE_EDITOR:
	//	App->CL_Editor_Control->Start_Editor_Scene();
	//	return TRUE;

	case IDM_3D_ENVIRONMENT:
	{
		/*if (flag_Environment_On == true)
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
		}*/

		return TRUE;
	}

	case IDM_SCENE_DESELECT:
	{
		App->CL_Interface->Deselect_All_Brushes_Update_Dlgs();
		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
		App->CL_Ogre->OGL_Listener->Show_Visuals(false);
		return TRUE;
	}
		
	case IDM_3D_HELP:
	{
		App->Open_HTML((LPSTR)"Help\\ContextMenu_3D.html");
		return TRUE;
	}

	default:
		return FALSE;
	}
}

// *************************************************************************
// *			Context_Menu:- Terry Mo and Hazel 2025				 	   *
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

	//if (App->CL_Level->flag_UseGrid == true)
	//{
	//	AppendMenuW(hMenu, MF_STRING | MF_CHECKED, IDM_GRID_SNAP, L"&Grid Snap");
	//}
	//else
	//{
	//	AppendMenuW(hMenu, MF_STRING | MF_UNCHECKED, IDM_GRID_SNAP, L"&Grid Snap");
	//}

	//// Move Scale Rotate
	//AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	//if (App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes))
	//{
	//	// Enabled
	//	AppendMenuW(hMenu, MF_STRING , IDM_MOVE, L"&Move");
	//	AppendMenuW(hMenu, MF_STRING , IDM_SCALE, L"&Scale");
	//	AppendMenuW(hMenu, MF_STRING , IDM_ROTATE, L"&Rotate");
	//	AppendMenuW(hMenu, MF_STRING, IDM_SCENE_DESELECT, L"&Deselect");

	//	// Copy Functions
	//	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	//	AppendMenuW(hMenu, MF_STRING, IDM_SCENE_DUPLICATE, L"&Duplicate");
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
	//}
	//else
	//{
	//	// Greyed
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_MOVE, L"&Move");
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCALE, L"&Scale");
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_ROTATE, L"&Rotate");
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCENE_DESELECT, L"&Deselect");

	//	// Copy Functions
	//	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_SCENE_DUPLICATE, L"&Duplicate");
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
	//	AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
	//}

	// Panel View
	//AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_SCENE_MAX_VIEW, L"&Full View");
	AppendMenuW(hMenu, MF_STRING, IDM_SCENE_RESTORE_VIEW, L"&All Views");

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	//AppendMenuW(hMenu, MF_STRING, IDM_RESET_VIEW, L"&Reset View");
	AppendMenuW(hMenu, MF_STRING, IDM_CENTRE_ONCAMERA, L"&Centre On Camera");
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

	//
	//AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	//AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Zoom Ctrl and Mouse Wheel");
	//AppendMenuW(hMenu, MF_STRING | MF_GRAYED, NULL, L"&Pan Ctrl+Left Mouse Button");

	//AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	//AppendMenuW(hMenu, MF_STRING, IDM_SCENE_HELP, L"&Help");

	flag_Context_Menu_Active = 1;
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
	flag_Context_Menu_Active = 0;

	DestroyMenu(hMenu);
}

// *************************************************************************
// *			 Context_Command:- Terry Mo and Hazel 2025				   *
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

	case IDM_SCENE_MAX_VIEW:
	{
		auto& Views_Com = App->CL_Views_Com;

		if (Views_Com->Selected_Window == Enums::Selected_Map_View_TL)
		{
			Views_Com->Init_Views(Enums::Selected_Map_View_TL);
			Views_Com->Resize_Windows(Views_Com->Main_View_Dlg_Hwnd, Views_Com->nleftWnd_width, Views_Com->nleftWnd_Depth);
			App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_TL);
		}

		if (Views_Com->Selected_Window == Enums::Selected_Map_View_TR)
		{
			Views_Com->Init_Views(Enums::Selected_Map_View_TR);
			Views_Com->Resize_Windows(Views_Com->Main_View_Dlg_Hwnd, Views_Com->nleftWnd_width, Views_Com->nleftWnd_Depth);
			App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_TR);
		}

		if (Views_Com->Selected_Window == Enums::Selected_Map_View_BL)
		{
			Views_Com->Init_Views(Enums::Selected_Map_View_BL);
			Views_Com->Resize_Windows(Views_Com->Main_View_Dlg_Hwnd, Views_Com->nleftWnd_width, Views_Com->nleftWnd_Depth);
			App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_BL);
		}
		return TRUE;
	}

	case IDM_SCENE_RESTORE_VIEW:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Set_Splitter_WidthDepth(Views_Com->Copy_Spliter_Width, Views_Com->Copy_Spliter_Depth);
		Views_Com->Resize_Windows(Views_Com->Main_View_Dlg_Hwnd, Views_Com->nleftWnd_width, Views_Com->nleftWnd_Depth);
		App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_None);
		return TRUE;
	}
		
	case IDM_RESET_VIEW:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Set_Splitter_WidthDepth(Views_Com->Copy_Spliter_Width, Views_Com->Copy_Spliter_Depth);
		Views_Com->Resize_Windows(Views_Com->Main_View_Dlg_Hwnd, Views_Com->nleftWnd_width, Views_Com->nleftWnd_Depth);
		App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_None);
		return TRUE;
	}

	case IDM_CENTRE_ONCAMERA:
	{
		auto& Views_Com = App->CL_Views_Com;
		ViewVars* Temp_VCam = nullptr;
		RECT		Rect;

		// Determine the selected view and assign the corresponding camera
		switch (Views_Com->Selected_Window)
		{
		case Enums::Selected_Map_View_TL:
			Temp_VCam = App->CL_View_Top_Left->VCam_TL;
			break;
		case Enums::Selected_Map_View_TR:
			Temp_VCam = App->CL_View_Top_Right->VCam_TR;
			break;
		case Enums::Selected_Map_View_BL:
			Temp_VCam = App->CL_View_Bottom_Left->VCam_BL;
			break;
		default:
			break;
		}

		if (Temp_VCam)
		{
			GetClientRect(Temp_VCam->hDlg, &Rect);

			Temp_VCam->XCenter = static_cast<float>(Rect.right) / 2;
			Temp_VCam->YCenter = static_cast<float>(Rect.bottom) / 2;

			Ogre::Vector3 Pos;
			if (App->CL_Model->flag_Model_Loaded == true)
			{
				Pos = App->CL_Ogre->camNode->getPosition();
			}
			else
			{
				Pos = Ogre::Vector3( 0, 0, 0 );
			}

			Temp_VCam->CamPos.x = Pos.x;
			Temp_VCam->CamPos.y = Pos.y;
			Temp_VCam->CamPos.z = Pos.z;

			// Redraw the appropriate window based on the selected view
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
		

		return TRUE;
	}

	//case IDM_PREVIEW:
	//	//App->CL_Editor_Control->Start_Preview_Mode();
	//	return TRUE;

	case IDM_MOVE:
		App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 1);
		return TRUE;

	case IDM_SCALE:
		App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_SCALEBRUSH, 2);
		return TRUE;

	case IDM_ROTATE:
		App->CL_Top_Tabs->Set_Brush_Mode(ID_TOOLS_BRUSH_MOVEROTATEBRUSH, 3);
		return TRUE;

	case IDM_SCENE_DESELECT:
	{
		App->CL_Interface->Deselect_All_Brushes_Update_Dlgs();
		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
		App->CL_Ogre->OGL_Listener->Show_Visuals(false);
		return TRUE;
	}

	case IDM_SCENE_DUPLICATE:
	{
		App->CL_Sandbox->Duplicate_Brush();

		return TRUE;
	}
		
	case IDM_SCENE_EDITOR:
	{
		App->CL_Editor_Control->Start_Editor_Scene();
		return TRUE;
	}

	case IDM_SCENE_HELP:
	{
		App->Open_HTML((LPSTR)"Help\\ContextMenu_Maps.html");
		return TRUE;
	}
		
	default:
		return FALSE;
	}
}



// *************************************************************************
// *	  			On_Mouse_Move:- Terry Mo and Hazel 2025				   *
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

	if (flag_Left_Button_Down == true && KEYDOWN(VK_CONTROL) == false)
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
			}

			if (App->CL_Top_Tabs->flag_Brush_Rotate == 1)
			{
				App->CL_Doc->LockAxis(&dv);
				App->CL_Render->Render_ViewDeltaToRotation(Current_View, (float)dx, &dv);
				
				App->CL_Doc->RotateSelectedBrushes(Current_View, &dv);

				//Draw_Screen(hDlg);
			}
		}

		if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
		{
			//LockAxisView (&dx, &dy);
			App->CL_Doc->ScaleSelected(dx, dy);

			//Draw_Screen(hDlg);
		}

		if (KEYDOWN(VK_CONTROL) == false)
		{
			POINT pt = mStartPoint;	// The position works on the delta mStartPoint...
			ClientToScreen(hDlg, &pt);
			SetCursorPos(pt.x, pt.y);
		}
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
// *	  			Render_RenderBrushFacesOrtho		Genesis			   *
// *************************************************************************
void CL64_Views_Com::Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC)
{
	int	i, j;

	for (i = 0; i < App->CL_X_Brush->Brush_GetNumFaces(b); i++)
	{
		Face* f = App->CL_X_Brush->Brush_GetFace(b, i);
		const T_Vec3* pnts = App->CL_X_Face->Face_GetPoints(f);

		for (j = 0; j < App->CL_X_Face->Face_GetNumPoints(f); j++)
		{
			plist[j] = App->CL_Render->Render_OrthoWorldToView(Cam, &pnts[j]);
		}

		plist[j] = plist[0];
		Polyline(ViewDC, plist, j + 1);
	}
}

// *************************************************************************
// *			Draw_Camera:- Terry Mo and Hazel 2024	 		   *
// *************************************************************************
void CL64_Views_Com::Draw_Camera(HDC ViewDC)
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
	App->CL_X_Maths->Vector3_Set(&EntSizeWorld, ENTITY_SIZE, ENTITY_SIZE, ENTITY_SIZE);
	EntSizeView = App->CL_Render->Render_OrthoWorldToView(Current_View, &EntSizeWorld);
	App->CL_X_Maths->Vector3_Clear(&VecOrigin);
	OriginView = App->CL_Render->Render_OrthoWorldToView(Current_View, &VecOrigin);

	// Calculate width and height of the entity
	EntWidthHeight.x = std::abs(OriginView.x - EntSizeView.x);
	EntWidthHeight.y = std::abs(OriginView.y - EntSizeView.y);

	// Adjust entity size view
	EntSizeView.x -= OriginView.x;
	EntSizeView.y -= OriginView.y;

	// Entity's position in the view
	if (App->flag_3D_Started == true)
	{
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
	App->CL_X_Maths->Vector3_Set(&Cam_Angles, Cam_Angles.z, (-Cam_Angles.y - M_PI / 2.0f), Cam_Angles.x);

	Matrix3d Xfm{};
	T_Vec3 VecTarg{};
	App->CL_X_Maths->XForm3d_SetEulerAngles(&Xfm, &Cam_Angles);
	App->CL_X_Maths->Vector3_Set(&VecTarg, fRadius, 0.0f, 0.0f);
	App->CL_X_Maths->XForm3d_Transform(&Xfm, &VecTarg, &VecTarg);
	App->CL_X_Maths->Vector3_Add(&OgrePos, &VecTarg, &VecTarg);

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
// *	  		Set_3D_FullView:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Views_Com::Set_3D_FullView()
{
	auto& Views_Com = App->CL_Views_Com;

	Views_Com->Init_Views(Enums::Selected_Map_View_3D);
	Views_Com->Resize_Windows(Views_Com->Main_View_Dlg_Hwnd, 
		nleftWnd_width, 
		nleftWnd_Depth);
}



