/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
#include "CX_FaceList.h"
#include "Structures.cpp"

CX_FaceList::CX_FaceList(void)
{
}

CX_FaceList::~CX_FaceList(void)
{
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* CX_FaceList::GetVersion()
{
	return (LPSTR)" TMH_Scene_Builder :-- CX_FaceList [ 06-09-25 ] Build 1 ";
}

void CX_FaceList::FaceList_SetBrushNames(const FaceList* fl, const char* Name)
{
	int			i;

	for (i = 0; i < fl->NumFaces; i++)
	{
		App->CL_X_Face->Face_SetBrushName(fl->Faces[i], Name);
	}
}

// *************************************************************************
// *							FaceList_Create						 	   *
// *************************************************************************
FaceList* CX_FaceList::FaceList_Create(int NumFaces)
{
	FaceList* pList;

	// allocate the structure
	pList = (FaceList*)App->CL_X_Maths->Ram_Allocate(sizeof(FaceList));
	if (pList != NULL)
	{
		pList->NumFaces = 0;
		pList->Limit = NumFaces;
		// allocate space for NumFaces pointers
		pList->Faces = (struct FaceTag**)App->CL_X_Maths->Ram_Allocate(NumFaces * sizeof(Face*));
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
void CX_FaceList::FaceList_AddFace(FaceList* pList, Face* pFace)
{
	pList->Faces[pList->NumFaces] = pFace;
	++(pList->NumFaces);
	pList->Dirty = true;
}

// *************************************************************************
// *						FaceList_GetBounds						 	   *
// *************************************************************************
void CX_FaceList::FaceList_GetBounds(const FaceList* pList, Box3d* pBounds)
{
	int i;

	if (pList->Dirty)
	{
		Box3d	Bounds;

		App->CL_X_Face->Face_GetBounds(pList->Faces[0], &Bounds);
		for (i = 1; i < pList->NumFaces; i++)
		{
			Box3d FaceBounds;

			App->CL_X_Face->Face_GetBounds(pList->Faces[i], &FaceBounds);
			App->CL_X_Box->Box3d_Union(&Bounds, &FaceBounds, &Bounds);
		}


		((FaceList*)pList)->Bounds = Bounds;
		((FaceList*)pList)->Dirty = false;
	}

	*pBounds = pList->Bounds;
}

// *************************************************************************
// *						FaceList_GetFace						 	   *
// *************************************************************************
Face* CX_FaceList::FaceList_GetFace(const FaceList* pList, int WhichFace)
{
	return	pList->Faces[WhichFace];
}

// *************************************************************************
// *						FaceList_GetNumFaces					 	   *
// *************************************************************************
int	CX_FaceList::FaceList_GetNumFaces(const FaceList* pList)
{
	return pList->NumFaces;
}

// *************************************************************************
// *						FaceList_Destroy						 	   *
// *************************************************************************
void CX_FaceList::FaceList_Destroy(FaceList** ppList)
{
	int i;
	FaceList* pList;

	pList = *ppList;
	for (i = 0; i < pList->NumFaces; i++)
	{
		App->CL_X_Face->Face_Destroy(&pList->Faces[i]);
	}
	App->CL_X_Maths->Ram_Free(pList->Faces);
	App->CL_X_Maths->Ram_Free(*ppList);
	*ppList = NULL;
}

// *************************************************************************
// *						FaceList_RemoveFace						 	   *
// *************************************************************************
void CX_FaceList::FaceList_RemoveFace(FaceList* pList, int WhichFace)
{
	int	i;

	App->CL_X_Face->Face_Destroy(&pList->Faces[WhichFace]);

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
FaceList* CX_FaceList::FaceList_Clone(const FaceList* pList)
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
				cpf = App->CL_X_Face->Face_Clone(f);
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
void CX_FaceList::FaceList_SetDirty(FaceList* pList)
{
	pList->Dirty = true;
}

// *************************************************************************
// *					FaceList_GetUsedTextures					 	   *
// *************************************************************************
signed int CX_FaceList::FaceList_GetUsedTextures(const FaceList* pList, signed int* WrittenTex, CL64_WadFile* WadFile)
{
	int i, index;
	for (i = 0; i < pList->NumFaces; i++)
	{
		index = App->CL_X_Face->Face_GetTextureDibId(pList->Faces[i]);

		if (index < WadFile->mBitmapCount)
			WrittenTex[index] = true;
		else
		{
			WrittenTex[0] = true;
			App->CL_X_Face->Face_SetTextureDibId(pList->Faces[i], 0);
			App->CL_X_Face->Face_SetTextureName(pList->Faces[i], WadFile->mBitmaps[0].Name);
			App->CL_X_Face->Face_SetTextureSize(pList->Faces[i], WadFile->mBitmaps[0].Width, WadFile->mBitmaps[0].Height);
		}
	}

	return true;
}

// *************************************************************************
// *						FaceList_CopyFaceInfo					 	   *
// *************************************************************************
void CX_FaceList::FaceList_CopyFaceInfo(const FaceList* src, FaceList* dst)
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
		App->CL_X_Face->Face_CopyFaceInfo(src->Faces[i], dst->Faces[i]);
	}
	dst->Dirty = true;
}

static	float		dists[256];
static	Ogre::uint8		sides[256];

// *************************************************************************
// *						FaceList_ClipFaceToList					 	   *
// *************************************************************************
void CX_FaceList::FaceList_ClipFaceToList(const FaceList* fl, Face** f)
{
	int			i;
	const GPlane* p;
	Ogre::uint8		cnt[3];

	for (i = 0; i < fl->NumFaces; i++)
	{
		p = App->CL_X_Face->Face_GetPlane(fl->Faces[i]);

		App->CL_X_Face->Face_GetSplitInfo(*f, p, dists, sides, cnt);
		if (!cnt[SIDE_FRONT] && !cnt[SIDE_BACK])	//coplanar
		{
			App->CL_X_Face->Face_Destroy(f);
			return;
		}
		else if (!cnt[SIDE_FRONT])	//back
		{
			continue;
		}
		else if (!cnt[SIDE_BACK])	//front
		{
			App->CL_X_Face->Face_Destroy(f);
			return;
		}
		else	//split
		{
			App->CL_X_Face->Face_Clip(*f, p, dists, sides);
		}
	}
}

// *************************************************************************
// *							FaceList_Move						 	   *
// *************************************************************************
void CX_FaceList::FaceList_Move(FaceList* pList, const T_Vec3* trans)
{
	int i;

	for (i = 0; i < pList->NumFaces; i++)
	{
		App->CL_X_Face->Face_Move(pList->Faces[i], trans);
	}
	pList->Dirty = true;
}

// *************************************************************************
// *							FaceList_Scale						 	   *
// *************************************************************************
signed int CX_FaceList::FaceList_Scale(FaceList* pList, const T_Vec3* ScaleVec)
{
	signed int Success = 1;

	int i;

	assert(pList != NULL);
	assert(ScaleVec != NULL);

	for (i = 0; i < pList->NumFaces; i++)
	{
		Success = Success && App->CL_X_Face->Face_Scale(pList->Faces[i], ScaleVec);
	}
	pList->Dirty = true;

	return Success;
}

// *************************************************************************
// *					FaceList_SetNextSelectedFace				 	   *
// *************************************************************************
signed int CX_FaceList::FaceList_SetNextSelectedFace(FaceList* fl)
{
	int		i;

	assert(fl);

	for (i = 0; i < fl->NumFaces; i++)
	{
		if (App->CL_X_Face->Face_IsSelected(fl->Faces[i]))
		{
			break;
		}
	}

	if (i < fl->NumFaces - 1)
	{
		App->CL_X_Face->Face_SetSelected(fl->Faces[i], false);
		i++;
		i %= fl->NumFaces;
		App->CL_X_Face->Face_SetSelected(fl->Faces[i], true);
		return	true;
	}
	else if (i < fl->NumFaces)
	{
		App->CL_X_Face->Face_SetSelected(fl->Faces[i], false);
		return	false;	//skip to next brush or select first 
	}
	else	//if it didn't overflow... there simply wasn't
	{		//anything selected, select the first face
		App->CL_X_Face->Face_SetSelected(fl->Faces[0], true);
		return	true;
	}
}

// *************************************************************************
// *					FaceList_SetPrevSelectedFace				 	   *
// *************************************************************************
signed int CX_FaceList::FaceList_SetPrevSelectedFace(FaceList* fl)
{
	int		i;

	assert(fl);

	for (i = 0; i < fl->NumFaces; i++)
	{
		if (App->CL_X_Face->Face_IsSelected(fl->Faces[i]))
		{
			break;
		}
	}

	if (i >= fl->NumFaces)	//if it didn't underflow... there simply wasn't
	{						//anything selected, select the last face
		App->CL_X_Face->Face_SetSelected(fl->Faces[fl->NumFaces - 1], true);
		return	true;
	}
	else if (i == 0)
	{
		App->CL_X_Face->Face_SetSelected(fl->Faces[i], false);
		return	false;	//skip to prev brush or select last
	}
	else
	{
		App->CL_X_Face->Face_SetSelected(fl->Faces[i], false);
		i--;
		App->CL_X_Face->Face_SetSelected(fl->Faces[i], true);
		return	true;
	}
}

// *************************************************************************
// *					FaceList_GetSelectedFace					 	   *
// *************************************************************************
Face* CX_FaceList::FaceList_GetSelectedFace(const FaceList* fl)
{
	int		i;

	assert(fl);

	for (i = 0; i < fl->NumFaces; i++)
	{
		if (App->CL_X_Face->Face_IsSelected(fl->Faces[i]))
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
void CX_FaceList::FaceList_Rotate(FaceList* pList, const Matrix3d* pXfm, const T_Vec3* pCenter)
{
	int i;

	for (i = 0; i < pList->NumFaces; i++)
	{
		Face_Rotate(pList->Faces[i], pXfm, pCenter);
	}

	pList->Dirty = true;
}

// *************************************************************************
// *							Face_Rotate							 	   *
// *************************************************************************
void CX_FaceList::Face_Rotate(Face* f, const Matrix3d* pXfmRotate, const T_Vec3* pCenter)
{
	int	i;
	T_Vec3* pPoint;

	assert(f != NULL);
	assert(pXfmRotate != NULL);
	assert(pCenter != NULL);

	for (i = 0; i < f->NumPoints; i++)
	{
		pPoint = &f->Points[i];
		App->CL_X_Maths->Vector3_Subtract(pPoint, pCenter, pPoint);
		App->CL_X_Maths->XForm3d_Rotate(pXfmRotate, pPoint, pPoint);
		App->CL_X_Maths->Vector3_Add(pPoint, pCenter, pPoint);
	}

	App->CL_X_Face->Face_SetPlaneFromFace(f);
	App->CL_X_Face->Face_XfmTexture(f, pXfmRotate);

	pPoint = &f->Tex.Pos;

	App->CL_X_Maths->Vector3_Subtract(pPoint, pCenter, pPoint);
	App->CL_X_Maths->XForm3d_Rotate(pXfmRotate, pPoint, pPoint);
	App->CL_X_Maths->Vector3_Add(pPoint, pCenter, pPoint);

	f->Tex.DirtyFlag = true;
}
