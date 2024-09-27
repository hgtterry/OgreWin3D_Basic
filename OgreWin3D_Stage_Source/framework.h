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
	enum Loaded_File_Type
	{
		Loaded_File_Type_None = 0,
		Loaded_File_Type_Assimp = 1,
		Loaded_File_Type_Ogre = 2
	};

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
		Scene_Mode_None = 0,		// No Scene Mode Set
		Scene_Mode_TestMesh = 2,	// Test Mesh at startup
		Scene_Mode_Imported_Entity = 3,
		Scene_Mode_MeshData = 4
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

	// Stage
	enum Shape
	{
		NoShape = 0,
		Shape_Box = 1,
		Sphere = 2,
		Capsule = 3,
		Cylinder = 4,
		Cone = 5,
		Shape_TriMesh = 6,
		VolumeBox = 7
	};

	enum Bullet_Type 
	{
		Bullet_Type_None = 0,
		Bullet_Type_Dynamic = 1, // Free Moving Physics
		Bullet_Type_Static = 2, // Fixed Non Moving Shape
		Bullet_Type_TriMesh = 3,
		Type_NXS_Convex = 4,
		Type_JustOgre = 5,
		Bullet_Type_Volume = 6
	};

	enum Stage_Usage
	{
		Stage_Usage_None = -1,
		Stage_Usage_Room = 0,
		Stage_Usage_Static = 1,
		Stage_Usage_Dynamic = 2,
		Stage_Usage_Object = 3,
		Stage_Usage_Ammo = 4,
		Stage_Usage_Weapon = 5,
		Stage_Usage_Teleport = 6,
		Stage_Usage_ChangeLevel = 7,
		Stage_Usage_MediKit = 8,
		Stage_Usage_Recevier = 9,
		Stage_Usage_Particle = 10,
		Stage_Usage_Light = 11,
		Stage_Usage_Colectable = 12,
		Stage_Usage_Message = 13,
		Stage_Usage_Move = 14,
		Stage_Usage_Sound = 15,
		Stage_Usage_PlayerDeath = 16,
		Stage_Usage_Counter = 17,
		Stage_Usage_Player = 18,
		Stage_Usage_Environment = 19,
		Stage_Usage_RoomChange = 20,
		Stage_Usage_EnvironEntity = 21,
		Stage_Usage_UserObject = 22
	};

	enum Folder_Type
	{
		Folder_None = 0,
		Folder_Objects = 1,
		Folder_Sound_Entity = 2,
		Folder_Message_Entity = 3,
		Folder_Move_Entity = 4,
		Folder_Collectables = 5,
		Folder_Teleports = 6,
		Folder_Environment = 7,
		Folder_RoomChange = 8,
		Folder_Change_Level = 9,
		Folder_Particle = 10
	};

	enum Axis
	{
		Axis_None = 0,
		Axis_x = 1,
		Axis_y = 2,
		Axis_z = 3
	};
}

#define Debug App->Debug_Text();
