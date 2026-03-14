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
	mFileString_Ogre.clear();
}

CL64_Properties_Materials::~CL64_Properties_Materials(void)
{
}

// *************************************************************************
// *	  		Reset_Class:- Terry and Hazel Flanigan 2026				   *
// *************************************************************************
void CL64_Properties_Materials::Reset_Class()
{
	/*mFileString_Ogre.clear();
	
	App->CL_Interface->Show_Materials_Dlg(false);
	App->CL_Interface->Menu_Enable_Materials(false);

	App->CL_Properties_Textures_Com->Fill_Textures_ListBox();
	App->CL_Properties_Textures_Com->Fill_Materials_ListBox();

	App->CL_Properties_Textures_Com->Update_Dlg_Bmp_Texture();*/
}

// *************************************************************************
// *	  	Show_Materials_Dialog_Ogre:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Properties_Materials::Show_Materials_Dialog_Ogre(bool Show)
{
	ShowWindow(App->CL_Properties_Textures_Com->Textures_Dlg_Hwnd_Ogre, Show);
}

// *************************************************************************
// *	  	Start_Materials_Dialog_Ogre:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Properties_Materials::Start_Materials_Dialog_Ogre()
{
	App->CL_Properties_Textures_Com->Textures_Dlg_Hwnd_Ogre = CreateDialog(App->hInst, (LPCTSTR)IDD_TEXTURES_OGRE, App->MainHwnd, (DLGPROC)Proc_Materials_Dialog_Ogre);
	Show_Materials_Dialog_Ogre(false);
}

// *************************************************************************
// *        Proc_TextureDialog_Ogre:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_Properties_Materials::Proc_Materials_Dialog_Ogre(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LIST_MATERIALS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LIST_TEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
	
		SendDlgItemMessage(hDlg, IDC_BT_PT_VIEWMAT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MATERIAL_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_GROUPDETAILS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PT_CHANGETEXTURE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_PT_DIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_PT_NUMTEXTUNITS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		
		SetWindowLongPtr(GetDlgItem(hDlg, IDC_PROP_BASETEXTURE_OGRE), GWLP_WNDPROC, (LONG_PTR)App->CL_Properties_Textures_Com->ViewerBasePic);

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

			App->Custom_Button_Toggle(item, App->CL_Dialogs->flag_FileViewer_Active);
		}

		if (some_item->idFrom == IDC_BT_GROUPDETAILS)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			//if (App->CL_Dialogs->m_ListType == Enums::ListBox_Mesh_Data)
			{
				App->Custom_Button_Toggle(item,App->CL_Dialogs->flag_General_ListBox_Active);
			}
		}

		if (some_item->idFrom == IDC_BT_MATERIAL_FACES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			App->Custom_Button_Toggle(item, App->CL_Ogre->OGL_Listener->flag_Show_Material_Faces);
			
		}

		if (some_item->idFrom == IDC_BT_PT_CHANGETEXTURE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PT_CHANGETEXTURE));
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
		if (LOWORD(wParam) == IDC_BT_GROUPDETAILS)
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

		if (LOWORD(wParam) == IDC_LIST_MATERIALS) // Click inside Materials list box
		{
			if (App->CL_Model->flag_Model_Loaded == true && App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
			{
				int Index = SendDlgItemMessage(hDlg, IDC_LIST_MATERIALS, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (Index == LB_ERR)
				{
					App->Say("ListBox No Selection Available", (LPSTR)"");
					return TRUE;
				}

				App->CL_Properties_Textures_Com->List_Material_Changed(Index);
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_LIST_TEXTURES) // Click inside Textures list box
		{
			if (App->CL_Model->flag_Model_Loaded == true && App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
			{
				int Index = SendDlgItemMessage(hDlg, IDC_LIST_TEXTURES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (Index == LB_ERR)
				{
					App->Say("ListBox No Selection Available", (LPSTR)"");
					return TRUE;
				}
				
				App->CL_Properties_Materials->List_Texture_Changed(Index);
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PT_VIEWMAT)
		{
			if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
			{
				if (App->CL_Dialogs->flag_FileViewer_Active == true)
				{
					App->CL_Dialogs->flag_FileViewer_Active = false;
					EndDialog(App->CL_Dialogs->FileViewer_Dlg_Hwnd, LOWORD(wParam));
					return TRUE;
				}
				else
				{
					App->CL_Resources->View_File(App->CL_Mesh->Group[App->CL_Properties_Textures_Com->Selected_Group]->Ogre_Material_File, App->MainHwnd);
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

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Interface->Show_Materials_Dlg(false);
			return TRUE;
		}

		break;
	}

	}
	return FALSE;
}

// *************************************************************************
// *	  	List_Texture_Changed:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Materials::List_Texture_Changed(int Index)
{
	if (App->CL_Model->flag_Model_Loaded == true && App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		if (App->CL_Model->GroupCount > 0)
		{
			strcpy(App->CL_Properties_Textures_Com->m_Current_TextureName, App->CL_Mesh->Group[App->CL_Properties_Textures_Com->Selected_Group]->v_Texture_Names[Index].c_str());
			strcpy(App->CL_Properties_Textures_Com->m_Current_MaterialName, App->CL_Mesh->Group[App->CL_Properties_Textures_Com->Selected_Group]->Ogre_Material);
			App->CL_Properties_Textures_Com->View_Texture(App->CL_Properties_Textures_Com->m_Current_TextureName, App->CL_Properties_Textures_Com->m_Current_MaterialName);
		}
	}
}




