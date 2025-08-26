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
	Last_Selected_Object = 0;
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
	switch (Edit_Category)
	{
	case Enums::Edit_Area:
		// Edit_Area_Onclick(lParam);
		return;

	case Enums::Edit_Camera:
		// Edit_Camera_Onclick(lParam);
		return;

	case Enums::Edit_Player:
		if (flag_Edit_Physics == 0)
		{
			Edit_Player(lParam);
		}
		else
		{
			// Edit_Player_Physics(lParam);
		}
		return;

	case Enums::Edit_Object:
		Edit_Object(lParam);
		return;

	case Enums::Edit_Message:
		// Edit_Messages(lParam);
		return;

	case Enums::Edit_Move_Entity:
		// Edit_Move_Entity(lParam);
		return;

	case Enums::Edit_Sounds:
		 Edit_Sounds(lParam);
		return;

	case Enums::Edit_Teleport:
		// Edit_Teleport_Entity(lParam);
		return;

	case Enums::Edit_Collectable:
		// Edit_Collectables(lParam);
		return;

	case Enums::Edit_Counters:
		// Edit_Counters_OnClick(lParam);
		return;

	case Enums::Edit_Environs:
		Edit_Environs_OnClick(lParam);
		return;

	case Enums::Edit_Particles:
		// Edit_Particle(lParam);
		return;

	case Enums::Edit_Lights:
		// Edit_Light_Onclick(lParam);
		return;

	case Enums::Edit_UserObjects:
		if (flag_Edit_Physics == 0)
		{
			// Edit_UserObjects_Onclick(lParam);
		}
		return;

	case Enums::Edit_Locations:
		Edit_Locations(lParam);
		return;

	default:
		return;
	}
}

// *************************************************************************
// *		Update_ListView_Objects:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
bool CL64_Properties_Scene::Update_ListView_Objects()
{
	int index = Current_Selected_Object;

	Base_Object* m_Object = App->CL_Scene->B_Object[index];

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(m_Object->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);


	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)m_Object->Object_Name);


	const int NUM_ITEMS = 4;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name",		grid[1][0] = m_Object->Object_Name;
	grid[0][1] = "Mesh File",	grid[1][1] = m_Object->Mesh_FileName;
	grid[0][2] = "Materials",	grid[1][2] = m_Object->Material_File;
	grid[0][3] = " ",			grid[1][3] = " ";

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
// *		Update_ListView_Player:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
bool CL64_Properties_Scene::Update_ListView_Player() 
{
	int index = Current_Selected_Object;
	auto& m_Player = App->CL_Scene->B_Player[0];

	// Update the properties dialog title
	std::string title = "Properties ID=0";
	SetWindowText(Properties_Dlg_hWnd, title.c_str());
	SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, m_Player->Player_Name);

	// Prepare player properties
	std::string str_Speed = std::to_string(m_Player->Ground_speed / 100.0f);
	std::string str_TurnRate = std::to_string(m_Player->TurnRate);
	std::string str_Height = std::to_string(m_Player->PlayerHeight);
	std::string str_StartPosX = std::to_string(m_Player->StartPos.x);
	std::string str_StartPosY = std::to_string(m_Player->StartPos.y);
	std::string str_StartPosZ = std::to_string(m_Player->StartPos.z);
	std::string str_LookUp_Limit = std::to_string(m_Player->Limit_Look_Up);
	std::string str_LookDown_Limit = std::to_string(m_Player->Limit_Look_Down);

	const int NUM_ITEMS = 13;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table

	// Populate the grid with player properties
	grid[0][0] = "Name";			grid[1][0] = m_Player->Player_Name;
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
	std::string str_uniqueID = std::to_string(App->CL_Scene->B_Object[index]->This_Object_UniqueID);
	std::string str_index = std::to_string(index);
	std::string str_chr_ID = "Unique ID " + str_uniqueID + "  Object Index " + str_index;

	SetWindowText(Properties_Dlg_hWnd, str_chr_ID.c_str());
	SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, App->CL_Scene->B_Object[index]->Object_Name);

	const int NUM_ITEMS = 7;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS];
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name",		grid[1][0] = App->CL_Scene->B_Object[index]->Object_Name;
	grid[0][1] = " ",			grid[1][1] = " ";
	grid[0][2] = "Evironment",	grid[1][2] = "Settings";
	grid[0][3] = " ",			grid[1][3] = " ";
	grid[0][4] = " ",			grid[1][4] = " ";
	grid[0][5] = " ",			grid[1][5] = " ";
	grid[0][6] = " ",			grid[1][6] = " ";
	
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
// *			Update_ListView_Sounds:- Terry and Hazel Flanigan 2024     *
// *************************************************************************
bool CL64_Properties_Scene::Update_ListView_Sounds()
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
	SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Object_Name);


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
// *	Update_ListView_Locations:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Properties_Scene::Update_ListView_Locations()
{
	int index = Current_Selected_Object;
	auto& m_Loc = App->CL_Locations->B_Location[index];

	SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)m_Loc->Location_Name);

	// Convert physics position to strings
	std::string str_StartPosX = std::to_string(m_Loc->Physics_Pos.x);
	std::string str_StartPosY = std::to_string(m_Loc->Physics_Pos.y);
	std::string str_StartPosZ = std::to_string(m_Loc->Physics_Pos.z);

	// Convert quaternion to strings needs converting to angles

	Ogre::Quaternion q = { m_Loc->Physics_Quat.getW(), m_Loc->Physics_Quat.getX(), m_Loc->Physics_Quat.getY(), m_Loc->Physics_Quat.getZ() };
	//q = { 0.707, 0.0, 0.0, 0.707 };
	
	double x, y, z;
	x = 0;
	y = 0;
	z = 0;

	App->CL_X_Maths->Ogre_QuaternionToEuler(q, z, y, z);

	std::string str_RotX = std::to_string(x * 180.0 / M_PI);

	char str_RotZ[10];
	char str_RotY[10];
	sprintf(str_RotY, "%.02f", y * 180.0 / M_PI);
	sprintf(str_RotZ, "%.02f", z * 180.0 / M_PI);

	const int NUM_ITEMS = 11;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name",		grid[1][0] = m_Loc->Location_Name;
	grid[0][1] = " ";			grid[1][1] = " ";
	grid[0][2] = "Start Pos_X";	grid[1][2] = str_StartPosX;
	grid[0][3] = "Start Pos_Y";	grid[1][3] = str_StartPosY;
	grid[0][4] = "Start Pos_Z";	grid[1][4] = str_StartPosZ;
	grid[0][5] = " ";			grid[1][5] = " ";
	grid[0][6] = "Rot_X";		grid[1][6] = str_RotX;
	grid[0][7] = "Rot_Y";		grid[1][7] = str_RotY;
	grid[0][8] = "Rot_Z";		grid[1][8] = str_RotZ;
	grid[0][9] = " ";			grid[1][9] = " ";
	grid[0][10] = "Goto";		grid[1][10] = " ";

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
// *		Update_ListView_Teleport():- Terry and Hazel Flanigan 2025		*
// **************************************************************************
void CL64_Properties_Scene::Update_ListView_Teleport()
{
	int index = Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	_itoa(App->CL_Scene->B_Object[index]->This_Object_UniqueID, Num, 10);
	strcpy(chr_ID, "Properties ID=");
	strcat(chr_ID, Num);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->CL_Scene->B_Object[index]->Object_Name);

	// new sound
	char chr_Play[100];
	if (App->CL_Scene->B_Object[index]->S_Teleport[0]->flag_Play == 1)
	{
		strcpy(chr_Play, "True");
	}
	else
	{
		strcpy(chr_Play, "False");
	}


	char chr_Goto_Location[100];
	int Goto_ID = App->CL_Scene->B_Object[index]->S_Teleport[0]->Location_ID;
	strcpy(chr_Goto_Location, App->CL_Locations->B_Location[Goto_ID]->Location_Name);

	char chr_Volume[100];
	float sum2 = App->CL_Scene->B_Object[index]->S_Teleport[0]->SndVolume;
	int Percent = int(sum2 * 100);
	_itoa(Percent, chr_Volume, 10);


	char chr_Counter_Disabled[20];
	if (App->CL_Scene->B_Object[index]->S_Teleport[0]->flag_Counter_Disabled == 1)
	{
		strcpy(chr_Counter_Disabled, "Disabled");
	}
	else
	{
		strcpy(chr_Counter_Disabled, "Enabled");
	}


	// Environ
	char chr_Environ_Disabled[100];
	if (App->CL_Scene->B_Object[index]->S_Environ[0]->flag_Environ_Enabled == 1)
	{
		strcpy(chr_Environ_Disabled, "Enabled");
	}
	else
	{
		strcpy(chr_Environ_Disabled, "Disabled");
	}

	// Environ
	char Chr_Counter_Index[100];
	_itoa(App->CL_Scene->B_Object[index]->S_Teleport[0]->Counter_ID, Chr_Counter_Index, 10);


	if (App->CL_Scene->B_Object[index]->S_Environ[0]->flag_Environ_Enabled == true)
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
}

// *************************************************************************
// *			Edit_Object:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Properties_Scene::Edit_Object(LPARAM lParam)
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
		App->CL_Entities->Rename_Object(Index);
		Update_ListView_Objects();
	}

	/*result = strcmp(btext, "Materials");
	if (result == 0)
	{
		App->SBC_Materials->Start_Material_Editor();
	}*/
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

	auto& m_Player = App->CL_Scene->B_Player[0]; // Pointer to Current Player

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Com_Player->Rename_Player(Index);
		Update_ListView_Player();
	}

	result = strcmp(btext, "Ground Speed");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.01, 1, m_Player->Ground_speed / 100, (LPSTR)"Ground Speed");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			m_Player->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float * 100;
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			m_Player->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float * 100;

			App->CL_Scene->B_Object[Index]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy * 100;
			m_Player->Ground_speed = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy * 100;

		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Turn Rate");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.01, 1, App->CL_Scene->B_Player[0]->TurnRate, (LPSTR)"Turn Rate");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			m_Player->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float;
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			m_Player->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Scene->B_Object[Index]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			m_Player->TurnRate = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;

		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Player Height");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.1, 2, m_Player->PlayerHeight, (LPSTR)"Player Height");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			if (App->CL_ImGui_Dialogs->m_Dialog_Float < 0)
			{
				App->CL_ImGui_Dialogs->m_Dialog_Float = 0;
			}

			m_Player->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float;
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			m_Player->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Scene->B_Object[Index]->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			m_Player->PlayerHeight = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();
		return 1;
	}

	result = strcmp(btext, "Start Pos_X");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, m_Player->StartPos.x, (LPSTR)"Start Pos_X");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			m_Player->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			m_Player->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Brush_X->Move_Player_Brush();
			
			m_Player->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(App->CL_Scene->B_Player[0]->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			m_Player->StartPos.x = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
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
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, m_Player->StartPos.y, (LPSTR)"Start Pos_Y");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			m_Player->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			m_Player->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Brush_X->Move_Player_Brush();

			m_Player->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(m_Player->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			m_Player->StartPos.y = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Physics->Reset_Physics();
		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Start Pos_Z");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.50, 3, m_Player->StartPos.z, (LPSTR)"Start Pos_Z");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();

			m_Player->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float;
			App->CL_Physics->Reset_Physics();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
		{
			App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

			m_Player->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float;

			App->CL_Brush_X->Move_Player_Brush();

			m_Player->flag_Altered = 1;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(m_Player->FileViewItem);
		}
		else
		{
			App->CL_ImGui_Dialogs->m_Dialog_Float = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			m_Player->StartPos.z = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy;
			App->CL_Physics->Reset_Physics();
		}

		//App->CL_Panels->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

	result = strcmp(btext, "Look Up");
	if (result == 0)
	{
		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.5, 3, m_Player->Limit_Look_Up, (LPSTR)"Player Look Up Limit");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == 1)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = 0;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == false)
		{
			m_Player->Limit_Look_Up = App->CL_ImGui_Dialogs->m_Dialog_Float;

			m_Player->flag_Altered = true;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(m_Player->FileViewItem);
		}

		Update_ListView_Player();

	//	App->Disable_Panels(false);

		return 1;
	}

	result = strcmp(btext, "Look Down");
	if (result == 0)
	{

		App->CL_ImGui_Dialogs->Start_Dialog_Float(0.5, 0.3, m_Player->Limit_Look_Down, (LPSTR)"Player Look Down Limit");

		while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float == true)
		{
			App->CL_ImGui_Dialogs->BackGround_Render_Loop();
		}

		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float = false;

		if (App->CL_ImGui_Dialogs->flag_Float_Canceld == false)
		{
			m_Player->Limit_Look_Down = App->CL_ImGui_Dialogs->m_Dialog_Float;

			m_Player->flag_Altered = true;
			App->CL_Level->flag_Level_is_Modified = true;
			App->CL_FileView->Mark_Altered(m_Player->FileViewItem);
		}

		//App->Disable_Panels(false);

		Update_ListView_Player();

		return 1;
	}

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

	return 1;
}

// *************************************************************************
// *			Edit_Sounds:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Properties_Scene::Edit_Sounds(LPARAM lParam)
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

		App->CL_SoundMgr->Show_Sound_Player();

		strcpy(App->CL_Scene->B_Object[Index]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->SndVolume = App->CL_SoundMgr->SndVolume;

		//Mark_As_Altered(Index);

		Update_ListView_Sounds();

		return;
	}

	// Sound
	result = strcmp(btext, "Volume");
	if (result == 0)
	{
		App->CL_SoundMgr->flag_Accessed = 1;
		strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Index]->Sound_File);

		App->CL_SoundMgr->Show_Sound_Player();

		strcpy(App->CL_Scene->B_Object[Index]->Sound_File, App->CL_SoundMgr->Access_File);

		App->CL_Scene->B_Object[Index]->SndVolume = App->CL_SoundMgr->SndVolume;

		//Mark_As_Altered(Index);

		Update_ListView_Sounds();

		return;
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

		//Update_ListView_Teleport();

		return;
	}

	return;
}

// *************************************************************************
// *			Edit_Locations:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Scene::Edit_Locations(LPARAM lParam)
{
	int index = Current_Selected_Object;
	int result = 1;
	int List_Index;

	LPNMLISTVIEW List = (LPNMLISTVIEW)lParam;
	List_Index = List->iItem;
	ListView_GetItemText(Properties_hLV, List_Index, 0, btext, 20);

	result = strcmp(btext, "Name");
	if (result == 0)
	{
		App->CL_Locations->Rename_Object(index);
		Update_ListView_Locations();
	}

	result = strcmp(btext, "Goto");
	if (result == 0)
	{
		App->CL_Com_Player->Set_Player_Location(index);

		if (App->CL_Camera->flag_Free == true)
		{
			App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = true;
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;

			App->CL_Ogre->RenderFrame(5);

			App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = false;
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
		}
	}
}
