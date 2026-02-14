/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
#include "CL64_Properties_Textures.h"

CL64_Properties_Textures::CL64_Properties_Textures()
{
	Textures_Dlg_Hwnd = nullptr;

	mExport_PathAndName[0] = 0;
	strcpy(m_CurrentTexture, "stfloor1");
	strcpy(m_MaterialName, "Material Name");

	Dialog_Created = 0;
	Dialog_Textures_Visible = 0;

	Selected_Index = 0;
	Sel_BaseBitmap = NULL;
	BasePicWidth = 0;
	BasePicHeight = 0;

	mSelected_Face = NULL;
}

CL64_Properties_Textures::~CL64_Properties_Textures()
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Properties_Textures::Reset_Class()
{
	App->CL_Properties_Textures->Enable_FaceProps_Button(false);
}


// *************************************************************************
// *	  	Show_Textures_Dialog:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Textures::Show_Textures_Dialog(bool Show)
{
	ShowWindow(Textures_Dlg_Hwnd, Show);
	Dialog_Textures_Visible = Show;

	Get_Selected_Face();
}

// *************************************************************************
// *			Init_Bmps_Globals:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Properties_Textures::Init_Bmps_Globals(void)
{
	HWND Temp = GetDlgItem(Textures_Dlg_Hwnd, IDC_BT_HELP);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_Help_Bmp);

	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOFADE, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);
	SendMessage(hTooltip_TB_2, TTM_SETMAXTIPWIDTH, 0, 250);

	Temp = GetDlgItem(Textures_Dlg_Hwnd, IDC_BT_HELP);
	TOOLINFO ti1 = { 0 };
	ti1.cbSize = sizeof(ti1);
	ti1.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti1.uId = (UINT_PTR)Temp;
	ti1.lpszText = (LPSTR)"Help / Information.";
	ti1.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti1);
}

// *************************************************************************
// *	  	Start_TextureDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Start_TextureDialog()
{
	Textures_Dlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEXTURES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_TextureDialog);
	Init_Bmps_Globals();

	Dialog_Created = 1;
	Fill_ListBox();
	SelectBitmap();
}

// *************************************************************************
// *        Proc_TextureDialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Textures::Proc_TextureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_GD_TEXTURES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_LISTTDTEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTDAPPLY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TXL_FILE_EDIT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TEXTURE_FACE_PROPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STWIDTHHEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_FACESALL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		//App->CLSB_TextureDialog->f_TextureDlg_Active = 1;

		SetWindowLongPtr(GetDlgItem(hDlg, IDC_BASETEXTURE2), GWLP_WNDPROC, (LONG_PTR)ViewerBasePic);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_GD_TEXTURES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STWIDTHHEIGHT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_FACESALL) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
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

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BTTDAPPLY)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BTTDAPPLY));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TXL_FILE_EDIT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TEXTURE_FACE_PROPS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TEXTURE_FACE_PROPS));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_GL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Ogre->OGL_Listener->flag_Render_Groups);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TEXTURE_FACE_PROPS)
		{
			int SF = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
			if (SF > 0)
			{
				App->CL_X_Face_Editor->Start_FaceDialog();
			}
			else
			{
				App->Say("No Face Selected");
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TXL_FILE_EDIT)
		{
			App->CL_TXL_Editor->Selected_Texure_Index = App->CL_Properties_Textures->Selected_Index;
			App->CL_TXL_Editor->Start_Texl_Dialog();

			//App->CL_Level->Level_SetWadPath(App->CLSB_Doc->pLevel, Level_GetWadPath(App->CLSB_Doc->pLevel));
			//App->CL_World->Set_Current_TxlPath();
			//App->CL_Doc->UpdateAfterWadChange();
			//App->CL_Properties_Textures->Fill_ListBox();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_LISTTDTEXTURES)
		{
			if (App->CL_Properties_Textures->Dialog_Created == 1)
			{
				App->CL_Properties_Textures->List_Selection_Changed();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_FACESALL)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_FACESALL);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Top_Tabs->flag_All_Faces = true;
				App->CL_Doc->SelectAllFacesInBrushes();
				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
				return 1;
			}
			else
			{
				App->CL_Doc->ResetAllSelectedFaces();
				App->CL_Top_Tabs->Select_Face();
				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTTDAPPLY)
		{
			App->CL_Properties_Textures->Update_Texture();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_HELP)
		{
			App->Open_HTML((LPSTR)"Help\\Properties_Textures.html");
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *			Update_Texture Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Update_Texture()
{
	int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

	if (NumSelBrushes == 0)
	{
		App->Say("No Brushes Selected");
	}
	else
	{
		App->CL_Properties_Textures->Apply_Texture();

		App->CL_Doc->ResetAllSelectedFaces();

		if (App->CL_Top_Tabs->flag_All_Faces == true)
		{
			App->CL_Doc->SelectAllFacesInBrushes();
		}
		else
		{
			App->CL_Top_Tabs->Select_Face();
		}

		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	}
}

static void TextureBrushList(BrushList* pList, int SelId, char const* Name, WadFileEntry* pbmp);

// *************************************************************************
// *					( Static ) TextureFace							   *
// *************************************************************************
static void TextureFace(Face* pFace, int SelId, char const* Name, WadFileEntry* pbmp)
{
	App->CL_X_Face->Face_SetTextureDibId(pFace, SelId);
	App->CL_X_Face->Face_SetTextureName(pFace, Name);
	App->CL_X_Face->Face_SetTextureSize(pFace, pbmp->Width, pbmp->Height);
}

// *************************************************************************
// *					( Static ) TextureBrush							   *
// *************************************************************************
static void TextureBrush(Brush* pBrush, int SelId, char const* Name, WadFileEntry* pbmp) // changed QD 12/03)
{
	int j;

	if (App->CL_X_Brush->Brush_IsMulti(pBrush))
	{
		TextureBrushList((BrushList*)App->CL_X_Brush->Brush_GetBrushList(pBrush), SelId, Name, pbmp);
	}
	else
	{
		for (j = 0; j < App->CL_X_Brush->Brush_GetNumFaces(pBrush); ++j)
		{
			Face* pFace;

			pFace = App->CL_X_Brush->Brush_GetFace(pBrush, j);
			TextureFace(pFace, SelId, Name, pbmp); 
		}
	}
}

// *************************************************************************
// *					( Static ) TextureBrushList						   *
// *************************************************************************
static void TextureBrushList(BrushList* pList, int SelId, char const* Name, WadFileEntry* pbmp)
{
	Brush* b;
	BrushIterator bi;

	for (b = App->CL_X_Brush->BrushList_GetFirst(pList, &bi); b; b = App->CL_X_Brush->BrushList_GetNext(&bi))
	{
		TextureBrush(b, SelId, Name, pbmp); // changed QD 12/03
	}
}

// *************************************************************************
// *			Apply_Texture:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_Properties_Textures::Apply_Texture()
{
	int SelectedItem;
	int		i;

	char TextureName[MAX_PATH]{ 0 };

	SelectedItem = SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

	SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETTEXT, (WPARAM)SelectedItem, (LPARAM)TextureName);

	SelectedItem = GetIndexFromTextureName(TextureName);
	if (SelectedItem == -1)
	{
		App->Say("Cant Find Texture");
		return;
	}

	SelectedItem = SelectedItem;

	if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
	{
		return;
	}

	App->CL_Level->flag_Level_is_Modified = true;

	App->CL_Doc->mAdjustMode = ADJUST_MODE_FACE;

	switch (App->CL_Doc->mAdjustMode)
	{
	case ADJUST_MODE_FACE:
	{
		int Size;

		Size = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
		for (i = 0; i < Size; ++i)
		{
			Face* pFace;
			pFace = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, i);

			WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(m_CurrentTexture);
			TextureFace(pFace, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
			
		}
		
		int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
		for (i = 0; i < NumSelBrushes; ++i)
		{
			Brush* pBrush;

			pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
			App->CL_X_Brush->Brush_UpdateChildFaces(pBrush);
		}
		break;
	}

	case ADJUST_MODE_BRUSH:
	{
		if (App->CL_Doc->GetSelState() & MULTIBRUSH)
		{
			int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
			for (i = 0; i < NumSelBrushes; ++i)
			{
				Brush* pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
				
				WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(m_CurrentTexture);
				TextureBrush(pBrush, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
				
				App->CL_X_Brush->Brush_UpdateChildFaces(pBrush);
			}
		}
		else
		{
			
			WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(m_CurrentTexture);
			TextureBrush(App->CL_Doc->CurBrush, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
			
			App->CL_X_Brush->Brush_UpdateChildFaces(App->CL_Doc->CurBrush);
		}
		break;
	}

	default:
		return;
	}

	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
}

// *************************************************************************
// *		 GetIndexFromTextureName:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
int CL64_Properties_Textures::GetIndexFromTextureName(char* TextureName)
{
	CL64_WadFile* pWad = App->CL_Level->Level_GetWad_Class();
	
	// Check 
	if (pWad == nullptr) 
	{
		App->Say("Error Getting Wad Class");
		return -1;
	}

	// Search Textures
	for (int index = 0; index < pWad->mBitmapCount; index++)
	{
		char mName[MAX_PATH];
		strcpy(mName, pWad->mBitmaps[index].Name);

		bool test = strcmp(mName, TextureName);
		if (test == 0)
		{
			// Found return texture index
			return index;
		}
	}

	// No Texture Found
	return -1;
}

// *************************************************************************
// *	  	List_Selection_Changed:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Properties_Textures::List_Selection_Changed()
{
	int Index = SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (Index == LB_ERR)
	{
		App->Say("ListBox No Selection Available",(LPSTR)"");
	}
	else
	{
		char TextureName[MAX_PATH];
		TextureName[0] = 0;

		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETTEXT, (WPARAM)Index, (LPARAM)TextureName);
		strcpy(m_CurrentTexture, TextureName);
		
		SelectBitmap();
	}

	char buf[255];
	sprintf(buf, "Index = %i        %i X %i", Index, BasePicWidth, BasePicHeight);
	SetDlgItemText(Textures_Dlg_Hwnd, IDC_STWIDTHHEIGHT, (LPCTSTR)buf);

	Selected_Index = Index;
}

// *************************************************************************
// *			ViewerBasePic:- Terry and Hazel Flanigan 2025	  		   *
// *************************************************************************
bool CALLBACK CL64_Properties_Textures::ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		if (App->CL_Properties_Textures->Sel_BaseBitmap != nullptr)
		{
			RECT sourceRect = { 0, 0, App->CL_Properties_Textures->BasePicWidth, App->CL_Properties_Textures->BasePicHeight };
			RECT destRect = clientRect;

			// Get the device context and set the stretch mode
			HDC renderDC = GetDC(hwnd);
			SetStretchBltMode(renderDC, HALFTONE);

			// Render the texture
			App->CL_Properties_Textures->RenderTexture_Blit(renderDC, App->CL_Properties_Textures->Sel_BaseBitmap, &sourceRect, &destRect);
			ReleaseDC(hwnd, renderDC);
		}

		EndPaint(hwnd, &ps);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// *************************************************************************
// *		RenderTexture_Blit:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
bool CL64_Properties_Textures::RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect)
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
// *			A_SelectBitmap:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
bool CL64_Properties_Textures::SelectBitmap()
{
	char mTextureName[MAX_PATH];
	int TrueIndex = App->CL_TXL_Editor->GetIndex_From_Name(m_CurrentTexture);
	strcpy(mTextureName, App->CL_TXL_Editor->Texture_List[TrueIndex]->FileName);
	
	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	for (i = RFI->begin(); i != iend; ++i)
	{
		if (i->filename == mTextureName)
		{
			Ogre::DataStreamPtr ff = i->archive->open(i->filename);

			App->CL_Properties_Materials->mFileString_Ogre = ff->getAsString();

			char mFileName[MAX_PATH];
			strcpy(mFileName, App->RB_Directory_FullPath);
			strcat(mFileName, "\\Data\\");
			strcat(mFileName, mTextureName);

			std::ofstream outFile;
			outFile.open(mFileName, std::ios::binary);
			outFile << App->CL_Properties_Materials->mFileString_Ogre;
			outFile.close();

			App->CL_Properties_Materials->mFileString_Ogre.clear();

			Texture_To_HBITMP(mFileName);
			remove(mFileName);
			return 1;
		}
	}

	return 0;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Properties_Textures::Texture_To_HBITMP(char* TextureFileName)
{
	HWND PreviewWnd = GetDlgItem(Textures_Dlg_Hwnd, IDC_BASETEXTURE2);
	HDC	hDC = GetDC(PreviewWnd);

	Sel_BaseBitmap = App->CL_Textures->Get_HBITMP(TextureFileName, hDC);

	BasePicWidth = App->CL_Textures->BasePicWidth;
	BasePicHeight = App->CL_Textures->BasePicHeight;
	
	ReleaseDC(PreviewWnd, hDC);

	RedrawWindow(PreviewWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Fill_ListBox:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Fill_ListBox()
{
	int LBIndex;

	//if (f_TextureDlg_Active == 1)
	{
		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		for (int index = 0; index < App->CL_TXL_Editor->Texture_Count; index++)
		{
			char mName[MAX_PATH];

			strcpy(mName, App->CL_TXL_Editor->Texture_List[index]->Name);

			LBIndex = SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
		}

		SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		
	}
}

// *************************************************************************
// *	  	Get_Selected_Face:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Get_Selected_Face()
{
	mSelected_Face = NULL;

	int NumberOfFaces = App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);

	if (NumberOfFaces > 0)
	{
		mSelected_Face = App->CL_X_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, (NumberOfFaces - 1));

		Select_With_TextureName(App->CL_X_Face->Face_GetTextureName(mSelected_Face));
	}
}

// *************************************************************************
// *	  	Select_With_TextureName:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Properties_Textures::Select_With_TextureName(const char* TextureName)
{
	SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)TextureName);
	
	strcpy(m_CurrentTexture, TextureName);
	List_Selection_Changed();
}

// *************************************************************************
// *		Select_With_List_Index:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
void CL64_Properties_Textures::Select_With_List_Index(int Index)
{
	SendDlgItemMessage(Textures_Dlg_Hwnd, IDC_LISTTDTEXTURES, LB_SETCURSEL, (WPARAM)Index, (LPARAM)0);
	List_Selection_Changed();
}

// *************************************************************************
// *	Enable_FaceProps_Button:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Properties_Textures::Enable_FaceProps_Button(bool Enable)
{
	EnableWindow(GetDlgItem(Textures_Dlg_Hwnd, IDC_BT_TEXTURE_FACE_PROPS), Enable);
	EnableWindow(GetDlgItem(Textures_Dlg_Hwnd, IDC_BTTDAPPLY), Enable);
	
	EnableWindow(GetDlgItem(Textures_Dlg_Hwnd, IDC_CK_FACESALL), Enable);

	HWND Temp = GetDlgItem(Textures_Dlg_Hwnd, IDC_CK_FACESALL);
	SendMessage(Temp, BM_SETCHECK, 0, 0);
	
}
