/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
class CL64_Resources
{
public:
	CL64_Resources();
	~CL64_Resources();

	void Start_Resources();
	void Load_Texture_Resources();
	bool View_Texture(char* FileName, HWND Owner_hDlg);

	// Project
	bool Create_Project_Resources_Group();
	bool Delete_Project_Resources_Group();
	bool Add_Resource_Location_Project(char* Resource_Location);

	Ogre::String Project_Resource_Group;
	Ogre::String mSelected_Resource_Group;

private:
	static LRESULT CALLBACK Proc_Resources(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


	void CreateListGeneral_FX(HWND hDlg);
	void ListView_OnClickOptions(LPARAM lParam);

	void Update_Resource_Groups_Combo(HWND hDlg);
	bool Scan_Resource_Group(Ogre::String ResourceGroup);
	bool Get_File_Extensions(char* FileName, int Index);
	int Show_Resource_Group_All();
	int Show_Resource_Group_Type(int mType);
	void Update_Counter(int Value, HWND hDlg);
	void Set_Title(HWND hDlg, char* Title);
	void Reset_Flags();
	void Set_Selection(int Index);

	char mSelected_File[MAX_PATH];
	char mbtext[MAX_PATH];

	Ogre::String mFileString;

	HWND FX_General_hLV;
	HWND Resource_Dlg_hWnd;

	int Extension_Type;

	int GroupSelIndex;
	int RV_Size;

	bool flag_Show_Group_All;
	bool flag_Show_App_Res;
	bool flag_Show_Demo_Res;
	bool flag_Show_All_Materials;
	bool flag_Show_All_Meshes;
	bool flag_Show_All_Textures;
	bool flag_Show_All_Skeleton;
	bool flag_Show_All_Overlays;

	std::vector<std::string> RV_FileName;
	std::vector<std::string> RV_Archive_GetName;
	std::vector<std::string> RV_Archive_GetType;
	std::vector<int> RV_File_Extension;

};

