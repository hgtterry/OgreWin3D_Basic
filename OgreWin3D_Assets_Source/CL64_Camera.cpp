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
	//App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 5, 15));
	//App->CL_Ogre->camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 35, 60));
	App->CL_Ogre->camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	if (App->CL_Scene->flag_Ogre_Model_Loaded == 1)
	{
		App->CL_Scene->Imported_Ogre_Node->setOrientation(Ogre::Quaternion::IDENTITY);
		App->CL_Scene->Imported_Ogre_Node->setPosition(0, 0, 0);
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
