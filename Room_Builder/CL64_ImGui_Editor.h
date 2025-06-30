/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

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
class CL64_ImGui_Editor
{
public:
	CL64_ImGui_Editor();
	~CL64_ImGui_Editor();

	void Dimensions_GUI(void);
	void ImGui_Render_Editor_Loop(void);

	void Visuals_GUI(void);

	int Visuals_PosX;
	int Visuals_PosY;
	bool flag_Show_Visuals;

	int Dimensions_PosX;
	int Dimensions_PosY;
	bool flag_Show_Dimensions;
	bool flag_Show_Position;
	bool flag_Show_Scale;
	bool flag_Show_Rotation;

	bool flag_Block_GUI;
	bool flag_Object_Highlighted;
	bool flag_Show_Physics_Debug;
	bool flag_Show_Mesh;

private:

	void Selected_Button(bool IsSelected);

};

