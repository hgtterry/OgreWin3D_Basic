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

#pragma once

class CL64_Textures
{
public:
	CL64_Textures(void);
	~CL64_Textures(void);

	void RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect);
	bool Soil_Load_Texture(UINT textureArray[], LPSTR strFileName, int textureID);
	
	void Texture_To_HBITMP(char* TextureFileName);
	HBITMAP Get_HBITMP(char* TextureFileName, HDC hDC);

	bool Texture_To_Bmp(char* File);

	bool Covert_Texture(const char* File);
	void Remove_Temp_Texture();
	void Lock_All_Textures();

	long BasePicWidth;
	long BasePicHeight;
	long BasePicDepth;

	char Temp_Texture_File[MAX_PATH];
	char Temp_Texture_Location[MAX_PATH];
};

