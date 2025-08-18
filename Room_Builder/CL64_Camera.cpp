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
	flag_First_Person = 0;
	flag_Free = 1;

	Saved_Cam_Pos = { 0, 0, 0 };
	Saved_Rotation = {1, 0, 0, 0 };
}

CL64_Camera::~CL64_Camera(void)
{
}

// *************************************************************************
// *			Track_Camera:- Terry and Hazel Flanigan 2024			   *
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
// *			Camera_Look_At:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Camera_Look_At(Ogre::Vector3 Location)
{
	Base_Object* Object = App->CL_Scene->B_Object[App->CL_Properties_Scene->Current_Selected_Object];

	//Tan
	AxisAlignedBox bbox = Object->Object_Node->_getWorldAABB();
	Vector3 offset = bbox.getMaximum() - bbox.getCenter(); // Radius of bounding sphere
	Vector3 center = bbox.getCenter();
	App->CL_Ogre->camNode->setPosition(center);
	App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);

	float oneOverSine = 1.0f / (float)sin(App->CL_Ogre->mCamera->getFOVy().valueRadians() / 2.0); // 1 / sin = adjacent / opposite
	float distanceToCenter = offset.length() * oneOverSine; // (adjacent / opposite) * opposite = adjacent
	
	App->CL_Ogre->camNode->setPosition(center.x, center.y + distanceToCenter, center.z+20);// (Vector3(1, 1, 1).normalisedCopy() * distanceToCenter);
	App->CL_Ogre->camNode->lookAt(Ogre::Vector3(center), Ogre::Node::TS_WORLD);

}

// *************************************************************************
// *			Camera_Reset_Zero:- Terry and Hazel Flanigan 2024		   *
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
	App->CL_Gizmos->Grid_Node->setPosition(0, 0, 0);
	App->CL_Gizmos->Grid_Node->resetOrientation();

	App->CL_Gizmos->Hair_Node->setPosition(0, 0, 0);
	App->CL_Gizmos->Hair_Node->resetOrientation();

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
	if (App->CL_X_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Ogre->OGL_Listener->flag_Render_Ogre = 1;
		App->CL_Ogre->OGL_Listener->flag_Render_Groups = 0;
		App->CL_Ogre->OGL_Listener->flag_Render_Brushes = 0;

		App->CL_Ogre->OGL_Listener->Render_Mode = Enums::Render_Ogre;
		App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Ogre;

		if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
		{
			App->CL_Mesh_Mgr->World_Node->setVisible(true);
		}

		CheckMenuItem(App->Menu_Map, ID_CAMERA_TEXTURED, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(App->Menu_Map, ID_CAMERA_WIREFRAMED, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// *************************************************************************
// *			Camera_Wired:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Camera_Wired(void)
{
	if (App->CL_X_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Ogre->OGL_Listener->flag_Render_Groups = 1;
		App->CL_Ogre->OGL_Listener->flag_Render_Brushes = 0;
		App->CL_Ogre->OGL_Listener->flag_Render_Ogre = 0;

		App->CL_Ogre->OGL_Listener->Render_Mode = Enums::Render_Groups;
		App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Groups;

		if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
		{
			App->CL_Mesh_Mgr->World_Node->setVisible(false);
		}

		CheckMenuItem(App->Menu_Map, ID_CAMERA_WIREFRAMED, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(App->Menu_Map, ID_CAMERA_TEXTURED, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// *************************************************************************
// *			Camera_Brushes:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Camera_Brushes(void)
{
	if (App->CL_X_Brush->Get_Brush_Count() > 0)
	{
		App->CL_Ogre->OGL_Listener->flag_Render_Brushes = 1;
		App->CL_Ogre->OGL_Listener->flag_Render_Groups = 0;
		App->CL_Ogre->OGL_Listener->flag_Render_Ogre = 0;

		App->CL_Ogre->OGL_Listener->Render_Mode = Enums::Render_Brushes;
		App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Brushes;

		if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
		{
			App->CL_Mesh_Mgr->World_Node->setVisible(false);
		}

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

// *************************************************************************
// *		Camera_Save_Location:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Camera::Camera_Save_Location(void)
{
	Saved_Cam_Pos = App->CL_Ogre->camNode->getPosition();
	Saved_Rotation = App->CL_Ogre->camNode->getOrientation();
}

// *************************************************************************
// *		SetCameraMode_FirstPerson:- Terry and Hazel Flanigan 2025  	   *
// *************************************************************************
void CL64_Camera::SetCameraMode_FirstPerson()
{
	// Check for a Player
	if (App->CL_Scene->flag_Player_Added == false)
	{
		App->Say("No Player Added");
		return;
	}

	// Set the camera mode flags
	flag_First_Person = true; 
	flag_Free = false;

	// Update the physics and camera mode in the Ogre3D listener
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = true;
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;

	// Redraw the window to reflect the changes
	RedrawWindow(App->CL_Editor_Scene->Scene_Headers_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			SetCameraMode_Free:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Camera::SetCameraMode_Free()
{
	// Set the camera mode flags
	flag_Free = true;
	flag_First_Person = false;

	// Update the physics and camera mode in the Ogre3D listener
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = false;
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

	// Redraw the window to reflect the changes
	RedrawWindow(App->CL_Editor_Scene->Scene_Headers_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

