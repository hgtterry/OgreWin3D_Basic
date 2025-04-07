/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_Export.h"

#include <string>
#include <shobjidl_core.h>

CL64_Export::CL64_Export()
{
	mFolder_Path[0] = 0;
	mDirectory_Name[0] = 0;

	Object_OutputFolder[0] = 0;
	Object_FileName[0] = 0;
	mtl_FileName[0] = 0;
	Just_mtl_FileName[0] = 0;

	strcpy(mJustName,"World1");

	DeskTop_Folder[0] = 0;

	flag_Build_Edge_List = 1;
	flag_Version_Latest = 1;
	flag_Version_18 = 0;
	flag_Version_17 = 0;

	mOgre_Version = Enums::Export_Ogre_Latest;

	Write_OBJECTFILE = NULL;
	Write_MTLFile = NULL;

}

CL64_Export::~CL64_Export()
{
}

// *************************************************************************
// *	  		Object_Export_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Export::Object_Export_Dlg()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_OBJECT_EXPORT, App->MainHwnd, (DLGPROC)Proc_Object_Export_Dlg);
}

// *************************************************************************
// *		Proc_Object_Export_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Export::Proc_Object_Export_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_BANNER, WM_SETFONT, (WPARAM)App->Font_Banner, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_STFILENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STPATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STOGRESUB, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_OGRE_FILENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_OGRE_PATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_OGRE_SUBFOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_OGRE_NAMECHANGE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_OGREBROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CK_BL_DESKTOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_CREATE_SUBDIR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_BUILDEDGELIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_ST_OGRE_FILENAME, (LPCTSTR)App->CL_Export->mJustName);
		SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);

		strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
		strcat(App->CL_Export->mDirectory_Name, "_Object_All");

		SetDlgItemText(hDlg, IDC_ST_OGRE_SUBFOLDER, (LPCTSTR)App->CL_Export->mDirectory_Name);

		HWND Temp = GetDlgItem(hDlg, IDC_CK_CREATE_SUBDIR);
		SendMessage(Temp, BM_SETCHECK, 1, 0);

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_OGRE_FILENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_OGRE_PATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_OGRE_SUBFOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_STFILENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STPATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STOGRESUB) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BL_DESKTOP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_CREATE_SUBDIR) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		return FALSE;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_OGRE_NAMECHANGE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_OGREBROWSE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_OGRE_NAMECHANGE)
		{
			strcpy(App->CL_Dialogs->btext, "Change File Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Export->mJustName);

			App->CL_Dialogs->Dialog_Text(Enums::Check_Name_None);

			if (App->CL_Dialogs->flag_Dlg_Canceled == 0)
			{
				strcpy(App->CL_Export->mJustName, App->CL_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_OGRE_FILENAME, App->CL_Export->mJustName);

			strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
			strcat(App->CL_Export->mDirectory_Name, "_Object_All");

			SetDlgItemText(hDlg, IDC_ST_OGRE_SUBFOLDER, (LPCTSTR)App->CL_Export->mDirectory_Name);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_OGREBROWSE)
		{
			App->CL_File_IO->Select_Folder();
			if (App->CL_File_IO->flag_Canceled == 0)
			{
				strcpy(App->CL_Export->mFolder_Path, App->CL_File_IO->szSelectedDir);
				SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_BL_DESKTOP)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BL_DESKTOP);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				strcpy(App->CL_Export->mFolder_Path, App->CL_Export->DeskTop_Folder);
				SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);
				return 1;
			}
			else
			{
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			// Check Path
			int result = strcmp(App->CL_Export->mFolder_Path, "");
			if (result == 0)
			{
				App->Say("No Path Selected");
				return 1;
			}

			strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
			strcat(App->CL_Export->mDirectory_Name, "_Object_All");

			App->CL_Export->Export_To_Object();

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	}

	return FALSE;
}

// *************************************************************************
// *			Export_To_Object:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Export::Export_To_Object(void)
{
	//App->CLSB_PB->Nudge("Build_Brush_List");
	App->CL_Mesh_Mgr->Brush_Build_List(false);

	//App->CLSB_PB->Nudge("Converting to Groups");
	App->CL_Mesh_Mgr->WE_Convert_All_Texture_Groups();

	//App->CLSB_PB->Nudge("Exporting Wavefront");
	Create_ObjectFile();

	//App->CLSB_PB->Nudge("Finished");
}

// *************************************************************************
// *		Create_ObjectFile:- Terry and Hazel Flanigan 2025   	 	   *
// *************************************************************************
bool CL64_Export::Create_ObjectFile(void)
{
	strcpy(Object_OutputFolder, "");

	char buff[MAX_PATH];
	strcpy(buff, App->CL_Export->mFolder_Path);
	strcat(buff, "\\");
	strcat(buff, App->CL_Export->mDirectory_Name);
	strcat(buff, "\\");

	strcpy(Object_OutputFolder, buff);

	CreateDirectory(Object_OutputFolder, NULL);

	Export_Object_Textures();

	strcpy(Object_FileName, Object_OutputFolder);
	strcat(Object_FileName, App->CL_Export->mJustName);
	strcat(Object_FileName, ".obj");

	strcpy(mtl_FileName, Object_OutputFolder);
	strcat(mtl_FileName, App->CL_Export->mJustName);
	strcat(mtl_FileName, ".mtl");

	strcpy(Just_mtl_FileName, App->CL_Export->mJustName);
	strcat(Just_mtl_FileName, ".mtl");

	Write_ObjectFile();

	WriteMTLFile();

	char File[MAX_PATH];
	strcpy(File,App->CL_Export->mJustName);
	strcat(File, ".obj");
	App->Say(File,"Exported");

	return 1;
}

// *************************************************************************
// *		Write_ObjectFile:- Terry and Hazel Flanigan 2025  			   *
// *************************************************************************
void CL64_Export::Write_ObjectFile(void)
{
	Write_OBJECTFILE = 0;

	Write_OBJECTFILE = fopen(Object_FileName, "wt");

	if (!Write_OBJECTFILE)
	{
		return;
	}
	
	Write_ObjectFile_Commit();
	
	fclose(Write_OBJECTFILE);
}

// *************************************************************************
// *		Write_ObjectFile_Commit:- Terry and Hazel Flanigan 2025  	   *
// *************************************************************************
void CL64_Export::Write_ObjectFile_Commit(void)
{
	fprintf(Write_OBJECTFILE, "#\n");
	fprintf(Write_OBJECTFILE, "# Wavefront OBJ file\n");
	fprintf(Write_OBJECTFILE, "# Created with OW3D\n");
	fprintf(Write_OBJECTFILE, "#\n");
	fprintf(Write_OBJECTFILE, "mtllib %s\n", Just_mtl_FileName);
	fprintf(Write_OBJECTFILE, "%s \n", " ");

	int	VertCount = 0;
	int GroupCount = 0;
	int VertTextCords = 0;
	int VertNormals = 0;
	int FaceCount = 0;
	int FaceIndex = 1;

	int GroupCountTotal = App->CL_Editor_Com->GroupCount;

	char MaterialName[MAX_PATH];

	//--------------------------------------------- Vertices v-
	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Editor_Com->VerticeCount);

	while (GroupCount < GroupCountTotal)
	{
		VertCount = 0;
		while (VertCount < App->CL_Editor_Com->Group[GroupCount]->GroupVertCount)
		{
			float X = App->CL_Editor_Com->Group[GroupCount]->vertex_Data[VertCount].x;
			float Y = App->CL_Editor_Com->Group[GroupCount]->vertex_Data[VertCount].y;
			float Z = App->CL_Editor_Com->Group[GroupCount]->vertex_Data[VertCount].z;

			fprintf(Write_OBJECTFILE, "v %.06f %.06f %.06f\n", X, Y, Z);

			VertCount++;
		}
		GroupCount++;
	}

	fprintf(Write_OBJECTFILE, "%s \n", " ");

	//--------------------------------------------- Texture Cords vt-
	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Editor_Com->VerticeCount);

	GroupCount = 0;

	while (GroupCount < GroupCountTotal)
	{
		VertTextCords = 0;
		while (VertTextCords < App->CL_Editor_Com->Group[GroupCount]->GroupVertCount)
		{
			float U = App->CL_Editor_Com->Group[GroupCount]->MapCord_Data[VertTextCords].u;
			float V = App->CL_Editor_Com->Group[GroupCount]->MapCord_Data[VertTextCords].v;

			fprintf(Write_OBJECTFILE, "vt %.06f %.06f\n", U, V);

			VertTextCords++;
		}
		GroupCount++;
	}

	fprintf(Write_OBJECTFILE, "%s \n", " ");

	//--------------------------------------------- Normals vn-
	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Editor_Com->VerticeCount);

	GroupCount = 0;

	while (GroupCount < GroupCountTotal)
	{
		VertNormals = 0;
		while (VertNormals < App->CL_Editor_Com->Group[GroupCount]->GroupVertCount)
		{
			float X = App->CL_Editor_Com->Group[GroupCount]->Normal_Data[VertNormals].x;
			float Y = App->CL_Editor_Com->Group[GroupCount]->Normal_Data[VertNormals].y;
			float Z = App->CL_Editor_Com->Group[GroupCount]->Normal_Data[VertNormals].z;

			fprintf(Write_OBJECTFILE, "vn %.06f %.06f %.06f\n", X, Y, Z);

			VertNormals++;
		}
		GroupCount++;
	}

	fprintf(Write_OBJECTFILE, "%s \n", " ");

	GroupCount = 0;

	int Offset = 1;
	while (GroupCount < GroupCountTotal)
	{
		FaceCount = 0;
		int LineIndex = 0;

		strcpy(MaterialName, App->CL_Editor_Com->Group[GroupCount]->MaterialName);
		fprintf(Write_OBJECTFILE, "usemtl %s\n", MaterialName);

		while (FaceCount < App->CL_Editor_Com->Group[GroupCount]->GroupFaceCount)
		{
			int A0 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].a + Offset;
			int B0 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].a + Offset;
			int C0 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].a + Offset;

			int A1 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].b + Offset;
			int B1 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].b + Offset;
			int C1 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].b + Offset;

			int A2 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].c + Offset;
			int B2 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].c + Offset;
			int C2 = App->CL_Editor_Com->Group[GroupCount]->Face_Data[FaceCount].c + Offset;

			FaceCount++;

			fprintf(Write_OBJECTFILE, "f %i/%i/%i %i/%i/%i %i/%i/%i\n", A0, B0, C0, A1, B1, C1, A2, B2, C2);

		}

		Offset = Offset + App->CL_Editor_Com->Group[GroupCount]->GroupVertCount;

		fprintf(Write_OBJECTFILE, "%s \n", " ");

		GroupCount++;
	}
}

// *************************************************************************
// *			WriteMTLFile:- Terry and Hazel Flanigan 2025	  	   	   *
// *************************************************************************
bool CL64_Export::WriteMTLFile(void)
{
	Write_MTLFile = 0;

	char buf[1024];

	Write_MTLFile = fopen(mtl_FileName, "wt");

	if (!Write_MTLFile)
	{
		return 0;
	}

	fprintf(Write_MTLFile, "#\n");
	fprintf(Write_MTLFile, "# Wavefront material file\n");
	fprintf(Write_MTLFile, "# Created with Equity_XL\n");
	fprintf(Write_MTLFile, "#\n");
	fprintf(Write_MTLFile, "%s \n", " ");

	int GroupCount = 0;
	int GroupCountTotal = App->CL_Editor_Com->GroupCount;

	while (GroupCount < GroupCountTotal)
	{
		strcpy(buf, App->CL_Editor_Com->Group[GroupCount]->MaterialName);
		fprintf(Write_MTLFile, "newmtl %s\n", buf);

		fprintf(Write_MTLFile, "illum 2\n");
		fprintf(Write_MTLFile, "Kd 0.800000 0.800000 0.800000\n");
		fprintf(Write_MTLFile, "Ka 0.200000 0.200000 0.200000\n");
		fprintf(Write_MTLFile, "Ks 0.000000 0.000000 0.000000\n");
		fprintf(Write_MTLFile, "Ke 0.000000 0.000000 0.000000\n");
		fprintf(Write_MTLFile, "Ns 0.000000\n");

		strcpy(buf, App->CL_Editor_Com->Group[GroupCount]->Text_FileName);

		fprintf(Write_MTLFile, "map_Kd %s\n", buf);

		fprintf(Write_MTLFile, "%s \n", " ");

		GroupCount++;
	}

	fclose(Write_MTLFile);

	return 1;
}

// *************************************************************************
// *		Export_Object_Textures:- Terry and Hazel Flanigan 2025     	   *
// *************************************************************************
bool CL64_Export::Export_Object_Textures(void)
{
	char buf[MAX_PATH];

	int GroupCount = 0;
	int GroupCountTotal = App->CL_Editor_Com->GroupCount;

	while (GroupCount < GroupCountTotal)
	{
		strcpy(buf, App->CL_Editor_Com->Group[GroupCount]->Text_FileName);
		App->CL_Ogre3D->Export_Texture(buf, Object_OutputFolder);

		GroupCount++;
	}

	return 1;
}

// *************************************************************************
// *	  		Ogre_Export_Dlg:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Export::Ogre_Export_Dlg()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_OGRE_EXPORT, App->MainHwnd, (DLGPROC)Proc_Ogre_Export_Dlg);
}

// *************************************************************************
// *		Proc_Ogre_Export_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Export::Proc_Ogre_Export_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_BANNER, WM_SETFONT, (WPARAM)App->Font_Banner, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_STFILENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STPATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STOGRESUB, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_OGRE_FILENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_OGRE_PATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_OGRE_SUBFOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_OGRE_NAMECHANGE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_OGREBROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_CK_BL_DESKTOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_CREATE_SUBDIR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_BUILDEDGELIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_VERSION, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_VLATEST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_VL18, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_VL17, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_ST_OGRE_FILENAME, (LPCTSTR)App->CL_Export->mJustName);
		SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);
		
		
		strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
		strcat(App->CL_Export->mDirectory_Name, "_Ogre_All");

		SetDlgItemText(hDlg, IDC_ST_OGRE_SUBFOLDER, (LPCTSTR)App->CL_Export->mDirectory_Name);
		
		HWND Temp = GetDlgItem(hDlg, IDC_CK_CREATE_SUBDIR);
		SendMessage(Temp, BM_SETCHECK, 1, 0);

		Temp = GetDlgItem(hDlg, IDC_CK_BUILDEDGELIST);
		if (App->CL_Export->flag_Build_Edge_List == 1)
		{
			SendMessage(Temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			SendMessage(Temp, BM_SETCHECK, 0, 0);
		}
		
		/*if (App->CL_Export->mOgre_Version == Enums::Export_Ogre_Latest)
		{
			Temp = GetDlgItem(hDlg, IDC_CK_VLATEST);
			SendMessage(Temp, BM_SETCHECK, 1, 0);
		}*/

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_ST_OGRE_FILENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_OGRE_PATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_OGRE_SUBFOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_STFILENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STPATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STOGRESUB) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_CK_BL_DESKTOP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_CK_CREATE_SUBDIR) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BUILDEDGELIST) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_ST_VERSION) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		return FALSE;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_OGRE_NAMECHANGE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_OGREBROWSE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_VLATEST)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Export->flag_Version_Latest);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_VL18)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Export->flag_Version_18);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_VL17)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Export->flag_Version_17);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_OGRE_NAMECHANGE)
		{
			strcpy(App->CL_Dialogs->btext, "Change File Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Export->mJustName);

			App->CL_Dialogs->Dialog_Text(Enums::Check_Name_None);

			if (App->CL_Dialogs->flag_Dlg_Canceled == 0)
			{
				strcpy(App->CL_Export->mJustName, App->CL_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_OGRE_FILENAME, App->CL_Export->mJustName);
			
			strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
			strcat(App->CL_Export->mDirectory_Name, "_Ogre_All");

			SetDlgItemText(hDlg, IDC_ST_OGRE_SUBFOLDER, (LPCTSTR)App->CL_Export->mDirectory_Name);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_OGREBROWSE)
		{
			App->CL_File_IO->Select_Folder();
			if (App->CL_File_IO->flag_Canceled == 0)
			{
				strcpy(App->CL_Export->mFolder_Path, App->CL_File_IO->szSelectedDir);
				SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);
			}
			
			return TRUE;
		}
	
		if (LOWORD(wParam) == IDC_CK_BL_DESKTOP)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BL_DESKTOP);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				strcpy(App->CL_Export->mFolder_Path, App->CL_Export->DeskTop_Folder);
				SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Export->mFolder_Path);
				return 1;
			}
			else
			{
				return 1;
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_CK_BUILDEDGELIST)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BUILDEDGELIST);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Export->flag_Build_Edge_List = 1;
				return 1;
			}
			else
			{
				App->CL_Export->flag_Build_Edge_List = 0;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_VLATEST)
		{
			App->CL_Export->mOgre_Version = Enums::Export_Ogre_Latest;

			App->CL_Export->flag_Version_Latest = 1;
			App->CL_Export->flag_Version_18 = 0;
			App->CL_Export->flag_Version_17 = 0;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_VL18)
		{
			App->CL_Export->mOgre_Version = Enums::Export_Ogre_18;

			App->CL_Export->flag_Version_Latest = 0;
			App->CL_Export->flag_Version_18 = 1;
			App->CL_Export->flag_Version_17 = 0;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_VL17)
		{
			App->CL_Export->mOgre_Version = Enums::Export_Ogre_17;

			App->CL_Export->flag_Version_Latest = 0;
			App->CL_Export->flag_Version_18 = 0;
			App->CL_Export->flag_Version_17 = 1;

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			// Check Path
			int result = strcmp(App->CL_Export->mFolder_Path, "");
			if (result == 0)
			{
				App->Say("No Path Selected");
				return 1;
			}

			strcpy(App->CL_Export->mDirectory_Name, App->CL_Export->mJustName);
			strcat(App->CL_Export->mDirectory_Name, "_Ogre_All");

			App->CL_Ogre3D->Export_To_Ogre3D(false);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	}

	return FALSE;
}

