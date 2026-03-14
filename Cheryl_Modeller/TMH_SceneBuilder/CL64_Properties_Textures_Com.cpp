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
}

CL64_Properties_Textures_Com::~CL64_Properties_Textures_Com(void)
{
}

// *************************************************************************
// *		Fill_Textures_ListBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Properties_Textures_Com::Fill_Textures_ListBox()
{
	// --------------------------------------------------------------------- Assimp
	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		auto& m_Textures_Dlg = App->CL_Properties_Textures_Assimp->Textures_Dlg_Hwnd_Assimp;

		SendDlgItemMessage(m_Textures_Dlg, IDC_LIST_AT_TEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		if (App->CL_Model->GroupCount > 0)
		{
			char mName[MAX_PATH];
			strcpy(mName, App->CL_Mesh->Group[App->CL_Properties_Textures_Assimp->Selected_Group]->Assimp_Text_FileName);
			SendDlgItemMessage(m_Textures_Dlg, IDC_LIST_AT_TEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);


			SendDlgItemMessage(m_Textures_Dlg, IDC_LIST_AT_TEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		}
	}

	// --------------------------------------------------------------------- Ogre
	if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		auto& m_Textures_Dlg = App->CL_Properties_Materials->Textures_Dlg_Hwnd_Ogre;

		SendDlgItemMessage(m_Textures_Dlg, IDC_LIST_TEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		if (App->CL_Model->GroupCount > 0)
		{
			char mName[MAX_PATH];

			int Count = 0;
			while (Count < App->CL_Mesh->Group[App->CL_Properties_Materials->Selected_Material_Index]->Ogre_NumTextureUnits)
			{
				strcpy(mName, App->CL_Mesh->Group[App->CL_Properties_Materials->Selected_Material_Index]->v_Texture_Names[Count].c_str());

				SendDlgItemMessage(m_Textures_Dlg, IDC_LIST_TEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
				Count++;
			}

			SendDlgItemMessage(m_Textures_Dlg, IDC_LIST_TEXTURES, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		}
	}
}

