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

// *************************************************************************
// *						FaceList_GetBounds						 	   *
// *************************************************************************
void CL64_FaceList::FaceList_GetBounds(const FaceList* pList, Box3d* pBounds)
{
	int i;

	if (pList->Dirty)
	{
		Box3d	Bounds;

		App->CL_Face->Face_GetBounds(pList->Faces[0], &Bounds);
		for (i = 1; i < pList->NumFaces; i++)
		{
			Box3d FaceBounds;

			App->CL_Face->Face_GetBounds(pList->Faces[i], &FaceBounds);
			App->CL_Box->Box3d_Union(&Bounds, &FaceBounds, &Bounds);
		}
		

		((FaceList*)pList)->Bounds = Bounds;
		((FaceList*)pList)->Dirty = false;
	}

	*pBounds = pList->Bounds;
}

// *************************************************************************
// *						FaceList_GetFace						 	   *
// *************************************************************************
Face* CL64_FaceList::FaceList_GetFace(const FaceList* pList, int WhichFace)
{
	return	pList->Faces[WhichFace];
}

// *************************************************************************
// *						FaceList_GetNumFaces					 	   *
// *************************************************************************
int	CL64_FaceList::FaceList_GetNumFaces(const FaceList* pList)
{
	return pList->NumFaces;
}

// *************************************************************************
// *						FaceList_Destroy						 	   *
// *************************************************************************
void CL64_FaceList::FaceList_Destroy(FaceList** ppList)
{
	int i;
	FaceList* pList;

	pList = *ppList;
	for (i = 0; i < pList->NumFaces; i++)
	{
		App->CL_Face->Face_Destroy(&pList->Faces[i]);
	}
	App->CL_Maths->Ram_Free(pList->Faces);
	App->CL_Maths->Ram_Free(*ppList);
	*ppList = NULL;
}

// *************************************************************************
// *						FaceList_RemoveFace						 	   *
// *************************************************************************
void CL64_FaceList::FaceList_RemoveFace(FaceList* pList, int WhichFace)
{
	int	i;

	App->CL_Face->Face_Destroy(&pList->Faces[WhichFace]);

	for (i = WhichFace; i < pList->NumFaces - 1; i++)
	{
		pList->Faces[i] = pList->Faces[i + 1];
	}
	pList->Faces[--pList->NumFaces] = NULL;

	pList->Dirty = true;
}

// *************************************************************************
// *						FaceList_Clone							 	   *
// *************************************************************************
FaceList* CL64_FaceList::FaceList_Clone(const FaceList* pList)
{
	FaceList* cpList;
	Face* cpf;
	int			i;
	const Face* f;

	cpList = FaceList_Create(pList->NumFaces);

	if (cpList)
	{
		for (i = 0; i < pList->NumFaces; i++)
		{
			f = FaceList_GetFace(pList, i);
			if (f)
			{
				cpf = App->CL_Face->Face_Clone(f);
				if (cpf)
				FaceList_AddFace(cpList, cpf);
			}
		}
	}
	return	cpList;
}

// *************************************************************************
// *						FaceList_SetDirty						 	   *
// *************************************************************************
void CL64_FaceList::FaceList_SetDirty(FaceList* pList)
{
	pList->Dirty = true;
}

// *************************************************************************
// *					FaceList_GetUsedTextures					 	   *
// *************************************************************************
signed int CL64_FaceList::FaceList_GetUsedTextures(const FaceList* pList, geBoolean* WrittenTex, CL64_WadFile* WadFile)
{
	int i, index;
	for (i = 0; i < pList->NumFaces; i++)
	{
		index = App->CL_Face->Face_GetTextureDibId(pList->Faces[i]);

		if (index < WadFile->mBitmapCount)
			WrittenTex[index] = GE_TRUE;
		else
		{
			WrittenTex[0] = GE_TRUE;
			App->CL_Face->Face_SetTextureDibId(pList->Faces[i], 0);
			App->CL_Face->Face_SetTextureName(pList->Faces[i], WadFile->mBitmaps[0].Name);
			App->CL_Face->Face_SetTextureSize(pList->Faces[i], WadFile->mBitmaps[0].Width, WadFile->mBitmaps[0].Height);
		}
	}

	return GE_TRUE;
}

// *************************************************************************
// *						FaceList_CopyFaceInfo					 	   *
// *************************************************************************
void CL64_FaceList::FaceList_CopyFaceInfo(const FaceList* src, FaceList* dst)
{
	int i;

	assert(src);
	assert(dst);

	if (src->NumFaces != dst->NumFaces)
	{
		return;
	}

	for (i = 0; i < src->NumFaces; i++)
	{
		App->CL_Face->Face_CopyFaceInfo(src->Faces[i], dst->Faces[i]);
	}
	dst->Dirty = GE_TRUE;
}

static	float		dists[256];
static	Ogre::uint8		sides[256];

enum SideFlags
{
	SIDE_FRONT = 0,
	SIDE_BACK = 1,
	SIDE_ON = 2,
	SIDE_SPLIT = 3
};

// *************************************************************************
// *						FaceList_ClipFaceToList					 	   *
// *************************************************************************
void CL64_FaceList::FaceList_ClipFaceToList(const FaceList* fl, Face** f)
{
	int			i;
	const GPlane* p;
	Guint8		cnt[3];

	assert(fl != NULL);
	assert(f != NULL);
	assert(*f != NULL);

	for (i = 0; i < fl->NumFaces; i++)
	{
		p = App->CL_Face->Face_GetPlane(fl->Faces[i]);

		App->CL_Face->Face_GetSplitInfo(*f, p, dists, sides, cnt);
		if (!cnt[SIDE_FRONT] && !cnt[SIDE_BACK])	//coplanar
		{
			App->CL_Face->Face_Destroy(f);
			return;
		}
		else if (!cnt[SIDE_FRONT])	//back
		{
			continue;
		}
		else if (!cnt[SIDE_BACK])	//front
		{
			App->CL_Face->Face_Destroy(f);
			return;
		}
		else	//split
		{
			App->CL_Face->Face_Clip(*f, p, dists, sides);
		}
	}
}
