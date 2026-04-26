/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_File_IO.h"

#include <string>
#include <shobjidl_core.h>


CL64_File_IO::CL64_File_IO(void)
{
	mHistoryMenu = NULL;

	szSelectedDir[0] = 0;
	
	flag_Canceled = true;

	UserData_Folder[0] = 0;

	WriteRecentFiles = NULL;
	ReadRecentFiles = NULL;

	rgbCurrent = 0;
	mPreviousFiles.resize(0);
}

CL64_File_IO::~CL64_File_IO(void)
{
}

// *************************************************************************
// *				Select_Folder:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_File_IO::Select_Folder()
{
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (FAILED(f_SysHr))
	{
		App->Say("Can Not Create Browse Dialog");
		return;
	}

	strcpy(szSelectedDir, "");
	flag_Canceled = 1;

	IFileDialog* pfd;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
	{
		DWORD dwOptions;
		if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
		{
			pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
		}
		if (SUCCEEDED(pfd->Show(App->MainHwnd)))
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
					flag_Canceled = 0;

				}

				psi->Release();
				CoUninitialize();
			}
		}

		pfd->Release();
		CoUninitialize();
	}
}

// *************************************************************************
// *				Open_File:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_File_IO::Open_File(const LPCWSTR Type, const LPCWSTR Extensions)
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
	save_filter[0].pszName = Type;
	save_filter[0].pszSpec = Extensions;

	f_SysHr = f_FileSystem->SetFileTypes(1, save_filter);
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
	s_Path_And_File = c;

	//  FORMAT STRING FOR EXECUTABLE NAME
	const size_t slash = s_Path_And_File.find_last_of("/\\");
	s_Just_FileName = s_Path_And_File.substr(slash + 1);

	//  SUCCESS, CLEAN UP
	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();

	return TRUE;
}

// COMDLG_FILTERSPEC ComDlgFS[3] = {{L"C++ code files", L"*.cpp;*.h;*.rc"},{L"Executable Files", L"*.exe;*.dll"}, {L"All Files",L"*.*"}};

// *************************************************************************
// *				Save_File:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_File_IO::Save_File(const LPCWSTR Type, const LPCWSTR Extensions)
{
	flag_Canceled = true;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* pFileSave = NULL;

		HRESULT hr = CoCreateInstance(__uuidof(FileSaveDialog), NULL,
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileSave));

		if (SUCCEEDED(hr))
		{
			const COMDLG_FILTERSPEC c_rgSaveTypes[] =
			{
				{Type, Extensions}
			};

			LPCWSTR Test;
			wchar_t wtext[MAX_PATH];
			mbstowcs(wtext, App->CL_Level->MTF_Just_FileName, strlen(App->CL_Level->MTF_Just_FileName) + 1);
			Test = wtext;
			
			hr = pFileSave->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
			hr = pFileSave->SetFileName(Test);
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
						s_Path_And_File = c;
						
						const size_t slash = s_Path_And_File.find_last_of("/\\");
						s_Just_FileName = s_Path_And_File.substr(slash + 1);
						CoTaskMemFree(pszFilePath);

						flag_Canceled = false;
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
// *			Colour_Picker:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_File_IO::Colour_Picker()
{
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16]; // array of custom colors 

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = App->MainHwnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE)
	{
		//hbrush = CreateSolidBrush(cc.rgbResult);
		rgbCurrent = cc.rgbResult;
	}

	return 1;
}

// *************************************************************************
// *			Init_History:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void  CL64_File_IO::Init_History()
{
	char DirCheck[1024];
	strcpy(DirCheck, UserData_Folder);
	strcat(DirCheck, "\\");
	strcat(DirCheck, "Cheryl_3D");

	bool check = 0;
	check = Check_File_Exist(DirCheck);
	if (check == 0)
	{
		mPreviousFiles.resize(EQUITY_NUM_RECENT_FILES);

		CreateDirectory(DirCheck, NULL);

		char buf[1024];
		strcpy(buf, UserData_Folder);
		strcat(buf, "\\Cheryl_3D\\Cheryl_3D.ini");
		WriteRecentFiles = fopen(buf, "wt");
		fclose(WriteRecentFiles);

		ResentHistory_Clear(1); // Set all slots to Empty
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
	strcat(buf, "\\Cheryl_3D\\Cheryl_3D.ini");

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
		fgets(buffer, MAX_PATH, ReadRecentFiles);

		char Path[MAX_PATH];
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
		char szText[MAX_PATH];
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

	std::string sz = std::string(App->CL_Model->Loaded_PathFileName);
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
void  CL64_File_IO::ResentHistory_Clear(bool FirstTime)
{

	if (FirstTime == 0)
	{
		App->CL_Dialogs->YesNo((LPSTR)"Delete file history.", (LPSTR)"Are you sure all File history will be Deleted Procede.");
		if (App->CL_Dialogs->flag_Dlg_Canceled == true)
		{
			return;
		}
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

	char buf[MAX_PATH];
	strcpy(buf, UserData_Folder);
	strcat(buf, "\\Cheryl_3D\\Cheryl_3D.ini");


	WriteRecentFiles = fopen(buf, "wt");

	if (!WriteRecentFiles)
	{
		App->Say("Why Cant Find Recent Files");
		return;
	}

	// Save out to RecentFile.ini
	for (unsigned int i = 0; i < EQUITY_NUM_RECENT_FILES; ++i)
	{
		char szName[MAX_PATH];
		strcpy(szName, mPreviousFiles[i].c_str());

		fprintf(WriteRecentFiles, "%s\n", szName);
	}

	fclose(WriteRecentFiles);
	return;
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
// *				Open_HTML:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_File_IO::Open_HTML(char* HelpTitle)
{
	char Path[1024];
	strcpy(Path, App->RB_Directory_FullPath);
	strcat(Path, "\\");
	strcat(Path, HelpTitle);

	ShellExecute(0, "open", Path, 0, 0, SW_SHOW);
}

