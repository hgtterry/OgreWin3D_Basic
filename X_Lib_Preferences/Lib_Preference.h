#pragma once
class Lib_Preference
{
public:
	Lib_Preference(void);
	~Lib_Preference(void);

	void Start_Options_Dlg();
	void Read_Preferences();
	bool Write_Preferences();

	void Init_Configuration();
	void Save_Config_File();

	char Wad_File_Name[MAX_PATH];
	char UserData_Folder[MAX_PATH];

	int Grid_Fine_Spacing;
	int Grid_Spacing;

	bool flag_OpenLastFile;

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

