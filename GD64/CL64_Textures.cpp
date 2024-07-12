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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CL64_Textures::CL64_Textures(void)
{
	TextureFileName[0] = 0;
	Just_Texture_FileName[0] = 0;

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
// *		Load_Textures_Assimp:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Textures::Load_Textures_Assimp()
{
	int v = 0;
	int Count = 0;
	bool DummyCreated = 0;
	Just_Texture_FileName[0] = 0;

	int mGroupCount = App->CL_Scene->GroupCount;

	while (Count < mGroupCount)
	{

		Get_Just_FileName(App->CL_Scene->Group[Count]->Text_FileName, App->CL_Scene->Group[Count]->Text_FileName);
		strcpy(App->CL_Scene->Group[Count]->Text_FileName, Just_Texture_FileName);

		int Test = strcmp(Just_Texture_FileName, "No_Texture");
		if (Test != 0) // not a match
		{

			char ImageFullPath[MAX_PATH];
			strcpy(ImageFullPath, App->CL_Scene->Texture_FolderPath);
			strcat(ImageFullPath, App->CL_Scene->Group[Count]->Text_FileName);

			strcpy(App->CL_Scene->Group[v]->Texture_PathFileName, ImageFullPath);

			strcpy(TextureFileName, ImageFullPath);

			//Windows_Preview_FullPath(v, ImageFullPath);

			bool test = Load_OpenGL_Textures(App->CL_Scene->Group[Count]->MaterialIndex);
			if (test == 0)
			{
				//App->Error_ToFile("Loading Dummy Texture Instead");
				App->CL_Textures->Create_DummyTexture(App->CL_Scene->Texture_FolderPath);

				char buf[MAX_PATH];
				strcpy(buf, App->CL_Scene->Texture_FolderPath);
				strcat(buf, "TTemp.bmp");
				UINT* Texture_List = g_Texture;
				Import_OpenGL_Texture(Texture_List, buf, App->CL_Scene->Group[Count]->MaterialIndex);

				//App->CL_Loader->LoadError = 1;
				DummyCreated = 1;
			}
			v++;
		}
		else
		{
			//App->Error_ToFile("No Texture in File");
			//App->Error_ToFile("Loading Dummy Texture Instead");
			App->CL_Textures->Create_DummyTexture(App->CL_Scene->Texture_FolderPath);

			char buf[MAX_PATH];
			strcpy(buf, App->CL_Scene->Texture_FolderPath);
			strcat(buf, "TTemp.bmp");
			UINT* Texture_List = g_Texture;
			Import_OpenGL_Texture(Texture_List, buf, App->CL_Scene->Group[Count]->MaterialIndex);

			//App->CL_Loader->LoadError = 1;
			DummyCreated = 1;

			v++;
		}

		Count++;
	}

	if (DummyCreated == 1)
	{
		char buf[MAX_PATH];
		strcpy(buf, App->CL_Scene->Texture_FolderPath);
		strcat(buf, "TTemp.bmp");
		remove(buf);
	}
}

// *************************************************************************
// *		Load_OpenGL_Textures:- Terry and Hazel Flanigan 2024   	  	   *
// *************************************************************************
bool CL64_Textures::Load_OpenGL_Textures(int TextureID)
{
	int Index = 0;
	int Dont = 0;
	int jpg = 0;
	int DontAdd = 0;
	int AddDummy = 0;

	char buf[1024];
	strcpy(buf, App->CL_Scene->Texture_FolderPath);
	strcat(buf, "Etemp.bmp");

	UINT* Texture_List = g_Texture;

	// ----------------------------------- Bitmap
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".BMP") == 0)
	{
		bool test = Import_OpenGL_Texture(Texture_List, TextureFileName, TextureID);

		if (test == 0){App->Say("Failed to load Bmp");return 0;}
		return 1;
	}
	// ------------------------------------ JPEG
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".JPG") == 0)
	{
		bool test = Import_OpenGL_Texture(Texture_List, TextureFileName, TextureID);

		if (test == 0){App->Say("Failed to load JPG");return 0;}
		return 1;
	}
	// ------------------------------------ TGA
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".TGA") == 0)
	{
		bool test = Import_OpenGL_Texture(Texture_List, TextureFileName, TextureID);

		if (test == 0) { App->Say("Failed to load TGA"); return 0; }
		return 1;
	}
	// ------------------------------------ DDS
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".DDS") == 0)
	{
		bool test = Import_OpenGL_Texture(Texture_List, TextureFileName, TextureID);

		if (test == 0) { App->Say("Failed to load DDS"); return 0; }
		return 1;
	}
	// ------------------------------------ PNG
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".PNG") == 0)
	{
		bool test = Import_OpenGL_Texture(Texture_List, TextureFileName, TextureID);

		if (test == 0) { App->Say("Failed to load PNG"); return 0; }
		return 1;
	}

	Create_DummyTexture(App->CL_Scene->Texture_FolderPath);
	Import_OpenGL_Texture(g_Texture, buf, TextureID);
	remove(buf);

	return 1;
}

// *************************************************************************
// *		 Import_OpenGL_Texture:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
bool CL64_Textures::Import_OpenGL_Texture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	int image_width = 0;
	int image_height = 0;
	int channels = 0;
	int force_channels = 4;

	unsigned char* image_data = stbi_load(strFileName, &image_width, &image_height, &channels, force_channels);
	if (image_data == NULL)
	{
		App->Say("Cant Create Texture");
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
		//Texture_To_Bmp(strFileName);
		remove("Etemp.bmp");
	}

	return 1;
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
