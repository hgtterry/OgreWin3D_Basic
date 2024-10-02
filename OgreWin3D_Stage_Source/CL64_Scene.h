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

#include "Base_Object.h"
#include "Base_Player.h"
#include "Base_Area.h"
#include "Base_Locations.h"
#include "Base_Counter.h"

class CL64_Scene
{
public:

	CL64_Scene(void);
	~CL64_Scene(void);

	void Reset_Class(void);
	void Reset_Counters();

	void Set_Scene(); 

	bool Clear_Level();

	// Stage 
	int Object_Count;
	int Area_Count;
	int Camera_Count;
	int Player_Location_Count;
	int Counters_Count;
	int UniqueID_Object_Counter;
	int UniqueID_Counters_Count;
	int UniqueID_Area_Count;

	bool flag_Area_Added;
	bool flag_Scene_Loaded;
	bool flag_GameMode_Running_Flag;
	bool flag_Project_Resources_Created;
	bool flag_Player_Added;
	bool flag_Scene_Modified;

	Base_Area* B_Area[20];
	Base_Locations* B_Locations[20];
	Base_Counter* B_Counter[20];
	std::vector<Base_Player*> B_Player;
	std::vector<Base_Object*> V_Object;
};

