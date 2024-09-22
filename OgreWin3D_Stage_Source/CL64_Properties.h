/*
Copyright (c) OgreWin3D_Stage 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

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
class CL64_Properties
{
public:

	CL64_Properties(void);
	~CL64_Properties(void);

	void Start_Properties(void);

	HWND Properties_Dlg_hWnd;

private:
	static LRESULT CALLBACK Proc_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Create_Properties_hLV(void);

	HWND Properties_hLV;
};

