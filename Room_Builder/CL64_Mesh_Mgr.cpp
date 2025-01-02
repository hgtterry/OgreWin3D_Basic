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
#include "CL64_Mesh_Mgr.h"
#include "CL64_WadFile.h"

CL64_Mesh_Mgr::CL64_Mesh_Mgr()
{
}

CL64_Mesh_Mgr::~CL64_Mesh_Mgr()
{
}

// *************************************************************************
// * 		Delete_Brush_List:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Mesh_Mgr::Delete_Brush_List()
{
	CL64_Model* pModel = App->CL_Model;

	//int Count = 0;
	//while (Count < 11999)
	//{
	//	if (pModel->B_Brush[Count] != nullptr)
	//	{
	//		delete pModel->B_Brush[Count];
	//		//Debug
	//	}

	//	pModel->B_Brush[Count] = nullptr;

	//	Count++;
	//}

	//App->CLSB_Model->BrushCount = 0;
	//App->CLSB_Model->Brush_Face_Count = 0;
	//Global_Faces_Index = 0;

}

// *************************************************************************
// *		Update_World:- Terry and Hazel Flanigan 2025	 			   *
// *************************************************************************
bool CL64_Mesh_Mgr::Update_World()
{

	WE_Build_Brush_List(0);
	WE_Convert_All_Texture_Groups();

	//App->CLSB_Ogre3D->Convert_ToOgre3D(1);

	return 1;
}

// *************************************************************************
// *	WE_Convert_All_Texture_Groups:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
bool CL64_Mesh_Mgr::WE_Convert_All_Texture_Groups()
{
	/*int mTotalVertices = 0;
	ActualFaceCount = 0;

	Delete_Group_Brushes();

	strcpy(App->CLSB_Model->JustName, "Test");

	App->CLSB_Model->Set_Groupt_Count(mTextureCount);*/

	int Count = 0;
	/*while (Count < mTextureCount)
	{
		App->CLSB_Model->Create_Mesh_Group(Count);
		int FaceCount = WE_Get_Vertice_Count(Count);

		strcpy(App->CLSB_Model->Group[Count]->GroupName, TextureName2[Count]);
		strcpy(App->CLSB_Model->Group[Count]->MaterialName, TextureName2[Count]);

		char buff[MAX_PATH];
		strcpy(buff, TextureName2[Count]);
		if (IsTextureAlpha[Count] == 1)
		{
			strcat(buff, ".tga");
		}
		else
		{
			strcat(buff, ".bmp");
		}

		strcpy(App->CLSB_Model->Group[Count]->Text_FileName, buff);

		App->CLSB_Model->Group[Count]->MaterialIndex = Count;
		App->CLSB_Model->Group[Count]->vertex_Data.resize(FaceCount * 3);
		App->CLSB_Model->Group[Count]->Normal_Data.resize(FaceCount * 3);
		App->CLSB_Model->Group[Count]->MapCord_Data.resize(FaceCount * 3);
		App->CLSB_Model->Group[Count]->Face_Data.resize(FaceCount);
		App->CLSB_Model->Group[Count]->FaceIndex_Data.resize(FaceCount * 3);

		WE_Convert_To_Texture_Group(Count);

		mTotalVertices = mTotalVertices + FaceCount;

		Count++;
	}

	App->CLSB_Model->VerticeCount = mTotalVertices * 3;
	App->CLSB_Model->FaceCount = mTotalVertices;

	App->CLSB_Model->Render_Type = Enums::LoadedFile_Assimp;*/
	//Update_Brush_List(Mesh_Viewer_HWND);

	return true;
}

// *************************************************************************
// * 		WE_Build_Brush_List:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_Mesh_Mgr::WE_Build_Brush_List(int ExpSelected)
{
	Delete_Brush_List();

	//App->CLSB_Model->BrushCount = 0;

	//mBrushCount = 0;
	//mSubBrushCount = 0;

	//char Path[MAX_PATH];
	//strcpy(Path, App->WorldEditor_Directory);
	//strcat(Path, "Data\\3DSTemp.txt");

	BrushList* BList;
	signed int fResult;

	BList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

	if (!ExpSelected)	// Build All
	{
		fResult = WE_Level_Build_Brushes(reinterpret_cast<tag_Level*> (App->CL_Doc->pLevel), "FileName", BList, 0, 0, -1);
	}
	//else				// Build Selected
	//{
	//	int i, GroupID, GroupCount;
	//	char NewFileName[MAX_PATH];
	//	GroupID = -1;
	//	GroupCount = 1;

	//	for (i = 0; i < GroupCount; i++)
	//	{
	//		BrushList* SBList;
	//		Brush* pBrush;
	//		BrushIterator bi;

	//		SBList = BrushList_Create();
	//		pBrush = BrushList_GetFirst(BList, &bi);

	//		while (pBrush != NULL)
	//		{

	//			if (SelBrushList_Find(App->CLSB_Doc->pSelBrushes, pBrush))
	//			{
	//				Brush* pClone = Brush_Clone(pBrush);
	//				BrushList_Append(SBList, pClone);
	//			}

	//			pBrush = BrushList_GetNext(&bi);
	//		}
	//		// do CSG
	//		{
	//			ModelIterator	mi;
	//			int				i, CurId = 0;
	//			ModelInfo_Type* ModelInfo;
	//			Model* pMod;

	//			BrushList_ClearAllCSG(SBList);

	//			BrushList_DoCSG(SBList, CurId, Brush_CSG_Callback, this);

	//			//build individual model mini trees
	//			ModelInfo = Level_GetModelInfo(App->CLSB_Doc->pLevel);
	//			pMod = ModelList_GetFirst(ModelInfo->Models, &mi);

	//			for (i = 0; i < ModelList_GetCount(ModelInfo->Models); i++)
	//			{
	//				CurId = Model_GetId(pMod);

	//				BrushList_DoCSG(SBList, CurId, Brush_CSG_Callback, this);
	//			}
	//		}

	//		fResult = WE_Level_Build_Brushes(reinterpret_cast<tag_Level3*> (App->CLSB_Doc->pLevel), NewFileName, SBList, 0, 0, -1);
	//		if (!fResult)
	//		{
	//			App->Say("Error exporting group");
	//		}

	//		BrushList_Destroy(&SBList);
	//	}
	//}

	//App->Say("Converted NEW");
}

// *************************************************************************
// *		WE_Level_Build_Brushes:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool CL64_Mesh_Mgr::WE_Level_Build_Brushes(Level* pLevel, const char* Filename, BrushList* BList, int ExpSelected, geBoolean ExpLights, int GroupID)
{
	/*mTextureCount = 0;
	memset(mAdjusedIndex_Store, 0, 500);
	Actual_Brush_Index = 0;*/

	int i;
	geBoolean* WrittenTex;

	//WrittenTex = (geBoolean*)calloc(sizeof(geBoolean), pLevel->WadFile->mBitmapCount);

	// which textures are used?
	//BrushList_GetUsedTextures(BList, WrittenTex, pLevel->WadFile);

	// Add Textures GL
	int AdjustedIndex = 0;
	//for (i = 0; i < pLevel->WadFile->mBitmapCount; i++)
	//{
	//	if (WrittenTex[i])
	//	{
	//		char matname[MAX_PATH];
	//		int j, k;
	//		strncpy(matname, pLevel->WadFile->mBitmaps[i].Name, MAX_PATH - 1);

	//		/*strcpy(TextureName2[AdjustedIndex], matname);

	//		if (geBitmap_HasAlpha(pLevel->WadFile->mBitmaps[i].bmp))
	//		{
	//			IsTextureAlpha[AdjustedIndex] = 1;
	//		}
	//		else
	//		{
	//			IsTextureAlpha[AdjustedIndex] = 0;
	//		}

	//		mAdjusedIndex_Store[AdjustedIndex] = i;

	//		AddTexture_GL(NULL, matname, AdjustedIndex);*/

	//		AdjustedIndex++;
	//	}
	//}

	//mTextureCount = AdjustedIndex;


	WE_BrushList_Decode(BList, GE_FALSE);

	free(WrittenTex);

	return 1;
}

// *************************************************************************
// *			WE_BrushList_Decode:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Mesh_Mgr::WE_BrushList_Decode(BrushList* BList, geBoolean SubBrush)
{
	Brush* pBrush;
	BrushIterator bi;

	//pBrush = BrushList_GetFirst(BList, &bi);



	while (pBrush != NULL)
	{
		/*if (mSubBrushCount == 0 && pBrush->Flags & 1 || pBrush->Flags & 1024)
		{
			if (SubBrush == 0)
			{
				strcpy(mBrush_Name, pBrush->Name);
				mBrush_Index = mBrushCount;
			}
		}*/

		/*if (!WE_Brush_Create(pBrush, Actual_Brush_Index))
		{
			return GE_FALSE;
		}*/



		/*pBrush = BrushList_GetNext(&bi);

		if (SubBrush)
		{
			mSubBrushCount++;
		}
		else
		{
			mBrushCount++;
			Actual_Brush_Index++;
		}*/


	}

	/*mSubBrushCount = 0;

	if (!SubBrush)
	{
		mBrushCount = 0;
	}*/

	return GE_TRUE;
}
