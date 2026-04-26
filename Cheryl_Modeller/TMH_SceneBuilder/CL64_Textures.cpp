/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
	TextureFileName[0] = 0;

	BasePicWidth = 0;
	BasePicHeight = 0;
	BasePicDepth = 0;

	strcpy(Temp_Texture_Location, App->App_Directory_FullPath);
	strcat(Temp_Texture_Location, "\\Data\\World_Test\\");

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
// *		Load_Textures_Assimp:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Textures::Load_Textures_Assimp()
{
	int v = 0;
	int Count = 0;
	bool DummyCreated = 0;
	App->CL_Utilities->JustFileName[0] = 0;

	int mGroupCount = App->CL_Model->GroupCount;

	while (Count < mGroupCount)
	{
		App->CL_Utilities->Get_FileName_FromPath(App->CL_Mesh->Group[Count]->Assimp_Text_FileName, App->CL_Mesh->Group[Count]->Assimp_Text_FileName);
		strcpy(App->CL_Mesh->Group[Count]->Assimp_Text_FileName, App->CL_Utilities->JustFileName);

		int Test = strcmp(App->CL_Utilities->JustFileName, "No_Texture");
		if (Test != 0) // not a match
		{

			char ImageFullPath[MAX_PATH];
			strcpy(ImageFullPath, App->CL_Model->Model_FolderPath);
			strcat(ImageFullPath, App->CL_Mesh->Group[Count]->Assimp_Text_FileName);

			strcpy(App->CL_Mesh->Group[v]->Assimp_Texture_PathFileName, ImageFullPath);

			strcpy(TextureFileName, ImageFullPath);

			Windows_Preview_FullPath(v, ImageFullPath);

			bool test = Load_OpenGL_Textures(App->CL_Mesh->Group[Count]->MaterialIndex);
			if (test == 0)
			{
				//App->Error_ToFile("Loading Dummy Texture Instead");
				Create_DummyTexture(App->CL_Model->Model_FolderPath);

				char buf[MAX_PATH];
				strcpy(buf, App->CL_Model->Model_FolderPath);
				strcat(buf, "TTemp.bmp");
				UINT* Texture_List = g_Texture;
				Import_OpenGL_Texture(Texture_List, buf, App->CL_Mesh->Group[Count]->MaterialIndex);

				//App->CL_Loader->LoadError = 1;
				DummyCreated = 1;
			}
			v++;
		}
		else
		{
			//App->Error_ToFile("No Texture in File");
			//App->Error_ToFile("Loading Dummy Texture Instead");
			Create_DummyTexture(App->CL_Model->Model_FolderPath);

			char buf[MAX_PATH];
			strcpy(buf, App->CL_Model->Model_FolderPath);
			strcat(buf, "TTemp.bmp");
			UINT* Texture_List = g_Texture;
			Import_OpenGL_Texture(Texture_List, buf, App->CL_Mesh->Group[Count]->MaterialIndex);

			//App->CL_Loader->LoadError = 1;
			DummyCreated = 1;

			v++;
		}

		Count++;
	}

	if (DummyCreated == 1)
	{
		char buf[MAX_PATH];
		strcpy(buf, App->CL_Model->Model_FolderPath);
		strcat(buf, "TTemp.bmp");
		remove(buf);
	}

}

// *************************************************************************
// *		Create_DummyTexture:- Terry and Hazel Flanigan 2024    	 	   *
// *************************************************************************
bool CL64_Textures::Create_DummyTexture(char* Folder)
{
	HBITMAP hbmpTemp;
	hbmpTemp = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_NO_TEXTURE));

	char OutFile[MAX_PATH];
	strcpy(OutFile, Folder);
	strcat(OutFile, "TTemp.bmp");

	HBITMAP_TO_BmpFile(hbmpTemp, OutFile, (LPSTR)"");

	return 1;
}

// *************************************************************************
// *					CreateDummyTexture Terry Bernie   		 	 	   *
// *************************************************************************
bool CL64_Textures::LoadDummyTexture(int Index)
{
	if (App->CL_Model->GroupCount > 0)
	{
		App->CL_Mesh->Group[Index]->Base_Bitmap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_NO_TEXTURE));

		strcpy(App->CL_Mesh->Group[Index]->Assimp_Texture_PathFileName, "Internal Dummy BMP");
		strcpy(App->CL_Mesh->Group[Index]->Assimp_Text_FileName, "Dummy.bmp");

		App->CL_Mesh->Group[Index]->Depth = 8;
		App->CL_Mesh->Group[Index]->Width = 256;
		App->CL_Mesh->Group[Index]->Height = 256;

		App->CL_Mesh->Group[Index]->Bitmap_Loaded = -1;
	}

	return 1;
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
	strcpy(buf, App->CL_Model->Model_FolderPath);
	strcat(buf, "Etemp.bmp");

	UINT* Texture_List = g_Texture;

	// ----------------------------------- Bitmap
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".BMP") == 0)
	{
		bool test = Import_OpenGL_Texture(Texture_List, TextureFileName, TextureID);

		if (test == 0) { App->Say("Failed to load Bmp"); return 0; }
		return 1;
	}
	// ------------------------------------ JPEG
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".JPG") == 0)
	{
		bool test = Import_OpenGL_Texture(Texture_List, TextureFileName, TextureID);

		if (test == 0) { App->Say("Failed to load JPG"); return 0; }
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

	Create_DummyTexture(App->CL_Model->Model_FolderPath);
	Import_OpenGL_Texture(g_Texture, buf, TextureID);
	remove(buf);

	return 1;
}

// *************************************************************************
// *		Windows_Preview_FullPath:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
bool CL64_Textures::Windows_Preview_FullPath(int Index, char* FullPath)
{
	if (App->CL_Model->GroupCount > 0)
	{

		char mFileName[MAX_PATH];
		strcpy(mFileName, FullPath);

		HWND PreviewWnd = GetDlgItem(App->CL_Properties_Textures_Com->Textures_Dlg_Hwnd_Assimp, IDC_AT_BASETEXTURE);
		HDC	hDC = GetDC(PreviewWnd);

		App->CL_Mesh->Group[Index]->Base_Bitmap = NULL;

		// ----------------------------------- Bitmap
		if (_stricmp(mFileName + strlen(mFileName) - 4, ".BMP") == 0)
		{

			App->CL_Mesh->Group[Index]->Base_Bitmap = ilutWinLoadImage(mFileName, hDC);
			ReleaseDC(PreviewWnd, hDC);

			if (App->CL_Mesh->Group[Index]->Base_Bitmap == NULL)
			{

				LoadDummyTexture(Index);

				return 1;
			}
			else
			{
				BITMAPINFO    bmiData;
				ilutGetBmpInfo(&bmiData);

				App->CL_Mesh->Group[Index]->Depth = bmiData.bmiHeader.biBitCount;
				App->CL_Mesh->Group[Index]->Width = bmiData.bmiHeader.biWidth;
				App->CL_Mesh->Group[Index]->Height = bmiData.bmiHeader.biHeight;
				App->CL_Mesh->Group[Index]->Bitmap_Loaded = 1;
			}

			return 1;
		}

		// ------------------------------------ JPEG
		if (_stricmp(mFileName + strlen(mFileName) - 4, ".JPG") == 0)
		{
			App->CL_Mesh->Group[Index]->Base_Bitmap = ilutWinLoadImage(mFileName, hDC);
			ReleaseDC(PreviewWnd, hDC);

			if (App->CL_Mesh->Group[Index]->Base_Bitmap == NULL)
			{

				LoadDummyTexture(Index);

				return 1;
			}
			else
			{
				BITMAPINFO    bmiData;
				ilutGetBmpInfo(&bmiData);

				App->CL_Mesh->Group[Index]->Depth = bmiData.bmiHeader.biBitCount;
				App->CL_Mesh->Group[Index]->Width = bmiData.bmiHeader.biWidth;
				App->CL_Mesh->Group[Index]->Height = bmiData.bmiHeader.biHeight;
				App->CL_Mesh->Group[Index]->Bitmap_Loaded = 1;
			}

			return 1;
		}

		// ------------------------------------ DDS
		if (_stricmp(mFileName + strlen(mFileName) - 4, ".DDS") == 0)
		{
			App->CL_Mesh->Group[Index]->Base_Bitmap = ilutWinLoadImage(mFileName, hDC);
			ReleaseDC(PreviewWnd, hDC);

			if (App->CL_Mesh->Group[Index]->Base_Bitmap == NULL)
			{

				LoadDummyTexture(Index);

				return 1;
			}
			else
			{
				BITMAPINFO    bmiData;
				ilutGetBmpInfo(&bmiData);

				App->CL_Mesh->Group[Index]->Depth = bmiData.bmiHeader.biBitCount;
				App->CL_Mesh->Group[Index]->Width = bmiData.bmiHeader.biWidth;
				App->CL_Mesh->Group[Index]->Height = bmiData.bmiHeader.biHeight;
				App->CL_Mesh->Group[Index]->Bitmap_Loaded = 1;
			}

			return 1;
		}

		// ------------------------------------ TGA
		if (_stricmp(mFileName + strlen(mFileName) - 4, ".TGA") == 0)
		{
			App->CL_Mesh->Group[Index]->Base_Bitmap = ilutWinLoadImage(mFileName, hDC);
			ReleaseDC(PreviewWnd, hDC);

			if (App->CL_Mesh->Group[Index]->Base_Bitmap == NULL)
			{

				LoadDummyTexture(Index);

				return 1;
			}
			else
			{
				BITMAPINFO    bmiData;
				ilutGetBmpInfo(&bmiData);

				App->CL_Mesh->Group[Index]->Depth = bmiData.bmiHeader.biBitCount;
				App->CL_Mesh->Group[Index]->Width = bmiData.bmiHeader.biWidth;
				App->CL_Mesh->Group[Index]->Height = bmiData.bmiHeader.biHeight;
				App->CL_Mesh->Group[Index]->Bitmap_Loaded = 1;
			}

			return 1;
		}

		// ------------------------------------ PNG
		if (_stricmp(mFileName + strlen(mFileName) - 4, ".PNG") == 0)
		{
			App->CL_Mesh->Group[Index]->Base_Bitmap = ilutWinLoadImage(mFileName, hDC);
			ReleaseDC(PreviewWnd, hDC);

			if (App->CL_Mesh->Group[Index]->Base_Bitmap == NULL)
			{
				LoadDummyTexture(Index);

				return 1;
			}
			else
			{
				BITMAPINFO    bmiData;
				ilutGetBmpInfo(&bmiData);

				App->CL_Mesh->Group[Index]->Depth = bmiData.bmiHeader.biBitCount;
				App->CL_Mesh->Group[Index]->Width = bmiData.bmiHeader.biWidth;
				App->CL_Mesh->Group[Index]->Height = bmiData.bmiHeader.biHeight;
				App->CL_Mesh->Group[Index]->Bitmap_Loaded = 1;
			}

			return 1;
		}

		LoadDummyTexture(Index);

		App->CL_Mesh->Group[Index]->Bitmap_Loaded = -1;
	}

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
// *							Jpg_To_Tga					  		 	   *
// *************************************************************************
bool CL64_Textures::Jpg_To_Tga24(char* File)
{
	char OldFile[1024];
	strcpy(OldFile, File);

	ilLoadImage(File);

	int Len = strlen(File);
	File[Len - 4] = 0;
	strcat(File, ".tga");

	ilSaveImage(File);

	remove(OldFile);
	return 1;
}

// *************************************************************************
// *							Jpg_To_png24				  		 	   *
// *************************************************************************
bool CL64_Textures::Jpg_To_png24(char* File)
{
	char OldFile[1024];
	strcpy(OldFile, File);

	ilLoadImage(File);

	int Len = strlen(File);
	File[Len - 4] = 0;
	strcat(File, ".png");

	ilSaveImage(File);

	remove(OldFile);
	return 1;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Textures::Texture_To_HBITMP(char* TextureFileName)
{
	HWND PreviewWnd = GetDlgItem(App->CL_Dialogs->TextureView_Hwnd, IDC_BASETEXTURE);
	HDC	hDC = GetDC(PreviewWnd);

	App->CL_Dialogs->Sel_BaseBitmap = Get_HBITMP(TextureFileName, hDC);

	RedrawWindow(App->CL_Dialogs->TextureView_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			 Get_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
HBITMAP CL64_Textures::Get_HBITMP(char* TextureFileName, HDC hDC)
{
	HBITMAP hBmp = NULL;

	hBmp = ilutWinLoadImage(TextureFileName, hDC);

	if (hBmp == NULL)
	{
		App->Say("Could not load Image");

	}

	BasePicWidth = ilGetInteger(IL_IMAGE_WIDTH);
	BasePicHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	//BasePicDepth = ilGetInteger(IL_IMAGE_DEPTH);

	return hBmp;
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

// *************************************************************************
// *			Lock_All_Textures:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Textures::Lock_All_Textures()
{
	
}

// *************************************************************************
// *		DummyTexture_To_HBITMAP:- Terry and Hazel Flanigan 2026    	   *
// *************************************************************************
HBITMAP CL64_Textures::DummyTexture_To_HBITMAP()
{
	HBITMAP hbmpTemp;
	hbmpTemp = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_NO_TEXTURE));

	return hbmpTemp;
}

// *************************************************************************
// *		HBITMAP_TO_BmpFile:- Terry and Hazel Flanigan 2026		 	   *
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

	dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8 * Bitmap0.bmHeight;

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




