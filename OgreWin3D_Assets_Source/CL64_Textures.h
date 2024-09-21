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

#pragma once
class CL64_Textures
{
public:

	CL64_Textures(void);
	~CL64_Textures(void);

	void Load_Textures_Assimp();
	bool Windows_Preview_FullPath(int Index, char* FullPath);
	bool DecompileTextures(char* Path);

	bool Load_OpenGL_Textures(int TextureID);
	bool Import_OpenGL_Texture(UINT textureArray[], LPSTR strFileName, int textureID);

	bool Create_DummyTexture(char* Folder);
	bool LoadDummyTexture(int Index);

	bool HBITMAP_TO_BmpFile(HBITMAP Bitmap, char* Filename, char* SaveFolder);
	void Texture_To_HBITMP(char* TextureFileName);
	HBITMAP DummyTexture_To_HBITMAP();

	void Get_Just_FileName(char* pString, char* FileName);

	void RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect);

	char TextureFileName[MAX_PATH];
	char Just_Texture_FileName[MAX_PATH];

	long BasePicWidth;
	long BasePicHeight;
	long BasePicDepth;

	UINT  g_Texture[400]; //Texture IDs for loaded textures OpenGL
};

