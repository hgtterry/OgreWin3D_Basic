/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_TXL_Editor.h"

CL64_TXL_Editor::CL64_TXL_Editor()
{
	FileName[0] = 0;
	pData = NULL;
	Entry = NULL;
}

CL64_TXL_Editor::~CL64_TXL_Editor()
{
}

// *************************************************************************
// *			Start_Texl_Dialog:- Terry and Hazel Flanigan 2025   	   *
// *************************************************************************
void CL64_TXL_Editor::Start_Texl_Dialog()
{
	strcpy(FileName, App->CL_Doc->mCurrent_TXL_Path_And_File);

	DialogBox(App->hInst, (LPCTSTR)IDD_TEXTURE_EDITOR, App->MainHwnd, (DLGPROC)Proc_Texture_Lib);
}

// *************************************************************************
// *			Proc_Texture_Lib:- Terry and Hazel Flanigan 2025  		   *
// *************************************************************************
LRESULT CALLBACK CL64_TXL_Editor::Proc_Texture_Lib(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STTXLFILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		/*SendDlgItemMessage(hDlg, IDC_STTEXTURE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STALPHA, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_TEXTURELIST2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_GEINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		/*SendDlgItemMessage(hDlg, IDC_ADD, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTXLDELETE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_EXPORTSELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/

		
		App->CL_TXL_Editor->Entry = new BitmapEntry;
		App->CL_TXL_Editor->Entry->Bitmap = NULL;
		App->CL_TXL_Editor->Entry->WinABitmap = NULL;
		App->CL_TXL_Editor->Entry->WinBitmap = NULL;
		App->CL_TXL_Editor->Entry->Deleted = 0;

		/*SetWindowLong(GetDlgItem(hDlg, IDC_PREVIEW), GWL_WNDPROC, (LONG)TextureLibPreviewWnd);

		bool Test = App->CL_TxlEditor->LoadFile(hDlg);

		SetWindowText(hDlg, "Texture Library Editor");*/

		App->CL_TXL_Editor->pData = new TPack_WindowData;

		char buf1[200];
		strcpy(buf1, "Texture library: - ");
		strcat(buf1, App->CL_TXL_Editor->FileName);
		SetDlgItemText(hDlg, IDC_STTXLFILE, buf1);

		return 1;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_STTEXTURE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STALPHA) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		if (GetDlgItem(hDlg, IDC_STTXLFILE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		break;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_ADD && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTTXLDELETE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_EXPORTSELECTED && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

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
	{

		//-------------------Click in Texture List Box ----------------
		/*if (LOWORD(wParam) == IDC_TEXTURELIST2)
		{
			App->CL_TxlEditor->SelectBitmap();
			return TRUE;
		}*/

		//--------------------------------- Add -----------------------
		/*if (LOWORD(wParam) == IDC_ADD)
		{
			int test = App->CLSB_FileIO->Open_File_Model(App->MainHwnd, "Texture Files ( *.bmp *.tga )\0*.bmp;*.tga\0*.tga\0*.tga\0*.bmp\0*.bmp\0", "Add Texture", "Bitmap Files");
			if (test == 0)
			{
				return TRUE;
			}

			strcpy(App->CL_TxlEditor->Add_Texture_FileName, App->CLSB_FileIO->FileName);

			App->CL_TxlEditor->AddTexture(NULL, App->CL_TxlEditor->Add_Texture_FileName);
			App->CL_TxlEditor->pData->Dirty = 1;
			return TRUE;
		}*/

		//--------------------------------- Delete -----------------------
		//if (LOWORD(wParam) == IDC_BTTXLDELETE)
		//{

		//	int Index = SendDlgItemMessage(App->CL_TxlEditor->pData->hwnd, IDC_TEXTURELIST2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		//	if (Index == -1)
		//	{
		//		return TRUE;
		//	}

		//	//App->CL_TxlEditor->Current_Entry->Deleted = 1;

		//	App->CL_TxlEditor->NewBitmapList[Index]->Deleted = 1;

		//	App->CL_TxlEditor->UpDateList();
		//	App->CL_TxlEditor->SelectBitmap();

		//	App->CL_TxlEditor->pData->Dirty = 1;

		//	return TRUE;
		//}


		////--------------------------------- Save ----------------------
		//if (LOWORD(wParam) == IDC_SAVE)
		//{

		//	App->CL_Texture_Lib->Save(App->CL_Texture_Lib->pData->TXLFileName);
		//	return TRUE;
		//}
		////--------------------------------- Rename ----------------------
		//if (LOWORD(wParam) == IDC_RENAME)
		//{

		//	//DialogBox(hInst, (LPCTSTR)IDD_RENAME, Fdlg, (DLGPROC)RenameProc);
		//	//App->CL_Texture_Lib->pData->Dirty = 1;  // it as changed reqest save
		//	return TRUE;
		//}
		////--------------------------------- Save AS --------------------
		/*if (LOWORD(wParam) == IDC_TEST)
		{
			App->CL_TxlEditor->CreateTexture(NULL, App->CL_TxlEditor->Add_Texture_FileName);
			App->CL_TxlEditor->pData->Dirty = 1;
			return TRUE;
		}*/

		//--------------------------------- IDC_LOAD --------------------
		/*if (LOWORD(wParam) == IDC_BTTXLOPEN)
		{
			int test = App->CLSB_FileIO->Open_File_Model(App->MainHwnd, "Texture Libary   *.txl\0*.txl\0", "Texure Editor", NULL);
			if (test == 0)
			{
				return 1;
			}

			strcpy(App->CL_TxlEditor->FileName, App->CLSB_FileIO->FileName);
			SendDlgItemMessage(App->CL_TxlEditor->pData->hwnd, IDC_TEXTURELIST2, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

			App->CL_TxlEditor->CleanUp();

			bool Test = App->CL_TxlEditor->LoadFile(hDlg);

			if (Test == 0)
			{
				App->Say("Failed");
				return 0;
			}
			return TRUE;
		}*/

		//--------------------------------- EXPORTSELECTED --------------------
		/*if (LOWORD(wParam) == IDC_EXPORTSELECTED)
		{
			App->CL_TxlEditor->TPack_ExtractSelected();
			return TRUE;
		}*/

		//--------------------------------- Export All -----------------
		//if (LOWORD(wParam) == IDC_EXPORTALL)
		//{
		//	/*char File[256];

		//	_getcwd(App->CL_Texture_Lib->LastPath, 256);
		//	_splitpath(App->CL_Texture_Lib->pData->TXLFileName, NULL, NULL, File, NULL);
		//	strcat(File, "_Textures");

		//	strcpy(S_File[0]->NewSubFolderName, File);

		//	strcpy(S_File[0]->SaveMessage, "Select Folder To Place Decompiled Textures");

		//	int Test = GetSaveFolder();
		//	if (Test == 0)
		//	{
		//		return 0;
		//	}

		//	App->CL_Texture_Lib->TPack_ExtractAll();*/

		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{

			/*if (App->CL_TxlEditor->pData->Dirty)
			{
				int	Result;

				Result = MessageBox(NULL,
					"Do you want to save changes before quitting?",
					"Texture Packer",
					MB_YESNOCANCEL);

				if (Result == IDCANCEL)
				{
					return 0;
				}

				if (Result == IDYES)
				{
					App->CL_TxlEditor->Save(App->CL_TxlEditor->pData->TXLFileName);
				}
			}

			App->CL_TxlEditor->CleanUp();*/

			if (App->CL_TXL_Editor->pData)
			{
				delete App->CL_TXL_Editor->pData;
				App->CL_TXL_Editor->pData = NULL;
			}

			if (App->CL_TXL_Editor->Entry)
			{
				delete App->CL_TXL_Editor->Entry;
				App->CL_TXL_Editor->Entry = NULL;
			}

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

	}

	}

	return FALSE;
}
