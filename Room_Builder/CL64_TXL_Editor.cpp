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
#include "CL64_TXL_Editor.h"

CL64_TXL_Editor::CL64_TXL_Editor()
{
	FileName[0] = 0;
	TextureName[0] = 0;

	pData = NULL;
	Entry = NULL;
	TXL_Dlg_HWND = NULL;

	int Count = 0;
	while (Count < 199)
	{
		NewBitmapList[Count] = NULL;
		Count++;
	}
}

CL64_TXL_Editor::~CL64_TXL_Editor()
{
}

// *************************************************************************
// *			Start_Texl_Dialog:- Terry and Hazel Flanigan 2025   	   *
// *************************************************************************
void CL64_TXL_Editor::Start_Texl_Dialog()
{
	strcpy(FileName, App->CL_Doc->mCurrent_TXL_Path_And_File);

	DialogBox(App->hInst, (LPCTSTR)IDD_TEXTURE_EDITOR, App->MainHwnd, (DLGPROC)Proc_Texture_Lib);
}

// *************************************************************************
// *			Proc_Texture_Lib:- Terry and Hazel Flanigan 2025  		   *
// *************************************************************************
LRESULT CALLBACK CL64_TXL_Editor::Proc_Texture_Lib(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//SendDlgItemMessage(hDlg, IDC_STTEXTURE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_STALPHA, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_TEXTURELIST2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_GEINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_ADD_TEXTURE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DELETE_TEXTURE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		/*SendDlgItemMessage(hDlg, IDC_EXPORTSELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/

		App->CL_TXL_Editor->TXL_Dlg_HWND = hDlg;

		App->CL_TXL_Editor->Entry = new BitmapEntry;
		App->CL_TXL_Editor->Entry->Bitmap = NULL;
		App->CL_TXL_Editor->Entry->WinABitmap = NULL;
		App->CL_TXL_Editor->Entry->WinBitmap = NULL;
		App->CL_TXL_Editor->Entry->Deleted = 0;
		App->CL_TXL_Editor->Entry->Dirty = 0;
		App->CL_TXL_Editor->Entry->Flags = 0;

		App->CL_TXL_Editor->pData = new TPack_WindowData;
		App->CL_TXL_Editor->pData->BitmapCount = 0;
		App->CL_TXL_Editor->pData->Bitmaps = NULL;
		App->CL_TXL_Editor->pData->Dirty = 0;
		App->CL_TXL_Editor->pData->FileNameIsValid = 0;
		App->CL_TXL_Editor->pData->Instance = App->hInst;
		App->CL_TXL_Editor->pData->SelectedEntry = NULL;
		strcpy(App->CL_TXL_Editor->pData->TXLFileName, App->CL_TXL_Editor->FileName);

		char buf1[200];
		strcpy(buf1, "Texture library: - ");
		strcat(buf1, App->CL_TXL_Editor->FileName);
		
		bool Test = App->CL_TXL_Editor->LoadFile(hDlg);

		SetWindowText(hDlg, buf1);

		SetWindowLongPtr(GetDlgItem(hDlg, IDC_PREVIEW), GWLP_WNDPROC, (LONG_PTR)TextureLibPreviewWnd);

		return 1;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_STTEXTURE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STALPHA) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		break;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_ADD_TEXTURE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_DELETE_TEXTURE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		/*if (some_item->idFrom == IDC_EXPORTSELECTED && some_item->code == NM_CUSTOMDRAW)
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

	case WM_COMMAND:
	{

		//-------------------Click in Texture List Box ----------------
		if (LOWORD(wParam) == IDC_TEXTURELIST2)
		{
			App->CL_TXL_Editor->SelectBitmap();
			return TRUE;
		}

		//--------------------------------- Add -----------------------
		if (LOWORD(wParam) == IDC_BT_ADD_TEXTURE)
		{
			/*int test = App->CLSB_FileIO->Open_File_Model(App->MainHwnd, "Texture Files ( *.bmp *.tga )\0*.bmp;*.tga\0*.tga\0*.tga\0*.bmp\0*.bmp\0", "Add Texture", "Bitmap Files");
			if (test == 0)
			{
				return TRUE;
			}

			strcpy(App->CL_TxlEditor->Add_Texture_FileName, App->CLSB_FileIO->FileName);

			App->CL_TxlEditor->AddTexture(NULL, App->CL_TxlEditor->Add_Texture_FileName);
			App->CL_TxlEditor->pData->Dirty = 1;*/

			return TRUE;
		}

		//--------------------------------- Delete -----------------------
		if (LOWORD(wParam) == IDC_BT_DELETE_TEXTURE)
		{

			//int Index = SendDlgItemMessage(App->CL_TxlEditor->pData->hwnd, IDC_TEXTURELIST2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			//if (Index == -1)
			//{
			//	return TRUE;
			//}

			////App->CL_TxlEditor->Current_Entry->Deleted = 1;

			//App->CL_TxlEditor->NewBitmapList[Index]->Deleted = 1;

			//App->CL_TxlEditor->UpDateList();
			//App->CL_TxlEditor->SelectBitmap();

			//App->CL_TxlEditor->pData->Dirty = 1;

			return TRUE;
		}

		////--------------------------------- Save ----------------------
		//if (LOWORD(wParam) == IDC_SAVE)
		//{

		//	App->CL_Texture_Lib->Save(App->CL_Texture_Lib->pData->TXLFileName);
		//	return TRUE;
		//}
		////--------------------------------- Rename ----------------------
		//if (LOWORD(wParam) == IDC_RENAME)
		//{

		//	//DialogBox(hInst, (LPCTSTR)IDD_RENAME, Fdlg, (DLGPROC)RenameProc);
		//	//App->CL_Texture_Lib->pData->Dirty = 1;  // it as changed reqest save
		//	return TRUE;
		//}
		////--------------------------------- Save AS --------------------
		/*if (LOWORD(wParam) == IDC_TEST)
		{
			App->CL_TxlEditor->CreateTexture(NULL, App->CL_TxlEditor->Add_Texture_FileName);
			App->CL_TxlEditor->pData->Dirty = 1;
			return TRUE;
		}*/

		//--------------------------------- IDC_LOAD --------------------
		/*if (LOWORD(wParam) == IDC_BTTXLOPEN)
		{
			int test = App->CLSB_FileIO->Open_File_Model(App->MainHwnd, "Texture Libary   *.txl\0*.txl\0", "Texure Editor", NULL);
			if (test == 0)
			{
				return 1;
			}

			strcpy(App->CL_TxlEditor->FileName, App->CLSB_FileIO->FileName);
			SendDlgItemMessage(App->CL_TxlEditor->pData->hwnd, IDC_TEXTURELIST2, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

			App->CL_TxlEditor->CleanUp();

			bool Test = App->CL_TxlEditor->LoadFile(hDlg);

			if (Test == 0)
			{
				App->Say("Failed");
				return 0;
			}
			return TRUE;
		}*/

		//--------------------------------- EXPORTSELECTED --------------------
		/*if (LOWORD(wParam) == IDC_EXPORTSELECTED)
		{
			App->CL_TxlEditor->TPack_ExtractSelected();
			return TRUE;
		}*/

		//--------------------------------- Export All -----------------
		//if (LOWORD(wParam) == IDC_EXPORTALL)
		//{
		//	/*char File[256];

		//	_getcwd(App->CL_Texture_Lib->LastPath, 256);
		//	_splitpath(App->CL_Texture_Lib->pData->TXLFileName, NULL, NULL, File, NULL);
		//	strcat(File, "_Textures");

		//	strcpy(S_File[0]->NewSubFolderName, File);

		//	strcpy(S_File[0]->SaveMessage, "Select Folder To Place Decompiled Textures");

		//	int Test = GetSaveFolder();
		//	if (Test == 0)
		//	{
		//		return 0;
		//	}

		//	App->CL_Texture_Lib->TPack_ExtractAll();*/

		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{

			/*if (App->CL_TxlEditor->pData->Dirty)
			{
				int	Result;

				Result = MessageBox(NULL,
					"Do you want to save changes before quitting?",
					"Texture Packer",
					MB_YESNOCANCEL);

				if (Result == IDCANCEL)
				{
					return 0;
				}

				if (Result == IDYES)
				{
					App->CL_TxlEditor->Save(App->CL_TxlEditor->pData->TXLFileName);
				}
			}

			App->CL_TxlEditor->CleanUp();*/

			if (App->CL_TXL_Editor->pData)
			{
				delete App->CL_TXL_Editor->pData;
				App->CL_TXL_Editor->pData = NULL;
			}

			if (App->CL_TXL_Editor->Entry)
			{
				delete App->CL_TXL_Editor->Entry;
				App->CL_TXL_Editor->Entry = NULL;
			}

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

	}
	break;
	}

	return FALSE;
}

// *************************************************************************
// *					TextureLibPreviewWnd							   *
// *************************************************************************
bool CALLBACK CL64_TXL_Editor::TextureLibPreviewWnd(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		FillRect(hDC, &Rect, (HBRUSH)(COLOR_WINDOW + 1));

		if (App->CL_TXL_Editor->Entry->Bitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = geBitmap_Height(App->CL_TXL_Editor->Entry->Bitmap);
			Source.right = geBitmap_Width(App->CL_TXL_Editor->Entry->Bitmap);

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CL_TXL_Editor->Render2d_Blit(hDC,
				App->CL_TXL_Editor->Entry->WinBitmap,
				App->CL_TXL_Editor->Entry->WinABitmap,
				&Source,
				&Dest);

			ReleaseDC(hwnd, hDC);
		}

		EndPaint(hwnd, &ps);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// *************************************************************************
// *							Render2d_Blit		 			  		   *
// *************************************************************************
bool CL64_TXL_Editor::Render2d_Blit(HDC hDC, HBITMAP Bmp, HBITMAP Alpha, const RECT* SourceRect, const RECT* DestRect)
{
	HDC		MemDC;
	int		SourceWidth;
	int		SourceHeight;
	int		DestWidth;
	int		DestHeight;

	MemDC = CreateCompatibleDC(hDC);
	if (MemDC == NULL)
		return FALSE;

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
	if (Alpha)
	{
		SelectObject(MemDC, Alpha);
		SetStretchBltMode(hDC, COLORONCOLOR);
		StretchBlt(hDC,
			DestRect->left + DestHeight + 2,
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
// *			LoadFile:- Terry and Hazel Flanigan 2025		  		   *
// *************************************************************************
bool CL64_TXL_Editor::LoadFile(HWND ChDlg)
{
	geVFile* VFS;
	geVFile_Finder* Finder = NULL;

	pData = new TPack_WindowData;
	pData->BitmapCount = 0;

	int TextureCount = 0;

	VFS = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, FileName, NULL, GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);
	if (!VFS)
	{
		App->Say("Could not open file %s");
		return 0;
	}

	geVFile_Finder* Finder2;
	Finder2 = geVFile_CreateFinder(VFS, "*.*");
	if (!Finder2)
	{
		App->Say("XX Could not load textures from %s");
		geVFile_Close(VFS);
		return 0;
	}

	while (geVFile_FinderGetNextFile(Finder2) != GE_FALSE)
	{

		TextureCount++;

	}

	Finder = geVFile_CreateFinder(VFS, "*.*");
	if (!Finder)
	{
		App->Say("Could not load textures from %s");
		geVFile_Close(VFS);
		return 0;
	}

	while (geVFile_FinderGetNextFile(Finder) != GE_FALSE)
	{
		geVFile_Properties	Properties;

		geVFile_FinderGetProperties(Finder, &Properties);
		if (!AddTexture(VFS, Properties.Name))
		{
			geVFile_Close(VFS);
			return 0;
		}
	}

	strcpy(pData->TXLFileName, FileName);
	pData->FileNameIsValid = TRUE;
	pData->Dirty = FALSE;
	geVFile_Close(VFS);

	SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	SelectBitmap();

	return 1;
}

// *************************************************************************
// *				AddTexture:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_TXL_Editor::AddTexture(geVFile* BaseFile, const char* Path)
{
	geBitmap_Info	PInfo;
	geBitmap_Info	SInfo;
	geBitmap* Bitmap;

	geVFile* File;
	char			FileName[_MAX_FNAME];
	char* Name;

	Bitmap = NULL;
	File = NULL;

	_splitpath(Path, NULL, NULL, FileName, NULL);
	Name = _strdup(FileName);
	if (!Name)
	{
		NonFatalError("Memory allocation error processing %s", Path);
		return FALSE;
	}

	if (BaseFile)
		File = geVFile_Open(BaseFile, Path, GE_VFILE_OPEN_READONLY);
	else
		File = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_DOS, Path, NULL, GE_VFILE_OPEN_READONLY);

	if (!File)
	{
		NonFatalError("Could not open %s", Path);
		return TRUE;
	}
	//geBitmap_Create()
	Bitmap = geBitmap_CreateFromFile(File);
	geVFile_Close(File);
	if (!Bitmap)
	{
		NonFatalError("%s is not a valid bitmap", Path);
		return TRUE;
	}
	geBitmap_GetInfo(Bitmap, &PInfo, &SInfo);
	//	if	(PInfo.Format != GE_PIXELFORMAT_8BIT)
	//	{
	//		NonFatalError("%s is not an 8bit bitmap", Path);
	//		goto fail;
	//	}
	//	NewBitmapList = geRam_Realloc(pData->Bitmaps, sizeof(*NewBitmapList) * (pData->BitmapCount + 1));
	NewBitmapList[pData->BitmapCount] = new BitmapEntry;

	if (!NewBitmapList)
	{
		NonFatalError("Memory allocation error processing %s", Path);
		return TRUE;
	}

	NewBitmapList[pData->BitmapCount]->Name = Name;
	NewBitmapList[pData->BitmapCount]->Bitmap = Bitmap;
	NewBitmapList[pData->BitmapCount]->WinBitmap = NULL;
	NewBitmapList[pData->BitmapCount]->WinABitmap = NULL;
	NewBitmapList[pData->BitmapCount]->Flags = 0;
	NewBitmapList[pData->BitmapCount]->Deleted = 0;
	pData->BitmapCount++;

	SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_ADDSTRING, (WPARAM)0, (LPARAM)Name);

	return TRUE;
}

// *************************************************************************
// *			SelectBitmap:- Terry and Hazel Flanigan 2025 	  		   *
// *************************************************************************
bool CL64_TXL_Editor::SelectBitmap()
{
	int		Index;

	int location = 0;

	Index = SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (Index == -1)
	{
		return 0;
	}
	else
	{
		char buff[256]{ 0 };
		SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETTEXT, (WPARAM)Index, (LPARAM)TextureName);
		
		location = FindBitmap(pData, TextureName);
		
		if (!NewBitmapList[location]->WinBitmap)
		{
			HWND	PreviewWnd;
			HBITMAP	hbm;
			HBITMAP	ahbm;
			HDC		hDC;

			PreviewWnd = GetDlgItem(TXL_Dlg_HWND, IDC_PREVIEW);
			hDC = GetDC(PreviewWnd);
			hbm = CreateHBitmapFromgeBitmap(NewBitmapList[location]->Bitmap, hDC);
			NewBitmapList[location]->WinBitmap = hbm;

			if (geBitmap_HasAlpha(NewBitmapList[location]->Bitmap))
			{
				ahbm = CreateHBitmapFromgeBitmap(geBitmap_GetAlpha(NewBitmapList[location]->Bitmap), hDC);
				NewBitmapList[location]->WinABitmap = ahbm;
			}
			ReleaseDC(PreviewWnd, hDC);
		}
	}

	InvalidateRect(GetDlgItem(TXL_Dlg_HWND, IDC_PREVIEW), NULL, TRUE);

	Entry = NewBitmapList[location];
	UpDateGeList(location);

	return 0;

}

// *************************************************************************
// *				FindBitmap:- Terry and Hazel Flanigan 2025 		  	   *
// *************************************************************************
int CL64_TXL_Editor::FindBitmap(TPack_WindowData* pData, const char* Name)
{
	int	i;

	for (i = 0; i < pData->BitmapCount; i++)
	{

		if (!strcmp(Name, NewBitmapList[i]->Name))
		{
			return i;
		}
	}

	return -1;
}

// *************************************************************************
// *	CreateHBitmapFromgeBitmap:- Terry and Hazel Flanigan 2025    	   *
// *************************************************************************
HBITMAP CL64_TXL_Editor::CreateHBitmapFromgeBitmap(geBitmap* Bitmap, HDC hdc)
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
// *			UpDateGeList:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_TXL_Editor::UpDateGeList(int Location)
{
	int B = 0;
	geBitmap_Info	MPInfo;
	geBitmap_Info	MSInfo;

	char buff[256];
	strcpy(buff, "no info");
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	sprintf(buff, "%s %s", "Texture Name :-", NewBitmapList[Location]->Name);
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %i", "Index :-", Location);
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %s", "Bitmap :-", "Valid");
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %d", "Width :-", geBitmap_Width(NewBitmapList[Location]->Bitmap));
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %d", "Height :-", geBitmap_Height(NewBitmapList[Location]->Bitmap));
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);


	geBitmap_GetInfo(NewBitmapList[Location]->Bitmap, &MPInfo, &MSInfo);
	/*geBitmap_GetAverageColor(NewBitmapList[Location]->Bitmap,&R,&G,&B);
	sprintf(buff, "%s %i","RGB :-",MPInfo->Format);
	SendDlgItemMessage(pData->hwnd, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);*/
}

// *************************************************************************
// *						NonFatalError   					  		   *
// *************************************************************************
bool CL64_TXL_Editor::NonFatalError(const char* Msg, ...)
{
	char Buffer[1024];
	va_list argptr;

	va_start(argptr, Msg);
	vsprintf(Buffer, Msg, argptr);
	va_end(argptr);

	MessageBox(NULL, Buffer, "Error", MB_ICONEXCLAMATION | MB_OK);
	return 1;
}
