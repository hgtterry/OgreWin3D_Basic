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
#include "CL64_File.h"
#include "Structures.cpp"

CL64_File::CL64_File(void)
{
	PathFileName_3dt[0] = 0;
	FileName_3dt[0] = 0;

	Read_Buffer[0] = 0;
	flag_loading = 0;

	// MTF File
	strcpy(MTF_PathAndFile, "");
	strcat(MTF_PathAndFile, "New_Model.cbf");

	strcpy(MTF_Just_FileName, "New_Model.cbf");
	strcpy(MTF_Just_Name, "New_Model");
	strcpy(MTF_Just_Path, "No_Path");
	strcpy(MTF_FolderPath, "");
	
	strcpy(Prj_Working_Folder, "No_Path");

	strcpy(Loaded_PathFileName, "No File");
	strcpy(Loaded_FileName, "No File");
	strcpy(Model_Just_Name, "No Name");
	strcpy(Model_FolderPath, "No Path");

	MTF_Directory_Name[0] = 0;

	fp = NULL;
}

CL64_File::~CL64_File(void)
{
}

// *************************************************************************
// *	  	Start_Model_Export_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_File::Start_Model_Export_Dlg()
{
	int brushCount = App->CL_X_Brush->Get_Brush_Count();
	if (brushCount <= 0)
	{
		App->Say("No Brushes to Save");
		return;
	}

	DialogBox(App->hInst, (LPCTSTR)IDD_EXPORT_BRUSH, App->MainHwnd, (DLGPROC)Proc_Model_Export_Dlg);

}

// *************************************************************************
// *		Proc_Model_Export_Dlg:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_File::Proc_Model_Export_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_MODEL_BANNER, WM_SETFONT, (WPARAM)App->Font_Banner, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_MODEL_FILENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_MODEL_PATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_MODEL_SUBFOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_MODEL_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_ST_MODEL_PATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_STMODELSUB, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_MODEL_NAMECHANGE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_MODEL_BROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CK_BL_MODEL_DESKTOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_MODEL_CREATE_SUBDIR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_ST_MODEL_FILENAME, (LPCTSTR)App->CL_File->MTF_Just_Name);
		SetDlgItemText(hDlg, IDC_ST_MODEL_PATH, (LPCTSTR)App->CL_File->MTF_FolderPath);

		strcpy(App->CL_File->MTF_Directory_Name, App->CL_File->MTF_Just_Name);
		strcat(App->CL_File->MTF_Directory_Name, "_C3D_prj");

		SetDlgItemText(hDlg, IDC_ST_MODEL_SUBFOLDER, (LPCTSTR)App->CL_File->MTF_Directory_Name);

		HWND Temp = GetDlgItem(hDlg, IDC_CK_MODEL_CREATE_SUBDIR);
		SendMessage(Temp, BM_SETCHECK, 1, 0);

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_MODEL_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_MODEL_FILENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_MODEL_PATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_MODEL_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_ST_MODEL_PATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_STMODELSUB) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BL_MODEL_DESKTOP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_MODEL_CREATE_SUBDIR) == (HWND)lParam)
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

		if (some_item->idFrom == IDC_BT_MODEL_NAMECHANGE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_MODEL_BROWSE)
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
		if (LOWORD(wParam) == IDC_BT_MODEL_NAMECHANGE)
		{
			strcpy(App->CL_Dialogs->btext, "Change File Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_File->MTF_Just_Name);

			App->CL_Dialogs->Dialog_Text(Enums::Check_Name_None);

			if (App->CL_Dialogs->flag_Dlg_Canceled == 0)
			{
				strcpy(App->CL_File->MTF_Just_Name, App->CL_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_MODEL_FILENAME, App->CL_File->MTF_Just_Name);

			strcpy(App->CL_File->MTF_Directory_Name, App->CL_File->MTF_Just_Name);
			strcat(App->CL_File->MTF_Directory_Name, "_C3D_prj");

			SetDlgItemText(hDlg, IDC_ST_MODEL_SUBFOLDER, (LPCTSTR)App->CL_File->MTF_Directory_Name);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_MODEL_BROWSE)
		{
			App->CL_File_IO->Select_Folder();
			if (App->CL_File_IO->flag_Canceled == 0)
			{
				strcpy(App->CL_File->MTF_FolderPath, App->CL_File_IO->szSelectedDir);
				SetDlgItemText(hDlg, IDC_ST_MODEL_PATH, (LPCTSTR)App->CL_File->MTF_FolderPath);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_BL_MODEL_DESKTOP)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BL_MODEL_DESKTOP);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				strcpy(App->CL_File->MTF_FolderPath, App->CL_Exp_Obj->DeskTop_Folder);
				SetDlgItemText(hDlg, IDC_ST_MODEL_PATH, (LPCTSTR)App->CL_File->MTF_FolderPath);
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
			int result = strcmp(App->CL_File->MTF_FolderPath, "");
			if (result == false)
			{
				App->Say("No Path Selected");
				return 1;
			}

			strcpy(App->CL_File->MTF_PathAndFile, App->CL_File->MTF_FolderPath);
			strcat(App->CL_File->MTF_PathAndFile, "\\");
			strcat(App->CL_File->MTF_PathAndFile, App->CL_File->MTF_Just_Name);
			strcat(App->CL_File->MTF_PathAndFile, ".cbf");

			App->CL_File->Start_Save();

			//App->CL_Exp_Obj->flag_File_Created = true;
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
// *				 Start_Save:- Terry Mo and Hazel 2026			       *
// *************************************************************************
void CL64_File::Start_Save()
{
	// Check there are brushes to Save
	int brushCount = App->CL_X_Brush->Get_Brush_Count();
	if (brushCount <= 0)
	{
		App->Say("No Brushes to Save");
		return;
	}

	// Create Working Folder
	char ProjectFolder[MAX_PATH];
	strcpy(ProjectFolder, App->CL_File->MTF_FolderPath);
	strcat(ProjectFolder, "\\");
	strcat(ProjectFolder, App->CL_File->MTF_Directory_Name);
	strcat(ProjectFolder, "\\");
	CreateDirectory(ProjectFolder, NULL);

	App->CL_Level->flag_Working_Folder_Exists = true;

	strcpy(MTF_Just_FileName, MTF_Just_Name);
	strcat(MTF_Just_FileName, ".cbf");

	/*char Path_And_File[MAX_PATH];
	strcpy(Path_And_File, ProjectFolder);
	strcat(Path_And_File, MTF_Just_FileName);*/

	//Save_Document(Path_And_File);

	char New_Path_And_File[MAX_PATH];
	strcpy(New_Path_And_File, App->CL_File->MTF_FolderPath);
	strcat(New_Path_And_File, "\\");
	strcat(New_Path_And_File, MTF_Just_FileName);

	Save_Document(New_Path_And_File);
	
	// Save Texture Zip Version 2.0
	//-------------------------------------------------
	std::string Source = App->CL_Level->TXL_PathAndFile;
	std::string Destination = std::string(ProjectFolder);

	char Zip_File[MAX_PATH];
	strcpy(Zip_File, MTF_Just_Name);
	strcat(Zip_File, ".zip");

	Destination.append(Zip_File);

	// Check if the source and destination are different
	if (Destination != Source)
	{
		if (!CopyFile(Source.c_str(),Destination.c_str(), false))
		{
			App->Say("Error","Failed to copy Texture Zip file");
			return;
		}
	}
	
	// Update the level's Texture Zip file paths
	strcpy(App->CL_Level->TXL_PathAndFile,Destination.c_str());
	strcpy(App->CL_Level->TXL_Just_File_Name, Zip_File);

	App->Set_Title(MTF_PathAndFile);
	App->CL_Level->flag_File_Been_Saved = true;
	App->CL_Libs->CL_Preference->Save_Config_File();

	App->Say("Saved", MTF_Just_FileName);
}

// *************************************************************************
// *	          Save_Document:- Terry and Hazel Flanigan 2026		       *
// *************************************************************************
void CL64_File::Save_Document(const char* Path_And_File)
{
	if (Save(Path_And_File) == false)
	{
		App->Say("Error: Unable to save file");
		return;
	}

	App->CL_Doc->flag_IsNewDocument = false;
	App->CL_Model->flag_Model_is_Modified = false;
}

// *************************************************************************
// *					Save:- :- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
bool CL64_File::Save(const char* FileName)
{
	// Construct the TXL file name
	std::string TXL_File_Name = std::string(MTF_Just_Name) + ".zip";

	// Open the file for writing
	FILE* Write_File = fopen(FileName, "wt");
	if (!Write_File) 
	{
		App->Say("Error: Failed to open file for writing");
		return false;
	}

	// Write version and texture library name to the file
	float Version = 2.0;
	fprintf(Write_File, "MTF_Version %.2f\n", Version);
	fprintf(Write_File, "TextureLib %s\n", TXL_File_Name.c_str());

	// Write brush list to the file
	BrushList_Write(App->CL_Doc->Current_Level->Brushes, Write_File);

	// Close the file
	fclose(Write_File);

	return true;
}

// *************************************************************************
// *			BrushList_Write:- Terry Mo and Hazel 2026				   *
// *************************************************************************
signed int CL64_File::BrushList_Write(BrushList* BList, FILE* ofile)
{
	if (!BList || !ofile) return false; // Check for null pointers

	Brush* pBrush;
	BrushIterator bi;
	int Count;

	Count = App->CL_X_Brush->BrushList_Count(BList, (BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF | BRUSH_COUNT_NORECURSE));
	if (fprintf(ofile, "Brushlist %d\n", Count) < 0) return false;

	pBrush = App->CL_X_Brush->BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		if (!Brush_Write(pBrush, ofile)) return false;
		pBrush = App->CL_X_Brush->BrushList_GetNext(&bi);
	}

	return true;
}

// *************************************************************************
// *			BrushList_Write:- Terry Mo and Hazel 2026				   *
// *************************************************************************
signed int CL64_File::Brush_Write(const Brush* b, FILE* ofile)
{
	if (!b || !ofile) return false; // Check for null pointers

	if (b->Type == BRUSH_CSG)
	{
		return true;
	}

	{
		if (fprintf(ofile, "Brush %s\n", b->Name) < 0) return GE_FALSE;
	}

	if (fprintf(ofile, "\tFlags %d\n", b->Flags) < 0) return GE_FALSE;
	if (fprintf(ofile, "\tModelId %d\n", b->ModelId) < 0) return GE_FALSE;
	if (fprintf(ofile, "\tGroupId %d\n", b->GroupId) < 0) return GE_FALSE;
	{
		if (b->HullSize < 1.0f)
		{
			((Brush*)b)->HullSize = 1.0f;
		}
		if (fprintf(ofile, "\tHullSize %f\n", b->HullSize) < 0) return GE_FALSE;
	}
	if (fprintf(ofile, "\tType %d\n", b->Type) < 0) return GE_FALSE;

	switch (b->Type)
	{
	case	BRUSH_MULTI:
		return BrushList_Write(b->BList, ofile);

	case	BRUSH_LEAF:
		return FaceList_Write(b->Faces, ofile);

	default:
		assert(0);		// invalid brush type
		break;
	}
	return true;
}

// *************************************************************************
// *			FaceList_Write:- Terry Mo and Hazel 2026				   *
// *************************************************************************
signed int CL64_File::FaceList_Write(const FaceList* pList, FILE* f) 
{
	// Check for null pointers and return false if any are found
	if (!pList || !f) {
		return false;
	}

	// Write the number of faces to the file and check for errors
	if (fprintf(f, "\tBrushFaces %d\n", pList->NumFaces) < 0) 
	{
		return false;
	}

	// Iterate through each face and write it to the file
	for (int i = 0; i < pList->NumFaces; i++) 
	{
		if (!Face_Write(pList->Faces[i], f)) {
			return false;
		}
	}

	return true; // Return true if all faces were written successfully
}

// *************************************************************************
// *				Face_Write:- Terry Mo and Hazel 2026				   *
// *************************************************************************
signed int CL64_File::Face_Write(const Face* f, FILE* wf)
{
	// Check for null pointers
	if (!f || !wf) return false;

	// Write number of points and flags
	fprintf(wf, "\t\tNumPoints %d\n", f->NumPoints);
	fprintf(wf, "\t\tFlags %d\n", f->Flags);

	// Write each point's coordinates
	for (int i = 0; i < f->NumPoints; i++) 
	{
		if (fprintf(wf, "\t\t\tVec3d %f %f %f\n", f->Points[i].x, f->Points[i].y, f->Points[i].z) < 0) 
		{
			return GE_FALSE; // Handle write error
		}
	}

	// Retrieve texture information
	int xShift, yShift;
	float xScale, yScale, Rotate;
	App->CL_X_Face->Face_GetTextureShift(f, &xShift, &yShift);
	App->CL_X_Face->Face_GetTextureScale(f, &xScale, &yScale);
	Rotate = App->CL_X_Face->Face_GetTextureRotate(f);

	// Write texture information
	fprintf(wf, "\t\t\tTexInfo Rotate %f Shift %d %d Scale %f %f Name %s\n",
		Rotate, xShift, yShift, xScale, yScale, App->CL_X_Face->Face_GetTextureName(f));

	// Write transformation data
	fprintf(wf, "\tTransform %f %f %f %f %f %f %f %f %f %f %f %f\n",
		f->Tex.XfmFaceAngle.AX, f->Tex.XfmFaceAngle.AY, f->Tex.XfmFaceAngle.AZ,
		f->Tex.XfmFaceAngle.BX, f->Tex.XfmFaceAngle.BY, f->Tex.XfmFaceAngle.BZ,
		f->Tex.XfmFaceAngle.CX, f->Tex.XfmFaceAngle.CY, f->Tex.XfmFaceAngle.CZ,
		f->Tex.XfmFaceAngle.Translation.x, f->Tex.XfmFaceAngle.Translation.y, f->Tex.XfmFaceAngle.Translation.z);

	// Write position data
	fprintf(wf, "\tPos %f %f %f\n", f->Tex.Pos.x, f->Tex.Pos.y, f->Tex.Pos.z);

	return true;
}


// ----------------------------------------------------------------------
// --------------------------- Open -------------------------------------
// ----------------------------------------------------------------------

// *************************************************************************
// *	           Start_Load:- Terry Mo and Hazel 2026				       *
// *************************************************************************
void CL64_File::Start_Load(bool useOpenDialog)
{
	if (useOpenDialog)
	{
		LPCWSTR mType = L"Mesh Text File";
		LPCWSTR mExtensions = L"*.cbf";

		bool test = App->CL_File_IO->Open_File((LPCWSTR)mType, (LPCWSTR)mExtensions);
		if (test == false)
		{
			return;
		}

		strcpy(PathFileName_3dt, App->CL_File_IO->s_Path_And_File.c_str());
		strcpy(FileName_3dt, App->CL_File_IO->s_Just_FileName.c_str());

		strcpy(MTF_PathAndFile, App->CL_File_IO->s_Path_And_File.c_str());
		strcpy(MTF_Just_FileName, App->CL_File_IO->s_Just_FileName.c_str());

		char buf[MAX_PATH];
		strcpy(buf, MTF_Just_FileName);
		int Len = strlen(buf);
		buf[Len - 4] = 0;
		strcpy(MTF_Just_Name, buf);

		strcpy(App->CL_Export->mJustName, MTF_Just_Name);

	}
	
	flag_loading = true;

	// Clear the current level
	App->CL_Model->Clear_Model();
	App->CL_Editor_Control->Set_Editor_Design_Model();

	
	// Attempt to open the CBF file
	if (Open_3dt_File())
	{
		App->CL_Doc->Do_General_Select_Dlg(false);

		Set_Editor();

		App->CL_Camera->Reset_View_and_Zoom();

		App->CL_Interface->Set_Menu_Items_Design();

		App->CL_Libs->CL_Preference->Save_Config_File();
		App->CL_Ogre->OGL_Listener->Show_Visuals(false);

		
		

		if (App->CL_Libs->CL_Preference->flag_OpenLastFile == false)
		{
			App->Say("File Loaded", App->CL_File->FileName_3dt);
		}
		
		App->CL_Level->flag_File_Been_Saved = true;

		App->CL_Doc->Do_General_Select_Dlg(true);


		//std::string LastFolder = App->CL_Utilities->Get_Directory_From_Path(MTF_PathAndFile);
		//strcpy(App->CL_File->MTF_FolderPath, App->CL_Utilities->Get_Directory_From_Path(LastFolder).c_str());

		App->CL_Model->Set_BondingBox_Model();
	}
	else
	{
		App->Say("Can Not Find File", FileName_3dt);
	}

	flag_loading = false;
}

// *************************************************************************
// *							Open_3dt_File 							   *
// *************************************************************************
bool CL64_File::Open_3dt_File()
{
	App->CL_Doc->ResetAllSelections();
	App->CL_Doc->Do_General_Select_Dlg(false);

	// Load Brushes MTF File
	if (!Load_File(PathFileName_3dt))
	{
		return false;
	}
	
	App->CL_Doc->Set_Paths();

	// Check if Working Folder Exsits
	char Work_Folder[MAX_PATH];
	strcpy(Work_Folder, MTF_Just_Path);
	strcat(Work_Folder, MTF_Just_Name);
	strcat(Work_Folder, "_C3D_prj");
	bool Folder_Test = App->CL_Utilities->Check_Directory_Exists(Work_Folder);
	if (Folder_Test == true)
	{
		App->CL_Level->flag_Working_Folder_Exists = true;

		strcat(Work_Folder, "\\");
		strcpy(Prj_Working_Folder, Work_Folder);
	}
	else
	{
		App->CL_Level->flag_Working_Folder_Exists = false;
		strcpy(Prj_Working_Folder, "None");
	}
	
	// Load Texture Zip File
	char pathAndFile[MAX_PATH];

	if (App->CL_Level->Level_Version == 1.0)
	{
		strcpy(pathAndFile, MTF_Just_Path);
		strcat(pathAndFile, App->CL_Level->TXL_Just_File_Name); // Gets it from MTF File

		if (!App->CL_Utilities->Check_File_Exist(pathAndFile))
		{
			App->Say("Texture Library Does Not Exist", "Loading Default");
			strcpy(pathAndFile, App->App_Directory_FullPath);
			strcat(pathAndFile, "\\Data\\Room_Builder\\Default.zip");
		}

		App->Say("File Version is 1.0","Please Re-Save to Update 1.5");
	}

	if (App->CL_Level->Level_Version == 2.0)// && App->CL_Level->flag_Working_Folder_Exists == true)
	{
		strcpy(pathAndFile, Prj_Working_Folder);
		strcat(pathAndFile, App->CL_Level->TXL_Just_File_Name); // Gets it from MTF File

		if (!App->CL_Utilities->Check_File_Exist(pathAndFile))
		{
			strcpy(pathAndFile, MTF_Just_Path); // Old System
			strcat(pathAndFile, App->CL_Level->TXL_Just_File_Name); // Gets it from MTF File

			if (!App->CL_Utilities->Check_File_Exist(pathAndFile))
			{
				App->Say("Texture Library Does Not Exist", "Loading Default");
				strcpy(pathAndFile, App->App_Directory_FullPath);
				strcat(pathAndFile, "\\Data\\Room_Builder\\Default.zip");
			}
			else
			{
				std::string LastFolder = App->CL_Utilities->Get_Directory_From_Path(MTF_PathAndFile);
				strcpy(App->CL_File->MTF_FolderPath, App->CL_Utilities->Get_Directory_From_Path(LastFolder).c_str());
			}
		}
		else
		{
			char Just_Path[MAX_PATH];
			strcpy(Just_Path, MTF_Just_Path);
			
			int Len1 = strlen(Just_Path);
			Just_Path[Len1 - 1] = 0;

			strcpy(App->CL_File->MTF_FolderPath, Just_Path);
		}


	}
	else
	{
		App->Say("Texture Library Does Not Exist", "Loading Default");
		strcpy(pathAndFile, App->App_Directory_FullPath);
		strcat(pathAndFile, "\\Data\\Room_Builder\\Default.zip");
	}

	// Acctualy Load Texture Zip File
	App->CL_TXL_Editor->Load_Zipped_TXL_File(pathAndFile);
	App->CL_Doc->UpdateAfterWadChange();

	// Update Editor
	App->CL_Properties_Brushes->Fill_ListBox();
	App->CL_Properties_Textures->Fill_ListBox();
	App->CL_Properties_Textures->List_Selection_Changed();

	App->CL_Ogre->Listener_3D->CameraMode = Enums::Cam_Mode_Free;
	App->CL_Doc->Set_Faces_To_Brush_Name_All(); // TODO: Fix up Brush Names and set Indexes
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);

	return true;
}

// *************************************************************************
// *			Load_File:- Terry Mo and Hazel 2026 					   *
// *************************************************************************
bool CL64_File::Load_File(const char* FileName)
{
	Level* m_pLevel = nullptr;

	int Count = 0;

	fp = fopen(FileName, "r");
	if (!fp)
	{
		return false;
	}

	memset(Read_Buffer, 0, MAX_PATH);

	// Read File Line by Line 
	while (fgets(Read_Buffer, sizeof(Read_Buffer), fp))
	{
		App->CL_ParseFile->Tag_Float = 0;

		if (App->CL_ParseFile->Get_Version(Read_Buffer) == false)
		{
			return false;
		}
		/*else
		{
			App->Say_Float(App->CL_Level->Level_Version);
		}*/
		
		// Create Clean Level
		m_pLevel = App->CL_Level->Level_Create();

		fgets(Read_Buffer, sizeof(Read_Buffer), fp);
		if (App->CL_ParseFile->Get_TextureLib(Read_Buffer) == 0)
		{
			break;
		}

		// Clean up existing brushes if they exist
		if (App->CL_Doc->Current_Level->Brushes != NULL)
		{
			App->CL_X_Brush->BrushList_Destroy(&App->CL_Doc->Current_Level->Brushes);
			App->CL_Doc->Current_Level->Brushes = NULL;
		}
		
		if (m_pLevel == NULL)
		{
			App->Say("Can not Create Level");
			break;
		}
		
		// Create brushes from file
		m_pLevel->Brushes = App->CL_ParseFile->BrushList_CreateFromFile(false);
		if (m_pLevel->Brushes == NULL)
		{
			App->Say("Can not Create Brushes");
			break;
		}
		
		break;

		Count++;
		memset(Read_Buffer, 0, MAX_PATH);
	}

	fclose(fp);
	
	// Make Current Level
	App->CL_Doc->Current_Level = m_pLevel;

	return true;
}

// *************************************************************************
// *			Set_Editor:- Terry Mo and Hazel 2026 					   *
// *************************************************************************
void CL64_File::Set_Editor() 
{
	// Set title based on the current level's path and file
	App->Set_Title(MTF_PathAndFile);

	// Enable the select button and disable the insert button in the properties templates
	//App->CL_Top_Tabs->Enable_Select_Button(true, 1);

	// Deselect any selected face dialog buttons
	App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();

	// Reset the selected face index and all selections in the document
	App->CL_X_Face->Selected_Face_Index = 0;
	App->CL_Doc->ResetAllSelections();

	// Disable brush options buttons and select the templates tab
	//App->CL_Top_Tabs->Enable_TopBar_Brush_Buttons(false, false);
	//App->CL_Top_Tabs->Enable_TopBar_Face_Buttons(false);
	App->CL_Properties_Tabs->Select_Templates_Tab();

	// Reset the camera and views
	App->CL_Ogre->Camera_Reset_Zero();

	if (App->CL_Editor_Control->flag_Scene_Editor_Active == false)
	{
		App->CL_Views_Com->Reset_Views_All();
	}

	// Set the editor dialog to the first brush
	App->CL_Doc->Editor_Set_Dlgs(Enums::Editor_Dlgs_First_Brush);
	App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(false);

	// Create a test environment if in development mode and level version is 1.0
	if (App->Development == 1 && App->CL_Level->Level_Version == 1.0) 
	{
		//App->CL_Com_Environments->Create_Test_Environment();
	}

	// Hide entities in the object component
	//App->CL_Com_Objects->Show_Entities(false);

	// Get the first environment index and set it if valid
	/*int index = App->CL_Com_Environments->Get_First_Environ();
	if (index != -1) 
	{
		App->CL_Com_Environments->Set_Environment_By_Index(false, index);
	}*/

	// Set the selected render mode and apply textured camera
	App->CL_Mesh_Mgr->Selected_Render_Mode = Enums::Render_Ogre;
	App->CL_Camera->Camera_Textured();

	// Switch back to map editor mode and render the frame
	if (App->CL_Editor_Control->flag_Scene_Editor_Active == false)
	{
		//App->CL_Editor_Control->Return_To_Map_Editor();
	}

	App->CL_Properties_Tabs->Select_Templates_Tab();
	App->CL_Editor_Control->Set_Map_Editor_Startup();

	App->CL_Properties_Templates->Enable_Shape_Buttons(true);

	App->CL_Ogre->RenderFrame(7);
}


