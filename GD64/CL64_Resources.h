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
class CL64_Resources
{
public:

	CL64_Resources(void);
	~CL64_Resources(void);

	void Reset_Class(void) const;

	void Start_Resources();

	void Load_OgreCFG_Resources(const Ogre::String& file);

	bool Ogre_ExternalResourceLoaded;

private:

	static LRESULT CALLBACK Resources_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Reset_Flags();
	void CreateListGeneral_FX(HWND hDlg);
	void Update_Counter(int Value, HWND hDlg);

	int ShowAllMaterials();
	int ShowUsedMaterials();

	void Start_List_Folders(HWND List, char* FileName, bool ListDlg);
	void List_Folders(HWND List, char* StartFolder, char* FileName, bool ListDlg);
	bool FindPath_New(char* File, char* Folder);

	void Show_Resource_Group(const Ogre::String& ResourceGroup);
	int ShowAllMeshes();
	int ShowAllTextures();

	void UnloadUserResources();
	Ogre::ConfigFile cf;
	std::string	ResourcesCfgFile;

	char ResourcePath[MAX_PATH];

	bool flag_Show_App_Res;
	bool flag_Show_Demo_Res;
	bool flag_Show_All_Materials;
	bool flag_Show_Used_Materials;
	bool flag_Show_All_Meshes;
	bool flag_Show_All_Textures;

	HWND FX_General_hLV;
};

