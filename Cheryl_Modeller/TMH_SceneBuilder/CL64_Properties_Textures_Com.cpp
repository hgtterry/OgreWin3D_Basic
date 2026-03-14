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
	Selected_Group = 0;

	BasePicWidth = 0;
	BasePicHeight = 0;
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

	//App->CL_Interface->Show_Textures_Assimp_Dlg(false);

	//App->CL_Properties_Textures_Com->Fill_Textures_ListBox();
	//App->CL_Properties_Textures_Com->Fill_Materials_ListBox();

	//Update_Texture_Ogre_Dlg();
}

// *************************************************************************
// *		Fill_Textures_ListBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures_Com::Fill_Textures_ListBox()
{
	// --------------------------------------------------------------------- Assimp
	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		auto& m_Textures_Dlg_hWnd = App->CL_Properties_Textures_Assimp->Textures_Dlg_Hwnd_Assimp;

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
		auto& m_Textures_Dlg_hWnd = App->CL_Properties_Materials->Textures_Dlg_Hwnd_Ogre;

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
		auto& m_Textures_Dlg_hWnd = App->CL_Properties_Textures_Assimp->Textures_Dlg_Hwnd_Assimp;

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
		auto& m_Textures_Dlg_hWnd = App->CL_Properties_Materials->Textures_Dlg_Hwnd_Ogre;

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

		strcpy(m_Textures_Class->mMaterialName, App->CL_Mesh->Group[Index]->MaterialName);
		strcpy(m_Textures_Class->mTextureName, App->CL_Mesh->Group[Index]->Assimp_Text_FileName);

		m_Textures_Class->Sel_BaseBitmap = App->CL_Mesh->Group[Index]->Base_Bitmap;

		BITMAP bm;
		GetObject(m_Textures_Class->Sel_BaseBitmap, sizeof(bm), &bm);

		BasePicWidth = bm.bmWidth;
		BasePicHeight = bm.bmHeight;

		char Dimensions[MAX_PATH];
		sprintf(Dimensions, "%i X %i", BasePicWidth, BasePicHeight);// , bm.bmBitsPixel);
		SetDlgItemText(m_Textures_Class->Textures_Dlg_Hwnd_Assimp, IDC_ST_AT_DIMENSIONS, Dimensions);

		ShowWindow(GetDlgItem(m_Textures_Class->Textures_Dlg_Hwnd_Assimp, IDC_AT_BASETEXTURE), 0);
		ShowWindow(GetDlgItem(m_Textures_Class->Textures_Dlg_Hwnd_Assimp, IDC_AT_BASETEXTURE), 1);
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
			SetDlgItemText(m_Textures_Class->Textures_Dlg_Hwnd_Ogre, IDC_ST_PT_NUMTEXTUNITS, NumTextUnits);
		}

		BITMAP bm;
		GetObject(m_Textures_Class->Sel_BaseBitmap_Ogre, sizeof(bm), &bm);

		BasePicWidth = bm.bmWidth;
		BasePicHeight = bm.bmHeight;

		char Dimensions[MAX_PATH];
		sprintf(Dimensions, "%i X %i", BasePicWidth, BasePicHeight);// , bm.bmBitsPixel);
		SetDlgItemText(m_Textures_Class->Textures_Dlg_Hwnd_Ogre, IDC_ST_PT_DIMENSIONS, Dimensions);

		ShowWindow(GetDlgItem(m_Textures_Class->Textures_Dlg_Hwnd_Ogre, IDC_PROP_BASETEXTURE_OGRE), 0);
		ShowWindow(GetDlgItem(m_Textures_Class->Textures_Dlg_Hwnd_Ogre, IDC_PROP_BASETEXTURE_OGRE), 1);

		RedrawWindow(m_Textures_Class->Textures_Dlg_Hwnd_Ogre, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

