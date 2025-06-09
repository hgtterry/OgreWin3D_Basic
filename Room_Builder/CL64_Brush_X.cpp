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
	Face_Index_Set = 0;
	Last_FinalRot = { 0, 0 ,0 };
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

	BrushList* pList = App->CL_Level->Level_Get_Main_Brushes();

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

	BrushList* pList = App->CL_Level->Level_Get_Main_Brushes();

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

	BrushList* pList = App->CL_Level->Level_Get_Main_Brushes();

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
	App->CL_Doc->CurBrush = b;
	// Entity Selected in OnSelchangeBrushlist
	// 
	// Check if there are any brushes available
	int Bnum = App->CL_Brush->Get_Brush_Count();
	if (Bnum > 0)
	{
		// Perform brush selection and update views
		App->CL_Doc->DoBrushSelection(b, brushSelToggle);
		App->CL_Doc->UpdateSelected();
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		
		// Update the brush list dialog with the current brush
		App->CL_Properties_Brushes->Select_in_BrushList_Dlg(App->CL_Doc->CurBrush);
		App->CL_Top_Tabs->Enable_TopBar_Brush_Buttons(true, false);
		
		// Enable or disable face buttons based on brush group ID
		if (b->GroupId == Enums::Brushs_ID_Area)
		{
			App->CL_Top_Tabs->Enable_TopBar_Face_Buttons(true);
		}
		else
		{
			App->CL_Top_Tabs->Enable_TopBar_Face_Buttons(false);
			App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();
			App->CL_Properties_Textures->Enable_FaceProps_Button(false);
		}
		
		// Update brush options and selected brushes count
		App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(true);
		App->CL_Properties_Brushes->Update_SelectedBrushesCount_Dlg();
		App->CL_Top_Tabs->Update_Faces_Combo();

		// Set the active tab to brushes
		App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 1;
		App->CL_Properties_Tabs->Select_Brushes_Tab();

	}
}

// *************************************************************************
// *	  	  Set_Brush_Faces_Index:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Brush_X::Set_Brush_Faces_Index(Brush* b)
{
	Face_Index_Set = 0;

	if (b)
	{
		if (b)
		{
			Decode_Brush(App->CL_Properties_Brushes->Selected_Brush);
		}
	}
	else
	{
		char buf[20];
		sprintf(buf, "%s", "Brushes Invalid");
		App->Say(buf);
	}
}

// *************************************************************************
// *	  	Show_Brush_Info:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_Brush_X::Decode_Brush(const Brush* b)
{
	if (b->Type == BRUSH_MULTI)
	{
		return Set_Sub_Brush_Faces_Indexs(b->BList);
	}
	if (b->Type == BRUSH_LEAF) // Single
	{
		return Set_Brush_Faces_Indexs(b->Faces);
	}
	if (b->Type == BRUSH_CSG)
	{
		App->Say("BRUSH_CSG");
	}

	return 1;
}

// *************************************************************************
// *	  Set_Sub_Brush_Faces_Indexs:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
bool CL64_Brush_X::Set_Sub_Brush_Faces_Indexs(BrushList* BList)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		Decode_Brush(pBrush); // Recursive
		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}

	return 1;
}

// *************************************************************************
// *	  Set_Brush_Faces_Indexs:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Brush_X::Set_Brush_Faces_Indexs(const FaceList* pList)
{
	int i;

	if (pList->NumFaces < 0)
	{
	}
	else
	{
		for (i = 0; i < pList->NumFaces; i++)
		{
			if (!Set_Face_Index(Face_Index_Set, pList->Faces[i])) return 0;
			Face_Index_Set++;
		}
	}

	return 1;
}

// *************************************************************************
// *		  Set_Face_Index:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Brush_X::Set_Face_Index(int Index, Face* f)
{
	int m_Index = Index+1;

	f->Real_Brush_Face_Index = m_Index;
	
	return 1;
}

// *************************************************************************
// *		  Move_Player_Brush:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Brush_X::Move_Player_Brush()
{
	Brush* b = NULL;
	b = App->CL_Brush_X->Get_Brush_By_Name("Player_Main");
	if (b)
	{
		T_Vec3 Pos;
		Pos.x = App->CL_Scene->B_Player[0]->StartPos.x;
		Pos.y = App->CL_Scene->B_Player[0]->StartPos.y;
		Pos.z = App->CL_Scene->B_Player[0]->StartPos.z;

		App->CL_Brush->Brush_Move(b, &Pos);

	}
}

// *************************************************************************
// *		  Move_Brush_By_Name:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Brush_X::Move_Brush_By_Name(char* Brush_Name, int Object_Index)
{
	Brush* b = App->CL_Brush_X->Get_Brush_By_Name(Brush_Name);
	if (!b) return; // No Nrush

	App->CL_Brush->Brush_Get_Center(b, &App->CL_Doc->SelectedGeoCenter);

	T_Vec3 Pos;
	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Object_Index]->Object_Ent->getWorldBoundingBox(true).getCenter();

	Pos.x = Centre.x;
	Pos.y = Centre.y;
	Pos.z = Centre.z;

	App->CL_Maths->Vector3_Subtract(&Pos, &App->CL_Doc->SelectedGeoCenter, &Pos);

	App->CL_Brush->Brush_Move(b, &Pos);

}

// *************************************************************************
// *		  Scale_Brush_By_Name:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Brush_X::Scale_Brush_By_Name(const char* Brush_Name, int Object_Index, float SX, float SY, float SZ)
{
	Brush* b = App->CL_Brush_X->Get_Brush_By_Name(Brush_Name);
	if (!b) return; // No Nrush

	T_Vec3 mFinalScale;
	float mSizeX = fabs(b->BoundingBox.Max.x - b->BoundingBox.Min.x);
	float mSizeY = fabs(b->BoundingBox.Max.y - b->BoundingBox.Min.y);
	float mSizeZ = fabs(b->BoundingBox.Max.z - b->BoundingBox.Min.z);

	// Convert to a Scale
	mFinalScale.x = SX / mSizeX;
	mFinalScale.y = SY / mSizeY;
	mFinalScale.z = SZ / mSizeZ;

	T_Vec3 mSelectedGeoCenter = { 0, 0, 0 };
	T_Vec3 VecOrigin = { 0.0f, 0.0f, 0.0f };
	T_Vec3 MoveTo, MoveBack;

	// Calculate movement vectors
	App->CL_Maths->Vector3_Subtract(&VecOrigin, &mSelectedGeoCenter, &MoveTo);
	App->CL_Maths->Vector3_Subtract(&mSelectedGeoCenter, &VecOrigin, &MoveBack);
	App->CL_Maths->Vector3_Subtract(&mFinalScale, &mSelectedGeoCenter, &mFinalScale);

	// Move and scale the brush
	App->CL_Brush->Brush_Move(b, &MoveTo);
	App->CL_Brush->Brush_Scale3d(b, &mFinalScale);

	if (App->CL_Brush->Brush_IsMulti(b))
	{
		// App->CL_Brush->BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(b), App->CL_Brush->Brush_GetModelId(b));
		// App->CL_Brush->BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(b), App->CL_Brush->Brush_GetModelId(b), ::fdocBrushCSGCallback, this);
	}

	App->CL_Brush_X->Move_Brush_By_Name(App->CL_Scene->B_Object[Object_Index]->Object_Name, Object_Index);
}

// *************************************************************************
// *		Rotate_Reset_Brush_By_Name:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Brush_X::Rotate_Reset_Brush_By_Name(const char* Brush_Name, float SX, float SY, float SZ)
{
	Brush* b = App->CL_Brush_X->Get_Brush_By_Name(Brush_Name);
	if (b)
	{
		Matrix3d rm;
		T_Vec3 RotationPoint;
		T_Vec3 FinalRot = { 0,0,0 };

		float rotationAngleX = Units_DegreesToRadians(SX);
		float rotationAngleY = Units_DegreesToRadians(SY);
		float rotationAngleZ = Units_DegreesToRadians(SZ);

		const int maxCount = 3;

		for (int Count = 0; Count < maxCount; ++Count)
		{
			switch (Count) 
			{
			case 0:
				FinalRot = { rotationAngleX, 0, 0 };
				break;
			case 1:
				FinalRot = { 0, rotationAngleY, 0 };
				break;
			case 2:
				FinalRot = { 0, 0, rotationAngleZ };
				break;
			}

			App->CL_Brush->Brush_Get_Center(b, &RotationPoint);

			App->CL_Maths->XForm3d_SetIdentity(&rm);

			App->CL_Maths->XForm3d_SetEulerAngles(&rm, &FinalRot);

			App->CL_Brush->Brush_Rotate(b, &rm, &RotationPoint);
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		}
	}
}

// *************************************************************************
// *		  Rotate_Brush_By_Name:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Brush_X::Rotate_Brush_By_Name(const char* Brush_Name, float SX, float SY, float SZ)
{
	Brush* b = App->CL_Brush_X->Get_Brush_By_Name(Brush_Name);
	if (b)
	{
		T_Vec3 FinalRot = { 0,0,0 };

		FinalRot.x = Units_DegreesToRadians(SX);
		FinalRot.y = Units_DegreesToRadians(SY);
		FinalRot.z = Units_DegreesToRadians(SZ);

		Matrix3d rm;
		T_Vec3 RotationPoint;

		App->CL_Brush->Brush_Get_Center(b, &RotationPoint);

		App->CL_Maths->XForm3d_SetIdentity(&rm);

		App->CL_Maths->XForm3d_SetEulerAngles(&rm, &FinalRot);

		App->CL_Brush->Brush_Rotate(b, &rm, &RotationPoint);

		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

		b->Last_Rotation = { SX ,SY ,SZ };

	}
}

// *************************************************************************
// *		Set_Brush_Face_Points:- Terry and Hazel Flanigan 2025	       *
// *************************************************************************
void CL64_Brush_X::Set_Brush_Face_Points(Brush* pBrush, bool Update) 
{
	// 0=NBL 1=NBR 2=NTR 3=NTL 4=FBL 5=FBR 6=FTR 7=FTL
	// Anticlockwise
	
	// Array of face indices corresponding to the brush faces
	const int faceIndices[6][4] = 
	{
		{3, 2, 1, 0}, // Top
		{0, 1, 5, 4}, // Bottom
		{3, 0, 1, 2}, // Front
		{7, 4, 5, 6}, // Back
		{7, 4, 0, 3}, // Left
		{2, 1, 5, 6}  // Right
	};

	// Loop through each face and set the points
	for (int i = 0; i < 6; ++i) 
	{
		Face* pFace = App->CL_Brush->Brush_GetFace(pBrush, i);

		for (int j = 0; j < 4; ++j) 
		{
			pFace->Points[j] = App->CL_Entities->Verts[faceIndices[i][j]];
		}
	}

	// Update views if required
	if (Update) 
	{
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
	}
}

// *************************************************************************
// *	Set_Brush_From_Entity_ByName:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Brush_X::Set_Brush_From_Entity_ByName(char* Name, bool Update)
{
	int Index = App->CL_Entities->GetIndex_By_Name(Name);

	if (Index > -1)
	{
		auto& m_object = App->CL_Scene->B_Object[Index];

		Brush* pBrush = NULL;

		pBrush = Get_Brush_By_Name(Name);

		if (pBrush)
		{
			App->CL_Entities->Ogre_To_Mesh_Data(m_object->Object_Node);
			Set_Brush_Face_Points(pBrush, Update);
		}
	}

}



