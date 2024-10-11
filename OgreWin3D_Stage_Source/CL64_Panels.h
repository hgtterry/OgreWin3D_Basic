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
class CL64_Panels
{
public:

	CL64_Panels(void);
	~CL64_Panels(void);

	void Resize_Fldg(void);
	void Resize_OgreWin(void);
	void Resize_TopDlg(void);
	bool Move_Panels(void);
	void Move_FileView_Window(void);
	bool Resize_FileView(void);
	bool Place_Properties_Dlg(void);
	void Disable_Panels(bool Disable);

	bool flag_Panels_Disabled;

};

