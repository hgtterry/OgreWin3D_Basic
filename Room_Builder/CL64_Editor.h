/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "Base_Brush.h"
#include "Base_Group.h"
#include "Base_Player.h"

#pragma once
class CL64_Editor
{
public:
	CL64_Editor();
	~CL64_Editor();

	void Create_Brush_XX(int Index);
	void Create_Mesh_Group(int Index);

	void Preview_Mode(void);
	void Editor_Mode(void);

	Base_Group* Group[5000];
	Base_Brush* B_Brush[12000];

	char JustName[MAX_PATH];

	// Groups
	int VerticeCount;
	int GroupCount;
	int FaceCount;

	// Brushes
	int BrushCount;
	int Brush_Face_Count;

	// Player
	int Player_Count;
	bool flag_Player_Added;

	HWND Parent_hWnd;

	bool flag_PreviewMode_Running;
	bool flag_Show_Debug_Area;
	bool flag_Enable_Physics_Debug;

	std::vector<Base_Player*> B_Player;

};

