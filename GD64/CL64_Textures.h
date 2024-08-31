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

	bool Load_OpenGL_Textures(int TextureID);
	bool Import_OpenGL_Texture(UINT textureArray[], LPSTR strFileName, int textureID);

	bool Create_DummyTexture(char* Folder);
	bool HBITMAP_TO_BmpFile(HBITMAP Bitmap, char* Filename, char* SaveFolder);
	void Texture_To_HBITMP();

	void Get_Just_FileName(char* pString, char* FileName);

	char TextureFileName[MAX_PATH];
	char Just_Texture_FileName[MAX_PATH];

	UINT  g_Texture[400]; //Texture IDs for loaded textures OpenGL
};

