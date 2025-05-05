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
#include "CL64_Properties_Scene.h"

CL64_Properties_Scene::CL64_Properties_Scene()
{
	Properties_Dlg_hWnd = nullptr;
	Properties_hLV = nullptr;

	flag_Properties_Dlg_Active = 0;
	flag_Edit_Physics = 0;

	Edit_Category = Enums::Edit_Object;
	Current_Selected_Object = 0;

	btext[0] = 0;
}

CL64_Properties_Scene::~CL64_Properties_Scene()
{
}

// *************************************************************************
// *		Start_Properties:- Terry and Hazel Flanigan 2024    	 	   *
// *************************************************************************
void CL64_Properties_Scene::Start_Properties(void)
{
	if (flag_Properties_Dlg_Active == 1)
	{
		return;
	}

	flag_Properties_Dlg_Active = 1;

	Properties_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPERTIES_SCENE, App->MainHwnd, (DLGPROC)Proc_Properties);
	Show_Properties_Scene(false);

	//HMENU mMenu = GetMenu(App->MainHwnd);
	//CheckMenuItem(mMenu, ID_WINDOWS_PROPERTIES, MF_BYCOMMAND | MF_CHECKED);

	Create_Properties_hLV();

}
// *************************************************************************
// *		Proc_Properties_Proc:- Terry and Hazel Flanigan 2024  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Scene::Proc_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STOBJECTNAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		//App->SBC_Properties->Edit_Category = Enums::FV_Edit_Object;*/

		return 1;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STOBJECTNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CLOSE:
	{
		App->CL_Properties_Scene->flag_Properties_Dlg_Active = 0;

		HMENU mMenu = GetMenu(App->MainHwnd);
		//CheckMenuItem(mMenu, ID_WINDOWS_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);

		ShowWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, 0);

		break;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->hwndFrom == App->CL_Properties_Scene->Properties_hLV)
		{
			switch (some_item->code)
			{
			case NM_CLICK:
			{
				App->CL_Properties_Scene->ListView_OnClickOptions(lParam);
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
void CL64_Properties_Scene::Create_Properties_hLV(void)
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

// **************************************************************************
// *		Show_Properties_Scene:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Properties_Scene::Show_Properties_Scene(bool Enable)
{
	if (Enable == 1)
	{
		flag_Properties_Dlg_Active = 1;
		ShowWindow(Properties_Dlg_hWnd, 1);
	}
	else
	{
		flag_Properties_Dlg_Active = 0;
		ShowWindow(Properties_Dlg_hWnd, 0);
	}
}

// *************************************************************************
// *		ListView_OnClickOptions:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Properties_Scene::ListView_OnClickOptions(LPARAM lParam)
{
	// Area
	/*if (Edit_Category == Enums::Edit_Area)
	{
		Edit_Area_Onclick(lParam);
		return;
	}*/

	// Camera
	/*if (Edit_Category == Enums::Edit_Camera)
	{
		Edit_Camera_Onclick(lParam);
		return;
	}*/

	// Player
	if (Edit_Category == Enums::Edit_Player)
	{
		if (flag_Edit_Physics == 0)
		{
			Edit_Player(lParam);
		}
		else
		{
			//Edit_Player_Physics(lParam);
		}

		return;
	}

	// Objects
	if (Edit_Category == Enums::Edit_Object)
	{
		//Edit_Object(lParam);
		return;
	}

	// Messages
	if (Edit_Category == Enums::Edit_Message)
	{
		//Edit_Messages(lParam);
		return;
	}

	// Move Entity
	if (Edit_Category == Enums::Edit_Move_Entity)
	{
		//Edit_Move_Entity(lParam);
		return;
	}

	// Sounds
	if (Edit_Category == Enums::Edit_Sounds)
	{
		//Edit_Sounds(lParam);
		return;
	}

	// Teleports
	if (Edit_Category == Enums::Edit_Teleport)
	{
		//Edit_Teleport_Entity(lParam);
		return;
	}

	// Collectables
	if (Edit_Category == Enums::Edit_Collectable)
	{
		//Edit_Collectables(lParam);
		return;
	}

	// Counters
	if (Edit_Category == Enums::Edit_Counters)
	{
		//Edit_Counters_OnClick(lParam);
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
		//Edit_Particle(lParam);
		return;
	}

	// Lights
	if (Edit_Category == Enums::Edit_Lights)
	{
		//Edit_Light_Onclick(lParam);
		return;
	}

	// UserObjects
	if (Edit_Category == Enums::Edit_UserObjects)
	{
		if (flag_Edit_Physics == 0)
		{
			//Edit_UserObjects_Onclick(lParam);
		}

		return;
	}

	return;
}

// *************************************************************************
// *		Update_ListView_Player:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
bool CL64_Properties_Scene::Update_ListView_Player() 
{
	int index = Current_Selected_Object;

	// Update the properties dialog title
	std::string title = "Properties ID=0";
	SetWindowText(Properties_Dlg_hWnd, title.c_str());
	SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, App->CL_Editor_Com->B_Player[0]->Player_Name);

	// Prepare player properties
	std::string str_Speed = std::to_string(App->CL_Editor_Com->B_Player[0]->Ground_speed / 100.0f);
	std::string str_TurnRate = std::to_string(App->CL_Editor_Com->B_Player[0]->TurnRate);
	std::string str_Height = std::to_string(App->CL_Editor_Com->B_Player[0]->PlayerHeight);
	std::string str_StartPosX = std::to_string(App->CL_Editor_Com->B_Player[0]->StartPos.x);
	std::string str_StartPosY = std::to_string(App->CL_Editor_Com->B_Player[0]->StartPos.y);
	std::string str_StartPosZ = std::to_string(App->CL_Editor_Com->B_Player[0]->StartPos.z);
	std::string str_LookUp_Limit = std::to_string(App->CL_Editor_Com->B_Player[0]->Limit_Look_Up);
	std::string str_LookDown_Limit = std::to_string(App->CL_Editor_Com->B_Player[0]->Limit_Look_Down);

	const int NUM_ITEMS = 13;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table

	// Populate the grid with player properties
	grid[0][0] = "Name";			grid[1][0] = App->CL_Editor_Com->B_Player[0]->Player_Name;
	grid[0][1] = "Game Mode";		grid[1][1] = "1st_Person";
	grid[0][2] = " ";				grid[1][2] = " ";
	grid[0][3] = "Ground Speed";	grid[1][3] = str_Speed;
	grid[0][4] = "Turn Rate";		grid[1][4] = str_TurnRate;
	grid[0][5] = "Player Height";	grid[1][5] = str_Height;
	grid[0][6] = " ";				grid[1][6] = " ";
	grid[0][7] = "Start Pos_X";		grid[1][7] = str_StartPosX;
	grid[0][8] = "Start Pos_Y";		grid[1][8] = str_StartPosY;
	grid[0][9] = "Start Pos_Z";		grid[1][9] = str_StartPosZ;
	grid[0][10] = " ";				grid[1][10] = " ";
	grid[0][11] = "Look Up";		grid[1][11] = str_LookUp_Limit;
	grid[0][12] = "Look Down";		grid[1][12] = str_LookDown_Limit;

	// Clear existing items in the ListView
	ListView_DeleteAllItems(Properties_hLV);

	// Insert new items into the ListView
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	for (DWORD row = 0; row < NUM_ITEMS; row++) {
		pitem.iItem = row;
		pitem.pszText = const_cast<char*>(grid[0][row].c_str());
		ListView_InsertItem(Properties_hLV, &pitem);

		for (DWORD col = 1; col < NUM_COLS; col++) {
			ListView_SetItemText(Properties_hLV, row, col, const_cast<char*>(grid[col][row].c_str()));
		}
	}

	return true;
}

// *************************************************************************
// *		Update_ListView_Environs:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
bool CL64_Properties_Scene::Update_ListView_Environs()
{
	int index = Current_Selected_Object;

	// Update the properties dialog title
	std::string str_uniqueID = std::to_string(App->CL_Editor_Com->B_Object[index]->This_Object_UniqueID);
	std::string str_index = std::to_string(index);
	std::string str_chr_ID = "Unique ID " + str_uniqueID + "  Object Index " + str_index;

	SetWindowText(Properties_Dlg_hWnd, str_chr_ID.c_str());
	SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, App->CL_Editor_Com->B_Object[index]->Object_Name);

	const int NUM_ITEMS = 9;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS];
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name",		grid[1][0] = App->CL_Editor_Com->B_Object[index]->Object_Name;
	grid[0][1] = "Evironment",	grid[1][1] = "Settings";
	grid[0][2] = " ",			grid[1][2] = " ";
	grid[0][3] = " ",			grid[1][3] = " ";
	grid[0][4] = " ",			grid[1][4] = " ";
	grid[0][5] = " ",			grid[1][5] = " ";
	grid[0][6] = " ",			grid[1][6] = " ";
	grid[0][7] = "Position",	grid[1][7] = "Click";
	grid[0][8] = "Scale",		grid[1][8] = "Click";

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

	return true;
}

// *************************************************************************
// *			Edit_Player:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_Properties_Scene::Edit_Player(LPARAM lParam)
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
		App->CL_Com_Player->Rename_Player(Index);
		Update_ListView_Player();
	}

	result = strcmp(btext, "Ground Speed");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.01, 1, App->CL_Editor_Com->B_Player[0]->Ground_speed / 100, (LPSTR)"Ground Speed");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			App->CL_Editor_Com->B_Player[0]->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float * 100;
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			App->CL_Editor_Com->B_Player[0]->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float * 100;

			App->CL_Editor_Com->B_Object[Index]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy * 100;
			App->CL_Editor_Com->B_Player[0]->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy * 100;

		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Turn Rate");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.01, 1, App->CL_Editor_Com->B_Player[0]->TurnRate, (LPSTR)"Turn Rate");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			App->CL_Editor_Com->B_Player[0]->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float;
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			App->CL_Editor_Com->B_Player[0]->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Editor_Com->B_Object[Index]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Editor_Com->B_Player[0]->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;

		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Player Height");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.1, 2, App->CL_Editor_Com->B_Player[0]->PlayerHeight, (LPSTR)"Player Height");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			App->CL_Editor_Com->B_Player[0]->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float;
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			App->CL_Editor_Com->B_Player[0]->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Editor_Com->B_Object[Index]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Editor_Com->B_Player[0]->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;

		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();
		return 1;
	}

	result = strcmp(btext, "Start Pos_X");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, App->CL_Editor_Com->B_Player[0]->StartPos.x, (LPSTR)"Start Pos_X");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Editor_Com->B_Player[0]->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			App->CL_Editor_Com->B_Player[0]->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Brush_X->Move_Player_Brush();
			
			App->CL_Editor_Com->B_Player[0]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Player[0]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Editor_Com->B_Player[0]->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Physics->Reset_Physics();
		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Start Pos_Y");
	if (result == 0)
	{
		//App->CL_Player->Show_Player_And_Physics(true);
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, App->CL_Editor_Com->B_Player[0]->StartPos.y, (LPSTR)"Start Pos_Y");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Editor_Com->B_Player[0]->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			App->CL_Editor_Com->B_Player[0]->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Brush_X->Move_Player_Brush();

			App->CL_Editor_Com->B_Player[0]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Player[0]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Editor_Com->B_Player[0]->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Physics->Reset_Physics();
		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Start Pos_Z");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, App->CL_Editor_Com->B_Player[0]->StartPos.z, (LPSTR)"Start Pos_Z");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Editor_Com->B_Player[0]->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			App->CL_Editor_Com->B_Player[0]->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Brush_X->Move_Player_Brush();

			App->CL_Editor_Com->B_Player[0]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Player[0]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Editor_Com->B_Player[0]->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Physics->Reset_Physics();
		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	//result = strcmp(App->SBC_Properties->btext, "Look Up");
	//if (result == 0)
	//{
	//	App->SBC_Gui_Dialogs->Start_Dialog_Float(0.5, App->CL_Editor_Com->B_Player[0]->Limit_Look_Up, "Player Look Up Limit");

	//	while (App->SBC_Gui_Dialogs->Show_Dialog_Float == 1)
	//	{
	//		App->SBC_Gui_Dialogs->BackGround_Render_Loop();
	//	}

	//	App->SBC_Gui_Dialogs->Show_Dialog_Float = 0;

	//	if (App->SBC_Gui_Dialogs->Float_Canceld == 0)
	//	{
	//		App->CL_Editor_Com->B_Player[0]->Limit_Look_Up = App->SBC_Gui_Dialogs->m_Dialog_Float;

	//		App->CL_Editor_Com->B_Player[0]->Altered = 1;
	//		App->CL_Editor_Com->Scene_Modified = 1;
	//		App->SBC_FileView->Mark_Altered(App->CL_Editor_Com->B_Player[0]->FileViewItem);
	//	}

	//	App->Disable_Panels(false);

	//	Update_ListView_Player();

	//	return 1;
	//}

	//result = strcmp(App->SBC_Properties->btext, "Look Down");
	//if (result == 0)
	//{

	//	App->SBC_Gui_Dialogs->Start_Dialog_Float(0.5, App->CL_Editor_Com->B_Player[0]->Limit_Look_Down, "Player Look Down Limit");

	//	while (App->SBC_Gui_Dialogs->Show_Dialog_Float == 1)
	//	{
	//		App->SBC_Gui_Dialogs->BackGround_Render_Loop();
	//	}

	//	App->SBC_Gui_Dialogs->Show_Dialog_Float = 0;

	//	if (App->SBC_Gui_Dialogs->Float_Canceld == 0)
	//	{
	//		App->CL_Editor_Com->B_Player[0]->Limit_Look_Down = App->SBC_Gui_Dialogs->m_Dialog_Float;

	//		App->CL_Editor_Com->B_Player[0]->Altered = 1;
	//		App->CL_Editor_Com->Scene_Modified = 1;
	//		App->SBC_FileView->Mark_Altered(App->CL_Editor_Com->B_Player[0]->FileViewItem);
	//	}

	//	App->Disable_Panels(false);

	//	Update_ListView_Player();

	//	return 1;
	//}


	return 1;
}

// *************************************************************************
// *		Edit_Environs_OnClick:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Properties_Scene::Edit_Environs_OnClick(LPARAM lParam)
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

	result = strcmp(btext, "Position");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float_Vec3(1.00, 4, App->CL_Editor_Com->B_Object[Index]->Object_Node->getPosition(), (LPSTR)"Position");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Editor_Com->B_Object[Index]->Object_Node->setPosition(App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3);
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

			App->CL_Editor_Com->B_Object[Index]->Object_Node->setPosition(App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3);
			App->CL_Editor_Com->B_Object[Index]->Mesh_Pos = App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3;

			App->CL_Physics->Set_Physics_New(Index);
			App->CL_Brush_X->Move_Brush_By_Name(App->CL_Editor_Com->B_Object[Index]->Object_Name,Index);

			App->CL_Gizmos->MarkerBox_Addjust(Index);

			App->CL_Editor_Com->B_Object[Index]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3 = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3;
			App->CL_Editor_Com->B_Object[Index]->Object_Node->setPosition(App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3);
			App->CL_Physics->Reset_Physics();
		}

		App->CL_Panels->Enable_Scene_Editor_Dialogs(true);

		Update_ListView_Environs();
		
		return 1;
	}

	result = strcmp(btext, "Scale");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float_Vec3(0.10, 2, App->CL_Editor_Com->B_Object[Index]->Object_Node->getScale(), (LPSTR)"Scale");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			App->CL_Editor_Com->B_Object[Index]->Object_Node->setScale(App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3);
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

			App->CL_Editor_Com->B_Object[Index]->Object_Node->setScale(App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3);
			App->CL_Editor_Com->B_Object[Index]->Mesh_Scale = App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3;

			App->CL_Physics->Set_Physics_New(Index);

			float sizeX = App->CL_Editor_Com->B_Object[Index]->Object_Node->_getWorldAABB().getSize().x;
			float sizeY = App->CL_Editor_Com->B_Object[Index]->Object_Node->_getWorldAABB().getSize().y;
			float sizeZ = App->CL_Editor_Com->B_Object[Index]->Object_Node->_getWorldAABB().getSize().z;

			App->CL_Brush_X->Scale_Brush_By_Name(App->CL_Editor_Com->B_Object[Index]->Object_Name, Index, sizeX, sizeY, sizeZ);
			
			App->CL_Gizmos->MarkerBox_Addjust(Index);

			App->CL_Editor_Com->B_Object[Index]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3 = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3;
			App->CL_Editor_Com->B_Object[Index]->Object_Node->setScale(App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3);
			App->CL_Physics->Reset_Physics();
		}

		App->CL_Panels->Enable_Scene_Editor_Dialogs(true);

		Update_ListView_Environs();

		return 1;
	}
	
	return 1;
}
