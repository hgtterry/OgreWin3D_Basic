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

	flag_Show_FPS = 1;
	flag_StartPos = 0;

	flag_Show_Tool_ID_Debug = 0;
	flag_Show_Paths = 0;
	flag_Show_Render_Reports = 0;
	flag_Show_Press_Excape = 0;
	flag_Show_Camera_Mode = 0;
	flag_Show_App_Stats = 0;

	guiFunctions.reserve(20);
}

CL64_ImGui::~CL64_ImGui()
{
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
		{flag_Show_Tool_ID_Debug == 1, [&]() { App_Tool_Selection_GUI(); }},
		{flag_Show_Paths == 1, [&]() { Paths_GUI(); }},
		{flag_Show_Render_Reports == 1, [&]() { Render_Report_GUI(); }},
		{flag_Show_Press_Excape == 1, [&]() { Press_Excape_GUI(); }},
		{flag_Show_Camera_Mode == 1, [&]() { Camera_Mode_GUI(); }},
		{App->CL_Gui_Environment->flag_Show_PropertyEditor == 1, [&]() { App->CL_Gui_Environment->Environ_PropertyEditor(); }},
		{flag_Show_App_Stats == 1, [&]() { App_Stats_GUI(); }}
	};

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

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *			Camera_Mode_GUI:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui::Camera_Mode_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Cam_Mode_PosX, Cam_Mode_PosY));
	ImGui::SetNextWindowSize(ImVec2(165, 300), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));
	ImGuiStyle* style = &ImGui::GetStyle();

	if (!ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		// -------------- First Person
		if (App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_First)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}
		else
		{
			style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		}

		if (ImGui::Button("First Person", ImVec2(150, 30)))
		{
			App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;
		}

		// -------------- Camera Free
		if (App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_Free)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}
		else
		{
			style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		}

		if (ImGui::Button("Free", ImVec2(150, 30)))
		{
			App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
		}

		// -------------- Physics On
		if (App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics == 1)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}
		else
		{
			style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Physics On", ImVec2(150, 30)))
		{
			/*if (App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics == 1)
			{
				App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;
			}
			else
			{
				App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;
			}*/
		}

		// -------------- Physics Debug
		if (App->CL_Editor_Com->flag_Show_Debug_Area == 1)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}
		else
		{
			style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		}

		if (ImGui::Button("Physics Debug", ImVec2(150, 30)))
		{
			if (App->CL_Editor_Com->flag_Show_Debug_Area == 1)
			{
				App->CL_Physics->Show_Debug_Area(false);
				App->CL_Editor_Com->flag_Show_Debug_Area = 0;
			}
			else
			{
				App->CL_Physics->Show_Debug_Area(true);
				App->CL_Editor_Com->flag_Show_Debug_Area = 1;
			}
		}

		// -------------- Reset Scene
		style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 0.5f, 1.00f);

		if (ImGui::Button("Reset Scene", ImVec2(150, 30)))
		{
			App->CL_Physics->Reset_Scene();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		style->Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		if (ImGui::Button("Exit-Return", ImVec2(150, 30)))
		{
			App->CL_Editor_Com->Editor_Mode();
		}

		ImVec2 Size = ImGui::GetWindowSize();
		Cam_Mode_PosX = ((float)App->CL_Ogre->mWindow->getViewport(0)->getActualWidth()) - (Size.x) - 10;
		Cam_Mode_PosY = 10;

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *		App_Tool_Selection_GUI:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_ImGui::App_Tool_Selection_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("App_Debug", &flag_Show_Tool_ID_Debug, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{
		char Tool[MAX_PATH];
		if (App->CL_Doc->mModeTool == 32886)
		{
			strcpy(Tool, "GENERAL_SELECT");
		}

		if (App->CL_Doc->mModeTool == 32784)
		{
			strcpy(Tool, "MOVE_ROTATE");
		}

		if (App->CL_Doc->mModeTool == 32785)
		{
			strcpy(Tool, "SCALE");
		}

		if (App->CL_Doc->mModeTool == 32910)
		{
			strcpy(Tool, "TEMPLATE");
		}

		ImGui::Text("Width:= %i", App->CL_Editor_Map->nleftWnd_width);
		ImGui::Text("Depth:= %i", App->CL_Editor_Map->nleftWnd_Depth);

		ImGui::Text("mModeTool:= %s", Tool);
		ImGui::Separator();
		ImGui::Text("Selected Brushs:= %i", App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes));
		ImGui::Text("Current Brush:= %s", App->CL_Doc->CurBrush->Name);
		//ImGui::Text("Current Brush Faces:= %i", App->CL_Brush->Brush_GetNumFaces(App->CL_Doc->CurBrush));
		
		ImGui::Separator();

		/*ImGui::Text("Selected Faces:= %i", App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces));
		ImGui::Text("Current Face:= %i", App->CL_Face->Selected_Face_Index);
		ImGui::Text("Texture Count:= %i", App->CL_TXL_Editor->Texture_Count);
		ImGui::Text("Processes Time:= %i %s", App->CL_Ogre->m_Processes_Time," ms");
		
		ImGui::Text("Picking");
		ImGui::Text("Picked:= %i ", App->CL_Picking->flag_Selected_Ok);
		ImGui::Text("Mesh Name:= %s ", App->CL_Picking->Pl_Entity_Name);
		ImGui::Text("Texture Name:= %s ", App->CL_Picking->TextureName);
		ImGui::Text("Ogre Model Faces:= %i ", App->CL_Ogre3D->m_Total_Faces);
		ImGui::Text("Ogre Faces:= %i ", App->CL_Mesh_Mgr->ActualFaceCount);
		ImGui::Text("Brushes3 Faces:= %i ", App->CL_Mesh_Mgr->Global_Faces_Index);
		ImGui::Text("Brushes Faces:= %i ", App->CL_Mesh_Mgr->m_Total_Faces);
		ImGui::Text("V_Face Count:= %i ", App->CL_Mesh_Mgr->v_Face_Data_Count);*/
		

		ImGui::Text("Texture:= %s ", App->CL_Picking->m_Texture_FileName);
		ImGui::Text("Group Index:= %i ", App->CL_Picking->Group_Index);
		ImGui::Text("Brush Index:= %i ", App->CL_Picking->Brush_Index);
		//ImGui::Text("Face Index:= %i ", App->CL_Picking->Face_Index);
		ImGui::Text("Local Face:= %i ", App->CL_Picking->Local_Face);
		ImGui::Text("WE Face:= %i ", App->CL_Picking->m_Main_Face);
		//ImGui::Text("Brush Hit:= %s ", App->CL_Mesh_Mgr->v_Face_Data[App->CL_Picking->Face_Hit]->Brush_Name);
		
		if (ImGui::Button("Close"))
		{
			flag_Show_Tool_ID_Debug = 0;
		}

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
			ImGui::Text("Path And File:= %s", App->CL_Doc->mDoc_MTF_PathAndFile);
			ImGui::Text("Just_FileName:= %s", App->CL_Doc->mDoc_MTF_Just_FileName);
			ImGui::Text("Just_File:= %s", App->CL_Doc->mDoc_MTF_JustName_NoExt);
			ImGui::Separator();
			ImGui::Text("TXL Path And File:= %s", App->CL_Doc->mDoc_TXL_Path_And_File);
			ImGui::Text("TXL Just_File:= %s", App->CL_Doc->mDoc_TXL_Just_FileName);
			ImGui::Text("File Wad Path:= %s", App->CL_File->WadPathFile);
			ImGui::Text("Level Wad_File:= %s", App->CL_Level->Level_GetWadPath(App->CL_Doc->Current_Level));

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
		ImGui::Text("Path And File:= %s", App->CL_Doc->mDoc_MTF_PathAndFile);
		ImGui::Text("Just_FileName:= %s", App->CL_Doc->mDoc_MTF_Just_FileName);
		ImGui::Text("Just_File:= %s", App->CL_Doc->mDoc_MTF_JustName_NoExt);

		ImGui::Separator();

		ImGui::Text("TXL Path And File:= %s", App->CL_Doc->mDoc_TXL_Path_And_File);
		ImGui::Text("TXL Just_File:= %s", App->CL_Doc->mDoc_TXL_Just_FileName);

		if (ImGui::Button("Close"))
		{
			flag_Show_Paths = 0;
		}

		ImGui::End();
	}
}
