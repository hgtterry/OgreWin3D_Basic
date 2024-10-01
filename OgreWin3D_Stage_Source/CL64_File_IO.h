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
	
	bool Open_File();
	void Save_File();
	void Pick_Folder();
	
	// -------------------
	bool SaveSelectedFile(char* Extension, char* File);
	bool Check_File_Exist(char* Full_Path);
	bool SearchFolders(char* Path, char* File);

	void Open_HTML(char* HelpTitle);

	char Save_PathFileName[MAX_PATH];
	char Save_FileName[MAX_PATH];

	char DeskTop_Folder[MAX_PATH];

	TCHAR szSelectedDir[MAX_PATH];
	char BrowserMessage[MAX_PATH];

	std::string sSelectedFile;
	std::string sFilePath;

private:
	
	OPENFILENAMEA ofn;
};

