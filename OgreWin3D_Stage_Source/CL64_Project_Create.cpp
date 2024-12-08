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
	
}

CL64_Project_Create::~CL64_Project_Create(void)
{
}

// *************************************************************************
// *	  	Create_Options_Dialog:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Project_Create::Create_Options_Dialog()
{
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
		//SendDlgItemMessage(hDlg, IDC_EDIT1, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//SetDlgItemText(hDlg, IDC_BANNER, (LPCTSTR)App->CL_Dialogs->btext);

		//SetDlgItemText(hDlg, IDC_EDIT1, (LPCTSTR)App->CL_Dialogs->Chr_Int);

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
			App->CL_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Dialogs->Canceled = 1;
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
	App->CL_Scene->Clear_Level();
	App->CL_Resources->Create_Project_Resources_Group();

	Create_Options_Dialog();

	App->CL_Project->flag_Is_New_Project = 1;
	App->CL_Project->Start_Save_Project_Dialog();
	App->CL_Project->flag_Is_New_Project = 0;

	// ----------------------------------------------- Player
	App->CL_Player->Create_Player_Object();
	strcpy(App->CL_Scene->B_Player[0]->Player_Name, "Player_1");

	HTREEITEM Temp1 = App->CL_FileView->Add_Item(App->CL_FileView->FV_Players_Folder, (LPSTR)"Player_1", 0, true);
	App->CL_Scene->B_Player[0]->FileViewItem = Temp1;
	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Players_Folder);

	// ----------------------------------------------- Counter
	App->CL_Display->Add_New_Counter();

	// ------------------------ Add Location
	App->CL_Locations->Create_Location_Entity((LPSTR)"Start_Position");

	// ----------------------------------------------- Add Environ
	App->CL_Com_Environments->Add_New_Environ_Entity(1);
	int mIndex = App->CL_Com_Environments->Get_First_Environ();
	App->CL_Com_Environments->Set_First_Environment(mIndex);

	// ----------------------------------------------- Camera
	App->CL_Com_Cameras->Add_New_Camera();


	App->CL_Project->Save_All();

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
	App->CL_Scene->flag_Scene_Loaded = 1;

	App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Add Area", (LPSTR)"Do you want to add a new Area", (LPSTR)"");

	bool Doit = App->CL_Dialogs->Canceled;
	if (Doit == 0)
	{
		App->CL_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Area;
		App->CL_MeshViewer->Start_MeshViewer_Dlg();
	}
	else
	{
		
	}
}

// *************************************************************************
// *	  	Add_First_New_Area:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Project_Create::Add_First_New_Area()
{
	First_Area_Build_Project(0);
}

// *************************************************************************
//		First_Area_Build_Project:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Project_Create::First_Area_Build_Project(bool NoArea)
{

	char Temp[MAX_PATH];
	//strcpy(Temp, App->CL_Scene->Project_Resource_Group.c_str());

	// ------------------------ Add Area
	if (NoArea == 0)
	{
		App->CL_Scene->B_Area[0] = new Base_Area();
		App->CL_Com_Area->Add_Aera_To_Project(0, App->CL_MeshViewer->Selected_MeshFile, Temp);
		//strcpy(App->CL_Scene->B_Area[0]->Material_File, App->CL_MeshViewer->m_Material_File);
		App->CL_Scene->Area_Count++;
		//App->CL_Scene->Area_Added = 1;

		HTREEITEM Temp2 = App->CL_FileView->Add_Item(App->CL_FileView->FV_Areas_Folder, (LPSTR)"Area_1", 0, true);
		App->CL_Scene->B_Area[0]->FileViewItem = Temp2;
		App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Areas_Folder);
	}

	// ------------------------ Add Default Camera
	//App->CL_Com_Camera->Add_New_Camera();
	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Cameras_Folder);
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

	// ------------------------ Add Default Player
	App->CL_Player->Create_Player_Object();
	strcpy(App->CL_Scene->B_Player[0]->Player_Name, "Player_1");

	HTREEITEM Temp1 = App->CL_FileView->Add_Item(App->CL_FileView->FV_Players_Folder, (LPSTR)"Player_1", 0, true);
	App->CL_Scene->B_Player[0]->FileViewItem = Temp1;
	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Players_Folder);

	// ------------------------ Add Counter
	App->CL_Display->Add_New_Counter();

	// ------------------------ Add Location
	App->CL_Locations->Create_Location_Entity((LPSTR)"Start_Position");

	// ------------------------ Add Environ

	App->CL_Com_Environments->Add_New_Environ_Entity(1);
	int mIndex = App->CL_Com_Environments->Get_First_Environ();
	App->CL_Com_Environments->Set_First_Environment(mIndex);

	// ------------------------ Set Scene
	App->CL_Grid->Grid_SetVisible(1);
	App->CL_FileView->Redraw_FileView();

	App->CL_FileView->SelectItem(App->CL_Scene->B_Area[0]->FileViewItem);

	App->CL_Physics->Reset_Physics();
	App->CL_Physics->Enable_Physics(1);
	App->CL_Scene->flag_Scene_Loaded = 1;
	//App->CL_Scene->Area_Added = 1;
	App->CL_Ogre->flag_Block_Rendering = 0;

	//------------------------------------------------------------------------------ WHY
	int f = App->CL_Scene->B_Player[0]->Phys_Body->getCollisionFlags();
	App->CL_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	//App->CL_Ogre->Block_RenderingQueued = 0;

	return 1;
}
