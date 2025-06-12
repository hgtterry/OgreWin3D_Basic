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
#include "CL64_FileView.h"

#define IDM_FILE_NEW 1
#define IDM_FILE_DELETE 2
#define IDM_FILE_RENAME 3
#define IDM_COPY 4
#define IDM_PASTE 5
#define IDM_GOTO 6

CL64_FileView::CL64_FileView()
{
	hImageList = nullptr;
	hBitMap = nullptr;

	Root = nullptr;
	GD_ProjectFolder = nullptr;
	FV_Objects_Folder = nullptr;
	FV_LevelFolder = nullptr;
	FV_Cameras_Folder = nullptr;
	GD_TriggerFolder = nullptr;
	FV_EntitiesFolder = nullptr;
	FV_Sounds_Folder = nullptr;
	FV_Message_Trigger_Folder = nullptr;
	FV_Move_Folder = nullptr;
	FV_Collectables_Folder = nullptr;
	FV_Teleporters_Folder = nullptr;
	FV_Evirons_Folder = nullptr;
	FV_Lights_Folder = nullptr;

	GD_Environment_Folder = nullptr;
	GD_Area_Change_Folder = nullptr;
	GD_Level_Change_Folder = nullptr;
	FV_Particles_Folder = nullptr;

	FV_Counters_Folder = nullptr;
	FV_Counters2_Folder = nullptr;
	FV_UserObjects_Folder = nullptr;

	FV_Players_Folder = nullptr;
	FV_Areas_Folder = nullptr;

	flag_FileView_Active = 0;

	FileView_Folder[0] = 0;
	FileView_File[0] = 0;

	tvinsert = { 0 };

	hMenu = 0;
	Context_Selection = Enums::FileView_None;
}

CL64_FileView::~CL64_FileView()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_FileView::Reset_Class()
{
	TreeView_DeleteAllItems(GetDlgItem(App->ListPanel, IDC_TREE1));

	Root = nullptr;
	GD_ProjectFolder = nullptr;
	FV_Objects_Folder = nullptr;
	FV_LevelFolder = nullptr;
	FV_Cameras_Folder = nullptr;
	GD_TriggerFolder = nullptr;
	FV_EntitiesFolder = nullptr;
	FV_Sounds_Folder = nullptr;
	FV_Message_Trigger_Folder = nullptr;
	FV_Move_Folder = nullptr;
	FV_Collectables_Folder = nullptr;
	FV_Teleporters_Folder = nullptr;
	GD_Environment_Folder = nullptr;
	GD_Area_Change_Folder = nullptr;
	GD_Level_Change_Folder = nullptr;
	FV_Particles_Folder = nullptr;
	FV_UserObjects_Folder = nullptr;


	FV_Players_Folder = nullptr;
	FV_Areas_Folder = nullptr;

	strcpy(App->CL_Project->m_Level_File_Name, "No Level");

	AddRootFolder();
	MoreFolders(); //  Folders under root 
	ExpandRoot();
}

// **************************************************************************
// *			Start_FileView:- Terry and Hazel Flanigan 2022				*
// **************************************************************************
void CL64_FileView::Start_FileView(void)
{
	App->ListPanel = CreateDialog(App->hInst, (LPCTSTR)IDD_LISTSCENE, App->MainHwnd, (DLGPROC)Proc_ListPanel);
}

// *************************************************************************
// *		Proc_ListPanel_Proc:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
LRESULT CALLBACK CL64_FileView::Proc_ListPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		App->CL_FileView->flag_FileView_Active = 1;

		SendDlgItemMessage(hDlg, IDC_TREE1, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MAINENVIRONMENT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
	}

	case WM_SIZE:
	{
		App->CL_Panels->Resize_FileView();

	}break;

	case WM_CONTEXTMENU:
	{
		App->CL_FileView->Context_Menu(hDlg);
		break;
	}

	case WM_NOTIFY:
	{
		LPNMHDR nmhdr = (LPNMHDR)lParam;
		if (nmhdr->idFrom == IDC_TREE1)
		{
			switch (nmhdr->code)
			{

			case TVN_SELCHANGED:
			{
				App->CL_FileView->Get_Selection((LPNMHDR)lParam);
			}

			}
		}

		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_MAINENVIRONMENT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_MAINENVIRONMENT));
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

		/*if (some_item->idFrom == IDC_STOCK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->SBC_FileView->Stock_But_Active);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_INFO_FILEVIEW && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return 1;
	}
	case WM_COMMAND:
	{
		//if (LOWORD(wParam) == IDM_GOTO)
		//{
		//	App->SBC_FileView->Context_Goto(hDlg);
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDM_COPY)
		//{
		//	App->SBC_FileView->Context_Copy(hDlg);
		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDM_FILE_DELETE)
		{
			App->CL_FileView->Context_Delete();
			return TRUE;
		}

		if (LOWORD(wParam) == IDM_FILE_NEW)
		{
			App->CL_FileView->Context_New(hDlg);
			return TRUE;
		}

		if (LOWORD(wParam) == IDM_FILE_RENAME)
		{
			App->CL_FileView->Context_Rename();
			return TRUE;
		}

		//if (LOWORD(wParam) == IDC_LEVELS)
		//{
		//	//App->SBC_FileView->Level_But_Active = 1;
		//	//App->SBC_FileView->Stock_But_Active = 0;
		//	//App->RedrawWindow_Dlg(hDlg);

		//	//ShowWindow(GetDlgItem(App->ListPanel, IDC_TREE1), 1);

		//	//App->GDCL_FileView->HideRightPanes();
		//	//ShowWindow(App->GD_Properties_Hwnd, 1);*/

		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_STOCK)
		//{
		//	if (App->SBC_Scene->Scene_Loaded == 1)
		//	{
		//		/*App->Cl_FileView->HideRightPanes();
		//		ShowWindow(App->GD_Stock_Hwnd, 1);*/
		//		//App->Cl_Stock->Start_Stock_Dialog();
		//	}
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_BT_INFO_FILEVIEW)
		//{
		//	App->Cl_Utilities->OpenHTML("Help\\FileView.html");
		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDC_BT_MAINENVIRONMENT)
		{
			int Index = App->CL_Com_Environments->Get_First_Environ();
			if (Index == -1)
			{
				App->Say("No Environment to Edit");
			}
			else
			{
				HTREEITEM Temp = App->CL_Scene->B_Object[Index]->FileViewItem;
				App->CL_FileView->SelectItem(Temp);

				App->CL_Gui_Environment->Start_Environment_Editor(Index, false);
			}

			return TRUE;
		}

		break;
	}

	case WM_CLOSE:
	{
		/*App->CL_FileView->flag_FileView_Active = 0;
		ShowWindow(App->ListPanel, 0);

		CheckMenuItem(App->mMenu, ID_WINDOWS_FILEVIEW, MF_BYCOMMAND | MF_UNCHECKED);*/

		return TRUE;
	}

	break;
	}
	return FALSE;
}

// *************************************************************************
// *			Init_FileView:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_FileView::Init_FileView(void)
{
	InitCommonControls();	    // make our tree control to work

	////====================================================//
	hImageList = ImageList_Create(16, 16, FALSE, 8, 0); // Zero Index

	//--------- Grayed Folder Closed Open 0 1
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILEINACTIVE));
	ImageList_Add(hImageList, hBitMap, NULL);
	DeleteObject(hBitMap);

	//--------- Green Folder Closed Open 2 3
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_TREE));
	ImageList_Add(hImageList, hBitMap, NULL);
	DeleteObject(hBitMap);

	//--------- Uselected File Open 4
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILE));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	//--------- Selected File Open 5
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILESELECTED));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	//--------- File changed Open 6
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILECHANGED));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	//--------- File changed Selected Open 7
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILECHANGEDSELECTED));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	//--------- File changed Selected Open 8 and 9
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FVFOLDERRED));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);

	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	TreeView_DeleteAllItems(Temp);

	TreeView_SetBkColor(Temp, (COLORREF)RGB(255, 255, 255));

	AddRootFolder();
	MoreFolders(); //  Folders under root 
	ExpandRoot();

}

// *************************************************************************
// *			AddRootFolder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_FileView::AddRootFolder(void)
{
	tvinsert.hParent = Root;			// top most level no need handle
	tvinsert.hInsertAfter = TVI_LAST; // work as root level
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)App->CL_Project->m_Project_Name;
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	GD_ProjectFolder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
}

// *************************************************************************
// *			MoreFoldersD:- Terry and Hazel Flanigan 2024 		 	   *
// *************************************************************************
void CL64_FileView::MoreFolders(void) // last folder level
{
	//------------------------------------------------------- Level 
	tvinsert.hParent = GD_ProjectFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)App->CL_Project->m_Level_Name;
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_LevelFolder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	////------------------------------------------------------- Camera
	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Camera";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Cameras_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Player";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Players_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Area";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Areas_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Objects";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Objects_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Entities";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_EntitiesFolder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Sounds
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Sounds";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Sounds_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Messages
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Messages";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Message_Trigger_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Move_Entities
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Move_Entities";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Move_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Collectables
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Collectables";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Collectables_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Teleporters
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Teleporters";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Teleporters_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Evironments Eviron_Entity
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Evironments";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Evirons_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Particles
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Particles";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Particles_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- User_Objects
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"User_Objects";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_UserObjects_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Lights
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Lights";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Lights_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Collectables
	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = (LPSTR)"Counters";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Counters_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
}

// *************************************************************************
// *			ExpandRoot:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_FileView::ExpandRoot(void)
{
	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	HTREEITEM i = TreeView_GetSelection(Temp);

	TreeView_Expand(Temp, GD_ProjectFolder, TVE_EXPAND);
	TreeView_Expand(Temp, FV_LevelFolder, TVE_EXPAND);
	TreeView_Expand(Temp, FV_EntitiesFolder, TVE_EXPAND);
}

// **************************************************************************
// *			Show_FileView:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_FileView::Show_FileView(bool Enable)
{
	if (Enable == 1)
	{
		App->CL_FileView->flag_FileView_Active = 1;
		ShowWindow(App->ListPanel, 1);
	}
	else
	{
		App->CL_FileView->flag_FileView_Active = 0;
		ShowWindow(App->ListPanel, 0);
	}
}

// *************************************************************************
// *		Get_Selection Terry:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_FileView::Get_Selection(LPNMHDR lParam)
{
	strcpy(FileView_Folder, "");
	strcpy(FileView_File, "");

	int Index = 0;
	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	HTREEITEM i = TreeView_GetSelection(Temp);

	TVITEM item;
	item.hItem = i;
	item.pszText = FileView_Folder;
	item.cchTextMax = sizeof(FileView_Folder);
	item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeView_GetItem(((LPNMHDR)lParam)->hwndFrom, &item);
	Index = item.lParam;

	HTREEITEM p = TreeView_GetParent(Temp, i);

	TVITEM item1;
	item1.hItem = p;
	item1.pszText = FileView_File;
	item1.cchTextMax = sizeof(FileView_File);
	item1.mask = TVIF_TEXT;
	TreeView_GetItem(((LPNMHDR)lParam)->hwndFrom, &item1);

	// ---------------------------------------------------- Player
	if (!strcmp(FileView_Folder, "Player")) // Folder
	{
		//Context_Selection = Enums::FileView_Player_Folder;
		return;
	}
	if (!strcmp(FileView_File, "Player"))
	{
		//Context_Selection = Enums::FileView_Player_File;

		HideRightPanes();
		//App->CL_Props_Dialogs->Show_Player_Dlg(true);
		//----------------------------------------------------------------------------

		//App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		//App->SBC_Properties->Last_Selected_Object = Index;
		//----------------------------------------------------------------------------
		App->CL_Gizmos->Show_MarkerBox(false);

		App->CL_Properties_Scene->Current_Selected_Object = Index;

		App->CL_Properties_Scene->Edit_Category = Enums::Edit_Player;
		//App->CL_LookUps->Update_Types();

		//ShowWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, 1);
		App->CL_Properties_Scene->Update_ListView_Player();

		return;
	}

	// ---- Areas
	//if (!strcmp(FileView_Folder, "Area")) // Folder
	//{
	//	Context_Selection = Enums::FileView_Areas_Folder;

	//	return;
	//}

	//if (!strcmp(FileView_File, "Area"))
	//{
	//	Context_Selection = Enums::FileView_Areas_File;

	//	HideRightPanes();
	//	App->CL_Props_Dialogs->Show_Details_Goto_Dlg(true);

	//	App->CL_Props_Dialogs->Hide_Debug_Dlg(1);
	//	App->CL_Props_Dialogs->Show_Dimensions_Dlg(1);
	//	//App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
	//	//App->CL_Props_Dialogs->Show_Materials_Dlg(true);

	//	//----------------------------------------------------------------------------
	//	//App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
	//	//App->SBC_Properties->Last_Selected_Object = Index;
	//	//----------------------------------------------------------------------------

	//	App->CL_Properties->Current_Selected_Object = Index;
	//	App->CL_Properties->Edit_Category = Enums::Edit_Area;
	//	App->CL_LookUps->Update_Types();

	//	ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
	//	App->CL_Properties->Update_ListView_Area();


	//	/*if (App->SBC_Dimensions->Show_Dimensions == 1)
	//	{
	//		App->SBC_Dimensions->Prepare_Dimensions();
	//	}*/

	//	return;
	//}
	
	// ------------------------- Objects
	if (!strcmp(FileView_Folder, "Objects")) // Clicked Folder
	{
		Context_Selection = Enums::FileView_Objects_Folder;
		return;
	}
	if (!strcmp(FileView_File, "Objects")) // Clicked File
	{
		Context_Selection = Enums::FileView_Objects_File;

		HideRightPanes();
		App->CL_Props_Dialogs->Show_Details_Goto_Dlg(true);

		App->CL_Props_Dialogs->Show_Dimensions_Dlg(true);
		App->CL_Props_Dialogs->Show_Debug_Dlg(true);
		App->CL_Props_Dialogs->Show_Materials_Dlg(true);

		//----------------------------------------------------------------------------
		App->CL_Gizmos->unhighlight(App->CL_Scene->B_Object[App->CL_Properties_Scene->Last_Selected_Object]->Object_Ent);
		App->CL_Properties_Scene->Last_Selected_Object = Index;
		App->CL_Gizmos->Last_Selected_Object = Index;
		//----------------------------------------------------------------------------

		App->CL_Gizmos->MarkerBox_Adjust(Index);
		
		App->CL_Properties_Scene->Current_Selected_Object = Index;
		App->CL_Properties_Scene->Edit_Category = Enums::Edit_Object;

		Select_Brush(Index);
				
		if (App->CL_Editor_Control->flag_Scene_Editor_Active == true)
		{
			ShowWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, true);
		}

		App->CL_Properties_Scene->Update_ListView_Objects();

		App->CL_Gizmos->highlight(App->CL_Scene->B_Object[Index]->Object_Ent);
		return;
	}

	// ------------------------- Eviron_Entities
	if (!strcmp(FileView_Folder, "Evironments")) // Folder
	{
		Context_Selection = Enums::FileView_EnvironEntity_Folder;
		return;
	}
	if (!strcmp(FileView_File, "Evironments"))
	{
		Context_Selection = Enums::FileView_EnvironEntity_File;

		HideRightPanes();
		//App->CL_Props_Dialogs->Show_Details_Goto_Dlg(true);

		App->CL_Props_Dialogs->Show_Dimensions_Dlg(true);
		App->CL_Props_Dialogs->Show_Debug_Dlg(true);

		//---------------------------------------------------------------------------
		App->CL_Gizmos->unhighlight(App->CL_Scene->B_Object[App->CL_Properties_Scene->Last_Selected_Object]->Object_Ent);
		App->CL_Properties_Scene->Last_Selected_Object = Index;
		App->CL_Gizmos->Last_Selected_Object = Index;		
		//---------------------------------------------------------------------------

		App->CL_Gizmos->MarkerBox_Adjust(Index);

		App->CL_Properties_Scene->Current_Selected_Object = Index;
		App->CL_Properties_Scene->Edit_Category = Enums::Edit_Environs;

		//-----------------------------
		if (App->CL_File->flag_loading == false)
		{
			Brush* pMinBrush = App->CL_Brush_X->Get_Brush_By_Name(App->CL_Scene->B_Object[App->CL_Properties_Scene->Last_Selected_Object]->Object_Name);

			// TODO Check App->CL_Editor_Scene->flag_Environment_Available == true
			if (pMinBrush)// && App->CL_Editor_Scene->flag_Environment_Available == true)
			{
				App->CL_Doc->ResetAllSelections();
				App->CL_Doc->DoBrushSelection(pMinBrush, brushSelToggle);
				App->CL_Brush_X->Select_Brush_Editor(pMinBrush);

				App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 1;
				App->CL_Properties_Tabs->Select_Brushes_Tab();
				App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 0;
			}
		}
		//-----------------------------

		if (App->CL_Editor_Control->flag_Scene_Editor_Active == true)
		{
			//ShowWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, 1);
		}

		App->CL_Properties_Scene->Update_ListView_Environs();

		App->CL_Gizmos->highlight(App->CL_Scene->B_Object[Index]->Object_Ent);

		return;
	}

	// ------------------------- Sound Entity
	if (!strcmp(FileView_Folder, "Sounds")) // Folder
	{
		Context_Selection = Enums::FileView_Sounds_Folder;
		return;
	}

	if (!strcmp(FileView_File, "Sounds"))
	{
		Context_Selection = Enums::FileView_Sounds_File;

		HideRightPanes();
		App->CL_Props_Dialogs->Show_Details_Goto_Dlg(true);
		App->CL_Props_Dialogs->Show_Dimensions_Dlg(1);
		//App->CL_Props_Dialogs->Hide_Debug_Dlg(1);

		//App->SBC_Properties->Is_Player = 0;*/

		//---------------------------------------------------------------------------
		App->CL_Gizmos->unhighlight(App->CL_Scene->B_Object[App->CL_Properties_Scene->Last_Selected_Object]->Object_Ent);
		App->CL_Properties_Scene->Last_Selected_Object = Index;
		App->CL_Gizmos->Last_Selected_Object = Index;
		//---------------------------------------------------------------------------

		App->CL_Gizmos->MarkerBox_Adjust(Index);

		App->CL_Properties_Scene->Current_Selected_Object = Index;
		App->CL_Properties_Scene->Edit_Category = Enums::Edit_Sounds;

		Select_Brush(Index);
		
		//App->CL_LookUps->Update_Types();

		//ShowWindow(App->CL_Properties_Scene->Properties_Dlg_hWnd, 1);

		App->CL_Properties_Scene->Update_ListView_Sounds();
		App->CL_Gizmos->highlight(App->CL_Scene->B_Object[Index]->Object_Ent);
		
		return;
	}

}

// *************************************************************************
// *			Select_Brush:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_FileView::Select_Brush(int index)
{
	if (App->CL_File->flag_loading == false)
	{
		Brush* pMinBrush = App->CL_Brush_X->Get_Brush_By_Name(App->CL_Scene->B_Object[index]->Object_Name);

		if (pMinBrush)
		{
			int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
			if (NumSelBrushes > 0)
			{
				App->CL_Doc->ResetAllSelections();
			}
		
			App->CL_Doc->DoBrushSelection(pMinBrush, brushSelToggle);


			App->CL_Brush_X->Select_Brush_Editor(pMinBrush);

			App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 1;
			App->CL_Properties_Tabs->Select_Brushes_Tab();
			App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 0;
			
		}
	}
}

// *************************************************************************
// *			Context_Menu:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_FileView::Context_Menu(HWND hDlg)
{
	RECT rcTree;
	HWND hwndTV;
	HTREEITEM htvItem;
	TVHITTESTINFO htInfo = { 0 };
	POINT pt;
	GetCursorPos(&pt);

	long xPos = pt.x;   // x position from message, in screen coordinates
	long yPos = pt.y;   // y position from message, in screen coordinates 

	hwndTV = GetDlgItem(hDlg, IDC_TREE1);         // get the tree view 
	GetWindowRect(hwndTV, &rcTree);              // get its window coordinates
	htInfo.pt.x = xPos - rcTree.left;              // convert to client coordinates
	htInfo.pt.y = yPos - rcTree.top;

	if (htvItem = TreeView_HitTest(hwndTV, &htInfo)) 
	{    // hit test
		TreeView_SelectItem(hwndTV, htvItem);           // success; select the item

		//---- Player
		if (!strcmp(FileView_Folder, "Player")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Player_Folder;
		}

		if (!strcmp(FileView_File, "Player"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Player_File;
		}

		//------------------------------------- Camera
		if (!strcmp(FileView_Folder, "Camera")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Cameras_Folder;
		}

		if (!strcmp(FileView_File, "Camera"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Cameras_File;
		}

		//------------------------------------- Objects
		if (!strcmp(FileView_Folder, "Objects")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Objects_Folder;
		}

		if (!strcmp(FileView_File, "Objects"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Create Copy");

			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Objects_File;
		}

		//------------------------------------- Messages
		if (!strcmp(FileView_Folder, "Messages")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Messages_Folder;
		}

		if (!strcmp(FileView_File, "Messages"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Create Copy");
			//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Messages_File;
		}

		//------------------------------------- Sounds
		if (!strcmp(FileView_Folder, "Sounds")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Sounds_Folder;
		}

		if (!strcmp(FileView_File, "Sounds"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Create Copy");
			//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Sounds_File;
		}

		//------------------------------------- Eviron_Entities
		if (!strcmp(FileView_Folder, "Evironments")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_EnvironEntity_Folder;
		}

		if (!strcmp(FileView_File, "Evironments"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_EnvironEntity_File;
		}

		//------------------------------------- Areas
		if (!strcmp(FileView_Folder, "Area")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Areas_Folder;
		}

		if (!strcmp(FileView_File, "Area"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Areas_File;
		}

		//------------------------------------- Move_Entities
		if (!strcmp(FileView_Folder, "Move_Entities")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Move_Folder;
		}

		if (!strcmp(FileView_File, "Move_Entities"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Create Copy");
			//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Move_File;
		}

		//------------------------------------- Teleport_Entities
		if (!strcmp(FileView_Folder, "Teleporters")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Teleports_Folder;
		}

		if (!strcmp(FileView_File, "Teleporters"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Create Copy");
			//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Teleports_File;
		}

		//------------------------------------- Collectables_Entities
		if (!strcmp(FileView_Folder, "Collectables")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Collectables_Folder;
		}

		if (!strcmp(FileView_File, "Collectables"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Create Copy");
			//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Collectables_File;
		}

		//------------------------------------- Counters
		if (!strcmp(FileView_Folder, "Counters")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Counters_Folder;
		}

		if (!strcmp(FileView_File, "Counters"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Counters_File;
		}

		//------------------------------------- Text_Messages
		if (!strcmp(FileView_Folder, "Text_Message")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_TextMessage_Folder;
		}

		if (!strcmp(FileView_File, "Text_Message"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_TextMessage_File;
		}

		//------------------------------------- Particles
		if (!strcmp(FileView_Folder, "Particles")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Particle_Folder;
		}

		if (!strcmp(FileView_File, "Particles"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Create Copy");
			//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Particle_File;
		}

	}
}

// *************************************************************************
// *				Context_New:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_FileView::Context_New(HWND hDlg)
{
	// Objects
	if (App->CL_FileView->Context_Selection == Enums::FileView_Objects_Folder)
	{
		if (App->CL_Scene->flag_Project_Resources_Created == true)
		{
			App->CL_Dialogs->YesNo((LPSTR)"Add Object", (LPSTR)"Do you want to add a new Object Entity");

			bool Doit = App->CL_Dialogs->flag_Dlg_Canceled;
			if (Doit == 0)
			{
				App->CL_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Objects;
				App->CL_MeshViewer->Start_MeshViewer_Dlg();
			}
		}
		else
		{
			if (App->CL_Level->flag_File_Been_Saved == 0)
			{
				App->Say("This Project is new", "Will use Save As for this first time save");
				App->CL_File->Start_Save(true);

				if (App->CL_Scene->flag_Project_Resources_Created == true)
				{
					App->CL_Dialogs->YesNo((LPSTR)"Add Object", (LPSTR)"Do you want to add a new Object Entity");

					bool Doit = App->CL_Dialogs->flag_Dlg_Canceled;
					if (Doit == 0)
					{
						App->CL_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Objects;
						App->CL_MeshViewer->Start_MeshViewer_Dlg();
					}
				}

				return ;
			}

		}

		return;
	}

	// Environment
	if (App->CL_FileView->Context_Selection == Enums::FileView_EnvironEntity_Folder)
	{
		App->CL_Dialogs->YesNo((LPSTR)"Add Environment Entity", (LPSTR)"Do you want to add a new Environ Entity");

		bool Doit = App->CL_Dialogs->flag_Dlg_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Environments->Add_New_Environ_Entity(false);
		}

		return;
	}

	// Sounds
	if (App->CL_FileView->Context_Selection == Enums::FileView_Sounds_Folder)
	{
		App->CL_Dialogs->YesNo((LPSTR)"Add Sound Entity", (LPSTR)"Do you want to add a new Sound Entity");

		bool Doit = App->CL_Dialogs->flag_Dlg_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Sounds->Add_New_Sound();
		}

		return;
	}

	return;
}

// *************************************************************************
// *				Context_Rename:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_FileView::Context_Rename() const
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	// Camera
	if (Context_Selection == Enums::FileView_Cameras_File)
	{
		//App->CL_Com_Cameras->Rename_Camera(Index);
		//App->CL_Properties->Update_ListView_Camera();
		return;
	}

	// Player
	if (Context_Selection == Enums::FileView_Player_File)
	{
		//App->CL_Com_Player->Rename_Player(Index);
		//App->CL_Properties->Update_ListView_Player();
		return;
	}

	// Aera
	if (Context_Selection == Enums::FileView_Areas_File)
	{
		//App->CL_Com_Area->Rename_Area(Index);
		//App->CL_Properties->Update_ListView_Area();
		return;
	}

	// Sound Entity
	if (Context_Selection == Enums::FileView_Sounds_File)
	{
		//App->CL_Com_Sounds->Rename_Sound(Index);
		//App->CL_Properties->Update_ListView_Sounds();
		return;
	}

	// Message Entity
	if (Context_Selection == Enums::FileView_Messages_File)
	{
		//App->CL_Com_Messages->Rename_Message_Entity(Index);
		//App->CL_Properties->Update_ListView_Messages();
		return;
	}

	// Move Entity
	if (Context_Selection == Enums::FileView_Move_File)
	{
		//App->CL_Com_MoveEntity->Rename_Move_Entity(Index);
		//App->CL_Properties->Update_ListView_Move_Entities();
		return;
	}

	// Collectable Entity
	if (Context_Selection == Enums::FileView_Collectables_File)
	{
		//App->CL_Com_Collectables->Rename_Collectable(Index);
		//App->CL_Properties->Update_ListView_Collectables();
		return;
	}

	// Teleport Entity
	if (Context_Selection == Enums::FileView_Teleports_File)
	{
		//App->CL_Com_Teleporters->Rename_Teleport_Entity(Index);
		//App->CL_Properties->Update_ListView_Teleport();
		return;
	}

	// Environment Entity
	if (Context_Selection == Enums::FileView_EnvironEntity_File)
	{
		//App->CL_Com_Environments->Rename_Environ_Entity(Index);
		//App->CL_Properties->Update_ListView_Environs();
		return;
	}

	// Particle Entity
	if (Context_Selection == Enums::FileView_Particle_File)
	{
		//App->CL_Com_Particles->Rename_Particle_Entity(Index);
		//App->CL_Properties->Update_ListView_Particles();
		return;
	}

	// Counter Entity
	if (Context_Selection == Enums::FileView_Counters_File)
	{
		//App->CL_Com_Counters->Rename_Counter(Index);
		//App->CL_Properties->Update_ListView_Counters();
		return;
	}

	App->CL_Entities->Rename_Object(Index);
	App->CL_Properties_Scene->Update_ListView_Objects();

}

// *************************************************************************
// *			Context_Delete:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_FileView::Context_Delete()
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	// ---------------- Areas
	if (Context_Selection == Enums::FileView_Areas_File)
	{

		/*if (App->CL_Scene->B_Area[Index]->This_Object_UniqueID == 0)
		{
			App->Say("This Area can not be Deleted");
			return;
		}

		App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Area", (LPSTR)App->CL_Scene->B_Area[Index]->Area_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Area->Delete_Area();
			App->CL_FileView->Mark_Altered_Folder(FV_Areas_Folder);
		}*/

		return;
	}

	// ---------------- Cameras
	if (Context_Selection == Enums::FileView_Cameras_File)
	{
		/*if (App->CL_Scene->Camera_Count == 1)
		{
			App->Say("This Camera can not be Deleted");
		}*/

		return;
	}

	// ---------------- Players
	if (Context_Selection == Enums::FileView_Player_File)
	{
		/*if (App->CL_Scene->Player_Count == 1)
		{
			App->Say("This Player can not be Deleted");
		}*/

		return;
	}

	// ---------------- Objects
	if (Context_Selection == Enums::FileView_Objects_File)
	{
		App->CL_Dialogs->YesNo((LPSTR)"Remove Object", (LPSTR)App->CL_Scene->B_Object[Index]->Object_Name);

		bool Doit = App->CL_Dialogs->flag_Dlg_Canceled;
		if (Doit == 0)
		{
			App->CL_Entities->Delete_Object();
			App->CL_FileView->Mark_Altered_Folder(FV_Objects_Folder);
		}

		return;
	}

	// ---------------- Message Triggers
	if (Context_Selection == Enums::FileView_Messages_File)
	{
		/*App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Message Entity", (LPSTR)App->CL_Scene->B_Object[Index]->Object_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Objects->Delete_Object();
			App->CL_FileView->Mark_Altered_Folder(FV_Message_Trigger_Folder);
		}*/

		return;
	}

	// ---------------- Sound Entities
	if (Context_Selection == Enums::FileView_Sounds_File)
	{
		/*App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Sound Entity", (LPSTR)App->CL_Scene->B_Object[Index]->Object_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Objects->Delete_Object();
			App->CL_FileView->Mark_Altered_Folder(FV_Sounds_Folder);
		}*/

		return;
	}

	// ---------------- Move Entities
	if (Context_Selection == Enums::FileView_Move_File)
	{
		/*App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Move Entity", (LPSTR)App->CL_Scene->B_Object[Index]->Object_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Objects->Delete_Object();
			App->CL_FileView->Mark_Altered_Folder(FV_Move_Folder);
		}*/

		return;
	}

	// ---------------- Counters
	if (Context_Selection == Enums::FileView_Counters_File)
	{
		/*if (App->CL_Scene->B_Counter[App->CL_Properties->Current_Selected_Object]->Unique_ID == 0)
		{
			App->Say("This Counter can not be Deleted");
			return;
		}

		App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Counter Entity", (LPSTR)App->CL_Scene->B_Counter[Index]->Panel_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Counters->Delete_Counter();
			App->CL_FileView->Mark_Altered_Folder(App->CL_FileView->FV_Counters_Folder);
		}*/

		return;
	}

	// ---------------- Environments
	if (Context_Selection == Enums::FileView_EnvironEntity_File)
	{
		int Test = App->CL_Com_Environments->Get_First_Environ();

		/*if (App->CL_Properties->Current_Selected_Object == Test)
		{
			App->Say("This Environment Entity can not be Deleted");
			return;
		}

		App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Environment Entity", (LPSTR)App->CL_Scene->B_Object[Index]->Object_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Objects->Delete_Object();
			App->CL_FileView->Mark_Altered_Folder(App->CL_FileView->FV_Evirons_Folder);
		}*/

		return;
	}

	// ---------------- Collectables
	if (App->CL_FileView->Context_Selection == Enums::FileView_Collectables_File)
	{
		/*App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Collectable Entity", (LPSTR)App->CL_Scene->B_Object[Index]->Object_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Objects->Delete_Object();
			App->CL_FileView->Mark_Altered_Folder(App->CL_FileView->FV_Collectables_Folder);
		}*/

		return;
	}

	// ---------------- Teleporters
	if (App->CL_FileView->Context_Selection == Enums::FileView_Teleports_File)
	{
		/*App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Teleport Entity", (LPSTR)App->CL_Scene->B_Object[Index]->Object_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Objects->Delete_Object();
			App->CL_FileView->Mark_Altered_Folder(App->CL_FileView->FV_Teleporters_Folder);
		}*/

		return;
	}

	// ---------------- Particles
	if (App->CL_FileView->Context_Selection == Enums::FileView_Particle_File)
	{
		/*App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Remove Particle Entity", (LPSTR)App->CL_Scene->B_Object[Index]->Object_Name, (LPSTR)"Are you sure");

		bool Doit = App->CL_Dialogs->flag_Canceled;
		if (Doit == 0)
		{
			App->CL_Com_Objects->Delete_Object();
			App->CL_FileView->Mark_Altered_Folder(App->CL_FileView->FV_Particles_Folder);
		}*/

		return;
	}

	return;
}

// *************************************************************************
// *			Redraw_FileView:- Terry and Hazel Flanigan 2024	 		   *
// *************************************************************************
void CL64_FileView::Redraw_FileView()
{
	//App->RedrawWindow_Dlg(GetDlgItem(App->ListPanel, IDC_TREE1));
}

// *************************************************************************
// *			Change_Level_Name:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_FileView::Change_Level_Name(void)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = FV_LevelFolder;
	Sitem.pszText = App->CL_Project->m_Level_Name;
	Sitem.iImage = 3;
	Sitem.iSelectedImage = 3;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);
}

// *************************************************************************
// *		Change_Project_Name:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_FileView::Change_Project_Name(void)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = GD_ProjectFolder;
	Sitem.pszText = (LPSTR)App->CL_Project->m_Project_Name;
	Sitem.iImage = 3;
	Sitem.iSelectedImage = 3;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);

}

// *************************************************************************
// *				Add_Item:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
HTREEITEM CL64_FileView::Add_Item(HTREEITEM Folder, char* SFileName, int Index, bool NewItem)
{
	HWND Temp2 = GetDlgItem(App->ListPanel, IDC_TREE1);

	tvinsert.hParent = Folder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.pszText = SFileName;

	if (NewItem == 1)
	{
		tvinsert.item.iImage = 6;
		tvinsert.item.iSelectedImage = 7;
		tvinsert.item.lParam = Index;
	}
	else
	{
		tvinsert.item.iImage = 4;
		tvinsert.item.iSelectedImage = 5;
		tvinsert.item.lParam = Index;
	}

	HTREEITEM Temp = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	return Temp;
}

// *************************************************************************
// *				DeleteItem:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
void CL64_FileView::DeleteItem()
{
	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	HTREEITEM i = TreeView_GetSelection(Temp);
	TreeView_DeleteItem(Temp, i);
}

// *************************************************************************
// *		Change_Item_Name:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_FileView::Change_Item_Name(HTREEITEM Folder, char* FolderName)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_TEXT;
	Sitem.hItem = Folder;
	Sitem.pszText = FolderName;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);
}

// *************************************************************************
// *			Set_FolderActive:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_FileView::Set_FolderActive(HTREEITEM Folder)
{
	TVITEM Sitem;

	Sitem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = Folder;
	Sitem.iImage = 3;
	Sitem.iSelectedImage = 3;

	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);
}

// *************************************************************************
// *				SelectItem:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_FileView::SelectItem(HTREEITEM TreeItem)
{
	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	TreeView_Select(Temp, TreeItem, TVGN_CARET);
}

// *************************************************************************
// *	  Enable_Environment_Button:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_FileView::Enable_Environment_Button(bool Enable)
{
	EnableWindow(GetDlgItem(App->ListPanel, IDC_BT_MAINENVIRONMENT), Enable);
}

// *************************************************************************
// *			Mark_Altered:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_FileView::Mark_Altered(HTREEITEM Item)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = Item;
	Sitem.iImage = 6;
	Sitem.iSelectedImage = 7;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);

	//EnableMenuItem(App->mMenu, ID_FILE_SAVEPROJECTALL, MF_ENABLED);
}

// *************************************************************************
// *		Mark_Altered_Folder:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_FileView::Mark_Altered_Folder(HTREEITEM Item)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = Item;
	Sitem.iImage = 8;
	Sitem.iSelectedImage = 9;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);

	//EnableMenuItem(App->mMenu, ID_FILE_SAVEPROJECTALL, MF_ENABLED);
}

// *************************************************************************
// *			HideRightPanesTerry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_FileView::HideRightPanes(void)
{
	App->CL_Props_Dialogs->Show_Details_Goto_Dlg(false);
	App->CL_Props_Dialogs->Show_Dimensions_Dlg(false);
	App->CL_Props_Dialogs->Show_Physics_Test_Dlg(false);
	App->CL_Props_Dialogs->Show_Debug_Dlg(false);
	App->CL_Props_Dialogs->Show_Materials_Dlg(false);
	App->CL_Props_Dialogs->Show_Cameras_Dlg(false);
	App->CL_Props_Dialogs->Show_Player_Dlg(false);
	App->CL_Props_Dialogs->Show_Override_Counter_Dlg(false);
}
