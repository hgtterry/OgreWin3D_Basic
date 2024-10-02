/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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
#include "irrKlang.h"

#include <gl/gl.h>
#include <gl/glu.h>

namespace Enums
{
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

	enum Edit_Type
	{
		Edit_None = 0,
		Edit_Object = 1,
		Edit_Physics_Object = 2,
		Edit_Player = 3,
		Edit_Message = 4,
		Edit_Move_Entity = 5,
		Edit_Sounds = 6,
		Edit_Collectable = 7,
		Edit_Teleport = 8,
		Edit_Environment = 9,
		Edit_Area_Change = 10,
		Edit_Change_Level = 11,
		Edit_Particles = 12,
		Edit_Camera = 13,
		Edit_Area = 14,
		Edit_Level = 15,
		Edit_Counters = 16,
		Edit_Environs = 17,
		Edit_Lights = 18,
		Edit_UserObjects = 19
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
