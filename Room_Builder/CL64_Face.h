/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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

typedef struct
{
	Ogre::Vector3 uVec, vVec;
	float uOffset, vOffset;
} TexInfo_Vectors;

typedef struct FaceTag Face;
typedef struct TexInfoTag TexInfo;

#pragma once
class CL64_Face
{
public:
	CL64_Face(void);
	~CL64_Face(void);

	Face* Face_Create(int NumPnts, const Ogre::Vector3* pnts, int DibId);
	void Face_SetVisible(Face* f, const signed int bState);
	signed int Face_SetPlaneFromFace(Face* f);
	void Face_SetTexInfoPlane(TexInfo* t, Ogre::Vector3 const* pNormal);
	void Face_InitTexInfo(TexInfo* t, Ogre::Vector3 const* pNormal);
	void Face_InitFaceAngle(TexInfo* t, Ogre::Vector3 const* pNormal);
	void Face_SetTextureDibId(Face* f, const int Dib);
	void Face_SetTexturePos(Face* f);


};

