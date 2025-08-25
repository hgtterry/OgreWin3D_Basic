/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D Scene Builder

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
#include "Base_Teleporter.h"

Base_Teleporter::Base_Teleporter(void)
{
	// Object
	Object_Ent = nullptr;
	Object_Node = nullptr;
	Phys_Body = nullptr;
	Phys_Shape = nullptr;

	strcpy(Mesh_FileName, "None");
	strcpy(Object_Name, "None");
	strcpy(Material_File, "Internal");

	Type = 0;
	Shape = -1;
	This_Object_UniqueID = 0;

	// Object Pos
	Mesh_Pos.x = 0;
	Mesh_Pos.y = 0;
	Mesh_Pos.z = 0;
	// Mesh Quat
	Mesh_Quat.w = 1;
	Mesh_Quat.x = 0;
	Mesh_Quat.y = 0;
	Mesh_Quat.z = 0;

	// Physics Scale ------------------------------------------------------
	Physics_Scale.x = 1;
	Physics_Scale.y = 1;
	Physics_Scale.z = 1;

	// Physics Pos
	Physics_Pos.x = 0;
	Physics_Pos.y = 0;
	Physics_Pos.z = 0;

	// Physics Rotation
	Physics_Rot.x = 0;
	Physics_Rot.y = 0;
	Physics_Rot.z = 0;

	// Physics Size
	Physics_Size.x = 0;
	Physics_Size.y = 0;
	Physics_Size.z = 0;

	// Physics Quat
	Physics_Quat.w = 1;
	Physics_Quat.x = 0;
	Physics_Quat.y = 0;
	Physics_Quat.z = 0;
	flag_Physics_Valid = false;
	Usage = Enums::Usage_None;

	// Teleporter
	strcpy(Location_Name, "Start_Location");
	Player_Position = { 0,0,0 };
	Physics_Position = { 0,0,0 };
	Physics_Rotation = { 0,0,0,0};
	Location_ID = 0;
	SndVolume = 0.5;
	strcpy(Sound_File, "None");
	flag_Play = false;
	Trigger_Value = 0;
	Counter_ID = 0;

	strcpy(Counter_Name, "None");
	flag_Counter_Disabled = true;

	FileViewItem = nullptr;
}

Base_Teleporter::~Base_Teleporter(void)
{
}
