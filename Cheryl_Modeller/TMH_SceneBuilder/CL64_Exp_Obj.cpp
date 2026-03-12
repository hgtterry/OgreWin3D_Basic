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
#include "CL64_Exp_Obj.h"

CL64_Exp_Obj::CL64_Exp_Obj(void)
{
	OutputFolder[0] = 0;
	Object_FileName[0] = 0;
	mtl_FileName[0] = 0;
	Just_mtl_FileName[0] = 0;

	Write_OBJECTFILE = NULL;
	Write_MTLFile = NULL;

	m_Out_JustName[0] = 0;
	m_Out_Folder_Path[0] = 0;
	DeskTop_Folder[0] = 0;

	flag_First_Run = true;
	flag_File_Created = false;
}

CL64_Exp_Obj::~CL64_Exp_Obj(void)
{
}

// *************************************************************************
// *	  		Object_Export_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Exp_Obj::Object_Export_Dlg()
{
	flag_File_Created = false;

	if (flag_First_Run == true)
	{
		strcpy(m_Out_JustName, App->CL_Model->Model_Just_Name);
		strcpy(m_Out_Folder_Path, DeskTop_Folder);

		flag_First_Run = false;
	}

	DialogBox(App->hInst, (LPCTSTR)IDD_OBJECT_EXPORT, App->MainHwnd, (DLGPROC)Proc_Object_Export_Dlg2);

}

// *************************************************************************
// *		Proc_Object_Export_Dlg:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
LRESULT CALLBACK CL64_Exp_Obj::Proc_Object_Export_Dlg2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

		SetDlgItemText(hDlg, IDC_ST_OGRE_FILENAME, (LPCTSTR)App->CL_Exp_Obj->m_Out_JustName);
		SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Exp_Obj->m_Out_Folder_Path);

		strcpy(App->CL_Export->mDirectory_Name, App->CL_Exp_Obj->m_Out_JustName);
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
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Exp_Obj->m_Out_JustName);

			App->CL_Dialogs->Dialog_Text(Enums::Check_Name_None);

			if (App->CL_Dialogs->flag_Dlg_Canceled == 0)
			{
				strcpy(App->CL_Exp_Obj->m_Out_JustName, App->CL_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_OGRE_FILENAME, App->CL_Exp_Obj->m_Out_JustName);

			strcpy(App->CL_Export->mDirectory_Name, App->CL_Exp_Obj->m_Out_JustName);
			strcat(App->CL_Export->mDirectory_Name, "_Object_All");

			SetDlgItemText(hDlg, IDC_ST_OGRE_SUBFOLDER, (LPCTSTR)App->CL_Export->mDirectory_Name);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_OGREBROWSE)
		{
			App->CL_File_IO->Select_Folder();
			if (App->CL_File_IO->flag_Canceled == 0)
			{
				strcpy(App->CL_Exp_Obj->m_Out_Folder_Path, App->CL_File_IO->szSelectedDir);
				SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Exp_Obj->m_Out_Folder_Path);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_BL_DESKTOP)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BL_DESKTOP);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				strcpy(App->CL_Exp_Obj->m_Out_Folder_Path, App->CL_Exp_Obj->DeskTop_Folder);
				SetDlgItemText(hDlg, IDC_ST_OGRE_PATH, (LPCTSTR)App->CL_Exp_Obj->m_Out_Folder_Path);
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
			int result = strcmp(App->CL_Exp_Obj->m_Out_Folder_Path, "");
			if (result == false)
			{
				App->Say("No Path Selected");
				return 1;
			}

			App->CL_Exp_Obj->Create_ObjectFile();
			App->CL_Exp_Obj->flag_File_Created = true;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Exp_Obj->flag_File_Created = false;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	}

	return FALSE;
}

// *************************************************************************
// *			Create_ObjectFile:- Terry and Hazel Flanigan 2026  	 	   *
// *************************************************************************
bool CL64_Exp_Obj::Create_ObjectFile(void)
{
	strcpy(OutputFolder, "");

	char buff[MAX_PATH];
	strcpy(buff, m_Out_Folder_Path);
	strcat(buff, "\\");
	strcat(buff, m_Out_JustName);
	strcat(buff, "_Object");
	strcat(buff, "\\");

	strcpy(OutputFolder, buff);
	
	CreateDirectory(OutputFolder, NULL);

	strcpy(Object_FileName, OutputFolder);
	strcat(Object_FileName, m_Out_JustName);
	strcat(Object_FileName, ".obj");

	strcpy(mtl_FileName, OutputFolder);
	strcat(mtl_FileName, m_Out_JustName);
	strcat(mtl_FileName, ".mtl");

	strcpy(Just_mtl_FileName, m_Out_JustName);
	strcat(Just_mtl_FileName, ".mtl");

	Write_ObjectFile();

	if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
	{
		WriteMTLFile_Ogre();
		Export_Textures_Ogre();
	}
	else
	{
		WriteMTLFile_Assimp();
		Export_Textures_Assimp(OutputFolder);
	}

	return 1;
}

// *************************************************************************
// *		Write_ObjectFile:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Exp_Obj::Write_ObjectFile(void)
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
// *		Write_ObjectFile_Commit:- Terry and Hazel Flanigan 2024  	   *
// *************************************************************************
void CL64_Exp_Obj::Write_ObjectFile_Commit(void)
{

	fprintf(Write_OBJECTFILE, "#\n");
	fprintf(Write_OBJECTFILE, "# Wavefront OBJ file\n");
	fprintf(Write_OBJECTFILE, "# Created with Cheryl 3D Modeller\n");
	fprintf(Write_OBJECTFILE, "#\n");
	fprintf(Write_OBJECTFILE, "mtllib %s\n", Just_mtl_FileName);
	fprintf(Write_OBJECTFILE, "%s \n", " ");

	int	VertCount = 0;
	int GroupCount = 0;
	int VertTextCords = 0;
	int VertNormals = 0;
	int FaceCount = 0;
	int FaceIndex = 1;

	int GroupCountTotal = App->CL_Model->GroupCount;

	char MaterialName[MAX_PATH];

	//--------------------------------------------- Vertices v-

	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Model->VerticeCount);

	while (GroupCount < GroupCountTotal)
	{
		VertCount = 0;
		while (VertCount < App->CL_Mesh->Group[GroupCount]->GroupVertCount)
		{
			float X = App->CL_Mesh->Group[GroupCount]->vertex_Data[VertCount].x;
			float Y = App->CL_Mesh->Group[GroupCount]->vertex_Data[VertCount].y;
			float Z = App->CL_Mesh->Group[GroupCount]->vertex_Data[VertCount].z;

			fprintf(Write_OBJECTFILE, "v %.06f %.06f %.06f\n", X, Y, Z);

			VertCount++;
		}
		GroupCount++;
	}

	fprintf(Write_OBJECTFILE, "%s \n", " ");

	//--------------------------------------------- Texture Cords vt-

	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Model->VerticeCount);

	GroupCount = 0;

	while (GroupCount < GroupCountTotal)
	{
		VertTextCords = 0;
		while (VertTextCords < App->CL_Mesh->Group[GroupCount]->GroupVertCount)
		{
			float U = App->CL_Mesh->Group[GroupCount]->MapCord_Data[VertTextCords].u;
			float V = App->CL_Mesh->Group[GroupCount]->MapCord_Data[VertTextCords].v;

			fprintf(Write_OBJECTFILE, "vt %.06f %.06f\n", U, V);

			VertTextCords++;
		}
		GroupCount++;
	}

	fprintf(Write_OBJECTFILE, "%s \n", " ");

	//--------------------------------------------- Normals vn-

	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Model->VerticeCount);

	GroupCount = 0;

	while (GroupCount < GroupCountTotal)
	{
		VertNormals = 0;
		while (VertNormals < App->CL_Mesh->Group[GroupCount]->GroupVertCount)
		{
			float X = App->CL_Mesh->Group[GroupCount]->Normal_Data[VertNormals].x;
			float Y = App->CL_Mesh->Group[GroupCount]->Normal_Data[VertNormals].y;
			float Z = App->CL_Mesh->Group[GroupCount]->Normal_Data[VertNormals].z;

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

		if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
		{
			strcpy(MaterialName, App->CL_Mesh->Group[GroupCount]->Ogre_Material);
		}
		else
		{
			strcpy(MaterialName, App->CL_Mesh->Group[GroupCount]->MaterialName);
		}

		fprintf(Write_OBJECTFILE, "usemtl %s\n", MaterialName);

		while (FaceCount < App->CL_Mesh->Group[GroupCount]->GroupFaceCount)
		{
			int A0 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].a + Offset;
			int B0 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].a + Offset;
			int C0 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].a + Offset;

			int A1 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].b + Offset;
			int B1 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].b + Offset;
			int C1 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].b + Offset;

			int A2 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].c + Offset;
			int B2 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].c + Offset;
			int C2 = App->CL_Mesh->Group[GroupCount]->Face_Data[FaceCount].c + Offset;

			FaceCount++;

			fprintf(Write_OBJECTFILE, "f %i/%i/%i %i/%i/%i %i/%i/%i\n", A0, B0, C0, A1, B1, C1, A2, B2, C2);

		}

		Offset = Offset + App->CL_Mesh->Group[GroupCount]->GroupVertCount;

		fprintf(Write_OBJECTFILE, "%s \n", " ");
		GroupCount++;
	}
}

// *************************************************************************
// *		WriteMTLFile_Assimp:- Terry and Hazel Flanigan 2026       	   *
// *************************************************************************
bool CL64_Exp_Obj::WriteMTLFile_Assimp(void)
{
	Write_MTLFile = 0;

	char buf[MAX_PATH];

	Write_MTLFile = fopen(mtl_FileName, "wt");

	if (!Write_MTLFile)
	{
		return 0;
	}

	fprintf(Write_MTLFile, "#\n");
	fprintf(Write_MTLFile, "# Wavefront material file\n");
	fprintf(Write_MTLFile, "# Created with Cheryl 3D Modeller\n");
	fprintf(Write_MTLFile, "#\n");
	fprintf(Write_MTLFile, "%s \n", " ");

	int GroupCount = 0;
	int GroupCountTotal = App->CL_Model->GroupCount;

	while (GroupCount < GroupCountTotal)
	{
		strcpy(buf, App->CL_Mesh->Group[GroupCount]->MaterialName);
		fprintf(Write_MTLFile, "newmtl %s\n", buf);

		fprintf(Write_MTLFile, "illum 2\n");
		fprintf(Write_MTLFile, "Kd 0.800000 0.800000 0.800000\n");
		fprintf(Write_MTLFile, "Ka 0.200000 0.200000 0.200000\n");
		fprintf(Write_MTLFile, "Ks 0.000000 0.000000 0.000000\n");
		fprintf(Write_MTLFile, "Ke 0.000000 0.000000 0.000000\n");
		fprintf(Write_MTLFile, "Ns 0.000000\n");

		strcpy(buf, App->CL_Mesh->Group[GroupCount]->Assimp_Text_FileName);
		fprintf(Write_MTLFile, "map_Kd %s\n", buf);

		fprintf(Write_MTLFile, "%s \n", " ");

		GroupCount++;
	}

	fclose(Write_MTLFile);

	return 1;
}

// *************************************************************************
// *		Export_Textures_Assimp:- Terry and Hazel Flanigan 2026 	   	   *
// *************************************************************************
bool CL64_Exp_Obj::Export_Textures_Assimp(char* Path)
{
	if (App->CL_Model->Model_Type == Enums::Model_Type_Assimp)
	{
		int MatCount = App->CL_Model->GroupCount;
		char PathandFile[MAX_PATH];
		char Texture_Folder_Path[MAX_PATH];

		Ogre::String mFileString;

		int Loop = 0;
		while (Loop < MatCount)
		{
			strcpy(Texture_Folder_Path, App->CL_Mesh->Group[Loop]->Assimp_Texture_FolderPath);

			strcpy(PathandFile, Texture_Folder_Path);
			strcat(PathandFile, App->CL_Mesh->Group[Loop]->Assimp_Text_FileName);

			mFileString.clear();
			bool image_loaded = false;

			std::ifstream ifs(PathandFile, std::ios::binary | std::ios::in);
			if (ifs.is_open())
			{
				Ogre::DataStreamPtr data_stream(new Ogre::FileStreamDataStream(PathandFile, &ifs, false));

				mFileString = data_stream->getAsString();

				char mFileName[MAX_PATH];
				strcpy(mFileName, App->CL_Exp_Obj->OutputFolder);
				strcat(mFileName, App->CL_Mesh->Group[Loop]->Assimp_Text_FileName);

				std::ofstream outFile;
				outFile.open(mFileName, std::ios::binary);
				outFile << mFileString;
				outFile.close();

				mFileString.clear();

				ifs.close();

				image_loaded = true;
			}

			Loop++;
		}
	}

	return 1;
}

// *************************************************************************
// *			WriteMTLFile_Ogre:- Terry and Hazel Flanigan 2024      	   *
// *************************************************************************
bool CL64_Exp_Obj::WriteMTLFile_Ogre(void)
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
	fprintf(Write_MTLFile, "# Created with OgreWin3D\n");
	fprintf(Write_MTLFile, "#\n");
	fprintf(Write_MTLFile, "%s \n", " ");

	int GroupCount = 0;
	int GroupCountTotal = App->CL_Model->GroupCount;

	while (GroupCount < GroupCountTotal)
	{
		strcpy(buf, App->CL_Mesh->Group[GroupCount]->Ogre_Material);
		fprintf(Write_MTLFile, "newmtl %s\n", buf);

		fprintf(Write_MTLFile, "illum 2\n");
		fprintf(Write_MTLFile, "Kd 0.800000 0.800000 0.800000\n");
		fprintf(Write_MTLFile, "Ka 0.200000 0.200000 0.200000\n");
		fprintf(Write_MTLFile, "Ks 0.000000 0.000000 0.000000\n");
		fprintf(Write_MTLFile, "Ke 0.000000 0.000000 0.000000\n");
		fprintf(Write_MTLFile, "Ns 0.000000\n");

		if (App->CL_Mesh->Group[GroupCount]->Ogre_Texture_IsValid == 1)
		{
			strcpy(buf, App->CL_Mesh->Group[GroupCount]->v_Texture_Names[0].c_str());
		}
		else
		{
			strcpy(buf, "Dummy.bmp");
		}

		fprintf(Write_MTLFile, "map_Kd %s\n", buf);

		fprintf(Write_MTLFile, "%s \n", " ");

		GroupCount++;
	}

	fclose(Write_MTLFile);

	return 1;
}

// *************************************************************************
// *		Export_Textures_Ogre:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
bool CL64_Exp_Obj::Export_Textures_Ogre()
{
	Ogre::String mFileString;
	mFileString.clear();

	char Path_and_File[MAX_PATH];

	int GroupCount = 0;
	int GroupCountTotal = App->CL_Model->GroupCount;

	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Resources->Ogre_Loader_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	while (GroupCount < GroupCountTotal)
	{
		for (i = RFI->begin(); i != iend; ++i)
		{
			if (i->filename == App->CL_Mesh->Group[GroupCount]->v_Texture_Names[0].c_str()) // Texture Exsists
			{
				Ogre::DataStreamPtr ff = i->archive->open(i->filename);

				mFileString = ff->getAsString();

				Path_and_File[0] = 0;
				strcpy(Path_and_File, OutputFolder);
				strcat(Path_and_File, App->CL_Mesh->Group[GroupCount]->v_Texture_Names[0].c_str());

				std::ofstream outFile;
				outFile.open(Path_and_File, std::ios::binary);
				outFile << mFileString;
				outFile.close();

				mFileString.clear();
			}
			else
			{
				HBITMAP hbmpTemp;
				hbmpTemp = App->CL_Textures->DummyTexture_To_HBITMAP();

				char OutFile[MAX_PATH];
				strcpy(OutFile, OutputFolder);
				strcat(OutFile, "Dummy.bmp");

				App->CL_Textures->HBITMAP_TO_BmpFile(hbmpTemp, OutFile, (LPSTR)"");
			}

		}

		GroupCount++;

	}

	return 1;
}
