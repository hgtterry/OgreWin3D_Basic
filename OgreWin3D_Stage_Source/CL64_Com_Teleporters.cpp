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
#include "CL64_Com_Teleporters.h"

CL64_Com_Teleporters::CL64_Com_Teleporters()
{
}

CL64_Com_Teleporters::~CL64_Com_Teleporters()
{
}

// *************************************************************************
// *	  Set_Teleports_Defaults:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Com_Teleporters::Set_Teleports_Defaults(int Index)
{
	Ogre::Vector4 V4 = Ogre::Vector4::ZERO;

	Base_Object* V_Object = App->CL_Scene->V_Object[Index];

	V_Object->S_Teleport[0]->Location_ID = 0;

	strcpy(V_Object->S_Teleport[0]->Name, "Start_Location");

	/*strcpy(V_Object->S_Teleport[0]->Sound_File, "magicspell.ogg");
	V_Object->S_Teleport[0]->SndVolume = 0.5;
	V_Object->S_Teleport[0]->Play = 1;*/

	V4.x = App->CL_Scene->B_Player[0]->StartPos.x;
	V4.y = App->CL_Scene->B_Player[0]->StartPos.y;
	V4.z = App->CL_Scene->B_Player[0]->StartPos.z;

	V_Object->S_Teleport[0]->Physics_Position = btVector3(V4.x, V4.y, V4.z);
	V_Object->S_Teleport[0]->Physics_Rotation = App->CL_Scene->B_Player[0]->Physics_Rotation;

	V_Object->S_Teleport[0]->Trigger_Value = 0;
	V_Object->S_Teleport[0]->Counter_ID = 0;
	strcpy(V_Object->S_Teleport[0]->Counter_Name, "None");
	V_Object->S_Teleport[0]->Counter_Disabled = 1;
}
