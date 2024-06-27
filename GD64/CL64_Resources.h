/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#pragma once
class CL64_Resources
{
public:

	CL64_Resources(void);
	~CL64_Resources(void);

	void Start_Resources();

	void Load_OgreCFG_Resources(const Ogre::String& file);

	bool Ogre_ExternalResourceLoaded;

private:

	static LRESULT CALLBACK Resources_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void CreateListGeneral_FX(HWND hDlg);

	void ShowAllMaterials();
	void ShowUsedMaterials();

	void Start_List_Folders(HWND List, char* FileName, bool ListDlg);
	void List_Folders(HWND List, char* StartFolder, char* FileName, bool ListDlg);
	bool FindPath_New(char* File, char* Folder);

	void Show_Resource_Group(const Ogre::String& ResourceGroup);
	bool ShowAllMeshes();
	void ShowAllTextures();

	void UnloadUserResources();
	Ogre::ConfigFile cf;
	std::string	ResourcesCfgFile;

	char ResourcePath[MAX_PATH];

	bool Show_App_Res_Flag;
	bool Show_Demo_Res_Flag;

	HWND FX_General_hLV;
};

