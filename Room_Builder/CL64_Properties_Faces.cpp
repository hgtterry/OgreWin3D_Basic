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

	m_TextureAngle = 0;
	m_TextureXScale = 0;
	m_TextureYScale = 0;

	m_TextureYOffset = 0;
	m_TextureXOffset = 0;

	ScaleX_Delta = 0.01;
	ScaleY_Delta = 0.01;

	m_Selected_Face_Index = 0;

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
		SendDlgItemMessage(hDlg, IDC_LST_FACELIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_EDITXOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_EDITYOFFSET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_EDITXSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_EDITYSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_EDITANGLE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Properties_Faces->m_NumberOfFaces = 0;
		App->CL_Properties_Faces->m_NumberOfFaces = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

		char buf[10];
		sprintf(buf, "%i", App->CL_Properties_Faces->m_NumberOfFaces);
		SetDlgItemText(hDlg, IDC_ST_NUM_FACES, (LPCTSTR)buf);

		App->CL_Properties_Faces->Update_Face_List(hDlg);
		App->CL_Properties_Faces->Update_Face_Members();
		App->CL_Properties_Faces->UpdateDialog(hDlg);

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

		/*if (GetDlgItem(hDlg, IDC_TEXTURELOCK) == (HWND)lParam)
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
				App->CL_Properties_Faces->m_TextureAngle++;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pAngle = (float)App->CL_Properties_Faces->m_TextureAngle;

				//App->CL_Properties_Faces->m_Selected_Face->Tex.Rotate = pAngle;
				
				App->CL_Face->Face_SetTextureRotate(App->CL_Properties_Faces->m_Selected_Face, pAngle);
				App->CL_Properties_Faces->Update();
				
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Faces->m_TextureAngle--;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pAngle = (float)App->CL_Properties_Faces->m_TextureAngle;

				//App->CL_Properties_Faces->m_Selected_Face->Tex.Rotate = pAngle;

				App->CL_Face->Face_SetTextureRotate(App->CL_Properties_Faces->m_Selected_Face, pAngle);
				App->CL_Properties_Faces->Update();
				
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

				App->CL_Properties_Faces->m_TextureXOffset++;

				sprintf(buf, "%i", App->CL_Properties_Faces->m_TextureXOffset);
				SetDlgItemText(hDlg, IDC_ST_EDITXOFFSET, (LPCTSTR)buf);
			
				int pXOffset = (int)App->CL_Properties_Faces->m_TextureXOffset;
				int xOff, yOff;

				App->CL_Face->Face_GetTextureShift(App->CL_Properties_Faces->m_Selected_Face, &xOff, &yOff);
				App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, pXOffset, yOff);

				App->CL_Properties_Faces->Update();
				
				break;
			}

			case SB_LINELEFT:
			{
				char buf[255];

				App->CL_Properties_Faces->m_TextureXOffset--;
				
				sprintf(buf, "%i", App->CL_Properties_Faces->m_TextureXOffset);
				SetDlgItemText(hDlg, IDC_ST_EDITXOFFSET, (LPCTSTR)buf);

				int pXOffset = (int)App->CL_Properties_Faces->m_TextureXOffset;
				int xOff, yOff;

				App->CL_Face->Face_GetTextureShift(App->CL_Properties_Faces->m_Selected_Face, &xOff, &yOff);
				App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, pXOffset, yOff);

				App->CL_Properties_Faces->Update();
				
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
				App->CL_Properties_Faces->m_TextureYOffset++;
				App->CL_Properties_Faces->UpdateDialog(hDlg);

				int pYOffset = (int)App->CL_Properties_Faces->m_TextureYOffset;
				int xOff, yOff;

				App->CL_Face->Face_GetTextureShift(App->CL_Properties_Faces->m_Selected_Face, &xOff, &yOff);
				App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, xOff, pYOffset);

				App->CL_Properties_Faces->Update();
				
				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Faces->m_TextureYOffset--;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				int pYOffset = (int)App->CL_Properties_Faces->m_TextureYOffset;
				int xOff, yOff;

				App->CL_Face->Face_GetTextureShift(App->CL_Properties_Faces->m_Selected_Face, &xOff, &yOff);
				App->CL_Face->Face_SetTextureShift(App->CL_Properties_Faces->m_Selected_Face, xOff, pYOffset);

				App->CL_Properties_Faces->Update();
				
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
				App->CL_Properties_Faces->m_TextureXScale = App->CL_Properties_Faces->m_TextureXScale + App->CL_Properties_Faces->ScaleX_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pXScale = (float)App->CL_Properties_Faces->m_TextureXScale;
				float xScale, yScale;

				App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
				App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, pXScale, yScale);

				App->CL_Properties_Faces->Update();

				break;
			}
			case SB_LINELEFT:
			{
				App->CL_Properties_Faces->m_TextureXScale = App->CL_Properties_Faces->m_TextureXScale - App->CL_Properties_Faces->ScaleX_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pXScale = (float)App->CL_Properties_Faces->m_TextureXScale;
				float xScale, yScale;

				App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
				App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, pXScale, yScale);

				App->CL_Properties_Faces->Update();
				
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
				App->CL_Properties_Faces->m_TextureYScale = App->CL_Properties_Faces->m_TextureYScale + App->CL_Properties_Faces->ScaleY_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pYScale = (float)App->CL_Properties_Faces->m_TextureYScale;
				float xScale, yScale;

				App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
				App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, xScale, pYScale);

				App->CL_Properties_Faces->Update();

				break;
			}

			case SB_LINELEFT:
			{
				App->CL_Properties_Faces->m_TextureYScale = App->CL_Properties_Faces->m_TextureYScale - App->CL_Properties_Faces->ScaleY_Delta;
				App->CL_Properties_Faces->UpdateDialog(hDlg);
				
				float pYScale = (float)App->CL_Properties_Faces->m_TextureYScale;
				float xScale, yScale;

				App->CL_Face->Face_GetTextureScale(App->CL_Properties_Faces->m_Selected_Face, &xScale, &yScale);
				App->CL_Face->Face_SetTextureScale(App->CL_Properties_Faces->m_Selected_Face, xScale, pYScale);

				App->CL_Properties_Faces->Update();

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

// *************************************************************************
// *			Update:- Terry and Hazel Flanigan 2025                     *
// *************************************************************************
void CL64_Properties_Faces::Update()
{
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_3D);

	App->CL_SelFaceList->SelFaceList_RemoveAll(App->CL_Doc->pSelFaces);

	App->CL_Doc->SelectAllFacesInBrushes();

	App->CL_Face->Select_Face_From_Index(App->CL_Face->Selected_Face_Index);

	App->CL_Ogre->RenderFrame(2);

	App->CL_Properties_Faces->m_Selected_Face = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, App->CL_Face->Selected_Face_Index);
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
// *		Update_Face_Members:- Terry and Hazel Flanigan 2025			*
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
// *		  UpdateDialog:- Terry and Hazel Flanigan 2025				*
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
