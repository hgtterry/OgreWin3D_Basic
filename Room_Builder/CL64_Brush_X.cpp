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
#include "Structures.cpp"

CL64_Brush_X::CL64_Brush_X()
{
	Face_Count = 0;
}

CL64_Brush_X::~CL64_Brush_X()
{
}

// *************************************************************************
// *		BrushList_GetUsedTextures_X:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Brush_X::BrushList_GetUsedTextures_X(BrushList* BList, signed int* UsedTex)
{
	Brush* b;
	b = BList->First;
	while (b != NULL)
	{
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
	int Dib = App->CL_Face->Face_GetTextureDibId(f);
	App->CL_Mesh_Mgr->UsedTextures[Dib] = 1;
	
	return 1;
}

// *************************************************************************
// *    ( Static ) Get_Brush_Face_Count:- Terry and Hazel Flanigan 2025    *
// *************************************************************************
static signed int Get_Brush_Face_Count(Brush* pBrush, void* lParam)
{
	int nFaces = 0;
	nFaces = App->CL_Brush->Brush_GetNumFaces(pBrush);

	App->CL_Brush_X->Face_Count = App->CL_Brush_X->Face_Count + nFaces;

	return true;
}

// *************************************************************************
// *       Get_Brush_All_Faces_Count:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
int CL64_Brush_X::Get_Brush_All_Faces_Count(void)
{
	Face_Count = 0;
	App->CL_Face->Selected_Face_Index = 0;

	Brush* pBrush;

	pBrush = App->CL_Doc->CurBrush;// App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, iBrush);

	if (App->CL_Brush->Brush_IsMulti(App->CL_Doc->CurBrush))
	{
		BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(App->CL_Doc->CurBrush), this, Get_Brush_Face_Count);
	}
	else
	{
		Get_Brush_Face_Count(App->CL_Doc->CurBrush, this);
	}

	return Face_Count;
}

// *************************************************************************
// *		  Set_Brush_Faces_Name:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Brush_X::Set_Brush_Faces_Name(Brush* b)
{
	if (App->CL_Brush->Brush_IsMulti(b))
	{
		App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(b), this, Set_BrushFaces_Name);
	}
	else
	{
		Set_BrushFaces_Name(b, this);
	}
}

// *************************************************************************
// *						BrushList_EnumLeafBrushes					   *
// *************************************************************************
int	CL64_Brush_X::BrushList_EnumLeafBrushes(const BrushList* pList, void* pVoid, BrushList_CB	CallBack)
{
	signed int	bResult = true;	// TRUE means entire list was processed
	Brush* b;

	for (b = pList->First; b; b = b->Next)
	{
		assert(b->Type != BRUSH_CSG);

		if (b->Type == BRUSH_MULTI)
		{
			if (!BrushList_EnumLeafBrushes(b->BList, pVoid, CallBack))
			{
				break;
			}
		}
		else if ((bResult = CallBack(b, pVoid)) == false)
		{
			
			break;
		}
	}

	return bResult;
}

// *************************************************************************
// *		Check_if_Brush_Name_Exist:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Brush_X::Check_if_Brush_Name_Exist(char* Name)
{
	int Result = 1;
	Brush* b;

	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

	b = pList->First;
	while (b != NULL)
	{
		Result = strcmp(b->Name, Name);
		if (Result == 0)
		{
			return 1;
		}

		b = b->Next;
	}

	return 0;
}

// *************************************************************************
// *		Get_Brush_By_Name:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
Brush* CL64_Brush_X::Get_Brush_By_Name(const char* Name)
{
	int Result = 1;
	Brush* b;

	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

	b = pList->First;
	while (b != NULL)
	{
		Result = strcmp(b->Name, Name);
		if (Result == 0)
		{
			return b;
		}

		b = b->Next;
	}

	return NULL;
}

// *************************************************************************
// *			Get_Brush_Index_By_Name:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
int CL64_Brush_X::Get_Brush_Index_By_Name(const char* Name)
{
	int Count = 0;
	int Result = 1;
	Brush* b;

	BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

	b = pList->First;
	while (b != NULL)
	{
		Result = strcmp(b->Name, Name);
		if (Result == 0)
		{
			return Count;
		}

		b = b->Next;
		Count++;
	}

	return -1;
}

// *************************************************************************
// *		  Select_Brush_Editor:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Brush_X::Select_Brush_Editor(Brush* b)
{
	int Bnum = App->CL_Brush->Get_Brush_Count();
	if (Bnum > 0)
	{
		App->CL_Doc->UpdateSelected();
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

		
		App->CL_Properties_Brushes->Get_Index(App->CL_Doc->CurBrush);

		App->CL_Top_Tabs->Enable_Brush_Options_Buttons(true, false);
		App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(true);

		App->CL_Properties_Brushes->Update_SelectedBrushesCount_Dlg();

		App->CL_Top_Tabs->Update_Faces_Combo();

		App->CL_Properties_Tabs->Select_Brushes_Tab();
	}
}

