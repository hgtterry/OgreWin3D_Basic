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
class CL64_Properties_Textures_Assimp
{
public:

	CL64_Properties_Textures_Assimp(void);
	~CL64_Properties_Textures_Assimp(void);

	void Reset_Class(void);

	bool Start_Props_Textures_Dialog();

	bool Update_Texture_Ogre_Dlg();

	bool View_Texture(char* TextureName, char* MaterialName);
	void Texture_To_HBITMP(char* TextureFileName);
	void Enable_Export_Button(bool Enable);

	void Get_First_Texture_Ogre();
	void List_Material_Changed(int Index);
	void Select_By_Index(int Index);
	
	char mTextureName[MAX_PATH];
	char mMaterialName[MAX_PATH];
	char mExport_PathAndName[MAX_PATH];

	HWND Textures_Dlg_Hwnd_Assimp;

	HBITMAP	Sel_BaseBitmap;
	
protected:

	static LRESULT CALLBACK Proc_Textures_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static bool CALLBACK ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect);

};

