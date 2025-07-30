/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
#include "CL64_Properties_Faces.h"
#include "Structures.cpp"

CL64_Properties_Faces::CL64_Properties_Faces(void)
{
	FaceDlg_Hwnd = nullptr;

	m_Selected_Face = NULL;

	m_NumberOfFaces = 0;

	m_TextureAngle_Copy = 0;
	m_TextureAngle = 0;
	m_TextureAngle_Delta = 15;

	m_TextureXScale_Copy = 0;
	m_TextureXScale = 0;
	ScaleX_Delta = 0.01;

	m_TextureYScale_Copy = 0;
	m_TextureYScale = 0;
	ScaleY_Delta = 0.01;

	m_TextureXOffset_Copy = 0;
	m_TextureXOffset = 0;
	m_TextureXOffset_Delta = 16;

	m_TextureYOffset_Copy = 0;
	m_TextureYOffset = 0;
	m_TextureYOffset_Delta = 16;

	m_Selected_Face_Index = 0;

	flag_FaceDlg_Active = 0;
}

CL64_Properties_Faces::~CL64_Properties_Faces(void)
{
}

// *************************************************************************
// *		  ChangeTextureAngle:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
signed int CL64_Properties_Faces::ChangeTextureAngle(Face* pFace, void* lParam)
{
	float* pAngle = (float*)lParam;

	App->CL_Face->Face_SetTextureRotate(pFace, *pAngle);
	return true;
}

// *************************************************************************
// *		  FlipVertical:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
signed int CL64_Properties_Faces::FlipVertical(Face* pFace, void*)
{
	float xScale, yScale;

	App->CL_Face->Face_GetTextureScale(pFace, &xScale, &yScale);
	App->CL_Face->Face_SetTextureScale(pFace, xScale, -yScale);

	return GE_TRUE;
}

// *************************************************************************
// *		  FlipHorizontal:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
signed int CL64_Properties_Faces::FlipHorizontal(Face* pFace, void*)
{
	float xScale, yScale;

	App->CL_Face->Face_GetTextureScale(pFace, &xScale, &yScale);
	App->CL_Face->Face_SetTextureScale(pFace, -xScale, yScale);

	return GE_TRUE;
}

// *************************************************************************
// *		  ChangeTextureXScale:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
signed int CL64_Properties_Faces::ChangeTextureXScale(Face* pFace, void* lParam)
{
	float* pXScale = (float*)lParam;
	float xScale, yScale;

	App->CL_Face->Face_GetTextureScale(pFace, &xScale, &yScale);
	App->CL_Face->Face_SetTextureScale(pFace, *pXScale, yScale);
	return true;
}

// *************************************************************************
// *		  ChangeTextureYScale:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
signed int CL64_Properties_Faces::ChangeTextureYScale(Face* pFace, void* lParam)
{
	float* pYScale = (float*)lParam;
	float xScale, yScale;

	App->CL_Face->Face_GetTextureScale(pFace, &xScale, &yScale);
	App->CL_Face->Face_SetTextureScale(pFace, xScale, *pYScale);
	return true;
}

// *************************************************************************
// *		  ChangeXOffset:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
signed int CL64_Properties_Faces::ChangeXOffset(Face* pFace, void* lParam)
{
	int* pXOffset = (int*)(lParam);
	int xOff, yOff;

	App->CL_Face->Face_GetTextureShift(pFace, &xOff, &yOff);
	App->CL_Face->Face_SetTextureShift(pFace, *pXOffset, yOff);
	return true;
}

// *************************************************************************
// *		  ChangeYOffset:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
signed int CL64_Properties_Faces::ChangeYOffset(Face* pFace, void* lParam)
{
	int* pYOffset = (int*)(lParam);
	int xOff, yOff;

	App->CL_Face->Face_GetTextureShift(pFace, &xOff, &yOff);
	App->CL_Face->Face_SetTextureShift(pFace, xOff, *pYOffset);
	return true;
}

// *************************************************************************
// *	  	Start_FaceDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Faces::Start_FaceDialog()
{
	if (flag_FaceDlg_Active == 0)
	{
		FaceDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_FACES, App->MainHwnd, (DLGPROC)Proc_FaceDialog);
		flag_FaceDlg_Active = 1;
	}
}

// *************************************************************************
// *		 Proc_FaceDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Faces::Proc_FaceDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LABEL_NUM_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_NUM_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LST_FACELIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_STTEXTOFFSET, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXTSCALE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_ANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_X, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_Y, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_FLIPHORIZONTAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_FLIPVERTICAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_FACEINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LST_FACE_INFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_EDITXOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_EDITYOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_EDITXSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_EDITYSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_EDITANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_CBXOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBYOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBXSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CBYSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_CBANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_COPY_TEXTINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PASTE_TEXTINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_ALLFACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Properties_Faces->m_NumberOfFaces = 0;
		App->CL_Properties_Faces->m_NumberOfFaces = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

		char buf[10];
		sprintf(buf, "%i", App->CL_Properties_Faces->m_NumberOfFaces);
		SetDlgItemText(hDlg, IDC_ST_NUM_FACES, (LPCTSTR)buf);

		if (App->CL_Top_Tabs->flag_All_Faces == 0)
		{
			App->CL_Properties_Faces->Update_Face_List(hDlg);
		}

		App->CL_Properties_Faces->Update_Face_Members();
		App->CL_Properties_Faces->UpdateDialog(hDlg);

		HWND CB_hWnd = GetDlgItem(hDlg, IDC_CBXOFFSET);
		App->CL_Properties_Faces->Fill_ComboBox_OffSetValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBYOFFSET);
		App->CL_Properties_Faces->Fill_ComboBox_OffSetValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBXSCALE);
		App->CL_Properties_Faces->Fill_ComboBox_ScaleValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBYSCALE);
		App->CL_Properties_Faces->Fill_ComboBox_ScaleValues(CB_hWnd);

		App->CL_Properties_Faces->Fill_ComboBox_AngleValues(hDlg);

		if (App->CL_Top_Tabs->flag_All_Faces == 0)
		{
			App->CL_Properties_Faces->Update_Face_Info(hDlg);
		}

	if(App->CL_Top_Tabs->flag_All_Faces == 1)
	{
		HWND temp = GetDlgItem(hDlg, IDC_CK_ALLFACES);
		SendMessage(temp,BM_SETCHECK,1,0);
		EnableWindow(GetDlgItem(hDlg, IDC_LST_FACELIST), false);
		EnableWindow(GetDlgItem(hDlg, IDC_BT_COPY_TEXTINFO), false);
		EnableWindow(GetDlgItem(hDlg, IDC_BT_PASTE_TEXTINFO), false);
	}
	else
	{
		HWND temp = GetDlgItem(hDlg, IDC_CK_ALLFACES);
		SendMessage(temp,BM_SETCHECK,0,0);
		EnableWindow(GetDlgItem(hDlg, IDC_LST_FACELIST), true);
		EnableWindow(GetDlgItem(hDlg, IDC_BT_COPY_TEXTINFO), true);
		EnableWindow(GetDlgItem(hDlg, IDC_BT_PASTE_TEXTINFO), true);
	}

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STTEXTOFFSET) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTEXTSCALE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_X) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_Y) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_ANGLE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_LABEL_NUM_FACES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_NUM_FACES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_EDITXOFFSET) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_EDITYOFFSET) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}
		
		if (GetDlgItem(hDlg, IDC_ST_EDITXSCALE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_EDITYSCALE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_EDITANGLE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_FACEINFO) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_CK_ALLFACES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_FLIPHORIZONTAL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_FLIPVERTICAL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_COPY_TEXTINFO)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_PASTE_TEXTINFO)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_HSCROLL:
	{
		// -------- Angle
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBANGLE))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Faces->m_TextureAngle += App->CL_Properties_Faces->m_TextureAngle_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pAngle = (float)App->CL_Properties_Faces->m_TextureAngle;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
					App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeTextureAngle, &pAngle);
				}
				else
				{
					App->CL_Face->Face_SetTextureRotate(App->CL_Properties_Faces->m_Selected_Face, pAngle);
				}

				App->CL_Properties_Faces->Update_Faces();
				
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Faces->m_TextureAngle -= App->CL_Properties_Faces->m_TextureAngle_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pAngle = (float)App->CL_Properties_Faces->m_TextureAngle;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
					App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeTextureAngle, &pAngle);
				}
				else
				{
					App->CL_Face->Face_SetTextureRotate(App->CL_Properties_Faces->m_Selected_Face, pAngle);
				}

				App->CL_Properties_Faces->Update_Faces();
				
				break;
			}
			}

			return 0;
		}

		// -------- Offset X
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBXOFFSET))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				char buf[255];

				App->CL_Properties_Faces->m_TextureXOffset += App->CL_Properties_Faces->m_TextureXOffset_Delta;

				sprintf(buf, "%i", App->CL_Properties_Faces->m_TextureXOffset);
				SetDlgItemText(hDlg, IDC_ST_EDITXOFFSET, (LPCTSTR)buf);
			
				int pXOffset = (int)App->CL_Properties_Faces->m_TextureXOffset;
				int xOff, yOff;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
					App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeXOffset, &pXOffset);
				}
				else
				{
					App->CL_Face->Face_GetTextureShift(App->CL_Properties_Faces->m_Selected_Face, &xOff, &yOff);
					App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, pXOffset, yOff);
				}

				App->CL_Properties_Faces->Update_Faces();
				
				break;
			}

			case SB_LINELEFT:
			{
				char buf[255];

				App->CL_Properties_Faces->m_TextureXOffset -= App->CL_Properties_Faces->m_TextureXOffset_Delta;
				
				sprintf(buf, "%i", App->CL_Properties_Faces->m_TextureXOffset);
				SetDlgItemText(hDlg, IDC_ST_EDITXOFFSET, (LPCTSTR)buf);

				int pXOffset = (int)App->CL_Properties_Faces->m_TextureXOffset;
				int xOff, yOff;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
					App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeXOffset, &pXOffset);
				}
				else
				{
					App->CL_Face->Face_GetTextureShift(App->CL_Properties_Faces->m_Selected_Face, &xOff, &yOff);
					App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, pXOffset, yOff);
				}

				App->CL_Properties_Faces->Update_Faces();
				
				break;
			}
			}

			return 0;
		}

		// -------- Offset Y
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBYOFFSET))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Faces->m_TextureYOffset += App->CL_Properties_Faces->m_TextureYOffset_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);

				int pYOffset = (int)App->CL_Properties_Faces->m_TextureYOffset;
				int xOff, yOff;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
					App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeYOffset, &pYOffset);
				}
				else
				{
					App->CL_Face->Face_GetTextureShift(App->CL_Properties_Faces->m_Selected_Face, &xOff, &yOff);
					App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, xOff, pYOffset);
				}

				App->CL_Properties_Faces->Update_Faces();
				
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Faces->m_TextureYOffset -= App->CL_Properties_Faces->m_TextureYOffset_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				int pYOffset = (int)App->CL_Properties_Faces->m_TextureYOffset;
				int xOff, yOff;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
					App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeYOffset, &pYOffset);
				}
				else
				{
					App->CL_Face->Face_GetTextureShift(App->CL_Properties_Faces->m_Selected_Face, &xOff, &yOff);
					App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, xOff, pYOffset);
				}

				App->CL_Properties_Faces->Update_Faces();
				
				break;
			}
			}

			return 0;
		}

		// -------- Scale X
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBXSCALE))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Faces->m_TextureXScale += App->CL_Properties_Faces->ScaleX_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pXScale = (float)App->CL_Properties_Faces->m_TextureXScale;
				float xScale, yScale;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
					App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeTextureXScale, &pXScale);
				}
				else
				{
					App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
					App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, pXScale, yScale);
				}

				App->CL_Properties_Faces->Update_Faces();

				break;
			}
			case SB_LINELEFT:
			{
				App->CL_Properties_Faces->m_TextureXScale -= App->CL_Properties_Faces->ScaleX_Delta;
				
				if (App->CL_Properties_Faces->m_TextureXScale < 0.001)
				{
					App->CL_Properties_Faces->m_TextureXScale = 0;
				}
				
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pXScale = (float)App->CL_Properties_Faces->m_TextureXScale;
				float xScale, yScale;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
				}
				else
				{
					App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
				}
				
				if (pXScale > 0)
				{
					if (App->CL_Top_Tabs->flag_All_Faces == 1)
					{
						App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeTextureXScale, &pXScale);
					}
					else
					{
						App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, pXScale, yScale);
					}
				}
				else
				{
					if (App->CL_Top_Tabs->flag_All_Faces == 1)
					{
						pXScale = 0.001;
						App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeTextureXScale, &pXScale);
					}
					else
					{
						App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, 0.001, yScale);
					}
				}
	
				App->CL_Properties_Faces->Update_Faces();

				break;
			}
			}

			return 0;
		}

		// -------- Scale Y
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBYSCALE))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINERIGHT:
			{
				App->CL_Properties_Faces->m_TextureYScale += App->CL_Properties_Faces->ScaleY_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pYScale = (float)App->CL_Properties_Faces->m_TextureYScale;
				float xScale, yScale;

				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
					App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeTextureYScale, &pYScale);
				}
				else
				{
					App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
					App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, xScale, pYScale);
				}

				App->CL_Properties_Faces->Update_Faces();

				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Faces->m_TextureYScale -= App->CL_Properties_Faces->ScaleY_Delta;
				
				if (App->CL_Properties_Faces->m_TextureYScale < 0.001)
				{
					App->CL_Properties_Faces->m_TextureYScale = 0;
				}

				App->CL_Properties_Faces->UpdateDialog(hDlg);
			
				float pYScale = (float)App->CL_Properties_Faces->m_TextureYScale;
				float xScale, yScale;
				if (App->CL_Top_Tabs->flag_All_Faces == 1)
				{
				}
				else
				{
					App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
				}

				if (pYScale > 0)
				{
					if (App->CL_Top_Tabs->flag_All_Faces == 1)
					{
						App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeTextureYScale, &pYScale);
					}
					else
					{
						App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, xScale, pYScale);
					}
				}
				else
				{
					if (App->CL_Top_Tabs->flag_All_Faces == 1)
					{
						pYScale = 0.001;
						App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, ChangeTextureYScale, &pYScale);
					}
					else
					{
						App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, xScale, 0.001);
					}
				}

				App->CL_Properties_Faces->Update_Faces();

				break;
			}
			}

			return 0;
		}

		return 0;
	}

	case WM_COMMAND:
	{
	
		if (LOWORD(wParam) == IDC_LST_FACELIST)
		{
			int Index = SendDlgItemMessage(hDlg, IDC_LST_FACELIST, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			if (Index == -1)
			{
				return TRUE;
			}
			else
			{
				App->CL_Properties_Faces->m_Selected_Face_Index = Index;
				App->CL_Properties_Faces->m_Selected_Face = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, Index);
				
				App->CL_Face->Selected_Face_Index = Index;

				App->CL_Top_Tabs->Select_Face();
				App->CL_Properties_Faces->Change_Selection();
				App->CL_Properties_Faces->Update_Face_Info(hDlg);
			}

			return TRUE;
		}

		/*if (LOWORD(wParam) == IDC_TEXTURELOCK)
		{
			HWND temp = GetDlgItem(hDlg, IDC_TEXTURELOCK);

			App->Get_Current_Document();

			Face* pFace;
			int NumberOfFaces;

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				NumberOfFaces = SelFaceList_GetSize(App->CLSB_Doc->pSelFaces);

				for (int i = 0; i < NumberOfFaces; ++i)
				{
					pFace = SelFaceList_GetFace(App->CLSB_Doc->pSelFaces, i);
					Face_SetTextureLock(pFace, true);
				}

				App->CLSB_Doc->UpdateAllViews(UAV_RENDER_ONLY, NULL);

				return 1;
			}
			else
			{
				NumberOfFaces = SelFaceList_GetSize(App->CLSB_Doc->pSelFaces);

				for (int i = 0; i < NumberOfFaces; ++i)
				{
					pFace = SelFaceList_GetFace(App->CLSB_Doc->pSelFaces, i);
					Face_SetTextureLock(pFace, false);
				}

				App->CLSB_Doc->UpdateAllViews(UAV_RENDER_ONLY, NULL);
				return 1;
			}
			return TRUE;
		}*/

		if (LOWORD(wParam) == IDC_CBXOFFSET)
		{
			switch (HIWORD(wParam)) 
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBXOFFSET);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Faces->m_TextureXOffset_Delta = atoi(buff);
			}
			}
			
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CBYOFFSET)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBYOFFSET);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Faces->m_TextureYOffset_Delta = atoi(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CBXSCALE)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBXSCALE);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Faces->ScaleX_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CBYSCALE)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBYSCALE);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Faces->ScaleY_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		// Angle Combo
		if (LOWORD(wParam) == IDC_CBANGLE)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CBANGLE);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Properties_Faces->m_TextureAngle_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_FLIPHORIZONTAL)
		{
			float xScale, yScale;

			if (App->CL_Top_Tabs->flag_All_Faces == 1)
			{
				App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, FlipHorizontal, NULL);
			}
			else
			{
				App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
				App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, xScale, -yScale);
			}

			App->CL_Properties_Faces->UpdateDialog(hDlg);
			App->CL_Properties_Faces->Update_Faces();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_FLIPVERTICAL)
		{
			float xScale, yScale;

			if (App->CL_Top_Tabs->flag_All_Faces == 1)
			{
				App->CL_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, FlipVertical, NULL);
			}
			else
			{
				App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
				App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, -xScale, yScale);
			}

			App->CL_Properties_Faces->UpdateDialog(hDlg);
			App->CL_Properties_Faces->Update_Faces();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_COPY_TEXTINFO)
		{
			App->CL_Properties_Faces->m_TextureAngle_Copy = App->CL_Properties_Faces->m_TextureAngle;

			App->CL_Properties_Faces->m_TextureXScale_Copy = App->CL_Properties_Faces->m_TextureXScale;
			App->CL_Properties_Faces->m_TextureYScale_Copy = App->CL_Properties_Faces->m_TextureYScale;

			App->CL_Properties_Faces->m_TextureXOffset_Copy = App->CL_Properties_Faces->m_TextureXOffset;
			App->CL_Properties_Faces->m_TextureYOffset_Copy = App->CL_Properties_Faces->m_TextureYOffset;

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PASTE_TEXTINFO)
		{
			App->CL_Dialogs->YesNo("Update Face Settings", "Are you sure");

			if (App->CL_Dialogs->flag_Dlg_Canceled == true)
			{
				return 1;
			}

			App->CL_Properties_Faces->m_TextureAngle = App->CL_Properties_Faces->m_TextureAngle_Copy;

			App->CL_Properties_Faces->m_TextureXScale = App->CL_Properties_Faces->m_TextureXScale_Copy;
			App->CL_Properties_Faces->m_TextureYScale = App->CL_Properties_Faces->m_TextureYScale_Copy;

			App->CL_Properties_Faces->m_TextureXOffset = App->CL_Properties_Faces->m_TextureXOffset_Copy;
			App->CL_Properties_Faces->m_TextureYOffset = App->CL_Properties_Faces->m_TextureYOffset_Copy;
			
			App->CL_Face->Face_SetTextureRotate(App->CL_Properties_Faces->m_Selected_Face, App->CL_Properties_Faces->m_TextureAngle);
			App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, App->CL_Properties_Faces->m_TextureXOffset, App->CL_Properties_Faces->m_TextureYOffset);
			App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, App->CL_Properties_Faces->m_TextureXScale, App->CL_Properties_Faces->m_TextureYScale);
			
			App->CL_Properties_Faces->UpdateDialog(hDlg);
			App->CL_Properties_Faces->Update_Faces();

			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Properties_Faces->flag_FaceDlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Faces->flag_FaceDlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}

	return FALSE;
}

// *************************************************************************
// *			Update_Face_Info:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
void CL64_Properties_Faces::Update_Face_Info(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_LST_FACE_INFO, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	char buff[MAX_PATH];
	Brush* pBrush;

	pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, 0);

	sprintf(buff, "%s %i", "Main Face:", m_Selected_Face->Real_Brush_Face_Index);
	SendDlgItemMessage(hDlg, IDC_LST_FACE_INFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %s     %s %i", "Brush Name: ", App->CL_Brush->Brush_GetName(pBrush),"Selected Face: ", App->CL_Face->Selected_Face_Index+1);
	SendDlgItemMessage(hDlg, IDC_LST_FACE_INFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);
	
	sprintf(buff, "%s %s", "Texture: ", App->CL_Face->Face_GetTextureName(m_Selected_Face));
	SendDlgItemMessage(hDlg, IDC_LST_FACE_INFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %i", "Bitmap ID: ", App->CL_Face->Face_GetTextureDibId(m_Selected_Face));
	SendDlgItemMessage(hDlg, IDC_LST_FACE_INFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	bool Test_Lock = App->CL_Face->Face_IsTextureLocked(m_Selected_Face);

	if (Test_Lock)
	{
		sprintf(buff, "%s %s", "Locked: ", "Yes");
		SendDlgItemMessage(hDlg, IDC_LST_FACE_INFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);
	}
	else
	{
		sprintf(buff, "%s %s", "Locked: ", "No");
		SendDlgItemMessage(hDlg, IDC_LST_FACE_INFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);
	}
}

// *************************************************************************
// *			Update_Faces:- Terry and Hazel Flanigan 2025               *
// *************************************************************************
void CL64_Properties_Faces::Update_Faces()
{
	if (App->CL_Top_Tabs->flag_All_Faces == 1)
	{
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_3D);
		App->CL_Doc->SelectAllFacesInBrushes();
	}
	else
	{
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_3D);
		App->CL_SelFaceList->SelFaceList_RemoveAll(App->CL_Doc->pSelFaces);
		App->CL_Doc->SelectAllFacesInBrushes();
		App->CL_Face->Select_Face_From_Index(App->CL_Face->Selected_Face_Index);
	}

	App->CL_Ogre->RenderFrame(2);

	App->CL_Properties_Faces->m_Selected_Face = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, App->CL_Face->Selected_Face_Index);
}

// *************************************************************************
// *				Fill_ComboBox_AngleValues Terry Flanigan	  		   *
// *************************************************************************
void CL64_Properties_Faces::Fill_ComboBox_AngleValues(HWND hDlg)
{
	HWND CB_hWnd = GetDlgItem(hDlg, IDC_CBANGLE);

	SendMessage(CB_hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"5");
	SendMessage(CB_hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"10");
	SendMessage(CB_hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"15");
	SendMessage(CB_hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"30");
	SendMessage(CB_hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"45");
	SendMessage(CB_hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"90");

	SendMessage(CB_hWnd, CB_SETCURSEL, 2, 0);
}

// *************************************************************************
// *				Fill_ComboBox_ScaleValues Terry Flanigan	  		   *
// *************************************************************************
void CL64_Properties_Faces::Fill_ComboBox_ScaleValues(HWND hDlg)
{
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.001");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.01");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.02");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.05");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.10");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"0.50");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"1.0");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"2.0");

	SendMessage(hDlg, CB_SETCURSEL, 1, 0);
}

// *************************************************************************
// *				Fill_ComboBox_OffSetValues Terry Flanigan	  		   *
// *************************************************************************
void CL64_Properties_Faces::Fill_ComboBox_OffSetValues(HWND hDlg)
{
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"4");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"8");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"16");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"32");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"64");
	SendMessage(hDlg, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"128");

	SendMessage(hDlg, CB_SETCURSEL, 2, 0);
}

// *************************************************************************
// *		Update_Face_List:- Terry and Hazel Flanigan 2025			*
// *************************************************************************
void CL64_Properties_Faces::Update_Face_List(HWND hDlg)
{
	int Count = 0;
	Face* pFace = NULL;
	char buff[MAX_PATH];

	SendDlgItemMessage(hDlg, IDC_LST_FACELIST, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	int SB = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

	if (SB > 0)
	{
		int Count = 0;
		int Face_Count = App->CL_Brush_X->Face_Count;

		while (Count < Face_Count)
		{
			sprintf(buff, "%s %i", "Face:-", Count + 1);
			SendDlgItemMessage(hDlg, IDC_LST_FACELIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);
			Count++;
		}

		SendDlgItemMessage(hDlg, IDC_LST_FACELIST, LB_SETCURSEL, (WPARAM)App->CL_Face->Selected_Face_Index, (LPARAM)0);
	
	}

	m_Selected_Face = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, App->CL_Face->Selected_Face_Index);
}

// *************************************************************************
// *		Update_Face_Members:- Terry and Hazel Flanigan 2025            *
// *************************************************************************
void CL64_Properties_Faces::Update_Face_Members()
{
	int NumberOfFaces = 0;
	m_Selected_Face = NULL;

	NumberOfFaces = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
	if (NumberOfFaces > 0)
	{
		m_Selected_Face = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, m_Selected_Face_Index);
	}
	else
	{
		m_Selected_Face = NULL;
		return;
	}
	
	if (NumberOfFaces && m_Selected_Face)
	{
		m_TextureAngle = App->CL_Face->Face_GetTextureRotate(m_Selected_Face);
		App->CL_Face->Face_GetTextureShift(m_Selected_Face, &m_TextureXOffset, &m_TextureYOffset);
		App->CL_Face->Face_GetTextureScale(m_Selected_Face, &m_TextureXScale, &m_TextureYScale);

		//		m_TextureLock	=Face_IsTextureLocked (pFace);
		//		GetDlgItem( IDC_FACELIGHTINTENSITY )->EnableWindow( m_Light && !m_Sky) ;
		//		DisplayingNULL = FALSE;
		//		EnabledChange(TRUE);
	}
}

// *************************************************************************
// *			UpdateDialog:- Terry and Hazel Flanigan 2025               *
// *************************************************************************
void CL64_Properties_Faces::UpdateDialog(HWND hDlg)
{
	char buf[255];

	sprintf(buf, "%i", m_TextureXOffset);
	SetDlgItemText(hDlg, IDC_ST_EDITXOFFSET, (LPCTSTR)buf);

	sprintf(buf, "%i", m_TextureYOffset);
	SetDlgItemText(hDlg, IDC_ST_EDITYOFFSET, (LPCTSTR)buf);

	sprintf(buf, "%.3f", m_TextureXScale);
	SetDlgItemText(hDlg, IDC_ST_EDITXSCALE, (LPCTSTR)buf);

	sprintf(buf, "%.3f", m_TextureYScale);
	SetDlgItemText(hDlg, IDC_ST_EDITYSCALE, (LPCTSTR)buf);

	sprintf(buf, "%.0f", m_TextureAngle);
	SetDlgItemText(hDlg, IDC_ST_EDITANGLE, (LPCTSTR)buf);

	//Update_FaceProperties_Dlg(hDlg);
}

// *************************************************************************
// *			Change_Selection:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
void CL64_Properties_Faces::Change_Selection()
{
	App->CL_Properties_Faces->Update_Face_Members();
	App->CL_Properties_Faces->UpdateDialog(FaceDlg_Hwnd);

	SendDlgItemMessage(FaceDlg_Hwnd, IDC_LST_FACELIST, LB_SETCURSEL, (WPARAM)App->CL_Face->Selected_Face_Index, (LPARAM)0);
}

// *************************************************************************
// *			Close_Faces_Dialog:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
void CL64_Properties_Faces::Close_Faces_Dialog()
{
	App->CL_Properties_Faces->flag_FaceDlg_Active = 0;
	EndDialog(FaceDlg_Hwnd, 0);
}