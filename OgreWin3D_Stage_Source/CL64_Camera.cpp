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
#include "CL64_Camera.h"

CL64_Camera::CL64_Camera(void)
{
}

CL64_Camera::~CL64_Camera(void)
{

}

// *************************************************************************
// *			Reset_View:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Camera::Reset_View(void)
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

	if (App->CL_Ogre->Ogre3D_Listener->flag_Ogre_Model_Loaded == 1)
	{
		//App->CL_Scene->Imported_Ogre_Node->setOrientation(Ogre::Quaternion::IDENTITY);
		//App->CL_Scene->Imported_Ogre_Node->setPosition(0, 0, 0);
	}
}

// *************************************************************************
// *		Set_Camera_Position:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Set_Camera_Position(float X, float Y, float Z)
{
	App->CL_Ogre->camNode->setPosition(X, Y, Z);
}

// *************************************************************************
// *		Set_Camera_Rotation:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Set_Camera_Rotation(float Pitch, float Yaw, float Roll)
{
	App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);

	App->CL_Ogre->camNode->pitch(Ogre::Degree(Pitch));
	App->CL_Ogre->camNode->yaw(Ogre::Degree(Yaw));
	App->CL_Ogre->camNode->roll(Ogre::Degree(Roll));
}

// *************************************************************************
//			Camera_Goto_Object:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Camera::Camera_Goto_Object(int Object_Index)
{
	App->CL_TopDlg->Camera_Set_Free();

	Ogre::Vector3 WS;
	Ogre::Vector3 WS2;
	Ogre::Vector3 Centre;
	Ogre::Vector3 CentreNode;
	Ogre::Vector3 Size;

	int Index = Object_Index;

	if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
	{
		Centre = App->CL_Scene->B_Area[Index]->Area_Ent->getWorldBoundingBox(true).getCenter();
		App->CL_Ogre->camNode->setPosition(Centre);
		return;
	}

	if (App->CL_Properties->Edit_Category == Enums::Edit_Particles)
	{
		Centre = App->CL_Scene->V_Object[Index]->S_Particle[0]->Particle->getWorldBoundingBox(true).getCenter();
		App->CL_Ogre->camNode->setPosition(Centre);
		return;
	}

	if (App->CL_Properties->Edit_Category == Enums::Edit_Player)
	{
		Centre = App->CL_Scene->B_Player[Index]->Player_Ent->getWorldBoundingBox(true).getCenter();
		App->CL_Ogre->camNode->setPosition(Centre);
		return;
	}

	Centre = App->CL_Scene->V_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	App->CL_Ogre->camNode->setPosition(Centre);

}
