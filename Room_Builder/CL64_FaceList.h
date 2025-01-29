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

#pragma once

#include "CL64_Face.h"

class CL64_FaceList
{
public:
	CL64_FaceList(void);
	~CL64_FaceList(void);

	FaceList* FaceList_Create(int NumFaces);
	void FaceList_AddFace(FaceList* pList, Face* pFace);
	void FaceList_GetBounds(const FaceList* pList, Box3d* pBounds);
	Face* FaceList_GetFace(const FaceList* pList, int WhichFace);
	int	FaceList_GetNumFaces(const FaceList* pList);
	void FaceList_Destroy(FaceList** ppList);
	void FaceList_RemoveFace(FaceList* pList, int WhichFace);
	FaceList* FaceList_Clone(const FaceList* pList);
	void FaceList_SetDirty(FaceList* pList);
	signed int FaceList_GetUsedTextures(const FaceList* pList, geBoolean* WrittenTex, CL64_WadFile* WadFile);
	void FaceList_CopyFaceInfo(const FaceList* src, FaceList* dst);
	void FaceList_ClipFaceToList(const FaceList* fl, Face** f);
	void FaceList_Move(FaceList* pList, const T_Vec3* trans);
	signed int FaceList_Scale(FaceList* pList, const T_Vec3* ScaleVec);
	signed int FaceList_SetNextSelectedFace(FaceList* fl);
	Face* FaceList_GetSelectedFace(const FaceList* fl);

};

