/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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
#include "CL64_Importers.h"

CL64_Importers::CL64_Importers()
{
	flag_Reload_Ogre_Model = 0;
	Ogre_CFG_Resource_Group = "Ogre_CFG_Resource_Group";

}

CL64_Importers::~CL64_Importers()
{
}

// *************************************************************************
// *			Load_Project:- Terry and Hazel Flanigan 2024   			   *
// *************************************************************************
bool CL64_Importers::Load_Project()
{
	
	bool test = App->CL_File_IO->Open_File();
	if (test == 1)
	{
		App->CL_Project->Project_File_Name[0] = 0;
		App->CL_Project->Project_Path_File_Name[0] = 0;

		strcpy(App->CL_Project->Project_File_Name, App->CL_File_IO->sSelectedFile.c_str());
		strcpy(App->CL_Project->Project_Path_File_Name, App->CL_File_IO->sFilePath.c_str());
	}
	else
	{
		return 1;
	}

	bool result = App->CL_Project->Load_Project();
	App->CL_Scene->Set_Scene();
	App->CL_Ogre->RenderFrame(8);

	//App->SBC_TopTabs->Project_Loaded_Reset();
	
	if (result == 1)
	{
		App->Say("Project Loaded", App->CL_Project->Project_File_Name);
	}
	else
	{
		App->Say("Project Failed to Load");
	}

	return 1;
}

