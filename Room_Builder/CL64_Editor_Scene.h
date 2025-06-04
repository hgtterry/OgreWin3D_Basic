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
class CL64_Editor_Scene
{
public:
	CL64_Editor_Scene();
	~CL64_Editor_Scene();

	void Start_Headers_Scene();

	void Start_Editor_Scene(void);
	void Return_From_Preview(void);

	void Back_To_Map_Editor(void);

	void Context_Menu_Ogre(HWND hDlg);
	bool Context_Command_Ogre(WPARAM wParam);

	void Show_Headers(bool Enable);

	bool flag_Scene_Editor_Active;
	bool flag_Environment_Available;
	bool flag_Show_Physics_Objects;

	HWND Scene_Headers_hWnd;

	HMENU hMenu;

private:
	static LRESULT CALLBACK Proc_Headers_Scene(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

};

