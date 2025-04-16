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
#include "CL64_Editor.h"

CL64_Editor_Com::CL64_Editor_Com()
{
	
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

	// Scene
	Object_Count = 0;
	UniqueID_Object_Counter = 0;

	// Ogre
	Ogre_Face_Count = 0;

	JustName[0] = 0;

	Parent_hWnd = NULL;

	flag_PreviewMode_Running = 0;
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

CL64_Editor_Com::~CL64_Editor_Com()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Editor_Com::Reset_Class()
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
}

// *************************************************************************
// *			Create_Brush_XX:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
void CL64_Editor_Com::Create_Brush_XX(int Index)
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
void CL64_Editor_Com::Create_Mesh_Group(int Index)
{
	if (Group[Index] != nullptr)
	{
		delete Group[Index];
		Group[Index] = nullptr;
	}

	Group[Index] = new Base_Group();
}

// *************************************************************************
// *			Preview_Mode:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Editor_Com::Preview_Mode(void)
{
	if (App->CL_Physics->flag_TriMesh_Created == 1)
	{
		App->CL_Physics->Clear_Trimesh();
	}

	if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
	{
		App->CL_Physics->Create_New_Trimesh(App->CL_Mesh_Mgr->World_Ent, App->CL_Mesh_Mgr->World_Node);
		App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 1;

		flag_PreviewMode_Running = 1;

		Parent_hWnd = GetParent(App->CL_Editor_Map->Bottom_Right_Hwnd);

		App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

		App->CL_Ogre->OGL_Listener->Show_Visuals(false);

		//SetCursorPos(App->CursorPosX, App->CursorPosY);

		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(App->CL_Editor_Map->Bottom_Right_Hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		SetWindowPos(App->CL_Editor_Map->Bottom_Right_Hwnd, NULL, 0, 0, cx, cy, SWP_NOZORDER);
		SetParent(App->CL_Editor_Map->Bottom_Right_Hwnd, NULL);

		SetWindowPos(App->ViewGLhWnd, NULL, 0, 0, cx, cy, SWP_NOZORDER);

		App->CL_Ogre->mWindow->resize(cx, cy);

		App->CL_Ogre->mWindow->windowMovedOrResized();
		App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());

		App->CL_ImGui->flag_Show_Press_Excape = 1;
		App->CL_ImGui->flag_Show_Camera_Mode = 1;

		Root::getSingletonPtr()->renderOneFrame();
	}

}

// *************************************************************************
// *				Editor_Mode:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
void CL64_Editor_Com::Editor_Mode(void)
{
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

	flag_PreviewMode_Running = 0;

	if (App->CL_Physics->flag_TriMesh_Created == 1)
	{
		App->CL_Physics->Clear_Trimesh();
		App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;
	}

	App->CL_ImGui->flag_Show_Press_Excape = 0;
	App->CL_ImGui->flag_Show_Camera_Mode = 0;

	SetParent(App->CL_Editor_Map->Bottom_Right_Hwnd, Parent_hWnd);
	
	App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
	
	App->CL_Ogre->OGL_Listener->Show_Visuals(true);

}

// *************************************************************************
// *			Clear_Level:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Editor_Com::Clear_Level(bool FromFile)
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
	
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
	
	if (FromFile == false) // Not from a file load
	{
		App->CL_Editor_Map->Reset_Class();

		App->CL_Doc->Init_Doc();
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

		if (!App->CL_Level->Level_LoadWad(App->CL_Doc->pLevel))
		{
			App->Say_Win("Can not load Wad File");
		}

		App->CL_Properties_Templates->Enable_Insert_Button(true);

		if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
		{
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Model;
		}

		App->CL_Camera->Reset_View_Editor();
	
	}

}
