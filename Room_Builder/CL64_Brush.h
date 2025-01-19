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
#include "CL64_WadFile.h"

typedef struct tag_FaceList FaceList;
typedef struct tag_BrushList BrushList;

#define BRUSH_COUNT_MULTI 1
#define BRUSH_COUNT_LEAF  2
#define BRUSH_COUNT_CSG   4
#define BRUSH_COUNT_ALL (BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF | BRUSH_COUNT_CSG)
#define BRUSH_COUNT_NORECURSE 8

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
	char			Name[MAX_PATH];
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
typedef signed int (*Brush_FaceCallback)(Face* pFace, void* lParam);
typedef signed int(*Brush_CSGCallback)(const Brush* pBrush, void* lParam);

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
	signed int Brush_IsDetail(const Brush* b);
	signed int Brush_IsHollowCut(const Brush* b);

	int	BrushList_EnumLeafBrushes(const BrushList* pList, void* pVoid, BrushList_CB	CallBack);
	const BrushList* Brush_GetBrushList(const Brush* b);
	int	Brush_GetNumFaces(const Brush* b);
	Face* Brush_GetFace(const Brush* b, int i);
	int Get_Brush_Count(void);
	signed int Brush_IsSubtract(const Brush* b);
	Brush* Brush_Clone(Brush const* from);
	BrushList* BrushList_Clone(BrushList* inList);
	void Brush_SetName(Brush* b, const char* newname);
	void Brush_SetFaceListDirty(Brush* b);
	void Brush_Center(const Brush* b, T_Vec3* center);
	void Brush_EnumFaces(Brush* b, void* lParam, Brush_FaceCallback Callback);
	void Brush_UpdateChildFaces(Brush* b);
	signed int	Brush_TestBoundsIntersect(const Brush* b, const Box3d* pBox);
	signed int Brush_IsHollow(const Brush* b);
	const char* Brush_GetName(const Brush* b);
	signed int BrushList_GetUsedTextures(BrushList* BList, signed int* UsedTex, CL64_WadFile* WadFile);
	signed int Brush_GetUsedTextures(const Brush* b, signed int* UsedTex, CL64_WadFile* WadFile);
	signed int Brush_GetParent(const BrushList* pList, const Brush* b, Brush** bParent);
	Brush* Get_Brush_ByIndex(int Index);
	int BrushList_Count(BrushList const* pList,int CountFlags);
	void BrushList_ClearAllCSG(BrushList* pList);
	void BrushList_DoCSG(BrushList* inList, int mid, Brush_CSGCallback Callback, void* lParam);
	void BrushList_ClearCSGAndHollows(BrushList* inList, int mid);
	void BrushList_RebuildHollowFaces(BrushList* inList, int mid, Brush_CSGCallback Callback, void* lParam);
	void Brush_SplitByFace(Brush* ogb, Face* sf, Brush** fb, Brush** bb);
	void BrushList_Prepend(BrushList* pList, Brush* pBrush);
	signed int Brush_IsVisible(const Brush* b);
	void Brush_Move(Brush* b, const T_Vec3* trans);
	void BrushList_Move(BrushList* pList, const T_Vec3* trans);
	const Box3d* Brush_GetBoundingBox(const Brush* b);
	void Brush_Resize(Brush* b, float dx, float dy, int sides, int inidx, T_Vec3* fnscale, int* ScaleNum);
	int	Brush_GetModelId(const Brush* b);
	void Brush_ResizeFinal(Brush* b, int sides, int inidx, T_Vec3* fnscale);
	signed int Brush_Scale3d(Brush* b, const T_Vec3* mag);
	signed int BrushList_Scale3d(BrushList* pList, const T_Vec3* trans);
	Brush* Get_By_Index(int Index);
	signed int BrushList_Enum(BrushList const* pList, void* lParam, BrushList_CB CallBack);

};

