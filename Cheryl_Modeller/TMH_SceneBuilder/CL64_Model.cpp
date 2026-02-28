#include "pch.h"
#include "CL64_App.h"
#include "CL64_Model.h"

CL64_Model::CL64_Model(void)
{
	strcpy(Loaded_PathFileName,"No File");
	strcpy(Loaded_FileName, "No File");
	strcpy(Model_FolderPath, "No Path");
	strcpy(Model_Just_Name, "No Name");

	Model_Type = Enums::Model_Type_None;

	// Internal
	GroupCount = 0;
	TextureCount = 0;
	MotionCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	BoneCount = 0;

	flag_Model_Loaded = false;

	Imported_Ogre_Ent = nullptr;
	Imported_Ogre_Node = nullptr;

	S_BoundingBox[0] = nullptr;
}

CL64_Model::~CL64_Model(void)
{
}

// *************************************************************************
// *			Set_Paths:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Model::Set_Paths(void)
{
	// Model Folder Path
	int len1 = strlen(Loaded_FileName);
	int len2 = strlen(Loaded_PathFileName);
	strcpy(Model_FolderPath, Loaded_PathFileName);
	Model_FolderPath[len2 - len1] = 0;

	//strcpy(Texture_FolderPath, Model_FolderPath); // Back Slash remains

	if (_stricmp(Loaded_FileName + strlen(Loaded_FileName) - 5, ".mesh") == 0)
	{
		strcpy(Model_Just_Name, Loaded_FileName);
		int Len = strlen(Model_Just_Name);
		Model_Just_Name[Len - 5] = 0;
	}
	else if (_stricmp(Loaded_FileName + strlen(Loaded_FileName) - 5, ".G3ds") == 0)
	{
		strcpy(Loaded_FileName, Loaded_FileName);
		int Len = strlen(Loaded_FileName);
		Loaded_FileName[Len - 5] = 0;
	}
	else if (_stricmp(Loaded_FileName + strlen(Loaded_FileName) - 5, ".Wepf") == 0)
	{

	}
	else
	{
		strcpy(Loaded_FileName, Loaded_FileName);
		int Len = strlen(Loaded_FileName);
		Model_Just_Name[Len - 4] = 0;
	}
}

// *************************************************************************
// *		Create_BondingBox_Model:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Model::Set_BondingBox_Model(bool Create)
{
	if (Create == 1)
	{
		S_BoundingBox[0] = new AABB_Type;
	}

	auto& G = App->CL_Mesh->Group;

	if (GroupCount > 0 && App->CL_Model->VerticeCount > 4)
	{
		S_BoundingBox[0]->BB_Min[0].x = G[0]->vertex_Data[0].x;
		S_BoundingBox[0]->BB_Min[0].y = G[0]->vertex_Data[0].y;
		S_BoundingBox[0]->BB_Min[0].z = G[0]->vertex_Data[0].z;

		S_BoundingBox[0]->BB_Max[0].x = G[0]->vertex_Data[0].x;
		S_BoundingBox[0]->BB_Max[0].y = G[0]->vertex_Data[0].y;
		S_BoundingBox[0]->BB_Max[0].z = G[0]->vertex_Data[0].z;

		int Count = 0;
		int VertCount = 0;

		while (Count < GroupCount)
		{
			VertCount = 0;
			while (VertCount < G[Count]->GroupVertCount)
			{
				if (G[Count]->vertex_Data[VertCount].x < S_BoundingBox[0]->BB_Min[0].x) S_BoundingBox[0]->BB_Min[0].x = G[Count]->vertex_Data[VertCount].x;
				if (G[Count]->vertex_Data[VertCount].y < S_BoundingBox[0]->BB_Min[0].y) S_BoundingBox[0]->BB_Min[0].y = G[Count]->vertex_Data[VertCount].y;
				if (G[Count]->vertex_Data[VertCount].z < S_BoundingBox[0]->BB_Min[0].z) S_BoundingBox[0]->BB_Min[0].z = G[Count]->vertex_Data[VertCount].z;
				if (G[Count]->vertex_Data[VertCount].x > S_BoundingBox[0]->BB_Max[0].x) S_BoundingBox[0]->BB_Max[0].x = G[Count]->vertex_Data[VertCount].x;
				if (G[Count]->vertex_Data[VertCount].y > S_BoundingBox[0]->BB_Max[0].y) S_BoundingBox[0]->BB_Max[0].y = G[Count]->vertex_Data[VertCount].y;
				if (G[Count]->vertex_Data[VertCount].z > S_BoundingBox[0]->BB_Max[0].z) S_BoundingBox[0]->BB_Max[0].z = G[Count]->vertex_Data[VertCount].z;
				VertCount++;
			}
			Count++;
		}

		S_BoundingBox[0]->Size[0].x = (fabs(S_BoundingBox[0]->BB_Max[0].x - S_BoundingBox[0]->BB_Min[0].x));
		S_BoundingBox[0]->Size[0].y = (fabs(S_BoundingBox[0]->BB_Max[0].y - S_BoundingBox[0]->BB_Min[0].y));
		S_BoundingBox[0]->Size[0].z = (fabs(S_BoundingBox[0]->BB_Max[0].z - S_BoundingBox[0]->BB_Min[0].z));

		S_BoundingBox[0]->radius = (S_BoundingBox[0]->Size[0].x > S_BoundingBox[0]->Size[0].z) ? S_BoundingBox[0]->Size[0].z / 2.0f : S_BoundingBox[0]->Size[0].x / 2.0f;

		S_BoundingBox[0]->Centre[0].x = (S_BoundingBox[0]->BB_Min[0].x + S_BoundingBox[0]->BB_Max[0].x) / 2.0f;
		S_BoundingBox[0]->Centre[0].y = (S_BoundingBox[0]->BB_Min[0].y + S_BoundingBox[0]->BB_Max[0].y) / 2.0f;
		S_BoundingBox[0]->Centre[0].z = (S_BoundingBox[0]->BB_Min[0].z + S_BoundingBox[0]->BB_Max[0].z) / 2.0f;
	}
}

// *************************************************************************
// *			Clear_Model:- Terry and Hazel Flanigan 2026				   *
// *************************************************************************
void CL64_Model::Clear_Model()
{
	// Check if the imported Ogre entity and node exist
	if (App->CL_Model->Imported_Ogre_Ent && App->CL_Model->Imported_Ogre_Node)
	{
		// Detach all objects from the node and destroy the node and entity
		App->CL_Model->Imported_Ogre_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Model->Imported_Ogre_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Model->Imported_Ogre_Ent);

		// Nullify pointers to prevent dangling references
		App->CL_Model->Imported_Ogre_Ent = nullptr;
		App->CL_Model->Imported_Ogre_Node = nullptr;
	}

	if (App->CL_Resources->flag_Ogre_CFG_Loaded == true)
	{
		App->CL_Dialogs->YesNo((LPSTR)"Use Loaded Resources", (LPSTR)"Yes No");
		if (App->CL_Dialogs->flag_Dlg_Canceled == true) // No
		{
			App->CL_Resources->Unload_OgreCFG_Resources();
		}
	}


	// Reset the mesh data filename
	App->CL_Mesh->S_OgreMeshData[0]->mFileName_Str = "No Model Loaded";

	// Clear groups
	for (int Count = 0; Count < GroupCount; ++Count)
	{
		auto& p_Group = App->CL_Mesh->Group;

		// Check if the group exists before clearing
		if (p_Group[Count] != nullptr)
		{
			// Clear vertex, face, normal, and texture coordinate data
			p_Group[Count]->vertex_Data.clear();
			p_Group[Count]->Face_Data.clear();
			p_Group[Count]->Normal_Data.clear();
			p_Group[Count]->MapCord_Data.clear();

			// Delete the base bitmap if it exists
			if (p_Group[Count]->Base_Bitmap)
			{
				DeleteObject(p_Group[Count]->Base_Bitmap);
			}

			// Delete the group and nullify the pointer
			delete p_Group[Count];
			p_Group[Count] = nullptr;
		}
	}

	// Reset counts to zero
	GroupCount = 0;
	TextureCount = 0;
	MotionCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	BoneCount = 0;

	App->CL_Properties_Materials->Reset_Class();

	// Reset editor, camera, and panels
	App->CL_Editor_Control->Reset_Editor();
	App->CL_Camera->Reset_View();
	App->CL_Interface->Reset_All_Dialogs();
}
