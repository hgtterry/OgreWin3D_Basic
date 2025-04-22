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
	char	Name[20];
	char	FileName[20];
	int		Dib_Index;
	bool	Dirty;
	bool	Deleted;
	bool	Has_Alpha;
	int		Width;
	int		Height;
}	BitmapEntry;

class CL64_TXL_Editor
{
public:
	CL64_TXL_Editor();
	~CL64_TXL_Editor();

	void Start_Texl_Dialog();
	void Scan_Textures_Resource_Group();

	int GetIndex_From_FileName(const char* Name);
	int GetIndex_From_Name(const char* Name);
	void Select_From_TextureName(const char* TextureName);
	int Get_Used_Textures_Count();

	bool flag_Texl_Dialog_Active;

	int Texture_Count;
	int Selected_Texure_Index;

	Ogre::String mFileString;

	long BasePicWidth;
	long BasePicHeight;
	long BasePicDepth;

	HBITMAP	Sel_BaseBitmap;
	BitmapEntry* Texture_List[200];
	// ----------------------------------------

	int Check_if_Name_Exist(const char* Name);
	int Check_if_FileName_Exist(const char* Name);
	bool Check_If_Texture_Used(const char* TextureName);

	char mFileName[MAX_PATH];
	char Add_Texture_FileName[MAX_PATH];

	bool flag_Textures_Scanned;
	
private:
	static LRESULT CALLBACK Proc_Texl_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static bool CALLBACK Proc_ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void UpDateList();
	bool SelectBitmap();
	void Texture_To_HBITMP(char* TextureFileName);
	void Update_Texture_Info(int Index);

	void Delete_File(const char* File);
	bool Add_File();

	HWND TXL_Dlg_HWND;

	char m_Selected_TextureName[MAX_PATH];
	char Just_Name[MAX_PATH];

};

