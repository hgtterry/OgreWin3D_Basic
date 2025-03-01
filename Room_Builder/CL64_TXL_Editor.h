/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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

typedef	struct	BitmapEntry
{
	char* Name;
	geBitmap* Bitmap;
	HBITMAP		WinBitmap;
	HBITMAP		WinABitmap;
	unsigned	Flags;
	int			Dirty;
	bool		Deleted;
}	BitmapEntry;

typedef struct	TPack_WindowData
{
	HINSTANCE		Instance;
	int				BitmapCount;
	BitmapEntry* Bitmaps;
	BitmapEntry* SelectedEntry;
	BOOL			FileNameIsValid;
	char			TXLFileName[256];
	BOOL			Dirty;
}	TPack_WindowData;

class CL64_TXL_Editor
{
public:
	CL64_TXL_Editor();
	~CL64_TXL_Editor();

	void Start_Texl_Dialog();

	std::vector<std::string> Vec_Texture_Name;
	int Texture_Count;
	Ogre::String mFileString;

	long BasePicWidth;
	long BasePicHeight;
	long BasePicDepth;

	HBITMAP	Sel_BaseBitmap;

	// ----------------------------------------

	int Check_if_Name_Exist(const char* Name);

	char mFileName[MAX_PATH];
	char Add_Texture_FileName[MAX_PATH];
	
private:
	static LRESULT CALLBACK Proc_Texture_Lib(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static bool CALLBACK Proc_ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool Scan_Textures_Group(); // Private
	void UpDateList();
	bool SelectBitmap();
	void Texture_To_HBITMP(char* TextureFileName);

	void Create_New_pData();
	void Delete_pData();

	TPack_WindowData*	pData;
	BitmapEntry*		Current_Entry;
	BitmapEntry*		NewBitmapList[200];

	HWND TXL_Dlg_HWND;

	char mTextureName[MAX_PATH];
	char Just_Name[MAX_PATH];

};

