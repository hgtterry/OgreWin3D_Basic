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
		Render_Brushes = 0,
		Render_Groups = 1,
		Render_Ogre = 2,
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
}