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

CL64_Editor::CL64_Editor()
{
	// Groups
	GroupCount = 0;
	VerticeCount = 0;
	FaceCount = 0;

	// Brushes
	BrushCount = 0;
	Brush_Face_Count = 0;

	JustName[0] = 0;

	Parent_hWnd = NULL;

	flag_PreviewMode_Running = 0;

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

}

CL64_Editor::~CL64_Editor()
{
}

// *************************************************************************
// *			Create_Brush_XX:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
void CL64_Editor::Create_Brush_XX(int Index)
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
void CL64_Editor::Create_Mesh_Group(int Index)
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
void CL64_Editor::Preview_Mode(void)
{
	/*if (App->CL_Build_Game->flag_Use_Front_Dlg == 1)
	{
		if (App->CL_Dialogs->flag_Game_Start_Option == Enums::Game_Start_Full)
		{
			App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 1;
			App->CL_Keyboard->flag_Block_Keyboard = 1;
			App->flag_Block_Mouse_Buttons = 1;
			App->CL_Front_Dialog->flag_Show_Front_Dlg_Flag = 1;
		}
		else
		{
			SetCapture(App->ViewGLhWnd);
			App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
			App->CUR = SetCursor(NULL);
			App->CL_Front_Dialog->flag_Game_Running_Flag = 1;
		}
	}

	App->CL_ImGui->flag_Show_FPS = App->CL_Build_Game->flag_Show_FPS;*/

	flag_PreviewMode_Running = 1;

	//App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;

	/*App->CL_Grid->Grid_SetVisible(false);
	App->CL_Grid->Hair_SetVisible(false);
	App->CL_Gizmos->Show_MarkerBox(false);*/

	//App->CL_Com_Environments->Set_Environment_GameMode(true);

	//App->CL_Ogre->Ogre3D_Listener->Run_Physics = 1;

	//CurrentCamMode = App->CL_Ogre->Ogre3D_Listener->CameraMode;

	//App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;

	//Show_Entities(false); // Hide All Visible Trigers

	Parent_hWnd = GetParent(App->CL_MapEditor->Bottom_Right_Hwnd);

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	SetCursorPos(App->CursorPosX, App->CursorPosY);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(App->CL_MapEditor->Bottom_Right_Hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos(App->CL_MapEditor->Bottom_Right_Hwnd, NULL, 0, 0, cx, cy, SWP_NOZORDER);
	SetParent(App->CL_MapEditor->Bottom_Right_Hwnd, NULL);

	App->CL_Ogre->mWindow->resize(cx, cy);

	App->CL_Ogre->mWindow->windowMovedOrResized();
	App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());

	App->CL_ImGui->flag_Show_Press_Excape = 1;

	Root::getSingletonPtr()->renderOneFrame();

	/*if (App->CL_Build_Game->flag_Use_Front_Dlg == 0)
	{
		SetCapture(App->ViewGLhWnd);
		App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
		App->CUR = SetCursor(NULL);
	}

	App->CL_Physics->Reset_Triggers();

	App->CL_ImGui_Dialogs->flag_Show_Physics_Console = 0;
	App->CL_Com_Player->Show_Player_And_Physics(false);*/

}

// *************************************************************************
// *				Editor_Mode:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
void CL64_Editor::Editor_Mode(void)
{
	flag_PreviewMode_Running = 0;

	App->CL_ImGui->flag_Show_Press_Excape = 0;

	SetParent(App->CL_MapEditor->Bottom_Right_Hwnd, Parent_hWnd);
	
	App->CL_MapEditor->Init_Views(Enums::Selected_View_None);
	App->CL_MapEditor->Resize_Windows(App->CL_MapEditor->Main_Dlg_Hwnd, App->CL_MapEditor->nleftWnd_width, App->CL_MapEditor->nleftWnd_Depth);

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
	
	//App->CL_Front_Dialog->flag_Show_Front_Dlg_Flag = 0; // temp

	//flag_GameMode_Running = 0;
	//flag_FullScreen_Mode = 0;

	////App->CL_Ogre->BulletListener->Render_Debug_Flag = 1;

	//App->CL_Grid->Grid_SetVisible(true);
	//App->CL_Grid->Hair_SetVisible(true);

	//App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
	//ReleaseCapture();
	//SetCursor(App->CUR);

	//if (App->CL_Scene->flag_Scene_Loaded == 1)
	//{
	//	App->CL_Com_Environments->Set_Environment_GameMode(false);

	//	Show_Entities(true); // Show All Visible Trigers

	//	App->CL_Physics->Reset_Triggers();
	//}


	//App->CL_Ogre->Ogre3D_Listener->CameraMode = CurrentCamMode;

	//if (CurrentCamMode == Enums::Cam_Mode_Free)
	//{
	//	App->CL_Com_Player->Show_Player_And_Physics(true);
	//}
	//else
	//{
	//	App->CL_Com_Player->Show_Player_And_Physics(false);
	//}

	//App->CL_ImGui->flag_Show_FPS = App->CL_Build_Game->flag_Saved_Show_FPS;

	//App->CL_ImGui_Dialogs->flag_Show_Physics_Console = 1;

}
