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

#define	M_PI ((geFloat)3.14159265358979323846f)

#define UNITS_DEGREES_TO_RADIANS(d) Units_DegreesToRadians(d)
#define UNITS_RADIANS_TO_DEGREES(r) Units_RadiansToDegrees(r)
#define Units_DegreesToRadians(d) ((((geFloat)(d)) * M_PI) / 180.0f)
#define Units_RadiansToDegrees(r) ((((geFloat)(r)) * 180.0f) / M_PI)

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
	void Face_GetBounds(const Face* f, Box3d* b);
	void Face_SetSheet(Face* f, const signed int bState);
	const GPlane* Face_GetPlane(const Face* f);
	signed int Face_IsFixedHull(const Face* f);
	Face* Face_CreateFromPlane(const GPlane* p, float Radius, int DibId);
	void Face_CopyFaceInfo(const Face* src, Face* dst);
	void Face_Destroy(Face** f);
	void Face_GetSplitInfo(const Face* f, const GPlane* p, float* dists, Ogre::uint8* sides, Ogre::uint8* cnt);
	void Face_Clip(Face* f, const GPlane* p, float* dists, Ogre::uint8* sides);
	const Ogre::Vector3* Face_GetPoints(const Face* f);
	int	Face_GetNumPoints(const Face* f);
	Face* Face_Clone(const Face* src);
	int	Face_GetTextureDibId(const Face* f);
	void Face_SetLightScale(Face* f, const float xScale, const float yScale);
	void Face_SetTextureScale(Face* f, const float xScale, const float yScale);
	void Face_SetTextureName(Face* f, const char* pName);
	void Face_SetTextureSize(Face* f, const int txSize, const int tySize);
	char const* Face_GetTextureName(const Face* f);
	void Face_GetTextureSize(const Face* f, int* ptxSize, int* ptySize);
	const TexInfo_Vectors* Face_GetTextureVecs(const Face* f);
	Face* Face_CloneReverse(const Face* src);
	void Face_Split(const Face* f, const GPlane* p, Face** ff, Face** bf, float* dists, Ogre::uint8* sides);
	void Face_MostlyOnSide(const Face* f, const GPlane* p, float* max, int* side);
	void Face_Move(Face* f, const Ogre::Vector3* trans);


};

