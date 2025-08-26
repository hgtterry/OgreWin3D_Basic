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
class CL64_Properties_Scene
{
public:
	CL64_Properties_Scene();
	~CL64_Properties_Scene();

	void Start_Properties(void);
	void Create_Properties_hLV(void);
	void Show_Properties_Scene(bool Enable);

	bool Update_ListView_Player();
	bool Update_ListView_Environs();
	bool Update_ListView_Sounds();
	bool Update_ListView_Locations();
	void Update_ListView_Teleport();

	bool Update_ListView_Objects();
	bool Edit_Player(LPARAM lParam);
	bool Edit_Environs_OnClick(LPARAM lParam);
	void Edit_Object(LPARAM lParam);
	void Edit_Sounds(LPARAM lParam);
	void Edit_Locations(LPARAM lParam);
	void Edit_Teleport_Entity(LPARAM lParam);

	HWND Properties_Dlg_hWnd;
	HWND Properties_hLV;

	int Edit_Category;
	int Current_Selected_Object;
	int Last_Selected_Object;

	bool flag_Properties_Dlg_Active;
	bool flag_Edit_Physics;

private:
	static LRESULT CALLBACK Proc_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void ListView_OnClickOptions(LPARAM lParam);

	char btext[20]; // ListView_GetItemText Text Holder for Compare
};

