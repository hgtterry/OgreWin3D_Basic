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
#include "Shlobj.h"

CL64_App::CL64_App(void)
{
	CL_Libs =					nullptr;
	CL_Editor_Map =				nullptr;
	CL_X_Maths =				nullptr; // Library

	CL_X_Box =					nullptr; // Library
	CL_Render =					nullptr;
	CL_Level =					nullptr;

	CL_X_Preference =			nullptr; // Library
	CL_X_Ini_File =				nullptr; // Library

	CL_Doc =					nullptr;
	CL_Utilities =				nullptr;

	CL_X_Brush =				nullptr; // Library


	CL_X_FaceList =				nullptr; // Library

	CL_X_Face =					nullptr; // Library

	CL_X_BrushTemplate =		nullptr; // Library
	
	CL_X_SelBrushList =			nullptr; // Library
	CL_X_Array =				nullptr; // Library

	CL_X_SelFaceList =			nullptr; // Library

	CL_Mesh_Mgr =				nullptr;
	// ------------------------------
	CL_Dialogs =				nullptr;
	CL_Properties_Tabs =		nullptr;
	CL_Properties_Templates =	nullptr;
	CL_Properties_Brushes =		nullptr;
	CL_Properties_Textures =	nullptr;
	CL_Properties_Motions =		nullptr;

	CL_X_Face_Editor =			nullptr;

	CL_Ogre	=					nullptr;
	CL_Interface =				nullptr;
	CL_Keyboard =				nullptr;
	CL_Scene =					nullptr;
	CL_Ogre3D =					nullptr;
	CL_Brush_X =				nullptr;
	CL_Top_Tabs =				nullptr;
	CL_File =					nullptr;
	CL_ParseFile =				nullptr;
	CL_Export =					nullptr;
	CL_File_IO =				nullptr;
	CL_ImGui =					nullptr;
	CL_ImGui_Dialogs =			nullptr;
	CL_ImGui_Editor =			nullptr;
	CL_TXL_Editor =				nullptr;
	CL_Textures =				nullptr;
	CL_Resources =				nullptr;
	CL_Picking =				nullptr;
	CL_Camera =					nullptr;
	CL_Entities =				nullptr;
	CL_Motions =				nullptr;

	CL_PB =						nullptr;

	CL_Dimensions =				nullptr;
	CL_Props_Dialogs =			nullptr;
	CL_Editor_Control =			nullptr;

	// Templates Dialogs Libraries
	CL_App_Templates =				nullptr;
	CL_X_CreateStaircaseDialog =	nullptr;
	CL_X_Shapes_3D =				nullptr; // Needs Routing to just Shapes stuff

	// Project
	CL_Gizmos =					nullptr;
	
	CL_Sandbox =				nullptr;
	CL_Importers =				nullptr;
	CL_Mesh =					nullptr;
	CL_Properties_Materials =	nullptr;

	CUR = 0;
	Menu_Map = 0;
	
	hInst = nullptr;
	MainHwnd = nullptr;
	
	CursorPosX = 500;
	CursorPosY = 500;

	AppBackground = 0;

	Brush_But_Pressed = 0;
	Brush_But_Normal = 0;
	Brush_But_Hover = 0;
	Brush_Tabs_UnSelected = 0;
	Brush_Tabs = 0;

	GreenPen = 0;
	WhitePen = 0;

	BlackBrush = 0;

	Brush_White = 0;

	Brush_Red = 0;
	Brush_Green = 0;
	Brush_Blue = 0;

	Font_CB10 = 0;
	Font_CB15 = 0;
	Font_CB18 = 0;
	Font_Arial20 = 0;
	Font_Banner = 0;

	Hnd_GridOn_Bmp = NULL;
	Hnd_GridOff_Bmp = NULL;

	Hnd_HairOn_Bmp = NULL;
	Hnd_HairOff_Bmp = NULL;

	Hnd_TexturesOn_Bmp = NULL;
	Hnd_TexturesOff_Bmp = NULL;

	Hnd_NormalsOn_Bmp = NULL;
	Hnd_NormalsOff_Bmp = NULL;

	Hnd_MeshOff_Bmp = NULL;
	Hnd_MeshOn_Bmp = NULL;

	Hnd_MeshPointsOn_Bmp = NULL;
	Hnd_MeshPointsOff_Bmp = NULL;

	Hnd_BonesOn_Bmp = NULL;
	Hnd_BonesOff_Bmp = NULL;

	Hnd_BBOn_Bmp = NULL;
	Hnd_BBOff_Bmp = NULL;

	Hnd_SolidBox_Bmp = NULL;
	Hnd_HollowBox_Bmp = NULL;

	Hnd_Stairs_Bmp = NULL;
	Hnd_Ramp_Bmp = NULL;
	Hnd_TR_Off_Bmp = NULL;
	Hnd_TR_On_Bmp = NULL;
	Hnd_Help_Bmp = NULL;

	Hnd_ModelInfoOn_Bmp = NULL;
	Hnd_ModelInfo_Bmp = NULL;

	Hnd_Box_Icon =	NULL;
	Hnd_Cyl_Icon =	NULL;
	Hnd_Cone_Icon = NULL;
	Hnd_Arch_Icon = NULL;
	Hnd_Spheriod_Icon = NULL;
	Hnd_Stairs_Icon = NULL;
	
	ViewGLhWnd = NULL;
	ListPanel = NULL;

	flag_Debug_App = false;
	flag_3D_Started = false;
	flag_Block_Mouse_Buttons = false;
	flag_Release = false;

	Development = true; // Allow New Code to Run for Testing
	flag_Start_3DEditor_Mode = true;

	ViewPLeaseWait = nullptr;

	RB_Directory_FullPath[0] = 0;
}

CL64_App::~CL64_App(void)
{
}

// *************************************************************************
// *				InitApp:- Terry Mo and Hazel 2025					   *
// *************************************************************************
void CL64_App::InitApp(void)
{
	CL_Libs =					new Init_Libs();
	CL_Editor_Map =				new CL64_Editor_Map();
	CL_X_Maths =				new CL64_Lib_Maths(); // Library
	CL_X_Box =					new CX_Box(); // Library

	CL_Render =					new CL64_Render_Map();
	CL_Level =					new CL64_Level();

	CL_X_Preference =			new Lib_Preference(); // Library
	CL_X_Ini_File =				new Lib_Ini_File(); // Library
	
	CL_Doc =					new CL64_Doc();
	CL_Utilities =				new CL64_Utilities();

	CL_X_Brush =				new CX_Brush(); // Library

	CL_X_FaceList =				new CX_FaceList(); // Library
	CL_X_Face =					new CX_Face(); // Library
	CL_X_BrushTemplate =		new BrushTemplate(); // Library

	CL_X_SelBrushList =			new CX_SelBrushList(); // Library
	CL_X_Array =				new CX_Array(); // Library
	CL_X_SelFaceList =			new CX_SelFaceList(); // Library
	
	CL_Mesh_Mgr =				new CL64_Mesh_Mgr();

	// ------------------------------
	CL_Dialogs =				new CL64_Dialogs();
	CL_Properties_Tabs =		new CL64_Properties_Tabs();
	CL_Properties_Templates =	new CL64_Properties_Templates();
	CL_Properties_Brushes =		new CL64_Properties_Brushes();
	CL_Properties_Textures =	new CL64_Properties_Textures();
	CL_Properties_Motions =		new CL64_Properties_Motions();

	CL_X_Face_Editor =			new Face_Editor();

	CL_Ogre =					new CLP_3D_Setup();
	CL_Interface =				new CL64_Interface();
	CL_Keyboard =				new CL64_Keyboard();
	CL_Scene =					new CL64_Scene();
	CL_Ogre3D =					new CL64_Ogre3D();
	CL_Top_Tabs =				new CL64_Top_Tabs();
	
	CL_Brush_X =				new CL64_Brush_X();
	CL_File =					new CL64_File;
	CL_ParseFile =				new CL64_ParseFile();
	CL_Export =					new	CL64_Export();
	CL_File_IO =				new CL64_File_IO();
	CL_ImGui =					new CL64_ImGui();
	CL_ImGui_Dialogs =			new CL64_ImGui_Dialogs();
	CL_ImGui_Editor =			new CL64_ImGui_Editor();
	CL_TXL_Editor =				new CL64_TXL_Editor();
	CL_Textures =				new CL64_Textures();
	CL_Resources =				new CL64_Resources();
	CL_Picking =				new CL64_Picking();
	CL_Camera =					new CL64_Camera();
	CL_Entities =				new CL64_Entities();
	CL_Motions =				new CL64_Motioins();

	CL_PB =						new CL64_PB();

	CL_Props_Dialogs =			new CL64_Props_Dialogs();

	CL_Editor_Control =			new CL64_Editor_Control();
	
	// Templates Dialogs Libraries
	CL_App_Templates =				new App_Templates();
	CL_App_Templates->Init_Templates();

	CL_X_CreateStaircaseDialog =	new CreateStaircaseDialog();
	CL_X_Shapes_3D =				new Shapes_3D();

	// Project
	CL_Gizmos =					new CL64_Gizmos();
	CL_Dimensions =				new CL64_Dimensions();

	CL_Sandbox =				new Sandbox();
	CL_Importers =				new CL64_Importers();
	CL_Model =					new CL64_Model();
	CL_Mesh =					new CL64_Mesh();
	CL_Properties_Materials =	new CL64_Properties_Materials;

	Set_Brushes_Fonts();

	char path[MAX_PATH];

	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))//KF_FLAG_CREATE
	{
		strcpy(App->CL_X_Preference->UserData_Folder, path);
	}
	else
	{
		App->Say("Can not access user folder");
	}

	char Deskdir[MAX_PATH];
	wchar_t* d_path = new wchar_t[128];
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &d_path)))//KF_FLAG_CREATE
	{
		wsprintf(Deskdir, "%ls", d_path);
		CoTaskMemFree(d_path);
		strcpy(App->CL_Export->DeskTop_Folder, Deskdir);
	}
	else
	{
		App->Say("Can not access user folder");
	}

	LoadString(hInst, IDS_APP_TITLE, App_Title, MAX_PATH);
}

// *************************************************************************
// *				Init_Dialogs:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_App::Init_Dialogs(void)
{
	CL_Interface->Position_Properties_Dlg();
	CL_Props_Dialogs->Start_Props_Dialogs();
}

// *************************************************************************
// *				Set_Brushes_Fonts:- Terry Mo and Hazel 2025			   *
// *************************************************************************
void CL64_App::Set_Brushes_Fonts(void)
{
	// Initialize brushes
	AppBackground = CreateSolidBrush(RGB(213, 222, 242));
	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	Brush_White = CreateSolidBrush(RGB(255, 255, 255));

	GreenPen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 255, 0));
	WhitePen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 255, 255));

	Brush_Red = CreateSolidBrush(RGB(255, 0, 0));
	Brush_Green = CreateSolidBrush(RGB(0, 255, 0));
	Brush_Blue = CreateSolidBrush(RGB(0, 0, 255));

	Brush_Tabs = CreateSolidBrush(RGB(255, 255, 255));
	Brush_Tabs_UnSelected = CreateSolidBrush(RGB(240, 240, 240));
	Brush_But_Normal = CreateSolidBrush(RGB(255, 255, 180));
	Brush_But_Hover = CreateSolidBrush(RGB(255, 255, 230));
	Brush_But_Pressed = CreateSolidBrush(RGB(240, 240, 190));

	// Initialize fonts
	Font_CB10 = CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	Font_CB15 = CreateFont(-15, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	Font_CB18 = CreateFont(-18, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	Font_Arial20 = CreateFont(-20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Arial");
	Font_Banner = CreateFont(-30, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Arial Black");

}

// *************************************************************************
// *			LoadProgramResource:- Terry Mo and Hazel 2025		  	   *
// *************************************************************************
void CL64_App::LoadProgramResource(void)
{
	Hnd_HollowBox_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HOLLOWBOX);
	Hnd_SolidBox_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_SOLIDBOX);

	Hnd_Stairs_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_CREATESTAIRS);
	Hnd_Ramp_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_CREATERAMP);

	// -----------------------------------

	Hnd_HairOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HAIRON);
	Hnd_HairOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HAIROFF);

	Hnd_GridOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_GRIDON);
	Hnd_GridOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_GRIDOFF);

	Hnd_TexturesOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TEXTURESON);
	Hnd_TexturesOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TEXTURESOFF);

	Hnd_NormalsOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_NORMALSON);
	Hnd_NormalsOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_NORMALSOFF);

	Hnd_MeshOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHON);
	Hnd_MeshOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHOFF);

	Hnd_BonesOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONESON);
	Hnd_BonesOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONESOFF);

	Hnd_MeshPointsOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHPOINTSON);
	Hnd_MeshPointsOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHPOINTSOFF);

	Hnd_BBOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONDINGBOXON);
	Hnd_BBOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONDINGBOXOFF);

	Hnd_ModelInfo_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TB_MODELDATAOFF);
	Hnd_ModelInfoOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TB_MODELDATAON);

	// -----------------------------------

	/*Hnd_HollowSpheroid_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HOLLOWSPHERE);
	Hnd_SolidSpheroid_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_SOLIDSPHERE);*/

	Hnd_Arch_Icon = LoadIcon(hInst, (LPCTSTR)IDI_ARCH_PRIMITIVE);
	Hnd_Cone_Icon = LoadIcon(hInst, (LPCTSTR)IDI_CONE_PRIMITIVE);
	Hnd_Cyl_Icon = LoadIcon(hInst, (LPCTSTR)IDI_CYLINDER_PRIMITIVE);
	Hnd_Spheriod_Icon = LoadIcon(hInst, (LPCTSTR)IDI_SPHEROID_PRIMITIVE);
	Hnd_Box_Icon = LoadIcon(hInst, (LPCTSTR)IDI_CUBE_PRIMITIVE);
	Hnd_Stairs_Icon = LoadIcon(hInst, (LPCTSTR)IDI_STAIRCASE_PRIMITIVE);

	Hnd_TR_Off_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TR_OFF);
	Hnd_TR_On_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TR_ON);

	Hnd_Help_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HELP);
	
	// Set Defaults for now
	App->CL_Level->flag_UseGrid = 1;
	CheckMenuItem(App->Menu_Map, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(App->Menu_Map, ID_CAMERA_TEXTURED, MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(App->Menu_Map, ID_SPEED_NORMAL, MF_BYCOMMAND | MF_CHECKED);

	EnableMenuItem(App->Menu_Map, ID_EDIT_DELETE, MF_DISABLED | MF_GRAYED);
}

// *************************************************************************
// *		Custom_Button_Globals:- Terry and Hazel Flanigan 2024  	  	   *
// *************************************************************************
void CL64_App::Custom_Button_Globals(LPNMCUSTOMDRAW item) const
{
	if (item->uItemState & CDIS_SELECTED) // Push Down
	{
		SelectObject(item->hdc, GreenPen);
		SelectObject(item->hdc, App->Brush_But_Pressed);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		SelectObject(item->hdc, GreenPen);
		SelectObject(item->hdc, App->Brush_But_Pressed);

		return;
	}
	else
	{
		if (item->uItemState & CDIS_HOT) //Our mouse is over the button
		{
			SelectObject(item->hdc, GreenPen);
			SelectObject(item->hdc, Brush_But_Hover);

			RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

			SelectObject(item->hdc, GreenPen);
			SelectObject(item->hdc, Brush_But_Hover);

			return;
		}

		SelectObject(item->hdc, WhitePen);
		SelectObject(item->hdc, Brush_But_Normal);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		SelectObject(item->hdc, WhitePen);
		SelectObject(item->hdc, Brush_But_Normal);

		return;
	}
}

// *************************************************************************
// *			Custom_Button_Greyed:- Terry Mo and Hazel 2025   	  	   *
// *************************************************************************
bool CL64_App::Custom_Button_Greyed(LPNMCUSTOMDRAW item)
{
	HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 255)); // Idle 

	HGDIOBJ old_pen = SelectObject(item->hdc, pen);
	HGDIOBJ old_brush = SelectObject(item->hdc, Brush_Tabs_UnSelected);

	RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

	SelectObject(item->hdc, old_pen);
	SelectObject(item->hdc, old_brush);
	DeleteObject(pen);

	return CDRF_DODEFAULT;
}

// *************************************************************************
// *			Custom_Button_Normal:- Terry Mo and Hazel 2025   	  	   *
// *************************************************************************
bool CL64_App::Custom_Button_Normal(LPNMCUSTOMDRAW item)
{
	// Determine the pen color and brush based on the button state
	COLORREF penColor = RGB(0, 0, 0); // Default to black for idle state
	HGDIOBJ old_pen, old_brush;

	// Set pen color and brush based on the button state
	if (item->uItemState & CDIS_SELECTED) // Button is pressed
	{
		penColor = RGB(0, 0, 0); // Black for pressed state
		old_brush = App->Brush_But_Pressed;
	}
	else if (item->uItemState & CDIS_HOT) // Mouse is over the button
	{
		penColor = RGB(0, 255, 0); // Green for hover state
		old_brush = App->Brush_But_Hover;
	}
	else // Idle state
	{
		old_brush = App->Brush_But_Normal;
	}

	// Create pen for button border
	HPEN pen = CreatePen(PS_INSIDEFRAME, 0, penColor);
	old_pen = SelectObject(item->hdc, pen);
	old_brush = SelectObject(item->hdc, old_brush);

	// Draw the rounded rectangle
	RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

	// Clean up
	SelectObject(item->hdc, old_pen);
	SelectObject(item->hdc, old_brush);
	DeleteObject(pen);

	return CDRF_DODEFAULT;

	return CDRF_DODEFAULT;
}

// *************************************************************************
// *				Custom_Button:- Terry Mo and Hazel 2025   		  	   *
// *************************************************************************
bool CL64_App::Custom_Button_Toggle(LPNMCUSTOMDRAW item, bool Toggle)
{
	static HBRUSH defaultbrush = NULL;
	static HBRUSH hotbrush = NULL;
	static HBRUSH selectbrush = NULL;

	{
		if (item->uItemState & CDIS_HOT) //Our mouse is over the button
		{
			//Select our colour when the mouse hovers our button

			if (Toggle == 1)
			{
				hotbrush = CreateSolidBrush(RGB(0, 255, 0));
			}
			else
			{
				hotbrush = CreateSolidBrush(RGB(240, 240, 240));
			}

			HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

			HGDIOBJ old_pen = SelectObject(item->hdc, pen);
			HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

			RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

			SelectObject(item->hdc, old_pen);
			SelectObject(item->hdc, old_brush);
			DeleteObject(pen);

			return CDRF_DODEFAULT;
		}

		//Select our colour when our button is doing nothing

		if (Toggle == 1)
		{
			defaultbrush = App->Brush_Green;
		}
		else
		{
			defaultbrush = Brush_But_Normal;
		}

		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return CDRF_DODEFAULT;
	}

	return CDRF_DODEFAULT;
}

// *************************************************************************
// *		Custom_Button_Toggle_Tabs:- Terry Mo and Hazel 2025			   *
// *************************************************************************
bool CL64_App::Custom_Button_Toggle_Tabs(LPNMCUSTOMDRAW item, bool Toggle)
{
	static HBRUSH defaultbrush = NULL;
	static HBRUSH hotbrush = NULL;
	static HBRUSH selectbrush = NULL;

	{
		if (item->uItemState & CDIS_HOT) //Our mouse is over the button
		{
			//Select our colour when the mouse hovers our button

			if (Toggle == 1)
			{
				hotbrush = hotbrush = CreateSolidBrush(RGB(0, 255, 0));
			}
			else
			{
				//hotbrush = Brush_Tabs_UnSelected; // Unselected 
				hotbrush = CreateSolidBrush(RGB(240, 240, 240));
			}

			HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

			HGDIOBJ old_pen = SelectObject(item->hdc, pen);
			HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

			RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

			SelectObject(item->hdc, old_pen);
			SelectObject(item->hdc, old_brush);
			DeleteObject(pen);

			return CDRF_DODEFAULT;
		}

		//Select our colour when our button is doing nothing

		if (Toggle == 1)
		{
			defaultbrush = CreateSolidBrush(RGB(154, 255, 154));
		}
		else
		{
			defaultbrush = Brush_But_Normal; // Unselected 
		}

		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return CDRF_DODEFAULT;
	}

	return CDRF_DODEFAULT;
}

// *************************************************************************
// *				Flash_Window:- Terry Mo and Hazel 2024				   *
// *************************************************************************
void CL64_App::Flash_Window()
{
	FlashWindow(App->MainHwnd, true);
}

// *************************************************************************
// *				SetMainWinCentre:- Terry Mo and Hazel 2024			   *
// *************************************************************************
void CL64_App::SetMainWinCentre(void) const
{
	int ClientWidth_X = GetSystemMetrics(SM_CXSCREEN);
	int ClientHeight_Y = GetSystemMetrics(SM_CYSCREEN);

	RECT Control;
	GetClientRect(MainHwnd, &Control);

	int AllX = (ClientWidth_X / 2) - (Control.right / 2) - 10;
	int AllY = (ClientHeight_Y / 2) - (Control.bottom / 2) - 30;

	SetWindowPos(MainHwnd, NULL, AllX, AllY, 0, 0, SWP_NOSIZE);
}

// *************************************************************************
// *				Say:- Terry Mo and Hazel 2024						   *
// *************************************************************************
void CL64_App::Say(const char* Message, const char* Message2)
{
	// Prepare Message
	std::string text(Message);
	std::string text2 = (Message2 != nullptr) ? Message2 : " ";

	// Call the Message function
	App->CL_Dialogs->Message((LPSTR)text.c_str(), (LPSTR)text2.c_str());
}

// *************************************************************************
// *				Report_Error:- Terry Mo and Hazel 2025	   	  		   *
// *************************************************************************
void CL64_App::Report_Error(const char* Msg, ...)
{
	char Buffer[MAX_PATH];
	va_list argptr;

	va_start(argptr, Msg);
	vsprintf(Buffer, Msg, argptr);
	va_end(argptr);

	App->Say("Error", Buffer);
}

// *************************************************************************
// *				Say_Int:- Terry Mo and Hazel 2025					   *
// *************************************************************************
void CL64_App::Say_Int(int Value)
{
	char buf[MAX_PATH];
	_itoa(Value, buf, 10);
	MessageBox(MainHwnd, buf, "Display Int", MB_OK);
}

// *************************************************************************
// *			Say_Float:- Terry Mo and Hazel 2024				   *
// *************************************************************************
void CL64_App::Say_Float(float Value)
{
	char buf[MAX_PATH];
	sprintf(buf, "%f", Value);
	MessageBox(MainHwnd, buf, "Display float", MB_OK);
}

// *************************************************************************
// *			Say_Vector3:- Terry Mo and Hazel 2024					   *
// *************************************************************************
void CL64_App::Say_Vector3(Ogre::Vector3 Vector)
{
	char buf[255];
	sprintf(buf, "%f %f %f", Vector.x, Vector.y, Vector.z);
	MessageBox(MainHwnd, buf, "Display Vector", MB_OK);
}

// *************************************************************************
// *				Say_Win:- Terry Mo and Hazel 2024					   *
// *************************************************************************
void CL64_App::Say_Win(const char* Message)
{
	char text[MAX_PATH];
	strcpy(text, Message);
	MessageBox(App->MainHwnd, Message, "Message", MB_OK);
}

// *************************************************************************
// *				Set_Title:- Terry Mo and Hazel 2024					   *
// *************************************************************************
void CL64_App::Set_Title(const char* Title)
{
	char TitleBar[MAX_PATH];
	strcpy(TitleBar, App_Title);
	strcat(TitleBar, "    ");
	strcat(TitleBar, Title);
	SetWindowText(App->MainHwnd, TitleBar);
}

// *************************************************************************
// *				Beep:- Terry Mo and Hazel 2024						   *
// *************************************************************************
void CL64_App::BeepBeep()
{
	Beep(400, 200);
}

// *************************************************************************
// *				Open_HTML:- Terry Mo and Hazel 2025					   *
// *************************************************************************
void CL64_App::Open_HTML(char* HelpTitle)
{
	char Path[MAX_PATH];
	strcpy(Path, App->RB_Directory_FullPath);
	strcat(Path, "\\");
	strcat(Path, HelpTitle);

	ShellExecute(0, "open", Path, 0, 0, SW_SHOW);
}

