#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Properties_Motions.h"

CL64_Properties_Motions::CL64_Properties_Motions()
{
	Motions_Dlg_Hwnd = nullptr;
}

CL64_Properties_Motions::~CL64_Properties_Motions()
{
}

// *************************************************************************
// *	  	Show_Motions_Dialog_Ogre:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Properties_Motions::Show_Motions_Dialog(bool Show)
{
	ShowWindow(Motions_Dlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Start_Motions_Dialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Motions::Start_Motions_Dialog()
{
	Motions_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_MOTIONS, App->MainHwnd, (DLGPROC)Proc_Motions_Dialog);
	Init_Bmps_Globals();

	Show_Motions_Dialog(false);
	Update_Motions_Combo();
	Update_Speed_Combo();
}

// *************************************************************************
// *	     Proc_Motions_Dialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Motions::Proc_Motions_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_MOTIONS_PAUSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MOT_PLAY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MOT_STOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CB_MOTIONS_MOTIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_MOTIONS_SPEED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_MOT_MOTIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_MOT_SPEED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_MOT_MOTIONS) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_MOT_SPEED) == (HWND)lParam)
		{
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

	/*case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
	}*/

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_MOT_PLAY)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Motions->flag_Motion_Playing);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_MOT_STOP)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_MOTIONS_PAUSE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Motions->flag_Motion_Paused);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_MOT_BONES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_MOT_MESH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Globals(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_MOTIONS_PAUSE)
		{
			if (App->CL_Motions->flag_Motion_Playing == 1)
			{
				if (App->CL_Motions->flag_Motion_Paused == 0)
				{
					App->CL_Motions->Pause_SelectedMotion();
				}
				else
				{
					App->CL_Motions->flag_Motion_Paused = 0;
					App->CL_Motions->Play_SelectedMotion();
				}
			}

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_MOT_PLAY)
		{
			if (App->CL_ImGui->PreviouseMotion_Ogre > -1)
			{
				App->CL_Motions->Play_SelectedMotion();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MOT_STOP)
		{
			App->CL_Motions->Stop_SelectedMotion();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CB_MOTIONS_MOTIONS)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_MOTIONS_MOTIONS);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);
				strcpy(App->CL_Motions->Selected_Motion_Name, buff);

				// Select in Imgui Model Data
				App->CL_ImGui->listMotionItems_Ogre[App->CL_ImGui->PreviouseMotion_Ogre] = false;
				App->CL_ImGui->listMotionItems_Ogre[Index] = true;
				App->CL_ImGui->PreviouseMotion_Ogre = Index;


				App->CL_Motions->Stop_SelectedMotion();
				App->CL_Motions->Play_SelectedMotion();

				RedrawWindow(App->CL_Properties_Motions->Motions_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CB_MOTIONS_SPEED)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_MOTIONS_SPEED);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)buff);

				App->CL_Motions->AnimationScale = atof(buff);

			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MOT_MESH)
		{
			App->CL_Mesh->Show_Mesh_Faces();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MOT_BONES)
		{
			App->CL_Mesh->Show_Mesh_Bones();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MOT_BBOX)
		{
			App->CL_Mesh->Show_Mesh_BoundBox();
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Interface->Show_Motions_Dlg(false);
			return TRUE;
		}

		break;
	}

	}
	return FALSE;
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Properties_Motions::Init_Bmps_Globals(void)
{
	auto& Mot_Dlg = Motions_Dlg_Hwnd;

	HWND Temp = GetDlgItem(Motions_Dlg_Hwnd, IDC_BT_MOT_BONES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);

	Temp = GetDlgItem(Motions_Dlg_Hwnd, IDC_BT_MOT_MESH);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);

	Temp = GetDlgItem(Motions_Dlg_Hwnd, IDC_BT_MOT_BBOX);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);

	
	// ----------------------------------

	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOFADE, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);
	SendMessage(hTooltip_TB_2, TTM_SETMAXTIPWIDTH, 0, 250);

	SendMessage(hTooltip_TB_2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

	Temp = GetDlgItem(Mot_Dlg, IDC_BT_MOT_BONES);
	TOOLINFO ti1 = { 0 };
	ti1.cbSize = sizeof(ti1);
	ti1.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti1.uId = (UINT_PTR)Temp;
	ti1.lpszText = (LPSTR)"Show Bones/Skeleton.";
	ti1.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti1);

	Temp = GetDlgItem(Mot_Dlg, IDC_BT_MOT_MESH);
	TOOLINFO ti2 = { 0 };
	ti2.cbSize = sizeof(ti2);
	ti2.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti2.uId = (UINT_PTR)Temp;
	ti2.lpszText = (LPSTR)"Show Mesh/Faces.";
	ti2.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti2);

	Temp = GetDlgItem(Mot_Dlg, IDC_BT_MOT_BBOX);
	TOOLINFO ti3 = { 0 };
	ti3.cbSize = sizeof(ti3);
	ti3.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti3.uId = (UINT_PTR)Temp;
	ti3.lpszText = (LPSTR)"Show Bounding Box.";
	ti3.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti3);

	SendMessage(hTooltip_TB_2, TTM_SETTITLE, (WPARAM)TTI_INFO, (LPARAM)"");
}

// *************************************************************************
// *		Update_Motions_Combo:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Motions::Update_Motions_Combo(void)
{
	auto& Dlg_Hwnd = Motions_Dlg_Hwnd;

	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_MOTIONS, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	if (App->CL_Model->MotionCount > 0)
	{
		int Count = 0;
		int Size = App->CL_Model->MotionCount;

		while (Count < Size)
		{
			SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_MOTIONS, CB_ADDSTRING, (WPARAM)0, (LPARAM)App->CL_Mesh->S_OgreMeshData[0]->m_Motion_Names[Count].c_str());
			Count++;
		}

		strcpy(App->CL_Motions->Selected_Motion_Name, App->CL_Mesh->S_OgreMeshData[0]->m_Motion_Names[0].c_str());
	}
	else
	{
		SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_MOTIONS, CB_ADDSTRING, (WPARAM)0, (LPARAM)"None");
	}

	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_MOTIONS, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

// *************************************************************************
// *		Update_Speed_Combo:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Properties_Motions::Update_Speed_Combo(void)
{
	auto& Dlg_Hwnd = Motions_Dlg_Hwnd;

	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"2");
	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"1.5");
	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"1");
	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0.5");
	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0.2");
	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0.1");
	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0.01");

	SendDlgItemMessage(Dlg_Hwnd, IDC_CB_MOTIONS_SPEED, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);
}

// *************************************************************************
// *		Update_Motions_By_Name:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Motions::Update_Motions_By_Name(const char* Name, bool Play_Motion)
{
	SendDlgItemMessage(App->CL_Properties_Motions->Motions_Dlg_Hwnd, IDC_CB_MOTIONS_MOTIONS, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)Name);

	strcpy(App->CL_Motions->Selected_Motion_Name, Name);

	if (Play_Motion == true)
	{
		App->CL_Motions->Stop_SelectedMotion();
		App->CL_Motions->Play_SelectedMotion();
	}

	RedrawWindow(App->CL_Properties_Motions->Motions_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
