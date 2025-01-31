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

	flag_Show_FPS = 1;
	flag_StartPos = 0;

	flag_Show_Tool_ID_Debug = 0;
	flag_Show_Paths = 0;
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
	if (flag_Show_FPS == 1)
	{
		ImGui_FPS();
	}

	if (flag_Show_Tool_ID_Debug == 1)
	{
		App_Tool_Selection_GUI();
	}

	if (flag_Show_Paths == 1)
	{
		Paths_GUI();
	}
	
	/*if (flag_Show_ImGui_Demo == 1)
	{
		ImGui::ShowDemoWindow();
	}*/
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
		PosX = 20;// ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (Size.x / 2);
		PosY = 10;

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

		ImGui::Text("mModeTool:= %s", Tool);
		ImGui::Separator();
		ImGui::Text("Selected Brushs:= %i", App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes));
		ImGui::Text("Current Brush:= %s", App->CL_Doc->CurBrush->Name);

		ImGui::Separator();

		ImGui::Text("Selected Faces:= %i", App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces));
		
		
		//ImGui::Text("Cam_Pos_X:= %f", App->CL_MapEditor->Current_View->CamPos.x);
		
		if (ImGui::Button("Close"))
		{
			flag_Show_Tool_ID_Debug = 0;
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
