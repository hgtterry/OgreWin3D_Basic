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
	enum Base
	{
		Base_None = 0,
		Base_Object = 1,
		Base_Teleporter = 2
	};

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
		Check_Names_Textures = 3,
		Check_Names_Locations = 4
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
		ListBox_Used_Textures = 1,
		ListBox_Libraries = 2
	};

	enum Selected_Map_View
	{
		Selected_Map_View_None = 0,
		Selected_Map_View_3D = 1,
		Selected_Map_View_TL = 2,
		Selected_Map_View_TR = 3,
		Selected_Map_View_BL = 4
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
		Edit_UserObjects = 19,
		Edit_Locations = 20
	};

	enum FileView_Context
	{
		FileView_None = 0,
		FileView_Objects_Folder = 1,
		FileView_Objects_File = 2,
		FileView_Messages_Folder = 3,
		FileView_Messages_File = 4,
		FileView_Sounds_Folder = 5,
		FileView_Sounds_File = 6,
		FileView_Cameras_Folder = 7,
		FileView_Cameras_File = 8,
		FileView_Areas_Folder = 9,
		FileView_Areas_File = 10,
		FileView_Move_Folder = 11,
		FileView_Move_File = 12,
		FileView_Player_Folder = 13,
		FileView_Player_File = 14,
		FileView_Teleports_Folder = 15,
		FileView_Teleports_File = 16,
		FileView_Collectables_Folder = 17,
		FileView_Collectables_File = 18,
		FileView_Counters_Folder = 19,
		FileView_Counters_File = 20,
		FileView_TextMessage_Folder = 21,
		FileView_TextMessage_File = 22,
		FileView_Environments_Folder = 23,
		FileView_Environments_File = 24,
		FileView_EnvironEntity_Folder = 25,
		FileView_EnvironEntity_File = 26,
		FileView_Particle_Folder = 27,
		FileView_Particle_File = 28,
		FileView_Light_Folder = 29,
		FileView_Light_File = 30,
		FileView_UserObjects_Folder = 31,
		FileView_UserObjects_File = 32,
		FileView_Locations_Folder = 33,
		FileView_Locations_File = 34
	};

	enum Brushs_ID
	{
		Brushs_ID_Only_Brush = -1,
		Brushs_ID_Area = 0,
		Brushs_ID_Players = 1,
		Brushs_ID_Evirons = 2
	};

	enum Mesh_Viewer
	{
		Mesh_Viewer_None = -1,
		Mesh_Viewer_Objects = 0,
		Mesh_Viewer_Collectables = 1,
		Mesh_Viewer_Area = 2
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
}