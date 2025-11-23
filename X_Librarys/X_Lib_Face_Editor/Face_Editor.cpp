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
#include "Face_Editor.h"
#include "Structures.cpp"


Face_Editor::Face_Editor(void)
{
	Slider_Scale_X_hWnd = nullptr;
	Slider_Scale_Y_hWnd = nullptr;
	Slider_Offset_X_hWnd = nullptr;
	Slider_Offset_Y_hWnd = nullptr;
	Slider_Rotation_hWnd = nullptr;

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

Face_Editor::~Face_Editor(void)
{
}

// *************************************************************************
// *		  ChangeTextureAngle:- Terry Mo and Hazel 2025				   *
// *************************************************************************
signed int Face_Editor::ChangeTextureAngle(Face* pFace, void* lParam)
{
	float* pAngle = (float*)lParam;

	App->CL_X_Face->Face_SetTextureRotate(pFace, *pAngle);
	return true;
}

// *************************************************************************
// *		  FlipVertical:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
signed int Face_Editor::FlipVertical(Face* pFace, void*)
{
	float xScale, yScale;

	App->CL_X_Face->Face_GetTextureScale(pFace, &xScale, &yScale);
	App->CL_X_Face->Face_SetTextureScale(pFace, xScale, -yScale);

	return true;
}

// *************************************************************************
// *		  FlipHorizontal:- Terry Mo and Hazel 2025					   *
// *************************************************************************
signed int Face_Editor::FlipHorizontal(Face* pFace, void*)
{
	float xScale, yScale;

	App->CL_X_Face->Face_GetTextureScale(pFace, &xScale, &yScale);
	App->CL_X_Face->Face_SetTextureScale(pFace, -xScale, yScale);

	return true;
}

// *************************************************************************
// *		  ChangeTextureXScale:- Terry Mo and Hazel 2025				   *
// *************************************************************************
signed int Face_Editor::ChangeTextureXScale(Face* pFace, void* lParam)
{
	float* pXScale = (float*)lParam;
	float xScale, yScale;

	App->CL_X_Face->Face_GetTextureScale(pFace, &xScale, &yScale);
	App->CL_X_Face->Face_SetTextureScale(pFace, *pXScale, yScale);
	return true;
}

// *************************************************************************
// *		  ChangeTextureYScale:- Terry Mo and Hazel 2025		   *
// *************************************************************************
signed int Face_Editor::ChangeTextureYScale(Face* pFace, void* lParam)
{
	float* pYScale = (float*)lParam;
	float xScale, yScale;

	App->CL_X_Face->Face_GetTextureScale(pFace, &xScale, &yScale);
	App->CL_X_Face->Face_SetTextureScale(pFace, xScale, *pYScale);
	return true;
}

// *************************************************************************
// *			 ChangeXOffset:- Terry Mo and Hazel 2025				   *
// *************************************************************************
signed int Face_Editor::ChangeXOffset(Face* pFace, void* lParam)
{
	int* pXOffset = (int*)(lParam);
	int xOff, yOff;

	App->CL_X_Face->Face_GetTextureShift(pFace, &xOff, &yOff);
	App->CL_X_Face->Face_SetTextureShift(pFace, *pXOffset, yOff);
	return true;
}

// *************************************************************************
// *			  ChangeYOffset:- Terry Mo and Hazel 2025				   *
// *************************************************************************
signed int Face_Editor::ChangeYOffset(Face* pFace, void* lParam)
{
	int* pYOffset = (int*)(lParam);
	int xOff, yOff;

	App->CL_X_Face->Face_GetTextureShift(pFace, &xOff, &yOff);
	App->CL_X_Face->Face_SetTextureShift(pFace, xOff, *pYOffset);
	return true;
}

// *************************************************************************
// *	  	Start_FaceDialog:- Terry Mo and Hazel 2025					   *
// *************************************************************************
void Face_Editor::Start_FaceDialog()
{
	if (flag_FaceDlg_Active == 0)
	{
		FaceDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_FACES, App->MainHwnd, (DLGPROC)Proc_FaceDialog);
		flag_FaceDlg_Active = 1;
	}
}

// *************************************************************************
// *		 Proc_FaceDialog:- Terry Mo and Hazel 2025					   *
// *************************************************************************
LRESULT CALLBACK Face_Editor::Proc_FaceDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& m_FaceEditor = App->CL_X_Face_Editor; // App->CL_X_Face_Editor

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LABEL_NUM_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_NUM_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_STTEXTOFFSET, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXTSCALE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_ANGLE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_WSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_WPOS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_HSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_HPOS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_RANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_FLIPHORIZONTAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_FLIPVERTICAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_FACEINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
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

		SendDlgItemMessage(hDlg, IDC_BT_FACEDATA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		m_FaceEditor->m_NumberOfFaces = 0;
		m_FaceEditor->m_NumberOfFaces = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

		char buf[10];
		sprintf(buf, "%i", m_FaceEditor->m_NumberOfFaces);
		SetDlgItemText(hDlg, IDC_ST_NUM_FACES, (LPCTSTR)buf);

		if (App->CL_Top_Tabs->flag_All_Faces == 0)
		{
			m_FaceEditor->Update_Face_List(hDlg);
		}

		m_FaceEditor->Update_Face_Members();
		m_FaceEditor->UpdateDialog(hDlg);

		HWND CB_hWnd = GetDlgItem(hDlg, IDC_CBXOFFSET);
		m_FaceEditor->Fill_ComboBox_OffSetValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBYOFFSET);
		m_FaceEditor->Fill_ComboBox_OffSetValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBXSCALE);
		m_FaceEditor->Fill_ComboBox_ScaleValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBYSCALE);
		m_FaceEditor->Fill_ComboBox_ScaleValues(CB_hWnd);

		m_FaceEditor->Fill_ComboBox_AngleValues(hDlg);

		if (App->CL_Top_Tabs->flag_All_Faces == 0)
		{
			m_FaceEditor->Update_Face_Info(hDlg);
		}

		if (App->CL_Top_Tabs->flag_All_Faces == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ALLFACES);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			EnableWindow(GetDlgItem(hDlg, IDC_BT_COPY_TEXTINFO), false);
			EnableWindow(GetDlgItem(hDlg, IDC_BT_PASTE_TEXTINFO), false);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_ALLFACES);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			EnableWindow(GetDlgItem(hDlg, IDC_BT_COPY_TEXTINFO), true);
			EnableWindow(GetDlgItem(hDlg, IDC_BT_PASTE_TEXTINFO), true);
		}

		m_FaceEditor->Slider_Scale_X_hWnd = GetDlgItem(hDlg, IDC_SLDR_SCALE_X);
		SendMessageW(m_FaceEditor->Slider_Scale_X_hWnd, TBM_SETRANGE, TRUE, MAKELONG(0, 200));
		SendMessageW(m_FaceEditor->Slider_Scale_X_hWnd, TBM_SETPAGESIZE, 0, 1);
		SendMessageW(m_FaceEditor->Slider_Scale_X_hWnd, TBM_SETTICFREQ, 1, 0);
		SendMessageW(m_FaceEditor->Slider_Scale_X_hWnd, TBM_SETPOS, true, m_FaceEditor->m_TextureXScale * 100);


		m_FaceEditor->Slider_Scale_Y_hWnd = GetDlgItem(hDlg, IDC_SLDR_SCALE_Y);
		SendMessageW(m_FaceEditor->Slider_Scale_Y_hWnd, TBM_SETRANGE, TRUE, MAKELONG(0, 200));
		SendMessageW(m_FaceEditor->Slider_Scale_Y_hWnd, TBM_SETPAGESIZE, 0, 1);
		SendMessageW(m_FaceEditor->Slider_Scale_Y_hWnd, TBM_SETTICFREQ, 1, 0);
		SendMessageW(m_FaceEditor->Slider_Scale_Y_hWnd, TBM_SETPOS, true, m_FaceEditor->m_TextureYScale * 100);

		m_FaceEditor->Slider_Offset_X_hWnd = GetDlgItem(hDlg, IDC_SLDR_OFFSET_X);
		SendMessageW(m_FaceEditor->Slider_Offset_X_hWnd, TBM_SETRANGE, TRUE, MAKELONG(0, 2000));
		SendMessageW(m_FaceEditor->Slider_Offset_X_hWnd, TBM_SETPAGESIZE, 0, 1);
		SendMessageW(m_FaceEditor->Slider_Offset_X_hWnd, TBM_SETTICFREQ, 1, 0);
		SendMessageW(m_FaceEditor->Slider_Offset_X_hWnd, TBM_SETPOS, true, m_FaceEditor->m_TextureXOffset);

		m_FaceEditor->Slider_Offset_Y_hWnd = GetDlgItem(hDlg, IDC_SLDR_OFFSET_Y);
		SendMessageW(m_FaceEditor->Slider_Offset_Y_hWnd, TBM_SETRANGE, TRUE, MAKELONG(0, 2000));
		SendMessageW(m_FaceEditor->Slider_Offset_Y_hWnd, TBM_SETPAGESIZE, 0, 1);
		SendMessageW(m_FaceEditor->Slider_Offset_Y_hWnd, TBM_SETTICFREQ, 1, 0);
		SendMessageW(m_FaceEditor->Slider_Offset_Y_hWnd, TBM_SETPOS, true, m_FaceEditor->m_TextureYOffset);

		m_FaceEditor->Slider_Rotation_hWnd = GetDlgItem(hDlg, IDC_SLDR_ROTATION);
		SendMessageW(m_FaceEditor->Slider_Rotation_hWnd, TBM_SETRANGE, TRUE, MAKELONG(0, 360));
		SendMessageW(m_FaceEditor->Slider_Rotation_hWnd, TBM_SETPAGESIZE, 0, 1);
		SendMessageW(m_FaceEditor->Slider_Rotation_hWnd, TBM_SETTICFREQ, 1, 0);
		SendMessageW(m_FaceEditor->Slider_Rotation_hWnd, TBM_SETPOS, true, m_FaceEditor->m_TextureAngle);

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

		if (GetDlgItem(hDlg, IDC_ST_WSCALE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_WPOS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_HSCALE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_HPOS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_RANGLE) == (HWND)lParam)
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

		if (GetDlgItem(hDlg, IDC_SLDR_SCALE_X) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(1, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 1, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			SendMessage(m_FaceEditor->Slider_Scale_X_hWnd, PBM_SETBARCOLOR, 0, RGB(255, 255, 0));
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_SLDR_SCALE_Y) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_SLDR_OFFSET_X) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_SLDR_OFFSET_Y) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_SLDR_ROTATION) == (HWND)lParam)
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

		if (some_item->idFrom == IDC_BT_FACEDATA)
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
		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SLDR_SCALE_X))
		{
			auto& TScaleX = m_FaceEditor->m_TextureXScale;

			float ScalePosX = 0;
			ScalePosX = SendMessageW(m_FaceEditor->Slider_Scale_X_hWnd, TBM_GETPOS, 0, 0);

			TScaleX = ScalePosX / 100;

			if (TScaleX < 0.001)
			{
				TScaleX = 0.010;
				ScalePosX = 0.010;
			}

			char buf[20];
			sprintf(buf, "%.3f", TScaleX);
			SetDlgItemText(hDlg, IDC_ST_EDITXSCALE, (LPCTSTR)buf);

			float xScale, yScale;

			App->CL_X_Face->Face_GetTextureScale(m_FaceEditor->m_Selected_Face, &xScale, &yScale);
			App->CL_X_Face->Face_SetTextureScale(m_FaceEditor->m_Selected_Face, TScaleX, yScale);

			m_FaceEditor->Update_Faces();

			return 0;
		}

		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SLDR_SCALE_Y))
		{
			float ScalePosY = 0;
			ScalePosY = SendMessageW(m_FaceEditor->Slider_Scale_Y_hWnd, TBM_GETPOS, 0, 0);

			m_FaceEditor->m_TextureYScale = ScalePosY / 200;

			char buf[20];
			sprintf(buf, "%.3f", m_FaceEditor->m_TextureYScale);
			SetDlgItemText(hDlg, IDC_ST_EDITYSCALE, (LPCTSTR)buf);

			float xScale, yScale;

			App->CL_X_Face->Face_GetTextureScale(m_FaceEditor->m_Selected_Face, &xScale, &yScale);
			App->CL_X_Face->Face_SetTextureScale(m_FaceEditor->m_Selected_Face, xScale, ScalePosY / 100);

			m_FaceEditor->Update_Faces();

			return 0;
		}

		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SLDR_OFFSET_X))
		{
			int PosX = 0;
			PosX = SendMessageW(m_FaceEditor->Slider_Offset_X_hWnd, TBM_GETPOS, 0, 0);

			m_FaceEditor->m_TextureXOffset = PosX;

			char buf[20];
			sprintf(buf, "%i", m_FaceEditor->m_TextureXOffset);
			SetDlgItemText(hDlg, IDC_ST_EDITXOFFSET, (LPCTSTR)buf);

			int pXOffset = (int)m_FaceEditor->m_TextureXOffset;
			int xOff, yOff;

			App->CL_X_Face->Face_GetTextureShift(m_FaceEditor->m_Selected_Face, &xOff, &yOff);
			App->CL_X_Face->Face_SetTextureShift(m_FaceEditor->m_Selected_Face, pXOffset, yOff);

			m_FaceEditor->Update_Faces();

			return 0;
		}

		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SLDR_OFFSET_Y))
		{
			int PosY = 0;
			PosY = SendMessageW(m_FaceEditor->Slider_Offset_Y_hWnd, TBM_GETPOS, 0, 0);

			m_FaceEditor->m_TextureYOffset = PosY;

			char buf[20];
			sprintf(buf, "%i", m_FaceEditor->m_TextureYOffset);
			SetDlgItemText(hDlg, IDC_ST_EDITYOFFSET, (LPCTSTR)buf);

			int pYOffset = (int)m_FaceEditor->m_TextureYOffset;
			int xOff, yOff;

			App->CL_X_Face->Face_GetTextureShift(m_FaceEditor->m_Selected_Face, &xOff, &yOff);
			App->CL_X_Face->Face_SetTextureShift(m_FaceEditor->m_Selected_Face, xOff, pYOffset);

			m_FaceEditor->Update_Faces();

			return 0;
		}

		if (HWND(lParam) == GetDlgItem(hDlg, IDC_SLDR_ROTATION))
		{
			auto& TRotation = m_FaceEditor->m_TextureAngle;

			float ScalePosX = 0;
			ScalePosX = SendMessageW(m_FaceEditor->Slider_Rotation_hWnd, TBM_GETPOS, 0, 0);

			TRotation = ScalePosX;

			char buf[20];
			sprintf(buf, "%.0f", TRotation);
			SetDlgItemText(hDlg, IDC_ST_EDITANGLE, (LPCTSTR)buf);

			App->CL_X_Face->Face_SetTextureRotate(m_FaceEditor->m_Selected_Face, TRotation);

			/*float xScale, yScale;

			App->CL_X_Face->Face_GetTextureScale(m_FaceEditor->m_Selected_Face, &xScale, &yScale);*/
			//App->CL_X_Face->Face_SetTextureScale(m_FaceEditor->m_Selected_Face, TScaleX, yScale);

			m_FaceEditor->Update_Faces();

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
				m_FaceEditor->m_Selected_Face_Index = Index;
				m_FaceEditor->m_Selected_Face = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, Index);

				App->CL_X_Face->Selected_Face_Index = Index;

				App->CL_Top_Tabs->Select_Face();
				m_FaceEditor->Change_Selection();
				m_FaceEditor->Update_Face_Info(hDlg);
			}

			return TRUE;
		}

		//if (LOWORD(wParam) == IDC_TEXTURELOCK)
		//{
		//	HWND temp = GetDlgItem(hDlg, IDC_TEXTURELOCK);

		//	Face* pFace;
		//	int NumberOfFaces;

		//	int test = SendMessage(temp, BM_GETCHECK, 0, 0);
		//	if (test == BST_CHECKED)
		//	{
		//		NumberOfFaces = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

		//		for (int i = 0; i < NumberOfFaces; ++i)
		//		{
		//			pFace = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, i);
		//			App->CL_X_Face->Face_SetTextureLock(pFace, true);
		//		}

		//		//App->CL_Doc->UpdateAllViews(UAV_RENDER_ONLY, NULL);

		//		return 1;
		//	}
		//	else
		//	{
		//		NumberOfFaces = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

		//		for (int i = 0; i < NumberOfFaces; ++i)
		//		{
		//			pFace = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, i);
		//			App->CL_X_Face->Face_SetTextureLock(pFace, false);
		//		}

		//		//App->CL_Doc->UpdateAllViews(UAV_RENDER_ONLY, NULL);
		//		return 1;
		//	}

		//	return TRUE;
		//}

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

				m_FaceEditor->m_TextureXOffset_Delta = atoi(buff);
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

				m_FaceEditor->m_TextureYOffset_Delta = atoi(buff);
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

				m_FaceEditor->ScaleX_Delta = atof(buff);
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

				m_FaceEditor->ScaleY_Delta = atof(buff);
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

				m_FaceEditor->m_TextureAngle_Delta = atof(buff);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_FLIPHORIZONTAL)
		{
			float xScale, yScale;

			if (App->CL_Top_Tabs->flag_All_Faces == 1)
			{
				App->CL_X_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, FlipHorizontal, NULL);
			}
			else
			{
				App->CL_X_Face->Face_GetTextureScale(m_FaceEditor->m_Selected_Face, &xScale, &yScale);
				App->CL_X_Face->Face_SetTextureScale(m_FaceEditor->m_Selected_Face, xScale, -yScale);
			}

			m_FaceEditor->UpdateDialog(hDlg);
			m_FaceEditor->Update_Faces();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_FLIPVERTICAL)
		{
			float xScale, yScale;

			if (App->CL_Top_Tabs->flag_All_Faces == 1)
			{
				App->CL_X_SelFaceList->SelFaceList_Enum(App->CL_Doc->pSelFaces, FlipVertical, NULL);
			}
			else
			{
				App->CL_X_Face->Face_GetTextureScale(m_FaceEditor->m_Selected_Face, &xScale, &yScale);
				App->CL_X_Face->Face_SetTextureScale(m_FaceEditor->m_Selected_Face, -xScale, yScale);
			}

			m_FaceEditor->UpdateDialog(hDlg);
			m_FaceEditor->Update_Faces();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_COPY_TEXTINFO)
		{
			m_FaceEditor->m_TextureAngle_Copy = m_FaceEditor->m_TextureAngle;

			m_FaceEditor->m_TextureXScale_Copy = m_FaceEditor->m_TextureXScale;
			m_FaceEditor->m_TextureYScale_Copy = m_FaceEditor->m_TextureYScale;

			m_FaceEditor->m_TextureXOffset_Copy = m_FaceEditor->m_TextureXOffset;
			m_FaceEditor->m_TextureYOffset_Copy = m_FaceEditor->m_TextureYOffset;

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PASTE_TEXTINFO)
		{
			App->CL_Dialogs->YesNo("Update Face Settings", "Are you sure");

			if (App->CL_Dialogs->flag_Dlg_Canceled == true)
			{
				return 1;
			}

			m_FaceEditor->m_TextureAngle = m_FaceEditor->m_TextureAngle_Copy;

			m_FaceEditor->m_TextureXScale = m_FaceEditor->m_TextureXScale_Copy;
			m_FaceEditor->m_TextureYScale = m_FaceEditor->m_TextureYScale_Copy;

			m_FaceEditor->m_TextureXOffset = m_FaceEditor->m_TextureXOffset_Copy;
			m_FaceEditor->m_TextureYOffset = m_FaceEditor->m_TextureYOffset_Copy;

			App->CL_X_Face->Face_SetTextureRotate(m_FaceEditor->m_Selected_Face, m_FaceEditor->m_TextureAngle);
			App->CL_X_Face->Face_SetTextureShift(m_FaceEditor->m_Selected_Face, m_FaceEditor->m_TextureXOffset, m_FaceEditor->m_TextureYOffset);
			App->CL_X_Face->Face_SetTextureScale(m_FaceEditor->m_Selected_Face, m_FaceEditor->m_TextureXScale, m_FaceEditor->m_TextureYScale);

			m_FaceEditor->UpdateDialog(hDlg);
			m_FaceEditor->Update_Faces();

			return TRUE;
		}

		/*if (LOWORD(wParam) == IDC_CK_LOCKTEXTURES)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_LOCKTEXTURES);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_X_Brush->Brush_Lock_Textures(App->CL_Doc->CurBrush, true);
				m_FaceEditor->Update_Face_Info(hDlg);
				return TRUE;
			}
			else
			{
				App->CL_X_Brush->Brush_Lock_Textures(App->CL_Doc->CurBrush, false);
				m_FaceEditor->Update_Face_Info(hDlg);
				return TRUE;
			}

			return TRUE;
		}*/

		if (LOWORD(wParam) == IDC_BT_FACEDATA)
		{
			App->CL_Dialogs->Start_General_ListBox(3, m_FaceEditor->FaceDlg_Hwnd);// TODO: Enums::ListBox_Libraries);
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			m_FaceEditor->flag_FaceDlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			m_FaceEditor->flag_FaceDlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}

	return FALSE;
}

// *************************************************************************
// *			Update_Face_Info:- Terry Mo and Hazel 2025		           *
// *************************************************************************
void Face_Editor::Update_Face_Info(HWND hDlg)
{

}

// *************************************************************************
// *		 	List_Face_Data:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void Face_Editor::List_Face_Data(HWND List) // TODO: proper name
{
	char buff[MAX_PATH];
	Brush* pBrush;

	pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, 0);

	sprintf(buff, "%s %s     %s %i", "Brush Name: ", App->CL_X_Brush->Brush_GetName(pBrush), "Selected Face: ", App->CL_X_Face->Selected_Face_Index + 1);
	SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %d", "NumPoints", m_Selected_Face->NumPoints);
	SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s", "   ");
	SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %s", "Texture: ", App->CL_X_Face->Face_GetTextureName(m_Selected_Face));
	SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	int txSize, tySize;
	App->CL_X_Face->Face_GetTextureSize(m_Selected_Face, &txSize, &tySize);
	sprintf(buff, "%s %i %i", "Size: ", txSize, tySize);
	SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %i", "Bitmap ID: ", App->CL_X_Face->Face_GetTextureDibId(m_Selected_Face));
	SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s", "   ");
	SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	const TexInfo_Vectors* TVecs = App->CL_X_Face->Face_GetTextureVecs(m_Selected_Face);
	const T_Vec3* verts = App->CL_X_Face->Face_GetPoints(m_Selected_Face);

	T_Vec3 uVec, vVec;
	float U, V;

	App->CL_X_Maths->Vector3_Scale(&TVecs->uVec, 1.f / (float)txSize, &uVec);
	App->CL_X_Maths->Vector3_Scale(&TVecs->vVec, -1.f / (float)tySize, &vVec);

	int j = 0;
	for (j = 0; j < m_Selected_Face->NumPoints; j++)
	{
		U = App->CL_X_Maths->Vector3_DotProduct(&(verts[j]), &uVec);
		V = App->CL_X_Maths->Vector3_DotProduct(&(verts[j]), &vVec);
		U += (TVecs->uOffset / txSize);
		V -= (TVecs->vOffset / tySize);

		sprintf(buff, "UV %.3f %.3f", U, V);
		SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);
	}

	
	sprintf(buff, "%s", "   ");
	SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	bool Test_Lock = App->CL_X_Face->Face_IsTextureLocked(m_Selected_Face);

	if (Test_Lock)
	{
		sprintf(buff, "%s %s", "Locked: ", "Yes");
		SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);
	}
	else
	{
		sprintf(buff, "%s %s", "Locked: ", "No");
		SendMessage(List, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);
	}

}

// *************************************************************************
// *			Update_Faces:- Terry Mo and Hazel 2025		               *
// *************************************************************************
void Face_Editor::Update_Faces()
{
	if (App->CL_Top_Tabs->flag_All_Faces == true)
	{
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_3D);
		App->CL_Doc->SelectAllFacesInBrushes();
	}
	else
	{
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_3D);
		App->CL_X_SelFaceList->SelFaceList_RemoveAll(App->CL_Doc->pSelFaces);
		App->CL_Doc->SelectAllFacesInBrushes();
		App->CL_X_Face->Select_Face_From_Index(App->CL_X_Face->Selected_Face_Index);
	}

	App->CL_Ogre->RenderFrame(1);

	App->CL_X_Face_Editor->m_Selected_Face = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, App->CL_X_Face->Selected_Face_Index);
}

// *************************************************************************
// *		Fill_ComboBox_AngleValues:- Terry Mo and Hazel 2025	  		   *
// *************************************************************************
void Face_Editor::Fill_ComboBox_AngleValues(HWND hDlg)
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
// *		Fill_ComboBox_ScaleValues:- Terry Mo and Hazel 2025	  		   *
// *************************************************************************
void Face_Editor::Fill_ComboBox_ScaleValues(HWND hDlg)
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
// *		Fill_ComboBox_OffSetValues:- Terry Mo and Hazel 2025	  	   *
// *************************************************************************
void Face_Editor::Fill_ComboBox_OffSetValues(HWND hDlg)
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
// *			Update_Face_List:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void Face_Editor::Update_Face_List(HWND hDlg)
{
	int Count = 0;
	Face* pFace = NULL;
	char buff[MAX_PATH];

	SendDlgItemMessage(hDlg, IDC_LST_FACELIST, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	int SB = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

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

		SendDlgItemMessage(hDlg, IDC_LST_FACELIST, LB_SETCURSEL, (WPARAM)App->CL_X_Face->Selected_Face_Index, (LPARAM)0);

	}

	m_Selected_Face = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, App->CL_X_Face->Selected_Face_Index);
}

// *************************************************************************
// *			Update_Face_Members:- Terry Mo and Hazel 2025			   *
// *************************************************************************
void Face_Editor::Update_Face_Members()
{
	int NumberOfFaces = 0;
	m_Selected_Face = NULL;

	NumberOfFaces = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
	if (NumberOfFaces > 0)
	{
		m_Selected_Face = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, m_Selected_Face_Index);
	}
	else
	{
		m_Selected_Face = NULL;
		return;
	}

	if (NumberOfFaces && m_Selected_Face)
	{
		m_TextureAngle = App->CL_X_Face->Face_GetTextureRotate(m_Selected_Face);
		App->CL_X_Face->Face_GetTextureShift(m_Selected_Face, &m_TextureXOffset, &m_TextureYOffset);
		App->CL_X_Face->Face_GetTextureScale(m_Selected_Face, &m_TextureXScale, &m_TextureYScale);

		if (m_TextureXOffset < 0)
		{
			m_TextureXOffset = -m_TextureXOffset;
		}

		if (m_TextureYOffset < 0)
		{
			m_TextureYOffset = -m_TextureYOffset;
		}

		if (m_TextureAngle < 0)
		{
			m_TextureAngle = -m_TextureAngle;
		}

		//		m_TextureLock	=Face_IsTextureLocked (pFace);
		//		GetDlgItem( IDC_FACELIGHTINTENSITY )->EnableWindow( m_Light && !m_Sky) ;
		//		DisplayingNULL = FALSE;
		//		EnabledChange(TRUE);
	}
}

// *************************************************************************
// *				UpdateDialog:- Terry Mo and Hazel 2025		           *
// *************************************************************************
void Face_Editor::UpdateDialog(HWND hDlg)
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
// *			Change_Selection:- Terry Mo and Hazel 2025			       *
// *************************************************************************
void Face_Editor::Change_Selection()
{
	App->CL_X_Face_Editor->Update_Face_Members();
	App->CL_X_Face_Editor->UpdateDialog(FaceDlg_Hwnd);

	SendDlgItemMessage(FaceDlg_Hwnd, IDC_LST_FACELIST, LB_SETCURSEL, (WPARAM)App->CL_X_Face->Selected_Face_Index, (LPARAM)0);
}

// *************************************************************************
// *			Close_Faces_Dialog:- Terry Mo and Hazel 2025			   *
// *************************************************************************
void Face_Editor::Close_Faces_Dialog()
{
	flag_FaceDlg_Active = false;
	EndDialog(FaceDlg_Hwnd, 0);
}

// *************************************************************************
// *			Is_Faces_Dialog_Active:- Terry Mo and Hazel 2025		   *
// *************************************************************************
bool Face_Editor::Is_Faces_Dialog_Active()
{
	return flag_FaceDlg_Active;
}
