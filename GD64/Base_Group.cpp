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
	GroupName[0] = 0;
	MaterialName[0] = 0;
	Text_FileName[0] = 0;

	Texture_FolderPath[0] = 0;
	Texture_PathFileName[0] = 0;

	Bitmap_Loaded = 0;

	GroupVertCount = 0;

	IndicesCount = 0; // Ogre
	BoneAssignMentCount = 0; // Ogre

	MaterialIndex = 0;
	Soil_TextureIndex = 0;

	Base_Bitmap = NULL;
	//RF_Bitmap = NULL;
	ListView_Item = NULL;
}


Base_Group::~Base_Group()
{
}
