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
#include "Shlobj.h"

CL64_App::CL64_App(void)
{
	CL_MapEditor =				nullptr;
	CL_Maths =					nullptr;
	CL_Box =					nullptr;
	CL_Render =					nullptr;
	CL_Level =					nullptr;
	CL_Prefs =					nullptr;
	CL_Ini_File =				nullptr;
	CL_Doc =					nullptr;
	CL_Utilities =				nullptr;
	CL_Brush =					nullptr;
	CL_FaceList =				nullptr;
	CL_Face =					nullptr;
	CL_BrushTemplate =			nullptr;
	CL_SelBrushList =			nullptr;
	CL_Array =					nullptr;
	CL_SelFaceList =			nullptr;
	CL_Mesh_Mgr =				nullptr;
	// ------------------------------
	CL_Dialogs =				nullptr;
	CL_Properties_Tabs =		nullptr;
	CL_Properties_Templates =	nullptr;
	CL_Properties_Brushes =		nullptr;
	CL_Properties_Textures =	nullptr;
	CL_Ogre	=					nullptr;
	CL_Grid =					nullptr;
	CL_Panels =					nullptr;
	CL_Keyboard =				nullptr;
	CL_Model =					nullptr;
	CL_Ogre3D =					nullptr;
	CL_Brush_X =				nullptr;
	CL_Top_Tabs =				nullptr;
	CL_File =					nullptr;
	CL_ParseFile =				nullptr;
	CL_Export =					nullptr;
	CL_File_IO =				nullptr;
	CL_ImGui =					nullptr;
	CL_TXL_Editor =				nullptr;

	// Templates Dialogs
	CL_CreateBoxDialog =		nullptr;
	CL_CreateCylDialog =		nullptr;
	CL_CreateConeDialog =		nullptr;

	CUR = 0;
	mMenu = 0;

	hInst = nullptr;
	MainHwnd = nullptr;
	
	CursorPosX = 500;
	CursorPosY = 500;

	AppBackground = 0;

	Font_CB15 = 0;
	Font_CB18 = 0;
	Font_Arial20 = 0;
	Font_Banner = 0;

	Hnd_Box_Icon =	NULL;
	Hnd_Cyl_Icon =	NULL;
	Hnd_Cone_Icon = NULL;
	Hnd_Arch_Icon = NULL;
	Hnd_Spheriod_Icon = NULL;
	Hnd_Stairs_Icon = NULL;

	flag_Debug_App = 0;
	flag_OgreStarted = 0;

	ViewPLeaseWait = nullptr;

	RB_Directory_FullPath[0] = 0;
}

CL64_App::~CL64_App(void)
{
}

// *************************************************************************
// *			InitApp:- Terry and Hazel Flanigan 2025					   *
// *************************************************************************
void CL64_App::InitApp(void)
{
	CL_MapEditor =				new CL64_MapEditor();
	CL_Maths =					new CL64_Maths();
	CL_Box =					new CL64_Box();
	CL_Render =					new CL64_Render();
	CL_Level =					new CL64_Level();
	CL_Prefs =					new CL64_Prefs();
	CL_Ini_File =				new CL64_Ini_File();
	CL_Doc =					new CL64_Doc();
	CL_Utilities =				new CL64_Utilities();
	CL_Brush =					new CL64_Brush();
	CL_FaceList =				new CL64_FaceList();
	CL_Face =					new CL64_Face();
	CL_BrushTemplate =			new CL64_BrushTemplate();
	CL_SelBrushList =			new CL64_SelBrushList();
	CL_Array =					new CL64_Array();
	CL_SelFaceList =			new CL64_SelFaceList();
	CL_Mesh_Mgr =				new CL64_Mesh_Mgr();

	// ------------------------------
	CL_Dialogs =				new CL64_Dialogs();
	CL_Properties_Tabs =		new CL64_Properties_Tabs();
	CL_Properties_Templates =	new CL64_Properties_Templates();
	CL_Properties_Brushes =		new CL64_Properties_Brushes();
	CL_Properties_Textures =	new CL64_Properties_Textures();
	CL_Ogre =					new CL64_Ogre();
	CL_Grid =					new CL64_Grid();
	CL_Panels =					new CL64_Panels();
	CL_Keyboard =				new CL64_Keyboard();
	CL_Model =					new CL64_Model();
	CL_Ogre3D =					new CL64_Ogre3D();
	CL_Top_Tabs =				new CL64_Top_Tabs();
	
	CL_Brush_X =				new CL64_Brush_X();
	CL_File =					new CL64_File;
	CL_ParseFile =				new CL64_ParseFile();
	CL_Export =					new	CL64_Export();
	CL_File_IO =				new CL64_File_IO();
	CL_ImGui =					new CL64_ImGui();
	CL_TXL_Editor =				new CL64_TXL_Editor();
	
	// Templates Dialogs
	CL_CreateBoxDialog =		new A_CreateBoxDialog();
	CL_CreateCylDialog =		new A_CreateCylDialog();
	CL_CreateConeDialog =		new A_CreateConeDialog();

	SetBrushes_Fonts();

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
// *			SetBrushes_Fonts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_App::SetBrushes_Fonts(void)
{
	AppBackground = CreateSolidBrush(RGB(213, 222, 242));

	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	Brush_White = CreateSolidBrush(RGB(255, 255, 255));
	Brush_Green = CreateSolidBrush(RGB(0, 255, 0));

	Brush_Tabs = CreateSolidBrush(RGB(255, 255, 255));
	Brush_Tabs_UnSelected = CreateSolidBrush(RGB(240, 240, 240));

	Brush_But_Normal = CreateSolidBrush(RGB(255, 255, 180));
	Brush_But_Hover = CreateSolidBrush(RGB(255, 255, 230));
	Brush_But_Pressed = CreateSolidBrush(RGB(240, 240, 190));

	Font_CB15 = CreateFont(-15, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	Font_CB18 = CreateFont(-18, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	Font_Arial20 = CreateFont(-20, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Arial");
	Font_Banner = CreateFont(-30, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Aerial Black");

	//Hnd_Box_Icon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_CUBE_PRIMITIVE));

	//Hnd_Box_Icon = LoadImage(hInst, MAKEINTRESOURCE(IDI_CUBE_PRIMITIVE), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE);
}

// *************************************************************************
// *		LoadProgramResource:- Terry and Hazel Flanigan 2025		  	   *
// *************************************************************************
void CL64_App::LoadProgramResource(void)
{
	/*Hnd_HollowBox_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HOLLOWBOX);
	Hnd_SolidBox_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_SOLIDBOX);

	Hnd_HollowSpheroid_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HOLLOWSPHERE);
	Hnd_SolidSpheroid_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_SOLIDSPHERE);*/

	Hnd_Arch_Icon = LoadIcon(hInst, (LPCTSTR)IDI_ARCH_PRIMITIVE);
	Hnd_Cone_Icon = LoadIcon(hInst, (LPCTSTR)IDI_CONE_PRIMITIVE);
	Hnd_Cyl_Icon = LoadIcon(hInst, (LPCTSTR)IDI_CYLINDER_PRIMITIVE);
	Hnd_Spheriod_Icon = LoadIcon(hInst, (LPCTSTR)IDI_SPHEROID_PRIMITIVE);
	Hnd_Box_Icon = LoadIcon(hInst, (LPCTSTR)IDI_CUBE_PRIMITIVE);
	Hnd_Stairs_Icon = LoadIcon(hInst, (LPCTSTR)IDI_STAIRCASE_PRIMITIVE);

	/*Hnd_MeshOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHON);
	Hnd_MeshOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHOFF);

	Hnd_MeshPointsOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHPOINTSON);
	Hnd_MeshPointsOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHPOINTSOFF);

	Hnd_BBOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONDINGBOXON);
	Hnd_BBOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONDINGBOXOFF);

	Hnd_BonesOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONESON);
	Hnd_BonesOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONESOFF);

	Hnd_GridOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_GRIDON);
	Hnd_GridOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_GRIDOFF);

	Hnd_HairOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HAIRON);
	Hnd_HairOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HAIROFF);

	Hnd_TexturesOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TEXTURESON);
	Hnd_TexturesOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TEXTURESOFF);

	Hnd_NormalsOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_NORMALSON);
	Hnd_NormalsOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_NORMALSOFF);

	Hnd_LightsOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_LIGHTON);
	Hnd_LightsOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_LIGHTOFF);

	Hnd_ModelInfo_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TB_MODELDATA);
	Hnd_ModelInfoOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TB_MODELDATAON);

	Hnd_FullScreen_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_FULLSCREEN);*/

}

// *************************************************************************
// *					Custom_Button_Greyed Terry Bernie   		  	   *
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
// *					Custom_Button_Normal Terry Bernie   		  	   *
// *************************************************************************
bool CL64_App::Custom_Button_Normal(LPNMCUSTOMDRAW item)
{

	if (item->uItemState & CDIS_SELECTED) // Push Down
	{
		//Create pen for button border
		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

		//Select our brush into hDC
		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, App->Brush_But_Pressed);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		//Clean up
		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return CDRF_DODEFAULT;
	}
	else
	{
		if (item->uItemState & CDIS_HOT) //Our mouse is over the button
		{

			HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 255, 0));

			HGDIOBJ old_pen = SelectObject(item->hdc, pen);
			HGDIOBJ old_brush = SelectObject(item->hdc, App->Brush_But_Hover);

			RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

			SelectObject(item->hdc, old_pen);
			SelectObject(item->hdc, old_brush);
			DeleteObject(pen);

			return CDRF_DODEFAULT;
		}

		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0)); // Idle 

		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, App->Brush_But_Normal);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return CDRF_DODEFAULT;
	}

	return CDRF_DODEFAULT;
}

// *************************************************************************
// *					Custom_Button Terry Bernie   			 	 	   *
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
// *		Custom_Button_Toggle_Tabs:- Terry and Hazel Flanigan 2025 	   *
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
// *			Flash_Window:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_App::Flash_Window()
{
	FlashWindow(App->MainHwnd, true);
}

// *************************************************************************
// *			SetMainWinCentre:- Terry and Hazel Flanigan 2024		   *
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
// *				Say:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void CL64_App::Say(const char* Message, char* Message2)
{
	char text[MAX_PATH];
	char text2[MAX_PATH];

	strcpy(text, Message);
	if (Message2 == NULL)
	{
		strcpy(text2, " ");
	}
	else
	{
		strcpy(text2, Message2);
	}

	App->CL_Dialogs->Message(text, text2);
}

// *************************************************************************
// *						Report_Error:- Terry and Hazel Flanigan 2025	   					  		   *
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
// *				Say_Int:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_App::Say_Int(int Value)
{
	char buf[MAX_PATH];
	_itoa(Value, buf, 10);
	MessageBox(MainHwnd, buf, "Display Int", MB_OK);
}

// *************************************************************************
// *			Say_Float:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_App::Say_Float(float Value)
{
	char buf[MAX_PATH];
	sprintf(buf, "%f", Value);
	MessageBox(MainHwnd, buf, "Display float", MB_OK);
}

// *************************************************************************
// *			Say_Vector3:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_App::Say_Vector3(Ogre::Vector3 Vector)
{
	char buf[255];
	sprintf(buf, "%f %f %f", Vector.x, Vector.y, Vector.z);
	MessageBox(MainHwnd, buf, "Display Vector", MB_OK);
}

// *************************************************************************
// *				Say_Win:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_App::Say_Win(const char* Message)
{
	char text[MAX_PATH];
	strcpy(text, Message);
	MessageBox(App->MainHwnd, Message, "Message", MB_OK);
}

// *************************************************************************
// *				Set_Title:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_App::Set_Title(const char* Title)
{
	char TitleBar[MAX_PATH];
	strcpy(TitleBar, App_Title);
	strcat(TitleBar, "    ");
	strcat(TitleBar, Title);
	SetWindowText(App->MainHwnd, TitleBar);
}
