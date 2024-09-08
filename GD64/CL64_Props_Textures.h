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

	bool Start_Groups_Dialog();
	bool Update_Groups();

	int Selected_Group;

	bool RightGroups_Visable;

	HWND RightGroups_Hwnd;

	HBITMAP	Sel_BaseBitmap;
	long BasePicWidth;
	long BasePicHeight;

protected:

	static LRESULT CALLBACK Groups_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static bool CALLBACK ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect);
};

