/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
#include "CL64_App.h"
#include "resource.h"
#include "CL64_Properties_Textures_Com.h"

CL64_Properties_Textures_Com::CL64_Properties_Textures_Com(void)
{
	Textures_Dlg_Hwnd_Assimp =	nullptr;
	Textures_Dlg_Hwnd_Ogre =	nullptr;

	Selected_Group = 0;

	BasePicWidth = 0;
	BasePicHeight = 0;

	m_Current_TextureName[0] = 0;
	m_Current_MaterialName[0] = 0;
	m_Export_PathAndName[0] = 0;

	Sel_BaseBitmap = NULL;

}

CL64_Properties_Textures_Com::~CL64_Properties_Textures_Com(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2026				   *
// *************************************************************************
void CL64_Properties_Textures_Com::Reset_Class(void)
{
	Selected_Group = 0;

	BasePicWidth = 0;
	BasePicHeight = 0;

	Sel_BaseBitmap = NULL;

	//App->CL_Interface->Show_Textures_Assimp_Dlg(false);

	//App->CL_Properties_Textures_Com->Fill_Textures_ListBox();
	Fill_Materials_ListBox();

	//Update_Texture_Ogre_Dlg();
}

// *************************************************************************
// *	 Start_Props_Textures_Dialog:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
bool CL64_Properties_Textures_Com::Start_Props_Textures_Dialog()
{
	Textures_Dlg_Hwnd_Assimp = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPERTIES_TEXTURES_ASSIMP, App->MainHwnd, (DLGPROC)Proc_Textures_Dialog);

	//App->CL_Props_Textures->Enable_Export_Button(false);
	App->CL_Interface->Show_Textures_Assimp_Dlg(false);

	return 1;
}

// *************************************************************************
// *		Proc_Textures_Dialog:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Textures_Com::Proc_Textures_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

				App->CL_Properties_Textures_Com->List_Material_Changed(Index);

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
// *			ViewerBasePic:- Terry and Hazel Flanigan 2026	  		   *
// *************************************************************************
bool CALLBACK CL64_Properties_Textures_Com::ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		if (App->CL_Properties_Textures_Com->Sel_BaseBitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = App->CL_Properties_Textures_Com->BasePicHeight;
			Source.right = App->CL_Properties_Textures_Com->BasePicWidth;

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CL_Properties_Textures_Com->RenderTexture_Blit(hDC, App->CL_Properties_Textures_Com->Sel_BaseBitmap, &Source, &Dest);
			ReleaseDC(hwnd, hDC);
		}

		EndPaint(hwnd, &ps);
		return 0;
	}
	return 0;
}

// *************************************************************************
// *		RenderTexture_Blit:- Terry and Hazel Flanigan 2026		  	   *
// *************************************************************************
bool CL64_Properties_Textures_Com::RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect)
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
// *			Get_First_Texture:- Terry and Hazel Flanigan 2026	 	   *
// *************************************************************************
void CL64_Properties_Textures_Com::Get_First_Texture()
{
	// --------------------------------------------------------------------- Assimp
	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		App->CL_Properties_Textures_Com->Selected_Group = 0;
		App->CL_Properties_Textures_Com->Fill_Textures_ListBox();
	}

	// --------------------------------------------------------------------- Ogre3D
	if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		if (App->CL_Model->GroupCount > 0)
		{
			bool test = strcmp(App->CL_Mesh->Group[0]->Ogre_Material, "No_Material_Loaded");
			if (test == 0) // Match
			{
				if (App->CL_Model->GroupCount > 0)
				{
					strcpy(App->CL_Properties_Textures_Com->m_Current_TextureName, App->CL_Mesh->Group[0]->v_Texture_Names[0].c_str());
					strcpy(m_Current_MaterialName, App->CL_Mesh->Group[0]->Ogre_Material);
					//strcpy(App->CL_Resources->mSelected_File, m_CurrentTexture);
				}

				App->CL_Properties_Textures_Com->Selected_Group = 0;
				App->CL_Ogre->OGL_Listener->Selected_Face_Group = 0;

				App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";

				View_Texture(App->CL_Properties_Textures_Com->m_Current_TextureName, m_Current_MaterialName);
				App->CL_Properties_Textures_Com->Fill_Textures_ListBox();
				App->CL_Properties_Textures_Com->Update_Dlg_Bmp_Texture();
			}
			else
			{
				strcpy(App->CL_Properties_Textures_Com->m_Current_TextureName, App->CL_Mesh->Group[0]->v_Texture_Names[0].c_str());
				strcpy(m_Current_MaterialName, App->CL_Mesh->Group[0]->Ogre_Material);

				bool test = strcmp(m_Current_MaterialName, "No_Material_Loaded");
				if (test == 0)
				{
					App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";
				}

				if (App->CL_Mesh->Group[0]->Ogre_Texture_IsValid == 1)
				{
					View_Texture(App->CL_Properties_Textures_Com->m_Current_TextureName, m_Current_MaterialName);
					App->CL_Properties_Textures_Com->Fill_Textures_ListBox();
					App->CL_Properties_Textures_Com->Update_Dlg_Bmp_Texture();
				}
				else
				{
					App->CL_Mesh->Group[0]->Base_Bitmap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_NO_TEXTURE));
					App->CL_Properties_Textures_Com->Sel_BaseBitmap = App->CL_Mesh->Group[0]->Base_Bitmap;
					App->CL_Properties_Textures_Com->Update_Dlg_Bmp_Texture();
				}

			}
		}
	}
}

// *************************************************************************
// *				View_Texture:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
bool CL64_Properties_Textures_Com::View_Texture(char* TextureName, char* MaterialName)
{
	strcpy(App->CL_Properties_Textures_Com->m_Current_TextureName, TextureName);
	strcpy(m_Current_MaterialName, MaterialName);

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

			strcpy(m_Export_PathAndName, mFileName);

			std::ofstream outFile;
			outFile.open(mFileName, std::ios::binary);
			outFile << App->CL_Resources->mFileString;
			outFile.close();

			App->CL_Resources->mFileString.clear();

			Texture_To_HBITMP(mFileName);

			App->CL_Properties_Textures_Com->Update_Dlg_Bmp_Texture();

			remove(mFileName);

			return 1;
		}
	}

	return 0;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2026	 	   *
// *************************************************************************
void CL64_Properties_Textures_Com::Texture_To_HBITMP(char* TextureFileName)
{
	// Retrieve the handle for the preview window
	HWND previewWnd = GetDlgItem(App->CL_Properties_Textures_Com->Textures_Dlg_Hwnd_Ogre, IDC_PROP_BASETEXTURE_OGRE);

	// Obtain the device context for the preview window
	HDC hdc = GetDC(previewWnd);

	// Load the bitmap texture using the application texture manager
	App->CL_Properties_Textures_Com->Sel_BaseBitmap = App->CL_Textures->Get_HBITMP(TextureFileName, hdc);

	// Store the dimensions of the base picture
	App->CL_Properties_Textures_Com->BasePicWidth = App->CL_Textures->BasePicWidth;
	App->CL_Properties_Textures_Com->BasePicHeight = App->CL_Textures->BasePicHeight;

	// Release the device context
	ReleaseDC(previewWnd, hdc);

	// Redraw the preview window to reflect the updated texture
	RedrawWindow(previewWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *		Fill_Textures_ListBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures_Com::Fill_Textures_ListBox()
{
	// --------------------------------------------------------------------- Assimp
	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		auto& m_Textures_Dlg_hWnd = Textures_Dlg_Hwnd_Assimp;

		SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_AT_TEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		if (App->CL_Model->GroupCount > 0)
		{
			char mName[MAX_PATH];
			strcpy(mName, App->CL_Mesh->Group[Selected_Group]->Assimp_Text_FileName);
			SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_AT_TEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);


			SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_AT_TEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		}
	}

	// --------------------------------------------------------------------- Ogre
	if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		auto& m_Textures_Dlg_hWnd = Textures_Dlg_Hwnd_Ogre;

		SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_TEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		if (App->CL_Model->GroupCount > 0)
		{
			char mName[MAX_PATH];

			int Count = 0;
			while (Count < App->CL_Mesh->Group[Selected_Group]->Ogre_NumTextureUnits)
			{
				strcpy(mName, App->CL_Mesh->Group[Selected_Group]->v_Texture_Names[Count].c_str());

				SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_TEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
				Count++;
			}

			SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_TEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		}
	}
}

// *************************************************************************
// *		Fill_Materials_ListBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures_Com::Fill_Materials_ListBox()
{
	// --------------------------------------------------------------------- Assimp
	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		auto& m_Textures_Dlg_hWnd = Textures_Dlg_Hwnd_Assimp;

		SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_AT_MATERIALS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		if (App->CL_Model->GroupCount > 0)
		{
			int Count = 0;
			int Size = App->CL_Model->GroupCount;
			while (Count < Size)
			{
				char mName[MAX_PATH];

				strcpy(mName, App->CL_Mesh->Group[Count]->MaterialName);

				SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_AT_MATERIALS, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);

				Count++;
			}

			SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_AT_MATERIALS, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		}
	}

	// --------------------------------------------------------------------- Ogre
	if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		auto& m_Textures_Dlg_hWnd = Textures_Dlg_Hwnd_Ogre;

		SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_MATERIALS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		if (App->CL_Model->GroupCount > 0)
		{
			int Count = 0;
			int Size = App->CL_Model->GroupCount;
			while (Count < Size)
			{
				char mName[MAX_PATH];

				strcpy(mName, App->CL_Mesh->Group[Count]->Ogre_Material);

				SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_MATERIALS, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);

				Count++;
			}

			SendDlgItemMessage(m_Textures_Dlg_hWnd, IDC_LIST_MATERIALS, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		}
	}
}

// *************************************************************************
// *		Update_Dlg_Bmp_Texture:- Terry and Hazel Flanigan 2026	      *
// *************************************************************************
void CL64_Properties_Textures_Com::Update_Dlg_Bmp_Texture()
{
	// --------------------------------------------------------------------- Assimp
	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		auto& m_Textures_Class = App->CL_Properties_Textures_Assimp;

		int Index = Selected_Group;

		strcpy(m_Current_MaterialName, App->CL_Mesh->Group[Index]->MaterialName);
		strcpy(m_Current_TextureName, App->CL_Mesh->Group[Index]->Assimp_Text_FileName);

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
	}

	// --------------------------------------------------------------------- Ogre
	if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		auto& m_Textures_Class = App->CL_Properties_Materials;

		int Index = Selected_Group;

		if (App->CL_Model->GroupCount > 0)
		{
			char NumTextUnits[20];
			sprintf(NumTextUnits, "%s %i", "Texture Units", App->CL_Mesh->Group[Index]->Ogre_NumTextureUnits);// , bm.bmBitsPixel);
			SetDlgItemText(Textures_Dlg_Hwnd_Ogre, IDC_ST_PT_NUMTEXTUNITS, NumTextUnits);
		}

		BITMAP bm;
		GetObject(Sel_BaseBitmap, sizeof(bm), &bm);

		BasePicWidth = bm.bmWidth;
		BasePicHeight = bm.bmHeight;

		char Dimensions[MAX_PATH];
		sprintf(Dimensions, "%i X %i", BasePicWidth, BasePicHeight);// , bm.bmBitsPixel);
		SetDlgItemText(Textures_Dlg_Hwnd_Ogre, IDC_ST_PT_DIMENSIONS, Dimensions);

		ShowWindow(GetDlgItem(Textures_Dlg_Hwnd_Ogre, IDC_PROP_BASETEXTURE_OGRE), 0);
		ShowWindow(GetDlgItem(Textures_Dlg_Hwnd_Ogre, IDC_PROP_BASETEXTURE_OGRE), 1);

		RedrawWindow(Textures_Dlg_Hwnd_Ogre, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

// *************************************************************************
// *	  	List_Material_Changed:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures_Com::List_Material_Changed(int index)
{
	// Check if the model is loaded and is of type Assimp
	if (App->CL_Model->flag_Model_Loaded && App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		Selected_Group = index;

		SendDlgItemMessage(Textures_Dlg_Hwnd_Assimp, IDC_LIST_AT_MATERIALS, LB_SETCURSEL, (WPARAM)index, (LPARAM)0);

		// Update texture and fill the textures list box
		Update_Dlg_Bmp_Texture();
		Fill_Textures_ListBox();

		// Set the selected materials/groups in ImGui
		App->CL_ImGui->Set_Materials_Index_Imgui(index);

		// If the file viewer is active, search for the new material
		if (App->CL_Dialogs->flag_FileViewer_Active)
		{
			std::string materialText = "newmtl " + std::string(App->CL_Mesh->Group[App->CL_Properties_Textures_Com->Selected_Group]->MaterialName);
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

	if (App->CL_Model->flag_Model_Loaded == true && App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		Selected_Group = index;

		SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_MATERIALS, LB_SETCURSEL, (WPARAM)index, (LPARAM)0);


		// For Material Search
		char mMaterial[MAX_PATH];
		char Texture[MAX_PATH];
		strcpy(Texture, App->CL_Mesh->Group[index]->v_Texture_Names[0].c_str());
		strcpy(mMaterial, App->CL_Mesh->Group[index]->Ogre_Material);
		strcpy(m_Current_TextureName, Texture);
		View_Texture(Texture, mMaterial);



		if (App->CL_Mesh->Group[index]->Ogre_Texture_IsValid == true)
		{
			View_Texture(Texture, mMaterial);
			Update_Dlg_Bmp_Texture();
		}
		else
		{
			Sel_BaseBitmap = App->CL_Mesh->Group[index]->Base_Bitmap;
			Update_Dlg_Bmp_Texture();
		}

		// Select Materials/Groups in Imgu
		App->CL_ImGui->Set_Materials_Index_Imgui(index);

		Fill_Textures_ListBox();

		if (App->CL_Dialogs->flag_FileViewer_Active == true)
		{
			std::string materialText = "material " + std::string(App->CL_Mesh->Group[App->CL_Properties_Textures_Com->Selected_Group]->Ogre_Material);
			App->CL_Dialogs->Material_Search(materialText.c_str());
		}

		if (App->CL_Dialogs->flag_General_ListBox_Active == true)
		{
			HWND List = GetDlgItem(App->CL_Dialogs->ListBox_Dlg_Hwnd, IDC_LST_GENERAL);
			App->CL_Dialogs->List_Mesh_Data(List);
		}

		Update_Dlg_Bmp_Texture();
		RedrawWindow(Textures_Dlg_Hwnd_Ogre, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

