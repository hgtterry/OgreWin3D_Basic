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

	ViewType = 8;
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
// *	  	Map_View_Main_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MapEditor::Map_View_Main_Dlg()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_MAPEDITOR, App->MainHwnd, (DLGPROC)Splitter_Proc);
}

// *************************************************************************
// *			Splitter_Proc:- Terry and Hazel Flanigan 2024 			   *
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
	if (m_View == 1)
	{
		ViewType = 8;
	}

	if (m_View == 2)
	{
		ViewType = 32;
	}

	if (m_View == 3)
	{
		ViewType = 16;
	}

	HDC			RealhDC;
	HDC			MemoryhDC;
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

	HPEN pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(MemoryhDC, pen);

	Draw_Grid(MemoryhDC, 8, Rect); // Snap grid


	HPEN pen2 = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
	SelectObject(MemoryhDC, pen2);

	Draw_Grid(MemoryhDC, 128, Rect); // Big grid

	/*int BrushCount = App->CL_Brush->Get_Brush_Count();*/

	HPEN pen3 = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	SelectObject(MemoryhDC, pen3);

	MeshData_Render_Faces(MemoryhDC);
	
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
// *	  			m_Render_OrthoWorldToView							   *
// *************************************************************************
POINT CL64_MapEditor::m_Render_OrthoWorldToView(Ogre::Vector3 const* wp)
{
	int vx = App->CL_MapEditor->nleftWnd_width;
	int vy = App->CL_MapEditor->nleftWnd_Depth;

	POINT	sc = { 0, 0 };
	Ogre::Vector3 ptView;
	Ogre::Vector3 Campos;

	Campos = Ogre::Vector3(-40, -40, 0);
	float ZoomFactor = 1;
	float XCenter = ((float)vx) / 2.0f - 0.5f;;
	float YCenter = ((float)vy) / 2.0f - 0.5f;

	switch (ViewType)
	{
	case VIEWTOP:
	{
		App->CL_Utilities->Vector3_Subtract(wp, &Campos, &ptView);
		App->CL_Utilities->Vector3_Scale(&ptView, ZoomFactor, &ptView);

		sc.x = (int)(XCenter + ptView.x);
		sc.y = (int)(YCenter + ptView.z);
		break;
	}
	case VIEWFRONT:
	{
		App->CL_Utilities->Vector3_Subtract(wp, &Campos, &ptView);
		App->CL_Utilities->Vector3_Scale(&ptView, ZoomFactor, &ptView);

		sc.x = (int)(XCenter + ptView.x);
		sc.y = (int)(YCenter - ptView.y);
		break;
	}
	case VIEWSIDE:
	{
		App->CL_Utilities->Vector3_Subtract(wp, &Campos, &ptView);
		App->CL_Utilities->Vector3_Scale(&ptView, ZoomFactor, &ptView);

		sc.x = (int)(XCenter + ptView.z);
		sc.y = (int)(YCenter - ptView.y);
		break;
	}
	default:
		
		break;
	}

	return sc;
}

static POINT plist[64];

// *************************************************************************
// *		MeshData_Render_Faces:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_MapEditor::MeshData_Render_Faces(HDC ViewDC)
{
	int Count = 0;

	int Map_Count = App->CL_Scene->Map_Group_Count;

	//while (Count < Map_Count)
	{
		MeshData_Face_Groups(0, ViewDC);
		//Count++;
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
