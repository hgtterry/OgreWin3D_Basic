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
	/*Ogre::String FileName;
	Ogre::String Archive_GetName;
	Ogre::String Archive_GetType;*/
}Resource_Type;



class CL64_Resources
{
public:

	CL64_Resources(void);
	~CL64_Resources(void);

	void Reset_Class(void) const;

	void Start_Resources();

	void Load_OgreCFG_Resources(const Ogre::String& file);

	bool Scan_Resource_Group(Ogre::String ResourceGroup);
	bool Export_Texture(char* FileName) const;

	void Get_Resource_Groups();
	void Update_Resource_Groups_Combo(HWND hDlg);

	bool Ogre_ExternalResourceLoaded;

	Ogre::String Ogre_Loader_Resource_Group;
	Ogre::String mSelected_Resource_Group;

private:

	static LRESULT CALLBACK Resources_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Reset_Flags();

	void CreateListGeneral_FX(HWND hDlg);
	void ListView_OnClickOptions(LPARAM lParam);

	void Update_Counter(int Value, HWND hDlg);

	int ShowAllMaterials();
	int ShowUsedMaterials();

	void Start_List_Folders(HWND List, char* FileName, bool ListDlg);
	void List_Folders(HWND List, char* StartFolder, char* FileName, bool ListDlg);
	bool FindPath_New(char* File, char* Folder);

	void Show_Resource_Group(const Ogre::String& ResourceGroup);
	int ShowAllMeshes();
	int ShowAllTextures();


	int Show_Resource_Group_All();
	int Show_Resource_Group_Materials();
	int Show_Resource_Group_Meshes();

	void UnloadUserResources();
	Ogre::ConfigFile cf;
	std::string	ResourcesCfgFile;

	Ogre::String mFileString;

	char ResourcePath[MAX_PATH];

	bool flag_Show_App_Res;
	bool flag_Show_Demo_Res;
	bool flag_Show_All_Materials;
	bool flag_Show_Used_Materials;
	bool flag_Show_All_Meshes;
	bool flag_Show_All_Textures;

	HWND FX_General_hLV;

	char btext[MAX_PATH];
	
	int RV_Size;
	std::vector<std::string> RV_FileName;
	std::vector<std::string> RV_Archive_GetName;
	std::vector<std::string> RV_Archive_GetType;
};

