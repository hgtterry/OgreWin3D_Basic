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

CL64_Com_Cameras::CL64_Com_Cameras(void)
{
}

CL64_Com_Cameras::~CL64_Com_Cameras(void)
{

}

// *************************************************************************
// *			Add_New_Camera:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Cameras::Add_New_Camera(void)
{
	int Index = App->CL_Scene->Camera_Count;

	App->CL_Scene->B_Camera[Index] = new Base_Camera();
	strcpy(App->CL_Scene->B_Camera[Index]->Camera_Name, "Camera_0");

	App->CL_Scene->Camera_Count++;
	App->CL_Scene->flag_Camera_Added;

	App->CL_Scene->B_Camera[Index]->FileViewItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Cameras_Folder,
		App->CL_Scene->B_Camera[Index]->Camera_Name, Index, true);

	App->CL_Com_Cameras->Set_Camera(0);

	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Cameras_Folder);
}

// *************************************************************************
// *			Reset_View:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Com_Cameras::Reset_View(void)
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
void CL64_Com_Cameras::Set_Camera_Position(float X, float Y, float Z)
{
	App->CL_Ogre->camNode->setPosition(X, Y, Z);
}

// *************************************************************************
// *		Set_Camera_Rotation:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Cameras::Set_Camera_Rotation(float Pitch, float Yaw, float Roll)
{
	App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);

	App->CL_Ogre->camNode->pitch(Ogre::Degree(Pitch));
	App->CL_Ogre->camNode->yaw(Ogre::Degree(Yaw));
	App->CL_Ogre->camNode->roll(Ogre::Degree(Roll));
}

// *************************************************************************
//			Camera_Goto_Object:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Cameras::Camera_Goto_Object(int Object_Index)
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
		Centre = App->CL_Scene->B_Object[Index]->S_Particle[0]->Particle->getWorldBoundingBox(true).getCenter();
		App->CL_Ogre->camNode->setPosition(Centre);
		return;
	}

	if (App->CL_Properties->Edit_Category == Enums::Edit_Player)
	{
		Centre = App->CL_Scene->B_Player[Index]->Player_Ent->getWorldBoundingBox(true).getCenter();
		App->CL_Ogre->camNode->setPosition(Centre);
		return;
	}

	Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	App->CL_Ogre->camNode->setPosition(Centre);

}

// *************************************************************************
// *	  		Set_Camera:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Com_Cameras::Set_Camera(int Index)
{
	App->CL_Ogre->camNode->setPosition(App->CL_Scene->B_Camera[Index]->CamPos);
	App->CL_Ogre->camNode->setOrientation(App->CL_Scene->B_Camera[Index]->Cam_Quat);
}

// *************************************************************************
// *	  Update_Camera_Entity_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Cameras::Update_Camera_Entity_Data(int Index)
{
	App->CL_Scene->B_Camera[Index]->CamPos = App->CL_Ogre->camNode->getPosition();

	App->CL_Scene->B_Camera[Index]->Cam_Quat = App->CL_Ogre->camNode->getOrientation();

}
