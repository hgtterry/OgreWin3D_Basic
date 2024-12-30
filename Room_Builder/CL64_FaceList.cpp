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
#include "CL64_FaceList.h"

struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	bool Dirty;
	Box3d Bounds;
};

CL64_FaceList::CL64_FaceList(void)
{
}

CL64_FaceList::~CL64_FaceList(void)
{
}

// *************************************************************************
// *							FaceList_Create						 	   *
// *************************************************************************
FaceList* CL64_FaceList::FaceList_Create(int NumFaces)
{
	FaceList* pList;

	assert(NumFaces > 0);

	// allocate the structure
	pList = (FaceList*)App->CL_Maths->Ram_Allocate(sizeof(FaceList));
	if (pList != NULL)
	{
		pList->NumFaces = 0;
		pList->Limit = NumFaces;
		// allocate space for NumFaces pointers
		pList->Faces = (struct FaceTag**)App->CL_Maths->Ram_Allocate(NumFaces * sizeof(Face*));
		if (pList->Faces != NULL)
		{
			int i;
			// set the pointers to NULL
			for (i = 0; i < pList->Limit; i++)
			{
				pList->Faces[i] = NULL;
			}
		}
		pList->Dirty = true;
	}

	return pList;
}

// *************************************************************************
// *						FaceList_AddFace						 	   *
// *************************************************************************
void CL64_FaceList::FaceList_AddFace(FaceList* pList, Face* pFace)
{
	pList->Faces[pList->NumFaces] = pFace;
	++(pList->NumFaces);
	pList->Dirty = true;
}
