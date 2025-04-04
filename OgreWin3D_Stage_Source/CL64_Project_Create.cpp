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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Project_Create.h"

CL64_Project_Create::CL64_Project_Create(void)
{
	flag_Canceled = 0;
	flag_Indoors = 1;
}

CL64_Project_Create::~CL64_Project_Create(void)
{
}

// *************************************************************************
// *	  	Create_Options_Dialog:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Project_Create::Create_Options_Dialog()
{
	flag_Canceled = 0;
	DialogBox(App->hInst, (LPCTSTR)IDD_CREATE_OPTIONS, App->Fdlg, (DLGPROC)Proc_Options_Dialog);
}

// *************************************************************************
// *        Proc_Options_Dialog:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_Project_Create::Proc_Options_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CP_INDOORSCENE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CP_TERRAIN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CP_PLAIN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{

		/*if (GetDlgItem(hDlg, IDC_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
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

		if (some_item->idFrom == IDC_BT_CP_INDOORSCENE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Project_Create->flag_Indoors);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CP_TERRAIN)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_CP_TERRAIN));
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

		if (some_item->idFrom == IDC_BT_CP_PLAIN)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_CP_PLAIN));
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

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Project_Create->flag_Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Project_Create->flag_Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  		Start_New_Project:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Project_Create::Start_New_Project()
{
	App->CL_Ogre->Log_Message_To_File((LPSTR)" // -------------------------  Create Project");

	App->CL_Ogre->Log_Message_To_File((LPSTR)"Open Options Diologs");
	Create_Options_Dialog();
	if (flag_Canceled == 1)
	{
		return;
	}

	App->CL_Scene->Clear_Level();
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Level Cleared");

	App->CL_Resources->Create_Project_Resources_Group();
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Created Resource Group");

	strcpy(App->CL_Project->m_Project_Name, "New_Project_Name");
	strcpy(App->CL_Project->m_Level_Name, "New_Project_Level");

	App->CL_Project->flag_Is_New_Project = 1;
	App->CL_Project->Start_Save_Project_Dialog();
	App->CL_Project->flag_Is_New_Project = 0;
	if (App->CL_Project->flag_Canceled == 1)
	{
		return;
	}

	App->CL_Ogre->Log_Message_To_File((LPSTR)"Save Project Dialog Closed");

	Setup_Scene_Indoors();

	App->CL_Com_Objects->flag_Dont_Clear_Objects = 1;
	bool test = App->CL_Project->Save_All(true);
	if (test == 0)
	{
		return;
	}
	App->CL_Com_Objects->flag_Dont_Clear_Objects = 0;

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
	App->CL_Scene->flag_Scene_Loaded = 1;

	// ---------------------------------------------------
	App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Add Area", (LPSTR)"Do you want to add a new Area", (LPSTR)"");

	bool Doit = App->CL_Dialogs->flag_Canceled;
	if (Doit == 0)
	{
		App->CL_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Area;

		App->CL_Ogre->Log_Message_To_File((LPSTR)"MeshViewer Started");
		App->CL_MeshViewer->Start_MeshViewer_Dlg();
		App->CL_Ogre->Log_Message_To_File((LPSTR)"MeshViewer Ended");
	}
	else
	{
		
	}
}

// *************************************************************************
// *	  	Setup_Scene_Indoors:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Project_Create::Setup_Scene_Indoors()
{
	// ----------------------------------------------- Player
	App->CL_Com_Player->Create_Player_Object();
	strcpy(App->CL_Scene->B_Player[0]->Player_Name, "Player_1");
	HTREEITEM Temp1 = App->CL_FileView->Add_Item(App->CL_FileView->FV_Players_Folder, (LPSTR)"Player_1", 0, true);
	App->CL_Scene->B_Player[0]->FileViewItem = Temp1;
	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Players_Folder);
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Player Added");
	// ----------------------------------------------- Counter
	App->CL_Com_Counters->Add_New_Counter();
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Counter Added");
	// ------------------------ Add Location
	App->CL_Locations->Create_Location_Entity((LPSTR)"Start_Position");
	
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Location Added");
	// ----------------------------------------------- Add Environ
	App->CL_Com_Environments->Add_New_Environ_Entity(1);
	int mIndex = App->CL_Com_Environments->Get_First_Environ();
	App->CL_Com_Environments->Set_First_Environment(mIndex);
	//App->CL_Com_Environments->Mark_As_Altered_Environ(0);
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Environment Added");
	// ----------------------------------------------- Camera
	App->CL_Com_Cameras->Add_New_Camera();
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Camera Added");
}

