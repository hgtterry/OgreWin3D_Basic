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
#include "resource.h"
#include "CL64_TopDlg.h"

CL64_TopDlg::CL64_TopDlg(void)
{
	TabsHwnd =		nullptr;
	Tabs_TB_hWnd =	nullptr;

	// Tab Options
	Game_TB_hWnd =		nullptr;
	Camera_TB_hWnd =	nullptr;
	Physics_TB_hWnd =	nullptr;
	Motions_TB_hWnd =	nullptr;
	Locations_TB_hWnd = nullptr;

	flag_FPS_Dlg_Running = 0;
		
	flag_Toggle_Tabs_Game = 0;
	flag_Toggle_Tabs_Camera = 0;
	flag_Toggle_Tabs_Physics = 0;
	flag_Toggle_Tabs_Motions = 0;
	flag_Toggle_Tabs_Resources = 0;
	flag_Toggle_Tabs_Locations = 0;

	flag_Toggle_Cam_ModelMode = 0;
	flag_Toggle_Cam_FreeMode = 0;
	flag_Toggle_Cam_FirstMode = 1;

	flag_Toggle_PhysicaDebug_Node = 0;

	flag_ShowOnlySubFaces_Saved = 0;

	Selected_Motion_Name[0] = 0;

}

CL64_TopDlg::~CL64_TopDlg(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_TopDlg::Reset_Class(void) const
{
	App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 0;
	App->CL_Ogre->OGL_Listener->Flag_ShowPoints = 0;
	App->CL_Ogre->OGL_Listener->Flag_ShowBones = 0;
	App->CL_Ogre->OGL_Listener->Flag_ShowNormals = 0;
	App->CL_Ogre->OGL_Listener->Flag_ShowBoundingBox = 0;
	App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 1;

	SendMessage(GetDlgItem(TabsHwnd, IDC_BTSHOWTEXTURES), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);
	SendMessage(GetDlgItem(TabsHwnd, IDC_TBSHOWFACES), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);
	SendMessage(GetDlgItem(TabsHwnd, IDC_BTSHOWPOINTS), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOff_Bmp);
	SendMessage(GetDlgItem(TabsHwnd, IDC_BTSHOWBONES), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);
	SendMessage(GetDlgItem(TabsHwnd, IDC_BTSHOWNORMALS), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOff_Bmp);
	SendMessage(GetDlgItem(TabsHwnd, IDC_TBBOUNDBOX), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);

	App->CL_TopDlg->Enable_Info_Panel(false);
	App->CL_TopDlg->Enable_ImGui_Demo_Panel(false);
	App->CL_TopDlg->Enable_FPSLock_Dlg_Panel(false);

	Enable_Grid_Hair_Icons(true);
	
}

// **************************************************************************
// *	  		 Start_TopBar:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_TopDlg::Start_TopBar()
{
	TabsHwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOPBAR, App->MainHwnd, (DLGPROC)Proc_TopBar);
	Init_Bmps_Globals();
}

// *************************************************************************
// *			Proc_TopBar:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Proc_TopBar(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_TopDlg->TabsHwnd = hDlg;

		App->CL_TopDlg->Start_Tabs_Headers();
		App->CL_TopDlg->Start_Game_TB();
		App->CL_TopDlg->Start_Camera_TB();
		App->CL_TopDlg->Start_Physics_TB();
		App->CL_TopDlg->Start_Motions_TB();
		App->CL_TopDlg->Start_Locations_TB();

		App->CL_TopDlg->Hide_Tabs();

		// Default Tab
		App->CL_TopDlg->flag_Toggle_Tabs_Camera = 1;
		ShowWindow(App->CL_TopDlg->Camera_TB_hWnd, SW_SHOW);

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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_TBSHOWHAIR)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBSHOWGRID)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBSHOWFACES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBINFO)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBBOUNDBOX)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BTSHOWPOINTS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BTSHOWBONES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BTSHOWNORMALS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BTSHOWTEXTURES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_TBSHOWGRID)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_TBSHOWGRID);

			if (App->CL_Grid->flag_ShowGrid == 1)
			{
				App->CL_Grid->Grid_SetVisible(0);
				
				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOff_Bmp);
			}
			else
			{
				App->CL_Grid->Grid_SetVisible(1);
				
				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOn_Bmp);
			}

			return TRUE;
		}

		//-------------------------------------------------------- Show Hair
		if (LOWORD(wParam) == IDC_TBSHOWHAIR)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_TBSHOWHAIR);

			if (App->CL_Grid->flag_ShowHair == 1)
			{
				App->CL_Grid->Hair_SetVisible(0);

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOff_Bmp);
			}
			else
			{
				App->CL_Grid->Hair_SetVisible(1);

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOn_Bmp);
			}
			return TRUE;
		}

		//-------------------------------------------------------- Show Textures
		if (LOWORD(wParam) == IDC_BTSHOWTEXTURES)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWTEXTURES);


			/*if (App->CL_Scene->Scene_Mode == Enums::Scene_Mode_TestMesh)
			{
				if (App->CL_Ogre->flag_Show_Test_Mesh == 1)
				{
					App->CL_Ogre->Show_Test_Mesh(false);
					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOff_Bmp);
				}
				else
				{
					App->CL_Ogre->Show_Test_Mesh(true);
					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);
				}

				return TRUE;
			}

			if (App->CL_Scene->Scene_Mode == Enums::Scene_Mode_Imported_Entity)
			{
				App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 0;

				if (App->CL_Scene->flag_Show_Main_Entity == 1)
				{
					App->CL_Scene->Show_Main_Entity(false);
					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOff_Bmp);
				}
				else
				{
					App->CL_Scene->Show_Main_Entity(true);
					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);
				}

				return TRUE;
			}

			if (App->CL_Scene->flag_Model_Loaded == 1)
			{
	
				if (App->CL_Ogre->OGL_Listener->Flag_ShowTextured == 1)
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOff_Bmp);
				}
				else
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowTextured = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);
				}
			}*/

			return TRUE;
		}

		// Show Hide Mesh/Faces
		if (LOWORD(wParam) == IDC_TBSHOWFACES)
		{
			//if (App->CL_Scene->flag_Model_Loaded == 1)
			//{
			//	HWND Temp = GetDlgItem(hDlg, IDC_TBSHOWFACES);

			//	if (App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces == 1)
			//	{
			//		App->CL_TopDlg->flag_ShowOnlySubFaces_Saved = 1;
			//		App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 0;
			//		App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 1;
			//		SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOn_Bmp);
			//		return TRUE;
			//	}

			//	if (App->CL_Ogre->OGL_Listener->Flag_ShowFaces == 1)
			//	{
			//		if (App->CL_TopDlg->flag_ShowOnlySubFaces_Saved == 1)
			//		{
			//			App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 1;
			//			App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 1;
			//			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);
			//			return TRUE;
			//		}

			//		App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 0;

			//		SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);

			//	}
			//	else
			//	{
			//		/*if (App->CL_TopDlg->flag_ShowOnlySubFaces_Saved == 1)
			//		{
			//			App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 1;
			//		}*/

			//		App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 1;

			//		SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOn_Bmp);

			//	}

			//}

			return TRUE;
		}

		//-------------------------------------------------------- Show Points
		if (LOWORD(wParam) == IDC_BTSHOWPOINTS)
		{
			/*if (App->CL_Scene->flag_Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWPOINTS);

				if (App->CL_Ogre->OGL_Listener->Flag_ShowPoints == 1)
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowPoints = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOff_Bmp);
				}
				else
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowPoints = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOn_Bmp);
				}
			}*/

			return TRUE;
		}

		//-------------------------------------------------------- Show Info
		if (LOWORD(wParam) == IDC_TBINFO)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_TBINFO);

			/*if (App->CL_ImGui->flag_Show_Model_Data == 1)
			{
				App->CL_TopDlg->Enable_Info_Panel(false);
			}
			else
			{
				App->CL_TopDlg->Enable_Info_Panel(true);
			}*/

			return TRUE;
		}

		//-------------------------------------------------------- Show Bones
		if (LOWORD(wParam) == IDC_BTSHOWBONES)
		{
			/*if (App->CL_Scene->flag_Model_Loaded == 1 && App->CL_Scene->BoneCount > 0)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWBONES);

				if (App->CL_Ogre->OGL_Listener->Flag_ShowBones == 1)
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowBones = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);
				}
				else
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowBones = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOn_Bmp);
				}
			}*/
			return TRUE;
		}

		//-------------------------------------------------------- Show Normals
		if (LOWORD(wParam) == IDC_BTSHOWNORMALS)
		{
			/*if (App->CL_Scene->flag_Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWNORMALS);

				if (App->CL_Ogre->OGL_Listener->Flag_ShowNormals == 1)
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowNormals = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOff_Bmp);
				}
				else
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowNormals = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOn_Bmp);
				}
			}*/

			return TRUE;
		}

		//-------------------------------------------------------- Show Bound Box
		if (LOWORD(wParam) == IDC_TBBOUNDBOX)
		{
			/*if (App->CL_Scene->flag_Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_TBBOUNDBOX);

				if (App->CL_Ogre->OGL_Listener->Flag_ShowBoundingBox == 1)
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowBoundingBox = 0;
				
					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);
				}
				else
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowBoundingBox = 1;
					
					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOn_Bmp);
				}
			}*/

			return TRUE;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *		Start_Tabs_Headers:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_TopDlg::Start_Tabs_Headers(void)
{
	Tabs_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_TAB, TabsHwnd, (DLGPROC)Proc_Tabs_Headers);
}

// *************************************************************************
// *		Proc_Tabs_Headers:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Proc_Tabs_Headers(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{

		SendDlgItemMessage(hDlg, IDC_BT_TBH_GAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TBH_CAMERA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TD_PHYSICSTAB, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TD_MOTIONSTAB, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TD_RESOURCETAB, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TBH_LOCATIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TBH_GAME)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopDlg->flag_Toggle_Tabs_Game);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TBH_CAMERA)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopDlg->flag_Toggle_Tabs_Camera);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TD_PHYSICSTAB)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopDlg->flag_Toggle_Tabs_Physics);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TD_MOTIONSTAB)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopDlg->flag_Toggle_Tabs_Motions);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_TD_RESOURCETAB)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopDlg->flag_Toggle_Tabs_Resources);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TBH_LOCATIONS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopDlg->flag_Toggle_Tabs_Locations);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TBH_GAME)
		{
			App->CL_TopDlg->Hide_Tabs();
			ShowWindow(App->CL_TopDlg->Game_TB_hWnd, SW_SHOW);
			App->CL_TopDlg->flag_Toggle_Tabs_Game = 1;

			RedrawWindow(App->CL_TopDlg->Tabs_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_TBH_CAMERA)
		{
			App->CL_TopDlg->Hide_Tabs();
			ShowWindow(App->CL_TopDlg->Camera_TB_hWnd, SW_SHOW);
			App->CL_TopDlg->flag_Toggle_Tabs_Camera = 1;

			RedrawWindow(App->CL_TopDlg->Tabs_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TD_PHYSICSTAB)
		{
			App->CL_TopDlg->Hide_Tabs();
			ShowWindow(App->CL_TopDlg->Physics_TB_hWnd, SW_SHOW);
			App->CL_TopDlg->flag_Toggle_Tabs_Physics = 1;

			RedrawWindow(App->CL_TopDlg->Tabs_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TD_MOTIONSTAB)
		{
			App->CL_TopDlg->Hide_Tabs();
			ShowWindow(App->CL_TopDlg->Motions_TB_hWnd, SW_SHOW);
			App->CL_TopDlg->flag_Toggle_Tabs_Motions = 1;

			RedrawWindow(App->CL_TopDlg->Tabs_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_TD_RESOURCETAB)
		{
			App->CL_Resources->Start_Resources();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TBH_LOCATIONS)
		{
			App->CL_TopDlg->Hide_Tabs();
			ShowWindow(App->CL_TopDlg->Locations_TB_hWnd, SW_SHOW);
			App->CL_TopDlg->flag_Toggle_Tabs_Locations = 1;

			RedrawWindow(App->CL_TopDlg->Tabs_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}
		
	}
	}
	return FALSE;
}

// *************************************************************************
// *				Hide_Tabs:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_TopDlg::Hide_Tabs(void)
{
	ShowWindow(Game_TB_hWnd, SW_HIDE);
	ShowWindow(Camera_TB_hWnd, SW_HIDE);
	ShowWindow(Physics_TB_hWnd, SW_HIDE);
	ShowWindow(Motions_TB_hWnd, SW_HIDE);
	ShowWindow(Locations_TB_hWnd, SW_HIDE);
	
	flag_Toggle_Tabs_Game = 0;
	flag_Toggle_Tabs_Camera = 0;
	flag_Toggle_Tabs_Physics = 0;
	flag_Toggle_Tabs_Motions = 0;
	flag_Toggle_Tabs_Locations = 0;

}

// *************************************************************************
// *			Start_Camera_TB:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_TopDlg::Start_Camera_TB(void)
{
	Camera_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_CAMERA, Tabs_TB_hWnd, (DLGPROC)Proc_Camera_TB);
}

// *************************************************************************
// *			Proc_Camera_TB:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Proc_Camera_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_CAMERA_MODEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CAMERA_FREE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TD_DEBUG_RESETVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CAMERA_CAMDATA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CAMERA_FIRST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CAMERA_SPEED1, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Tabs;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_CAMERA_MODEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_CAMERA_MODEL));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_TopDlg->flag_Toggle_Cam_ModelMode);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CAMERA_FREE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_TopDlg->flag_Toggle_Cam_FreeMode);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CAMERA_FIRST)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_CAMERA_FIRST));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_TopDlg->flag_Toggle_Cam_FirstMode);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CAMERA_CAMDATA)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_ImGui->flag_Show_Camera_Data);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_CAMERA_SPEED1)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_TD_DEBUG_RESETVIEW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_CAMERA_FIRST)
		{

			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;
			App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;

			App->CL_Player->Show_Debug_Player(false);
			
			App->CL_TopDlg->flag_Toggle_Cam_FirstMode = 1;
			App->CL_TopDlg->flag_Toggle_Cam_ModelMode = 0;
			App->CL_TopDlg->flag_Toggle_Cam_FreeMode = 0;

			RedrawWindow(App->CL_TopDlg->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_CAMERA_FREE)
		{
			App->CL_TopDlg->Camera_Set_Free();
			return 1;
		}
		

		/*if (LOWORD(wParam) == IDC_BT_CAMERA_MODEL)
		{
			App->CL_Camera->Reset_View();
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Model;

			App->CL_TopDlg->flag_Toggle_Cam_ModelMode = 1;
			App->CL_TopDlg->flag_Toggle_Cam_FreeMode = 0;
			App->CL_TopDlg->flag_Toggle_Cam_FirstMode = 0;

			RedrawWindow(App->CL_TopDlg->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}*/

		if (LOWORD(wParam) == IDC_BT_TD_DEBUG_RESETVIEW)
		{
			App->CL_Camera->Reset_View();
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BT_CAMERA_SPEED1)
		{
			App->CL_Dialogs->Start_Speed_Camera_Dlg();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_CAMERA_CAMDATA)
		{
			if (App->CL_ImGui->flag_Show_Camera_Data == 1)
			{
				App->CL_ImGui->flag_Show_Camera_Data = 0;
			}
			else
			{
				App->CL_ImGui->flag_Show_Camera_Data = 1;
			}

			return 1;
		}

		return FALSE;
	}

	}
	return FALSE;
}

// *************************************************************************
// *			Start_Game_TB:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_TopDlg::Start_Game_TB(void)
{
	Game_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_GAME, Tabs_TB_hWnd, (DLGPROC)Proc_Game_TB);
}

// *************************************************************************
// *			Proc_Game_TB:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Proc_Game_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_GAME_PREVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_GAME_CONFIG, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_GAME_BUILDGAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Tabs;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_GAME_PREVIEW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_GAME_CONFIG)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_GAME_BUILDGAME)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{

		if (LOWORD(wParam) == IDC_BT_GAME_PREVIEW)
		{
			if (App->CL_Scene->flag_Scene_Loaded == 1)
			{
				App->CL_Scene->Game_Mode();
			}

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_GAME_CONFIG)
		{
			Debug
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_GAME_BUILDGAME)
		{
			Debug
			return 1;
		}

		return FALSE;
	}

	}
	return FALSE;
}

// *************************************************************************
// *			Start_Physics_TB:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_TopDlg::Start_Physics_TB(void)
{
	Physics_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_PHYSICS, Tabs_TB_hWnd, (DLGPROC)Proc_Physics_TB);
}

// *************************************************************************
// *			Proc_Physics_TB:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Proc_Physics_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TD_DEBUG_PHYSICSDEBUG, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Tabs;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TD_DEBUG_PHYSICSDEBUG)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{

		if (LOWORD(wParam) == IDC_BT_TD_DEBUG_PHYSICSDEBUG)
		{
			if (App->CL_Bullet->flag_Debug_All == 1)
			{
				App->CL_Bullet->flag_Debug_All = 0;
				App->CL_Bullet->Show_Debug_Objects(false);
				App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node = 0;
				if (App->CL_Scene->flag_Player_Added == 1 && App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_Free)
				{
					App->CL_Player->Show_Debug_Player(true);
				}
			}
			else
			{
				App->CL_Bullet->flag_Debug_All = 1;
				App->CL_Bullet->Show_Debug_Objects(true);
				App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node = 1;

				App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node = 1;
				App->CL_Ogre->Bullet_Debug_Listener->btDebug_Node->setVisible(true);
			}

			return 1;
		}

		return FALSE;
	}

	}
	return FALSE;
}

// *************************************************************************
// *			Start_Locations_TB:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_TopDlg::Start_Locations_TB(void)
{
	Locations_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_LOCATIONS, Tabs_TB_hWnd, (DLGPROC)Proc_Locations_TB);
}

// *************************************************************************
// *			Proc_Locations_TB:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Proc_Locations_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_PLAYER_LOCATION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Tabs;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_PLAYER_LOCATION && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_PLAYER_LOCATION)
		{
			if (App->CL_Scene->flag_Player_Added == 1)
			{
				App->CL_Locations->Start_Locations_Dlg();
			}

			return TRUE;
		}

		return FALSE;
	}

	}
	return FALSE;
}

// *************************************************************************
// *			Start_Motions_TB:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_TopDlg::Start_Motions_TB(void)
{
	Motions_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_MOTIONS, Tabs_TB_hWnd, (DLGPROC)Proc_Motions_TB);
	Update_Speed_Combo();
}

// *************************************************************************
// *			Proc_Motions_TB:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Proc_Motions_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_CB_MOTIONS_MOTIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_MOTIONS_SPEED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MOTIONS_PAUSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MOTIONS_PLAY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MOTIONS_STOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_MOTIONS_MOTIOINS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_MOTIONS_SPEED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_MOTIONS_MOTIOINS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_Tabs;
		}

		if (GetDlgItem(hDlg, IDC_ST_MOTIONS_SPEED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_Tabs;
		}

		if (GetDlgItem(hDlg, IDC_CB_MOTIONS_SPEED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_Tabs;
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Tabs;
	}
	
	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BT_MOTIONS_PLAY)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Motions->flag_Motion_Playing);
			return CDRF_DODEFAULT;
		}*/

		if (some_item->idFrom == IDC_BT_MOTIONS_STOP)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		/*if (some_item->idFrom == IDC_BT_MOTIONS_PAUSE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Motions->flag_Motion_Paused);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_MOTIONS_PAUSE)
		{
			/*if (App->CL_Motions->flag_Motion_Playing == 1)
			{
				if (App->CL_Motions->flag_Motion_Paused == 0)
				{
					App->CL_Motions->Pause_SelectedMotion();
				}
				else
				{
					App->CL_Motions->flag_Motion_Paused = 0;
					App->CL_Motions->Play_SelectedMotion();
				}
			}*/

			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BT_MOTIONS_PLAY)
		{
			//App->CL_Motions->Play_SelectedMotion();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_MOTIONS_STOP)
		{
			//App->CL_Motions->Stop_SelectedMotion();
			return 1;
		}

		if (LOWORD(wParam) == IDC_CB_MOTIONS_MOTIONS)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_MOTIONS_MOTIONS);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);
				strcpy(App->CL_TopDlg->Selected_Motion_Name,buff);

				//App->CL_Motions->Stop_SelectedMotion();
				//App->CL_Motions->Play_SelectedMotion();

				RedrawWindow(App->CL_TopDlg->Motions_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CB_MOTIONS_SPEED)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_MOTIONS_SPEED);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);
				
				//App->CL_Motions->AnimationScale = atof(buff);

			}
			}

			return TRUE;
		}

		return FALSE;
	}

	}
	return FALSE;
}

// *************************************************************************
// *		Update_Motions_By_Name:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_TopDlg::Update_Motions_By_Name(const char* Name)
{
	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_MOTIONS, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) Name);

	strcpy(App->CL_TopDlg->Selected_Motion_Name, Name);

	//App->CL_Motions->Stop_SelectedMotion();
	//App->CL_Motions->Play_SelectedMotion();

	RedrawWindow(App->CL_TopDlg->Tabs_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *		Switch_To_Motions_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_TopDlg::Switch_To_Motions_Dlg(void)
{
	App->CL_TopDlg->Hide_Tabs();
	ShowWindow(App->CL_TopDlg->Motions_TB_hWnd, SW_SHOW);
	App->CL_TopDlg->flag_Toggle_Tabs_Motions = 1;

	RedrawWindow(App->CL_TopDlg->Tabs_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *		Update_Speed_Combo:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_TopDlg::Update_Speed_Combo(void)
{
	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"2");
	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"1.5");
	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"1");
	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0.5");
	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0.2");
	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0.1");
	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0.01");

	SendDlgItemMessage(Motions_TB_hWnd, IDC_CB_MOTIONS_SPEED, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_TopDlg::Init_Bmps_Globals(void)
{

	HWND Temp = GetDlgItem(TabsHwnd, IDC_TBSHOWGRID);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOn_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_TBSHOWHAIR);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOn_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_BTSHOWTEXTURES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_TBSHOWFACES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_TBSHOWFACES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_TBINFO);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_ModelInfo_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_TBBOUNDBOX);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_BTSHOWPOINTS);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOff_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_BTSHOWBONES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);

	Temp = GetDlgItem(TabsHwnd, IDC_BTSHOWNORMALS);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOff_Bmp);

	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOFADE, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);
	SendMessage(hTooltip_TB_2, TTM_SETMAXTIPWIDTH, 0, 250);

	Temp = GetDlgItem(TabsHwnd, IDC_TBSHOWHAIR);
	TOOLINFO ti1 = { 0 };
	ti1.cbSize = sizeof(ti1);
	ti1.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti1.uId = (UINT_PTR)Temp;
	ti1.lpszText = (LPSTR) "Toggle Main Cross Hair.";
	ti1.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti1);

	Temp = GetDlgItem(TabsHwnd, IDC_TBSHOWGRID);
	TOOLINFO ti2 = { 0 };
	ti2.cbSize = sizeof(ti2);
	ti2.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti2.uId = (UINT_PTR)Temp;
	ti2.lpszText = (LPSTR) "Toggle Main Grid.";
	ti2.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti2);

	Temp = GetDlgItem(TabsHwnd, IDC_TBSHOWFACES);
	TOOLINFO ti3 = { 0 };
	ti3.cbSize = sizeof(ti3);
	ti3.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti3.uId = (UINT_PTR)Temp;
	ti3.lpszText = (LPSTR) "Toggle Mesh/Faces.";
	ti3.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti3);

	Temp = GetDlgItem(TabsHwnd, IDC_TBINFO);
	TOOLINFO ti4 = { 0 };
	ti4.cbSize = sizeof(ti4);
	ti4.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti4.uId = (UINT_PTR)Temp;
	ti4.lpszText = (LPSTR) "Show Information.";
	ti4.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti4);

	Temp = GetDlgItem(TabsHwnd, IDC_TBBOUNDBOX);
	TOOLINFO ti5 = { 0 };
	ti5.cbSize = sizeof(ti5);
	ti5.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti5.uId = (UINT_PTR)Temp;
	ti5.lpszText = (LPSTR) "Toggle Bounding Box.";
	ti5.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti5);

	Temp = GetDlgItem(TabsHwnd, IDC_BTSHOWPOINTS);
	TOOLINFO ti6 = { 0 };
	ti6.cbSize = sizeof(ti6);
	ti6.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti6.uId = (UINT_PTR)Temp;
	ti6.lpszText = (LPSTR) "Toggle Mesh Points.";
	ti6.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti6);

	Temp = GetDlgItem(TabsHwnd, IDC_BTSHOWBONES);
	TOOLINFO ti7 = { 0 };
	ti7.cbSize = sizeof(ti7);
	ti7.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti7.uId = (UINT_PTR)Temp;
	ti7.lpszText = (LPSTR) "Toggle Bones/Joints.";
	ti7.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti7);

	Temp = GetDlgItem(TabsHwnd, IDC_BTSHOWNORMALS);
	TOOLINFO ti8 = { 0 };
	ti8.cbSize = sizeof(ti8);
	ti8.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti8.uId = (UINT_PTR)Temp;
	ti8.lpszText = (LPSTR) "Toggle Normals.";
	ti8.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti8);

	Temp = GetDlgItem(TabsHwnd, IDC_BTSHOWTEXTURES);
	TOOLINFO ti9 = { 0 };
	ti9.cbSize = sizeof(ti9);
	ti9.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti9.uId = (UINT_PTR)Temp;
	ti9.lpszText = (LPSTR) "Toggle Textures.";
	ti9.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti9);

	// ------------------------- Camera Panel

	Temp = GetDlgItem(Camera_TB_hWnd, IDC_BT_TD_DEBUG_RESETVIEW);
	TOOLINFO ti30 = { 0 };
	ti30.cbSize = sizeof(ti30);
	ti30.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti30.uId = (UINT_PTR)Temp;
	ti30.lpszText = (LPSTR)"Reset Camera\rResets Camera View Based on whats Loaded.";
	ti30.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti30);

	Temp = GetDlgItem(Camera_TB_hWnd, IDC_BT_CAMERA_MODEL);
	TOOLINFO ti31 = { 0 };
	ti31.cbSize = sizeof(ti31);
	ti31.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti31.uId = (UINT_PTR)Temp;
	ti31.lpszText = (LPSTR)"Sets the Camera into Model Mode.\rModel Mode is like a Model Editor View.";
	ti31.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti31);

	Temp = GetDlgItem(Camera_TB_hWnd, IDC_BT_CAMERA_FREE);
	TOOLINFO ti32 = { 0 };
	ti32.cbSize = sizeof(ti32);
	ti32.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti32.uId = (UINT_PTR)Temp;
	ti32.lpszText = (LPSTR)"Sets the Camera into Free Mode.\rFree Mode is like a Game where you can roam around freely there is no Physics so you can go through walls and things.";
	ti32.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti32);

	Temp = GetDlgItem(Camera_TB_hWnd, IDC_BT_CAMERA_FIRST);
	TOOLINFO ti33 = { 0 };
	ti33.cbSize = sizeof(ti33);
	ti33.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti33.uId = (UINT_PTR)Temp;
	ti33.lpszText = (LPSTR)"Sets the Camera into 1st Viewer Mode.\r1st Viewer Mode sets the camera as the Player and folows the Physics Player.";
	ti33.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti33);
	//
	Temp = GetDlgItem(Camera_TB_hWnd, IDC_BT_CAMERA_SPEED1);
	TOOLINFO ti34 = { 0 };
	ti34.cbSize = sizeof(ti34);
	ti34.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti34.uId = (UINT_PTR)Temp;
	ti34.lpszText = (LPSTR)"Speed Options.\r.";
	ti34.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti34);

	Temp = GetDlgItem(Camera_TB_hWnd, IDC_BT_CAMERA_CAMDATA);
	TOOLINFO ti35 = { 0 };
	ti35.cbSize = sizeof(ti35);
	ti35.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti35.uId = (UINT_PTR)Temp;
	ti35.lpszText = (LPSTR)"Shows Camera Data.\rPosition and Rotation.";
	ti35.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti35);

}

// **************************************************************************
// *	  	Enable_Grid_Hair_Icons:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_TopDlg::Enable_Grid_Hair_Icons(bool Enable) const
{
	if (Enable == 1)
	{
		SendMessage(GetDlgItem(TabsHwnd, IDC_TBSHOWGRID), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOn_Bmp);
		SendMessage(GetDlgItem(TabsHwnd, IDC_TBSHOWHAIR), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOn_Bmp);
	}
	else
	{
		SendMessage(GetDlgItem(TabsHwnd, IDC_TBSHOWGRID), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOff_Bmp);
		SendMessage(GetDlgItem(TabsHwnd, IDC_TBSHOWHAIR), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOff_Bmp);
	}
}

// **************************************************************************
// *	  		 Enable_Info_Icon:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_TopDlg::Enable_Info_Icon(bool Enable) const
{
	if (Enable == 1)
	{
		SendMessage(GetDlgItem(TabsHwnd, IDC_TBINFO), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_ModelInfoOn_Bmp);
		CheckMenuItem(App->mMenu, ID_WINDOWS_MODELDATA, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		SendMessage(GetDlgItem(TabsHwnd, IDC_TBINFO), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_ModelInfo_Bmp);
		CheckMenuItem(App->mMenu, ID_WINDOWS_MODELDATA, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// **************************************************************************
// *	  		 Enable_Info_Panel:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_TopDlg::Enable_Info_Panel(bool Enable)
{
	HWND Temp = GetDlgItem(TabsHwnd, IDC_TBINFO);

	if (Enable == 1)
	{
		//App->CL_ImGui->flag_Show_Model_Data = 1;

		SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_ModelInfoOn_Bmp);
		CheckMenuItem(App->mMenu, ID_WINDOWS_MODELDATA, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		//App->CL_ImGui->flag_Show_Model_Data = 0;

		SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_ModelInfo_Bmp);
		CheckMenuItem(App->mMenu, ID_WINDOWS_MODELDATA, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

// **************************************************************************
// *	  	Enable_ImGui_Demo_Panel:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_TopDlg::Enable_ImGui_Demo_Panel(bool Enable)
{
	if (Enable == 1)
	{
		App->CL_ImGui->flag_Show_ImGui_Demo = 1;
	}
	else
	{
		App->CL_ImGui->flag_Show_ImGui_Demo = 0;
	}

}

// **************************************************************************
// *	  	Enable_FPSLock_Dlg_Panel:- Terry and Hazel Flanigan 2024		*
// **************************************************************************
void CL64_TopDlg::Enable_FPSLock_Dlg_Panel(bool Enable)
{
	if (Enable == 1)
	{
		App->CL_Dialogs->Start_FPSLock_Dlg();
	}
	else
	{
		if (App->CL_TopDlg->flag_FPS_Dlg_Running == 1)
		{
			EndDialog(App->CL_Dialogs->FPSLock_Dlg_hWnd, 0);
			App->CL_TopDlg->flag_FPS_Dlg_Running = 0;
		}
	}
}

// **************************************************************************
// *			Camera_Set_Free:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_TopDlg::Camera_Set_Free()
{
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

	if (App->CL_Scene->flag_Player_Added == 1)
	{
		App->CL_Player->Show_Debug_Player(true);
	}

	App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node = 1;
	App->CL_Ogre->Bullet_Debug_Listener->btDebug_Node->setVisible(true);

	App->CL_TopDlg->flag_Toggle_Cam_FreeMode = 1;
	App->CL_TopDlg->flag_Toggle_Cam_ModelMode = 0;
	App->CL_TopDlg->flag_Toggle_Cam_FirstMode = 0;

	RedrawWindow(App->CL_TopDlg->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

