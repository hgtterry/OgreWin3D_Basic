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
#include "Structures.cpp"

#define	VectorToSUB(a, b)			(*((((float *)(&a))) + (b)))
static const int		axidx[3][2] = { 2, 1, 0, 2, 0, 1 };
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
		strcpy(pBrush->Name,"NoName");
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
	T_Vec3 pnt;
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
// *							Get_Brush_Count							   *
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
// *							Brush_IsDetail							   *
// *************************************************************************
signed int CL64_Brush::Brush_IsDetail(const Brush* b)
{
	return	(b->Flags & BRUSH_DETAIL) ? GE_TRUE : GE_FALSE;
}

// *************************************************************************
// *						Brush_IsHollowCut							   *
// *************************************************************************
signed int CL64_Brush::Brush_IsHollowCut(const Brush* b)
{
	return	(b->Flags & BRUSH_HOLLOWCUT) ? GE_TRUE : GE_FALSE;
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

	strcpy(b->Name,newname);
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
void CL64_Brush::Brush_Center(const Brush* b, T_Vec3* center)
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
								T_Vec3	v;
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

// *************************************************************************
// *						Brush_IsHollow								   *
// *************************************************************************
signed int CL64_Brush::Brush_IsHollow(const Brush* b)
{
	return	(b->Flags & BRUSH_HOLLOW) ? true : false;
}

// *************************************************************************
// *							Brush_GetName							   *
// *************************************************************************
const char* CL64_Brush::Brush_GetName(const Brush* b)
{
	if (b == 0)
	{
		App->Say("Null Brush",(LPSTR)"Brush_GetName");
		assert(b != NULL);
	}

	return	b->Name;
}

// *************************************************************************
// *					BrushList_GetUsedTextures						   *
// *************************************************************************
signed int CL64_Brush::BrushList_GetUsedTextures(BrushList* BList, signed int* UsedTex, CL64_WadFile* WadFile)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		if (!Brush_GetUsedTextures(pBrush, UsedTex, WadFile)) return GE_FALSE;

		pBrush = BrushList_GetNext(&bi);
	}

	return GE_TRUE;
}

// *************************************************************************
// *						Brush_GetUsedTextures						   *
// *************************************************************************
signed int CL64_Brush::Brush_GetUsedTextures(const Brush* b, signed int* UsedTex, CL64_WadFile* WadFile)
{
	assert(UsedTex);
	assert(b);

	switch (b->Type)
	{
	case	BRUSH_MULTI:
	{
		return BrushList_GetUsedTextures(b->BList, UsedTex, WadFile);
	}

	case	BRUSH_LEAF:
		if (b->BList)
		{
			return BrushList_GetUsedTextures(b->BList, UsedTex, WadFile);
		}
		else
		{
			if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
			{
				return App->CL_FaceList->FaceList_GetUsedTextures(b->Faces, UsedTex, WadFile);
			}
		}
		break;

	case	BRUSH_CSG:
		if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
			return App->CL_FaceList->FaceList_GetUsedTextures(b->Faces, UsedTex, WadFile);
		break;
	default:
		assert(0);		// invalid brush type
		break;
	}
	return GE_TRUE;
}

// *************************************************************************
// *							Brush_GetParent							   *
// *************************************************************************
signed int CL64_Brush::Brush_GetParent(const BrushList* pList,const Brush* b,Brush** bParent)
{
	Brush* b2;

	assert(b);
	assert(pList);
	assert(bParent);

	for (b2 = pList->First; b2; b2 = b2->Next)

	{
		if (b2 == b)
		{
			*bParent = (Brush*)b;	//const override!
			return		GE_TRUE;
		}

		if (b2->Type == BRUSH_LEAF)
		{
			if (b2->BList)
			{
				if (Brush_GetParent(b2->BList, b, bParent))
				{
					*bParent = b2;
					return		GE_TRUE;
				}
			}
		}
		else if (b2->Type == BRUSH_MULTI)
		{
			if (Brush_GetParent(b2->BList, b, bParent))
			{
				*bParent = b2;
				return		GE_TRUE;
			}
		}
	}

	return	GE_FALSE;
}

// *************************************************************************
// *							Get_Brush_ByIndex						   *
// *************************************************************************
Brush* CL64_Brush::Get_Brush_ByIndex(int Index)
{
	Level* pLevel = App->CL_Doc->pLevel;
	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

	int Count = 0;
	Brush* b;
	b = pList->First;
	while (b != NULL)
	{
		if (Count == Index)
		{
			return b;
		}

		Count++;
		b = b->Next;
	}

	return NULL;
}

// *************************************************************************
// *							BrushList_Count							   *
// *************************************************************************
int CL64_Brush::BrushList_Count(BrushList const* pList,int CountFlags)
{
	int Count;
	Brush* b;
	//	geBoolean bResult = GE_TRUE;

	assert(pList != NULL);

	Count = 0;

	b = pList->First;
	while (b != NULL)
	{
		geBoolean CountIt;
		switch (b->Type)
		{
		case BRUSH_MULTI:
			CountIt = (CountFlags & BRUSH_COUNT_MULTI);
			break;

		case BRUSH_LEAF:
			CountIt = (CountFlags & BRUSH_COUNT_LEAF);
			break;

		case BRUSH_CSG:
			CountIt = (CountFlags & BRUSH_COUNT_CSG);
			break;

		default:
			assert(0);
			CountIt = GE_FALSE;
			break;
		}
		if (CountIt)
		{
			++Count;
		}

		if ((b->Type == BRUSH_MULTI) && (!(CountFlags & BRUSH_COUNT_NORECURSE)))
		{
			Count += BrushList_Count(b->BList, CountFlags);
		}
		b = b->Next;
	}

	return Count;
}

// *************************************************************************
// *						BrushList_ClearAllCSG						   *
// *************************************************************************
void CL64_Brush::BrushList_ClearAllCSG(BrushList* pList)
{
	Brush* b;

	for (b = pList->First; b != NULL; b = b->Next)
	{
		if (b->Type == BRUSH_MULTI)
		{
			BrushList_ClearAllCSG(b->BList);
		}
		else if (b->BList != NULL)
		{
			BrushList_Destroy(&b->BList);
		}
	}
}

// *************************************************************************
// *						BrushList_Prepend							   *
// *************************************************************************
void CL64_Brush::BrushList_Prepend(BrushList* pList,Brush* pBrush)
{
	assert(pList != NULL);
	assert(pBrush != NULL);

	if (pList->First == NULL)
	{
		// it's the first brush in the list
		assert(pList->Last == NULL);
		pList->First = pBrush;
		pList->Last = pBrush;
		pBrush->Next = NULL;
		pBrush->Prev = NULL;
	}
	else
	{
		// put it at the head of the list
		assert(pList->Last != NULL);
		assert(pList->First->Prev == NULL);

		pBrush->Prev = NULL;			// nothing before me
		pBrush->Next = pList->First;	// point to previous head
		pList->First->Prev = pBrush;	// previous head points to me
		pList->First = pBrush;			// and now we're the head of the list
	}
}

static Brush* Brush_CreateFromParent(const Brush* ParentBrush, const FaceList* fl)
{
	Brush* pBrush;

	assert(ParentBrush);
	assert(fl != NULL);
	assert(ParentBrush->Type != BRUSH_MULTI);

	pBrush = (Brush*)App->CL_Maths->Ram_Allocate(sizeof(Brush));
	if (pBrush != NULL)
	{
		pBrush->Prev = NULL;
		pBrush->Next = NULL;
		pBrush->Faces = (FaceList*)fl;
		pBrush->BList = NULL;
		pBrush->Flags = ParentBrush->Flags;
		pBrush->ModelId = ParentBrush->ModelId;
		pBrush->GroupId = ParentBrush->GroupId;
		pBrush->HullSize = ParentBrush->HullSize;
		pBrush->Color = ParentBrush->Color;
		strcpy(pBrush->Name,ParentBrush->Name);
		pBrush->Type = BRUSH_CSG;
		App->CL_FaceList->FaceList_GetBounds(fl, &pBrush->BoundingBox);
	}
	return pBrush;
}

static int	Brush_MostlyOnSide(const Brush* b, const GPlane* p)
{
	int		i, side;
	geFloat	max;

	max = 0;
	side = SIDE_FRONT;
	for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(b->Faces); i++)
	{
		App->CL_Face->Face_MostlyOnSide(App->CL_FaceList->FaceList_GetFace(b->Faces, i), p, &max, &side);
	}
	return	side;
}

// *************************************************************************
// *						Brush_SplitByFace							   *
// *************************************************************************
void CL64_Brush::Brush_SplitByFace(Brush* ogb,Face* sf,Brush** fb,Brush** bb)
{
	const GPlane* p;
	int			i;
	Guint8		cnt[3], fcnt[4];
	FaceList* fl, * bl;
	const Face* f;
	Face* cpf, * ff, * bf, * midf;
	geBoolean	WasSplit = GE_FALSE;

	assert(ogb);
	assert(sf);
	assert(fb);
	assert(bb);
	assert(*fb == NULL);
	assert(*bb == NULL);

	p = App->CL_Face->Face_GetPlane(sf);

	fcnt[0] = fcnt[1] = fcnt[2] = fcnt[3] = 0;

	for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(ogb->Faces); i++)
	{
		f = App->CL_FaceList->FaceList_GetFace(ogb->Faces, i);
		App->CL_Face->Face_GetSplitInfo(f, p, dists, sides, cnt);
		if (!cnt[SIDE_FRONT] && !cnt[SIDE_BACK])	//coplanar
		{
			fsides[i] = SIDE_ON;
		}
		else if (!cnt[SIDE_FRONT])	//back
		{
			fsides[i] = SIDE_BACK;
		}
		else if (!cnt[SIDE_BACK])	//front
		{
			fsides[i] = SIDE_FRONT;
		}
		else	//split
		{
			fsides[i] = SIDE_SPLIT;
		}
		fcnt[fsides[i]]++;
	}
	fsides[i] = fsides[0];

	if (fcnt[SIDE_SPLIT])	//at least one face split
	{
		//clip the split face
		midf = App->CL_Face->Face_Clone(sf);
		App->CL_FaceList->FaceList_ClipFaceToList(ogb->Faces, &midf);
		if (!midf)
		{
			if (Brush_MostlyOnSide(ogb, App->CL_Face->Face_GetPlane(sf)) == SIDE_FRONT)
			{
				*fb = Brush_Clone(ogb);
			}
			else
			{
				*bb = Brush_Clone(ogb);
			}
			return;
		}
		//alloc face lists for front and back
		fl = App->CL_FaceList->FaceList_Create(fcnt[SIDE_FRONT] + fcnt[SIDE_SPLIT] + 1);
		bl = App->CL_FaceList->FaceList_Create(fcnt[SIDE_BACK] + fcnt[SIDE_SPLIT] + 1);

		WasSplit = GE_FALSE;
		for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(ogb->Faces); i++)
		{
			f = App->CL_FaceList->FaceList_GetFace(ogb->Faces, i);

			switch (fsides[i])
			{
			case	SIDE_FRONT:
				cpf = App->CL_Face->Face_Clone(f);
				if (cpf)
				{
					App->CL_FaceList->FaceList_AddFace(fl, cpf);
				}
				break;
			case	SIDE_BACK:
				cpf = App->CL_Face->Face_Clone(f);
				if (cpf)
				{
					App->CL_FaceList->FaceList_AddFace(bl, cpf);
				}
				break;
			case	SIDE_SPLIT:	//this info should be reused from above!!!
				ff = bf = NULL;
				App->CL_Face->Face_GetSplitInfo(f, p, dists, sides, cnt);
				if (!cnt[SIDE_FRONT] && !cnt[SIDE_BACK])	//coplanar
				{
					assert(0);	//shouldn't happen
				}
				else if (!cnt[SIDE_FRONT])	//back
				{
					assert(0);	//shouldn't happen
					//					bf	=Face_Clone(f);
				}
				else if (!cnt[SIDE_BACK])	//front
				{
					assert(0);	//shouldn't happen
					//					ff	=Face_Clone(f);
				}
				else	//split
				{
					WasSplit = GE_TRUE;
					App->CL_Face->Face_Split(f, p, &ff, &bf, dists, sides);
				}

				if (ff)
				{
					App->CL_FaceList->FaceList_AddFace(fl, ff);
				}
				if (bf)
				{
					App->CL_FaceList->FaceList_AddFace(bl, bf);
				}
			}
		}
		if (WasSplit)
		{
			//add and clip the split face
//			FaceList_ClipFaceToList(bl, &cpf);
			App->CL_FaceList->FaceList_AddFace(bl, midf);

			//flip for front side brush
			cpf = App->CL_Face->Face_CloneReverse(midf);
			App->CL_FaceList->FaceList_AddFace(fl, cpf);
		}
		*fb = Brush_CreateFromParent(ogb, fl);
		*bb = Brush_CreateFromParent(ogb, bl);

		Brush_SealFaces(fb);
		Brush_SealFaces(bb);
	}
	else if (fcnt[SIDE_FRONT] && fcnt[SIDE_BACK])
	{
		if (fcnt[SIDE_FRONT] > fcnt[SIDE_BACK])	//mostly in front
		{
			*fb = Brush_Clone(ogb);
		}
		else									//mostly behind
		{
			*bb = Brush_Clone(ogb);
		}
	}
	else if (!fcnt[SIDE_FRONT])		//all faces behind
	{
		*bb = Brush_Clone(ogb);
	}
	else if (!fcnt[SIDE_BACK])		//all faces front
	{
		*fb = Brush_Clone(ogb);
	}
}

//cuts b2 by b (b should be a cut brush)
static void	Brush_CutBrush(Brush* b, Brush* b2)
{
	Brush* cb, * fb, * bb = NULL, * cb2;
	Face* f, * sf;
	const GPlane* p;
	int			i;

	assert(b);
	assert(b2);
	assert(b2->Type != BRUSH_CSG);
	assert(b->Flags & BRUSH_SUBTRACT);

	if (b2->Type == BRUSH_LEAF)
	{
		if (b2->BList)
		{
			for (cb = b2->BList->First; cb;)
			{
				if (!App->CL_Brush->Brush_TestBoundsIntersect(b, &cb->BoundingBox))
				{
					cb = cb->Next;
					continue;
				}
				cb2 = cb->Next;
				for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(b->Faces); i++)
				{
					f = App->CL_FaceList->FaceList_GetFace(b->Faces, i);
					p = App->CL_Face->Face_GetPlane(f);

					//create a new face from the split plane
					sf = App->CL_Face->Face_CreateFromPlane(p, BOGUS_RANGE, 0);
					App->CL_Face->Face_CopyFaceInfo(f, sf);

					fb = bb = NULL;

					//split b by sf, adding the front brush to the brushlist
					App->CL_Brush->Brush_SplitByFace(cb, sf, &fb, &bb);
					if (fb)
					{	//clear hollow for csg
						fb->Flags &= ~(BRUSH_HOLLOW | BRUSH_HOLLOWCUT);
						App->CL_Brush->BrushList_Prepend(b2->BList, fb);
					}
					App->CL_Face->Face_Destroy(&sf);
					if (!i)
					{
						App->CL_Brush->BrushList_Remove(b2->BList, cb);
					}
					App->CL_Brush->Brush_Destroy(&cb);

					//make the back side brush current
					cb = bb;
					if (!cb)
						break;
				}
				if (bb)
				{
					App->CL_Brush->Brush_Destroy(&bb);
				}
				cb = cb2;
			}
		}
		else
		{
			b2->BList = App->CL_Brush->BrushList_Create();
			cb = b2;
			for (i = 0; i < App->CL_FaceList->FaceList_GetNumFaces(b->Faces); i++)
			{
				f = App->CL_FaceList->FaceList_GetFace(b->Faces, i);
				p = App->CL_Face->Face_GetPlane(f);

				//create a new face from the split plane
				sf = App->CL_Face->Face_CreateFromPlane(p, BOGUS_RANGE, 0);
				App->CL_Face->Face_CopyFaceInfo(f, sf);

				fb = bb = NULL;

				//split b by sf, adding the front brush to the brushlist
				App->CL_Brush->Brush_SplitByFace(cb, sf, &fb, &bb);
				if (fb)
				{	//clear hollow for csg
					fb->Flags &= ~(BRUSH_HOLLOW | BRUSH_HOLLOWCUT);
					App->CL_Brush->BrushList_Append(b2->BList, fb);
				}
				App->CL_Face->Face_Destroy(&sf);
				if (i)
				{
					App->CL_Brush->Brush_Destroy(&cb);
				}

				//make the back side brush current
				cb = bb;
				if (!cb)
					break;
			}
			if (bb)
			{
				App->CL_Brush->Brush_Destroy(&bb);
			}
		}
	}
	else
	{
		for (cb = b2->BList->First; cb; cb = cb->Next)
		{
			if (App->CL_Brush->Brush_TestBoundsIntersect(b, &cb->BoundingBox))
			{
				if (!(cb->Flags & BRUSH_SUBTRACT))
				{
					Brush_CutBrush(b, cb);
				}
			}
		}
	}
}

static void	BrushList_DoHollowCuts(BrushList* pList, int mid, Brush_CSGCallback Callback, void* lParam)
{
	Brush* b, * b2, * cb;

	assert(pList != NULL);

	//iterate cuts in reverse list order cutting solids
	for (b = pList->Last; b; b = b->Prev)
	{
		if ((b->ModelId != mid) || (!Callback(b, lParam)))
		{
			continue;
		}

		//recurse to cut with private multicuts
		if (b->BList && b->Type == BRUSH_MULTI)
		{
			BrushList_DoHollowCuts(b->BList, mid, Callback, lParam);
		}

		if (b->Flags & BRUSH_HOLLOWCUT)
		{
			if (b->BList)
			{
				for (cb = b->BList->First;;)	//leaf hollows or multis
				{
					if (!cb)
					{
						if (--bsp >= bstack)
						{
							cb = (*bsp)->Next;
							continue;
						}
						else
						{
							bsp++;
							break;
						}
					}
					//true subtract flags are always passed on now
					assert(cb->Flags & BRUSH_SUBTRACT);

					if (cb->BList)
					{
						*bsp++ = cb;
						cb = cb->BList->First;
						continue;
					}

					for (b2 = b->Prev; b2; b2 = b2->Prev)
					{
						if (b2->ModelId != mid)
						{
							continue;
						}

						if (App->CL_Brush->Brush_TestBoundsIntersect(cb, &b2->BoundingBox))
						{
							assert(b2->Type != BRUSH_CSG);
							Brush_CutBrush(cb, b2);
						}
					}
					cb = cb->Next;
				}
			}
			else
			{
				for (b2 = b->Prev; b2; b2 = b2->Prev)
				{
					if (b2->ModelId != mid)
					{
						continue;
					}

					if (App->CL_Brush->Brush_TestBoundsIntersect(b, &b2->BoundingBox))
					{
						assert(b2->Type != BRUSH_CSG);

						Brush_CutBrush(b, b2);
					}
				}
			}
		}
	}
}

static void	Brush_CopyFaceInfo(Brush const* src, Brush* dst)
{
	App->CL_FaceList->FaceList_CopyFaceInfo(src->Faces, dst->Faces);
}

static void	BrushList_DoCuts(BrushList* pList, int mid, Brush_CSGCallback Callback, void* lParam)
{
	Brush* b, * b2, * cb;

	assert(pList != NULL);

	//iterate cuts in reverse list order cutting solids
	for (b = pList->Last; b; b = b->Prev)
	{
		if ((b->ModelId != mid) || !Callback(b, lParam))
		{
			continue;
		}

		if (b->Flags & BRUSH_SUBTRACT)
		{
			if (b->BList)
			{
				for (cb = b->BList->First;;)	//leaf hollows or multis
				{
					if (!cb)
					{
						if (--bsp >= bstack)
						{
							cb = (*bsp)->Next;
							continue;
						}
						else
						{
							bsp++;
							break;
						}
					}
					//true subtract flags are always passed on now
					assert(cb->Flags & BRUSH_SUBTRACT);

					if (cb->BList)
					{
						*bsp++ = cb;
						cb = cb->BList->First;
						continue;
					}

					for (b2 = b->Prev; b2; b2 = b2->Prev)
					{
						if ((b2->Flags & BRUSH_SUBTRACT) || (b2->ModelId != mid))
						{
							continue;
						}

						if (App->CL_Brush->Brush_TestBoundsIntersect(cb, &b2->BoundingBox))
						{
							assert(b2->Type != BRUSH_CSG);
							if (!(cb->Flags & BRUSH_HOLLOWCUT))
							{
								Brush_CutBrush(cb, b2);
							}
						}
					}
					cb = cb->Next;
				}
			}
			else
			{
				for (b2 = b->Prev; b2; b2 = b2->Prev)
				{
					if ((b2->Flags & BRUSH_SUBTRACT) || (b2->ModelId != mid))
					{
						continue;
					}

					if (App->CL_Brush->Brush_TestBoundsIntersect(b, &b2->BoundingBox))
					{
						assert(b2->Type != BRUSH_CSG);

						Brush_CutBrush(b, b2);
					}
				}
			}
		}
	}
}

// *************************************************************************
// *							BrushList_DoCSG							   *
// *************************************************************************
void CL64_Brush::BrushList_DoCSG(BrushList* inList, int mid, Brush_CSGCallback Callback, void* lParam)
{
	assert(inList != NULL);

	bsp = bstack;

	BrushList_ClearCSGAndHollows(inList, mid);
	BrushList_RebuildHollowFaces(inList, mid, Callback, lParam);
	BrushList_DoHollowCuts(inList, mid, Callback, lParam);

	bsp = bstack;	//reset this just incase...

	BrushList_DoCuts(inList, mid, Callback, lParam);

}

// *************************************************************************
// *					BrushList_ClearCSGAndHollows					   *
// *************************************************************************
void CL64_Brush::BrushList_ClearCSGAndHollows(BrushList* inList, int mid)
{
	Brush* b;

	assert(inList != NULL);

	for (b = inList->First; b; b = b->Next)
	{
		if (b->ModelId == mid)
		{
			if (b->Type == BRUSH_MULTI)
			{
				BrushList_ClearCSGAndHollows(b->BList, mid);
			}
			else if (b->BList != NULL)
			{
				BrushList_Destroy(&b->BList);
			}
		}
	}
}

// *************************************************************************
// *					BrushList_RebuildHollowFaces					   *
// *************************************************************************
void CL64_Brush::BrushList_RebuildHollowFaces(BrushList* inList, int mid, Brush_CSGCallback Callback, void* lParam)
{
	Brush* b;

	assert(inList != NULL);

	for (b = inList->First; b; b = b->Next)
	{
		if (!Callback(b, lParam))
		{
			continue;
		}
		if (b->Type == BRUSH_MULTI)	//recurse multibrushes
		{
			assert(b->BList);

			BrushList_RebuildHollowFaces(b->BList, mid, Callback, lParam);
			continue;
		}
		else if (!(b->Flags & BRUSH_HOLLOW))
		{
			Brush* bh;

			if (mid == b->ModelId && b->BList)
			{
				BrushList_Destroy(&b->BList);
			}
			if (b->Flags & BRUSH_HOLLOWCUT)
			{
				//hollows are contained in a multi with the brush
				//that generates them
				bh = Brush_CreateHollowFromBrush(inList->First);
				if (bh)
				{
					BrushList_Append(inList, bh);
					bh->Flags = b->Flags | BRUSH_SUBTRACT; //make sure this is set
					bh->Type = b->Type;
					bh->ModelId = b->ModelId;
					bh->GroupId = b->GroupId;
					bh->HullSize = b->HullSize;
					bh->Color = b->Color;
					/*if (bh->Name)
					{
						geRam_Free(bh->Name);
					}*/
					strcpy(bh->Name,b->Name);
					Brush_CopyFaceInfo(b, bh);
					BrushList_Remove(inList, b);
					Brush_Destroy(&b);
					b = bh;
				}
				else	//destroy old core, probably was clipped away
				{
					bh = b->Prev;
					BrushList_Remove(inList, b);
					Brush_Destroy(&b);
					b = bh;
				}
			}
			continue;
		}
		else if (mid != b->ModelId)
		{
			continue;
		}
		assert(!b->BList);	//these should be nuked
		assert(b->Type == BRUSH_LEAF);

		if (b->Next)
		{
			if (b->Next->Flags & BRUSH_HOLLOWCUT)
			{
				continue;
			}
		}

		//generate from scratch if theres nothing
		{
			Brush* bh;

			bh = Brush_CreateHollowFromBrush(b);
			if (bh)
			{
				Brush_SetHollowCut(bh, GE_TRUE);
				BrushList_Append(inList, bh);
				bh->Flags = b->Flags & (~BRUSH_HOLLOW);	//clear hollow
				bh->Type = b->Type;
				bh->ModelId = b->ModelId;
				bh->GroupId = b->GroupId;
				bh->HullSize = b->HullSize;
				bh->Color = b->Color;
				Brush_SetHollowCut(bh, GE_TRUE);
				/*if (bh->Name)
				{
					geRam_Free(bh->Name);
				}*/
				strcpy(bh->Name,b->Name);
			}
		}
	}
}

// *************************************************************************
// *						Brush_IsVisible								   *
// *************************************************************************
signed int CL64_Brush::Brush_IsVisible(const Brush* b)
{
	return	(b->Flags & BRUSH_HIDDEN) ? GE_FALSE : GE_TRUE;
}

// *************************************************************************
// *							Brush_Move								   *
// *************************************************************************
void CL64_Brush::Brush_Move(Brush* b, const T_Vec3* trans)
{
	assert(b && trans);

	if (b->Type == BRUSH_MULTI)
	{
		BrushList_Move(b->BList, trans);
	}
	else
	{
		App->CL_FaceList->FaceList_Move(b->Faces, trans);
	}

	Brush_Bound(b);
}

// *************************************************************************
// *							BrushList_Move							   *
// *************************************************************************
void CL64_Brush::BrushList_Move(BrushList* pList, const T_Vec3* trans)
{
	Brush* b;

	assert(pList);
	assert(trans);

	for (b = pList->First; b; b = b->Next)
	{
		Brush_Move(b, trans); // Recursive
	}
}

// *************************************************************************
// *					Brush_GetBoundingBox							   *
// *************************************************************************
const Box3d* CL64_Brush::Brush_GetBoundingBox(const Brush* b)
{
	assert(b != NULL);

	return &b->BoundingBox;
}

// *************************************************************************
// *							Brush_Resize							   *
// *************************************************************************
void CL64_Brush::Brush_Resize(Brush* b, float dx, float dy, int sides, int inidx, T_Vec3* fnscale, int* ScaleNum)
{
	//MRB BEGIN
	Brush* pClone;
	//MRB END

	int		i;
	T_Vec3 FixOrg, BrushOrg, ScaleVec;

	assert(b);
	assert(fnscale);
	assert(ScaleNum);

	App->CL_Maths->Vector3_Add(&b->BoundingBox.Min, &b->BoundingBox.Max, &BrushOrg);
	App->CL_Maths->Vector3_Scale(&BrushOrg, 0.5f, &BrushOrg);

	//find the corner of the bounds to keep fixed
	VectorToSUB(FixOrg, inidx) = 0.0f;
	if ((sides & 3) == 0)	//center x
	{
		dx = -dx;
		VectorToSUB(FixOrg, axidx[inidx][0]) = VectorToSUB(BrushOrg, axidx[inidx][0]);
	}
	else if ((sides & 3) == 2)	//less x
	{
		VectorToSUB(FixOrg, axidx[inidx][0]) = VectorToSUB(b->BoundingBox.Min, axidx[inidx][0]);
	}
	else if ((sides & 3) == 1)	//greater x
	{
		dx = -dx;
		VectorToSUB(FixOrg, axidx[inidx][0]) = VectorToSUB(b->BoundingBox.Max, axidx[inidx][0]);
	}

	if ((sides & 0x0c) == 0)	//center y
	{
		VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(BrushOrg, axidx[inidx][1]);
	}
	else if ((sides & 0x0c) == 4)	//less y
	{
		dy = -dy;
		if (inidx != 1)
			VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(b->BoundingBox.Min, axidx[inidx][1]);
		else
			VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(b->BoundingBox.Max, axidx[inidx][1]);
	}
	else if ((sides & 0x0c) == 8)	//greater y
	{
		if (inidx != 1)
			VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(b->BoundingBox.Max, axidx[inidx][1]);
		else
			VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(b->BoundingBox.Min, axidx[inidx][1]);
	}

	if ((sides & 3) == 0)	//center x
		dx = 0;
	if ((sides & 0x0c) == 0)	//center x
		dy = 0;

	//translate to fixed origin
	App->CL_Maths->Vector3_Inverse(&FixOrg);
	Brush_Move(b, &FixOrg);

	dx *= 0.005f;
	dy *= 0.005f;

	dx = 1 - dx;
	dy = 1 - dy;

	VectorToSUB(ScaleVec, inidx) = 1.0f;
	VectorToSUB(ScaleVec, axidx[inidx][0]) = dx;
	VectorToSUB(ScaleVec, axidx[inidx][1]) = dy;

	for (i = 0; i < 3; i++)
		VectorToSUB(*fnscale, i) *= VectorToSUB(ScaleVec, i);

	(*ScaleNum)++;

	pClone = Brush_Clone(b);
	if (Brush_Scale3d(pClone, &ScaleVec))
		Brush_Scale3d(b, &ScaleVec);
	Brush_Destroy(&pClone);

		//translate back
	App->CL_Maths->Vector3_Inverse(&FixOrg);
	Brush_Move(b, &FixOrg);

	Brush_Bound(b);
}

// *************************************************************************
// *							Brush_Scale3d							   *
// *************************************************************************
signed int CL64_Brush::Brush_Scale3d(Brush* b, const T_Vec3* mag)
{
	signed int Success;
	
	if (b->Type == BRUSH_MULTI)
	{
		//MRB BEGIN
		Success =
			//MRB END
			BrushList_Scale3d(b->BList, mag);
	}
	else
	{
		Success =App->CL_FaceList->FaceList_Scale(b->Faces, mag);
	}

	Brush_Bound(b);

	return Success;
}

// *************************************************************************
// *							BrushList_Scale3d						   *
// *************************************************************************
signed int CL64_Brush::BrushList_Scale3d(BrushList* pList, const T_Vec3* trans)
{
	signed int Success = 1;
	Brush* b;

	assert(pList);
	assert(trans);

	for (b = pList->First; b; b = b->Next)
	{
		Success =( Success && Brush_Scale3d(b, trans));
	}
	
	return Success;
	
}

// *************************************************************************
// *							Brush_GetModelId						   *
// *************************************************************************
int	CL64_Brush::Brush_GetModelId(const Brush* b)
{
	return	b->ModelId;
}

// *************************************************************************
// *							Brush_ResizeFinal						   *
// *************************************************************************
void CL64_Brush::Brush_ResizeFinal(Brush* b, int sides, int inidx, T_Vec3* fnscale)
{
	Brush* pClone;
	
	T_Vec3 FixOrg, BrushOrg;

	App->CL_Maths->Vector3_Add(&b->BoundingBox.Min, &b->BoundingBox.Max, &BrushOrg);
	App->CL_Maths->Vector3_Scale(&BrushOrg, 0.5f, &BrushOrg);

	//find the corner of the bounds to keep fixed
	VectorToSUB(FixOrg, inidx) = 0.0f;
	if ((sides & 3) == 0)	//center x
	{
		VectorToSUB(FixOrg, axidx[inidx][0]) = VectorToSUB(BrushOrg, axidx[inidx][0]);
	}
	else if ((sides & 3) == 2)	//less x
	{
		VectorToSUB(FixOrg, axidx[inidx][0]) = VectorToSUB(b->BoundingBox.Min, axidx[inidx][0]);
	}
	else if ((sides & 3) == 1)	//greater x
	{
		VectorToSUB(FixOrg, axidx[inidx][0]) = VectorToSUB(b->BoundingBox.Max, axidx[inidx][0]);
	}

	if ((sides & 0x0c) == 0)	//center y
	{
		VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(BrushOrg, axidx[inidx][1]);
	}
	else if ((sides & 0x0c) == 4)	//less y
	{
		if (inidx != 1)
		{
			VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(b->BoundingBox.Min, axidx[inidx][1]);
		}
		else
		{
			VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(b->BoundingBox.Max, axidx[inidx][1]);
		}
	}
	else if ((sides & 0x0c) == 8)	//greater y
	{
		if (inidx != 1)
		{
			VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(b->BoundingBox.Max, axidx[inidx][1]);
		}
		else
		{
			VectorToSUB(FixOrg, axidx[inidx][1]) = VectorToSUB(b->BoundingBox.Min, axidx[inidx][1]);
		}
	}

	if ((sides & 3) == 0)	//center x
	{
		VectorToSUB(*fnscale, axidx[inidx][0]) = 1.0f;
	}

	if ((sides & 0x0c) == 0)	//center y
	{
		VectorToSUB(*fnscale, axidx[inidx][1]) = 1.0f;
	}

	//translate to fixed origin
	App->CL_Maths->Vector3_Inverse(&FixOrg);
	Brush_Move(b, &FixOrg);

	VectorToSUB(*fnscale, inidx) = 1.0f;

	pClone = Brush_Clone(b);
	if (Brush_Scale3d(pClone, fnscale))
		Brush_Scale3d(b, fnscale); // hgtterry Debug
	Brush_Destroy(&pClone);
	

	//translate back
	App->CL_Maths->Vector3_Inverse(&FixOrg);
	Brush_Move(b, &FixOrg);

	Brush_Bound(b);
}

// *************************************************************************
// *							Get_By_Index							   *
// *************************************************************************
Brush* CL64_Brush::Get_By_Index(int Index)
{
	int Count = 0;
	int Result = 1;
	Brush* b;

	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

	b = pList->First;
	while (b != NULL)
	{
		if (Count == Index)
		{
			return b;
		}

		Count++;

		b = b->Next;
	}

	return NULL;
}

// *************************************************************************
// *							BrushList_Enum							   *
// *************************************************************************
signed int CL64_Brush::BrushList_Enum(BrushList const* pList,void* lParam,BrushList_CB	CallBack)
{
	geBoolean bResult = true;	// TRUE means entire list was processed
	Brush* b;

	b = pList->First;
	while (b != NULL)
	{
		if ((bResult = CallBack(b, lParam)) == false)
			break;
		b = b->Next;
	}
	return bResult;
}

// *************************************************************************
// *						Brush_SelectFirstFace						   *
// *************************************************************************
Face* CL64_Brush::Brush_SelectFirstFace(Brush* b)
{
	Face* pFace;

	assert(b);

	if (Brush_IsMulti(b))
	{
		return Brush_SelectFirstFace(b->BList->First); // Recursive
	}
	else
	{
		pFace = App->CL_FaceList->FaceList_GetFace(b->Faces, 0);

		App->CL_Face->Face_SetSelected(pFace, GE_TRUE);
		return pFace;
	}
}

// *************************************************************************
// *						Brush_SelectLastFace						   *
// *************************************************************************
Face* CL64_Brush::Brush_SelectLastFace(Brush* b)
{
	assert(b);

	if (Brush_IsMulti(b))
	{
		return Brush_SelectLastFace(b->BList->Last);
	}
	else
	{
		Face* pFace;

		pFace = App->CL_FaceList->FaceList_GetFace(b->Faces, 0);
		App->CL_Face->Face_SetSelected(pFace, GE_TRUE);
		return pFace;
	}
}

// *************************************************************************
// *				BrushList_FindTopLevelFaceParent					   *
// *************************************************************************
Brush* CL64_Brush::BrushList_FindTopLevelFaceParent(const BrushList* pList, const Face* pFace)
{
	Brush* bFound;

	bFound = BrushList_FindFaceParent(pList, pFace);
	if (bFound != NULL)
	{
		bFound = Brush_GetTopLevelParent(pList, bFound);
	}
	return bFound;
}

// *************************************************************************
// *					BrushList_FindFaceParent						   *
// *************************************************************************
Brush* CL64_Brush::BrushList_FindFaceParent(const BrushList* pList, const Face* pFace)
{
	Brush* pBrush;

	for (pBrush = pList->First; pBrush != NULL; pBrush = pBrush->Next)
	{
		switch (pBrush->Type)

		{
		case BRUSH_MULTI:
		{
			Brush* pFound;

			pFound = BrushList_FindFaceParent(pBrush->BList, pFace);
			if (pFound != NULL)
			{
				return pFound;
			}
			break;
		}
		case BRUSH_LEAF:
		case BRUSH_CSG:
		{
			int i;
			for (i = 0; i < Brush_GetNumFaces(pBrush); i++)


			{
				Face* pCheckFace;

				pCheckFace = Brush_GetFace(pBrush, i);
				if (pFace == pCheckFace)
				{
					return pBrush;
				}
			}
			break;;
		}
		default:
			assert(0);
			break;
		}
	}

	return NULL;
}

// *************************************************************************
// *						Brush_GetTopLevelParent						   *
// *************************************************************************
Brush* CL64_Brush::Brush_GetTopLevelParent(const BrushList* pList,const Brush* b)
{
	Brush const* bWork;
	Brush* pImmediateParent;

	bWork = b;

	while (Brush_GetParent(pList, bWork, &pImmediateParent) == GE_TRUE)
	{
		if (bWork == pImmediateParent)
		{
			break;
		}

		bWork = pImmediateParent;
	}

	return (Brush*)bWork;
}

// *************************************************************************
// *			( Static ) BrushList_SetNextSelectedFace				   *
// *************************************************************************
static signed int BrushList_SetNextSelectedFace(BrushList* pList)
{
	Brush* b;

	assert(pList);

	for (b = pList->First; b; b = b->Next)
	{
		if (App->CL_Brush->Brush_GetSelectedFace(b))
		{
			break;
		}
	}
	if (!b)	//no faces found selected
	{
		App->CL_Brush->Brush_SelectFirstFace(pList->First);	//in case it's also a multi
		return	GE_TRUE;
	}
	for (; b; b = b->Next)
	{
		if (App->CL_Brush->Brush_SetNextSelectedFace(b))
		{
			return	GE_TRUE;
		}
	}

	return	GE_FALSE;	//wrapped around the end... handle outside
}

// *************************************************************************
// *			( Static ) BrushList_SetPrevSelectedFace				   *
// *************************************************************************
static signed int BrushList_SetPrevSelectedFace(BrushList* pList)
{
	Brush* b;

	assert(pList);

	for (b = pList->Last; b; b = b->Prev)
	{
		if (App->CL_Brush->Brush_GetSelectedFace(b))
		{
			break;
		}
	}
	if (!b)	//no faces found selected
	{
		App->CL_Brush->Brush_SelectLastFace(pList->Last);	//in case it's also a multi
		return	GE_TRUE;
	}
	for (; b; b = b->Prev)
	{
		if (App->CL_Brush->Brush_SetPrevSelectedFace(b))
		{
			return	GE_TRUE;
		}
	}
	return	GE_FALSE;	//wrapped around the end... handle outside
}

// *************************************************************************
// *				( Static ) BrushList_GetSelectedFace				   *
// *************************************************************************
static Face* BrushList_GetSelectedFace(const BrushList* pList)
{
	Face* f;
	Brush* b;

	assert(pList);

	for (f = NULL, b = pList->First; (b && !f); b = b->Next)
	{
		f = App->CL_Brush->Brush_GetSelectedFace(b);
	}
	return	f;
}

// *************************************************************************
// *					Brush_SetNextSelectedFace						   *
// *************************************************************************
Face* CL64_Brush::Brush_GetSelectedFace(const Brush* b)
{
	assert(b);

	if (Brush_IsMulti(b))
	{
		return BrushList_GetSelectedFace(b->BList);
	}
	else
	{
		return	App->CL_FaceList->FaceList_GetSelectedFace(b->Faces);
	}
}

// *************************************************************************
// *					Brush_SetNextSelectedFace						   *
// *************************************************************************
signed int CL64_Brush::Brush_SetNextSelectedFace(Brush* b)
{
	assert(b);

	if (Brush_IsMulti(b))
	{
		return	BrushList_SetNextSelectedFace(b->BList);
	}
	else
	{
		return	App->CL_FaceList->FaceList_SetNextSelectedFace(b->Faces);
	}
}

// *************************************************************************
// *					Brush_SetPrevSelectedFace						   *
// *************************************************************************
signed int CL64_Brush::Brush_SetPrevSelectedFace(Brush* b)
{
	assert(b);

	if (Brush_IsMulti(b))
	{
		return	BrushList_SetPrevSelectedFace(b->BList);
	}
	else
	{
		return	App->CL_FaceList->FaceList_SetPrevSelectedFace(b->Faces);
	}
}

// *************************************************************************
// *							Brush_Rotate							   *
// *************************************************************************
void CL64_Brush::Brush_Rotate(Brush* b,const Matrix3d* pXfmRotate,const T_Vec3* pCenter)
{

	if (b->Type == BRUSH_MULTI)
	{
		BrushList_Rotate(b->BList, pXfmRotate, pCenter);
	}
	else
	{
		App->CL_FaceList->FaceList_Rotate(b->Faces, pXfmRotate, pCenter);
	}

	Brush_Bound(b);
}

// *************************************************************************
// *							Brush_Rotate							   *
// *************************************************************************
void CL64_Brush::BrushList_Rotate(BrushList* pList, const Matrix3d* pXfmRotate, const T_Vec3* pCenter)
{
	Brush* b;

	for (b = pList->First; b; b = b->Next)
	{
		Brush_Rotate(b, pXfmRotate, pCenter); // Recursive
	}
}


