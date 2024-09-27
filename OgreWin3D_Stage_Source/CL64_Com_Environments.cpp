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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Com_Environments.h"

CL64_Com_Environments::CL64_Com_Environments()
{
}

CL64_Com_Environments::~CL64_Com_Environments()
{
}

// *************************************************************************
// *		V_Set_Environ_Defaults:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Environments::V_Set_Environ_Defaults(int Index)
{
	Base_Object* V_Object = App->CL_Scene->V_Object[Index];

	V_Object->Altered = 0;

	V_Object->S_Environ[0]->Environment_ID = 0;
	strcpy(V_Object->S_Environ[0]->Environment_Name, "Not_Set");

	V_Object->S_Environ[0]->Environ_Enabled = 1;

	//----------------------- Sound
	/*strcpy(V_Object->S_Environ[0]->Sound_File, "The_Sun.ogg");
	V_Object->S_Environ[0]->SndFile = NULL;
	V_Object->S_Environ[0]->Play = 0;
	V_Object->S_Environ[0]->Loop = 1;
	V_Object->S_Environ[0]->SndVolume = 0.5;*/

	//----------------------- Light
	V_Object->S_Environ[0]->AmbientColour.x = 1;
	V_Object->S_Environ[0]->AmbientColour.y = 1;
	V_Object->S_Environ[0]->AmbientColour.z = 1;

	V_Object->S_Environ[0]->Light_Position.x = 0;
	V_Object->S_Environ[0]->Light_Position.y = 0;
	V_Object->S_Environ[0]->Light_Position.z = 0;

	// Sky
	V_Object->S_Environ[0]->Curvature = 15;
	V_Object->S_Environ[0]->Distance = 4000;
	V_Object->S_Environ[0]->Enabled = 0;
	strcpy(V_Object->S_Environ[0]->Material, "Examples/CloudySky");
	V_Object->S_Environ[0]->Tiling = 15;
	V_Object->S_Environ[0]->type = 1;

	// Fog
	V_Object->S_Environ[0]->Fog_On = 0;
	V_Object->S_Environ[0]->Fog_Mode = FOG_LINEAR;
	V_Object->S_Environ[0]->Fog_Density = 0.001000;
	V_Object->S_Environ[0]->Fog_Start = 50;
	V_Object->S_Environ[0]->Fog_End = 300;
	V_Object->S_Environ[0]->Fog_Colour = Ogre::Vector3(1, 1, 1);
}
