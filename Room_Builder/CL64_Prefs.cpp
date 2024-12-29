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
#include "CL64_Prefs.h"

CL64_Prefs::CL64_Prefs(void)
{
	Grid_Fine_Spacing = 8;
	Grid_Spacing = 128;

	WriteData = nullptr;
}

CL64_Prefs::~CL64_Prefs(void)
{
}

// *************************************************************************
// *			Read_Preferences:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Prefs::Read_Preferences()
{
	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->RB_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Room_Builder.ini");

	App->CL_Ini_File->SetPathName(Preferences_Path);


	Grid_Fine_Spacing = App->CL_Ini_File->GetInt("Grid", "Grid_Fine_Spacing", 0, 10);
	Grid_Spacing = App->CL_Ini_File->GetInt("Grid", "Grid_Spacing", 0, 10);

	App->CL_MapEditor->GridSize = Grid_Spacing;
	App->CL_MapEditor->GridSnapSize = Grid_Fine_Spacing;

}

// *************************************************************************
// *		Write_Preferences:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_Prefs::Write_Preferences()
{
	WriteData = nullptr;

	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->RB_Directory_FullPath);
	strcat(Preferences_Path, "\\Data\\Room_Builder.ini");

	WriteData = fopen(Preferences_Path, "wt");
	if (!WriteData)
	{
		App->Say_Win("Cant Open Preference File");
		return 0;
	}

	fprintf(WriteData, "%s\n", "[Grid]");
	fprintf(WriteData, "%s%i\n", "Grid_Fine_Spacing=", Grid_Fine_Spacing);
	fprintf(WriteData, "%s%i\n", "Grid_Spacing=", Grid_Spacing);

	fclose(WriteData);

	return 1;
}
