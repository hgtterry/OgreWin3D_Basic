#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CreateBoxDialog.h"

CreateBoxDialog::CreateBoxDialog(void)
{
	pBoxTemplate = NULL;
}

CreateBoxDialog::~CreateBoxDialog(void)
{
}

// *************************************************************************
// *	  	Start_CreateBox_Dlg:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CreateBoxDialog::Start_CreateBox_Dlg()
{

	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();

	App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

	//App->Say("Here");
	DialogBox(App->hInst, (LPCTSTR)IDD_CREATE_BOX, App->MainHwnd, (DLGPROC)Proc_CreateBox);
}

// *************************************************************************
// *        CreateBox_Proc:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
LRESULT CALLBACK CreateBoxDialog::Proc_CreateBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Array of control IDs to set font to Font_CB15
		const int controlIDs[] =
		{
			IDC_STBOTTOM, IDC_STBOTX, IDC_STBOTZ, IDC_XSIZEBOT, IDC_ZSIZEBOT,
			IDC_STTOP, IDC_STTOPX, IDC_STZTOP, IDC_XSIZETOP, IDC_ZSIZETOP,
			IDC_BT_BOXSOLID, IDC_BT_BOXHOLLOW, IDC_BT_BOXCUTBRUSH,
			IDC_STGENERAL, IDC_STYSIZE, IDC_STWALL, IDC_YSIZE, IDC_THICKNESS,
			IDC_STNAME, IDC_EDITNAME,
			IDC_BOXDEFAULTS, IDC_BT_BOXROOM,
			IDC_STCAMPOS, IDC_CKWORLDCENTRE, IDC_CKCAMPOSITION,
			IDOK, IDCANCEL
		};

		// Set font for each control
		for (int id : controlIDs) {
			SendDlgItemMessage(hDlg, id, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		}

		// Initialize dialog members
		/*App->CL_CreateBoxDialog->SetMembers();
		App->CL_CreateBoxDialog->Set_Dialog_Members(hDlg);
		App->CL_CreateBoxDialog->SetDefaults(hDlg);*/

		// Generate and set the box name
		int Count = App->CL_Brush->Get_Brush_Count();
		char Name[32];
		snprintf(Name, sizeof(Name), "Box_%d", Count);
		SetDlgItemText(hDlg, IDC_EDITNAME, Name);

		// Set checkbox state
		HWND Temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
		SendMessage(Temp, BM_SETCHECK, 1, 0);
		App->CL_CreateBoxDialog->m_UseCamPos = 0;

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		const int controlIds[] =
		{
			IDC_STTOP, IDC_STBOTTOM, IDC_STTOPX, IDC_STWALL,
			IDC_STGENERAL, IDC_STZTOP, IDC_STBOTX, IDC_STBOTZ,
			IDC_STYSIZE, IDC_STNAME, IDC_STCAMPOS,
			IDC_CKWORLDCENTRE, IDC_CKCAMPOSITION
		};

		for (const auto& id : controlIds)
		{
			if (GetDlgItem(hDlg, id) == (HWND)lParam)
			{
				SetBkColor((HDC)wParam, RGB(0, 0, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
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
		LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

		switch (some_item->idFrom)
		{
		case IDC_BT_BOXSOLID:
			//App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->Solid_Flag);
			break;

		case IDC_BT_BOXHOLLOW:
			//App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->Hollow_Flag);
			break;

		case IDC_BT_BOXCUTBRUSH:
			//App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->Cut_Flag);
			break;

		case IDC_BOXDEFAULTS:
			//App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->flag_Default);
			break;

		case IDC_BT_BOXROOM:
			//App->Custom_Button_Toggle_Tabs(item, App->CL_CreateBoxDialog->flag_Room);
			break;

		case IDOK:
		case IDCANCEL:
			App->Custom_Button_Normal(item);
			break;

		default:
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_BOXSOLID)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
			SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_SolidBox_Bmp);

			/*App->CL_CreateBoxDialog->Zero_Dlg_Flags(hDlg);
			App->CL_CreateBoxDialog->m_Solid = 0;
			App->CL_CreateBoxDialog->Solid_Flag = 1;*/

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXHOLLOW)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_PICTURE);
			SendMessage(Temp, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HollowBox_Bmp);

			/*App->CL_CreateBoxDialog->Zero_Dlg_Flags(hDlg);
			App->CL_CreateBoxDialog->m_Solid = 1;
			App->CL_CreateBoxDialog->Hollow_Flag = 1;*/

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXCUTBRUSH)
		{
			/*App->CL_CreateBoxDialog->m_TCut = !App->CL_CreateBoxDialog->Cut_Flag;
			App->CL_CreateBoxDialog->Cut_Flag = !App->CL_CreateBoxDialog->Cut_Flag;*/

			int Count = App->CL_Brush->Get_Brush_Count();
			char Name[32];
			//snprintf(Name, sizeof(Name), "Box_%d%s", Count, App->CL_CreateBoxDialog->Cut_Flag ? "_Cut" : "");

			SetDlgItemText(hDlg, IDC_EDITNAME, (LPTSTR)Name);
			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}

		if (LOWORD(wParam) == IDC_CKWORLDCENTRE)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CL_CreateBoxDialog->m_UseCamPos = 0;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKCAMPOSITION)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKCAMPOSITION);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKWORLDCENTRE);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CL_CreateBoxDialog->m_UseCamPos = 1;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BOXDEFAULTS)
		{
			/*App->CL_CreateBoxDialog->flag_Default = 1;
			App->CL_CreateBoxDialog->flag_Room = 0;*/
			RedrawWindow(hDlg, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

			int brushCount = App->CL_Brush->Get_Brush_Count();
			std::string name = "Box_" + std::to_string(brushCount);

			SetDlgItemText(hDlg, IDC_EDITNAME, name.c_str());

			//App->CL_CreateBoxDialog->SetDefaults(hDlg);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BOXROOM)
		{
			/*App->CL_CreateBoxDialog->flag_Default = 0;
			App->CL_CreateBoxDialog->flag_Room = 1;*/
			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			int brushCount = App->CL_Brush->Get_Brush_Count();
			std::string boxName = "Box_" + std::to_string(brushCount);

			SetDlgItemText(hDlg, IDC_EDITNAME, boxName.c_str());

			//App->CL_CreateBoxDialog->SetRoom(hDlg);
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			/*App->CL_CreateBoxDialog->Get_Dialog_Members(hDlg);
			App->CL_CreateBoxDialog->Set_BoxTemplate();
			App->CL_CreateBoxDialog->CreateCube();*/

			App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
			App->CL_Properties_Templates->Enable_Insert_Button(true);

			EndDialog(hDlg, LOWORD(wParam));
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
	}
	return FALSE;
}

