/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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

class CL64_Model
{
public:
	CL64_Model(void);
	~CL64_Model(void);

	void Set_Paths(void);
	void Set_BondingBox_Model(bool Create);
	void Clear_Model();
	int Selected_BoneIndex;

	int Model_Type;

	// Internal
	int GroupCount;
	int TextureCount;
	int MotionCount;
	int VerticeCount;
	int FaceCount;
	int BoneCount;

	// Brushes
	int BrushCount;		// Total number of brushes created
	int Brush_Face_Count;

	bool flag_Model_Loaded;
	bool flag_BoundingBox_Created;

	bool flag_Model_is_Modified;

	Ogre::Entity* Imported_Ogre_Ent;
	Ogre::SceneNode* Imported_Ogre_Node;

	Base_Brush* B_Brush[12000];

	Ogre::Vector3 BBox_Max;
	Ogre::Vector3 BBox_Min;
	Ogre::Vector3 BBox_Size;
	Ogre::Vector3 BBox_Centre;
	float BBox_Radius;

};

