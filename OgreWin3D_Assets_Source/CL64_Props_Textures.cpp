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

	mExport_PathAndName[0] = 0;
	strcpy(mTextureName, "Texture Name");
	strcpy(mMaterialName, "Material Name");
}

CL64_Props_Textures::~CL64_Props_Textures(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Props_Textures::Reset_Class(void)
{
	Selected_Group = 0;
	Sel_BaseBitmap = nullptr;
	BasePicWidth = 0;
	BasePicHeight = 0;

	ShowWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_PROP_BASETEXTURE), 0);
	ShowWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_PROP_BASETEXTURE), 1);

	SetDlgItemText(Props_Dlg_Hwnd, IDC_PT_TEXTURENAME, "-----------");
	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIAL, "-----------");
	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIALFILE, "-----------");
	
	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_DIMENSIONS,"0 x 0");

	RightGroups_Visable = 0;
	ShowWindow(Props_Dlg_Hwnd, 0);
	CheckMenuItem(App->mMenu, ID_WINDOWS_TEXTURESDIALOG, MF_BYCOMMAND | MF_UNCHECKED);

	Enable_Export_Button(false);
}

// *************************************************************************
// *	 Start_Props_Textures_Dialog:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Props_Textures::Start_Props_Textures_Dialog()
{
	Props_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEXTURES, App->MainHwnd, (DLGPROC)Proc_Textures_Dialog);

	App->CL_Props_Textures->Enable_Export_Button(false);
	//ShowWindow(Props_Dlg_Hwnd, 1);
	RightGroups_Visable = 1;
	CheckMenuItem(App->mMenu, ID_WINDOWS_TEXTURESDIALOG, MF_BYCOMMAND | MF_UNCHECKED);
	
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
		SendDlgItemMessage(hDlg, IDC_ST_MATERIAL, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_PT_MATERIAL, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_TEXURENAME, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_PT_TEXTURENAME, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_PT_DIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_PT_EXPORT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PT_VIEWMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PT_VIEWMAT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_PT_MATERIALFILE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_PT_NUMTEXTUNITS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		
		SetWindowLongPtr(GetDlgItem(hDlg, IDC_PROP_BASETEXTURE), GWLP_WNDPROC, (LONG_PTR)ViewerBasePic);
		
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_PT_DIMENSIONS) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_TEXURENAME) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_PT_TEXTURENAME) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_MATERIAL) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_ST_PT_MATERIAL) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_PT_MATERIALFILE) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_PT_NUMTEXTUNITS) == (HWND)lParam)
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
		CheckMenuItem(App->mMenu, ID_WINDOWS_TEXTURESDIALOG, MF_BYCOMMAND | MF_UNCHECKED);
		break;
	}


	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_PT_EXPORT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PT_EXPORT));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}
		}

		if (some_item->idFrom == IDC_BT_PT_VIEWMESH)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PT_VIEWMESH));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				if (App->flag_OgreStarted == 1)
				{
					App->Custom_Button_Toggle(item, App->CL_Ogre->OGL_Listener->Flag_ShowFaces);
				}
			}
		}

		if (some_item->idFrom == IDC_BT_PT_VIEWMAT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			if (App->flag_OgreStarted == 1)
			{
				App->Custom_Button_Normal(item);
			}

		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_PT_VIEWMAT)
		{
			strcpy(App->CL_Resources->mSelected_File, App->CL_Scene->Group[App->CL_Props_Textures->Selected_Group]->Ogre_Material_File);
			App->CL_Resources->View_File(App->CL_Scene->Group[App->CL_Props_Textures->Selected_Group]->Ogre_Material_File, App->Fdlg);

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_PT_EXPORT)
		{
			App->CL_Resources->Export_File(App->CL_Scene->Group[App->CL_Props_Textures->Selected_Group]->Ogre_Texture_FileName);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PT_VIEWMESH)
		{
			if (App->CL_Scene->GroupCount > 0)
			{
				if (App->CL_Ogre->OGL_Listener->Flag_ShowFaces == 1)
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 0;
					App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 0;
				}
				else
				{
					App->CL_Ogre->OGL_Listener->Flag_ShowFaces = 1;
					App->CL_Ogre->OGL_Listener->flag_ShowOnlySubFaces = 1;
					App->CL_Ogre->OGL_Listener->Selected_Face_Group = App->CL_Props_Textures->Selected_Group;
				}
			}
			
			return TRUE;
		}

	}

	}
	return FALSE;
}

// *************************************************************************
// *		Enable_Export_Button:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Props_Textures::Enable_Export_Button(bool Enable)
{
	EnableWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_BT_PT_EXPORT), Enable);
}

// *************************************************************************
// *			ViewerBasePic:- Terry and Hazel Flanigan 2024	  		   *
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
// *		RenderTexture_Blit:- Terry and Hazel Flanigan 2024		  	   *
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
// *			Update_Texture_Assimp:- Terry and Hazel Flanigan 2024      *
// *************************************************************************
bool CL64_Props_Textures::Update_Texture_Assimp()
{
	int Index = Selected_Group;

	strcpy(mMaterialName, App->CL_Scene->Group[Index]->MaterialName);
	strcpy(mTextureName, App->CL_Scene->Group[Index]->Text_FileName);

	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIAL, mMaterialName);
	SetDlgItemText(Props_Dlg_Hwnd, IDC_PT_TEXTURENAME, mTextureName);

	RightGroups_Visable = 1;
	ShowWindow(Props_Dlg_Hwnd, 1);

	CheckMenuItem(App->mMenu, ID_WINDOWS_TEXTURESDIALOG, MF_BYCOMMAND | MF_CHECKED);

	Sel_BaseBitmap = App->CL_Scene->Group[Index]->Base_Bitmap;;

	BITMAP bm;
	GetObject(Sel_BaseBitmap, sizeof(bm), &bm);

	BasePicWidth = bm.bmWidth;
	BasePicHeight = bm.bmHeight;

	char Dimensions[MAX_PATH];
	sprintf(Dimensions, "%i X %i", BasePicWidth, BasePicHeight);// , bm.bmBitsPixel);
	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_DIMENSIONS, Dimensions);

	ShowWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_PROP_BASETEXTURE), 0);
	ShowWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_PROP_BASETEXTURE), 1);
	
	return 1;
}

// *************************************************************************
// *		Update_Texture_Ogre_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Props_Textures::Update_Texture_Ogre_Dlg()
{
	int Index = Selected_Group;

	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIAL, mMaterialName);
	SetDlgItemText(Props_Dlg_Hwnd, IDC_PT_TEXTURENAME, mTextureName);

	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIALFILE, App->CL_Scene->Group[Index]->Ogre_Material_File);

	char NumTextUnits[20];

	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_NUMTEXTUNITS, _itoa(App->CL_Scene->Group[Index]->Ogre_NumTextureUnits, NumTextUnits,10));
	
	RightGroups_Visable = 1;
	ShowWindow(Props_Dlg_Hwnd, 1);
	CheckMenuItem(App->mMenu, ID_WINDOWS_TEXTURESDIALOG, MF_BYCOMMAND | MF_CHECKED);

	BITMAP bm;
	GetObject(Sel_BaseBitmap, sizeof(bm), &bm);

	BasePicWidth = bm.bmWidth;
	BasePicHeight = bm.bmHeight;

	char Dimensions[MAX_PATH];
	sprintf(Dimensions, "%i X %i", BasePicWidth, BasePicHeight);// , bm.bmBitsPixel);
	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_DIMENSIONS, Dimensions);

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

			strcpy(mExport_PathAndName, mFileName);

			std::ofstream outFile;
			outFile.open(mFileName, std::ios::binary);
			outFile << App->CL_Resources->mFileString;
			outFile.close();

			App->CL_Resources->mFileString.clear();

			Texture_To_HBITMP(mFileName);

			Update_Texture_Ogre_Dlg();

			App->CL_Props_Textures->Enable_Export_Button(true);

			remove(mFileName);

			return 1;
		}
	}

	return 0;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Props_Textures::Texture_To_HBITMP(char* TextureFileName)
{
	HWND PreviewWnd = GetDlgItem(App->CL_Dialogs->RightGroups_Hwnd, IDC_BASETEXTURE);
	HDC	hDC = GetDC(PreviewWnd);

	Sel_BaseBitmap = ilutWinLoadImage(TextureFileName, hDC);
}

// *************************************************************************
// *		Get_First_Texture_Ogre:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Props_Textures::Get_First_Texture_Ogre()
{

	if (App->CL_Scene->GroupCount > 0)
	{
		bool test = strcmp(App->CL_Scene->Group[0]->Ogre_Material, "No_Material_Loaded");
		if (test == 0) // Match
		{
			if (App->CL_Scene->GroupCount > 0)
			{
				strcpy(mTextureName, App->CL_Scene->Group[0]->Ogre_Texture_FileName);
				strcpy(mMaterialName, App->CL_Scene->Group[0]->Ogre_Material);
				strcpy(App->CL_Resources->mSelected_File, mTextureName);
			}

			App->CL_Props_Textures->Selected_Group = 0;
			App->CL_Ogre->OGL_Listener->Selected_Face_Group = 0;

			App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";

			App->CL_Props_Textures->View_Texture(mTextureName, mMaterialName);
		}
		else
		{
			strcpy(mTextureName, App->CL_Scene->Group[0]->Ogre_Texture_FileName);
			strcpy(mMaterialName, App->CL_Scene->Group[0]->Ogre_Material);

			bool test = strcmp(mMaterialName, "No_Material_Loaded");
			if (test == 0)
			{
				App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";
			}

			if (App->CL_Scene->Group[0]->Ogre_Texture_IsValid == 1)
			{
				App->CL_Props_Textures->View_Texture(mTextureName, mMaterialName);
			}
			else
			{
				App->CL_Scene->Group[0]->Base_Bitmap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_NO_TEXTURE));
				Sel_BaseBitmap = App->CL_Scene->Group[0]->Base_Bitmap;
				Update_Texture_Ogre_Dlg();
			}

		}
	}
}

