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
class CL64_Objects_Create
{
public:

	CL64_Objects_Create(void);
	~CL64_Objects_Create(void);

	void Add_Objects_From_MeshViewer();
	bool Dispatch_MeshViewer();

	bool Add_Objects_From_File();
	bool Add_New_Object(int Index, bool From_MeshViewer);

	void Add_Physics_Box(bool isDynamic, int index);
	void Add_Physics_Sphere(bool Dynamic, int Index);
	void Add_Physics_Capsule(bool Dynamic, int Index);
	void Add_Physics_Cylinder(bool Dynamic, int Index);
	void Add_Physics_Cone(bool Dynamic, int Index);
	void AddItemToFileView(HTREEITEM folder, int count);

	btBvhTriangleMeshShape* create_New_Trimesh(int Index);

};

