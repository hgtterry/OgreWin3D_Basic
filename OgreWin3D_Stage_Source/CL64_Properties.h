/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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
class CL64_Properties
{
public:

	CL64_Properties(void);
	~CL64_Properties(void);

	void Reset_Class();
	void Clear_Listview();

	void Start_Properties(void);

	bool Update_ListView_Camera();
	bool Update_ListView_Area();

	bool Update_ListView_Player();
	bool Update_ListView_Player_Physics();

	bool Update_ListView_Objects();
	bool Update_ListView_Collectables();
	bool Update_ListView_Sounds();
	bool Update_ListView_Move_Entities();
	bool Update_ListView_Messages();
	bool Update_ListView_Teleport();
	bool Update_ListView_Particles();
	bool Update_ListView_Counters();
	bool Update_ListView_Environs();
	
	void Mark_As_Altered(int Index);

	int Edit_Category;
	int Current_Selected_Object;

	bool flag_Properties_Dlg_Active;
	bool flag_Edit_Physics;

	HWND Properties_Dlg_hWnd;

private:
	static LRESULT CALLBACK Proc_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Create_Properties_hLV(void);
	void ListView_OnClickOptions(LPARAM lParam);

	bool Edit_Object(LPARAM lParam);

	bool Edit_Player(LPARAM lParam);
	bool Edit_Player_Physics(LPARAM lParam);

	bool Edit_Sounds(LPARAM lParam);
	bool Edit_Environs_OnClick(LPARAM lParam);
	bool Edit_Area_Onclick(LPARAM lParam);
	bool Edit_Camera_Onclick(LPARAM lParam);
	bool Edit_Collectables(LPARAM lParam);
	bool Edit_Counters_OnClick(LPARAM lParam);
	void Edit_Light_Onclick(LPARAM lParam);
	bool Edit_Messages(LPARAM lParam);
	void Edit_Particle(LPARAM lParam);
	bool Edit_Move_Entity(LPARAM lParam);
	bool Edit_Teleport_Entity(LPARAM lParam);

	char btext[20]; // ListView_GetItemText Text Holder for Compare

	HWND Properties_hLV;
};

