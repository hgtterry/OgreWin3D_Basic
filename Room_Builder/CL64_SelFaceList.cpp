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
#include "CL64_SelFaceList.h"

struct tag_SelFaceList
{
	Array* pItems;
	int FirstFree;
};

CL64_SelFaceList::CL64_SelFaceList(void)
{
}

CL64_SelFaceList::~CL64_SelFaceList(void)
{
}

// *************************************************************************
// *						SelFaceList_Create							   *
// *************************************************************************
SelFaceList* CL64_SelFaceList::SelFaceList_Create(void)
{
	SelFaceList* pList;

	pList = (SelFaceList*)App->CL_Maths->Ram_Allocate(sizeof(SelFaceList));
	if (pList != NULL)
	{
		pList->pItems = App->CL_Array->Array_Create(10, sizeof(Face*));
		if (pList->pItems != NULL)
		{
			pList->FirstFree = 0;
		}
		else
		{
			SelFaceList_Destroy(&pList);
		}
	}
	return pList;
}

// *************************************************************************
// *						SelFaceList_Destroy							   *
// *************************************************************************
void CL64_SelFaceList::SelFaceList_Destroy(SelFaceList** ppList)
{
	SelFaceList* pList;

	assert(ppList != NULL);
	assert(*ppList != NULL);
	pList = *ppList;

	if (pList->pItems != NULL)
	{
		App->CL_Array->Array_Destroy(&pList->pItems);
	}

	App->CL_Maths->Ram_Free(*ppList);
}