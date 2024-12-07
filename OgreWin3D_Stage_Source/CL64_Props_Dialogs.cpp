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
#include "CL64_Props_Dialogs.h"

CL64_Props_Dialogs::CL64_Props_Dialogs(void)
{
	Details_Goto_Hwnd =		nullptr;
	PhysicsTest_Dlg_hWnd =	nullptr;
	Dimensions_Dlg_hWnd =	nullptr;
	Debug_Dlg_hWnd =		nullptr;
	Material_Props_Hwnd =	nullptr;
	Cam_Props_HWND =		nullptr;

	Show_Area_Physics_Debug = 0;
}

CL64_Props_Dialogs::~CL64_Props_Dialogs(void)
{
}

// **************************************************************************
// *	  		Start_Props_Dialogs:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Props_Dialogs::Start_Props_Dialogs()
{

	Start_Dialog_Dimensions();
	Start_Dialog_PhysicsTest();
	Start_Dialog_Debug();
	Start_Camera_PropsPanel();
	//Start_Panels_Test_Dlg();
	//Start_Area_PropsPanel();
	Start_Details_Goto_Dlg();
	Start_Materials_PropsPanel();

}

// *************************************************************************
// *	Start_Details_Goto_PropsPanel:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Details_Goto_Dlg(void)
{
	Details_Goto_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_OBJECT, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Details_Goto);

	//Init_Bmps_DetailsGo();
	Show_Details_Goto_Dlg(false);

}

// *************************************************************************
// *			Proc_Details_Goto:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Proc_Details_Goto(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_GOTO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DETAIL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_GOTO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_DETAIL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BT_DETAIL)
		{
			if (App->CL_ImGui->flag_Show_Object_Data == 1)
			{
				App->CL_ImGui->flag_Show_Object_Data = 0;
				App->CL_ImGui->flag_Do_Object_Data_Pos = 0;
			}
			else
			{
				App->CL_LookUps->Update_Types();
				App->CL_ImGui->flag_Show_Object_Data = 1;
				App->CL_Ogre->RenderFrame(8);
				App->CL_ImGui->flag_Do_Object_Data_Pos = 1;
			}

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_GOTO)
		{
			App->CL_Com_Cameras->Camera_Goto_Object(App->CL_Properties->Current_Selected_Object);
			
			return 1;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  	Start_Dialog_PhysicsTest:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Dialog_PhysicsTest()
{

	PhysicsTest_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_PHYSTEST, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Dialog_PhysicsTest);

	//Init_Bmps_PhysicsReset();
	Show_Physics_Test_Dlg(false);

}

// *************************************************************************
// *		 Proc_Dialog_PhysicsTest:- Terry and Hazel Flanigan 2024  	   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Proc_Dialog_PhysicsTest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_PHYSRESET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TEST_ENTITY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_PHYSRESET && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TEST_ENTITY && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{

		if (LOWORD(wParam) == IDC_BT_TEST_ENTITY)
		{
			App->CL_Com_MoveEntity->Test_Move_Entity(App->CL_Properties->Current_Selected_Object);
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_PHYSRESET)
		{
			App->CL_Com_MoveEntity->Reset_Move_Entity(App->CL_Properties->Current_Selected_Object);
			return 1;
		}

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
	}

	}
	return FALSE;
}

// *************************************************************************
// *	  Start_Dialog_Dimensions:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Dialog_Dimensions()
{

	Dimensions_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_DIMENSIONS, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Dialog_Dimensions);

	//Init_Bmps_Dimensions();
	Show_Dimensions_Dlg(0);

}

// *************************************************************************
// *		Proc_Dialog_Dimensions:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Proc_Dialog_Dimensions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_POSITION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ROTATION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_SCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_LOCK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{

		if (GetDlgItem(hDlg, IDC_CK_LOCK) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_Panel;
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Panel;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_POSITION && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dimensions->Show_Position);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_SCALE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dimensions->Show_Scale);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ROTATION && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Dimensions->Show_Rotation);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_CK_LOCK)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_LOCK);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				int EditCat = App->CL_Properties->Edit_Category;
				//if (EditCat == Enums::FV_Edit_Object || EditCat == Enums::Edit_Teleport)
				{
					int Index = App->CL_Properties->Current_Selected_Object;
					App->CL_Scene->B_Object[Index]->Dimensions_Locked = 1;

					App->CL_Scene->B_Object[Index]->Altered = 1;
					App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
					App->CL_Scene->flag_Scene_Modified = 1;
				}

				EnableWindow(GetDlgItem(hDlg, IDC_BT_POSITION), 0);
				EnableWindow(GetDlgItem(hDlg, IDC_BT_SCALE), 0);
				EnableWindow(GetDlgItem(hDlg, IDC_BT_ROTATION), 0);
				return 1;
			}
			else
			{
				int EditCat = App->CL_Properties->Edit_Category;
				//if (EditCat == Enums::FV_Edit_Object || EditCat == Enums::Edit_Teleport)
				{
					int Index = App->CL_Properties->Current_Selected_Object;
					App->CL_Scene->B_Object[Index]->Dimensions_Locked = 0;

					App->CL_Scene->B_Object[Index]->Altered = 1;
					App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
					App->CL_Scene->flag_Scene_Modified = 1;
				}

				if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
				{
					if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
					{
						//App->CL_Dialogs->YesNo("Edit Area", "Are You Sure", 1);
						bool Doit = App->CL_Dialogs->Canceled;
						if (Doit == 1)
						{
							SendMessage(temp, BM_SETCHECK, 1, 0);
							return TRUE;
						}
					}
				}

				EnableWindow(GetDlgItem(hDlg, IDC_BT_POSITION), 1);
				EnableWindow(GetDlgItem(hDlg, IDC_BT_SCALE), 1);
				EnableWindow(GetDlgItem(hDlg, IDC_BT_ROTATION), 1);
				return 1;
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_POSITION)
		{

			if (App->CL_Dimensions->Show_Position == 1)
			{
				App->CL_Dimensions->Show_Position = 0;
				//App->SBC_Markers->Hide_Axis_Marker();
			}
			else
			{
				//App->SBC_Markers->Hide_Axis_Marker();

				App->CL_Dimensions->Prepare_Dimensions();
				App->CL_Dimensions->Show_Position = 1;
				App->CL_Dimensions->Show_Scale = 0;
				App->CL_Dimensions->Show_Rotation = 0;
			}

			RedrawWindow(App->CL_Props_Dialogs->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_SCALE)
		{

			if (App->CL_Dimensions->Show_Scale == 1)
			{
				App->CL_Dimensions->Show_Scale = 0;
				//App->SBC_Markers->Hide_Axis_Marker();
			}
			else
			{
				//App->SBC_Markers->Hide_Axis_Marker();

				App->CL_Dimensions->Prepare_Dimensions();
				App->CL_Dimensions->Show_Scale = 1;
				App->CL_Dimensions->Show_Position = 0;
				App->CL_Dimensions->Show_Rotation = 0;
			}

			RedrawWindow(App->CL_Props_Dialogs->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_ROTATION)
		{

			if (App->CL_Dimensions->Show_Rotation == 1)
			{
				App->CL_Dimensions->Show_Rotation = 0;
				//App->SBC_Markers->Hide_Axis_Marker();
			}
			else
			{
				//App->SBC_Markers->Hide_Axis_Marker();

				App->CL_Dimensions->Prepare_Dimensions();
				App->CL_Dimensions->Show_Rotation = 1;
				App->CL_Dimensions->Show_Position = 0;
				App->CL_Dimensions->Show_Scale = 0;
			}

			RedrawWindow(App->CL_Props_Dialogs->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}

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
	}

	}
	return FALSE;
}

// *************************************************************************
// *	  		Start_Dialog_Debug:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Dialog_Debug()
{
	Debug_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_DEBUG, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Dialog_Debug);

	//Init_Bmps_Debug();

	Hide_Debug_Dlg(true);

}

// *************************************************************************
// *			Proc_Dialog_Debug:- Terry and Hazel Flanigan 2024  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Proc_Dialog_Debug(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_PHYSDEBUG, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_SHOWMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ONLYMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Panel;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_PHYSDEBUG)
		{
			if (App->CL_Scene->Object_Count > 0)
			{
				int Index = App->CL_Properties->Current_Selected_Object;

				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
				App->Custom_Button_Toggle(item, App->CL_Scene->B_Object[Index]->Physics_Debug_On);
				return CDRF_DODEFAULT;
			}
			else
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
				App->Custom_Button_Toggle(item, 0);
				return CDRF_DODEFAULT;
			}
		}

		if (some_item->idFrom == IDC_BT_SHOWMESH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Com_Objects->flag_Show_Mesh_Debug);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ONLYMESH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Com_Objects->flag_Hide_All_Except);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{

		if (LOWORD(wParam) == IDC_BT_ONLYMESH)
		{
			int Index = App->CL_Properties->Current_Selected_Object;

			// -----------------------  Area
			if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
			{
				if (App->CL_Com_Objects->flag_Hide_All_Except == 1)
				{
					App->CL_Com_Objects->flag_Hide_All_Except = 0;
					App->CL_Com_Objects->Hide_AllObjects_Except(Index, true);
				}
				else
				{
					App->CL_Com_Objects->flag_Hide_All_Except = 1;
					App->CL_Com_Objects->Hide_AllObjects_Except(Index, false);
				}
				return 1;
			}

			if (App->CL_Com_Objects->flag_Hide_All_Except == 1)
			{
				App->CL_Com_Objects->flag_Hide_All_Except = 0;
				App->CL_Com_Objects->Hide_AllObjects_Except(Index, true);
			}
			else
			{
				App->CL_Com_Objects->flag_Hide_All_Except = 1;
				App->CL_Com_Objects->Hide_AllObjects_Except(Index, false);
			}

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_SHOWMESH)
		{
			int Index = App->CL_Properties->Current_Selected_Object;

			// -----------------------  Area
			if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
			{
				if (App->CL_Com_Objects->flag_Show_Mesh_Debug == 1)
				{
					App->CL_Scene->B_Area[Index]->Area_Node->setVisible(false);
					App->CL_Com_Objects->flag_Show_Mesh_Debug = 0;
				}
				else
				{
					App->CL_Scene->B_Area[Index]->Area_Node->setVisible(true);
					App->CL_Com_Objects->flag_Show_Mesh_Debug = 1;
				}
				return 1;
			}

			if (App->CL_Com_Objects->flag_Show_Mesh_Debug == 1)
			{
				App->CL_Scene->B_Object[Index]->Object_Node->setVisible(false);
				App->CL_Com_Objects->flag_Show_Mesh_Debug = 0;
			}
			else
			{
				App->CL_Scene->B_Object[Index]->Object_Node->setVisible(true);
				App->CL_Com_Objects->flag_Show_Mesh_Debug = 1;
			}

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_PHYSDEBUG)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_BT_PHYSDEBUG);

			int Index = App->CL_Properties->Current_Selected_Object;


			// -----------------------  Area
			if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
			{

				int f = App->CL_Scene->B_Area[Index]->Phys_Body->getCollisionFlags();

				if (App->CL_Props_Dialogs->Show_Area_Physics_Debug == 1)
				{
					App->CL_Props_Dialogs->Show_Area_Physics_Debug = 0;
					App->CL_Scene->B_Area[Index]->Phys_Body->setCollisionFlags(f | (1 << 5)); // Off

					App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
					App->CL_Ogre->RenderFrame(4);
					App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;

					//SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_PhysicsOff_Bmp);
				}
				else
				{
					App->CL_Props_Dialogs->Show_Area_Physics_Debug = 1;
					App->CL_Scene->B_Area[Index]->Phys_Body->setCollisionFlags(f & (~(1 << 5))); // on

					//SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_PhysicsOn_Bmp);
				}

				return 1;
			}

			// -----------------------  Objects
			if (App->CL_Scene->Object_Count > 0)
			{
				int f = App->CL_Scene->B_Object[Index]->Phys_Body->getCollisionFlags();

				if (App->CL_Scene->B_Object[Index]->Physics_Debug_On == 1)
				{
					App->CL_Com_Objects->flag_Show_Physics_Debug = 0;
					App->CL_Scene->B_Object[Index]->Phys_Body->setCollisionFlags(f | (1 << 5)); // Off

					App->CL_Scene->B_Object[Index]->Physics_Debug_On = 0;

					App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
					App->CL_Ogre->RenderFrame(4);
					App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;

					//SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_PhysicsOff_Bmp);
				}
				else
				{
					App->CL_Scene->B_Object[Index]->Physics_Debug_On = 1;
					App->CL_Com_Objects->flag_Show_Physics_Debug = 1;
					App->CL_Scene->B_Object[Index]->Phys_Body->setCollisionFlags(f & (~(1 << 5))); // on

					//SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_PhysicsOn_Bmp);
				}
			}

			return 1;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	}
	return FALSE;
}

// *************************************************************************
// *	  Start_Materials_PropsPanel:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Materials_PropsPanel()
{
	Material_Props_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_MATERIALS, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Prop_Materials_PropsPanel);
	Show_Materials_Dlg(false);
}

// *************************************************************************
// *	Prop_Materials_PropsPanel:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Prop_Materials_PropsPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{

		SendDlgItemMessage(hDlg, IDC_PROPMATERIALS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_PROPMATERIALS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_PROPMATERIALS)
		{
			App->CL_Materials->Start_Material_Editor();

			return 1;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  Start_Camera_PropsPanel:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Props_Dialogs::Start_Camera_PropsPanel()
{
	Cam_Props_HWND = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_CAMERA, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Camera_PropsPanel);
	Show_Cameras_Dlg(false);
	return 1;
}

// *************************************************************************
// *		Proc_Camera_PropsPanel:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Proc_Camera_PropsPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BTCAMSAVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTCAMGOTO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{

		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BTCAMSAVE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTCAMGOTO)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BTCAMSAVE)
		{
			int Index = App->CL_Properties->Current_Selected_Object;

			App->CL_Com_Cameras->Update_Camera_Entity_Data(Index);

			App->CL_Scene->B_Camera[Index]->Altered = 1;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Camera[Index]->FileViewItem);
			App->CL_Scene->flag_Scene_Modified = 1;
			App->CL_Properties->Update_ListView_Camera();
			App->Say("Camera Saved");

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTCAMGOTO)
		{
			App->CL_Com_Cameras->Set_Camera(App->CL_Properties->Current_Selected_Object);
			return TRUE;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *			Hide_Debug_Dlg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Props_Dialogs::Hide_Debug_Dlg(bool Show)
{
	ShowWindow(Debug_Dlg_hWnd, Show);
}

// *************************************************************************
// *		Show_Details_Goto_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Props_Dialogs::Show_Details_Goto_Dlg(bool Show)
{
	ShowWindow(Details_Goto_Hwnd, Show);
}

// *************************************************************************
// *		Show_Details_Test_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Props_Dialogs::Show_Physics_Test_Dlg(bool Show)
{
	ShowWindow(PhysicsTest_Dlg_hWnd, Show);
}

// *************************************************************************
// *		Show_Dimensions_Dlg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Props_Dialogs::Show_Dimensions_Dlg(bool Show)
{
	ShowWindow(Dimensions_Dlg_hWnd, Show);
}

// *************************************************************************
// *		Show_Materials_Dlg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Props_Dialogs::Show_Materials_Dlg(bool Show)
{
	ShowWindow(Material_Props_Hwnd, Show);
}

// *************************************************************************
// *		Show_Cameras_Dlg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Props_Dialogs::Show_Cameras_Dlg(bool Show)
{
	ShowWindow(Cam_Props_HWND, Show);
}
