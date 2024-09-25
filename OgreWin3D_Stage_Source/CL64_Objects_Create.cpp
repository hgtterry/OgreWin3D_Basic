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
			App->CL_Objects_Create->Add_New_Object(Count, 0);
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

// *************************************************************************
//				Add_New_Object:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Objects_Create::Add_New_Object(int Index, bool From_MeshViewer)
{
	char Mesh_File[255];
	char ConNum[256];
	char Ogre_Name[256];

	Base_Object* Object = App->CL_Scene->V_Object[Index];


	strcpy_s(Ogre_Name, "GDEnt_");
	_itoa(Index, ConNum, 10);
	strcat(Ogre_Name, ConNum);

	strcpy(Mesh_File, Object->Mesh_FileName);

	Object->Object_Ent = App->CL_Ogre->mSceneMgr->createEntity(Ogre_Name, Mesh_File, App->CL_Resources->Project_Resource_Group);
	Object->Object_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Object->Object_Node->attachObject(Object->Object_Ent);

	Object->Object_Node->setVisible(true);

	Object->Object_Node->setOrientation(Object->Mesh_Quat);


	// Get Material Name
	Ogre::String text = Object->Object_Ent->getMesh()->getSubMesh(0)->getMaterialName().c_str();
	Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));
	strcpy(Object->Material_File, Mat->getOrigin().c_str());
	Object->UsageEX = 777;

	//// If from MeshViewer Get Placement Method
	//if (From_MeshViewer == 1 && App->SBC_MeshViewer->Placement_Camera == 1)
	//{
	//	Ogre::Vector3 Pos = App->CL_Object->GetPlacement();
	//	Object->Mesh_Pos = Pos;
	//	Object->Object_Node->setPosition(Pos);
	//}
	//else
	{
		Object->Object_Node->setPosition(Object->Mesh_Pos);
	}


	App->CL_Scene->Scene_Loaded = 1;


	//---------------------- Static
	if (Object->Type == Enums::Bullet_Type_Static)
	{
		/*if (Object->Shape == Enums::Shape_Box)
		{
			Add_Physics_Box(false, Index);
		}

		if (Object->Shape == Enums::Sphere)
		{
			Add_Physics_Sphere(false, Index);
		}

		if (Object->Shape == Enums::Capsule)
		{
			Add_Physics_Capsule(false, Index);
		}

		if (Object->Shape == Enums::Cylinder)
		{
			Add_Physics_Cylinder(false, Index);
		}

		if (Object->Shape == Enums::Cone)
		{
			Add_Physics_Cone(false, Index);
		}*/
	}

	//---------------------- Dynamic
	if (Object->Type == Enums::Bullet_Type_Dynamic)
	{
		/*if (Object->Shape == Enums::Shape_Box)
		{
			Add_Physics_Box(true, Index);
		}

		if (Object->Shape == Enums::Sphere)
		{
			Add_Physics_Sphere(true, Index);
		}

		if (Object->Shape == Enums::Capsule)
		{
			Add_Physics_Capsule(true, Index);
		}

		if (Object->Shape == Enums::Cylinder)
		{
			Add_Physics_Cylinder(true, Index);
		}

		if (Object->Shape == Enums::Cone)
		{
			Add_Physics_Cone(true, Index);
		}*/
	}

	//---------------------- Tri_Mesh
	if (Object->Type == Enums::Bullet_Type_TriMesh)
	{
		//create_New_Trimesh(Index);
	}


	//if (Object->Usage == Enums::Usage_Room) // Rooms
	{
		//App->SBC_Scene->Area_Added = 1;
	}

	ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);

	return 1;
}
