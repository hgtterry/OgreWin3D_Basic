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

CL64_Properties_Faces::CL64_Properties_Faces(void)
{
	FaceDlg_Hwnd = nullptr;

	mNumberOfFaces = 0;

	f_FaceDlg_Active = 0;
}

CL64_Properties_Faces::~CL64_Properties_Faces(void)
{
}

// *************************************************************************
// *	  	Start_FaceDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Faces::Start_FaceDialog()
{
	if (f_FaceDlg_Active == 0)
	{
		FaceDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_FACES, App->MainHwnd, (DLGPROC)Proc_FaceDialog);
		f_FaceDlg_Active = 1;
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
		
		App->CL_Properties_Faces->mNumberOfFaces = 0;
		App->CL_Properties_Faces->mNumberOfFaces = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

		char buf[10];
		sprintf(buf, "%i", App->CL_Properties_Faces->mNumberOfFaces);
		SetDlgItemText(hDlg, IDC_ST_NUM_FACES, (LPCTSTR)buf);

		/*SendDlgItemMessage(hDlg, IDC_STTEXT2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_LISTFACEPROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_FaceDialog->UpdatePolygonFocus();

		App->CL_FaceDialog->UpdateDialog(hDlg);

		App->CL_FaceDialog->Update_FaceProperties_Dlg(hDlg);

		HWND CB_hWnd = GetDlgItem(hDlg, IDC_CBXOFFSET);
		App->CL_FaceDialog->Fill_ComboBox_OffSetValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBYOFFSET);
		App->CL_FaceDialog->Fill_ComboBox_OffSetValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBXSCALE);
		App->CL_FaceDialog->Fill_ComboBox_ScaleValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBYSCALE);
		App->CL_FaceDialog->Fill_ComboBox_ScaleValues(CB_hWnd);

		CB_hWnd = GetDlgItem(hDlg, IDC_CBANGLE);
		App->CL_FaceDialog->Fill_ComboBox_AngleValues(CB_hWnd);*/

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_STTEXT2) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTEXT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

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

		/*if (GetDlgItem(hDlg, IDC_LABEL_TEXTURE_OFFSET_SCALEX) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_LABEL_TEXTURE_OFFSET_SCALEY) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_LABEL_TEXTURE_OFFSET_SCALEANGLE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_TEXTURELOCK) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_FLIPHORIZONTAL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_FLIPVERTICAL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_VSCROLL:
	{
		// -------- Angle
		/*if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBANGLE_UNIT))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINEUP:

				App->CL_FaceDialog->m_TextureAngle++;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusAngle();

				break;

			case SB_LINEDOWN:
				App->CL_FaceDialog->m_TextureAngle--;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusAngle();

				break;
			}

			return 0;
		}*/

		// -------- Offset X
		/*if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBXOFFSET_UNIT))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINEUP:

				App->CL_FaceDialog->m_TextureXOffset++;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusXOffset();

				break;

			case SB_LINEDOWN:
				App->CL_FaceDialog->m_TextureXOffset--;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusXOffset();

				break;
			}

			return 0;
		}*/

		// -------- Offset Y
		/*if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBYOFFSET_UNIT))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINEUP:

				App->CL_FaceDialog->m_TextureYOffset++;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusYOffset();

				break;

			case SB_LINEDOWN:
				App->CL_FaceDialog->m_TextureYOffset--;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusYOffset();

				break;
			}

			return 0;
		}*/

		// -------- Scale Y
		/*if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBYSCALE_UNIT))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINEUP:

				App->CL_FaceDialog->m_TextureYScale = App->CL_FaceDialog->m_TextureYScale + App->CL_FaceDialog->ScaleY_Delta;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusYScale();

				break;

			case SB_LINEDOWN:
				App->CL_FaceDialog->m_TextureYScale = App->CL_FaceDialog->m_TextureYScale - App->CL_FaceDialog->ScaleY_Delta;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusYScale();

				break;
			}

			return 0;
		}*/

		// -------- Scale X
		/*if (HWND(lParam) == GetDlgItem(hDlg, IDC_SBXSCALE_UNIT))
		{
			switch ((int)LOWORD(wParam))
			{
			case SB_LINEUP:

				App->CL_FaceDialog->m_TextureXScale = App->CL_FaceDialog->m_TextureXScale + App->CL_FaceDialog->ScaleX_Delta;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusXScale();

				break;

			case SB_LINEDOWN:
				App->CL_FaceDialog->m_TextureXScale = App->CL_FaceDialog->m_TextureXScale - App->CL_FaceDialog->ScaleX_Delta;
				App->CL_FaceDialog->UpdateDialog(hDlg);
				App->CL_FaceDialog->OnKillfocusXScale();

				break;
			}

			return 0;
		}*/

		return 0;
	}

	case WM_COMMAND:
	{

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

		//if (LOWORD(wParam) == IDC_CBXSCALE)
		//{
		//	switch (HIWORD(wParam)) // Find out what message it was
		//	{
		//	case CBN_DROPDOWN:
		//		break;
		//	case CBN_CLOSEUP:
		//	{
		//		char buff[MAX_PATH]{ 0 };

		//		HWND temp = GetDlgItem(hDlg, IDC_CBXSCALE);
		//		int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
		//		SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

		//		App->CL_FaceDialog->ScaleX_Delta = atof(buff);
		//	}
		//	}

		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_CBYSCALE)
		//{
		//	switch (HIWORD(wParam)) // Find out what message it was
		//	{
		//	case CBN_DROPDOWN:
		//		break;
		//	case CBN_CLOSEUP:
		//	{
		//		char buff[MAX_PATH]{ 0 };

		//		HWND temp = GetDlgItem(hDlg, IDC_CBYSCALE);
		//		int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
		//		SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

		//		App->CL_FaceDialog->ScaleY_Delta = atof(buff);
		//	}
		//	}

		//	return TRUE;
		//}

		/*if (LOWORD(wParam) == IDC_FLIPHORIZONTAL)
		{
			App->CL_FaceDialog->On_FlipHorizontal();
			App->CL_FaceDialog->Update_FaceProperties_Dlg(hDlg);
			return TRUE;
		}*/

		/*if (LOWORD(wParam) == IDC_FLIPVERTICAL)
		{
			App->CL_FaceDialog->OnFlipvertical();
			App->CL_FaceDialog->Update_FaceProperties_Dlg(hDlg);
			return TRUE;
		}*/

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Properties_Faces->f_FaceDlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Faces->f_FaceDlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}
