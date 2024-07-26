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
#include "OW3D_Motioins.h"

OW3D_Motioins::OW3D_Motioins(void)
{
}

OW3D_Motioins::~OW3D_Motioins(void)
{
}

// *************************************************************************
// *		Play_SelectedMotion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void OW3D_Motioins::Play_SelectedMotion(void)
{
	if (App->CL_Ogre->TestMesh_Entity)
	{
		App->CL_Ogre->Ogre3D_Listener->Animate_State = App->CL_Ogre->TestMesh_Entity->getAnimationState(App->CL_TopDlg->Selected_Motion_Name);
		App->CL_Ogre->Ogre3D_Listener->Animate_State->setEnabled(true);
		App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 1;
	}

	if (App->CL_Scene->Imported_Ogre_Ent)
	{
		App->CL_Ogre->Ogre3D_Listener->Animate_State = App->CL_Scene->Imported_Ogre_Ent->getAnimationState(App->CL_TopDlg->Selected_Motion_Name);
		App->CL_Ogre->Ogre3D_Listener->Animate_State->setEnabled(true);
		App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 1;
	}
	
}

// *************************************************************************
// *		Stop_SelectedMotion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void OW3D_Motioins::Stop_SelectedMotion(void)
{
	if (App->CL_Ogre->TestMesh_Entity)
	{
		App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 0;
		App->CL_Ogre->Ogre3D_Listener->Animate_State->setEnabled(false);
	}

	if (App->CL_Scene->Imported_Ogre_Ent)
	{
		App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 0;
		App->CL_Ogre->Ogre3D_Listener->Animate_State->setEnabled(false);
	}
}
