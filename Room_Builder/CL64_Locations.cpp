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
	Location_Count = 0;
	UniqueID_Location_Counter = 0;

	B_Location.reserve(20);
}

CL64_Locations::~CL64_Locations(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Locations::Reset_Class()
{
	int Count = 0;
	while (Count < Location_Count)
	{
		delete B_Location[Count];
		B_Location[Count] = nullptr;
		Count++;
	}

	B_Location.resize(0);

	Location_Count = 0;
	UniqueID_Location_Counter = 0;
}

// *************************************************************************
// *			Add_New_Location:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Locations::Add_New_Location(bool isFirstLocation)
{
	const int index = Location_Count;
	B_Location[index] = nullptr;

	// Create a new Location
	B_Location[index] = new Base_Location();

	if (B_Location[index])
	{
		Set_Location_Defaults(index);

		// Set Main Location
		if (isFirstLocation == true)
		{
			strcpy(B_Location[index]->Location_Name, "Start_Location");
			B_Location[index]->Physics_Pos = App->CL_Scene->B_Player[0]->StartPos;
			B_Location[index]->Physics_Quat = App->CL_Scene->B_Player[0]->Physics_Quat;
			B_Location[index]->Location_UniqueID = UniqueID_Location_Counter;
		}
		else
		{
			float x = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin().getX();
			float y = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin().getY();
			float z = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin().getZ();

			B_Location[index]->Physics_Pos = Ogre::Vector3(x,y,z);
			B_Location[index]->Physics_Quat = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getRotation();
			B_Location[index]->Location_UniqueID = UniqueID_Location_Counter;
		}

		// Add item to the file view
		HTREEITEM tempItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Locations_Folder, B_Location[index]->Location_Name, index, true);
		B_Location[index]->FileViewItem = tempItem;

		// Mark the object as altered
		B_Location[index]->flag_Altered = 1;

		// Set Locations Folder Active
		App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Locations_Folder);

		// Increment counters
		UniqueID_Location_Counter++;
		Location_Count++;
	}
}

// *************************************************************************
// *		Set_Location_Defaults:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Locations::Set_Location_Defaults(int index)
{
	Base_Location* m_Location = B_Location[index];

	// Generate a unique Location name
	std::string objectName = "Loc_" + std::to_string(index);
	strcpy(m_Location->Location_Name, objectName.c_str());

	m_Location->flag_Altered = true;
}

// *************************************************************************
// *			Rename_Object:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Locations::Rename_Object(int Index)
{
	Base_Location* m_Location = B_Location[Index];

	strcpy(App->CL_Dialogs->btext, "Change Object Name");
	strcpy(App->CL_Dialogs->Chr_Text, m_Location->Location_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Locations);

	if (App->CL_Dialogs->flag_Dlg_Canceled == 1)
	{
		return;
	}

	strcpy(m_Location->Location_Name, App->CL_Dialogs->Chr_Text);
	m_Location->flag_Altered = 1;

	App->CL_Level->flag_Level_is_Modified = 1;
	App->CL_FileView->Mark_Altered(m_Location->FileViewItem);

	App->CL_FileView->Change_Item_Name(m_Location->FileViewItem, m_Location->Location_Name);
}

// *************************************************************************
// *		 Check_Location_Names:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Locations::Check_Location_Names(const char* name)
{
	for (int count = 0; count < Location_Count; ++count)
	{
		// Check if the object is not deleted
		if (B_Location[count]->flag_Deleted == false)
		{
			// Compare the object name with the provided name
			if (strcmp(B_Location[count]->Location_Name, name) == 0)
			{
				return true; // Name found
			}
		}
	}

	return false; // Name not found
}
