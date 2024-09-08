/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "CL64_App.h"
#include "resource.h"
#include "CL64_Props_Textures.h"

#include "il.h"
#include "ilu.h"
#include "ilut.h"

CL64_Props_Textures::CL64_Props_Textures(void)
{
	Props_Dlg_Hwnd = NULL;

	Selected_Group = 0;
	RightGroups_Visable = 0;

	Sel_BaseBitmap = nullptr;
	BasePicWidth = 0;
	BasePicHeight = 0;

	strcpy(mTextureName, "Texture Name");
	strcpy(mMaterialName, "Material Name");
}

CL64_Props_Textures::~CL64_Props_Textures(void)
{
}

// *************************************************************************
// *					Reaet_Class Terry Flanigan						   *
// *************************************************************************
void CL64_Props_Textures::Reset_Class(void)
{
	Selected_Group = 0;
	Sel_BaseBitmap = nullptr;
	BasePicWidth = 0;
	BasePicHeight = 0;

	ShowWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_PROP_BASETEXTURE), 0);
	ShowWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_PROP_BASETEXTURE), 1);

	//SetDlgItemText(RightGroups_Hwnd, IDC_RGGROUPNAME, "Group Name");
	SetDlgItemText(Props_Dlg_Hwnd, IDC_PT_TEXTURENAME, "Texture Name");
	SetDlgItemText(Props_Dlg_Hwnd, IDC_PT_TEXTURENAME, "Material Name");
}

// *************************************************************************
// *	 Start_Props_Textures_Dialog:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Props_Textures::Start_Props_Textures_Dialog()
{
	Props_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEXTURES, App->MainHwnd, (DLGPROC)Proc_Textures_Dialog);
	ShowWindow(Props_Dlg_Hwnd, 1);
	RightGroups_Visable = 1;
	return 1;
}

// *************************************************************************
// *			Groups_Proc:- Terry and Hazel Flanigan 2024		  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Props_Textures::Proc_Textures_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_PT_MATERIAL, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_PT_TEXTURENAME, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_RGTEXTURENAME, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		//SendDlgItemMessage(hDlg, IDC_BTCHANGETEXTURE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		//SendDlgItemMessage(hDlg, IDC_BTGROUPINFO, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0))

		SetWindowLongPtr(GetDlgItem(hDlg, IDC_PROP_BASETEXTURE), GWLP_WNDPROC, (LONG_PTR)ViewerBasePic);
		
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_RGGROUPNAME) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		if (GetDlgItem(hDlg, IDC_PT_TEXTURENAME) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_PT_MATERIAL) == (HWND)lParam)
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

	case WM_CLOSE:
	{
		ShowWindow(App->CL_Props_Textures->Props_Dlg_Hwnd, 0);
		App->CL_Props_Textures->RightGroups_Visable = 0;
		//CheckMenuItem(App->mMenu, ID_WINDOWS_GROUPS, MF_BYCOMMAND | MF_UNCHECKED);
		break;
	}


	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDC_BTCHANGETEXTURE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		/*if (some_item->idFrom == IDC_BTGROUPINFO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Editor_Gui->Show_Group_Data_F);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		/*if (LOWORD(wParam) == IDC_BTCHANGETEXTURE)
		{
			if (App->CL_Model->Model_Loaded == 1)
			{
				App->CL_Panels->Enable_Panels(0);
				App->CL_Textures->ChangeTexture_Model();
				App->CL_Panels->Enable_Panels(1);
			}
			return TRUE;
		}*/

		//if (LOWORD(wParam) == IDC_BTGROUPINFO)
		//{
		//	//if (App->CL_Model->Model_Loaded == 1)
		//	//{
		//	//	if (App->CL_Editor_Gui->Show_Group_Data_F == 1)
		//	//	{
		//	//		App->CL_Editor_Gui->Close_Group_Data();
		//	//	}
		//	//	else
		//	//	{
		//	//		App->CL_Editor_Gui->Start_Group_Data();
		//	//	}


		//	//	/*App->CL_Panels->Enable_Panels(0);

		//	//	App->CL_Dialogs->What_List = Enums::Show_List_Group;
		//	//	App->CL_Dialogs->Show_ListData();

		//	//	App->CL_Panels->Enable_Panels(1);*/
		//	//}
		//	return TRUE;
		//}
	}

	}
	return FALSE;
}

// *************************************************************************
// *						ViewerBasePic Terry Flanigan	  			   *
// *************************************************************************
bool CALLBACK CL64_Props_Textures::ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		if (App->CL_Props_Textures->Sel_BaseBitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = App->CL_Props_Textures->BasePicHeight;
			Source.right = App->CL_Props_Textures->BasePicWidth;

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CL_Props_Textures->RenderTexture_Blit(hDC, App->CL_Props_Textures->Sel_BaseBitmap, &Source, &Dest);
			ReleaseDC(hwnd, hDC);
		}

		EndPaint(hwnd, &ps);
		return 0;
	}
	return 0;// DefWindowProc(hwnd, msg, wParam, lParam);
}

// *************************************************************************
// *					RenderTexture_Blit Terry Bernie		  		   *
// *************************************************************************
bool CL64_Props_Textures::RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect)
{
	HDC		MemDC;
	int		SourceWidth;
	int		SourceHeight;
	int		DestWidth;
	int		DestHeight;

	MemDC = CreateCompatibleDC(hDC);
	if (MemDC == NULL)
		return FALSE;

	if (Bmp)
	{
		SelectObject(MemDC, Bmp);

		SourceWidth = SourceRect->right - SourceRect->left;
		SourceHeight = SourceRect->bottom - SourceRect->top;
		DestWidth = DestRect->right - DestRect->left;
		DestHeight = DestRect->bottom - DestRect->top;
		SetStretchBltMode(hDC, COLORONCOLOR);
		StretchBlt(hDC,
			DestRect->left,
			DestRect->top,
			DestHeight,
			DestHeight,
			MemDC,
			SourceRect->left,
			SourceRect->top,
			SourceWidth,
			SourceHeight,
			SRCCOPY);
	}

	DeleteDC(MemDC);

	return TRUE;
}

// *************************************************************************
// *					Update_Groups  Terry Flanigan		  		 	   *
// *************************************************************************
bool CL64_Props_Textures::Update_Groups()
{
	int Index = Selected_Group;

	//SetDlgItemText(RightGroups_Hwnd, IDC_RGGROUPNAME, App->CL_Scene->Group[Index]->GroupName);

	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIAL, mMaterialName);
	SetDlgItemText(Props_Dlg_Hwnd, IDC_PT_TEXTURENAME, mTextureName);

	RightGroups_Visable = 1;
	ShowWindow(Props_Dlg_Hwnd, 1);
	//CheckMenuItem(App->mMenu, ID_WINDOWS_GROUPS, MF_BYCOMMAND | MF_CHECKED);

	//Sel_BaseBitmap = App->CL_Dialogs->Sel_BaseBitmap;// App->CL_Scene->Group[Index]->Base_Bitmap;

	BITMAP bm;
	GetObject(Sel_BaseBitmap, sizeof(bm), &bm);

	BasePicWidth = bm.bmWidth;
	BasePicHeight = bm.bmHeight;

	ShowWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_PROP_BASETEXTURE), 0);
	ShowWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_PROP_BASETEXTURE), 1);

	return 1;
}

// *************************************************************************
// *				View_Texture:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Props_Textures::View_Texture(char* TextureName, char* MaterialName)
{
	strcpy(mTextureName, TextureName);
	strcpy(mMaterialName, MaterialName);

	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Resources->mSelected_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	for (i = RFI->begin(); i != iend; ++i)
	{
		if (i->filename == TextureName)
		{
			Ogre::DataStreamPtr ff = i->archive->open(i->filename);

			App->CL_Resources->mFileString = ff->getAsString();

			char mFileName[MAX_PATH];
			strcpy(mFileName, App->GD_Directory_FullPath);
			strcat(mFileName, "\\Data\\");
			strcat(mFileName, TextureName);

			std::ofstream outFile;
			outFile.open(mFileName, std::ios::binary);
			outFile << App->CL_Resources->mFileString;
			outFile.close();

			App->CL_Resources->mFileString.clear();

			Texture_To_HBITMP(mFileName);

			Update_Groups();

			return 1;
		}
	}

	return 1;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Props_Textures::Texture_To_HBITMP(char* TextureFileName)
{
	HWND PreviewWnd = GetDlgItem(App->CL_Dialogs->RightGroups_Hwnd, IDC_BASETEXTURE);
	HDC	hDC = GetDC(PreviewWnd);

	Sel_BaseBitmap = ilutWinLoadImage(TextureFileName, hDC);

	//BasePicWidth = ilGetInteger(IL_IMAGE_WIDTH);
	//BasePicHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	//BasePicDepth = ilGetInteger(IL_IMAGE_DEPTH);
}

