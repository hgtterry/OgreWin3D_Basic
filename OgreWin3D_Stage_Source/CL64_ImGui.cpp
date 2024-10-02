/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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

CL64_ImGui::CL64_ImGui(void)
{
	flag_Imgui_Initialized = 0;

	Camera_Data_PosX = 10;
	Camera_Data_Posy = 100;

	StartPos = 0;
	flag_Show_FPS = 1;
	flag_Show_ImGui_Demo = 0;
	flag_Show_Camera_Data = 0;
	flag_Show_Demo_Options = 0;
	flag_Show_App_Debug = 0;
	flag_Show_Ogre_Data = 0;
	flag_Open_Textures_List = 1;
	Show_Collision_Debug = 0;

	// Demo 1
	flag_Show_Physics_Debug = 0;

	font0 = 0;
	font1 = 0;
	font2 = 0;
	font3 = 0;
	fontDroid = 0;

	PosX = 500;
	PosY = 500;

	Model_Data_PosX = 0;
	Model_Data_PosY = 0;

	Float_Step = 0.50f;

}

CL64_ImGui::~CL64_ImGui(void)
{
	
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_ImGui::Reset_Class(void)
{
	flag_Show_Demo_Options = 0;
	flag_Show_Ogre_Data = 0;
	Show_Collision_Debug = 0;

}

// *************************************************************************
// *			Init_ImGui:- Terry and Hazel Flanigan 2024				   *
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
// *	  	ImGui_Set_Colours:- Terry and Hazel Flanigan 2024			   *
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
}

// *************************************************************************
// *			Load_Font:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_ImGui::Load_Font(void)
{
	ImGuiIO& io = ImGui::GetIO();
	font1 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 20);
	font2 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 48);
	font0 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 18);
	fontDroid = io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 12);

	//font2 = io.Fonts->AddFontDefault().;

	io.IniFilename = NULL;
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

	if (flag_Show_ImGui_Demo == 1)
	{
		ImGui::ShowDemoWindow();
	}

	if (flag_Show_Camera_Data == 1)
	{
		Camera_Data_GUI();
	}

	if (flag_Show_Demo_Options == 1)
	{
		Demo_Options_Gui();
	}

	if (flag_Show_App_Debug == 1)
	{
		App_Debug();
	}

	if (Show_Collision_Debug == 1)
	{
		ImGui_Collision_Debug();
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
		if (StartPos == 0)
		{
			ImGui::SetWindowPos("Ogre Data", ImVec2(500, 5));
			ImGui::SetWindowSize(ImVec2(350, 90));
			StartPos = 1;
		}

		ImGui::Spacing();
		ImGui::Text("FPS average %.0f", ImGui::GetIO().Framerate);

		ImVec2 Size = ImGui::GetWindowSize();
		PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (Size.x / 2);
		PosY = 10;

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *			Camera_Data_GUI:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui::Camera_Data_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Camera_Data_PosX, Camera_Data_Posy));

	if (!ImGui::Begin("Camera Data", &flag_Show_Camera_Data, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImVec2 Size = ImGui::GetWindowSize();

		auto textWidth = ImGui::CalcTextSize("Camera Data").x;

		float Yaw = App->CL_Ogre->Ogre3D_Listener->mCamNode->getOrientation().getYaw().valueDegrees();
		float Pitch = App->CL_Ogre->Ogre3D_Listener->mCamNode->getOrientation().getPitch().valueDegrees();
		float Roll = App->CL_Ogre->Ogre3D_Listener->mCamNode->getOrientation().getRoll().valueDegrees();

		float X = App->CL_Ogre->Ogre3D_Listener->mCamNode->getPosition().x;
		float Y = App->CL_Ogre->Ogre3D_Listener->mCamNode->getPosition().y;
		float Z = App->CL_Ogre->Ogre3D_Listener->mCamNode->getPosition().z;

		ImGui::Spacing();
		ImGui::SetCursorPosX((Size.x - textWidth) * 0.5f);
		ImGui::TextColored(ImVec4(0,0,1,1),"Camera Data");
		
		textWidth = ImGui::CalcTextSize("Position").x;
		ImGui::SetCursorPosX((Size.x - textWidth) * 0.5f);
		ImGui::Text("Position");
		ImGui::Text("X: %f Y: %f Z: %f",X,Y,Z);

		ImGui::Separator();

		textWidth = ImGui::CalcTextSize("Rotation").x;
		ImGui::SetCursorPosX((Size.x - textWidth) * 0.5f);
		ImGui::Text("Rotation");
		ImGui::Text("X: %f Y: %f Z: %f", Yaw, Pitch, Roll);

		Camera_Data_Posy = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height) - (Size.y)-10;

		ImGui::End();
	}
}

// *************************************************************************
// *			Demo_Options_Gui:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_ImGui::Demo_Options_Gui(void)
{
	ImGui::SetNextWindowPos(ImVec2(220, 80), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 70), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Control", &flag_Show_Demo_Options, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Keys WASD And Mouse Left Button");
		
		ImVec2 Size = ImGui::GetWindowSize();
		auto textWidth = ImGui::CalcTextSize("Reset Scene").x;
		
		Model_Data_PosX = 10;
		Model_Data_PosY = 10;

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *			App_Debug:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_ImGui::App_Debug(void)
{
	ImGui::SetNextWindowPos(ImVec2(10,10), ImGuiCond_FirstUseEver);
	
	if (!ImGui::Begin("App_Debug", &flag_Show_App_Debug, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Always);
		if (ImGui::TreeNode("Debug"))
		{

			// File Names
			if (ImGui::TreeNode("File Names"))
			{
				ImGui::Separator();
				ImGui::Text("Project File Name: %s", App->CL_Project->Project_File_Name);
				ImGui::Text("Project Path and File: %s", App->CL_Project->Project_Path_File_Name);
				ImGui::Text("Length: %i", strlen(App->CL_Project->Project_File_Name));
				/*ImGui::Text("Scene Just Name: %s", App->CL_Scene->JustName);
				ImGui::Text("Scene File Name: %s", App->CL_Scene->FileName);
				ImGui::Text("File and Path: %s", App->CL_Scene->Path_FileName);
				ImGui::Text("Model Folder: %s", App->CL_Scene->Model_FolderPath);
				ImGui::Text("Texture Folder: %s", App->CL_Scene->Texture_FolderPath);
				ImGui::Text("Resources File: %s", App->CL_Resources->Resource_File_FileName);
				ImGui::Text("Resources File and Path: %s", App->CL_Resources->Resource_File_Path_And_File);*/
				ImGui::Separator();
				ImGui::TreePop();
			}

			// Counters and Flags
			if (ImGui::TreeNode("Counters and Flags"))
			{
				ImGui::Separator();
				ImGui::Text("Counters ---------------------");
				ImGui::Text("Object Count: %i", App->CL_Scene->Object_Count);
				ImGui::Text("Aera Count: %i", App->CL_Scene->Area_Count);
				ImGui::Text("Camera Count: %i", App->CL_Scene->Camera_Count);
				ImGui::Text("Player Location Count: %i", App->CL_Scene->Player_Location_Count);
				ImGui::Text("Counters Count: %i", App->CL_Scene->Counters_Count);

				ImGui::Text("UniqueID Object Counter: %i", App->CL_Scene->UniqueID_Object_Counter);
				ImGui::Text("UniqueID Area Count: %i", App->CL_Scene->UniqueID_Area_Count);
				ImGui::Text("UniqueID Counters Count: %i", App->CL_Scene->UniqueID_Counters_Count);
				ImGui::Text("Flags ------------------------");
				ImGui::Text("Scene Loaded: %i", App->CL_Scene->flag_Scene_Loaded);
				ImGui::Text("Player Added: %i", App->CL_Scene->flag_Player_Added);
				ImGui::Text("Scene Modified: %i", App->CL_Scene->flag_Scene_Modified);
				
				ImGui::Separator();
				ImGui::TreePop();

			}

			//// Motions
			//if (ImGui::TreeNode("Motions"))
			//{
			//	ImGui::TreePop();
			//}
		}
		
		if (ImGui::Button("Close"))
		{
			flag_Show_App_Debug = 0;
			App->CL_FileView->Show_FileView(true);

		}

		ImGui::End();
	}
}

// *************************************************************************
// *		ImGui_Collision_Debug:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_ImGui::ImGui_Collision_Debug(void)
{
	ImGui::SetNextWindowPos(ImVec2(530, 50), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Collisions_Debug", &Show_Collision_Debug, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{

		ImGui::Spacing();

		ImGui::Text("Manifolds %i", App->CL_Player->Col_numManifolds);
		ImGui::Text("PlayerIndex %i", App->CL_Player->Col_Player_Index);
		ImGui::Text("Object_ID %i", App->CL_Player->Col_Object_Index);
		ImGui::Text("Usage %i", App->CL_Player->Col_Usage_Index);

		ImGui::Text(" ");

		ImGui::Text("Round %i", App->CL_Player->Round);
		ImGui::Text("Distance %i", App->CL_Player->Distance);
		ImGui::Text("Life_Time %i", App->CL_Player->Life_Time);

		if (ImGui::Button("Close"))
		{
			Show_Collision_Debug = 0;
		}

		ImGui::End();
	}
}

/*if (ImGui::TreeNode("Paths"))
		{
			ImGui::PushID("foo");
			if (ImGui::BeginMenu("Model Name"))
			{
				ImGui::Text("%s", App->CL_Scene->JustName);
				ImGui::EndMenu();
			}
			ImGui::PopID();

			ImGui::PushID("foo");
			if (ImGui::BeginMenu("Model File"))
			{
				ImGui::Text("%s", App->CL_Scene->FileName);
				ImGui::EndMenu();
			}
			ImGui::PopID();

			ImGui::PushID("foo");
			if (ImGui::BeginMenu("Model Path"))
			{
				ImGui::Text("%s", App->CL_Scene->Path_FileName);
				ImGui::EndMenu();
			}
			ImGui::PopID();

			ImGui::PushID("foo");
			if (ImGui::BeginMenu("Texture Path"))
			{
				ImGui::Text("%s", App->CL_Scene->Texture_FolderPath);
				ImGui::EndMenu();
			}
			ImGui::PopID();

			ImGui::TreePop();

		}*/

		/*if (ImGui::TreeNode("Mesh Data"))
				{
					ImGui::Indent();
					ImGui::Text("Vertices:- %i", App->CL_Scene->VerticeCount);
					ImGui::Text("Faces:- %i", App->CL_Scene->FaceCount);
					ImGui::Text("Groups:- %i", App->CL_Scene->GroupCount);
					ImGui::Text("Bones:- %i", App->CL_Scene->BoneCount);
					ImGui::Text("Motions:- %i", App->CL_Scene->MotionCount);
					ImGui::Text("Loaded:- %i", App->CL_Scene->flag_Model_Loaded);
					ImGui::Unindent();

					ImGui::TreePop();
				}*/
