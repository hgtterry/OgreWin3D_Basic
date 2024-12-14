/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#pragma once
class CL64_Preferences
{
public:

	CL64_Preferences(void);
	~CL64_Preferences(void);

	void Clean_Up(void);

	void Preferences_Editor_ImGui();
	void Close_Preferences_Editor();

	void Start_Preferences_Dlg();
	void Read_Preferences();
	bool Write_Preferences();

	bool flag_Show_Preferences_Editor;
	int PropertyEditor_Page;
	bool flag_Preferences_Start_Pos;
	float Preferences_Pos_X;
	float Preferences_Pos_Y;

	bool flag_Start_FullScreen;
	bool flag_Start_Full_3DWin;
	bool flag_Use_Default_Directories;
	bool flag_Load_Last_Project;
	bool flag_Load_Test_Project;

	char Last_Loaded_File[MAX_PATH];

private:

	FILE* WriteData;
};

