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
#include "CL64_Scene.h"

CL64_Scene::CL64_Scene()
{
	
	flag_Project_Resources_Created = 0;

	// Groups
	GroupCount = 0;
	VerticeCount = 0;
	FaceCount = 0;

	// Brushes
	BrushCount = 0;
	Brush_Face_Count = 0;

	// Player
	Player_Count = 0;
	flag_Player_Added = 0;

	// Scene Objects
	Object_Count = 0;
	UniqueID_Object_Counter = 0;

	// Locations
	Location_Count = 0;

	// Ogre
	Ogre_Face_Count = 0;

	JustName[0] = 0;

	flag_Show_Debug_Area = 0;
	flag_Enable_Physics_Debug = 0;
	

	int Count = 0;
	while (Count < 11999)
	{
		B_Brush[Count] = nullptr;
		Count++;
	}

	Count = 0;
	while (Count < 4999)
	{
		Group[Count] = nullptr;
		Count++;
	}

	B_Player.reserve(20);
	B_Object.reserve(200);

}

CL64_Scene::~CL64_Scene()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Reset_Class()
{
	int Count = 0; // Remove Ogre Objects
	while (Count < Object_Count)
	{
		if (B_Object[Count]->Object_Node && B_Object[Count]->Object_Ent)
		{
			B_Object[Count]->Object_Node->detachAllObjects();

			App->CL_Ogre->mSceneMgr->destroySceneNode(B_Object[Count]->Object_Node);

			App->CL_Ogre->mSceneMgr->destroyEntity(B_Object[Count]->Object_Ent);

			B_Object[Count]->Object_Node = nullptr;
			B_Object[Count]->Object_Ent = nullptr;
		}

		Count++;
	}

	// Remove B_Objects
	Count = 0;
	int NumObjects = B_Object.size();

	while (Count < NumObjects)
	{
		delete B_Object[Count];
		B_Object[Count] = nullptr;
		Count++;
	}

	B_Object.resize(0);

	Object_Count = 0;
	UniqueID_Object_Counter = 0;

	Location_Count = 0;
}

// *************************************************************************
// *			Create_Brush_XX:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
void CL64_Scene::Create_Brush_XX(int Index)
{
	if (B_Brush[Index] != nullptr)
	{
		delete B_Brush[Index];
		B_Brush[Index] = nullptr;
	}

	B_Brush[Index] = new Base_Brush();

	B_Brush[Index]->Vertice_Count = 0;
	B_Brush[Index]->Face_Count = 0;
}

// *************************************************************************
// *		Create_Mesh_Group:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
void CL64_Scene::Create_Mesh_Group(int Index)
{
	if (Group[Index] != nullptr)
	{
		delete Group[Index];
		Group[Index] = nullptr;
	}

	Group[Index] = new Base_Group();
}

// *************************************************************************
// *			Clear_Level:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Clear_Level(bool FromFile)
{
	App->CL_Ogre->OGL_Listener->Show_Visuals(false);

	Reset_Class(); // This Class
	
	App->CL_FileView->Reset_Class();
	
	App->CL_Doc->ResetAllSelectedFaces();
	App->CL_Doc->SelectAll();
	App->CL_Doc->DeleteCurrentThing();
	App->CL_Ogre->Camera_Reset_Zero();
	
	App->CL_Properties_Textures->Reset_Class();
	App->CL_Properties_Brushes->Reset_Class();
	App->CL_Properties_Tabs->Reset_Class();
	App->CL_Top_Tabs->Reset_Class();
	App->CL_Mesh_Mgr->Reset_Class();
	
	App->CL_Level->flag_Working_Folder_Exists = 0;
	strcpy(App->CL_Level->Prj_Working_Folder, "None");

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
	App->CL_Gizmos->Show_MarkerBox(false);

	App->CL_Resources->Delete_Project_Resources_Group();
	flag_Project_Resources_Created = 0;

	// Bullet Related
	int i;
	int CO = App->CL_Physics->dynamicsWorld->getNumCollisionObjects();
	if (CO > 0)
	{
		for (i = App->CL_Physics->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = App->CL_Physics->dynamicsWorld->getCollisionObjectArray()[i];

			if (obj)
			{
				if (obj->getUserIndex() == Enums::Obj_Usage_Player)
				{

				}
				else
				{
					App->CL_Physics->dynamicsWorld->removeCollisionObject(obj);
					//delete obj;
				}
			}
		}
	}

	if (FromFile == false) // Not from a file load
	{
		App->CL_Editor_Map->Reset_Class();

		App->CL_Doc->Init_Doc();
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

		if (!App->CL_Level->Level_Create_TXL_Class())
		{
			App->Say_Win("Can not create TXL Class");
		}

		App->CL_Properties_Templates->Enable_Insert_Button(true);

		if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
		{
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Model;
		}

		App->CL_Camera->Reset_View_Editor();

	}
}
