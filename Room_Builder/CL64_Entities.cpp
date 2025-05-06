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
	int Total = App->CL_Editor_Com->Object_Count;

	while (Count < Total)
	{
		if (App->CL_Editor_Com->B_Object[Count]->flag_Deleted == 0)
		{
			int Result = 1;
			Result = strcmp(App->CL_Editor_Com->B_Object[Count]->Object_Name, Name);
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
void CL64_Entities::Create_Player_Brush()
{
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

	Player_Brush = App->CL_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);

	App->CL_Brush->Brush_Bound(Player_Brush);

	App->CL_Doc->SetDefaultBrushTexInfo(Player_Brush);

	App->CL_Brush->Brush_Bound(Player_Brush);

	Brush_SetGroupId(Player_Brush, Enums::Brushs_ID_Players);

	fdocFaceScales Scales;

	Scales.DrawScale = App->CL_Level->Level_GetDrawScale(App->CL_Doc->Current_Level);
	Scales.LightmapScale = App->CL_Level->Level_GetLightmapScale(App->CL_Doc->Current_Level);
	App->CL_Brush->Brush_EnumFaces(Player_Brush, &Scales, fdocSetFaceScales);

	strcpy(Player_Brush->Name, "Player_Main");

	App->CL_Level->Level_AppendBrush(Player_Brush);

	App->CL_Doc->CurBrush = Player_Brush;
	App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);

	App->CL_Brush->Brush_SetLocked(Player_Brush, true);
}

// *************************************************************************
// *		Create_Environment_Brush:- Terry and Hazel Flanigan 2025 	   *
// *************************************************************************
void CL64_Entities::Create_Environment_Brush(int Object_Index)
{
	char m_Name[MAX_PATH];
	strcpy(m_Name, App->CL_Editor_Com->B_Object[Object_Index]->Object_Name);

	bool test = App->CL_Brush_X->Check_if_Brush_Name_Exist((LPSTR)m_Name);
	if (test == true)
	{
		return;
	}

	Brush* Environ_Brush = NULL;

	BrushTemplate_Box* pBoxTemplate;

	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();

	Ogre::Vector3 size = App->CL_Com_Objects->GetMesh_BB_Size(App->CL_Editor_Com->B_Object[Object_Index]->Object_Node);


	pBoxTemplate->Solid = 0;
	pBoxTemplate->YSize = size.y;

	pBoxTemplate->XSizeBot = size.x;
	pBoxTemplate->XSizeTop = size.x;
	pBoxTemplate->ZSizeBot = size.z;
	pBoxTemplate->ZSizeTop = size.z;

	Environ_Brush = App->CL_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);

	App->CL_Brush->Brush_Bound(Environ_Brush);

	App->CL_Doc->SetDefaultBrushTexInfo(Environ_Brush);

	App->CL_Brush->Brush_Bound(Environ_Brush);

	Brush_SetGroupId(Environ_Brush, Enums::Brushs_ID_Evirons);

	fdocFaceScales Scales;

	Scales.DrawScale = App->CL_Level->Level_GetDrawScale(App->CL_Doc->Current_Level);
	Scales.LightmapScale = App->CL_Level->Level_GetLightmapScale(App->CL_Doc->Current_Level);
	App->CL_Brush->Brush_EnumFaces(Environ_Brush, &Scales, fdocSetFaceScales);

	strcpy(Environ_Brush->Name, App->CL_Editor_Com->B_Object[Object_Index]->Object_Name);

	T_Vec3 Pos;
	Pos.x = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Pos.x;
	Pos.y = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Pos.y;
	Pos.z = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Pos.z;

	App->CL_Brush->Brush_Move(Environ_Brush, &Pos);

	App->CL_Level->Level_AppendBrush(Environ_Brush);

	App->CL_Doc->CurBrush = Environ_Brush;
	App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);

	App->CL_Brush->Brush_SetLocked(Environ_Brush, true);
}
