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
#include "Structures.cpp"

enum BrushFlags
{
	BRUSH_SOLID = 0x0001,
	BRUSH_WINDOW = 0x0002,
	BRUSH_WAVY = 0x0004,
	BRUSH_DETAIL = 0x0008,	//not included in vis calculations
	BRUSH_HOLLOWCUT = 0x0010,
	BRUSH_TRANSLUCENT = 0x0020,
	BRUSH_EMPTY = 0x0040,
	BRUSH_SUBTRACT = 0x0080,
	BRUSH_CLIP = 0x0100,
	BRUSH_FLOCKING = 0x0200,
	BRUSH_HOLLOW = 0x0400,
	BRUSH_SHEET = 0x0800,
	BRUSH_HIDDEN = 0x1000,
	BRUSH_LOCKED = 0x2000,
	BRUSH_HINT = 0x4000,
	BRUSH_AREA = 0x8000
	// All flags larger than 0x8000 (i.e. 0x00010000 through 0x80000000)
	// are reserved for user contents.
};


struct tag_Level3
{
	BrushList* Brushes;
	char* WadPath;
	char* HeadersDir;
	// changed QD Actors
	char* ActorsDir;
	signed int ShowActors;
	char* PawnIniPath;
	// end change
	SizeInfo* WadSizeInfos;
	CL64_WadFile* WadFile;

	//ModelInfo_Type	ModelInfo;

	
	float SkyRotationSpeed;
	float	SkyTextureScale;

	// level edit settings
	
	int GroupVisSetting;
	//GridInfo GridSettings;
	signed int BspRebuildFlag;
	//ViewStateInfo ViewInfo[NUM_VIEWS];

	BrushTemplate_Arch ArchTemplate;
	BrushTemplate_Box	BoxTemplate;
	BrushTemplate_Cone	ConeTemplate;
	BrushTemplate_Cylinder CylinderTemplate;
	BrushTemplate_Spheroid	SpheroidTemplate;
	BrushTemplate_Staircase StaircaseTemplate;

	Ogre::Vector3 TemplatePos;

	float DrawScale;		// default draw scale
	float LightmapScale;	// default lightmap scale
};

CL64_Mesh_Mgr::CL64_Mesh_Mgr()
{
	World_Node = nullptr;
	World_Ent = nullptr;

	m_Total_Faces = 0;
	mBrushCount = 0;
	mSubBrushCount = 0;
	Global_Faces_Index = 0;
	mTextureCount = 0;
	Actual_Brush_Index = 0;
	ActualFaceCount = 0;
	mBrush_Index = 0;
	mBrush_Name[0] = 0;

	memset(mAdjusedIndex_Store, 0, 500);

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
// * 		Delete_Brush_List:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Mesh_Mgr::Delete_Brush_List()
{
	CL64_Editor* pModel = App->CL_Editor;

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

	App->CL_Editor->BrushCount = 0;
	App->CL_Editor->Brush_Face_Count = 0;
	Global_Faces_Index = 0;
	m_Total_Faces = 0;
}

// *************************************************************************
// * 		Delete_Group_Brushes:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Mesh_Mgr::Delete_Group_Brushes()
{
	CL64_Editor* pModel = App->CL_Editor;

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

	App->CL_Editor->GroupCount = 0;
	App->CL_Editor->VerticeCount = 0;
	
}

// *************************************************************************
// *		Update_World:- Terry and Hazel Flanigan 2025	 			   *
// *************************************************************************
bool CL64_Mesh_Mgr::Update_World()
{
	v_Face_Data_Count = 0;

	int BC = App->CL_Brush->Get_Brush_Count();
	if (BC > 0)
	{
		Brush_Build_List(0);
		WE_Convert_All_Texture_Groups();

		if (App->CL_Ogre->OGL_Listener->Flag_Render_Brushes == 0)
		{
			App->CL_Ogre3D->Convert_ToOgre3D(1); // Will Set Node Visable
		}
	}
	else
	{
		App->CL_Mesh_Mgr->World_Node->setVisible(false);
	}
	
	return 1;
}

// *************************************************************************
// * 		Brush_Build_List:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Mesh_Mgr::Brush_Build_List(int ExpSelected)
{
	Delete_Brush_List();

	//App->CL_Doc->Set_Faces_To_Brush_Name_All();

	App->CL_Editor->BrushCount = 0;

	mBrushCount = 0;
	mSubBrushCount = 0;

	BrushList* BList = NULL;
	signed int fResult;

	BList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);
	if (BList == NULL)
	{
		App->Say("Error");
	}

	if (ExpSelected == 0)	// Build All
	{
		fResult = Brush_Build_Level_Brushes(reinterpret_cast<tag_Level3*> (App->CL_Doc->pLevel), "FileName", BList, 0, 0, -1);
	}
}

// *************************************************************************
// *		Brush_Build_Level_Brushes:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_Build_Level_Brushes(Level3* pLevel, const char* Filename, BrushList* BList, int ExpSelected, signed int ExpLights, int GroupID)
{
	CL64_WadFile* pWad;
	pWad = NULL;
	pWad = App->CL_Level->Level_GetWadFile(App->CL_Doc->pLevel);
	int ml_BitMap_Count = pWad->mBitmapCount;

	mTextureCount = 0;
	memset(mAdjusedIndex_Store, 0, 500);
	memset(UsedTextures, 0, 500);
	Actual_Brush_Index = 0;

	int i;

	App->CL_Brush_X->BrushList_GetUsedTextures_X(UsedTextures);

	// Add Textures GL
	int AdjustedIndex = 0;

	for (i = 0; i < ml_BitMap_Count; i++)
	{
		if (UsedTextures[i])
		{
			char matname[MAX_PATH];

			strncpy(matname, pWad->mBitmaps[i].Name, MAX_PATH - 1);
			strcpy(TextureName2[AdjustedIndex], matname);

			mAdjusedIndex_Store[AdjustedIndex] = i;

			if (App->CL_Ogre->OGL_Listener->Flag_Render_Brushes == 0)
			{
				//AddTexture_GL(NULL, matname, AdjustedIndex);
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

	return 1;
}

// *************************************************************************
// *			Brush_Decode_List:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_Decode_List(BrushList* BList, signed int SubBrush)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);

	while (pBrush != NULL)
	{
		strcpy(mBrush_Name, pBrush->Name);

		if (mSubBrushCount == 0 && pBrush->Flags & 1 || pBrush->Flags & 1024)
		{
			if (SubBrush == 0)
			{
				mBrush_Index = mBrushCount;
			}
		}

		if (!Brush_Create(pBrush, Actual_Brush_Index))
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
			Actual_Brush_Index++;
			strcpy(Actual_mBrush_Name, pBrush->Name);
		}

		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}

	mSubBrushCount = 0;

	if (!SubBrush)
	{
		mBrushCount = 0;
	}

	return true;
}

// *************************************************************************
// *			Brush_Create:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_Create(const Brush* b, int Actual_Brush_Index)
{
	switch (b->Type)
	{
	case BRUSH_MULTI:
		return Brush_Decode_List(b->BList, true); // Recursive

	case BRUSH_LEAF:
		if (b->BList)
		{
			return Brush_Decode_List(b->BList, true); // Recursive
		}
		else
		{
			if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
			{
				return Brush_FaceList_Create(b, b->Faces, mBrushCount, mSubBrushCount, Actual_Brush_Index);
			}
			else if ((b->Flags & BRUSH_SUBTRACT) && !(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT)))
				mBrushCount--;
		}
		break;


	case BRUSH_CSG:
		if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
		{
			return Brush_FaceList_Create(b, b->Faces, mBrushCount, mSubBrushCount, Actual_Brush_Index);
		}
		break;
	default:
		assert(0);		// invalid brush type
		break;
	}

	return true;
}

// *************************************************************************
// *		Brush_FaceList_Create:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Mesh_Mgr::Brush_FaceList_Create(const Brush* b, const FaceList* pList, int BrushCount, int SubBrushCount, int Actual_Brush_Index)
{
	App->CL_Editor->Create_Brush_XX(App->CL_Editor->BrushCount);
	App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Group_Index = mBrush_Index;
	strcpy(App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Brush_Name, mBrush_Name);

	m_Total_Faces = m_Total_Faces + pList->NumFaces;

	int i, j, k, num_faces, num_verts, num_mats, num_chars, curnum_verts;
	char matname[MAX_PATH];

	char* matf = (char*)calloc(sizeof(char), pList->NumFaces);

	assert(pList != NULL);
	assert(f != NULL);

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
		matf[i] = 0;

	// -----------------------------------  Vertices
	int VertIndex = 0;

	App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Vertice_Count = num_verts;
	App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->vertex_Data.resize(num_verts);
	App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Normal_Data.resize(num_verts);

	for (i = 0; i < pList->NumFaces; i++)
	{
		
		const T_Vec3* verts;
		verts = App->CL_Face->Face_GetPoints(pList->Faces[i]);
		curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[i]);
		for (j = 0; j < curnum_verts; j++)
		{
	

			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->vertex_Data[VertIndex].x = verts[j].x;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->vertex_Data[VertIndex].y = verts[j].y;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->vertex_Data[VertIndex].z = verts[j].z;

			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Normal_Data[VertIndex].x = 0.5;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Normal_Data[VertIndex].y = 0.5;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Normal_Data[VertIndex].z = 0.5;

			VertIndex++;
		}

	}

	int UVIndex = 0;
	// -----------------------------------  UVS
	App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->MapCord_Data.resize(num_verts);
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
			txSize = 32;
		if (tySize == 0)
			tySize = 32;

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

			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->MapCord_Data[UVIndex].u = U;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->MapCord_Data[UVIndex].v = V;
			UVIndex++;
		}
	}


	// -----------------------------------  Faces
	int FaceIndex = 0;
	App->CL_Editor->Brush_Face_Count = App->CL_Editor->Brush_Face_Count + num_faces;
	App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Face_Count = num_faces;
	App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Face_Data.resize(num_faces);
	App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Picking_Data.resize(num_faces);
	num_verts = 0;
	for (i = 0; i < pList->NumFaces; i++)
	{
		curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[i]); //4

		for (j = 0; j < curnum_verts - 2; j++)
		{
			Create_V_Face(v_Face_Data_Count);
			strcpy(v_Face_Data[v_Face_Data_Count]->Brush_Name, App->CL_Face->Face_GetBrushName(pList->Faces[i]));

			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Face_Data[FaceIndex].a = num_verts;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Face_Data[FaceIndex].b = num_verts + 2 + j;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Face_Data[FaceIndex].c = num_verts + 1 + j;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Face_Data[FaceIndex].face_Index = v_Face_Data_Count;

			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Picking_Data[FaceIndex].WE_Face_Index = i;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Picking_Data[FaceIndex].Global_Face = Global_Faces_Index;
			App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Picking_Data[FaceIndex].Actual_Brush_Index = Actual_Brush_Index;

			FaceIndex++;
			Global_Faces_Index++;
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
			matf[i] = 1;

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
				App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Picking_Data[curnum_faces + j].TextID = TextureId;

			}

			curnum_faces += (curnum_verts - 2);

			for (j = i + 1; j < pList->NumFaces; j++)
			{
				curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[j]);
				if (strcmp(App->CL_Face->Face_GetTextureName(pList->Faces[i]), App->CL_Face->Face_GetTextureName(pList->Faces[j])) == 0)
				{
					matf[j] = 1;
					for (k = 0; k < curnum_verts - 2; k++)
					{
						
						TextureId = Get_Adjusted_Index(App->CL_Face->Face_GetTextureDibId(pList->Faces[i]));
						App->CL_Editor->B_Brush[App->CL_Editor->BrushCount]->Picking_Data[curnum_faces + k].TextID = TextureId;
						
						
					}
				}

				curnum_faces += (curnum_verts - 2);
			}
		}
	}

	free(matf);

	App->CL_Editor->BrushCount++;

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
bool CL64_Mesh_Mgr::WE_Convert_All_Texture_Groups()
{
	int mTotalVertices = 0;
	ActualFaceCount = 0;

	Delete_Group_Brushes();

	strcpy(App->CL_Editor->JustName, "Test");

	App->CL_Editor->GroupCount = mTextureCount;

	int Count = 0;
	while (Count < mTextureCount)
	{
		App->CL_Editor->Create_Mesh_Group(Count);
		int FaceCount = WE_Get_Vertice_Count(Count);

		strcpy(App->CL_Editor->Group[Count]->GroupName, TextureName2[Count]);
		strcpy(App->CL_Editor->Group[Count]->MaterialName, TextureName2[Count]);

		int TrueIndex = App->CL_TXL_Editor->GetIndex_From_Name(TextureName2[Count]);
		
		//App->Say(TextureName2[Count]);
		//App->Say(App->CL_TXL_Editor->Texture_List[TrueIndex]->FileName);

		char buff[MAX_PATH];
		strcpy(buff, App->CL_TXL_Editor->Texture_List[TrueIndex]->FileName);
		strcpy(App->CL_Editor->Group[Count]->Text_FileName, buff);

		App->CL_Editor->Group[Count]->Has_Alpha = App->CL_TXL_Editor->Texture_List[TrueIndex]->Has_Alpha;
		App->CL_Editor->Group[Count]->MaterialIndex = Count;
		App->CL_Editor->Group[Count]->vertex_Data.resize(FaceCount * 3);
		App->CL_Editor->Group[Count]->Normal_Data.resize(FaceCount * 3);
		App->CL_Editor->Group[Count]->MapCord_Data.resize(FaceCount * 3);
		App->CL_Editor->Group[Count]->Face_Data.resize(FaceCount);
		App->CL_Editor->Group[Count]->FaceIndex_Data.resize(FaceCount * 3);

		WE_Convert_To_Texture_Group(Count);

		mTotalVertices = mTotalVertices + FaceCount;

		Count++;
	}

	App->CL_Editor->VerticeCount = mTotalVertices * 3;
	App->CL_Editor->FaceCount = mTotalVertices;

	//App->CL_Editor->Render_Type = Enums::LoadedFile_Assimp;

	//Update_Brush_List(Mesh_Viewer_HWND);

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

	int BrushCount = App->CL_Editor->BrushCount;
	Count = 0;
	

	while (Count < BrushCount)
	{
		FaceCount = 0;

		while (FaceCount < App->CL_Editor->B_Brush[Count]->Face_Count)
		{
			int FaceIndexNum = 0;

			if (App->CL_Editor->B_Brush[Count]->Picking_Data[FaceCount].TextID == TextureID)
			{
				A = App->CL_Editor->B_Brush[Count]->Face_Data[FaceCount].a;
				B = App->CL_Editor->B_Brush[Count]->Face_Data[FaceCount].b;
				C = App->CL_Editor->B_Brush[Count]->Face_Data[FaceCount].c;

				int FI = App->CL_Editor->B_Brush[Count]->Face_Data[FaceCount].face_Index;

				X = App->CL_Editor->B_Brush[Count]->vertex_Data[A].x;
				Y = App->CL_Editor->B_Brush[Count]->vertex_Data[A].y;
				Z = App->CL_Editor->B_Brush[Count]->vertex_Data[A].z;

				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].x = X;
				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].y = Y;
				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].z = Z;

				U = App->CL_Editor->B_Brush[Count]->MapCord_Data[A].u,
				V = App->CL_Editor->B_Brush[Count]->MapCord_Data[A].v;

				App->CL_Editor->Group[TextureID]->MapCord_Data[vertexIndex].u = U;
				App->CL_Editor->Group[TextureID]->MapCord_Data[vertexIndex].v = V;

				X = App->CL_Editor->B_Brush[Count]->Normal_Data[A].x;
				Y = App->CL_Editor->B_Brush[Count]->Normal_Data[A].y;
				Z = App->CL_Editor->B_Brush[Count]->Normal_Data[A].z;

				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].x = X;
				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].y = Y;
				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].z = Z;

				vertexIndex++;

				X = App->CL_Editor->B_Brush[Count]->vertex_Data[B].x;
				Y = App->CL_Editor->B_Brush[Count]->vertex_Data[B].y;
				Z = App->CL_Editor->B_Brush[Count]->vertex_Data[B].z;

				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].x = X;
				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].y = Y;
				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].z = Z;

				U = App->CL_Editor->B_Brush[Count]->MapCord_Data[B].u,
					V = App->CL_Editor->B_Brush[Count]->MapCord_Data[B].v;

				App->CL_Editor->Group[TextureID]->MapCord_Data[vertexIndex].u = U;
				App->CL_Editor->Group[TextureID]->MapCord_Data[vertexIndex].v = V;

				X = App->CL_Editor->B_Brush[Count]->Normal_Data[B].x;
				Y = App->CL_Editor->B_Brush[Count]->Normal_Data[B].y;
				Z = App->CL_Editor->B_Brush[Count]->Normal_Data[B].z;

				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].x = X;
				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].y = Y;
				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].z = Z;

				vertexIndex++;

				X = App->CL_Editor->B_Brush[Count]->vertex_Data[C].x;
				Y = App->CL_Editor->B_Brush[Count]->vertex_Data[C].y;
				Z = App->CL_Editor->B_Brush[Count]->vertex_Data[C].z;

				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].x = X;
				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].y = Y;
				App->CL_Editor->Group[TextureID]->vertex_Data[vertexIndex].z = Z;

				U = App->CL_Editor->B_Brush[Count]->MapCord_Data[C].u,
				V = App->CL_Editor->B_Brush[Count]->MapCord_Data[C].v;

				App->CL_Editor->Group[TextureID]->MapCord_Data[vertexIndex].u = U;
				App->CL_Editor->Group[TextureID]->MapCord_Data[vertexIndex].v = V;

				X = App->CL_Editor->B_Brush[Count]->Normal_Data[C].x;
				Y = App->CL_Editor->B_Brush[Count]->Normal_Data[C].y;
				Z = App->CL_Editor->B_Brush[Count]->Normal_Data[C].z;

				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].x = X;
				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].y = Y;
				App->CL_Editor->Group[TextureID]->Normal_Data[vertexIndex].z = Z;

				vertexIndex++;

				App->CL_Editor->Group[TextureID]->Face_Data[FacePos].a = FaceIndex;
				FaceIndex++;
				App->CL_Editor->Group[TextureID]->Face_Data[FacePos].b = FaceIndex;
				FaceIndex++;
				App->CL_Editor->Group[TextureID]->Face_Data[FacePos].c = FaceIndex;

				App->CL_Editor->Group[TextureID]->FaceIndex_Data[FacePos].Index = ActualFaceCount;

				int Brush_Index = App->CL_Editor->B_Brush[Count]->Picking_Data[FaceCount].Actual_Brush_Index;
				App->CL_Editor->Group[TextureID]->Face_Data[FacePos].Brush_Index = Brush_Index;
				App->CL_Editor->Group[TextureID]->Face_Data[FacePos].WE_Face_Index = App->CL_Editor->B_Brush[Count]->Picking_Data[FaceCount].WE_Face_Index;
				FaceIndexNum++;
				ActualFaceCount++;

				FaceIndex++;
				FacePos++;
			}

			FaceCount++;
		}

		Count++;
	}

	App->CL_Editor->Group[TextureID]->GroupVertCount = vertexIndex;
	App->CL_Editor->Group[TextureID]->GroupFaceCount = vertexIndex / 3;

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

	int BrushCount = App->CL_Editor->BrushCount;
	Count = 0;

	while (Count < BrushCount)
	{
		FaceCount = 0;
		while (FaceCount < App->CL_Editor->B_Brush[Count]->Face_Count)
		{
			if (App->CL_Editor->B_Brush[Count]->Picking_Data[FaceCount].TextID == TextureID)
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
