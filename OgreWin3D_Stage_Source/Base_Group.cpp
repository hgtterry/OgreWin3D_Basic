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
#include "Base_Group.h"


Base_Group::Base_Group()
{
	Sub_Mesh_Count = 0;
	GroupName[0] = 0;

	Total_VertCount = 0;
	Total_FaceCount = 0;
	Total_IndicesCount = 0; // Ogre

	int Count = 0;
	while (Count < 99)
	{
		B_Sub_Mesh[Count] = nullptr;
		Count++;
	}
}


Base_Group::~Base_Group()
{
}
