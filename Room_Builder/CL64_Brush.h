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
#include "CL64_Box.h"
#include "CL64_Face.h"

typedef struct tag_FaceList FaceList;
typedef struct tag_BrushList BrushList;

typedef struct BrushTag
{
	struct BrushTag* Prev, * Next;
	FaceList* Faces;			//null if multibrush
	BrushList* BList;			//null if csgbrush
	unsigned long	Flags;
	int				Type;
	int				ModelId;
	int				GroupId;
	float			HullSize;		//for hollows
	Ogre::int32		Color;
	char* Name;
	Box3d			BoundingBox;
	bool			Centre_Marker;
} Brush;

enum BrushTypeFlags
{
	BRUSH_MULTI = 0x0001,
	BRUSH_LEAF = 0x0002,
	BRUSH_CSG = 0x0004
};

typedef Brush* BrushIterator;
typedef signed int (*BrushList_CB)(Brush* pBrush, void* pVoid);

class CL64_Brush
{
public:
	CL64_Brush(void);
	~CL64_Brush(void);

	BrushList* BrushList_Create(void);
	Brush* Brush_Create(int Type, const FaceList* fl, const BrushList* BList);
	void BrushList_GetBounds(const BrushList* BList, Box3d* pBounds);
	void Brush_Bound(Brush* b);
	//void Brush_SetSubtract(Brush* b, const signed int bState);
	void Brush_SetSheet(Brush* b, const signed int bState);
	void Brush_SealFaces(Brush** b);
	void Brush_Destroy(Brush** b);
	void BrushList_Destroy(BrushList** ppList);
	void BrushList_DeleteAll(BrushList* pList);

	Brush* BrushList_GetFirst(BrushList* pList, BrushIterator* bi);
	Brush* BrushList_GetNext(BrushIterator* bi);
	Brush* BrushList_GetLast(BrushList* pList, BrushIterator* bi);
	Brush* BrushList_GetPrev(BrushIterator* bi);
	void BrushList_Remove(BrushList* pList, Brush* pBrush);

	Brush* Brush_CreateHollowFromBrush(const Brush* b);
	void Brush_SetHollowCut(Brush* b, const signed int bState);
	void BrushList_Append(BrushList* pList, Brush* pBrush);

	signed int Brush_IsMulti(const Brush* b);
	int	BrushList_EnumLeafBrushes(const BrushList* pList, void* pVoid, BrushList_CB	CallBack);
	const BrushList* Brush_GetBrushList(const Brush* b);
	int	Brush_GetNumFaces(const Brush* b);
	Face* Brush_GetFace(const Brush* b, int i);

};

