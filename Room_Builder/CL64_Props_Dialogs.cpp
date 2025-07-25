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
	Details_Goto_Hwnd =				nullptr;
	PhysicsTest_Dlg_hWnd =			nullptr;
	Material_Props_Hwnd =			nullptr;
	Cam_Props_HWND =				nullptr;
	Player_Props_HWND =				nullptr;
	Override_Counter_Goto_Hwnd =	nullptr;

	flag_isHighlighted = false;

	flag_Show_Area_Physics_Debug = 0;

	flag_Toggle_Objects = 1;
	flag_Toggle_Physics = 0;
	flag_Toggle_OverrideCounter = 0;
}

CL64_Props_Dialogs::~CL64_Props_Dialogs(void)
{
}

// **************************************************************************
// *	  		Start_Props_Dialogs:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Props_Dialogs::Start_Props_Dialogs()
{
	Start_Dialog_PhysicsTest();
	Start_Camera_PropsPanel();
	Start_Details_Goto_Dlg();
	Start_Materials_PropsPanel();
	Start_Player_PropsPanel();
	Start_Override_Counter();
}

// *************************************************************************
// *	Start_Details_Goto_PropsPanel:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Details_Goto_Dlg(void)
{
	//Details_Goto_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_OBJECT, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Details_Goto);

	////Init_Bmps_DetailsGo();
	//Show_Details_Goto_Dlg(false);

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
		//SendDlgItemMessage(hDlg, IDC_BT_GOTO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BT_DETAIL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		//return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BT_GOTO)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_DETAIL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		/*if (LOWORD(wParam) == IDC_BT_DETAIL)
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
		}*/

		break;
	}

	return FALSE;
}

// *************************************************************************
// *		Start_Overide_Counter:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Override_Counter(void)
{
	//Override_Counter_Goto_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_OVERRIDECOUNTER, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Override_Counter);

	//Init_Bmps_DetailsGo();
	Show_Override_Counter_Dlg(false);

}

// *************************************************************************
// *		Proc_Overide_Counter:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Proc_Override_Counter(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//SendDlgItemMessage(hDlg, IDC_BT_DP_OVERIDECOUNTER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		//return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BT_DP_OVERIDECOUNTER)
		{
			int Index = App->CL_Properties->Current_Selected_Object;

			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			if (App->CL_Scene->Object_Count > 0)
			{
				App->Custom_Button_Toggle(item, App->CL_Scene->B_Object[Index]->flag_OverRide_Counter);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		/*if (LOWORD(wParam) == IDC_BT_DP_OVERIDECOUNTER)
		{
			int Index = App->CL_Properties->Current_Selected_Object;

			if (App->CL_Scene->B_Object[Index]->flag_OverRide_Counter == 1)
			{
				App->CL_Scene->B_Object[Index]->flag_OverRide_Counter = 0;
				App->CL_Props_Dialogs->flag_Toggle_OverrideCounter = 0;
			}
			else
			{
				App->CL_Scene->B_Object[Index]->flag_OverRide_Counter = 1;
				App->CL_Props_Dialogs->flag_Toggle_OverrideCounter = 1;
			}

			RedrawWindow(App->CL_Props_Dialogs->Override_Counter_Goto_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}*/

		break;
	}
	}

	return FALSE;
}

// *************************************************************************
// *	  	Start_Dialog_PhysicsTest:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Dialog_PhysicsTest()
{

	//PhysicsTest_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_PHYSTEST, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Dialog_PhysicsTest);

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
		//SendDlgItemMessage(hDlg, IDC_BT_PHYSRESET, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BT_TEST_ENTITY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		//return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BT_PHYSRESET)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TEST_ENTITY)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{

		/*if (LOWORD(wParam) == IDC_BT_TEST_ENTITY)
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
		}*/
	}

	}
	return FALSE;
}


// *************************************************************************
// *	  Start_Materials_PropsPanel:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Props_Dialogs::Start_Materials_PropsPanel()
{
	//Material_Props_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_MATERIALS, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Prop_Materials_PropsPanel);
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

		//SendDlgItemMessage(hDlg, IDC_PROPMATERIALS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		//return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_PROPMATERIALS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		/*if (LOWORD(wParam) == IDC_PROPMATERIALS)
		{
			App->CL_Materials->Start_Material_Editor();

			return 1;
		}*/

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  Start_Camera_PropsPanel:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Props_Dialogs::Start_Camera_PropsPanel()
{
	//Cam_Props_HWND = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_CAMERA, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Camera_PropsPanel);
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
		//SendDlgItemMessage(hDlg, IDC_BTCAMSAVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BTCAMGOTO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		//return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{

		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BTCAMSAVE)
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
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		/*if (LOWORD(wParam) == IDC_BTCAMSAVE)
		{
			int Index = App->CL_Properties->Current_Selected_Object;

			App->CL_Com_Cameras->Update_Camera_Entity_Data(Index);

			App->CL_Scene->B_Camera[Index]->flag_Altered = 1;
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
		}*/

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  Start_Player_PropsPanel:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Props_Dialogs::Start_Player_PropsPanel()
{
	//Player_Props_HWND = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_PLAYER, App->CL_Properties->Properties_Dlg_hWnd, (DLGPROC)Proc_Player_PropsPanel);
	Show_Player_Dlg(false);

	return 1;
}

// *************************************************************************
// *		Player_PropsPanel_Proc:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Dialogs::Proc_Player_PropsPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		/*SendDlgItemMessage(hDlg, IDC_BTSAVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTOBJECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTPHYSICS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_PHYSICSDEBUG, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_GOTO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BTPL_LOCATIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_COLLISIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		//return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;


		/*if (some_item->idFrom == IDC_BTSAVE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		/*if (some_item->idFrom == IDC_BTLOOKAT && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		//if (some_item->idFrom == IDC_BTPL_LOCATIONS)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Normal(item);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_BT_GOTO)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Normal(item);
		//	return CDRF_DODEFAULT;
		//}

		//// ------------------------------------------ 
		//if (some_item->idFrom == IDC_BTOBJECT)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Toggle(item, App->CL_Props_Dialogs->flag_Toggle_Objects);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_BTPHYSICS)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Toggle(item, App->CL_Props_Dialogs->flag_Toggle_Physics);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_PHYSICSDEBUG)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Toggle(item, App->CL_Com_Player->flag_Show_Physics_Debug);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_BT_COLLISIONS)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Toggle(item, 0);//App->CL_Vm_ImGui->Show_Collision_Debug);
		//	return CDRF_DODEFAULT;
		//}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		//if (LOWORD(wParam) == IDC_BTPL_LOCATIONS)
		//{
		//	App->CL_Locations->Start_Locations_Dlg();
		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BT_COLLISIONS)
		//{
		//	/*if (App->CL_Vm_ImGui->Show_Collision_Debug == 1)
		//	{
		//		App->CL_Vm_ImGui->Show_Collision_Debug = 0;
		//	}
		//	else
		//	{
		//		App->CL_Vm_ImGui->Show_Collision_Debug = 1;
		//	}*/

		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BT_GOTO)
		//{

		//	//App->CL_Ogre->mCamera->setPosition(Ogre::Vector3(App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin()));

		//	float x = App->CL_Scene->B_Player[0]->StartPos.x;
		//	float y = App->CL_Scene->B_Player[0]->StartPos.y;
		//	float z = App->CL_Scene->B_Player[0]->StartPos.z;

		//	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(btVector3(x, y, z));
		//	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);


		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BTSAVE)
		//{
		//	if (App->CL_Scene->flag_Scene_Loaded == 1)
		//	{

		//		App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Are You Sure", (LPSTR)"Save new Player Start Location", (LPSTR)"");

		//		bool Doit = App->CL_Dialogs->flag_Canceled;
		//		if (Doit == 1)
		//		{
		//			return 1;
		//		}

		//		// Start Position
		//		App->CL_Scene->B_Player[0]->StartPos.x = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin().getX();
		//		App->CL_Scene->B_Player[0]->StartPos.y = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin().getY();
		//		App->CL_Scene->B_Player[0]->StartPos.z = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin().getZ();
		//		
		//		App->CL_Scene->B_Player[0]->Physics_Rotation = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getRotation();

		//		App->CL_Scene->B_Locations[0]->Physics_Position = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin();
		//		App->CL_Scene->B_Locations[0]->Physics_Rotation = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getRotation();

		//		App->CL_Scene->B_Player[0]->flag_Altered = 1;
		//		App->CL_Scene->flag_Scene_Modified = 1;
		//		App->CL_FileView->Mark_Altered(App->CL_Scene->B_Player[0]->FileViewItem);

		//		App->CL_Properties->Update_ListView_Player();

		//		App->Say("Player Saved");
		//	}
		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BTOBJECT)
		//{
		//	if (App->CL_Scene->flag_Scene_Loaded == 1)
		//	{
		//		App->CL_Properties->flag_Edit_Physics = 0;
		//		App->CL_Properties->Update_ListView_Player();

		//		App->CL_Props_Dialogs->flag_Toggle_Objects = 1;
		//		App->CL_Props_Dialogs->flag_Toggle_Physics = 0;

		//		RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		//	}
		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BTPHYSICS)
		//{
		//	if (App->CL_Scene->flag_Scene_Loaded == 1)
		//	{
		//		App->CL_Properties->flag_Edit_Physics = 1;
		//		App->CL_Properties->Update_ListView_Player_Physics();

		//		App->CL_Props_Dialogs->flag_Toggle_Objects = 0;
		//		App->CL_Props_Dialogs->flag_Toggle_Physics = 1;

		//		RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		//	}
		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_PHYSICSDEBUG)
		//{
		//	
		//	if (App->CL_Com_Player->flag_Show_Physics_Debug == 1)
		//	{
		//		App->CL_Com_Player->Show_Physics(false);
		//	}
		//	else
		//	{
		//		App->CL_Com_Player->Show_Physics(true);
		//	}

		//	return 1;
		//}


		break;
	}

	return FALSE;
}

// *************************************************************************
// *	Show_Override_Counter_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Props_Dialogs::Show_Override_Counter_Dlg(bool Show)
{
	ShowWindow(Override_Counter_Goto_Hwnd, Show);
}

// *************************************************************************
// *		Show_Player_Dlg:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Props_Dialogs::Show_Player_Dlg(bool Show)
{
	ShowWindow(Player_Props_HWND, Show);
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
