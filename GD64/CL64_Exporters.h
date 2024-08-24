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
class CL64_Exporters
{
public:

	CL64_Exporters(void);
	~CL64_Exporters(void);

	bool Export_Ogre(bool Use_FileDialog);
	void Start_Ogre_Export_Dlg();
	void Object_Model(void);

	char mJustName[MAX_PATH];
	char mDirectory_Name[MAX_PATH];
	char mFolder_Path[MAX_PATH];

private:
	static LRESULT CALLBACK Export_Ogre_Dlg_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Dialog_Data(HWND m_hDlg);
	int Is_Canceled;
};

