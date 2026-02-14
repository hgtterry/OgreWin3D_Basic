#pragma once

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning( disable : 4251 4193 4275 4005 4267 4244)
#pragma warning( disable : 4305 4311 4302)
#pragma warning(disable : 4996 4715) // Needs Looking At

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
#include "irrKlang.h"

#include <gl/gl.h>
#include <gl/glu.h>

#define GE_FALSE	(0)
#define GE_TRUE		(1)
#define	GE_PI		((float)3.14159265358979323846f)

struct Colour_Int
{
	int R = 0, G = 0, B = 0;
};

//float Version = 1.0;

namespace Enums
{
	enum UpdateViews
	{
		UpdateViews_None = 0,
		UpdateViews_Grids = 1,
		UpdateViews_3D = 2,
		UpdateViews_All = 3
	};
}