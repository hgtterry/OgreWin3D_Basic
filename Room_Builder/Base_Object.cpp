/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "CL64_App.h"
#include "Base_Object.h"
#include "Structures.cpp"

Base_Object::Base_Object()
{
	Init_Object();
}

Base_Object::~Base_Object()
{
}

// *************************************************************************
// *	  					Init_Object Terry Flanigan					   *
// *************************************************************************
bool Base_Object::Init_Object(void)
{
	Object_Ent = nullptr;
	Object_Node = nullptr;

	Phys_Body = nullptr;
	Phys_Shape = nullptr;

	Object_Brush = nullptr;
	Brus_BoxTemplate = { 0 };


	strcpy(Object_Name, "None");
	strcpy(Mesh_FileName, "None");
	Mesh_Resource_Path[0] = 0;
	strcpy(Material_File, "Internal");

	// Actor Scale ------------------------------------------------------
	Mesh_Scale.x = 1;
	Mesh_Scale.y = 1;
	Mesh_Scale.z = 1;

	// Actor Pos
	Mesh_Pos.x = 0;
	Mesh_Pos.y = 0;
	Mesh_Pos.z = 0;

	// Mesh Rotation
	Mesh_Rot.x = 0;
	Mesh_Rot.y = 0;
	Mesh_Rot.z = 0;

	// Mesh Quat
	Mesh_Quat.w = 1;
	Mesh_Quat.x = 0;
	Mesh_Quat.y = 0;
	Mesh_Quat.z = 0;

	// Physics Scale ------------------------------------------------------
	Physics_Scale.x = 1;
	Physics_Scale.y = 1;
	Physics_Scale.z = 1;

	// Physics Pos
	Physics_Pos.x = 0;
	Physics_Pos.y = 0;
	Physics_Pos.z = 0;

	// Physics Rotation
	Physics_Rot.x = 0;
	Physics_Rot.y = 0;
	Physics_Rot.z = 0;

	// Physics Size
	Physics_Size.x = 0;
	Physics_Size.y = 0;
	Physics_Size.z = 0;

	// Physics Quat
	Physics_Quat.w = 1;
	Physics_Quat.x = 0;
	Physics_Quat.y = 0;
	Physics_Quat.z = 0;
	flag_Physics_Valid = 0;

	Usage = Enums::Usage_None;

	UsageEX = 0;

	Folder = 0;// Enums::Folder_None;

	Shape = -1;

	FileViewItem = nullptr;

	This_Object_UniqueID = 0; // This Needs Checking

	flag_Deleted = false;
	flag_Altered = false;

	Collision = false;
	flag_Triggered = false;
	flag_Physics_Debug_On = false;
	flag_Dimensions_Locked = false;
	flag_Is_Visible = true;

	flag_OverRide_Counter = false;

	//------------------------------ Message Entity
	//strcpy(Message_Text, "This is in the Object");

	//------------------------------ Sound Entity
	flag_HasSound = false;
	strcpy(Sound_File, "Door_Open.wav");
	Sound_Path[0] = 0;
	flag_Play_Sound = 1;
	SndVolume = 0.5; // Default Half Volume

	flag_Show_Message_Flag = false;
	//strcpy(Message_Text, "Welcome");
	strcpy(ImGui_Panel_Name, "TextMessage_");

	Vertice_Count_Copy = 0;
	return 1;
}

// *************************************************************************
// *		Set_ImGui_Panel_Name:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void Base_Object::Set_ImGui_Panel_Name(void)
{
	ImGui_Panel_Name[0] = 0;

	char Num[10];
	_itoa(This_Object_UniqueID, Num, 10);

	strcpy(ImGui_Panel_Name, "TextMessage_");
	strcat(ImGui_Panel_Name, Num);

}

// *************************************************************************
// *			Render_ImGui_Panel:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void Base_Object::Render_ImGui_Panel(void)
{
	/*ImGui::SetNextWindowPos(ImVec2(S_Message[0]->Message_PosX, S_Message[0]->Message_PosY));

	ImGuiWindowFlags window_flags = 0;

	if (S_Message[0]->flag_Show_BackGround == 1)
	{
		window_flags |= ImGuiWindowFlags_NoSavedSettings;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	}
	else
	{
		window_flags |= ImGuiWindowFlags_NoSavedSettings;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		window_flags |= ImGuiWindowFlags_NoBackground;
	}

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(S_Message[0]->BackGround_Colour.x, S_Message[0]->BackGround_Colour.y, S_Message[0]->BackGround_Colour.z, 255));

	if (!ImGui::Begin(ImGui_Panel_Name, &flag_Show_Message_Flag, window_flags))
	{
		ImGui::End();
	}
	else
	{

		ImVec2 Size = ImGui::GetWindowSize();

		ImGui::PushFont(App->CL_ImGui->font2);

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(S_Message[0]->Text_Colour.x, S_Message[0]->Text_Colour.y, S_Message[0]->Text_Colour.z, S_Message[0]->Text_Colour.w));

		ImGui::Text("%s", S_Message[0]->Message_Text);

		if (S_Message[0]->flag_PosXCentre_Flag == 1)
		{
			S_Message[0]->Message_PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (Size.x / 2);
		}

		if (S_Message[0]->flag_PosYCentre_Flag == 1)
		{
			S_Message[0]->Message_PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (Size.y / 2);
		}

		ImGui::PopFont();
		ImGui::PopStyleColor();

		ImGui::End();


	}*/
}

// *************************************************************************
// *			Create_Brush:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void Base_Object::Create_Brush(void)
{
	Brus_BoxTemplate = App->CL_Level->Level_GetBoxTemplate();

	Brus_BoxTemplate->Solid = 0;
	Brus_BoxTemplate->TCut = 0;
	Brus_BoxTemplate->Thickness = 16;
	Brus_BoxTemplate->TSheet = 0;

	Brus_BoxTemplate->XSizeBot = 100;
	Brus_BoxTemplate->XSizeTop = 100;
	Brus_BoxTemplate->YSize = 100;
	Brus_BoxTemplate->ZSizeBot = 100;
	Brus_BoxTemplate->ZSizeTop = 100;

	Object_Brush = BrushTemplate_CreateBox(Brus_BoxTemplate);
	if (Object_Brush != NULL)
	{

		strcpy(Object_Brush->Name, "Test");
		App->CL_Level->Level_AppendBrush(Object_Brush);

		App->CL_Doc->Set_Faces_To_Brush_Name_All();
		//App->CL_Brush_X->Set_Brush_Faces_Name(Br_Test);

		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		App->CL_Properties_Brushes->Fill_ListBox();

		//Set_Brush_Vecs(Br_Test);
		//Ogre_To_Mesh_Data(Object_Ent);
	}
	else
	{
		App->Say("No pCube");
	}
}

// *************************************************************************
// *					BrushTemplate_CreateBox							   *
// *************************************************************************
Brush* Base_Object::BrushTemplate_CreateBox(const BrushTemplate_Box* pTemplate)
{
	
	T_Vec3	FaceVerts[4]{ 0 };
	FaceList* fl;
	Face* f;
	Brush* b;

	fl = App->CL_X_FaceList->FaceList_Create(6);

	FaceVerts[3] = Verts[0];
	FaceVerts[2] = Verts[1];
	FaceVerts[1] = Verts[2];
	FaceVerts[0] = Verts[3];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 1;
		App->CL_X_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[4];
	FaceVerts[2] = Verts[5];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[7];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 2;
		App->CL_X_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[1];
	FaceVerts[2] = Verts[7];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[2];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 3;
		App->CL_X_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[0];
	FaceVerts[2] = Verts[3];
	FaceVerts[1] = Verts[5];
	FaceVerts[0] = Verts[4];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 4;
		App->CL_X_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[0];
	FaceVerts[2] = Verts[4];
	FaceVerts[1] = Verts[7];
	FaceVerts[0] = Verts[1];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 5;
		App->CL_X_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[3];
	FaceVerts[2] = Verts[2];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[5];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 6;
		App->CL_X_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	b = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);

	if (b)
	{
		return	b;
	}
	
	return	NULL;
}


