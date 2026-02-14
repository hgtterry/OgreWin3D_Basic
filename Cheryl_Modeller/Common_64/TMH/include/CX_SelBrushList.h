/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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

typedef struct tag_SelBrushList SelBrushList;
typedef signed int(*SelBrushList_Callback)(Brush* pBrush, void* lParam);

class CX_SelBrushList
{
public:
	CX_SelBrushList(void);
	~CX_SelBrushList(void);

	char* GetVersion();

	/*SelBrushList* SelBrushList_Create(void);
	void SelBrushList_Destroy(SelBrushList** ppList);
	signed int SelBrushList_Find(SelBrushList* pList, const Brush* pBrush);
	Brush* SelBrushList_GetBrush(SelBrushList* pList, int BrushIndex);
	int SelBrushList_GetSize(SelBrushList* pList);
	void SelBrushList_Center(SelBrushList* pList, T_Vec3* center);
	void SelBrushList_Enum(SelBrushList* pList, SelBrushList_Callback Callback, void* lParam);
	signed int SelBrushList_Add(SelBrushList* pList, Brush* pBrush);
	void SelBrushList_RemoveAll(SelBrushList* pList);
	signed int SelBrushList_Remove(SelBrushList* pList, Brush* pBrush);*/

};

