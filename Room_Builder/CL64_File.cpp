/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
#include "CL64_File.h"

struct tag_Level
{
	BrushList* Brushes;
	char* WadPath;
	char* HeadersDir;
	char* ActorsDir;
	char* PawnIniPath;
	SizeInfo* WadSizeInfos;
	CL64_WadFile* WadFile;
	int GroupVisSetting;

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

struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	geBoolean Dirty;
	Box3d Bounds;
};

typedef struct TexInfoTag
{
	Ogre::Vector3 VecNormal;
	geFloat xScale, yScale;
	int xShift, yShift;
	geFloat	Rotate;			// texture rotation angle in degrees
	TexInfo_Vectors TVecs;
	int Dib;				// index into the wad
	char Name[16];
	geBoolean DirtyFlag;
	Ogre::Vector3 Pos;
	int txSize, tySize;		// texture size (not currently used)
	Matrix3d XfmFaceAngle;	// face rotation angle
} TexInfo;

typedef struct FaceTag
{
	int				NumPoints;
	int				Flags;
	GPlane			Face_Plane;
	int				LightIntensity;
	float			Reflectivity;
	float			Translucency;
	float			MipMapBias;
	float			LightXScale, LightYScale;
	TexInfo			Tex;
	Ogre::Vector3* Points;

} Face;

CL64_File::CL64_File(void)
{
    Level_Version = 1.0;

	PathFileName_3dt[0] = 0;
	FileName_3dt[0] = 0;

}

CL64_File::~CL64_File(void)
{
}

// *************************************************************************
// *	            Save:- Terry and Hazel Flanigan 2025	               *
// *************************************************************************
void CL64_File::Save()
{
    int BC = App->CL_Brush->Get_Brush_Count();
    if (BC > 0)
    {
        Save_Document();
    }
    else
    {
        App->Say("No Brushes to Save");
    }

    App->Say("Saved");
}

// *************************************************************************
// *	          Save_Document:- Terry and Hazel Flanigan 2025	           *
// *************************************************************************
void CL64_File::Save_Document()
{
    if (Save(App->CL_Doc->mCurrent_MTF_PathAndFile) == GE_FALSE)
    {
        App->Say("Error: Unable to save file");
        return;;
    }

    App->CL_Doc->flag_IsNewDocument = 0;
    App->CL_Doc->flag_Is_Modified = false;
}

// *************************************************************************
// *				Save:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_File::Save(const char* FileName)
{
    FILE* Write_File = NULL;

    Write_File = fopen(FileName, "wt");

    fprintf(Write_File, "MTF_Version %f\n", Level_Version);

    fprintf(Write_File, "TextureLib %s\n", App->CL_Doc->pLevel->WadPath);

	fprintf(Write_File, "Brush_Count %i\n", App->CL_Brush->Get_Brush_Count());

	BrushList_Write(App->CL_Doc->pLevel->Brushes, Write_File);

    fclose(Write_File);

    return 1;
}

// *************************************************************************
// *			BrushList_Write:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
signed int CL64_File::BrushList_Write(BrushList* BList, FILE* ofile)
{
	Brush* pBrush;
	BrushIterator bi;
	int Count;

	Count = App->CL_Brush->BrushList_Count(BList, (BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF | BRUSH_COUNT_NORECURSE));
	if (fprintf(ofile, "Brushlist %d\n", Count) < 0) return GE_FALSE;

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		if (!Brush_Write(pBrush, ofile)) return GE_FALSE;
		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}
	return GE_TRUE;
}

// *************************************************************************
// *			BrushList_Write:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
signed int CL64_File::Brush_Write(const Brush* b, FILE* ofile)
{
	if (b->Type == BRUSH_CSG)
	{
		return GE_TRUE;
	}

	{
		if (fprintf(ofile, "Brush %s\n", b->Name) < 0) return GE_FALSE;
	}

	if (fprintf(ofile, "\tFlags %d\n", b->Flags) < 0) return GE_FALSE;
	if (fprintf(ofile, "\tModelId %d\n", b->ModelId) < 0) return GE_FALSE;
	if (fprintf(ofile, "\tGroupId %d\n", b->GroupId) < 0) return GE_FALSE;
	{
		if (b->HullSize < 1.0f)
		{
			((Brush*)b)->HullSize = 1.0f;
		}
		if (fprintf(ofile, "\tHullSize %f\n", b->HullSize) < 0) return GE_FALSE;
	}
	if (fprintf(ofile, "\tType %d\n", b->Type) < 0) return GE_FALSE;

	switch (b->Type)
	{
	case	BRUSH_MULTI:
		return BrushList_Write(b->BList, ofile);

	case	BRUSH_LEAF:
		return FaceList_Write(b->Faces, ofile);

	default:
		assert(0);		// invalid brush type
		break;
	}
	return GE_TRUE;
}

// *************************************************************************
// *			FaceList_Write:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
signed int CL64_File::FaceList_Write(const FaceList* pList, FILE* f)
{
	int i;

	assert(pList != NULL);
	assert(f != NULL);

	if (fprintf(f, "\tBrushFaces %d\n", pList->NumFaces) < 0) return GE_FALSE;
	for (i = 0; i < pList->NumFaces; i++)
	{
		if (!Face_Write(pList->Faces[i], f)) return GE_FALSE;
	}
	return GE_TRUE;
}

// *************************************************************************
// *				Face_Write:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
signed int CL64_File::Face_Write(const Face* f, FILE* wf)
{
	int	i, xShift, yShift;
	float xScale, yScale, Rotate;

	fprintf(wf, "\t\tNumPoints %d\n", f->NumPoints);
	fprintf(wf, "\t\tFlags %d\n", f->Flags);
	fprintf(wf, "\t\tLight %d\n", f->LightIntensity);
	fprintf(wf, "\t\tMipMapBias %f\n", f->MipMapBias);
	fprintf(wf, "\t\tTranslucency %f\n", f->Translucency);
	fprintf(wf, "\t\tReflectivity %f\n", f->Reflectivity);

	for (i = 0; i < f->NumPoints; i++)
	{
		if (fprintf(wf, "\t\t\tVec3d %f %f %f\n", f->Points[i].x, f->Points[i].y, f->Points[i].z) < 0) return GE_FALSE;
	}

	App->CL_Face->Face_GetTextureShift(f, &xShift, &yShift);
	App->CL_Face->Face_GetTextureScale(f, &xScale, &yScale);
	Rotate = App->CL_Face->Face_GetTextureRotate(f);
		
	fprintf(wf, "\t\t\tTexInfo Rotate %f Shift %d %d Scale %f %f Name %s\n",
		Rotate, xShift, yShift, xScale, yScale, App->CL_Face->Face_GetTextureName(f));
		
	
	fprintf(wf, "\t\tLightScale %f %f\n", f->LightXScale, f->LightYScale);

	fprintf(wf, "%s%f %f %f %f %f %f %f %f %f %f %f %f\n", "\tTransform=",
		f->Tex.XfmFaceAngle.AX, f->Tex.XfmFaceAngle.AY, f->Tex.XfmFaceAngle.AZ,
		f->Tex.XfmFaceAngle.BX, f->Tex.XfmFaceAngle.BY, f->Tex.XfmFaceAngle.BZ,
		f->Tex.XfmFaceAngle.CX, f->Tex.XfmFaceAngle.CY, f->Tex.XfmFaceAngle.CZ,
		f->Tex.XfmFaceAngle.Translation.x, f->Tex.XfmFaceAngle.Translation.y, f->Tex.XfmFaceAngle.Translation.z);

	fprintf(wf, "%s%f %f %f\n", "\tPos=", f->Tex.Pos.x, f->Tex.Pos.y, f->Tex.Pos.z);
	
	return true;
}


// ----------------------------------------------------------------------
// --------------------------- Open -------------------------------------
// ----------------------------------------------------------------------

// *************************************************************************
// *	            Open:- Terry and Hazel Flanigan 2025	               *
// *************************************************************************
void CL64_File::Open()
{
	char FullPath[MAX_PATH];

	strcpy(FullPath, App->RB_Directory_FullPath);
	strcat(FullPath, "\\");
	strcat(FullPath, "Data");
	strcat(FullPath, "\\");
	strcat(FullPath, "Room_Builder");
	strcat(FullPath, "\\");
	strcat(FullPath, "Test.mtf");

	strcpy(App->CL_File->PathFileName_3dt, FullPath);
	strcpy(App->CL_File->FileName_3dt, "Test.mtf");

	//App->Say_Win(App->CL_File->PathFileName_3dt);

	//App->CLSB_File_WE->Open_Example_File();
	Open_3dt_File();
	//App->Say("Opened");
}

// *************************************************************************
// *			Open_3dt_File:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_File::Open_3dt_File()
{
	Load_File(PathFileName_3dt);

	//App->m_pDoc->SetTitle(PathFileName_3dt);
	//App->m_pDoc->SetPathName(PathFileName_3dt, FALSE);

	//App->CL_World->Set_Paths();

	//if (Quick_load_Flag == 0)
	//{
	//	App->CLSB_TextureDialog->Fill_ListBox();
	//}
	//else
	//{
	//	char Txlpath[MAX_PATH];
	//	strcpy(Txlpath, App->WorldEditor_Directory);
	//	strcat(Txlpath, "Data\\Default.txl");

	//	Level_SetWadPath(App->CLSB_Doc->pLevel, Txlpath);
	//	App->CL_World->Set_Current_TxlPath();
	//	App->CLSB_Doc->UpdateAfterWadChange();
	//	App->CLSB_TextureDialog->Fill_ListBox();

	//	strcpy(App->CL_World->mCurrent_TXL_FileName, Txlpath);

	//}

	////App->CLSB_Level->Check_For_Centre_Brush(); // Centre Brush XYZ
	////App->CLSB_Level->Check_For_Centre_Texture(); // Centre Texture Dummy

	//App->CL_TabsGroups_Dlg->Fill_ListBox();

	////App->CLSB_Level->Change_Centre_Brush_Texture();

	//App->CL_World->Set_Paths();
	//App->CL_World->Reset_Editor();

	//App->CLSB_RecentFiles->RecentFile_Files_Update();

	return 1;
}

// *************************************************************************
// *								Load_File 							   *
// *************************************************************************
bool CL64_File::Load_File(const char* FileName)
{
	App->CL_ParseFile->Load_File((LPSTR)FileName);
//	App->Get_Current_Document();
//
//	const char* Errmsg, * WadPath;
//	int				i;
//	Level* NewLevel;
//	EntityViewList* pEntityView;
//	const Prefs* pPrefs = App->m_pDoc->GetPrefs();
//
//	/*char WorkingDir[MAX_PATH];
//	FilePath_GetDriveAndDir(FileName, WorkingDir);
//	::SetCurrentDirectory(WorkingDir);*/
//
	//bool Test = NewLevel = Level_CreateFromFile(FileName);// , & Errmsg, Prefs_GetHeadersList(pPrefs),
		//Prefs_GetActorsList(pPrefs), Prefs_GetPawnIni(pPrefs));
//
//	if (NewLevel == NULL)
//	{
//		goto LoadError;
//	}
//
//	// get fully-qualified path name to texture library
//	WadPath = Level_GetWadPath(NewLevel);
//
//	if (!Level_LoadWad(NewLevel))
//	{
//		App->Say("Can not open TXL File");
//	}
//
//	Level_EnumLeafBrushes(NewLevel, NewLevel, Level_FaceFixupCallback);
//
//	if (App->CLSB_Doc->pLevel != NULL)
//	{
//		Level_Destroy(&App->CLSB_Doc->pLevel);
//	}
//
//	App->CLSB_Doc->pLevel = NewLevel;
//
//	// Validate data, groups are read after entities and brushes, so this must be last
//	if (App->m_pDoc->ValidateEntities() == FALSE || App->m_pDoc->ValidateBrushes() == FALSE)
//	{
//		//m_pDoc->SelectTab( m_pDoc->CONSOLE_TAB ) ;
//		App->Say("Can not open Validate Brushes");
//	}
//
//	GroupIterator gi;
//	GroupListType* Groups;
//
//	Groups = Level_GetGroups(App->CLSB_Doc->pLevel);
//	App->CLSB_Doc->mCurrentGroup = Group_GetFirstId(Groups, &gi);
//	{
//		Brush* pBox = BrushTemplate_CreateBox(Level_GetBoxTemplate(App->CLSB_Doc->pLevel));
//		if (pBox != NULL)
//		{
//			CreateNewTemplateBrush(pBox);
//		}
//		else
//		{
//			App->Say("Error");
//		}
//	}
//
//	// update entity visibility info
//	pEntityView = Level_GetEntityVisibilityInfo(App->CLSB_Doc->pLevel);
//	for (i = 0; i < pEntityView->nEntries; ++i)
//	{
//		Level_EnumEntities(App->CLSB_Doc->pLevel, &pEntityView->pEntries[i], ::fdocSetEntityVisibility);
//	}
//
//
//	AddCameraEntityToLevel();
//
//	App->CLSB_Doc->DoGeneralSelect();
//
//	return GE_TRUE;
//LoadError:
//	if (NewLevel != NULL)
//	{
//		Level_Destroy(&NewLevel);
//	}
//
//	App->Say("Error Loading File");

	return GE_FALSE;
}

// *************************************************************************
// * 						Level_CreateFromFile						   *
// *************************************************************************
Level* CL64_File::Level_CreateFromFile(const char* FileName)//, const char** ErrMsg, const char* DefaultHeadersDir, const char* DefaultActorsDir, const char* DefaultPawnIni)
{
	//int NumModels;
	//int VersionMajor, VersionMinor;
	//int NumGroups = 0;
	//int NumBrushes = 0;
	//int NumEntities;
	////Parse3dt* Parser;
	//const char* Expected = "!*ERROR*!";
	//int k;
	Level* pLevel = NULL;
	//char WadPath[MAX_PATH];
	//char HeadersDir[MAX_PATH];

	//char ActorsDir[MAX_PATH];
	//char PawnIniPath[MAX_PATH];

	/*Parser = Parse3dt_Create(FileName);
	if (Parser == NULL)
	{
		*ErrMsg = "Can't open file";
		return NULL;
	}*/

	/*Expected = "3dtVersion";
	if (!Parse3dt_GetVersion(Parser, &VersionMajor, &VersionMinor)) goto DoneLoad;

	if (VersionMajor > LEVEL_VERSION_MAJOR)
	{
		*ErrMsg = "Version mismatch.";
		return NULL;
	}*/

//	if (VersionMajor <= 1 && VersionMinor < 16)
//	{
//		char	PalPath[_MAX_PATH];
//
//		if (!Parse3dt_GetPath(Parser, (Expected = "PalLoc"), PalPath)) goto DoneLoad;
//	}
//
//	// texture library path
//	if ((VersionMajor <= 1) && (VersionMinor < 18))
//	{
//		if (!Parse3dt_GetPath(Parser, (Expected = "WadLoc"), WadPath)) goto DoneLoad;
//	}
//	else
//	{
//		if (!Parse3dt_GetLiteral(Parser, (Expected = "TextureLib"), WadPath))
//		{
//			goto DoneLoad;
//		}
//
//		char m_FileName[MAX_PATH];
//		strcpy(m_FileName, FileName);
//
//		bool test = Resolve_TXL_File(WadPath, m_FileName);
//		if (test == 0)
//		{
//			goto DoneLoad;
//		}
//
//	}
//
//	// headers directory
//	if ((VersionMajor <= 1) && (VersionMinor < 31))
//	{
//		strcpy(HeadersDir, DefaultHeadersDir);
//	}
//	else
//	{
//		if (!Parse3dt_GetLiteral(Parser, (Expected = "HeadersDir"), HeadersDir)) goto DoneLoad;
//	}
//	// changed QD Actors
//		// actors directory
//	if ((VersionMajor <= 1) && (VersionMinor < 33))
//	{
//		strcpy(ActorsDir, DefaultActorsDir);
//	}
//	else
//	{
//		if (!Parse3dt_GetLiteral(Parser, (Expected = "ActorsDir"), ActorsDir)) goto DoneLoad;
//	}
//	// PawnIni
//	if ((VersionMajor <= 1) && (VersionMinor < 33))
//	{
//		strcpy(PawnIniPath, DefaultPawnIni);
//	}
//	else
//	{
//		if (!Parse3dt_GetLiteral(Parser, (Expected = "PawnIni"), PawnIniPath)) goto DoneLoad;
//	}
//
//	pLevel = Level_Create(WadPath, HeadersDir, ActorsDir, PawnIniPath);
//	// end change
//	if (pLevel == NULL)
//	{
//		*ErrMsg = "Error creating level.";
//		return NULL;
//	}
//
//
//	if ((VersionMajor == 1) && (VersionMinor < 15))
//	{
//		if (!Parse3dt_GetInt(Parser, (Expected = "NumBrushes"), &NumBrushes)) goto DoneLoad;
//	}
//	if (!Parse3dt_GetInt(Parser, (Expected = "NumEntities"), &NumEntities)) goto DoneLoad;
//	if (!Parse3dt_GetInt(Parser, (Expected = "NumModels"), &NumModels)) goto DoneLoad;
//
//	if ((VersionMajor > 1) || ((VersionMajor == 1) && (VersionMinor >= 3)))
//	{
//		if (!Parse3dt_GetInt(Parser, (Expected = "NumGroups"), &NumGroups)) goto DoneLoad;
//	}
//
//
//	if ((VersionMajor == 1) && (VersionMinor < 15))
//	{
//		for (k = 0; k < NumBrushes; k++)
//		{
//			Brush* pBrush;
//
//			pBrush = Brush_CreateFromFile(Parser, VersionMajor, VersionMinor, &Expected);
//			if (pBrush == NULL) goto DoneLoad;
//			BrushList_Append(pLevel->Brushes, pBrush);
//
//		}
//	}
//	else
//	{
//		if (pLevel->Brushes != NULL)
//		{
//			BrushList_Destroy(&pLevel->Brushes);
//		}
//
//		pLevel->Brushes = BrushList_CreateFromFile(Parser, VersionMajor, VersionMinor, &Expected);
//		if (pLevel->Brushes == NULL)
//			goto DoneLoad;
//	}
//
//	if ((VersionMajor > 1) || ((VersionMajor == 1) && (VersionMinor < 6)))
//	{
//		Level_BrushListToTexels(pLevel);
//	}
//
//	// collapse model list so numbers are consecutive
//	Level_CollapseModels(pLevel, 1);
//
//	goto AllDone;
//
//DoneLoad:
//	*ErrMsg = Parse3dt_Error(Parser, "Expected %s", Expected);
//
//	//DoneLoad1:
//	if (pLevel != NULL)
//	{
//		Level_Destroy(&pLevel);
//	}
//
//AllDone:
//	if (Parser != NULL)
//	{
//		Parse3dt_Destroy(&Parser);
//	}
//
//	//fixup hollows
//	if (pLevel != NULL)
//	{
//		BrushList_MakeHollowsMulti(pLevel->Brushes);
//	}

App->Say("Here Level_CreateFromFile");

	return pLevel;
}
