/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
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
	flag_Show_Model_Data = 1;
	flag_Show_Demo_Options = 0;
	flag_Show_App_Debug = 0;

	// Demo 1
	flag_Show_Physics_Debug = 0;

	PosX = 500;
	PosY = 500;

	Model_Data_PosX = 0;
	Model_Data_PosY = 0;

	Float_Step = 0.50f;

	PreviouseMaterial = 0;
	PreviouseSubMesh = -1;
	PreviouseMotion = 0;
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
	flag_Show_Model_Data = 1;
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

	if (flag_Show_Model_Data == 1)
	{
		Model_Data_GUI();
	}

	if (flag_Show_Demo_Options == 1)
	{
		Demo_Options_Gui();
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
// *			Model_Data_GUI:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_ImGui::Model_Data_GUI(void)
{

	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Model Data", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Model Info");
		ImGui::Separator();

		if (ImGui::TreeNode("Paths"))
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
			
		}

		ImGui::Separator();

		if (ImGui::TreeNode("Ogre3D Model","Ogre3D Model:  %s", App->CL_Scene->S_OgreMeshData[0]->mFileName_Str.c_str()))
		{
			if (App->CL_Import_Ogre3D->flag_Ogre_Model_Loaded == 1)
			{
				ImGui::Text("User Name:- %s", App->CL_Scene->S_OgreMeshData[0]->mStrName.c_str());
				if (ImGui::TreeNode("Materials"))
				{
					int Count = 0;
					int Size = App->CL_Scene->S_OgreMeshData[0]->m_Materials_Names.size();

					while (Count < Size)
					{
						if (ImGui::Selectable(App->CL_Scene->S_OgreMeshData[0]->m_Materials_Names[Count].c_str(), listMaterialItems[Count]))
						{
							listMaterialItems[PreviouseMaterial] = 0;
							listMaterialItems[Count] = 1;
							PreviouseMaterial = Count;
						}

						Count++;
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Geometry"))
				{
					int Count = 0;
					int Size = App->CL_Scene->S_OgreMeshData[0]->mSubMeshCount;

					ImGui::PushID("foo");
					if (ImGui::BeginMenu("Status"))
					{
						ImGui::Text("Edge List:- %s", App->CL_Scene->S_OgreMeshData[0]->mStrEdgeList.c_str());
						ImGui::Text("Skeleton:- %s", App->CL_Scene->S_OgreMeshData[0]->mStrSkeleton.c_str());

						ImGui::EndMenu();
					}
					ImGui::PopID();

					while (Count < Size)
					{
						ImGui::PushID("foo");
						if (ImGui::BeginMenu(App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_SubMesh_Name_str.c_str()))
						{
							ImGui::Text("Dedicated vertices:  %s", App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_HasSharedVertices_str.c_str());
							ImGui::Text("Material Name: %s", App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_Matrial_Name_str.c_str());
							ImGui::Text("Vertices Count: %i", App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].VerticesCount);
							ImGui::Text("Bones Used: %i", App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].BonesCount);

							ImGui::Separator();

							if (ImGui::Checkbox("Show Mesh", &listSubMeshItems[Count]))
							{
								App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 1;
								App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 1;
								App->CL_Ogre->OGL_Listener->Selected_Face_Group = Count;

								listSubMeshItems[PreviouseSubMesh] = 0;
								PreviouseSubMesh = Count;
							}

							ImGui::EndMenu();
						}

						ImGui::PopID();

						Count++;
					}

					if (listSubMeshItems[PreviouseSubMesh] == 0 && App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces == 1)
					{
						App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 0;
						App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 0;
						PreviouseSubMesh = -1;
					}

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

				if (ImGui::TreeNode("Motions"))
				{
					int Count = 0;
					int Size = App->CL_Scene->MotionCount;

					while (Count < Size)
					{
						if (ImGui::Selectable(App->CL_Scene->S_OgreMeshData[0]->m_Motion_Names[Count].c_str(), listMotionItems[Count]))
						{
							App->CL_TopDlg->Switch_To_Motions_Dlg();
							App->CL_TopDlg->Update_Motions_By_Name(App->CL_Scene->S_OgreMeshData[0]->m_Motion_Names[Count].c_str());

							listMotionItems[PreviouseMotion] = 0;
							listMotionItems[Count] = 1;
							PreviouseMotion = Count;
						}

						Count++;

					}

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

		ImGui::Separator();

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

		ImGui::Separator();
		
		if (ImGui::Button("Close"))
		{
			App->CL_TopDlg->Enable_Info_Icon(false);
			flag_Show_Model_Data = 0;
		}

		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouseUnlessPopupClose == 0)
		{
			ImGui::SetWindowFocus(nullptr);
		}

		ImGui::End();
	}
}

// *************************************************************************
// *			Demo_Options_Gui:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_ImGui::Demo_Options_Gui(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(280, 250), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Demo_1", &flag_Show_Demo_Options, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		// -------------------------------------- Title Bar
		ImGui::Text("Options");
		ImGui::SameLine(0, 180);
		if (ImGui::Button("X"))
		{
			flag_Show_Demo_Options = 0;
			RedrawWindow(App->CL_TopDlg->Demos_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		ImGui::Separator();
		// ---------------------------------------------

		ImGui::Text("Keys WASD");
		ImGui::Spacing();

		if (ImGui::Checkbox("Show Fog", &App->CL_Ogre->flag_Show_Fog))
		{
			App->CL_Ogre->Enable_Fog(App->CL_Ogre->flag_Show_Fog);
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

		ImGui::Text("");

		ImVec2 Size = ImGui::GetWindowSize();
		auto textWidth = ImGui::CalcTextSize("Reset Scene").x;
		ImGui::SetCursorPosX((Size.x - textWidth) * 0.5f);
		if (ImGui::Button("Reset Scene"))
		{
			App->CL_Demos->Reset_View();
		}

		Model_Data_PosX = 10;
		Model_Data_PosY = 10;

		ImGui::End();
	}
}

// *************************************************************************
// *			App_Debug:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_ImGui::App_Debug(void)
{
	ImGui::SetNextWindowPos(ImVec2(10,10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(280, 500), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("App_Debug", &flag_Show_App_Debug, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		
		if (ImGui::Checkbox("Debug_Textures", &App->flag_Debug_Textures))
		{

		}

		ImGui::Text("Debug Textures: %i", App->flag_Debug_Textures);
		ImGui::Text("TestMesh_Active: %i", App->CL_Ogre->flag_Test_Mesh_Active);
		ImGui::Text("Ogre_Model_Loaded: %i", App->CL_Ogre->Ogre3D_Listener->Ogre_Model_Loaded);
		ImGui::Text("Ogre_Resources_Loaded: %i", App->CL_Resources->Ogre_ExternalResourceLoaded);
		
		ImGui::Text(" ");

		ImGui::Text("Physics Running: %i", App->CL_Ogre->Ogre3D_Listener->Run_Physics);
		ImGui::Text("Fog Enabled: %i", App->CL_Ogre->flag_Show_Fog);
		ImGui::Text("Camera Mode: %i", App->CL_Ogre->Ogre3D_Listener->CameraMode);
		ImGui::Text(" ");
		ImGui::Text("TriMesh_Created: %i", App->CL_Bullet->flag_TriMesh_Created);
		ImGui::Text("Colision Objects: %i", App->CL_Bullet->dynamicsWorld->getNumCollisionObjects());
		
		if (ImGui::Button("Close"))
		{
			flag_Show_App_Debug = 0;
		}

		ImGui::End();
	}
}
