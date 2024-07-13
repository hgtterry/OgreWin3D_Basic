/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_ImGui.h"

CL64_ImGui::CL64_ImGui(void)
{
	Imgui_Initialized = 0;

	Camera_Data_PosX = 10;
	Camera_Data_Posy = 100;

	StartPos = 0;
	Show_FPS = 1;
	Show_ImGui_Demo = 0;
	Show_Camera_Data_F = 0;
	Show_Model_Data_F = 0;
	Show_Demo_1_F = 0;
	flag_Show_App_Debug = 0;

	// Demo 1
	Show_Physics_Debug_F = 0;

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

			Imgui_Initialized = 1;
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
	ImFont* font0;
	ImFont* font1;
	ImFont* font2;
	ImFont* fontDroid;

	ImGuiIO& io = ImGui::GetIO();
	font1 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 20);
	font2 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 48);
	font0 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 18);
	fontDroid = io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 12);

	//font2 = io.Fonts->AddFontDefault().;

	//io.IniFilename = NULL;
}

// **************************************************************************
// *			Render_FPS:- Terry and Hazel Flanigan 2024					*
// **************************************************************************
void CL64_ImGui::ImGui_Render_Loop(void)
{
	if (Show_FPS == 1)
	{
		ImGui_FPS();
	}

	if (Show_ImGui_Demo == 1)
	{
		ImGui::ShowDemoWindow();
	}

	if (Show_Camera_Data_F == 1)
	{
		Camera_Data_GUI();
	}

	if (Show_Model_Data_F == 1)
	{
		Model_Data_GUI();
	}

	if (Show_Demo_1_F == 1)
	{
		Demo_1_GUI();
	}

	if (flag_Show_App_Debug == 1)
	{
		App_Debug();
	}
}

// *************************************************************************
// *				ImGui_FPS:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui::ImGui_FPS(void)
{
	ImGui::SetNextWindowPos(ImVec2(PosX, PosY));

	if (!ImGui::Begin("Ogre Data", &Show_FPS, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
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

		ImGui::End();
	}
}

// *************************************************************************
// *			Camera_Data_GUI:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui::Camera_Data_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Camera_Data_PosX, Camera_Data_Posy));

	if (!ImGui::Begin("Camera Data", &Show_Camera_Data_F, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImVec2 Size = ImGui::GetWindowSize();

		auto windowHeight = ImGui::GetWindowSize().y;
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
// *			Model_Data_GUI:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_ImGui::Model_Data_GUI(void)
{
	//ImGui::SetNextWindowPos(ImVec2(Model_Data_PosX, Model_Data_PosY));
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Model Data", &Show_Model_Data_F, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Model Info");
		if (ImGui::TreeNode("Paths"))
		{
			ImGui::Text("Model Name:- %s", App->CL_Scene->JustName);
			ImGui::Text("Model File:- %s", App->CL_Scene->FileName);
			ImGui::Text("Model Path:- %s", App->CL_Scene->Path_FileName);
			ImGui::Text("Texture Path:- %s", App->CL_Scene->Texture_FolderPath);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Ogre3D Model"))
		{
			ImGui::Text("Name:- %s",App->CL_Scene->S_OgreMeshData[0]->mName.c_str());
			if (ImGui::TreeNode("Materials"))
			{
				int Count = 0;
				int Size = App->CL_Scene->S_OgreMeshData[0]->mMaterials.size();

				while (Count < Size)
				{
					ImGui::Text(App->CL_Scene->S_OgreMeshData[0]->mMaterials[Count].c_str());
					Count++;
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Geometry"))
			{
				ImGui::Text("Edge List:- %s", App->CL_Scene->S_OgreMeshData[0]->mEdgeList.c_str());
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Bounds"))
			{
				ImGui::Text("Min:- %.5f  %.5f  %.5f", App->CL_Scene->S_OgreMeshData[0]->vMin.x, App->CL_Scene->S_OgreMeshData[0]->vMin.y, App->CL_Scene->S_OgreMeshData[0]->vMin.z);
				ImGui::Text("Max:- %.5f  %.5f  %.5f", App->CL_Scene->S_OgreMeshData[0]->vMax.x, App->CL_Scene->S_OgreMeshData[0]->vMax.y, App->CL_Scene->S_OgreMeshData[0]->vMax.z);
				ImGui::Text("Centre:- %.5f  %.5f  %.5f", App->CL_Scene->S_OgreMeshData[0]->Center.x, App->CL_Scene->S_OgreMeshData[0]->Center.y, App->CL_Scene->S_OgreMeshData[0]->Center.z);

				ImGui::Text("Width:- %f", App->CL_Scene->S_OgreMeshData[0]->Width);
				ImGui::Text("Height:- %f", App->CL_Scene->S_OgreMeshData[0]->Height);
				ImGui::Text("Depth:- %f", App->CL_Scene->S_OgreMeshData[0]->Depth);
				ImGui::Text("Area:- %f", App->CL_Scene->S_OgreMeshData[0]->Area);
				ImGui::Text("Volume:- %f", App->CL_Scene->S_OgreMeshData[0]->Volume);
				ImGui::Text("Radius:- %f", App->CL_Scene->S_OgreMeshData[0]->Radius);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Mesh Data"))
		{
			ImGui::Indent();
			ImGui::Text("Vertices:- %i", App->CL_Scene->VerticeCount);
			ImGui::Text("Faces:- %i", App->CL_Scene->FaceCount);
			ImGui::Text("Groups:- %i", App->CL_Scene->GroupCount);
			ImGui::Text("Bones:- %i", App->CL_Scene->BoneCount);
			ImGui::Text("Motions:- %i", App->CL_Scene->MotionCount);
			ImGui::Text("Loaded:- %i", App->CL_Resources->Ogre_ExternalResourceLoaded);
			ImGui::Unindent();

			ImGui::TreePop();
		}

		ImGui::Text("  ");
		ImGui::Text("  ");
		
		ImVec2 Size = ImGui::GetWindowSize();
		//Model_Data_PosX = ((float)App->CL_Ogre->OgreListener->View_Width / 2) - (Size.x / 2);
		//Model_Data_PosY = ((float)App->CL_Ogre->OgreListener->View_Height / 2) - (Size.y / 2);

		ImGui::Separator();

		if (ImGui::Button("Close"))
		{
			Show_Model_Data_F = 0;
		}

		ImGui::End();
	}
}

// *************************************************************************
// *			Demo_1_GUI:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_ImGui::Demo_1_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Model_Data_PosX, Model_Data_PosY));
	ImGui::SetNextWindowSize(ImVec2(280, 300), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Demo_1", &Show_Demo_1_F, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Keys WASD");
		ImGui::Spacing();

		if (ImGui::Checkbox("Show Debug Physics", &App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node))
		{

			if (App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node == 1)
			{
				App->CL_Ogre->Bullet_Debug_Listener->btDebug_Node->setVisible(true);
			}
			else
			{
				App->CL_Ogre->Bullet_Debug_Listener->btDebug_Node->setVisible(false);
			}
		}

		ImGui::PushItemWidth(120);
		ImGui::InputFloat("", &App->CL_Scene->B_Player[0]->Ground_speed, Float_Step, 0, "%.1f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text("Ground Speed");
		ImGui::Spacing();
		
		ImGui::Text("Pitch:- %f", App->CL_Scene->B_Player[0]->CameraPitch_Node->getOrientation().getPitch().valueDegrees());
		ImGui::Text("Yaw:- %f", App->CL_Ogre->camNode->getOrientation().getYaw().valueDegrees());
		ImGui::Separator();

		if (ImGui::Button("Reset View"))
		{
			App->CL_Demos->Reset_View();
		}


		if (ImGui::Button("1st view"))
		{
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;
		}

		ImGui::SameLine();

		if (ImGui::Button("Free view"))
		{
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
		}

		Model_Data_PosX = 10;
		Model_Data_PosY = 10;

		/*if (ImGui::Button("Close"))
		{
			Show_Demo_1_F = 0;
		}*/

		ImGui::End();
	}
}

// *************************************************************************
// *			App_Debug:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_ImGui::App_Debug(void)
{
	ImGui::SetNextWindowPos(ImVec2(10,10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(280, 300), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("App_Debug", &flag_Show_App_Debug, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		
		if (ImGui::Checkbox("Debug_Textures", &App->flag_Debug_Textures))
		{

		}

		ImGui::Text("Debug Textures:- %i", App->flag_Debug_Textures);
		ImGui::Text("TestMesh_Deleted:- %i", App->CL_Ogre->flag_TestMesh_Deleted);

		if (ImGui::Button("Close"))
		{
			flag_Show_App_Debug = 0;
		}

		ImGui::End();
	}
}
