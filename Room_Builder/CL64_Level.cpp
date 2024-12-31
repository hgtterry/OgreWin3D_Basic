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

struct tag_Level
{
	BrushList* Brushes;
	//CEntityArray* Entities;
	char* WadPath;
	char* HeadersDir;
	// changed QD Actors
	char* ActorsDir;
	//geBoolean ShowActors;
	char* PawnIniPath;
	// end change
	//EntTypeNameList* EntTypeNames;
	//GroupListType* Groups;
	//SizeInfo* WadSizeInfos;
	//CWadFile* WadFile;
	//EntityTable* pEntityDefs;

	//ModelInfo_Type	ModelInfo;

	//SkyFaceTexture SkyFaces[6];
	//geVec3d SkyRotationAxis;
	//geFloat SkyRotationSpeed;
	//geFloat	SkyTextureScale;

	// level edit settings
	//CompileParamsType CompileParams;
	int GroupVisSetting;
	//EntityViewList* pEntityView;

	//GridInfo GridSettings;
	//geBoolean BspRebuildFlag;
	//ViewStateInfo ViewInfo[NUM_VIEWS];

	BrushTemplate_Arch ArchTemplate;
	BrushTemplate_Box	BoxTemplate;
	BrushTemplate_Cone	ConeTemplate;
	BrushTemplate_Cylinder CylinderTemplate;
	BrushTemplate_Spheroid	SpheroidTemplate;
	BrushTemplate_Staircase StaircaseTemplate;

	Ogre::Vector3 TemplatePos;

	float DrawScale;		// default draw scale
	float LightmapScale;	// default lightmap scale
};

CL64_Level::CL64_Level(void)
{
}

CL64_Level::~CL64_Level(void)
{
}

// *************************************************************************
// *							Level_Create							   *
// *************************************************************************
Level* CL64_Level::Level_Create(const char* pWadName, const char* HeadersDir, const char* ActorsDir, const char* PawnIni)
{
	Level* pLevel = NULL;

	pLevel = new Level;

	if (pLevel != NULL)
	{
		pLevel->Brushes = App->CL_Brush->BrushList_Create();
		if (pLevel->Brushes == NULL)
		{
			App->Say_Win("Can not create brush list");
			return pLevel;
		}

		/*pLevel->Entities = new (CEntityArray);
		if (pLevel->Entities == NULL) goto CreateError;*/

		/*pLevel->Entities->SetSize(0, 20);*/

		/*pLevel->EntTypeNames = EntTypeNameList_Create();
		if (pLevel->EntTypeNames == NULL) goto CreateError;*/

		//pLevel->Groups = Group_CreateList();
		//if (pLevel->Groups == NULL) goto CreateError;

		//{
		//	// add the default group
		//	Group* pGroup = Group_Create(0, "Default");
		//	if (pGroup != NULL)
		//	{
		//		GroupList_Add(pLevel->Groups, pGroup);
		//	}
		//}


		/*pLevel->ModelInfo.CurrentModel = 0;
		pLevel->ModelInfo.Models = ModelList_Create();
		if (pLevel->ModelInfo.Models == NULL) goto CreateError;

		pLevel->HeadersDir = NULL;
		pLevel->pEntityDefs = NULL;
		if (Level_LoadEntityDefs(pLevel, HeadersDir) == GE_FALSE)
		{
			goto CreateError;
		}*/

		/*pLevel->ActorsDir = Util_Strdup(ActorsDir);
		pLevel->ShowActors = GE_TRUE;
		pLevel->PawnIniPath = Util_Strdup(PawnIni);*/

		/*pLevel->WadPath = Util_Strdup(pWadName);
		pLevel->WadFile = NULL;
		pLevel->WadSizeInfos = NULL;*/

		// initialize sky
		/*geVec3d_Set(&pLevel->SkyRotationAxis, 1.0f, 0.0f, 0.0f);
		pLevel->SkyRotationSpeed = 10.0f;
		pLevel->SkyTextureScale = 1.0f;
		for (int i = 0; i < 6; ++i)
		{
			pLevel->SkyFaces[i].TextureName = NULL;
			pLevel->SkyFaces[i].Apply = GE_FALSE;
		}*/

		// Set default compile dialog params
		/*pLevel->CompileParams.EntitiesOnly = GE_FALSE;
		pLevel->CompileParams.VisDetailBrushes = GE_FALSE;
		pLevel->CompileParams.DoVis = GE_TRUE;
		pLevel->CompileParams.DoLight = GE_TRUE;
		pLevel->CompileParams.RunBsp = GE_TRUE;
		pLevel->CompileParams.RunPreview = GE_TRUE;
		pLevel->CompileParams.UseMinLight = GE_TRUE;
		pLevel->CompileParams.SuppressHidden = GE_FALSE;
		pLevel->CompileParams.Filename[0] = '\0';*/

		/*pLevel->CompileParams.Light.Verbose = GE_FALSE;
		pLevel->CompileParams.Light.ExtraSamples = GE_FALSE;
		pLevel->CompileParams.Light.LightScale = 1.0f;
		pLevel->CompileParams.Light.Radiosity = GE_FALSE;
		pLevel->CompileParams.Light.NumBounce = 10;
		pLevel->CompileParams.Light.PatchSize = 128.0f;
		pLevel->CompileParams.Light.FastPatch = GE_FALSE;
		pLevel->CompileParams.Light.ReflectiveScale = 1.0f;
		geVec3d_Set(&pLevel->CompileParams.Light.MinLight, 128.0f, 128.0f, 128.0f);*/

		/*pLevel->CompileParams.Bsp.Verbose = GE_FALSE;
		pLevel->CompileParams.Bsp.EntityVerbose = GE_FALSE;

		pLevel->CompileParams.Vis.Verbose = GE_FALSE;
		pLevel->CompileParams.Vis.FullVis = GE_FALSE;
		pLevel->CompileParams.Vis.SortPortals = GE_TRUE;

		pLevel->GroupVisSetting = Group_ShowVisible;

		pLevel->pEntityView = EntityViewList_Create(pLevel->pEntityDefs);*/

		// grid settings
		//default to texel grid and snap
		//with rotational snap of 15
		{
			/*GridInfo* pGridInfo;

			pGridInfo = &pLevel->GridSettings;
			pGridInfo->UseGrid = GE_TRUE;
			pGridInfo->GridType = GridTexel;
			pGridInfo->SnapType = GridTexel;
			pGridInfo->MetricSnapSize = GridSize_Decimeter;
			pGridInfo->TexelSnapSize = 8;
			pGridInfo->RotationSnap = 15;*/
		}

		//		pLevel->BspRebuildFlag = GE_TRUE;
		/*pLevel->BspRebuildFlag = GE_FALSE;
		for (int iView = 0; iView < NUM_VIEWS; ++iView)
		{
			ViewStateInfo* pInfo;

			pInfo = &pLevel->ViewInfo[iView];
			pInfo->IsValid = GE_FALSE;
			pInfo->ZoomFactor = 1.0f;
			geVec3d_Clear(&pInfo->PitchRollYaw);
			geVec3d_Set(&pInfo->CameraPos, 0.0f, 0.0f, 0.0f);
		}*/

		App->CL_BrushTemplate->BrushTemplate_ArchDefaults(&pLevel->ArchTemplate);
		App->CL_BrushTemplate->BrushTemplate_BoxDefaults(&pLevel->BoxTemplate);
		App->CL_BrushTemplate->BrushTemplate_ConeDefaults(&pLevel->ConeTemplate);
		App->CL_BrushTemplate->BrushTemplate_CylinderDefaults(&pLevel->CylinderTemplate);
		App->CL_BrushTemplate->BrushTemplate_SpheroidDefaults(&pLevel->SpheroidTemplate);
		App->CL_BrushTemplate->BrushTemplate_StaircaseDefaults(&pLevel->StaircaseTemplate);

		App->CL_Maths->Vector3_Clear(&pLevel->TemplatePos);

		pLevel->DrawScale = 1.0f;
		pLevel->LightmapScale = 2.0f;
		//Debug
	}

	return pLevel;

//CreateError:
//	Level_Destroy(&pLevel);
//	return pLevel;
}

// *************************************************************************
// *						Level_GetBoxTemplate						   *
// *************************************************************************
BrushTemplate_Box* CL64_Level::Level_GetBoxTemplate(Level* pLevel)
{
	return &pLevel->BoxTemplate;
}

// *************************************************************************
// *							Level_GetBrushes						   *
// *************************************************************************
BrushList* CL64_Level::Level_GetBrushes(Level* pLevel)
{
	return pLevel->Brushes;
}

// *************************************************************************
// *						Level_GetTemplatePos						   *
// *************************************************************************
Ogre::Vector3* CL64_Level::Level_GetTemplatePos(Level* pLevel)
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
