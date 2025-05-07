/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_ImGui.h"

CL64_ImGui::CL64_ImGui()
{
	flag_Imgui_Initialized = 0;

	font0 = 0;
	font1 = 0;
	font2 = 0;
	font3 = 0;
	fontDroid = 0;

	PosX = 500;
	PosY = 500;

	Cam_Mode_PosX = 10;
	Cam_Mode_PosY = 10;

	flag_Show_Player_Data = 0;
	flag_PlayerData_Start_Pos = 0;
	Player_Data_PosX = 10;
	Player_Data_PosY = 10;

	flag_Show_FPS = 1;
	flag_StartPos = 0;

	flag_Show_Paths = 0;
	flag_Show_Render_Reports = 0;
	flag_Show_Press_Excape = 0;
	flag_Show_Preview_Options = 0;
	flag_Show_App_Stats = 0;

	guiFunctions.reserve(20);
}

CL64_ImGui::~CL64_ImGui()
{
}

// *************************************************************************
// *			Close_Dialogs:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_ImGui::Close_Dialogs(void)
{
	flag_Show_Player_Data = 0;
}

// *************************************************************************
// *			Init_ImGui:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_ImGui::Init_ImGui(void)
{
	bool mOwnsImGuiOverlay = !Ogre::OverlayManager::getSingleton().getByName("ImGuiOverlay");

	OgreBites::ApplicationContextBase* Base = new OgreBites::ApplicationContextBase();

	Ogre::ImGuiOverlay* imguiOverlay = Base->initialiseImGui();

	if (imguiOverlay)
	{
		Load_Font();

		imguiOverlay->setZOrder(300);
		imguiOverlay->show();

		if (imguiOverlay->isInitialised())
		{
			if (mOwnsImGuiOverlay)
			{
				ImGui_Set_Colours();
				Ogre::ImGuiOverlay::NewFrame();
			}

			flag_Imgui_Initialized = 1;
		}
		else
		{
			App->Say("Could Not Initialised Imgui");
		}
	}

}

// *************************************************************************
// *			Load_Font:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_ImGui::Load_Font(void)
{
	ImGuiIO& io = ImGui::GetIO();
	font1 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 20);
	font2 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 48);
	font0 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 18);
	fontDroid = io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 12);

	io.IniFilename = NULL;
}

// *************************************************************************
// *	  	ImGui_Set_Colours:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_ImGui::ImGui_Set_Colours(void)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;

	style->WindowRounding = 6.0;
	style->FrameRounding = 0.0;
	style->WindowBorderSize = 0;
	style->FrameBorderSize = 1.0;

	colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	//colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.90f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	////colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	//colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	//colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style->HoverDelayNormal = 1;
}

// **************************************************************************
// *			Render_FPS:- Terry and Hazel Flanigan 2024					*
// **************************************************************************
void CL64_ImGui::ImGui_Render_Loop(void)
{
	guiFunctions =
	{
		{flag_Show_FPS == 1, [&]() { ImGui_FPS(); }},
		{flag_Show_Paths == 1, [&]() { Paths_GUI(); }},
		{flag_Show_Render_Reports == 1, [&]() { Render_Report_GUI(); }},
		{flag_Show_Press_Excape == 1, [&]() { Press_Excape_GUI(); }},
		{flag_Show_Preview_Options == 1, [&]() { Preview_Options_GUI(); }},
		{App->CL_Gui_Environment->flag_Show_PropertyEditor == 1, [&]() { App->CL_Gui_Environment->Environ_PropertyEditor(); }},
		{flag_Show_App_Stats == 1, [&]() { App_Stats_GUI(); }}
	};

	// SBC_Gui_Dialogs
	if (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
	{
		App->CL_ImGui_Dialogs->Dialog_Float();
	}

	if (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 == 1)
	{
		App->CL_ImGui_Dialogs->Dialog_Float_Vec3();
	}

	if (flag_Show_Player_Data == 1)
	{
		Player_Data_GUI();
	}

	// SBC_Dimensions
	if (App->CL_Dimensions->flag_Show_Position == 1 || App->CL_Dimensions->flag_Show_Scale == 1 || App->CL_Dimensions->flag_Show_Rotation == 1)
	{
		App->CL_Dimensions->flag_Show_Dimensions = 1;
		App->CL_Dimensions->ImGui_Dimensions();
	}
	else
	{
		App->CL_Dimensions->flag_Show_Dimensions = 0;
	}

	// Iterate through the mapping and call the functions where the flag is true
	for (const auto& guiFunction : guiFunctions)
	{
		if (guiFunction.first)
		{
			guiFunction.second();
		}
	}
}

// *************************************************************************
// *				ImGui_FPS:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui::ImGui_FPS(void)
{
	ImGui::SetNextWindowPos(ImVec2(PosX, PosY));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Ogre Data", &flag_Show_FPS, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		if (flag_StartPos == 0)
		{
			ImGui::SetWindowPos("Ogre Data", ImVec2(500, 5));
			ImGui::SetWindowSize(ImVec2(350, 90));
			flag_StartPos = 1;
		}

		ImGui::Spacing();
		ImGui::Text("FPS average %.0f", ImGui::GetIO().Framerate);

		ImVec2 Size = ImGui::GetWindowSize();
		PosX = ((float)App->CL_Ogre->mWindow->getViewport(0)->getActualWidth() / 2) - (Size.x / 2);
		PosY = 10;

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *		Press_Excape_GUI:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui::Press_Excape_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Press Excape", &flag_Show_Press_Excape, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		
		ImGui::Text("Press Excape to Exit");
		ImGui::Text("Keys WASD");

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *		Preview_Options_GUI:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui::Preview_Options_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Cam_Mode_PosX, Cam_Mode_PosY));
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
			App->CL_Physics->Reset_Physics();
			App->CL_Physics->Reset_Scene();

			App->CL_Camera->Set_Camera_Mode_First_Person();
		
			flag_Show_Preview_Options = 0;
		}

		if (ImGui::Button("Player Position", ImVec2(150, 100)))
		{
			App->CL_Camera->Set_Camera_Mode_First_Person();

			flag_Show_Preview_Options = 0;
		}

		// -------------- Return
		if (ImGui::Button("Return", ImVec2(150, 100)))
		{
			App->CL_Editor_Preview->Map_Editor_Mode();

			if (App->CL_Editor_Scene->flag_Scene_Editor_Active == 1)
			{
				App->CL_Editor_Scene->Return_From_Preview();
			}
		}

		ImVec2 Size = ImGui::GetWindowSize();
		Cam_Mode_PosX = ((float)App->CL_Ogre->mWindow->getViewport(0)->getActualWidth() / 2) - (Size.x / 2);
		Cam_Mode_PosY = ((float)App->CL_Ogre->mWindow->getViewport(0)->getActualHeight() / 2) - (Size.y / 2);

		style->Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *			App_Stats_GUI:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_ImGui::App_Stats_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("App_Stats", &flag_Show_App_Stats, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{
		if (ImGui::TreeNode("Paths"))
		{
			ImGui::Text("Path And File:= %s", App->CL_Level->MTF_PathAndFile);
			ImGui::Text("Just_Path:= %s", App->CL_Level->MTF_Just_Path);
			ImGui::Text("Just_FileName:= %s", App->CL_Level->MTF_Just_FileName);
			ImGui::Text("Just_File:= %s", App->CL_Level->MTF_JustName_NoExt);
			ImGui::Text("Working Folder:= %s", App->CL_Level->Prj_Working_Folder);
			ImGui::Separator();
			ImGui::Text("TXL Path And File:= %s", App->CL_Level->TXL_PathAndFile);
			ImGui::Text("TXL Just_File:= %s", App->CL_Level->TXL_Just_File_Name);

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Level"))
		{
			ImGui::Separator();
			ImGui::Text("Level Version:= %f", App->CL_Level->Level_Version);
			ImGui::Text("Level Folder:= %i", App->CL_Level->flag_Working_Folder_Exists);
			ImGui::Text("Been_Saved:= %i", App->CL_Level->flag_File_Been_Saved);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Project"))
		{
			ImGui::Separator();
			ImGui::Text("Project Name:= %s", App->CL_Project->m_Project_Name);
			ImGui::Text("Level Name:= %s", App->CL_Project->m_Level_Name);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Brushes"))
		{
			ImGui::Separator();
			ImGui::Text("Brush Count:= %i", App->CL_Brush->Get_Brush_Count());
			ImGui::Text("Selected Brushs Count:= %i", App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes));
			ImGui::Text("Current Brush:= %s", App->CL_Doc->CurBrush->Name);
			ImGui::Separator();
			ImGui::TreePop();
		}
	
		if (ImGui::TreeNode("Faces"))
		{
			ImGui::Separator();
			ImGui::Text("Selected Faces:= %i", App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces));
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("App Editor"))
		{
			ImGui::Separator();
			ImGui::Text("Player_Count:= %i", App->CL_Editor_Com->Player_Count);
			ImGui::Text("Player Added:= %i", App->CL_Editor_Com->flag_Player_Added);
			ImGui::Separator();
			ImGui::Text("Object_Count:= %i", App->CL_Editor_Com->Object_Count);
			ImGui::Text("Unique_Object_Count:= %i", App->CL_Editor_Com->Object_Count);
			ImGui::Text("Scene Editor Active:= %i", App->CL_Editor_Scene->flag_Scene_Editor_Active);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Physics"))
		{
			ImGui::Separator();
			ImGui::Text("Physics Running:= %i", App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Render"))
		{
			ImGui::Separator();
			ImGui::Text("Groups:= %i", App->CL_Editor_Com->GroupCount);
			ImGui::Text("Brushes:= %i", App->CL_Editor_Com->BrushCount);

			ImGui::Text("Render Groups:= %i", App->CL_Ogre->OGL_Listener->flag_Render_Groups);
			ImGui::Text("Render Brushes:= %i", App->CL_Ogre->OGL_Listener->flag_Render_Brushes);
			ImGui::Text("Render Ogre:= %i", App->CL_Ogre->OGL_Listener->flag_Render_Ogre);

			ImGui::Text("Render Mode:= %i", App->CL_Ogre->OGL_Listener->Render_Mode);
			
			ImGui::Text("Render Selected Brush:= %i", App->CL_Ogre->OGL_Listener->flag_Show_Selected_Brush);
			ImGui::Text("Render Selected Face:= %i", App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face);

			ImGui::Separator();
			ImGui::TreePop();
		}
		
		if (ImGui::Button("Close"))
		{
			App->CL_Top_Tabs->flag_Full_View_3D = 0;

			App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Top_Tabs->Copy_Spliter_Width, App->CL_Top_Tabs->Copy_Spliter_Depth);

			App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

			flag_Show_App_Stats = 0;
		}

		ImGui::End();
	}
}

// *************************************************************************
// *			Render_Report:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_ImGui::Render_Report_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Render_Report", &flag_Show_Render_Reports, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Render Reports");
		ImGui::Separator();
		ImGui::Text("Groups:= %i", App->CL_Editor_Com->GroupCount);

		ImGui::Text("Selected Group:= %i", App->CL_Picking->m_SubMesh);
		ImGui::Text("Group Face Count:= %i", App->CL_Editor_Com->Group[App->CL_Picking->m_SubMesh]->GroupFaceCount);
		ImGui::Text("Selected Face:= %i", App->CL_Picking->Local_Face);
		
		ImGui::Separator();
		ImGui::Text("WE_Brush:= %i", App->CL_Editor_Com->Group[App->CL_Picking->m_SubMesh]->Face_Data[App->CL_Picking->Local_Face].Brush_Index);
		ImGui::Text("WE_Face:= %i", App->CL_Editor_Com->Group[App->CL_Picking->m_SubMesh]->Face_Data[App->CL_Picking->Local_Face].Main_Face);

		if (ImGui::Button("Textured"))
		{
			App->CL_Camera->Camera_Textured();
		}

		if (ImGui::Button("Wired"))
		{
			App->CL_Camera->Camera_Wired();
		}

		if (ImGui::Button("Close"))
		{
			flag_Show_Render_Reports = 0;
		}

		ImGui::End();
	}
}

// *************************************************************************
// *				Paths_GUI:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_ImGui::Paths_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("App_Debug", &flag_Show_Paths, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Path And File:= %s", App->CL_Level->MTF_PathAndFile);
		ImGui::Text("Just_FileName:= %s", App->CL_Level->MTF_Just_FileName);
		ImGui::Text("Just_File:= %s", App->CL_Level->MTF_JustName_NoExt);

		ImGui::Separator();

		ImGui::Text("TXL Path And File:= %s", App->CL_Level->TXL_PathAndFile);
		ImGui::Text("TXL Just_File:= %s", App->CL_Level->TXL_Just_File_Name);

		if (ImGui::Button("Close"))
		{
			flag_Show_Paths = 0;
		}

		ImGui::End();
	}
}

// *************************************************************************
// *			Player_Data_GUI:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui::Player_Data_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Player Data", &flag_Show_Player_Data, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{
		if (flag_PlayerData_Start_Pos == 0)
		{
			ImVec2 Size = ImGui::GetWindowSize();

			//Player_Data_PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (210 / 2);
			//Player_Data_PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (310 / 2);
			//ImGui::SetWindowPos("Player Data", ImVec2(Player_Data_PosX, Player_Data_Posy));

			flag_PlayerData_Start_Pos = 1;
		}

		//ImGui::BeginDisabled(true);
		ImVec2 Size = ImGui::GetWindowSize();

		auto textWidth = ImGui::CalcTextSize("Player Data").x;

		if (ImGui::TreeNode("Collision"))
		{
			ImGui::Separator();
			ImGui::Text("Player_ID %i", App->CL_Com_Player->Col_Player_Index);
			ImGui::Text("Usage_ID %i", App->CL_Com_Player->Col_Usage_Index);
			ImGui::Text("Object_ID %i", App->CL_Com_Player->Col_Object_Index);
			ImGui::Separator();
			ImGui::TreePop();
		}

		//ImGui::Spacing();
		//ImGui::SetCursorPosX((Size.x - textWidth) * 0.5f);
		//ImGui::TextColored(ImVec4(0, 0, 1, 1), "Player Data");
		//ImGui::Separator();

		//textWidth = ImGui::CalcTextSize("Ray Cast").x;
		//ImGui::SetCursorPosX((Size.x - textWidth) * 0.5f);
		//ImGui::Text("Ray Cast");
		//ImGui::Text("Ray Cast End: %f", App->CL_Com_Player->Ray_End_Gravity);
		//ImGui::Separator();

		//textWidth = ImGui::CalcTextSize("Gravity").x;
		//ImGui::SetCursorPosX((Size.x - textWidth) * 0.5f);
		//ImGui::Text("Gravity");
		//ImGui::Text("Add Gravity: %i", App->CL_Com_Player->flag_AddGravity);
		//ImGui::Text("Is on Ground: %i", App->CL_Com_Player->flag_Is_On_Ground);
		//ImGui::Separator();

		//textWidth = ImGui::CalcTextSize("Collision").x;
		//ImGui::SetCursorPosX((Size.x - textWidth) * 0.5f);
		//ImGui::Text("Collision");

		////App->CL_LookUps->Get_Usage(App->CL_Player->Col_Usage_Index);
		////ImGui::Text("Usage %s", App->CL_LookUps->Chr_Usage);
		//ImGui::Text("Usage_ID %i", App->CL_Com_Player->Col_Usage_Index);
		//ImGui::Text("Object_ID %i", App->CL_Com_Player->Col_Object_Index);

		//ImGui::Text("Pick Object %s", App->CL_Picking->TestName);

		//ImGui::Separator();

		//ImGui::Text(" ");

		//ImGuiStyle& style = ImGui::GetStyle();

		//float size = ImGui::CalcTextSize("Close").x + style.FramePadding.x * 2.0f;
		//float avail = ImGui::GetContentRegionAvail().x;

		//float off = (avail - size) * 0.5f;
		//if (off > 0.0f)
		//{
		//	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
		//}

		if (ImGui::Button("Close"))
		{
			flag_PlayerData_Start_Pos = 0;

			flag_Show_Player_Data = 0;
			//App->Check_Menu_Camera_Data(false);

			ImGui::PopStyleColor();
			ImGui::End();
		}

		ImGui::PopStyleColor();
		ImGui::End();
	}
}
