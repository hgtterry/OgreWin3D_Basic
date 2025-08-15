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
#include "Structures.cpp"


CL64_FaceList::CL64_FaceList(void)
{
}

CL64_FaceList::~CL64_FaceList(void)
{
}

void CL64_FaceList::FaceList_SetBrushNames(const FaceList* fl, const char* Name)
{
	int			i;
	
	for (i = 0; i < fl->NumFaces; i++)
	{
		App->CL_Face->Face_SetBrushName(fl->Faces[i], Name);
	}
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
			App->CL_Box_x->Box3d_Union(&Bounds, &FaceBounds, &Bounds);
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
signed int CL64_FaceList::FaceList_GetUsedTextures(const FaceList* pList, signed int* WrittenTex, CL64_WadFile* WadFile)
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

// *************************************************************************
// *						FaceList_ClipFaceToList					 	   *
// *************************************************************************
void CL64_FaceList::FaceList_ClipFaceToList(const FaceList* fl, Face** f)
{
	int			i;
	const GPlane* p;
	Ogre::uint8		cnt[3];

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

// *************************************************************************
// *							FaceList_Move						 	   *
// *************************************************************************
void CL64_FaceList::FaceList_Move(FaceList* pList, const T_Vec3* trans)
{
	int i;

	for (i = 0; i < pList->NumFaces; i++)
	{
		App->CL_Face->Face_Move(pList->Faces[i], trans);
	}
	pList->Dirty = GE_TRUE;
}

// *************************************************************************
// *							FaceList_Scale						 	   *
// *************************************************************************
signed int CL64_FaceList::FaceList_Scale(FaceList* pList, const T_Vec3* ScaleVec)
{
	signed int Success = 1;
	
	int i;

	assert(pList != NULL);
	assert(ScaleVec != NULL);

	for (i = 0; i < pList->NumFaces; i++)
	{
		Success = Success && App->CL_Face->Face_Scale(pList->Faces[i], ScaleVec);
	}
	pList->Dirty = GE_TRUE;
	
	return Success;
}

// *************************************************************************
// *					FaceList_SetNextSelectedFace				 	   *
// *************************************************************************
signed int CL64_FaceList::FaceList_SetNextSelectedFace(FaceList* fl)
{
	int		i;

	assert(fl);

	for (i = 0; i < fl->NumFaces; i++)
	{
		if (App->CL_Face->Face_IsSelected(fl->Faces[i]))
		{
			break;
		}
	}

	if (i < fl->NumFaces - 1)
	{
		App->CL_Face->Face_SetSelected(fl->Faces[i], GE_FALSE);
		i++;
		i %= fl->NumFaces;
		App->CL_Face->Face_SetSelected(fl->Faces[i], GE_TRUE);
		return	GE_TRUE;
	}
	else if (i < fl->NumFaces)
	{
		App->CL_Face->Face_SetSelected(fl->Faces[i], GE_FALSE);
		return	GE_FALSE;	//skip to next brush or select first 
	}
	else	//if it didn't overflow... there simply wasn't
	{		//anything selected, select the first face
		App->CL_Face->Face_SetSelected(fl->Faces[0], GE_TRUE);
		return	GE_TRUE;
	}
}

// *************************************************************************
// *					FaceList_SetPrevSelectedFace				 	   *
// *************************************************************************
signed int CL64_FaceList::FaceList_SetPrevSelectedFace(FaceList* fl)
{
	int		i;

	assert(fl);

	for (i = 0; i < fl->NumFaces; i++)
	{
		if (App->CL_Face->Face_IsSelected(fl->Faces[i]))
		{
			break;
		}
	}

	if (i >= fl->NumFaces)	//if it didn't underflow... there simply wasn't
	{						//anything selected, select the last face
		App->CL_Face->Face_SetSelected(fl->Faces[fl->NumFaces - 1], GE_TRUE);
		return	GE_TRUE;
	}
	else if (i == 0)
	{
		App->CL_Face->Face_SetSelected(fl->Faces[i], GE_FALSE);
		return	GE_FALSE;	//skip to prev brush or select last
	}
	else
	{
		App->CL_Face->Face_SetSelected(fl->Faces[i], GE_FALSE);
		i--;
		App->CL_Face->Face_SetSelected(fl->Faces[i], GE_TRUE);
		return	GE_TRUE;
	}
}

// *************************************************************************
// *					FaceList_GetSelectedFace					 	   *
// *************************************************************************
Face* CL64_FaceList::FaceList_GetSelectedFace(const FaceList* fl)
{
	int		i;

	assert(fl);

	for (i = 0; i < fl->NumFaces; i++)
	{
		if (App->CL_Face->Face_IsSelected(fl->Faces[i]))
		{
			break;
		}
	}

	if (i < fl->NumFaces)
	{
		return	fl->Faces[i];
	}
	else
	{
		return	NULL;
	}
}

// *************************************************************************
// *							FaceList_Rotate						 	   *
// *************************************************************************
void CL64_FaceList::FaceList_Rotate(FaceList* pList, const Matrix3d* pXfm, const T_Vec3* pCenter)
{
	int i;

	for (i = 0; i < pList->NumFaces; i++)
	{
		Face_Rotate(pList->Faces[i], pXfm, pCenter);
	}

	pList->Dirty = GE_TRUE;
}

// *************************************************************************
// *							Face_Rotate							 	   *
// *************************************************************************
void CL64_FaceList::Face_Rotate(Face* f, const Matrix3d* pXfmRotate, const T_Vec3* pCenter)
{
	int	i;
	T_Vec3* pPoint;

	assert(f != NULL);
	assert(pXfmRotate != NULL);
	assert(pCenter != NULL);

	for (i = 0; i < f->NumPoints; i++)
	{
		pPoint = &f->Points[i];
		App->CL_Maths->Vector3_Subtract(pPoint, pCenter, pPoint);
		App->CL_Maths->XForm3d_Rotate(pXfmRotate, pPoint, pPoint);
		App->CL_Maths->Vector3_Add(pPoint, pCenter, pPoint);
	}

	App->CL_Face->Face_SetPlaneFromFace(f);
	App->CL_Face->Face_XfmTexture(f, pXfmRotate);

	pPoint = &f->Tex.Pos;

	App->CL_Maths->Vector3_Subtract(pPoint, pCenter, pPoint);
	App->CL_Maths->XForm3d_Rotate(pXfmRotate, pPoint, pPoint);
	App->CL_Maths->Vector3_Add(pPoint, pCenter, pPoint);

	f->Tex.DirtyFlag = GE_TRUE;
}
