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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Level.h"
#include "CL64_WadFile.h"
#include "Structures.cpp"

enum { GridMetric = 0, GridTexel = 1 };

CL64_Level::CL64_Level(void)
{
	flag_UseGrid = 1;
	flag_Working_Folder_Exists = 0;

	strcpy(MTF_PathAndFile, App->RB_Directory_FullPath);
	strcat(MTF_PathAndFile, "\\Data\\Room_Builder\\Room_1.mtf");

	strcpy(MTF_Just_FileName, "Room_1.mtf");
	strcpy(MTF_JustName_NoExt, "Room_1");

	strcpy(Prj_Working_Folder, "None");

	Level_Version = 1.5;

	TXL_PathAndFile[0] = 0;
	TXL_Just_File_Name[0] = 0;
	TXL_Origin_File_Name[0] = 0;
}

CL64_Level::~CL64_Level(void)
{
}

// *************************************************************************
// *							Level_Create							   *
// *************************************************************************
Level* CL64_Level::Level_Create()
{
	Level* m_pLevel = NULL;

	m_pLevel = new Level;

	if (m_pLevel != NULL)
	{
		m_pLevel->Brushes = App->CL_Brush->BrushList_Create();

		if (m_pLevel->Brushes == NULL)
		{
			App->Say_Win("Can not create brush lists");
			return m_pLevel;
		}

		strcpy(TXL_PathAndFile,"Empty");
		strcpy(TXL_Just_File_Name, "Empty");

		m_pLevel->WadSizeInfos = NULL;

		GridInfo* pGridInfo;
		pGridInfo = &m_pLevel->GridSettings;
		pGridInfo->GridType = GridTexel;
		pGridInfo->SnapType = GridTexel;
		pGridInfo->MetricSnapSize = GridSize_Decimeter;
		pGridInfo->TexelSnapSize = 8;
		pGridInfo->RotationSnap = 15;
		

		/*for (int iView = 0; iView < NUM_VIEWS; ++iView)
		{
			ViewStateInfo* pInfo;

			pInfo = &pLevel->ViewInfo[iView];
			pInfo->IsValid = GE_FALSE;
			pInfo->ZoomFactor = 1.0f;
			geVec3d_Clear(&pInfo->PitchRollYaw);
			geVec3d_Set(&pInfo->CameraPos, 0.0f, 0.0f, 0.0f);
		}*/

		App->CL_BrushTemplate->BrushTemplate_ArchDefaults(&m_pLevel->ArchTemplate);
		App->CL_BrushTemplate->BrushTemplate_BoxDefaults(&m_pLevel->BoxTemplate);
		App->CL_BrushTemplate->BrushTemplate_ConeDefaults(&m_pLevel->ConeTemplate);
		App->CL_BrushTemplate->BrushTemplate_CylinderDefaults(&m_pLevel->CylinderTemplate);
		App->CL_BrushTemplate->BrushTemplate_SpheroidDefaults(&m_pLevel->SpheroidTemplate);
		App->CL_BrushTemplate->BrushTemplate_StaircaseDefaults(&m_pLevel->StaircaseTemplate);

		App->CL_Maths->Vector3_Clear(&m_pLevel->TemplatePos);

		m_pLevel->DrawScale = 0.50f;
		m_pLevel->LightmapScale = 2.0f;
	}
	else
	{
		App->Say("Cannot Creat Level");
		return nullptr;

	}

	return m_pLevel;
}

// *************************************************************************
// *						Level_GetBoxTemplate						   *
// *************************************************************************
BrushTemplate_Box* CL64_Level::Level_GetBoxTemplate()
{
	return &App->CL_Doc->Current_Level->BoxTemplate;
}

// *************************************************************************
// *					Level_GetCylinderTemplate						   *
// *************************************************************************
BrushTemplate_Cylinder* CL64_Level::Level_GetCylinderTemplate()
{
	return &App->CL_Doc->Current_Level->CylinderTemplate;
}

// *************************************************************************
// *						Level_GetConeTemplate						   *
// *************************************************************************
BrushTemplate_Cone* CL64_Level::Level_GetConeTemplate()
{
	return &App->CL_Doc->Current_Level->ConeTemplate;
}

// *************************************************************************
// *						Level_GetStaircaseTemplate					   *
// *************************************************************************
BrushTemplate_Staircase* CL64_Level::Level_GetStaircaseTemplate()
{
	return &App->CL_Doc->Current_Level->StaircaseTemplate;
}

// *************************************************************************
// *						Level_GetArchTemplate						   *
// *************************************************************************
BrushTemplate_Arch* CL64_Level::Level_GetArchTemplate()
{
	return &App->CL_Doc->Current_Level->ArchTemplate;
}

// *************************************************************************
// *					Level_Get_Main_Brushes							   *
// *************************************************************************
BrushList* CL64_Level::Level_Get_Main_Brushes()
{
	return App->CL_Doc->Current_Level->Brushes;
}

// *************************************************************************
// *						Level_GetTemplatePos						   *
// *************************************************************************
T_Vec3* CL64_Level::Level_GetTemplatePos(Level* pLevel)
{
	return &pLevel->TemplatePos;
}

// *************************************************************************
// *						Level_GetDrawScale							   *
// *************************************************************************
float CL64_Level::Level_GetDrawScale(const Level* pLevel)
{
	return pLevel->DrawScale;
}

// *************************************************************************
// *						Level_GetLightmapScale						   *
// *************************************************************************
float CL64_Level::Level_GetLightmapScale(const Level* pLevel)
{
	return pLevel->LightmapScale;
}

// *************************************************************************
// *						Level_AppendBrush							   *
// *************************************************************************
void CL64_Level::Level_AppendBrush(Brush* pBrush)
{
	App->CL_Brush->BrushList_Append(App->CL_Doc->Current_Level->Brushes, pBrush);
}

// *************************************************************************
// *						Level_EnumLeafBrushes						   *
// *************************************************************************
int CL64_Level::Level_EnumLeafBrushes(void* lParam, BrushList_CB Callback)
{
	return App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Doc->Current_Level->Brushes, lParam, Callback);
}

// *************************************************************************
// *					Level_Delete_TXL_Class							   *
// *************************************************************************
void CL64_Level::Level_Delete_TXL_Class()
{
	Level* m_pLevel = App->CL_Doc->Current_Level;

	if (m_pLevel->WadSizeInfos != NULL)
	{
		App->CL_Maths->Ram_Free(m_pLevel->WadSizeInfos);
		m_pLevel->WadSizeInfos = NULL;
	}

	if (CL_Wad_Class != NULL)
	{
		delete CL_Wad_Class;
		CL_Wad_Class = NULL;
	}
}

// *************************************************************************
// *					Level_Create_TXL_Class							   *
// *************************************************************************
signed int CL64_Level::Level_Create_TXL_Class()
{
	Level* m_pLevel = App->CL_Doc->Current_Level;

	Level_Delete_TXL_Class();

	CL_Wad_Class = new CL64_WadFile();

	if (CL_Wad_Class == NULL)
	{
		App->Say("Cant Create Wad File", (LPSTR)"");

		return false;
	}


	if (CL_Wad_Class->Setup())
	{
		m_pLevel->WadSizeInfos = (SizeInfo*)App->CL_Maths->Ram_Allocate(sizeof(SizeInfo) * CL_Wad_Class->mBitmapCount);

		if (m_pLevel->WadSizeInfos != NULL)
		{
			int i;

			for (i = 0; i < App->CL_TXL_Editor->Texture_Count; i++)
			{
				SizeInfo* pInfo;
				WadFileEntry* Entry;

				pInfo = &(m_pLevel->WadSizeInfos[i]);
				Entry = &(CL_Wad_Class->mBitmaps[i]);

				pInfo->TexWidth = Entry->Width;
				pInfo->TexHeight = Entry->Height;
				//pInfo->TexData = (Guint8*)Entry->BitsPtr;
			}
		}
	}

	return  (m_pLevel->WadSizeInfos != NULL);
}

// *************************************************************************
// *							Level_GetWadPath						   *
// *************************************************************************
const char* CL64_Level::Level_GetWadPath()
{
	return (TXL_PathAndFile);
}

// *************************************************************************
// *							Level_SetWadPath						   *
// *************************************************************************
void CL64_Level::Level_SetWadPath(const char* NewWad)
{
	strcpy(TXL_PathAndFile, NewWad);
}

// *************************************************************************
// *							Level_GetWad_Class						   *
// *************************************************************************
CL64_WadFile* CL64_Level::Level_GetWad_Class()
{
	return CL_Wad_Class;
}

// *************************************************************************
// *						Level_GetDibIdFromWad						   *
// *************************************************************************
static Ogre::uint16 Level_GetDibIdFromWad(const CL64_WadFile* WadFile, const char* Name)
{
	Ogre::uint16	i;

	for (i = 0; i < WadFile->mBitmapCount; i++)
	{
		if (_strcmpi(WadFile->mBitmaps[i].Name, Name) == 0)
		{
			return i;
		}
	}

	return 0xffff;
}

// *************************************************************************
// *							Level_GetWadBitmap						   *
// *************************************************************************
WadFileEntry* CL64_Level::Level_GetWadBitmap(const char* Name)
{
	Ogre::uint16 i;

	i = Level_GetDibIdFromWad(CL_Wad_Class, Name);

	if (i != 0xffff)
	{
		return &(CL_Wad_Class->mBitmaps[i]);
	}
	else
	{
		return NULL;
	}
}

// *************************************************************************
// *							Level_GetDibId							   *
// *************************************************************************
Ogre::uint16 CL64_Level::Level_GetDibId(const char* Name)
{
	return Level_GetDibIdFromWad(CL_Wad_Class, Name);
}

// *************************************************************************
// *						Level_RemoveBrush							   *
// *************************************************************************
void CL64_Level::Level_RemoveBrush(Brush* pBrush)
{
	App->CL_Brush->BrushList_Remove(App->CL_Doc->Current_Level->Brushes, pBrush);
}

// *************************************************************************
// *						Level_EnumBrushes							   *
// *************************************************************************
int CL64_Level::Level_EnumBrushes(void* lParam, BrushList_CB Callback)
{
	return App->CL_Brush->BrushList_Enum(App->CL_Doc->Current_Level->Brushes, lParam, Callback);
}

// *************************************************************************
// *							Level_UseGrid							   *
// *************************************************************************
signed int CL64_Level::Level_UseGrid(const Level* pLevel)
{
	return 0;// pLevel->GridSettings.UseGrid;
}

// *************************************************************************
// *					Level_GetRotationSnap							   *
// *************************************************************************
int CL64_Level::Level_GetRotationSnap(const Level* pLevel)
{
	return pLevel->GridSettings.RotationSnap;
}

// *************************************************************************
// *					Level_GetRotationSnap							   *
// *************************************************************************
float CL64_Level::Level_GetGridSnapSize (const Level *pLevel)
{
	const GridInfo *pGridInfo = &pLevel->GridSettings;

	switch (pGridInfo->SnapType)
	{
		case GridMetric :
			return CENTIMETERS_TO_ENGINE (pGridInfo->MetricSnapSize);
			break;
		default :
			assert (0);
		case GridTexel :
			return (float)pGridInfo->TexelSnapSize;
			break;
	}
}

