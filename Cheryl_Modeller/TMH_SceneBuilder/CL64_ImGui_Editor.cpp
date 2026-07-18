/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
#include "CL64_ImGui_Editor.h"

enum System_Page
{
	System_Page_Camera = 0,
	System_Page_Data = 1,
	System_Page_Model = 2,
	System_Page_Grids = 3
};

CL64_ImGui_Editor::CL64_ImGui_Editor()
{
	Selected_System_Page = System_Page_Grids;

	flag_Block_GUI = false;

	Visuals_PosX = 500;
	Visuals_PosY = 300;
	flag_Show_Visuals = false;

	Dimensions_PosX = 500;
	Dimensions_PosY = 300;
	flag_Show_Dimensions = false;
	flag_Show_Position = false;
	flag_Show_Scale = false;
	flag_Show_Rotation = false;

	flag_Object_Highlighted = false;
	flag_Show_Physics_Debug = false;
	flag_Show_Mesh = true;

	// Current 

	flag_Loop_Enabled = false;
	flag_Show_System_Data = false;
}

CL64_ImGui_Editor::~CL64_ImGui_Editor()
{
}

// *************************************************************************
// *			Selected_Button:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_ImGui_Editor::Selected_Button(bool IsSelected)
{
	ImGuiStyle* style = &ImGui::GetStyle();

	if (IsSelected == true)
	{
		style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 0.5f, 1.00f);
	}
}

// **************************************************************************
// *			ImGui_Render_Loop:- Terry and Hazel Flanigan 2026			*
// **************************************************************************
void CL64_ImGui_Editor::ImGui_Render_Loop(void)
{
	if (flag_Loop_Enabled == true)
	{
		if (flag_Show_System_Data == true)
		{
			Imgui_System_Data();
		}
	}
}


// *************************************************************************
// *			Imgui_System_Data:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_ImGui_Editor::Imgui_System_Data(void)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(550, 620));

	bool doStyle = true;

	if (doStyle == true)
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(239, 239, 239, 255));
	}

	if (!ImGui::Begin("Ogre Data", &flag_Show_System_Data, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(-1, 120);

		// Just Bottons
		if (ImGui::Button(" Camera ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Camera;
		}

		if (ImGui::Button(" Editor ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Data;
		}

		if (ImGui::Button(" Model ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Model;
		}

		if (ImGui::Button(" Grids ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Grids;
		}

		// Functions
		if (Selected_System_Page == System_Page_Camera)
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();

			ImGui::Text("Camera:");
			
			Camera_Data();
		}

		if (Selected_System_Page == System_Page_Data)
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();

			ImGui::Text("Editor:");

			Editor_Data();
		}

		if (Selected_System_Page == System_Page_Model)
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();

			ImGui::Text("Model:");

			Model_Data();
		}

		if (Selected_System_Page == System_Page_Grids)
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();

			ImGui::Text("Grids:");

			Grids_Data();
		}

		ImGui::Columns(0);

		if (doStyle == true)
		{
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		ImGui::End();
	}
}

// *************************************************************************
// *				Camera_Data:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_Editor::Camera_Data(void)
{
	char Buff[MAX_PATH];

	switch (App->CL_Ogre->Listener_3D->CameraMode) {
	case Enums::Cam_Mode_None:
		strcpy(Buff, "No Camera");
		break;
	case Enums::Cam_Mode_First:
		strcpy(Buff, "First Person View Mode");
		break;
	case Enums::Cam_Mode_Free:
		strcpy(Buff, "Free Mode");
		break;
	case Enums::Cam_Mode_Third:
		strcpy(Buff, "Third Person View Mode");
		break;
	case Enums::Cam_Mode_Model:
		strcpy(Buff, "   -- Model Mode --");
		break;
	default:
		strcpy(Buff, "Unknown Camera Mode");
		break;
	}

	ImGui::Text("Camera Mode %s", Buff);
	ImGui::Spacing();

	ImGui::Text("Cam X %f", App->CL_Ogre->camNode->getPosition().x);
	ImGui::Text("Cam Y %f", App->CL_Ogre->camNode->getPosition().y);
	ImGui::Text("Cam Z %f", App->CL_Ogre->camNode->getPosition().z);
}

// *************************************************************************
// *				Editor_Data:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_Editor::Editor_Data(void)
{
	char Buff[MAX_PATH];

	switch (App->CL_Editor_Control->Editor_Mode) 
	{
	case Enums::Editor_Mode_None:
		strcpy(Buff, "  -- Editor Not Set -- ");
		break;
	case Enums::Editor_Mode_Import_Model:
		strcpy(Buff, "  -- Mode Import Model -- ");
		break;
	case Enums::Editor_Mode_Design_Model:
		strcpy(Buff, "  -- Mode Design Model -- ");
		break;
	default:
		strcpy(Buff, "Unknown Model Mode");
		break;
	}

	ImGui::Text("Editor Mode %s", Buff);

}

// *************************************************************************
// *				Model_Data:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_Editor::Model_Data(void)
{
	char Buff[MAX_PATH];
	
	if (App->CL_Model->Imported_Ogre_Ent)
	{
		strcpy(Buff, "  -- Loaded -- ");
	}
	else
	{
		strcpy(Buff, "  -- Nothing --");
	}

	ImGui::Text("Imported_Ogre_Ent %s", Buff);

	if (App->CL_Mesh_Mgr->World_Ent)
	{
		strcpy(Buff, "  -- Loaded -- ");
	}
	else
	{
		strcpy(Buff, "  -- Nothing --");
	}

	ImGui::Text("World Entity %s", Buff);

	ImGui::Text(" ");
	ImGui::Text("Model Name %s", App->CL_Model->Model_Just_Name);
	
	switch (App->CL_Model->Model_Type)
	{
	case Enums::Model_Type_None:
		strcpy(Buff, "  -- Model_Type_None -- ");
		break;
	case Enums::Model_Type_Ogre3D:
		strcpy(Buff, "  -- Model_Type_Ogre3D -- ");
		break;
	case Enums::Model_Type_Assimp:
		strcpy(Buff, "  -- Model_Type_Assimp -- ");
		break;
	case Enums::Model_Type_Brush:
		strcpy(Buff, "  -- Model_Type_Brush -- ");
		break;

	default:
		strcpy(Buff, "Unknown Model Mode");
		break;
	}

	ImGui::Text("Model Type %s", Buff);

	ImGui::Text(" ");
	ImGui::Text("Counts ----------------------------------- ");
	ImGui::Text("Group Count %i", App->CL_Model->GroupCount);
	ImGui::Text("Texture Count %i", App->CL_Model->TextureCount);
	ImGui::Text("Motions %i", App->CL_Model->MotionCount);
	ImGui::Text("Vertice Count %i", App->CL_Model->VerticeCount);
	ImGui::Text("Face Count %i", App->CL_Model->FaceCount);
	ImGui::Text("Bone Count %i", App->CL_Model->BoneCount);
	ImGui::Text(" ");

	ImGui::Text("Model Loaded %i", App->CL_Model->flag_Model_Loaded);

	if (App->CL_Model->flag_BoundingBox_Created == true)
	{
		strcpy(Buff, "  -- Yes -- ");
	}
	else
	{
		strcpy(Buff, "  -- No --");
	}
	ImGui::Text("Bounding Box Created %s", Buff);
	
}

// *************************************************************************
// *				Grids_Data:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_Editor::Grids_Data(void)
{
	char Buff[MAX_PATH];

	switch (App->CL_Views_Com->Selected_Window)
	{
	case Enums::Selected_Map_View_None:
		strcpy(Buff, "  -- None -- ");
		break;
	case Enums::Selected_Map_View_3D:
		strcpy(Buff, "  -- 3D View -- ");
		break;
	case Enums::Selected_Map_View_TL:
		strcpy(Buff, "  -- Top Left Grid -- ");
		break;

	case Enums::Selected_Map_View_TR:
		strcpy(Buff, "  -- Top Right Grid -- ");
		break;

	case Enums::Selected_Map_View_BL:
		strcpy(Buff, "  -- Bottom Left Window -- ");
		break;

	default:
		strcpy(Buff, "Unknown Model Mode");
		break;
	}

	
	ImGui::Text("Selected Window %s", Buff);
	ImGui::Text(" ");

	ImGui::Text("Name %s", App->CL_View_Top_Left->VCam_TL->Name);
	ImGui::Text("Zoom Speed %f", App->CL_View_Top_Left->m_Zoom_Amount);
	ImGui::Text("Zoom Amount %f", App->CL_View_Top_Left->VCam_TL->ZoomFactor);
	ImGui::Text("Height %f", (float)App->CL_View_Top_Left->VCam_TL->Height);
	ImGui::Text("Width %f", (float)App->CL_View_Top_Left->VCam_TL->Width);
	ImGui::Text(" ");

	ImGui::Text("Name %s", App->CL_View_Top_Right->VCam_TR->Name);
	ImGui::Text("Zoom Speed %f", App->CL_View_Top_Right->m_Zoom_Amount);
	ImGui::Text("Zoom Amount %f", App->CL_View_Top_Right->VCam_TR->ZoomFactor);
	ImGui::Text("Height %f", (float)App->CL_View_Top_Right->VCam_TR->Height);
	ImGui::Text("Width %f", (float)App->CL_View_Top_Right->VCam_TR->Width);
	ImGui::Text(" ");

	ImGui::Text("Name %s", App->CL_View_Bottom_Left->VCam_BL->Name);
	ImGui::Text("Zoom Speed %f", App->CL_View_Bottom_Left->m_Zoom_Amount);
	ImGui::Text("Zoom Amount %f", App->CL_View_Bottom_Left->VCam_BL->ZoomFactor);
	ImGui::Text("Height %f", (float)App->CL_View_Bottom_Left->VCam_BL->Height);
	ImGui::Text("Width %f", (float)App->CL_View_Bottom_Left->VCam_BL->Width);
	ImGui::Text(" ");

}
