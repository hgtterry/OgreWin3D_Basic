/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
#include "CL64_Model.h"

CL64_Model::CL64_Model(void)
{
	Model_Type = Enums::Model_Type_None;
	
	// Internal
	GroupCount = 0;
	TextureCount = 0;
	MotionCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	BoneCount = 0;

	// Brushes
	BrushCount = 0;
	Brush_Face_Count = 0;

	Selected_BoneIndex = 0;

	flag_Model_Loaded = false;
	flag_BoundingBox_Created = false;

	flag_Model_is_Modified = false;

	Imported_Ogre_Ent = nullptr;
	Imported_Ogre_Node = nullptr;

	int Count = 0;
	while (Count < 11999)
	{
		B_Brush[Count] = nullptr;
		Count++;
	}

	BBox_Max = Ogre::Vector3::ZERO;
	BBox_Min = Ogre::Vector3::ZERO;
	BBox_Size = Ogre::Vector3::ZERO;
	BBox_Centre = Ogre::Vector3::ZERO;
	BBox_Radius = 0;

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
	int len1 = strlen(App->CL_File->Loaded_FileName);
	int len2 = strlen(App->CL_File->Loaded_PathFileName);
	strcpy(App->CL_File->Model_FolderPath, App->CL_File->Loaded_PathFileName);
	App->CL_File->Model_FolderPath[len2 - len1] = 0;

	//strcpy(Texture_FolderPath, Model_FolderPath); // Back Slash remains

	if (_stricmp(App->CL_File->Loaded_FileName + strlen(App->CL_File->Loaded_FileName) - 5, ".mesh") == 0)
	{
		strcpy(App->CL_File->Model_Just_Name, App->CL_File->Loaded_FileName);
		int Len = strlen(App->CL_File->Model_Just_Name);
		App->CL_File->Model_Just_Name[Len - 5] = 0;
	}
	else if (_stricmp(App->CL_File->Loaded_FileName + strlen(App->CL_File->Loaded_FileName) - 5, ".G3ds") == 0)
	{
		strcpy(App->CL_File->Loaded_FileName, App->CL_File->Loaded_FileName);
		int Len = strlen(App->CL_File->Loaded_FileName);
		App->CL_File->Loaded_FileName[Len - 5] = 0;
	}
	else if (_stricmp(App->CL_File->Loaded_FileName + strlen(App->CL_File->Loaded_FileName) - 5, ".Wepf") == 0)
	{

	}
	else
	{
		strcpy(App->CL_File->Model_Just_Name, App->CL_File->Loaded_FileName);
		int Len = strlen(App->CL_File->Model_Just_Name);
		App->CL_File->Model_Just_Name[Len - 4] = 0;

	}
}

// *************************************************************************
// *		Create_BondingBox_Model:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Model::Set_BondingBox_Model(bool Create)
{
	auto& G = App->CL_Mesh->Group;
	
	if (GroupCount > 0 && App->CL_Model->VerticeCount > 4)
	{
		// Initialize bounding box with the first vertex
		const auto& firstVertex = G[0]->vertex_Data[0];
		BBox_Min = Ogre::Vector3(firstVertex.x, firstVertex.y, firstVertex.z);
		BBox_Max = Ogre::Vector3(firstVertex.x, firstVertex.y, firstVertex.z);

		// Iterate through each group and vertex to find the min and max
		for (int count = 0; count < GroupCount; ++count)
		{
			for (int vertCount = 0; vertCount < G[count]->GroupVertCount; ++vertCount)
			{
				const auto& vertex = G[count]->vertex_Data[vertCount];
				BBox_Min.x = std::min(BBox_Min.x, vertex.x);
				BBox_Min.y = std::min(BBox_Min.y, vertex.y);
				BBox_Min.z = std::min(BBox_Min.z, vertex.z);
				BBox_Max.x = std::max(BBox_Max.x, vertex.x);
				BBox_Max.y = std::max(BBox_Max.y, vertex.y);
				BBox_Max.z = std::max(BBox_Max.z, vertex.z);
			}
		}

		// Calculate size and radius
		BBox_Size.x = std::fabs(BBox_Max.x - BBox_Min.x);
		BBox_Size.y = std::fabs(BBox_Max.y - BBox_Min.y);
		BBox_Size.z = std::fabs(BBox_Max.z - BBox_Min.z);

		BBox_Radius = std::min(BBox_Size.x, BBox_Size.z) / 2.0f;

		// Calculate center
		BBox_Centre.x = (BBox_Min.x + BBox_Max.x) / 2.0f;
		BBox_Centre.y = (BBox_Min.y + BBox_Max.y) / 2.0f;
		BBox_Centre.z = (BBox_Min.z + BBox_Max.z) / 2.0f;
	}

	flag_BoundingBox_Created = true;
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
	
	// Check if the imported Ogre entity and node exist
	if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
	{
		// Detach all objects from the node and destroy the node and entity
		App->CL_Mesh_Mgr->World_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Mesh_Mgr->World_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Mesh_Mgr->World_Ent);

		// Nullify pointers to prevent dangling references
		App->CL_Mesh_Mgr->World_Ent = nullptr;
		App->CL_Mesh_Mgr->World_Node = nullptr;
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
	
	App->CL_Properties_Materials->Reset_Class();
	
	App->CL_Interface->Show_Properties_Panel(false);
	App->CL_Interface->Show_Motions_Dlg(false);
	
	// Reset editor, camera, and panels
	App->CL_Editor_Control->Reset_Editor();

	App->CL_Camera->Reset_View();
	
	App->CL_Interface->Reset_All_Dialogs();
	
	App->CL_Interface->Set_Title(true);
	App->CL_Interface->Set_Menu_Items_Startup();

	
	if (App->CL_Importers->Model_Brush)
	{
		App->CL_Level->Level_RemoveBrush(App->CL_Importers->Model_Brush);
		App->CL_Importers->Model_Brush = NULL;

		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
	}

	//Debug
	//	App->CL_Views_Com->Init_Views(Enums::Selected_Map_View_None);
	////App->CL_Views_Com->Reset_Views_All();
	//Debug

	if (App->CL_Editor_Control->flag_Mode_3DEditor_View == true)
	{
		//App->CL_Editor_Control->Set_3DEditor_View();
	}
	else
	{
		App->CL_Views_Com->Reset_Views_All();
	}

	App->CL_Editor_Control->flag_Just_Loaded = false;

	App->CL_Editor_Control->Editor_Mode = Enums::Editor_Mode_None;

	BBox_Max = Ogre::Vector3::ZERO;
	BBox_Min = Ogre::Vector3::ZERO;
	BBox_Size = Ogre::Vector3::ZERO;
	BBox_Centre = Ogre::Vector3::ZERO;
	BBox_Radius = 0;
	flag_BoundingBox_Created = false;

	if (App->CL_X_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Doc->ResetAllSelectedFaces();
		App->CL_Doc->SelectAll();
		App->CL_Doc->DeleteCurrentThing();
	}

	App->CL_Model->flag_Model_is_Modified = false;
}

