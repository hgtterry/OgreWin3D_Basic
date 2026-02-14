/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
#include "Sandbox.h"
#include "Structures.cpp"

enum Base
{
	Colour_None = 0,
	Colour_Background = 1,
};

Sandbox::Sandbox(void)
{
	Actual_Colour = CreateSolidBrush(RGB(213, 0, 0));

	Selected_Item = Colour_Background;

	R_pos = 0;
	G_pos = 0;
	B_pos = 0;

	hTrack_Red = nullptr;
	hTrack_Green = nullptr;
	hTrack_Blue = nullptr;

	Colour_Box_hWnd = nullptr;

	Render_hWnd = nullptr;

	// 3D
	RenderListener = nullptr;

	Ogre_MV_Window = nullptr;
	Ogre_MV_SceneMgr = nullptr;
	Ogre_MV_Camera = nullptr;
	Ogre_MV_CamNode = nullptr;

}

Sandbox::~Sandbox(void)
{
}

// *************************************************************************
// *			Start_Colour_Mixer:- Terry Mo and Hazel  2025			   *
// *************************************************************************
void Sandbox::Start_Colour_Mixer()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_COLOUR_MIXER, App->MainHwnd, (DLGPROC)Proc_Colour_Mixer);
}

// *************************************************************************
// *		   Proc_Colour_Mixer:- Terry Mo and Hazel  2025				   *
// *************************************************************************
LRESULT Sandbox::Proc_Colour_Mixer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_RED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GREEN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_BLUE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_ACTUAL_RED, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_ACTUAL_GREEN, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_ACTUAL_BLUE, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_CK_BACKGROUND, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_Sandbox->hTrack_Red = GetDlgItem(hDlg, IDC_SLIDER_RED);
		App->CL_Sandbox->hTrack_Green = GetDlgItem(hDlg, IDC_SLIDER_GREEN);
		App->CL_Sandbox->hTrack_Blue = GetDlgItem(hDlg, IDC_SLIDER_BLUE);

		App->CL_Sandbox->Colour_Box_hWnd = GetDlgItem(hDlg, IDC_ST_ACTUAL_COLOUR);

		SendMessageW(App->CL_Sandbox->hTrack_Red, TBM_SETRANGE, TRUE, MAKELONG(-255, 0));
		SendMessageW(App->CL_Sandbox->hTrack_Red, TBM_SETPAGESIZE, 0, 1);
		SendMessageW(App->CL_Sandbox->hTrack_Red, TBM_SETTICFREQ, 1, 0);

		SendMessageW(App->CL_Sandbox->hTrack_Green, TBM_SETRANGE, TRUE, MAKELONG(-255, 0));
		SendMessageW(App->CL_Sandbox->hTrack_Green, TBM_SETPAGESIZE, 0, 1);
		SendMessageW(App->CL_Sandbox->hTrack_Green, TBM_SETTICFREQ, 1, 0);

		SendMessageW(App->CL_Sandbox->hTrack_Blue, TBM_SETRANGE, TRUE, MAKELONG(-255, 0));
		SendMessageW(App->CL_Sandbox->hTrack_Blue, TBM_SETPAGESIZE, 0, 1);
		SendMessageW(App->CL_Sandbox->hTrack_Blue, TBM_SETTICFREQ, 1, 0);

		App->CL_Sandbox->Set_Sliders(hDlg);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{

		if (GetDlgItem(hDlg, IDC_ST_ACTUAL_COLOUR) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->CL_Sandbox->Actual_Colour;
		}

		if (GetDlgItem(hDlg, IDC_ST_RED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_GREEN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_BLUE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_SLIDER_RED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_SLIDER_GREEN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_SLIDER_BLUE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_BACKGROUND) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		/*if (GetDlgItem(hDlg, IDC_STMESSAGE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		return CDRF_DODEFAULT;
	}

	case WM_VSCROLL:
	{
		App->CL_Sandbox->Get_Sliders(hDlg, lParam);
		return false;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_CK_BACKGROUND)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_BACKGROUND);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Sandbox->Selected_Item = Colour_Background;
				App->CL_Sandbox->Set_Sliders(hDlg);
				return TRUE;
			}
			else
			{
				App->CL_Sandbox->Selected_Item = Colour_None;
				return TRUE;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_WINCOLOR)
		{
			App->CL_File_IO->Colour_Picker();

			App->CL_Sandbox->Colour.R = (App->CL_File_IO->rgbCurrent) & 0xff;
			App->CL_Sandbox->Colour.G = (App->CL_File_IO->rgbCurrent >> 8) & 0xff;
			App->CL_Sandbox->Colour.B = (App->CL_File_IO->rgbCurrent >> 16) & 0xff;

			App->CL_Sandbox->R_pos = App->CL_Sandbox->Colour.R;
			App->CL_Sandbox->G_pos = App->CL_Sandbox->Colour.G;
			App->CL_Sandbox->B_pos = App->CL_Sandbox->Colour.B;

			//int iA = (App->CL_File_IO->rgbCurrent >> 24) & 0xff;
			App->CL_Sandbox->Get_Sliders(hDlg,NULL);
			App->CL_Sandbox->Set_Sliders(hDlg);
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDOK)
		{
			DeleteObject(App->CL_Sandbox->Actual_Colour);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			DeleteObject(App->CL_Sandbox->Actual_Colour);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	return FALSE;
}

// *************************************************************************
// *				Set_Sliders:- Terry Mo and Hazel  2025				   *
// *************************************************************************
void Sandbox::Set_Sliders(HWND hDlg)
{
	if (Selected_Item == Colour_Background)
	{
		R_pos = App->CL_Editor_Map->Background_Colour.R;
		G_pos = App->CL_Editor_Map->Background_Colour.G;
		B_pos = App->CL_Editor_Map->Background_Colour.B;

		Colour.R = App->CL_Editor_Map->Background_Colour.R;
		Colour.G = App->CL_Editor_Map->Background_Colour.G;
		Colour.B = App->CL_Editor_Map->Background_Colour.B;

		Set_ColourBox();
	}

	// Red
	SendMessageW(App->CL_Sandbox->hTrack_Red, TBM_SETPOS, TRUE, - Colour.R);
	SetDlgItemInt(hDlg, IDC_ST_ACTUAL_RED, Colour.R, false);

	// Green
	SendMessageW(App->CL_Sandbox->hTrack_Green, TBM_SETPOS, TRUE, - Colour.G);
	SetDlgItemInt(hDlg, IDC_ST_ACTUAL_GREEN, Colour.G, false);

	// Blue
	SendMessageW(App->CL_Sandbox->hTrack_Blue, TBM_SETPOS, TRUE, - Colour.B);
	SetDlgItemInt(hDlg, IDC_ST_ACTUAL_BLUE, Colour.B, false);

}

// *************************************************************************
// *				Get_Sliders:- Terry Mo and Hazel  2025				   *
// *************************************************************************
void Sandbox::Get_Sliders(HWND hDlg, LPARAM lParam)
{
	if (HWND(lParam) == GetDlgItem(hDlg, IDC_SLIDER_RED))
	{
		char Num_Red[10]{ 0 };

		R_pos = SendMessageW(hTrack_Red, TBM_GETPOS, 0, 0);
		_itoa(abs(R_pos), Num_Red, 10);
		SetDlgItemText(hDlg, IDC_ST_ACTUAL_RED, Num_Red);
	}

	if (HWND(lParam) == GetDlgItem(hDlg, IDC_SLIDER_GREEN))
	{
		char Num_Green[10]{ 0 };

		G_pos = SendMessageW(hTrack_Green, TBM_GETPOS, 0, 0);
		_itoa(abs(G_pos), Num_Green, 10);
		SetDlgItemText(hDlg, IDC_ST_ACTUAL_GREEN, Num_Green);
	}

	if (HWND(lParam) == GetDlgItem(hDlg, IDC_SLIDER_BLUE))
	{
		char Num_Blue[10]{ 0 };

		B_pos = SendMessageW(hTrack_Blue, TBM_GETPOS, 0, 0);
		_itoa(abs(B_pos), Num_Blue, 10);
		SetDlgItemText(hDlg, IDC_ST_ACTUAL_BLUE, Num_Blue);
	}

	if (Selected_Item == Colour_Background)
	{
		Colour.R = abs(R_pos);
		Colour.G = abs(G_pos);
		Colour.B = abs(B_pos);

		App->CL_Editor_Map->Background_Colour.R = abs(R_pos);
		App->CL_Editor_Map->Background_Colour.G = abs(G_pos);
		App->CL_Editor_Map->Background_Colour.B = abs(B_pos);

		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

		Set_ColourBox();
		RedrawWindow(Colour_Box_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		//App->AppBackground = CreateSolidBrush(RGB(Colour.R, Colour.G, Colour.B));
		//UpdateWindow(App->MainHwnd);

		//RedrawWindow(App->CL_Properties_Tabs->Tabs_Control_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

// *************************************************************************
// *				Get_Sliders:- Terry Mo and Hazel  2025				   *
// *************************************************************************
void Sandbox::Set_ColourBox()
{
	DeleteObject(Actual_Colour);
	Actual_Colour = CreateSolidBrush(RGB(Colour.R, Colour.G, Colour.B));
}

// *************************************************************************
// *			Duplicate_Brush:- Terry Mo and Hazel  2025				   *
// *************************************************************************
void Sandbox::Duplicate_Brush()
{
	int SB = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

	if (SB > 0)
	{
		Brush* pBrush = nullptr;
		Brush* pClone = nullptr;

		pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, 0);
		pClone = App->CL_X_Brush->Brush_Clone(pBrush);

		int Count = App->CL_X_Brush->Get_Brush_Count();
		char Num[32];
		char Name[32];
		_itoa(Count + 1, Num, 10);
		strcpy(Name, pBrush->Name);
		strcat(Name, "_");
		strcat(Name, Num);

		if (pClone)
		{
			strcpy(pClone->Name, Name);

			App->CL_Level->Level_AppendBrush(pClone);
			App->CL_X_SelBrushList->SelBrushList_Remove(App->CL_Doc->pSelBrushes, pBrush);

			App->CL_Properties_Brushes->Fill_ListBox();

			App->CL_Doc->CurBrush = pClone;
			App->CL_Brush_X->Select_Brush_Editor(App->CL_Doc->CurBrush);

			App->CL_Doc->Set_Faces_To_Brush_Name_Selected();
			App->CL_Level->flag_Level_is_Modified = true;
			App->Say("Duplicated");
		}
	}

	//App->Say(pClone->Name);
}

// *************************************************************************
// *			Start_Ogre_Window:- Terry Mo and Hazel  2025			   *
// *************************************************************************
void Sandbox::Start_Ogre_Window()
{
	CreateDialog(App->hInst, (LPCTSTR)IDD_OGRE_WINDOW, App->MainHwnd, (DLGPROC)Proc_Ogre_Dialog);
}

// *************************************************************************
// *		  Proc_Ogre_Dialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK Sandbox::Proc_Ogre_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//auto& m_Box = App->CL_App_Templates->CL_CreateBox; // App->CL_App_Templates->CL_CreateBox

	switch (message)
	{

	case WM_INITDIALOG:
	{
		App->CL_Sandbox->Render_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_OGRE_CANVAS, hDlg, (DLGPROC)Proc_Viewer_3D);
		App->CL_Sandbox->Set_OgreWindow();
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*const int controlIds[] =
		{
			IDC_STTOP, IDC_STBOTTOM, IDC_STTOPX, IDC_STWALL,
			IDC_STGENERAL, IDC_STZTOP, IDC_STBOTX, IDC_STBOTZ,
			IDC_STYSIZE, IDC_STNAME, IDC_STCAMPOS,
			IDC_CKWORLDCENTRE, IDC_CKCAMPOSITION
		};

		for (const auto& id : controlIds)
		{
			if (GetDlgItem(hDlg, id) == (HWND)lParam)
			{
				SetBkColor((HDC)wParam, RGB(0, 0, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		return CDRF_DODEFAULT;
	}

	//case WM_MOUSEWHEEL:
	//{
	//	int zDelta = (short)HIWORD(wParam);    // wheel rotation

	//	if (zDelta > 0)
	//	{
	//		App->CL_X_Shapes_3D->RenderListener->Wheel_Move = -1;
	//	}
	//	else if (zDelta < 0)
	//	{
	//		App->CL_X_Shapes_3D->RenderListener->Wheel_Move = 1;
	//	}

	//	return 1;
	//}

	case WM_COMMAND:
	{
		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Sandbox->Close_OgreWindow();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_Sandbox->Close_OgreWindow();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *		Proc_Viewer_3D:- Terry and Hazel Flanigan 2025 			   *
// *************************************************************************
LRESULT CALLBACK Sandbox::Proc_Viewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG: // Bernie as the dialog is created
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		//if (App->flag_3D_Started == false)
		{
			return (LONG)App->BlackBrush;
		}
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		//SetFocus(App->CL_X_Shapes_3D->Render_hWnd);
		break;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		////if (App->flag_3D_Started == true)
		//{
		//	POINT cursorPosition;
		//	GetCursorPos(&cursorPosition);
		//	App->CL_X_Shapes_3D->CursorPosX = cursorPosition.x;
		//	App->CL_X_Shapes_3D->CursorPosY = cursorPosition.y;

		//	auto& listener = App->CL_X_Shapes_3D->RenderListener;
		//	listener->Pl_Cent500X = cursorPosition.x;
		//	listener->Pl_Cent500Y = cursorPosition.y;

		//	SetCapture(App->CL_X_Shapes_3D->Render_hWnd);
		//	SetCursorPos(cursorPosition.x, cursorPosition.y);

		//	App->CL_X_Shapes_3D->RenderListener->flag_Pl_RightMouseDown = true;
		//	App->CUR = SetCursor(NULL);

		//	return 1;
		//}

		return 1;
	}
	case WM_RBUTTONUP:
	{
		////if (App->flag_3D_Started == true)
		//{
		//	ReleaseCapture();
		//	App->CL_X_Shapes_3D->RenderListener->flag_Pl_RightMouseDown = 0;
		//	SetCursor(App->CUR);
		//	return 1;
		//}

		return 1;
	}
	// Left Mouse Button
	case WM_LBUTTONDOWN:
	{
		////if (App->flag_3D_Started == true)
		//{
		//	POINT p;
		//	GetCursorPos(&p);

		//	App->CL_X_Shapes_3D->CursorPosX = p.x;
		//	App->CL_X_Shapes_3D->CursorPosY = p.y;

		//	auto& listener = App->CL_X_Shapes_3D->RenderListener;
		//	listener->Pl_Cent500X = p.x;
		//	listener->Pl_Cent500Y = p.y;

		//	SetCapture(App->CL_X_Shapes_3D->Render_hWnd);

		//	SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);

		//	App->CL_X_Shapes_3D->RenderListener->flag_Pl_LeftMouseDown = 1;

		//	App->CUR = SetCursor(NULL);

		//	return 1;
		//}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		////if (App->flag_3D_Started == true)
		//{
		//	ReleaseCapture();
		//	App->CL_X_Shapes_3D->RenderListener->flag_Pl_LeftMouseDown = 0;
		//	SetCursor(App->CUR);
		//	return 1;
		//}

		return 1;
	}

	}

	return FALSE;
}

// *************************************************************************
// *			SetOgreWindow:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void Sandbox::Set_OgreWindow()
{

	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)Render_hWnd);

	Ogre_MV_Window = App->CL_Ogre->mRoot->createRenderWindow("MeshViewWin", 1024, 768, false, &options);

	Ogre_MV_SceneMgr = App->CL_Ogre->mRoot->createSceneManager("DefaultSceneManager", "MeshViewGD");

	Ogre_MV_CamNode = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode("Camera_Node");

	Ogre_MV_Camera = Ogre_MV_SceneMgr->createCamera("CameraMV");
	Ogre_MV_Camera->setNearClipDistance(0.1);
	Ogre_MV_Camera->setFarClipDistance(8000);

	Ogre_MV_CamNode->attachObject(Ogre_MV_Camera);
	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, 20));

	Ogre::Viewport* vp = Ogre_MV_Window->addViewport(Ogre_MV_Camera);
	Ogre_MV_Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	Ogre_MV_SceneMgr->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	//App->CL_ImGui->Init_ImGui();
	
	RenderListener = new Ogre_Win_Render_Listener();
	App->CL_Ogre->mRoot->addFrameListener(RenderListener);

	Ogre::Entity* Ogre_Ent;
	Ogre::SceneNode* Ogre_Node;

	Ogre_Ent = Ogre_MV_SceneMgr->createEntity("Imported_Entity", "Sinbad.mesh", App->CL_Ogre->App_Resource_Group);
	Ogre_Node = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre_Node->attachObject(Ogre_Ent);

	Ogre_Node->setVisible(true);
	Ogre_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	Ogre_Node->setPosition(0, 0, 0);
	Ogre_Node->setScale(1, 1, 1);

	App->CL_Ogre->mRoot->_setCurrentSceneManager(Ogre_MV_SceneMgr);
	App->CL_ImGui->Init_ImGui();
}

// *************************************************************************
// *		Close_MeshWindow:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void Sandbox::Close_OgreWindow(void)
{
	//App->CL_MeshViewer->flag_MV_Render_Debug = 0;

	App->CL_Ogre->mRoot->detachRenderTarget("MeshViewWin");
	Ogre_MV_Window->destroy();
	App->CL_Ogre->mRoot->destroySceneManager(Ogre_MV_SceneMgr);

	App->CL_Ogre->mRoot->removeFrameListener(RenderListener);

	delete RenderListener;
	RenderListener = nullptr;
}



