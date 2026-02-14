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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Properties_Materials.h"

CL64_Properties_Materials::CL64_Properties_Materials(void)
{
	strcpy(m_CurrentTexture_Ogre, "stfloor1");
	strcpy(m_MaterialName_Ogre, "Material Name");

	Sel_BaseBitmap_Ogre = NULL;
	BasePicWidth_Ogre = 0;
	BasePicHeight_Ogre = 0;

	Selected_Material_Index = 0;
	Selected_Group = 0;

	mFileString_Ogre.clear();

	Textures_Dlg_Hwnd_Ogre = nullptr;
	mExport_PathAndName_Ogre[0] = 0;
}

CL64_Properties_Materials::~CL64_Properties_Materials(void)
{
}

// *************************************************************************
// *	  		Reset_Class:- Terry and Hazel Flanigan 2026				   *
// *************************************************************************
void CL64_Properties_Materials::Reset_Class()
{
	Selected_Group = 0;
	Selected_Material_Index = 0;
	Sel_BaseBitmap_Ogre = NULL;
	mFileString_Ogre.clear();
	BasePicWidth_Ogre = 0;
	BasePicHeight_Ogre = 0;

	Fill_Textures_ListBox();
	Fill_Materials_ListBox();

	Update_Texture_Ogre_Dlg();
}

// *************************************************************************
// *	  	Show_Textures_Dialog_Ogre:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Properties_Materials::Show_Textures_Dialog_Ogre(bool Show)
{
	ShowWindow(Textures_Dlg_Hwnd_Ogre, Show);
}

// *************************************************************************
// *	  	Start_TextureDialog_Ogre:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Properties_Materials::Start_TextureDialog_Ogre()
{
	Textures_Dlg_Hwnd_Ogre = CreateDialog(App->hInst, (LPCTSTR)IDD_TEXTURES_OGRE, App->MainHwnd, (DLGPROC)Proc_TextureDialog_Ogre);
	Show_Textures_Dialog_Ogre(false);
}

// *************************************************************************
// *        Proc_TextureDialog_Ogre:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Materials::Proc_TextureDialog_Ogre(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LIST_MATERIALS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LIST_TEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
	
		SendDlgItemMessage(hDlg, IDC_BT_PT_VIEWMAT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MATERIAL_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_PT_DIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_PT_NUMTEXTUNITS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		
		SetWindowLongPtr(GetDlgItem(hDlg, IDC_PROP_BASETEXTURE_OGRE), GWLP_WNDPROC, (LONG_PTR)ViewerBasePic_Ogre);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_PT_DIMENSIONS) == (HWND)lParam)
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

	/*case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
	}*/

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_PT_VIEWMAT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			//if (App->flag_OgreStarted == 1)
			{
				App->Custom_Button_Normal(item);
			}
		}

		if (some_item->idFrom == IDC_BT_MATERIAL_FACES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			//if (App->flag_OgreStarted == 1)
			{
				App->Custom_Button_Toggle(item, App->CL_Ogre->OGL_Listener->flag_Show_Material_Faces);
			}
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_LIST_MATERIALS)
		{
			App->CL_Properties_Materials->List_Material_Changed();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PT_VIEWMAT)
		{
			if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
			{
				if (App->CL_Dialogs->flag_FileViewer_Active == true)
				{
					return TRUE;
				}
				else
				{
					//strcpy(App->CL_Resources->mSelected_File, App->CL_Mesh->Group[App->CL_Props_Textures->Selected_Group]->Ogre_Material_File);
					App->CL_Resources->View_File(App->CL_Mesh->Group[App->CL_Properties_Materials->Selected_Group]->Ogre_Material_File, App->MainHwnd);
				}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MATERIAL_FACES)
		{
			if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
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

		break;
	}

	}
	return FALSE;
}

// *************************************************************************
// *			ViewerBasePic_Ogre:- Terry and Hazel Flanigan 2026	  	   *
// *************************************************************************
bool CALLBACK CL64_Properties_Materials::ViewerBasePic_Ogre(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hwnd, &ps);
		RECT clientRect;

		// Get the client rectangle and adjust its dimensions
		GetClientRect(hwnd, &clientRect);
		clientRect.left--;
		clientRect.bottom--;

		// Fill the rectangle with a green brush
		FillRect(hDC, &clientRect, (HBRUSH)(RGB(0, 255, 0)));

		// Check if a base bitmap is selected
		if (App->CL_Properties_Materials->Sel_BaseBitmap_Ogre != nullptr)
		{
			RECT sourceRect = { 0, 0, App->CL_Properties_Materials->BasePicWidth_Ogre, App->CL_Properties_Materials->BasePicHeight_Ogre };
			RECT destRect = clientRect;

			// Get the device context and set the stretch mode
			HDC renderDC = GetDC(hwnd);
			SetStretchBltMode(renderDC, HALFTONE);

			// Render the texture
			App->CL_Properties_Materials->RenderTexture_Blit_Ogre(renderDC, App->CL_Properties_Materials->Sel_BaseBitmap_Ogre, &sourceRect, &destRect);
			ReleaseDC(hwnd, renderDC);
		}

		EndPaint(hwnd, &ps);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// *************************************************************************
// *		RenderTexture_Blit_Ogre:- Terry and Hazel Flanigan 2026	 	   *
// *************************************************************************
bool CL64_Properties_Materials::RenderTexture_Blit_Ogre(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect)
{
	HDC MemDC = CreateCompatibleDC(hDC);
	if (MemDC == NULL)
	{
		return FALSE;
	}

	// Check if the bitmap is valid
	if (Bmp)
	{
		SelectObject(MemDC, Bmp);

		int SourceWidth = SourceRect->right - SourceRect->left;
		int SourceHeight = SourceRect->bottom - SourceRect->top;
		int DestWidth = DestRect->right - DestRect->left;
		int DestHeight = DestRect->bottom - DestRect->top;
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
// *		Get_First_Texture_Ogre:- Terry and Hazel Flanigan 2026	 	   *
// *************************************************************************
void CL64_Properties_Materials::Get_First_Texture_Ogre()
{
	if (App->CL_Model->GroupCount > 0)
	{
		bool test = strcmp(App->CL_Mesh->Group[0]->Ogre_Material, "No_Material_Loaded");
		if (test == 0) // Match
		{
			if (App->CL_Model->GroupCount > 0)
			{
				strcpy(m_CurrentTexture_Ogre, App->CL_Mesh->Group[0]->Ogre_Texture_FileName);
				strcpy(m_MaterialName_Ogre, App->CL_Mesh->Group[0]->Ogre_Material);
				//strcpy(App->CL_Resources->mSelected_File, m_CurrentTexture);
			}

			App->CL_Properties_Materials->Selected_Material_Index = 0;
			App->CL_Properties_Materials->Selected_Group = 0;
			App->CL_Ogre->OGL_Listener->Selected_Face_Group = 0;

			App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";

			View_Texture(m_CurrentTexture_Ogre, m_MaterialName_Ogre);
			App->CL_Properties_Materials->Fill_Textures_ListBox();
			Update_Texture_Ogre_Dlg();
		}
		else
		{
			strcpy(m_CurrentTexture_Ogre, App->CL_Mesh->Group[0]->Ogre_Texture_FileName);
			strcpy(m_MaterialName_Ogre, App->CL_Mesh->Group[0]->Ogre_Material);

			bool test = strcmp(m_MaterialName_Ogre, "No_Material_Loaded");
			if (test == 0)
			{
				App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";
			}

			if (App->CL_Mesh->Group[0]->Ogre_Texture_IsValid == 1)
			{
				View_Texture(m_CurrentTexture_Ogre, m_MaterialName_Ogre);
				App->CL_Properties_Materials->Fill_Textures_ListBox();
				Update_Texture_Ogre_Dlg();
			}
			else
			{
				App->CL_Mesh->Group[0]->Base_Bitmap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_NO_TEXTURE));
				Sel_BaseBitmap_Ogre = App->CL_Mesh->Group[0]->Base_Bitmap;
				Update_Texture_Ogre_Dlg();
			}

		}
	}
}

// *************************************************************************
// *				View_Texture:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
bool CL64_Properties_Materials::View_Texture(char* TextureName, char* MaterialName)
{
	strcpy(m_CurrentTexture_Ogre, TextureName);
	strcpy(m_MaterialName_Ogre, MaterialName);

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

			strcpy(mExport_PathAndName_Ogre, mFileName);

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
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2026	 	   *
// *************************************************************************
void CL64_Properties_Materials::Texture_To_HBITMP(char* TextureFileName)
{
	// Retrieve the handle for the preview window
	HWND previewWnd = GetDlgItem(Textures_Dlg_Hwnd_Ogre, IDC_PROP_BASETEXTURE_OGRE);

	// Obtain the device context for the preview window
	HDC hdc = GetDC(previewWnd);

	// Load the bitmap texture using the application texture manager
	Sel_BaseBitmap_Ogre = App->CL_Textures->Get_HBITMP(TextureFileName, hdc);

	// Store the dimensions of the base picture
	BasePicWidth_Ogre = App->CL_Textures->BasePicWidth;
	BasePicHeight_Ogre = App->CL_Textures->BasePicHeight;

	// Release the device context
	ReleaseDC(previewWnd, hdc);

	// Redraw the preview window to reflect the updated texture
	RedrawWindow(previewWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			SelectBitmap:- Terry and Hazel Flanigan 2026		  	   *
// *************************************************************************
bool CL64_Properties_Materials::SelectBitmap()
{
	char mTextureName[MAX_PATH];
	int TrueIndex = App->CL_TXL_Editor->GetIndex_From_Name(m_CurrentTexture_Ogre);
	strcpy(mTextureName, App->CL_TXL_Editor->Texture_List[TrueIndex]->FileName);
	//App->Say(mTextureName);

	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	for (i = RFI->begin(); i != iend; ++i)
	{
		if (i->filename == mTextureName)
		{
			Ogre::DataStreamPtr ff = i->archive->open(i->filename);

			mFileString_Ogre = ff->getAsString();

			char mFileName[MAX_PATH];
			strcpy(mFileName, App->RB_Directory_FullPath);
			strcat(mFileName, "\\Data\\");
			strcat(mFileName, mTextureName);

			std::ofstream outFile;
			outFile.open(mFileName, std::ios::binary);
			outFile << mFileString_Ogre;
			outFile.close();

			mFileString_Ogre.clear();

			Texture_To_HBITMP(mFileName);
			remove(mFileName);
			return 1;
		}
	}

	return 0;
}

// *************************************************************************
// *		Update_Texture_Ogre_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
bool CL64_Properties_Materials::Update_Texture_Ogre_Dlg()
{
	int Index = Selected_Group;

	//SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIAL, mMaterialName);
	//SetDlgItemText(Props_Dlg_Hwnd, IDC_PT_TEXTURENAME, mTextureName);

	//SetDlgItemText(Props_Dlg_Hwnd, IDC_ST_PT_MATERIALFILE, App->CL_Scene->Group[Index]->Ogre_Material_File);

	if (App->CL_Model->GroupCount > 0)
	{
		char NumTextUnits[20];
		sprintf(NumTextUnits, "%s %i","Texture Units", App->CL_Mesh->Group[Index]->Ogre_NumTextureUnits);// , bm.bmBitsPixel);
		SetDlgItemText(Textures_Dlg_Hwnd_Ogre, IDC_ST_PT_NUMTEXTUNITS, NumTextUnits);
	}

	//RightGroups_Visable = 1;
	//ShowWindow(Props_Dlg_Hwnd, 1);
	//CheckMenuItem(App->mMenu, ID_WINDOWS_TEXTURESDIALOG, MF_BYCOMMAND | MF_CHECKED);

	BITMAP bm;
	GetObject(Sel_BaseBitmap_Ogre, sizeof(bm), &bm);

	BasePicWidth_Ogre = bm.bmWidth;
	BasePicHeight_Ogre = bm.bmHeight;

	char Dimensions[MAX_PATH];
	sprintf(Dimensions, "%i X %i", BasePicWidth_Ogre, BasePicHeight_Ogre);// , bm.bmBitsPixel);
	SetDlgItemText(Textures_Dlg_Hwnd_Ogre, IDC_ST_PT_DIMENSIONS, Dimensions);

	ShowWindow(GetDlgItem(Textures_Dlg_Hwnd_Ogre, IDC_PROP_BASETEXTURE_OGRE), 0);
	ShowWindow(GetDlgItem(Textures_Dlg_Hwnd_Ogre, IDC_PROP_BASETEXTURE_OGRE), 1);

	return 1;
}

// *************************************************************************
// *	  	List_Material_Changed:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Properties_Materials::List_Material_Changed()
{
	if (App->CL_Model->flag_Model_Loaded == true && App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		int Index = SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_MATERIALS, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		if (Index == LB_ERR)
		{
			App->Say("ListBox No Selection Available", (LPSTR)"");
		}
		else
		{
			char mMaterial[MAX_PATH];
			char Texture[MAX_PATH];
			strcpy(Texture, App->CL_Mesh->Group[Index]->Ogre_Texture_FileName);
			strcpy(mMaterial, App->CL_Mesh->Group[Index]->Ogre_Material);

			strcpy(m_CurrentTexture_Ogre, Texture);

			App->CL_Properties_Materials->View_Texture(Texture, mMaterial);
			
			if (App->CL_Mesh->Group[Index]->Ogre_Texture_IsValid == true)
			{
				App->CL_Properties_Materials->View_Texture(Texture, mMaterial);
			}
			else
			{
				App->CL_Properties_Materials->Sel_BaseBitmap_Ogre = App->CL_Mesh->Group[Index]->Base_Bitmap;
				App->CL_Properties_Materials->Update_Texture_Ogre_Dlg();
			}
		}

		if (Index < 40)
		{
			App->CL_ImGui->listMaterialItems_Ogre[App->CL_ImGui->PreviouseMaterial_Ogre] = 0;
			App->CL_ImGui->listMaterialItems_Ogre[Index] = 1;
			App->CL_ImGui->PreviouseMaterial_Ogre = Index;
		}

		Selected_Material_Index = Index;
		Selected_Group = Index;

		Fill_Textures_ListBox();

		if (App->CL_Dialogs->flag_FileViewer_Active == true)
		{
			char Text[MAX_PATH];
			strcpy(Text, "material ");

			strcat(Text, App->CL_Mesh->Group[Selected_Group]->Ogre_Material);
			App->CL_Dialogs->Material_Search((LPSTR)Text);
		}
	}

}
// *************************************************************************
// *			Select_By_Index:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Properties_Materials::Select_By_Index(int Index)
{
	SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_MATERIALS, LB_SETCURSEL, (WPARAM)Index, (LPARAM)0);
}

// *************************************************************************
// *		Fill_Materials_ListBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Materials::Fill_Materials_ListBox()
{
	SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_MATERIALS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	if (App->CL_Model->GroupCount > 0)
	{
		int Count = 0;
		int Size = App->CL_Model->GroupCount;
		while (Count < Size)
		{
			char mName[MAX_PATH];

			strcpy(mName, App->CL_Mesh->Group[Count]->Ogre_Material);

			SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_MATERIALS, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);

			Count++;
		}

		SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_MATERIALS, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}

}

// *************************************************************************
// *		Fill_Textures_ListBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Materials::Fill_Textures_ListBox()
{
	SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_TEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	if (App->CL_Model->GroupCount > 0)
	{
		char mName[MAX_PATH];
		strcpy(mName, App->CL_Mesh->Group[Selected_Material_Index]->Ogre_Texture_FileName);

		SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_TEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);

		SendDlgItemMessage(Textures_Dlg_Hwnd_Ogre, IDC_LIST_TEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}
}

