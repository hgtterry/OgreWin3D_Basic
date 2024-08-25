/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Resources.h"

CL64_Resources::CL64_Resources(void)
{
	ResourcePath[0] = 0;

	flag_Show_App_Res = 0;
	flag_Show_Demo_Res = 0;
	flag_Show_All_Meshes = 0;
	flag_Show_All_Textures = 0;
	flag_Show_All_Materials = 0;
	flag_Show_Group_All = 0;

	mFileString.clear();

	Ogre_ExternalResourceLoaded = 0;

	Ogre_Loader_Resource_Group = "Ogre_Loader_Resource_Group";
	mSelected_Resource_Group = "App_Resource_Group";

	FX_General_hLV = nullptr;
	btext[0] = 0;
	RV_Size = 0;
}

CL64_Resources::~CL64_Resources(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Resources::Reset_Class(void) const
{
	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Converters->Temp_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Converters->Temp_Resource_Group);
	}

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Ogre->World_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->World_Resource_Group);
	}

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Resources->Ogre_Loader_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Resources->Ogre_Loader_Resource_Group);
	}

	//Debug
}

// *************************************************************************
// *	  		Start_Resources:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Resources::Start_Resources()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_RESOURCESMATERIAL, App->Fdlg, (DLGPROC)Resources_Proc);
}

// *************************************************************************
// *			Resources_Proc:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Resources::Resources_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STCOUNT, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_RESOURCE_GROUP, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GB_LISTOPTIONS, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GB_OPTIONS, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_APPRESOURCES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DEMORESOURCES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_RE_SCAN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		
		SendDlgItemMessage(hDlg, IDC_GROUPALL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ALLMATERIALS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ALLTEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ALLMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_RESOURCEGROUPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Resources->Update_Resource_Groups_Combo(hDlg);

		App->CL_Resources->CreateListGeneral_FX(hDlg);
		App->CL_Resources->Reset_Flags();
		App->CL_Resources->flag_Show_Group_All = 1;

		App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";
		App->CL_Resources->Scan_Resource_Group(App->CL_Resources->mSelected_Resource_Group);

		int Items = App->CL_Resources->Show_Resource_Group_All();
		App->CL_Resources->Update_Counter(Items, hDlg);

		App->CL_Resources->Set_Title(hDlg, (LPSTR)"All");

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STCOUNT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_RESOURCE_GROUP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_GB_LISTOPTIONS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_GB_OPTIONS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->hwndFrom == App->CL_Resources->FX_General_hLV)
		{
			switch (some_item->code)
			{
			case NM_CLICK:
			{
				App->CL_Resources->ListView_OnClickOptions(lParam);
				
			}
			}
		}

		if (some_item->idFrom == IDC_BT_APPRESOURCES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Resources->flag_Show_App_Res);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_DEMORESOURCES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Resources->flag_Show_Demo_Res);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_RE_SCAN)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_GROUPALL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Resources->flag_Show_Group_All);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_ALLMATERIALS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Resources->flag_Show_All_Materials);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_ALLTEXTURES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Resources->flag_Show_All_Textures);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_ALLMESH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Resources->flag_Show_All_Meshes);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}


	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_GROUPALL)
		{
			App->CL_Resources->Set_Title(hDlg, (LPSTR)"All");

			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_Group_All = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			int Items = App->CL_Resources->Show_Resource_Group_All();
			App->CL_Resources->Update_Counter(Items, hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_ALLMATERIALS)
		{
			App->CL_Resources->Set_Title(hDlg, (LPSTR)"Materials");

			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_All_Materials = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			//int Items = App->CL_Resources->ShowAllMaterials();
			int Items = App->CL_Resources->Show_Resource_Group_Materials();
			App->CL_Resources->Update_Counter(Items, hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_ALLTEXTURES)
		{
			App->CL_Resources->Set_Title(hDlg, (LPSTR)"Textures");

			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_All_Textures = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			int Items = App->CL_Resources->ShowAllTextures();
			App->CL_Resources->Update_Counter(Items, hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_ALLMESH)
		{
			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_All_Meshes = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			//int Items = App->CL_Resources->ShowAllMeshes();
			
			App->CL_Resources->Set_Title(hDlg,(LPSTR)"Meshes");

			int Items = App->CL_Resources->Show_Resource_Group_Meshes();
			App->CL_Resources->Update_Counter(Items, hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_DEMORESOURCES)
		{
			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)"Resources:- Demo Resources");
			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_Demo_Res = 1;
			
			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_Resources->Show_Resource_Group(App->CL_Ogre->World_Resource_Group);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_APPRESOURCES)
		{
			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)"Resources:- App Resources");
			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_App_Res = 1;
			
			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_Resources->Show_Resource_Group(App->CL_Ogre->App_Resource_Group);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CB_RESOURCEGROUPS)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };
				char Title[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_RESOURCEGROUPS);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);
				App->CL_Resources->mSelected_Resource_Group = buff;

				App->CL_Resources->Set_Title(hDlg, (LPSTR)"All");

				App->CL_Resources->Scan_Resource_Group(App->CL_Resources->mSelected_Resource_Group);
				int Items = App->CL_Resources->Show_Resource_Group_All();
				App->CL_Resources->Update_Counter(Items, hDlg);

				App->CL_Resources->Reset_Flags();
				App->CL_Resources->flag_Show_Group_All = 1;

				RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_RE_SCAN)
		{
			int Items = App->CL_Resources->Show_Resource_Group_All();
			App->CL_Resources->Update_Counter(Items, hDlg);
			return TRUE;
		}
		
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
// *			Set_Title:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Resources::Set_Title(HWND hDlg,char* Title)
{
	char mTitle[MAX_PATH]{ 0 };
	
	strcpy(mTitle, App->CL_Resources->mSelected_Resource_Group.c_str());
	strcat(mTitle, ":  ( ");
	strcat(mTitle, Title);
	strcat(mTitle, " )");

	SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)mTitle);
}

// *************************************************************************
// *			Reset_Flags:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Resources::Reset_Flags()
{
	flag_Show_App_Res = 0;
	flag_Show_Demo_Res = 0;
	flag_Show_All_Materials = 0;
	flag_Show_All_Textures = 0;
	flag_Show_All_Meshes = 0;
	flag_Show_Group_All = 0;
}

// *************************************************************************
// *			Update_Counter:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Resources::Update_Counter(int Value, HWND hDlg)
{
	char buf[100];
	char num[100];
	_itoa(Value, num, 10);

	strcpy(buf, "Items:- ");
	strcat(buf, num);
	SetDlgItemText(hDlg, IDC_STCOUNT, (LPCTSTR)buf);
}

// *************************************************************************
// *		CreateListGeneral_FX:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Resources::CreateListGeneral_FX(HWND hDlg)
{
	int NUM_COLS = 5;
	FX_General_hLV = CreateWindowEx(0, WC_LISTVIEW, "",
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS, 2, 50,
		1280, 430, hDlg, 0, App->hInst, NULL);

	DWORD exStyles = LVS_EX_GRIDLINES;

	ListView_SetExtendedListViewStyleEx(FX_General_hLV, exStyles, exStyles);

	ListView_SetBkColor(FX_General_hLV, RGB(250, 250, 250));
	ListView_SetTextBkColor(FX_General_hLV, RGB(250, 250, 250));

	LV_COLUMN lvC;
	memset(&lvC, 0, sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column
	std::string headers[] =
	{
		"Script","Resource Group","Used","Material File","Path"
	};
	int headerSize[] =
	{
		165,380,70,250,250
	};

	//Groups

	for (int header = 0; header < NUM_COLS; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<char*>(headers[header].c_str());
		ListView_InsertColumn(FX_General_hLV, header, &lvC);
	}
	HFONT Font;
	Font = CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Aerial Black");
	SendMessage(FX_General_hLV, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

}

// *************************************************************************
// *			ListView_OnClickOptions  Terry Bernie			 		   *
// *************************************************************************
void CL64_Resources::ListView_OnClickOptions(LPARAM lParam)
{
	int List_Index;

	LPNMLISTVIEW List = (LPNMLISTVIEW)lParam;
	List_Index = List->iItem;
	ListView_GetItemText(FX_General_hLV, List_Index, 0, btext, MAX_PATH);

	//App->Say(btext);
}

// *************************************************************************
// *	Update_Resource_Groups_Combo:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Resources::Update_Resource_Groups_Combo(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_CB_RESOURCEGROUPS, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	Ogre::StringVector sv = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();

	int Count = 0;
	int Size = sv.size();

	while (Count < Size)
	{
		if (sv[Count] == "General" || sv[Count] == "OgreInternal" || sv[Count] == "OgreAutodetect")
		{

		}
		else
		{
			SendDlgItemMessage(hDlg, IDC_CB_RESOURCEGROUPS, CB_ADDSTRING, (WPARAM)0, (LPARAM)sv[Count].c_str());
		}

		Count++;
	}

	SendDlgItemMessage(hDlg, IDC_CB_RESOURCEGROUPS, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

// *************************************************************************
// *			Get_Resource_Groups:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Resources::Get_Resource_Groups()
{
	Ogre::StringVector sv = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();

	int Count = 0;
	while (Count < sv.size())
	{
		App->Say(sv[Count].c_str());
		Count++;
	}
}

// *************************************************************************
// *	   Show_Resource_Group_All:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
int CL64_Resources::Show_Resource_Group_All()
{
	ListView_DeleteAllItems(FX_General_hLV);

	int NUM_COLS = 5;

	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	LV_COLUMN lvC;
	memset(&lvC, 0, sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column
	std::string headers[] =
	{
		"File", "Archive Type","Path"," "," "
	};
	int headerSize[] =
	{
		165,120,470,150
	};

	for (int header = 0; header < NUM_COLS; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<char*>(headers[header].c_str());
		ListView_SetColumn(FX_General_hLV, header, &lvC);
	}

	int Count = 0;
	int End = RV_Size;

	while (Count < End)
	{
		pitem.iItem = Count;
		pitem.pszText = (LPSTR)RV_FileName[Count].c_str();

		ListView_InsertItem(FX_General_hLV, &pitem);
		ListView_SetItemText(FX_General_hLV, Count, 1, (LPSTR)RV_Archive_GetType[Count].c_str());
		ListView_SetItemText(FX_General_hLV, Count, 2, (LPSTR)RV_Archive_GetName[Count].c_str());

		Count++;

	}

	return Count;
}

// *************************************************************************
// *	  Show_Resource_Group_Materials:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
int CL64_Resources::Show_Resource_Group_Materials()
{
	ListView_DeleteAllItems(FX_General_hLV);

	int	 pRow = 0;
	int NUM_COLS = 4;

	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	LV_COLUMN lvC;
	memset(&lvC, 0, sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column
	std::string headers[] =
	{
		"File", "Archive Type","Path"," "
	};
	int headerSize[] =
	{
		165,120,470,150
	};

	for (int header = 0; header < NUM_COLS; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<char*>(headers[header].c_str());
		ListView_SetColumn(FX_General_hLV, header, &lvC);
	}

	int Count = 0;
	int End = RV_Size;
	while (Count < End)
	{
		int test = RV_FileName[Count].find(".material");

		if (test > 0)
		{
			pitem.iItem = pRow;
			pitem.pszText = (LPSTR)RV_FileName[Count].c_str();

			ListView_InsertItem(FX_General_hLV, &pitem);
			ListView_SetItemText(FX_General_hLV, pRow, 1, (LPSTR)RV_Archive_GetType[Count].c_str());
			ListView_SetItemText(FX_General_hLV, pRow, 2, (LPSTR)RV_Archive_GetName[Count].c_str());

			pRow++;
		}

		Count++;
	}

	RedrawWindow(FX_General_hLV, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	
	return pRow;
}

// *************************************************************************
// *	  Show_Resource_Group_Meshes:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
int CL64_Resources::Show_Resource_Group_Meshes()
{
	ListView_DeleteAllItems(FX_General_hLV);

	int	 pRow = 0;
	int NUM_COLS = 4;

	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	LV_COLUMN lvC;
	memset(&lvC, 0, sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column
	std::string headers[] =
	{
		"File", "Archive Type","Path"," "
	};
	int headerSize[] =
	{
		165,120,470,150
	};

	for (int header = 0; header < NUM_COLS; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<char*>(headers[header].c_str());
		ListView_SetColumn(FX_General_hLV, header, &lvC);
	}

	int Count = 0;
	int End = RV_Size;
	while (Count < End)
	{
		int test = RV_FileName[Count].find(".mesh");

		if (test > 0)
		{
			pitem.iItem = pRow;
			pitem.pszText = (LPSTR)RV_FileName[Count].c_str();

			ListView_InsertItem(FX_General_hLV, &pitem);
			ListView_SetItemText(FX_General_hLV, pRow, 1, (LPSTR)RV_Archive_GetType[Count].c_str());
			ListView_SetItemText(FX_General_hLV, pRow, 2, (LPSTR)RV_Archive_GetName[Count].c_str());

			pRow++;
		}

		Count++;

	}

	RedrawWindow(FX_General_hLV, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	return pRow;
}

// *************************************************************************
// *			ShowAllTextures:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
int CL64_Resources::ShowAllTextures()
{
	ListView_DeleteAllItems(FX_General_hLV);

	int NUM_COLS = 5;

	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	LV_COLUMN lvC;
	memset(&lvC, 0, sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column
	std::string headers[] =
	{
		"Texture", "Resource Group","Path",""," "
	};
	int headerSize[] =
	{
		165,250,170,150
	};

	for (int header = 0; header < NUM_COLS; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<char*>(headers[header].c_str());
		ListView_SetColumn(FX_General_hLV, header, &lvC);
	}

	bool pIsLoaded = 0;
	int	 pRow = 0;
	char Origin[MAX_PATH];
	char pUsed[255];
	char pScriptName[255];
	bool pHasSkel = 0;

	Ogre::String st;
	Ogre::ResourcePtr pp;

	Ogre::ResourceManager::ResourceMapIterator TextureIterator = Ogre::TextureManager::getSingleton().getResourceIterator();

	while (TextureIterator.hasMoreElements())
	{
		//strcpy(pScriptName,(static_cast<Ogre::MaterialPtr>(TextureIterator.peekNextValue()))->getName().c_str());

		strcpy(pScriptName, TextureIterator.peekNextValue()->getName().c_str());
		strcpy(Origin, TextureIterator.peekNextValue()->getGroup().c_str());
		
		if (pIsLoaded == 1)
		{
			strcpy(pUsed, "Yes");
		}
		else
		{
			strcpy(pUsed, "No");
		}

		char Filename[255];
		char ext[255];
		char JustFile[255];
		char SubFolder[255];
		_splitpath(pScriptName, NULL, SubFolder, Filename, ext);
		strcpy(JustFile, Filename);
		strcat(JustFile, ext);

		/*if(SubFolder[0] > 0)
		{
		App->Say(SubFolder);
		}*/

		Start_List_Folders(NULL, JustFile, 0);

		int test = strcmp(Origin, mSelected_Resource_Group.c_str());

		if (test == 0)
		{
			pitem.iItem = pRow;
			pitem.pszText = JustFile;

			ListView_InsertItem(FX_General_hLV, &pitem);
			ListView_SetItemText(FX_General_hLV, pRow, 1, Origin);
			ListView_SetItemText(FX_General_hLV, pRow, 2, (LPSTR)"----");// ResourcePath);
			ListView_SetItemText(FX_General_hLV, pRow, 3, (LPSTR)" ");

			pRow++;
		}

		TextureIterator.moveNext();
	}


	/*char File[MAX_PATH];
	strcpy(File, App->GD_Directory_FullPath);
	strcat(File, "\\Media\\test.tga");

	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->App_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	for (i = RFI->begin(); i != iend; ++i)
	{
		if (i->filename == "sinbad_body.tga")
		{
			App->Say(i->filename.c_str());
			App->Say(i->archive->getName().c_str());

			Ogre::DataStreamPtr ff = i->archive->open(i->filename);

			mFileString = ff->getAsString();

			Export_Texture(File);

			mFileString.clear();
		}
	}*/

	return pRow;
}

// *************************************************************************
// *			Scan_Resource_Group:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
bool CL64_Resources::Scan_Resource_Group(Ogre::String ResourceGroup)
{
	RV_FileName.resize(0);
	RV_Archive_GetName.resize(0);
	RV_Archive_GetType.resize(0);

	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(ResourceGroup, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();
	int Count = 0;
	
	for (i = RFI->begin(); i != iend; ++i)
	{
		RV_FileName.push_back(i->filename);
		RV_Archive_GetName.push_back(i->archive->getName());
		RV_Archive_GetType.push_back(i->archive->getType());
	
		Count++;
	}

	RV_Size = Count;

	return 1;
}

// *************************************************************************
// *			Export_Texture:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
bool CL64_Resources::Export_Texture(char* FileName) const
{
	std::ofstream outFile;
	outFile.open(FileName, std::ios::binary);
	outFile << mFileString;
	outFile.close();

	return 1;
}

// *************************************************************************
// *			ShowAllMaterials:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
int CL64_Resources::ShowAllMaterials()
{
	ListView_DeleteAllItems(FX_General_hLV);

	int NUM_COLS = 5;

	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	LV_COLUMN lvC;
	memset(&lvC, 0, sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column
	std::string headers[] =
	{
		"Material", "Material File","Resource Group","Used"," "
	};
	int headerSize[] =
	{
		165,250,170,50
	};

	for (int header = 0; header < NUM_COLS; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<char*>(headers[header].c_str());
		ListView_SetColumn(FX_General_hLV, header, &lvC);
	}
	

	int	 pRow = 0;
	char Group[MAX_PATH];
	char pScriptName[255];
	char pScriptFile[255];
	char pUsed[255];
	bool pIsLoaded = 0;
	Ogre::String st;
	Ogre::ResourcePtr pp;
	Ogre::ResourceManager::ResourceMapIterator materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();

	while (materialIterator.hasMoreElements())
	{
		strcpy(pScriptName, materialIterator.peekNextValue()->getName().c_str());

		pp = Ogre::MaterialManager::getSingleton().getByName(pScriptName);
		st = pp->getOrigin();
		pIsLoaded = pp->isLoaded();

		strcpy(Group, materialIterator.peekNextValue()->getGroup().c_str());

		if (pIsLoaded == 1)
		{
			strcpy(pUsed, "Yes");
		}
		else
		{
			strcpy(pUsed, "No");
		}

		//--------------------------------
		if (st == "")
		{
			strcpy(pScriptFile, "Internal");
			strcpy(ResourcePath, "Internal");

		}
		else if (st == "SdkTrays.material")
		{
			strcpy(pScriptFile, st.c_str());
			strcpy(ResourcePath, "packs\\SdkTrays.zip");
		}
		else if (st == "OgreCore.material")
		{
			strcpy(pScriptFile, st.c_str());
			strcpy(ResourcePath, "packs\\OgreCore.zip");
		}
		else if (st == "OgreProfiler.material")
		{
			strcpy(pScriptFile, st.c_str());
			strcpy(ResourcePath, "packs\\OgreCore.zip");
		}
		else if (st == "PhysCore.material")
		{
			strcpy(pScriptFile, st.c_str());
			strcpy(ResourcePath, "packs\\GDCore.zip");
		}
		else
		{
			strcpy(pScriptFile, st.c_str());
			//App->GDCL_Resources->SearchFolders(pScriptFile,"Media\\materials\\");
			Start_List_Folders(NULL, pScriptFile, 0);

		}

		pitem.iItem = pRow;
		pitem.pszText = pScriptName;

		ListView_InsertItem(FX_General_hLV, &pitem);
		ListView_SetItemText(FX_General_hLV, pRow, 1, pScriptFile);
		ListView_SetItemText(FX_General_hLV, pRow, 2, Group);
		ListView_SetItemText(FX_General_hLV, pRow, 3, pUsed);
		//ListView_SetItemText(FX_General_hLV, pRow, 3, ResourcePath);
		
		pRow++;

		materialIterator.moveNext();
	}

	return pRow;
}

// **************************************************************************
// *			Show_Resource_Group:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Resources::Show_Resource_Group(const Ogre::String& ResourceGroup)
{
	ListView_DeleteAllItems(FX_General_hLV);

	bool Test = Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(ResourceGroup);

	if (Test == 1)
	{

		Ogre::String st;
		int NUM_COLS = 4;

		LV_ITEM pitem;
		memset(&pitem, 0, sizeof(LV_ITEM));
		pitem.mask = LVIF_TEXT;

		LV_COLUMN lvC;
		memset(&lvC, 0, sizeof(LV_COLUMN));
		lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvC.fmt = LVCFMT_LEFT;  // left-align the column
		std::string headers[] =
		{
			"Resource Group", "Path","Type"," "
		};
		int headerSize[] =
		{
			165,380,170,250
		};

		for (int header = 0; header < NUM_COLS; header++)
		{
			lvC.iSubItem = header;
			lvC.cx = headerSize[header]; // width of the column, in pixels
			lvC.pszText = const_cast<char*>(headers[header].c_str());
			ListView_SetColumn(FX_General_hLV, header, &lvC);
		}

		int	 pRow = 0;
		char pPath[MAX_PATH];
		char chr_Type[255];

		Ogre::ResourceGroupManager::LocationList resLocationsList = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(ResourceGroup);
		Ogre::ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
		Ogre::ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();

		for (; it != itEnd; ++it)
		{
			pitem.iItem = pRow;
			pitem.pszText = (LPSTR)"App Resource Group";

			strcpy(pPath, it->archive->getName().c_str());
			strcpy(chr_Type, it->archive->getType().c_str());

			ListView_InsertItem(FX_General_hLV, &pitem);
			ListView_SetItemText(FX_General_hLV, pRow, 1, pPath);
			ListView_SetItemText(FX_General_hLV, pRow, 2, chr_Type);
			ListView_SetItemText(FX_General_hLV, pRow, 3, (LPSTR)" ");
		}
	}
	else
	{
		App->Say("No Project Loaded");
	}

}

#include <string.h>

// *************************************************************************
// *			ShowAllMeshes:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
int CL64_Resources::ShowAllMeshes()
{
	Ogre::String st;
	int NUM_COLS = 4;

	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	ListView_DeleteAllItems(FX_General_hLV);

	LV_COLUMN lvC;
	memset(&lvC, 0, sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column
	std::string headers[] =
	{
		"Mesh", "Has Skeleton","Material File","Path"
	};
	int headerSize[] =
	{
		165,180,170,250
	};

	for (int header = 0; header < NUM_COLS; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<char*>(headers[header].c_str());
		ListView_SetColumn(FX_General_hLV, header, &lvC);
	}

	int	 pRow = 0;
	char pMeshName[255];
	char chr_AsSkell[255];
	char buff[255];
	char Origin[MAX_PATH];
	bool pHasSkel = 0;
	Ogre::ResourcePtr pp;

	Ogre::ResourceManager::ResourceMapIterator MeshIterator = Ogre::MeshManager::getSingleton().getResourceIterator();
	
	while (MeshIterator.hasMoreElements())
	{
		//strcpy(pMeshName,(static_cast<Ogre::MeshPtr>(MeshIterator.peekNextValue()))->getName().c_str());

		strcpy(pMeshName, MeshIterator.peekNextValue()->getName().c_str());
		
		pp = Ogre::MeshManager::getSingleton().getByName(pMeshName);
		
		strcpy(Origin, MeshIterator.peekNextValue()->getGroup().c_str());
		
		Ogre::Entity* DummyEnt = App->CL_Ogre->mSceneMgr->createEntity("GDTemp1", pMeshName);
		pHasSkel = DummyEnt->hasSkeleton();
		App->CL_Ogre->mSceneMgr->destroyEntity(DummyEnt);

		if (pHasSkel == 1)
		{
			strcpy(chr_AsSkell, "Yes");
		}
		else
		{
			strcpy(chr_AsSkell, "No");
		}

		strcpy(buff, pMeshName);
		if (_stricmp(buff + strlen(buff) - 5, ".mesh") != 0)
		{
			strcpy(ResourcePath, "Internal");
		}
		else
		{
			Start_List_Folders(NULL, pMeshName, 0);
		}

		pitem.iItem = pRow;
		pitem.pszText = pMeshName;

		ListView_InsertItem(FX_General_hLV, &pitem);
		ListView_SetItemText(FX_General_hLV, pRow, 1, chr_AsSkell);
		ListView_SetItemText(FX_General_hLV, pRow, 2, ResourcePath);
		ListView_SetItemText(FX_General_hLV, pRow, 3, Origin);

		pRow++;

		MeshIterator.moveNext();
	}

	return pRow;
}

// *************************************************************************
// *			Start_List_Folders:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Resources::Start_List_Folders(HWND List, char* FileName, bool ListDlg)
{
	char StartFolder[MAX_PATH];

	if (ListDlg == 1)
	{
		SendMessage(List, LB_RESETCONTENT, 0, 0);
	}

	strcpy(StartFolder, App->GD_Directory_FullPath);
	strcat(StartFolder, "\\");
	strcat(StartFolder, "Media\\");

	List_Folders(List, StartFolder, FileName, ListDlg);
	return;
}

// *************************************************************************
// *			List_Folders:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
void CL64_Resources::List_Folders(HWND List, char* StartFolder, char* FileName, bool ListDlg)
{
	char SearchPath[2048];

	if (ListDlg == 1)
	{
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)StartFolder);
	}

	int Result = FindPath_New(FileName, StartFolder);
	if (Result == 1)
	{
		ResourcePath[0] = 0;
		strcat(ResourcePath, StartFolder);

		if (ListDlg == 1)
		{
			SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)StartFolder);
		}

		return;
	}

	strcpy(SearchPath, StartFolder);
	strcat(SearchPath, "*.*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(SearchPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//App->Say("Cant Find File");
		return;
	}

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

			if (strcmp(FindFileData.cFileName, "."))
			{
				if (strcmp(FindFileData.cFileName, ".."))
				{
					char NewPath[2048];
					strcpy(NewPath, StartFolder);
					strcat(NewPath, FindFileData.cFileName);
					strcat(NewPath, "\\");

					List_Folders(List, NewPath, FileName, ListDlg);

				}
			}
		}
	}

	while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);

	return;
}

// *************************************************************************
// *			FindPath_New:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
bool CL64_Resources::FindPath_New(char* File, char* Folder)
{
	char pSearchPath[MAX_PATH];
	char pReturnPath[MAX_PATH];
	char* pPartPath;

	strcpy(pSearchPath, Folder);

	int result = SearchPath((LPCTSTR)pSearchPath, (LPCTSTR)File, NULL, MAX_PATH, pReturnPath, &pPartPath);
	if (result == 0)
	{
		return 0;
	}

	return 1;
}

// *************************************************************************
// *		Load_OgreCFG_Resources:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Resources::Load_OgreCFG_Resources(const Ogre::String& file)
{
	cf.clear();

	char ConfigFile1[255];
	strcpy(ConfigFile1, "resources.cfg");

	char Path[1024];
	char Path2[1024];
	strcpy(Path2, file.c_str());
	strcpy(Path, file.c_str());

	int Len = strlen(ConfigFile1);
	int Len2 = strlen(Path2);
	Path[Len2 - Len] = 0;

	std::vector<Ogre::String> materialNames;
	
	if (App->CL_Scene->Imported_Ogre_Node)
	{
		App->CL_Ogre->Ogre3D_Listener->Ogre_Model_Loaded = 0;

		App->CL_Scene->Imported_Ogre_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Scene->Imported_Ogre_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Scene->Imported_Ogre_Ent);
		App->CL_Scene->Imported_Ogre_Ent = nullptr;
		App->CL_Scene->Imported_Ogre_Node = nullptr;

		UnloadUserResources();

		App->CL_Importers->Flag_Reload_Ogre_Model = 1;
	}

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Resources->Ogre_Loader_Resource_Group) == NULL)
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Resources->Ogre_Loader_Resource_Group);
	}

#pragma warning(disable : 4996) // Nightmare why

	ResourcesCfgFile = file;

	cf.load(ResourcesCfgFile);

	Ogre::ConfigFile::SectionIterator index = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (index.hasMoreElements())
	{
		secName = index.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = index.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;

			if (!archName.empty())
			{
				if (archName[0] != '/')
				{
					archName = Path + archName;
				}
			}

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, App->CL_Resources->Ogre_Loader_Resource_Group);
		}
	}

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		Ogre_ExternalResourceLoaded = 1;
	}
	catch (...)
	{
		Ogre_ExternalResourceLoaded = 0;
	}

}

// **************************************************************************
// *		UnloadUserResources:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_Resources::UnloadUserResources()
{
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Resources->Ogre_Loader_Resource_Group);
	Ogre_ExternalResourceLoaded = 0;
}

