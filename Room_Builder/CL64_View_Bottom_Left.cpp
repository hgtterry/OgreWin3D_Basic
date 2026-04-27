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
#include "CL64_View_Bottom_Left.h"

CL64_View_Bottom_Left::CL64_View_Bottom_Left()
{
	VCam_BL = { 0 };
	m_brushDrawData_BL = { 0 };

	Bottom_Left_Window_Hwnd = nullptr;
	Bottom_Left_Banner_Hwnd = nullptr;

	Pen_Fine_Grid = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	m_Pen_Grid = CreatePen(PS_SOLID, 0, RGB(0, 112, 112));
	Pen_Camera = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));

	PenBrushes = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	m_GridSize = 128;
	m_GridSnapSize = 8;
	m_Zoom_Amount = 0.1;

	Saved_Cam_Position = { 0 };

	m_MemoryhDC_BL = nullptr;
}

CL64_View_Bottom_Left::~CL64_View_Bottom_Left()
{
}

// *************************************************************************
// *			Set_VCam_BL_Defaults:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_View_Bottom_Left::Set_VCam_BL_Defaults()
{
	strcpy(VCam_BL->Name, "Bottom_Left");
	VCam_BL->ViewType = BOTTOM_LEFT_VIEW;
	VCam_BL->ZoomFactor = 1.5;

	VCam_BL->XCenter = 310;
	VCam_BL->YCenter = 174;

	VCam_BL->XScreenScale = 0;
	VCam_BL->YScreenScale = 0;

	VCam_BL->Width = 310;
	VCam_BL->Height = 174;

	App->CL_X_Maths->Vector3_Set(&VCam_BL->CamPos, 0, 0, 0);

	VCam_BL->MaxScreenScaleInv = 100;
}

// *************************************************************************
// *			Create_Bottom_Left_Window:- Terry Mo and Hazel  2026	   *
// *************************************************************************
void CL64_View_Bottom_Left::Create_Bottom_Left_Window()
{

	VCam_BL = new ViewVars;

	Set_VCam_BL_Defaults();

	Bottom_Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_LEFT, App->CL_Views_Com->Main_View_Dlg_Hwnd, (DLGPROC)Proc_Bottom_Left_Window);
	VCam_BL->hDlg = Bottom_Left_Window_Hwnd;
}

// *************************************************************************
// *		 Proc_Bottom_Left_Window:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
LRESULT CALLBACK CL64_View_Bottom_Left::Proc_Bottom_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_BL_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_View_Bottom_Left->Bottom_Left_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_BL_TITLE);

		return TRUE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BL_TITLE) == (HWND)lParam)
		{
			if (App->CL_Views_Com->Selected_Window == Enums::Selected_Map_View_BL)
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

		if (Views_Com->Selected_Window == Enums::Selected_Map_View_BL)
		{
			Views_Com->flag_Wheel_Active = true;

			if (Views_Com->flag_Left_Button_Down == false)
			{
				int zDelta = static_cast<short>(HIWORD(wParam)); // wheel rotation

				auto& View_Zoom = App->CL_View_Bottom_Left->VCam_BL->ZoomFactor;

				if (zDelta > 0)
				{
					View_Zoom += +App->CL_View_Bottom_Left->m_Zoom_Amount;
					App->CL_View_Bottom_Left->Redraw_Window_BL();
				}

				if (zDelta < 0)
				{
					View_Zoom += -App->CL_View_Bottom_Left->m_Zoom_Amount;
					App->CL_View_Bottom_Left->Redraw_Window_BL();
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

		Views_Com->Current_View = App->CL_View_Bottom_Left->VCam_BL;

		if (Views_Com->Selected_Window != Enums::Selected_Map_View_BL)
		{
			Views_Com->Set_Selected_View(Enums::Selected_Map_View_BL);
		}

		Views_Com->flag_Right_Button_Down = false;
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

		Views_Com->Current_View = App->CL_View_Bottom_Left->VCam_BL;

		Views_Com->flag_Left_Button_Down = false;
		Views_Com->flag_Right_Button_Down = false;

		Views_Com->On_Left_Button_Up(RealCursorPosition);

		return 1;
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->Current_View = App->CL_View_Bottom_Left->VCam_BL;

		if (Views_Com->Selected_Window != Enums::Selected_Map_View_BL)
		{
			Views_Com->Set_Selected_View(Enums::Selected_Map_View_BL);
		}

		GetCursorPos(&Views_Com->mStartPoint);
		ScreenToClient(hDlg, &Views_Com->mStartPoint);

		Views_Com->flag_Right_Button_Down = true;
		Views_Com->flag_Left_Button_Down = false;

		App->CUR = SetCursor(NULL);

		App->CL_View_Bottom_Left->Saved_Cam_Position = App->CL_View_Bottom_Left->VCam_BL->CamPos;

		return 1;
	}

	// Right Mouse Up
	case WM_RBUTTONUP:
	{
		auto& Views_Com = App->CL_Views_Com;

		Views_Com->flag_Right_Button_Down = false;
		Views_Com->flag_Left_Button_Down = false;

		App->CUR = SetCursor(App->CUR);

		int cameraComparison = App->CL_X_Maths->Vector3_Compare(&App->CL_View_Bottom_Left->VCam_BL->CamPos, &App->CL_View_Bottom_Left->Saved_Cam_Position, 0);

		if (cameraComparison == 1)
		{
			Views_Com->Current_View = App->CL_View_Bottom_Left->VCam_BL;
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
			App->CL_Views_Com->Current_View = App->CL_View_Bottom_Left->VCam_BL;
			App->CL_View_Bottom_Left->Draw_Screen_BL(hDlg);
		}

		return 0;
	}

	}

	return FALSE;
}

// ---------------------------------------------------------------------------------- Stuff
#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

signed int CL64_View_Bottom_Left::fdocShowBrush(Brush const* b, Box3d const* ViewBox)
{
	return 1;// (App->CL_Brush->BrushIsVisible(b) && App->CL_Brush->Brush_TestBoundsIntersect(b, ViewBox));
}

// *************************************************************************
// *	  						BrushDraw								   *
// *************************************************************************
signed int CL64_View_Bottom_Left::Draw_Brush(Brush* pBrush, void* lParam)
{
	BrushDrawData_TL* pData = (BrushDrawData_TL*)lParam;


	//if (App->CL_Brush->Brush_GetGroupId(pBrush) == pData->GroupId))
	{
		if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
		{
			if (App->CL_View_Bottom_Left->fdocShowBrush(pBrush, pData->pViewBox))
			{
				App->CL_View_Bottom_Left->Draw_Brush_Faces_Ortho(pData->v, pBrush);
			}
		}
	}

	return true;
}

static POINT plist[64];

// *************************************************************************
// *			Redraw_Window_BL:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_View_Bottom_Left::Redraw_Window_BL()
{
	RedrawWindow(Bottom_Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Draw_Brush_Faces_Ortho:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_View_Bottom_Left::Draw_Brush_Faces_Ortho(const ViewVars* Cam, Brush* b)
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

		Polyline(m_MemoryhDC_BL, plist, j + 1);
	}
}

static signed int BrushDrawSelFacesOrtho(Brush* pBrush, void* lParam)
{
	BrushDrawData_BL* pData;

	pData = (BrushDrawData_BL*)lParam;

	App->CL_Views_Com->Render_RenderBrushSelFacesOrtho(pData->v, pBrush, pData->pDC);

	return	GE_TRUE;
}

// *************************************************************************
// *			Draw_Screen_BL:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_View_Bottom_Left::Draw_Screen_BL(HWND hwnd)
{
	// Exit if preview mode is active
	if (App->CL_Editor_Control->flag_PreviewMode_Active == 1)
	{
		return;
	}

	// Initialize variables
	int			inidx = 0;
	HDC RealhDC = GetDC(hwnd);
	m_MemoryhDC_BL = CreateCompatibleDC(RealhDC);

	RECT		Rect;

	// Get client rectangle and set current view dimensions
	GetClientRect(hwnd, &Rect);
	Rect.left--;
	Rect.bottom--;
	VCam_BL->Width = Rect.left;
	VCam_BL->Height = Rect.bottom;
	VCam_BL->XScreenScale = Rect.left;
	VCam_BL->YScreenScale = Rect.bottom;

	// Set up view box
	T_Vec3 XTemp;
	Box3d ViewBox;
	inidx = App->CL_Render->Render_GetInidx(VCam_BL);
	App->CL_X_Box->Box3d_SetBogusBounds(&ViewBox);
	App->CL_Render->Render_ViewToWorld(VCam_BL, 0, 0, &XTemp);
	App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
	App->CL_Render->Render_ViewToWorld(VCam_BL, App->CL_Render->Render_GetWidth(VCam_BL), App->CL_Render->Render_GetHeight(VCam_BL), &XTemp);
	App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	// Prepare brush draw data
	m_brushDrawData_BL.pViewBox = &ViewBox;
	m_brushDrawData_BL.pDC = m_MemoryhDC_BL;
	m_brushDrawData_BL.v = VCam_BL;
	m_brushDrawData_BL.pDoc = App->CL_Doc;
	m_brushDrawData_BL.GroupId = 0;
	m_brushDrawData_BL.FlagTest = NULL;

	GetClipBox(RealhDC, &Rect);

	auto& Views_Com = App->CL_Views_Com;

	//SetDCBrushColor(m_MemoryhDC_BL, (RGB(Views_Com->Background_Colour.R, Views_Com->Background_Colour.G, Views_Com->Background_Colour.B)));


	HBITMAP OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);
	SelectObject(m_MemoryhDC_BL, OffScreenBitmap);
	FillRect(m_MemoryhDC_BL, &Rect, (HBRUSH)Views_Com->BackGround_Brush); // BackGround

	// ---------------------- Draw Grid Fine
	if (VCam_BL->ZoomFactor > 0.1)
	{
		SelectObject(m_MemoryhDC_BL,Pen_Fine_Grid);
		App->CL_Render->Render_RenderOrthoGridFromSize(VCam_BL, int(m_GridSnapSize), m_MemoryhDC_BL, Rect);
	}

	// ---------------------- Draw Grid
	if (VCam_BL->ZoomFactor < 0.1)
	{
		VCam_BL->ZoomFactor = 0.1;
	}

	SelectObject(m_MemoryhDC_BL, m_Pen_Grid);
	App->CL_Render->Render_RenderOrthoGridFromSize(VCam_BL, int(m_GridSize), m_MemoryhDC_BL, Rect);

	bool test = 0;
	if (test == 0)
	{
		auto& Views_Com = App->CL_Views_Com;

		// ------------------------------------------ Draw Brushes
		SelectObject(m_MemoryhDC_BL, PenBrushes);

		// Draw Template Brush
		if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
		{
			SelectObject(m_MemoryhDC_BL, Views_Com->PenTemplate);

			if (App->CL_X_Brush->Brush_IsMulti(App->CL_Doc->CurBrush))
			{

				App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(App->CL_Doc->CurBrush), &m_brushDrawData_BL, Draw_Brush);
			}
			else
			{
				Views_Com->Render_RenderBrushFacesOrtho(Views_Com->Current_View, App->CL_Doc->CurBrush, m_MemoryhDC_BL);

			}
		}

		// Iterate through all brushes
		int BrushCount = App->CL_X_Brush->Get_Brush_Count();
		int Count = 0;
		Brush* SB = nullptr;

		while (Count < BrushCount)
		{
			SB = App->CL_X_Brush->Get_By_Index(Count);

			switch (SB->GroupId)
			{
			case Enums::Brushs_ID_Area:
				SelectObject(m_MemoryhDC_BL, PenBrushes);
				break;

			case Enums::Brushs_ID_Evirons:
				SelectObject(m_MemoryhDC_BL, Views_Com->PenEntity);
				break;

			default:
				break;
			}

			if (App->CL_X_Brush->Brush_IsSubtract(SB))
			{
				SelectObject(m_MemoryhDC_BL, Views_Com->PenCutBrush);
			}

			if (App->CL_X_Brush->Brush_IsMulti(SB))
			{
				SelectObject(m_MemoryhDC_BL, PenBrushes);
				App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(SB), &m_brushDrawData_BL, Draw_Brush);
			}
			else
			{
				Views_Com->Render_RenderBrushFacesOrtho(VCam_BL, SB, m_MemoryhDC_BL);
			}

			Count++;
		}

		bool Draw_Sel = 0;
		if (Draw_Sel == 0)
		{
			// Draw selected brushes
			SelectObject(m_MemoryhDC_BL, Views_Com->PenSelected);
			int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			int i = 0;
			for (i = 0; i < NumSelBrushes; i++)
			{
				Brush* pBrush;

				pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
				{
					if (App->CL_X_Brush->Brush_IsMulti(pBrush))
					{
						App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(pBrush), &m_brushDrawData_BL, Draw_Brush);
					}
					else
					{
						Views_Com->Render_RenderBrushFacesOrtho(VCam_BL, App->CL_Doc->CurBrush, m_MemoryhDC_BL);
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
			SelectObject(m_MemoryhDC_BL, Pen_Camera);
			Views_Com->Draw_Camera(m_MemoryhDC_BL);
		}

	}

	// BitBlt to the real device context
	BitBlt(RealhDC, Rect.left, Rect.top + 17, Rect.right - Rect.left, Rect.bottom - Rect.top, m_MemoryhDC_BL, 0, 0, SRCCOPY);

	// Clean up
	SetDCBrushColor(m_MemoryhDC_BL, (RGB(255, 255, 255)));

	DeleteObject(OffScreenBitmap);
	DeleteDC(m_MemoryhDC_BL);
	ReleaseDC(hwnd, RealhDC);

}
