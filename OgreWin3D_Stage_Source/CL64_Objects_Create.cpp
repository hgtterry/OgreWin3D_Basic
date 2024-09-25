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
#include "CL64_Objects_Create.h"

CL64_Objects_Create::CL64_Objects_Create(void)
{
}

CL64_Objects_Create::~CL64_Objects_Create(void)
{
}

// *************************************************************************
//			Add_Objects_From_File:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Objects_Create::Add_Objects_From_File() // From File
{

	int Object_Count = App->CL_Scene->Object_Count;
	int Count = 0;

	while (Count < Object_Count)
	{
		if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Sound)
		{
			/*App->CL_Com_Sounds->Create_Sound_Entity(Count);

			HTREEITEM Temp = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Sounds_Folder, App->CL_Scene->V_Object[Count]->Mesh_Name, Count, false);
			App->CL_Scene->V_Object[Count]->FileViewItem = Temp;

			App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Sounds_Folder)*/;
		}
		else if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Message)
		{
			/*App->CL_Com_Messages->Create_Message_Entity(Count);
			App->CL_Scene->V_Object[Count]->Set_ImGui_Panel_Name();

			HTREEITEM Temp = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Message_Trigger_Folder, App->CL_Scene->V_Object[Count]->Mesh_Name, Count, false);
			App->CL_Scene->V_Object[Count]->FileViewItem = Temp;

			App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Message_Trigger_Folder);*/
		}
		else if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Move)
		{
			/*App->SBC_Com_MoveEntity->Create_Move_Entity(Count);

			HTREEITEM Temp = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Move_Folder, App->CL_Scene->V_Object[Count]->Mesh_Name, Count, false);
			App->CL_Scene->V_Object[Count]->FileViewItem = Temp;

			App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Move_Folder);*/

		}
		else if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Teleport)
		{
			/*App->CL_Com_Teleporters->Create_Teleport_Entity(Count);

			HTREEITEM Temp = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Teleporters_Folder, App->CL_Scene->V_Object[Count]->Mesh_Name, Count, false);
			App->CL_Scene->V_Object[Count]->FileViewItem = Temp;

			App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Teleporters_Folder);*/

		}
		else if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Colectable)
		{

			/*App->CL_Com_Collectables->Create_Collectable_Entity(Count);

			HTREEITEM Temp = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Collectables_Folder, App->CL_Scene->V_Object[Count]->Mesh_Name, Count, false);
			App->CL_Scene->V_Object[Count]->FileViewItem = Temp;

			App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Collectables_Folder);
			App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_EntitiesFolder);*/

		}
		else if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_EnvironEntity)
		{
			//App->SBC_Com_Environments->Create_Environ_Entity(Count);

			HTREEITEM Temp = App->CL_FileView->Add_Item(App->CL_FileView->FV_Evirons_Folder, App->CL_Scene->V_Object[Count]->Mesh_Name, Count, false);
			App->CL_Scene->V_Object[Count]->FileViewItem = Temp;

			App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Evirons_Folder);

		} // -------------------------------------------------------------------------- Particles
		else if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Particle)
		{
			/*App->CL_Com_Particles->Create_Particle_Entity(Count);

			HTREEITEM Temp = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Particles_Folder, App->CL_Scene->V_Object[Count]->Mesh_Name, Count, false);
			App->CL_Scene->V_Object[Count]->FileViewItem = Temp;

			App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Particles_Folder);*/

		}
		else
		{
			//App->CL_Objects_Create->Add_New_Object(Count, 0);
			App->CL_Scene->V_Object[Count]->Altered = 0;
			App->CL_Scene->V_Object[Count]->Folder = Enums::Folder_Objects;
			App->CL_Scene->V_Object[Count]->FileViewItem = App->CL_FileView->Add_Item(App->CL_FileView->FV_Objects_Folder, App->CL_Scene->V_Object[Count]->Mesh_Name, Count, false);
		}

		Count++;
	}

	if (Object_Count > 0)
	{
		App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Objects_Folder);
		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_FileView->SelectItem(App->CL_Scene->V_Object[0]->FileViewItem);
	}

	return 1;
}
