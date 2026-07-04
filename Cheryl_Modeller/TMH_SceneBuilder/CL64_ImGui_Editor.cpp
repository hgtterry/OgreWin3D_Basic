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
};

CL64_ImGui_Editor::CL64_ImGui_Editor()
{
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

	Selected_System_Page = System_Page_Camera;
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

		if (ImGui::Button(" Camera ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Camera;
		}

		if (ImGui::Button(" Data ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Data;
		}

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

			ImGui::Text("Data:");

			//Camera_Data();
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
	ImGui::Text("Cam X %f", App->CL_Ogre->camNode->getPosition().x);
	ImGui::Text("Cam Y %f", App->CL_Ogre->camNode->getPosition().y);
	ImGui::Text("Cam Z %f", App->CL_Ogre->camNode->getPosition().z);
}
