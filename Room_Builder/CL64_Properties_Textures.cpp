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

	Sel_BaseBitmap = NULL;
	BasePicWidth = NULL;
	BasePicHeight = NULL;

	mSelected_Face = NULL;
}

CL64_Properties_Textures::~CL64_Properties_Textures()
{
}

// *************************************************************************
// *	  	Show_Textures_Dialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Textures::Show_Textures_Dialog(bool Show)
{
	//if (App->CL_Properties_Textures->f_TextureDlg_Active == 1)
	{
		ShowWindow(Textures_Dlg_Hwnd, Show);
		Get_Selected_Face();
	}
}

// *************************************************************************
// *	  	Start_TextureDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Start_TextureDialog()
{
	Textures_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEXTURES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_TextureDialog);

	Dialog_Created = 1;
	//Set_Txl_FileName();
	Fill_ListBox();
	Get_BitMap();
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

		SendDlgItemMessage(hDlg, IDC_LISTTDTEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTDAPPLY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ADD_TEXTURE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TXL_FILE_EDIT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TEXTURE_FACE_PROPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STWIDTHHEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//App->CLSB_TextureDialog->f_TextureDlg_Active = 1;

		SetWindowLongPtr(GetDlgItem(hDlg, IDC_BASETEXTURE2), GWLP_WNDPROC, (LONG_PTR)ViewerBasePic);

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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BTTDAPPLY)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ADD_TEXTURE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_TXL_FILE_EDIT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TEXTURE_FACE_PROPS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TEXTURE_FACE_PROPS));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_GL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Ogre->OGL_Listener->Flag_Render_Brushes);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TEXTURE_FACE_PROPS)
		{
			int SF = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
			if (SF > 0)
			{
				App->CL_Properties_Faces->Start_FaceDialog();
			}
			else
			{
				App->Say("No Face Selected");
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_ADD_TEXTURE)
		{
			//App->CL_TXL_Editor->Load_Texture_Panel();
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_TXL_FILE_EDIT)
		{
			//App->CL_TXL_Editor->Start_Texl_Dialog();

			//App->CL_Level->Level_SetWadPath(App->CLSB_Doc->pLevel, Level_GetWadPath(App->CLSB_Doc->pLevel));
			//App->CL_World->Set_Current_TxlPath();
			App->CL_Doc->UpdateAfterWadChange();

			App->CL_Properties_Textures->Fill_ListBox();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_LISTTDTEXTURES)
		{
			if (App->CL_Properties_Textures->Dialog_Created == 1)
			{
				App->CL_Properties_Textures->List_Selection_Changed();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTTDAPPLY)
		{
			int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

			if (NumSelBrushes == 0)
			{
				App->Say("No Brushes Selected");
			}
			else
			{
				//App->CL_Doc->SelectAllFacesInBrushes();
				App->CL_Properties_Textures->Apply_Texture();

				App->CL_Doc->ResetAllSelectedFaces();
				App->CL_Top_Tabs->Select_Face();
				//App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
				

				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

			}
			return TRUE;
		}

		//// -----------------------------------------------------------------
		//if (LOWORD(wParam) == IDOK)
		//{
		//	//App->CL_TextureDialog->f_TextureDlg_Active = 0;
		//	//EndDialog(hDlg, LOWORD(wParam));
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDCANCEL)
		//{
		//	//App->CL_TextureDialog->f_TextureDlg_Active = 0;
		//	//EndDialog(hDlg, LOWORD(wParam));
		//	return TRUE;
		//}

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
	App->CL_Face->Face_SetTextureDibId(pFace, SelId);
	App->CL_Face->Face_SetTextureName(pFace, Name);
	App->CL_Face->Face_SetTextureSize(pFace, pbmp->Width, pbmp->Height);
}

// *************************************************************************
// *					( Static ) TextureBrush							   *
// *************************************************************************
static void TextureBrush(Brush* pBrush, int SelId, char const* Name, WadFileEntry* pbmp) // changed QD 12/03)
{
	int j;

	if (App->CL_Brush->Brush_IsMulti(pBrush))
	{
		TextureBrushList((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), SelId, Name, pbmp);
	}
	else
	{
		for (j = 0; j < App->CL_Brush->Brush_GetNumFaces(pBrush); ++j)
		{
			Face* pFace;

			pFace = App->CL_Brush->Brush_GetFace(pBrush, j);
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

	for (b = App->CL_Brush->BrushList_GetFirst(pList, &bi); b; b = App->CL_Brush->BrushList_GetNext(&bi))
	{
		TextureBrush(b, SelId, Name, pbmp); // changed QD 12/03
	}
}

// *************************************************************************
// *			Apply_Texture:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_Properties_Textures::Apply_Texture()
{
	int SelectedItem;
	int		i;

	char TextureName[MAX_PATH];

	SelectedItem = SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

	SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETTEXT, (WPARAM)SelectedItem, (LPARAM)TextureName);

	SelectedItem = Get_Index_FromName(TextureName);
	if (SelectedItem == -1)
	{
		App->Say("Cant Find Texture");
		return;
	}

	SelectedItem = SelectedItem;

	if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
	{
		return;
	}

	//App->m_pDoc->SetModifiedFlag();

	App->CL_Doc->mAdjustMode = ADJUST_MODE_FACE;

	switch (App->CL_Doc->mAdjustMode)
	{
	case ADJUST_MODE_FACE:
	{
		int Size;

		Size = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
		for (i = 0; i < Size; ++i)
		{
			Face* pFace;
			pFace = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, i);

			WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(m_CurrentTexture);
			TextureFace(pFace, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
			
		}
		
		int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
		for (i = 0; i < NumSelBrushes; ++i)
		{
			Brush* pBrush;

			pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
			App->CL_Brush->Brush_UpdateChildFaces(pBrush);
		}
		break;
	}

	case ADJUST_MODE_BRUSH:
	{
		if (App->CL_Doc->GetSelState() & MULTIBRUSH)
		{
			int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
			for (i = 0; i < NumSelBrushes; ++i)
			{
				Brush* pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
				
				WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(m_CurrentTexture);
				TextureBrush(pBrush, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
				
				App->CL_Brush->Brush_UpdateChildFaces(pBrush);
			}
		}
		else
		{
			
			WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(m_CurrentTexture);
			TextureBrush(App->CL_Doc->CurBrush, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
			
			App->CL_Brush->Brush_UpdateChildFaces(App->CL_Doc->CurBrush);
		}
		break;
	}

	default:
		return;
	}

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);

	/*if (App->CLSB_Equity->EquitySB_Dialog_Visible == 1)
	{
		App->CLSB_Mesh_Mgr->Update_World();
	}*/

}

// *************************************************************************
// *	  	Get_Index_FromName:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
int CL64_Properties_Textures::Get_Index_FromName(char* TextureName)
{
	CL64_WadFile* pWad;
	pWad = NULL;

	pWad = App->CL_Level->Level_GetWadFile(App->CL_Doc->pLevel);
	if (pWad == NULL)
	{
		App->Say("Error Getting Wad File");
		return -1;
	}

	for (int index = 0; index < pWad->mBitmapCount; index++)
	{
		char mName[MAX_PATH];
		strcpy(mName, pWad->mBitmaps[index].Name);

		bool test = strcmp(mName, TextureName);
		if (test == 0)
		{
			return index;
		}
	}

	return -1;
}

// *************************************************************************
// *	  	List_Selection_Changed:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Properties_Textures::List_Selection_Changed()
{
	int Index = SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (Index == LB_ERR)
	{
		App->Say("ListBox No Selection Available",(LPSTR)"");
	}
	else
	{
		
		char TextureName[MAX_PATH];
		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETTEXT, (WPARAM)Index, (LPARAM)TextureName);
		(void) strcpy(m_CurrentTexture, TextureName);
		//App->Say_Win(m_CurrentTexture);
		Get_BitMap();
		//Debug
	}

	char buf[255];
	sprintf(buf, "Index = %i        %i X %i", Index, BasePicWidth, BasePicHeight);
	SetDlgItemText(Textures_Dlg_Hwnd, IDC_STWIDTHHEIGHT, (LPCTSTR)buf);

	//IDC_STWIDTHHEIGHT
	//	Also set the current selection value in the document...
	//m_pDoc->mCurTextureSelection = SelNum;
}

// *************************************************************************
// *						ViewerBasePic Terry Flanigan	  			   *
// *************************************************************************
bool CALLBACK CL64_Properties_Textures::ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_PAINT)
	{
		PAINTSTRUCT	ps;
		HDC			hDC;
		RECT		Rect;

		hDC = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &Rect);
		Rect.left--;
		Rect.bottom--;
		FillRect(hDC, &Rect, (HBRUSH)(RGB(0, 255, 0)));

		if (App->CL_Properties_Textures->Sel_BaseBitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = App->CL_Properties_Textures->BasePicHeight;
			Source.right = App->CL_Properties_Textures->BasePicWidth;

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CL_Properties_Textures->RenderTexture_Blit(hDC, App->CL_Properties_Textures->Sel_BaseBitmap, &Source, &Dest);
			ReleaseDC(hwnd, hDC);

		}
		
		EndPaint(hwnd, &ps);
		return 0;
	}

	DefWindowProc(hwnd, msg, wParam, lParam);
}

// *************************************************************************
// *					RenderTexture_Blit Terry Bernie		  		   *
// *************************************************************************
bool CL64_Properties_Textures::RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect)
{
	HDC		MemDC;
	int		SourceWidth;
	int		SourceHeight;
	int		DestWidth;
	int		DestHeight;

	MemDC = CreateCompatibleDC(hDC);
	if (MemDC == NULL)
		return FALSE;

	if (Bmp)
	{
		SelectObject(MemDC, Bmp);

		SourceWidth = SourceRect->right - SourceRect->left;
		SourceHeight = SourceRect->bottom - SourceRect->top;
		DestWidth = DestRect->right - DestRect->left;
		DestHeight = DestRect->bottom - DestRect->top;
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
// *	  		Get_BitMap:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Properties_Textures::Get_BitMap()
{
	WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(m_CurrentTexture);

	HWND	PreviewWnd;
	HBITMAP	hbm;
	HDC		hDC;
	
	PreviewWnd = GetDlgItem(Textures_Dlg_Hwnd, IDC_BASETEXTURE2);
	hDC = GetDC(PreviewWnd);
	hbm = CreateHBitmapFromgeBitmap(BitmapPtr->bmp, hDC);

	if (geBitmap_HasAlpha(BitmapPtr->bmp))
	{
		//hbm = CreateHBitmapFromgeBitmap(BitmapPtr->bmp, hDC);
		hbm = CreateHBitmapFromgeBitmap(geBitmap_GetAlpha(BitmapPtr->bmp), hDC);
		if (hbm == NULL)
		{
			App->Say("Cant Assign Bitmap", (LPSTR)"");
			Sel_BaseBitmap = NULL;
			return;
		}

		Sel_BaseBitmap = hbm;
	}
	else
	{
		hbm = CreateHBitmapFromgeBitmap(BitmapPtr->bmp, hDC);
		if (hbm == NULL)
		{
			App->Say("Cant Assign Bitmap",(LPSTR)"");
			Sel_BaseBitmap = NULL;
			return;
		}

		Sel_BaseBitmap = hbm;
	}

	App->CL_Properties_Textures->BasePicHeight = BitmapPtr->Height;
	App->CL_Properties_Textures->BasePicWidth = BitmapPtr->Width;

	ReleaseDC(PreviewWnd, hDC);
	InvalidateRect(GetDlgItem(Textures_Dlg_Hwnd, IDC_BASETEXTURE2), NULL, TRUE);
}

// *************************************************************************
// *				CreateHBitmapFromgeBitmap  06/06/08 		  		   *
// *************************************************************************
HBITMAP CL64_Properties_Textures::CreateHBitmapFromgeBitmap(geBitmap* Bitmap, HDC hdc)
{
	geBitmap* Lock;
	gePixelFormat Format;
	geBitmap_Info info;
	HBITMAP hbm = NULL;

	// <> choose format to be 8,16,or 24, whichever is closest to Bitmap
	Format = GE_PIXELFORMAT_24BIT_BGR;

	if (geBitmap_GetBits(Bitmap))
	{
		Lock = Bitmap;
	}
	else
	{
		if (!geBitmap_LockForRead(Bitmap, &Lock, 0, 0, Format, GE_FALSE, 0))
		{
			return NULL;
		}
	}

	geBitmap_GetInfo(Lock, &info, NULL);

	if (info.Format != Format)
		return NULL;

	{
		void* bits;
		BITMAPINFOHEADER bmih;
		int pelbytes;

		pelbytes = gePixelFormat_BytesPerPel(Format);
		bits = geBitmap_GetBits(Lock);

		bmih.biSize = sizeof(bmih);
		bmih.biHeight = -info.Height;
		bmih.biPlanes = 1;
		bmih.biBitCount = 24;
		bmih.biCompression = BI_RGB;
		bmih.biSizeImage = 0;
		bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 10000;
		bmih.biClrUsed = bmih.biClrImportant = 0;

		if ((info.Stride * pelbytes) == (((info.Stride * pelbytes) + 3) & (~3)))
		{
			bmih.biWidth = info.Stride;
			hbm = CreateDIBitmap(hdc, &bmih, CBM_INIT, bits, (BITMAPINFO*)&bmih, DIB_RGB_COLORS);
		}
		else
		{
			void* newbits;
			int Stride;

			bmih.biWidth = info.Width;
			Stride = (((info.Width * pelbytes) + 3) & (~3));
			newbits = App->CL_Maths->Ram_Allocate(Stride * info.Height);
			if (newbits)
			{
				char* newptr, * oldptr;
				int y;

				newptr = (char*)newbits;
				oldptr = (char*)bits;
				for (y = 0; y < info.Height; y++)
				{
					memcpy(newptr, oldptr, (info.Width) * pelbytes);
					oldptr += info.Stride * pelbytes;
					newptr += Stride;
				}
				hbm = CreateDIBitmap(hdc, &bmih, CBM_INIT, newbits, (BITMAPINFO*)&bmih, DIB_RGB_COLORS);
				App->CL_Maths->Ram_Free(newbits);
			}
		}
	}

	if (Lock != Bitmap)
	{
		geBitmap_UnLock(Lock);
	}

	return hbm;
}

// *************************************************************************
// *			Fill_ListBox:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Fill_ListBox()
{
	int LBIndex;

	//if (f_TextureDlg_Active == 1)
	{
		CL64_WadFile* pWad;
		pWad = NULL;

		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		pWad = App->CL_Level->Level_GetWadFile(App->CL_Doc->pLevel);
		if (pWad == NULL)
		{
			App->Say("Error Getting Wad File",(LPSTR)"");
			return;
		}
		
		for (int index = 0; index < pWad->mBitmapCount; index++)
		{
			char mName[MAX_PATH];

			strcpy(mName, pWad->mBitmaps[index].Name);

			LBIndex = SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
		}

		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		
		//Set_Txl_FileName();
		//List_Selection_Changed();
	}
}

// *************************************************************************
// *	  	Get_Selected_Face:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Get_Selected_Face()
{
	mSelected_Face = NULL;

	int NumberOfFaces = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

	if (NumberOfFaces > 0)
	{
		mSelected_Face = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, (NumberOfFaces - 1));

		char buff[100];
		App->CL_Face->Face_GetTextureName(mSelected_Face);

		strcpy(buff, App->CL_Face->Face_GetTextureName(mSelected_Face));

		int Index = Get_Index_FromName(buff);

		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)buff);
		List_Selection_Changed();
	}
}

// *************************************************************************
// *		Enable_FaceProps_Button:- Terry and Hazel Flanigan 2025		*
// *************************************************************************
void CL64_Properties_Textures::Enable_FaceProps_Button(bool Enable)
{
	EnableWindow(GetDlgItem(Textures_Dlg_Hwnd, IDC_BT_TEXTURE_FACE_PROPS), Enable);
}
