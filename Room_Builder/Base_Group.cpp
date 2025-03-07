/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_App.h"
#include "Base_Group.h"

Base_Group::Base_Group()
{
	GroupName[0] = 0;
	MaterialName[0] = 0;
	Text_FileName[0] = 0;

	Texture_FolderPath[0] = 0;
	Texture_PathFileName[0] = 0;

	Bitmap_Loaded = 0;
	Dont_Use = 0;

	GroupVertCount = 0;
	GroupFaceCount = 0;

	IndicesCount = 0; // Ogre
	BoneAssignMentCount = 0; // Ogre

	MaterialIndex = 0;
	Soil_TextureIndex = 0;

	Base_Bitmap = NULL;
	//RF_Bitmap = NULL;
	ListView_Item = NULL;

	Has_Alpha = 0;
	// Bounding Box
	BB_Max.x = 0;
	BB_Max.y = 0;
	BB_Max.z = 0;

	BB_Min.x = 0;
	BB_Min.y = 0;
	BB_Min.z = 0;

	Size.x = 0;
	Size.y = 0;
	Size.z = 0;

	Centre.x = 0;
	Centre.y = 0;
	Centre.z = 0;

	radius = 0;;
}

Base_Group::~Base_Group()
{
}
