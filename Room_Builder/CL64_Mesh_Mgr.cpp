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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Mesh_Mgr.h"
#include "CL64_WadFile.h"
#include "Structures.cpp"

CL64_Mesh_Mgr::CL64_Mesh_Mgr()
{
	World_Node = nullptr;
	World_Ent = nullptr;

	m_Total_Faces = 0;
	mBrushCount = 0;
	mSubBrushCount = 0;
	mTextureCount = 0;
	ActualFaceCount = 0;
	m_Main_Brush_Name[0] = 0;

	Mesh_Viewer_HWND = nullptr;

	memset(mAdjusedIndex_Store, 0, 500);

	flag_Mesh_Viewer_Active = 0;
	flag_Select_Brush = 0;

	Ogre_List_Index = 0;
	Groups_List_Index = 0;
	Brushes_List_Index = 0;
	
	Selected_Render_Mode = 0;
	v_Face_Data_Count = 0;
	
	int Count = 0;
	while (Count < 999)
	{
		v_Face_Data[Count] = nullptr;
		Count++;
	}

}

CL64_Mesh_Mgr::~CL64_Mesh_Mgr()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Mesh_Mgr::Reset_Class()
{
	App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Ogre;
	App->CL_Camera->Camera_Textured();

	if (flag_Mesh_Viewer_Active == 1)
	{
		flag_Mesh_Viewer_Active = 0;
		EndDialog(Mesh_Viewer_HWND, LOWORD(0));
	}

	Ogre_List_Index = 0;
	Groups_List_Index = 0;
	Brushes_List_Index = 0;
}

// *************************************************************************
// * 		Brush_CSG_Callback:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
signed int CL64_Mesh_Mgr::Brush_CSG_Callback(const Brush* pBrush, void* lParam)
{
	return 1;// (App->CL_Doc->BrushIsVisible(pBrush));
}

// *************************************************************************
// * 		Delete_Brush_List:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Mesh_Mgr::Delete_Brush_List()
{
	CL64_Editor_Com* pModel = App->CL_Editor_Com;

	int Count = 0;
	while (Count < 11999)
	{
		if (pModel->B_Brush[Count] != nullptr)
		{
			delete pModel->B_Brush[Count];
		}

		pModel->B_Brush[Count] = nullptr;

		Count++;
	}

	App->CL_Editor_Com->BrushCount = 0;
	App->CL_Editor_Com->Brush_Face_Count = 0;
	m_Total_Faces = 0;
}

// *************************************************************************
// * 		Delete_Group_Brushes:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Mesh_Mgr::Delete_Group_Brushes()
{
	CL64_Editor_Com* pModel = App->CL_Editor_Com;

	int Count = 0;
	while (Count < 4999)
	{
		if (pModel->Group[Count] != nullptr)
		{
			delete pModel->Group[Count];
		}

		pModel->Group[Count] = nullptr;

		Count++;
	}

	App->CL_Editor_Com->GroupCount = 0;
	App->CL_Editor_Com->VerticeCount = 0;
	
}

// *************************************************************************
// *		Update_World:- Terry and Hazel Flanigan 2025	 			   *
// *************************************************************************
bool CL64_Mesh_Mgr::Update_World(int selected)
{
	v_Face_Data_Count = 0;

	int brushCount = App->CL_Brush->Get_Brush_Count();
	if (brushCount > 0)
	{
		Brush_Build_List(selected);
		WE_Convert_All_Texture_Groups();

		if (App->CL_Ogre->OGL_Listener->flag_Render_Groups == 0)
		{
			App->CL_Ogre3D->Convert_ToOgre3D(1); // Will Set Node Visible
		}
	}
	else
	{
		if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
		{
			App->CL_Mesh_Mgr->World_Node->setVisible(false);
		}
	}

	return true;
}

// *************************************************************************
// * 		Brush_Build_List:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Mesh_Mgr::Brush_Build_List(int ExpSelected)
{
	Delete_Brush_List();

	App->CL_Editor_Com->BrushCount = 0;
	mBrushCount = 0;
	mSubBrushCount = 0;

	BrushList* BList = App->CL_Level->Level_Get_Main_Brushes();
	if (!BList)
	{
		App->Say("Error: No brushes found.");
		return;
	}

	if (ExpSelected == 0) // Build All
	{
		signed int fResult = Brush_Build_Level_Brushes(reinterpret_cast<tag_Level3*>(App->CL_Doc->Current_Level), "FileName", BList, 0, 0, -1);

	}
	else
	{
		Brush_Build_Selected(BList);
	}
}

// *************************************************************************
// *		Brush_Build_Selected:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_Build_Selected(BrushList* BList)
{
	signed int fResult;
	int i, GroupID, GroupCount;
	char NewFileName[MAX_PATH];
	GroupID = -1;
	GroupCount = 1;

	for (i = 0; i < GroupCount; i++)
	{
		BrushList* SBList;
		Brush* pBrush;
		BrushIterator bi;

		SBList = App->CL_Brush->BrushList_Create();
		pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);

		while (pBrush != NULL)
		{

			if (App->CL_SelBrushList->SelBrushList_Find(App->CL_Doc->pSelBrushes, pBrush))
			{
				Brush* pClone = App->CL_Brush->Brush_Clone(pBrush);
				App->CL_Brush->BrushList_Append(SBList, pClone);
			}

			pBrush = App->CL_Brush->BrushList_GetNext(&bi);
		}

		bool Do_CSG = 1;
		if (Do_CSG == 1)
		{
			int CurId = 0;
			
			App->CL_Brush->BrushList_ClearAllCSG(SBList);

			App->CL_Brush->BrushList_DoCSG(SBList, CurId, Brush_CSG_Callback, this);

		}

		fResult = Brush_Build_Level_Brushes(reinterpret_cast<tag_Level3*>(App->CL_Doc->Current_Level), NewFileName, SBList, 0, 0, -1);
		if (!fResult)
		{
			App->Say("Error exporting group");
		}

		App->CL_Brush->BrushList_Destroy(&SBList);
	}

	return 1;
}

// *************************************************************************
// *		Brush_Build_Level_Brushes:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_Build_Level_Brushes(Level3* pLevel, const char* Filename, BrushList* BList, int ExpSelected, signed int ExpLights, int GroupID)
{
	CL64_WadFile* pWad = App->CL_Level->Level_GetWad_Class();
	if (!pWad) {
		App->Say("Failed to retrieve WAD Class.");
		return false;
	}

	int ml_BitMap_Count = pWad->mBitmapCount;
	mTextureCount = 0;
	memset(mAdjusedIndex_Store, 0, sizeof(mAdjusedIndex_Store));
	memset(UsedTextures, 0, sizeof(UsedTextures));
	
	App->CL_Brush_X->BrushList_GetUsedTextures_X(BList, UsedTextures);

	// Add Textures GL
	int AdjustedIndex = 0;

	for (int i = 0; i < ml_BitMap_Count; i++)
	{
		if (UsedTextures[i])
		{
			strncpy(TextureName2[AdjustedIndex], pWad->mBitmaps[i].Name, MAX_PATH - 1);
			TextureName2[AdjustedIndex][MAX_PATH - 1] = '\0'; // Ensure null-termination
			mAdjusedIndex_Store[AdjustedIndex] = i;

			if (App->CL_Ogre->OGL_Listener->flag_Render_Groups == 0)
			{
				// AddTexture_GL(NULL, TextureName2[AdjustedIndex], AdjustedIndex);
			}

			AdjustedIndex++;
		}
	}

	mTextureCount = AdjustedIndex;

	if (mTextureCount == 0)
	{
		App->Say("No Textures in Brush");
	}

	Brush_Decode_List(BList, false);
	return true;
}

// *************************************************************************
// *			Brush_Decode_List:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_Decode_List(BrushList* BList, signed int SubBrush)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);

	while (pBrush != nullptr)
	{
		if (pBrush->GroupId != 1)
		{
			// Get Main Brush Name not sub brushes 
			strcpy(m_Main_Brush_Name, pBrush->Name);

			bool isFirstBrush = (mSubBrushCount == 0 && (pBrush->Flags & 1)) || (pBrush->Flags & 1024);
			if (isFirstBrush && SubBrush == 0)
			{
				
			}

			if (!Brush_Create(pBrush))
			{
				return false;
			}

			if (SubBrush)
			{
				mSubBrushCount++;
			}
			else
			{
				mBrushCount++;
				strcpy(Actual_mBrush_Name, pBrush->Name);
			}
		}

		//App->Say_Int(pBrush->Faces->NumFaces);
		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}

	mSubBrushCount = 0;

	if (SubBrush == 0)
	{
		mBrushCount = 0;
	}

	return true;
}

// *************************************************************************
// *			Brush_Create:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_Create(const Brush* b)
{
	if (!b) return false; // Check for null pointer
	
	switch (b->Type)
	{
	case BRUSH_MULTI:
		return Brush_Decode_List(b->BList, true); // Recursive

	case BRUSH_LEAF:
		return HandleLeafBrush(b);

	case BRUSH_CSG:
		return HandleCSGBrush(b);

	default:
		return false;
	}
}

// *************************************************************************
// *			HandleLeafBrush:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Mesh_Mgr::HandleLeafBrush(const Brush* b)
{
	if (b->BList)
	{
		return Brush_Decode_List(b->BList, true); // Recursive
	}
	else if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
	{
		return Brush_FaceList_Create(b, b->Faces, mBrushCount, mSubBrushCount);
	}
	else if ((b->Flags & BRUSH_SUBTRACT) && !(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT)))
	{
		mBrushCount--;
	}
	return true;
}

// *************************************************************************
// *			HandleCSGBrush:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Mesh_Mgr::HandleCSGBrush(const Brush* b)
{
	if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
	{
		return Brush_FaceList_Create(b, b->Faces, mBrushCount, mSubBrushCount);
	}

	return true;
}

// *************************************************************************
// *		Brush_FaceList_Create:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_FaceList_Create(const Brush* b, const FaceList* pList, int BrushCount, int SubBrushCount)
{
	App->CL_Editor_Com->Create_Brush_XX(App->CL_Editor_Com->BrushCount);
	strcpy(App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Brush_Name, m_Main_Brush_Name);

	// Get Brush Index
	int Main_Brush_Index = App->CL_Brush_X->Get_Brush_Index_By_Name(m_Main_Brush_Name);
	if (Main_Brush_Index == -1)
	{
		App->Say("Can not find Brush");
	}

	App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Group_Index = Main_Brush_Index;


	m_Total_Faces = m_Total_Faces + pList->NumFaces;

	int i, j, k, num_faces, num_verts, num_mats, num_chars, curnum_verts;
	char matname[MAX_PATH];

	std::vector<bool> matf(pList->NumFaces, false);

	num_faces = num_verts = num_mats = num_chars = 0;
	// get the total number of verts, faces and materials of the object

	for (i = 0; i < pList->NumFaces; i++)
	{

		curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[i]);
		num_faces += (curnum_verts - 2);
		num_verts += curnum_verts;

		if (!matf[i])
		{
			matf[i] = 1;
			num_mats++;

			for (j = i + 1; j < pList->NumFaces; j++)
			{
				if (strcmp(App->CL_Face->Face_GetTextureName(pList->Faces[i]), App->CL_Face->Face_GetTextureName(pList->Faces[j])) == 0)
					matf[j] = 1;
			}

			strncpy(matname, App->CL_Face->Face_GetTextureName(pList->Faces[i]), MAX_PATH);
		}
	}

	for (i = 0; i < pList->NumFaces; i++)
	{
		matf[i] = false;
	}

	// -----------------------------------  Vertices
	int VertIndex = 0;

	App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Vertice_Count = num_verts;
	App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->vertex_Data.resize(num_verts);
	App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Normal_Data.resize(num_verts);

	for (i = 0; i < pList->NumFaces; i++)
	{
		
		const T_Vec3* verts;
		verts = App->CL_Face->Face_GetPoints(pList->Faces[i]);
		curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[i]);
		for (j = 0; j < curnum_verts; j++)
		{
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->vertex_Data[VertIndex].x = verts[j].x;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->vertex_Data[VertIndex].y = verts[j].y;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->vertex_Data[VertIndex].z = verts[j].z;

			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Normal_Data[VertIndex].x = 0.5;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Normal_Data[VertIndex].y = 0.5;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Normal_Data[VertIndex].z = 0.5;

			VertIndex++;
		}

	}

	int UVIndex = 0;
	// -----------------------------------  UVS
	App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->MapCord_Data.resize(num_verts);
	for (i = 0; i < pList->NumFaces; i++)
	{
		const TexInfo_Vectors* TVecs = App->CL_Face->Face_GetTextureVecs(pList->Faces[i]); // hgtterry needs finishing
		const T_Vec3* verts;
		T_Vec3 uVec, vVec;
		float U, V;

		int txSize, tySize;

		App->CL_Face->Face_GetTextureSize(pList->Faces[i], &txSize, &tySize);

		// make sure that the texture size is set correctly (division!)
		if (txSize == 0)
		{
			txSize = 128;
		}

		if (tySize == 0)
		{
			tySize = 128;
		}

		App->CL_Maths->Vector3_Scale(&TVecs->uVec, 1.f / (float)txSize, &uVec);
		App->CL_Maths->Vector3_Scale(&TVecs->vVec, -1.f / (float)tySize, &vVec);

		verts = App->CL_Face->Face_GetPoints(pList->Faces[i]);
		curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[i]);

		for (j = 0; j < curnum_verts; j++)
		{
			U = App->CL_Maths->Vector3_DotProduct(&(verts[j]), &uVec);
			V = App->CL_Maths->Vector3_DotProduct(&(verts[j]), &vVec);
			U += (TVecs->uOffset / txSize);
			V -= (TVecs->vOffset / tySize);

			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->MapCord_Data[UVIndex].u = U;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->MapCord_Data[UVIndex].v = V;
			UVIndex++;
		}
	}

	// -----------------------------------  Faces
	int FaceIndex = 0;
	App->CL_Editor_Com->Brush_Face_Count = App->CL_Editor_Com->Brush_Face_Count + num_faces;
	App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Face_Count = num_faces;
	App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Face_Data.resize(num_faces);
	App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Picking_Data.resize(num_faces);
	num_verts = 0;
	for (i = 0; i < pList->NumFaces; i++)
	{
		curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[i]); //4

		for (j = 0; j < curnum_verts - 2; j++)
		{
			Create_V_Face(v_Face_Data_Count);
			strcpy(v_Face_Data[v_Face_Data_Count]->Brush_Name, App->CL_Face->Face_GetBrushName(pList->Faces[i]));

			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Face_Data[FaceIndex].a = num_verts;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Face_Data[FaceIndex].b = num_verts + 2 + j;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Face_Data[FaceIndex].c = num_verts + 1 + j;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Face_Data[FaceIndex].Brush_Index = Main_Brush_Index;
			App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Face_Data[FaceIndex].Main_Face = pList->Faces[i]->Real_Brush_Face_Index;

			FaceIndex++;
			v_Face_Data_Count++;
		}

		num_verts += curnum_verts;
	}

	// -----------------------------------  Texture IDs
	int TextureId = 0;

	for (i = 0; i < pList->NumFaces; i++)
	{
		if (!matf[i])
		{
			matf[i] = true;

			int curnum_faces = (App->CL_Face->Face_GetNumPoints(pList->Faces[i]) - 2);

			for (j = i + 1; j < pList->NumFaces; j++)
			{
				if (strcmp(App->CL_Face->Face_GetTextureName(pList->Faces[i]), App->CL_Face->Face_GetTextureName(pList->Faces[j])) == 0)
				{
					curnum_faces += (App->CL_Face->Face_GetNumPoints(pList->Faces[j]) - 2);
				}
			}

			strncpy(matname, App->CL_Face->Face_GetTextureName(pList->Faces[i]), 11);

			// Material Name
			int DibId = Get_Adjusted_Index(App->CL_Face->Face_GetTextureDibId(pList->Faces[i]));

			// write face numbers
			curnum_faces = 0;
			for (j = 0; j < i; j++)
			{
				curnum_faces += (App->CL_Face->Face_GetNumPoints(pList->Faces[j]) - 2);
			}

			curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[i]);
			for (j = 0; j < curnum_verts - 2; j++)
			{
				TextureId = Get_Adjusted_Index(App->CL_Face->Face_GetTextureDibId(pList->Faces[i]));
				App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Picking_Data[curnum_faces + j].TextID = TextureId;

			}

			curnum_faces += (curnum_verts - 2);

			for (j = i + 1; j < pList->NumFaces; j++)
			{
				curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[j]);
				if (strcmp(App->CL_Face->Face_GetTextureName(pList->Faces[i]), App->CL_Face->Face_GetTextureName(pList->Faces[j])) == 0)
				{
					matf[j] = true;
					for (k = 0; k < curnum_verts - 2; k++)
					{
						TextureId = Get_Adjusted_Index(App->CL_Face->Face_GetTextureDibId(pList->Faces[i]));
						App->CL_Editor_Com->B_Brush[App->CL_Editor_Com->BrushCount]->Picking_Data[curnum_faces + k].TextID = TextureId;

					}
				}

				curnum_faces += (curnum_verts - 2);
			}
		}
	}

	App->CL_Editor_Com->BrushCount++;

	return true;
}

// *************************************************************************
// *			Create_V_Face:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
void CL64_Mesh_Mgr::Create_V_Face(int Index)
{
	if (v_Face_Data[Index] != nullptr)
	{
		delete v_Face_Data[Index];
		v_Face_Data[Index] = nullptr;
	}

	v_Face_Data[Index] = new Face_Data;

	strcpy(v_Face_Data[Index]->Brush_Name,"No_Brush");
}

// *************************************************************************
// *	WE_Convert_All_Texture_Groups:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
bool CL64_Mesh_Mgr::WE_Convert_All_Texture_Groups() {
	int totalVertices = 0;
	ActualFaceCount = 0;
	int Group_Count = 0;
	Delete_Group_Brushes();
	strcpy(App->CL_Editor_Com->JustName, "Test");
	//App->CL_Editor_Com->GroupCount = mTextureCount;
	//Debug
	int count = 0;

	while (count < mTextureCount)
	{
		int64_t faceCount = WE_Get_Vertice_Count(count);

		//if (faceCount > 0)
		{
			//if (faceCount > 0)
			{
				App->CL_Editor_Com->Create_Mesh_Group(Group_Count);
			}
			
			// Get Group
			if (faceCount > 0)
			{
				auto& group = App->CL_Editor_Com->Group[Group_Count];

				strcpy(group->GroupName, TextureName2[Group_Count]);
				strcpy(group->MaterialName, TextureName2[Group_Count]);

				int trueIndex = App->CL_TXL_Editor->GetIndex_From_Name(TextureName2[Group_Count]);
				strcpy(group->Text_FileName, App->CL_TXL_Editor->Texture_List[trueIndex]->FileName);

				group->Has_Alpha = App->CL_TXL_Editor->Texture_List[trueIndex]->Has_Alpha;
				group->MaterialIndex = Group_Count;
				group->vertex_Data.resize(faceCount * 3);
				group->Normal_Data.resize(faceCount * 3);
				group->MapCord_Data.resize(faceCount * 3);
				group->Face_Data.resize(faceCount);
				group->FaceIndex_Data.resize(faceCount * 3);

				WE_Convert_To_Texture_Group(Group_Count);
				totalVertices += faceCount;
			}

			//if (faceCount > 0)
			{
				Group_Count++;
			}
		}

		count++;
	}
	
	bool Get_Call = 0;
	if (Get_Call == 1)
	{
		App->Say("Rebuild Called");
	}

	App->CL_Editor_Com->GroupCount = Group_Count;
	App->CL_Editor_Com->VerticeCount = totalVertices * 3;
	App->CL_Editor_Com->FaceCount = totalVertices;

	return true;
}

// *************************************************************************
// *	WE_Convert_To_Texture_Group:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
bool CL64_Mesh_Mgr::WE_Convert_To_Texture_Group(int TextureID)
{
	int Count = 0;
	int FaceCount = 0;
	int vertexIndex = 0;
	int FaceIndex = 0;
	int FacePos = 0;

	int A = 0;
	int B = 0;
	int C = 0;

	float X = 0;
	float Y = 0;
	float Z = 0;

	float U = 0;
	float V = 0;

	int BrushCount = App->CL_Editor_Com->BrushCount;
	Count = 0;
	

	while (Count < BrushCount)
	{
		FaceCount = 0;
		if (!App->CL_Editor_Com->B_Brush[Count])
		{
			App->Say("Brush Error");
		}

		while (FaceCount < App->CL_Editor_Com->B_Brush[Count]->Face_Count)
		{
			int FaceIndexNum = 0;

			if (App->CL_Editor_Com->B_Brush[Count]->Picking_Data[FaceCount].TextID == TextureID)
			{
				A = App->CL_Editor_Com->B_Brush[Count]->Face_Data[FaceCount].a;
				B = App->CL_Editor_Com->B_Brush[Count]->Face_Data[FaceCount].b;
				C = App->CL_Editor_Com->B_Brush[Count]->Face_Data[FaceCount].c;

				X = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[A].x;
				Y = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[A].y;
				Z = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[A].z;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].x = X;
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].y = Y;
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].z = Z;
				}

				U = App->CL_Editor_Com->B_Brush[Count]->MapCord_Data[A].u,
				V = App->CL_Editor_Com->B_Brush[Count]->MapCord_Data[A].v;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->MapCord_Data[vertexIndex].u = U;
					App->CL_Editor_Com->Group[TextureID]->MapCord_Data[vertexIndex].v = V;
				}
				

				X = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[A].x;
				Y = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[A].y;
				Z = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[A].z;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].x = X;
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].y = Y;
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].z = Z;
				}

				vertexIndex++;

				X = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[B].x;
				Y = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[B].y;
				Z = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[B].z;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].x = X;
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].y = Y;
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].z = Z;
				}

				U = App->CL_Editor_Com->B_Brush[Count]->MapCord_Data[B].u,
				V = App->CL_Editor_Com->B_Brush[Count]->MapCord_Data[B].v;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->MapCord_Data[vertexIndex].u = U;
					App->CL_Editor_Com->Group[TextureID]->MapCord_Data[vertexIndex].v = V;
				}

				X = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[B].x;
				Y = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[B].y;
				Z = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[B].z;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].x = X;
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].y = Y;
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].z = Z;
				}

				vertexIndex++;

				X = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[C].x;
				Y = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[C].y;
				Z = App->CL_Editor_Com->B_Brush[Count]->vertex_Data[C].z;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].x = X;
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].y = Y;
					App->CL_Editor_Com->Group[TextureID]->vertex_Data[vertexIndex].z = Z;
				}

				U = App->CL_Editor_Com->B_Brush[Count]->MapCord_Data[C].u,
				V = App->CL_Editor_Com->B_Brush[Count]->MapCord_Data[C].v;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->MapCord_Data[vertexIndex].u = U;
					App->CL_Editor_Com->Group[TextureID]->MapCord_Data[vertexIndex].v = V;
				}

				X = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[C].x;
				Y = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[C].y;
				Z = App->CL_Editor_Com->B_Brush[Count]->Normal_Data[C].z;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].x = X;
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].y = Y;
					App->CL_Editor_Com->Group[TextureID]->Normal_Data[vertexIndex].z = Z;
				}

				vertexIndex++;

				if (App->CL_Editor_Com->Group[TextureID])
				{
					App->CL_Editor_Com->Group[TextureID]->Face_Data[FacePos].a = FaceIndex;
					FaceIndex++;
					App->CL_Editor_Com->Group[TextureID]->Face_Data[FacePos].b = FaceIndex;
					FaceIndex++;
					App->CL_Editor_Com->Group[TextureID]->Face_Data[FacePos].c = FaceIndex;
					//App->CL_Editor_Com->Group[TextureID]->Face_Data[FacePos].Brush_Index = App->CL_Editor_Com->B_Brush[Count]->Face_Data[FaceCount].Brush_Index;

					App->CL_Editor_Com->Group[TextureID]->FaceIndex_Data[FacePos].Index = ActualFaceCount;

					//int Brush_Index = App->CL_Editor_Com->B_Brush[Count]->Picking_Data[FaceCount].Actual_Brush_Index;
					App->CL_Editor_Com->Group[TextureID]->Face_Data[FacePos].Brush_Index = App->CL_Editor_Com->B_Brush[Count]->Face_Data[FaceCount].Brush_Index;
					App->CL_Editor_Com->Group[TextureID]->Face_Data[FacePos].Main_Face = App->CL_Editor_Com->B_Brush[Count]->Face_Data[FaceCount].Main_Face;
				}

				FaceIndexNum++;
				ActualFaceCount++;

				FaceIndex++;
				FacePos++;
			}

			FaceCount++;
		}

		Count++;
	}

	if (App->CL_Editor_Com->Group[TextureID])
	{
		App->CL_Editor_Com->Group[TextureID]->GroupVertCount = vertexIndex;
		App->CL_Editor_Com->Group[TextureID]->GroupFaceCount = vertexIndex / 3;
	}

	return 1;
}

// *************************************************************************
// *		WE_Get_Vertice_Count:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
int CL64_Mesh_Mgr::WE_Get_Vertice_Count(int TextureID)
{
	int Count = 0;
	int FaceCount = 0;
	int TotalFaceCount = 0;

	int BrushCount = App->CL_Editor_Com->BrushCount;
	Count = 0;

	while (Count < BrushCount)
	{
		FaceCount = 0;
		while (FaceCount < App->CL_Editor_Com->B_Brush[Count]->Face_Count)
		{
			if (App->CL_Editor_Com->B_Brush[Count]->Picking_Data[FaceCount].TextID == TextureID)
			{
				TotalFaceCount++;
			}

			FaceCount++;
		}

		Count++;
	}

	return TotalFaceCount;
}

// *************************************************************************
// *			Get_Adjusted_Index:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
int CL64_Mesh_Mgr::Get_Adjusted_Index(int RealIndex)
{
	int Count = 0;
	while (Count < 500)
	{
		if (mAdjusedIndex_Store[Count] == RealIndex)
		{
			return Count;
		}

		Count++;
	}

	return -1;
}

//// *************************************************************************
//// *			AddTexture_GL:- Terry and Hazel Flanigan 2025		  	   *
//// *************************************************************************
//bool CL64_Mesh_Mgr::AddTexture_GL(geVFile* BaseFile, const char* TextureName, int GroupIndex)
//{
	//Debug
	
	//int index = 0;
	//geBitmap* Bitmap = NULL;
	//CL64_WadFile* pWad;
	//pWad = NULL;
	//pWad = App->CL_Level->Level_GetWadFile(App->CL_Doc->pLevel);
	//for (index = 0; index < pWad->mBitmapCount; index++)
	//{
	//	char mName[MAX_PATH];
	//	char TempTextureFile[MAX_PATH];

	//	strcpy(mName, pWad->mBitmaps[index].Name);

	//	bool test = strcmp(mName, TextureName);
	//	if (test == 0)
	//	{
	//		Bitmap = pWad->mBitmaps[index].bmp;

	//		if (geBitmap_HasAlpha(Bitmap))
	//		{
	//			//App->CL_Textures->LoadTextures_TXL(pWad->mBitmaps[index].Name);

	//			strcpy(TempTextureFile, App->RB_Directory_FullPath);
	//			strcat(TempTextureFile, "\\");
	//			strcat(TempTextureFile, "TextureLoad.tga");

	//			//App->CL_Textures->WriteTGA(TempTextureFile, App->CLSB_Textures->Temp_RF_Bitmap);

	//			//App->CL_Textures->Soil_Load_Texture(App->CL_Ogre->OGL_Listener->g_BrushTexture, TempTextureFile, GroupIndex);
	//		}
	//		else
	//		{
	//			strcpy(TempTextureFile, App->RB_Directory_FullPath);
	//			strcat(TempTextureFile, "\\");
	//			strcat(TempTextureFile, "TextureLoad.bmp");

	//			App->CL_Textures->Write_BMP(TempTextureFile,Bitmap);

	//			App->CL_Textures->Soil_Load_Texture(App->CL_Ogre->OGL_Listener->g_BrushTexture, TempTextureFile, GroupIndex);
	//		}

	//		DeleteFile((LPCTSTR)TempTextureFile);
	//	}
	//}

//	return TRUE;
//}

// *************************************************************************
// *	  		Start_Mesh_Viewer:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Mesh_Mgr::Start_Mesh_Viewer()
{
	if (App->CL_Mesh_Mgr->flag_Mesh_Viewer_Active == 0 && App->CL_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Mesh_Mgr->flag_Mesh_Viewer_Active = 1;
		Mesh_Viewer_HWND = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_BRUSH_VIEWER, App->MainHwnd, (DLGPROC)Proc_Mesh_Viewer);

	}
}

// *************************************************************************
// *			Brush_Viewer_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK CL64_Mesh_Mgr::Proc_Mesh_Viewer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LISTDATA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LT_WORLDINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_SELECT_BRUSH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BT_LOOKAT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_BT_CONVERT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CB_RENDERMODE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		/*SendDlgItemMessage(hDlg, IDC_BT_PICKSELECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_SHOWDATA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/


		//SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		HWND CB_hWnd = GetDlgItem(hDlg, IDC_CB_RENDERMODE);
		App->CL_Mesh_Mgr->Populate_RenderMode_Combo(CB_hWnd); // Populate Combo

		//HWND Temp = GetDlgItem(hDlg, IDC_BT_MESH);
		//if (App->CLSB_Ogre_Setup->RenderListener->ShowFaces == 1)
		//{
		//	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOn_Bmp);
		//}
		//else
		//{
		//	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);
		//}

		App->CL_Mesh_Mgr->Update_Brush_List(hDlg);
		App->CL_Mesh_Mgr->UpdateBrushData(hDlg, 0);// App->CLSB_Mesh_Mgr->Compiled_List_Index);

		//App->CLSB_Ogre_Setup->RenderListener->Render_Just_Brush = 0;

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_ST_TEXTURE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BTJUSTBRUSH && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BTJUSTBRUSH));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Ogre_Setup->RenderListener->Render_Just_Brush);
			}

			return CDRF_DODEFAULT;
		}*/

		/*if (some_item->idFrom == IDC_BT_LOOKAT && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_LOOKAT));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}*/

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_SELECT_BRUSH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_SELECT_BRUSH));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_Mesh_Mgr->flag_Select_Brush);
			}

			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BT_SELECT_BRUSH)
		{
			if (App->CL_Mesh_Mgr->flag_Select_Brush == 1)
			{
				App->CL_Mesh_Mgr->flag_Select_Brush = 0;
				App->CL_Doc->ResetAllSelections();
				App->CL_Doc->UpdateSelected();
				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			}
			else
			{
				int Brush_Index = App->CL_Editor_Com->B_Brush[App->CL_Mesh_Mgr->Brushes_List_Index]->Group_Index;
				
				App->CL_Mesh_Mgr->flag_Select_Brush = 1;
				App->CL_Properties_Brushes->OnSelchangeBrushlist(Brush_Index, true);
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_LISTBRUSHES)
		{
			int Index = 0;
			Index = SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (Index == -1)
			{
				return 1;
			}

			// ------------------- Ogre
			if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Ogre)
			{
				App->CL_Mesh_Mgr->Ogre_List_Index = Index;
			}

			// ------------------- Groups
			if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Groups)
			{
				App->CL_Mesh_Mgr->Groups_List_Index = Index;
			}

			// ------------------- Brushes
			if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Brushes)
			{
				App->CL_Mesh_Mgr->Brushes_List_Index = Index;
				App->CL_Ogre->OGL_Listener->Selected_Brush_Index = Index;

				if (App->CL_Mesh_Mgr->flag_Select_Brush == 1)
				{
					int Brush_Index = App->CL_Editor_Com->B_Brush[Index]->Group_Index;
					App->CL_Properties_Brushes->OnSelchangeBrushlist(Brush_Index, true);
				}
			}

			App->CL_Mesh_Mgr->UpdateBrushData(hDlg, Index);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CB_RENDERMODE)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{

				HWND temp = GetDlgItem(hDlg, IDC_CB_RENDERMODE);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				if (Index == -1)
				{
					return 1;
				}

				App->CL_Mesh_Mgr->Selected_Render_Mode = Index;

				if (Index == Enums::Render_Ogre)
				{
					App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Ogre;
					App->CL_Camera->Camera_Textured();

					App->CL_Mesh_Mgr->Update_Brush_List(hDlg);
					//EnableWindow(GetDlgItem(hDlg, IDC_BTJUSTBRUSH), false);
					//EnableWindow(GetDlgItem(hDlg, IDC_BT_LOOKAT), false);
				}

				if (Index == Enums::Render_Groups) // Groups
				{
					App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Groups;
					App->CL_Camera->Camera_Wired();

					App->CL_Mesh_Mgr->Update_Brush_List(hDlg);
				}

				if (Index == Enums::Render_Brushes) // Brushes
				{
					App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Brushes;
					App->CL_Camera->Camera_Brushes();

					App->CL_Mesh_Mgr->Update_Brush_List(hDlg);

					EnableWindow(GetDlgItem(hDlg, IDC_BT_SELECT_BRUSH), true);
					//EnableWindow(GetDlgItem(hDlg, IDC_BT_LOOKAT), true);
				}
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Mesh_Mgr->flag_Mesh_Viewer_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Ogre;
			App->CL_Camera->Camera_Textured();

			App->CL_Mesh_Mgr->flag_Mesh_Viewer_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	return FALSE;
}

// *************************************************************************
// *	  	Populate_RenderMode_Combo:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void CL64_Mesh_Mgr::Populate_RenderMode_Combo(HWND DropHwnd)
{
	SendMessage(DropHwnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Ogre");
	SendMessage(DropHwnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Groups");
	SendMessage(DropHwnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Brushes");
	SendMessage(DropHwnd, CB_SETCURSEL, Selected_Render_Mode, 0);
}

// *************************************************************************
// *		Update_Brush_List:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CL64_Mesh_Mgr::Update_Brush_List(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	char buf[MAX_PATH];

	// ------------------- Ogre
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Ogre)
	{
		if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
		{
			int SubCount = World_Ent->getNumSubEntities();
			int Count = 0;
			while (Count < SubCount)
			{
				sprintf(buf, "%s %i", "Sub mesh - ", Count);
				SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
				Count++;
			}

			Update_World_Model_Info(hDlg);
			UpdateBrushData(hDlg, App->CL_Mesh_Mgr->Ogre_List_Index);
			SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_SETCURSEL, (WPARAM)App->CL_Mesh_Mgr->Ogre_List_Index, (LPARAM)0);
		}

	}

	// ------------------- Groups
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Groups)
	{
		int SubCount = App->CL_Editor_Com->GroupCount;
		int Count = 0;
		while (Count < SubCount)
		{
			sprintf(buf, "%s %i", "Group - ", Count);
			SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
			Count++;
		}

		Update_World_Model_Info(hDlg);
		UpdateBrushData(hDlg, App->CL_Mesh_Mgr->Groups_List_Index);
		SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_SETCURSEL, (WPARAM)App->CL_Mesh_Mgr->Groups_List_Index, (LPARAM)0);
	}

	// ------------------- Brushes
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Brushes)
	{
		int SubCount = App->CL_Editor_Com->BrushCount;
		int Count = 0;
		while (Count < SubCount)
		{
			sprintf(buf, "%s %i", "Brushes - ", Count);
			SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
			Count++;
		}

		Update_World_Model_Info(hDlg);
		UpdateBrushData(hDlg, Brushes_List_Index);
		SendDlgItemMessage(hDlg, IDC_LISTBRUSHES, LB_SETCURSEL, (WPARAM)Brushes_List_Index, (LPARAM)0);
	}

}

// *************************************************************************
// *			UpdateBrushData:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CL64_Mesh_Mgr::UpdateBrushData(HWND hDlg, int Index)
{
	SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	char buf[MAX_PATH];

	// ------------------- Ogre
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Ogre)
	{
		if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
		{
			World_Ent->getNumSubEntities();

			Ogre::SubMesh const* subMesh = World_Ent->getSubEntity(Index)->getSubMesh();

			int FaceCount = subMesh->indexData->indexCount;
			sprintf(buf, "Face Count - %i", FaceCount / 3);
			SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

			int VerticesCount = subMesh->vertexData->vertexCount;
			sprintf(buf, "Vertices Count - %i", VerticesCount);
			SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

			Ogre::String MatName = subMesh->getMaterialName();
			sprintf(buf, "Material Name - %s", MatName.c_str());
			SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

			Ogre::MaterialPtr  MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(MatName));
			sprintf(buf, "Texture Name - %s", MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());
			SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		}
	}

	// ------------------- Groups / Wireframed
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Groups)
	{
		sprintf(buf, "Group Name %s", App->CL_Editor_Com->Group[Index]->GroupName);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "Vertices %i", App->CL_Editor_Com->Group[Index]->GroupVertCount);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "Mat Index %i", App->CL_Editor_Com->Group[Index]->MaterialIndex);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "Mat Name %s", App->CL_Editor_Com->Group[Index]->MaterialName);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "Mat File Name %s", App->CL_Editor_Com->Group[Index]->Text_FileName);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "Faces %i", App->CL_Editor_Com->Group[Index]->GroupFaceCount);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		int Count = 0;

		while (Count < App->CL_Editor_Com->Group[Index]->GroupFaceCount)
		{
			int Brush_Index = App->CL_Editor_Com->Group[Index]->Face_Data[Count].Brush_Index;

			sprintf(buf, "Face %i Brush_Index %i", Count, Brush_Index);
			SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

			Count++;
		}
		//sprintf(buf, "Faces2 %i", App->CLSB_Mesh_Mgr->ActualFaceCount);
		//SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	// ------------------- Brushes
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Brushes)
	{
		sprintf(buf, "Brush Name %s",App->CL_Editor_Com->B_Brush[Index]->Brush_Name);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "Brush Index %i", App->CL_Editor_Com->B_Brush[Index]->Group_Index);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "Faces %i", App->CL_Editor_Com->B_Brush[Index]->Face_Count);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "Vertices %i", App->CL_Editor_Com->B_Brush[Index]->Vertice_Count);
		SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		int Count = 0;

		while (Count < App->CL_Editor_Com->B_Brush[Index]->Face_Count)
		{
			int TextureID = App->CL_Editor_Com->B_Brush[Index]->Picking_Data[Count].TextID;
			int Brush_Index = App->CL_Editor_Com->B_Brush[Index]->Face_Data[Count].Brush_Index;
			int Main_Face = App->CL_Editor_Com->B_Brush[Index]->Face_Data[Count].Main_Face;

			sprintf(buf, "Text_ID %i %s Brush_Index %i Main_Face %i", TextureID, TextureName2[TextureID], Brush_Index, Main_Face);
			SendDlgItemMessage(hDlg, IDC_LISTDATA, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
			Count++;
		}

		//App->CL_Mesh_Mgr->Set_BBox_Selected_Brush(Index);
	}
}

// *************************************************************************
// *		Update_World_Model_Info:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Mesh_Mgr::Update_World_Model_Info(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_LT_WORLDINFO, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	
	char buf[MAX_PATH];

	// ------------------- Ogre
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Ogre)
	{
		sprintf(buf, "%s %i", "Total Sub Meshs - ", (int)World_Ent->getNumSubEntities());
		SendDlgItemMessage(hDlg, IDC_LT_WORLDINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "%s %i", "Total Faces - ", App->CL_Editor_Com->Ogre_Face_Count);
		SendDlgItemMessage(hDlg, IDC_LT_WORLDINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	// ------------------- Groups
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Groups)
	{
		sprintf(buf, "%s %i", "Total Groups - ", App->CL_Editor_Com->GroupCount);
		SendDlgItemMessage(hDlg, IDC_LT_WORLDINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "%s %i", "Total Faces - ", App->CL_Mesh_Mgr->ActualFaceCount);
		SendDlgItemMessage(hDlg, IDC_LT_WORLDINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	// ------------------- Brushes
	if (App->CL_Mesh_Mgr->Selected_Render_Mode == Enums::Render_Brushes)
	{
		sprintf(buf, "%s %i", "Total Brushes - ", App->CL_Editor_Com->BrushCount);
		SendDlgItemMessage(hDlg, IDC_LT_WORLDINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "%s %i", "Total Faces - ", App->CL_Editor_Com->Ogre_Face_Count);
		SendDlgItemMessage(hDlg, IDC_LT_WORLDINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

}






