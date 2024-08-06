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

#include "Shlobj.h"
#include "io.h"

CL64_File_IO::CL64_File_IO()
{
	Model_FileName[0] = 0;
	Model_Path_FileName[0] = 0;
	szSelectedDir[0] = 0;
	BrowserMessage[0] = 0;

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

#pragma warning( disable : 4090)
// *************************************************************************
// *							StartBrowser   							   *
// *************************************************************************
bool CL64_File_IO::StartBrowser(char* szInitDir)
{
	TCHAR dname[MAX_PATH * 2] = { 0 };
	IMalloc* imalloc; 
	HRESULT Test1 = SHGetMalloc(&imalloc);
	BROWSEINFO bi; ZeroMemory(&bi, sizeof(bi));

	bi.hwndOwner = App->MainHwnd;
	bi.pszDisplayName = dname;
	bi.lpszTitle = BrowserMessage;
	bi.lParam = (LPARAM)szInitDir;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	bi.lpfn = BrowseCallbackProc;

	HRESULT Test2 = CoInitialize(NULL);
	ITEMIDLIST* pidl = SHBrowseForFolder(&bi);

	if (pidl)
	{
		imalloc->Free(pidl);
		imalloc->Release();
		return 1;
	}

	imalloc->Free(pidl);
	imalloc->Release();

	return 0;
}

// *************************************************************************
// *						BrowseCallbackProc   						   *
// *************************************************************************
int __stdcall CL64_File_IO::BrowseCallbackProc(HWND  hwnd, UINT  uMsg, LPARAM  lParam, LPARAM  lpData)
{
	//Initialization callback message
	if (uMsg == BFFM_INITIALIZED)
	{
		//SendMessage(hWnd, BFFM_SETSELECTION, 1, (LPARAM) szInitialPathName); 

		LPCTSTR path = reinterpret_cast<LPCTSTR>(lpData);

		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)path);

	}

	if (uMsg == BFFM_SELCHANGED)
	{
		TCHAR szDir[MAX_PATH * 2] = { 0 };

		BOOL bRet = SHGetPathFromIDList((LPITEMIDLIST)lParam, szDir);
		if (bRet)
		{
			if (_taccess(szDir, 00) != 0)
			{
				bRet = FALSE;
			}
			else
			{
				SHFILEINFO sfi;
				::SHGetFileInfo((LPCTSTR)lParam, 0, &sfi, sizeof(sfi),
					SHGFI_PIDL | SHGFI_ATTRIBUTES);

				if (sfi.dwAttributes & SFGAO_LINK)
					bRet = FALSE;
			}
		}

		if (!bRet)
		{
			::EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
			strcpy(App->CL_File_IO->szSelectedDir, "");
		}
		else
			strcpy(App->CL_File_IO->szSelectedDir, szDir);
	}

	return 0;
}

