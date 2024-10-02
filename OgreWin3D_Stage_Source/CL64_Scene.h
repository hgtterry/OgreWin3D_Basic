/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
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

	char FileName[MAX_PATH];
	char Path_FileName[MAX_PATH];
	char Model_FolderPath[MAX_PATH];
	char Texture_FolderPath[MAX_PATH];
	char JustName[MAX_PATH];

	// Internal
	int Loaded_File_Type;
	int Scene_Mode;

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
	
	Base_Area* B_Area[20];
	Base_Locations* B_Locations[20];
	Base_Counter* B_Counter[20];
	std::vector<Base_Player*> B_Player;
	std::vector<Base_Object*> V_Object;
};

