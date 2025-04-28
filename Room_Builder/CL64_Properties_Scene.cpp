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

	Current_Selected_Object = 0;
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
		//SendDlgItemMessage(hDlg, IDC_STOBJECTNAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		//App->SBC_Properties->Edit_Category = Enums::FV_Edit_Object;*/

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
				//App->CL_Properties_Scene->ListView_OnClickOptions(lParam);
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
