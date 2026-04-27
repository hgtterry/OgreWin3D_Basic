/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
#include "CL64_View_Top_Left.h"

//#include "CL64_Lib_Maths.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

CL64_View_Top_Left::CL64_View_Top_Left()
{
	VCam_TL = { 0 };
	m_brushDrawData_TL = { 0 };

	Top_Left_Window_Hwnd = nullptr;
	Top_Left_Banner_Hwnd = nullptr;

	Pen_Fine_Grid = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	m_Pen_Grid = CreatePen(PS_SOLID, 0, RGB(0, 112, 112));
	Pen_Camera = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));

	PenBrushes = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	m_GridSize = 128;
	m_GridSnapSize = 8;
	m_Zoom_Amount = 0.1;

	Saved_Cam_Position = { 0 };

	m_MemoryhDC = nullptr;
}

CL64_View_Top_Left::~CL64_View_Top_Left()
{
}

//enum ViewTypes
//{
//	VIEWSOLID = 1,
//	VIEWTEXTURE = 2,
//	VIEWWIRE = 4,
//	TOP_LEFT_VIEW = 8,
//	BOTTOM_LEFT_VIEW = 16,
//	TOP_RIGHT_VIEW = 32,
//	VIEWOGRE = 64
//};

// *************************************************************************
// *			Set_VCam_TL_Defaults:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_View_Top_Left::Set_VCam_TL_Defaults()
{
	int TOP_LEFT_VIEW = 8;

	strcpy(VCam_TL->Name, "Top_Left");
	VCam_TL->ViewType = TOP_LEFT_VIEW;
	VCam_TL->ZoomFactor = 1.5;

	VCam_TL->XCenter = 310;
	VCam_TL->YCenter = 174;

	VCam_TL->XScreenScale = 0;
	VCam_TL->YScreenScale = 0;

	VCam_TL->Width = 310;
	VCam_TL->Height = 174;

	App->CL_X_Maths->Vector3_Set(&VCam_TL->CamPos, 0, 0, 0);

	VCam_TL->MaxScreenScaleInv = 100;
}

// *************************************************************************
// *		Create_Top_Left_Window:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_View_Top_Left::Create_Top_Left_Window()
{
	VCam_TL = new ViewVars;
	
	Set_VCam_TL_Defaults();

	Top_Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, App->CL_Views_Com->Main_View_Dlg_Hwnd, (DLGPROC)Proc_Top_Left_Window);

	VCam_TL->hDlg = Top_Left_Window_Hwnd;

}

// *************************************************************************
// *		  Proc_Top_Left_Window:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_View_Top_Left::Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TL_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_View_Top_Left->Top_Left_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_TL_TITLE);

		return TRUE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_TL_TITLE) == (HWND)lParam)
		{
			if (App->CL_Views_Com->Selected_Window == Enums::Selected_Map_View_TL)
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
				return (UINT)App->Brush_But_Test;
			}
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		return CDRF_DODEFAULT;
	}

	case WM_SETCURSOR:
	{
		auto& Views_Com = App->CL_Views_Com;

		if (Views_Com->flag_Context_Menu_Active == true)
		{
			return false;
		}

		if (Views_Com->flag_Right_Button_Down == true || Views_Com->flag_Left_Button_Down == true)
		{
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			SetCursor(Views_Com->hcBoth);
			return true;
		}
		else if (App->CL_Doc->mModeTool == ID_TOOLS_BRUSH_SCALEBRUSH)
		{
			return true;
		}
		else
		{
			return false;
		}

		return false;
	}

	case WM_MOUSEWHEEL:
	{
		auto& Views_Com = App->CL_Views_Com;
		
		if (Views_Com->flag_Left_Button_Down == true)
		{
			return 1;
		}

		if (Views_Com->Selected_Window == Enums::Selected_Map_View_TL)
		{
			Views_Com->flag_Wheel_Active = true;

			if (Views_Com->flag_Left_Button_Down == false)
			{
				int zDelta = static_cast<short>(HIWORD(wParam)); // wheel rotation

				auto& View_Zoom = App->CL_View_Top_Left->VCam_TL->ZoomFactor;

				if (zDelta > 0)
				{
					View_Zoom += +App->CL_View_Top_Left->m_Zoom_Amount;
					App->CL_View_Top_Left->Redraw_Window_TL();
				}

				if (zDelta < 0)
				{
					View_Zoom += -App->CL_View_Top_Left->m_Zoom_Amount;
					App->CL_View_Top_Left->Redraw_Window_TL();
				}

			}

			return 1;
		}

		Views_Com->flag_Wheel_Active = false;
		
		return 1;
	}

	case WM_MOUSEMOVE:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		auto& Views_Com = App->CL_Views_Com;

		Views_Com->On_Mouse_Move(RealCursorPosition,hDlg);

		return 1;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Current_View = App->CL_View_Top_Left->VCam_TL;

		if (Views_Com->Selected_Window != Enums::Selected_Map_View_TL)
		{
			Views_Com->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Current_View = App->CL_View_Top_Left->VCam_TL;

		Views_Com->flag_Left_Button_Down = false;
		Views_Com->flag_Right_Button_Down = false;

		Views_Com->On_Left_Button_Up(RealCursorPosition);

		return 1;
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Current_View = App->CL_View_Top_Left->VCam_TL;

		if (Views_Com->Selected_Window != Enums::Selected_Map_View_TL)
		{
			Views_Com->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		GetCursorPos(&Views_Com->mStartPoint);
		ScreenToClient(hDlg, &Views_Com->mStartPoint);

		Views_Com->flag_Right_Button_Down = true;
		Views_Com->flag_Left_Button_Down = false;

		App->CUR = SetCursor(NULL);
		
		App->CL_View_Top_Left->Saved_Cam_Position = App->CL_View_Top_Left->VCam_TL->CamPos;
		
		return 1;
	}

	// Right Mouse Up
	case WM_RBUTTONUP:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->flag_Right_Button_Down = false;
		Views_Com->flag_Left_Button_Down = false;

		App->CUR = SetCursor(App->CUR);
		
		
		int cameraComparison = App->CL_X_Maths->Vector3_Compare(&App->CL_View_Top_Left->VCam_TL->CamPos, &App->CL_View_Top_Left->Saved_Cam_Position, 0);
		
		if (cameraComparison == 1)
		{
			Views_Com->Current_View = App->CL_View_Top_Left->VCam_TL;
			Views_Com->Context_Menu(hDlg);
		}

		return 1;
	}

	case WM_COMMAND:
	{
		if (App->CL_Views_Com->Context_Command(LOWORD(wParam)))
		{
			return TRUE;
		}

		break;
	}

	case WM_PAINT:
	{
		if (App->flag_3D_Started == true)
		{
			App->CL_Views_Com->Current_View = App->CL_View_Top_Left->VCam_TL;
			App->CL_View_Top_Left->Draw_Screen_TL(hDlg);
		}

		return 0;
	}

	}

	return FALSE;
}


// ---------------------------------------------------------------------------------- Stuff
#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

signed int CL64_View_Top_Left::fdocShowBrush(Brush const* b, Box3d const* ViewBox)
{
	return 1;// (App->CL_Brush->BrushIsVisible(b) && App->CL_Brush->Brush_TestBoundsIntersect(b, ViewBox));
}

// *************************************************************************
// *	  						BrushDraw								   *
// *************************************************************************
signed int CL64_View_Top_Left::Draw_Brush_2D(Brush* pBrush, void* lParam)
{
	BrushDrawData_TL* pData = (BrushDrawData_TL*)lParam;


	//if (App->CL_Brush->Brush_GetGroupId(pBrush) == pData->GroupId))
	{
		if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
		{
			if (App->CL_View_Top_Left->fdocShowBrush(pBrush, pData->pViewBox))
			{
				App->CL_View_Top_Left->Draw_Brush_Faces_2D(pData->v, pBrush);
			}
		}
	}

	return true;
}

static POINT plist[64];

// *************************************************************************
// *			Redraw_Window_TL:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_View_Top_Left::Redraw_Window_TL()
{
	RedrawWindow(Top_Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Draw_Brush_Faces_2D:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_View_Top_Left::Draw_Brush_Faces_2D(const ViewVars* Cam, Brush* b)
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

		Polyline(m_MemoryhDC, plist, j + 1);
	}
}

static signed int BrushDrawSelFacesOrtho(Brush* pBrush, void* lParam)
{
	BrushDrawData_TL* pData;

	pData = (BrushDrawData_TL*)lParam;

	App->CL_Views_Com->Render_RenderBrushSelFacesOrtho(pData->v, pBrush, pData->pDC);

	return	GE_TRUE;
}

// *************************************************************************
// *			Draw_Screen_TL:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_View_Top_Left::Draw_Screen_TL(HWND hwnd)
{
	// Initialize variables

	auto& Views_Com = App->CL_Views_Com;

	int			inidx = 0;
	HDC RealhDC = GetDC(hwnd);
	m_MemoryhDC = CreateCompatibleDC(RealhDC);

	RECT		Rect;
	
	// Get client rectangle and set current view dimensions
	GetClientRect(hwnd, &Rect);
	Rect.left--;
	Rect.bottom--;
	VCam_TL->Width = Rect.left;
	VCam_TL->Height = Rect.bottom;
	VCam_TL->XScreenScale = Rect.left;
	VCam_TL->YScreenScale = Rect.bottom;

	// Set up view box
	T_Vec3 XTemp;
	Box3d ViewBox;
	inidx = App->CL_Render->Render_GetInidx(VCam_TL);
	App->CL_X_Box->Box3d_SetBogusBounds(&ViewBox);
	App->CL_Render->Render_ViewToWorld(VCam_TL, 0, 0, &XTemp);
	App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
	App->CL_Render->Render_ViewToWorld(VCam_TL, App->CL_Render->Render_GetWidth(VCam_TL), App->CL_Render->Render_GetHeight(VCam_TL), &XTemp);
	App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	// Prepare brush draw data
	m_brushDrawData_TL.pViewBox = &ViewBox;
	m_brushDrawData_TL.pDC = m_MemoryhDC;
	m_brushDrawData_TL.v = VCam_TL;
	m_brushDrawData_TL.pDoc = App->CL_Doc;
	m_brushDrawData_TL.GroupId = 0;
	m_brushDrawData_TL.FlagTest = NULL;

	GetClipBox(RealhDC, &Rect);

	//SetDCBrushColor(m_MemoryhDC, (RGB(Views_Com->Background_Colour.R, Views_Com->Background_Colour.G, Views_Com->Background_Colour.B)));

	HBITMAP OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);
	SelectObject(m_MemoryhDC, OffScreenBitmap);
	//FillRect(m_MemoryhDC, &Rect, (HBRUSH)Views_Com->Stock_Brush); // BackGround

	// ---------------------- Draw Grid Fine
	if (VCam_TL->ZoomFactor > 0.1)
	{
		SelectObject(m_MemoryhDC, Pen_Fine_Grid);
		App->CL_Render->Render_RenderOrthoGridFromSize(VCam_TL, int(m_GridSnapSize), m_MemoryhDC, Rect);
	}

	// ---------------------- Draw Grid
	if (VCam_TL->ZoomFactor < 0.1)
	{
		VCam_TL->ZoomFactor = 0.1;
	}

	SelectObject(m_MemoryhDC, m_Pen_Grid);
	App->CL_Render->Render_RenderOrthoGridFromSize(VCam_TL, int(m_GridSize), m_MemoryhDC, Rect);

	bool test = 0;
	if (test == 0)
	{
		// ------------------------------------------ Draw Brushes
		SelectObject(m_MemoryhDC, PenBrushes);

		// Iterate through all brushes
		int BrushCount = App->CL_X_Brush->Get_Brush_Count();
		int Count = 0;
		Brush* SB = nullptr;

		while (Count < BrushCount)
		{
			SB = App->CL_X_Brush->Get_By_Index(Count);

			if (App->CL_X_Brush->Brush_IsMulti(SB))
			{
				App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(SB), &m_brushDrawData_TL, Draw_Brush_2D);
			}
			else
			{
				Views_Com->Render_RenderBrushFacesOrtho(VCam_TL, SB, m_MemoryhDC);
			}

			Count++;
		}

		bool Draw_Sel = 0;
		if (Draw_Sel == 0)
		{
			// Draw selected brushes
			SelectObject(m_MemoryhDC, Views_Com->PenSelected);
			int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			int i = 0;
			for (i = 0; i < NumSelBrushes; i++)
			{
				Brush* pBrush;

				pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
				{
					if (App->CL_X_Brush->Brush_IsMulti(pBrush))
					{
						App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(pBrush), &m_brushDrawData_TL, Draw_Brush_2D);
					}
					else
					{
						Views_Com->Render_RenderBrushFacesOrtho(VCam_TL, App->CL_Doc->CurBrush, m_MemoryhDC);
					}
				}
			}
		}

		//// Draw selected faces
		//BrushList* BList = App->CL_Level->Level_Get_Main_Brushes();
		//SelectObject(m_MemoryhDC, Views_Com->PenSelectedFaces);
		//App->CL_X_Brush->BrushList_EnumLeafBrushes(BList, &brushDrawData, BrushDrawSelFacesOrtho);


		// Draw camera if tracking
		if (App->CL_Doc->flag_Track_Camera == true)
		{
			SelectObject(m_MemoryhDC, Pen_Camera);
			Views_Com->Draw_Camera(m_MemoryhDC);
		}

	}

	// BitBlt to the real device context
	BitBlt(RealhDC, Rect.left, Rect.top + 17, Rect.right - Rect.left, Rect.bottom - Rect.top, m_MemoryhDC, 0, 0, SRCCOPY);
	
	// Clean up
	SetDCBrushColor(m_MemoryhDC, (RGB(255, 255, 255)));

	DeleteObject(OffScreenBitmap);
	DeleteDC(m_MemoryhDC);
	ReleaseDC(hwnd, RealhDC);

}

