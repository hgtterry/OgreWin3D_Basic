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

// ------------------------ Game Options
typedef struct Game_Options
{
	bool flag_Show_FPS;
	bool flag_FullScreen;
	bool flag_Zipped_Assets_Flag;
	bool flag_Front_Dialog_Flag;

}Game_Options;

class CL64_Build_Game
{
public:

	CL64_Build_Game(void);
	~CL64_Build_Game(void);

	void Start_Project_Build();

	char StartFolder[MAX_PATH];
	char Desktop[MAX_PATH];

	char GameName[MAX_PATH];
	Game_Options* GameOptions;

	bool flag_Use_Front_Dlg;
	bool flag_Show_FPS;
	bool flag_Saved_Show_FPS;

protected:
	static LRESULT CALLBACK Project_Build_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Init_Build_Game_Class();

	bool Build_Project();
	bool Build_Project_Ini();
	bool Build_Level_Folder();
	bool Build_Main_Asset_Folder();

	bool Copy_Assets(char* SourceFolder, char* DestinationFolder);
	bool Zip_Assets(char* SourceFolder, char* DestinationFolder);

	bool Build_Area_Folder();
	bool Build_Areas_Data();
	bool Build_Players_Folder();
	bool Build_Player_Data();
	bool Build_Cameras_Folder();
	bool Build_Cameras_Data();
	bool Build_Objects_Folder();
	bool Build_Objects_Data();
	bool Build_Display_Folder();
	bool Build_Display_Data();

	void Copy_Sound_Files(void);
	void Copy_Particle_Files(void);

	//------------------------------------------
	void Read_From_Config(void);
	void Create_ProjectFolder(void);
	void Copy_SystemFiles(void);
	void Copy_ZipFiles(void);
	//void Create_Config_File(void);

	bool Directory_Altered;

	char ProjectFolder[MAX_PATH];
	char Sub_ProjectFolder[MAX_PATH];

	char Sub_Build_Folder[MAX_PATH];

	char SourceFile[MAX_PATH];
	char DestinationFile[MAX_PATH];
	char MediaFolder[MAX_PATH];

	char CoreDataFolder[MAX_PATH];
	char SoundFolder[MAX_PATH];
	char ParticleFolder[MAX_PATH];

	//------------------------------------------

	char m_Build_Sub_Folder[MAX_PATH];
	char m_Ini_Path_File_Name[MAX_PATH];
	char m_Level_Folder_Path[MAX_PATH];
	char m_Main_Assets_Path[MAX_PATH];
	char m_Aera_Folder_Path[MAX_PATH];
	char m_Players_Folder_Path[MAX_PATH];
	char m_Cameras_Folder_Path[MAX_PATH];
	char m_Objects_Folder_Path[MAX_PATH];
	char m_Display_Folder_Path[MAX_PATH];
	char m_Enviromnet_Folder_Path[MAX_PATH];

	HWND DlgHwnd;
	HWND Banner;

	FILE* WriteFile;

};

