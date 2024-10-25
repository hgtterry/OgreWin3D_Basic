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
#include "CL64_Locations.h"

CL64_Locations::CL64_Locations(void)
{
	Locations_Dlg_hWnd = nullptr;

	Toggle_Player_Flag = 0;
	Toggle_FreeCam_Flag = 0;

	ListBox_Index = 0;
}

CL64_Locations::~CL64_Locations(void)
{
}

// *************************************************************************
// *	  		Start_Locations:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Locations::Start_Locations_Dlg()
{
	//Toggle_Player_Flag = App->SBC_TopTabs->Toggle_FirstCam_Flag;
	//Toggle_FreeCam_Flag = App->SBC_TopTabs->Toggle_FreeCam_Flag;

	Locations_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROJECTION, App->Fdlg, (DLGPROC)Proc_Locations);
	//Init_Bmps_PlayerLocations();
}

// *************************************************************************
// *			Proc_Locations:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_Locations::Proc_Locations(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_Locations->ListBox_Index = 0;

		SendDlgItemMessage(hDlg, IDC_STBANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_LOC_PLAYER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_LOC_FREECAM, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_LOC_PLAYERTOCAMERA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTMOVE_PLAYER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTSAVE_LOCATION_PLAYER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTEDITT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTDELETE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		int Count = 0;
		while (Count < App->CL_Scene->Player_Location_Count)
		{
			if (App->CL_Scene->B_Locations[Count]->Deleted == 0)
			{
				SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LPCTSTR)App->CL_Scene->B_Locations[Count]->Name);
			}

			Count++;
		}

		SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_SETCURSEL, (WPARAM)0, (LPARAM)(LPCTSTR)0);

		/*if (App->SBC_TopTabs->Toggle_FirstCam_Flag == 1)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_BT_LOC_PLAYERTOCAMERA), 0);
		}*/

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STBANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}
	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BTSAVE_LOCATION_PLAYER)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTMOVE_PLAYER)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTEDITT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTDELETE)
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

		//----------------------------------- 
		if (some_item->idFrom == IDC_BT_LOC_PLAYER)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Locations->Toggle_Player_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_LOC_FREECAM)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Locations->Toggle_FreeCam_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_LOC_PLAYERTOCAMERA)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		/*if (some_item->idFrom == IDC_PLAYERLOCATIONS_INFO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		// --------------------- 1st Player
		if (LOWORD(wParam) == IDC_BT_LOC_PLAYER)
		{
			App->CL_Locations->Set_To_PlayerView();
			return TRUE;
		}

		// --------------------- FreeCam
		if (LOWORD(wParam) == IDC_BT_LOC_FREECAM)
		{
			App->CL_Locations->Set_To_FreeCam();
			return TRUE;
		}

		// --------------------- Player To Camera
		if (LOWORD(wParam) == IDC_BT_LOC_PLAYERTOCAMERA)
		{
			App->CL_Locations->Move_Player_To_Camera();
			return TRUE;
		}

		// --------------------- Rename
		if (LOWORD(wParam) == IDC_BTEDITT)
		{
			int Index = App->CL_Locations->ListBox_Index;
			if (Index == 0)
			{
				App->Say("Start Location can not be Edited");
				return 1;
			}

			char buff[255];
			SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_GETTEXT, (WPARAM)Index, (LPARAM)(LPCTSTR)buff);

			int Location_Index = App->CL_LookUps->Player_Location_GetIndex_ByName(buff); // Get by Referance (Name)

			strcpy(App->CL_Dialogs->btext, "Location Name");

			char numbuf[255];
			_itoa(App->CL_Scene->Player_Location_Count, numbuf, 10);

			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Scene->B_Locations[Location_Index]->Name);

			App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Locatoins);

			if (App->CL_Dialogs->Canceled == 0)
			{
				strcpy(App->CL_Scene->B_Locations[Location_Index]->Name, App->CL_Dialogs->Chr_Text);

				SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
				int Count = 0;
				while (Count < App->CL_Scene->Player_Location_Count)
				{

					if (App->CL_Scene->B_Locations[Count]->Deleted == 0)
					{
						SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LPCTSTR)App->CL_Scene->B_Locations[Count]->Name);
					}

					Count++;
				}

				SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_SETCURSEL, (WPARAM)Index, (LPARAM)(LPCTSTR)0);

				App->CL_Scene->flag_Scene_Modified = 1;
				App->CL_FileView->Mark_Altered_Folder(App->CL_FileView->FV_LevelFolder);
			}
			return TRUE;
		}

		// --------------------------------------- Delete Location
		if (LOWORD(wParam) == IDC_BTDELETE)
		{
			App->CL_Locations->Delete_Location();
			return TRUE;
		}

		// --------------------------------------- Move Player
		if (LOWORD(wParam) == IDC_BTMOVE_PLAYER)
		{
			int Index = App->CL_Locations->ListBox_Index;
			char buff[255];
			SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_GETTEXT, (WPARAM)Index, (LPARAM)(LPCTSTR)buff);

			int Location_Index = App->CL_LookUps->Player_Location_GetIndex_ByName(buff); // Get by Referance (Name)

			if (Location_Index > -1)
			{
				App->CL_Locations->Goto_Location(Location_Index);
			}
			else
			{
				App->Say("Cant Find Location");
			}

			return TRUE;
		}

		// --------------------- SAVE LOCATION
		if (LOWORD(wParam) == IDC_BTSAVE_LOCATION_PLAYER)
		{
			App->CL_Locations->Save_Location();
			return TRUE;
		}


		if (LOWORD(wParam) == IDC_LSTLOCATIONS)
		{
			int Index = 0;

			Index = SendDlgItemMessage(hDlg, IDC_LSTLOCATIONS, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (Index == -1)
			{
				return 1;
			}

			App->CL_Locations->ListBox_Index = Index;

			return TRUE;
		}

		/*if (LOWORD(wParam) == IDC_PLAYERLOCATIONS_INFO)
		{
			App->Cl_Utilities->OpenHTML("Help\\PlayerLocations.html");
			return TRUE;
		}*/

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// *************************************************************************
// *  Init_Bmps_PlayerLocationsLocations:- Terry and Hazel Flanigan 2024   *
// *************************************************************************
void CL64_Locations::Init_Bmps_PlayerLocations()
{
	/*HWND Temp = GetDlgItem(Locations_Dlg_hWnd, IDC_PLAYERLOCATIONS_INFO);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_InfoSmall_Bmp);

	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);

	Temp = GetDlgItem(Locations_Dlg_hWnd, IDC_PLAYERLOCATIONS_INFO);
	TOOLINFO ti2 = { 0 };
	ti2.cbSize = sizeof(ti2);
	ti2.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti2.uId = (UINT_PTR)Temp;
	ti2.lpszText = (LPSTR)"Show Help File";
	ti2.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti2);*/

}

// *************************************************************************
// *	  		Goto_Locatio:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Locations::Goto_Location(int Index)
{
	App->CL_Scene->B_Player[0]->Player_Node->setPosition(App->CL_Scene->B_Locations[Index]->Current_Position);
	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(App->CL_Scene->B_Locations[Index]->Physics_Position);
	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Locations[Index]->Physics_Rotation);
	Set_To_PlayerView();

}

// *************************************************************************
// *	  		Set_To_PlayerView:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Locations::Set_To_PlayerView()
{
	if (App->CL_Scene->flag_Player_Added == 1)
	{
		App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;
		//App->SBC_TopTabs->Toggle_FirstCam_Flag = 1;
		//App->SBC_TopTabs->Toggle_FreeCam_Flag = 0;

		App->CL_Locations->Toggle_Player_Flag = 1;
		App->CL_Locations->Toggle_FreeCam_Flag = 0;

		App->CL_Scene->B_Player[0]->Player_Node->setVisible(false);

		int f = App->CL_Scene->B_Player[0]->Phys_Body->getCollisionFlags();
		App->CL_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f | (1 << 5));

		App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
		App->CL_Ogre->RenderFrame(7);
		App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;

		//App->CL_Physics->Physics_On(true);

		RedrawWindow(App->CL_TopDlg->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		RedrawWindow(App->CL_Locations->Locations_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		EnableWindow(GetDlgItem(Locations_Dlg_hWnd, IDC_BT_LOC_PLAYERTOCAMERA), 0);
	}
}

// *************************************************************************
// *	  		Set_To_FreeCam:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Locations::Set_To_FreeCam()
{
	if (App->CL_Scene->flag_Scene_Loaded == 1)
	{
		App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
		//App->CL_TopDlg->flag_Toggle_FirstCam = 0;
		//App->CL_TopDlg->flag_Toggle_FreeCam = 1;

		App->CL_Locations->Toggle_FreeCam_Flag = 1;
		App->CL_Locations->Toggle_Player_Flag = 0;

		int f = App->CL_Scene->B_Player[0]->Phys_Body->getCollisionFlags();
		App->CL_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));

		App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
		App->CL_Ogre->RenderFrame(7);
		App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;

		RedrawWindow(App->CL_TopDlg->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		RedrawWindow(App->CL_Locations->Locations_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		EnableWindow(GetDlgItem(Locations_Dlg_hWnd, IDC_BT_LOC_PLAYERTOCAMERA), 1);
	}
}

// *************************************************************************
// *	  		Delete_Location:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Locations::Delete_Location()
{
	int Index = App->CL_Locations->ListBox_Index;
	if (Index == 0)
	{
		App->Say("Start Location can not be deleted");
		return;
	}

	char buff[255];
	SendDlgItemMessage(Locations_Dlg_hWnd, IDC_LSTLOCATIONS, LB_GETTEXT, (WPARAM)Index, (LPARAM)(LPCTSTR)buff);

	int Location_Index = App->CL_LookUps->Player_Location_GetIndex_ByName(buff); // Get by Referance (Name)

	char tag[255];
	strcpy(tag, "Delete Location  ");
	strcat(tag, App->CL_Scene->B_Locations[Location_Index]->Name);

	App->CL_Dialogs->Show_YesNo_Dlg(tag, (LPSTR)"Are you sure", (LPSTR)"");
	if (App->CL_Dialogs->Canceled == 0)
	{
		App->CL_Scene->B_Locations[Location_Index]->Deleted = 1;

		SendDlgItemMessage(Locations_Dlg_hWnd, IDC_LSTLOCATIONS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
		int Count = 0;
		while (Count < App->CL_Scene->Player_Location_Count)
		{
			if (App->CL_Scene->B_Locations[Count]->Deleted == 0)
			{
				SendDlgItemMessage(Locations_Dlg_hWnd, IDC_LSTLOCATIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LPCTSTR)App->CL_Scene->B_Locations[Count]->Name);
			}

			Count++;
		}

		App->CL_Scene->flag_Scene_Modified = 1;
		App->CL_FileView->Mark_Altered_Folder(App->CL_FileView->FV_LevelFolder);
	}
}

// *************************************************************************
// *	  		Save_Location:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Locations::Save_Location()
{
	strcpy(App->CL_Dialogs->btext, "Location Name");

	char buf[255];
	char numbuf[255];
	_itoa(App->CL_Scene->Player_Location_Count, numbuf, 10);

	strcpy(buf, "New_Location_");
	strcat(buf, numbuf);
	strcpy(App->CL_Dialogs->Chr_Text, buf);

	App->CL_Dialogs->Dialog_Text(1);

	if (App->CL_Dialogs->Canceled == 0)
	{
		strcpy(buf, App->CL_Dialogs->Chr_Text);

		Create_Location_Entity(buf);

		SendDlgItemMessage(Locations_Dlg_hWnd, IDC_LSTLOCATIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LPCTSTR)buf);

		App->CL_Scene->flag_Scene_Modified = 1;
		App->CL_FileView->Mark_Altered_Folder(App->CL_FileView->FV_LevelFolder);
	}
}

// *************************************************************************
// *	  	Create_Location_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Locations::Create_Location_Entity(char* name)
{
	int Count = App->CL_Scene->Player_Location_Count;

	App->CL_Scene->B_Locations[Count] = new Base_Locations();

	App->CL_Scene->B_Locations[Count]->Deleted = 0;

	//App->CL_Scene->B_Locations[Count]->This_Object_UniqueID = App->CL_Scene->Locations_ID_Counter;

	strcpy(App->CL_Scene->B_Locations[Count]->Name, name);
	App->CL_Scene->B_Locations[Count]->Current_Position = App->CL_Scene->B_Player[0]->Player_Node->getPosition();
	App->CL_Scene->B_Locations[Count]->Physics_Position = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin();
	App->CL_Scene->B_Locations[Count]->Physics_Rotation = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getRotation();

	//App->CL_Scene->Locations_ID_Counter++;
	App->CL_Scene->Player_Location_Count++;
}

// *************************************************************************
// *	  	Move_Player_To_Camera:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Locations::Move_Player_To_Camera()
{
	//if (App->CL_Scene->flag_Scene_Loaded == 1)
	//{
	//	//App->SBC_Physics->Physics_On(false);

	//	Ogre::Vector3 Pos = App->CL_Ogre->mCamera->getPosition();
	//	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(btVector3(Pos.x, Pos.y, Pos.z));

	//	App->CL_Scene->B_Player[0]->Player_Node->setPosition(Pos);

	//	Set_To_PlayerView();
	//}
}
