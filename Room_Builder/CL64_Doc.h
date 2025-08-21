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

#include "CX_Brush.h"
#include "CX_SelBrushList.h"
#include "CX_SelFaceList.h"

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

enum SelectionState
{
	NOBRUSHES = 0, ONEBRUSH = 1, MULTIBRUSH = 2, RESERVEDBRUSH = 4,
	NOFACES = 8, ONEFACE = 16, MULTIFACE = 32, RESERVEDFACE = 64,
	NOENTITIES = 128, ONEENTITY = 256, MULTIENTITY = 512, RESERVEDENTITY = 1024,
	ONEBRUSHONLY = 137, ONEBRUSH_ONEFACE = 145, ONEENTITYONLY = 264,
	ONEBRUSH_ONEENTITY = 265, NOSELECTIONS = 136,
	BRUSHCLEAR = 7, FACECLEAR = 120, ENTITYCLEAR = 1920,
	ANYENTITY = 1792, ANYBRUSH = 7, ANYFACE = 112
};

enum fdocAdjustEnum
{
	ADJUST_MODE_TOGGLE,
	ADJUST_MODE_BRUSH,
	ADJUST_MODE_FACE
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
	void Load_Wad_File(char* TXL_File);

	void Editor_Set_Dlgs(int Set_State);

	const char* FindTextureLibrary(char const* WadName);
	void SetLockAxis(int Lock) { mLockAxis = Lock; };

	void Set_Faces_To_Brush_Name_All();
	void Set_Faces_To_Brush_Name_Selected();

	void AddBrushToWorld();
	void Brush_Add_To_world();
	void SetDefaultBrushTexInfo(Brush* b);
	void UpdateAllViews(int Update_Mode);

	void Set_Tool_GeneralSelect();
	void Do_General_Select_Dlg(bool from_Insert);

	WadFileEntry* GetDibBitmap(const char* Name);

	// Selection Move Brush
	void SelectOrtho(POINT point, ViewVars* v);
	void ResetAllSelections(void);
	void ResetAllSelectedFaces(void);
	void ResetAllSelectedBrushes(void);
	void DoBrushSelection(Brush* pBrush, BrushSel	nSelType); //	brushSelToggle | brushSelAlways);

	// Selection Scale Brush
	void ScaleSelected(int dx, int dy);
	void ResizeSelected(float dx, float dy, int sides, int inidx);
	void DoneResize(int sides, int inidx);
	void ScaleSelectedBrushes(T_Vec3* ScaleVector);

	int FindClosestThing(POINT const* ptFrom, ViewVars* v, Brush** ppMinBrush, float* pDist);
	signed int FindClosestBrush(POINT const* ptFrom, ViewVars* v, Brush** ppFoundBrush, float* pMinEdgeDist);
	signed int BrushIsSelected(Brush const* pBrush);
	void UpdateSelected(void);
	DWORD GetSelState(void) { return SelState; }

	void RebuildTrees(void);
	signed int BrushIsVisible(const Brush* pBrush) const;
	void TempCopySelectedBrushes(void);
	void LockAxis(T_Vec3* pWP);
	int	GetLockAxis(void);
	void MoveSelectedBrushes(T_Vec3 const* v);
	void MoveSelectedBrushList(SelBrushList* pList, T_Vec3 const* v);
	void DoneMovingBrushes();
	void DoneRotate(void);
	void DoneMove(void);
	BOOL TempDeleteSelected(void);
	void OnToolsTemplate();
	void OnBrushSubtractfromworld();
	void SelectAllFacesInBrushes(void);
	void SelectAll(void);
	void DeleteCurrentThing();
	bool DeleteSelectedBrushes();
	void UpdateAfterWadChange();
	void Set_Paths(void);
	void Set_Current_3DT_Paths(void);
	void Set_Current_TxlPath(void);

	// Rotation
	void RotateSelectedBrushes(const ViewVars* view, T_Vec3 const* v);
	void RotateSelectedBrushList(const ViewVars* view, SelBrushList* pList, T_Vec3 const* v);
	void GetRotationPoint(T_Vec3* pVec);
	void SnapScaleNearest(int sides, int inidx, ViewVars* v);

	// ------------------------------------------------------------------- 

	char LastTemplateTypeName[MAX_PATH];

	BOOL SelectLock, TempEnt;

	Level* Current_Level;
	Brush* BTemplate, * CurBrush, * TempShearTemplate;
	SelBrushList* pSelBrushes;
	SelBrushList* pTempSelBrushes;
	SelFaceList* pSelFaces;
	SelFaceList* Temp_SelFaces;

	fdocAdjustEnum mAdjustMode;

	DWORD SelState;

	T_Vec3	SelectedGeoCenter, FinalPos, FinalRot, FinalScale;

	int ScaleNum;
	int	sides;

	int mLastOp;
	int mLockAxis;
	int mModeTool;
	int mCurrentGroup;
	int mCurrentTool;
	int NumSelEntities;

	bool flag_IsNewDocument;
	bool flag_Track_Camera;

};

