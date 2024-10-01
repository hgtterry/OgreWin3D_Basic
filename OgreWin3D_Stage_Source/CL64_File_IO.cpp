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

#include <string>
#include <shobjidl.h> 

CL64_File_IO::CL64_File_IO()
{

	Model_FileName[0] = 0;
	Model_Path_FileName[0] = 0;

	Save_PathFileName[0] = 0;
	Save_FileName[0] = 0;

	szSelectedDir[0] = 0;
	BrowserMessage[0] = 0;

	OgreCFG_FileName[0] = 0;
	OgreCFG_Path_FileName[0] = 0;

	DeskTop_Folder[0] = 0;

	ofn = { 0 };
}

CL64_File_IO::~CL64_File_IO()
{
}

// *************************************************************************
// *				Open_File:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_File_IO::Open_File()
{
	//  CREATE FILE OBJECT INSTANCE
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(f_SysHr))
		return FALSE;

	// CREATE FileOpenDialog OBJECT
	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}

	//  SHOW OPEN FILE DIALOG WINDOW
	COMDLG_FILTERSPEC save_filter[1];
	save_filter[0].pszName = L"All files";
	save_filter[0].pszSpec = L"*.*";

	f_SysHr = f_FileSystem->SetFileTypes(1,save_filter);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  SHOW OPEN FILE DIALOG WINDOW
	f_SysHr = f_FileSystem->Show(App->MainHwnd);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
	IShellItem* f_Files;
	f_SysHr = f_FileSystem->GetResult(&f_Files);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  STORE AND CONVERT THE FILE NAME
	PWSTR f_Path;
	f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
	if (FAILED(f_SysHr)) {
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  FORMAT AND STORE THE FILE PATH
	std::wstring path(f_Path);
	std::string c(path.begin(), path.end());
	sFilePath = c;

	//  FORMAT STRING FOR EXECUTABLE NAME
	const size_t slash = sFilePath.find_last_of("/\\");
	sSelectedFile = sFilePath.substr(slash + 1);

	//  SUCCESS, CLEAN UP
	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();
	return TRUE;
}

//bool result = FALSE;
//int main()
//{
//	result = openFile();
//	switch (result) {
//	case(TRUE): {
//		printf("SELECTED FILE: %s\nFILE PATH: %s\n\n", sSelectedFile.c_str(), sFilePath.c_str());
//		system("pause");
//	}
//	case(FALSE): {
//		printf("ENCOUNTERED AN ERROR: (%d)\n", GetLastError());
//		system("pause");
//	}
//	}
//	return 0;
//}

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

// *************************************************************************
// *			SaveSelectedFile:- Terry and Hazel Flanigan 2024   		   *
// *************************************************************************
bool CL64_File_IO::SaveSelectedFile(char* Extension, char* File)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = App->MainHwnd;
	ofn.hInstance = App->hInst;
	ofn.lpstrFile = Save_PathFileName;
	ofn.nMaxFile = sizeof(Save_PathFileName);
	ofn.lpstrFilter = Extension;

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = Save_FileName;
	ofn.nMaxFileTitle = sizeof(Save_FileName);
	ofn.lpstrInitialDir = File;
	ofn.lpstrTitle = "Save File";

	ofn.Flags =
		OFN_PATHMUSTEXIST |
		OFN_FILEMUSTEXIST |
		OFN_EXPLORER |
		OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&ofn) == TRUE)
	{
		return 1;
	}

	return 0;
}

// *************************************************************************
// *		Check_File_Exist:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
bool CL64_File_IO::Check_File_Exist(char* Full_Path)
{
	char pSearchPath[1024];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(pSearchPath, Full_Path);

	hFind = FindFirstFile(pSearchPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		FindClose(hFind);
		return 1;
	}

	return 0;
}

// *************************************************************************
// *			SearchFolders:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
bool CL64_File_IO::SearchFolders(char* Path, char* File)
{
	char pSearchPath[1024];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(pSearchPath, Path);
	strcat(pSearchPath, File);

	hFind = FindFirstFile(pSearchPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		FindClose(hFind);
		return 1;
	}

	return 0;
}

