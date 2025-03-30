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

	BasePicWidth = 0;
	BasePicHeight = 0;
	BasePicDepth = 0;

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




