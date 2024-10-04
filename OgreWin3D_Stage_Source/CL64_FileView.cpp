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
#include "CL64_FileView.h"

CL64_FileView::CL64_FileView(void)
{
	Flag_FileView_Active = 0;

	FileView_Folder[0] = 0;
	FileView_File[0] = 0;

	Root =				nullptr;
	GD_ProjectFolder =	nullptr;
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

	FV_Players_Folder = nullptr;
	FV_Areas_Folder = nullptr;

	hImageList = nullptr;
	hBitMap = nullptr;
}

CL64_FileView::~CL64_FileView(void)
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
	//Init_Bmps_FileView();
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
		App->CL_FileView->Flag_FileView_Active = 1;

		SendDlgItemMessage(hDlg, IDC_TREE1, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MAINENVIRONMENT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		CheckMenuItem(App->mMenu, ID_WINDOWS_FILEVIEW, MF_BYCOMMAND | MF_CHECKED);
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_SIZE:
	{
		App->CL_Panels->Resize_FileView();

	}break;

	case WM_CONTEXTMENU:
	{
		//App->SBC_FileView->Context_Menu(hDlg);
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
			App->Custom_Button_Normal(item);
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

		//if (LOWORD(wParam) == IDM_FILE_DELETE)
		//{
		//	App->SBC_FileView->Context_Delete(hDlg);

		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDM_FILE_NEW)
		//{
		//	App->SBC_FileView->Context_New(hDlg);
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDM_FILE_RENAME)
		//{
		//	App->SBC_FileView->Context_Rename(hDlg);
		//	return TRUE;
		//}

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
				HTREEITEM Temp = App->CL_Scene->V_Object[Index]->FileViewItem;
				App->CL_FileView->SelectItem(Temp);

				App->CL_Gui_Environment->Start_Environment_Editor(Index, false);
			}

			return TRUE;
		}
		
		break;
	}

	case WM_CLOSE:
	{
		App->CL_FileView->Flag_FileView_Active = 0;
		ShowWindow(App->ListPanel, 0);

		CheckMenuItem(App->mMenu, ID_WINDOWS_FILEVIEW, MF_BYCOMMAND | MF_UNCHECKED);

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


	//--------------------------------------------------------------------------

	//if (!strcmp(FileView_Folder, App->SBC_Project->m_Level_Name)) // Level Folder
	//{
	//	HideRightPanes();
	//	ShowWindow(App->GD_Properties_Hwnd, 1);

	//	App->SBC_Properties->Edit_Category = Enums::FV_Edit_Level;
	//	App->SBC_Properties->Update_ListView_Level();
	//}

	// ---- Areas
	if (!strcmp(FileView_Folder, "Area")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Areas_Folder;

		return;
	}

	if (!strcmp(FileView_File, "Area"))
	{
		//App->Say(FileView_Folder);
		//App->SBC_FileView->Context_Selection = Enums::FileView_Areas_File;
		
		//HideRightPanes();
		
		/*App->SBC_Props_Dialog->Hide_Area_Dlg(1);
		App->SBC_Props_Dialog->Hide_Debug_Dlg(1);
		App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, 1);
		App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		App->SBC_Props_Dialog->Hide_Material_Dlg(1);*/

		//----------------------------------------------------------------------------
		//App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		//App->SBC_Properties->Last_Selected_Object = Index;
		//----------------------------------------------------------------------------
		
		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Area;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Area();


		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/

		return;
	}

	// ---- Player
	if (!strcmp(FileView_Folder, "Player")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Player_Folder;
		return;
	}
	if (!strcmp(FileView_File, "Player"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Player_File;

		//HideRightPanes();
		
		//App->SBC_Player->Hide_Player_Dlg(1);

		//----------------------------------------------------------------------------
		
		//App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		//App->SBC_Properties->Last_Selected_Object = Index;
		//----------------------------------------------------------------------------

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Player;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Player();
		
		return;
	}

	// ---- Objects
	if (!strcmp(FileView_Folder, "Objects")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Objects_Folder;
		return;
	}
	if (!strcmp(FileView_File, "Objects"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Objects_File;

		//HideRightPanes();
		

		/*App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);
		App->SBC_Props_Dialog->Hide_Debug_Dlg(1);
		App->SBC_Props_Dialog->Hide_Material_Dlg(1);*/


		

		//----------------------------------------------------------------------------
		/*App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		App->SBC_Properties->Last_Selected_Object = Index;*/
		//----------------------------------------------------------------------------
		/*App->SBC_Markers->MarkerBB_Addjust(Index);*/

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Object;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Objects();


		//if (App->SBC_Dimensions->Show_Dimensions == 1)
		//{
		//	App->SBC_Dimensions->Prepare_Dimensions();
		//}

		return;
	}

	// ------------------------------------------------------------ Collectables
	if (!strcmp(FileView_Folder, "Collectables")) // Folder
	{

		//App->SBC_FileView->Context_Selection = Enums::FileView_Collectables_Folder;

		return;
	}
	if (!strcmp(FileView_File, "Collectables"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Collectables_File;

		//HideRightPanes();
		
		//App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);

		//App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);
		//App->SBC_Props_Dialog->Hide_Debug_Dlg(1);
		//App->SBC_Props_Dialog->Hide_Material_Dlg(1);

		//----------------------------------------------------------------------------
		//App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		//App->SBC_Properties->Last_Selected_Object = Index;
		//----------------------------------------------------------------------------

		//App->SBC_Markers->MarkerBB_Addjust(Index);

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Collectable;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Collectables();

		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/

		return;
	}

	// ------------------------------------------------------------ Sounds
	if (!strcmp(FileView_Folder, "Sounds")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Sounds_Folder;
		return;
	}
	if (!strcmp(FileView_File, "Sounds"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Sounds_File;

		//HideRightPanes();
		/*App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);
		App->SBC_Props_Dialog->Hide_Debug_Dlg(1);

		App->SBC_Properties->Is_Player = 0;*/
		
		//----------------------------------------------------------------------------
		/*App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		App->SBC_Properties->Last_Selected_Object = Index;*/
		//----------------------------------------------------------------------------

		//App->SBC_Markers->MarkerBB_Addjust(Index);

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Sounds;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Sounds();

		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/

		return;
	}

	// ------------------------------------------------------------ Move Entity
	if (!strcmp(FileView_Folder, "Move_Entities")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Move_Folder;

		return;
	}
	if (!strcmp(FileView_File, "Move_Entities"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Move_File;

		//HideRightPanes();
		/*App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);
		App->SBC_Props_Dialog->Hide_Physics_Reset_Dlg(1);
		App->SBC_Props_Dialog->Hide_Debug_Dlg(1);*/

		//App->SBC_Properties->Is_Player = 0; // Mark as Object selected

		//----------------------------------------------------------------------------
		/*App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		App->SBC_Properties->Last_Selected_Object = Index;*/
		//----------------------------------------------------------------------------

		//App->SBC_Markers->MarkerBB_Addjust(Index);

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Move_Entity;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Move_Entities();

		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/

		return;
	}

	// ------------------------------------------------------------ Message_Triggers
	if (!strcmp(FileView_Folder, "Messages")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Messages_Triggers_Folder;
		return;
	}

	if (!strcmp(FileView_File, "Messages"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Messages_Triggers_File;

		//HideRightPanes();
		//App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		//App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);
		//App->SBC_Props_Dialog->Hide_Debug_Dlg(1);
		//App->SBC_Props_Dialog->Hide_Panel_Test_Dlg(1);

		//App->SBC_Properties->Is_Player = 0; // Mark as Object selected
		

		//----------------------------------------------------------------------------
		/*App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		App->SBC_Properties->Last_Selected_Object = Index;*/
		//----------------------------------------------------------------------------

		//App->SBC_Markers->MarkerBB_Addjust(Index);

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Message;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Messages();

		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/

		return;
	}

	// ------------------------------------------------------------ Teleporter Entity
	if (!strcmp(FileView_Folder, "Teleporters")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Teleports_Folder;

		return;
	}
	if (!strcmp(FileView_File, "Teleporters"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Teleports_File;

		//HideRightPanes();
		//App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		//App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);
		//App->SBC_Props_Dialog->Hide_Debug_Dlg(1);

		//App->SBC_Properties->Is_Player = 0; // Mark as Object selected

		//----------------------------------------------------------------------------
		/*App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		App->SBC_Properties->Last_Selected_Object = Index;*/
		//----------------------------------------------------------------------------

		//App->SBC_Markers->MarkerBB_Addjust(Index);

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Teleport;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Teleport();

		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/
		return;
	}

	// ------------------------------------------------------------ Particles
	if (!strcmp(FileView_Folder, "Particles")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Particle_Folder;

		return;
	}

	if (!strcmp(FileView_File, "Particles"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Particle_File;

		//HideRightPanes();

		//App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		//App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);

		//----------------------------------------------------------------------------
		//App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		//App->SBC_Properties->Last_Selected_Object = Index;
		//----------------------------------------------------------------------------

		//App->SBC_Markers->MarkerBB_Addjust(Index);

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Particles;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Particles();

		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/

		return;
	}

	// ------------------------------------------------------------ Counters
	if (!strcmp(FileView_Folder, "Counters")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Counters_Folder;
		return;
	}
	if (!strcmp(FileView_File, "Counters"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Counters_File;

		//HideRightPanes();
		
		//App->SBC_Props_Dialog->Hide_Panel_Test_Dlg(1);

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Counters;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Counters();

		return;
	}

	// ------------------------------------------------------------ Eviron_Entities
	if (!strcmp(FileView_Folder, "Evironments")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_EnvironEntity_Folder;
		return;
	}
	if (!strcmp(FileView_File, "Evironments"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_EnvironEntity_File;

		//HideRightPanes();
		/*App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);
		App->SBC_Props_Dialog->Hide_Debug_Dlg(1);*/

		//---------------------------------------------------------------------------

		/*App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		App->SBC_Properties->Last_Selected_Object = Index;*/
		//----------------------------------------------------------------------------

		//App->SBC_Markers->MarkerBB_Addjust(Index);

		App->CL_Properties->Current_Selected_Object = Index;
		App->CL_Properties->Edit_Category = Enums::Edit_Environs;
		App->CL_LookUps->Update_Types();

		ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
		App->CL_Properties->Update_ListView_Environs();

		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/

		return;
	}
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
// *			Redraw_FileViewTerry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_FileView::Redraw_FileView()
{
	//App->RedrawWindow_Dlg(GetDlgItem(App->ListPanel, IDC_TREE1));
}

// *************************************************************************
// *			Change_Level_NameTerry and Hazel Flanigan 2024 			   *
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
// *		Change_Project_NameTerry and Hazel Flanigan 2024 			   *
// *************************************************************************
void CL64_FileView::Change_Project_Name(void)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = GD_ProjectFolder;
	Sitem.pszText = App->CL_Project->m_Project_Name;
	Sitem.iImage = 3;
	Sitem.iSelectedImage = 3;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);
}

// **************************************************************************
// *			Show_FileView:- Terry and Hazel Flanigan 2022				*
// **************************************************************************
void CL64_FileView::Show_FileView(bool Enable)
{
	//ShowWindow(App->ListPanel, 1);

	if (Enable == 1)
	{
		App->CL_FileView->Flag_FileView_Active = 1;
		ShowWindow(App->ListPanel, 1);
		CheckMenuItem(App->mMenu, ID_WINDOWS_FILEVIEW, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		App->CL_FileView->Flag_FileView_Active = 0;
		ShowWindow(App->ListPanel, 0);
		CheckMenuItem(App->mMenu, ID_WINDOWS_FILEVIEW, MF_BYCOMMAND | MF_UNCHECKED);
	}
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
