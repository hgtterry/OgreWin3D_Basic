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
#include "CL64_Entities.h"
#include "Room Builder.h"
#include "Structures.cpp"

CL64_Entities::CL64_Entities()
{
	memset(Verts, 0, sizeof(Verts));
}

CL64_Entities::~CL64_Entities()
{
}
// *************************************************************************
// *						Brush_SetGroupId							   *
// *************************************************************************
//void Brush_SetGroupId(Brush* b, const int gid)
//{
//	if (b->Type == BRUSH_MULTI)
//	{
//		//BrushList_SetInt(b->BList, gid, Brush_SetGroupId);
//	}
//
//	b->GroupId = gid;
//}

// **************************************************************************
// *	  		GetIndex_By_Name:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
int CL64_Entities::GetIndex_By_Name(char* Name)
{
	int Count = 0;
	int Total = App->CL_Scene->Object_Count;

	while (Count < Total)
	{
		if (App->CL_Scene->B_Object[Count]->flag_Deleted == 0)
		{
			int Result = 1;
			Result = strcmp(App->CL_Scene->B_Object[Count]->Object_Name, Name);
			if (Result == 0)
			{
				return Count;
			}
		}

		Count++;
	}

	return -1;
}

// *************************************************************************
// *		Create_Player_Entity:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Entities::Create_Player_Brush(const char* name)
{
	// Need to check for duplicate names
	/*bool test = App->CL_Brush_X->Check_if_Brush_Name_Exist((LPSTR)"Player_Main");
	if (test == 0)
	{
		return;
	}*/

	Brush* Player_Brush;

	BrushTemplate_Box* pBoxTemplate;

	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();


	pBoxTemplate->Solid = 0;
	pBoxTemplate->YSize = 25;

	pBoxTemplate->XSizeBot = 8;
	pBoxTemplate->XSizeTop = 8;
	pBoxTemplate->ZSizeBot = 8;
	pBoxTemplate->ZSizeTop = 8;

	Player_Brush = App->CL_X_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);

	App->CL_Brush->Brush_Bound(Player_Brush);

	App->CL_Doc->SetDefaultBrushTexInfo(Player_Brush);

	App->CL_Brush->Brush_Bound(Player_Brush);

	Brush_SetGroupId(Player_Brush, Enums::Brushs_ID_Players);

	fdocFaceScales Scales;

	Scales.DrawScale = App->CL_Level->Level_GetDrawScale(App->CL_Doc->Current_Level);
	Scales.LightmapScale = App->CL_Level->Level_GetLightmapScale(App->CL_Doc->Current_Level);
	App->CL_Brush->Brush_EnumFaces(Player_Brush, &Scales, fdocSetFaceScales);

	strcpy(Player_Brush->Name, name);

	App->CL_Level->Level_AppendBrush(Player_Brush);

	App->CL_Doc->CurBrush = Player_Brush;
	App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);

	App->CL_Brush->Brush_SetLocked(Player_Brush, true);
}

// *************************************************************************
// *		Create_Entity_Brush:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Entities::Create_Entity_Brush(int Object_Index)
{
	char m_Name[MAX_PATH];
	strcpy(m_Name, App->CL_Scene->B_Object[Object_Index]->Object_Name);

	bool test = App->CL_Brush_X->Check_if_Brush_Name_Exist((LPSTR)m_Name);
	if (test == true)
	{
		// TODO Test
		App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_Name, true);

		Debug
		return;
	}

	Brush* New_Brush = NULL;

	BrushTemplate_Box* pBoxTemplate;

	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();

	Ogre::Vector3 size = App->CL_Com_Objects->GetMeshBoundingBoxSize(App->CL_Scene->B_Object[Object_Index]->Object_Node);


	pBoxTemplate->Solid = 0;
	pBoxTemplate->YSize = size.y;

	pBoxTemplate->XSizeBot = size.x;
	pBoxTemplate->XSizeTop = size.x;
	pBoxTemplate->ZSizeBot = size.z;
	pBoxTemplate->ZSizeTop = size.z;

	New_Brush = App->CL_X_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);

	App->CL_Brush->Brush_Bound(New_Brush);

	App->CL_Doc->SetDefaultBrushTexInfo(New_Brush);

	App->CL_Brush->Brush_Bound(New_Brush);

	Brush_SetGroupId(New_Brush, Enums::Brushs_ID_Evirons);

	fdocFaceScales Scales;

	Scales.DrawScale = App->CL_Level->Level_GetDrawScale(App->CL_Doc->Current_Level);
	Scales.LightmapScale = App->CL_Level->Level_GetLightmapScale(App->CL_Doc->Current_Level);
	App->CL_Brush->Brush_EnumFaces(New_Brush, &Scales, fdocSetFaceScales);

	strcpy(New_Brush->Name, App->CL_Scene->B_Object[Object_Index]->Object_Name);

	T_Vec3 Pos{ 0, 0, 0 };
	Pos.x = App->CL_Scene->B_Object[Object_Index]->Mesh_Pos.x;
	Pos.y = App->CL_Scene->B_Object[Object_Index]->Mesh_Pos.y;
	Pos.z = App->CL_Scene->B_Object[Object_Index]->Mesh_Pos.z;

	App->CL_Brush->Brush_Move(New_Brush, &Pos);

	App->CL_Level->Level_AppendBrush(New_Brush);

	App->CL_Doc->CurBrush = New_Brush;
	App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);

	App->CL_Brush->Brush_SetLocked(New_Brush, true);

	App->CL_Properties_Brushes->Fill_ListBox();
}

// *************************************************************************
// *			Ogre_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Entities::Ogre_To_Mesh_Data(Ogre::SceneNode* Ogre_Node)
{
	Get_Mesh_Data(Ogre_Node);
	return 1;
}

// *************************************************************************
// *			Get_Mesh_Data:- Terry and Hazel Flanigan 2024	   		   *
// *************************************************************************
void CL64_Entities::Get_Mesh_Data(Ogre::SceneNode* Ogre_Node)
{
	// Retrieve the bounding box size of the mesh
	//Ogre::Vector3 size = App->CL_Com_Objects->GetMeshBoundingBoxSize(Ogre_Node);
	//Ogre::Vector3 newSize = size / 2; // Halve the size for new dimensions

	// Retrieve the bounding box size of the mesh
	Ogre::Vector3 size = App->CL_Com_Objects->GetMeshBoundingBoxSize(Ogre_Node);
	Ogre::Vector3 newSize = size / 2; // Halve the size for new dimensions

	// Calculate the center of the bounding box and convert to world space
	Ogre::Vector3 center = Ogre_Node->getAttachedObject(0)->getBoundingBox().getCenter();
	
	
	
	Ogre::Vector3 position = Ogre_Node->convertLocalToWorldPosition(center);

	const Quaternion& orientation = Ogre_Node->getOrientation();
	const Vector3& scale = { 1, 1, 1 };

	// Initialize vertices array
	Ogre::Vector3 vertices[8]; // Only need 8 vertices for a box

	// Define the 8 corners of the bounding box
	vertices[0] = { -newSize.x, -newSize.y, newSize.z };
	vertices[1] = { newSize.x, -newSize.y, newSize.z };
	vertices[2] = { newSize.x, newSize.y, newSize.z };
	vertices[3] = { -newSize.x, newSize.y, newSize.z };
	vertices[4] = { -newSize.x, -newSize.y, -newSize.z };
	vertices[5] = { newSize.x, -newSize.y, -newSize.z };
	vertices[6] = { newSize.x, newSize.y, -newSize.z };
	vertices[7] = { -newSize.x, newSize.y, -newSize.z };

	// Transform vertices based on orientation and position
	for (int i = 0; i < 8; ++i) 
	{
		vertices[i] = (orientation * (vertices[i] * scale)) + position;
	}

	// Assign transformed vertices to Verts array
	for (int i = 0; i < 8; ++i) 
	{
		Verts[i] = { vertices[i].x, vertices[i].y, vertices[i].z };
	}
}

// *************************************************************************
// *			Rename_Brush():- Terry and Hazel Flanigan 2024	   		   *
// *************************************************************************
void CL64_Entities::Rename_Brush()
{
	int numSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

	if (numSelBrushes > 0)
	{ 
		char name[MAX_PATH];

		strcpy(App->CL_Dialogs->btext, "Change Brush Name");
		strcpy(App->CL_Dialogs->Chr_Text, App->CL_Properties_Brushes->Selected_Brush->Name);

		App->CL_Dialogs->Dialog_Text(Enums::Check_Name_Brushes);

		if (App->CL_Dialogs->flag_Dlg_Canceled == 0)
		{
			strcpy(name, App->CL_Dialogs->Chr_Text);

			// Entity
			if (App->CL_Properties_Brushes->Selected_Brush->GroupId > Enums::Brushs_ID_Players)
			{
				int index = App->CL_Entities->GetIndex_By_Name(App->CL_Properties_Brushes->Selected_Brush->Name);
				strcpy(App->CL_Scene->B_Object[index]->Object_Name, name);
				App->CL_FileView->Change_Item_Name(App->CL_Scene->B_Object[index]->FileViewItem, name);
			}

			// Brush
			App->CL_Brush->Brush_SetName(App->CL_Properties_Brushes->Selected_Brush, name);
			App->CL_Properties_Brushes->Fill_ListBox();
			App->CL_Doc->Set_Faces_To_Brush_Name_Selected();
			App->CL_Level->flag_Level_is_Modified = true;

			SendDlgItemMessage(App->CL_Properties_Brushes->BrushesDlg_Hwnd, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)App->CL_Properties_Brushes->Selected_Index, (LPARAM)0);
		}
	}
	else
	{
		App->Say("No Brush Selected");
	}
}

// *************************************************************************
// *			Rename_Object:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Entities::Rename_Object(int Index)
{
	Base_Object* Object = App->CL_Scene->B_Object[Index];

	strcpy(App->CL_Dialogs->btext, "Change Object Name");
	strcpy(App->CL_Dialogs->Chr_Text, Object->Object_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CL_Dialogs->flag_Dlg_Canceled == 1)
	{
		return;
	}

	Brush* b = App->CL_Brush_X->Get_Brush_By_Name(Object->Object_Name);

	// Entity
	strcpy(Object->Object_Name, App->CL_Dialogs->Chr_Text);

	// Brush
	strcpy(b->Name, App->CL_Dialogs->Chr_Text);
	App->CL_Properties_Brushes->Fill_ListBox();
	App->CL_Doc->Set_Faces_To_Brush_Name_Selected();

	Object->flag_Altered = 1;

	App->CL_Level->flag_Level_is_Modified = 1;
	App->CL_FileView->Mark_Altered(Object->FileViewItem);

	App->CL_FileView->Change_Item_Name(Object->FileViewItem, Object->Object_Name);
}

// **************************************************************************
// *	  		Delete_Object:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_Entities::Delete_Brush()
{
	Delete_Object();
}

// **************************************************************************
// *	  		Delete_Object:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_Entities::Delete_Object()
{
	App->CL_Doc->DeleteCurrentThing();

	if (App->CL_Properties_Brushes->Selected_Brush->GroupId > Enums::Brushs_ID_Players)
	{
		int MeshIndex = App->CL_Properties_Scene->Current_Selected_Object;
		btRigidBody* body = App->CL_Scene->B_Object[MeshIndex]->Phys_Body;

		if (body)
		{
			App->CL_Physics->dynamicsWorld->removeCollisionObject(body);
		}

		App->CL_FileView->DeleteItem();

		App->CL_Scene->B_Object[MeshIndex]->flag_Deleted = 1;
		App->CL_Scene->B_Object[MeshIndex]->Object_Node->setVisible(false);
	}
	else
	{
		int selectedIndex = App->CL_Properties_Brushes->Selected_Index - 1;
		if (selectedIndex < 0)
		{
			selectedIndex = 0;
		}

		App->CL_Properties_Brushes->Select_From_List(selectedIndex);
	}

	App->CL_Level->flag_Level_is_Modified = 1;
}



