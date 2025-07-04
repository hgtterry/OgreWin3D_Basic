/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
#include "CL64_Editor_Scene.h"

#define IDM_3D_MAP_EDITOR 1

CL64_Editor_Scene::CL64_Editor_Scene()
{
	flag_Environment_Available = 0;
	flag_Show_Physics_Objects = 0;

	Scene_Headers_hWnd = NULL;
	hMenu = NULL;
}

CL64_Editor_Scene::~CL64_Editor_Scene()
{
}

// *************************************************************************
// *	  		Start_Headers_Scene:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Editor_Scene::Start_Headers_Scene()
{
	Scene_Headers_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOP_TABS_HEADERS_SCENE, App->MainHwnd, (DLGPROC)Proc_Headers_Scene);
}

// *************************************************************************
// *        	Proc_Headers_Scene:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Editor_Scene::Proc_Headers_Scene(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_MAP_EDITOR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_HD_PREVIEW_ED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_SCENE_FIRST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_SCENE_FREE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_SCENE_RESET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_MAP_EDITOR)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_HD_PREVIEW_ED)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_SCENE_FIRST)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item,App->CL_Camera->flag_First_Person);

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_SCENE_FREE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Camera->flag_Free);

			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_SCENE_RESET)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);

			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_MAP_EDITOR)
		{
			App->CL_Camera->SetCameraMode_Free();
			App->CL_Editor_Control->Return_To_Map_Editor();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HD_PREVIEW_ED)
		{
			App->CL_Editor_Control->Start_Preview_Mode();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_SCENE_FIRST)
		{
			App->CL_Camera->SetCameraMode_FirstPerson();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_SCENE_FREE)
		{
			App->CL_Camera->SetCameraMode_Free();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_SCENE_RESET)
		{
			App->CL_Physics->Reset_Scene(false);
			return TRUE;
		}
		
	}
	}

	return FALSE;
}

// **************************************************************************
// *			Show_Headers:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_Editor_Scene::Show_Headers(bool Enable)
{
	if (Enable == 1)
	{
		ShowWindow(Scene_Headers_hWnd, 1);
	}
	else
	{
		ShowWindow(Scene_Headers_hWnd, 0);
	}
}

// *************************************************************************
// *			Return_From_Preview:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Editor_Scene::Return_From_Preview(void)
{
	// Set view flags
	auto& topTabs = App->CL_Top_Tabs;
	topTabs->flag_Full_View_3D = true;
	topTabs->flag_View_Top_Left = false;
	topTabs->flag_View_Top_Right = false;
	topTabs->flag_View_Bottom_Left = false;

	App->CL_Editor_Map->Init_Views(Enums::Selected_View_3D);
	App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_Dlg_Hwnd, 
									   App->CL_Editor_Map->nleftWnd_width, 
									   App->CL_Editor_Map->nleftWnd_Depth);

	RECT rcl;

	GetClientRect(App->CL_Editor_Map->Bottom_Right_Hwnd, &rcl);

	SetWindowPos(App->ViewGLhWnd, NULL, 0, 0, rcl.right, rcl.bottom, SWP_NOZORDER);

	App->CL_Ogre->mWindow->windowMovedOrResized();
	App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());
	App->CL_Com_Objects->Show_Entities(true);
	App->CL_Gizmos->Show_MarkerBox(true);

	// TODO Recreating Physics Not Nessasery
	if (App->CL_Physics->flag_TriMesh_Created == 1)
	{
		App->CL_Physics->Clear_Trimesh();
	}

	if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
	{
		App->CL_Physics->Create_New_Trimesh(App->CL_Mesh_Mgr->World_Ent, App->CL_Mesh_Mgr->World_Node);
		App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 1;
	}
	
	App->CL_ImGui_Editor->flag_Block_GUI = false;
	App->CL_Camera->SetCameraMode_Free();
}

// *************************************************************************
// *			Context_Menu_Ogre:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Editor_Scene::Context_Menu_Ogre(HWND hDlg)
{
	RECT rcTree;
	TVHITTESTINFO htInfo = { 0 };
	POINT pt;
	GetCursorPos(&pt);

	long xPos = pt.x;   // x position from message, in screen coordinates
	long yPos = pt.y;   // y position from message, in screen coordinates 

	GetWindowRect(hDlg, &rcTree);        // get its window coordinates
	htInfo.pt.x = xPos - rcTree.left;      // convert to client coordinates
	htInfo.pt.y = yPos - rcTree.top;

	hMenu = CreatePopupMenu();

	AppendMenuW(hMenu, MF_STRING, IDM_3D_MAP_EDITOR, L"&Map Editor");
	
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	
	//flag_Context_Menu_Active = 1;
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
	//flag_Context_Menu_Active = 0;

	DestroyMenu(hMenu);
}

// *************************************************************************
// *		Context_Command_Ogre:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Editor_Scene::Context_Command_Ogre(WPARAM wParam)
{
	if (LOWORD(wParam) == IDM_3D_MAP_EDITOR)
	{
		App->CL_Editor_Control->Return_To_Map_Editor();
		return TRUE;
	}
}

