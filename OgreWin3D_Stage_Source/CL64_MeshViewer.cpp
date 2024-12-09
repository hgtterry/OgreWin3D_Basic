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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_MeshViewer.h"

#include "CL64_MeshView_Listener.h"

#pragma warning(disable : 4101)

CL64_MeshViewer::CL64_MeshViewer(void)
{
	MainDlgHwnd =			nullptr;
	MeshViewer_3D_hWnd =	nullptr;
	ListHwnd =				nullptr;
	CB_hWnd =				nullptr;

	// Ogre
	Ogre_MV_Window =		nullptr;
	Ogre_MV_SceneMgr =		nullptr;
	Ogre_MV_Camera =		nullptr;
	Ogre_MV_CamNode =		nullptr;
	Ogre_MvEnt =			nullptr;
	Ogre_MvNode =			nullptr;

	Ogre_MV_Phys_Body =		nullptr;
	Ogre_MV_Phys_Shape =	nullptr;

	MV_btDebug_Manual =		nullptr;
	MV_btDebug_Node =		nullptr;

	flag_MV_Resource_Path_Loaded = 0;
	flag_MeshViewer_Running = 0;

	flag_SelectDynamic = 0;
	flag_SelectStatic = 0;
	flag_SelectTriMesh = 0;

	flag_SelectDynamic = 0;
	flag_SelectStatic = 0;
	flag_SelectTriMesh = 0;

	flag_Selected_Shape_Box = 0;
	flag_Selected_Shape_Sphere = 0;
	flag_Selected_Shape_Capsule = 0;
	flag_Selected_Shape_Cylinder = 0;
	flag_Selected_Shape_Cone = 0;

	MV_Render_Debug = 0;

	MV_Resource_Group = "MV_Resource_Group";

	Object_Name[0] = 0;
	m_Just_Folder[0] = 0;
	m_Resource_Folder_Full[0] = 0;
	Selected_MeshFile[0] = 0;

	Mesh_Viewer_Mode = 0; // 0 = Defaulet Objects 1 = Collectables

	Physics_Shape = Enums::Shape_None;
	Physics_Type = Enums::Bullet_Type_None;

	// Old Copy System
	m_Material_File[0] = 0;
	Texure_Count = 0;
	NumSub_Meshes = 0;
	DestinationFile[0] = 0;
	SourceFile[0] = 0;
}

CL64_MeshViewer::~CL64_MeshViewer(void)
{
}

// *************************************************************************
// *			Reset_Data:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_MeshViewer::Reset_Data()
{
	Ogre_MV_Window = nullptr;
	Ogre_MV_SceneMgr = nullptr;
	Ogre_MV_Camera = nullptr;
	Ogre_MV_CamNode = nullptr;
	Ogre_MvEnt = nullptr;
	Ogre_MvNode = nullptr;

	Ogre_MV_Phys_Body = nullptr;
	Ogre_MV_Phys_Shape = nullptr;

	MV_btDebug_Manual = nullptr;
	MV_btDebug_Node = nullptr;

	MV_Render_Debug = 0;

	v_Texture_Names.resize(0);
	v_Scrip_Names.resize(0);
}

// *************************************************************************
// *	  	Start_MeshViewer_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Start_MeshViewer_Dlg()
{
	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;

	App->CL_Bullet->Show_Debug_Objects(false);

	App->CL_Panels->Disable_Panels(true);
	App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 1;
	App->CL_ImGui_Dialogs->flag_Disable_Physics_Console = 1;
	App->CL_Keyboard->flag_Block_Keyboard = 1;

	Reset_Data();

	App->CL_Ogre->RenderFrame(1);

	App->CL_Ogre->Log_Message_To_File((LPSTR)" // -------------------------  MeshViewer Dialog");

	CreateDialog(App->hInst, (LPCTSTR)IDD_MESHVIEWER, App->Fdlg, (DLGPROC)Proc_MeshViewer_Dlg);
	
}

// *************************************************************************
// *			Proc_MeshViewer:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_MeshViewer::Proc_MeshViewer_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_FOLDERS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LISTFILES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SELECTEDNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BOX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_MVSTATIC, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_DYNAMIC, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TRIMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SPHERE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CAPSULE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CYLINDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CONE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STSHAPE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTYPE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STFOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_PROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MV_RESOURCES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_OBJECTNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_MeshViewer->MainDlgHwnd = hDlg;
		App->CL_MeshViewer->ListHwnd = GetDlgItem(hDlg, IDC_LISTFILES);
		App->CL_MeshViewer->CB_hWnd = GetDlgItem(hDlg, IDC_CB_FOLDERS);

		App->CL_MeshViewer->MeshViewer_3D_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MESHVIEWER_3D, hDlg, (DLGPROC)Proc_MeshViewer_3D);
		App->CL_MeshViewer->Set_OgreWindow();
		App->CL_Ogre->Log_Message_To_File((LPSTR)"MeshVierer Ogre Started");

		if (App->CL_MeshViewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area)
		{
			App->CL_MeshViewer->Set_For_Areas(hDlg);

			strcpy(App->CL_MeshViewer->Selected_MeshFile, "Indoor.mesh");
			strcpy(App->CL_MeshViewer->m_Just_Folder, "Areas");
			strcpy(App->CL_MeshViewer->m_Resource_Folder_Full, App->GD_Directory_FullPath); // Full Path Stock Folders 
			strcat(App->CL_MeshViewer->m_Resource_Folder_Full, "\\Stock\\");
			strcat(App->CL_MeshViewer->m_Resource_Folder_Full, App->CL_MeshViewer->m_Just_Folder);
			strcat(App->CL_MeshViewer->m_Resource_Folder_Full, "\\");
			App->CL_MeshViewer->Add_Resources();
			App->CL_MeshViewer->Get_Mesh_Files();
			App->CL_Ogre->Log_Message_To_File((LPSTR)"Area Get_Mesh_Files");
			App->CL_MeshViewer->Show_Mesh(App->CL_MeshViewer->Selected_MeshFile);
			App->CL_Ogre->Log_Message_To_File((LPSTR)"Area Show Mesh");
			App->CL_MeshViewer->Get_Stock_Folders(App->CL_MeshViewer->CB_hWnd);
		}
		else
		{
			App->CL_MeshViewer->Get_Stock_Folders(App->CL_MeshViewer->CB_hWnd);
			App->CL_MeshViewer->Add_Resources();
			App->CL_MeshViewer->Get_Mesh_Files();
			App->CL_Ogre->Log_Message_To_File((LPSTR)"Get_Mesh_Files");

			App->CL_MeshViewer->Show_Mesh(App->CL_MeshViewer->Selected_MeshFile);
			App->CL_Ogre->Log_Message_To_File((LPSTR)"Show Mesh");
		}

		SetWindowText(hDlg, App->CL_MeshViewer->m_Resource_Folder_Full);

		App->CL_MeshViewer->Enable_ShapeButtons(false);

		if (App->CL_MeshViewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Objects)
		{
			char ATest[MAX_PATH];
			char ConNum[MAX_PATH];

			strcpy(ATest, "Object_");
			_itoa(App->CL_Scene->Object_Count, ConNum, 10);
			strcat(ATest, ConNum);

			SetDlgItemText(hDlg, IDC_OBJECTNAME, ATest);
			strcpy(App->CL_MeshViewer->Object_Name, ATest);
		}

		

		App->CL_MeshViewer->flag_MeshViewer_Running = 1;

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_SELECTEDNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_STSHAPE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTYPE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STFOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_OBJECTNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}


		return FALSE;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_PROPERTIES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_MV_RESOURCES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_MVSTATIC)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_SelectStatic);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_DYNAMIC)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_SelectDynamic);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TRIMESH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_SelectTriMesh);
			return CDRF_DODEFAULT;
		}


		if (some_item->idFrom == IDC_JUSTOGRE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_JUSTOGRE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}

			//App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_SelectTriMesh);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TEST)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_TEST));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}

			//App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_SelectTriMesh);
			return CDRF_DODEFAULT;
		}

		// ---------------------------------------------------------------------
		if (some_item->idFrom == IDC_BOX)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BOX));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_Selected_Shape_Box);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_SPHERE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_SPHERE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_Selected_Shape_Sphere);
			}
			
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CAPSULE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_CAPSULE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_Selected_Shape_Capsule);
			}
			
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CYLINDER)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_CYLINDER));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_Selected_Shape_Cylinder);
			}
			
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CONE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_CONE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_MeshViewer->flag_Selected_Shape_Cone);
			}
			
			return CDRF_DODEFAULT;
		}

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

	case WM_MOUSEWHEEL:
	{
		int zDelta = (short)HIWORD(wParam);    // wheel rotation

		if (zDelta > 0)
		{
			App->CL_MeshViewer->RenderListener->Wheel_Move = -1;
		}
		else if (zDelta < 0)
		{
			App->CL_MeshViewer->RenderListener->Wheel_Move = 1;
		}

		return 1;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_CB_FOLDERS)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				HWND temp = GetDlgItem(hDlg, IDC_CB_FOLDERS);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)App->CL_MeshViewer->m_Just_Folder);

				SendMessage(App->CL_MeshViewer->ListHwnd, LB_RESETCONTENT, 0, 0);

				int cmp = strcmp(App->CL_MeshViewer->m_Just_Folder, "Project_Assets");
				if (cmp == 0)
				{
					strcpy(App->CL_MeshViewer->m_Resource_Folder_Full, App->CL_Project->m_Main_Assets_Path);
				}
				else
				{
					strcpy(App->CL_MeshViewer->m_Resource_Folder_Full, App->GD_Directory_FullPath);
					strcat(App->CL_MeshViewer->m_Resource_Folder_Full, "\\Stock\\");
					strcat(App->CL_MeshViewer->m_Resource_Folder_Full, App->CL_MeshViewer->m_Just_Folder);
					strcat(App->CL_MeshViewer->m_Resource_Folder_Full, "\\");
				}
				
				//SetDlgItemText(hDlg, IDC_ST_CURRENTFOLDER, App->SBC_MeshViewer->mResource_Folder);
				SetWindowText(hDlg, App->CL_MeshViewer->m_Resource_Folder_Full);

				App->CL_MeshViewer->Delete_Resources_Group();
				App->CL_MeshViewer->Add_Resources();
				App->CL_MeshViewer->Get_Mesh_Files();
				App->CL_MeshViewer->Show_Mesh(App->CL_MeshViewer->Selected_MeshFile);
				
				//App->CL_MeshViewer->GridNode->resetOrientation();
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PROPERTIES)
		{
			App->CL_MeshViewer->Show_Mesh_Properties();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MV_RESOURCES)
		{
			App->CL_Resources->Start_Resources();
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_LISTFILES)
		{
			char buff[MAX_PATH] { 0 };
			int Index = 0;
			Index = SendDlgItemMessage(hDlg, IDC_LISTFILES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (Index == -1)
			{
				return 1;
			}

			SendDlgItemMessage(hDlg, IDC_LISTFILES, LB_GETTEXT, (WPARAM)Index, (LPARAM)buff);
			SetDlgItemText(hDlg, IDC_SELECTEDNAME, buff);

			strcpy(App->CL_MeshViewer->Selected_MeshFile, buff);

			App->CL_MeshViewer->Show_Mesh(App->CL_MeshViewer->Selected_MeshFile);

			return TRUE;

		}

		if (LOWORD(wParam) == IDC_MVSTATIC)
		{
			/*if (App->CL_MeshViewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Collectables)
			{
				return 1;
			}*/

			App->CL_MeshViewer->Physics_Type = Enums::Bullet_Type_Static;
			App->CL_MeshViewer->Physics_Shape = Enums::Shape_None;
			App->CL_MeshViewer->flag_SelectStatic = 1;
			App->CL_MeshViewer->flag_SelectDynamic = 0;
			App->CL_MeshViewer->flag_SelectTriMesh = 0;

			App->CL_MeshViewer->Enable_ShapeButtons(true);

			RedrawWindow(App->CL_MeshViewer->MainDlgHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_MeshViewer->Show_Physics_None();

			return 1;
		}

		if (LOWORD(wParam) == IDC_DYNAMIC)
		{
			App->CL_MeshViewer->Physics_Type = Enums::Bullet_Type_Dynamic;
			App->CL_MeshViewer->Physics_Shape = Enums::Shape_None;
			App->CL_MeshViewer->flag_SelectDynamic = 1;
			App->CL_MeshViewer->flag_SelectStatic = 0;
			App->CL_MeshViewer->flag_SelectTriMesh = 0;

			App->CL_MeshViewer->Enable_ShapeButtons(true);

			RedrawWindow(App->CL_MeshViewer->MainDlgHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_MeshViewer->Show_Physics_None();

			return 1;
		}

		if (LOWORD(wParam) == IDC_TRIMESH)
		{
			App->CL_MeshViewer->Physics_Type = Enums::Bullet_Type_TriMesh;
			App->CL_MeshViewer->flag_SelectStatic = 0;
			App->CL_MeshViewer->flag_SelectDynamic = 0;
			App->CL_MeshViewer->flag_SelectTriMesh = 1;

			App->CL_MeshViewer->Enable_ShapeButtons(false);

			RedrawWindow(App->CL_MeshViewer->MainDlgHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_MeshViewer->Physics_Shape = Enums::Shape_None;

			App->CL_MeshViewer->Show_Physics_Trimesh();

			return 1;
		}

		if (LOWORD(wParam) == IDC_BOX)
		{
			App->CL_MeshViewer->Clear_Shape_Buttons();
			App->CL_MeshViewer->flag_Selected_Shape_Box = 1;
			RedrawWindow(App->CL_MeshViewer->MainDlgHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_MeshViewer->Physics_Shape = Enums::Shape_Box;

			App->CL_MeshViewer->Show_Physics_Box();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_SPHERE)
		{
			App->CL_MeshViewer->Clear_Shape_Buttons();
			App->CL_MeshViewer->flag_Selected_Shape_Sphere = 1;
			RedrawWindow(App->CL_MeshViewer->MainDlgHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_MeshViewer->Physics_Shape = Enums::Shape_Sphere;

			App->CL_MeshViewer->Show_Physics_Sphere();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CAPSULE)
		{
			App->CL_MeshViewer->Clear_Shape_Buttons();
			App->CL_MeshViewer->flag_Selected_Shape_Capsule = 1;
			RedrawWindow(App->CL_MeshViewer->MainDlgHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_MeshViewer->Physics_Shape = Enums::Shape_Capsule;

			App->CL_MeshViewer->Show_Physics_Capsule();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CYLINDER)
		{
			App->CL_MeshViewer->Clear_Shape_Buttons();
			App->CL_MeshViewer->flag_Selected_Shape_Cylinder = 1;
			RedrawWindow(App->CL_MeshViewer->MainDlgHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_MeshViewer->Physics_Shape = Enums::Shape_Cylinder;

			App->CL_MeshViewer->Show_Physics_Cylinder();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CONE)
		{
			App->CL_MeshViewer->Clear_Shape_Buttons();
			App->CL_MeshViewer->flag_Selected_Shape_Cone = 1;
			RedrawWindow(App->CL_MeshViewer->MainDlgHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_MeshViewer->Physics_Shape = Enums::Shape_Cone;

			App->CL_MeshViewer->Show_Physics_Cone();

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			if (App->CL_MeshViewer->Physics_Type == Enums::Bullet_Type_TriMesh)
			{

			}
			else if (App->CL_MeshViewer->Physics_Type == Enums::Bullet_Type_None || App->CL_MeshViewer->Physics_Shape == Enums::Shape_None)
			{
				App->Say("No Type or Shape Selected");
				return TRUE;
			}


			if (App->CL_MeshViewer->Ogre_MV_Phys_Body)
			{
				App->CL_Bullet->dynamicsWorld->removeCollisionObject(App->CL_MeshViewer->Ogre_MV_Phys_Body);
				App->CL_MeshViewer->Ogre_MV_Phys_Body = nullptr;
			}

			int cmp = strcmp(App->CL_MeshViewer->m_Just_Folder, "Project_Assets");
			if (cmp == 0)
			{
				strcpy(App->CL_MeshViewer->m_Resource_Folder_Full, App->CL_Project->m_Main_Assets_Path);
				App->CL_Objects_Create->Add_Objects_From_MeshViewer();
			}
			else if (App->CL_MeshViewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area)
			{

				App->CL_MeshViewer->Copy_Assets();
				App->CL_Com_Area->Add_New_Area();
			}
			else
			{
				App->CL_MeshViewer->Copy_Assets();
				App->CL_Objects_Create->Add_Objects_From_MeshViewer();
			}


			App->CL_Ogre->RenderFrame(10);
			App->CL_MeshViewer->Close_OgreWindow();
			App->CL_MeshViewer->Delete_Resources_Group();

			App->CL_MeshViewer->Clear_Shape_Buttons();
			App->CL_MeshViewer->Clear_Type_Buttons();
			
			App->CL_MeshViewer->flag_MeshViewer_Running = 0;

			App->CL_Panels->Disable_Panels(false);
			App->CL_ImGui_Dialogs->flag_Disable_Physics_Console = 0;
			App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 0;
			App->CL_Keyboard->flag_Block_Keyboard = 0;
			

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_MeshViewer->Close_OgreWindow();
			App->CL_MeshViewer->Delete_Resources_Group();

			App->CL_MeshViewer->Clear_Type_Buttons();

			App->CL_MeshViewer->flag_MeshViewer_Running = 0;

			App->CL_Panels->Disable_Panels(false);
			App->CL_ImGui_Dialogs->flag_Disable_Physics_Console = 0;
			App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 0;
			App->CL_Keyboard->flag_Block_Keyboard = 0;

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	return FALSE;
}

// *************************************************************************
// *			Set_For_Areas:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MeshViewer::Set_For_Areas(HWND hDlg)
{
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_MVSTATIC), false);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_DYNAMIC), false);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_TRIMESH), true);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_JUSTOGRE), false);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_TEST), false);

	//SelectTriMesh = 1;

	Enable_ShapeButtons(0);

	char ConNum[256];
	char ATest[256];

	strcpy_s(ATest, "Area_");
	_itoa(App->CL_Scene->Area_Count, ConNum, 10);
	strcat(ATest, ConNum);

	SetDlgItemText(hDlg, IDC_OBJECTNAME, ATest);
	/*strcpy(App->SBC_MeshViewer->Object_Name, ATest);
	strcpy(App->SBC_MeshViewer->m_Current_Folder, "Areas");
	HWND temp = GetDlgItem(hDlg, IDC_CB_FOLDERS);
	SendMessage(temp, CB_SELECTSTRING, -1, LPARAM(App->SBC_MeshViewer->m_Current_Folder));

	SendMessage(App->SBC_MeshViewer->ListHwnd, LB_RESETCONTENT, 0, 0);

	strcpy(App->SBC_MeshViewer->mResource_Folder, App->EquityDirecory_FullPath);
	strcat(App->SBC_MeshViewer->mResource_Folder, "\\Media_New\\");
	strcat(App->SBC_MeshViewer->mResource_Folder, App->SBC_MeshViewer->m_Current_Folder);
	strcat(App->SBC_MeshViewer->mResource_Folder, "\\");

	SetDlgItemText(hDlg, IDC_ST_CURRENTFOLDER, App->SBC_MeshViewer->mResource_Folder);
	SetWindowText(hDlg, App->SBC_MeshViewer->mResource_Folder);

	App->SBC_MeshViewer->Add_Resources();
	App->SBC_MeshViewer->Get_Files();*/
}

// *************************************************************************
// *	  	Clear_Shape_Buttons:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MeshViewer::Clear_Shape_Buttons()
{
	flag_Selected_Shape_Box = 0;
	flag_Selected_Shape_Sphere = 0;
	flag_Selected_Shape_Capsule = 0;
	flag_Selected_Shape_Cylinder = 0;
	flag_Selected_Shape_Cone = 0;
}

// *************************************************************************
// *	  	Clear_Type_Buttons:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MeshViewer::Clear_Type_Buttons()
{
	App->CL_MeshViewer->flag_SelectStatic = 0;
	App->CL_MeshViewer->flag_SelectDynamic = 0;
	App->CL_MeshViewer->flag_SelectTriMesh = 0;
}

// *************************************************************************
// *		Close_MeshWindow:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MeshViewer::Close_OgreWindow(void)
{
	App->CL_MeshViewer->MV_Render_Debug = 0;


	App->CL_Ogre->mRoot->removeFrameListener(RenderListener);

	App->CL_Ogre->mRoot->detachRenderTarget("MeshViewWin");
	Ogre_MV_Window->destroy();
	App->CL_Ogre->mRoot->destroySceneManager(Ogre_MV_SceneMgr);
}

// *************************************************************************
// *			OgreWindow:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_MeshViewer::Set_OgreWindow(void)
{

	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)MeshViewer_3D_hWnd);

	Ogre_MV_Window = App->CL_Ogre->mRoot->createRenderWindow("MeshViewWin", 1024, 768, false, &options);

	Ogre_MV_SceneMgr = App->CL_Ogre->mRoot->createSceneManager("DefaultSceneManager", "MeshViewGD");

	Ogre_MV_CamNode = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode("Camera_Node");

	Ogre_MV_Camera = Ogre_MV_SceneMgr->createCamera("CameraMV");
	Ogre_MV_Camera->setNearClipDistance(0.1);
	Ogre_MV_Camera->setFarClipDistance(8000);

	Ogre_MV_CamNode->attachObject(Ogre_MV_Camera);
	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, 0));

	Ogre::Viewport* vp = Ogre_MV_Window->addViewport(Ogre_MV_Camera);
	Ogre_MV_Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	Ogre_MV_SceneMgr->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	RenderListener = new CL64_MeshView_Listener();

	App->CL_Ogre->mRoot->addFrameListener(RenderListener);

	//Reset_Camera();

	// Debug Physics Shape
	MV_btDebug_Manual = Ogre_MV_SceneMgr->createManualObject("MVManual");
	MV_btDebug_Manual->setRenderQueueGroup(RENDER_QUEUE_MAX);
	MV_btDebug_Manual->setDynamic(true);
	MV_btDebug_Manual->estimateVertexCount(2000);
	MV_btDebug_Manual->begin("Template/White_Alpha_GD64", Ogre::RenderOperation::OT_LINE_LIST, App->CL_Ogre->App_Resource_Group);
	MV_btDebug_Manual->position(0, 0, 0);
	MV_btDebug_Manual->colour(1, 1, 1, 1);
	MV_btDebug_Manual->position(0, 0, 0);
	MV_btDebug_Manual->colour(1, 1, 1, 1);
	MV_btDebug_Manual->end();
	MV_btDebug_Node = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode();
	MV_btDebug_Node->attachObject(MV_btDebug_Manual);
	MV_btDebug_Node->setVisible(true);

	App->CL_MeshViewer->MV_Render_Debug = 1;
	return 1;
}

// *************************************************************************
// *		Proc_MeshViewer_3D:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
LRESULT CALLBACK CL64_MeshViewer::Proc_MeshViewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG: // Bernie as the dialog is created
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		if (App->flag_OgreStarted == 0)
		{
			return (LONG)App->BlackBrush;
		}
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		SetFocus(App->CL_MeshViewer->MeshViewer_3D_hWnd);

		break;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		if (App->flag_OgreStarted == 1)
		{
			SetCapture(App->CL_MeshViewer->MeshViewer_3D_hWnd);// Bernie
			SetCursorPos(App->CursorPosX, App->CursorPosY);
			App->CL_MeshViewer->RenderListener->Pl_RightMouseDown = 1;
			App->CUR = SetCursor(NULL);
			return 1;
		}

		return 1;
	}
	case WM_RBUTTONUP:
	{
		if (App->flag_OgreStarted == 1)
		{
			ReleaseCapture();
			App->CL_MeshViewer->RenderListener->Pl_RightMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}

		return 1;
	}
	// Left Mouse Button
	case WM_LBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		if (App->flag_OgreStarted == 1)
		{

			SetCapture(App->CL_MeshViewer->MeshViewer_3D_hWnd);// Bernie
			SetCursorPos(App->CursorPosX, App->CursorPosY);

			App->CL_MeshViewer->RenderListener->Pl_LeftMouseDown = 1;

			App->CUR = SetCursor(NULL);

			return 1;
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		if (App->flag_OgreStarted == 1)
		{
			ReleaseCapture();
			App->CL_MeshViewer->RenderListener->Pl_LeftMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}

		return 1;
	}
	
	}

	return FALSE;
}

// *************************************************************************
// *			Add_Resources:- Terry and Hazel Flanigan 2024 		 	   *
// *************************************************************************
bool CL64_MeshViewer::Add_Resources()
{
	bool Test = Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(m_Resource_Folder_Full, MV_Resource_Group);

	if (Test == 0)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(m_Resource_Folder_Full, "FileSystem", MV_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(MV_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(MV_Resource_Group);

	}

	App->CL_Resources->mSelected_Resource_Group = "MV_Resource_Group";
	flag_MV_Resource_Path_Loaded = 1;

	return 1;
}

// *************************************************************************
// *		Delete_Resources_Group:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
bool CL64_MeshViewer::Delete_Resources_Group()
{

	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(MV_Resource_Group);

	// if scene loaded need to goto project resources
	App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";
	flag_MV_Resource_Path_Loaded = 0;

	return 1;
}

// *************************************************************************
// *			Get_Stock_Folders:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Get_Stock_Folders(HWND DropHwnd)
{
	SendMessage(DropHwnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Project_Assets");

	char Path[MAX_PATH]{ 0 };

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(Path, App->GD_Directory_FullPath);
	strcat(Path, "\\Stock\\*.*");

	hFind = FindFirstFile(Path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				{

				}
				else
				{
					SendMessage(DropHwnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)FindFileData.cFileName);
				}

			}
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}

	// -------------------------------------------------- Colectables
	if (Mesh_Viewer_Mode == Enums::Mesh_Viewer_Collectables)
	{
		strcpy(m_Just_Folder, "Collectables");

		strcpy(m_Resource_Folder_Full, App->GD_Directory_FullPath); // Full Path Stock Folders 
		strcat(m_Resource_Folder_Full, "\\Stock\\");
		strcat(m_Resource_Folder_Full, m_Just_Folder);
		strcat(m_Resource_Folder_Full, "\\");

		SendMessage(DropHwnd, CB_SELECTSTRING, -1, (LPARAM)"Collectables");

		return;
	}

	// -------------------------------------------------- Objects
	if (Mesh_Viewer_Mode == Enums::Mesh_Viewer_Objects)
	{
		SendMessage(DropHwnd, CB_SETCURSEL, 0, 0);

		int Index = SendMessage(DropHwnd, CB_GETCURSEL, 0, 0); // Default Project Assets
		SendMessage(CB_hWnd, CB_GETLBTEXT, Index, (LPARAM)m_Just_Folder);

		int cmp = strcmp(m_Just_Folder, "Project_Assets");
		if (cmp == 0)
		{
			strcpy(m_Resource_Folder_Full, App->CL_Project->m_Main_Assets_Path); // Projects Full Resource Path
			return;
		}

		strcpy(m_Resource_Folder_Full, App->GD_Directory_FullPath); // Full Path Stock Folders 
		strcat(m_Resource_Folder_Full, "\\Stock\\");
		strcat(m_Resource_Folder_Full, m_Just_Folder);
		strcat(m_Resource_Folder_Full, "\\");
	}

	// -------------------------------------------------- Areas
	if (Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area)
	{
		strcpy(m_Just_Folder, "Areas");
		strcpy(m_Resource_Folder_Full, App->GD_Directory_FullPath); // Full Path Stock Folders 
		strcat(m_Resource_Folder_Full, "\\Stock\\");
		strcat(m_Resource_Folder_Full, m_Just_Folder);
		strcat(m_Resource_Folder_Full, "\\");

		SendMessage(DropHwnd, CB_SELECTSTRING, -1, (LPARAM)"Areas");

	}
	
}

// *************************************************************************
// *			Get_Mesh_Files:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_MeshViewer::Get_Mesh_Files()
{
	SendMessage(ListHwnd, LB_RESETCONTENT, 0, 0);

	char Path[MAX_PATH]{ 0 };

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(Path, m_Resource_Folder_Full);
	strcat(Path, "*.*");

	hFind = FindFirstFile(Path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.dwFileAttributes)
			{
				if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				{

				}
				else
				{
					if (_stricmp(FindFileData.cFileName + strlen(FindFileData.cFileName) - 5, ".mesh") == 0)
					{
						SendMessage(ListHwnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)FindFileData.cFileName);
					}
				}

			}
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}

	char buff[MAX_PATH]{ 0 };
	SendDlgItemMessage(MainDlgHwnd, IDC_LISTFILES, LB_GETTEXT, (WPARAM)0, (LPARAM)buff);
	SetDlgItemText(MainDlgHwnd, IDC_SELECTEDNAME, buff);

	strcpy(Selected_MeshFile, buff);

	//App->SBC_MeshViewer->Update_Mesh(App->SBC_MeshViewer->Selected_MeshFile);
}

// *************************************************************************
// *			Update_Mesh:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void CL64_MeshViewer::Show_Mesh(char* MeshFile)
{
	if (Ogre_MvEnt || Ogre_MvNode)
	{
		Ogre_MvNode->detachAllObjects();
		Ogre_MV_SceneMgr->destroySceneNode(Ogre_MvNode);
		Ogre_MV_SceneMgr->destroyEntity(Ogre_MvEnt);
		Ogre_MvEnt = NULL;
		Ogre_MvNode = NULL;
	}

	if (Ogre_MvEnt == NULL)
	{
		Ogre_MvEnt = Ogre_MV_SceneMgr->createEntity("MV", MeshFile, MV_Resource_Group);
		Ogre_MvNode = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode();
		Ogre_MvNode->attachObject(Ogre_MvEnt);
		Ogre_MvNode->setPosition(0, 0, 0);
	}

	App->CL_Ogre->Log_Message_To_File((LPSTR)"Show Mesh Create Entity");

	//if (App->SBC_MeshViewer->View_Zoomed_Flag == 1)
	{
		Ogre::Vector3 Centre = Ogre_MvEnt->getBoundingBox().getCenter();
		Ogre::Real Radius = Ogre_MvEnt->getBoundingRadius();

		Ogre_MV_CamNode->setPosition(0, Centre.y, Radius * 2.5);
		Ogre_MV_CamNode->lookAt(Ogre::Vector3(0, Centre.y, 0), Ogre::Node::TS_WORLD);
	}

	Get_Ogre_Mesh_Data(Ogre_MvEnt);

	Clear_Debug_Shape();
	MV_btDebug_Node->setOrientation(Ogre::Quaternion::IDENTITY);

	if (Physics_Shape == Enums::Shape_Box)
	{
		Show_Physics_Box();
	}

	if (Physics_Shape == Enums::Shape_Sphere)
	{
		Show_Physics_Sphere();
	}

	if (Physics_Shape == Enums::Shape_Capsule)
	{
		Show_Physics_Capsule();
	}

	if (Physics_Shape == Enums::Shape_Cylinder)
	{
		Show_Physics_Cylinder();
	}

	if (Physics_Shape == Enums::Shape_Cone)
	{
		Show_Physics_Cone();
	}
}

// *************************************************************************
// *		Show_Physics_Trimesh:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Show_Physics_Trimesh()
{
#pragma warning(disable : 4996) // Nightmare why
	Clear_Debug_Shape();

	if (Ogre_MV_Phys_Body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(Ogre_MV_Phys_Body);
		Ogre_MV_Phys_Body = nullptr;
	}

	// Get the mesh from the entity
	Ogre::MeshPtr myMesh = Ogre_MvEnt->getMesh();
	Ogre::Mesh::SubMeshIterator SubMeshIter = myMesh->getSubMeshIterator();

	// Create the triangle mesh
	btTriangleMesh* triMesh = NULL;
	btVector3 vert0, vert1, vert2;
	int i = 0;

	while (SubMeshIter.hasMoreElements())
	{
		i = 0;
		Ogre::SubMesh* subMesh = SubMeshIter.getNext();
		Ogre::IndexData* indexData = subMesh->indexData;
		Ogre::VertexData* vertexData = subMesh->vertexData;

		// -------------------------------------------------------
		// Get the position element
		const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		// Get a pointer to the vertex buffer
		Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
		// Get a pointer to the index buffer
		Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;

		// -------------------------------------------------------
		// The vertices and indices used to create the triangle mesh
		std::vector<Ogre::Vector3> vertices;
		vertices.reserve(vertexData->vertexCount);
		std::vector<unsigned long> indices;
		indices.reserve(indexData->indexCount);

		// -------------------------------------------------------
		// Lock the Vertex Buffer (READ ONLY)
		unsigned char* vertex = static_cast<unsigned char*> (vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		float* pReal = NULL;

		for (size_t j = 0; j < vertexData->vertexCount; ++j, vertex += vBuffer->getVertexSize()) {
			posElem->baseVertexPointerToElement(vertex, &pReal);
			Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

			vertices.push_back(pt);
		}
		vBuffer->unlock();
		// -------------------------------------------------------
		bool use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		// -------------------------------------------------------
		// Lock the Index Buffer (READ ONLY)
		unsigned long* pLong = static_cast<unsigned long*> (iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*> (pLong);

		if (use32bitindexes) {
			for (size_t k = 0; k < indexData->indexCount; ++k) {
				//
				indices.push_back(pLong[k]);
			}
		}
		else {
			for (size_t k = 0; k < indexData->indexCount; ++k) {
				//
				indices.push_back(static_cast<unsigned long> (pShort[k]));
			}
		}
		iBuffer->unlock();

		// -------------------------------------------------------
		// We now have vertices and indices ready to go
		// ----

		if (triMesh == nullptr)
		{
			triMesh = new btTriangleMesh(use32bitindexes);
		}

		for (size_t y = 0; y < indexData->indexCount / 3; y++) {
			// Set each vertex
			vert0.setValue(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
			vert1.setValue(vertices[indices[i + 1]].x, vertices[indices[i + 1]].y, vertices[indices[i + 1]].z);
			vert2.setValue(vertices[indices[i + 2]].x, vertices[indices[i + 2]].y, vertices[indices[i + 2]].z);

			// Add the triangle into the triangle mesh
			triMesh->addTriangle(vert0, vert1, vert2);

			// Increase index count
			i += 3;
		}

		//App->Say("here");
	}

	const bool useQuantizedAABB = true;
	btBvhTriangleMeshShape* mShape = new btBvhTriangleMeshShape(triMesh, false, true);
	//mShape->buildOptimizedBvh();

	float x = Ogre_MvNode->getPosition().x;
	float y = Ogre_MvNode->getPosition().y;
	float z = Ogre_MvNode->getPosition().z;

	btVector3 inertia(0, 0, 0);
	mShape->calculateLocalInertia(0.0, inertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
	btVector3 initialPosition(x, y, z);
	startTransform.setOrigin(initialPosition);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);


	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI
	(
		0,  // mass
		myMotionState,// initial position
		mShape,      // collision shape of body
		inertia   // local inertia
	);

	Ogre_MV_Phys_Body = new btRigidBody(rigidBodyCI);
	Ogre_MV_Phys_Body->clearForces();
	Ogre_MV_Phys_Body->setLinearVelocity(btVector3(0, 0, 0));
	Ogre_MV_Phys_Body->setAngularVelocity(btVector3(0, 0, 0));
	Ogre_MV_Phys_Body->setWorldTransform(startTransform);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Ogre_MV_Phys_Body);

}

// *************************************************************************
// *			Show_Physics_None:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Show_Physics_None()
{
	Clear_Debug_Shape();

	if (Ogre_MV_Phys_Body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(Ogre_MV_Phys_Body);
		Ogre_MV_Phys_Body = nullptr;
	}

	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
	App->CL_Ogre->RenderFrame(1);
	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;
}

// *************************************************************************
// *			Show_Physics_Box:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Show_Physics_Box()
{
	Clear_Debug_Shape();

	if (Ogre_MV_Phys_Body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(Ogre_MV_Phys_Body);
		Ogre_MV_Phys_Body = nullptr;
	}

	Ogre::Vector3 Centre = Ogre_MvEnt->getWorldBoundingBox(true).getCenter();
	

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 0, 1));

	btScalar mass;
	mass = 0.0f;

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);
	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Ogre_MvNode);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	btCollisionShape* newRigidShape = new btBoxShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->CL_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Ogre_MV_Phys_Body = new btRigidBody(rbInfo);
	Ogre_MV_Phys_Body->setRestitution(1.0);
	Ogre_MV_Phys_Body->setFriction(1.5);
	Ogre_MV_Phys_Body->setUserPointer(Ogre_MvNode);
	Ogre_MV_Phys_Body->setWorldTransform(startTransform);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Ogre_MV_Phys_Body);

	Set_Physics();
}

// *************************************************************************
// *		Show_Physics_Sphere:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MeshViewer::Show_Physics_Sphere()
{
	Clear_Debug_Shape();

	if (Ogre_MV_Phys_Body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(Ogre_MV_Phys_Body);
		Ogre_MV_Phys_Body = nullptr;
	}

	Ogre::Vector3 Centre = Ogre_MvEnt->getWorldBoundingBox(true).getCenter();
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	mass = 0.0f;


	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	float Radius = App->CL_Com_Objects->GetMesh_BB_Radius(Ogre_MvNode);

	btCollisionShape* newRigidShape = new btSphereShape(Radius);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->CL_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Ogre_MV_Phys_Body = new btRigidBody(rbInfo);
	Ogre_MV_Phys_Body->setRestitution(1.0);
	Ogre_MV_Phys_Body->setFriction(1.5);
	Ogre_MV_Phys_Body->setUserPointer(Ogre_MvNode);
	Ogre_MV_Phys_Body->setWorldTransform(startTransform);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Ogre_MV_Phys_Body);

	Set_Physics();
}

// *************************************************************************
// *		Show_Physics_Capsule:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Show_Physics_Capsule()
{
	Clear_Debug_Shape();

	if (Ogre_MV_Phys_Body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(Ogre_MV_Phys_Body);
		Ogre_MV_Phys_Body = nullptr;
	}

	Ogre::Vector3 Centre = Ogre_MvEnt->getWorldBoundingBox(true).getCenter();

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	mass = 0.0f;

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Ogre_MvNode);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	float Radius = App->CL_Com_Objects->GetMesh_BB_Radius(Ogre_MvNode);

	btCollisionShape* newRigidShape = new btCapsuleShape(Radius, sy);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->SBC_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Ogre_MV_Phys_Body = new btRigidBody(rbInfo);
	Ogre_MV_Phys_Body->setRestitution(1.0);
	Ogre_MV_Phys_Body->setFriction(1.5);
	Ogre_MV_Phys_Body->setUserPointer(Ogre_MvNode);
	Ogre_MV_Phys_Body->setWorldTransform(startTransform);


	App->CL_Bullet->dynamicsWorld->addRigidBody(Ogre_MV_Phys_Body);

	Set_Physics();
}

// *************************************************************************
// *		Show_Physics_Cylinder:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Show_Physics_Cylinder()
{
	Clear_Debug_Shape();

	if (Ogre_MV_Phys_Body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(Ogre_MV_Phys_Body);
		Ogre_MV_Phys_Body = nullptr;
	}

	Ogre::Vector3 Centre = Ogre_MvEnt->getWorldBoundingBox(true).getCenter();

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	mass = 0.0f;


	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Ogre_MvNode);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	float Radius = App->CL_Com_Objects->GetMesh_BB_Radius(Ogre_MvNode);

	btCollisionShape* newRigidShape = new btCylinderShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->SBC_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Ogre_MV_Phys_Body = new btRigidBody(rbInfo);
	Ogre_MV_Phys_Body->setRestitution(1.0);
	Ogre_MV_Phys_Body->setFriction(1.5);
	Ogre_MV_Phys_Body->setUserPointer(Ogre_MvNode);
	Ogre_MV_Phys_Body->setWorldTransform(startTransform);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Ogre_MV_Phys_Body);

	Set_Physics();
}

// *************************************************************************
// *		Show_Physics_Cone:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MeshViewer::Show_Physics_Cone()
{

	Clear_Debug_Shape();
	if (Ogre_MV_Phys_Body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(Ogre_MV_Phys_Body);
		Ogre_MV_Phys_Body = nullptr;
	}

	Ogre::Vector3 Centre = Ogre_MvEnt->getWorldBoundingBox(true).getCenter();

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	mass = 0.0f;

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Ogre_MvNode);
	float sx = Size.x / 2;
	float sy = Size.y;// / 2;
	float sz = Size.z / 2;

	float Radius = App->CL_Com_Objects->GetMesh_BB_Radius(Ogre_MvNode);

	btCollisionShape* newRigidShape = new btConeShape(Radius, sy);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->SBC_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Ogre_MV_Phys_Body = new btRigidBody(rbInfo);
	Ogre_MV_Phys_Body->setRestitution(1.0);
	Ogre_MV_Phys_Body->setFriction(1.5);
	Ogre_MV_Phys_Body->setUserPointer(Ogre_MvNode);
	Ogre_MV_Phys_Body->setWorldTransform(startTransform);

	App->CL_Bullet->dynamicsWorld->addRigidBody(Ogre_MV_Phys_Body);

	Set_Physics();

}

// *************************************************************************
// *			Clear_Debug_Shape:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Clear_Debug_Shape()
{
	MV_btDebug_Manual->beginUpdate(0);
	MV_btDebug_Manual->position(0, 0, 0);
	MV_btDebug_Manual->colour(1, 1, 1);
	MV_btDebug_Manual->position(0, 0, 0);
	MV_btDebug_Manual->colour(1, 1, 1);
	MV_btDebug_Manual->end();

	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
	App->CL_Ogre->RenderFrame(1);
	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;
}

// *************************************************************************
//				Set_Physics:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_MeshViewer::Set_Physics()
{
	MV_btDebug_Node->resetOrientation();
	Ogre_MvNode->resetOrientation();
	// Rotation
	Ogre::Quaternion Rot = Ogre_MvNode->getOrientation();

	float w = Rot.w;
	float x = Rot.x;
	float y = Rot.y;
	float z = Rot.z;

	Ogre_MV_Phys_Body->getWorldTransform().setRotation(btQuaternion(x, y, z, w));

	// Scale
	Ogre_MvNode->setScale(1,1,1);

	Ogre::Vector3 Scale = Ogre_MvNode->getScale();
	Ogre_MV_Phys_Body->getCollisionShape()->setLocalScaling(btVector3(Scale.x, Scale.y, Scale.z));

	// Position
	Ogre::Vector3 Centre = Ogre_MvEnt->getWorldBoundingBox(true).getCenter();
	Ogre_MV_Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));

}

// *************************************************************************
// *			Enable_ShapeButtons:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Enable_ShapeButtons(bool state)
{
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_BOX), state);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_SPHERE), state);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CAPSULE), state);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CYLINDER), state);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CONE), state);

	flag_Selected_Shape_Box = 0;
	flag_Selected_Shape_Sphere = 0;
	flag_Selected_Shape_Capsule = 0;
	flag_Selected_Shape_Cylinder = 0;
	flag_Selected_Shape_Cone = 0;
}

// *************************************************************************
// *	  			Copy_Assets:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MeshViewer::Copy_Assets()
{
	int Count = 0;
	std::vector<std::string> Test_Texture_Names;
	Test_Texture_Names.reserve(20);

	// ------------------ Textures
	while (Count < Texure_Count)
	{
		strcpy(DestinationFile, App->CL_Project->m_Main_Assets_Path);
		strcat(DestinationFile, v_Texture_Names[Count].c_str());
		
		if (std::find(Test_Texture_Names.begin(), Test_Texture_Names.end(), v_Texture_Names[Count]) != Test_Texture_Names.end()) 
		{
			// Found Skip Anding Texture again or we will crash
		}
		else
		{
			bool test = Check_if_Files_Exsit((LPSTR)v_Texture_Names[Count].c_str());
			if (test == 1)
			{
				//App->Say("File Exsists", (LPSTR)v_Texture_Names[Count].c_str());
				Test_Texture_Names.push_back(v_Texture_Names[Count]);
			}
			else
			{
				//App->Say("Creating", (LPSTR)v_Texture_Names[Count].c_str());
				strcpy(SourceFile, App->CL_MeshViewer->m_Resource_Folder_Full);
				strcat(SourceFile, v_Texture_Names[Count].c_str());
				CopyFile(SourceFile, DestinationFile, false);
				//Ogre::ResourceGroupManager::getSingleton().declareResource(,App->CL_Resources->Project_Resource_Group);

				Create_Texture(SourceFile, (LPSTR)v_Texture_Names[Count].c_str());

				Test_Texture_Names.push_back(v_Texture_Names[Count]);

			}
		}

		Count++;
	}


	// ------------------ Material
	bool test = Check_if_Files_Exsit((LPSTR)m_Material_File);
	if (test == 1)
	{
		//App->Say("File Exsists", (LPSTR)m_Material_File);
		
	}
	else
	{
		strcpy(SourceFile, m_Resource_Folder_Full);
		strcat(SourceFile, m_Material_File);
		Create_Material(SourceFile);

		strcpy(DestinationFile, App->CL_Project->m_Main_Assets_Path);
		strcat(DestinationFile, App->CL_MeshViewer->m_Material_File);
		CopyFile(SourceFile, DestinationFile, false);
	}


	// ------------------ Mesh
	test = Check_if_Files_Exsit(Selected_MeshFile);
	if (test == 1)
	{
			//App->Say("File Exsists", Selected_MeshFile);
	}
	else
	{
		strcpy(SourceFile, m_Resource_Folder_Full);
		strcat(SourceFile, Selected_MeshFile);
		Create_Mesh(SourceFile);

		strcpy(DestinationFile, App->CL_Project->m_Main_Assets_Path);
		strcat(DestinationFile, App->CL_MeshViewer->Selected_MeshFile);
		CopyFile(SourceFile, DestinationFile, false);
	}

	
	App->CL_Ogre->RenderFrame(8);
	//App->Say("initialiseResourceGroup");
}

// *************************************************************************
// *			Create_Texture:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_MeshViewer::Create_Texture(char* Texture_Path, char* Texture_Name)
{
	Ogre::TextureManager::getSingleton().setVerbose(true);

	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(App->CL_Resources->Project_Resource_Group,m_Material_File))
	{
		
		//App->Say("Resource Exsist", m_Material_File);

		Ogre::ResourcePtr RP = NULL;
		RP = Ogre::TextureManager::getSingleton().getResourceByName(Texture_Name, App->CL_Resources->Project_Resource_Group);

		if (RP)
		{
			bool test = RP->isLoaded();
			if (test == 1)
			{
				//App->Say(Texture_Name, (LPSTR)"Loaded");
				Ogre::TextureManager::getSingleton().remove(Texture_Name, App->CL_Resources->Project_Resource_Group);
				App->Say("Remove");
				//return 0;
			}
			else
			{
				RP->isPrepared();
				
				bool tt = RP->isPrepared();
				if (tt == 1)
				{
					//App->Say(Texture_Name, (LPSTR)"Is Prepared");
				}

			}
		}
	}

	Ogre::String source = Texture_Path;
	FILE* pFile = fopen(source.c_str(), "rb");
	
	if (!pFile)
	{
		OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "File " + source + " not found.", "OgreMeshLoaded");
	}
	else
	{
		struct stat tagStat;
		stat(source.c_str(), &tagStat);
		Ogre::MemoryDataStream* memstream = new MemoryDataStream(source, tagStat.st_size, true);
		fread((void*)memstream->getPtr(), tagStat.st_size, 1, pFile);
		fclose(pFile);

		Ogre::DataStreamPtr stream(memstream);

		Ogre::Image mm;
		mm.load(stream);

		try
		{
			Ogre::TextureManager::getSingleton().loadImage(Texture_Name, App->CL_Resources->Project_Resource_Group, mm);
		}
		catch (Ogre::Exception& Ex)
		{
	
			Ogre::TextureManager::getSingleton().remove(Texture_Name, App->CL_Resources->Project_Resource_Group);
			Ogre::TextureManager::getSingleton().loadImage(Texture_Name, App->CL_Resources->Project_Resource_Group, mm);

		}

	}

	return 1;
}

// *************************************************************************
// *			Create_Material:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_MeshViewer::Create_Material(char* File)
{
	Ogre::String source = File;

	FILE* pFile = fopen(source.c_str(), "rt");

	if (!pFile)
	{
		OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "File " + source + " not found.", "OgreMeshLoaded");
	}
	else
	{
		struct stat tagStat;
		stat(source.c_str(), &tagStat);
		Ogre::MemoryDataStream* memstream = new MemoryDataStream(source, tagStat.st_size, true);
		fread((void*)memstream->getPtr(), tagStat.st_size, 1, pFile);
		fclose(pFile);

		Ogre::DataStreamPtr stream(memstream);

		try
		{
			Ogre::ScriptCompilerManager::getSingleton().parseScript(stream, App->CL_Resources->Project_Resource_Group);
		}
		catch (Ogre::Exception& Ex)
		{
			App->Say_Win(Ex.what());
		}

	}

	return 1;
}

// *************************************************************************
// *				Create_Mesh:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_MeshViewer::Create_Mesh(char* File)
{
	Ogre::String source = File;

	FILE* pFile = fopen(source.c_str(), "rb");

	if (!pFile)
	{
		OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "File " + source + " not found.", "OgreMeshLoaded");
	}
	else
	{
		struct stat tagStat;
		stat(source.c_str(), &tagStat);
		MemoryDataStream* memstream = new MemoryDataStream(source, tagStat.st_size, true);
		fread((void*)memstream->getPtr(), tagStat.st_size, 1, pFile);
		fclose(pFile);

		MeshPtr pMesh = MeshManager::getSingleton().createManual(Selected_MeshFile, App->CL_Resources->Project_Resource_Group);

		MeshSerializer meshSerializer;
		DataStreamPtr stream(memstream);
		meshSerializer.importMesh(stream, pMesh.getPointer());
		
		
		/*Ogre::Entity* Testxx_1 = App->CL_Ogre->mSceneMgr->createEntity("LocalMesh_Ent", Selected_MeshFile,App->CL_Resources->Project_Resource_Group);
		Ogre::SceneNode* vNode = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		vNode->attachObject(Testxx_1);
		vNode->setVisible(true);
		vNode->scale(1, 1, 1);*/

	}

	return 1;
}


// *************************************************************************
// *		Check_if_Files_Exsit:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_MeshViewer::Check_if_Files_Exsit(char* File)
{
	char CheckFile[MAX_PATH]{ 0 };

	strcpy(CheckFile, App->CL_Project->m_Main_Assets_Path);
	strcat(CheckFile, File);

	bool test = App->CL_File_IO->Check_File_Exist(CheckFile);
	if (test == 1)
	{
		return 1;
	}

	return 0;
}

// *************************************************************************
// *	    	Get_Ogre_Mesh_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Get_Ogre_Mesh_Data(Ogre::Entity* Ogre_Entity)
{
	App->CL_MeshViewer->m_Material_File[0] = 0;

	v_Texture_Names.resize(0);
	v_Scrip_Names.resize(0);

	Texure_Count = 0;
	NumSub_Meshes = 0;

	int Count = 0;

	int NumSubEnts = Ogre_Entity->getNumSubEntities();
	
	NumSub_Meshes = NumSubEnts;

	if (NumSubEnts > 0)
	{
		bool test = Ogre_Entity->getSubEntity(0)->getMaterial()->isLoaded();

		if (test == 1)
		{
			strcpy(m_Material_File, Ogre_Entity->getSubEntity(0)->getMaterial()->getOrigin().c_str());

			bool test2 = strcmp(m_Material_File, "");
			if (test2 == 0)
			{
				strcpy(m_Material_File, "No_Material_File");
				v_Texture_Names.push_back("No Texture");

				Texure_Count = v_Texture_Names.size();
				return;
			}
		}
		else
		{
			strcpy(m_Material_File, "No_Material_File");
			v_Texture_Names.push_back("No Texture");

			Texure_Count = v_Texture_Names.size();
			return;
		}
	}
	
	while (Count < NumSubEnts)
	{
		// Material
		char mMaterial[MAX_PATH];
		Ogre::SubEntity* subEnt = Ogre_Entity->getSubEntity(Count);
		strcpy(mMaterial, subEnt->getMaterialName().c_str());


		// Texture
		char mTexture[MAX_PATH];
		char mMaterial_Script[MAX_PATH];
		Ogre::MaterialPtr MatCurent;
		MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(mMaterial));

		if (MatCurent->getNumTechniques() > 0)
		{
			int TUSCount = MatCurent->getTechnique(0)->getPass(0)->getNumTextureUnitStates();

			
			if (TUSCount > 0)
			{
				strcpy(mTexture, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());
				strcpy(mMaterial_Script, MatCurent->getName().c_str());
				v_Texture_Names.push_back(mTexture);
				v_Scrip_Names.push_back(mMaterial_Script);

				Texure_Count = v_Texture_Names.size();
				Material_Scripts_Count = v_Scrip_Names.size();
			}
		}
		
		Count++;

	}

}

// *************************************************************************
// *	Add_Resource_Location_Project:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_MeshViewer::Add_Resource_Location_Project(char* Resource_Location)
{
	bool Test = Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(Resource_Location, App->CL_Resources->Project_Resource_Group);

	if (Test == 0)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Resource_Location, "FileSystem", App->CL_Resources->Project_Resource_Group);
		//Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(App->CL_Resources->Project_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Resources->Project_Resource_Group);
		//Ogre::ResourceGroupManager::getSingleton().prepareResourceGroup(App->CL_Resources->Project_Resource_Group);
		//App->Say("poo");
	}

	return 1;
}

// *************************************************************************
// *	  	Show_Mesh_Properties:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Show_Mesh_Properties()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_LISTDATA, MainDlgHwnd, (DLGPROC)Proc_Mesh_Properties);
}

// *************************************************************************
// *		Proc_Mesh_Properties:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_MeshViewer::Proc_Mesh_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LISTGROUP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		HWND List = GetDlgItem(hDlg, IDC_LISTGROUP);
		ListView_DeleteAllItems(List);

		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)App->CL_MeshViewer->m_Resource_Folder_Full);
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"");
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)App->CL_MeshViewer->Selected_MeshFile);

		char buf[20];
		char buf2[20];
		strcpy(buf2, "Submeshs: ");
		itoa(App->CL_MeshViewer->NumSub_Meshes, buf, 10);
		strcat(buf2, buf);
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buf2);
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"");
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)App->CL_MeshViewer->m_Material_File);
		
		int Count = 0;
		char buf4[100];
		while (Count < App->CL_MeshViewer->Texure_Count)
		{
			sprintf(buf4, "%s %s", App->CL_MeshViewer->v_Scrip_Names[Count].c_str(), App->CL_MeshViewer->v_Texture_Names[Count].c_str());
			SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buf4);

			Count++;
		}

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
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

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}

	return FALSE;
}
