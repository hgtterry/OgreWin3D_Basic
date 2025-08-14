/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

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
#include "CL64_Editor_Control.h"

CL64_Editor_Control::CL64_Editor_Control(void)
{
	Parent_hWnd = nullptr;

	flag_PreviewMode_Active = 0;
	flag_Scene_Editor_Active = 0;
}

CL64_Editor_Control::~CL64_Editor_Control(void)
{
}

// *************************************************************************
// *			Start_Preview_Mode:- Terry and Hazel Flanigan 2025	  	   *
// *************************************************************************
void CL64_Editor_Control::Start_Preview_Mode(void)
{
	if (App->CL_Scene->flag_Player_Added == false)
	{
		App->Say("No Player Added");
		return;
	}

    App->CL_ImGui->Close_Dialogs();
	App->CL_ImGui_Editor->flag_Block_GUI = true;
    // Clear existing trimesh if it has been created
    if (App->CL_Physics->flag_TriMesh_Created)
    {
        App->CL_Physics->Clear_Trimesh();
    }

    // Check if World Entity and Node are valid
    if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
    {
        // Create a new trimesh and set debug rendering flag
        App->CL_Physics->Create_New_Trimesh(App->CL_Mesh_Mgr->World_Ent, App->CL_Mesh_Mgr->World_Node);
        App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = true;

        flag_PreviewMode_Active = true;

        // Get the parent window handle
        Parent_hWnd = GetParent(App->CL_Editor_Map->Bottom_Ogre_Right_Hwnd);

        // Disable property tabs and hide visuals
        App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
        App->CL_Ogre->OGL_Listener->Show_Visuals(false);
        App->CL_Com_Objects->Show_Entities(false);
        App->CL_Gizmos->Enable_Grid_And_Hair(false);
        App->CL_Gizmos->Show_MarkerBox(false);
        App->CL_Gizmos->unhighlight(App->CL_Scene->B_Object[App->CL_Gizmos->Last_Selected_Object]->Object_Ent);

        // Get screen dimensions
        int cx = GetSystemMetrics(SM_CXSCREEN);
        int cy = GetSystemMetrics(SM_CYSCREEN);

        // Set window positions and sizes
        SetWindowPos(App->CL_Editor_Map->Bottom_Ogre_Right_Hwnd, HWND_TOP, 0, 0, cx, cy, SWP_NOZORDER);
        SetParent(App->CL_Editor_Map->Bottom_Ogre_Right_Hwnd, NULL);
        SetWindowPos(App->ViewGLhWnd, NULL, 0, 0, cx, cy, SWP_NOZORDER);

        // Resize the Ogre window
        App->CL_Ogre->mWindow->resize(cx, cy);
        App->CL_Ogre->mWindow->windowMovedOrResized();
        App->CL_Ogre->mCamera->setAspectRatio(static_cast<Ogre::Real>(cx) / static_cast<Ogre::Real>(cy));

        // Set flags for ImGui
        App->CL_ImGui->flag_Show_Press_Excape = true;
        App->CL_ImGui->flag_Show_Preview_Options = true;

        // Reset the physics scene and render the frame
       // App->CL_Physics->Reset_Scene(true);
        App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = false;
        App->CL_Ogre->RenderFrame(1);

        // Stop all sounds and set environment mode
        App->CL_SoundMgr->SoundEngine->stopAllSounds();
        App->CL_Com_Environments->Set_Environment_GameMode();
    }
}

// *************************************************************************
// *	Start_Editor_MapBrush_Mode:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Editor_Control::Start_Editor_MapBrush_Mode(void)
{
	App->CL_SoundMgr->SoundEngine->stopAllSounds();

	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

	flag_PreviewMode_Active = false;

	if (App->CL_Physics->flag_TriMesh_Created == 1)
	{
		App->CL_Physics->Clear_Trimesh();
		App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;
	}

	App->CL_ImGui->flag_Show_Press_Excape = false;
	App->CL_ImGui->flag_Show_Preview_Options = false;
	App->CL_Gizmos->Enable_Grid_And_Hair(true);
	App->CL_Gizmos->highlight(App->CL_Scene->B_Object[App->CL_Gizmos->Last_Selected_Object]->Object_Ent);
    App->CL_Com_Objects->Show_Entities(true);

	SetParent(App->CL_Editor_Map->Bottom_Ogre_Right_Hwnd, Parent_hWnd);

	App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
	App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 1;
    
	
	if (App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes) > 0)
	{
		App->CL_Ogre->OGL_Listener->Show_Visuals(true);
	}

	if (App->CL_Editor_Map->flag_Environment_On == false)
	{
		App->CL_Com_Environments->Set_Environment_By_Index(false, -1);
	}
}

// *************************************************************************
// *		Return_To_Map_Editor:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Editor_Control::Return_To_Map_Editor(void)
{
	App->CL_SoundMgr->SoundEngine->stopAllSounds();

	// Turn off Editor Dialogs and Gizmos
	App->CL_FileView->Show_FileView(false);
	App->CL_Properties_Scene->Show_Properties_Scene(false);
	App->CL_Gui_Environment->PropertyEditor_Page = false;
	App->CL_Gui_Environment->flag_Show_PropertyEditor = false;
	App->CL_Gizmos->Show_MarkerBox(false);
	App->CL_ImGui_Editor->flag_Block_GUI = true;

	// Reset Flags
	App->CL_Editor_Control->flag_Scene_Editor_Active = false;
	App->CL_Top_Tabs->flag_Full_View_3D = false;

	// Show top tabs and configure editor map
	App->CL_Editor_Scene->Show_Headers(false);
	App->CL_Top_Tabs->Show_TopTabs(true);
	App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);
	App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

	// Show properties tabs
	App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(true);
	App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 1;

	// Set menu to map
	SetMenu(App->MainHwnd, App->Menu_Map);

	// Set Visuals
	App->CL_Com_Objects->Show_Entities(true);
	App->CL_Ogre->OGL_Listener->Show_Visuals(true);

	// Set environment if not active
	if (App->CL_Editor_Map->flag_Environment_On == false)
	{
		App->CL_Com_Environments->Set_Environment_By_Index(false, -1);
	}

	if (App->CL_Physics->flag_TriMesh_Created == 1)
	{
		App->CL_Physics->Clear_Trimesh();
		App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;
	}
}

// *************************************************************************
// *			Start_Editor_Scene:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Editor_Control::Start_Editor_Scene()
{
	App->CL_Editor_Control->flag_Scene_Editor_Active = true;

	// Handle physics and trimesh
	if (App->CL_Physics->flag_TriMesh_Created)
	{
		App->CL_Physics->Clear_Trimesh();
	}

	if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
	{
		App->CL_Physics->Create_New_Trimesh(App->CL_Mesh_Mgr->World_Ent,
			App->CL_Mesh_Mgr->World_Node);
		App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = true;
	}

	// Set view flags
	auto& topTabs = App->CL_Top_Tabs;
	topTabs->flag_Full_View_3D = true;
	topTabs->flag_View_Top_Left = false;
	topTabs->flag_View_Top_Right = false;
	topTabs->flag_View_Bottom_Left = false;

	// Set 3D View to Full View
	App->CL_Editor_Map->Set_3D_FullView();
	
	// Hide visuals and tabs
	App->CL_Ogre->OGL_Listener->Show_Visuals(false);
	topTabs->Show_TopTabs(false);
	App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(false);
	App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = false;
	App->CL_Ogre->OGL_Listener->Show_Visuals(false);
	
	// Show headers and file view
	App->CL_Editor_Scene->Show_Headers(true);
	App->CL_FileView->Show_FileView(true);
	App->CL_Panels->Move_FileView_Window();
	App->CL_Panels->Resize_FileView();
	App->CL_Panels->Place_Properties_Dlg();
	App->CL_Properties_Scene->Show_Properties_Scene(true);
	
	// Set menu
	SetMenu(App->MainHwnd, App->Menu_Scene);

	App->CL_Com_Objects->Show_Entities(true);
	
	App->CL_Gizmos->MarkerBox_Adjust(App->CL_Properties_Scene->Current_Selected_Object);

	App->CL_ImGui_Editor->flag_Block_GUI = false;
}

// *************************************************************************
// *		Set_Map_Editor_Select_Dlg:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Editor_Control::Set_Map_Editor_Select_Dlg()
{
	//if (App->CL_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Doc->Set_Tool_GeneralSelect();

		App->CL_Top_Tabs->Enable_TopBar_Brush_Buttons(false, false);
		App->CL_Top_Tabs->Enable_TopBar_Face_Buttons(false);
		App->CL_Top_Tabs->Enable_Select_Button(true, true);

		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

		return;
	}
}
