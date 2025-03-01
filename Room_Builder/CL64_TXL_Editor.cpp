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
	A_TXL_Dlg_HWND = NULL;

	BasePicWidth = 0;
	BasePicHeight = 0;
	BasePicDepth = 0;

	mFileName[0] = 0;
	mTextureName[0] = 0;
	Add_Texture_FileName[0] = 0;
	mFileString.clear();

	pData = NULL;
	Current_Entry = NULL;
	TXL_Dlg_HWND = NULL;

	Texture_Count = 0;

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
// *	  		Dialog_Text:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_TXL_Editor::A_Start_Texl_Dialog()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_TXL_EDITOR, App->MainHwnd, (DLGPROC)A_Proc_Texture_Lib);
}

// **************************************************************************
// *			Dialog_Text_Proc:- Terry and Hazel Flanigan 2025			*
// **************************************************************************
LRESULT CALLBACK CL64_TXL_Editor::A_Proc_Texture_Lib(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_TEXTURELIST2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_TEXTURE_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SetWindowLongPtr(GetDlgItem(hDlg, IDC_PREVIEW), GWLP_WNDPROC, (LONG_PTR)Proc_ViewerBasePic);

		App->CL_TXL_Editor->A_TXL_Dlg_HWND = hDlg;
		App->CL_TXL_Editor->A_Scan_Textures_Group();
		App->CL_TXL_Editor->A_UpDateList();

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_TEXTURE_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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
			App->CL_TXL_Editor->A_SelectBitmap();
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *		Proc_ViewerBasePic:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
bool CALLBACK CL64_TXL_Editor::Proc_ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		if (App->CL_TXL_Editor->Sel_BaseBitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = App->CL_TXL_Editor->BasePicHeight;
			Source.right = App->CL_TXL_Editor->BasePicWidth;

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CL_Textures->RenderTexture_Blit(hDC, App->CL_TXL_Editor->Sel_BaseBitmap, &Source, &Dest);
			ReleaseDC(hwnd, hDC);
		}

		EndPaint(hwnd, &ps);

		return 0;
	}

	return 0;
}

// *************************************************************************
// *			A_SelectBitmap:- Terry and Hazel Flanigan 2025 	  	   *
// *************************************************************************
bool CL64_TXL_Editor::A_SelectBitmap()
{
	int	Index = -1;

	int location = 0;

	Index = SendDlgItemMessage(A_TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (Index == -1)
	{
		return 0;
	}
	else
	{
		char buff[MAX_PATH]{ 0 };
		SendDlgItemMessage(A_TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETTEXT, (WPARAM)Index, (LPARAM)mTextureName);

		SetDlgItemText(A_TXL_Dlg_HWND, IDC_ST_TEXTURE_NAME, (LPCTSTR)mTextureName);
		
		Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
		Ogre::FileInfoList::const_iterator i, iend;
		iend = RFI->end();

		for (i = RFI->begin(); i != iend; ++i)
		{
			if (i->filename == mTextureName)
			{
				Ogre::DataStreamPtr ff = i->archive->open(i->filename);

				mFileString = ff->getAsString();

				char mFileName[MAX_PATH];
				strcpy(mFileName, App->RB_Directory_FullPath);
				strcat(mFileName, "\\Data\\");
				strcat(mFileName, mTextureName);

				std::ofstream outFile;
				outFile.open(mFileName, std::ios::binary);
				outFile << mFileString;
				outFile.close();

				mFileString.clear();

				Texture_To_HBITMP(mFileName);
				remove(mFileName);
				return 1;
			}
		}
	}

	return 0;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_TXL_Editor::Texture_To_HBITMP(char* TextureFileName)
{
	HWND PreviewWnd = GetDlgItem(A_TXL_Dlg_HWND, IDC_PREVIEW);
	HDC	hDC = GetDC(PreviewWnd);

	Sel_BaseBitmap = App->CL_Textures->Get_HBITMP(TextureFileName, hDC);

	BasePicWidth = App->CL_Textures->BasePicWidth;
	BasePicHeight = App->CL_Textures->BasePicHeight;
	//BasePicDepth = ilGetInteger(IL_IMAGE_DEPTH);

	RedrawWindow(A_TXL_Dlg_HWND, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *		Scan_Textures_Group:- Terry and Hazel Flanigan 2025			*
// *************************************************************************
bool CL64_TXL_Editor::A_Scan_Textures_Group()
{
	Vec_Texture_Name.resize(0);
	
	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();
	int Count = 0;

	for (i = RFI->begin(); i != iend; ++i)
	{
		Vec_Texture_Name.push_back(i->filename);
		Count++;
	}

	Texture_Count = Count;

	return 1;
}

// *************************************************************************
// *						UpDateList  13/06/08 					  	   *
// *************************************************************************
void CL64_TXL_Editor::A_UpDateList()
{
	int Index = 0;// SendDlgItemMessage(A_TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	int Count = 0;
	while (Count < Texture_Count)
	{
		SendDlgItemMessage(A_TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_ADDSTRING, (WPARAM)0, (LPARAM)Vec_Texture_Name[Count].c_str());
		Count++;
	}

	SendDlgItemMessage(A_TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_SETCURSEL, (WPARAM)Index, (LPARAM)0);

}

// *************************************************************************
// *						Create_New_pData 						  	   *
// *************************************************************************
void CL64_TXL_Editor::Create_New_pData()
{
	Delete_pData();

	pData = new TPack_WindowData;
	pData->BitmapCount = 0;
	pData->Bitmaps = NULL;
	pData->Dirty = 0;
	pData->FileNameIsValid = 0;
	pData->Instance = App->hInst;
	pData->SelectedEntry = NULL;

	strcpy(pData->TXLFileName, mFileName);
}

// *************************************************************************
// *							Delete_pData 						  	   *
// *************************************************************************
void CL64_TXL_Editor::Delete_pData()
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

// *************************************************************************
// *						UpDateList  13/06/08 					  	   *
// *************************************************************************
void CL64_TXL_Editor::UpDateList()
{
	int Index = SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	int	i;

	for (i = 0; i < pData->BitmapCount; i++)
	{

		if (NewBitmapList[i]->Deleted == 0)
		{
			SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_ADDSTRING, (WPARAM)0, (LPARAM)NewBitmapList[i]->Name);
		}
	}

	SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_SETCURSEL, (WPARAM)Index, (LPARAM)0);

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
// *				Check_if_Name_Exist:- Terry and Hazel Flanigan 2025 		  	   *
// *************************************************************************
int CL64_TXL_Editor::Check_if_Name_Exist(const char* Name)
{
	int	i;

	for (i = 0; i < pData->BitmapCount; i++)
	{

		if (!strcmp(Name, NewBitmapList[i]->Name))
		{
			return 1;
		}
	}

	return 0;
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
	//geBitmap_Info	MPInfo;
	//geBitmap_Info	MSInfo;

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

	//geBitmap_GetInfo(NewBitmapList[Location]->Bitmap, &MPInfo, &MSInfo);
	
}

// *************************************************************************
// *				Load_New:- Terry and Hazel Flanigan 2025 		 	   *
// *************************************************************************
signed int CL64_TXL_Editor::Load_New(const char* FileName)
{
	//DestroyBmps();

	pData = new TPack_WindowData;
	pData->BitmapCount = 0;

	geVFile* VFS = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, FileName, NULL, GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);
	if (!VFS)
	{
		//DestroyBmps();
		return false;
	}

	geVFile_Finder* Finder = geVFile_CreateFinder(VFS, "*.*");
	if (!Finder)
	{
		geVFile_Close(VFS);
		//DestroyBmps();
		return false;
	}

	while (geVFile_FinderGetNextFile(Finder) != GE_FALSE)
	{
		geVFile_Properties	Properties;

		geVFile_FinderGetProperties(Finder, &Properties);
		if (!AddTexture_New(VFS, Properties.Name))
		{
			geVFile_Close(VFS);
			//DestroyBmps();
			return false;
		}
	}

	geVFile_Close(VFS);

	return true;
}

// *************************************************************************
// *			AddTexture_New:- Terry and Hazel Flanigan 2025 		 	   *
// *************************************************************************
signed int CL64_TXL_Editor::AddTexture_New(geVFile* pVFS, const char* TextureName)
{
	geBitmap_Info	PInfo;
	geBitmap_Info	SInfo;
	geBitmap* pBitmap;
	geVFile* pTextureFile;

	if (pVFS)
		pTextureFile = geVFile_Open(pVFS, TextureName, GE_VFILE_OPEN_READONLY);
	else
		return false;

	if (!pTextureFile)
	{
		return false;
	}

	pBitmap = geBitmap_CreateFromFile(pTextureFile);
	geVFile_Close(pTextureFile);

	if (!pBitmap)
	{
		return false;
	}

	geBitmap_GetInfo(pBitmap, &PInfo, &SInfo);

	//Support several pixelformats. JJT
	if (PInfo.Format != GE_PIXELFORMAT_8BIT_GRAY
		&& PInfo.Format != GE_PIXELFORMAT_8BIT
		&& PInfo.Format != GE_PIXELFORMAT_16BIT_555_RGB
		&& PInfo.Format != GE_PIXELFORMAT_16BIT_565_RGB
		&& PInfo.Format != GE_PIXELFORMAT_16BIT_4444_ARGB
		&& PInfo.Format != GE_PIXELFORMAT_24BIT_RGB
		&& PInfo.Format != GE_PIXELFORMAT_24BIT_BGR

		)
	{
		//Default
		if (!geBitmap_SetFormat(pBitmap, GE_PIXELFORMAT_16BIT_555_RGB, GE_FALSE, 0, NULL))
		{
			geBitmap_Destroy(&pBitmap);
			return false;
		}
	}

	NewBitmapList[pData->BitmapCount] = new BitmapEntry;

	if (!NewBitmapList)
	{
		App->Report_Error("Memory allocation error processing %s", TextureName);
		return TRUE;
	}

	NewBitmapList[pData->BitmapCount]->Name = (LPSTR)TextureName;
	NewBitmapList[pData->BitmapCount]->Bitmap = pBitmap;
	NewBitmapList[pData->BitmapCount]->WinBitmap = NULL;
	NewBitmapList[pData->BitmapCount]->WinABitmap = NULL;
	NewBitmapList[pData->BitmapCount]->Flags = 0;
	NewBitmapList[pData->BitmapCount]->Deleted = 0;
	pData->BitmapCount++;

	return true;
}

// *************************************************************************
// *			AddBitmap_New:- Terry and Hazel Flanigan 2025 		 	   *
// *************************************************************************
signed int CL64_TXL_Editor::AddBitmap_New(const char* BitmapFileName)
{
	geBitmap_Info	PInfo;
	geBitmap_Info	SInfo;
	geBitmap* pBitmap = 0;
	geVFile* pBMPFile = 0;
	char FileName[MAX_PATH];
	//CString Msg;

	_splitpath(BitmapFileName, NULL, NULL, FileName, NULL);

	//int DuplicateTexture = FindTexture(FileName);
	//if (DuplicateTexture != -1)
	{
		/*AfxFormatString1(Msg, IDS_REPLACEBMPINWAD, FileName);
		if (AfxMessageBox(Msg, MB_YESNO | MB_ICONQUESTION) != IDYES)
			return GE_FALSE;

		CTexturePackBmp& CurrentTexture = Textures->ElementAt(DuplicateTexture);*/
		//geBitmap_Destroy(&CurrentTexture.pBmp);
		//Textures->RemoveAt(DuplicateTexture);
	}

	pBMPFile = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_DOS, BitmapFileName, NULL, GE_VFILE_OPEN_READONLY);

	if (!pBMPFile)
	{
		/*AfxFormatString1(Msg, IDS_CANTOPENBMP, FileName);
		AfxMessageBox(Msg, MB_OK | MB_ICONEXCLAMATION);*/

		return GE_FALSE;
	}

	pBitmap = geBitmap_CreateFromFile(pBMPFile);
	geVFile_Close(pBMPFile);

	if (!pBitmap)
	{
		/*AfxFormatString1(Msg, IDS_INVALIDBITMAPFILE, FileName);
		AfxMessageBox(Msg, MB_OK | MB_ICONEXCLAMATION);*/

		return GE_FALSE;
	}

	geBitmap_GetInfo(pBitmap, &PInfo, &SInfo);

	if ((PInfo.Width != PInfo.Height) ||
		(
			(PInfo.Width != 1) &&
			(PInfo.Width != 2) &&
			(PInfo.Width != 4) &&
			(PInfo.Width != 8) &&
			(PInfo.Width != 16) &&
			(PInfo.Width != 32) &&
			(PInfo.Width != 64) &&
			(PInfo.Width != 128) &&
			(PInfo.Width != 256)
			)
		)
	{
		/*CString SizeString;
		SizeString.Format("%d (width) by %d (height)", PInfo.Width, PInfo.Height);
		AfxFormatString2(Msg, IDS_BMPSIZEPROBLEM, FileName, SizeString);
		if (AfxMessageBox(Msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)*/
			return GE_FALSE;
	}

	//Support several pixelformats. JJT
	if (PInfo.Format != GE_PIXELFORMAT_8BIT_GRAY
		&& PInfo.Format != GE_PIXELFORMAT_8BIT
		&& PInfo.Format != GE_PIXELFORMAT_16BIT_555_RGB
		&& PInfo.Format != GE_PIXELFORMAT_16BIT_565_RGB
		&& PInfo.Format != GE_PIXELFORMAT_16BIT_4444_ARGB
		&& PInfo.Format != GE_PIXELFORMAT_24BIT_RGB
		&& PInfo.Format != GE_PIXELFORMAT_24BIT_BGR
		)
	{
		//Default
		if (!geBitmap_SetFormat(pBitmap, GE_PIXELFORMAT_16BIT_555_RGB, GE_FALSE, 0, NULL))
		{
			/*AfxFormatString1(Msg, IDS_FAILED16555RGB, FileName);
			AfxMessageBox(Msg, MB_OK | MB_ICONEXCLAMATION);*/

			geBitmap_Destroy(&pBitmap);
			return GE_FALSE;
		}
	}

	NewBitmapList[pData->BitmapCount] = new BitmapEntry;

	if (!NewBitmapList)
	{
		App->Report_Error("Memory allocation error processing %s", BitmapFileName);
		return TRUE;
	}

	NewBitmapList[pData->BitmapCount]->Name = (LPSTR)BitmapFileName;
	NewBitmapList[pData->BitmapCount]->Bitmap = pBitmap;
	NewBitmapList[pData->BitmapCount]->WinBitmap = NULL;
	NewBitmapList[pData->BitmapCount]->WinABitmap = NULL;
	NewBitmapList[pData->BitmapCount]->Flags = 0;
	NewBitmapList[pData->BitmapCount]->Deleted = 0;
	pData->BitmapCount++;
	//CTexturePackBmp NewTexture(FileName, pBitmap);
	//Textures->Add(NewTexture);

	return GE_TRUE;
}


