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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_File_IO.h"

CL64_File_IO::CL64_File_IO()
{
	Model_FileName[0] = 0;
	Model_Path_FileName[0] = 0;

	OgreCFG_FileName[0] = 0;
	OgreCFG_Path_FileName[0] = 0;
}

CL64_File_IO::~CL64_File_IO()
{
}

// *************************************************************************
// *			Open_File_Model:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_File_IO::Open_File_Model(const char* Extension, const char* Title, const char* StartDirectory)
{

	Model_FileName[0] = 0;
	Model_Path_FileName[0] = 0;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = App->MainHwnd;
	ofn.hInstance = App->hInst;
	ofn.lpstrFile = Model_Path_FileName;						// full path and file name
	ofn.nMaxFile = sizeof(Model_Path_FileName);
	ofn.lpstrFilter = Extension;

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = Model_FileName;						// Just File Name
	ofn.nMaxFileTitle = sizeof(Model_FileName);;
	ofn.lpstrInitialDir = StartDirectory;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_PATHMUSTEXIST |
		OFN_FILEMUSTEXIST |
		OFN_EXPLORER |
		OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		return 1;
	}

	return 0;
}

// *************************************************************************
// *			Open_Resource_File:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_File_IO::Open_Resource_File(char* Extension, char* Title, char* StartDirectory)
{
	
	strcpy(OgreCFG_FileName, "");
	strcpy(OgreCFG_Path_FileName, "");
	
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = App->MainHwnd;
	ofn.hInstance = App->hInst;
	ofn.lpstrFile = OgreCFG_Path_FileName;						// full path and file name
	ofn.nMaxFile = sizeof(OgreCFG_Path_FileName);
	ofn.lpstrFilter = Extension;

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = OgreCFG_FileName;						// Just File Name
	ofn.nMaxFileTitle = sizeof(OgreCFG_FileName);
	ofn.lpstrInitialDir = StartDirectory;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_PATHMUSTEXIST |
		OFN_FILEMUSTEXIST |
		OFN_EXPLORER |
		OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		return 1;
	}

	return 0;
}

// *************************************************************************
// *	Get_Model_Path_File_Name:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
std::string CL64_File_IO::Get_Model_Path_File_Name()
{
	return Model_Path_FileName;
}

// *************************************************************************
// *				Open_HTML:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_File_IO::Open_HTML(char* HelpTitle)
{
	char Path[1024];
	strcpy(Path, App->GD_Directory_FullPath);
	strcat(Path, "\\");
	strcat(Path, HelpTitle);

	ShellExecute(0, "open", Path, 0, 0, SW_SHOW);
}

