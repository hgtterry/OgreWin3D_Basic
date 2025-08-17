/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
#include "CL64_Locations.h"

CL64_Locations::CL64_Locations(void)
{
}

CL64_Locations::~CL64_Locations(void)
{
}

// *************************************************************************
// *		Add_New_Environ_Entiry:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Locations::Add_New_Location(bool isFirstLocation)
{
	const int index = App->CL_Scene->Location_Count;
	App->CL_Scene->B_Location[index] = nullptr;

	// Create a new Base_Object and assign it to the scene
	App->CL_Scene->B_Location[index] = new Base_Location();

	if (App->CL_Scene->B_Location[index])
	{
		Set_Location_Defaults(index);

		// Add item to the file view
		HTREEITEM tempItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Locations_Folder, App->CL_Scene->B_Location[index]->Location_Name, index, true);
		App->CL_Scene->B_Location[index]->FileViewItem = tempItem;

		// Mark the object as altered
		App->CL_Scene->B_Location[index]->flag_Altered = 1;

		// Update the file view
		App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Locations_Folder);

		// Increment counters
		App->CL_Scene->UniqueID_Location_Counter++;
		App->CL_Scene->Location_Count++;
	}
}

// *************************************************************************
// *		Add_New_Environ_Entiry:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Locations::Set_Location_Defaults(int index)
{
	Base_Location* bObject = App->CL_Scene->B_Location[index];

	// Generate a unique object name
	std::string objectName = "Loc_" + std::to_string(index);
	strcpy(bObject->Location_Name, objectName.c_str());

	bObject->flag_Altered = true;
}
