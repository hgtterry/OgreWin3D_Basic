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

#include "CL64_Brush.h"
#include "CL64_SelBrushList.h"
#include "CL64_SelFaceList.h"

typedef signed int(*BrushFlagTest)(const Brush* pBrush);

enum UpdateFlags
{
	REBUILD_QUICK = 1,
	UAV_RENDER_ONLY = 2,
	UAV_ACTIVE3DVIEW_ONLY = 4,
	UAV_NONACTIVE3DVIEWS_ONLY = 8,
	UAV_TEXTUREVIEW_ONLY = 16,
	UAV_GRID_ONLY = 32,
	UAV_TEXTUREBROWSER_ONLY = 64,
	UAV_ALLVIEWS = 128,
	UAV_ALL3DVIEWS = 256,
	UAV_THIS_GRID_ONLY = 512
};

enum
{
	fctNOTHING,
	fctENTITY,
	fctBRUSH
};

enum BrushSel
{
	brushSelToggle,
	brushSelAlways,
	brushDeselAlways
};

#define WINDOW_TOP_VIEW 0
#define WINDOW_FRONT_VIEW 1
#define WINDOW_SIDE_VIEW 2
#define MAX_SEL	1000
#define CURTOOL_NONE	(-1)

class CL64_Doc
{
public:
	CL64_Doc(void);
	~CL64_Doc(void);

	void Init_Doc();
	const char* FindTextureLibrary(char const* WadName);
	void SetLockAxis(int Lock) { mLockAxis = Lock; };

	void AddBrushToWorld();
	void Brush_Add_To_world();
	void SetDefaultBrushTexInfo(Brush* b);
	void UpdateAllViews(int Mode, BOOL Override);
	void DoGeneralSelect(void);
	WadFileEntry* GetDibBitmap(const char* Name);

	// Selection
	void SelectOrtho(POINT point, ViewVars* v);
	void DoBrushSelection(Brush* pBrush, BrushSel	nSelType); //	brushSelToggle | brushSelAlways);

	int FindClosestThing(POINT const* ptFrom, ViewVars* v, Brush** ppMinBrush, geFloat* pDist);
	signed int FindClosestBrush(POINT const* ptFrom, ViewVars* v, Brush** ppFoundBrush, geFloat* pMinEdgeDist);
	signed int BrushIsSelected(Brush const* pBrush);

	char LastTemplateTypeName[MAX_PATH];

	BOOL SelectLock, TempEnt;

	Level* pLevel;
	Brush* BTemplate, * CurBrush, * TempShearTemplate;
	SelBrushList* pSelBrushes;
	SelBrushList* pTempSelBrushes;
	SelFaceList* pSelFaces;

	Ogre::Vector3	SelectedGeoCenter, FinalPos, FinalRot, FinalScale;

	int mLockAxis;
	int mModeTool;
	int mCurrentGroup;
	int mCurrentTool;
	bool flag_Is_Modified;
	//SelFaceList* pSelFaces;
};

