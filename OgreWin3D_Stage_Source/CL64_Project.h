/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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

// ------------------------ Load Options
typedef struct Load_Options
{
	int Has_Area;
	int Has_Player;
	int Has_Camera;
	int Has_Objects;
	int Has_Counters;

}Load_Options;

class CL64_Project
{
public:
	CL64_Project();
	~CL64_Project();

	void Start_Save_Project_Dialog();
	
	bool Load_Project();
	bool Load_Project_Objects();
	bool Load_Project_Aera();
	bool Load_Project_Player();
	bool Load_Project_Counters();

	bool Read_EnvironEntity(int Index, char* Section);
	bool Read_Message(int Index, char* Section);
	bool Read_Collectable(int Index, char* Section);
	bool Read_Teleport(int Index, char* Section);
	bool Read_Particle(int Index, char* Section);
	bool Read_MoveEntity(int Index, char* Section);

	bool Copy_Assets(char* SourceFolder, char* DestinationFolder);

	char m_Project_Name[MAX_PATH];
	char m_Level_Name[MAX_PATH];

	char m_Ini_Path_File_Name[MAX_PATH];
	char m_Level_File_Name[MAX_PATH];
	char m_Project_Sub_Folder[MAX_PATH];
	char m_Level_Folder_Path[MAX_PATH];

	char m_Main_Assets_Path[MAX_PATH];

	// Save
	char m_Aera_Folder_Path[MAX_PATH];
	char m_Players_Folder_Path[MAX_PATH];
	char m_Cameras_Folder_Path[MAX_PATH];

	bool Directory_Changed_Flag;

private:

	static LRESULT CALLBACK Save_Project_Dialog_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool Save_Project();
	bool Save_Project_Ini();
	bool Save_Level_Folder();
	bool Save_Main_Asset_Folder();
	bool Save_Aera_Folder();
	bool Save_Aeras_Data();
	bool Save_Players_Folder();
	bool Save_Player_Data();
	bool Save_Cameras_Folder();
	bool Save_Cameras_Data();

	void Set_Paths();
	bool Load_Get_Resource_Path();

	FILE* WriteFile;
};

