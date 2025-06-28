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
#include "CL64_ImGui_Editor.h"

CL64_ImGui_Editor::CL64_ImGui_Editor()
{
	Visuals_PosX = 100;
	Visuals_PosY = 100;

	flag_Show_Visuals = 0;
}

CL64_ImGui_Editor::~CL64_ImGui_Editor()
{
}


// **************************************************************************
// *		ImGui_Render_Editor_Loop:- Terry and Hazel Flanigan 2025		*
// **************************************************************************
void CL64_ImGui_Editor::ImGui_Render_Editor_Loop(void)
{
	if (flag_Show_Visuals == true)
	{
		Visuals_GUI();
	}
}

// *************************************************************************
// *			Visuals_GUI:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_ImGui_Editor::Visuals_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Visuals_PosX, Visuals_PosY));
	ImGui::SetNextWindowSize(ImVec2(166, 350), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));
	ImGuiStyle* style = &ImGui::GetStyle();

	if (!ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 0.5f, 1.00f);

		// -------------- Level Start
		if (ImGui::Button("Level Start", ImVec2(150, 100)))
		{
			

			flag_Show_Visuals = 0;
		}

		if (ImGui::Button("Player Position", ImVec2(150, 100)))
		{
			

			flag_Show_Visuals = 0;
		}

		// -------------- Return
		if (ImGui::Button("Return", ImVec2(150, 100)))
		{
			
		}

		ImVec2 Size = ImGui::GetWindowSize();
		Visuals_PosX = ((float)App->CL_Ogre->mWindow->getViewport(0)->getActualWidth() / 2) - (Size.x / 2);
		Visuals_PosY = ((float)App->CL_Ogre->mWindow->getViewport(0)->getActualHeight() / 2) - (Size.y / 2);

		style->Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);

		ImGui::PopStyleColor();
		ImGui::End();
	}
}
