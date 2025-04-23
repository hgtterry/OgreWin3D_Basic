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

	fp = NULL;
}

CL64_File::~CL64_File(void)
{
}

// *************************************************************************
// *	          Start_Save:- Terry and Hazel Flanigan 2025		       *
// *************************************************************************
void CL64_File::Start_Save(bool useSaveDialog)
{
	// Check there are brushes to Save
	int brushCount = App->CL_Brush->Get_Brush_Count();
	if (brushCount <= 0)
	{
		App->Say("No Brushes to Save");
		return;
	}

	// if Menu->Save As Use Dialog
	if (useSaveDialog)
	{
		LPCWSTR fileType = L"Mesh Text File(*.mtf) *.mtf";
		LPCWSTR fileExtension = L" *.mtf";

		App->CL_File_IO->Save_File(fileType, fileExtension);

		if (App->CL_File_IO->flag_Canceled)
		{
			return;
		}

		std::string& pathAndFile = App->CL_File_IO->s_Path_And_File;
		strcpy(App->CL_Level->MTF_PathAndFile, pathAndFile.c_str());

		if (_stricmp(App->CL_Level->MTF_PathAndFile + pathAndFile.length() - 4, ".mtf") != 0)
		{
			strcat(App->CL_Level->MTF_PathAndFile, ".mtf");
		}

		App->CL_Utilities->Get_FileName_FromPath(App->CL_Level->MTF_PathAndFile, App->CL_Level->MTF_PathAndFile);
		strcpy(App->CL_Level->MTF_Just_FileName, App->CL_Utilities->JustFileName);

		char buf[MAX_PATH];
		strcpy(buf, App->CL_Level->MTF_Just_FileName);
		buf[strlen(buf) - 4] = '\0';
		strcpy(App->CL_Level->MTF_JustName_NoExt, buf);
		strcpy(App->CL_Export->mJustName, App->CL_Level->MTF_JustName_NoExt);
	}

	// If from Menu->Save Check if file exsits and ask for conformatin to overwrite
	if (!useSaveDialog)
	{
		bool test = App->CL_Utilities->Check_File_Exist(App->CL_Level->MTF_PathAndFile);
		if (test == 1)
		{
			App->CL_Dialogs->YesNo("File Exsits", "Do you want to update File");
			bool Doit = App->CL_Dialogs->flag_Dlg_Canceled;
			if (Doit == 1)
			{
				return;
			}
		}
	}

	Save_Document();

	char ProjectFolder[MAX_PATH];
	strcpy(ProjectFolder, App->CL_Level->MTF_PathAndFile);

	int Len1 = strlen(App->CL_Level->MTF_PathAndFile);
	int Len2 = strlen(App->CL_Level->MTF_Just_FileName);
	ProjectFolder[Len1 - Len2] = 0;

	// Create Working Folder
	strcat(ProjectFolder, App->CL_Level->MTF_JustName_NoExt);
	strcat(ProjectFolder, "_ow3d_prj");
	CreateDirectory(ProjectFolder, NULL);

	App->Set_Title(App->CL_Level->MTF_PathAndFile);
	App->Say("Saved", App->CL_Level->MTF_Just_FileName);
}

// *************************************************************************
// *	          Save_Document:- Terry and Hazel Flanigan 2025	           *
// *************************************************************************
void CL64_File::Save_Document()
{
	if (Save(App->CL_Level->MTF_PathAndFile) == false)
	{
		App->Say("Error: Unable to save file");
		return;
	}

	App->CL_Doc->flag_IsNewDocument = false;
	App->CL_Doc->flag_Is_Modified = false;
}

// *************************************************************************
// *				Save:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_File::Save(const char* FileName)
{
	// Construct the TXL file name
	std::string TXL_File_Name = std::string(App->CL_Level->MTF_JustName_NoExt) + ".zip";

	// Open the file for writing
	FILE* Write_File = fopen(FileName, "wt");
	if (!Write_File) 
	{
		App->Say("Error: Failed to open file for writing");
		return false;
	}

	// Write version and texture library name to the file
	fprintf(Write_File, "MTF_Version %.2f\n", Level_Version);
	fprintf(Write_File, "TextureLib %s\n", TXL_File_Name.c_str());

	// Write brush list to the file
	BrushList_Write(App->CL_Doc->Current_Level->Brushes, Write_File);

	// Close the file
	fclose(Write_File);

	// Prepare source and destination paths for file copying Textures Zip
	std::string Source = App->CL_Level->Wad_PathAndFile;
	std::string Destination = std::string(FileName);
	Destination.replace(Destination.end() - 4, Destination.end(), ".zip");

	// Cant copy to its self so test
	if (Destination != Source)
	{
		if (!CopyFile(Source.c_str(), Destination.c_str(), false))
		{
			App->Say("Error: Failed to copy file");
			return false;
		}
	}
	
	// Update the level's file paths
	strcpy(App->CL_Level->Wad_PathAndFile,Destination.c_str());
	strcpy(App->CL_Level->Wad_Just_File_Name,TXL_File_Name.c_str());

	return true;
}

// *************************************************************************
// *			BrushList_Write:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
signed int CL64_File::BrushList_Write(BrushList* BList, FILE* ofile)
{
	if (!BList || !ofile) return false; // Check for null pointers

	Brush* pBrush;
	BrushIterator bi;
	int Count;

	Count = App->CL_Brush->BrushList_Count(BList, (BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF | BRUSH_COUNT_NORECURSE));
	if (fprintf(ofile, "Brushlist %d\n", Count) < 0) return false;

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		if (!Brush_Write(pBrush, ofile)) return false;
		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}

	return true;
}

// *************************************************************************
// *			BrushList_Write:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
signed int CL64_File::Brush_Write(const Brush* b, FILE* ofile)
{
	if (!b || !ofile) return false; // Check for null pointers

	if (b->Type == BRUSH_CSG)
	{
		return true;
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
	return true;
}

// *************************************************************************
// *			FaceList_Write:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
signed int CL64_File::FaceList_Write(const FaceList* pList, FILE* f)
{
	if (!pList || !f) return false; // Check for null pointers

	int i;

	assert(pList != NULL);
	assert(f != NULL);

	if (fprintf(f, "\tBrushFaces %d\n", pList->NumFaces) < 0) return false;
	for (i = 0; i < pList->NumFaces; i++)
	{
		if (!Face_Write(pList->Faces[i], f)) return false;
	}

	return true;
}

// *************************************************************************
// *				Face_Write:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
signed int CL64_File::Face_Write(const Face* f, FILE* wf)
{
	if (!f || !wf) return false; // Check for null pointers

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

		strcpy(App->CL_Level->MTF_PathAndFile, App->CL_File_IO->s_Path_And_File.c_str());
		strcpy(App->CL_Level->MTF_Just_FileName, App->CL_File_IO->s_Just_FileName.c_str());

		char buf[MAX_PATH];
		strcpy(buf, App->CL_Level->MTF_Just_FileName);
		int Len = strlen(buf);
		buf[Len - 4] = 0;
		strcpy(App->CL_Level->MTF_JustName_NoExt, buf);

		strcpy(App->CL_Export->mJustName, App->CL_Level->MTF_JustName_NoExt);

	}
	
	App->CL_Editor_Com->Clear_Level(true);

	bool Test = Open_3dt_File();
	if (Test == true)
	{
		App->CL_Doc->Do_General_Select_Dlg(false);

		Set_Editor();

		// Check if Working Folder Exsits
		char Work_Folder[MAX_PATH];
		strcpy(Work_Folder, App->CL_Level->MTF_Just_Path);
		strcat(Work_Folder, App->CL_Level->MTF_JustName_NoExt);
		strcat(Work_Folder, "_ow3d_prj");
		bool Folder_Test = App->CL_Utilities->Check_Directory_Exists(Work_Folder);
		App->CL_Level->flag_Working_Folder_Exists = Folder_Test;

		App->Say("File Loaded", App->CL_File->FileName_3dt);
		
		App->CL_Doc->Do_General_Select_Dlg(true);

	}
	else
	{
		App->Say("Can Not Find File", FileName_3dt);
	}
}

// *************************************************************************
// *							Open_3dt_File 							   *
// *************************************************************************
bool CL64_File::Open_3dt_File()
{
	App->CL_Doc->ResetAllSelections();
	App->CL_Doc->Do_General_Select_Dlg(false);

	if (!Load_File(PathFileName_3dt))
	{
		return false;
	}

	App->CL_Doc->Set_Paths();

	char pathAndFile[MAX_PATH];
	strcpy(pathAndFile, App->CL_Level->MTF_Just_Path);
	strcat(pathAndFile, App->CL_Level->Wad_Just_File_Name);
	
	if (!App->CL_Utilities->Check_File_Exist(pathAndFile))
	{
		App->Say("Texture Library Does Not Exist", "Loading Default");
		strcpy(pathAndFile, App->RB_Directory_FullPath);
		strcat(pathAndFile, "\\Data\\Room_Builder\\Default.zip");
	}

	App->CL_Doc->Load_Wad_File(pathAndFile);
	App->CL_Doc->UpdateAfterWadChange();
	Set_Player();
	App->CL_Properties_Brushes->Fill_ListBox();
	App->CL_Properties_Textures->Fill_ListBox();
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
	App->CL_Doc->Set_Faces_To_Brush_Name_All(); // TODO: Fix up Brush Names and set Indexes
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);

	return true;
}

// *************************************************************************
// *								Load_File 							   *
// *************************************************************************
bool CL64_File::Load_File(const char* FileName)
{
	Level* m_pLevel = nullptr;

	int Count = 0;

	fp = fopen(FileName, "r");
	if (!fp)
	{
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
		
		m_pLevel = App->CL_Level->Level_Create();

		fgets(Read_Buffer, sizeof(Read_Buffer), fp);
		if (App->CL_ParseFile->Get_TextureLib(Read_Buffer) == 0)
		{
			break;
		}

		if (App->CL_Doc->Current_Level->Brushes != NULL)
		{
			App->CL_Brush->BrushList_Destroy(&App->CL_Doc->Current_Level->Brushes);
			App->CL_Doc->Current_Level->Brushes = NULL;
		}
		
		if (m_pLevel == NULL)
		{
			App->Say("Can not Create Level");
			break;
		}
		
		m_pLevel->Brushes = App->CL_ParseFile->BrushList_CreateFromFile(false);
		if (m_pLevel->Brushes == NULL)
		{
			App->Say("Can not Create Brushes");
			break;
		}
		
		break;

		Count++;
		memset(Read_Buffer, 0, MAX_PATH);
	}

	fclose(fp);
	
	App->CL_Doc->Current_Level = m_pLevel;

	return true;
}

// *************************************************************************
// *			Set_Editor:- Terry and Hazel Flanigan 2025 				   *
// *************************************************************************
void CL64_File::Set_Editor()
{
	App->Set_Title(App->CL_Level->MTF_PathAndFile);
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
		//App->CL_Entities->Create_Player_Entity();

		App->CL_Com_Environments->Create_Test_Environment();
		App->CL_Editor_Scene->Show_Entities(false);
	}
	
	App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Ogre;
	
	App->CL_Camera->Camera_Textured();
	
	App->CL_Ogre->RenderFrame(7);

}

// *************************************************************************
// *			Set_Player:- Terry and Hazel Flanigan 2025 				   *
// *************************************************************************
void CL64_File::Set_Player()
{
	// TODO Needed at the Moment to test for Player Brush and Set Ogre Player

	bool test = App->CL_Brush_X->Check_if_Brush_Name_Exist((LPSTR)"Player_Main");
	if (test == 0)
	{
		App->CL_Entities->Create_Player_Entity();
	}

	Brush* Player = App->CL_Brush_X->Get_Brush_By_Name("Player_Main");
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

