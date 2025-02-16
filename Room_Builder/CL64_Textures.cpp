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
#include "CL64_App.h"
#include "CL64_Textures.h"

typedef struct tagMY_BITMAPINFO
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD          bmiColors[256];
} MY_BITMAPINFO, * pMY_BITMAPINFO, ** ppMY_BITMAPINFO;

#define PAD32(i)     (((i)+31)/32*4)

CL64_Textures::CL64_Textures(void)
{
}

CL64_Textures::~CL64_Textures(void)
{
}

// *************************************************************************
// *			Write_BMP:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
int CL64_Textures::Write_BMP(const char* pszFile, geBitmap* pBitmap)
{
	geBitmap* pLock = NULL;
	gePixelFormat    Format;
	geBitmap_Info    BitmapInfo;
	int    nErrorCode = 0;// nErrorCode = TPACKERROR_UNKNOWN;      // Return code
	BITMAPFILEHEADER BmpHeader;                            // bitmap file-header 
	MY_BITMAPINFO    BmInfo;
	Ogre::uint32           nBytesPerPixel;
	void* pPixelData;
	Ogre::uint8* pOut = NULL;
	Ogre::uint8* pTmp = NULL;
	int              nNewStride = 0;
	int              nOldStride = 0;
	int              i;
	HANDLE           hFile = NULL;
	DWORD            nBytesWritten;
	Ogre::uint8* pNew;
	Ogre::uint8* pOld;
	int    y;

	// Create the .BMP file.
	hFile = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		App->Say("Error");
	}

	// get 8-bit palettized bitmap
	Format = GE_PIXELFORMAT_8BIT;

	if (geBitmap_GetBits(pBitmap))
	{
		pLock = pBitmap;
	}
	else
	{
		if (!geBitmap_LockForRead(pBitmap, &pLock, 0, 0, Format, GE_FALSE, 0))
		{
			return FALSE;
		}
	}

	geBitmap_GetInfo(pLock, &BitmapInfo, NULL);
	if (BitmapInfo.Format != Format)
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}


	for (i = 0; i < 256; i++)
	{
		int r, g, b, a;
		geBitmap_Palette_GetEntryColor(BitmapInfo.Palette, i, &r, &g, &b, &a);

		BmInfo.bmiColors[i].rgbRed = (Ogre::uint8)r;
		BmInfo.bmiColors[i].rgbGreen = (Ogre::uint8)g;
		BmInfo.bmiColors[i].rgbBlue = (Ogre::uint8)b;
		BmInfo.bmiColors[i].rgbReserved = (Ogre::uint8)0;
	}

	nBytesPerPixel = gePixelFormat_BytesPerPel(Format);
	pPixelData = geBitmap_GetBits(pLock);

	// Build bitmap info
	BmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BmInfo.bmiHeader.biWidth = BitmapInfo.Width;
	BmInfo.bmiHeader.biHeight = BitmapInfo.Height;    // Bitmap are bottom-up
	BmInfo.bmiHeader.biPlanes = 1;
	BmInfo.bmiHeader.biBitCount = (WORD)8;
	BmInfo.bmiHeader.biCompression = BI_RGB;
	BmInfo.bmiHeader.biSizeImage = 0;
	BmInfo.bmiHeader.biXPelsPerMeter = BmInfo.bmiHeader.biYPelsPerMeter = 0;   // 10000;

	if (BmInfo.bmiHeader.biBitCount < 24)
		BmInfo.bmiHeader.biClrUsed = (1 << BmInfo.bmiHeader.biBitCount);
	else
		BmInfo.bmiHeader.biClrUsed = 0;

	BmInfo.bmiHeader.biClrImportant = 0;

	nNewStride = PAD32(BitmapInfo.Width * BmInfo.bmiHeader.biBitCount);
	nOldStride = BitmapInfo.Width * nBytesPerPixel;

	BmInfo.bmiHeader.biSizeImage = nNewStride * BitmapInfo.Height;

	if (nNewStride == nOldStride)
	{
		pTmp = (Ogre::uint8*)pPixelData;
	}

	// Allocate new pixel buffer.
	else
	{
		pTmp = (Ogre::uint8*)App->CL_Maths->Ram_Allocate(nNewStride * BitmapInfo.Height);
		if (pTmp == (Ogre::uint8*)0)
		{
			App->Say("Error");
			goto ExitWriteBitmap;
		}


		pNew = (Ogre::uint8*)pTmp;
		pOld = (Ogre::uint8*)pPixelData;

		// Copy old to new
		for (y = 0; y < BitmapInfo.Height; y++)
		{
			memcpy(pNew, pOld, nOldStride);

			// Next row
			pOld += nOldStride;
			pNew += nNewStride;
		}
	}

	pOut = (Ogre::uint8*)App->CL_Maths->Ram_Allocate(nNewStride * BitmapInfo.Height);
	if (pOut == (Ogre::uint8*)0)
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}

	pNew = (Ogre::uint8*)pOut;
	pOld = (Ogre::uint8*)(pTmp + (nNewStride * (BitmapInfo.Height - 1)));

	// Copy old to new
	for (y = 0; y < BitmapInfo.Height; y++)
	{
		memcpy(pNew, pOld, nNewStride);

		// Next row
		pOld -= nNewStride;
		pNew += nNewStride;
	}

	if (pTmp && nNewStride != nOldStride)
		App->CL_Maths->Ram_Free(pTmp);

	// Build the file header
	BmpHeader.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 

	// Compute the size of the entire file. 
	BmpHeader.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		BmInfo.bmiHeader.biSize +
		(BmInfo.bmiHeader.biClrUsed * sizeof(RGBQUAD)) +
		(nNewStride * BitmapInfo.Height));
	BmpHeader.bfReserved1 = 0;
	BmpHeader.bfReserved2 = 0;

	// Compute the offset to the array of color indices. 
	BmpHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		BmInfo.bmiHeader.biSize +
		(BmInfo.bmiHeader.biClrUsed * sizeof(RGBQUAD));

	// Write the BMP file header
	if (!WriteFile(hFile, (LPVOID)&BmpHeader, sizeof(BITMAPFILEHEADER), (LPDWORD)&nBytesWritten, (NULL)))
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}

	// Write the Bitmap infor header and palette
	if (!WriteFile(hFile, (LPVOID)&BmInfo, sizeof(MY_BITMAPINFO), (LPDWORD)&nBytesWritten, (NULL)))
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}

	// Write the pixel data
	if (!WriteFile(hFile, (LPVOID)pOut, nNewStride * BitmapInfo.Height, (LPDWORD)&nBytesWritten, (NULL)))
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}

	CloseHandle(hFile);
	hFile = NULL;

	// Success!
	//App->Say("Success");

ExitWriteBitmap:

	// Clean-up
	//------------------------------------
	// Make sure the file gets closed
	if (hFile)
		CloseHandle(hFile);

	App->CL_Maths->Ram_Free(pOut);

	// Unlock the geBitmap
	if (pLock != pBitmap)
	{
		geBitmap_UnLock(pLock);
	}
	return 1;// nErrorCode;
}
