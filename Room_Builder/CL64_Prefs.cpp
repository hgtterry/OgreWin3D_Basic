/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Prefs.h"

CL64_Prefs::CL64_Prefs(void)
{
	Grid_Fine_Spacing = 8;
	Grid_Spacing = 128;

	Wad_File_Name[0] = 0;
	UserData_Folder[0] = 0;

	WriteData = nullptr;

	WriteRecentFiles = nullptr;
	ReadRecentFiles = nullptr;
}

CL64_Prefs::~CL64_Prefs(void)
{
}

// *************************************************************************
// *			Read_Preferences:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Prefs::Read_Preferences()
{
	char chr_Tag1[MAX_PATH];
	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->RB_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Room_Builder\\Room_Builder.ini");

	App->CL_Ini_File->SetPathName(Preferences_Path);


	Grid_Fine_Spacing = App->CL_Ini_File->GetInt("Grid", "Grid_Fine_Spacing", 0, 10);
	Grid_Spacing = App->CL_Ini_File->GetInt("Grid", "Grid_Spacing", 0, 10);

	App->CL_Editor_Map->GridSize = Grid_Spacing;
	App->CL_Editor_Map->GridSnapSize = Grid_Fine_Spacing;

	App->CL_Ini_File->GetString("Wad", "Wad_File_Name", chr_Tag1, MAX_PATH);
	strcpy(Wad_File_Name, chr_Tag1);

}

// *************************************************************************
// *		Write_Preferences:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_Prefs::Write_Preferences()
{
	WriteData = nullptr;

	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->RB_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Room_Builder\\Room_Builder.ini");

	WriteData = fopen(Preferences_Path, "wt");
	if (!WriteData)
	{
		App->Say_Win("Cant Open Preference File");
		return 0;
	}

	fprintf(WriteData, "%s\n", "[Grid]");
	fprintf(WriteData, "%s%i\n", "Grid_Fine_Spacing=", Grid_Fine_Spacing);
	fprintf(WriteData, "%s%i\n", "Grid_Spacing=", Grid_Spacing);

	fprintf(WriteData, "%s\n", "[Wad]");
	fprintf(WriteData, "%s%s\n", "Wad_File_Name=", "Default.txl");

	fclose(WriteData);

	return 1;
}

// *************************************************************************
// *			Init_History:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_Prefs::Init_History()
{
	char DirCheck[MAX_PATH];
	strcpy(DirCheck, UserData_Folder);
	strcat(DirCheck, "\\");
	strcat(DirCheck, "OW3D_Dir");


	bool check = 0;
	check = Search_For_Folder(DirCheck);
	if (check == 0)
	{
		//mPreviousFiles_Files.resize(RECENT_FILES);

		CreateDirectory(DirCheck, NULL);
		IniFile_SetDefaults();

		Save_FileHistory_Files();

		//LoadHistory_Files();
	}
	else
	{
		char mCheckFile[MAX_PATH];
		strcpy(mCheckFile, DirCheck);
		strcat(mCheckFile, "\\OW3D_MeshBuilder.cfg");


		bool checkfile = Check_File_Exist(mCheckFile);

		if (checkfile == 1)
		{

		}
		else
		{
			//mPreviousFiles_Files.resize(RECENT_FILES);

			IniFile_SetDefaults();
			Save_FileHistory_Files();
			//LoadHistory_Files();
		}

		//LoadHistory_Files();
	}
}

// *************************************************************************
// *		ResentHistory_Files_Clear:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void CL64_Prefs::IniFile_SetDefaults()
{

	Save_FileHistory_Files();
}

// *************************************************************************
// *		Save_FileHistory_Files:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Prefs::Save_FileHistory_Files()
{
	WriteRecentFiles = nullptr;

	char buf[MAX_PATH];
	strcpy(buf, UserData_Folder);
	strcat(buf, "\\OW3D_Dir\\OW3D_MeshBuilder.cfg");

	WriteRecentFiles = std::fopen(buf, "wt");

	if (!WriteRecentFiles)
	{
		App->Say("Why Cant Find Recent Files");
		return;
	}

	fprintf(WriteRecentFiles, "%s\n", "[Version_Data]");
	fprintf(WriteRecentFiles, "%s%s\n", "Version=", "V1.0");

	//// Save out to Vima19.ini
	//for (unsigned int i = 0; i < RECENT_FILES; ++i)
	//{
	//	char szName[MAX_PATH];
	//	strcpy(szName, mPreviousFiles_Files[i].c_str());

	//	fprintf(WriteRecentFiles, "%s\n", szName);
	//}

	std::fclose(WriteRecentFiles);

	return;
}

// *************************************************************************
// *		Search_For_Folder:- Terry and Hazel Flanigan 2023		 	   *
// *************************************************************************
bool CL64_Prefs::Search_For_Folder(char* FolderPath)
{
	char pSearchPath[1024];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(pSearchPath, FolderPath);

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
// *		Check_File_Exist:- Terry and Hazel Flanigan 2022		 	   *
// *************************************************************************
bool CL64_Prefs::Check_File_Exist(char* Full_Path)
{
	char pSearchPath[1024];

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
