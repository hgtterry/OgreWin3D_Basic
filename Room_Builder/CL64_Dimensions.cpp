/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

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
#include "CL64_Dimensions.h"

CL64_Dimensions::CL64_Dimensions(void)
{
	flag_Show_Dimensions = 0;

	flag_Show_Position = 0;
	flag_Show_Scale = 0;
	flag_Show_Rotation = 0;

	flag_PosX_Selected = 1;
	flag_PosY_Selected = 0;
	flag_PosZ_Selected = 0;

	flag_ScaleX_Selected = 1;
	flag_ScaleY_Selected = 0;
	flag_ScaleZ_Selected = 0;

	flag_RotationX_Selected = 1;
	flag_RotationY_Selected = 0;
	flag_RotationZ_Selected = 0;

	Model_Pos_Delta = 1;
	Model_Scale_Delta = 0.01;
	Model_Rotation_Delta = 1;

	flag_Scale_Lock = 1;

	// -------------------------- Pointers
	pBase_Mesh_Name = NULL;

	pBase_Shape = NULL;

	pBase_Object_Ent = NULL;
	pBase_Object_Node = NULL;
	pBase_Mesh_Pos = NULL;
	pBase_Mesh_Rot = NULL;
	pBase_Mesh_Quat = NULL;
	pBase_Mesh_Scale = NULL;
	pBase_Phys_Body = NULL;
	pBase_Physics_Pos = NULL;
	pBase_Physics_Scale = NULL;
	pBase_Physics_Rot = NULL;
	pBase_Physics_Quat = NULL;

}

CL64_Dimensions::~CL64_Dimensions(void)
{
}

// *************************************************************************
// *						ImGui_Dimensions  Terry Flanigan			   *
// *************************************************************************
void CL64_Dimensions::Prepare_Dimensions(void)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	if (App->CL_Properties_Scene->Edit_Category == Enums::Edit_Area)
	{
		/*pBase_Mesh_Name = App->CL_Editor_Com->B_Area[Index]->Area_Name;

		pBase_Shape = &App->CL_Editor_Com->B_Area[Index]->Shape;

		pBase_Object_Ent = App->CL_Editor_Com->B_Area[Index]->Area_Ent;
		pBase_Object_Node = App->CL_Editor_Com->B_Area[Index]->Area_Node;
		pBase_Mesh_Pos = &App->CL_Editor_Com->B_Area[Index]->Mesh_Pos;
		pBase_Mesh_Scale = &App->CL_Editor_Com->B_Area[Index]->Mesh_Scale;
		pBase_Mesh_Rot = &App->CL_Editor_Com->B_Area[Index]->Mesh_Rot;
		pBase_Mesh_Quat = &App->CL_Editor_Com->B_Area[Index]->Mesh_Quat;

		pBase_Phys_Body = App->CL_Editor_Com->B_Area[Index]->Phys_Body;
		pBase_Physics_Pos = &App->CL_Editor_Com->B_Area[Index]->Physics_Pos;
		pBase_Physics_Scale = &App->CL_Editor_Com->B_Area[Index]->Physics_Scale;
		pBase_Physics_Rot = &App->CL_Editor_Com->B_Area[Index]->Physics_Rot;
		pBase_Physics_Quat = &App->CL_Editor_Com->B_Area[Index]->Physics_Quat;*/

		//App->Say("Area");
	}
	else
	{
		pBase_Mesh_Name = App->CL_Editor_Com->B_Object[Index]->Object_Name;

		pBase_Shape = &App->CL_Editor_Com->B_Object[Index]->Shape;

		pBase_Object_Ent = App->CL_Editor_Com->B_Object[Index]->Object_Ent;
		pBase_Object_Node = App->CL_Editor_Com->B_Object[Index]->Object_Node;
		pBase_Mesh_Pos = &App->CL_Editor_Com->B_Object[Index]->Mesh_Pos;
		pBase_Mesh_Scale = &App->CL_Editor_Com->B_Object[Index]->Mesh_Scale;
		pBase_Mesh_Rot = &App->CL_Editor_Com->B_Object[Index]->Mesh_Rot;
		pBase_Mesh_Quat = &App->CL_Editor_Com->B_Object[Index]->Mesh_Quat;

		pBase_Phys_Body = App->CL_Editor_Com->B_Object[Index]->Phys_Body;
		pBase_Physics_Pos = &App->CL_Editor_Com->B_Object[Index]->Physics_Pos;
		pBase_Physics_Scale = &App->CL_Editor_Com->B_Object[Index]->Physics_Scale;
		pBase_Physics_Rot = &App->CL_Editor_Com->B_Object[Index]->Physics_Rot;
		pBase_Physics_Quat = &App->CL_Editor_Com->B_Object[Index]->Physics_Quat;

		//App->Say("Object");
	}
}

// *************************************************************************
// *						ImGui_Dimensions  Terry Flanigan			   *
// *************************************************************************
void CL64_Dimensions::ImGui_Dimensions(void)
{
	ImGui::SetNextWindowPos(ImVec2(250, 10), ImGuiCond_FirstUseEver);

	ImGuiStyle* style = &ImGui::GetStyle();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255, 255, 255, 240));


	if (!ImGui::Begin("Rotation2", &flag_Show_Dimensions, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{


		int Index = App->CL_Properties_Scene->Current_Selected_Object;

		ImGui::Indent();
		ImGui::Indent();

		//--------------------------------------- Position
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.8f, 1);

		if (flag_Show_Position == 1)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}

		if (ImGui::Button("Position"))
		{
			flag_Show_Dimensions = 0;
			flag_Show_Position = 1;
			flag_Show_Scale = 0;
			flag_Show_Rotation = 0;

			//App->SBC_Markers->Hide_Axis_Marker();
			//RedrawWindow(App->SBC_Props_Dialog->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		}

		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.8f, 1);

		//--------------------------------------- Rotation
		ImGui::SameLine();

		if (flag_Show_Rotation == 1)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}

		if (ImGui::Button("Rotation"))
		{
			flag_Show_Dimensions = 0;
			flag_Show_Position = 0;
			flag_Show_Scale = 0;
			flag_Show_Rotation = 1;

			//App->SBC_Markers->Hide_Axis_Marker();
			//RedrawWindow(App->SBC_Props_Dialog->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		}
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.8f, 1);

		//--------------------------------------- Scale
		ImGui::SameLine();

		if (flag_Show_Scale == 1)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}

		if (ImGui::Button("Scale"))
		{
			flag_Show_Dimensions = 0;
			flag_Show_Position = 0;
			flag_Show_Scale = 1;
			flag_Show_Rotation = 0;

			//App->SBC_Markers->Hide_Axis_Marker();
			//RedrawWindow(App->SBC_Props_Dialog->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		}

		style->Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);

		ImGui::Unindent();
		ImGui::Unindent();

		if (flag_Show_Position == 1)
		{
			ImGui_Position();
		}

		if (flag_Show_Scale == 1)
		{
			ImGui_Scale();
		}

		if (flag_Show_Rotation == 1)
		{
			ImGui_Rotation();
		}

		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Close"))
		{
			flag_Show_Dimensions = 0;
			flag_Show_Position = 0;
			flag_Show_Scale = 0;
			flag_Show_Rotation = 0;

			//App->SBC_Markers->Hide_Axis_Marker();
			//RedrawWindow(App->SBC_Props_Dialog->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		}

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *						ImGui_Position Terry Flanigan				   *
// *************************************************************************
void CL64_Dimensions::ImGui_Position(void)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	Ogre::Vector3 Pos = *pBase_Mesh_Pos;// App->SBC_Scene->B_Object[Index]->Mesh_Pos;

	ImGuiStyle* style = &ImGui::GetStyle();

	ImGui::Text("Position");
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Object ID = %i", App->CL_Properties_Scene->Current_Selected_Object);
	ImGui::Text("Object Name: = %s", pBase_Mesh_Name);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	float vec4a[4] = { Pos.x, Pos.y, Pos.z, 0.44f };
	ImGui::InputFloat3("", vec4a, "%.3f", ImGuiInputTextFlags_ReadOnly);

	// ----------------------------------------------------------------------------- Pos CheckBox X 
	ImGui::Indent();
	ImGui::Indent();

	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

	ImGui::Checkbox("PX", &flag_PosX_Selected);

	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);


	if (flag_PosX_Selected == 1)
	{
		App->CL_Gizmos->Hide_Axis_Marker();
		App->CL_Gizmos->Update_Blue_Axis_Marker(Index);

		flag_PosY_Selected = 0;
		flag_PosZ_Selected = 0;
	}

	//------------------------------------------------------------------------------- Pos CheckBox Y
	ImGui::SameLine();
	ImGui::Text("        ");
	ImGui::SameLine();

	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.9f, 0.0f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.f, 1.f, 0.f, 1.00f);
	ImGui::Checkbox("PY", &flag_PosY_Selected);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	if (flag_PosY_Selected)
	{
		App->CL_Gizmos->Hide_Axis_Marker();
		App->CL_Gizmos->Update_Green_Axis_Marker(Index);

		flag_PosX_Selected = 0;
		flag_PosZ_Selected = 0;
	}

	//------------------------------------------------------------------------------- Pos CheckBox Z
	ImGui::SameLine();
	ImGui::Text("         ");
	ImGui::SameLine();

	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.9f, 0.0f, 0.0f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.f, 0.f, 0.f, 1.00f);
	ImGui::Checkbox("PZ", &flag_PosZ_Selected);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	if (flag_PosZ_Selected)
	{
		App->CL_Gizmos->Hide_Axis_Marker();
		App->CL_Gizmos->Update_Red_Axis_Marker(Index);

		flag_PosX_Selected = 0;
		flag_PosY_Selected = 0;
	}

	ImGui::Indent(10);

	ImGui::Spacing();
	ImGui::Spacing();

	// ----------------------------------------------------------------------------- Position Do Move 
	ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
	if (ImGui::Button("+##PosP", ImVec2(30, 0)))
	{
		//if (App->CL_Editor_Com->flag_Scene_Loaded == 1)
		{
			if (flag_PosX_Selected == 1)
			{
				Pos.x = Pos.x + Model_Pos_Delta;
				Set_Position(Pos);
			}

			if (flag_PosY_Selected == 1)
			{
				Pos.y = Pos.y + Model_Pos_Delta;
				Set_Position(Pos);
			}

			if (flag_PosZ_Selected == 1)
			{
				Pos.z = Pos.z + Model_Pos_Delta;
				Set_Position(Pos);
			}
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("-##PosM", ImVec2(30, 0)))
	{
		//if (App->CL_Editor_Com->flag_Scene_Loaded == 1)
		{
			if (flag_PosX_Selected == 1)
			{
				Pos.x = Pos.x - Model_Pos_Delta;
				Set_Position(Pos);
			}

			if (flag_PosY_Selected == 1)
			{
				Pos.y = Pos.y - Model_Pos_Delta;
				Set_Position(Pos);
			}

			if (flag_PosZ_Selected == 1)
			{
				Pos.z = Pos.z - Model_Pos_Delta;
				Set_Position(Pos);
			}
		}
	}

	ImGui::PopItemFlag();

	// ----------------------------------------------------------------------------- Position Combo Step
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	const char* XitemsPosXX[] = { "0.001","0.01","0.1","1", "2", "5", "10", "20" };
	static int XitemPosXX = 3;
	bool ChangedPosX = ImGui::Combo("Step Pos", &XitemPosXX, XitemsPosXX, IM_ARRAYSIZE(XitemsPosXX));
	if (ChangedPosX == 1)
	{
		Model_Pos_Delta = (float)atof(XitemsPosXX[XitemPosXX]);
	}

	style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

	ImGui::Spacing();
	ImGui::Unindent(10);
	ImGui::Unindent();
	ImGui::Unindent();
}

// *************************************************************************
// *						Set_Position Terry Flanigan					   *
// *************************************************************************
void CL64_Dimensions::Set_Position(Ogre::Vector3 Pos)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	pBase_Object_Node->setPosition(Pos);

	pBase_Mesh_Pos->x = Pos.x;
	pBase_Mesh_Pos->y = Pos.y;
	pBase_Mesh_Pos->z = Pos.z;

	App->CL_Brush_X->Move_Brush_By_Name(App->CL_Editor_Com->B_Object[Index]->Object_Name, Index);

	if (pBase_Phys_Body)
	{

		Ogre::Vector3 Centre = Get_BoundingBox_World_Centre();

		pBase_Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));

		UpDate_Physics(Index);
		App->CL_Gizmos->MarkerBox_Addjust(Index);
	}
	else
	{
		//App->CL_Gizmos->MarkerBox_Addjust(Index);
	}
}

// *************************************************************************
// *						ImGui_Scale Terry Flanigan					   *
// *************************************************************************
void CL64_Dimensions::ImGui_Scale(void)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	Ogre::Vector3 Scale = *pBase_Mesh_Scale;

	ImGuiStyle* style = &ImGui::GetStyle();

	ImGui::Text("Scale");
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Object ID = %i", App->CL_Properties_Scene->Current_Selected_Object);
	ImGui::Text("Object Name: = %s", pBase_Mesh_Name);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	float vec4a[4] = { Scale.x, Scale.y, Scale.z, 0.44f };
	ImGui::InputFloat3("", vec4a, "%.3f", ImGuiInputTextFlags_ReadOnly);

	// ----------------------------------------------------------------------------- Scale Checkbox X
	ImGui::Indent();
	ImGui::Indent();

	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

	ImGui::Checkbox("SX", &flag_ScaleX_Selected);

	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);

	if (flag_ScaleX_Selected == 1)
	{
		if (flag_Scale_Lock == 1)
		{
			App->CL_Gizmos->Hide_Axis_Marker();
			App->CL_Gizmos->Update_Red_Axis_Marker(Index);
			App->CL_Gizmos->Update_Green_Axis_Marker(Index);
			App->CL_Gizmos->Update_Blue_Axis_Marker(Index);
		}
		else
		{
			App->CL_Gizmos->Hide_Axis_Marker();
			App->CL_Gizmos->Update_Blue_Axis_Marker(Index);
		}

		flag_ScaleY_Selected = 0;
		flag_ScaleZ_Selected = 0;
	}

	//------------------------------------------------------------------------------- Scale Checkbox Y
	ImGui::SameLine();
	ImGui::Text("        ");
	ImGui::SameLine();

	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.9f, 0.0f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.f, 1.f, 0.f, 1.00f);
	ImGui::Checkbox("SY", &flag_ScaleY_Selected);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	if (flag_ScaleY_Selected)
	{
		if (flag_Scale_Lock == 1)
		{
			App->CL_Gizmos->Hide_Axis_Marker();
			App->CL_Gizmos->Update_Red_Axis_Marker(Index);
			App->CL_Gizmos->Update_Green_Axis_Marker(Index);
			App->CL_Gizmos->Update_Blue_Axis_Marker(Index);
		}
		else
		{
			App->CL_Gizmos->Hide_Axis_Marker();
			App->CL_Gizmos->Update_Green_Axis_Marker(Index);
		}

		flag_ScaleX_Selected = 0;
		flag_ScaleZ_Selected = 0;
	}

	//------------------------------------------------------------------------------- Scale Checkbox Z
	ImGui::SameLine();
	ImGui::Text("         ");
	ImGui::SameLine();

	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
	ImGui::Checkbox("SZ", &flag_ScaleZ_Selected);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	if (flag_ScaleZ_Selected)
	{
		if (flag_Scale_Lock == 1)
		{
			App->CL_Gizmos->Hide_Axis_Marker();
			App->CL_Gizmos->Update_Red_Axis_Marker(Index);
			App->CL_Gizmos->Update_Green_Axis_Marker(Index);
			App->CL_Gizmos->Update_Blue_Axis_Marker(Index);
		}
		else
		{
			App->CL_Gizmos->Hide_Axis_Marker();
			App->CL_Gizmos->Update_Red_Axis_Marker(Index);
		}

		flag_ScaleX_Selected = 0;
		flag_ScaleY_Selected = 0;
	}

	ImGui::Indent(10);
	ImGui::Spacing();
	ImGui::Spacing();

	// ----------------------------------------------------------------------------- Scale Do Scale
	ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);

	if (ImGui::Button("+##ScaleP", ImVec2(30, 0)))
	{
		//if (App->CL_Editor_Com->flag_Scene_Loaded == 1)
		{
			if (flag_Scale_Lock == 1)
			{
				Scale.x = Scale.x + Model_Scale_Delta;
				Scale.y = Scale.y + Model_Scale_Delta;
				Scale.z = Scale.z + Model_Scale_Delta;

				Set_Scale(Scale);
			}
			else
			{
				if (flag_ScaleX_Selected == 1)
				{
					Scale.x = Scale.x + Model_Scale_Delta;

					Set_Scale(Scale);

				}

				if (flag_ScaleY_Selected == 1)
				{
					Scale.y = Scale.y + Model_Scale_Delta;

					Set_Scale(Scale);

				}

				if (flag_ScaleZ_Selected == 1)
				{
					Scale.z = Scale.z + Model_Scale_Delta;

					Set_Scale(Scale);
				}
			}
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("-##ScaleM", ImVec2(30, 0)))
	{
		//if (App->CL_Editor_Com->flag_Scene_Loaded == 1)
		{
			if (flag_Scale_Lock == 1)
			{
				Scale.x = Scale.x - Model_Scale_Delta;
				Scale.y = Scale.y - Model_Scale_Delta;
				Scale.z = Scale.z - Model_Scale_Delta;

				Set_Scale(Scale);
			}
			else
			{
				if (flag_ScaleX_Selected == 1)
				{
					Scale.x = Scale.x - Model_Scale_Delta;

					Set_Scale(Scale);

				}

				if (flag_ScaleY_Selected == 1)
				{
					Scale.y = Scale.y - Model_Scale_Delta;

					Set_Scale(Scale);

				}

				if (flag_ScaleZ_Selected == 1)
				{
					Scale.z = Scale.z - Model_Scale_Delta;

					Set_Scale(Scale);

				}
			}
		}
	}
	
	ImGui::PopItemFlag();

	// ----------------------------------------------------------------------------- Scale Combo Step
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	const char* XitemsScaleXX[] = { "0.001","0.01","0.1","1", "2", "5", "10", "20" };
	static int XitemScaleXX = 1;
	bool ChangedScaleX = ImGui::Combo("Step Scale", &XitemScaleXX, XitemsScaleXX, IM_ARRAYSIZE(XitemsScaleXX));   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.
	if (ChangedScaleX == 1)
	{
		Model_Scale_Delta = (float)atof(XitemsScaleXX[XitemScaleXX]);
	}



	ImGui::Checkbox("Lock Axis", &flag_Scale_Lock);
	{

	}

	style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

	ImGui::Spacing();
	ImGui::Unindent(10);
	ImGui::Unindent();
	ImGui::Unindent();
}

// *************************************************************************
// *						Set_Scale Terry Flanigan					   *
// *************************************************************************
void CL64_Dimensions::Set_Scale(Ogre::Vector3 Scale)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	pBase_Object_Node->setScale(Scale);

	pBase_Mesh_Scale->x = Scale.x;
	pBase_Mesh_Scale->y = Scale.y;
	pBase_Mesh_Scale->z = Scale.z;

	float sizeX = App->CL_Editor_Com->B_Object[Index]->Object_Node->_getWorldAABB().getSize().x;
	float sizeY = App->CL_Editor_Com->B_Object[Index]->Object_Node->_getWorldAABB().getSize().y;
	float sizeZ = App->CL_Editor_Com->B_Object[Index]->Object_Node->_getWorldAABB().getSize().z;

	App->CL_Brush_X->Scale_Brush_By_Name(App->CL_Editor_Com->B_Object[Index]->Object_Name, Index, sizeX, sizeY, sizeZ);

	if (pBase_Phys_Body)
	{
		pBase_Phys_Body->getCollisionShape()->setLocalScaling(btVector3(Scale.x, Scale.y, Scale.z));
		pBase_Physics_Scale->x = Scale.x;
		pBase_Physics_Scale->y = Scale.y;
		pBase_Physics_Scale->z = Scale.z;

		UpDate_Physics(Index);
		App->CL_Gizmos->MarkerBox_Addjust(Index);
	}

//	App->SBC_Markers->MarkerBB_Addjust(Index);
}

// *************************************************************************
// *						ImGui_Rotation Terry Flanigan				   *
// *************************************************************************
void CL64_Dimensions::ImGui_Rotation(void)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	Ogre::Vector3 mRotation = *pBase_Mesh_Rot;

	ImGuiStyle* style = &ImGui::GetStyle();

	ImGui::Text("Rotation");
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Object ID = %i", App->CL_Properties_Scene->Current_Selected_Object);
	ImGui::Text("Object Name: = %s", pBase_Mesh_Name);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	float vec4a[4] = { mRotation.x, mRotation.y, mRotation.z, 0.44f };
	ImGui::InputFloat3("", vec4a, "%.3f", ImGuiInputTextFlags_ReadOnly);

	// ----------------------------------------------------------------------------- Rotation X
	ImGui::Indent();
	ImGui::Indent();
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	ImGui::Checkbox("RX", &flag_RotationX_Selected);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);

	if (flag_RotationX_Selected == 1)
	{
		flag_RotationY_Selected = 0;
		flag_RotationZ_Selected = 0;
	}

	//------------------------------------------------------------------------------- Rotation Y
	ImGui::SameLine();
	ImGui::Text("        ");
	ImGui::SameLine();

	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.9f, 0.0f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.f, 1.f, 0.f, 1.00f);
	ImGui::Checkbox("RY", &flag_RotationY_Selected);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	if (flag_RotationY_Selected)
	{
		flag_RotationX_Selected = 0;
		flag_RotationZ_Selected = 0;
	}

	//------------------------------------------------------------------------------- Rotation Z
	ImGui::SameLine();
	ImGui::Text("         ");
	ImGui::SameLine();

	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
	ImGui::Checkbox("RZ", &flag_RotationZ_Selected);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	if (flag_RotationZ_Selected)
	{
		flag_RotationX_Selected = 0;
		flag_RotationY_Selected = 0;
	}
	// ----------------------------------------------------------------------------- Rotation

	ImGui::Indent(10);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat,true);
	if (ImGui::Button("+##RotP", ImVec2(30, 0)))
	{
		//if (App->CL_Editor_Com->flag_Scene_Loaded == 1)
		{
			if (flag_RotationX_Selected == 1)
			{
				pBase_Mesh_Rot->x += Model_Rotation_Delta;
				Set_Rotation(Ogre::Vector3(1, 0, 0), Model_Rotation_Delta);

			}

			if (flag_RotationY_Selected == 1)
			{
				pBase_Mesh_Rot->y += Model_Rotation_Delta;
				Set_Rotation(Ogre::Vector3(0, 1, 0), Model_Rotation_Delta);
			}

			if (flag_RotationZ_Selected == 1)
			{
				pBase_Mesh_Rot->z += Model_Rotation_Delta;
				Set_Rotation(Ogre::Vector3(0, 0, 1), Model_Rotation_Delta);

			}
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("-##RotM", ImVec2(30, 0)))
	{
		//if (App->CL_Editor_Com->flag_Scene_Loaded == 1)
		{

			if (flag_RotationX_Selected == 1)
			{
				pBase_Mesh_Rot->x -= Model_Rotation_Delta;
				Set_Rotation(Ogre::Vector3(1, 0, 0), -Model_Rotation_Delta);

			}

			if (flag_RotationY_Selected == 1)
			{
				pBase_Mesh_Rot->y -= Model_Rotation_Delta;
				Set_Rotation(Ogre::Vector3(0, 1, 0), -Model_Rotation_Delta);

			}

			if (flag_RotationZ_Selected == 1)
			{
				pBase_Mesh_Rot->z -= Model_Rotation_Delta;
				Set_Rotation(Ogre::Vector3(0, 0, 1), -Model_Rotation_Delta);

			}

		}
	}

	ImGui::PopItemFlag();
	
	// ----------------------------------------------------------------------------- Rotation Combo Step
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	const char* XitemsRotXX[] = { "0.001","0.01","0.1","1", "2", "5", "10", "45" };
	static int XitemRotXX = 3;
	bool ChangedRotX = ImGui::Combo("Step Rot", &XitemRotXX, XitemsRotXX, IM_ARRAYSIZE(XitemsRotXX));   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.
	if (ChangedRotX == 1)
	{
		Model_Rotation_Delta = (float)atof(XitemsRotXX[XitemRotXX]);
	}

	style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

	ImGui::Spacing();
	ImGui::Unindent(10);
	ImGui::Unindent();
	ImGui::Unindent();
}

// *************************************************************************
// *						Set_Rotation Terry Flanigan					   *
// *************************************************************************
void CL64_Dimensions::Set_Rotation(Ogre::Vector3 Rotation, float Delta)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	pBase_Object_Node->rotate(Ogre::Quaternion(Ogre::Degree(Delta), Rotation), Ogre::Node::TransformSpace::TS_LOCAL);

	pBase_Mesh_Quat->w = pBase_Object_Node->getOrientation().w;
	pBase_Mesh_Quat->x = pBase_Object_Node->getOrientation().x;
	pBase_Mesh_Quat->y = pBase_Object_Node->getOrientation().y;
	pBase_Mesh_Quat->z = pBase_Object_Node->getOrientation().z;

	if (pBase_Phys_Body)
	{
		pBase_Physics_Rot->x += Model_Rotation_Delta;

		pBase_Physics_Quat->w = pBase_Object_Node->getOrientation().w;
		pBase_Physics_Quat->x = pBase_Object_Node->getOrientation().x;
		pBase_Physics_Quat->y = pBase_Object_Node->getOrientation().y;
		pBase_Physics_Quat->z = pBase_Object_Node->getOrientation().z;

		pBase_Phys_Body->getWorldTransform().setRotation(btQuaternion(pBase_Physics_Quat->x,
			pBase_Physics_Quat->y, pBase_Physics_Quat->z, pBase_Physics_Quat->w));

		UpDate_Physics(Index);
		App->CL_Gizmos->MarkerBox_Addjust(Index);
	}

	float RotX = App->CL_Editor_Com->B_Object[Index]->Mesh_Rot.x;
	float RotY = App->CL_Editor_Com->B_Object[Index]->Mesh_Rot.y;
	float RotZ = App->CL_Editor_Com->B_Object[Index]->Mesh_Rot.z;

	//App->CL_Brush_X->Rotate_Brush_By_Name(App->CL_Editor_Com->B_Object[Index]->Object_Name, Index, RotX, RotY, RotZ);

//	App->SBC_Markers->MarkerBB_Addjust(Index);
}

// *************************************************************************
// *	Get_BoundingBox_World_Centre:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
Ogre::Vector3 CL64_Dimensions::Get_BoundingBox_World_Centre()
{
	if (*pBase_Shape == Enums::Shape_TriMesh)
	{
		Ogre::Vector3 Pos = pBase_Object_Node->getPosition();
		return Pos;
	}
	else
	{
	
		Ogre::Vector3 Centre = pBase_Object_Ent->getWorldBoundingBox(true).getCenter();
		return Centre;
	}
}

// *************************************************************************
// *			UpDate_Physics:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Dimensions::UpDate_Physics(int Index)
{
	if (*pBase_Shape == Enums::Shape_TriMesh)
	{

	}
	else
	{
		{
			Set_Physics_Position();
		}
	}

	// Needs Looking at
	App->CL_Editor_Com->B_Object[Index]->flag_Altered = 1;
	App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Object[Index]->FileViewItem);
	App->CL_Level->flag_Level_is_Modified = 1;
}

// *************************************************************************
// *	  	Set_Physics_Position:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dimensions::Set_Physics_Position()
{

	Ogre::Vector3 Centre = pBase_Object_Ent->getWorldBoundingBox(true).getCenter();
	pBase_Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));

	pBase_Physics_Pos->x = Centre.x;
	pBase_Physics_Pos->y = Centre.y;
	pBase_Physics_Pos->z = Centre.z;

}

