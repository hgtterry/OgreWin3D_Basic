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

#pragma once

struct tag_Box3d
{
	T_Vec3 Min;
	T_Vec3 Max;
};

typedef struct tag_Box3d Box3d;

class CX_Box
{
public:
	CX_Box(void);
	~CX_Box(void);

	char*GetVersion();

	void Box3d_Clear(Box3d* b);
	void Box3d_SetBogusBounds(Box3d* b);
	void Box3d_Set(Box3d* b, float x1, float y1, float z1, float x2, float y2, float z2);
	void Box3d_SetSize(Box3d* b, float sx, float sy, float sz);
	void Box3d_AddPoint(Box3d* b, float px, float py, float pz);
	bool Box3d_Intersection(const Box3d* b1, const Box3d* b2, Box3d* bResult);
	void Box3d_Union(const Box3d* b1, const Box3d* b2, Box3d* bResult);
	signed int Box3d_ContainsPoint(const Box3d* b, float px, float py, float pz);
	const T_Vec3* Box3d_GetMin(const Box3d* b);
	const T_Vec3* Box3d_GetMax(const Box3d* b);
	void Box3d_GetCenter(const Box3d* b, T_Vec3* pCenter);
	float Box3d_GetWidth(const Box3d* b);
	float Box3d_GetHeight(const Box3d* b);
	float Box3d_GetDepth(const Box3d* b);
	void Box3d_GetSize(const Box3d* b, T_Vec3* pSize);
	void Box3d_Scale(Box3d* b, float Scale);
	void Box3d_Move(Box3d* b, float dx, float dy, float dz);
	void Box3d_Inflate(Box3d* b, float dx, float dy, float dz);
};

