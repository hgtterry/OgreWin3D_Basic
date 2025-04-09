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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Camera.h"

CL64_Camera::CL64_Camera(void)
{
}

CL64_Camera::~CL64_Camera(void)
{
}

// *************************************************************************
// *			SetBrushes_Fonts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Camera::Track_Camera(void)
{
	if (App->CL_Doc->flag_Track_Camera == 1)
	{
		App->CL_Doc->flag_Track_Camera = 0;
		CheckMenuItem(App->Menu_Map, ID_CAMERA_TRACKCAMERA, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		App->CL_Doc->flag_Track_Camera = 1;
		CheckMenuItem(App->Menu_Map, ID_CAMERA_TRACKCAMERA, MF_BYCOMMAND | MF_CHECKED);
	}

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
}

// *************************************************************************
// *			SetBrushes_Fonts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Camera::Camera_Reset_Zero(void)
{
	App->CL_Ogre->Camera_Reset_Zero();
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
}

// *************************************************************************
// *			Reset_View_Editor:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Camera::Reset_View_Editor(void)
{
	App->CL_Grid->GridNode->setPosition(0, 0, 0);
	App->CL_Grid->GridNode->resetOrientation();

	App->CL_Grid->HairNode->setPosition(0, 0, 0);
	App->CL_Grid->HairNode->resetOrientation();

	App->CL_Ogre->OGL_Listener->RX = 0;
	App->CL_Ogre->OGL_Listener->RZ = 0;

	App->CL_Ogre->camNode->resetOrientation();
	App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 5, 15));
	App->CL_Ogre->camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

}

// *************************************************************************
// *			Camera_Textured:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Camera_Textured(void)
{
	if (App->CL_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Ogre->OGL_Listener->flag_Render_Ogre = 1;
		App->CL_Ogre->OGL_Listener->flag_Render_Groups = 0;
		App->CL_Ogre->OGL_Listener->flag_Render_Brushes = 0;

		App->CL_Ogre->OGL_Listener->Render_Mode = Enums::Render_Nothing;
		App->CL_Mesh_Mgr->World_Node->setVisible(true);

		CheckMenuItem(App->Menu_Map, ID_CAMERA_TEXTURED, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(App->Menu_Map, ID_CAMERA_WIREFRAMED, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// *************************************************************************
// *			Camera_Wired:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Camera_Wired(void)
{
	if (App->CL_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Ogre->OGL_Listener->flag_Render_Groups = 1;
		App->CL_Ogre->OGL_Listener->flag_Render_Brushes = 0;
		App->CL_Ogre->OGL_Listener->flag_Render_Ogre = 0;

		App->CL_Ogre->OGL_Listener->Render_Mode = Enums::Render_Groups;
		App->CL_Mesh_Mgr->World_Node->setVisible(false);

		CheckMenuItem(App->Menu_Map, ID_CAMERA_WIREFRAMED, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(App->Menu_Map, ID_CAMERA_TEXTURED, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// *************************************************************************
// *			Camera_Brushes:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Camera_Brushes(void)
{
	if (App->CL_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Ogre->OGL_Listener->flag_Render_Brushes = 1;
		App->CL_Ogre->OGL_Listener->flag_Render_Groups = 0;
		App->CL_Ogre->OGL_Listener->flag_Render_Ogre = 0;

		App->CL_Ogre->OGL_Listener->Render_Mode = Enums::Render_Brushes;
		App->CL_Mesh_Mgr->World_Node->setVisible(false);

		CheckMenuItem(App->Menu_Map, ID_CAMERA_WIREFRAMED, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(App->Menu_Map, ID_CAMERA_TEXTURED, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// *************************************************************************
// *			Camera_Speed_Normal:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Camera::Camera_Speed_Normal(void)
{
	App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 50;
	App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 50;

	Camera_Set_Menu_Clear();
	CheckMenuItem(App->Menu_Map, ID_SPEED_NORMAL, MF_BYCOMMAND | MF_CHECKED);
}

// *************************************************************************
// *			Camera_Speed_Slow:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Camera::Camera_Speed_Slow(void)
{
	App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 5;
	App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 20;

	Camera_Set_Menu_Clear();
	CheckMenuItem(App->Menu_Map, ID_SPEED_SLOW, MF_BYCOMMAND | MF_CHECKED);
}

// *************************************************************************
// *		Camera_Speed_Very_Slow:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Camera::Camera_Speed_Very_Slow(void)
{
	App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 1;
	App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 1;

	Camera_Set_Menu_Clear();
	CheckMenuItem(App->Menu_Map, ID_SPEED_VERYSLOW, MF_BYCOMMAND | MF_CHECKED);
}

// *************************************************************************
// *			Camera_Speed_Normal:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Camera::Camera_Speed_Fast(void)
{
	App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity = 250;
	App->CL_Ogre->Ogre3D_Listener->mMoveSensitivityMouse = 250;

	Camera_Set_Menu_Clear();
	CheckMenuItem(App->Menu_Map, ID_SPEED_FAST, MF_BYCOMMAND | MF_CHECKED);
}

// *************************************************************************
// *		Camera_Set_Menu_Clear:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Camera::Camera_Set_Menu_Clear(void)
{
	CheckMenuItem(App->Menu_Map, ID_SPEED_NORMAL, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(App->Menu_Map, ID_SPEED_SLOW, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(App->Menu_Map, ID_SPEED_VERYSLOW, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(App->Menu_Map, ID_SPEED_FAST, MF_BYCOMMAND | MF_UNCHECKED);
}




