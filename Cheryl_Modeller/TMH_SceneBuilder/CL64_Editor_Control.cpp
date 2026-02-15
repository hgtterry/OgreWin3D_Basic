/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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

	flag_Map_Editor_Active = false;
	flag_Scene_Editor_Active = false;
	flag_Scene_Game_Running = false;

	flag_Mode_3DEditor_View = false;
	flag_Mode_Map_View = false;

	flag_Start_3D_View = true;
}

CL64_Editor_Control::~CL64_Editor_Control(void)
{
}

// *************************************************************************
// *			Reset_Editor:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Editor_Control::Reset_Editor(void)
{
	App->CL_Model->Model_Type = Enums::Model_Type_None;
	App->CL_Model->flag_Model_Loaded = false;

	auto& p_OGL_Rend = App->CL_Ogre->OGL_Listener; // Pointer-> App->CL_Ogre->OGL_Listener
	p_OGL_Rend->flag_ShowFaces = false;
	p_OGL_Rend->flag_ShowPoints = false;
	p_OGL_Rend->flag_ShowNormals = false;
	p_OGL_Rend->flag_ShowBoundingBox = false;
	p_OGL_Rend->flag_ShowBones = false;

	App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 5, 15));
	App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);
	App->CL_Ogre->camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	//App->CL_Editor_Map->Reset_Views_All();

	App->CL_Gizmos->flag_Show_Hair = true;
	App->CL_Gizmos->flag_ShowGrid = true;

	App->CL_Gizmos->Hair_SetVisible(true);
	App->CL_Gizmos->Grid_SetVisible(true);

}

// *************************************************************************
// *	Start_Editor_MapBrush_Mode:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Editor_Control::Start_Editor_MapBrush_Mode(void)
{
	EnableWindow(GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BT_HD_PREVIEW), true);
	
	App->CL_Dimensions->flag_Show_Dimensions = false;
	
	App->CL_Ogre->Listener_3D->flag_Run_Physics = 0;
	App->CL_Ogre->Listener_3D->CameraMode = Enums::Cam_Mode_Free;

	flag_Map_Editor_Active = true;

	App->CL_ImGui->flag_Show_Press_Excape = false;
	App->CL_Gizmos->Enable_Grid_And_Hair(true);
	App->CL_Gizmos->highlight(App->CL_Scene->B_Object[App->CL_Gizmos->Last_Selected_Object]->Object_Ent);
   // App->CL_Com_Objects->Show_Entities(true);

	SetParent(App->CL_Editor_Map->Bottom_Ogre_Right_Hwnd, Parent_hWnd);

	App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
	App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 1;
    
	
	if (App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes) > 0)
	{
		App->CL_Ogre->OGL_Listener->Show_Visuals(true);
	}

	if (App->CL_Editor_Map->flag_Environment_On == false)
	{
		//App->CL_Com_Environments->Set_Environment_By_Index(false, -1);
	}
}

// *************************************************************************
// *			Start_Editor_Scene:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Editor_Control::Start_Editor_Scene()
{
	flag_Scene_Editor_Active = true;
	flag_Map_Editor_Active = false;

	App->CL_Dimensions->flag_Show_Dimensions = App->CL_Dimensions->flag_Show_Dimensions_Copy;

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
	App->CL_Panels->Move_FileView_Window();
	App->CL_Panels->Resize_FileView();
	App->CL_Panels->Position_Properties_Dlg();
	
	//App->CL_Com_Objects->Show_Entities(true);
	
	App->CL_ImGui_Editor->flag_Block_GUI = false;
}

// *************************************************************************
// *		Set_Map_Editor_Startup:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Editor_Control::Set_Map_Editor_Startup()
{
	flag_Map_Editor_Active = true;

	App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_Ogre];

	if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_3D)
	{
		App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_3D);
	}

	App->CL_Gizmos->Reset_Grid_And_Hair();

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
}

// *************************************************************************
// *			Set_Map_View:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Editor_Control::Set_Map_View()
{

	App->CL_Top_Tabs->flag_Full_View_3D = 0;
	App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);
	App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

	flag_Mode_3DEditor_View = false;
	flag_Mode_Map_View = true;

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
}

// *************************************************************************
// *			Set_3DEditor_View:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Editor_Control::Set_3DEditor_View()
{
	App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_3D);
	App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_3D);
	App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

	flag_Mode_3DEditor_View = true;
	flag_Mode_Map_View = false;

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
}
