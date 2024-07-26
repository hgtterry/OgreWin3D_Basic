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
#include "OW3D_Import_Ogre3D.h"

OW3D_Import_Ogre3D::OW3D_Import_Ogre3D(void)
{
	flag_IsAnimated = 0;
}

OW3D_Import_Ogre3D::~OW3D_Import_Ogre3D(void)
{
}

// *************************************************************************
// *	  			Get_Motions:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void OW3D_Import_Ogre3D::Get_Motions(Ogre::Entity* Ogre_Entity)
{
	Ogre::SkeletonInstance* skeletonInstance = Ogre_Entity->getSkeleton();

	int Count = 0;
	if (skeletonInstance)
	{
		int AnimationCount = skeletonInstance->getNumAnimations();
		if (AnimationCount == 0)
		{
			flag_IsAnimated = 0;
			App->CL_Scene->MotionCount = 0;
		}
		else
		{

			flag_IsAnimated = 1;

			for (unsigned short i = 0; i < skeletonInstance->getNumAnimations(); ++i)
			{
				Ogre::Animation* animation = skeletonInstance->getAnimation(i);
				App->CL_Scene->S_OgreMeshData[0]->mMotionNames.push_back(animation->getName());

				Count = i;
			}

			App->CL_Scene->MotionCount = Count + 1;

		}
	}
	else
	{
		flag_IsAnimated = 0;
		App->CL_Scene->MotionCount = 0;
	}
}
