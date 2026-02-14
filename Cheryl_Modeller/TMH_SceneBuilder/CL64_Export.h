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
class CL64_Export
{
public:
	CL64_Export();
	~CL64_Export();

	void Ogre_Export_Dlg();
	void Object_Export_Dlg();

	bool Create_ObjectFile(void);

	void Export_To_Object(void);

	char mFolder_Path[MAX_PATH];
	char mDirectory_Name[MAX_PATH];
	char mJustName[MAX_PATH];

	char DeskTop_Folder[MAX_PATH];

	char Object_OutputFolder[MAX_PATH];

	bool flag_Build_Edge_List;
	bool flag_Version_Latest;
	bool flag_Version_18;
	bool flag_Version_17;

	int mOgre_Version;

private:
	static LRESULT CALLBACK Proc_Ogre_Export_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Object_Export_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool Export_Object_Textures(void);
	void Write_ObjectFile(void);
	void Write_ObjectFile_Commit(void);
	bool WriteMTLFile(void);

	char Object_FileName[MAX_PATH];
	char mtl_FileName[MAX_PATH];
	char Just_mtl_FileName[MAX_PATH];

	FILE* Write_OBJECTFILE;
	FILE* Write_MTLFile;

};

