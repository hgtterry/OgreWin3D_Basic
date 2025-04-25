/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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

#include "CL64_BrushTemplate.h"
#include "CL64_WadFile.h"

typedef struct tag_Level Level;

class CL64_Level
{
public:
	CL64_Level(void);
	~CL64_Level(void);

	Level* Level_Create();

	void Level_Delete_TXL_Class();
	signed int Level_Create_TXL_Class();

	// Get Main Top Level Brush List
	BrushList* Level_Get_Main_Brushes();

	BrushTemplate_Box* Level_GetBoxTemplate();
	BrushTemplate_Cylinder* Level_GetCylinderTemplate();
	BrushTemplate_Cone* Level_GetConeTemplate();
	BrushTemplate_Staircase* Level_GetStaircaseTemplate();
	BrushTemplate_Arch* Level_GetArchTemplate();

	T_Vec3* Level_GetTemplatePos(Level* pLevel);
	float Level_GetDrawScale(const Level* pLevel);
	float Level_GetLightmapScale(const Level* pLevel);
	void Level_AppendBrush(Brush* pBrush);
	int Level_EnumLeafBrushes(void* lParam, BrushList_CB Callback);

	const char* Level_GetWadPath();
	CL64_WadFile* Level_GetWad_Class();
	void Level_SetWadPath(const char* NewWad);

	WadFileEntry* Level_GetWadBitmap(const char* Name);
	Ogre::uint16 Level_GetDibId(const char* Name);
	void Level_RemoveBrush(Brush* pBrush);
	int Level_EnumBrushes(void* lParam, BrushList_CB Callback);
	signed int Level_UseGrid(const Level* pLevel);
	int Level_GetRotationSnap(const Level* pLevel);
	float Level_GetGridSnapSize(const Level* pLevel);

	bool flag_UseGrid;
	bool flag_Working_Folder_Exists;

	CL64_WadFile* CL_Wad_Class;

	float Level_Version;

	// MTF File
	char MTF_PathAndFile[MAX_PATH];
	char MTF_Just_FileName[MAX_PATH];
	char MTF_Just_Path[MAX_PATH];
	char MTF_JustName_NoExt[MAX_PATH];
	char Prj_Working_Folder[MAX_PATH];
	// TXL File
	char Wad_PathAndFile[MAX_PATH];
	char Wad_Just_File_Name[MAX_PATH];
	char Wad_Origin_File_Name[MAX_PATH];
};

