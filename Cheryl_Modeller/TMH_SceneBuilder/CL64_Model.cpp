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
	Selected_BoneIndex = 0;

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
		strcpy(Model_Just_Name, Loaded_FileName);
		int Len = strlen(Model_Just_Name);
		Model_Just_Name[Len - 4] = 0;

	}
}

// *************************************************************************
// *		Create_BondingBox_Model:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Model::Set_BondingBox_Model(bool Create)
{
	if (Create)
	{
		S_BoundingBox[0] = new AABB_Type;
	}

	auto& G = App->CL_Mesh->Group;

	if (GroupCount > 0 && App->CL_Model->VerticeCount > 4)
	{
		// Initialize bounding box with the first vertex
		const auto& firstVertex = G[0]->vertex_Data[0];
		S_BoundingBox[0]->BB_Min[0] = firstVertex;
		S_BoundingBox[0]->BB_Max[0] = firstVertex;

		// Iterate through each group and vertex to find the min and max
		for (int count = 0; count < GroupCount; ++count)
		{
			for (int vertCount = 0; vertCount < G[count]->GroupVertCount; ++vertCount)
			{
				const auto& vertex = G[count]->vertex_Data[vertCount];
				S_BoundingBox[0]->BB_Min[0].x = std::min(S_BoundingBox[0]->BB_Min[0].x, vertex.x);
				S_BoundingBox[0]->BB_Min[0].y = std::min(S_BoundingBox[0]->BB_Min[0].y, vertex.y);
				S_BoundingBox[0]->BB_Min[0].z = std::min(S_BoundingBox[0]->BB_Min[0].z, vertex.z);
				S_BoundingBox[0]->BB_Max[0].x = std::max(S_BoundingBox[0]->BB_Max[0].x, vertex.x);
				S_BoundingBox[0]->BB_Max[0].y = std::max(S_BoundingBox[0]->BB_Max[0].y, vertex.y);
				S_BoundingBox[0]->BB_Max[0].z = std::max(S_BoundingBox[0]->BB_Max[0].z, vertex.z);
			}
		}

		// Calculate size and radius
		S_BoundingBox[0]->Size[0].x = std::fabs(S_BoundingBox[0]->BB_Max[0].x - S_BoundingBox[0]->BB_Min[0].x);
		S_BoundingBox[0]->Size[0].y = std::fabs(S_BoundingBox[0]->BB_Max[0].y - S_BoundingBox[0]->BB_Min[0].y);
		S_BoundingBox[0]->Size[0].z = std::fabs(S_BoundingBox[0]->BB_Max[0].z - S_BoundingBox[0]->BB_Min[0].z);

		S_BoundingBox[0]->radius = std::min(S_BoundingBox[0]->Size[0].x, S_BoundingBox[0]->Size[0].z) / 2.0f;

		// Calculate center
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
	if (App->CL_Motions->flag_Motion_Playing == true)
	{
		App->CL_Motions->Stop_SelectedMotion();
	}

	App->CL_ImGui->Reset_Class();
	
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
	
	// Reset the mesh data filename
	App->CL_Mesh->S_OgreMeshData[0]->mFileName_Str = "No Model Loaded";
	App->CL_Mesh->S_OgreMeshData[0]->m_Motion_Names.resize(0);

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

	App->CL_Ogre->OGL_Listener->Reset_Class();
	
	App->CL_Properties_Textures_Com->Reset_Class();
	
	App->CL_Interface->Show_Textures_Com_Dlg(false);
	App->CL_Interface->Show_Motions_Dlg(false);
	App->CL_Interface->Menu_Enable_Textures(false);

	// Reset editor, camera, and panels
	App->CL_Editor_Control->Reset_Editor();
	App->CL_Camera->Reset_View();
	App->CL_Interface->Reset_All_Dialogs();
	App->CL_Interface->Set_Title(true);
	
	if (App->CL_Importers->Model_Brush)
	{
		App->CL_Level->Level_RemoveBrush(App->CL_Importers->Model_Brush);
		App->CL_Importers->Model_Brush = NULL;

		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
	}


	App->CL_Views_Com->Reset_Views_All();

	if (App->CL_Editor_Control->flag_Mode_3DEditor_View == true)
	{
		App->CL_Editor_Control->Set_3DEditor_View();
	}

	App->CL_Editor_Control->flag_Just_Loaded = false;
}

