/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
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

	flag_Model_Loaded = 0;
	flag_Ogre_Model_Loaded = 0;

	flag_Player_Added = 0;
	flag_Show_Main_Entity = 0;

	// Ogre
	Ogre_Face_Count = 0;

	Scene_Mode = Enums::Scene_Mode_None;

	strcpy(Texture_FolderPath, "None");
	strcpy(FileName, "None");
	strcpy(Path_FileName, "None");
	strcpy(Model_FolderPath, "None");
	strcpy(JustName, "None");

	S_OgreMeshData[0] = new OgreMeshData_Type;

	S_BoundingBox[0] = nullptr;

	int Count = 0;
	while (Count < 99)
	{
		Group[Count] = nullptr;
		Count++;
	}

	Count = 0;
	while (Count < 199)
	{
		S_Bones[Count] = nullptr;
		Count++;
	}
	
	Imported_Ogre_Ent = nullptr;
	Imported_Ogre_Node = nullptr;

	B_Player.reserve(20);
}

CL64_Scene::~CL64_Scene(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Reset_Class(void)
{
	flag_Model_Loaded = 0;
	
	App->CL_Scene->Scene_Mode = Enums::Scene_Mode_None;

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

	//--------------------- Reset Counters and Strings
	Texture_FolderPath[0] = 0;
	GroupCount = 0;
	TextureCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	MotionCount = 0;
	BoneCount = 0;

	strcpy(FileName, "None");
	strcpy(Path_FileName, "None");
	strcpy(Model_FolderPath, "None");
	strcpy(Texture_FolderPath, "None");
	strcpy(JustName, "None");

	flag_Model_Loaded = 0;
}

// *************************************************************************
// *		Clear_Scene_And_Reset:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Scene::Clear_Scene_And_Reset(void)
{
	App->CL_Motions->Reset_Class(); // Call First in case animations in Progress

	App->CL_Scene->flag_Ogre_Model_Loaded = 0;

	// Clear any Mesh Data
	Reset_Class(); // Reset this Class

	Reset_Main_Entity();

	App->CL_Import_Ogre3D->Reset_Class();
	
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Model;
	App->CL_Ogre->Ogre3D_Listener->Run_Physics = 0;
	App->CL_Ogre->Enable_Fog(false);

	App->CL_TopDlg->Reset_Class();

	App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);

	App->CL_Grid->Reset_Class();
	App->CL_Camera->Reset_View();
	App->CL_Bullet->Clear_Trimesh();
	App->CL_ImGui->Reset_Class();

	App->CL_Scene->flag_Ogre_Model_Loaded = 0;
	
	if (App->CL_Converters->World_Ent)
	{
		// Needs Looking At
		App->CL_Ogre->mSceneMgr->destroyAllEntities();
		App->CL_Converters->World_Ent = nullptr;
	}
	
	App->CL_Resources->Reset_Class();
	App->CL_Props_Textures->Reset_Class();
	App->CL_Dialogs->Reset_Class();

	SetWindowText(App->MainHwnd, "OgreWin3D_Assets");
}

// *************************************************************************
// *			Set_Scene:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Set_Scene(int Mode)
{
	// Set up Application and Scene
	
	// Imported Ogre3D Model
	if (Mode == Enums::Scene_Mode_Ogre_Model)
	{
		App->CL_Scene->Scene_Mode = Enums::Scene_Mode_Ogre_Model;

		App->CL_Scene->flag_Ogre_Model_Loaded = 1;

		App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 0;
		App->CL_Scene->flag_Show_Main_Entity = 1;

		return;
	}

	// Imported Assimp Imported Model
	if (Mode == Enums::Scene_Mode_Assimp_Model)
	{
		App->CL_Scene->Scene_Mode = Enums::Scene_Mode_Assimp_Model;

		App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 1;
		App->CL_Scene->flag_Model_Loaded = 1;
		
		return;
	}
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
// *			Reset_Main_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Scene::Reset_Main_Entity(void)
{
	if (Imported_Ogre_Ent && Imported_Ogre_Node)
	{
		Imported_Ogre_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Scene->Imported_Ogre_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Scene->Imported_Ogre_Ent);
		Imported_Ogre_Ent = nullptr;
		Imported_Ogre_Node = nullptr;
	}

	App->CL_Scene->flag_Ogre_Model_Loaded = 0;
}

// *************************************************************************
// *		Main_Entity_Set_Default:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Scene::Main_Entity_Set_Default(void)
{
	Imported_Ogre_Ent = App->CL_Ogre->mSceneMgr->createEntity("Imported_Entity", App->CL_Scene->FileName, App->CL_Resources->Ogre_Loader_Resource_Group);
	Imported_Ogre_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Imported_Ogre_Node->attachObject(App->CL_Scene->Imported_Ogre_Ent);

	Imported_Ogre_Node->setVisible(true);
	Imported_Ogre_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	Imported_Ogre_Node->setPosition(0, 0, 0);
	Imported_Ogre_Node->setScale(1, 1, 1);
}

// *************************************************************************
// *			Show_Main_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Scene::Show_Main_Entity(bool Show)
{
	if (Imported_Ogre_Node)
	{
		if (Show == 1)
		{
			Imported_Ogre_Node->setVisible(true);
			flag_Show_Main_Entity = 1;
		}
		else
		{
			Imported_Ogre_Node->setVisible(false);
			flag_Show_Main_Entity = 0;
		}
	}
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

	if (App->CL_Scene->GroupCount > 0 && App->CL_Scene->VerticeCount > 4)
	{
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
}
