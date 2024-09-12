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
#include "CL64_Dimensions.h"

CL64_Dimensions::CL64_Dimensions(void)
{
}

CL64_Dimensions::~CL64_Dimensions(void)
{
}

// *************************************************************************
// *	  Centre_Model_Mid_Assimp:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dimensions::Centre_Model_Mid_Assimp(void)
{
	//if (App->CL_Scene->flag_Model_Loaded == 1)
	if (App->CL_Scene->GroupCount  > 0)
	{
		float X = -App->CL_Scene->S_BoundingBox[0]->Centre->x;
		float Y = -App->CL_Scene->S_BoundingBox[0]->Centre->y;
		float Z = -App->CL_Scene->S_BoundingBox[0]->Centre->z;

		int Count = 0;
		int VertCount = 0;
		int GroupCount = App->CL_Scene->GroupCount;

		while (Count < GroupCount)
		{
			VertCount = 0;
			while (VertCount < App->CL_Scene->Group[Count]->GroupVertCount)
			{
				App->CL_Scene->Group[Count]->vertex_Data[VertCount].x += X;
				App->CL_Scene->Group[Count]->vertex_Data[VertCount].y += Y;
				App->CL_Scene->Group[Count]->vertex_Data[VertCount].z += Z;

				VertCount++;
			}

			Count++;
		}

		App->CL_Scene->Set_BondingBox_Model(0);

	}
}

// *************************************************************************
// *	  		Rotate_Z_Assimp:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Dimensions::Rotate_Z_Assimp(float Z)
{
	//if (App->CL_Scene->flag_Model_Loaded == 1)
	if (App->CL_Scene->GroupCount > 0)
	{

		Ogre::Vector3 Centre;

		Centre.x = App->CL_Scene->S_BoundingBox[0]->Centre->x;
		Centre.y = App->CL_Scene->S_BoundingBox[0]->Centre->y;
		Centre.z = App->CL_Scene->S_BoundingBox[0]->Centre->z;

		Ogre::Vector3 Rotate;
		Rotate.x = 0;
		Rotate.y = 0;
		Rotate.z = Z;


		int Count = 0;
		int VertCount = 0;

		int GroupCount = App->CL_Scene->GroupCount;

		while (Count < GroupCount)
		{
			VertCount = 0;
			while (VertCount < App->CL_Scene->Group[Count]->GroupVertCount)
			{
				Ogre::Vector3 VertPos;
				Ogre::Vector3 RotatedVert;

				VertPos.x = App->CL_Scene->Group[Count]->vertex_Data[VertCount].x;
				VertPos.y = App->CL_Scene->Group[Count]->vertex_Data[VertCount].y;
				VertPos.z = App->CL_Scene->Group[Count]->vertex_Data[VertCount].z;

				if (Rotate.z != 0) // Dont bother if Zero
				{
					RotatedVert = (Ogre::Quaternion(Ogre::Degree(Rotate.z), Ogre::Vector3::UNIT_X) * (VertPos - Centre));
				}
				else
				{
					RotatedVert = VertPos - Centre;
				}

				RotatedVert += Centre;

				App->CL_Scene->Group[Count]->vertex_Data[VertCount].x = RotatedVert.x;
				App->CL_Scene->Group[Count]->vertex_Data[VertCount].y = RotatedVert.y;
				App->CL_Scene->Group[Count]->vertex_Data[VertCount].z = RotatedVert.z;

				VertCount++;
			}
			Count++;
		}

		App->CL_Scene->Set_BondingBox_Model(0);
	}
}
