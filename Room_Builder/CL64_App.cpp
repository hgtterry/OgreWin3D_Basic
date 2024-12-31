/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
	CL_MapEditor =		nullptr;
	CL_Maths =			nullptr;
	CL_Box =			nullptr;
	CL_Render =			nullptr;
	CL_Level =			nullptr;
	CL_Prefs =			nullptr;
	CL_Ini_File =		nullptr;
	CL_Doc =			nullptr;
	CL_Utilities =		nullptr;
	CL_Brush =			nullptr;
	CL_FaceList =		nullptr;
	CL_Face =			nullptr;
	CL_BrushTemplate =	nullptr;
	CL_SelBrushList =	nullptr;
	CL_Array =			nullptr;
	CL_SelFaceList =	nullptr;
	CL_Dialogs =		nullptr;

	hInst = nullptr;
	MainHwnd = nullptr;
	
	AppBackground = 0;

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
	CL_MapEditor =		new CL64_MapEditor();
	CL_Maths =			new CL64_Maths();
	CL_Box =			new CL64_Box();
	CL_Render =			new CL64_Render();
	CL_Level =			new CL64_Level();
	CL_Prefs =			new CL64_Prefs();
	CL_Ini_File =		new CL64_Ini_File();
	CL_Doc =			new CL64_Doc();
	CL_Utilities =		new CL64_Utilities();
	CL_Brush =			new CL64_Brush();
	CL_FaceList =		new CL64_FaceList();
	CL_Face =			new CL64_Face();
	CL_BrushTemplate =	new CL64_BrushTemplate();
	CL_SelBrushList =	new CL64_SelBrushList();
	CL_Array =			new CL64_Array();
	CL_SelFaceList =	new CL64_SelFaceList();
	CL_Dialogs =		new CL64_Dialogs();

	SetBrushes_Fonts();
}

// *************************************************************************
// *			SetBrushes_Fonts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_App::SetBrushes_Fonts(void)
{
	AppBackground = CreateSolidBrush(RGB(213, 222, 242));
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
