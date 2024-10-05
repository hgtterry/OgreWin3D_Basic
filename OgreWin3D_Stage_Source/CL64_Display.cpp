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
#include "CL64_Display.h"

CL64_Display::CL64_Display(void)
{
}

CL64_Display::~CL64_Display(void)
{
}

// *************************************************************************
//			Add_Counters_From_File:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Display::Add_Counters_From_File() // From File
{

	int Counters_Count = App->CL_Scene->Counters_Count;
	int Count = 0;

	while (Count < Counters_Count)
	{

		HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Counters_Folder, App->CL_Scene->B_Counter[Count]->Panel_Name, Count, false);
		App->CL_Scene->B_Counter[Count]->FileViewItem = Temp;

		App->CL_Scene->B_Counter[Count]->Counter = App->CL_Scene->B_Counter[Count]->Start_Value;

		Count++;
	}

	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Counters_Folder);

	return 1;
}

// *************************************************************************
// *			Rename_Counter:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void CL64_Display::Rename_Counter(int Index)
{
	strcpy(App->CL_Dialogs->btext, "Change Counter Name");
	strcpy(App->CL_Dialogs->Chr_Text, App->CL_Scene->B_Counter[Index]->Panel_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CL_Dialogs->Canceled == 1)
	{
		return;
	}

	strcpy(App->CL_Scene->B_Counter[Index]->Panel_Name, App->CL_Dialogs->Chr_Text);

	Mark_As_Altered_Counter(Index);

	App->CL_FileView->Change_Item_Name(App->CL_Scene->B_Counter[Index]->FileViewItem, App->CL_Dialogs->Chr_Text);
}

// *************************************************************************
// *	Mark_As_Altered_Counter:- Terry and Hazel Flanigan 2022		 	   *
// *************************************************************************
void CL64_Display::Mark_As_Altered_Counter(int Index)
{
	App->CL_Scene->B_Counter[Index]->Altered = 1;

	App->CL_Scene->flag_Scene_Modified = 1;

	App->CL_FileView->Mark_Altered(App->CL_Scene->B_Counter[Index]->FileViewItem);
}
