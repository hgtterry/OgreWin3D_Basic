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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "il.h"
#include "ilu.h"
#include "ilut.h"

typedef struct tagMY_BITMAPINFO
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD          bmiColors[256];
} MY_BITMAPINFO, * pMY_BITMAPINFO, ** ppMY_BITMAPINFO;

#define PAD32(i)     (((i)+31)/32*4)

CL64_Textures::CL64_Textures(void)
{
	ilInit();

	Temp_Texture_File[0] = 0;

	strcpy(Temp_Texture_Location, App->RB_Directory_FullPath);
	strcat(Temp_Texture_Location, "\\Data\\World_Test\\");
}

CL64_Textures::~CL64_Textures(void)
{
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

// *************************************************************************
// *		 Soil_Load_Texture:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
bool CL64_Textures::Soil_Load_Texture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	int image_width = 0;
	int image_height = 0;
	int channels = 0;
	int force_channels = 4;

	unsigned char* image_data = stbi_load(strFileName, &image_width, &image_height, &channels, force_channels);
	if (image_data == NULL)
	{
		//App->Say("Cant Create Texture");
		return 0;
	}

	if ((force_channels >= 1) && (force_channels <= 4))
	{
		channels = force_channels;
	}

	bool INVERT_Y = 1;
	if (INVERT_Y == 1)
	{
		int i, j;
		for (j = 0; j * 2 < image_height; ++j)
		{
			int index1 = j * image_width * channels;
			int index2 = (image_height - 1 - j) * image_width * channels;
			for (i = image_width * channels; i > 0; --i)
			{
				unsigned char temp = image_data[index1];
				image_data[index1] = image_data[index2];
				image_data[index2] = temp;
				++index1;
				++index2;
			}
		}
	}

	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	textureArray[textureID] = image_texture;

	stbi_image_free(image_data);

	if (textureArray[textureID] == 0) // Fall back attemp to convert and load or Bail
	{
		Texture_To_Bmp(strFileName);

		remove("Etemp.bmp");

	}

	
	return 1;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Textures::Texture_To_HBITMP(char* TextureFileName)
{
	//HWND PreviewWnd = GetDlgItem(App->CL_Dialogs->RightGroups_Hwnd, IDC_BASETEXTURE);
	//HDC	hDC = GetDC(PreviewWnd);

	//App->CL_Dialogs->Sel_BaseBitmap = ilutWinLoadImage(TextureFileName, hDC);

	//BasePicWidth = ilGetInteger(IL_IMAGE_WIDTH);
	//BasePicHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	////BasePicDepth = ilGetInteger(IL_IMAGE_DEPTH);

	//RedrawWindow(App->CL_Dialogs->RightGroups_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Texture_To_Bmp:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Textures::Texture_To_Bmp(char* File)
{

	ilLoadImage(File);
	ilSaveImage("Etemp.bmp");
	return 1;
}

// *************************************************************************
// *			Covert_Texture:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Textures::Covert_Texture(const char* File)
{
	//---------------------------------------------------------- Bmp
	if (_stricmp(File + strlen(File) - 4, ".BMP") == 0)
	{
		// Do Nothing
		return 0;
	}

	//---------------------------------------------------------- tga
	if (_stricmp(File + strlen(File) - 4, ".TGA") == 0)
	{
		// Do Nothing
		return 0;
	}

	//---------------------------------------------------------- jpg
	if (_stricmp(File + strlen(File) - 4, ".jpg") == 0)
	{
		Temp_Texture_File[0] = 0;

		ilLoadImage(File);

		App->CL_Utilities->Get_FileName_FromPath((LPSTR)File, (LPSTR)File);

		strcpy(Temp_Texture_File, Temp_Texture_Location);
		strcat(Temp_Texture_File, App->CL_Utilities->JustFileName);
		
		int Len = strlen(Temp_Texture_File);
		Temp_Texture_File[Len - 4] = 0;
		strcat(Temp_Texture_File, ".bmp");

		ilSaveImage(Temp_Texture_File);

		strcpy((LPSTR)App->CL_File_IO->s_Path_And_File.c_str(), Temp_Texture_File);

		return 1;
	}

	return 0;
}

// *************************************************************************
// *			Remove_Temp_Texture:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Textures::Remove_Temp_Texture()
{
	remove(Temp_Texture_File);
}




