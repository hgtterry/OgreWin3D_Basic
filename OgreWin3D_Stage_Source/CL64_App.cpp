/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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

CL64_App::CL64_App(void)
{
	CL_Ogre =			nullptr;
	CL_Grid =			nullptr;
	CL_Keyboard =		nullptr;
	CL_Camera =			nullptr;
	CL_Ini_File =		nullptr;
	CL_Preferences =	nullptr;
	CL_Assimp =			nullptr;
	CL_Scene =			nullptr;
	CL_Importers =		nullptr;
	CL_File_IO =		nullptr;
	CL_ImGui =			nullptr;
	CL_Textures =		nullptr;
	CL_TopDlg =			nullptr;
	CL_Panels =			nullptr;
	CL_Converters =		nullptr;
	CL_Bullet =			nullptr;
	CL_Player =			nullptr;
	CL_Dialogs =		nullptr;
	CL_Resources =		nullptr;

	CL_Motions =		nullptr;
	CL_Dimensions =		nullptr;
	CL_Props_Textures = nullptr;
	CL_Mesh_Manager =	nullptr;
	CL_Level =			nullptr;

	CL_FileView =		nullptr;
	CL_Properties =		nullptr;
	CL_Project =		nullptr;
	CL_Com_Area =		nullptr;
	CL_Objects_Create = nullptr;
	CL_Object =			nullptr;
	//-------------------------------------

	hInst =				nullptr;
	MainHwnd =			nullptr;
	Fdlg =				nullptr;
	ViewGLhWnd =		nullptr;
	ViewPLeaseWait =	nullptr;
	ListPanel =			nullptr;

	GD_Directory_FullPath[0] = 0;
	App_Version[0] = 0;

	CUR = 0;
	mMenu = 0;

	CursorPosX = 500;
	CursorPosY = 500;

	AppBackground = 0;

	Font_CB15 = 0;
	Font_CB18 = 0;
	Font_Arial20 = 0;

	Hnd_TexturesOn_Bmp = NULL;
	Hnd_TexturesOff_Bmp = NULL;
	Hnd_MeshOff_Bmp = NULL;
	Hnd_MeshOn_Bmp = NULL;
	Hnd_HairOn_Bmp = NULL;
	Hnd_MeshPointsOn_Bmp = NULL;
	Hnd_MeshPointsOff_Bmp = NULL;
	Hnd_HairOn_Bmp = NULL;
	Hnd_HairOff_Bmp = NULL;
	Hnd_BonesOn_Bmp = NULL;
	Hnd_BonesOff_Bmp = NULL;
	Hnd_ModelInfo_Bmp = NULL;
	Hnd_ModelInfoOn_Bmp = NULL;
	Hnd_GridOn_Bmp = NULL;
	Hnd_GridOff_Bmp = NULL;
	Hnd_NormalsOn_Bmp = NULL;
	Hnd_NormalsOff_Bmp = NULL;
	Hnd_BBOn_Bmp = NULL;
	Hnd_BBOff_Bmp = NULL;

	BlackPen = 0;
	GreenPen = 0;
	WhitePen = 0;

	BlackBrush = 0;
	Brush_White = 0;
	Brush_But_Normal = 0;
	Brush_But_Normal = 0;
	Brush_But_Hover = 0;
	Brush_But_Pressed = 0;
	Brush_Green = 0;
	Brush_Tabs = 0;
	Brush_Tabs_UnSelected = 0;
	HotBrush_1 = 0;
	HotBrush_2 = 0;

	flag_OgreStarted = 0;

	flag_Debug_App = 1;
	flag_Debug_Textures = 0;

	flag_Block_Mouse_Buttons = 0;
}

CL64_App::~CL64_App(void)
{
	//Say("Destroyed");
}

// *************************************************************************
// *			InitApp:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void CL64_App::InitApp(void)
{
	
	CL_Ogre =			new CL64_Ogre();
	CL_Grid =			new CL64_Grid();
	CL_Keyboard =		new CL64_Keyboard();
	CL_Camera =			new	CL64_Camera();
	CL_Ini_File =		new CL64_Ini_File();
	CL_Preferences =	new CL64_Preferences();
	CL_Assimp =			new CL64_Assimp();
	CL_Scene =			new CL64_Scene();
	CL_Importers =		new CL64_Importers();
	CL_File_IO =		new CL64_File_IO();
	CL_ImGui =			new CL64_ImGui();
	CL_Textures =		new CL64_Textures();
	CL_TopDlg =			new CL64_TopDlg();
	CL_Panels =			new CL64_Panels();
	CL_Converters =		new CL64_Converters();
	CL_Bullet =			new CL64_Bullet();
	CL_Player =			new CL64_Player();
	CL_Dialogs =		new CL64_Dialogs();
	CL_Resources =		new CL64_Resources();

	CL_Motions =		new CL64_Motioins();
	
	CL_Dimensions =		new CL64_Dimensions();

	CL_Props_Textures = new CL64_Props_Textures();
	CL_Mesh_Manager =	new CL64_Mesh_Manager();
	CL_Level =			new CL64_Level();

	CL_FileView =		new CL64_FileView();
	CL_Properties =		new CL64_Properties();
	CL_Project =		new CL64_Project();
	CL_Com_Area =		new CL64_Com_Area();
	CL_Objects_Create = new CL64_Objects_Create();
	CL_Object =			new CL64_Object();

	SetBrushes_Fonts();

	LoadString(App->hInst, IDS_VERSION, App_Version, MAX_PATH);

	App->CL_Preferences->Read_Preferences();
}

// *************************************************************************
// *			Init_Dialogs:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_App::Init_Dialogs(void)
{
	App->LoadProgramResource();

	App->SetMainWinCentre();

	App->CL_TopDlg->Start_TopBar();
	App->CL_Panels->Resize_TopDlg();

	App->CL_Props_Textures->Start_Props_Textures_Dialog();
	App->CL_Panels->Move_Panels();

	App->CL_FileView->Start_FileView();
	App->CL_FileView->Init_FileView();
	App->CL_Panels->Move_FileView_Window();
	App->CL_Panels->Resize_FileView();

	App->CL_Properties->Start_Properties();
	App->CL_Panels->Place_Properties_Dlg();

	EnableMenuItem(App->mMenu, ID_OGRE3D_RELOADMESH, MF_BYCOMMAND | MF_GRAYED);
	EnableMenuItem(App->mMenu, ID_OGRE3D_RELOADRESOURCEFILE, MF_BYCOMMAND | MF_GRAYED);
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
void CL64_App::Set_Title()
{
	char TitleBar[MAX_PATH];
	strcpy(TitleBar, "OgreWin3D_Stage");
	strcat(TitleBar, "    ");
	strcat(TitleBar, App->CL_File_IO->Project_Path_File_Name);
	SetWindowText(App->MainHwnd, TitleBar);
}

// *************************************************************************
// *			Debug_Text:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_App::Debug_Text()
{
	char text[MAX_PATH];
	char text2[MAX_PATH];
	strcpy(text, "Here Debug");
	strcpy(text2, " ");
	App->CL_Dialogs->Message(text,text2);
}

// *************************************************************************
// *				Say_Int:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_App::Say_Int(int Value)
{
	char buf[MAX_PATH];
	_itoa(Value, buf, 10);
	MessageBox(MainHwnd, buf, "Equity Notice", MB_OK);
}

// *************************************************************************
// *			Say_Float:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_App::Say_Float(float Value)
{
	char buf[MAX_PATH];
	sprintf(buf, "%f", Value);
	MessageBox(MainHwnd, buf, "Equity Notice", MB_OK);
}

// *************************************************************************
// *			SetMainWinCentre:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_App::SetMainWinCentre(void) const
{
	int ClientWidth_X = GetSystemMetrics(SM_CXSCREEN);
	int ClientHeight_Y = GetSystemMetrics(SM_CYSCREEN);

	RECT Control;
	GetClientRect(MainHwnd, &Control);

	int AllX = (ClientWidth_X / 2) - (Control.right / 2) - 10;
	int AllY = (ClientHeight_Y / 2) - (Control.bottom / 2) - 30;

	SetWindowPos(MainHwnd, NULL, AllX, AllY, 0, 0, SWP_NOSIZE);

	return 1;
}

// *************************************************************************
// *			Flash_Window:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_App::Flash_Window()
{
	FlashWindow(App->MainHwnd, true);
}

// *************************************************************************
// *		LoadProgramResource:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_App::LoadProgramResource(void)
{
	
	Hnd_GridOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_GRIDON);
	Hnd_GridOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_GRIDOFF);

	Hnd_HairOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HAIRON);
	Hnd_HairOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HAIROFF);

	Hnd_TexturesOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TEXTURESON);
	Hnd_TexturesOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TEXTURESOFF);

	Hnd_MeshOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHON);
	Hnd_MeshOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHOFF);

	Hnd_MeshPointsOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHPOINTSON);
	Hnd_MeshPointsOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_MESHPOINTSOFF);

	Hnd_BonesOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONESON);
	Hnd_BonesOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONESOFF);

	Hnd_ModelInfo_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TB_MODELDATAOFF);
	Hnd_ModelInfoOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TB_MODELDATAON);

	Hnd_NormalsOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_NORMALSON);
	Hnd_NormalsOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_NORMALSOFF);

	Hnd_BBOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONDINGBOXON);
	Hnd_BBOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_BONDINGBOXOFF);
}

// *************************************************************************
// *			SetBrushes_Fonts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_App::SetBrushes_Fonts(void)
{
	AppBackground = CreateSolidBrush(RGB(213, 222, 242));
	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	Brush_White = CreateSolidBrush(RGB(255, 255, 255));
	Brush_But_Normal = CreateSolidBrush(RGB(255, 255, 180));
	
	Brush_Green = CreateSolidBrush(RGB(0, 255, 0));
	
	Brush_But_Normal = CreateSolidBrush(RGB(255, 255, 150));
	Brush_But_Hover = CreateSolidBrush(RGB(255, 255, 200));
	Brush_But_Pressed = CreateSolidBrush(RGB(240, 240, 190));
	Brush_Tabs = CreateSolidBrush(RGB(255, 255, 255));
	Brush_Tabs_UnSelected = CreateSolidBrush(RGB(190, 190, 190));

	BlackPen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));
	GreenPen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 255, 0));
	WhitePen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 255, 255));

	HotBrush_1 = CreateSolidBrush(RGB(0, 240, 0));
	HotBrush_2 = CreateSolidBrush(RGB(240, 240, 240));

	Font_CB15 = CreateFont(-15, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	Font_CB18 = CreateFont(-18, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	Font_Arial20 = CreateFont(-20, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Arial");

}

// *************************************************************************
// *		Custom_Button_Toggle:- Terry and Hazel Flanigan 2024   	 	   *
// *************************************************************************
void CL64_App::Custom_Button_Toggle(LPNMCUSTOMDRAW item, bool Toggle) const
{
	static HBRUSH defaultbrush = NULL;
	static HBRUSH hotbrush = NULL;

	// ---------------------------------------------- Hover
	if (item->uItemState & CDIS_HOT) // Hover States
	{
		if (Toggle == 1) // Selected
		{
			hotbrush = App->HotBrush_1; // Soft Green
		}
		else
		{
			hotbrush = App->HotBrush_2; // Gray
		}

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		SelectObject(item->hdc, BlackPen);
		SelectObject(item->hdc, hotbrush);

		return;
	}

	// ---------------------------------------------- Selectd States
	if (Toggle == 1) // Selected
	{
		defaultbrush = App->Brush_Green;
	}
	else
	{
		defaultbrush = Brush_But_Normal;
	}

	SelectObject(item->hdc, BlackPen);
	SelectObject(item->hdc, defaultbrush);

	RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

	SelectObject(item->hdc, BlackPen);
	SelectObject(item->hdc, defaultbrush);
}

// *************************************************************************
// *	Custom_Button_Toggle_Tabs:- Terry and Hazel Flanigan 2024  	  	   *
// *************************************************************************
void CL64_App::Custom_Button_Toggle_Tabs(LPNMCUSTOMDRAW item, bool Toggle) const
{
	static HBRUSH defaultbrush = NULL;
	static HBRUSH hotbrush = NULL;

	// ---------------------------------------------- Hover
	if (item->uItemState & CDIS_HOT)
	{
		if (Toggle == 1)
		{
			hotbrush = HotBrush_2; // Gray
		}
		else
		{
			hotbrush = HotBrush_2; // Gray
		}

		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(240, 240, 240));

		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return;
	}

	// ---------------------------------------------- Selectd States
	if (Toggle == 1)
	{
		defaultbrush = App->Brush_Tabs; // Selected
	}
	else
	{
		defaultbrush = Brush_Tabs_UnSelected; // Unselected 
	}

	SelectObject(item->hdc, WhitePen);
	SelectObject(item->hdc, defaultbrush);

	RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

	SelectObject(item->hdc, WhitePen);
	SelectObject(item->hdc, defaultbrush);
}

// *************************************************************************
// *		Custom_Button_Normal:- Terry and Hazel Flanigan 2024  	  	   *
// *************************************************************************
void CL64_App::Custom_Button_Normal(LPNMCUSTOMDRAW item) const
{
	if (item->uItemState & CDIS_SELECTED) // Push Down
	{
		SelectObject(item->hdc, BlackPen);
		SelectObject(item->hdc, Brush_But_Pressed);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		SelectObject(item->hdc, BlackPen);
		SelectObject(item->hdc, Brush_But_Pressed);
	
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

		SelectObject(item->hdc, BlackPen);
		SelectObject(item->hdc, App->Brush_But_Normal);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		SelectObject(item->hdc, BlackPen);
		SelectObject(item->hdc, Brush_But_Normal);
		
		return;
	}
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
// *		Custom_Button_Greyed:- Terry and Hazel Flanigan 2024   	  	   *
// *************************************************************************
void CL64_App::Custom_Button_Greyed(LPNMCUSTOMDRAW item)
{
	HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 255)); // Idle 

	HGDIOBJ old_pen = SelectObject(item->hdc, pen);
	HGDIOBJ old_brush = SelectObject(item->hdc, Brush_Tabs_UnSelected);

	RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

	SelectObject(item->hdc, old_pen);
	SelectObject(item->hdc, old_brush);
	DeleteObject(pen);
}

// *************************************************************************
// *			Open_Tool:- Terry and Hazel Flanigan 2024			  	   *
// *************************************************************************
void CL64_App::Open_Tool(char* Tool)
{
	char Path[1024];
	strcpy(Path, App->GD_Directory_FullPath);
	strcat(Path, "\\");
	strcat(Path, Tool);

	ShellExecute(0, "open", Path, 0, 0, SW_SHOW);
}

