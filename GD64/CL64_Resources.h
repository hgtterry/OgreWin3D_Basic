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
	bool View_Texture(char* FileName, HWND Owner_hDlg);
	bool Export_File(char* FileName);

	char mSelected_File[MAX_PATH];

	bool Ogre_ExternalResourceLoaded;

	HWND Resource_Dlg_hWnd;

	Ogre::String Ogre_Loader_Resource_Group;
	Ogre::String mSelected_Resource_Group;

private:

	static LRESULT CALLBACK Resources_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool Scan_Resource_Group(Ogre::String ResourceGroup);
	void Get_Resource_Groups();
	void Update_Resource_Groups_Combo(HWND hDlg);

	void Set_Title(HWND hDlg,char* Title);
	void Reset_Flags();
	void Set_Selection(int Index);

	void CreateListGeneral_FX(HWND hDlg);
	void ListView_OnClickOptions(LPARAM lParam);

	void Update_Counter(int Value, HWND hDlg);

	void Start_List_Folders(HWND List, char* FileName, bool ListDlg);
	void List_Folders(HWND List, char* StartFolder, char* FileName, bool ListDlg);
	bool FindPath_New(char* File, char* Folder);

	bool Get_File_Type(char* FileName, int Index);

	bool Export_Resource(char* FileName) const;

	int Show_Resource_Group_All();
	int Show_Resource_Group_Materials();
	int Show_Resource_Group_Meshes();
	int Show_Resource_Group_Textures();
	int Show_Resource_Group_Skeletons();

	void UnloadUserResources();
	Ogre::ConfigFile cf;
	std::string	ResourcesCfgFile;

	Ogre::String mFileString;

	char ResourcePath[MAX_PATH];

	bool flag_Show_Group_All;
	bool flag_Show_App_Res;
	bool flag_Show_Demo_Res;
	bool flag_Show_All_Materials;
	bool flag_Show_All_Meshes;
	bool flag_Show_All_Textures;
	bool flag_Show_All_Skeleton;

	HWND FX_General_hLV;
	HWND Export_Button;

	char mbtext[MAX_PATH];
	
	int RV_Size;
	std::vector<std::string> RV_FileName;
	std::vector<std::string> RV_Archive_GetName;
	std::vector<std::string> RV_Archive_GetType;
	std::vector<int> RV_File_Extension;
};

