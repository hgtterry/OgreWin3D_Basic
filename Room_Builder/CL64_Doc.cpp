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
#include "CL64_Doc.h"

CL64_Doc::CL64_Doc(void)
{
    LastTemplateTypeName[0] = 0;
	pLevel = NULL;
}

CL64_Doc::~CL64_Doc(void)
{
}

// *************************************************************************
// *		    Init_Doc:- Terry and Hazel Flanigan 2024 	    		   *
// *************************************************************************
void CL64_Doc::Init_Doc()
{
	
    const char* DefaultWadName;
    strcpy(LastTemplateTypeName, "Box");

    DefaultWadName = App->CL_Prefs->Wad_File_Name;

    //strcpy(LastPath, Prefs_GetProjectDir(pPrefs));

    const char* WadPath = FindTextureLibrary(DefaultWadName);

	/*pLevel = Level_Create(WadPath, Prefs_GetHeadersList(pPrefs),
		Prefs_GetActorsList(pPrefs), Prefs_GetPawnIni(pPrefs));*/
   
	pLevel = App->CL_Level->Level_Create(WadPath, NULL,NULL, NULL);

	/*if (!Level_LoadWad(pLevel))
	{
		App->Say_Win("Can not load Wad File");
	}*/

	pSelBrushes = App->CL_SelBrushList->SelBrushList_Create();
	pTempSelBrushes = App->CL_SelBrushList->SelBrushList_Create();
	//App->CLSB_Doc->pSelFaces = SelFaceList_Create();

	//App->CLSB_Doc->SetLockAxis(0);	// Start with no axis locked


	// create our default box
	BrushTemplate_Box* pBoxTemplate;
	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate(pLevel);
	//BTemplate = BrushTemplate_CreateBox(pBoxTemplate);
	

	/*Brush_Bound(App->CLSB_Doc->BTemplate);
	App->CLSB_Doc->CurBrush = App->CLSB_Doc->BTemplate;

	geVec3d_Clear(&App->CLSB_Doc->SelectedGeoCenter);*/

}

// *************************************************************************
// *		   FindTextureLibrary- Terry and Hazel Flanigan 2024 	   	   *
// *************************************************************************
const char* CL64_Doc::FindTextureLibrary(char const* WadName)
{
	static char Path_And_File[MAX_PATH];
	strcpy(Path_And_File, App->RB_Directory_FullPath);
	strcat(Path_And_File, "\\Data\\Room_Builder\\");
	strcat(Path_And_File, WadName);

	bool test = App->CL_Utilities->Check_File_Exist(Path_And_File);
	if (test == 1)
	{
		return Path_And_File;
	}
	else
	{
		App->Say_Win("File Does not Exist");

		strcpy(Path_And_File, "");
		return Path_And_File;
	}
	
	strcpy(Path_And_File, "");
	return Path_And_File;
}

