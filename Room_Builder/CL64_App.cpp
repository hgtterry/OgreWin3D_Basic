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
#include "CL64_App.h"

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
	CL_CL64_Brush_X =			nullptr;

	CUR = 0;

	hInst = nullptr;
	MainHwnd = nullptr;
	
	CursorPosX = 500;
	CursorPosY = 500;

	AppBackground = 0;

	Font_CB15 = 0;
	Font_CB18 = 0;
	Font_Arial20 = 0;
	Font_Banner = 0;

	flag_Debug_App = 0;
	flag_OgreStarted = 0;

	ViewPLeaseWait = nullptr;

	RB_Directory_FullPath[0] = 0;
}

CL64_App::~CL64_App(void)
{
}

// *************************************************************************
// *			InitApp:- Terry and Hazel Flanigan 2024					   *
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
	CL_CL64_Brush_X =			new CL64_Brush_X();
	
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
	
	SetBrushes_Fonts();
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
// *				Say_Int:- Terry and Hazel Flanigan 2024				   *
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
