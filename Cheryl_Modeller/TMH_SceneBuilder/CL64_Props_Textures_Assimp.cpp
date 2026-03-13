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
#include "CL64_Props_Textures_Assimp.h"

#include "il.h"
#include "ilu.h"
#include "ilut.h"

CL64_Properties_Textures_Assimp::CL64_Properties_Textures_Assimp(void)
{
	Textures_Dlg_Hwnd_Assimp = NULL;

	Selected_Group = 0;
	
	Sel_BaseBitmap = nullptr;
	BasePicWidth = 0;
	BasePicHeight = 0;

	mExport_PathAndName[0] = 0;
	strcpy(mTextureName, "Texture Name");
	strcpy(mMaterialName, "Material Name");
}

CL64_Properties_Textures_Assimp::~CL64_Properties_Textures_Assimp(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Properties_Textures_Assimp::Reset_Class(void)
{
	Selected_Group = 0;
	
	//BasePicWidth_Ogre = 0;
	//BasePicHeight_Ogre = 0;

	App->CL_Interface->Show_Textures_Assimp_Dlg(false);

	Fill_Textures_ListBox();
	Fill_Materials_ListBox();

	Update_Texture_Ogre_Dlg();
}

// *************************************************************************
// *	 Start_Props_Textures_Dialog:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
bool CL64_Properties_Textures_Assimp::Start_Props_Textures_Dialog()
{
	Textures_Dlg_Hwnd_Assimp = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPERTIES_TEXTURES_ASSIMP, App->MainHwnd, (DLGPROC)Proc_Textures_Dialog);

	//App->CL_Props_Textures->Enable_Export_Button(false);
	App->CL_Interface->Show_Textures_Assimp_Dlg(false);

	return 1;
}

// *************************************************************************
// *			Groups_Proc:- Terry and Hazel Flanigan 2024		  		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Textures_Assimp::Proc_Textures_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LIST_AT_MATERIALS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LIST_AT_TEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_AT_VIEWMAT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_AT_MATERIAL_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_AT_GROUPDETAILS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_AT_CHANGETEXTURE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_AT_DIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_AT_NUMTEXTUNITS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SetWindowLongPtr(GetDlgItem(hDlg, IDC_AT_BASETEXTURE), GWLP_WNDPROC, (LONG_PTR)ViewerBasePic);
		
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_AT_DIMENSIONS) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_ST_AT_NUMTEXTUNITS) == (HWND)lParam)
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
		/*ShowWindow(App->CL_Props_Textures->Props_Dlg_Hwnd, 0);
		App->CL_Props_Textures->RightGroups_Visable = 0;
		CheckMenuItem(App->mMenu, ID_WINDOWS_TEXTURESDIALOG, MF_BYCOMMAND | MF_UNCHECKED);*/
		break;
	}


	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_AT_VIEWMAT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			
			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_FileViewer_Active);
		}

		if (some_item->idFrom == IDC_BT_AT_MATERIAL_FACES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			App->Custom_Button_Toggle(item, App->CL_Ogre->OGL_Listener->flag_Show_Material_Faces);
		}

		if (some_item->idFrom == IDC_BT_AT_GROUPDETAILS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_General_ListBox_Active);
		}

		if (some_item->idFrom == IDC_BT_AT_CHANGETEXTURE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_AT_CHANGETEXTURE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CL_Ogre->OGL_Listener->flag_Show_Material_Faces);
			}
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_LIST_AT_MATERIALS) // Click inside Materials list box
		{
			if (App->CL_Model->flag_Model_Loaded == true && App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
			{
				int Index = SendDlgItemMessage(hDlg, IDC_LIST_AT_MATERIALS, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (Index == LB_ERR)
				{
					App->Say("ListBox No Selection Available", (LPSTR)"");
					return TRUE;
				}

				App->CL_Properties_Textures_Assimp->List_Material_Changed(Index);

			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_AT_MATERIAL_FACES)
		{
			if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
			{
				if (App->CL_Ogre->OGL_Listener->flag_Show_Material_Faces == true)
				{
					App->CL_Ogre->OGL_Listener->flag_Show_Material_Faces = false;
				}
				else
				{
					App->CL_Ogre->OGL_Listener->flag_Show_Material_Faces = true;
				}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_AT_VIEWMAT)
		{
			if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
			{
				if (App->CL_Dialogs->flag_FileViewer_Active == true)
				{
					App->CL_Dialogs->flag_FileViewer_Active = false;
					EndDialog(App->CL_Dialogs->FileViewer_Dlg_Hwnd, LOWORD(wParam));
					return TRUE;
				}
				else
				{
					char MTL_File_And_Path[MAX_PATH];

					strcpy(MTL_File_And_Path, App->CL_Model->Loaded_PathFileName);
					int Len = strlen(MTL_File_And_Path);
					MTL_File_And_Path[Len - 4] = 0;

					strcat(MTL_File_And_Path, ".mtl");

					App->CL_Resources->View_File(MTL_File_And_Path, App->MainHwnd);

				}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_AT_GROUPDETAILS)
		{
			if (App->CL_Dialogs->flag_General_ListBox_Active == true)
			{
				App->CL_Dialogs->flag_General_ListBox_Active = false;

				EndDialog(App->CL_Dialogs->ListBox_Dlg_Hwnd, LOWORD(wParam));
			}
			else
			{
				App->CL_Dialogs->Start_General_ListBox(Enums::ListBox_Mesh_Data);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Interface->Show_Textures_Assimp_Dlg(false);
			return TRUE;
		}
	}

	}
	return FALSE;
}

// *************************************************************************
// *		Enable_Export_Button:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Properties_Textures_Assimp::Enable_Export_Button(bool Enable)
{
	//EnableWindow(GetDlgItem(Props_Dlg_Hwnd, IDC_BT_PT_EXPORT), Enable);
}

// *************************************************************************
// *			ViewerBasePic:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
bool CALLBACK CL64_Properties_Textures_Assimp::ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		if (App->CL_Properties_Textures_Assimp->Sel_BaseBitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = App->CL_Properties_Textures_Assimp->BasePicHeight;
			Source.right = App->CL_Properties_Textures_Assimp->BasePicWidth;

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CL_Properties_Textures_Assimp->RenderTexture_Blit(hDC, App->CL_Properties_Textures_Assimp->Sel_BaseBitmap, &Source, &Dest);
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
bool CL64_Properties_Textures_Assimp::RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect)
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
// *		Fill_Materials_ListBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures_Assimp::Fill_Materials_ListBox()
{
	SendDlgItemMessage(Textures_Dlg_Hwnd_Assimp, IDC_LIST_AT_MATERIALS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	if (App->CL_Model->GroupCount > 0)
	{
		int Count = 0;
		int Size = App->CL_Model->GroupCount;
		while (Count < Size)
		{
			char mName[MAX_PATH];

			strcpy(mName, App->CL_Mesh->Group[Count]->MaterialName);

			SendDlgItemMessage(Textures_Dlg_Hwnd_Assimp, IDC_LIST_AT_MATERIALS, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);

			Count++;
		}

		SendDlgItemMessage(Textures_Dlg_Hwnd_Assimp, IDC_LIST_AT_MATERIALS, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}
}

// *************************************************************************
// *			Update_Texture_BMP:- Terry and Hazel Flanigan 2026	      *
// *************************************************************************
bool CL64_Properties_Textures_Assimp::Update_Texture_BMP()
{
	int Index = Selected_Group;

	strcpy(mMaterialName, App->CL_Mesh->Group[Index]->MaterialName);
	strcpy(mTextureName, App->CL_Mesh->Group[Index]->Assimp_Text_FileName);

	Sel_BaseBitmap = App->CL_Mesh->Group[Index]->Base_Bitmap;

	BITMAP bm;
	GetObject(Sel_BaseBitmap, sizeof(bm), &bm);

	BasePicWidth = bm.bmWidth;
	BasePicHeight = bm.bmHeight;

	char Dimensions[MAX_PATH];
	sprintf(Dimensions, "%i X %i", BasePicWidth, BasePicHeight);// , bm.bmBitsPixel);
	SetDlgItemText(Textures_Dlg_Hwnd_Assimp, IDC_ST_AT_DIMENSIONS, Dimensions);

	ShowWindow(GetDlgItem(Textures_Dlg_Hwnd_Assimp, IDC_AT_BASETEXTURE), 0);
	ShowWindow(GetDlgItem(Textures_Dlg_Hwnd_Assimp, IDC_AT_BASETEXTURE), 1);
	
	return 1;
}

// *************************************************************************
// *			Select_By_Index:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Properties_Textures_Assimp::Select_By_Index(int Index)
{
	SendDlgItemMessage(Textures_Dlg_Hwnd_Assimp, IDC_LIST_AT_MATERIALS, LB_SETCURSEL, (WPARAM)Index, (LPARAM)0);
	List_Material_Changed(Index);
}

// *************************************************************************
// *		Fill_Textures_ListBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures_Assimp::Fill_Textures_ListBox()
{
	SendDlgItemMessage(Textures_Dlg_Hwnd_Assimp, IDC_LIST_AT_TEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	if (App->CL_Model->GroupCount > 0)
	{
		char mName[MAX_PATH];
		strcpy(mName, App->CL_Mesh->Group[Selected_Group]->Assimp_Text_FileName);
		SendDlgItemMessage(Textures_Dlg_Hwnd_Assimp, IDC_LIST_AT_TEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
		
		
		SendDlgItemMessage(Textures_Dlg_Hwnd_Assimp, IDC_LIST_AT_TEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	}
}
// *************************************************************************
// *	  	List_Material_Changed:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures_Assimp::List_Material_Changed(int index)
{
	// Check if the model is loaded and is of type Assimp
	if (App->CL_Model->flag_Model_Loaded && App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		Selected_Group = index;

		// Update texture and fill the textures list box
		Update_Texture_BMP();
		Fill_Textures_ListBox();

		// Set the selected materials/groups in ImGui
		App->CL_ImGui->Set_Materials_Index_Imgui(index);

		// If the file viewer is active, search for the new material
		if (App->CL_Dialogs->flag_FileViewer_Active)
		{
			std::string materialText = "newmtl " + std::string(App->CL_Mesh->Group[Selected_Group]->MaterialName);
			App->CL_Dialogs->Material_Search(materialText.c_str());
		}

		// If the general list box is active, update the mesh data
		if (App->CL_Dialogs->flag_General_ListBox_Active)
		{
			HWND list = GetDlgItem(App->CL_Dialogs->ListBox_Dlg_Hwnd, IDC_LST_GENERAL);
			App->CL_Dialogs->List_Mesh_Data(list);
		}

		// Redraw the textures dialog window
		RedrawWindow(Textures_Dlg_Hwnd_Assimp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

// *************************************************************************
// *		Update_Texture_Ogre_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Properties_Textures_Assimp::Update_Texture_Ogre_Dlg()
{
	int Index = Selected_Group;

//	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIAL, mMaterialName);
//	SetDlgItemText(Props_Dlg_Hwnd, IDC_PT_TEXTURENAME, mTextureName);

//	SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIALFILE, App->CL_Scene->Group[Index]->Ogre_Material_File);

	//char NumTextUnits[20];

	//SetDlgItemText(Textures_Dlg_Hwnd_Assimp, IDC_ST_PT_NUMTEXTUNITS, _itoa(App->CL_Mesh->Group[Index]->Ogre_NumTextureUnits, NumTextUnits,10));
	
	//ShowWindow(Textures_Dlg_Hwnd_Assimp, true);
//	CheckMenuItem(App->mMenu, ID_WINDOWS_TEXTURESDIALOG, MF_BYCOMMAND | MF_CHECKED);

	BITMAP bm;
	GetObject(Sel_BaseBitmap, sizeof(bm), &bm);

	BasePicWidth = bm.bmWidth;
	BasePicHeight = bm.bmHeight;

	char Dimensions[MAX_PATH];
	sprintf(Dimensions, "%i X %i", BasePicWidth, BasePicHeight);// , bm.bmBitsPixel);
	SetDlgItemText(Textures_Dlg_Hwnd_Assimp, IDC_ST_AT_DIMENSIONS, Dimensions);

	ShowWindow(GetDlgItem(Textures_Dlg_Hwnd_Assimp, IDC_AT_BASETEXTURE), 0);
	ShowWindow(GetDlgItem(Textures_Dlg_Hwnd_Assimp, IDC_AT_BASETEXTURE), 1);

	return 1;
}

// *************************************************************************
// *				View_Texture:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Properties_Textures_Assimp::View_Texture(char* TextureName, char* MaterialName)
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
			strcpy(mFileName, App->RB_Directory_FullPath);
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

			//App->CL_Props_Textures->Enable_Export_Button(true);

			remove(mFileName);

			return 1;
		}
	}

	return 0;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Properties_Textures_Assimp::Texture_To_HBITMP(char* TextureFileName)
{
	//HWND PreviewWnd = GetDlgItem(App->CL_Dialogs->RightGroups_Hwnd, IDC_BASETEXTURE);
	//HDC	hDC = GetDC(PreviewWnd);

	//Sel_BaseBitmap = ilutWinLoadImage(TextureFileName, hDC);
}

// *************************************************************************
// *		Get_First_Texture_Ogre:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Properties_Textures_Assimp::Get_First_Texture_Ogre()
{

	//if (App->CL_Scene->GroupCount > 0)
	//{
	//	bool test = strcmp(App->CL_Mesh->Group[0]->Ogre_Material, "No_Material_Loaded");
	//	if (test == 0) // Match
	//	{
	//		if (App->CL_Scene->GroupCount > 0)
	//		{
	//			//strcpy(mTextureName, App->CL_Mesh->Group[0]->Ogre_Texture_FileName);
	//			strcpy(mMaterialName, App->CL_Mesh->Group[0]->Ogre_Material);
	//			//strcpy(App->CL_Resources->mSelected_Resource_Group, mTextureName);
	//		}

	//		App->CL_Properties_Textures_Assimp->Selected_Group = 0;
	//		App->CL_Ogre->OGL_Listener->Selected_Face_Group = 0;

	//		App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";

	//		App->CL_Properties_Textures_Assimp->View_Texture(mTextureName, mMaterialName);
	//	}
	//	else
	//	{
	//		//strcpy(mTextureName, App->CL_Mesh->Group[0]->Ogre_Texture_FileName);
	//		strcpy(mMaterialName, App->CL_Mesh->Group[0]->Ogre_Material);

	//		bool test = strcmp(mMaterialName, "No_Material_Loaded");
	//		if (test == 0)
	//		{
	//			App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";
	//		}

	//		if (App->CL_Mesh->Group[0]->Ogre_Texture_IsValid == 1)
	//		{
	//			App->CL_Properties_Textures_Assimp->View_Texture(mTextureName, mMaterialName);
	//		}
	//		else
	//		{
	//			App->CL_Mesh->Group[0]->Base_Bitmap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_NO_TEXTURE));
	//			Sel_BaseBitmap = App->CL_Mesh->Group[0]->Base_Bitmap;
	//			Update_Texture_Ogre_Dlg();
	//		}

	//	}

	Selected_Group = 0;
	Fill_Textures_ListBox();

}

