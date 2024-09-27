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
	Properties_Dlg_Active = 0;

	Current_Selected_Object = 0;

	Properties_Dlg_hWnd =	nullptr;
	Properties_hLV =		nullptr;
}

CL64_Properties::~CL64_Properties(void)
{
}

// *************************************************************************
// *		Start_GD_Properties:- Terry and Hazel Flanigan 2024    	 	   *
// *************************************************************************
void CL64_Properties::Start_Properties(void)
{
	if (Properties_Dlg_Active == 1)
	{
		return;
	}

	Properties_Dlg_Active = 1;

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
		App->CL_Properties->Properties_Dlg_Active = 0;

		HMENU mMenu = GetMenu(App->MainHwnd);
		CheckMenuItem(mMenu, ID_WINDOWS_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 0);

		break;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->hwndFrom == App->SBC_Properties->Properties_hLV)
		{
			switch (some_item->code)
			{
			case NM_CLICK:
			{
				App->SBC_Properties->ListView_OnClickOptions(lParam);
			}
			}
		}

		if (some_item->idFrom == IDC_BT_OBJECTHELP2 && some_item->code == NM_CUSTOMDRAW)
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
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->SBC_Scene->B_Area[Index]->Area_Name);

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
	//_itoa(App->SBC_Scene->V_Object[index]->This_Object_ID, Num, 10);
	strcpy(chr_ID, "Properties ID=0");
	//strcat(chr_ID, Num);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->SBC_Scene->B_Player[0]->Player_Name);

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
	grid[0][1] = "Mode", grid[1][1] = "1st_Person";
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
// *		Update_ListView_Objects:- Terry and Hazel Flanigan 2024 	   *
// *************************************************************************
bool CL64_Properties::Update_ListView_Objects()
{
	int index = Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(App->CL_Scene->V_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);


	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->SBC_Scene->V_Object[index]->Mesh_Name);


	const int NUM_ITEMS = 4;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->V_Object[index]->Mesh_Name;
	grid[0][1] = "Mesh File", grid[1][1] = App->CL_Scene->V_Object[index]->Mesh_FileName;
	grid[0][2] = "Materials", grid[1][2] = App->CL_Scene->V_Object[index]->Material_File;
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
	_itoa(App->CL_Scene->V_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->SBC_Scene->V_Object[index]->Mesh_Name);

	// new sound
	char chr_Play[100];
	if (App->CL_Scene->V_Object[index]->S_Collectable[0]->Play == 1)
	{
		strcpy(chr_Play, "True");
	}
	else
	{
		strcpy(chr_Play, "False");
	}

	char chr_Volume[100];
	float sum2 = 1;// App->CL_Scene->V_Object[index]->S_Collectable[0]->SndVolume;
	int Percent = int(sum2 * 100);
	_itoa(Percent, chr_Volume, 10);

	char chr_Counter_Disabled[20];
	if (App->CL_Scene->V_Object[index]->S_Collectable[0]->Counter_Disabled == 1)
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

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->V_Object[index]->Mesh_Name;
	grid[0][1] = "Mesh File", grid[1][1] = App->CL_Scene->V_Object[index]->Mesh_FileName;
	grid[0][2] = "Materials", grid[1][2] = App->CL_Scene->V_Object[index]->Material_File;
	grid[0][3] = " ", grid[1][3] = " ";
	grid[0][4] = "Sound", grid[1][4] = App->CL_Scene->V_Object[index]->S_Collectable[0]->Sound_File;
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
	_itoa(App->CL_Scene->V_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);

	char chr_Usage_Num[50];
	_itoa(App->CL_Scene->V_Object[index]->Usage, chr_Usage_Num, 10);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->SBC_Scene->V_Object[index]->Mesh_Name);


	char chr_Volume[100];
	float sum2 = 1;// App->SBC_Scene->V_Object[index]->SndVolume;
	int Percent = int(sum2 * 100);
	_itoa(Percent, chr_Volume, 10);

	const int NUM_ITEMS = 5;
	const int NUM_COLS = 2;
	std::string grid[NUM_COLS][NUM_ITEMS]; // string table
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->V_Object[index]->Mesh_Name;
	grid[0][1] = " ", grid[1][1] = " ";
	grid[0][2] = "Sound", grid[1][2] = App->CL_Scene->V_Object[index]->Sound_File;
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
// *	Update_ListView_Move_Entities:- Terry and Hazel Flanigan 2024 	    *
// *************************************************************************
bool CL64_Properties::Update_ListView_Move_Entities()
{

	int index = App->CL_Properties->Current_Selected_Object;

	char Num[10];
	char chr_ID[50];
	char IndexNum[10];
	_itoa(App->CL_Scene->V_Object[index]->This_Object_UniqueID, Num, 10);
	_itoa(index, IndexNum, 10);
	strcpy(chr_ID, "Unique ID ");
	strcat(chr_ID, Num);
	strcat(chr_ID, "  Object Index ");
	strcat(chr_ID, IndexNum);

	SetWindowText(Properties_Dlg_hWnd, chr_ID);
	//SetDlgItemText(App->SBC_Properties->Properties_Dlg_hWnd, IDC_STOBJECTNAME, (LPCTSTR)App->SBC_Scene->V_Object[index]->Mesh_Name);

	char chr_Distance[100];
	sprintf(chr_Distance, "%.3f ", App->CL_Scene->V_Object[index]->S_MoveType[0]->Move_Distance);

	char chr_Speed[100];
	sprintf(chr_Speed, "%.3f ", App->CL_Scene->V_Object[index]->S_MoveType[0]->Speed);

	char chr_Axis[100];
	if (App->CL_Scene->V_Object[index]->S_MoveType[0]->WhatDirection == Enums::Axis_x)
	{
		strcpy(chr_Axis, "X");
	}
	if (App->CL_Scene->V_Object[index]->S_MoveType[0]->WhatDirection == Enums::Axis_y)
	{
		strcpy(chr_Axis, "Y");
	}
	if (App->CL_Scene->V_Object[index]->S_MoveType[0]->WhatDirection == Enums::Axis_z)
	{
		strcpy(chr_Axis, "Z");
	}

	// new sound
	char chr_Play[100];
	if (App->CL_Scene->V_Object[index]->Play_Sound == 1)
	{
		strcpy(chr_Play, "True");
	}
	else
	{
		strcpy(chr_Play, "False");
	}

	char chr_Object_Name[100];
	strcpy(chr_Object_Name, App->CL_Scene->V_Object[App->CL_Scene->V_Object[index]->S_MoveType[0]->Object_To_Move_Index]->Mesh_Name);

	char chr_Trigger_Value[100];
	_itoa(App->CL_Scene->V_Object[index]->S_MoveType[0]->Trigger_Value, chr_Trigger_Value, 10);

	char chr_Volume[100];
	float sum2 = 1;// App->CL_Scene->V_Object[index]->SndVolume;
	int Percent = int(sum2 * 100);
	_itoa(Percent, chr_Volume, 10);

	char chr_Counter_Disabled[20];
	if (App->CL_Scene->V_Object[index]->S_MoveType[0]->Counter_Disabled == 1)
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

	grid[0][0] = "Name", grid[1][0] = App->CL_Scene->V_Object[index]->Mesh_Name;
	grid[0][1] = " ", grid[1][1] = " ";
	grid[0][2] = "Move_Object", grid[1][2] = chr_Object_Name;
	grid[0][3] = "Axis", grid[1][3] = chr_Axis;
	grid[0][4] = "Distance", grid[1][4] = chr_Distance;
	grid[0][5] = "Speed", grid[1][5] = chr_Speed;
	grid[0][6] = " ", grid[1][6] = " ";
	grid[0][7] = "Sound", grid[1][7] = App->CL_Scene->V_Object[index]->Sound_File;
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
