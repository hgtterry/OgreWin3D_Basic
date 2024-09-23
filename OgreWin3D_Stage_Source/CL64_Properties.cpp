/*
Copyright (c) OgreWin3D_Stage 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
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
