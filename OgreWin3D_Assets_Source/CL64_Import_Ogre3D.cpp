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
#include "CL64_Import_Ogre3D.h"

#pragma warning(disable:4996)

CL64_Imp_Ogre3D::CL64_Imp_Ogre3D(void)
{
	flag_Ogre_Model_Loaded = 0;
	App->CL_Scene->S_OgreMeshData[0]->mStrName = "No Model Loaded";
	App->CL_Scene->S_OgreMeshData[0]->mFileName_Str = "No Model Loaded";
}

CL64_Imp_Ogre3D::~CL64_Imp_Ogre3D(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Imp_Ogre3D::Reset_Class(void)
{
	// Clear Stored data about any loaded Ogre Models
	if (App->CL_Scene->S_OgreMeshData[0])
	{
		delete App->CL_Scene->S_OgreMeshData[0];
		App->CL_Scene->S_OgreMeshData[0] = nullptr;

		App->CL_Scene->S_OgreMeshData[0] = new OgreMeshData_Type;

		App->CL_Scene->S_OgreMeshData[0]->m_Motion_Names.resize(0);

		App->CL_Scene->S_OgreMeshData[0]->mStrName = "No Model Loaded";
		App->CL_Scene->S_OgreMeshData[0]->mFileName_Str = "No Model Loaded";

		App->CL_Scene->S_OgreMeshData[0]->mSubMeshCount = 0;
		App->CL_Scene->S_OgreMeshData[0]->Area = 0;
		App->CL_Scene->S_OgreMeshData[0]->Depth = 0;
		App->CL_Scene->S_OgreMeshData[0]->Height = 0;
		App->CL_Scene->S_OgreMeshData[0]->Width = 0;
		App->CL_Scene->S_OgreMeshData[0]->Volume = 0;
		App->CL_Scene->S_OgreMeshData[0]->Radius = 0;

		App->CL_Scene->S_OgreMeshData[0]->Center = (Ogre::Vector3::ZERO);
		App->CL_Scene->S_OgreMeshData[0]->vMin = (Ogre::Vector3::ZERO);
		App->CL_Scene->S_OgreMeshData[0]->vMax = (Ogre::Vector3::ZERO);

		flag_Ogre_Model_Loaded = 0;
		
	}
}

