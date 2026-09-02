/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_Properties_Textures.h"

CL64_Properties_Textures::CL64_Properties_Textures()
{
	Textures_Dlg_Hwnd = nullptr;
	strcpy(m_CurrentTexture, "stfloor1");

	Dialog_Created = 0;
	Dialog_Textures_Visible = 0;

	mFileString.clear();
	Selected_Index = 0;
	Sel_BaseBitmap = NULL;
	BasePicWidth = NULL;
	BasePicHeight = NULL;

	mSelected_Face = NULL;
}

CL64_Properties_Textures::~CL64_Properties_Textures()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Properties_Textures::Reset_Class()
{
	
}

// *************************************************************************
// *	  	Show_Textures_Dialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Textures::Show_Textures_Dialog(bool Show)
{
	ShowWindow(Textures_Dlg_Hwnd, Show);
	Dialog_Textures_Visible = Show;

	Get_Selected_Face();
}

// *************************************************************************
// *	  	Start_TextureDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Start_TextureDialog()
{
	Textures_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEXTURES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_TextureDialog);

	Dialog_Created = 1;
}

// *************************************************************************
// *        Proc_TextureDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Textures::Proc_TextureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_GD_TEXTURES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STWIDTHHEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_GD_TEXTURES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STWIDTHHEIGHT) == (HWND)lParam)
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

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_GL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Ogre->OGL_Listener->flag_Render_Groups);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		
		break;
	}
	}
	return FALSE;
}

static void TextureBrushList(BrushList* pList, int SelId, char const* Name, WadFileEntry* pbmp);

// *************************************************************************
// *					( Static ) TextureFace							   *
// *************************************************************************
static void TextureFace(Face* pFace, int SelId, char const* Name, WadFileEntry* pbmp)
{
	App->CL_X_Face->Face_SetTextureDibId(pFace, SelId);
	App->CL_X_Face->Face_SetTextureName(pFace, Name);
	App->CL_X_Face->Face_SetTextureSize(pFace, pbmp->Width, pbmp->Height);
}

// *************************************************************************
// *					( Static ) TextureBrush							   *
// *************************************************************************
static void TextureBrush(Brush* pBrush, int SelId, char const* Name, WadFileEntry* pbmp) // changed QD 12/03)
{
	int j;

	if (App->CL_X_Brush->Brush_IsMulti(pBrush))
	{
		TextureBrushList((BrushList*)App->CL_X_Brush->Brush_GetBrushList(pBrush), SelId, Name, pbmp);
	}
	else
	{
		for (j = 0; j < App->CL_X_Brush->Brush_GetNumFaces(pBrush); ++j)
		{
			Face* pFace;

			pFace = App->CL_X_Brush->Brush_GetFace(pBrush, j);
			TextureFace(pFace, SelId, Name, pbmp); 
		}
	}
}

// *************************************************************************
// *					( Static ) TextureBrushList						   *
// *************************************************************************
static void TextureBrushList(BrushList* pList, int SelId, char const* Name, WadFileEntry* pbmp)
{
	Brush* b;
	BrushIterator bi;

	for (b = App->CL_X_Brush->BrushList_GetFirst(pList, &bi); b; b = App->CL_X_Brush->BrushList_GetNext(&bi))
	{
		TextureBrush(b, SelId, Name, pbmp); // changed QD 12/03
	}
}

// *************************************************************************
// *		 GetIndexFromTextureName:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
int CL64_Properties_Textures::GetIndexFromTextureName(char* TextureName)
{
	CL64_WadFile* pWad = App->CL_Level->Level_GetWad_Class();
	
	// Check 
	if (pWad == nullptr) 
	{
		App->Say("Error Getting Wad Class");
		return -1;
	}

	// Search Textures
	for (int index = 0; index < pWad->mBitmapCount; index++)
	{
		char mName[MAX_PATH];
		strcpy(mName, pWad->mBitmaps[index].Name);

		bool test = strcmp(mName, TextureName);
		if (test == 0)
		{
			// Found return texture index
			return index;
		}
	}

	// No Texture Found
	return -1;
}

// *************************************************************************
// *		RenderTexture_Blit:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
bool CL64_Properties_Textures::RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect)
{
	HDC MemDC = CreateCompatibleDC(hDC);
	if (MemDC == NULL)
	{
		return FALSE;
	}

	// Check if the bitmap is valid
	if (Bmp)
	{
		SelectObject(MemDC, Bmp);

		int SourceWidth = SourceRect->right - SourceRect->left;
		int SourceHeight = SourceRect->bottom - SourceRect->top;
		int DestWidth = DestRect->right - DestRect->left;
		int DestHeight = DestRect->bottom - DestRect->top;
		SetStretchBltMode(hDC, COLORONCOLOR);
		StretchBlt(hDC,
			DestRect->left,
			DestRect->top,
			DestHeight,
			DestHeight,
			MemDC,
			SourceRect->left,
			SourceRect->top,
			SourceWidth,
			SourceHeight,
			SRCCOPY);
	}

	DeleteDC(MemDC);

	return TRUE;
}

// *************************************************************************
// *	  	Get_Selected_Face:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Get_Selected_Face()
{
	mSelected_Face = NULL;

	int NumberOfFaces = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

	if (NumberOfFaces > 0)
	{
		mSelected_Face = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, (NumberOfFaces - 1));

		Select_With_TextureName(App->CL_X_Face->Face_GetTextureName(mSelected_Face));
	}
}

// *************************************************************************
// *	  	Select_With_TextureName:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Textures::Select_With_TextureName(const char* TextureName)
{
	SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)TextureName);
	
	strcpy(m_CurrentTexture, TextureName);
}

// *************************************************************************
// *		Select_With_List_Index:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
void CL64_Properties_Textures::Select_With_List_Index(int Index)
{
	SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_SETCURSEL, (WPARAM)Index, (LPARAM)0);
}

