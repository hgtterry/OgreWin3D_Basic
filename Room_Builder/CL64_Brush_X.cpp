/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
#include "CL64_Brush_X.h"
#include "CL64_Brush.h"

struct tag_BrushList
{
	Brush* First;
	Brush* Last;
};

struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	geBoolean Dirty;
	Box3d Bounds;
};

CL64_Brush_X::CL64_Brush_X()
{
}

CL64_Brush_X::~CL64_Brush_X()
{
}

// *************************************************************************
// *		BrushList_GetUsedTextures_X:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Brush_X::BrushList_GetUsedTextures_X(signed int* UsedTex)
{
	Level* pLevel = App->CL_Doc->pLevel;
	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

	Brush* b;
	b = pList->First;
	while (b != NULL)
	{
		//Scan_Faces(b);
		Get_BrushData(b);
		b = b->Next;
	}
}

// *************************************************************************
// *	  	   Get_BrushData:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Brush_X::Get_BrushData(Brush* b)
{
	int Count = App->CL_Brush->Get_Brush_Count();

	if (Count > 0)
	{
		if (b)
		{
			Get_Brush_Info(b);
		}
		else
		{
			App->Say("No Brush Selected");	
		}
	}
	else
	{
		App->Say("The World has No Brushes");
	}
}

// *************************************************************************
// *	  	Get_Brush_Info:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_Brush_X::Get_Brush_Info(const Brush* b)
{
	if (b->Type == BRUSH_MULTI)
	{
		return Get_Brush_ListInfo(b->BList); // Recursive
	}
	if (b->Type == BRUSH_LEAF)
	{
		return Get_Brush_Faces_Info(b->Faces);
	}
	if (b->Type == BRUSH_CSG)
	{
		App->Say("BRUSH_CSG");
	}

	return 1;
}

// *************************************************************************
// *	  	Get_Brush_ListInfo:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Brush_X::Get_Brush_ListInfo(BrushList* BList)
{
	Brush* pBrush;
	BrushIterator bi;
	

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		Get_Brush_Info(pBrush);
		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}

	return 1;
}

// *************************************************************************
// *	  Get_Brush_Faces_Info:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Brush_X::Get_Brush_Faces_Info(const FaceList* pList)
{
	int i;
	
	if (pList->NumFaces < 0)
	{
	}
	else
	{
		for (i = 0; i < pList->NumFaces; i++)
		{
			Get_Face_Data(i, pList->Faces[i]);
		}

	}

	return 1;
}

// *************************************************************************
// *			  Get_Face_Data:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Brush_X::Get_Face_Data(int Index, const Face* f)
{
	//char buf[MAX_PATH];

	//int  i = 0;
	
	/*char buf2[MAX_PATH];
	strcpy(buf, App->CL_Face->Face_GetTextureName(f));*/
	int Dib = App->CL_Face->Face_GetTextureDibId(f);

	//sprintf(buf2, "%s%s %i", "Texture: = ", buf, Dib);

	App->CL_Mesh_Mgr->UsedTextures[Dib] = 1;
	//App->Say(buf2);
	
	return 1;
}

