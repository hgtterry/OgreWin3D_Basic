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
}