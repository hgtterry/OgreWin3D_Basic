/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
class CL64_Exp_Obj
{
public:

	CL64_Exp_Obj(void);
	~CL64_Exp_Obj(void);

	void Object_Export_Dlg();

	bool Create_ObjectFile(void);
	void Write_ObjectFile(void);
	void Write_ObjectFile_Commit(void);

	bool WriteMTLFile_Assimp(void);
	bool WriteMTLFile_Ogre(void);

	bool Export_Textures_Ogre();
	bool Export_Textures_Assimp(char* Path);

	char OutputFolder[MAX_PATH];
	char Object_FileName[MAX_PATH];
	char mtl_FileName[MAX_PATH];
	char Just_mtl_FileName[MAX_PATH];

	char DeskTop_Folder[MAX_PATH];
	char m_Out_JustName[MAX_PATH];
	char m_Out_Folder_Path[MAX_PATH];

	bool flag_First_Run;
	bool flag_File_Created;

	FILE* Write_OBJECTFILE;
	FILE* Write_MTLFile;

private:
	static LRESULT CALLBACK Proc_Object_Export_Dlg2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


};

