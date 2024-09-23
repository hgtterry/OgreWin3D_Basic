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

typedef struct Resource_Type {
	std::vector<Ogre::String*> FileName2;
}Resource_Type;


class CL64_Resources
{
public:

	CL64_Resources(void);
	~CL64_Resources(void);

	void Reset_Class(void);
	void Start_Resources();
	void Load_OgreCFG_Resources(const Ogre::String& file);
	void Destroy_Resources_Group(Ogre::String ResourceGroup);

	bool Create_Project_Resources_Group();
	bool Add_Resource_Location_Project(char* Resource_Location);

	bool View_Texture(char* FileName, HWND Owner_hDlg);
	bool View_File(char* FileName, HWND Owner_hDlg);

	bool Export_File(char* FileName);

	bool Project_Resources_Created;

	char mSelected_File[MAX_PATH];

	char Resource_File_Path_And_File[MAX_PATH];
	char Resource_File_FileName[MAX_PATH];

	bool Ogre_ExternalResourceLoaded;

	HWND Resource_Dlg_hWnd;

	Ogre::String Project_Resource_Group;
	Ogre::String Ogre_Loader_Resource_Group;
	Ogre::String mSelected_Resource_Group;
	Ogre::String mFileString;

private:

	static LRESULT CALLBACK Proc_Resources(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool Scan_Resource_Group(Ogre::String ResourceGroup);
	void Get_Resource_Groups();
	void Update_Resource_Groups_Combo(HWND hDlg);

	void Set_Title(HWND hDlg,char* Title);
	void Reset_Flags();
	void Set_Selection(int Index);

	void CreateListGeneral_FX(HWND hDlg);
	void ListView_OnClickOptions(LPARAM lParam);

	void Update_Counter(int Value, HWND hDlg);

	bool Get_File_Extensions(char* FileName, int Index);

	bool Export_Resource(char* FileName) const;

	int Show_Resource_Group_Type(int mType);
	int Show_Resource_Group_All();

	int Extension_Type;
	int GroupSelIndex;

	void UnloadUserResources();

	Ogre::ConfigFile cf;
	std::string	ResourcesCfgFile;

	char ResourcePath[MAX_PATH];

	bool flag_Show_Group_All;
	bool flag_Show_App_Res;
	bool flag_Show_Demo_Res;
	bool flag_Show_All_Materials;
	bool flag_Show_All_Meshes;
	bool flag_Show_All_Textures;
	bool flag_Show_All_Skeleton;
	bool flag_Show_All_Overlays;

	HWND FX_General_hLV;
	HWND Export_Button;

	char mbtext[MAX_PATH];
	
	int RV_Size;
	std::vector<std::string> RV_FileName;
	std::vector<std::string> RV_Archive_GetName;
	std::vector<std::string> RV_Archive_GetType;
	std::vector<int> RV_File_Extension;
};

