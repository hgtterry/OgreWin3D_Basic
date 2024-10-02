/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Textures.h"

#include "il.h"
#include "ilu.h"
#include "ilut.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CL64_Textures::CL64_Textures(void)
{
	ilInit();

	TextureFileName[0] = 0;
	Just_Texture_FileName[0] = 0;

	BasePicWidth = 0;
	BasePicHeight = 0;
	BasePicDepth = 0;

	int Count = 0;
	while (Count < 499)
	{
		g_Texture[0] = 0; // Texture IDs
		Count++;
	}

}

CL64_Textures::~CL64_Textures(void)
{
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Textures::Texture_To_HBITMP(char* TextureFileName)
{
	HWND PreviewWnd = GetDlgItem(App->CL_Dialogs->RightGroups_Hwnd, IDC_BASETEXTURE);
	HDC	hDC = GetDC(PreviewWnd);

	App->CL_Dialogs->Sel_BaseBitmap = ilutWinLoadImage(TextureFileName, hDC);
	
	BasePicWidth = ilGetInteger(IL_IMAGE_WIDTH);
	BasePicHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	//BasePicDepth = ilGetInteger(IL_IMAGE_DEPTH);

	RedrawWindow(App->CL_Dialogs->RightGroups_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *		Create_DummyTexture:- Terry and Hazel Flanigan 2024    	 	   *
// *************************************************************************
bool CL64_Textures::Create_DummyTexture(char* Folder)
{
	HBITMAP hbmpTemp;
	hbmpTemp = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_DUMMY));

	char OutFile[MAX_PATH];
	strcpy(OutFile, Folder);
	strcat(OutFile, "TTemp.bmp");

	HBITMAP_TO_BmpFile(hbmpTemp, OutFile, (LPSTR)"");

	return 1;
}

// *************************************************************************
// *		DummyTexture_To_HBITMAP:- Terry and Hazel Flanigan 2024    	   *
// *************************************************************************
HBITMAP CL64_Textures::DummyTexture_To_HBITMAP()
{
	HBITMAP hbmpTemp;
	hbmpTemp = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_DUMMY));

	return hbmpTemp;
}

// *************************************************************************
// *		HBITMAP_TO_BmpFile:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
bool CL64_Textures::HBITMAP_TO_BmpFile(HBITMAP Bitmap, char* Filename, char* SaveFolder)
{
	HDC hDC;
	int iBits;
	WORD wBitCount;
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	BITMAP Bitmap0 = { 0 };
	BITMAPFILEHEADER bmfHdr = { 0 };
	BITMAPINFOHEADER bi = { 0 };
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal, hOldPal2 = NULL;
	hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;
	GetObject(Bitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap0.bmWidth;
	bi.biHeight = -Bitmap0.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 256;

	dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8* Bitmap0.bmHeight;

	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}


	GetDIBits(hDC, Bitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	if (hOldPal2)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFile(Filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return 1;
}

// *************************************************************************
// *		Get_Just_FileName:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
void CL64_Textures::Get_Just_FileName(char* pString, char* FileName)
{
	int Count = 0;
	int Mark = 0;
	bool Test = 0;

	while (*pString != 0)
	{
		if (*pString == '\\' || *pString == '/')
		{
			Test = 1;
			Mark = Count;
		}

		Count++;
		pString++;
	}

	if (Mark == 0 && Test == 0)
	{
		strcpy(Just_Texture_FileName, FileName);
	}
	else
	{
		if (Mark == 0 && Test == 1)
		{
			Mark = 1;
			strcpy(Just_Texture_FileName, (FileName + Mark));
		}
		else
		{
			strcpy(Just_Texture_FileName, (FileName + Mark) + 1);
		}
	}
}

// *************************************************************************
// *			RenderTexture_Blit:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Textures::RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect)
{
	HDC		MemDC;
	int		SourceWidth;
	int		SourceHeight;
	int		DestWidth;
	int		DestHeight;

	MemDC = CreateCompatibleDC(hDC);
	if (MemDC == NULL)
	{
		App->Say("Can not create MemDC");
		return;
	}


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

}
