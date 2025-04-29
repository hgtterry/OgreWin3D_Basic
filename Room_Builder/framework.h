#pragma once

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning( disable : 4251 4193 4275 4005 4267 4244)
#pragma warning( disable : 4305 4311 4302)
#pragma warning(disable : 4996 4715) // Needs Looking At

#include "targetver.h"

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

#define Debug App->Say_Win("Here");//, "  Internal Debug Message  ");
#define Do_Timer App->CL_Ogre->Processes_Start_Time();
#define Get_Timer App->CL_Ogre->Processes_Get_Time();

#define GE_FALSE	(0)
#define GE_TRUE		(1)
#define	GE_PI		((float)3.14159265358979323846f)

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

	enum Check_Text
	{
		Check_Name_None = 0,
		Check_Name_Brushes = 1,
		Check_Names_Objects = 2,
		Check_Names_Textures = 3
	};

	enum UpdateViews
	{
		UpdateViews_None = 0,
		UpdateViews_Grids = 1,
		UpdateViews_3D = 2,
		UpdateViews_All = 3
	};

	enum Export_Ogre
	{
		Export_Ogre_Latest = 0,
		Export_Ogre_18 = 1,
		Export_Ogre_17 = 2
	};

	enum RenderMode
	{
		Render_Nothing = -1,
		Render_Ogre = 0,
		Render_Groups = 1,
		Render_Brushes = 2,
		Render_Assimp = 3
	};

	enum Editor_Mode
	{
		Editor_Dlgs_New = 0,
		Editor_Dlgs_First_Brush = 1
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

	enum ListBox_Type
	{
		ListBox_None = 0,
		ListBox_Used_Textures = 1
	};

	enum Selected_View
	{
		Selected_View_None = 0,
		Selected_View_3D = 1,
		Selected_View_TL = 2,
		Selected_View_TR = 3,
		Selected_View_BL = 4
	};

	enum Usage
	{
		Usage_None = -1,
		Usage_Player = 2
	};

	// Scene Editor
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

	enum Shape
	{
		Shape_None = 0,
		Shape_Box = 1,
		Shape_Sphere = 2,
		Shape_Capsule = 3,
		Shape_Cylinder = 4,
		Shape_Cone = 5,
		Shape_TriMesh = 6,
		Shape_VolumeBox = 7
	};

	enum Stage_Usage
	{
		Obj_Usage_None = -1,
		Obj_Usage_Room = 0,
		Obj_Usage_Static = 1,
		Obj_Usage_Dynamic = 2,
		Obj_Usage_Object = 3,
		Obj_Usage_Ammo = 4,
		Obj_Usage_Weapon = 5,
		Obj_Usage_Teleport = 6,
		Obj_Usage_ChangeLevel = 7,
		Obj_Usage_MediKit = 8,
		Obj_Usage_Recevier = 9,
		Obj_Usage_Particle = 10,
		Obj_Usage_Light = 11,
		Obj_Usage_Collectable = 12,
		Obj_Usage_Message = 13,
		Obj_Usage_Move = 14,
		Obj_Usage_Sound = 15,
		Obj_Usage_PlayerDeath = 16,
		Obj_Usage_Counter = 17,
		Obj_Usage_Player = 18,
		Obj_Usage_Environment = 19,
		Obj_Usage_RoomChange = 20,
		Obj_Usage_EnvironEntity = 21,
		Obj_Usage_UserObject = 22
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

}