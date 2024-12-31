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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Brush.h"

struct tag_BrushList
{
	Brush* First;
	Brush* Last;
};

enum BrushFlags
{
	BRUSH_SOLID = 0x0001,
	BRUSH_WINDOW = 0x0002,
	BRUSH_WAVY = 0x0004,
	BRUSH_DETAIL = 0x0008,	//not included in vis calculations
	BRUSH_HOLLOWCUT = 0x0010,
	BRUSH_TRANSLUCENT = 0x0020,
	BRUSH_EMPTY = 0x0040,
	BRUSH_SUBTRACT = 0x0080,
	BRUSH_CLIP = 0x0100,
	BRUSH_FLOCKING = 0x0200,
	BRUSH_HOLLOW = 0x0400,
	BRUSH_SHEET = 0x0800,
	BRUSH_HIDDEN = 0x1000,
	BRUSH_LOCKED = 0x2000,
	BRUSH_HINT = 0x4000,
	BRUSH_AREA = 0x8000
	// All flags larger than 0x8000 (i.e. 0x00010000 through 0x80000000)
	// are reserved for user contents.
};

static	float			dists[256];
static	Ogre::uint8		sides[256];
static	Ogre::uint8		fsides[256];

enum SideFlags
{
	SIDE_FRONT = 0,
	SIDE_BACK = 1,
	SIDE_ON = 2,
	SIDE_SPLIT = 3
};

#define BOGUS_RANGE					32000.0f

CL64_Brush::CL64_Brush(void)
{
}

CL64_Brush::~CL64_Brush(void)
{
}

typedef void (*BrushList_FlagCB)(Brush* pBrush, const signed int bState);
typedef void (*BrushList_IntCB)(Brush* pBrush, const int iVal);

// *************************************************************************
// *						BrushList_SetFlag							   *
// *************************************************************************
static void BrushList_SetFlag(BrushList* bl, const signed int bState, BrushList_FlagCB cbSetFlag)
{
	Brush* b;

	assert(bl != NULL);

	for (b = bl->First; b; b = b->Next)
	{
		cbSetFlag(b, bState);
	}
}

static void	BrushList_SetInt(BrushList* bl, const int iVal, BrushList_IntCB cbSetInt)
{
	Brush* b;

	assert(bl != NULL);

	for (b = bl->First; b; b = b->Next)
	{
		cbSetInt(b, iVal);
	}
}

// *************************************************************************
// *							BrushList_Create						   *
// *************************************************************************
BrushList* CL64_Brush::BrushList_Create(void)
{
	BrushList* pList;

	pList = (BrushList*)App->CL_Maths->Ram_Allocate(sizeof(BrushList));
	if (pList != NULL)
	{
		pList->First = NULL;
		pList->Last = NULL;
	}
	return pList;
}

// *************************************************************************
// *							Brush_Create							   *
// *************************************************************************
Brush* CL64_Brush::Brush_Create(int Type, const FaceList* fl, const BrushList* BList)
{
	Brush* pBrush;

	pBrush = (Brush*)App->CL_Maths->Ram_Allocate(sizeof(Brush));
	if (pBrush != NULL)
	{
		pBrush->Prev = NULL;
		pBrush->Next = NULL;
		pBrush->ModelId = 0;
		pBrush->GroupId = 0;
		pBrush->HullSize = 1.0f;
		pBrush->Color = 0;
		pBrush->Name = (LPSTR)"NoName";
		pBrush->Type = Type;
		pBrush->Centre_Marker = 0;
		switch (Type)
		{
		case	BRUSH_MULTI:
			assert(fl == NULL);
			assert(BList);

			pBrush->Faces = NULL;
			pBrush->BList = (BrushList*)BList;
			pBrush->Flags = BRUSH_SOLID;
			BrushList_GetBounds(BList, &pBrush->BoundingBox);
			break;

		case	BRUSH_LEAF:
		case	BRUSH_CSG:
			assert(fl);
			assert(BList == NULL);	//shouldn't create leaf from multiple

			pBrush->Faces = (FaceList*)fl;
			pBrush->BList = NULL;
			pBrush->Flags = BRUSH_SOLID;
			App->CL_FaceList->FaceList_GetBounds(fl, &pBrush->BoundingBox);
			break;

		default:
			assert(0);
		}
	}
	return pBrush;
}

// *************************************************************************
// *						BrushList_GetBounds							   *
// *************************************************************************
void CL64_Brush::BrushList_GetBounds(const BrushList* BList, Box3d* pBounds)
{
	Box3d	Bounds;
	Box3d	BrushBounds;
	Brush* b;

	assert(BList != NULL);
	assert(pBounds != NULL);

	b = BList->First;
	Brush_Bound(b);
	Bounds = b->BoundingBox;

	for (; b; b = b->Next)
	{
		Brush_Bound(b);
		BrushBounds = b->BoundingBox;

		App->CL_Box->Box3d_Union(&Bounds, &BrushBounds, &Bounds);
	}
	*pBounds = Bounds;
}

// *************************************************************************
// *							Brush_Bound								   *
// *************************************************************************
void CL64_Brush::Brush_Bound(Brush* b)
{
	assert(b);

	App->CL_Box->Box3d_SetBogusBounds(&b->BoundingBox);
	if (b->Type == BRUSH_MULTI)
	{
		BrushList_GetBounds(b->BList, &b->BoundingBox);
	}
	else
	{
		App->CL_FaceList->FaceList_GetBounds(b->Faces, &b->BoundingBox);
	}
}

typedef void (*BrushList_FloatCB)(Brush* pBrush, const float fVal);
typedef void (*BrushList_Uint32CB)(Brush* pBrush, const Ogre::uint32 uVal);



static void Brush_SetExclusiveState(Brush* b, int Mask, signed int bState)
{
	if (bState)
	{
		b->Flags &= ~(BRUSH_SOLID | BRUSH_CLIP | BRUSH_WINDOW | BRUSH_HINT | BRUSH_SUBTRACT | BRUSH_EMPTY);
		b->Flags |= Mask;
	}
	else
	{
		b->Flags &= ~Mask;
	}
}

// *************************************************************************
// *						Brush_SetSubtract							   *
// *************************************************************************
void Brush_SetSubtract(Brush* b, const signed int bState)
{

	if (b->Type == BRUSH_MULTI) // hgtterry Debug
	{
		BrushList_SetFlag(b->BList, bState, Brush_SetSubtract);
	}

	if (b->Flags & BRUSH_HOLLOWCUT)
	{
		return;	//can't reset these
	}

	Brush_SetExclusiveState(b, BRUSH_SUBTRACT, bState);
}

// *************************************************************************
// *						Brush_SetSheet								   *
// *************************************************************************
void CL64_Brush::Brush_SetSheet(Brush* b, const signed int bState)
{
	int		i;
	Face* f;

	if (b->Type != BRUSH_MULTI)
	{
		b->Flags = (bState) ? b->Flags | BRUSH_SHEET : b->Flags & ~BRUSH_SHEET;

		f = App->CL_FaceList->FaceList_GetFace(b->Faces, 0);
		App->CL_Face->Face_SetSheet(f, bState);

		for (i = 1; i < App->CL_FaceList->FaceList_GetNumFaces(b->Faces); i++)
		{
			f = App->CL_FaceList->FaceList_GetFace(b->Faces, i);
			App->CL_Face->Face_SetVisible(f, !bState);
		}
	}
}

// *************************************************************************
// *						Brush_SetHollow								   *
// *************************************************************************
void Brush_SetHollow(Brush* b, const signed int bState)
{
	assert(b != NULL);

	if (b->Type == BRUSH_MULTI)
	{
		BrushList_SetFlag(b->BList, bState,Brush_SetHollow);
	}
	if (b->Flags & BRUSH_HOLLOWCUT)
	{
		return;	//can't reset these
	}

	b->Flags = (bState) ? b->Flags | BRUSH_HOLLOW : b->Flags & ~BRUSH_HOLLOW;
}

// *************************************************************************
// *						BrushList_SetFloat							   *
// *************************************************************************
static void	BrushList_SetFloat(BrushList* bl, const float fVal, BrushList_FloatCB cbSetFloat)
{
	Brush* b;

	assert(bl != NULL);

	for (b = bl->First; b; b = b->Next)
	{
		cbSetFloat(b, fVal);
	}
}

// *************************************************************************
// *						Brush_SetHullSize							   *
// *************************************************************************
void Brush_SetHullSize(Brush* b, const float hsize)
{
	float RealHullSize = hsize;

	assert(b != NULL);

	if (hsize < 1.0f)
	{
		RealHullSize = 1.0f;
	}
	if (b->Type == BRUSH_MULTI)
	{
		BrushList_SetFloat(b->BList, RealHullSize, Brush_SetHullSize);
	}

	b->HullSize = RealHullSize;
}

// *************************************************************************
// *				Brush_CreateHollowFromBrush							   *
// *************************************************************************
Brush* CL64_Brush::Brush_CreateHollowFromBrush(const Brush* b)
{
	Face* f, * sf;
	GPlane		ExpandPlane;
	const GPlane* p;
	Ogre::Vector3 pnt;
	FaceList* fl;
	int			i;
	Brush* b2;

	assert(b);

	fl = App->CL_FaceList->FaceList_Create(App->CL_FaceList->FaceList_GetNumFaces(b->Faces));

	for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(b->Faces); i++)
	{
		f = App->CL_FaceList->FaceList_GetFace(b->Faces, i);
		p = App->CL_Face->Face_GetPlane(f);

		if (App->CL_Face->Face_IsFixedHull(f))
		{
			ExpandPlane.Normal = p->Normal;
			ExpandPlane.Dist = p->Dist;
		}
		else
		{
			App->CL_Maths->Vector3_Scale(&p->Normal, -b->HullSize, &pnt);
			App->CL_Maths->Vector3_MA(&pnt, p->Dist, &p->Normal, &pnt);

			ExpandPlane.Normal = p->Normal;
			ExpandPlane.Dist = App->CL_Maths->Vector3_DotProduct(&ExpandPlane.Normal, &pnt);
		}
		//create a face from the inner plane
		sf = App->CL_Face->Face_CreateFromPlane(&ExpandPlane, BOGUS_RANGE, 0);
		App->CL_Face->Face_CopyFaceInfo(f, sf);

		App->CL_FaceList->FaceList_AddFace(fl, sf);
	}
	b2 = Brush_Create(BRUSH_LEAF, fl, NULL);
	if (b2)
	{
		Brush_SealFaces(&b2);
	}

	return	b2;
}

// *************************************************************************
// *						Brush_SealFaces								   *
// *************************************************************************
void CL64_Brush::Brush_SealFaces(Brush** b)
{
	const GPlane* p;
	Face* f2;
	Face* f;
	FaceList* fl;
	int			i, j;
	Ogre::uint8		cnt[3];

	assert(b != NULL);
	assert(*b != NULL);
	assert((*b)->Type != BRUSH_MULTI);

	if (App->CL_FaceList->FaceList_GetNumFaces((*b)->Faces) < 4)
	{
		Brush_Destroy(b);
		return;
	}
	fl = App->CL_FaceList->FaceList_Create(App->CL_FaceList->FaceList_GetNumFaces((*b)->Faces));

	//expand all faces out to seal cracks
	for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces((*b)->Faces); i++)
	{
		f = (Face*)App->CL_FaceList->FaceList_GetFace((*b)->Faces, i);
		p = App->CL_Face->Face_GetPlane(f);
		f2 = App->CL_Face->Face_CreateFromPlane(p, BOGUS_RANGE, 0);
		App->CL_Face->Face_CopyFaceInfo(f, f2);
		App->CL_FaceList->FaceList_AddFace(fl, f2);
	}
	for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(fl); i++)
	{
		//const override here
		f = (Face*)App->CL_FaceList->FaceList_GetFace(fl, i);

		for (j = 0; j < App->CL_FaceList->FaceList_GetNumFaces(fl); j++)
		{
			if (j == i)
			{
				continue;
			}
			f2 = App->CL_FaceList->FaceList_GetFace(fl, j);
			p = App->CL_Face->Face_GetPlane(f2);

			App->CL_Face->Face_GetSplitInfo(f, p, dists, sides, cnt);

			if (!cnt[SIDE_FRONT] && !cnt[SIDE_BACK])	//coplanar
			{
				App->CL_FaceList->FaceList_RemoveFace(fl, i);
				j--;	//to ensure we restart if this is the last face
				break;
			}
			else if (!cnt[SIDE_FRONT])	//back
			{
				continue;
			}
			else if (!cnt[SIDE_BACK])	//front
			{
				App->CL_FaceList->FaceList_RemoveFace(fl, i);
				j--;	//to ensure we restart if this is the last face
				break;
			}
			else	//split
			{
				App->CL_Face->Face_Clip(f, p, dists, sides);
			}
		}
		if (j < App->CL_FaceList->FaceList_GetNumFaces(fl))
		{
			i = -1;	//restart!
		}
	}
	if (App->CL_FaceList->FaceList_GetNumFaces(fl) < 4)
	{
		//		ConPrintf("Overconstrained brush clipped away...\n");
		Brush_Destroy(b);
		App->CL_FaceList->FaceList_Destroy(&fl);
	}
	else
	{
		App->CL_FaceList->FaceList_Destroy(&(*b)->Faces);
		(*b)->Faces = fl;
		Brush_Bound((*b));
	}
}

// *************************************************************************
// *						Brush_Destroy								   *
// *************************************************************************
void CL64_Brush::Brush_Destroy(Brush** b)
{
	if ((*b)->Type != BRUSH_CSG)
	{
		if ((*b)->BList)
		{
			BrushList_Destroy(&((*b)->BList));
		}
	}
	if ((*b)->Type != BRUSH_MULTI)
	{
		if ((*b)->Faces)
		{
			App->CL_FaceList->FaceList_Destroy(&((*b)->Faces));
		}
	}

	if ((*b)->Name)
	{
		App->CL_Maths->Ram_Free((*b)->Name);
	}

	App->CL_Maths->Ram_Free(*b);

	*b = NULL;
}

// *************************************************************************
// *						BrushList_Destroy							   *
// *************************************************************************
void CL64_Brush::BrushList_Destroy(BrushList** ppList)
{
	BrushList* pList;

	pList = *ppList;
	BrushList_DeleteAll(pList);

	App->CL_Maths->Ram_Free(*ppList);
	*ppList = NULL;
}

// *************************************************************************
// *						BrushList_DeleteAll							   *
// *************************************************************************
void CL64_Brush::BrushList_DeleteAll(BrushList* pList)
{
	Brush* pBrush;
	BrushIterator bi;

	assert(pList != NULL);

	pBrush = BrushList_GetFirst(pList, &bi);
	while (pBrush != NULL)
	{
		BrushList_Remove(pList, pBrush);
		Brush_Destroy(&pBrush);
		pBrush = BrushList_GetNext(&bi);
	}
}

// *************************************************************************
// *						BrushList_GetFirst							   *
// *************************************************************************
Brush* CL64_Brush::BrushList_GetFirst(BrushList* pList,BrushIterator* bi)
{
	if (pList->First == NULL)
	{
		*bi = NULL;
	}
	else
	{
		*bi = pList->First->Next;
	}
	return pList->First;
}

// *************************************************************************
// *						BrushList_GetNext							   *
// *************************************************************************
Brush* CL64_Brush::BrushList_GetNext(BrushIterator* bi)
{
	if (*bi == NULL)
	{
		return NULL;
	}
	else
	{
		Brush* b;

		b = *bi;
		*bi = (*bi)->Next;

		return b;
	}
}

// *************************************************************************
// *						BrushList_GetLast							   *
// *************************************************************************
Brush* CL64_Brush::BrushList_GetLast(BrushList* pList,BrushIterator* bi)
{
	if (pList->Last == NULL)
	{
		*bi = NULL;
	}
	else
	{
		*bi = pList->Last->Prev;
	}
	return pList->Last;
}

// *************************************************************************
// *						BrushList_GetPrev							   *
// *************************************************************************
Brush* CL64_Brush::BrushList_GetPrev(BrushIterator* bi)
{

	if (*bi == NULL)
	{
		return NULL;
	}
	else
	{
		Brush* b;

		b = *bi;
		*bi = (*bi)->Prev;
		return b;
	}
}

// *************************************************************************
// *						BrushList_Remove							   *
// *************************************************************************
void CL64_Brush::BrushList_Remove(BrushList* pList,Brush* pBrush)
{
	Brush* pPrev;
	Brush* pNext;

	pPrev = pBrush->Prev;
	pNext = pBrush->Next;

	pBrush->Prev = NULL;
	pBrush->Next = NULL;

	if (pPrev != NULL)
	{
		pPrev->Next = pNext;
	}
	if (pNext != NULL)
	{
		pNext->Prev = pPrev;
	}

	if (pList->First == pBrush)
	{
		assert(pPrev == NULL);
		pList->First = pNext;
	}

	if (pList->Last == pBrush)
	{
		assert(pNext == NULL);
		pList->Last = pPrev;
	}
}

// *************************************************************************
// *						Brush_SetHollowCut							   *
// *************************************************************************
void CL64_Brush::Brush_SetHollowCut(Brush* b, const signed int bState)
{
	assert(b != NULL);
	assert(b->Type != BRUSH_MULTI);

	b->Flags = (bState) ? b->Flags | BRUSH_HOLLOWCUT : b->Flags & ~BRUSH_HOLLOWCUT;
	b->Flags = (bState) ? b->Flags | BRUSH_SUBTRACT : b->Flags & ~BRUSH_SUBTRACT;
}

// *************************************************************************
// *						BrushList_Append							   *
// *************************************************************************
void CL64_Brush::BrushList_Append(BrushList* pList,Brush* pBrush)
{
	if (pList->First == NULL)
	{
		pList->First = pBrush;
		pList->Last = pBrush;
		pBrush->Next = NULL;
		pBrush->Prev = NULL;
	}
	else
	{
		pBrush->Next = NULL;			// it's the end of the list
		pBrush->Prev = pList->Last;		// point back to previous end
		pList->Last->Next = pBrush;		// previous end points to me
		pList->Last = pBrush;			// and we're now the list end
	}
}

// *************************************************************************
// *						Brush_IsMulti								   *
// *************************************************************************
signed int CL64_Brush::Brush_IsMulti(const Brush* b)
{
	assert(b != NULL);

	return (b->Type == BRUSH_MULTI) ? true : false;
}

// *************************************************************************
// *						BrushList_EnumLeafBrushes					   *
// *************************************************************************
int	CL64_Brush::BrushList_EnumLeafBrushes(const BrushList* pList,void* pVoid,BrushList_CB	CallBack)
{
	signed int	bResult = true;	// TRUE means entire list was processed
	Brush* b;

	assert(pList != NULL);

	for (b = pList->First; b; b = b->Next)
	{
		assert(b->Type != BRUSH_CSG);

		if (b->Type == BRUSH_MULTI)
		{
			if (!BrushList_EnumLeafBrushes(b->BList, pVoid, CallBack))
			{
				break;
			}
		}
		else if ((bResult = CallBack(b, pVoid)) == false)
		{
			break;
		}
	}
	return bResult;
}

// *************************************************************************
// *						Brush_GetBrushList							   *
// *************************************************************************
const BrushList* CL64_Brush::Brush_GetBrushList(const Brush* b)
{
	if (b == 0)
	{
		App->Say_Win("Null Brush");
	}

	return	b->BList;
}

// *************************************************************************
// *						Brush_GetNumFaces							   *
// *************************************************************************
int	CL64_Brush::Brush_GetNumFaces(const Brush* b)
{
	assert(b != NULL);
	assert(b->Faces != NULL);

	return	App->CL_FaceList->FaceList_GetNumFaces(b->Faces);
}

// *************************************************************************
// *							Brush_GetFace							   *
// *************************************************************************
Face* CL64_Brush::Brush_GetFace(const Brush* b, int i)
{
	assert(b != NULL);
	assert(b->Faces != NULL);

	return	App->CL_FaceList->FaceList_GetFace(b->Faces, i);
}

// *************************************************************************
// *							Brush_GetFace							   *
// *************************************************************************
int CL64_Brush::Get_Brush_Count(void)
{
	int Count = 0;
	Brush* b;

	Level* pLevel = App->CL_Doc->pLevel;
	BrushList* pList = App->CL_Level->Level_GetBrushes(pLevel);

	b = pList->First;
	while (b != NULL)
	{
		Count++;
		b = b->Next;
	}

	return Count;
}

// *************************************************************************
// *						Brush_IsSubtract							   *
// *************************************************************************
signed int CL64_Brush::Brush_IsSubtract(const Brush* b)
{
	return	(b->Flags & BRUSH_SUBTRACT) ? true : false;
}

// *************************************************************************
// *							Brush_Clone								   *
// *************************************************************************
Brush* CL64_Brush::Brush_Clone(Brush const* from)
{
	Brush* to = NULL;
	FaceList* NewFaces;
	BrushList* MBList;

	assert(from != NULL);

	switch (from->Type)
	{
	case	BRUSH_MULTI:
		assert(from->Faces == NULL);
		assert(from->BList);

		MBList = BrushList_Clone(from->BList);
		if (!MBList)
		{
			break;
		}
		to = Brush_Create(from->Type, NULL, MBList);
		break;

	case	BRUSH_LEAF:
	case	BRUSH_CSG:
		assert(from->Faces != NULL);

		NewFaces = App->CL_FaceList->FaceList_Clone(from->Faces);
		if (NewFaces != NULL)
		{
			to = Brush_Create(from->Type, NewFaces, NULL);
		}
		if (to == NULL)
		{
			App->CL_Maths->Ram_Free(NewFaces);
		}
		break;

	default:
		assert(0);
		break;
	}

	if (to != NULL)
	{
		to->Flags = from->Flags;
		to->Type = from->Type;
		to->ModelId = from->ModelId;
		to->GroupId = from->GroupId;
		to->HullSize = from->HullSize;
		to->Color = from->Color;
		Brush_SetName(to, from->Name);
		to->BoundingBox = from->BoundingBox;
	}

	return	to;
}

// *************************************************************************
// *						BrushList_Clone								   *
// *************************************************************************
BrushList* CL64_Brush::BrushList_Clone(BrushList* inList)
{
	BrushList* outList;
	Brush* b, * b2;

	assert(inList != NULL);

	outList = BrushList_Create();

	for (b = inList->First; b; b = b->Next)
	{
		b2 = Brush_Clone(b);
		BrushList_Append(outList, b2);
	}
	return	outList;
}

// *************************************************************************
// *						Brush_SetName								   *
// *************************************************************************
void CL64_Brush::Brush_SetName(Brush* b, const char* newname)
{
	if (b->Name != NULL)
	{
		App->CL_Maths->Ram_Free(b->Name);
	}

	b->Name = (LPSTR)newname;
}

// *************************************************************************
// *					Brush_SetFaceListDirty							   *
// *************************************************************************
void CL64_Brush::Brush_SetFaceListDirty(Brush* b)
{
	App->CL_FaceList->FaceList_SetDirty(b->Faces);
}

// *************************************************************************
// *							Brush_Center							   *
// *************************************************************************
void CL64_Brush::Brush_Center(const Brush* b, Ogre::Vector3* center)
{
	App->CL_Box->Box3d_GetCenter(&b->BoundingBox, center);
}

// *************************************************************************
// *						Brush_SetGroupId							   *
// *************************************************************************
void Brush_SetGroupId(Brush* b, const int gid)
{
	if (b->Type == BRUSH_MULTI)
	{
		BrushList_SetInt(b->BList, gid, Brush_SetGroupId);
	}

	b->GroupId = gid;
}

// *************************************************************************
// *							Brush_EnumFaces							   *
// *************************************************************************
void CL64_Brush::Brush_EnumFaces(Brush* b, void* lParam, Brush_FaceCallback Callback)
{
	switch (b->Type)
	{
	case BRUSH_MULTI:
	{
		Brush* pBrush;

		pBrush = b->BList->First;
		while (pBrush != NULL)
		{
			Brush_EnumFaces(pBrush, lParam, Callback);
			pBrush = pBrush->Next;
		}
		break;
	}

	case BRUSH_LEAF:
	{
		int NumFaces, iFace;

		NumFaces = Brush_GetNumFaces(b);
		for (iFace = 0; iFace < NumFaces; ++iFace)
		{
			Face* pFace = Brush_GetFace(b, iFace);
			Callback(pFace, lParam);
		}
		Brush_UpdateChildFaces(b);
		break;
	}

	default:
		break;
	}
}

// *************************************************************************
// *						Brush_TestBoundsIntersect					   *
// *************************************************************************
signed int	CL64_Brush::Brush_TestBoundsIntersect(const Brush* b, const Box3d* pBox)
{
	return App->CL_Box->Box3d_Intersection(&b->BoundingBox, pBox, NULL);
}

static	Brush* bstack[8192];	//8192 levels of recursion
static	Brush** bsp;

// *************************************************************************
// *					Brush_UpdateChildFacesRecurse					   *
// *************************************************************************
static void	Brush_UpdateChildFacesRecurse(Brush* b, Brush* bp)
{
	Brush* cb;
	Face* f, * f2;
	const GPlane* p, * p2;
	int			i, j;
	signed int	Update;

	assert(b);

	if (b->Type == BRUSH_LEAF)
	{
		if (b->Flags & BRUSH_SUBTRACT)	//find cuts
		{
			if (!bp)
			{
				bp = b;
			}
			for (cb = bp->Prev;;)
			{
				if (!cb)
				{
					if (--bsp >= bstack)
					{
						cb = (*bsp)->Prev;
						continue;
					}
					else
					{
						bsp++;
						break;
					}
				}
				if (!(cb->Flags & BRUSH_SUBTRACT))
				{
					if (App->CL_Brush->Brush_TestBoundsIntersect(b, &cb->BoundingBox))
					{
						if (cb->BList)
						{
							*bsp++ = cb;
							cb = cb->BList->Last;
							continue;
						}

						Update = false;
						for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(b->Faces); i++)
						{
							f = App->CL_FaceList->FaceList_GetFace(b->Faces, i);
							p = App->CL_Face->Face_GetPlane(f);
							for (j = 0; j < App->CL_FaceList->FaceList_GetNumFaces(cb->Faces); j++)
							{
								Ogre::Vector3	v;
								f2 = App->CL_FaceList->FaceList_GetFace(cb->Faces, j);
								p2 = App->CL_Face->Face_GetPlane(f2);
								v = p->Normal;

								App->CL_Maths->Vector3_Inverse(&v);

								if (App->CL_Maths->Vector3_Compare(&v, &p2->Normal, 0.01f))
								{
									if (fabs(-p->Dist - p2->Dist) < 0.01f)
									{
										App->CL_Face->Face_CopyFaceInfo(f, f2);
										Update = true;
									}
								}
							}
						}
					}
				}
				cb = cb->Prev;
			}
		}
		else if (b->BList)
		{
			for (cb = b->BList->First; cb; cb = cb->Next)
			{
				for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(b->Faces); i++)
				{
					f = App->CL_FaceList->FaceList_GetFace(b->Faces, i);
					p = App->CL_Face->Face_GetPlane(f);
					for (j = 0; j < App->CL_FaceList->FaceList_GetNumFaces(cb->Faces); j++)
					{
						f2 = App->CL_FaceList->FaceList_GetFace(cb->Faces, j);
						p2 = App->CL_Face->Face_GetPlane(f2);

						if (App->CL_Maths->Vector3_Compare(&p->Normal, &p2->Normal, 0.01f))
						{
							if (fabs(p->Dist - p2->Dist) < 0.01f)
							{
								App->CL_Face->Face_CopyFaceInfo(f, f2);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (bp)
		{
			for (cb = b->BList->First; cb; cb = cb->Next)
			{
				Brush_UpdateChildFacesRecurse(cb, bp);
			}
		}
		else
		{
			if (b->Flags & BRUSH_SUBTRACT)
			{
				//only pass down parents that need to affect
				//the list above... like in the case of a multicut
				for (cb = b->BList->First; cb; cb = cb->Next)
				{
					Brush_UpdateChildFacesRecurse(cb, b);
				}
			}
			else
			{
				//cuts should be localized if the entire
				//parent brush isn't a cut
				for (cb = b->BList->First; cb; cb = cb->Next)
				{
					Brush_UpdateChildFacesRecurse(cb, NULL);
				}
			}
		}
	}
}

// *************************************************************************
// *						Brush_UpdateChildFaces						   *
// *************************************************************************
void CL64_Brush::Brush_UpdateChildFaces(Brush* b)
{
	bsp = bstack;

	Brush_UpdateChildFacesRecurse(b, NULL);
}

