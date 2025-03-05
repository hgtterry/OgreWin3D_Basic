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
	TXL_Dlg_HWND = NULL;

	BasePicWidth = 0;
	BasePicHeight = 0;
	BasePicDepth = 0;

	mFileName[0] = 0;
	m_Selected_TextureName[0] = 0;
	Add_Texture_FileName[0] = 0;
	Just_Name[0] = 0;

	mFileString.clear();
	flag_Textures_Scanned = 0;
	Sel_BaseBitmap = NULL;

	Texture_Count = 0;
	Selected_Texure_Index = 0;

	int Count = 0;
	while (Count < 199)
	{
		Texture_List[Count] = NULL;
		Count++;
	}
}

CL64_TXL_Editor::~CL64_TXL_Editor()
{
}

// *************************************************************************
// *	  		Start_Texl_Dialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_TXL_Editor::Start_Texl_Dialog()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_TXL_EDITOR, App->MainHwnd, (DLGPROC)Proc_Texl_Dialog);
}

// **************************************************************************
// *			Dialog_Text_Proc:- Terry and Hazel Flanigan 2025			*
// **************************************************************************
LRESULT CALLBACK CL64_TXL_Editor::Proc_Texl_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_TEXTURELIST2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_TEXTURE_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_GEINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_TXL_DELETE_TEXTURE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetWindowLongPtr(GetDlgItem(hDlg, IDC_PREVIEW), GWLP_WNDPROC, (LONG_PTR)Proc_ViewerBasePic);

		App->CL_TXL_Editor->TXL_Dlg_HWND = hDlg;
		App->CL_TXL_Editor->UpDateList();
		App->CL_TXL_Editor->SelectBitmap();
		int TrueIndex = App->CL_TXL_Editor->GetIndex_From_FileName(App->CL_TXL_Editor->m_Selected_TextureName);

		App->CL_TXL_Editor->Update_Texture_Info(TrueIndex);
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_TEXTURE_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_TXL_DELETE_TEXTURE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
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
	{
		//-------------------Click in Texture List Box ----------------
		if (LOWORD(wParam) == IDC_TEXTURELIST2)
		{
			App->CL_TXL_Editor->SelectBitmap();

			int TrueIndex = App->CL_TXL_Editor->GetIndex_From_FileName(App->CL_TXL_Editor->m_Selected_TextureName);
			App->CL_TXL_Editor->Update_Texture_Info(TrueIndex);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TXL_DELETE_TEXTURE)
		{
			App->Say(App->CL_TXL_Editor->m_Selected_TextureName);

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *		Proc_ViewerBasePic:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
bool CALLBACK CL64_TXL_Editor::Proc_ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_PAINT)
	{
		PAINTSTRUCT	ps;
		HDC			hDC;
		RECT		Rect;

		hDC = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &Rect);
		Rect.left--;
		Rect.bottom--;
		FillRect(hDC, &Rect, (HBRUSH)(RGB(0, 255, 0)));

		if (App->CL_TXL_Editor->Sel_BaseBitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = App->CL_TXL_Editor->BasePicHeight;
			Source.right = App->CL_TXL_Editor->BasePicWidth;

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CL_Textures->RenderTexture_Blit(hDC, App->CL_TXL_Editor->Sel_BaseBitmap, &Source, &Dest);
			ReleaseDC(hwnd, hDC);
		}

		EndPaint(hwnd, &ps);

		return 0;
	}

	return 0;
}

// *************************************************************************
// *	Scan_Textures_Resource_Group:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_TXL_Editor::Scan_Textures_Resource_Group()
{
	Texture_Count = 0;

	if (flag_Textures_Scanned == 1)
	{
		char Just_Name[20];
		uint Height = 0;
		
		Ogre::TextureManager* texture_manager = Ogre::TextureManager::getSingletonPtr();

		Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
		Ogre::FileInfoList::const_iterator i, iend;
		iend = RFI->end();
		int Count = 0;

		for (i = RFI->begin(); i != iend; ++i)
		{
			Texture_List[Count] = new BitmapEntry;
			strcpy(Texture_List[Count]->FileName, i->filename.c_str());

			// Just Name no Extension
			strcpy(Just_Name, i->filename.c_str());
			int Len = strlen(Just_Name);
			Just_Name[Len - 4] = 0;
			strcpy(Texture_List[Count]->Name, Just_Name);

			Texture_List[Count]->Deleted = 0;
			Texture_List[Count]->Dib_Index = Count;
			Texture_List[Count]->Dirty = 0;

			Ogre::TexturePtr ogre_texture = texture_manager->getByName(i->filename, App->CL_Ogre->Texture_Resource_Group);
			if (ogre_texture == NULL)
			{
				ogre_texture = texture_manager->load(Ogre::String(i->filename),
					App->CL_Ogre->Texture_Resource_Group,
					Ogre::TEX_TYPE_2D,
					0);
			}

			Texture_List[Count]->Width = ogre_texture->getWidth();
			Texture_List[Count]->Height = ogre_texture->getHeight();
			Texture_List[Count]->Has_Alpha = ogre_texture->hasAlpha();

			Count++;
		}

		Texture_Count = Count;
	}
}

// *************************************************************************
// *						UpDateList  13/06/08 					  	   *
// *************************************************************************
void CL64_TXL_Editor::UpDateList()
{
	int Index = 0;// SendDlgItemMessage(A_TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	int Count = 0;
	while (Count < Texture_Count)
	{
		SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_ADDSTRING, (WPARAM)0, (LPARAM)Texture_List[Count]->FileName);
		Count++;
	}

	SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_SETCURSEL, (WPARAM)Index, (LPARAM)0);

}

// *************************************************************************
// *			A_SelectBitmap:- Terry and Hazel Flanigan 2025 	  	   *
// *************************************************************************
bool CL64_TXL_Editor::SelectBitmap()
{
	int	Index = -1;

	int location = 0;

	Index = SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (Index == -1)
	{
		return 0;
	}
	else
	{
		char buff[MAX_PATH]{ 0 };
		SendDlgItemMessage(TXL_Dlg_HWND, IDC_TEXTURELIST2, LB_GETTEXT, (WPARAM)Index, (LPARAM)m_Selected_TextureName);

		SetDlgItemText(TXL_Dlg_HWND, IDC_ST_TEXTURE_NAME, (LPCTSTR)m_Selected_TextureName);
		
		Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
		Ogre::FileInfoList::const_iterator i, iend;
		iend = RFI->end();

		for (i = RFI->begin(); i != iend; ++i)
		{
			if (i->filename == m_Selected_TextureName)
			{
				Ogre::DataStreamPtr ff = i->archive->open(i->filename);

				mFileString = ff->getAsString();

				char mFileName[MAX_PATH];
				strcpy(mFileName, App->RB_Directory_FullPath);
				strcat(mFileName, "\\Data\\");
				strcat(mFileName, m_Selected_TextureName);

				std::ofstream outFile;
				outFile.open(mFileName, std::ios::binary);
				outFile << mFileString;
				outFile.close();

				mFileString.clear();

				Texture_To_HBITMP(mFileName);
				remove(mFileName);
				return 1;
			}
		}
	}

	return 0;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_TXL_Editor::Texture_To_HBITMP(char* TextureFileName)
{
	HWND PreviewWnd = GetDlgItem(TXL_Dlg_HWND, IDC_PREVIEW);
	HDC	hDC = GetDC(PreviewWnd);

	Sel_BaseBitmap = App->CL_Textures->Get_HBITMP(TextureFileName, hDC);

	BasePicWidth = App->CL_Textures->BasePicWidth;
	BasePicHeight = App->CL_Textures->BasePicHeight;
	//BasePicDepth = ilGetInteger(IL_IMAGE_DEPTH);

	RedrawWindow(TXL_Dlg_HWND, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *				Check_if_Name_Exist:- Terry and Hazel Flanigan 2025 		  	   *
// *************************************************************************
int CL64_TXL_Editor::Check_if_Name_Exist(const char* Name)
{
	int	i;

	for (i = 0; i < Texture_Count; i++)
	{

		if (!strcmp(Name, Texture_List[i]->Name))
		{
			return 1;
		}
	}

	return 0;
}

// *************************************************************************
// *		GetIndex_From_FileName:- Terry and Hazel Flanigan 2025 	  	   *
// *************************************************************************
int CL64_TXL_Editor::GetIndex_From_FileName(const char* Name)
{
	int	Count = 0;

	while (Count < Texture_Count)
	{
		if (!strcmp(Name, Texture_List[Count]->FileName))
		{
			return Count;
		}

		Count++;
	}

	return -1;
}

// *************************************************************************
// *			GetIndex_From_Name:- Terry and Hazel Flanigan 2025 	  	   *
// *************************************************************************
int CL64_TXL_Editor::GetIndex_From_Name(const char* Name)
{
	int	Count = 0;

	while (Count < Texture_Count)
	{
		if (!strcmp(Name, Texture_List[Count]->Name))
		{
			return Count;
		}

		Count++;
	}

	return -1;
}

// *************************************************************************
// *			Update_Texture_Info:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_TXL_Editor::Update_Texture_Info(int Index)
{
	int B = 0;
	//geBitmap_Info	MPInfo;
	//geBitmap_Info	MSInfo;

	char buff[256];
	strcpy(buff, "no info");
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	sprintf(buff, "%s %s", "Texture Name :-", Texture_List[Index]->Name);
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %s", "File Name :-", Texture_List[Index]->FileName);
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	//sprintf(buff, "%s %i", "Index :-", Location);
	//SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	/*sprintf(buff, "%s %s", "Bitmap :-", "Valid");
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %d", "Width :-", geBitmap_Width(NewBitmapList[Location]->Bitmap));
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

	sprintf(buff, "%s %d", "Height :-", geBitmap_Height(NewBitmapList[Location]->Bitmap));
	SendDlgItemMessage(TXL_Dlg_HWND, IDC_GEINFO, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);*/

	//geBitmap_GetInfo(NewBitmapList[Location]->Bitmap, &MPInfo, &MSInfo);

}



