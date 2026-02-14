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

#include "Base_Brush.h"
#include "Base_Group.h"
#include "Base_Object.h"

#pragma once
class CL64_Scene
{
public:
	CL64_Scene();
	~CL64_Scene();

	void Reset_Class();

	void Create_Brush_XX(int Index);
	void Create_Mesh_Group(int Index);
	void Create_Location_Base(int Index);

	void Clear_Level(bool FromFile);

	Base_Brush* B_Brush[12000];

	char JustName[MAX_PATH];

	// Resources
	bool flag_Project_Resources_Created;

	// Groups
	int VerticeCount;
	int GroupCount;		// Total number of groups created
	int FaceCount;

	// Brushes
	int BrushCount;		// Total number of brushes created
	int Brush_Face_Count;

	// Scene Objects
	int Object_Count;
	int UniqueID_Object_Counter;

	// Ogre
	int Ogre_Face_Count;

	bool flag_Show_Debug_Area;		// Flag to indicate if the debug area should be displayed
	bool flag_Enable_Physics_Debug; // Flag to indicate if physics debugging is enabled

	std::vector<Base_Object*> B_Object; // Vector to hold pointers to Base_Object objects

};

