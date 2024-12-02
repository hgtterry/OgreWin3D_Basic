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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_File_IO.h"

#include <string>
#include <shobjidl.h> 

CL64_File_IO::CL64_File_IO()
{
	Save_PathFileName[0] = 0;
	Save_FileName[0] = 0;

	Texture_FileName[0] = 0;
	Texture_Path_FileName[0] = 0;
	UserData_Folder[0] = 0;

	szSelectedDir[0] = 0;
	BrowserMessage[0] = 0;
	Canceled = 0;
	DeskTop_Folder[0] = 0;
	Project_Path_File_Name[0] = 0;
	mHistoryMenu = NULL;

	ofn = { 0 };
	WriteRecentFiles = NULL;
	ReadRecentFiles = NULL;
}

CL64_File_IO::~CL64_File_IO()
{
}

// *************************************************************************
// *			OpenTextureFile:- Terry and Hazel Flanigan 2024	  	 	   *
// *************************************************************************
bool CL64_File_IO::OpenTextureFile(char* Title, char* StartDirectory, bool SaveLocation)
{
	Texture_FileName[0] = 0;
	Texture_Path_FileName[0] = 0;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = App->Fdlg;
	ofn.hInstance = App->hInst;
	ofn.lpstrFile = Texture_Path_FileName;						// full path and file name
	ofn.nMaxFile = sizeof(Texture_Path_FileName);
	ofn.lpstrFilter = "Available Formats\0*.bmp;*.tga;*.jpg;*.png;*.dds;*.pcx;*.tif;*.tiff;\0Windows Bitmap  (*.bmp)\0*.bmp\0Truevision Targa  (*.tga) \0*.tga\0JPEG  (*.jpg) \0*.jpg\0Portable Network Graphics (*.png) \0*.png\0Direct Draw  (*.dds) \0*.dds\0ZSoft PCX  (*.pcx) \0*.pcx\0Tagged Image File Format  (*.tif) \0*.tif\0Tagged Image File Format  (*.tiff) \0*.tiff\0";

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = Texture_FileName;				// Just File Name
	ofn.nMaxFileTitle = sizeof(Texture_FileName);
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

// *************************************************************************
// *				Save_File:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_File_IO::Save_File()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* pFileSave = NULL;

		HRESULT hr = CoCreateInstance(__uuidof(FileSaveDialog), NULL,
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileSave));

		if (SUCCEEDED(hr))
		{
			hr = pFileSave->SetDefaultExtension(L"owproj");
			//hr = pFileSave->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
			hr = pFileSave->Show(App->MainHwnd);

			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileSave->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						std::wstring path(pszFilePath);
						std::string c(path.begin(), path.end());
						sFilePath = c;
						App->Say(c.c_str());

						const size_t slash = sFilePath.find_last_of("/\\");
						sSelectedFile = sFilePath.substr(slash + 1);
						App->Say(sSelectedFile.c_str());
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
}

// *************************************************************************
// *				Pick_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_File_IO::Pick_Folder()
{
	strcpy(szSelectedDir,"No_Selection");
	Canceled = 1;

	IFileDialog* pfd;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
	{
		DWORD dwOptions;
		if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
		{
			pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
		}
		if (SUCCEEDED(pfd->Show(NULL)))
		{
			IShellItem* psi;
			if (SUCCEEDED(pfd->GetResult(&psi)))
			{
				PWSTR f_Path;
				if (SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &f_Path)))
				{
					std::wstring path(f_Path);
					std::string c(path.begin(), path.end());
					
					strcpy(szSelectedDir, c.c_str());
					Canceled = 0;
	
				}
				psi->Release();
			}
		}
		pfd->Release();
	}
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
	char pSearchPath[MAX_PATH];

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

// *************************************************************************
// *			Init_History:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void  CL64_File_IO::Init_History()
{
	char DirCheck[1024];
	strcpy(DirCheck, UserData_Folder);
	strcat(DirCheck, "\\");
	strcat(DirCheck, "OgreWin3D");

	bool check = 0;
	check = Check_File_Exist(DirCheck);
	if (check == 0)
	{
		mPreviousFiles.resize(EQUITY_NUM_RECENT_FILES);

		CreateDirectory(DirCheck, NULL);
		ResentHistory_Clear(); // Set all slots to Empty
		Save_FileHistory();
		LoadHistory();
	}
	else
	{
		LoadHistory();
	}
}
// *************************************************************************
// *			LoadHistory:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void  CL64_File_IO::LoadHistory()
{
	mPreviousFiles.resize(EQUITY_NUM_RECENT_FILES);

	char buffer[MAX_PATH];
	char buf[MAX_PATH];

	strcpy(buf, UserData_Folder);
	strcat(buf, "\\OgreWin3D\\OgreWin3D.ini");

	ReadRecentFiles = fopen(buf, "rt");

	if (!ReadRecentFiles)
	{
		App->Say("Cant Find Recent Files");
		return;
	}

	// Read in File Names from RecentFiles.ini
	for (unsigned int i = 0; i < EQUITY_NUM_RECENT_FILES; ++i)
	{
		memset(buffer, 0, MAX_PATH);
		fgets(buffer, 1024, ReadRecentFiles);

		char Path[1024];
		strcpy(Path, buffer);
		int Len = strlen(Path);
		Path[Len - 1] = 0;

		mPreviousFiles[i] = std::string(Path);
	}

	fclose(ReadRecentFiles);

	mHistoryMenu = CreateMenu();

	// Check for empty slots and gray out
	for (int i = EQUITY_NUM_RECENT_FILES - 1; i >= 0; --i)
	{
		char szText[1024];
		strcpy(szText, mPreviousFiles[i].c_str());

		UINT iFlags = 0;
		int Result = 0;
		Result = strcmp("<empty>", szText);
		if (Result == 0)
		{
			iFlags = MF_GRAYED | MF_DISABLED;
		}

		AppendMenu(mHistoryMenu, MF_STRING | iFlags, EQUITY_RECENT_FILE_ID(i), szText);
	}

	ModifyMenu(GetMenu(App->MainHwnd), ID_FILE_RECENTFILES, MF_BYCOMMAND | MF_POPUP,
		(UINT_PTR)mHistoryMenu, "Recent files");

	return;
}

// *************************************************************************
// *		RecentFileHistory_Update:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void  CL64_File_IO::RecentFileHistory_Update()
{

	if (!mHistoryMenu)return;

	std::string sz = std::string(Project_Path_File_Name);
	if (mPreviousFiles[EQUITY_NUM_RECENT_FILES - 1] == sz)return;

	// add the new file to the list of recent files
	for (unsigned int i = 0; i < EQUITY_NUM_RECENT_FILES - 1; ++i)
	{
		mPreviousFiles[i] = mPreviousFiles[i + 1];
	}

	mPreviousFiles[EQUITY_NUM_RECENT_FILES - 1] = sz;

	// Check for empty slots and gray out
	for (int i = EQUITY_NUM_RECENT_FILES - 1; i >= 0; --i)
	{
		char szText[1024];
		strcpy(szText, mPreviousFiles[i].c_str());

		UINT iFlags = 0;
		int Result = 0;
		Result = strcmp("<empty>", szText);
		if (Result == 0)
		{
			iFlags = MF_GRAYED | MF_DISABLED;
		}

		ModifyMenu(mHistoryMenu, EQUITY_RECENT_FILE_ID(i),
			MF_STRING | MF_BYCOMMAND | iFlags, EQUITY_RECENT_FILE_ID(i), szText);
	}

	// Save Changes
	Save_FileHistory();

	return;
}
// *************************************************************************
// *		ResentHistory_Clear:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void  CL64_File_IO::ResentHistory_Clear()
{
	App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Delete file history.", (LPSTR)"Are you sure all File history will be Deleted Procede.", (LPSTR)"");
	if (App->CL_Dialogs->Canceled == 1)
	{
		return;
	}

	// Set all slots to <empty>
	for (unsigned int i = 0; i < EQUITY_NUM_RECENT_FILES; ++i)
	{
		mPreviousFiles[i] = std::string("<empty>");
	}

	// Repopulate Menu system
	for (int i = EQUITY_NUM_RECENT_FILES - 1; i >= 0; --i)
	{
		ModifyMenu(mHistoryMenu, EQUITY_RECENT_FILE_ID(i),
			MF_STRING | MF_BYCOMMAND | MF_GRAYED | MF_DISABLED, EQUITY_RECENT_FILE_ID(i), "<empty>");
	}

	// Save Changes
	Save_FileHistory();
}

// *************************************************************************
// *			Save_FileHistory:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void  CL64_File_IO::Save_FileHistory()
{

	//	WriteRecentFiles = 0;

	char buf[1024];
	strcpy(buf, UserData_Folder);
	strcat(buf, "\\OgreWin3D\\OgreWin3D.ini");


	WriteRecentFiles = fopen(buf, "wt");

	if (!WriteRecentFiles)
	{
		App->Say("Why Cant Find Recent Files");
		return;
	}

	// Save out to RecentFile.ini
	for (unsigned int i = 0; i < EQUITY_NUM_RECENT_FILES; ++i)
	{
		char szName[1024];
		strcpy(szName, mPreviousFiles[i].c_str());

		fprintf(WriteRecentFiles, "%s\n", szName);
	}

	fclose(WriteRecentFiles);
	return;
}

