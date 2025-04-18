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
	BrushList* Level_GetBrushes(Level* pLevel);

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
	signed int Level_LoadWad();
	const char* Level_GetWadPath();
	CL64_WadFile* Level_GetWad_Class();
	void Level_SetWadPath(Level* pLevel, const char* NewWad);

	WadFileEntry* Level_GetWadBitmap(Level* pLevel, const char* Name);
	Ogre::uint16 Level_GetDibId(const Level* pLevel, const char* Name);
	void Level_RemoveBrush(Brush* pBrush);
	int Level_EnumBrushes(Level* pLevel, void* lParam, BrushList_CB Callback);
	signed int Level_UseGrid(const Level* pLevel);
	int Level_GetRotationSnap(const Level* pLevel);
	float Level_GetGridSnapSize(const Level* pLevel);

	bool flag_UseGrid;
};

