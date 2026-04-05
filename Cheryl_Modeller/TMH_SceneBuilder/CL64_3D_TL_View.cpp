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
#include "CL64_3D_TL_View.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

CL64_3D_TL_View::CL64_3D_TL_View()
{
	Render_hWnd = nullptr;
	m_V_TL = 0;
	m_Pen_Grid = CreatePen(PS_SOLID, 0, RGB(0, 112, 112));
	m_MemoryhDC = nullptr;
}

CL64_3D_TL_View::~CL64_3D_TL_View()
{
}

// *************************************************************************
// *			Create_Top_Left_Window:- Terry Mo and Hazel  2026		   *
// *************************************************************************
void CL64_3D_TL_View::Create_Top_Left_Window()
{
	App->CL_Editor_Map->Top_Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, App->CL_Editor_Map->Main_View_Dlg_Hwnd, (DLGPROC)Proc_Top_Left_Window);

}

// *************************************************************************
// *		  Proc_Top_Left_Window:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_3D_TL_View::Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TL_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_Editor_Map->Top_Left_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_TL_TITLE);

		App->CL_3D_TL_View->m_Pen_Grid = CreatePen(PS_SOLID, 0, RGB(0, 112, 112));

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

	case WM_MOUSEMOVE:
	{
		POINT		RealCursorPosition;
		GetCursorPos(&RealCursorPosition);
		ScreenToClient(hDlg, &RealCursorPosition);

		App->CL_Editor_Map->On_Mouse_Move(RealCursorPosition,hDlg);

		return 1;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		return 1;
	}

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

	case WM_MOUSEWHEEL:
	{
			if (App->CL_Editor_Map->flag_Left_Button_Down == true)
		{
			return 1;
		}

		if (App->CL_Editor_Map->Selected_Window == Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->flag_Wheel_Active = true;

			if (App->CL_Editor_Map->flag_Left_Button_Down == false)
			{
				int zDelta = static_cast<short>(HIWORD(wParam)); // wheel rotation

				if (zDelta > 0)
				{
					App->CL_Editor_Map->Current_View->ZoomFactor = App->CL_Editor_Map->Current_View->ZoomFactor + 0.1;
					App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
				}

				if (zDelta < 0)
				{
					App->CL_Editor_Map->Current_View->ZoomFactor = App->CL_Editor_Map->Current_View->ZoomFactor - 0.1;
					App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
				}
			}

			return 1;
		}
		
		App->CL_Editor_Map->flag_Wheel_Active = false;
		int zDelta = (short)HIWORD(wParam);    // wheel rotation

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
			App->CL_Editor_Map->flag_Right_Button_Down = false;
			App->CL_Editor_Map->flag_Left_Button_Down = false;

			App->CUR = SetCursor(App->CUR);
		}
		else
		{
			App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
			App->CL_Editor_Map->Context_Menu(hDlg);
		}

		return 1;
	}

	case WM_COMMAND:
	{
		break;
	}

	case WM_PAINT:
	{
		if (App->flag_3D_Started == true)
		{
			App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
			//App->CL_Editor_Map->Draw_Screen(hDlg);
			App->CL_3D_TL_View->Draw_Screen_TL(hDlg);
		}

		return 0;
	}

	}

	return FALSE;
}

typedef struct tagBrushDrawData
{
	const Box3d* pViewBox;
	HDC 			pDC;
	ViewVars* v;
	int				GroupId;
	CL64_Doc* pDoc;
	BrushFlagTest	FlagTest;
	Ogre::uint32	Color;
} BrushDrawData;

#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

signed int CL64_3D_TL_View::fdocShowBrush(Brush const* b, Box3d const* ViewBox)
{
	return 1;// (App->CL_Brush->BrushIsVisible(b) && App->CL_Brush->Brush_TestBoundsIntersect(b, ViewBox));
}

// *************************************************************************
// *	  						BrushDraw								   *
// *************************************************************************
signed int CL64_3D_TL_View::BrushDraw(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData = (BrushDrawData*)lParam;


	//if (App->CL_Brush->Brush_GetGroupId(pBrush) == pData->GroupId))
	{
		if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
		{
			if (App->CL_3D_TL_View->fdocShowBrush(pBrush, pData->pViewBox))
			{
				App->CL_Editor_Map->Render_RenderBrushFacesOrtho(pData->v, pBrush, App->CL_Editor_Map->MemoryhDC);
			}
		}
	}

	return true;
}

static signed int BrushDrawSelFacesOrtho(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData;

	pData = (BrushDrawData*)lParam;

	App->CL_Editor_Map->Render_RenderBrushSelFacesOrtho(pData->v, pBrush, pData->pDC);

	return	GE_TRUE;
}

// *************************************************************************
// *			Draw_Screen_TL:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_TL_View::Draw_Screen_TL(HWND hwnd)
{
	// Initialize variables
	int			inidx = 0;
	HDC RealhDC = GetDC(hwnd);
	m_MemoryhDC = CreateCompatibleDC(RealhDC);

	RECT		Rect;
	BrushDrawData	brushDrawData;

	// Get client rectangle and set current view dimensions
	GetClientRect(hwnd, &Rect);
	Rect.left--;
	Rect.bottom--;
	App->CL_Editor_Map->VCam[V_TL]->Width = Rect.left;
	App->CL_Editor_Map->VCam[V_TL]->Height = Rect.bottom;
	App->CL_Editor_Map->VCam[V_TL]->XScreenScale = Rect.left;
	App->CL_Editor_Map->VCam[V_TL]->YScreenScale = Rect.bottom;

	// Set up view box
	T_Vec3 XTemp;
	Box3d ViewBox;
	inidx = App->CL_Render->Render_GetInidx(App->CL_Editor_Map->VCam[V_TL]);
	App->CL_X_Box->Box3d_SetBogusBounds(&ViewBox);
	App->CL_Render->Render_ViewToWorld(App->CL_Editor_Map->VCam[V_TL], 0, 0, &XTemp);
	App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
	App->CL_Render->Render_ViewToWorld(App->CL_Editor_Map->VCam[V_TL], App->CL_Render->Render_GetWidth(App->CL_Editor_Map->VCam[V_TL]), App->CL_Render->Render_GetHeight(App->CL_Editor_Map->VCam[V_TL]), &XTemp);
	App->CL_X_Box->Box3d_AddPoint(&ViewBox, XTemp.x, XTemp.y, XTemp.z);
	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	// Prepare brush draw data
	brushDrawData.pViewBox = &ViewBox;
	brushDrawData.pDC = m_MemoryhDC;
	brushDrawData.v = App->CL_Editor_Map->VCam[V_TL];
	brushDrawData.pDoc = App->CL_Doc;
	brushDrawData.GroupId = 0;
	brushDrawData.FlagTest = NULL;

	GetClipBox(RealhDC, &Rect);

	SetDCBrushColor(m_MemoryhDC, (RGB(App->CL_Editor_Map->Background_Colour.R, App->CL_Editor_Map->Background_Colour.G, App->CL_Editor_Map->Background_Colour.B)));

	HBITMAP OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);
	SelectObject(m_MemoryhDC, OffScreenBitmap);
	FillRect(m_MemoryhDC, &Rect, (HBRUSH)App->CL_Editor_Map->Stock_Brush); // BackGround

	// ---------------------- Draw Grid Fine
	if (App->CL_Editor_Map->VCam[V_TL]->ZoomFactor > 0.1)
	{
		SelectObject(m_MemoryhDC, App->CL_Editor_Map->Pen_Fine_Grid);
		App->CL_Render->Render_RenderOrthoGridFromSize(App->CL_Editor_Map->VCam[V_TL], int(App->CL_Editor_Map->GridSnapSize), m_MemoryhDC, Rect);
	}

	// ---------------------- Draw Grid
	if (App->CL_Editor_Map->VCam[V_TL]->ZoomFactor < 0.1)
	{
		App->CL_Editor_Map->VCam[V_TL]->ZoomFactor = 0.1;
	}

	SelectObject(m_MemoryhDC, m_Pen_Grid);
	App->CL_Render->Render_RenderOrthoGridFromSize(App->CL_Editor_Map->VCam[V_TL], int(App->CL_Editor_Map->GridSize), m_MemoryhDC, Rect);

	bool test = 0;
	if (test == 0)
	{
		// ------------------------------------------ Draw Brushes
		SelectObject(m_MemoryhDC, App->CL_Editor_Map->PenBrushes);

		// Draw Template Brush
		/*if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
		{
			SelectObject(App->CL_Editor_Map->MemoryhDC, App->CL_Editor_Map->PenTemplate);

			if (App->CL_X_Brush->Brush_IsMulti(App->CL_Doc->CurBrush))
			{

				App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(App->CL_Doc->CurBrush), &brushDrawData, BrushDraw);
			}
			else
			{
				App->CL_Editor_Map->Render_RenderBrushFacesOrtho(App->CL_Editor_Map->VCam[V_TL], App->CL_Doc->CurBrush, App->CL_Editor_Map->MemoryhDC);

			}
		}*/

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
				SelectObject(m_MemoryhDC, App->CL_Editor_Map->PenBrushes);
				break;

			case Enums::Brushs_ID_Evirons:
				SelectObject(m_MemoryhDC, App->CL_Editor_Map->PenEntity);
				break;

			default:
				break;
			}

			if (App->CL_X_Brush->Brush_IsSubtract(SB))
			{
				SelectObject(m_MemoryhDC, App->CL_Editor_Map->PenCutBrush);
			}


			if (App->CL_X_Brush->Brush_IsMulti(SB))
			{
				App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(SB), &brushDrawData, BrushDraw);
			}
			else
			{
				App->CL_Editor_Map->Render_RenderBrushFacesOrtho(App->CL_Editor_Map->VCam[V_TL], SB, m_MemoryhDC);
			}

			Count++;
		}

		bool Draw_Sel = 0;
		if (Draw_Sel == 0)
		{
			// Draw selected brushes
			SelectObject(m_MemoryhDC, App->CL_Editor_Map->PenSelected);
			int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			int i = 0;
			for (i = 0; i < NumSelBrushes; i++)
			{
				Brush* pBrush;

				pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
				{
					if (App->CL_X_Brush->Brush_IsMulti(pBrush))
					{
						App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(pBrush), &brushDrawData, BrushDraw);
					}
					else
					{
						App->CL_Editor_Map->Render_RenderBrushFacesOrtho(App->CL_Editor_Map->VCam[V_TL], App->CL_Doc->CurBrush, m_MemoryhDC);
					}
				}
			}
		}

		// Draw selected faces
		BrushList* BList = App->CL_Level->Level_Get_Main_Brushes();
		SelectObject(m_MemoryhDC, App->CL_Editor_Map->PenSelectedFaces);
		App->CL_X_Brush->BrushList_EnumLeafBrushes(BList, &brushDrawData, BrushDrawSelFacesOrtho);


		// Draw camera if tracking
		if (App->CL_Doc->flag_Track_Camera == true)
		{
			SelectObject(m_MemoryhDC, App->CL_Editor_Map->Pen_Camera);
			App->CL_Editor_Map->Draw_Camera(m_MemoryhDC);
		}

	}

	// BitBlt to the real device context
	BitBlt(RealhDC, Rect.left, Rect.top + 17, Rect.right - Rect.left, Rect.bottom - Rect.top, m_MemoryhDC, 0, 0, SRCCOPY);
	//memcpy(&RealhDC, &MemoryhDC, sizeof(MemoryhDC));
	// Clean up
	SetDCBrushColor(m_MemoryhDC, (RGB(255, 255, 255)));

	DeleteObject(OffScreenBitmap);
	DeleteDC(m_MemoryhDC);
	ReleaseDC(hwnd, RealhDC);

	//App->Flash_Window();

}

