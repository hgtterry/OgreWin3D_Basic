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

#define	TOP_POS					8
#define	BOTTOM_POS				400
#define	SPLITTER_BAR_WIDTH		5
#define WIDTH_ADJUST			2

#define BOTTOM_POS_BOTLEFT		5

CL64_MapEditor::CL64_MapEditor()
{
	Spliter_Main_Hwnd = NULL;
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
	m_View = 0;
	BackGround_Brush = CreateSolidBrush(RGB(64, 64, 64));

	Pen_CutBrush = CreatePen(PS_SOLID, 0, RGB(255, 155, 0));
	Pen_Camera = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));

}

CL64_MapEditor::~CL64_MapEditor()
{
}

// *************************************************************************
// *	  	Start_Main_View_Dlg:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_MapEditor::Start_Main_View_Dlg()
{
	Start_Splitter();
}

// *************************************************************************
// *	  			Init_Views:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_MapEditor::Init_Views()
{
	RECT rect;
	GetClientRect(Spliter_Main_Hwnd, &rect);

	LEFT_WINDOW_WIDTH = rect.right / 2;
	nleftWnd_width = rect.right / 2;

	LEFT_WINDOW_DEPTH = rect.bottom / 2;
	TOP_POS_BOTLEFT = rect.bottom / 2;
	nleftWnd_Depth = LEFT_WINDOW_DEPTH;

	RIGHT_MINIMUM_SPACE = rect.right - 15;
	LEFT_MINIMUM_SPACE = rect.left + 15;

}

// *************************************************************************
// *	  			Start_Splitter:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_MapEditor::Start_Splitter()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_MAPEDITOR, App->MainHwnd, (DLGPROC)Splitter_Proc);
}

// *************************************************************************
// *			Splitter_Proc:- Terry and Hazel Flanigan 2023 			   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Splitter_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

		App->CL_MapEditor->Spliter_Main_Hwnd = hDlg;

		App->CL_MapEditor->Init_Views();
		App->CL_MapEditor->Create_Left_Window();
		App->CL_MapEditor->Create_Right_Window();
		App->CL_MapEditor->Create_Bottom_Left_Window();
		App->CL_MapEditor->Create_Bottom_Right_Window();
		App->CL_MapEditor->Resize_Windows(App->CL_MapEditor->Spliter_Main_Hwnd, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);
		
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
		RedrawWindow(App->CL_MapEditor->Spliter_Main_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		return 0;
	}


	case WM_LBUTTONDOWN:
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

		return 1;
	}

	case WM_LBUTTONUP:
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

		return 1;
	}

	case WM_MOUSEMOVE:
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


		return 1;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
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

	RedrawWindow(Spliter_Main_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return 1;
}

// *************************************************************************
// *	  	Create_Left_Window:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_MapEditor::Create_Left_Window()
{
	Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, Spliter_Main_Hwnd, (DLGPROC)Left_Window_Proc);
}

// *************************************************************************
// *			Left_Window_Proc:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Left_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
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

	case WM_PAINT:
	{
		App->CL_MapEditor->m_View = 1;
		App->CL_MapEditor->Draw_Screen(hDlg);
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  	Create_Right_Window:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_MapEditor::Create_Right_Window()
{
	Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_RIGHT, Spliter_Main_Hwnd, (DLGPROC)Right_Window_Proc);
}

// *************************************************************************
// *			Right_Window_Proc:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Right_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
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

	case WM_PAINT:
	{
		App->CL_MapEditor->m_View = 2;
		App->CL_MapEditor->Draw_Screen(hDlg);
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  Create_Bottom_Left_Window:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_MapEditor::Create_Bottom_Left_Window()
{
	Bottom_Left_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_LEFT, Spliter_Main_Hwnd, (DLGPROC)Bottom_Left_Proc);
}

// *************************************************************************
// *			Bottom_Left_Proc:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
LRESULT CALLBACK CL64_MapEditor::Bottom_Left_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
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

	case WM_PAINT:
	{
		App->CL_MapEditor->m_View = 3;
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
	Bottom_Right_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_RIGHT, Spliter_Main_Hwnd, (DLGPROC)Bottom_Right_Proc);
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

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_MapEditor->BackGround_Brush;
	}
	}

	return FALSE;
}

// *************************************************************************
// *						Draw_Screen Terry Flanigan		  			   *
// *************************************************************************
void CL64_MapEditor::Draw_Screen(HWND hwnd)
{
	//App->Get_Current_Document();

	//CMainFrame* m_pMainFrame;
	//m_pMainFrame = (CMainFrame*)AfxGetMainWnd(); // MFC POO

	//CFusionDoc* m_pDoc = (CFusionDoc*)m_pMainFrame->GetCurrentDoc();

	HDC			RealhDC;
	HDC			MemoryhDC;
	RECT		Rect;

	/*POSITION pos = m_pDoc->GetFirstViewPosition();
	m_pDoc->GetNextView(pos);
	CView* pView = m_pDoc->GetNextView(pos);
	CFusionView* pFusionView = (CFusionView*)pView;*/

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

	/*geVec3d		XTemp;
	Box3d ViewBox;
	Box3d_SetBogusBounds(&ViewBox);
	Render_ViewToWorld(pFusionView->VCam, 0, 0, &XTemp);
	Box3d_AddPoint(&ViewBox, XTemp.X, XTemp.Y, XTemp.Z);
	Render_ViewToWorld(pFusionView->VCam, Render_GetWidth(pFusionView->VCam), Render_GetHeight(pFusionView->VCam), &XTemp);
	Box3d_AddPoint(&ViewBox, XTemp.X, XTemp.Y, XTemp.Z);*/

	//VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	//VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	//BrushDrawData	brushDrawData;

	//brushDrawData.pViewBox = &ViewBox;
	//brushDrawData.pDC = MemoryhDC;
	//brushDrawData.v = pFusionView->VCam;
	//brushDrawData.pDoc = m_pDoc;//this;
	//brushDrawData.GroupId = 0;
	//brushDrawData.FlagTest = NULL;

	HBRUSH hBrush = CreateSolidBrush(RGB(64, 64, 64));
	FillRect(MemoryhDC, &Rect, (HBRUSH)hBrush); // BackGround
	DeleteObject(hBrush);

	int Center_X, Center_Y;
	int Width, Depth;

	Width = Rect.right;
	Depth = Rect.bottom;

	Center_Y = (Depth / 2);
	Center_X = (Width / 2);

	/*GridSize = Render_GetFineGrid(pFusionView->VCam, (Level_GetGridType(App->CLSB_Doc->pLevel) == GridTexel) ? GRID_TYPE_TEXEL : GRID_TYPE_METRIC);


	if (Level_GetGridType(App->CLSB_Doc->pLevel) == GridMetric)
	{
		GridSize /= 2.54f;
	}

	GridSnapSize = Level_GetGridSnapSize(App->CLSB_Doc->pLevel);*/

	HPEN pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(MemoryhDC, pen);

	Draw_Grid(MemoryhDC, 8, Rect); // Snap grid


	HPEN pen2 = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
	SelectObject(MemoryhDC, pen2);

	Draw_Grid(MemoryhDC, 128, Rect); // Big grid

	/*int BrushCount = App->CL_Brush->Get_Brush_Count();*/

	HPEN pen3 = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	SelectObject(MemoryhDC, pen3);

	//int GroupVis = Level_GetGroupVisibility(App->CLSB_Doc->pLevel);

	//GroupListType* Groups = Level_GetGroups(App->CLSB_Doc->pLevel);

	//GroupIterator	gi;
	//int				GroupId;
	//GroupId = Group_GetFirstId(Groups, &gi);

	//Brush* b;
	//BrushList* pList = Level_GetBrushes(App->CLSB_Doc->pLevel);
	//b = pList->First;

	//while (b != NULL)
	//{
	//	brushDrawData.FlagTest = ::fdocBrushNotDetail;
	//	brushDrawData.GroupId = GroupId;
	//	if ((GroupVis == Group_ShowAll) ||
	//		((GroupVis == Group_ShowCurrent) && (GroupId == App->CLSB_Doc->mCurrentGroup)) ||
	//		((GroupVis == Group_ShowVisible) && (Group_IsVisible(Groups, GroupId)))
	//		)
	//	{
	//		HPEN pen3 = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	//		SelectObject(MemoryhDC, pen3);
	//		Level_EnumLeafBrushes(App->CLSB_Doc->pLevel, &brushDrawData, m_BrushDraw); // Draw Brushes

	//		// render cut brushes
	//		SelectObject(MemoryhDC, Pen_CutBrush);
	//		brushDrawData.FlagTest = fdocBrushIsSubtract;
	//		Level_EnumLeafBrushes(App->CLSB_Doc->pLevel, &brushDrawData, m_BrushDraw);

	//	}

	//	b = b->Next;
	//	//GroupId = Group_GetNextId(Groups, &gi);
	//}


	//CEntity* pCameraEntity = App->CLSB_Camera_WE->FindCameraEntity();

	//if ((pCameraEntity != NULL))
	//{
	//	if (pCameraEntity->IsSelected())
	//	{
	//		//pDC->SelectObject(&PenSelected);
	//	}
	//	else
	//	{
	//		SelectObject(MemoryhDC, Pen_Camera);
	//	}

	//	fdocDrawEntity(pCameraEntity, pFusionView->VCam, MemoryhDC, Level_GetEntityDefs(App->CLSB_Doc->pLevel), GE_TRUE);
	//}

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
	DeleteObject(pen3);
	DeleteObject(hBrush);

	DeleteObject(OffScreenBitmap);
	DeleteDC(MemoryhDC);
}

// *************************************************************************
// *	  			Draw_Grid:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool CL64_MapEditor::Draw_Grid(HDC hDC, int Interval, RECT Rect)
{
	int cnt = Rect.bottom / Interval;

	int SP = 0;
	int Count = 0;

	// horizontal lines
	while (Count < cnt + 1)
	{
		MoveToEx(hDC, 0, SP, NULL);
		LineTo(hDC, Rect.right, SP);

		SP = SP + Interval;
		Count++;
	}

	cnt = Rect.right / Interval;
	SP = 0;
	Count = 0;
	// vertical lines
	while (Count < cnt + 1)
	{
		MoveToEx(hDC, SP, 0, NULL);
		LineTo(hDC, SP, Rect.bottom);

		SP = SP + Interval;
		Count++;
	}

	return 1;
}
