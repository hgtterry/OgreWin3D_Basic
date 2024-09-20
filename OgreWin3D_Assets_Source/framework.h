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

#pragma once

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning( disable : 4251 4193 4275 4005 4267 4244)
#pragma warning( disable : 4305 4311 4302)
#pragma warning(disable : 4996) // Needs Looking At

#include "targetver.h"

// Windows Header Files
#include <windows.h>
#include "btBulletDynamicsCommon.h"

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <direct.h>
#include <Commctrl.h>   // includes the Common control header
#include <string>

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreException.h"
#include "OgreTextAreaOverlayElement.h"

#include <gl/gl.h>
#include <gl/glu.h>

namespace Enums
{
	enum Model_Type
	{
		Model_Type_None = 0,
		Model_Type_Obj = 1,
		Model_Type_3ds = 2,
		Model_Type_Milk = 3
	};

	enum Camera
	{
		Cam_Mode_None = 0,
		Cam_Mode_First = 1,
		Cam_Mode_Free = 2,
		Cam_Mode_Third = 3,
		Cam_Mode_Model = 4
	};

	enum Usage
	{
		Usage_None = -1,
		Usage_Player = 2
		
	};

	enum Scene_Mode
	{
		Scene_Mode_None = 0,		// No Scene Mode Set No Model
		Scene_Mode_Ogre_Model = 1,
		Scene_Mode_Assimp_Model = 72
	};

	enum Check_Text
	{
		Check_Name_None = 0,
	};

	enum Resource_File_Type
	{
		Resource_File_Type_None = 0,
		Resource_File_Type_Material = 1,
		Resource_File_Type_Mesh = 2,
		Resource_File_Type_Skeleton = 3,
		Resource_File_Type_Texture = 4,
		Resource_File_Type_Overlay = 5
	};

}

#define Debug App->Debug_Text();
