/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D Scene Builder

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
#include "CX_SelFaceList.h"
#include "Structures.cpp"

CX_SelFaceList::CX_SelFaceList(void)
{
}

CX_SelFaceList::~CX_SelFaceList(void)
{
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* CX_SelFaceList::GetVersion()
{
	return (LPSTR)" OW3D_Scene_Builder :-- CX_SelFaceList [ 21-08-25 ] Build 1 ";
}

// *************************************************************************
// *						SelFaceList_Create							   *
// *************************************************************************
SelFaceList* CX_SelFaceList::SelFaceList_Create(void)
{
	SelFaceList* pList;

	pList = (SelFaceList*)App->CL_X_Maths->Ram_Allocate(sizeof(SelFaceList));
	if (pList != NULL)
	{
		pList->pItems = App->CL_X_Array->Array_Create(10, sizeof(Face*));
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
void CX_SelFaceList::SelFaceList_Destroy(SelFaceList** ppList)
{
	SelFaceList* pList;

	assert(ppList != NULL);
	assert(*ppList != NULL);
	pList = *ppList;

	if (pList->pItems != NULL)
	{
		App->CL_X_Array->Array_Destroy(&pList->pItems);
	}

	App->CL_X_Maths->Ram_Free(*ppList);
}

// *************************************************************************
// *						SelFaceList_GetSize							   *
// *************************************************************************
int CX_SelFaceList::SelFaceList_GetSize(SelFaceList* pList)
{
	return pList->FirstFree;
}

// *************************************************************************
// *						SelFaceList_RemoveAll						   *
// *************************************************************************
void CX_SelFaceList::SelFaceList_RemoveAll(SelFaceList* pList)
{
	pList->FirstFree = 0;
}

// *************************************************************************
// *						SelFaceList_GetFace							   *
// *************************************************************************
Face* CX_SelFaceList::SelFaceList_GetFace(SelFaceList* pList, int FaceIndex)
{
	Face** ppFace;

	ppFace = (Face**)Array_ItemPtr(pList->pItems, FaceIndex);

	return *ppFace;
}

// *************************************************************************
// *						SelFaceList_Add								   *
// *************************************************************************
signed int CX_SelFaceList::SelFaceList_Add(SelFaceList* pList, Face* pFace)
{
	int i, Size;

	// go through list to see if this face is already in the list
	for (i = 0; i < pList->FirstFree; ++i)
	{
		Face* pRet;

		pRet = SelFaceList_GetFace(pList, i);
		if (pRet == pFace)
		{
			// face already in list
			return false;
		}
	}

	Size = Array_GetSize(pList->pItems);
	assert(pList->FirstFree <= Size);

	// Face isn't already in list.  Put it at the end...
	if (pList->FirstFree == Size)
	{
		int NewSize;
		// Need to allocate more space
		NewSize = App->CL_X_Array->Array_Resize(pList->pItems, 2 * Size);
		if (NewSize == Size)
		{
			// couldn't resize.  Guess I can't add the face
			return false;
		}
	}
	Array_PutAt(pList->pItems, pList->FirstFree, &pFace, sizeof(pFace));
	++(pList->FirstFree);

	return true;
}

// *************************************************************************
// *						SelFaceList_Enum							   *
// *************************************************************************
void CX_SelFaceList::SelFaceList_Enum(SelFaceList* pList, SelFaceList_Callback Callback, void* lParam)
{
	int i;

	for (i = 0; i < pList->FirstFree; ++i)
	{
		Face* pFace;

		pFace = SelFaceList_GetFace(pList, i);
		Callback(pFace, lParam);
	}
}