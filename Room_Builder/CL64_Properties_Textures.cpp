/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Scene Builder

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

CL64_Properties_Textures::CL64_Properties_Textures(void)
{
	TexturesDlg_Hwnd = nullptr;
	flag_Textures_Dlg_Created = false;

	flag_No_Faces = false;
	flag_All_Faces = false;
	flag_Next_Face = false;
	flag_Prev_Face = false;

	Sel_BaseBitmap = NULL;
	BasePicWidth = NULL;
	BasePicHeight = NULL;

	Selected_Face_Index = 0;
	Selected_Index = 0;

	strcpy(m_CurrentTexture, "stfloor1"); // TODO Why
	mSelected_Face = NULL;


	mFileString.clear();
}

CL64_Properties_Textures::~CL64_Properties_Textures(void)
{
}

// *************************************************************************
// *	  	Start_Tabs_Textures_Dlg:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Start_Tabs_Textures_Dlg()
{
	TexturesDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_TEXTURES, App->CL_Properties_Tabs->Tabs_Control_Hwnd, (DLGPROC)Proc_Tabs_Textures_Dlg);
	flag_Textures_Dlg_Created = true;

	Fill_Textures_ListBox();
}

// **************************************************************************
// *			Proc_Tabs_Textures_Dlg:- Terry and Hazel Flanigan 2026
// **************************************************************************
LRESULT CALLBACK CL64_Properties_Textures::Proc_Tabs_Textures_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& p_Faces = App->CL_Faces_Control; // Pointer to Faces Control

	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_FACES_NONE2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACES_ALL2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACE_NEXT2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FACE_PREV2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_FACE_SHOWSELECTEDFACE2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_FACE_FACEEDITOR2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_FE_LIST_TEXTURES2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PF_CHANGETEXTURE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SetWindowLongPtr(GetDlgItem(hDlg, IDC_FE_BASETEXTURE2), GWLP_WNDPROC, (LONG_PTR)ViewerBasePic);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
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
		
		case IDC_BT_FACES_NONE2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_FACES_NONE2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Textures->flag_No_Faces);
			}

			break;
		}

		case IDC_BT_TT_FACES_ALL2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACES_ALL2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Textures->flag_All_Faces);
			}

			break;
		}

		case IDC_BT_TT_FACE_NEXT2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACE_NEXT2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Textures->flag_Next_Face);
			}

			break;
		}

		case IDC_BT_TT_FACE_PREV2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_FACE_PREV2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Properties_Textures->flag_Prev_Face);
			}

			break;
		}

		case IDC_BT_FACE_SHOWSELECTEDFACE2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_FACE_SHOWSELECTEDFACE2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				if (App->flag_3D_Started == true)
				{
					App->Custom_Button_Toggle_Tabs(item, App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face);
				}
			}

			break;
		}

		case IDC_BT_PF_CHANGETEXTURE:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PF_CHANGETEXTURE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				if (App->flag_3D_Started == true)
				{
					App->Custom_Button_Toggle_Tabs(item, App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face);
				}
			}

			break;
		}
		case IDC_BT_FACE_FACEEDITOR2:
		{
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_FACE_FACEEDITOR2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			break;
		}
		default:
			return CDRF_DODEFAULT;
		}
		

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_FACE_FACEEDITOR2)
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

		if (LOWORD(wParam) == IDC_FE_LIST_TEXTURES2)
		{
			if (App->CL_Interface->flag_Tab_Texture == true)
			{
				App->CL_Properties_Textures->List_Selection_Changed();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PF_CHANGETEXTURE)
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

				if (App->CL_Properties_Textures->flag_All_Faces == true)
				{
					App->CL_Doc->SelectAllFacesInBrushes();
				}
				else
				{
					App->CL_Properties_Textures->Select_Face();
				}

				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_FACES_NONE2)
		{
			App->CL_Properties_Textures->Unselect_All_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACES_ALL2)
		{
			App->CL_Properties_Textures->Select_All_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACE_NEXT2)
		{
			App->CL_Properties_Textures->Select_Next_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FACE_PREV2)
		{
			App->CL_Properties_Textures->Select_Prev_Face();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_FACE_SHOWSELECTEDFACE2)
		{
			if (App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face == true)
			{
				App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = false;
			}
			else
			{
				App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = true;
			}

			return TRUE;
		}

	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *				Reset_Face_Buttons:- Terry Mo and Hazel 2026
// *************************************************************************
void CL64_Properties_Textures::Reset_Face_Buttons()
{
	// Reset all face-related flags to their default state
	flag_No_Faces = false;
	flag_All_Faces = false;
	flag_Next_Face = false;
	flag_Prev_Face = false;

	// Redraw the window to reflect the changes in the flags
	RedrawWindow(TexturesDlg_Hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Unselect_All_Face:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Unselect_All_Face()
{
	if (App->CL_X_Face_Editor->flag_FaceDlg_Active == true)
	{
		App->CL_X_Face_Editor->Close_Faces_Dialog();
	}

	App->CL_Doc->ResetAllSelectedFaces();
	App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = false;
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	Reset_Face_Buttons();
	flag_No_Faces = true;

	EnableWindow(GetDlgItem(TexturesDlg_Hwnd, IDC_BT_FACE_FACEEDITOR2), false);
	EnableWindow(GetDlgItem(TexturesDlg_Hwnd, IDC_BT_FACE_SHOWSELECTEDFACE2), false);
	//EnableWindow(GetDlgItem(Faces_Control_Dlg_hWnd, IDC_TT_CB_FACES), false);

	RedrawWindow(TexturesDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Select_All_Face:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Properties_Textures::Select_All_Face()
{
	Reset_Face_Buttons();
	flag_All_Faces = true;

	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

	App->CL_Doc->SelectAllFacesInBrushes();
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = true;

	EnableWindow(GetDlgItem(TexturesDlg_Hwnd, IDC_BT_FACE_FACEEDITOR2), true);
	EnableWindow(GetDlgItem(TexturesDlg_Hwnd, IDC_BT_FACE_SHOWSELECTEDFACE2), true);

	RedrawWindow(TexturesDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Select_Next_Face:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures::Select_Next_Face()
{
	Reset_Face_Buttons();
	flag_Next_Face = true;

	Selected_Face_Index++;

	if (Selected_Face_Index == App->CL_Brush_X->Face_Count)
	{
		Selected_Face_Index = 0;
	}

	Select_Face();

	if (App->CL_X_Face_Editor->flag_FaceDlg_Active == 1)
	{
		App->CL_X_Face_Editor->Change_Selection();
	}

	App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = true;

	EnableWindow(GetDlgItem(TexturesDlg_Hwnd, IDC_BT_FACE_FACEEDITOR2), true);
	EnableWindow(GetDlgItem(TexturesDlg_Hwnd, IDC_BT_FACE_SHOWSELECTEDFACE2), true);
	//EnableWindow(GetDlgItem(Faces_Control_Dlg_hWnd, IDC_TT_CB_FACES), true);

	RedrawWindow(TexturesDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Select_Prev_Face:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures::Select_Prev_Face()
{
	Reset_Face_Buttons();
	flag_Prev_Face = true;

	Selected_Face_Index--;

	if (Selected_Face_Index < 0)
	{
		Selected_Face_Index = App->CL_Brush_X->Face_Count - 1;
	}

	Select_Face();

	if (App->CL_X_Face_Editor->flag_FaceDlg_Active == true)
	{
		App->CL_X_Face_Editor->Change_Selection();
	}

	App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = true;

	EnableWindow(GetDlgItem(TexturesDlg_Hwnd, IDC_BT_FACE_FACEEDITOR2), true);
	EnableWindow(GetDlgItem(TexturesDlg_Hwnd, IDC_BT_FACE_SHOWSELECTEDFACE2), true);
	//EnableWindow(GetDlgItem(Faces_Control_Dlg_hWnd, IDC_TT_CB_FACES), true);

	RedrawWindow(TexturesDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Select_Face:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Select_Face()
{
	// Redraw the top tabs dialog
	App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

	// Check if there are no selected faces
	if (App->CL_X_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces) == 0)
	{
		// Select all faces in brushes if none are selected
		App->CL_Doc->SelectAllFacesInBrushes();
	}

	// Select the face from the index regardless of the selection state
	App->CL_X_Face->Select_Face_From_Index(Selected_Face_Index);

	// Update all views to reflect changes
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	// Set the current selection in the combo box
	HWND Temp = GetDlgItem(App->CL_Faces_Control->Faces_Control_Dlg_hWnd, IDC_TT_CB_FACES);
	SendMessage(Temp, CB_SETCURSEL, Selected_Face_Index, 0);
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
// *			Apply_Texture:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Apply_Texture()
{
	int SelectedItem;
	int		i;

	char TextureName[MAX_PATH]{ 0 };

	SelectedItem = SendDlgItemMessage(TexturesDlg_Hwnd, IDC_FE_LIST_TEXTURES2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

	SendDlgItemMessage(TexturesDlg_Hwnd, IDC_FE_LIST_TEXTURES2, LB_GETTEXT, (WPARAM)SelectedItem, (LPARAM)TextureName);

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

			WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(App->CL_Properties_Textures->m_CurrentTexture);
			TextureFace(pFace, SelectedItem, (LPCSTR)App->CL_Properties_Textures->m_CurrentTexture, BitmapPtr);

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

				WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(App->CL_Properties_Textures->m_CurrentTexture);
				TextureBrush(pBrush, SelectedItem, (LPCSTR)App->CL_Properties_Textures->m_CurrentTexture, BitmapPtr);

				App->CL_X_Brush->Brush_UpdateChildFaces(pBrush);
			}
		}
		else
		{

			WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(App->CL_Properties_Textures->m_CurrentTexture);
			TextureBrush(App->CL_Doc->CurBrush, SelectedItem, (LPCSTR)App->CL_Properties_Textures->m_CurrentTexture, BitmapPtr);

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
// *		 GetIndexFromTextureName:- Terry and Hazel Flanigan 2026
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
// *			Fill_Textures_ListBox:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Fill_Textures_ListBox()
{
	int LBIndex;

	if (flag_Textures_Dlg_Created == true)
	{
		SendDlgItemMessage(TexturesDlg_Hwnd, IDC_FE_LIST_TEXTURES2, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		for (int index = 0; index < App->CL_TXL_Editor->Texture_Count; index++)
		{
			char mName[MAX_PATH];

			strcpy(mName, App->CL_TXL_Editor->Texture_List[index]->Name);

			LBIndex = SendDlgItemMessage(TexturesDlg_Hwnd, IDC_FE_LIST_TEXTURES2, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
		}

		//SendDlgItemMessage(TexturesDlg_Hwnd, IDC_FE_LIST_TEXTURES2 ,LB_SETCURSEL, 0, 0);
		
		Get_Selected_Face_Texture();
	}
}

// *************************************************************************
// *	  	Get_Selected_Face_Texture:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Get_Selected_Face_Texture()
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
// *	  	Select_With_TextureName:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Select_With_TextureName(const char* TextureName)
{
	SendDlgItemMessage(TexturesDlg_Hwnd, IDC_FE_LIST_TEXTURES2, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)TextureName);

	strcpy(m_CurrentTexture, TextureName);
	List_Selection_Changed();
}

// *************************************************************************
// *	  	List_Selection_Changed:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::List_Selection_Changed()
{
	int Index = SendDlgItemMessage(TexturesDlg_Hwnd, IDC_FE_LIST_TEXTURES2, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (Index == LB_ERR)
	{
		App->Say("ListBox No Selection Available", (LPSTR)"");
	}
	else
	{
		char TextureName[MAX_PATH];
		TextureName[0] = 0;

		SendDlgItemMessage(TexturesDlg_Hwnd, IDC_FE_LIST_TEXTURES2, LB_GETTEXT, (WPARAM)Index, (LPARAM)TextureName);
		strcpy(m_CurrentTexture, TextureName);

		SelectBitmap();
	}

	/*char buf[255];
	sprintf(buf, "Index = %i        %i X %i", Index, BasePicWidth, BasePicHeight);
	SetDlgItemText(Textures_Dlg_Hwnd, IDC_STWIDTHHEIGHT, (LPCTSTR)buf);*/

	Selected_Index = Index;
}

// *************************************************************************
// *			A_SelectBitmap:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
bool CL64_Properties_Textures::SelectBitmap()
{
	char mTextureName[MAX_PATH];
	int TrueIndex = App->CL_TXL_Editor->GetIndex_From_Name(m_CurrentTexture);
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

			mFileString = ff->getAsString();

			char mFileName[MAX_PATH];
			strcpy(mFileName, App->RB_Directory_FullPath);
			strcat(mFileName, "\\Data\\");
			strcat(mFileName, mTextureName);

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

	return 0;
}

// *************************************************************************
// *			 Texture_To_HBITMP:- Terry and Hazel Flanigan 2026
// *************************************************************************
void CL64_Properties_Textures::Texture_To_HBITMP(char* TextureFileName)
{
	HWND PreviewWnd = GetDlgItem(TexturesDlg_Hwnd, IDC_FE_BASETEXTURE2);
	HDC	hDC = GetDC(PreviewWnd);

	Sel_BaseBitmap = App->CL_Textures->Get_HBITMP(TextureFileName, hDC);

	BasePicWidth = App->CL_Textures->BasePicWidth;
	BasePicHeight = App->CL_Textures->BasePicHeight;

	ReleaseDC(PreviewWnd, hDC);

	RedrawWindow(PreviewWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			ViewerBasePic:- Terry and Hazel Flanigan 2026
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
// *		RenderTexture_Blit:- Terry and Hazel Flanigan 2026
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
