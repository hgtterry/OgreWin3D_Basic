#include "pch.h"
#include "CL64_App.h"
#include "CX_Cut_Brush.h"
#include "Structures.cpp"

static	Brush* bstack[73314096];	//8192 levels of recursion
static	Brush** bsp;

static	float			dists[16384];
static	Ogre::uint8		sides[16384];
static	Ogre::uint8		fsides[16384];

#define BOGUS_RANGE					32000.0f

CX_Cut_Brush::CX_Cut_Brush(void)
{
}

CX_Cut_Brush::~CX_Cut_Brush(void)
{
}

// *************************************************************************
// *	( Static )				Brush_MostlyOnSide_T					   *
// *************************************************************************
static int	Brush_MostlyOnSide_T(const Brush* b, const GPlane* p)
{
	int		i, side;
	float	max;

	max = 0;
	side = SIDE_FRONT;
	for (i = 0; i < App->CL_X_FaceList->FaceList_GetNumFaces(b->Faces); i++)
	{
		App->CL_X_Face->Face_MostlyOnSide(App->CL_X_FaceList->FaceList_GetFace(b->Faces, i), p, &max, &side);
	}
	return	side;
}

// *************************************************************************
// *	( Static )			Brush_CreateFromParent_T					   *
// *************************************************************************
static Brush* Brush_CreateFromParent_T(const Brush* ParentBrush, const FaceList* fl)
{
	Brush* pBrush;

	assert(ParentBrush);
	assert(fl != NULL);
	assert(ParentBrush->Type != BRUSH_MULTI);

	pBrush = (Brush*)App->Ram_Allocate(sizeof(Brush), "Brush_CreateFromParent");
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
		strcpy(pBrush->Name, ParentBrush->Name);
		pBrush->Type = BRUSH_CSG;
		App->CL_X_FaceList->FaceList_GetBounds(fl, &pBrush->BoundingBox);
	}
	return pBrush;
}

// *************************************************************************
// *						Brush_SplitByFace							   *
// *************************************************************************
void CX_Cut_Brush::Brush_SplitByFace_T(Brush* ogb, Face* sf, Brush** fb, Brush** bb)
{
	const GPlane* p;
	int			i;
	Ogre::uint64	cnt[3], fcnt[4];
	FaceList* fl, * bl;
	const Face* f;
	Face* cpf, * ff, * bf, * midf;
	signed int	WasSplit = false;

	assert(ogb);
	assert(sf);
	assert(fb);
	assert(bb);
	assert(*fb == NULL);
	assert(*bb == NULL);

	p = App->CL_X_Face->Face_GetPlane(sf);

	fcnt[0] = fcnt[1] = fcnt[2] = fcnt[3] = 0;

	for (i = 0; i < App->CL_X_FaceList->FaceList_GetNumFaces(ogb->Faces); i++)
	{
		f = App->CL_X_FaceList->FaceList_GetFace(ogb->Faces, i);
		App->CL_X_Face->Face_GetSplitInfo(f, p, dists, sides, cnt);

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
		midf = App->CL_X_Face->Face_Clone(sf);
		App->CL_X_FaceList->FaceList_ClipFaceToList(ogb->Faces, &midf);
		if (!midf)
		{
			if (Brush_MostlyOnSide_T(ogb, App->CL_X_Face->Face_GetPlane(sf)) == SIDE_FRONT)
			{
				*fb = App->CL_X_Brush->Brush_Clone(ogb);
			}
			else
			{
				*bb = App->CL_X_Brush->Brush_Clone(ogb);
			}
			return;
		}
		//alloc face lists for front and back
		fl = App->CL_X_FaceList->FaceList_Create(fcnt[SIDE_FRONT] + fcnt[SIDE_SPLIT] + 1);
		bl = App->CL_X_FaceList->FaceList_Create(fcnt[SIDE_BACK] + fcnt[SIDE_SPLIT] + 1);

		WasSplit = false;
		for (i = 0; i < App->CL_X_FaceList->FaceList_GetNumFaces(ogb->Faces); i++)
		{
			f = App->CL_X_FaceList->FaceList_GetFace(ogb->Faces, i);

			switch (fsides[i])
			{
			case	SIDE_FRONT:
				cpf = App->CL_X_Face->Face_Clone(f);
				if (cpf)
				{
					App->CL_X_FaceList->FaceList_AddFace(fl, cpf);
				}
				break;
			case	SIDE_BACK:
				cpf = App->CL_X_Face->Face_Clone(f);
				if (cpf)
				{
					App->CL_X_FaceList->FaceList_AddFace(bl, cpf);
				}
				break;
			case	SIDE_SPLIT:	//this info should be reused from above!!!
				ff = bf = NULL;
				App->CL_X_Face->Face_GetSplitInfo(f, p, dists, sides, cnt);
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
					WasSplit = true;
					App->CL_X_Face->Face_Split(f, p, &ff, &bf, dists, sides);
				}

				if (ff)
				{
					App->CL_X_FaceList->FaceList_AddFace(fl, ff);
				}
				if (bf)
				{
					App->CL_X_FaceList->FaceList_AddFace(bl, bf);
				}
			}
		}
		if (WasSplit)
		{
			//add and clip the split face
			//FaceList_ClipFaceToList(bl, &cpf);
			App->CL_X_FaceList->FaceList_AddFace(bl, midf);

			//flip for front side brush
			cpf = App->CL_X_Face->Face_CloneReverse(midf);
			App->CL_X_FaceList->FaceList_AddFace(fl, cpf);
		}

		*fb = Brush_CreateFromParent_T(ogb, fl);
		*bb = Brush_CreateFromParent_T(ogb, bl);

		Brush_SealFaces_T(fb);
		Brush_SealFaces_T(bb);
	}
	else if (fcnt[SIDE_FRONT] && fcnt[SIDE_BACK])
	{
		if (fcnt[SIDE_FRONT] > fcnt[SIDE_BACK])	//mostly in front
		{
			*fb = App->CL_X_Brush->Brush_Clone(ogb);
		}
		else									//mostly behind
		{
			*bb = App->CL_X_Brush->Brush_Clone(ogb);
		}
	}
	else if (!fcnt[SIDE_FRONT])		//all faces behind
	{
		*bb = App->CL_X_Brush->Brush_Clone(ogb);
	}
	else if (!fcnt[SIDE_BACK])		//all faces front
	{
		*fb = App->CL_X_Brush->Brush_Clone(ogb);
	}
}

// *************************************************************************
// *    ( Static )     Brush_CutBrush:- Terry and Hazel Flanigan 2025      *
// *************************************************************************
//cuts b2 by b (b should be a cut brush)
static void	Brush_CutBrush_T(Brush* b, Brush* b2)
{
	Brush* cb, * fb, * bb = NULL, * cb2;
	Face* f, * sf;
	const GPlane* p;
	int			i;

	assert(b);
	assert(b2);
	assert(b2->Type != BRUSH_CSG);
	assert(b->Flags & BRUSH_SUBTRACT);

	int Cut_Brush_Index = App->CL_Brush_X->Get_Brush_Index_By_Name(b->Name);
	b2->Has_Been_Cut = 1;

	if (b2->Type == BRUSH_LEAF)
	{
		if (b2->BList)
		{
			for (cb = b2->BList->First; cb;)
			{
				if (!App->CL_X_Brush->Brush_TestBoundsIntersect(b, &cb->BoundingBox))
				{
					cb = cb->Next;
					continue;
				}

				cb2 = cb->Next;

				for (i = 0; i < App->CL_X_FaceList->FaceList_GetNumFaces(b->Faces); i++)
				{
					f = App->CL_X_FaceList->FaceList_GetFace(b->Faces, i);
					p = App->CL_X_Face->Face_GetPlane(f);

					//create a new face from the split plane
					sf = App->CL_X_Face->Face_CreateFromPlane(p, BOGUS_RANGE, 0);
					App->CL_X_Face->Face_CopyFaceInfo(f, sf);
					f->Cut_Brush_Index = Cut_Brush_Index;
					fb = bb = NULL;

					//split b by sf, adding the front brush to the brushlist
					App->CL_X_Brush->Brush_SplitByFace(cb, sf, &fb, &bb);
					if (fb)
					{	//clear hollow for csg
						fb->Flags &= ~(BRUSH_HOLLOW | BRUSH_HOLLOWCUT);
						App->CL_X_Brush->BrushList_Prepend(b2->BList, fb);
					}

					App->CL_X_Face->Face_Destroy(&sf);

					if (!i)
					{
						App->CL_X_Brush->BrushList_Remove(b2->BList, cb);
					}
					App->CL_X_Brush->Brush_Destroy(&cb);

					//make the back side brush current
					cb = bb;
					if (!cb)
						break;
				}

				if (bb)
				{
					App->CL_X_Brush->Brush_Destroy(&bb);

				}

				cb = cb2;
			}
		}
		else
		{
			b2->BList = App->CL_X_Brush->BrushList_Create();
			cb = b2;
			for (i = 0; i < App->CL_X_FaceList->FaceList_GetNumFaces(b->Faces); i++)
			{
				f = App->CL_X_FaceList->FaceList_GetFace(b->Faces, i);
				p = App->CL_X_Face->Face_GetPlane(f);

				//create a new face from the split plane
				sf = App->CL_X_Face->Face_CreateFromPlane(p, BOGUS_RANGE, 0);
				App->CL_X_Face->Face_CopyFaceInfo(f, sf);
				f->Cut_Brush_Index = Cut_Brush_Index;

				fb = bb = NULL;

				//split b by sf, adding the front brush to the brushlist
				App->CL_Cut_Brush->Brush_SplitByFace_T(cb, sf, &fb, &bb);
				if (fb)
				{	//clear hollow for csg
					fb->Flags &= ~(BRUSH_HOLLOW | BRUSH_HOLLOWCUT);
					App->CL_X_Brush->BrushList_Append(b2->BList, fb);
				}

				App->CL_X_Face->Face_Destroy(&sf);

				if (i)
				{
					App->CL_X_Brush->Brush_Destroy(&cb);
				}

				//make the back side brush current
				cb = bb;
				if (!cb)
					break;
			}

			if (bb)
			{
				App->CL_X_Brush->Brush_Destroy(&bb);
			}
		}
	}
	else
	{
		for (cb = b2->BList->First; cb; cb = cb->Next)
		{
			if (App->CL_X_Brush->Brush_TestBoundsIntersect(b, &cb->BoundingBox))
			{
				if (!(cb->Flags & BRUSH_SUBTRACT))
				{
					Brush_CutBrush_T(b, cb); // Recursive
				}
			}
		}
	}
}


// *************************************************************************
// *    ( Static ) BrushList_DoHollowCuts:- Terry and Hazel Flanigan 2025  *
// *************************************************************************
static void	BrushList_DoHollowCuts_T(BrushList* pList, int mid, Brush_CSGCallback Callback, void* lParam)
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
			BrushList_DoHollowCuts_T(b->BList, mid, Callback, lParam);
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

						if (App->CL_X_Brush->Brush_TestBoundsIntersect(cb, &b2->BoundingBox))
						{
							assert(b2->Type != BRUSH_CSG);
							Brush_CutBrush_T(cb, b2);
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

					if (App->CL_X_Brush->Brush_TestBoundsIntersect(b, &b2->BoundingBox))
					{
						assert(b2->Type != BRUSH_CSG);

						Brush_CutBrush_T(b, b2);
					}
				}
			}
		}
	}
}

// *************************************************************************
// *	( Static )			BrushList_DoCuts_T							   *
// *************************************************************************
static void	BrushList_DoCuts_T(BrushList* pList, int mid, Brush_CSGCallback Callback, void* lParam)
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

						if (App->CL_X_Brush->Brush_TestBoundsIntersect(cb, &b2->BoundingBox))
						{
							assert(b2->Type != BRUSH_CSG);
							if (!(cb->Flags & BRUSH_HOLLOWCUT))
							{
								Brush_CutBrush_T(cb, b2);
							}
						}
						else
						{
							b2->Has_Been_Cut = 0;
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

					if (App->CL_X_Brush->Brush_TestBoundsIntersect(b, &b2->BoundingBox))
					{
						assert(b2->Type != BRUSH_CSG);

						Brush_CutBrush_T(b, b2);
					}
					else
					{
						b2->Has_Been_Cut = 0;
					}
				}
			}
		}
	}
}

// *************************************************************************
// *							BrushList_DoCSG							   *
// *************************************************************************
void CX_Cut_Brush::BrushList_DoCSG(BrushList* inList, int mid, Brush_CSGCallback Callback, void* lParam)
{
	assert(inList != NULL);

	bsp = bstack;

	App->CL_X_Brush->BrushList_ClearCSGAndHollows(inList, mid);
	App->CL_X_Brush->BrushList_RebuildHollowFaces(inList, mid, Callback, lParam);
	BrushList_DoHollowCuts_T(inList, mid, Callback, lParam);

	bsp = bstack;	//reset this just incase...

	BrushList_DoCuts_T(inList, mid, Callback, lParam);
}

// *************************************************************************
// *						Brush_SealFaces								   *
// *************************************************************************
void CX_Cut_Brush::Brush_SealFaces_T(Brush** b)
{
	const GPlane* p;
	Face* f2;
	Face* f;
	FaceList* fl;
	int			i, j;
	Ogre::uint64		cnt[3];

	assert(b != NULL);
	assert(*b != NULL);
	assert((*b)->Type != BRUSH_MULTI);

	if (App->CL_X_FaceList->FaceList_GetNumFaces((*b)->Faces) < 4)
	{
		App->CL_X_Brush->Brush_Destroy(b);
		return;
	}
	fl = App->CL_X_FaceList->FaceList_Create(App->CL_X_FaceList->FaceList_GetNumFaces((*b)->Faces));

	//expand all faces out to seal cracks
	for (i = 0; i < App->CL_X_FaceList->FaceList_GetNumFaces((*b)->Faces); i++)
	{
		f = (Face*)App->CL_X_FaceList->FaceList_GetFace((*b)->Faces, i);
		p = App->CL_X_Face->Face_GetPlane(f);
		f2 = App->CL_X_Face->Face_CreateFromPlane(p, BOGUS_RANGE, 0);
		App->CL_X_Face->Face_CopyFaceInfo(f, f2);
		App->CL_X_FaceList->FaceList_AddFace(fl, f2);
	}
	for (i = 0; i < App->CL_X_FaceList->FaceList_GetNumFaces(fl); i++)
	{
		//const override here
		f = (Face*)App->CL_X_FaceList->FaceList_GetFace(fl, i);

		for (j = 0; j < App->CL_X_FaceList->FaceList_GetNumFaces(fl); j++)
		{
			if (j == i)
			{
				continue;
			}
			f2 = App->CL_X_FaceList->FaceList_GetFace(fl, j);
			p = App->CL_X_Face->Face_GetPlane(f2);

			App->CL_X_Face->Face_GetSplitInfo(f, p, dists, sides, cnt);

			if (!cnt[SIDE_FRONT] && !cnt[SIDE_BACK])	//coplanar
			{
				App->CL_X_FaceList->FaceList_RemoveFace(fl, i);
				j--;	//to ensure we restart if this is the last face
				break;
			}
			else if (!cnt[SIDE_FRONT])	//back
			{
				continue;
			}
			else if (!cnt[SIDE_BACK])	//front
			{
				App->CL_X_FaceList->FaceList_RemoveFace(fl, i);
				j--;	//to ensure we restart if this is the last face
				break;
			}
			else	//split
			{
				App->CL_X_Face->Face_Clip(f, p, dists, sides);
			}
		}
		if (j < App->CL_X_FaceList->FaceList_GetNumFaces(fl))
		{
			i = -1;	//restart!
		}
	}
	if (App->CL_X_FaceList->FaceList_GetNumFaces(fl) < 4)
	{
		//		ConPrintf("Overconstrained brush clipped away...\n");
		App->CL_X_Brush->Brush_Destroy(b);
		App->CL_X_FaceList->FaceList_Destroy(&fl);
	}
	else
	{
		App->CL_X_FaceList->FaceList_Destroy(&(*b)->Faces);
		(*b)->Faces = fl;
		App->CL_X_Brush->Brush_Bound((*b));
	}
}
