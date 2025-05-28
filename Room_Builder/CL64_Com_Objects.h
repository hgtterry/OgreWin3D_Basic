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
class CL64_Com_Objects
{
public:

	CL64_Com_Objects(void);
	~CL64_Com_Objects(void);

	float GetMesh_BB_Radius(SceneNode* mNode);
	Ogre::Vector3 GetMeshBoundingBoxSize(SceneNode* mNode);
	void Rename_Object(int Index);
	Ogre::Vector3 GetPlacement(int Distance = -6);
	void Hide_AllObjects_Except(int Index, bool Show);
	void Delete_Object();
	int GetIndex_By_Name(char* Name);
	void Clear_Modified_Objects();
	int CheckNames_Objects(const char* name);
	int Get_Adjusted_Object_Count(void) const;
	Ogre::Vector3 Get_BoundingBox_World_Centre(int objectIndex);
	void Show_Entities(bool Enable);

	bool flag_Show_Physics_Debug;
	bool flag_Hide_All_Except;
	bool flag_Show_Mesh_Debug;
	bool flag_Dont_Clear_Objects;
};

