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

//#include "CL64_Lib_Maths.h"
//
//typedef struct
//{
//	T_Vec3 uVec, vVec;
//	float uOffset, vOffset;
//} TexInfo_Vectors;
//
//typedef struct FaceTag Face;
//typedef struct TexInfoTag TexInfo;
//
//#define	M_PI ((float)3.14159265358979323846f)
//
//#define UNITS_DEGREES_TO_RADIANS(d) Units_DegreesToRadians(d)
//#define UNITS_RADIANS_TO_DEGREES(r) Units_RadiansToDegrees(r)
//#define Units_DegreesToRadians(d) ((((float)(d)) * M_PI) / 180.0f)
//#define Units_RadiansToDegrees(r) ((((float)(r)) * 180.0f) / M_PI)

class CX_Face
{
public:
	CX_Face(void);
	~CX_Face(void);

	Face* Face_Create(int NumPnts, const T_Vec3* pnts, int DibId);
	void Face_SetVisible(Face* f, const signed int bState);
	signed int Face_SetPlaneFromFace(Face* f);
	void Face_SetTexInfoPlane(TexInfo* t, T_Vec3 const* pNormal);
	void Face_InitTexInfo(TexInfo* t, T_Vec3 const* pNormal);
	void Face_InitFaceAngle(TexInfo* t, T_Vec3 const* pNormal);
	void Face_SetTextureDibId(Face* f, const int Dib);
	void Face_SetTexturePos(Face* f);
	void Face_GetBounds(const Face* f, Box3d* b);
	void Face_SetSheet(Face* f, const signed int bState);
	const GPlane* Face_GetPlane(const Face* f);
	signed int Face_IsFixedHull(const Face* f);
	Face* Face_CreateFromPlane(const GPlane* p, float Radius, int DibId);
	void Face_CopyFaceInfo(const Face* src, Face* dst);
	void Face_Destroy(Face** f);
	void Face_GetSplitInfo(const Face* f, const GPlane* p, float* dists, Ogre::uint8* sides, Ogre::uint8* cnt);
	void Face_Clip(Face* f, const GPlane* p, float* dists, Ogre::uint8* sides);
	const T_Vec3* Face_GetPoints(const Face* f);
	int	Face_GetNumPoints(const Face* f);
	Face* Face_Clone(const Face* src);
	int	Face_GetTextureDibId(const Face* f);
	void Face_SetLightScale(Face* f, const float xScale, const float yScale);
	void Face_SetTextureScale(Face* f, const float xScale, const float yScale);
	void Face_SetTextureName(Face* f, const char* pName);
	void Face_SetBrushName(Face* f, const char* pName);
	void Face_SetTextureSize(Face* f, const int txSize, const int tySize);
	char const* Face_GetTextureName(const Face* f);
	char const* Face_GetBrushName(const Face* f);

	//void FaceList_SetBrushNames(const FaceList* fl, const char* Name);

	void Face_GetTextureSize(const Face* f, int* ptxSize, int* ptySize);
	const TexInfo_Vectors* Face_GetTextureVecs(const Face* f);
	Face* Face_CloneReverse(const Face* src);
	void Face_Split(const Face* f, const GPlane* p, Face** ff, Face** bf, float* dists, Ogre::uint8* sides);
	void Face_MostlyOnSide(const Face* f, const GPlane* p, float* max, int* side);
	void Face_Move(Face* f, const T_Vec3* trans);
	void Face_SetSelected(Face* f, const signed int bState);
	signed int Face_Scale(Face* f, const T_Vec3* ScaleVec);
	void Face_XfmTexture(Face* f, const Matrix3d* pXfm);
	void Face_GetTextureScale(const Face* f, float* pxScale, float* pyScale);
	void Face_GetTextureShift(const Face* f, int* pxShift, int* pyShift);
	float Face_GetTextureRotate(const Face* f);
	void Face_SetTextureRotate(Face* f, const float Rotate);
	void Face_SetTextureShift(Face* f, const int xShift, const int yShift);
	void Face_SetFixedHull(Face* f, const signed int bState);

	void Select_Face_From_Index(int mFace_Index);

	bool Face_IsSelected(const Face* f);

	void Face_SetTextureLock(Face* f, const signed int bState);
	bool Face_IsTextureLocked(const Face* f);

	int Selected_Face_Index;
};

