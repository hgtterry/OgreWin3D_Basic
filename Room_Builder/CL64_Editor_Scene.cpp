/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
#include "CL64_Editor_Scene.h"

CL64_Editor_Scene::CL64_Editor_Scene()
{
	flag_Scene_Editor_Active = 0;
}

CL64_Editor_Scene::~CL64_Editor_Scene()
{
}

// *************************************************************************
// *			Set_Editor_Scene:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Editor_Scene::Set_Editor_Scene(void)
{
	flag_Scene_Editor_Active = 1;

	App->CL_Top_Tabs->flag_Full_View_3D = 1;
	App->CL_Top_Tabs->flag_View_Top_Left = 0;
	App->CL_Top_Tabs->flag_View_Top_Right = 0;
	App->CL_Top_Tabs->flag_View_Bottom_Left = 0;

	App->CL_Editor_Map->Init_Views(Enums::Selected_View_3D);
	App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

	RECT rcl;

	GetClientRect(App->CL_Editor_Map->Bottom_Right_Hwnd, &rcl);

	SetWindowPos(App->ViewGLhWnd, NULL, 0, 0, rcl.right, rcl.bottom, SWP_NOZORDER);

	App->CL_Ogre->mWindow->windowMovedOrResized();
	App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());

	// Hide Properties window
	App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(false);
	App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 0;

}

// *************************************************************************
// *			Back_To_Map_Editor:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Editor_Scene::Back_To_Map_Editor(void)
{
	flag_Scene_Editor_Active = 0;
}
