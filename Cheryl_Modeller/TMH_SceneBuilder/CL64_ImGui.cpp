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
#include "CL64_ImGui.h"

CL64_ImGui::CL64_ImGui()
{
	font0 = NULL;
	font1 = NULL;
	font2 = NULL;
	font3 = NULL;
	fontDroid = NULL;

	CB_Index = 3;

	flag_Imgui_Initialized = false;

	PosX = 500;
	PosY = 500;

	Cam_Mode_PosX = 10;
	Cam_Mode_PosY = 10;

	flag_PlayerData_Start_Pos = 0;

	flag_Show_FPS = true;

	flag_Show_Paths = false;
	flag_Show_Render_Reports = false;
	flag_Show_Press_Excape = false;
	flag_Show_App_Stats = false;
	flag_Show_Listbox = false;
	flag_Show_Model_Data = true;

	PreviouseSubMesh = -1;

	listMaterialItems_Ogre[0] = true;
	PreviouseMaterial_Ogre = 0;

	guiFunctions.reserve(20);

	Base = nullptr;
	Win = nullptr;
}

CL64_ImGui::~CL64_ImGui()
{
}

// *************************************************************************
// *			Close_Dialogs:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_ImGui::Close_Dialogs(void)
{
	
}

#include "imgui.h"
//struct ImGuiContext;
//extern thread_local ImGuiContext* ImGuiTLS;
//#define GImGui ImGuiTLS

// *************************************************************************
// *			Init_ImGui:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_ImGui::Init_ImGui(void)
{
	//ImGuiContext* main_context;
	//ImGuiContext* secondary_context;

	bool mOwnsImGuiOverlay = !Ogre::OverlayManager::getSingleton().getByName("ImGuiOverlay");
	
	Base = new OgreBites::ApplicationContextBase();
	
	Ogre::ImGuiOverlay* imguiOverlay = Base->initialiseImGui();
	//Ogre::RenderWindow* Win = Base->getRenderWindow();
	
	//Win->
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

			flag_Imgui_Initialized = true;
		}
		else
		{
			App->Say("Could Not Initialised Imgui");
		}

		//ImGuiIO& ImGuiInitialize();
		ImGuiContext* defaultContext = ImGui::GetCurrentContext();
		//defaultContext = "poo"
		ImGui::SetCurrentContext(defaultContext);
		//OgreBites::OgreImGui::Init
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

	
	//io.ConfigFlags
	//io.ConfigFlags |= ImGuiIO::ImGuiConfigFlags_ViewportsEnable;

	/*struct ImGui::Data
	{
		VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
		VkRenderPass render_pass_ = VK_NULL_HANDLE;*/

		//ImGuiContext* context = nullptr; // One for each windwo
		//ImGuiIO* io_ = nullptr; // Seems to be global

	/*	GLFWwindow* window_ = nullptr;

		std::weak_ptr<gpu::Device> device_ = {};
	};*/

	//context = ImGui::CreateContext();
	//context
	////ImGui::CreateContext();
	//ImGui::SetCurrentContext(context);
	//ImGui::SetCurrentContext(context_);

	io.IniFilename = NULL;
}

// *************************************************************************
// *	  	ImGui_Set_Colours:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_ImGui::ImGui_Set_Colours(void)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;
	

	//ImGui::()
	//ImGuiConfig*  
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
	//colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style->HoverDelayNormal = 1;
}

// **************************************************************************
// *			ImGui_Render_Loop:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_ImGui::ImGui_Render_Loop(void)
{
	guiFunctions =
	{
		{flag_Show_FPS == 1, [&]() { ImGui_FPS(); }},
		{flag_Show_Paths == 1, [&]() { Paths_GUI(); }},
		{flag_Show_Render_Reports == 1, [&]() { Render_Report_GUI(); }},
		{flag_Show_Press_Excape == 1, [&]() { Press_Excape_GUI(); }},
		{flag_Show_App_Stats == 1, [&]() { App_Stats_GUI(); }}
	};

	if (flag_Show_Model_Data == 1)
	{
		Model_Data_GUI();
	}

	// SBC_Gui_Dialogs
	if (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == true)
	{
		App->CL_ImGui_Dialogs->Dialog_Float();
	}

	if (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 == true)
	{
		App->CL_ImGui_Dialogs->Dialog_Float_Vec3();
	}

	if (flag_Show_Listbox == true)
	{
		Debug_Lists_ImGui();
	}

	if (App->CL_ImGui_Dialogs->flag_Show_Dialog_list == 1)
	{
		App->CL_ImGui_Dialogs->Dialog_List_Gui();
	}

	// SBC_Dimensions
	if (App->CL_Dimensions->flag_Show_Dimensions == true)
	{
		App->CL_Dimensions->ImGui_Dimensions();
	}
	
	App->CL_ImGui_Editor->ImGui_Render_Editor_Loop();
	
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
		ImGui::Text("Model");

		ImGui::Separator();

		//if (App->CL_Model->Model_Type == Enums::Model_Type_None)
		{
			Show_Ogre_Model_Data_GUI();
		}
		//else
		{
			//Show_Assimp_Model_Data_GUI();
		}

		ImGui::Separator();

		if (ImGui::Button("Close"))
		{
			//App->CL_TopDlg->Enable_Info_Icon(false);
			flag_Show_Model_Data = false;
		}

		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouseUnlessPopupClose == false)
		{
			ImGui::SetWindowFocus(nullptr);
		}

		ImGui::End();
	}
}

// *************************************************************************
// *		Show_Ogre_Model_Data_GUI:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void CL64_ImGui::Show_Ogre_Model_Data_GUI(void)
{
	auto& Ogre_Data = App->CL_Mesh->S_OgreMeshData[0]; // App->CL_Mesh->S_OgreMeshData[0]

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Header] = ImVec4(0, 1, 0.4, 0.7);

	ImGui::SetNextItemOpen(true, ImGuiCond_Always);
	if (ImGui::TreeNode("Ogre3D Model", "%s", Ogre_Data->mFileName_Str.c_str()))
	{
		if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
		{

			/*if (flag_Open_Textures_List == 1)
			{
				ImGui::SetNextItemOpen(true, ImGuiCond_Always);
				flag_Open_Textures_List = 0;
			}*/

			ImGui::SetNextItemOpen(true, ImGuiCond_Once);

			// Materials / Textures
			if (ImGui::TreeNode("Materials"))
			{
				int Count = 0;
				int Size = App->CL_Model->GroupCount;
				while (Count < Size)
				{
					if (ImGui::Selectable(App->CL_Mesh->Group[Count]->Ogre_ImGui_MatId, listMaterialItems_Ogre[Count]))
					{
						char mMaterial[MAX_PATH];
						char Texture[MAX_PATH];

						//strcpy(App->CL_Resources->mSelected_File, App->CL_Scene->Group[Count]->Ogre_Texture_FileName);
						strcpy(Texture, App->CL_Mesh->Group[Count]->Ogre_Texture_FileName);
						strcpy(mMaterial, App->CL_Mesh->Group[Count]->Ogre_Material);

						//App->CL_Props_Textures->Selected_Group = Count;
						//App->CL_Ogre->OGL_Listener->Selected_Face_Group = Count;

						if (App->CL_Mesh->Group[Count]->Ogre_Texture_IsValid == 1)
						{
							App->CL_Properties_Materials->View_Texture(Texture, mMaterial);
							App->CL_Properties_Materials->Select_By_Index(Count);
						}
						else
						{
							App->CL_Properties_Materials->Sel_BaseBitmap_Ogre = App->CL_Mesh->Group[Count]->Base_Bitmap;
							App->CL_Properties_Materials->Update_Texture_Ogre_Dlg();
						}

						listMaterialItems_Ogre[PreviouseMaterial_Ogre] = 0;
						listMaterialItems_Ogre[Count] = 1;
						PreviouseMaterial_Ogre = Count;
					}

					Count++;
				}

				ImGui::TreePop();
			}

			// Geometry
			if (ImGui::TreeNode("Geometry"))
			{
				int Count = 0;
				int Size = Ogre_Data->mSubMeshCount;

				ImGui::PushID("foo");
				if (ImGui::BeginMenu("Status"))
				{
					ImGui::Text("Edge List:- %s", Ogre_Data->mStrEdgeList.c_str());
					ImGui::Text("Skeleton:- %s", Ogre_Data->mStrSkeleton.c_str());

					ImGui::EndMenu();
				}
				ImGui::PopID();

				Count = 0;
				while (Count < Size)
				{
					ImGui::PushID("foo");
					if (ImGui::BeginMenu(Ogre_Data->mSubmeshes[Count].m_SubMesh_Name_str.c_str()))
					{
						ImGui::Text("Dedicated vertices:  %s", Ogre_Data->mSubmeshes[Count].m_HasSharedVertices_str.c_str());
						ImGui::Text("Material Name: %s", App->CL_Mesh->Group[Count]->Ogre_Material);
						ImGui::Text("Vertices Count: %i", Ogre_Data->mSubmeshes[Count].VerticesCount);
						ImGui::Text("Bones Used: %i", Ogre_Data->mSubmeshes[Count].BonesCount);

						ImGui::Separator();

						if (ImGui::Checkbox("Show Mesh", &listSubMeshItems[Count]))
						{
							App->CL_Ogre->OGL_Listener->flag_ShowFaces = 1;
							//App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 1;
							App->CL_Ogre->OGL_Listener->Selected_Face_Group = Count;

							listSubMeshItems[PreviouseSubMesh] = 0;
							PreviouseSubMesh = Count;
						}

						ImGui::EndMenu();
					}

					ImGui::PopID();

					Count++;
				}

				if (listSubMeshItems[PreviouseSubMesh] == false) //&& App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces == 1)
				{
					App->CL_Ogre->OGL_Listener->flag_ShowFaces = false;
					//App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 0;
					PreviouseSubMesh = -1;
				}

				ImGui::TreePop();
			}

			// Bounds
			if (ImGui::TreeNode("Bounds"))
			{
				ImGui::Text("Min:- %.5f  %.5f  %.5f", Ogre_Data->vMin.x, Ogre_Data->vMin.y, Ogre_Data->vMin.z);
				ImGui::Text("Max:- %.5f  %.5f  %.5f", Ogre_Data->vMax.x, Ogre_Data->vMax.y, Ogre_Data->vMax.z);
				ImGui::Text("Centre:- %.5f  %.5f  %.5f", Ogre_Data->Center.x, Ogre_Data->Center.y, Ogre_Data->Center.z);

				ImGui::Text("Width:- %f", Ogre_Data->Width);
				ImGui::Text("Height:- %f", Ogre_Data->Height);
				ImGui::Text("Depth:- %f", Ogre_Data->Depth);
				ImGui::Text("Area:- %f", Ogre_Data->Area);
				ImGui::Text("Volume:- %f", Ogre_Data->Volume);
				ImGui::Text("Radius:- %f", Ogre_Data->Radius);

				ImGui::TreePop();
			}

			// Motions
			if (ImGui::TreeNode("Motions"))
			{
				int Count = 0;
				int Size = Ogre_Data->m_Motion_Names.size();

				if (Size == 0)
				{
					ImGui::Text("No Motions:");
				}

				while (Count < Size)
				{
					ImGui::PushID("foo");
					if (ImGui::BeginMenu(Ogre_Data->m_Motion_Names[Count].c_str()))
					{

						ImGui::Text("Motion Name:  %s", Ogre_Data->m_Motion_Names[Count].c_str());
						ImGui::Text("Length:  %f", Ogre_Data->m_Motion_Length[Count]);
						ImGui::Text("Tracks: ( Bones )  %i", Ogre_Data->m_Motion_Num_Of_Tracks[Count]);

						//if (ImGui::Button("Play Motion"))
						//{
						//	//App->CL_TopDlg->Switch_To_Motions_Dlg();
						//	//App->CL_TopDlg->Update_Motions_By_Name(App->CL_Model->S_OgreMeshData[0]->m_Motion_Names[Count].c_str());
						//}

						ImGui::EndMenu();
					}

					ImGui::PopID();
					Count++;
				}

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
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
		ImGui::Spacing();
		ImGui::Text("FPS average %.0f", ImGui::GetIO().Framerate);
		
		/*ImGui::Text("Processes Time %i", App->CL_Ogre->m_Processes_Time);
		if (App->CL_Scene->flag_Player_Added == true)
		{
			ImGui::Text("Cam Pos XYZ %f %f %f", App->CL_Ogre->camNode->getPosition().x, App->CL_Ogre->camNode->getPosition().y, App->CL_Ogre->camNode->getPosition().z);
		}*/
		
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
			/*ImGui::Separator();
			ImGui::Text("Project Name:= %s", App->CL_Project->m_Project_Name);
			ImGui::Text("Level Name:= %s", App->CL_Project->m_Level_Name);
			ImGui::Text("Level Name:= %s", App->CL_Project->m_Level_Name);
			ImGui::Text("Assert Folder:= %s", App->CL_Project->m_Main_Assets_Path);
			ImGui::Separator();
			ImGui::TreePop();*/
		}

		if (ImGui::TreeNode("Brushes"))
		{
			ImGui::Separator();
			ImGui::Text("Brush Count:= %i", App->CL_X_Brush->Get_Brush_Count());
			ImGui::Text("Selected Brushs Count:= %i", App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes));
			ImGui::Text("Current Brush:= %s", App->CL_Doc->CurBrush->Name);
			ImGui::Separator();
			ImGui::TreePop();
		}
	
		if (ImGui::TreeNode("Faces"))
		{
			ImGui::Separator();
			ImGui::Text("Selected Faces:= %i", App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces));
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("App Editor"))
		{
			/*ImGui::Separator();
			ImGui::Text("Player_Count:= %i", App->CL_Scene->Player_Count);
			ImGui::Text("Player Added:= %i", App->CL_Scene->flag_Player_Added);
			ImGui::Separator();
			ImGui::Text("Object_Count:= %i", App->CL_Scene->Object_Count);
			ImGui::Text("Unique_Object_Count:= %i", App->CL_Scene->Object_Count);
			ImGui::Text("Scene Editor Active:= %i", App->CL_Editor_Control->flag_Scene_Editor_Active);
			ImGui::Text("Preview Active:= %i", App->CL_Editor_Control->flag_PreviewMode_Active);
			ImGui::Separator();
			ImGui::TreePop();*/
		}

		if (ImGui::TreeNode("Physics"))
		{
			ImGui::Separator();
			ImGui::Text("Physics Running:= %i", App->CL_Ogre->Listener_3D->flag_Run_Physics);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Render"))
		{
			ImGui::Separator();
			ImGui::Text("Groups:= %i", App->CL_Scene->GroupCount);
			ImGui::Text("Brushes:= %i", App->CL_Scene->BrushCount);

			ImGui::Text("Render Groups:= %i", App->CL_Ogre->OGL_Listener->flag_Render_Groups);
			ImGui::Text("Render Brushes:= %i", App->CL_Ogre->OGL_Listener->flag_Render_Brushes);
			ImGui::Text("Render Ogre:= %i", App->CL_Ogre->OGL_Listener->flag_Render_Ogre);

			ImGui::Text("Render Mode:= %i", App->CL_Ogre->OGL_Listener->Render_Mode);
			
			ImGui::Text("Render Selected Brush:= %i", App->CL_Ogre->OGL_Listener->flag_Show_Selected_Brush);
			ImGui::Text("Render Selected Face:= %i", App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face);

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Debug"))
		{
			ImGui::Separator();

			ImGui::Text("X:= %f", App->CL_Scene->B_Object[0]->Mesh_Rot.x);
			ImGui::Text("Y:= %f", App->CL_Scene->B_Object[0]->Mesh_Rot.y);
			ImGui::Text("Z:= %f", App->CL_Scene->B_Object[0]->Mesh_Rot.z);

			ImGui::Text("X:= %f", Vec_Debug.x);
			ImGui::Text("Y:= %f", Vec_Debug.y);
			ImGui::Text("Z:= %f", Vec_Debug.z);

			ImGui::Separator();
			ImGui::TreePop();
		}
		
		if (ImGui::Button("Close"))
		{
			App->CL_Top_Tabs->flag_Full_View_3D = 0;

			App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);

			App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

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
		ImGui::Text("Groups:= %i", App->CL_Scene->GroupCount);

		ImGui::Text("Selected Group:= %i", App->CL_Picking->m_SubMesh);
		ImGui::Text("Group Face Count:= %i", App->CL_Mesh->Group[App->CL_Picking->m_SubMesh]->GroupFaceCount);
		ImGui::Text("Selected Face:= %i", App->CL_Picking->Local_Face);
		
		ImGui::Separator();
		ImGui::Text("WE_Brush:= %i", App->CL_Mesh->Group[App->CL_Picking->m_SubMesh]->Face_Data[App->CL_Picking->Local_Face].Brush_Index);
		ImGui::Text("WE_Face:= %i", App->CL_Mesh->Group[App->CL_Picking->m_SubMesh]->Face_Data[App->CL_Picking->Local_Face].Main_Face);

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
// *			Debug_Lists_ImGui:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_ImGui::Debug_Lists_ImGui(void)
{
	ImGui::SetNextWindowSize(ImVec2(910, 360));
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("ListBox1", &flag_Show_Listbox, ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		static int item_current_idx = CB_Index; // Here we store our selection data as an index.

		if (ImGui::BeginListBox("##listbox2", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			if (CB_Index == 0) // Views Data
			{
				ImGui::Text("Current View %s", App->CL_Editor_Map->Current_View->Name);
				ImGui::Text("Selected Window %i", App->CL_Editor_Map->Selected_Window);
				ImGui::Text("Zoom %f", App->CL_Editor_Map->Current_View->ZoomFactor);
				ImGui::Text("Width %f", App->CL_Editor_Map->Current_View->Width);
				ImGui::Text("Height %f", App->CL_Editor_Map->Current_View->Height);
				ImGui::Text("Cam Pos %f %f %f", App->CL_Editor_Map->Current_View->CamPos.x, App->CL_Editor_Map->Current_View->CamPos.y, App->CL_Editor_Map->Current_View->CamPos.z);
			}

			if (CB_Index == 1) // Ogre Data
			{
				Ogre::Vector3 Pos = App->CL_Ogre->camNode->getPosition();

				ImGui::Text("Camera Position XYZ  %f %f %f", Pos.x, Pos.y, Pos.z);
			}

			if (CB_Index == 2) // Player Data
			{
				/*ImGui::Text("Player Added  %i", App->CL_Scene->flag_Player_Added);
				ImGui::Text("Player Count  %i", App->CL_Scene->Player_Count);

				btQuaternion Rotation = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getRotation();
				Ogre::Quaternion q = { Rotation.getW(), Rotation.getX(), Rotation.getY(), Rotation.getZ() };

				double x, y, z;
				App->CL_X_Maths->Ogre_QuaternionToEuler(q, x, y, z);

				ImGui::Text("Rotation Y %.02f", y * 180.0 / M_PI);

				float campitch = App->CL_Scene->B_Player[0]->CameraPitch_Node->getOrientation().getPitch().valueDegrees();
				ImGui::Text("Camera Pitch %.02f", campitch);*/
			}

			if (CB_Index == 3) // Locations
			{
				
			}
		
			ImGui::EndListBox();
		}

		static ImGuiComboFlags flags = 0;

		const char* items[] = { "Views Data", "Ogre Data", "Player", "Locations", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)

		if (ImGui::BeginCombo("Debug Category", combo_preview_value, flags))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
				{
					item_current_idx = n;
					CB_Index = item_current_idx;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::Button("Close"))
		{
			App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);
			App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
			flag_Show_Listbox = false;
		}

		//ImGui::PopStyleColor();
		ImGui::End();

	}
}
