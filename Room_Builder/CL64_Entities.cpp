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

// *************************************************************************
// *		Create_Player_Entity:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Entities::Create_Player_Entity()
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

	Brush_SetGroupId(Player_Brush, 1); // 0 = Main Mesh 1 = Entities

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
