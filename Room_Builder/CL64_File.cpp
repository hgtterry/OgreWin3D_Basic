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

	Read_Buffer[0] = 0;
	WadPath[0] = 0;
	WadFile_Name[0] = 0;

	fp = NULL;

}

CL64_File::~CL64_File(void)
{
}

// *************************************************************************
// *	          Start_Save:- Terry and Hazel Flanigan 2025		       *
// *************************************************************************
void CL64_File::Start_Save(bool Use_Save_Dialog)
{
    int BC = App->CL_Brush->Get_Brush_Count();
    if (BC > 0)
    {
		if (Use_Save_Dialog == 1)
		{
			App->CL_File_IO->Save_File();

			if (App->CL_File_IO->flag_Canceled == 1)
			{
				return;
			}

			strcpy(App->CL_Doc->mCurrent_MTF_PathAndFile, App->CL_File_IO->sFilePath.c_str());
		}

        Save_Document();

		App->Say("Saved");
    }
    else
    {
        App->Say("No Brushes to Save");
    }

    
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

	//fprintf(Write_File, "Brush_Count %i\n", App->CL_Brush->Get_Brush_Count());

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

	fprintf(wf, "%s%f %f %f %f %f %f %f %f %f %f %f %f\n", "\tTransform ",
		f->Tex.XfmFaceAngle.AX, f->Tex.XfmFaceAngle.AY, f->Tex.XfmFaceAngle.AZ,
		f->Tex.XfmFaceAngle.BX, f->Tex.XfmFaceAngle.BY, f->Tex.XfmFaceAngle.BZ,
		f->Tex.XfmFaceAngle.CX, f->Tex.XfmFaceAngle.CY, f->Tex.XfmFaceAngle.CZ,
		f->Tex.XfmFaceAngle.Translation.x, f->Tex.XfmFaceAngle.Translation.y, f->Tex.XfmFaceAngle.Translation.z);

	fprintf(wf, "%s%f %f %f\n", "\tPos ", f->Tex.Pos.x, f->Tex.Pos.y, f->Tex.Pos.z);
	
	return true;
}


// ----------------------------------------------------------------------
// --------------------------- Open -------------------------------------
// ----------------------------------------------------------------------

// *************************************************************************
// *	           Start_Load:- Terry and Hazel Flanigan 2025	           *
// *************************************************************************
void CL64_File::Start_Load(bool Use_Open_Dialog)
{
	if (Use_Open_Dialog == 1)
	{
		bool test = App->CL_File_IO->Open_File();
		if (test == false)
		{
			return;
		}

		strcpy(PathFileName_3dt, App->CL_File_IO->sFilePath.c_str());
		strcpy(FileName_3dt, App->CL_File_IO->sSelectedFile.c_str());
	}

	bool Test = Open_3dt_File();
	if (Test == true)
	{
		App->Set_Title(PathFileName_3dt);
		App->Say("File Loaded", App->CL_File->FileName_3dt);
	}
	else
	{
		App->Say("Failed To Open File", FileName_3dt);
	}
}

// *************************************************************************
// *			Open_3dt_File:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_File::Open_3dt_File()
{
	App->CL_Doc->DoGeneralSelect();
	
	bool Test = Load_File(PathFileName_3dt);

	if (Test == true)
	{
		App->CL_Doc->Set_Paths();

		// Temporary for now hgttery Debug
		static char Path_And_File[MAX_PATH];
		strcpy(Path_And_File, App->RB_Directory_FullPath);
		strcat(Path_And_File, "\\Data\\Room_Builder\\");
		strcat(Path_And_File, "Default.txl");
		strcpy(WadPath, Path_And_File);

		if (!App->CL_Level->Level_LoadWad(App->CL_Doc->pLevel))
		{
			App->Say_Win("Can not load Wad File");
		}

		App->CL_Doc->UpdateAfterWadChange();

		App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);

		/*Level_SetWadPath(App->CLSB_Doc->pLevel, Txlpath);
		App->CL_World->Set_Current_TxlPath();
		App->CLSB_Doc->UpdateAfterWadChange();
		App->CLSB_TextureDialog->Fill_ListBox();

		strcpy(App->CL_World->mCurrent_TXL_FileName, Txlpath);*/

	}

	////App->CLSB_Level->Check_For_Centre_Brush(); // Centre Brush XYZ
	////App->CLSB_Level->Check_For_Centre_Texture(); // Centre Texture Dummy

	//App->CL_TabsGroups_Dlg->Fill_ListBox();

	////App->CLSB_Level->Change_Centre_Brush_Texture();

	//App->CL_World->Set_Paths();
	//App->CL_World->Reset_Editor();

	//App->CLSB_RecentFiles->RecentFile_Files_Update();

	return true;
}

// *************************************************************************
// *								Load_File 							   *
// *************************************************************************
bool CL64_File::Load_File(const char* FileName)
{
	Level* pLevel = NULL;

	int Count = 0;

	fp = fopen(FileName, "r");
	if (!fp)
	{
		App->Say("Can Not Find File");
		return false;
	}

	memset(Read_Buffer, 0, MAX_PATH);

	while (fgets(Read_Buffer, sizeof(Read_Buffer), fp))
	{
		App->CL_ParseFile->Tag_Float = 0;

		if (App->CL_ParseFile->Get_Version(Read_Buffer) == 0)
		{
			break;
		}
		
		fgets(Read_Buffer, sizeof(Read_Buffer), fp);
		if (App->CL_ParseFile->Get_TextureLib(Read_Buffer) == 0)
		{
			break;
		}

		if (App->CL_Doc->pLevel->Brushes != NULL)
		{
			App->CL_Brush->BrushList_Destroy(&App->CL_Doc->pLevel->Brushes);
			App->CL_Doc->pLevel->Brushes = NULL;
		}
		
		pLevel = App->CL_Level->Level_Create(WadPath, NULL, NULL, NULL);
		if (pLevel == NULL)
		{
			App->Say("Can not Create Level");
			break;
		}
		
		pLevel->Brushes = App->CL_ParseFile->BrushList_CreateFromFile();
		if (pLevel->Brushes == NULL)
		{
			App->Say("Can not Create Brushes");
			break;
		}
		
		break;

		Count++;
		memset(Read_Buffer, 0, MAX_PATH);
	}

	fclose(fp);
	
	App->CL_Doc->pLevel = pLevel;

	App->CL_Properties_Brushes->Fill_ListBox();

	return true;
}

