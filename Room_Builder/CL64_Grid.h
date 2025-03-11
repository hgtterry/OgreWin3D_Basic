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

#pragma once
class CL64_Grid
{
public:
	CL64_Grid();
	~CL64_Grid();

	void Grid_Update(bool Create);
	void Hair_Update(bool Create);

	void Face_Update(bool Create);
	void Face_Update2();

	Ogre::ManualObject* GridManual;
	Ogre::SceneNode* GridNode;

	Ogre::ManualObject* HairManual;
	Ogre::SceneNode* HairNode;

	Ogre::ManualObject* FaceManual;
	Ogre::SceneNode* FaceNode;

	Ogre::ColourValue ColourMain;
	Ogre::ColourValue ColourDivision;

	Ogre::ColourValue FaceColour;

	int XAxis_min;
	int	XAxis_max;
	int YAxis_min;
	int	YAxis_max;
	int ZAxis_min;
	int	ZAxis_max;

	int Division_X;
	int Division_Y;
	int Division_Z;

	int Scale_X;
	int Scale_Y;
	int Scale_Z;

	int HairExtend;
	Ogre::ColourValue ColourHairZ;
	Ogre::ColourValue ColourHairX;
	Ogre::ColourValue ColourHairY;

	Ogre::Vector3 HitVertices[3];

	bool flag_ShowGrid;
	bool flag_ShowDivisions;
};

