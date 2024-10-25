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
class CL64_Locations
{
public:

	CL64_Locations(void);
	~CL64_Locations(void);

	void Start_Locations_Dlg();
	void Create_Location_Entity(char* name);
	void Goto_Location(int Index);
	void Set_To_PlayerView();

	int ListBox_Index;

protected:

	static LRESULT CALLBACK Proc_Locations(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_To_FreeCam();
	void Delete_Location();
	void Save_Location();
	void Move_Player_To_Camera();

	void Init_Bmps_PlayerLocations();

	HWND Locations_Dlg_hWnd;

	bool Toggle_Player_Flag;
	bool Toggle_FreeCam_Flag;
};

