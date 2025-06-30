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
	Visuals_PosX = 500;
	Visuals_PosY = 300;

	flag_Block_GUI = false;
	flag_Show_Visuals = false;
	flag_Object_Highlighted = false;
	flag_Show_Physics_Debug = false;
	flag_Show_Mesh = true;
}

CL64_ImGui_Editor::~CL64_ImGui_Editor()
{
}


// **************************************************************************
// *		ImGui_Render_Editor_Loop:- Terry and Hazel Flanigan 2025		*
// **************************************************************************
void CL64_ImGui_Editor::ImGui_Render_Editor_Loop(void)
{
	if (flag_Block_GUI == false)
	{
		if (flag_Show_Visuals == true)
		{
			Visuals_GUI();
		}
	}
}

// *************************************************************************
// *			Visuals_GUI:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_ImGui_Editor::Visuals_GUI(void)
{
	Visuals_PosX = ((float)App->CL_Ogre->mWindow->getViewport(0)->getActualWidth() -230);
	Visuals_PosY = ((float)App->CL_Ogre->mWindow->getViewport(0)->getActualHeight() -150);


	ImGui::SetNextWindowPos(ImVec2(Visuals_PosX, Visuals_PosY), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(216, 140), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));
	ImGuiStyle* style = &ImGui::GetStyle();

	if (!ImGui::Begin("Debug_Visuals", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 0.5f, 1.00f);

		// -------------- Show Mesh
		Selected_Button(flag_Show_Mesh);
		
		if (ImGui::Button("Show Mesh", ImVec2(200, 25)))
		{
			int Index = App->CL_Properties_Scene->Current_Selected_Object;

			if (App->CL_Scene->Object_Count > 0)
			{
				auto& currentObject = App->CL_Scene->B_Object[Index];

				// -----------------------  Area
				/*if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
				{
					if (App->CL_Com_Objects->flag_Show_Mesh_Debug == 1)
					{
						App->CL_Scene->B_Area[Index]->Area_Node->setVisible(false);
						App->CL_Com_Objects->flag_Show_Mesh_Debug = 0;
					}
					else
					{
						App->CL_Scene->B_Area[Index]->Area_Node->setVisible(true);
						App->CL_Com_Objects->flag_Show_Mesh_Debug = 1;
					}
					return 1;
				}*/

				if (currentObject->flag_Is_Visible == true)
				{
					flag_Show_Mesh = false;
					currentObject->flag_Is_Visible = false;
					App->CL_Scene->B_Object[Index]->Object_Node->setVisible(false);
					//App->CL_Com_Objects->flag_Show_Mesh_Debug = 0;
				}
				else
				{
					flag_Show_Mesh = true;
					currentObject->flag_Is_Visible = true;
					App->CL_Scene->B_Object[Index]->Object_Node->setVisible(true);
					//App->CL_Com_Objects->flag_Show_Mesh_Debug = true;
				}
			}
		}

		// -------------- Show Physics
		Selected_Button(flag_Show_Physics_Debug);
		
		if (ImGui::Button("Show Physics", ImVec2(200, 25)))
		{
			int Index = App->CL_Properties_Scene->Current_Selected_Object;

			// -----------------------  Objects
			if (App->CL_Scene->Object_Count > 0)
			{
				auto& currentObject = App->CL_Scene->B_Object[Index];

				// Check if the physics body is null
				if (currentObject->Phys_Body == nullptr)
				{
					App->Say("No Physics Shape");
					return;
				}

				int collisionFlags = currentObject->Phys_Body->getCollisionFlags();

				// Toggle physics debug mode
				if (currentObject->flag_Physics_Debug_On == 1)
				{
					flag_Show_Physics_Debug = false;
					//App->CL_Com_Objects->flag_Show_Physics_Debug = 0;
					currentObject->Phys_Body->setCollisionFlags(collisionFlags | (1 << 5)); // Disable debug
					currentObject->flag_Physics_Debug_On = false;

					// Draw debug shape
					//App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;
					//App->CL_Ogre->RenderFrame(4);
					//App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 1;
				}
				else
				{
					flag_Show_Physics_Debug = true;
					currentObject->flag_Physics_Debug_On = true;
					//App->CL_Com_Objects->flag_Show_Physics_Debug = 1;
					currentObject->Phys_Body->setCollisionFlags(collisionFlags & (~(1 << 5))); // Enable debug
				}
			}

		}
		
		// -------------- Only Mesh
		/*if (flag_Show_Mesh == true)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		else*/
		{
			style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 0.5f, 1.00f);
		}

		if (ImGui::Button("Only Mesh", ImVec2(200, 25)))
		{
			int Index = App->CL_Properties_Scene->Current_Selected_Object;

			//// -----------------------  Area
			//if (App->CL_Properties_Scene->Edit_Category == Enums::Edit_Area)
			//{
			//	if (App->CL_Com_Objects->flag_Hide_All_Except == 1)
			//	{
			//		App->CL_Com_Objects->flag_Hide_All_Except = 0;
			//		App->CL_Com_Objects->Hide_AllObjects_Except(Index, true);
			//	}
			//	else
			//	{
			//		App->CL_Com_Objects->flag_Hide_All_Except = 1;
			//		App->CL_Com_Objects->Hide_AllObjects_Except(Index, false);
			//	}
			//	return 1;
			//}

			if (App->CL_Com_Objects->flag_Hide_All_Except == 1)
			{
				App->CL_Com_Objects->flag_Hide_All_Except = 0;
				App->CL_Com_Objects->Hide_AllObjects_Except(Index, true);
			}
			else
			{
				App->CL_Com_Objects->flag_Hide_All_Except = 1;
				App->CL_Com_Objects->Hide_AllObjects_Except(Index, false);
			}
		}

		// -------------- Highlight
		Selected_Button(flag_Object_Highlighted);
	
		if (ImGui::Button("Highlight", ImVec2(200, 25)))
		{
			int index = App->CL_Properties_Scene->Current_Selected_Object;

			if (App->CL_Props_Dialogs->flag_isHighlighted == true)
			{
				flag_Object_Highlighted = false;
				App->CL_Gizmos->unhighlight(App->CL_Scene->B_Object[index]->Object_Ent);
			}
			else
			{
				flag_Object_Highlighted = true;
				App->CL_Gizmos->highlight(App->CL_Scene->B_Object[index]->Object_Ent);
			}
		}

		style->Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);

		ImGui::PopStyleColor();
		ImGui::End();
	}
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
