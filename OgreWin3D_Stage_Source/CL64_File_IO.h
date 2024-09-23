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
class CL64_File_IO
{
public:
	CL64_File_IO();
	~CL64_File_IO();
	bool StartBrowser(char* szInitDir);

	bool Open_Project_File(char* Extension, char* Title, char* StartDirectory);
	bool Open_File_Model(const char* Extension, const char* Title, const char* StartDirectory);
	bool Open_Resource_File(char* Extension, char* Title, char* StartDirectory);
	bool SaveSelectedFile(char* Extension, char* File);
	bool Check_File_Exist(char* Full_Path);

	std::string Get_Model_Path_File_Name();

	void Open_HTML(char* HelpTitle);

	char Project_File_Name[MAX_PATH];
	char Project_Path_File_Name[MAX_PATH];

	char Model_FileName[MAX_PATH];
	char Model_Path_FileName[MAX_PATH];

	char Data_mFilename[MAX_PATH];
	char Data_Path_mFilename[MAX_PATH];

	char Save_PathFileName[MAX_PATH];
	char Save_FileName[MAX_PATH];

	TCHAR szSelectedDir[MAX_PATH];
	char BrowserMessage[MAX_PATH];

	char OgreCFG_FileName[MAX_PATH];
	char OgreCFG_Path_FileName[MAX_PATH];

	OPENFILENAME ofn;

private:
	static int __stdcall BrowseCallbackProc(HWND  hwnd, UINT  uMsg, LPARAM  lParam, LPARAM  lpData);

};

