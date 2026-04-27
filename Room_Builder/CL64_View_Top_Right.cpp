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
#include "CL64_View_Top_Right.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

CL64_View_Top_Right::CL64_View_Top_Right()
{
	VCam_TR = { 0 };
	m_brushDrawData_TR = { 0 };

	Top_Right_Window_Hwnd = nullptr;
	Top_Right_Banner_Hwnd = nullptr;

	Pen_Fine_Grid = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	m_Pen_Grid = CreatePen(PS_SOLID, 0, RGB(0, 112, 112));
	Pen_Camera = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));

	PenBrushes = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	m_GridSize = 128;
	m_GridSnapSize = 8;
	m_Zoom_Amount = 0.1;

	Saved_Cam_Position = { 0 };

	m_MemoryhDC_TR = nullptr;
	
}

CL64_View_Top_Right::~CL64_View_Top_Right()
{
}

// *************************************************************************
// *			Set_VCam_TR_Defaults:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_View_Top_Right::Set_VCam_TR_Defaults()
{
	strcpy(VCam_TR->Name, "Top_Right");
	VCam_TR->ViewType = TOP_RIGHT_VIEW;
	VCam_TR->ZoomFactor = 1.5;

	VCam_TR->XCenter = 310;
	VCam_TR->YCenter = 174;

	VCam_TR->XScreenScale = 0;
	VCam_TR->YScreenScale = 0;

	VCam_TR->Width = 310;
	VCam_TR->Height = 174;

	App->CL_X_Maths->Vector3_Set(&VCam_TR->CamPos, 0, 0, 0);

	VCam_TR->MaxScreenScaleInv = 100;
}

// *************************************************************************
// *			Create_Top_Right_Window:- Terry Mo and Hazel  2026		   *
// *************************************************************************
void CL64_View_Top_Right::Create_Top_Right_Window()
{
	
	VCam_TR = new ViewVars;

	Set_VCam_TR_Defaults();

	Top_Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_RIGHT, App->CL_Views_Com->Main_View_Dlg_Hwnd, (DLGPROC)Proc_Top_Right_Window);
	VCam_TR->hDlg = Top_Right_Window_Hwnd;
}

// *************************************************************************
// *		  Proc_Top_Right_Window:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_View_Top_Right::Proc_Top_Right_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TR_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_View_Top_Right->Top_Right_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_TR_TITLE);

		return TRUE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_TR_TITLE) == (HWND)lParam)
		{
			if (App->CL_Views_Com->Selected_Window == Enums::Selected_Map_View_TR)
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

		if (Views_Com->Selected_Window == Enums::Selected_Map_View_TR)
		{
			Views_Com->flag_Wheel_Active = true;

			if (Views_Com->flag_Left_Button_Down == false)
			{
				int zDelta = static_cast<short>(HIWORD(wParam)); // wheel rotation

				auto& View_Zoom = App->CL_View_Top_Right->VCam_TR->ZoomFactor;

				if (zDelta > 0)
				{
					View_Zoom += +App->CL_View_Top_Right->m_Zoom_Amount;
					App->CL_View_Top_Right->Redraw_Window_TR();
				}

				if (zDelta < 0)
				{
					View_Zoom += -App->CL_View_Top_Right->m_Zoom_Amount;
					App->CL_View_Top_Right->Redraw_Window_TR();
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

		App->CL_Views_Com->On_Mouse_Move(RealCursorPosition, hDlg);

		return 1;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Current_View = App->CL_View_Top_Right->VCam_TR;

		if (Views_Com->Selected_Window != Enums::Selected_Map_View_TR)
		{
			Views_Com->Set_Selected_View(Enums::Selected_Map_View_TR);
		}

		Views_Com->flag_Right_Button_Down = 0;
		Views_Com->flag_Left_Button_Down = true;

		Views_Com->On_Left_Button_Down(RealCursorPosition, hDlg);

		return 1;
	}

	case WM_LBUTTONUP:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Current_View = App->CL_View_Top_Right->VCam_TR;

		Views_Com->flag_Left_Button_Down = false;
		Views_Com->flag_Right_Button_Down = false;

		Views_Com->On_Left_Button_Up(RealCursorPosition);

		return 1;
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Current_View = App->CL_View_Top_Right->VCam_TR;

		if (Views_Com->Selected_Window != Enums::Selected_Map_View_TR)
		{
			Views_Com->Set_Selected_View(Enums::Selected_Map_View_TR);
		}

		GetCursorPos(&Views_Com->mStartPoint);
		ScreenToClient(hDlg, &Views_Com->mStartPoint);

		Views_Com->flag_Right_Button_Down = true;
		Views_Com->flag_Left_Button_Down = false;

		App->CUR = SetCursor(NULL);

		App->CL_View_Top_Right->Saved_Cam_Position = App->CL_View_Top_Right->VCam_TR->CamPos;

		return 1;
	}

	// Right Mouse Up
	case WM_RBUTTONUP:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->flag_Right_Button_Down = false;
		Views_Com->flag_Left_Button_Down = false;

		App->CUR = SetCursor(App->CUR);

		
		int cameraComparison = App->CL_X_Maths->Vector3_Compare(&App->CL_View_Top_Right->VCam_TR->CamPos, &App->CL_View_Top_Right->Saved_Cam_Position, 0);

		if (cameraComparison == 1)
		{
			Views_Com->Current_View = App->CL_View_Top_Right->VCam_TR;
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
			App->CL_Views_Com->Current_View = App->CL_View_Top_Right->VCam_TR;
			App->CL_View_Top_Right->Draw_Screen_TR(hDlg);
		}

		return 0;
	}

	}

	return FALSE;
}

// ---------------------------------------------------------------------------------- Stuff
#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

signed int CL64_View_Top_Right::fdocShowBrush(Brush const* b, Box3d const* ViewBox)
{
	return 1;// (App->CL_Brush->BrushIsVisible(b) && App->CL_Brush->Brush_TestBoundsIntersect(b, ViewBox));
}

// *************************************************************************
// *	  						BrushDraw								   *
// *************************************************************************
signed int CL64_View_Top_Right::Draw_Brush(Brush* pBrush, void* lParam)
{
	BrushDrawData_TL* pData = (BrushDrawData_TL*)lParam;


	//if (App->CL_Brush->Brush_GetGroupId(pBrush) == pData->GroupId))
	{
		if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
		{
			if (App->CL_View_Top_Right->fdocShowBrush(pBrush, pData->pViewBox))
			{
				App->CL_View_Top_Right->Draw_Brush_Faces_Ortho(pData->v, pBrush);
			}
		}
	}

	return true;
}

static POINT plist[64];

// *************************************************************************
// *			Redraw_Window_TR:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_View_Top_Right::Redraw_Window_TR()
{
	RedrawWindow(Top_Right_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Draw_Brush_Faces_Ortho:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_View_Top_Right::Draw_Brush_Faces_Ortho(const ViewVars* Cam, Brush* b)
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

		Polyline(m_MemoryhDC_TR, plist, j + 1);
	}
}

static signed int BrushDrawSelFacesOrtho(Brush* pBrush, void* lParam)
{
	BrushDrawData_TR* pData;

	pData = (BrushDrawData_TR*)lParam;

	App->CL_Views_Com->Render_RenderBrushSelFacesOrtho(pData->v, pBrush, pData->pDC);

	return	GE_TRUE;
}

// *************************************************************************
// *			Draw_Screen_TR:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_View_Top_Right::Draw_Screen_TR(HWND hwnd)
{
	auto& Views_Com = App->CL_Views_Com;

	// Initialize variables
	int			inidx = 0;
	HDC RealhDC = GetDC(hwnd);
	m_MemoryhDC_TR = CreateCompatibleDC(RealhDC);

	RECT		Rect;

	// Get client rectangle and set current view dimensions
	GetClientRect(hwnd, &Rect);
	Rect.left--;
	Rect.bottom--;
	VCam_TR->Width = Rect.left;
	VCam_TR->Height = Rect.bottom;
	VCam_TR->XScreenScale = Rect.left;
	VCam_TR->YScreenScale = Rect.bottom;

	// Set up view box
	T_Vec3 XTemp;
	Box3d ViewBox;
	inidx = App->CL_Render->Render_GetInidx(VCam_TR);
	App->CL_X_Box->Box3d_SetBogusBounds(&ViewBox);
	App->CL_Render->Render_ViewToWorld(VCam_TR, 0, 0, &XTemp);
	App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
	App->CL_Render->Render_ViewToWorld(VCam_TR, App->CL_Render->Render_GetWidth(VCam_TR), App->CL_Render->Render_GetHeight(VCam_TR), &XTemp);
	App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	// Prepare brush draw data
	m_brushDrawData_TR.pViewBox = &ViewBox;
	m_brushDrawData_TR.pDC = m_MemoryhDC_TR;
	m_brushDrawData_TR.v = VCam_TR;
	m_brushDrawData_TR.pDoc = App->CL_Doc;
	m_brushDrawData_TR.GroupId = 0;
	m_brushDrawData_TR.FlagTest = NULL;

	GetClipBox(RealhDC, &Rect);

	//SetDCBrushColor(m_MemoryhDC_TR, (RGB(Views_Com->Background_Colour.R, Views_Com->Background_Colour.G, Views_Com->Background_Colour.B)));

	HBITMAP OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);
	SelectObject(m_MemoryhDC_TR, OffScreenBitmap);
	FillRect(m_MemoryhDC_TR, &Rect, (HBRUSH)Views_Com->BackGround_Brush); // BackGround

	// ---------------------- Draw Grid Fine
	if (VCam_TR->ZoomFactor > 0.1)
	{
		SelectObject(m_MemoryhDC_TR, Pen_Fine_Grid);
		App->CL_Render->Render_RenderOrthoGridFromSize(VCam_TR, int(m_GridSnapSize), m_MemoryhDC_TR, Rect);
	}

	// ---------------------- Draw Grid
	if (VCam_TR->ZoomFactor < 0.1)
	{
		VCam_TR->ZoomFactor = 0.1;
	}

	SelectObject(m_MemoryhDC_TR, m_Pen_Grid);
	App->CL_Render->Render_RenderOrthoGridFromSize(VCam_TR, int(m_GridSize), m_MemoryhDC_TR, Rect);

	bool test = 0;
	if (test == 0)
	{
		// ------------------------------------------ Draw Brushes
		SelectObject(m_MemoryhDC_TR, PenBrushes);

		// Iterate through all brushes
		int BrushCount = App->CL_X_Brush->Get_Brush_Count();
		int Count = 0;
		Brush* SB = nullptr;

		while (Count < BrushCount)
		{
			SB = App->CL_X_Brush->Get_By_Index(Count);

			if (App->CL_X_Brush->Brush_IsMulti(SB))
			{
				App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(SB), &m_brushDrawData_TR, Draw_Brush);
			}
			else
			{
				Views_Com->Render_RenderBrushFacesOrtho(VCam_TR, SB, m_MemoryhDC_TR);
			}

			Count++;
		}

		bool Draw_Sel = 0;
		if (Draw_Sel == 0)
		{
			// Draw selected brushes
			SelectObject(m_MemoryhDC_TR, Views_Com->PenSelected);
			int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			int i = 0;
			for (i = 0; i < NumSelBrushes; i++)
			{
				Brush* pBrush;

				pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
				{
					if (App->CL_X_Brush->Brush_IsMulti(pBrush))
					{
						App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(pBrush), &m_brushDrawData_TR, Draw_Brush);
					}
					else
					{
						Views_Com->Render_RenderBrushFacesOrtho(VCam_TR, App->CL_Doc->CurBrush, m_MemoryhDC_TR);
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
			SelectObject(m_MemoryhDC_TR, Pen_Camera);
			Views_Com->Draw_Camera(m_MemoryhDC_TR);
		}

	}

	// BitBlt to the real device context
	BitBlt(RealhDC, Rect.left, Rect.top + 17, Rect.right - Rect.left, Rect.bottom - Rect.top, m_MemoryhDC_TR, 0, 0, SRCCOPY);

	// Clean up
	SetDCBrushColor(m_MemoryhDC_TR, (RGB(255, 255, 255)));

	DeleteObject(OffScreenBitmap);
	DeleteDC(m_MemoryhDC_TR);
	ReleaseDC(hwnd, RealhDC);

}



