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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Model.h"

CL64_Model::CL64_Model()
{
	// Groups
	GroupCount = 0;
	VerticeCount = 0;
	FaceCount = 0;

	// Brushes
	BrushCount = 0;
	Brush_Face_Count = 0;

	JustName[0] = 0;

	int Count = 0;
	while (Count < 11999)
	{
		B_Brush[Count] = nullptr;
		Count++;
	}

	Count = 0;
	while (Count < 4999)
	{
		Group[Count] = nullptr;
		Count++;
	}

}

CL64_Model::~CL64_Model()
{
}

// *************************************************************************
// *			Create_Brush_XX:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
void CL64_Model::Create_Brush_XX(int Index)
{
	if (B_Brush[Index] != nullptr)
	{
		delete B_Brush[Index];
		B_Brush[Index] = nullptr;
	}

	B_Brush[Index] = new Base_Brush();

	B_Brush[Index]->Vertice_Count = 0;
	B_Brush[Index]->Face_Count = 0;
}

// *************************************************************************
// *		Create_Mesh_Group:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
void CL64_Model::Create_Mesh_Group(int Index)
{
	if (Group[Index] != nullptr)
	{
		delete Group[Index];
		Group[Index] = nullptr;
	}

	Group[Index] = new Base_Group();
}
