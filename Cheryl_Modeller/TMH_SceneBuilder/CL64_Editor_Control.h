/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
class CL64_Editor_Control
{
public:
	CL64_Editor_Control(void);
	~CL64_Editor_Control(void);

	void Reset_Editor(void);

	void Start_Editor_MapBrush_Mode(void);

	void Start_Editor_Scene();
	void Set_Map_Editor_Startup();

	void Set_Map_View();
	void Set_3DEditor_View();

	bool flag_Scene_Editor_Active;
	bool flag_Map_Editor_Active;
	bool flag_Scene_Game_Running;

	bool flag_Mode_3DEditor_View;
	bool flag_Mode_Map_View;
	bool flag_Start_3D_View;

	HWND Parent_hWnd;				// Handle to the parent window for the editor

private:
	

};

