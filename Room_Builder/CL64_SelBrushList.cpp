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
#include "CL64_SelBrushList.h"

struct tag_SelBrushList
{
	Array* pItems;
	int FirstFree;
};

CL64_SelBrushList::CL64_SelBrushList(void)
{
}

CL64_SelBrushList::~CL64_SelBrushList(void)
{
}

// *************************************************************************
// *						SelBrushList_Create						 	   *
// *************************************************************************
SelBrushList* CL64_SelBrushList::SelBrushList_Create(void)
{
	SelBrushList* pList;

	pList = (SelBrushList*)App->CL_Maths->Ram_Allocate(sizeof(SelBrushList));
	if (pList != NULL)
	{
		pList->pItems = App->CL_Array->Array_Create(10, sizeof(Brush*));
		if (pList->pItems != NULL)
		{
			pList->FirstFree = 0;
		}
		else
		{
			SelBrushList_Destroy(&pList);
		}
	}
	return pList;
}

// *************************************************************************
// *						SelBrushList_Destroy					 	   *
// *************************************************************************
void CL64_SelBrushList::SelBrushList_Destroy(SelBrushList** ppList)
{
	SelBrushList* pList;

	assert(ppList != NULL);
	assert(*ppList != NULL);
	pList = *ppList;

	if (pList->pItems != NULL)
	{
		App->CL_Array->Array_Destroy(&pList->pItems);
	}
	//geRam_Free(*ppList);
}

// *************************************************************************
// *							SelBrushList_Find					 	   *
// *************************************************************************
signed int CL64_SelBrushList::SelBrushList_Find(SelBrushList* pList, const Brush* pBrush)
{
	int i;

	// go through list to see if this Brush is already in the list
	for (i = 0; i < pList->FirstFree; ++i)
	{
		Brush* pRet;

		pRet = SelBrushList_GetBrush(pList, i);
		if (pRet == pBrush)
		{
			// Brush already in list
			return GE_TRUE;
		}
	}
	return GE_FALSE;
}

// *************************************************************************
// *						SelBrushList_GetBrush					 	   *
// *************************************************************************
Brush* CL64_SelBrushList::SelBrushList_GetBrush(SelBrushList* pList, int BrushIndex)
{
	Brush** ppBrush;

	ppBrush = (Brush**)Array_ItemPtr(pList->pItems, BrushIndex);

	return *ppBrush;
}

// *************************************************************************
// *						SelBrushList_GetSize					 	   *
// *************************************************************************
int CL64_SelBrushList::SelBrushList_GetSize(SelBrushList* pList)
{
	return pList->FirstFree;
}

// *************************************************************************
// *					SelBrushList_CenterEnum						 	   *
// *************************************************************************
static signed int SelBrushList_CenterEnum(Brush* b, void* lParam)
{
	Ogre::Vector3* center;
	Ogre::Vector3 newcenter;

	center = (Ogre::Vector3*)lParam;
	App->CL_Box->Box3d_GetCenter(&b->BoundingBox, &newcenter);
	App->CL_Maths->Vector3_Add(center, &newcenter, center);

	return GE_TRUE;
}

// *************************************************************************
// *						SelBrushList_Center						 	   *
// *************************************************************************
void CL64_SelBrushList::SelBrushList_Center(SelBrushList* pList, Ogre::Vector3* center)
{
	int listcount;
	Ogre::Vector3 average;

	assert(pList && center);

	listcount = SelBrushList_GetSize(pList);
	if (!listcount)
	{
		App->CL_Maths->Vector3_Clear(center);
		return;
	}

	App->CL_Maths->Vector3_Clear(&average);

	SelBrushList_Enum(pList, SelBrushList_CenterEnum, &average);

	App->CL_Maths->Vector3_Scale(&average, (1 / (float)listcount), center);
}

// *************************************************************************
// *						SelBrushList_Enum						 	   *
// *************************************************************************
void CL64_SelBrushList::SelBrushList_Enum(SelBrushList* pList, SelBrushList_Callback Callback, void* lParam)
{
	int i;

	for (i = 0; i < pList->FirstFree; ++i)
	{
		Brush* pBrush;

		pBrush = SelBrushList_GetBrush(pList, i);
		Callback(pBrush, lParam);
	}
}

// *************************************************************************
// *						SelBrushList_Add						 	   *
// *************************************************************************
signed int CL64_SelBrushList::SelBrushList_Add(SelBrushList* pList, Brush* pBrush)
{
	int Size = 0;

	if (SelBrushList_Find(pList, pBrush))
	{
		return GE_FALSE;
	}

	Size = Array_GetSize(pList->pItems);

	// Brush isn't already in list.  Put it at the end...
	if (pList->FirstFree == Size)
	{
		int NewSize;
		// Need to allocate more space
		NewSize = App->CL_Array->Array_Resize(pList->pItems, 2 * Size);
		if (NewSize == Size)
		{
			App->Say_Win("Can not assign Array");
			return GE_FALSE;
		}
	}

	Array_PutAt(pList->pItems, pList->FirstFree, &pBrush, sizeof(pBrush));
	++(pList->FirstFree);
	return GE_TRUE;
}

