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

#pragma once
class CL64_File_IO
{
public:
	CL64_File_IO(void);
	~CL64_File_IO(void);

	bool StartBrowser(char* szInitDir);

	void Select_Folder();
	bool Open_File(const LPCWSTR Type, const LPCWSTR Extensions);
	bool Save_File(const LPCWSTR Type, const LPCWSTR Extensions);
	bool Colour_Picker();
	bool Check_File_Exist(char* Full_Path);

	char szSelectedDir[MAX_PATH];
	char BrowserMessage[MAX_PATH];

	std::string s_Path_And_File;
	std::string s_Just_FileName;

	DWORD rgbCurrent;        // initial color selection

	bool flag_Canceled;

	void  Init_History();
	void  LoadHistory();
	void  RecentFileHistory_Update();
	void  ResentHistory_Clear(bool FirstTime);
	void  Save_FileHistory();

	char UserData_Folder[MAX_PATH];

	FILE* WriteRecentFiles;
	FILE* ReadRecentFiles;

	std::vector<std::string> mPreviousFiles;

#define EQUITY_NUM_RECENT_FILES 0x8
#define EQUITY_RECENT_FILE_ID(_n_) (5000 + _n_)
	HMENU mHistoryMenu;

private:
	static int __stdcall BrowseCallbackProc(HWND  hwnd, UINT  uMsg, LPARAM  lParam, LPARAM  lpData);
};

