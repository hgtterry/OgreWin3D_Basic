/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Resources.h"

CL64_Resources::CL64_Resources(void)
{
	ResourcePath[0] = 0;

	FX_General_hLV = nullptr;
}

CL64_Resources::~CL64_Resources(void)
{
}

// *************************************************************************
// *	  		Start_Resources:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Resources::Start_Resources()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_RESOURCESMATERIAL, App->Fdlg, (DLGPROC)Resources_Proc);
}
// *************************************************************************
// *			Materials_Proc:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Resources::Resources_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		/*App->SetTitleBar(hDlg);

		SendDlgItemMessage(hDlg, IDC_ST_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_SCENEMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTMATSF, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTMESHSF, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTEXTSF, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		*/

		App->CL_Resources->CreateListGeneral_FX(hDlg);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_ST_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->DialogBackGround;
		}

		if (GetDlgItem(hDlg, IDC_STCOUNT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}*/


		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BT_APPRESOURCES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->SBC_Resources->Show_App_Res_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_PRJRESOURCES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->SBC_Resources->Show_Project_Res_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTMVRESOURCES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->SBC_Resources->Show_MV_Res_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_SCENEMESH && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTMATSF && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTMESHSF && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTTEXTSF && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}


	case WM_COMMAND:


		//if (LOWORD(wParam) == IDC_MESHMATERIALS)
		//{
		//	App->GDCL_Dialogs->Start_Gen_ListBox(Enums::ListBox_Resource_MeshMaterials);
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_Paths)
		//{
		//	App->GDCL_Dialogs->Start_Gen_ListBox(Enums::ListBox_Resource_Paths);
		//	return TRUE;
		//}

		/*if (LOWORD(wParam) == IDC_MATERIALS)
		{
			App->SBC_Resources->ShowAllMaterials();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TEXTURES)
		{
			App->SBC_Resources->ShowAllTextures();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_MESH)
		{
			App->SBC_Resources->ShowAllMeshes();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_USED)
		{
			App->SBC_Resources->ShowUsedMaterials();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PRJRESOURCES)
		{
			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)"Project Resources");

			App->SBC_Resources->Show_Project_Res();
			return TRUE;
		}*/

		if (LOWORD(wParam) == IDC_BT_APPRESOURCES)
		{
			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)"App Resources");

			//App->SBC_Resources->Show_App_Res();
			return TRUE;
		}

		/*if (LOWORD(wParam) == IDC_BTMVRESOURCES)
		{
			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)"Mesh Viewer Resources");

			App->SBC_Resources->Show_MV_Res();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_SCENEMESH)
		{
			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)"Scene Meshes");
			App->SBC_Resources->Show_Scene_Meshes(hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTMATSF)
		{
			char Message[MAX_PATH];
			strcpy(Message, "Scene Folder Materials - ");
			strcat(Message, App->SBC_Project->m_Main_Assets_Path);

			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)Message);

			ListView_DeleteAllItems(App->SBC_Resources->FX_General_hLV);
			App->SBC_Resources->SearchFolders("*.material");
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTMESHSF)
		{
			char Message[MAX_PATH];
			strcpy(Message, "Scene Folder Mesh - ");
			strcat(Message, App->SBC_Project->m_Main_Assets_Path);

			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)Message);

			ListView_DeleteAllItems(App->SBC_Resources->FX_General_hLV);
			App->SBC_Resources->SearchFolders("*.mesh");
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTTEXTSF)
		{
			char Message[MAX_PATH];
			strcpy(Message, "Scene Folder Textures - ");
			strcat(Message, App->SBC_Project->m_Main_Assets_Path);

			SetDlgItemText(hDlg, IDC_ST_BANNER, (LPCTSTR)Message);

			ListView_DeleteAllItems(App->SBC_Resources->FX_General_hLV);
			App->SBC_Resources->SearchFolders("*.bmp");
			App->SBC_Resources->SearchFolders("*.tga");
			App->SBC_Resources->SearchFolders("*.jpg");
			App->SBC_Resources->SearchFolders("*.png");
			App->SBC_Resources->SearchFolders("*.dds");

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
// *		CreateListGeneral_FX:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Resources::CreateListGeneral_FX(HWND hDlg)
{
	int NUM_COLS = 4;
	FX_General_hLV = CreateWindowEx(0, WC_LISTVIEW, "",
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS, 2, 2,
		1280, 405, hDlg, 0, App->hInst, NULL);

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
		"Script", "Material File","Used","Path"
	};
	int headerSize[] =
	{
		165,380,70,250
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
	SendMessage(FX_General_hLV, WM_SETFONT, (WPARAM)Font, MAKELPARAM(TRUE, 0));

	ShowAllMaterials();
	//ShowAllTextures();
}

// *************************************************************************
// *			ShowAllMaterials:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
bool CL64_Resources::ShowAllMaterials()
{
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask = LVIF_TEXT;

	ListView_DeleteAllItems(FX_General_hLV);

	int	 pRow = 0;
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
		ListView_SetItemText(FX_General_hLV, pRow, 2, pUsed);
		ListView_SetItemText(FX_General_hLV, pRow, 3, ResourcePath);

		pRow++;

		materialIterator.moveNext();
	}

	return 1;
}

// *************************************************************************
// *			Start_List_Folders:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Resources::Start_List_Folders(HWND List, char* FileName, bool ListDlg)
{
	char StartFolder[2048];

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
	char pSearchPath[1024];
	char pReturnPath[1024];
	char* pPartPath;

	strcpy(pSearchPath, Folder);

	int result = SearchPath((LPCTSTR)pSearchPath, (LPCTSTR)File, NULL, 1024, pReturnPath, &pPartPath);
	if (result == 0)
	{
		return 0;
	}

	return 1;
}

