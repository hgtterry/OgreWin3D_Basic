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
#include "Structures.cpp"




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
			LPCWSTR mType = L"Mesh Text File(*.mtf) *.mtf";
			LPCWSTR mExtension = L" *.mtf";

			App->CL_File_IO->Save_File(mType, mExtension);

			if (App->CL_File_IO->flag_Canceled == 1)
			{
				return;
			}

			strcpy(App->CL_Doc->mDoc_MTF_PathAndFile, App->CL_File_IO->s_Path_And_File.c_str());

			if (_stricmp(App->CL_Doc->mDoc_MTF_PathAndFile + strlen(App->CL_Doc->mDoc_MTF_PathAndFile) - 4, ".mtf") == 0)
			{
				
			}
			else
			{
				strcat(App->CL_Doc->mDoc_MTF_PathAndFile, ".mtf");
			}

			App->CL_Utilities->Get_FileName_FromPath(App->CL_Doc->mDoc_MTF_PathAndFile, App->CL_Doc->mDoc_MTF_PathAndFile);

			strcpy(App->CL_Doc->mDoc_MTF_Just_FileName, App->CL_Utilities->JustFileName);
			
			char buf[MAX_PATH];
			strcpy(buf, App->CL_Doc->mDoc_MTF_Just_FileName);
			int Len = strlen(buf);
			buf[Len - 4] = 0;
			strcpy(App->CL_Doc->mDoc_MTF_JustName_NoExt, buf);

			strcpy(App->CL_Export->mJustName, App->CL_Doc->mDoc_MTF_JustName_NoExt);

		}

		Save_Document();

		App->Set_Title(App->CL_Doc->mDoc_MTF_PathAndFile);

		App->Say("Saved", App->CL_Doc->mDoc_MTF_Just_FileName);
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
	if (Save(App->CL_Doc->mDoc_MTF_PathAndFile) == GE_FALSE)
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

	fprintf(Write_File, "TextureLib %s\n", "Default.txl");// App->CL_Doc->pLevel->WadPath);

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
		LPCWSTR mType = L"Mesh Text File";
		LPCWSTR mExtensions = L"*.mtf";

		bool test = App->CL_File_IO->Open_File((LPCWSTR)mType, (LPCWSTR)mExtensions);
		if (test == false)
		{
			return;
		}

		strcpy(PathFileName_3dt, App->CL_File_IO->s_Path_And_File.c_str());
		strcpy(FileName_3dt, App->CL_File_IO->s_Just_FileName.c_str());

		strcpy(App->CL_Doc->mDoc_MTF_PathAndFile, App->CL_File_IO->s_Path_And_File.c_str());
		strcpy(App->CL_Doc->mDoc_MTF_Just_FileName, App->CL_File_IO->s_Just_FileName.c_str());

		char buf[MAX_PATH];
		strcpy(buf, App->CL_Doc->mDoc_MTF_Just_FileName);
		int Len = strlen(buf);
		buf[Len - 4] = 0;
		strcpy(App->CL_Doc->mDoc_MTF_JustName_NoExt, buf);

		strcpy(App->CL_Export->mJustName, App->CL_Doc->mDoc_MTF_JustName_NoExt);

	}

	bool Test = Open_3dt_File();
	if (Test == true)
	{
		App->CL_Doc->DoGeneralSelect(false);

		Set_Editor();

		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
		App->Say("File Loaded", App->CL_File->FileName_3dt);

		App->CL_Doc->Set_Faces_To_Brush_Name_All(); // Fix up Brush Names
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
	App->CL_Doc->ResetAllSelections();
	App->CL_Doc->DoGeneralSelect(false);
	
	bool Test = Load_File(PathFileName_3dt);

	if (Test == true)
	{
		App->CL_Doc->Set_Paths();

		// TODO Temporary for now hgttery Debug
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

		Set_Player();

		App->CL_Properties_Brushes->Fill_ListBox();

		App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);

	}

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
		
		pLevel = App->CL_Level->Level_Create();
		if (pLevel == NULL)
		{
			App->Say("Can not Create Level");
			break;
		}
		
		pLevel->Brushes = App->CL_ParseFile->BrushList_CreateFromFile(false);
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

	return true;
}

// *************************************************************************
// *			Set_Editor:- Terry and Hazel Flanigan 2025 				   *
// *************************************************************************
void CL64_File::Set_Editor()
{
	App->Set_Title(App->CL_Doc->mDoc_MTF_PathAndFile);
	App->CL_Top_Tabs->Enable_Select_Button(true, 1);
	App->CL_Properties_Templates->Enable_Insert_Button(false);

	App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();

	App->CL_Face->Selected_Face_Index = 0;
	//App->CL_Top_Tabs->Update_Faces_Combo();
	
	App->CL_Doc->ResetAllSelections();
	App->CL_Top_Tabs->Enable_Brush_Options_Buttons(false, false);
	App->CL_Properties_Tabs->Select_Templates_Tab();
	App->CL_Ogre->Camera_Reset_Zero();

	App->CL_Editor_Map->Reset_Views_All();

	App->CL_Doc->Editor_Set_Dlgs(Enums::Editor_Dlgs_First_Brush);
	App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(false);


	if (App->Development == 1)
	{
		App->CL_Entities->Create_Player_Entity();

		App->CL_Com_Environments->Create_Test_Environment();
		App->CL_Editor_Scene->Show_Entities(false);
	}

	App->CL_Ogre->RenderFrame(7);

}

// *************************************************************************
// *			Set_Player:- Terry and Hazel Flanigan 2025 				   *
// *************************************************************************
void CL64_File::Set_Player()
{
	// TODO Needed at the Moment to test for Player Brush and Set Ogre Player

	bool test = App->CL_Brush_X->Check_if_Brush_Name_Exist((LPSTR)"Player");
	if (test == 0)
	{
		App->CL_Entities->Create_Player_Entity();
	}

	Brush* Player = App->CL_Brush_X->Get_Brush_By_Name("Player");
	if (Player)
	{
		App->CL_Brush->Brush_SetLocked(Player, true);
	}

	T_Vec3 BrushPos;
	App->CL_Brush->Brush_Center(Player, &BrushPos);

	App->CL_Editor_Com->B_Player[0]->StartPos.x = BrushPos.x;
	App->CL_Editor_Com->B_Player[0]->StartPos.y = BrushPos.y;
	App->CL_Editor_Com->B_Player[0]->StartPos.z = BrushPos.z;

	App->CL_Physics->Reset_Physics();

}

