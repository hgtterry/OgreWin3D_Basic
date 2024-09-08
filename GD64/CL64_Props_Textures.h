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
class CL64_Props_Textures
{
public:

	CL64_Props_Textures(void);
	~CL64_Props_Textures(void);

	void Reset_Class(void);

	bool Start_Props_Textures_Dialog();
	bool Update_Groups();
	bool View_Texture(char* TextureName, char* MaterialName);
	void Texture_To_HBITMP(char* TextureFileName);
	void Enable_Export_Button(bool Enable);

	int Selected_Group;

	bool RightGroups_Visable;

	HWND Props_Dlg_Hwnd;

	HBITMAP	Sel_BaseBitmap;
	long BasePicWidth;
	long BasePicHeight;

protected:

	static LRESULT CALLBACK Proc_Textures_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static bool CALLBACK ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect);

	char mTextureName[MAX_PATH];
	char mMaterialName[MAX_PATH];
	char mExport_PathAndName[MAX_PATH];

};

