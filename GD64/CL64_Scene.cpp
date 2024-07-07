/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Scene.h"

CL64_Scene::CL64_Scene(void)
{
	// Internal
	GroupCount = 0;
	TextureCount = 0;
	MotionCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	BoneCount = 0;

	Model_Loaded = 0;
	Player_Added = 0;

	// Ogre
	Ogre_Face_Count = 0;

	Model_Type = Enums::LoadedFile_None;
	Scene_Mode = Enums::Scene_Mode_None;

	strcpy(Texture_FolderPath, "No Model Loaded");
	strcpy(FileName, "No Model Loaded");
	strcpy(Path_FileName, "No Model Loaded");
	strcpy(Model_FolderPath, "No Model Loaded");
	strcpy(JustName, "No Model Loaded");

	S_OgreMeshData[0] = new OgreMeshData_Type;

	S_BoundingBox[0] = nullptr;

	int Count = 0;
	while (Count < 99)
	{
		Group[Count] = nullptr;
		Count++;
	}

	Main_Ent = nullptr;
	Main_Node = nullptr;

	B_Player.reserve(20);
}

CL64_Scene::~CL64_Scene(void)
{
}

// *************************************************************************
// *			Reaet_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Reset_Class(void)
{
	Model_Loaded = 0;
	Model_Type = Enums::LoadedFile_None;

	int Count = 0;
	int Index = GroupCount;

	//--------------------- Clear Groups
	while (Count < Index)
	{
		if (Group[Count] != nullptr)
		{
			Group[Count]->vertex_Data.clear();
			Group[Count]->vertex_Data.resize(0);
			Group[Count]->Face_Data.resize(0);
			Group[Count]->Normal_Data.resize(0);
			Group[Count]->MapCord_Data.resize(0);


			if (Group[Count]->Base_Bitmap)
			{
				DeleteObject(Group[Count]->Base_Bitmap);
			}

			delete Group[Count];
			Group[Count] = nullptr;
		}
		Count++;
	}

	Texture_FolderPath[0] = 0;
	GroupCount = 0;
	TextureCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	MotionCount = 0;

}

// *************************************************************************
// *		Create_Mesh_Group:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Scene::Create_Mesh_Group(int Index)
{
	if (Group[Index] != nullptr)
	{
		delete Group[Index];
		Group[Index] = nullptr;
	}

	App->CL_Scene->Group[Index] = new Base_Group();
}

// *************************************************************************
// *			Clear Model:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Clear_Model_And_Reset(void)
{
	Reset_Class(); // Reset this Class

	//App->CL_FileView->Reset_Class(); // Reset List View

	//App->CL_Groups->Reset_Class();

	//App->CL_Grid->Reset_Class();

	//App->CL_TopBar->Reset_Class();

	//App->CL_Ogre->Ogre_Listener->ImGui_Render_Tab = Enums::ImGui_Render_Model;

	//App->CL_Export_Ogre3D->Export_As_RF = 0;

	SetWindowText(App->MainHwnd, "Equity_ME");
}

// *************************************************************************
// *			Reset_Main_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Scene::Reset_Main_Entity(void)
{
	if (Main_Ent && Main_Node)
	{
		Main_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Scene->Main_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Scene->Main_Ent);
		Main_Ent = nullptr;
		Main_Node = nullptr;
	}

	App->CL_Ogre->Ogre3D_Listener->Ogre_Model_Loaded = 0;
}

// *************************************************************************
// *		Main_Entity_Set_Default:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Scene::Main_Entity_Set_Default(void)
{
	Main_Ent = App->CL_Ogre->mSceneMgr->createEntity("UserMesh", App->CL_Scene->FileName, App->CL_Importers->Ogre_Loader_Resource_Group);
	Main_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Main_Node->attachObject(App->CL_Scene->Main_Ent);

	Main_Node->setVisible(true);
	Main_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	Main_Node->setPosition(0, 0, 0);
	Main_Node->setScale(1, 1, 1);
}

// *************************************************************************
// *			Set_Paths:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Set_Paths(void)
{
	strcpy(FileName, App->CL_File_IO->Model_FileName);
	strcpy(Path_FileName, App->CL_File_IO->Model_Path_FileName);

	// Get Texture path assumed at this point to be where model is
	int len1 = strlen(FileName);
	int len2 = strlen(Path_FileName);
	strcpy(Model_FolderPath, Path_FileName);
	Model_FolderPath[len2 - len1] = 0;

	strcpy(Texture_FolderPath, Model_FolderPath); // Back Slash remains


	if (_stricmp(FileName + strlen(FileName) - 5, ".ms3d") == 0)
	{
		strcpy(JustName, FileName);
		int Len = strlen(JustName);
		JustName[Len - 5] = 0;
	}
	else if (_stricmp(FileName + strlen(FileName) - 5, ".G3ds") == 0)
	{
		strcpy(JustName, FileName);
		int Len = strlen(JustName);
		JustName[Len - 5] = 0;
	}
	else if (_stricmp(FileName + strlen(FileName) - 5, ".Wepf") == 0)
	{

	}
	else
	{
		strcpy(JustName, FileName);
		int Len = strlen(JustName);
		JustName[Len - 4] = 0;
	}
}

// *************************************************************************
// *		Create_BondingBox_Model:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Scene::Set_BondingBox_Model(bool Create)
{
	if (Create == 1)
	{
		S_BoundingBox[0] = new AABB_Type;
	}

	S_BoundingBox[0]->BB_Min[0].x = Group[0]->vertex_Data[0].x;
	S_BoundingBox[0]->BB_Min[0].y = Group[0]->vertex_Data[0].y;
	S_BoundingBox[0]->BB_Min[0].z = Group[0]->vertex_Data[0].z;

	S_BoundingBox[0]->BB_Max[0].x = Group[0]->vertex_Data[0].x;
	S_BoundingBox[0]->BB_Max[0].y = Group[0]->vertex_Data[0].y;
	S_BoundingBox[0]->BB_Max[0].z = Group[0]->vertex_Data[0].z;

	int Count = 0;
	int VertCount = 0;

	while (Count < GroupCount)
	{
		VertCount = 0;
		while (VertCount < Group[Count]->GroupVertCount)
		{
			if (Group[Count]->vertex_Data[VertCount].x < S_BoundingBox[0]->BB_Min[0].x) S_BoundingBox[0]->BB_Min[0].x = Group[Count]->vertex_Data[VertCount].x;
			if (Group[Count]->vertex_Data[VertCount].y < S_BoundingBox[0]->BB_Min[0].y) S_BoundingBox[0]->BB_Min[0].y = Group[Count]->vertex_Data[VertCount].y;
			if (Group[Count]->vertex_Data[VertCount].z < S_BoundingBox[0]->BB_Min[0].z) S_BoundingBox[0]->BB_Min[0].z = Group[Count]->vertex_Data[VertCount].z;
			if (Group[Count]->vertex_Data[VertCount].x > S_BoundingBox[0]->BB_Max[0].x) S_BoundingBox[0]->BB_Max[0].x = Group[Count]->vertex_Data[VertCount].x;
			if (Group[Count]->vertex_Data[VertCount].y > S_BoundingBox[0]->BB_Max[0].y) S_BoundingBox[0]->BB_Max[0].y = Group[Count]->vertex_Data[VertCount].y;
			if (Group[Count]->vertex_Data[VertCount].z > S_BoundingBox[0]->BB_Max[0].z) S_BoundingBox[0]->BB_Max[0].z = Group[Count]->vertex_Data[VertCount].z;
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
