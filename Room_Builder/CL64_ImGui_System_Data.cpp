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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_ImGui_System_Data.h"

enum System_Page
{
	System_Page_Camera = 0,
	System_Page_Player = 1,
	System_Page_Model = 2,
	System_Page_Grids = 3,
	System_Page_File = 4,
	System_Page_Prefs = 5,
	System_Page_Entities = 6
};

CL64_ImGui_System_Data::CL64_ImGui_System_Data()
{
	Selected_System_Page = System_Page_Entities;
	flag_Dark_Mode = false;

	flag_System_Viewer_Active = false;
	flag_Loop_Enabled = false;
	flag_Show_System_Data = false;

	Render_hWnd = nullptr;

	RenderListener = nullptr;
	Ogre_MV_Window = nullptr;
	Ogre_MV_SceneMgr = nullptr;
	Ogre_MV_Camera = nullptr;
	Ogre_MV_CamNode = nullptr;
	vp_ImGui = nullptr;
}

CL64_ImGui_System_Data::~CL64_ImGui_System_Data()
{
}

// *************************************************************************
// *			Start_Ogre_Window:- Terry Mo and Hazel  2025			   *
// *************************************************************************
void CL64_ImGui_System_Data::Start_Ogre_Window()
{
	if (flag_System_Viewer_Active == true)
	{
		return;
	}

	CreateDialog(App->hInst, (LPCTSTR)IDD_OGRE_WINDOW, App->MainHwnd, (DLGPROC)Proc_Ogre_Dialog);

	flag_System_Viewer_Active = true;
}

// *************************************************************************
// *		  Proc_Ogre_Dialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_ImGui_System_Data::Proc_Ogre_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_ImGui_System_Data->Render_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_OGRE_CANVAS, hDlg, (DLGPROC)Proc_Viewer_3D);
		App->CL_ImGui_System_Data->Set_OgreWindow();
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->BlackBrush;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_ImGui_System_Data->flag_System_Viewer_Active = false;
			App->CL_ImGui_System_Data->Close_OgreWindow();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *		Proc_Viewer_3D:- Terry and Hazel Flanigan 2026 				   *
// *************************************************************************
LRESULT CALLBACK CL64_ImGui_System_Data::Proc_Viewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG: // Bernie as the dialog is created
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		//if (App->flag_3D_Started == false)
		{
			return (LONG)App->BlackBrush;
		}
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(App->CL_ImGui_System_Data->Render_hWnd, &pos);

		if (App->CL_ImGui->flag_Imgui_Initialized == true && App->CL_ImGui_System_Data->flag_Show_System_Data == true)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos.x = static_cast<float>(pos.x);
			io.MousePos.y = static_cast<float>(pos.y);
		}

		SetFocus(App->CL_ImGui_System_Data->Render_hWnd);

		return 1;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN:
	{
		return 1;
	}
	case WM_RBUTTONUP:
	{
		return 1;
	}
	// Left Mouse Button
	case WM_LBUTTONDOWN:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = true;

		return 1;
	}

	case WM_LBUTTONUP:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = false;

		return 1;
	}

	}

	return FALSE;
}

// *************************************************************************
// *			Set_OgreWindow:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_System_Data::Set_OgreWindow()
{
	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)Render_hWnd);

	Ogre_MV_Window = App->CL_Ogre->mRoot->createRenderWindow("ImGui_Render_Win", 1024, 768, false, &options);

	Ogre_MV_SceneMgr = App->CL_Ogre->mRoot->createSceneManager("DefaultSceneManager", "ImGui_Render_Win");

	Ogre_MV_CamNode = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode("Camera_Node_ImGui");

	Ogre_MV_Camera = Ogre_MV_SceneMgr->createCamera("Camera_ImGui");
	Ogre_MV_Camera->setNearClipDistance(0.1);
	Ogre_MV_Camera->setFarClipDistance(8000);

	Ogre_MV_CamNode->attachObject(Ogre_MV_Camera);
	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, 20));

	vp_ImGui = Ogre_MV_Window->addViewport(Ogre_MV_Camera);

	Ogre_MV_Camera->setAspectRatio(Ogre::Real(vp_ImGui->getActualWidth()) / Ogre::Real(vp_ImGui->getActualHeight()));
	vp_ImGui->setBackgroundColour(ColourValue(0.0, 0.0, 0.0));

	Ogre_MV_SceneMgr->addRenderQueueListener(App->CL_Ogre->mOverlaySystem);
	vp_ImGui->setOverlaysEnabled(true);

	App->CL_Ogre->Ogre3D_Listener->flag_Run_Imgui = false;
	App->CL_Ogre->vp->setOverlaysEnabled(false);

	RenderListener = new CL64_ImGui_Sytem_Listener();
	App->CL_Ogre->mRoot->addFrameListener(RenderListener);

	App->CL_ImGui_System_Data->flag_Loop_Enabled = true;
	App->CL_ImGui_System_Data->flag_Show_System_Data = true;

}

// *************************************************************************
// *		Close_OgreWindow:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_System_Data::Close_OgreWindow(void)
{
	App->CL_Ogre->mRoot->detachRenderTarget("ImGui_Render_Win");
	Ogre_MV_Window->destroy();
	App->CL_Ogre->mRoot->destroySceneManager(Ogre_MV_SceneMgr);

	App->CL_Ogre->mRoot->removeFrameListener(RenderListener);

	delete RenderListener;
	RenderListener = nullptr;

	App->CL_ImGui_System_Data->flag_Loop_Enabled = true;
	App->CL_ImGui_System_Data->flag_Show_System_Data = false;

	App->CL_Ogre->vp->setOverlaysEnabled(true);
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Imgui = true;
}

// *************************************************************************
// *			Selected_Button:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_ImGui_System_Data::Selected_Button(bool IsSelected)
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
void CL64_ImGui_System_Data::ImGui_Render_Loop(void)
{
	if (flag_Loop_Enabled == true)
	{
		if (flag_Show_System_Data == true)
		{
			Imgui_System_Dlg();
		}
	}
}


// *************************************************************************
// *			Imgui_System_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_ImGui_System_Data::Imgui_System_Dlg(void)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(550, 620));

	bool doStyle = true;

	if (doStyle == true)
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(239, 239, 239, 255));
	}

	if (!ImGui::Begin("System Data", &flag_Show_System_Data, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(-1, 120);

		// Just Bottons
		if (ImGui::Button(" Camera ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Camera;
		}

		if (ImGui::Button(" Player ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Player;
		}

		if (ImGui::Button(" Model ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Model;
		}

		if (ImGui::Button(" Grids ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Grids;
		}

		if (ImGui::Button(" File ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_File;
		}

		if (ImGui::Button(" Prefs ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Prefs;
		}

		if (ImGui::Button(" Entity ", ImVec2(100, 0)))
		{
			Selected_System_Page = System_Page_Entities;
		}

		// Functions
		switch (Selected_System_Page) 
		{
		case System_Page_Camera:
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Camera:");
			Data_Camera();
			break;
		}

		case System_Page_Player:
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Editor:");
			Data_Player();
			break;
		}

		case System_Page_Model:
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Model:");
			Data_Model();
			break;
		}

		case System_Page_Grids:
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Grids:");
			Data_Grids();
			break;
		}

		case System_Page_File:
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("File:");
			Data_File();
			break;
		}

		case System_Page_Prefs:
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Prefs:");
			Data_Prefs();
			break;
		}

		case System_Page_Entities:
		{
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Entities:");
			Data_Entity();
			break;
		}

		default:
			ImGui::Text("Unknown Page");
			break;
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
// *				Data_Camera:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_System_Data::Data_Camera(void)
{
	char Buff[MAX_PATH];

	/*switch (App->CL_Ogre->Listener_3D->CameraMode) {
	case Enums::Cam_Mode_None:
		strcpy(Buff, "No Camera");
		break;
	case Enums::Cam_Mode_First:
		strcpy(Buff, "First Person View Mode");
		break;
	case Enums::Cam_Mode_Free:
		strcpy(Buff, "Free Mode");
		break;
	case Enums::Cam_Mode_Third:
		strcpy(Buff, "Third Person View Mode");
		break;
	case Enums::Cam_Mode_Model:
		strcpy(Buff, "   -- Model Mode --");
		break;
	default:
		strcpy(Buff, "Unknown Camera Mode");
		break;
	}

	ImGui::Text("Camera Mode %s", Buff);
	ImGui::Spacing();*/

	ImGui::Text("Cam X %f", App->CL_Ogre->camNode->getPosition().x);
	ImGui::Text("Cam Y %f", App->CL_Ogre->camNode->getPosition().y);
	ImGui::Text("Cam Z %f", App->CL_Ogre->camNode->getPosition().z);
}

// *************************************************************************
// *				Data_Player:- Terry and Hazel Flanigan 2026			   
// *************************************************************************
void CL64_ImGui_System_Data::Data_Player(void)
{
	char Buff[MAX_PATH];

	if (App->CL_Scene->flag_Player_Added == true)
	{
		strcpy(Buff, "  -- Yes -- ");
	}
	else
	{
		strcpy(Buff, "  -- No --");
	}

	ImGui::Text("Player Added %s", Buff);

	ImGui::Text("Player Count %i", App->CL_Scene->Player_Count);

	if (App->CL_Scene->flag_Player_Added == true)
	{
		ImGui::Text("Player Name %s", App->CL_Scene->B_Player[0]->Player_Name);
	}

	ImGui::Text(" ");

	ImGui::Text("Group Count %i", App->CL_Scene->GroupCount);

	ImGui::Text(" ");

	if (App->CL_Mesh_Mgr->World_Ent)
	{
		strcpy(Buff, "  -- Loaded -- ");
	}
	else
	{
		strcpy(Buff, "  -- Nothing --");
	}

	ImGui::Text("World Entity %s", Buff);

	ImGui::Text(" ");
	ImGui::TextWrapped("TXL Path %s", App->CL_Level->TXL_PathAndFile);

	//ImGui::Text("Editor Mode %s", Buff);
}

// *************************************************************************
// *				Data_Model:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_System_Data::Data_Model(void)
{
	char Buff[MAX_PATH];
	
	/*if (App->CL_Model->Imported_Ogre_Ent)
	{
		strcpy(Buff, "  -- Loaded -- ");
	}
	else
	{
		strcpy(Buff, "  -- Nothing --");
	}*/

	ImGui::Text("Imported_Ogre_Ent %s", Buff);

	if (App->CL_Mesh_Mgr->World_Ent)
	{
		strcpy(Buff, "  -- Loaded -- ");
	}
	else
	{
		strcpy(Buff, "  -- Nothing --");
	}

	ImGui::Text("World Entity %s", Buff);

	ImGui::Text(" ");
	//ImGui::Text("Model Name %s", App->CL_File->Model_Just_Name);
	
	/*switch (App->CL_Model->Model_Type)
	{
	case Enums::Model_Type_None:
		strcpy(Buff, "  -- Model_Type_None -- ");
		break;
	case Enums::Model_Type_Ogre3D:
		strcpy(Buff, "  -- Model_Type_Ogre3D -- ");
		break;
	case Enums::Model_Type_Assimp:
		strcpy(Buff, "  -- Model_Type_Assimp -- ");
		break;
	case Enums::Model_Type_Brush:
		strcpy(Buff, "  -- Model_Type_Brush -- ");
		break;

	default:
		strcpy(Buff, "Unknown Model Mode");
		break;
	}*/

	ImGui::Text("Model Type %s", Buff);

	/*ImGui::Text(" ");
	ImGui::Text("Counts ----------------------------------- ");
	ImGui::Text("Group Count %i", App->CL_Model->GroupCount);
	ImGui::Text("Texture Count %i", App->CL_Model->TextureCount);
	ImGui::Text("Motions %i", App->CL_Model->MotionCount);
	ImGui::Text("Vertice Count %i", App->CL_Model->VerticeCount);
	ImGui::Text("Face Count %i", App->CL_Model->FaceCount);
	ImGui::Text("Bone Count %i", App->CL_Model->BoneCount);
	

	ImGui::Text("Model Loaded %i", App->CL_Model->flag_Model_Loaded);*/

	/*if (App->CL_Model->flag_BoundingBox_Created == true)
	{
		strcpy(Buff, "  -- Yes -- ");
	}
	else
	{
		strcpy(Buff, "  -- No --");
	}*/
	ImGui::Text("Bounding Box Created %s", Buff);

	ImGui::Text(" ");
	/*if (App->CL_Model->flag_Model_is_Modified == true)
	{
		strcpy(Buff, "  -- Yes -- ");
	}
	else
	{
		strcpy(Buff, "  -- No --");
	}*/
	ImGui::Text("Model Modified %s", Buff);
	
}

// *************************************************************************
// *				Data_Grids:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_System_Data::Data_Grids(void)
{
	char Buff[MAX_PATH];

	switch (App->CL_Views_Com->Selected_Window)
	{
	case Enums::Selected_Map_View_None:
		strcpy(Buff, "  -- None -- ");
		break;
	case Enums::Selected_Map_View_3D:
		strcpy(Buff, "  -- 3D View -- ");
		break;
	case Enums::Selected_Map_View_TL:
		strcpy(Buff, "  -- Top Left Grid -- ");
		break;

	case Enums::Selected_Map_View_TR:
		strcpy(Buff, "  -- Top Right Grid -- ");
		break;

	case Enums::Selected_Map_View_BL:
		strcpy(Buff, "  -- Bottom Left Window -- ");
		break;

	default:
		strcpy(Buff, "Unknown Model Mode");
		break;
	}

	
	ImGui::Text("Selected Window %s", Buff);
	ImGui::Text(" ");

	ImGui::Text("Name %s", App->CL_View_Top_Left->VCam_TL->Name);
	ImGui::Text("Zoom Speed %f", App->CL_View_Top_Left->m_Zoom_Amount);
	ImGui::Text("Zoom Amount %f", App->CL_View_Top_Left->VCam_TL->ZoomFactor);
	ImGui::Text("Height %f", (float)App->CL_View_Top_Left->VCam_TL->Height);
	ImGui::Text("Width %f", (float)App->CL_View_Top_Left->VCam_TL->Width);
	ImGui::Text(" ");

	ImGui::Text("Name %s", App->CL_View_Top_Right->VCam_TR->Name);
	ImGui::Text("Zoom Speed %f", App->CL_View_Top_Right->m_Zoom_Amount);
	ImGui::Text("Zoom Amount %f", App->CL_View_Top_Right->VCam_TR->ZoomFactor);
	ImGui::Text("Height %f", (float)App->CL_View_Top_Right->VCam_TR->Height);
	ImGui::Text("Width %f", (float)App->CL_View_Top_Right->VCam_TR->Width);
	ImGui::Text(" ");

	ImGui::Text("Name %s", App->CL_View_Bottom_Left->VCam_BL->Name);
	ImGui::Text("Zoom Speed %f", App->CL_View_Bottom_Left->m_Zoom_Amount);
	ImGui::Text("Zoom Amount %f", App->CL_View_Bottom_Left->VCam_BL->ZoomFactor);
	ImGui::Text("Height %f", (float)App->CL_View_Bottom_Left->VCam_BL->Height);
	ImGui::Text("Width %f", (float)App->CL_View_Bottom_Left->VCam_BL->Width);
	ImGui::Text(" ");

}

// *************************************************************************
// *				Data_File:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_ImGui_System_Data::Data_File(void)
{
	ImGui::Text("File Version %f", App->CL_Level->Level_Version);
	ImGui::Text(" ");
	/*ImGui::Text("Name %s", App->CL_File->Model_Just_Name);
	ImGui::Text("File Name %s", App->CL_File->Loaded_FileName);

	ImGui::Text(" ");
	ImGui::TextWrapped("File Path %s", App->CL_File->Loaded_PathFileName);

	ImGui::Text(" ");
	ImGui::Text("Model CBF File");
	ImGui::Text("Name %s", App->CL_File->MTF_Just_Name);
	ImGui::Text("File Name %s", App->CL_File->MTF_Just_FileName);*/
	ImGui::Text(" ");

	ImGui::Text("Texture Library Zip File");
	
	ImGui::Text("Texture Library File Name %s", App->CL_Level->TXL_Just_File_Name);
	ImGui::Text("Path");
	ImGui::TextWrapped("%s", App->CL_Level->TXL_PathAndFile);
	
}

// ************************************************************************
// *				Data_Prefs:- Terry and Hazel Flanigan 2026			   
// ************************************************************************
void CL64_ImGui_System_Data::Data_Prefs(void)
{
	/*ImGui::Text("Lib Version %s", App->CL_Libs->CL_Preference->GetVersion());
	
	ImGui::Text(" ");
	ImGui::TextWrapped("File Path %s", App->CL_Libs->CL_Preference->Prefs_Last_PathAndFile);*/

}

// ************************************************************************
// *				Data_Entity:- Terry and Hazel Flanigan 2026			   
// ************************************************************************
void CL64_ImGui_System_Data::Data_Entity(void)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	auto& Selected_Entity = App->CL_Scene->B_Object[Index];  // Pointer to Entity
	
	ImGui::Text("Total Entities  %i", App->CL_Scene->Object_Count);

	ImGui::Text("");

	ImGui::Text("Entity");
	ImGui::Text("Selected Entity Index %i", App->CL_Properties_Scene->Current_Selected_Object);
	ImGui::Text("Entity Name  - %s -", Selected_Entity->Object_Name);
	ImGui::Text("Entity Type  - %s -", Selected_Entity->Entity_Type_Name);
	
	ImGui::Text("");
	ImGui::Text("Mesh");
	ImGui::Text("Mesh Filename  - %s -", Selected_Entity->Mesh_FileName);
	ImGui::Text("Mesh Position  X %.3f  Y %.3f  Z %.3f", Selected_Entity->Mesh_Pos.x, Selected_Entity->Mesh_Pos.y, Selected_Entity->Mesh_Pos.z);
	ImGui::Text("Mesh Scale  X %.3f  Y %.3f  Z %.3f", Selected_Entity->Mesh_Scale.x, Selected_Entity->Mesh_Scale.y, Selected_Entity->Mesh_Scale.z);

	ImGui::Text("");
	ImGui::Text("Physics");
	ImGui::Text("Physics Position  X %.3f  Y %.3f  Z %.3f", Selected_Entity->Physics_Pos.x, Selected_Entity->Physics_Pos.y, Selected_Entity->Physics_Pos.z);
	ImGui::Text("Physics Scale  X %.3f  Y %.3f  Z %.3f", Selected_Entity->Physics_Scale.x, Selected_Entity->Physics_Scale.y, Selected_Entity->Physics_Scale.z);
	ImGui::Text("Physics Size  X %.3f  Y %.3f  Z %.3f", Selected_Entity->Physics_Size.x, Selected_Entity->Physics_Size.y, Selected_Entity->Physics_Size.z);
}


// ************************************************************************
// *
// *		CL64_ImGui_Sytem_Listener:- Terry and Hazel Flanigan 2026			   
// *
// ************************************************************************
CL64_ImGui_Sytem_Listener::CL64_ImGui_Sytem_Listener(void)
{

}

CL64_ImGui_Sytem_Listener::~CL64_ImGui_Sytem_Listener(void)
{

}

// *************************************************************************
// *				frameStarted:- Terry and Hazel Flanigan 2026
// *************************************************************************
bool CL64_ImGui_Sytem_Listener::frameStarted(const Ogre::FrameEvent& evt)
{
	return true;
}


// *************************************************************************
// *			frameRenderingQueued:- Terry and Hazel Flanigan 2026
// *************************************************************************
bool CL64_ImGui_Sytem_Listener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	Ogre::ImGuiOverlay::NewFrame();
	App->CL_ImGui_System_Data->ImGui_Render_Loop();

	return true;
}
