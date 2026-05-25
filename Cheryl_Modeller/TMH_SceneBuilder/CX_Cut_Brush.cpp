#include "pch.h"
#include "CL64_App.h"
#include "CX_Cut_Brush.h"
#include "Structures.cpp"

static	Brush* bstack[73314096];	//8192 levels of recursion
static	Brush** bsp;

#define BOGUS_RANGE					32000.0f

CX_Cut_Brush::CX_Cut_Brush(void)
{
}

CX_Cut_Brush::~CX_Cut_Brush(void)
{
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
				App->CL_X_Brush->Brush_SplitByFace(cb, sf, &fb, &bb);
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
