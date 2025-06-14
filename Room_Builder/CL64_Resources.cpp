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
#include "CL64_Resources.h"

CL64_Resources::CL64_Resources()
{
	FX_General_hLV = NULL;
	Resource_Dlg_hWnd = NULL;

	Project_Resource_Group = "Project_Resource_Group";
	mSelected_Resource_Group = "App_Resource_Group";

	Extension_Type = Enums::Resource_File_Type_None;

	mFileString.clear();

	mSelected_File[0] = 0;
	mbtext[0] = 0;

	flag_Show_Group_All = 0;
	flag_Show_App_Res = 0;
	flag_Show_Demo_Res = 0;
	flag_Show_All_Materials = 0;
	flag_Show_All_Meshes = 0;
	flag_Show_All_Textures = 0;
	flag_Show_All_Skeleton = 0;
	flag_Show_All_Overlays = 0;

	GroupSelIndex = 0;
	RV_Size = 0;
}

CL64_Resources::~CL64_Resources()
{
}

// *************************************************************************
// *	  		Start_Resources:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Resources::Start_Resources()
{
	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);
	DialogBox(App->hInst, (LPCTSTR)IDD_RESOURCE_VIEWER, App->MainHwnd, (DLGPROC)Proc_Resources);
}

// *************************************************************************
// *			Resources_Proc:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Resources::Proc_Resources(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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


		SendDlgItemMessage(hDlg, IDC_ST_SELECTEDFILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BT_EXPORT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_VIEWFILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		// -------------------------- Show Resource
		SendDlgItemMessage(hDlg, IDC_GROUPALL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ALLMATERIALS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ALLTEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ALLMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_LIST_SKELETON, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_LIST_OVERLAY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//// -------------------------- 
		SendDlgItemMessage(hDlg, IDC_LST_GROUPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_ST_SELECTEDFILE, (LPCTSTR)App->CL_Resources->mSelected_File);

		App->CL_Resources->Resource_Dlg_hWnd = hDlg;

		//App->CL_Resources->Export_Button = GetDlgItem(hDlg, IDC_BT_EXPORT);

		EnableWindow(GetDlgItem(hDlg, IDC_BT_VIEWFILE), false);
		//EnableWindow(GetDlgItem(hDlg, IDC_BT_EXPORT), true);

		App->CL_Resources->Update_Resource_Groups_Combo(hDlg);

		App->CL_Resources->CreateListGeneral_FX(hDlg);
		App->CL_Resources->Reset_Flags();
		App->CL_Resources->flag_Show_Group_All = 1;

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

		if (GetDlgItem(hDlg, IDC_ST_SELECTEDFILE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
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
				SetDlgItemText(hDlg, IDC_ST_SELECTEDFILE, (LPCTSTR)App->CL_Resources->mSelected_File);
			}
			}
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		/*if (some_item->idFrom == IDC_BT_EXPORT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_EXPORT));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}*/

		if (some_item->idFrom == IDC_BT_VIEWFILE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_VIEWFILE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

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

		if (some_item->idFrom == IDC_BT_LIST_SKELETON)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Resources->flag_Show_All_Skeleton);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_LIST_OVERLAY)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Resources->flag_Show_All_Overlays);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BT_VIEWFILE)
		{
			if (App->CL_Resources->Extension_Type == Enums::Resource_File_Type_Texture)
			{
				App->CL_Resources->View_Texture(App->CL_Resources->mSelected_File, hDlg);
			}

			/*if (App->CL_Resources->Extension_Type == Enums::Resource_File_Type_Material)
			{
				App->CL_Resources->View_File(App->CL_Resources->mSelected_File, hDlg);
			}*/

			/*if (App->CL_Resources->Extension_Type == Enums::Resource_File_Type_Overlay)
			{
				App->CL_Resources->View_File(App->CL_Resources->mSelected_File, hDlg);
			}*/

			return TRUE;
		}

		/*if (LOWORD(wParam) == IDC_BT_EXPORT)
		{
			App->CL_Resources->Export_File(App->CL_Resources->mSelected_File);

			return TRUE;
		}*/

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

			int Items = App->CL_Resources->Show_Resource_Group_Type(Enums::Resource_File_Type_Material);
			App->CL_Resources->Update_Counter(Items, hDlg);

			App->CL_Resources->Extension_Type = Enums::Resource_File_Type_Material;
			EnableWindow(GetDlgItem(hDlg, IDC_BT_VIEWFILE), true);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_ALLTEXTURES)
		{
			App->CL_Resources->Set_Title(hDlg, (LPSTR)"Textures");

			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_All_Textures = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			int Items = App->CL_Resources->Show_Resource_Group_Type(Enums::Resource_File_Type_Texture);

			App->CL_Resources->Update_Counter(Items, hDlg);

			App->CL_Resources->Extension_Type = Enums::Resource_File_Type_Texture;
			EnableWindow(GetDlgItem(hDlg, IDC_BT_VIEWFILE), true);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_ALLMESH)
		{
			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_All_Meshes = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_Resources->Set_Title(hDlg, (LPSTR)"Meshes");

			int Items = App->CL_Resources->Show_Resource_Group_Type(Enums::Resource_File_Type_Mesh);
			App->CL_Resources->Update_Counter(Items, hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_LIST_SKELETON)
		{
			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_All_Skeleton = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_Resources->Set_Title(hDlg, (LPSTR)"Skeletons");

			int Items = App->CL_Resources->Show_Resource_Group_Type(Enums::Resource_File_Type_Skeleton);
			App->CL_Resources->Update_Counter(Items, hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_LIST_OVERLAY)
		{
			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_All_Overlays = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_Resources->Set_Title(hDlg, (LPSTR)"Overlays");

			int Items = App->CL_Resources->Show_Resource_Group_Type(Enums::Resource_File_Type_Overlay);
			App->CL_Resources->Update_Counter(Items, hDlg);

			App->CL_Resources->Extension_Type = Enums::Resource_File_Type_Overlay;
			EnableWindow(GetDlgItem(hDlg, IDC_BT_VIEWFILE), true);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_LST_GROUPS)
		{
			char buff[MAX_PATH]{ 0 };
			char Title[MAX_PATH]{ 0 };

			int Index = SendDlgItemMessage(hDlg, IDC_LST_GROUPS, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (Index == -1)
			{
				return 1;
			}

			if (App->CL_Resources->GroupSelIndex == Index)
			{
				return 1;
			}

			App->CL_Resources->GroupSelIndex = Index;

			SendDlgItemMessage(hDlg, IDC_LST_GROUPS, LB_GETTEXT, (WPARAM)Index, (LPARAM)buff);

			App->CL_Resources->mSelected_Resource_Group = buff;

			App->CL_Resources->Set_Title(hDlg, (LPSTR)"All");

			App->CL_Resources->Scan_Resource_Group(App->CL_Resources->mSelected_Resource_Group);
			int Items = App->CL_Resources->Show_Resource_Group_All();
			App->CL_Resources->Update_Counter(Items, hDlg);

			App->CL_Resources->Reset_Flags();
			App->CL_Resources->flag_Show_Group_All = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;
}

// *************************************************************************
// *		CreateListGeneral_FX:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Resources::CreateListGeneral_FX(HWND hDlg)
{
	int NUM_COLS = 5;
	FX_General_hLV = CreateWindowEx(0, WC_LISTVIEW, "",
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 2, 50,
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
		ListView_InsertColumn(FX_General_hLV, header, &lvC);
	}

	SendMessage(FX_General_hLV, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

}

// *************************************************************************
// *		ListView_OnClickOptions:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Resources::ListView_OnClickOptions(LPARAM lParam)
{
	int List_Index;

	LPNMLISTVIEW List = (LPNMLISTVIEW)lParam;
	List_Index = List->iItem;
	ListView_GetItemText(FX_General_hLV, List_Index, 0, mbtext, MAX_PATH);

	int Count = 0;
	int End = RV_Size;

	strcpy(mSelected_File, mbtext);

	while (Count < End)
	{
		bool test = strcmp(RV_FileName[Count].c_str(), mbtext);
		if (test == 0)
		{
			if (RV_File_Extension[Count] == Enums::Resource_File_Type_Texture)
			{
				Extension_Type = Enums::Resource_File_Type_Texture;
				EnableWindow(GetDlgItem(Resource_Dlg_hWnd, IDC_BT_VIEWFILE), true);
				return;
			}

			if (RV_File_Extension[Count] == Enums::Resource_File_Type_Material)
			{
				Extension_Type = Enums::Resource_File_Type_Material;
				EnableWindow(GetDlgItem(Resource_Dlg_hWnd, IDC_BT_VIEWFILE), true);
				return;
			}

			if (RV_File_Extension[Count] == Enums::Resource_File_Type_Overlay)
			{
				Extension_Type = Enums::Resource_File_Type_Overlay;
				EnableWindow(GetDlgItem(Resource_Dlg_hWnd, IDC_BT_VIEWFILE), true);
				return;
			}
		}

		Count++;
	}

	EnableWindow(GetDlgItem(Resource_Dlg_hWnd, IDC_BT_VIEWFILE), false);
}

// *************************************************************************
// *	Update_Resource_Groups_Combo:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Resources::Update_Resource_Groups_Combo(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_LST_GROUPS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

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
			SendDlgItemMessage(hDlg, IDC_LST_GROUPS, LB_ADDSTRING, (WPARAM)0, (LPARAM)sv[Count].c_str());
		}

		Count++;
	}

	SendDlgItemMessage(hDlg, IDC_LST_GROUPS, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)mSelected_Resource_Group.c_str());

}

// *************************************************************************
// *			Scan_Resource_Group:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
bool CL64_Resources::Scan_Resource_Group(Ogre::String ResourceGroup)
{
	RV_FileName.resize(0);
	RV_Archive_GetName.resize(0);
	RV_Archive_GetType.resize(0);
	RV_File_Extension.resize(0);

	char mFilename[MAX_PATH];

	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(ResourceGroup, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();
	int Count = 0;

	for (i = RFI->begin(); i != iend; ++i)
	{
		RV_FileName.push_back(i->filename);

		strcpy(mFilename, i->filename.c_str());
		Get_File_Extensions(mFilename, Count);

		RV_Archive_GetName.push_back(i->archive->getName());
		RV_Archive_GetType.push_back(i->archive->getType());

		Count++;
	}

	RV_Size = Count;

	return 1;
}

// *************************************************************************
// *	  Show_Resource_Group_Type:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
int CL64_Resources::Show_Resource_Group_Type(int mType)
{
	ListView_DeleteAllItems(FX_General_hLV);

	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	int	 pRow = 0;
	int Count = 0;
	int End = RV_Size;

	while (Count < End)
	{
		if (RV_File_Extension[Count] == mType)
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

	Set_Selection(0);

	RedrawWindow(FX_General_hLV, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	return pRow;
}

// *************************************************************************
// *			Set_Selection:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_Resources::Set_Selection(int Index)
{
	ListView_SetItemState(FX_General_hLV, Index, LVNI_SELECTED, LVNI_SELECTED);
	ListView_SetSelectionMark(FX_General_hLV, Index);
	ListView_EnsureVisible(FX_General_hLV, Index, false);

	ListView_GetItemText(FX_General_hLV, Index, 0, mbtext, MAX_PATH);

	strcpy(mSelected_File, mbtext);
	SetDlgItemText(Resource_Dlg_hWnd, IDC_ST_SELECTEDFILE, (LPCTSTR)App->CL_Resources->mSelected_File);
}

// *************************************************************************
// *		Get_File_Extensions:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
bool CL64_Resources::Get_File_Extensions(char* FileName, int Index)
{
	if (_stricmp(FileName + strlen(FileName) - 9, ".material") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Material);
		return 1;
	}

	if (_stricmp(FileName + strlen(FileName) - 5, ".mesh") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Mesh);
		return 1;
	}

	if (_stricmp(FileName + strlen(FileName) - 9, ".skeleton") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Skeleton);
		return 1;
	}

	if (_stricmp(FileName + strlen(FileName) - 8, ".overlay") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Overlay);
		return 1;
	}

	// Textures
	if (_stricmp(FileName + strlen(FileName) - 4, ".png") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Texture);
		return 1;
	}

	if (_stricmp(FileName + strlen(FileName) - 4, ".tga") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Texture);
		return 1;
	}

	if (_stricmp(FileName + strlen(FileName) - 4, ".bmp") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Texture);
		return 1;
	}

	if (_stricmp(FileName + strlen(FileName) - 4, ".jpg") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Texture);
		return 1;
	}

	if (_stricmp(FileName + strlen(FileName) - 4, ".dds") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Texture);
		return 1;
	}

	if (_stricmp(FileName + strlen(FileName) - 4, ".ico") == 0)
	{
		RV_File_Extension.push_back(Enums::Resource_File_Type_Texture);
		return 1;
	}

	RV_File_Extension.push_back(Enums::Resource_File_Type_None);

	return 0;
}

// *************************************************************************
// *	   Show_Resource_Group_All:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
int CL64_Resources::Show_Resource_Group_All()
{
	ListView_DeleteAllItems(FX_General_hLV);

	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

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

	Set_Selection(0);

	return Count;
}

// *************************************************************************
// *			Set_Title:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Resources::Set_Title(HWND hDlg, char* Title)
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
	flag_Show_All_Skeleton = 0;
	flag_Show_All_Overlays = 0;

	EnableWindow(GetDlgItem(Resource_Dlg_hWnd, IDC_BT_VIEWFILE), false);
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
// *				View_Texture:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Resources::View_Texture(char* FileName, HWND Owner_hDlg)
{
	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(mSelected_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	for (i = RFI->begin(); i != iend; ++i)
	{
		if (i->filename == FileName)
		{
			Ogre::DataStreamPtr ff = i->archive->open(i->filename);

			mFileString = ff->getAsString();

			char mFileName[MAX_PATH];
			strcpy(mFileName, App->RB_Directory_FullPath);
			strcat(mFileName, "\\Data\\");
			strcat(mFileName, FileName);

			std::ofstream outFile;
			outFile.open(mFileName, std::ios::binary);
			outFile << mFileString;
			outFile.close();

			mFileString.clear();

			App->CL_Dialogs->Start_TextureViewer_Dialog(mFileName, Owner_hDlg);

			return 1;
		}
	}

	return 1;
}

// *************************************************************************
// *		Load_Texture_Resources:- Terry and Hazel Flanigan 2024         *
// *************************************************************************
void CL64_Resources::Load_Texture_Resources()
{
	char WadFile[MAX_PATH];
	strcpy(WadFile, App->CL_Level->TXL_PathAndFile);
	
	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Ogre->Texture_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(WadFile, App->CL_Ogre->Texture_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->Texture_Resource_Group);
	}

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->Texture_Resource_Group);
	
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(WadFile, "Zip", App->CL_Ogre->Texture_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(App->CL_Ogre->Texture_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->Texture_Resource_Group);

	App->CL_TXL_Editor->flag_Textures_Scanned = 1;
}

// *************************************************************************
// *	Create_Project_Resources_Group:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
bool CL64_Resources::Create_Project_Resources_Group()
{
	// Check if the project resources have already been created
	if (App->CL_Scene->flag_Project_Resources_Created == false)
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Project_Resource_Group);
		App->CL_Scene->flag_Project_Resources_Created = true;
	}

	return true;
}

// *************************************************************************
// *	Delete_Project_Resources_Group:- Terry and Hazel Flanigan 2025 	   *
// *************************************************************************
bool CL64_Resources::Delete_Project_Resources_Group()
{
	// Check if the project resources have already been created
	if (App->CL_Scene->flag_Project_Resources_Created)
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Resources->Project_Resource_Group);
		App->CL_Scene->flag_Project_Resources_Created = false;
		App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";
	}

	return true;
}

// *************************************************************************
// *	Add_Resource_Location_Project:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
bool CL64_Resources::Add_Resource_Location_Project(char* Resource_Location)
{
	bool Test = Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(Resource_Location, Project_Resource_Group);

	if (Test == 0)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Resource_Location, "FileSystem", Project_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(Project_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(Project_Resource_Group);
	}

	// Set the selected resource group to the project resource group
	mSelected_Resource_Group = "Project_Resource_Group";

	App->CL_Scene->flag_Project_Resources_Created = true;

	return true;
}

