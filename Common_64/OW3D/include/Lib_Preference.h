/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
class Lib_Preference
{
public:
	Lib_Preference(void);
	~Lib_Preference(void);

	char* GetVersion();

	void Start_Options_Dlg();
	void Read_Preferences();
	bool Write_Preferences();

	void Init_Configuration();
	void Save_Config_File();

	char Wad_File_Name[MAX_PATH];
	char UserData_Folder[MAX_PATH];

	int Grid_Fine_Spacing;
	int Grid_Spacing;
	float Defalut_Zoom;
	// Options
	bool flag_OpenLastFile;
	bool flag_MapEditor;
	bool flag_SceneEditor;

	char Prefs_PathAndFile[MAX_PATH];
	char Prefs_JustFileName[MAX_PATH];

private:
	static LRESULT CALLBACK Proc_Options_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	FILE* WriteData;

	bool Search_For_Folder(char* FolderPath);
	bool Check_File_Exist(char* Full_Path);
	void Config_SetDefaults();
	void Load_Config_File();

	FILE* WriteRecentFiles;
	FILE* ReadRecentFiles;
};

