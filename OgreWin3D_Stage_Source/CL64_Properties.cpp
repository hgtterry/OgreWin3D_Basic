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
#include "CL64_Properties.h"

CL64_Properties::CL64_Properties(void)
{
	flag_Properties_Dlg_Active = 0;
	flag_Edit_Physics = 0;

	Current_Selected_Object = 0;

	Edit_Category = Enums::Edit_Object;
	btext[0] = 0;

	Properties_Dlg_hWnd =	nullptr;
	Properties_hLV =		nullptr;
}

CL64_Properties::~CL64_Properties(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024 		 	   *
// *************************************************************************
void CL64_Properties::Reset_Class()
{
	//App->SBC_Com_Camera->Hide_Cam_Dlg(0);
	//App->SBC_Player->Hide_Player_Dlg(0);
	//App->SBC_Props_Dialog->Hide_Area_Dlg(0);
	Clear_Listview();
}

// *************************************************************************
// *			Clear_Listview:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Properties::Clear_Listview()
{
	ListView_DeleteAllItems(Properties_hLV);
}

// *************************************************************************
// *		Start_GD_Properties:- Terry and Hazel Flanigan 2024    	 	   *
// *************************************************************************
void CL64_Properties::Start_Properties(void)
{
	if (flag_Properties_Dlg_Active == 1)
	{
		return;
	}

	flag_Properties_Dlg_Active = 1;

	Properties_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPERTIES, App->Fdlg, (DLGPROC)Proc_Properties);
	ShowWindow(Properties_Dlg_hWnd, 1);

	HMENU mMenu = GetMenu(App->MainHwnd);
	CheckMenuItem(mMenu, ID_WINDOWS_PROPERTIES, MF_BYCOMMAND | MF_CHECKED);

	//Init_Bmps_Properties();

	Create_Properties_hLV();

}
// *************************************************************************
// *		Proc_Properties_Proc:- Terry and Hazel Flanigan 2024  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties::Proc_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		/*SendDlgItemMessage(hDlg, IDC_STOBJECTNAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		App->SBC_Properties->Edit_Category = Enums::FV_Edit_Object;*/

		return 1;
	}

	case WM_CTLCOLORSTATIC:
	{

		/*if (GetDlgItem(hDlg, IDC_STOBJECTNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->DialogBackGround;
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CLOSE:
	{
		App->CL_Properties->flag_Properties_Dlg_Active = 0;

		HMENU mMenu = GetMenu(App->MainHwnd);
		CheckMenuItem(mMenu, ID_WINDOWS_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 0);

		break;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->hwndFrom == App->CL_Properties->Properties_hLV)
		{
			switch (some_item->code)
			{
			case NM_CLICK:
			{
				App->CL_Properties->ListView_OnClickOptions(lParam);
			}
			}
		}

		/*if (some_item->idFrom == IDC_BT_OBJECTHELP2 && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		/*if (LOWORD(wParam) == IDC_BT_OBJECTHELP2)
		{
			if (App->SBC_Properties->Edit_Category == Enums::FV_Edit_Level)
			{
				App->Cl_Utilities->OpenHTML("Help\\LevelFolder.html");
				return 1;
			}

			return 1;

		}*/

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *		Create_Properties_hLV:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Properties::Create_Properties_hLV(void)
{
	int NUM_COLS = 2;
	Properties_hLV = CreateWindowEx(0, WC_LISTVIEW, "",
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_VSCROLL | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS, 7, 50,
		230, 280, Properties_Dlg_hWnd, 0, App->hInst, NULL);

	DWORD exStyles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	ListView_SetExtendedListViewStyleEx(Properties_hLV, exStyles, exStyles);

	ListView_SetBkColor(Properties_hLV, RGB(255, 255, 255));
	ListView_SetTextBkColor(Properties_hLV, RGB(255, 255, 255));
	//listView.CheckBoxes

	LV_COLUMN lvC;
	memset(&lvC, 0, sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column
	std::string headers[] =
	{
		"", ""
	};
	int headerSize[] =
	{
		100, 120
	};

	for (int header = 0; header < NUM_COLS; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<char*>(headers[header].c_str());
		ListView_InsertColumn(Properties_hLV, header, &lvC);
	}

	SendMessage(Properties_hLV, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

	return;
}

// *************************************************************************
// *		ListView_OnClickOptions:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Properties::ListView_OnClickOptions(LPARAM lParam)
{
	// Area
	if (Edit_Category == Enums::Edit_Area)
	{
		Edit_Area_Onclick(lParam);
		return;
	}

	// Camera
	if (Edit_Category == Enums::Edit_Camera)
	{
		Edit_Camera_Onclick(lParam);
		return;
	}

	// Player
	if (Edit_Category == Enums::Edit_Player)
	{
		Edit_Player(lParam);
		return;
	}

	// Objects
	if (Edit_Category == Enums::Edit_Object)
	{
		Edit_Object(lParam);
		return;
	}

	// Messages
	if (Edit_Category == Enums::Edit_Message)
	{
		Edit_Messages(lParam);
		return;
	}

	// Move Entity
	if (Edit_Category == Enums::Edit_Move_Entity)
	{
		Edit_Move_Entity(lParam);
		return;
	}

	// Sounds
	if (Edit_Category == Enums::Edit_Sounds)
	{
		Edit_Sounds(lParam);
		return;
	}

	// Teleports
	if (Edit_Category == Enums::Edit_Teleport)
	{
		Edit_Teleport_Entity(lParam);
		return;
	}

	// Collectables
	if (Edit_Category == Enums::Edit_Collectable)
	{
		Edit_Collectables(lParam);
		return;
	}

	// Counters
	if (Edit_Category == Enums::Edit_Counters)
	{
		Edit_Counters_OnClick(lParam);
		return;
	}

	// Environs
	if (Edit_Category == Enums::Edit_Environs)
	{
		Edit_Environs_OnClick(lParam);
		return;
	}

	// Particles
	if (Edit_Category == Enums::Edit_Particles)
	{
		Edit_Particle(lParam);
		return;
	}

	// Lights
	if (Edit_Category == Enums::Edit_Lights)
	{
		Edit_Light_Onclick(lParam);
		return;
	}

	//// UserObjects
	//if (Edit_Category == Enums::Edit_UserObjects)
	//{
	//	if (Edit_Physics == 0)
	//	{
	//		Edit_UserObjects_Onclick(lParam);
	//	}
	//	return;
	//}

	return;
}

// *************************************************************************
// *			Edit_Object:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Properties::Edit_Object(LPARAM lParam)
{
	int Index = Current_Selected_Object;
	int result = 1;
	int List_Index;

	Base_Object* Object = App->CL_Scene->B_Object[Index];

	LPNMLISTVIEW List = (LPNMLISTVIEW)lParam;
	List_Index = List->iItem;
	ListView_GetItemText(Properties_hLV, List_Index, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Objects->Rename_Object(Index);
		Update_ListView_Objects();
	}

	/*result = strcmp(btext, "Materials");
	if (result == 0)
	{
		App->SBC_Materials->Start_Material_Editor();
	}*/

	return 1;
}

// *************************************************************************
// *			Edit_Player:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Properties::Edit_Player(LPARAM lParam)
{
	int Index = Current_Selected_Object;
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Player->Rename_Player(Index);
		Update_ListView_Player();
	}

	result = strcmp(App->CL_Properties->btext, "Ground Speed");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.01,1, App->CL_Scene->B_Player[0]->Ground_speed / 100, (LPSTR)"Ground Speed");

		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			App->CL_Scene->B_Player[0]->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float * 100;
		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

			App->CL_Scene->B_Player[0]->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float * 100;

			App->CL_Scene->flag_Scene_Modified = 1;

			App->CL_Scene->B_Object[Index]->Altered = 1;
			App->CL_Scene->flag_Scene_Modified = 1;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy * 100;
			App->CL_Scene->B_Player[0]->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy * 100;

		}

		App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();
		
		return 1;
	}

	result = strcmp(App->CL_Properties->btext, "Turn Rate");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.01, 1, App->CL_Scene->B_Player[0]->TurnRate, (LPSTR)"Turn Rate");

		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			App->CL_Scene->B_Player[0]->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float;
		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

			App->CL_Scene->B_Player[0]->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Scene->flag_Scene_Modified = 1;

			App->CL_Scene->B_Object[Index]->Altered = 1;
			App->CL_Scene->flag_Scene_Modified = 1;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Scene->B_Player[0]->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;

		}

		App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(App->CL_Properties->btext, "Player Height");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.1, 2, App->CL_Scene->B_Player[0]->PlayerHeight, (LPSTR)"Player Height");

		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			App->CL_Scene->B_Player[0]->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float;
		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

			App->CL_Scene->B_Player[0]->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Scene->flag_Scene_Modified = 1;

			App->CL_Scene->B_Object[Index]->Altered = 1;
			App->CL_Scene->flag_Scene_Modified = 1;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Scene->B_Player[0]->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;

		}

		App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();
		return 1;
	}

	result = strcmp(btext, "Start Pos_X");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, App->CL_Scene->B_Player[0]->StartPos.x, (LPSTR)"Start Pos_X");
		
		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Scene->B_Player[0]->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

			App->CL_Scene->B_Player[0]->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Scene->B_Player[0]->Altered = 1;
			App->CL_Scene->flag_Scene_Modified = 1;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Player[0]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Scene->B_Player[0]->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Start Pos_Y");
	if (result == 0)
	{
		//App->CL_Player->Show_Player_And_Physics(true);
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, App->CL_Scene->B_Player[0]->StartPos.y, (LPSTR)"Start Pos_Y");

		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Scene->B_Player[0]->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

			App->CL_Scene->B_Player[0]->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Scene->B_Player[0]->Altered = 1;
			App->CL_Scene->flag_Scene_Modified = 1;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Player[0]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Scene->B_Player[0]->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Start Pos_Z");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, App->CL_Scene->B_Player[0]->StartPos.z, (LPSTR)"Start Pos_Z");

		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Scene->B_Player[0]->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

			App->CL_Scene->B_Player[0]->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Scene->B_Player[0]->Altered = 1;
			App->CL_Scene->flag_Scene_Modified = 1;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Player[0]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Scene->B_Player[0]->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	//result = strcmp(App->SBC_Properties->btext, "Look Up");
	//if (result == 0)
	//{
	//	App->SBC_Gui_Dialogs->Start_Dialog_Float(0.5, App->CL_Scene->B_Player[0]->Limit_Look_Up, "Player Look Up Limit");

	//	while (App->SBC_Gui_Dialogs->Show_Dialog_Float == 1)
	//	{
	//		App->SBC_Gui_Dialogs->BackGround_Render_Loop();
	//	}

	//	App->SBC_Gui_Dialogs->Show_Dialog_Float = 0;

	//	if (App->SBC_Gui_Dialogs->Float_Canceld == 0)
	//	{
	//		App->CL_Scene->B_Player[0]->Limit_Look_Up = App->SBC_Gui_Dialogs->m_Dialog_Float;

	//		App->CL_Scene->B_Player[0]->Altered = 1;
	//		App->CL_Scene->Scene_Modified = 1;
	//		App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Player[0]->FileViewItem);
	//	}

	//	App->Disable_Panels(false);

	//	Update_ListView_Player();

	//	return 1;
	//}

	//result = strcmp(App->SBC_Properties->btext, "Look Down");
	//if (result == 0)
	//{

	//	App->SBC_Gui_Dialogs->Start_Dialog_Float(0.5, App->CL_Scene->B_Player[0]->Limit_Look_Down, "Player Look Down Limit");

	//	while (App->SBC_Gui_Dialogs->Show_Dialog_Float == 1)
	//	{
	//		App->SBC_Gui_Dialogs->BackGround_Render_Loop();
	//	}

	//	App->SBC_Gui_Dialogs->Show_Dialog_Float = 0;

	//	if (App->SBC_Gui_Dialogs->Float_Canceld == 0)
	//	{
	//		App->CL_Scene->B_Player[0]->Limit_Look_Down = App->SBC_Gui_Dialogs->m_Dialog_Float;

	//		App->CL_Scene->B_Player[0]->Altered = 1;
	//		App->CL_Scene->Scene_Modified = 1;
	//		App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Player[0]->FileViewItem);
	//	}

	//	App->Disable_Panels(false);

	//	Update_ListView_Player();

	//	return 1;
	//}


	return 1;
}

// *************************************************************************
// *				Edit_Messages:- Terry and Hazel Flanigan 2024          *
// *************************************************************************
bool CL64_Properties::Edit_Messages(LPARAM lParam)
{
	int Index = Current_Selected_Object; // Get Selected Object Index 
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	// Name
	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Messages->Rename_Message_Entity(Index);
		Update_ListView_Messages();

		return 1;
	}

	result = strcmp(btext, "Text");
	if (result == 0)
	{
		strcpy(App->CL_Dialogs->btext, "Change Text");
		strcpy(App->CL_Dialogs->Chr_Text, App->CL_Scene->B_Object[Index]->S_Message[0]->Message_Text);

		App->CL_Dialogs->Dialog_Text_Props();

		if (App->CL_Dialogs->Canceled == 1)
		{
			return TRUE;
		}

		strcpy(App->CL_Scene->B_Object[Index]->S_Message[0]->Message_Text, App->CL_Dialogs->Chr_Text);

		App->CL_Properties->Mark_As_Altered(Index);

		Update_ListView_Messages();

		return 1;
	}

	result = strcmp(btext, "Pos_X");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_MessageEditor(Index);

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_MessageEditor == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_MessageEditor = 0;

		App->CL_Scene->B_Object[Index]->Show_Message_Flag = 0;

		App->CL_Properties->Mark_As_Altered(Index);

		App->CL_Panels->Disable_Panels(false);
		App->CL_Panels->Show_FileView(true);
		App->CL_Panels->Show_Properties(true);

		Update_ListView_Messages();

		return 1;
	}

	result = strcmp(btext, "Pos_Y");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_MessageEditor(Index);

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_MessageEditor == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_MessageEditor = 0;

		App->CL_Scene->B_Object[Index]->Show_Message_Flag = 0;

		App->CL_Properties->Mark_As_Altered(Index);

		App->CL_Panels->Disable_Panels(false);
		App->CL_Panels->Show_FileView(true);
		App->CL_Panels->Show_Properties(true);

		Update_ListView_Messages();

		return 1;
	}

	// Counter
	result = strcmp(btext, "Counter");
	if (result == 0)
	{
		App->CL_Dialogs->Dialog_Counter();

		if (App->CL_Dialogs->Canceled == 1)
		{
			return 1;
		}

		App->CL_Properties->Mark_As_Altered(Index);
		Update_ListView_Messages();

		return 1;
	}

	return 1;
}

// *************************************************************************
// *			Edit_Sounds:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Properties::Edit_Sounds(LPARAM lParam)
{
	int Index = Current_Selected_Object; // Get Selected Object Index 
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	// Name
	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Sounds->Rename_Sound(Index);
		Update_ListView_Sounds();
	}


	// Sound
	result = strcmp(btext, "Sound");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1;
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->Sound_File);

		App->CL_SoundMgr->Dialog_SoundFile();

		strcpy(App->CL_Scene->B_Object[Index]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->SndVolume = App->CL_SoundMgr->SndVolume;

		Mark_As_Altered(Index);

		Update_ListView_Sounds();

		return 1;
	}

	// Sound
	result = strcmp(btext, "Volume");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1;
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->Sound_File);

		App->CL_SoundMgr->Dialog_SoundFile();

		strcpy(App->CL_Scene->B_Object[Index]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->SndVolume = App->CL_SoundMgr->SndVolume;

		Mark_As_Altered(Index);

		Update_ListView_Sounds();

		return 1;
	}

	// Sound
	result = strcmp(btext, "Play");
	if (result == 0)
	{

		/*App->CL_Dialogs->Show_YesNo_Dlg("Play Sound",App->CL_Scene->B_Object[Index]->Sound_File,true);

		if (App->CL_Dialogs->Canceled == 0)
		{
			App->CL_Scene->B_Object[Index]->
		}*/

		Update_ListView_Teleport();

		return 1;
	}

	return 1;
}

// *************************************************************************
// *			Edit_Move_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Properties::Edit_Move_Entity(LPARAM lParam)
{
	int Index = Current_Selected_Object; // Get Selected Object Index 
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_MoveEntity->Rename_Move_Entity(Index);
		Update_ListView_Move_Entities();

		//App->SBC_Physics->Reset_Triggers();
	}

	result = strcmp(btext, "Move_Object");
	if (result == 0)
	{

		App->CL_ImGui_Dialogs->Start_Move_Entity_Editor(Index);

		while (App->CL_ImGui_Dialogs->flag_Show_Move_Ent_Editor == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();
		}

		App->CL_ImGui_Dialogs->flag_Show_Move_Ent_Editor = 0;

		App->CL_Properties->Mark_As_Altered(Index);

		App->CL_Panels->Disable_Panels(false);
		App->CL_Panels->Show_FileView(true);
		App->CL_Panels->Show_Properties(true);

		Update_ListView_Move_Entities();

		/*strcpy(App->Cl_Dialogs->btext, "Select Object to Move");
		strcpy(App->SBC_Dialogs->Chr_DropText, App->CL_Scene->B_Object[Index]->S_MoveType[0]->Object_Name);

		App->SBC_Dialogs->DropList_Data = Enums::DropDialog_TrigMoveObject;
		App->SBC_Dialogs->Dialog_DropGen();


		if (App->SBC_Dialogs->Canceled == 0)
		{
			strcpy(App->CL_Scene->B_Object[Index]->S_MoveType[0]->Object_Name, App->SBC_Dialogs->Chr_DropText);

			int MoveObjectIndex = App->CL_Object->GetIndex_By_Name(App->CL_Scene->B_Object[Index]->S_MoveType[0]->Object_Name);

			App->CL_Scene->B_Object[Index]->S_MoveType[0]->Object_To_Move_Index = MoveObjectIndex;

			App->CL_Scene->B_Object[Index]->S_MoveType[0]->MeshPos.x = App->CL_Scene->B_Object[MoveObjectIndex]->Mesh_Pos.x;
			App->CL_Scene->B_Object[Index]->S_MoveType[0]->MeshPos.y = App->CL_Scene->B_Object[MoveObjectIndex]->Mesh_Pos.y;
			App->CL_Scene->B_Object[Index]->S_MoveType[0]->MeshPos.z = App->CL_Scene->B_Object[MoveObjectIndex]->Mesh_Pos.z;

			App->CL_Scene->B_Object[Index]->S_MoveType[0]->PhysicsPos.x = App->CL_Scene->B_Object[MoveObjectIndex]->Physics_Pos.x;
			App->CL_Scene->B_Object[Index]->S_MoveType[0]->PhysicsPos.y = App->CL_Scene->B_Object[MoveObjectIndex]->Physics_Pos.y;
			App->CL_Scene->B_Object[Index]->S_MoveType[0]->PhysicsPos.z = App->CL_Scene->B_Object[MoveObjectIndex]->Physics_Pos.z;

			Mark_As_Altered(Index);

			Update_ListView_Move_Entities();

			App->SBC_Physics->Reset_Triggers();

		}*/

		return 1;
	}

	result = strcmp(btext, "Axis");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Move_Entity_Editor(Index);

		while (App->CL_ImGui_Dialogs->flag_Show_Move_Ent_Editor == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();
		}

		App->CL_ImGui_Dialogs->flag_Show_Move_Ent_Editor = 0;

		App->CL_Properties->Mark_As_Altered(Index);

		App->CL_Panels->Disable_Panels(false);
		App->CL_Panels->Show_FileView(true);
		App->CL_Panels->Show_Properties(true);

		Update_ListView_Move_Entities();

		return 1;
	}

	result = strcmp(btext, "Distance");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Move_Entity_Editor(Index);

		while (App->CL_ImGui_Dialogs->flag_Show_Move_Ent_Editor == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();
		}

		App->CL_ImGui_Dialogs->flag_Show_Move_Ent_Editor = 0;

		//App->CL_Scene->B_Object[Index]->Show_Message_Flag = 0;

		App->CL_Properties->Mark_As_Altered(Index);

		App->CL_Panels->Disable_Panels(false);
		App->CL_Panels->Show_FileView(true);
		App->CL_Panels->Show_Properties(true);

		Update_ListView_Move_Entities();

		return 1;
	}

	result = strcmp(btext, "Speed");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Move_Entity_Editor(Index);

		while (App->CL_ImGui_Dialogs->flag_Show_Move_Ent_Editor == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();
		}

		App->CL_ImGui_Dialogs->flag_Show_Move_Ent_Editor = 0;

		//App->CL_Scene->B_Object[Index]->Show_Message_Flag = 0;

		App->CL_Properties->Mark_As_Altered(Index);

		App->CL_Panels->Disable_Panels(false);
		App->CL_Panels->Show_FileView(true);
		App->CL_Panels->Show_Properties(true);

		Update_ListView_Move_Entities();

		return 1;
	}

	// Volume
	result = strcmp(btext, "Volume");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1;
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->Sound_File);

		App->CL_SoundMgr->Dialog_SoundFile();

		strcpy(App->CL_Scene->B_Object[Index]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->SndVolume = App->CL_SoundMgr->SndVolume;

		Mark_As_Altered(Index);

		Update_ListView_Move_Entities();

		App->CL_Physics->Reset_Triggers();

		return 1;
	}

	// Sound
	result = strcmp(btext, "Sound");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1;
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->Sound_File);

		App->CL_SoundMgr->Dialog_SoundFile();

		strcpy(App->CL_Scene->B_Object[Index]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->SndVolume = App->CL_SoundMgr->SndVolume;

		Mark_As_Altered(Index);

		Update_ListView_Move_Entities();

		App->CL_Physics->Reset_Triggers();

		return 1;
	}

	// Play
	result = strcmp(btext, "Play");
	if (result == 0)
	{

		strcpy(App->CL_Dialogs->btext, "Play Sound");

		App->CL_Dialogs->TrueFlase = App->CL_Scene->B_Object[Index]->Play_Sound;

		App->CL_Dialogs->Dialog_TrueFlase(App->MainHwnd);

		if (App->CL_Dialogs->Canceled == 0)
		{
			if (App->CL_Dialogs->TrueFlase == 1)
			{
				App->CL_Scene->B_Object[Index]->Play_Sound = 1;
			}
			else
			{
				App->CL_Scene->B_Object[Index]->Play_Sound = 0;

			}
		}

		Mark_As_Altered(Index);

		Update_ListView_Move_Entities();

		App->CL_Physics->Reset_Triggers();

		return 1;
	}

	// Counter
	result = strcmp(btext, "Counter");
	if (result == 0)
	{
		App->CL_Dialogs->Dialog_Counter();
		if (App->CL_Dialogs->Canceled == 1)
		{
			return 1;
		}

		Mark_As_Altered(Index);
		Update_ListView_Move_Entities();

		//App->SBC_Physics->Reset_Triggers();

		return 1;
	}

	return 1;
}

// *************************************************************************
// *		Edit_Teleport_Entity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Properties::Edit_Teleport_Entity(LPARAM lParam)
{
	int Index = Current_Selected_Object; // Get Selected Object Index 
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Teleporters->Rename_Teleport_Entity(Index);
		Update_ListView_Teleport();
	}

	result = strcmp(btext, "Goto");
	if (result == 0)
	{
		strcpy(App->CL_ImGui_Dialogs->List_Banner, "Select Location");

		App->CL_ImGui_Dialogs->List_Strings.resize(App->CL_Scene->Player_Location_Count);
		App->CL_ImGui_Dialogs->List_Count = App->CL_Scene->Player_Location_Count;
		App->CL_ImGui_Dialogs->List_Index = App->CL_Scene->B_Object[Index]->S_Teleport[0]->Location_ID;

		int Count = 0;
		while (Count < App->CL_Scene->Player_Location_Count)
		{
			if (App->CL_Scene->B_Locations[Count]->Deleted == 0)
			{
				App->CL_ImGui_Dialogs->List_Strings[Count] = App->CL_Scene->B_Locations[Count]->Name;
			}

			Count++;
		}

		App->CL_ImGui_Dialogs->Start_Dialog_List();

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_list == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_list = 0;
		App->CL_Panels->Disable_Panels(false);


		if (App->CL_ImGui_Dialogs->flag_List_Canceled == 0)
		{
			int LocationIndex = App->CL_ImGui_Dialogs->List_Index;

			App->CL_Scene->B_Object[Index]->S_Teleport[0]->Location_ID = LocationIndex;

			strcpy(App->CL_Scene->B_Object[Index]->S_Teleport[0]->Location_Name, App->CL_Scene->B_Locations[LocationIndex]->Name);

			App->CL_Scene->B_Object[Index]->S_Teleport[0]->Player_Position = App->CL_Scene->B_Locations[LocationIndex]->Current_Position;
			App->CL_Scene->B_Object[Index]->S_Teleport[0]->Physics_Position = App->CL_Scene->B_Locations[LocationIndex]->Physics_Position;
			App->CL_Scene->B_Object[Index]->S_Teleport[0]->Physics_Rotation = App->CL_Scene->B_Locations[LocationIndex]->Physics_Rotation;

			Update_ListView_Teleport();
		}

	}

	// Sound
	result = strcmp(btext, "Sound");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1; // For Sound Manager Dlg
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File);

		App->CL_SoundMgr->Dialog_SoundFile();

		strcpy(App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->S_Teleport[0]->SndVolume = App->CL_SoundMgr->SndVolume;

		Mark_As_Altered(Index);

		Update_ListView_Teleport();
		return 1;
	}

	// Volume
	result = strcmp(btext, "Volume");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1; // For Sound Manager Dlg
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File);

		App->CL_SoundMgr->Dialog_SoundFile();

		strcpy(App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->S_Teleport[0]->SndVolume = App->CL_SoundMgr->SndVolume;

		Mark_As_Altered(Index);

		Update_ListView_Teleport();
		return 1;
	}

	result = strcmp(btext, "Play");
	if (result == 0)
	{
		App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Play Sound", App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File, (LPSTR)"");

		if (App->CL_Dialogs->Canceled == 0)
		{
			App->CL_Scene->B_Object[Index]->S_Teleport[0]->Play = 1;
		}
		else
		{
			App->CL_Scene->B_Object[Index]->S_Teleport[0]->Play = 0;
		}

		Update_ListView_Teleport();

		return 1;
	}

	// Counter
	result = strcmp(btext, "Counter");
	if (result == 0)
	{
		App->CL_Dialogs->Dialog_Counter();
		if (App->CL_Dialogs->Canceled == 1)
		{
			return 1;
		}

		Mark_As_Altered(Index);
		Update_ListView_Teleport();

		return 1;
	}

	result = strcmp(btext, "Count_Name");
	if (result == 0)
	{
		App->CL_Dialogs->Dialog_Counter();
		if (App->CL_Dialogs->Canceled == 1)
		{
			return 1;
		}

		Mark_As_Altered(Index);
		Update_ListView_Teleport();

		return 1;
	}
	
	result = strcmp(btext, "Count_Index");
	if (result == 0)
	{
		App->CL_Dialogs->Dialog_Counter();
		if (App->CL_Dialogs->Canceled == 1)
		{
			return 1;
		}

		Mark_As_Altered(Index);
		Update_ListView_Teleport();

		return 1;
	}

	// Environment
	result = strcmp(btext, "Environment");
	if (result == 0)
	{
		//App->SBC_Gui_Environ->Start_Environment_Editor(Index, 1);

		/*App->SBC_Dialogs->YesNo("Enable Environment","Enable Teleport Environment", true);

		if (App->SBC_Dialogs->Canceled == 0)
		{
			App->CL_Scene->B_Object[Index]->S_Environ[0]->Enabled = 1;
		}
		else
		{
			App->CL_Scene->B_Object[Index]->S_Environ[0]->Enabled = 0;
		}*/

		//Update_ListView_Teleport();

		return 1;
	}

	result = strcmp(btext, "Main Light");
	if (result == 0)
	{
		//App->Cl_Environment->Start_Environment("Main Light");
		//Update_ListView_Teleport();

		return 1;
	}

	result = strcmp(btext, "Sound_Env");
	if (result == 0)
	{
		//App->Cl_Environment->Start_Environment("Sound");
		//Update_ListView_Teleport();
		return 1;
	}

	result = strcmp(btext, "Fog");
	if (result == 0)
	{
		//App->Cl_Environment->Start_Environment("Fog");
		//Update_ListView_Teleport();
		return 1;
	}

	result = strcmp(btext, "Sky");
	if (result == 0)
	{
		//App->Cl_Environment->Start_Environment("Sky");
		//Update_ListView_Teleport();
		return 1;
	}
	return 1;
}

// *************************************************************************
// *		Edit_Environs_OnClick:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Properties::Edit_Environs_OnClick(LPARAM lParam)
{
	int Index = Current_Selected_Object;

	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Environments->Rename_Environ_Entity(Index);
		Update_ListView_Environs();
	}

	result = strcmp(btext, "Evironment");
	if (result == 0)
	{
		App->CL_Gui_Environment->Start_Environment_Editor(Index, false);
		return 1;
	}

	return 1;
}

// *************************************************************************
// *		Edit_Particle_Onclick:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Properties::Edit_Particle(LPARAM lParam)
{
	int Index = Current_Selected_Object;
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Particles->Rename_Particle_Entity(Index);
		Update_ListView_Particles();
	}

	result = strcmp(btext, "Speed");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Float_Combo_Step = 4;
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.01,4, App->CL_Scene->B_Object[Index]->S_Particle[0]->SpeedFactor,(LPSTR) "Particle Speed");

		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			App->CL_Scene->B_Object[Index]->S_Particle[0]->SpeedFactor = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Scene->B_Object[Index]->S_Particle[0]->Particle->setSpeedFactor(App->CL_ImGui_Dialogs->m_Dialog_Float);

		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

			App->CL_Scene->B_Object[Index]->S_Particle[0]->SpeedFactor = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Scene->flag_Scene_Modified = 1;

			App->CL_Scene->B_Object[Index]->Altered = 1;
			App->CL_Scene->flag_Scene_Modified = 1;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Scene->B_Object[Index]->S_Particle[0]->SpeedFactor = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Scene->B_Object[Index]->S_Particle[0]->Particle->setSpeedFactor(App->CL_ImGui_Dialogs->m_Dialog_Float);
		}

		App->CL_Panels->Disable_Panels(false);

		Update_ListView_Particles();
	}

}

// *************************************************************************
// *		Edit_Area_Onclick:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Properties::Edit_Area_Onclick(LPARAM lParam)
{
	int Index = Current_Selected_Object;
	int result = 1;
	int List_Index;

	Base_Area* Area = App->CL_Scene->B_Area[Index];

	LPNMLISTVIEW List = (LPNMLISTVIEW)lParam;
	List_Index = List->iItem;
	ListView_GetItemText(Properties_hLV, List_Index, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Area->Rename_Area(Index);
		Update_ListView_Area();
	}

	//result = strcmp(btext, "Materials");
	//if (result == 0)
	//{
	//	App->SBC_Materials->Start_Material_Editor();
	//}

	return 1;
}

// *************************************************************************
// *		Edit_Camera_Onclick:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Properties::Edit_Camera_Onclick(LPARAM lParam)
{
	//int Index = App->SBC_Properties->Current_Selected_Object; // Get Selected Object Index 
	//int result = 1;
	//int test;

	//LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	//test = poo->iItem;
	//ListView_GetItemText(App->SBC_Properties->Properties_hLV, test, 0, App->SBC_Properties->btext, 20);

	//result = strcmp(App->SBC_Properties->btext, "Name");
	//if (result == 0)
	//{
	//	App->SBC_Com_Camera->Rename_Camera(Index);
	//	Update_ListView_Camera();
	//}

	//result = strcmp(App->SBC_Properties->btext, "Pos_X");
	//if (result == 0)
	//{
	//	char chr_Value[10];
	//	sprintf(chr_Value, "%.3f ", App->CL_Scene->B_Camera[Index]->CamPos.x);

	//	strcpy(App->Cl_Dialogs->Chr_Float, chr_Value);
	//	strcpy(App->Cl_Dialogs->btext, "Position X");

	//	App->Cl_Dialogs->Dialog_Float();
	//	if (App->Cl_Dialogs->Canceled == 1) { return TRUE; }

	//	App->CL_Scene->B_Camera[Index]->CamPos.x = App->Cl_Dialogs->mFloat;

	//	App->CL_Scene->B_Camera[Index]->Altered = 1;
	//	App->CL_Scene->Scene_Modified = 1;
	//	App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Camera[Index]->FileViewItem);

	//	Update_ListView_Camera();

	//	return 1;
	//}

	//result = strcmp(App->SBC_Properties->btext, "Pos_Y");
	//if (result == 0)
	//{
	//	char chr_Value[10];
	//	sprintf(chr_Value, "%.3f ", App->CL_Scene->B_Camera[Index]->CamPos.y);

	//	strcpy(App->Cl_Dialogs->Chr_Float, chr_Value);
	//	strcpy(App->Cl_Dialogs->btext, "Position Y");

	//	App->Cl_Dialogs->Dialog_Float();
	//	if (App->Cl_Dialogs->Canceled == 1) { return TRUE; }

	//	App->CL_Scene->B_Camera[Index]->CamPos.y = App->Cl_Dialogs->mFloat;

	//	App->CL_Scene->B_Camera[Index]->Altered = 1;
	//	App->CL_Scene->Scene_Modified = 1;
	//	App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Camera[Index]->FileViewItem);

	//	Update_ListView_Camera();

	//	return 1;
	//}

	//result = strcmp(App->SBC_Properties->btext, "Pos_Z");
	//if (result == 0)
	//{
	//	char chr_Value[10];
	//	sprintf(chr_Value, "%.3f ", App->CL_Scene->B_Camera[Index]->CamPos.z);

	//	strcpy(App->Cl_Dialogs->Chr_Float, chr_Value);
	//	strcpy(App->Cl_Dialogs->btext, "Position Z");

	//	App->Cl_Dialogs->Dialog_Float();
	//	if (App->Cl_Dialogs->Canceled == 1) { return TRUE; }

	//	App->CL_Scene->B_Camera[Index]->CamPos.z = App->Cl_Dialogs->mFloat;

	//	App->CL_Scene->B_Camera[Index]->Altered = 1;
	//	App->CL_Scene->Scene_Modified = 1;
	//	App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Camera[Index]->FileViewItem);

	//	Update_ListView_Camera();

	//	return 1;
	//}

	//result = strcmp(App->SBC_Properties->btext, "LookAt_X");
	//if (result == 0)
	//{
	//	char chr_Value[10];
	//	sprintf(chr_Value, "%.3f ", App->SBC_Com_Camera->LookAt_X);

	//	strcpy(App->Cl_Dialogs->Chr_Float, chr_Value);
	//	strcpy(App->Cl_Dialogs->btext, "Look At X");

	//	App->Cl_Dialogs->Dialog_Float();
	//	if (App->Cl_Dialogs->Canceled == 1) { return TRUE; }

	//	App->SBC_Com_Camera->LookAt_X = App->Cl_Dialogs->mFloat;

	//	App->CL_Scene->B_Camera[Index]->Altered = 1;
	//	App->CL_Scene->Scene_Modified = 1;
	//	App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Camera[Index]->FileViewItem);

	//	Update_ListView_Camera();

	//	return 1;
	//}

	//result = strcmp(App->SBC_Properties->btext, "LookAt_Y");
	//if (result == 0)
	//{
	//	char chr_Value[10];
	//	sprintf(chr_Value, "%.3f ", App->SBC_Com_Camera->LookAt_Y);

	//	strcpy(App->Cl_Dialogs->Chr_Float, chr_Value);
	//	strcpy(App->Cl_Dialogs->btext, "Look At Y");

	//	App->Cl_Dialogs->Dialog_Float();
	//	if (App->Cl_Dialogs->Canceled == 1) { return TRUE; }

	//	App->SBC_Com_Camera->LookAt_Y = App->Cl_Dialogs->mFloat;

	//	App->CL_Scene->B_Camera[Index]->Altered = 1;
	//	App->CL_Scene->Scene_Modified = 1;
	//	App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Camera[Index]->FileViewItem);

	//	Update_ListView_Camera();

	//	return 1;
	//}

	//result = strcmp(App->SBC_Properties->btext, "LookAt_Z");
	//if (result == 0)
	//{
	//	char chr_Value[10];
	//	sprintf(chr_Value, "%.3f ", App->SBC_Com_Camera->LookAt_Z);

	//	strcpy(App->Cl_Dialogs->Chr_Float, chr_Value);
	//	strcpy(App->Cl_Dialogs->btext, "Look At Z");

	//	App->Cl_Dialogs->Dialog_Float();
	//	if (App->Cl_Dialogs->Canceled == 1) { return TRUE; }

	//	App->SBC_Com_Camera->LookAt_Z = App->Cl_Dialogs->mFloat;

	//	App->CL_Scene->B_Camera[Index]->Altered = 1;
	//	App->CL_Scene->Scene_Modified = 1;
	//	App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Camera[Index]->FileViewItem);

	//	Update_ListView_Camera();

	//	return 1;
	//}

	return 1;
}

// *************************************************************************
// *		Edit_Collectables:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Properties::Edit_Collectables(LPARAM lParam)
{
	int Index = Current_Selected_Object; // Get Selected Object Index 
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Collectables->Rename_Collectable(Index);
		Update_ListView_Collectables();

		//App->SBC_Physics->Reset_Triggers();
	}

	//result = strcmp(btext, "Materials");
	//if (result == 0)
	//{
	//	App->SBC_Materials->Start_Material_Editor();
	//}

	result = strcmp(btext, "Volume");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1;
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->S_Collectable[0]->Sound_File);

		App->CL_SoundMgr->Dialog_SoundFile();

		strcpy(App->CL_Scene->B_Object[Index]->S_Collectable[0]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->S_Collectable[0]->SndVolume = App->CL_SoundMgr->SndVolume;

		Mark_As_Altered(Index);

		Update_ListView_Collectables();

		App->CL_Physics->Reset_Triggers();

		return 1;
	}

	//Sound
	result = strcmp(btext, "Sound");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1;
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->S_Collectable[0]->Sound_File);

		App->CL_SoundMgr->Dialog_SoundFile();

		strcpy(App->CL_Scene->B_Object[Index]->S_Collectable[0]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->S_Collectable[0]->SndVolume = App->CL_SoundMgr->SndVolume;

		Mark_As_Altered(Index);

		Update_ListView_Collectables();

		App->CL_Physics->Reset_Triggers();

		return 1;
	}

	// Play
	result = strcmp(btext, "Play");
	if (result == 0)
	{

		strcpy(App->CL_Dialogs->btext, "Play Sound In The Game");

		App->CL_Dialogs->TrueFlase = App->CL_Scene->B_Object[Index]->S_Collectable[0]->Play;

		App->CL_Dialogs->Dialog_TrueFlase(App->MainHwnd);

		if (App->CL_Dialogs->Canceled == 0)
		{
			if (App->CL_Dialogs->TrueFlase == 1)
			{
				App->CL_Scene->B_Object[Index]->S_Collectable[0]->Play = 1;
			}
			else
			{
				App->CL_Scene->B_Object[Index]->S_Collectable[0]->Play = 0;

			}
		}

		Mark_As_Altered(Index);

		Update_ListView_Collectables();

		App->CL_Physics->Reset_Triggers();

		return 1;
	}

	result = strcmp(btext, "Counter");
	if (result == 0)
	{
		App->CL_Dialogs->Dialog_Counter();

		if (App->CL_Dialogs->Canceled == 1)
		{
			return 1;
		}

		Mark_As_Altered(Index);
		Update_ListView_Collectables();
		App->CL_Physics->Reset_Triggers();

		return 1;
	}

	return 1;
}

// *************************************************************************
// *		Edit_Counters_OnClick:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Properties::Edit_Counters_OnClick(LPARAM lParam)
{
	int Index = Current_Selected_Object; // Get Selected Object Index 
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Display->Rename_Counter(Index);
		Update_ListView_Counters();
	}

	result = strcmp(btext, "Pos_X");
	if (result == 0)
	{
		App->CL_Panels->Disable_Panels(true);
		App->CL_Panels->Hide_Panels(true);

		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.5,3, App->CL_Scene->B_Counter[Index]->PosX, (LPSTR)"X Position");

		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Scene->B_Counter[Index]->PosX = App->CL_ImGui_Dialogs->m_Dialog_Float;
		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;
			App->CL_Scene->B_Counter[Index]->PosX = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Display->Mark_As_Altered_Counter(Index);
		}
		else
		{
			App->CL_Scene->B_Counter[Index]->PosX = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
		}

		App->CL_Panels->Disable_Panels(false);
		App->CL_Panels->Hide_Panels(false);

		Update_ListView_Counters();

		return 1;
	}

	result = strcmp(btext, "Pos_Y");
	if (result == 0)
	{
		App->CL_Panels->Disable_Panels(true);
		App->CL_Panels->Hide_Panels(true);

		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.5,3, App->CL_Scene->B_Counter[Index]->PosY, (LPSTR)"Y Position");

		while (App->CL_ImGui_Dialogs->Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Scene->B_Counter[Index]->PosY = App->CL_ImGui_Dialogs->m_Dialog_Float;
		}

		App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->Show_Dialog_Float = 0;
			App->CL_Scene->B_Counter[Index]->PosY = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Display->Mark_As_Altered_Counter(Index);
		}
		else
		{
			App->CL_Scene->B_Counter[Index]->PosY = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
		}

		App->CL_Panels->Disable_Panels(false);
		App->CL_Panels->Hide_Panels(false);
		
		Update_ListView_Counters();

		return 1;
	}

	result = strcmp(btext, "Text");
	if (result == 0)
	{
		strcpy(App->CL_Dialogs->btext, "Change Text");
		strcpy(App->CL_Dialogs->Chr_Text, App->CL_Scene->B_Counter[Index]->Text);

		App->CL_Dialogs->Dialog_Text(0);

		if (App->CL_Dialogs->Canceled == 1)
		{
			return TRUE;
		}

		strcpy(App->CL_Scene->B_Counter[Index]->Text, App->CL_Dialogs->Chr_Text);


		App->CL_Display->Mark_As_Altered_Counter(Index);

		Update_ListView_Counters();

		return 1;
	}

	result = strcmp(btext, "Start_Value");
	if (result == 0)
	{
		strcpy(App->CL_Dialogs->btext, "Start Value");
		char buff[256];
		sprintf(buff, "%i", App->CL_Scene->B_Counter[Index]->Start_Value);
		strcpy(App->CL_Dialogs->Chr_Int, buff);

		App->CL_Dialogs->Dialog_Int();

		if (App->CL_Dialogs->Canceled == 1)
		{
			return TRUE;
		}

		App->CL_Scene->B_Counter[Index]->Start_Value = App->CL_Dialogs->mInt;
		App->CL_Scene->B_Counter[Index]->Counter = App->CL_Dialogs->mInt;

		App->CL_Display->Mark_As_Altered_Counter(Index);

		Update_ListView_Counters();

		return 1;
	}

	result = strcmp(btext, "Display");
	if (result == 0)
	{
		strcpy(App->CL_ImGui_Dialogs->List_Banner, "Display Mode");

		App->CL_ImGui_Dialogs->List_Strings.resize(2);
		App->CL_ImGui_Dialogs->List_Count = 2;
		App->CL_ImGui_Dialogs->List_Index = App->CL_Scene->B_Counter[Index]->Show_Panel_Flag;

		App->CL_ImGui_Dialogs->List_Strings[0] = "Auto";
		App->CL_ImGui_Dialogs->List_Strings[1] = "Always";
		
		App->CL_ImGui_Dialogs->Start_Dialog_List();

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_list == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_list = 0;
		App->CL_Panels->Disable_Panels(false);

		int SelectionIndex = App->CL_ImGui_Dialogs->List_Index;

		if (App->CL_ImGui_Dialogs->flag_List_Canceled == 0)
		{
			if (SelectionIndex == 0)
			{
				App->CL_Scene->B_Counter[Index]->Show_Panel_Flag = 0;
			}

			if (SelectionIndex == 1)
			{
				App->CL_Scene->B_Counter[Index]->Show_Panel_Flag = 1;
			}

			App->CL_Display->Mark_As_Altered_Counter(Index);
		}

		Update_ListView_Counters();
		
		return 1;
	}

	return 1;
}

// *************************************************************************
// *		Edit_Light_Onclick:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Properties::Edit_Light_Onclick(LPARAM lParam)
{
	/*int Index = App->SBC_Properties->Current_Selected_Object;
	int result = 1;
	int test;

	LPNMLISTVIEW poo = (LPNMLISTVIEW)lParam;
	test = poo->iItem;
	ListView_GetItemText(Properties_hLV, test, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Object->Rename_Object(Index);
		Update_ListView_Particles();
	}

	result = strcmp(btext, "PosX");
	if (result == 0)
	{

		App->SBC_Gui_Dialogs->Start_Dialog_Float(1, App->CL_Scene->B_Object[Index]->Mesh_Pos.x, "Pos X");

		while (App->SBC_Gui_Dialogs->Show_Dialog_Float == 1)
		{
			App->SBC_Gui_Dialogs->BackGround_Render_Loop();

			App->CL_Scene->B_Object[Index]->Mesh_Pos.x = App->SBC_Gui_Dialogs->m_Dialog_Float;
			App->CL_Scene->B_Object[Index]->Object_Node->setPosition(App->CL_Scene->B_Object[Index]->Mesh_Pos);

		}

		App->Disable_Panels(false);

		Update_ListView_Lights();
	}

	result = strcmp(btext, "PosY");
	if (result == 0)
	{

		App->SBC_Gui_Dialogs->Start_Dialog_Float(1, App->CL_Scene->B_Object[Index]->Mesh_Pos.y, "Pos Y");

		while (App->SBC_Gui_Dialogs->Show_Dialog_Float == 1)
		{
			App->SBC_Gui_Dialogs->BackGround_Render_Loop();

			App->CL_Scene->B_Object[Index]->Mesh_Pos.y = App->SBC_Gui_Dialogs->m_Dialog_Float;
			App->CL_Scene->B_Object[Index]->Object_Node->setPosition(App->CL_Scene->B_Object[Index]->Mesh_Pos);

		}

		App->Disable_Panels(false);

		Update_ListView_Lights();
	}

	result = strcmp(btext, "Editor");
	if (result == 0)
	{
		App->SBC_Gui_Dialogs->Start_Light_Editor();
	}*/
}

// *************************************************************************
// *		Update_ListView_Camera:- Terry and Hazel Flanigan 2024 	 	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Camera()
{
	int Index = App->CL_Properties->Current_Selected_Object;

	//	char Num[10];
	char chr_ID[50];
	//_itoa(App->CL_Scene->B_Camera[Index]->, Num, 10);
	strcpy(chr_ID, "Properties ID=0");
	//strcat(chr_ID, Num);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Camera[Index]->Camera_Name);

	char chr_Pos_X[100];
	char chr_Pos_Y[100];
	char chr_Pos_Z[100];

	char chr_LookAT_X[100];
	char chr_LookAT_Y[100];
	char chr_LookAT_Z[100];

	sprintf(chr_Pos_X, "%.3f ", App->CL_Scene->B_Camera[Index]->CamPos.x);
	sprintf(chr_Pos_Y, "%.3f ", App->CL_Scene->B_Camera[Index]->CamPos.y);
	sprintf(chr_Pos_Z, "%.3f ", App->CL_Scene->B_Camera[Index]->CamPos.z);

	sprintf(chr_LookAT_X, "%.3f ", App->CL_Scene->B_Camera[Index]->LookAt.x);
	sprintf(chr_LookAT_Y, "%.3f ", App->CL_Scene->B_Camera[Index]->LookAt.y);
	sprintf(chr_LookAT_Z, "%.3f ", App->CL_Scene->B_Camera[Index]->LookAt.z);


	const int NUM_ITEMS = 6;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Camera[Index]->Camera_Name;
	grid[0][1] = " ", grid[1][1] = " ";
	grid[0][2] = "Pos_X", grid[1][2] = chr_Pos_X;
	grid[0][3] = "Pos_Y", grid[1][3] = chr_Pos_Y;
	grid[0][4] = "Pos_Z", grid[1][4] = chr_Pos_Z;
	grid[0][5] = " ", grid[1][5] = " ";
	/*grid[0][6] = "LookAt_X", grid[1][6] = chr_LookAT_X;
	grid[0][7] = "LookAt_Y", grid[1][7] = chr_LookAT_Y;
	grid[0][8] = "LookAt_Z", grid[1][8] = chr_LookAT_Z;*/


	ListView_DeleteAllItems(App->CL_Properties->Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(App->CL_Properties->Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(App->CL_Properties->Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *		Update_ListView_Area:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Area()
{
	int Index = Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	_itoa(App->CL_Scene->B_Area[Index]->This_Object_UniqueID, Num, 10);
	strcpy(chr_ID, "Properties ID=");
	strcat(chr_ID, Num);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Area[Index]->Area_Name);

	const int NUM_ITEMS = 3;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Area[Index]->Area_Name;
	grid[0][1] = "Mesh File", grid[1][1] = App->CL_Scene->B_Area[Index]->Area_FileName;
	grid[0][2] = "Materials", grid[1][2] = App->CL_Scene->B_Area[Index]->Material_File;

	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *		Update_ListView_Player:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Player()
{
	int index = Current_Selected_Object;

	//	char Num[10];
	char chr_ID[50];
	//_itoa(App->CL_Scene->B_Object[index]->This_Object_ID, Num, 10);
	strcpy(chr_ID, "Properties ID=0");
	//strcat(chr_ID, Num);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Player[0]->Player_Name);

	char chr_Speed[100];
	char chr_TurnRate[100];
	char chr_Height[100];
	char chr_StartPosX[100];
	char chr_StartPosY[100];
	char chr_StartPosZ[100];

	char chr_LookUp_Limit[100];
	char chr_LookDown_Limit[100];

	sprintf(chr_Speed, "%.3f ", App->CL_Scene->B_Player[0]->Ground_speed / 100);

	sprintf(chr_TurnRate, "%.6f ", App->CL_Scene->B_Player[0]->TurnRate);

	sprintf(chr_Height, "%.3f ", App->CL_Scene->B_Player[0]->PlayerHeight);

	sprintf(chr_StartPosX, "%.3f ", App->CL_Scene->B_Player[0]->StartPos.x);
	sprintf(chr_StartPosY, "%.3f ", App->CL_Scene->B_Player[0]->StartPos.y);
	sprintf(chr_StartPosZ, "%.3f ", App->CL_Scene->B_Player[0]->StartPos.z);

	sprintf(chr_LookUp_Limit, "%.3f ", App->CL_Scene->B_Player[0]->Limit_Look_Up);
	sprintf(chr_LookDown_Limit, "%.3f ", App->CL_Scene->B_Player[0]->Limit_Look_Down);


	const int NUM_ITEMS = 13;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Player[0]->Player_Name;
	grid[0][1] = "Game Mode", grid[1][1] = "1st_Person";
	grid[0][2] = " ", grid[1][2] = " ";
	grid[0][3] = "Ground Speed", grid[1][3] = chr_Speed;
	grid[0][4] = "Turn Rate", grid[1][4] = chr_TurnRate;
	grid[0][5] = "Player Height", grid[1][5] = chr_Height;
	grid[0][6] = " ", grid[1][6] = " ";
	grid[0][7] = "Start Pos_X", grid[1][7] = chr_StartPosX;
	grid[0][8] = "Start Pos_Y", grid[1][8] = chr_StartPosY;
	grid[0][9] = "Start Pos_Z", grid[1][9] = chr_StartPosZ;
	grid[0][10] = " ", grid[1][10] = " ";
	grid[0][11] = "Look Up", grid[1][11] = chr_LookUp_Limit;
	grid[0][12] = "Look Down", grid[1][12] = chr_LookDown_Limit;


	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *	Update_ListView_Player_Physics:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Player_Physics()
{
	if (App->CL_Scene->flag_Scene_Loaded == 0)
	{
		//return 1;
	}

	int index = Current_Selected_Object;

	char buff[255];
	strcpy(buff, App->CL_Scene->B_Player[0]->Player_Name);
	//strcat(buff, "   (Physics)");
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)buff);

	char chr_PhysicsType[100];
	strcpy(chr_PhysicsType, "Dynamic");

	char chr_PhysicsShape[100];
	strcpy(chr_PhysicsShape, "Capsule");

	char chr_Mass[100];
	char chr_Radius[100];
	char chr_Height[100];

	sprintf(chr_Mass, "%.3f ", App->CL_Scene->B_Player[0]->Capsule_Mass);
	//sprintf(chr_Mass,"%.3f ",App->GDSBC_Player->mObject->getGravity().getY());
	sprintf(chr_Radius, "%.3f ", App->CL_Scene->B_Player[0]->Capsule_Radius);
	sprintf(chr_Height, "%.3f ", App->CL_Scene->B_Player[0]->Capsule_Height);


	const int NUM_ITEMS = 6;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Player[0]->Player_Name;
	grid[0][1] = "Type", grid[1][1] = chr_PhysicsType;
	grid[0][2] = "Shape ", grid[1][2] = chr_PhysicsShape;
	grid[0][3] = " ", grid[1][3] = " ";
	grid[0][4] = "Radius", grid[1][4] = chr_Radius;
	grid[0][5] = "Height", grid[1][5] = chr_Height;

	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *		Update_ListView_Objects:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Objects()
{
	int index = Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);


	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Mesh_Name);


	const int NUM_ITEMS = 4;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
	grid[0][1] = "Mesh File", grid[1][1] = App->CL_Scene->B_Object[index]->Mesh_FileName;
	grid[0][2] = "Materials", grid[1][2] = App->CL_Scene->B_Object[index]->Material_File;
	grid[0][3] = " ", grid[1][3] = " ";

	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *	Update_ListView_Collectables():- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Collectables()
{
	int index = Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Mesh_Name);

	// new sound
	char chr_Play[100];
	if (App->CL_Scene->B_Object[index]->S_Collectable[0]->Play == 1)
	{
		strcpy(chr_Play, "True");
	}
	else
	{
		strcpy(chr_Play, "False");
	}

	char chr_Volume[100];
	float sum2 = App->CL_Scene->B_Object[index]->S_Collectable[0]->SndVolume;
	int Percent = int(sum2 * 100);
	_itoa(Percent, chr_Volume, 10);

	char chr_Counter_Disabled[20];
	if (App->CL_Scene->B_Object[index]->S_Collectable[0]->Counter_Disabled == 1)
	{
		strcpy(chr_Counter_Disabled, "Disabled");
	}
	else
	{
		strcpy(chr_Counter_Disabled, "Enabled");
	}

	const int NUM_ITEMS = 9;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
	grid[0][1] = "Mesh File", grid[1][1] = App->CL_Scene->B_Object[index]->Mesh_FileName;
	grid[0][2] = "Materials", grid[1][2] = App->CL_Scene->B_Object[index]->Material_File;
	grid[0][3] = " ", grid[1][3] = " ";
	grid[0][4] = "Sound", grid[1][4] = App->CL_Scene->B_Object[index]->S_Collectable[0]->Sound_File;
	grid[0][5] = "Volume", grid[1][5] = chr_Volume;
	grid[0][6] = "Play", grid[1][6] = chr_Play;
	grid[0][7] = " ", grid[1][7] = " ";
	grid[0][8] = "Counter", grid[1][8] = chr_Counter_Disabled;



	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *			Update_ListView_Sounds:- Terry and Hazel Flanigan 2024     *
// *************************************************************************
bool CL64_Properties::Update_ListView_Sounds()
{
	int index = Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);

	char chr_Usage_Num[50];
	_itoa(App->CL_Scene->B_Object[index]->Usage, chr_Usage_Num, 10);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Mesh_Name);


	char chr_Volume[100];
	float sum2 = App->CL_Scene->B_Object[index]->SndVolume;
	int Percent = int(sum2 * 100);
	_itoa(Percent, chr_Volume, 10);

	const int NUM_ITEMS = 5;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
	grid[0][1] = " ", grid[1][1] = " ";
	grid[0][2] = "Sound", grid[1][2] = App->CL_Scene->B_Object[index]->Sound_File;
	grid[0][3] = "Volume", grid[1][3] = chr_Volume;
	grid[0][4] = "Usage", grid[1][4] = chr_Usage_Num;

	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *	Update_ListView_Move_Entities:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Move_Entities()
{
	int index = App->CL_Properties->Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(App->SBC_Properties->Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Mesh_Name);

	char chr_Distance[100];
	sprintf(chr_Distance, "%.3f ", App->CL_Scene->B_Object[index]->S_MoveType[0]->Move_Distance);

	char chr_Speed[100];
	sprintf(chr_Speed, "%.3f ", App->CL_Scene->B_Object[index]->S_MoveType[0]->Speed);

	char chr_Axis[100] { 0 };
	if (App->CL_Scene->B_Object[index]->S_MoveType[0]->WhatDirection == Enums::Axis_x)
	{
		strcpy(chr_Axis, "X");
	}
	if (App->CL_Scene->B_Object[index]->S_MoveType[0]->WhatDirection == Enums::Axis_y)
	{
		strcpy(chr_Axis, "Y");
	}
	if (App->CL_Scene->B_Object[index]->S_MoveType[0]->WhatDirection == Enums::Axis_z)
	{
		strcpy(chr_Axis, "Z");
	}

	// new sound
	char chr_Play[100];
	if (App->CL_Scene->B_Object[index]->Play_Sound == 1)
	{
		strcpy(chr_Play, "True");
	}
	else
	{
		strcpy(chr_Play, "False");
	}

	char chr_Object_Name[100];
	strcpy(chr_Object_Name, App->CL_Scene->B_Object[App->CL_Scene->B_Object[index]->S_MoveType[0]->Object_To_Move_Index]->Object_Name);

	char chr_Trigger_Value[100];
	_itoa(App->CL_Scene->B_Object[index]->S_MoveType[0]->Trigger_Value, chr_Trigger_Value, 10);

	char chr_Volume[100];
	float sum2 = App->CL_Scene->B_Object[index]->SndVolume;
	int Percent = int(sum2 * 100);
	_itoa(Percent, chr_Volume, 10);

	char chr_Counter_Disabled[20];
	if (App->CL_Scene->B_Object[index]->S_MoveType[0]->Counter_Disabled == 1)
	{
		strcpy(chr_Counter_Disabled, "Disabled");
	}
	else
	{
		strcpy(chr_Counter_Disabled, "Enabled");
	}

	const int NUM_ITEMS = 12;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
	grid[0][1] = " ", grid[1][1] = " ";
	grid[0][2] = "Move_Object", grid[1][2] = chr_Object_Name;
	grid[0][3] = "Axis", grid[1][3] = chr_Axis;
	grid[0][4] = "Distance", grid[1][4] = chr_Distance;
	grid[0][5] = "Speed", grid[1][5] = chr_Speed;
	grid[0][6] = " ", grid[1][6] = " ";
	grid[0][7] = "Sound", grid[1][7] = App->CL_Scene->B_Object[index]->Sound_File;
	grid[0][8] = "Volume", grid[1][8] = chr_Volume;
	grid[0][9] = "Play", grid[1][9] = chr_Play;
	grid[0][10] = " ", grid[1][10] = " ";
	grid[0][11] = "Counter", grid[1][11] = chr_Counter_Disabled;

	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *			Update_ListView_Mesages	Terry Flanigan				 	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Messages()
{
	int index = App->CL_Properties->Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Mesh_Name);

	char chr_PosX[20];
	sprintf(chr_PosX, "%.3f ", App->CL_Scene->B_Object[index]->S_Message[0]->Message_PosX);

	char chr_PosY[20];
	sprintf(chr_PosY, "%.3f ", App->CL_Scene->B_Object[index]->S_Message[0]->Message_PosY);

	char chr_Counter_Disabled[20];
	if (App->CL_Scene->B_Object[index]->S_Message[0]->Counter_Disabled == 1)
	{
		strcpy(chr_Counter_Disabled, "Disabled");
	}
	else
	{
		strcpy(chr_Counter_Disabled, "Enabled");
	}

	const int NUM_ITEMS = 7;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
	grid[0][1] = " ", grid[1][1] = " ";
	grid[0][2] = "Pos_X", grid[1][2] = chr_PosX;
	grid[0][3] = "Pos_Y", grid[1][3] = chr_PosY;
	grid[0][4] = "Text", grid[1][4] = App->CL_Scene->B_Object[index]->S_Message[0]->Message_Text;
	grid[0][5] = " ", grid[1][5] = " ";
	grid[0][6] = "Counter", grid[1][6] = chr_Counter_Disabled;


	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// **************************************************************************
// *		Update_ListView_Teleport():- Terry and Hazel Flanigan 2024		*
// **************************************************************************
bool CL64_Properties::Update_ListView_Teleport()
{
	int index = App->CL_Properties->Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	strcpy(chr_ID, "Properties ID=");
	strcat(chr_ID, Num);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Mesh_Name);

	// new sound
	char chr_Play[100];
	if (App->CL_Scene->B_Object[index]->S_Teleport[0]->Play == 1)
	{
		strcpy(chr_Play, "True");
	}
	else
	{
		strcpy(chr_Play, "False");
	}


	char chr_Goto_Location[100];
	int Goto_ID = App->CL_Scene->B_Object[index]->S_Teleport[0]->Location_ID;
	strcpy(chr_Goto_Location, App->CL_Scene->B_Locations[Goto_ID]->Name);

	char chr_Volume[100];
	float sum2 = App->CL_Scene->B_Object[index]->S_Teleport[0]->SndVolume;
	int Percent = int(sum2 * 100);
	_itoa(Percent, chr_Volume, 10);


	char chr_Counter_Disabled[20];
	if (App->CL_Scene->B_Object[index]->S_Teleport[0]->Counter_Disabled == 1)
	{
		strcpy(chr_Counter_Disabled, "Disabled");
	}
	else
	{
		strcpy(chr_Counter_Disabled, "Enabled");
	}


	// Environ
	char chr_Environ_Disabled[100];
	if (App->CL_Scene->B_Object[index]->S_Environ[0]->Environ_Enabled == 1)
	{
		strcpy(chr_Environ_Disabled, "Enabled");
	}
	else
	{
		strcpy(chr_Environ_Disabled, "Disabled");
	}

	// Environ
	char Chr_Counter_Index[100];
	_itoa(App->CL_Scene->B_Object[index]->S_Teleport[0]->Counter_ID, Chr_Counter_Index,10);


	if (App->CL_Scene->B_Object[index]->S_Environ[0]->Environ_Enabled == 1)
	{
		const int NUM_ITEMS = 12;
		const int NUM_COLS = 2;
		std::string grid[NUM_COLS][NUM_ITEMS]; // string table

		LV_ITEM pitem;
		memset(&pitem, 0, sizeof(LV_ITEM));
		pitem.mask = LVIF_TEXT;

		grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
		grid[0][1] = " ", grid[1][1] = " ";
		grid[0][2] = "Goto", grid[1][2] = chr_Goto_Location;
		grid[0][3] = "Sound", grid[1][3] = App->CL_Scene->B_Object[index]->S_Teleport[0]->Sound_File;
		grid[0][4] = "Volume", grid[1][4] = chr_Volume;
		grid[0][5] = "Play", grid[1][5] = chr_Play;
		grid[0][6] = " ", grid[1][6] = " ";
		grid[0][7] = "Counter", grid[1][7] = chr_Counter_Disabled;
		grid[0][8] = "Count_Name", grid[1][8] = App->CL_Scene->B_Object[index]->S_Teleport[0]->Counter_Name;
		grid[0][9] = "Count_Index", grid[1][9] = Chr_Counter_Index;
		grid[0][10] = " ", grid[1][10] = " ";
		grid[0][11] = "Environment", grid[1][11] = chr_Environ_Disabled;

		ListView_DeleteAllItems(Properties_hLV);

		for (DWORD row = 0; row < NUM_ITEMS; row++)
		{
			pitem.iItem = row;
			pitem.pszText = const_cast<char*>(grid[0][row].c_str());
			ListView_InsertItem(Properties_hLV, &pitem);

			for (DWORD col = 1; col < NUM_COLS; col++)
			{
				ListView_SetItemText(Properties_hLV, row, col,
					const_cast<char*>(grid[col][row].c_str()));
			}
		}
	}
	else
	{
		const int NUM_ITEMS = 9;
		const int NUM_COLS = 2;
		std::string grid[NUM_COLS][NUM_ITEMS]; // string table

		LV_ITEM pitem;
		memset(&pitem, 0, sizeof(LV_ITEM));
		pitem.mask = LVIF_TEXT;

		grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
		grid[0][1] = " ", grid[1][1] = " ";
		grid[0][2] = "Goto", grid[1][2] = chr_Goto_Location;
		grid[0][3] = "Sound", grid[1][3] = App->CL_Scene->B_Object[index]->S_Teleport[0]->Sound_File;
		grid[0][4] = "Volume", grid[1][4] = chr_Volume;
		grid[0][5] = "Play", grid[1][5] = chr_Play;
		grid[0][6] = " ", grid[1][6] = " ";
		grid[0][7] = "Counter", grid[1][7] = chr_Counter_Disabled;
		grid[0][8] = "Environment", grid[1][8] = chr_Environ_Disabled;

		ListView_DeleteAllItems(Properties_hLV);

		for (DWORD row = 0; row < NUM_ITEMS; row++)
		{
			pitem.iItem = row;
			pitem.pszText = const_cast<char*>(grid[0][row].c_str());
			ListView_InsertItem(Properties_hLV, &pitem);
			for (DWORD col = 1; col < NUM_COLS; col++)
			{
				ListView_SetItemText(Properties_hLV, row, col,
					const_cast<char*>(grid[col][row].c_str()));
			}
		}
	}

	return 1;
}

// *************************************************************************
// *		Update_ListView_Particles:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Particles()
{
	int index = App->CL_Properties->Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);
	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Mesh_Name);

	char chr_Speed[100];

	sprintf(chr_Speed, "%.3f", App->CL_Scene->B_Object[index]->S_Particle[0]->Particle->getSpeedFactor());

	const int NUM_ITEMS = 4;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
	grid[0][1] = "Particle", grid[1][1] = App->CL_Scene->B_Object[index]->S_Particle[0]->ParticleScript;
	grid[0][2] = " ", grid[1][2] = " ";
	grid[0][3] = "Speed", grid[1][3] = chr_Speed;

	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *		Update_ListView_Counters():- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Counters()
{
	int index = Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	_itoa(App->CL_Scene->B_Counter[index]->Unique_ID, Num, 10);
	strcpy(chr_ID, "Properties ID=");
	strcat(chr_ID, Num);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Counter[index]->Panel_Name);

	char chr_PosX[20];
	sprintf(chr_PosX, "%.3f ", App->CL_Scene->B_Counter[index]->PosX);

	char chr_PosY[20];
	sprintf(chr_PosY, "%.3f ", App->CL_Scene->B_Counter[index]->PosY);

	char chr_Counter[20];
	_itoa(App->CL_Scene->B_Counter[index]->Counter, chr_Counter, 10);

	char chr_Display[20];
	if (App->CL_Scene->B_Counter[index]->Show_Panel_Flag == 1)
	{
		strcpy(chr_Display, "Always");
	}
	else
	{
		strcpy(chr_Display, "Auto");
	}


	const int NUM_ITEMS = 7;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Counter[index]->Panel_Name;
	grid[0][1] = " ", grid[1][1] = " ";
	grid[0][2] = "Pos_X", grid[1][2] = chr_PosX;
	grid[0][3] = "Pos_Y", grid[1][3] = chr_PosY;
	grid[0][4] = "Text", grid[1][4] = App->CL_Scene->B_Counter[index]->Text;
	grid[0][5] = "Start_Value", grid[1][5] = chr_Counter;
	grid[0][6] = "Display", grid[1][6] = chr_Display;


	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		//ListView_SetItemText

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *		Update_ListView_Environs:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Environs()
{
	int index = Current_Selected_Object;

	char Num[10];
	char IndexNum[10];
	char chr_ID[50];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Mesh_Name);

	const int NUM_ITEMS = 3;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS];
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
	grid[0][1] = " ", grid[1][1] = " ";
	grid[0][2] = "Evironment", grid[1][2] = "Settings";

	ListView_DeleteAllItems(Properties_hLV);

	for (DWORD row = 0; row < NUM_ITEMS; row++)
	{
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		for (DWORD col = 1; col < NUM_COLS; col++)
		{
			ListView_SetItemText(Properties_hLV, row, col,
				const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return 1;
}

// *************************************************************************
// *			Mark_As_Altered:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Properties::Mark_As_Altered(int Index)
{
	App->CL_Scene->B_Object[Index]->Altered = 1;
	App->CL_Scene->flag_Scene_Modified = 1;
	App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
}
