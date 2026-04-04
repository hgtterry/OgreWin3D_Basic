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
#include "CL64_3D_TL_View.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

CL64_3D_TL_View::CL64_3D_TL_View()
{
	Render_hWnd = nullptr;
	ViewGLhWnd_TL = nullptr;

	TL_RenderListener = nullptr;
	OGL_TL_Listener = nullptr;

	Ogre_TL_Window = nullptr;
	Ogre_TL_SceneMgr = nullptr;
	Ogre_TL_Camera = nullptr;
	Ogre_TL_CamNode = nullptr;

	vp = nullptr;

	CursorPosX = 0;
	CursorPosY = 0;
}

CL64_3D_TL_View::~CL64_3D_TL_View()
{
}

// *************************************************************************
// *			Create_Top_Left_Window:- Terry Mo and Hazel  2026		   *
// *************************************************************************
void CL64_3D_TL_View::Create_Top_Left_Window()
{
	App->CL_Editor_Map->Top_Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_TOP_LEFT, App->CL_Editor_Map->Main_View_Dlg_Hwnd, (DLGPROC)Proc_Top_Left_Window);

}

// *************************************************************************
// *		  Proc_Top_Left_Window:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
LRESULT CALLBACK CL64_3D_TL_View::Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_TL_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_Editor_Map->Top_Left_Banner_Hwnd = GetDlgItem(hDlg, IDC_ST_TL_TITLE);

		//App->CL_3D_TL_View->ViewGLhWnd_TL = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_RENDER_WINDOW, hDlg, (DLGPROC)Proc_Ogre_TL);

		return TRUE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_TL_TITLE) == (HWND)lParam)
		{
			if (App->CL_Editor_Map->Selected_Window == Enums::Selected_Map_View_TL)
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->Brush_Green;
			}
			else
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
		}

		return FALSE;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		return 1;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		return CDRF_DODEFAULT;
	}

	case WM_MOUSEWHEEL:
	{
		int zDelta = (short)HIWORD(wParam);    // wheel rotation

		if (zDelta > 0)
		{
			App->CL_3D_TL_View->TL_RenderListener->Wheel_Move = -1;
		}
		else if (zDelta < 0)
		{
			App->CL_3D_TL_View->TL_RenderListener->Wheel_Move = 1;
		}

		return 1;
	}

	case WM_COMMAND:
	{
		break;
	}

	case WM_PAINT:
	{
		if (App->flag_3D_Started == true)
		{
			App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];
			App->CL_Editor_Map->Draw_Screen(hDlg);
		}

		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *		Proc_Ogre_TL:- Terry and Hazel Flanigan 2026 				   *
// *************************************************************************
LRESULT CALLBACK CL64_3D_TL_View::Proc_Ogre_TL(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG: // Bernie as the dialog is created
	{

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		if (App->flag_3D_Started == false)
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
		if (App->flag_3D_Started == true)
		{
			POINT cursorPosition;
			GetCursorPos(&cursorPosition);
			App->CL_3D_TL_View->CursorPosX = cursorPosition.x;
			App->CL_3D_TL_View->CursorPosY = cursorPosition.y;

			auto& listener = App->CL_3D_TL_View->TL_RenderListener;
			listener->Pl_Cent500X = cursorPosition.x;
			listener->Pl_Cent500Y = cursorPosition.y;

			SetCapture(App->CL_3D_TL_View->ViewGLhWnd_TL);
			SetCursorPos(cursorPosition.x, cursorPosition.y);

			App->CL_3D_TL_View->TL_RenderListener->flag_RightMouseDown = true;
			App->CUR = SetCursor(NULL);

			return 1;
		}

		return 1;
	}
	case WM_RBUTTONUP:
	{
		if (App->flag_3D_Started == true)
		{
			ReleaseCapture();
			App->CL_3D_TL_View->TL_RenderListener->flag_RightMouseDown = false;
			SetCursor(App->CUR);
			return 1;
		}

		return 1;
	}
	// Left Mouse Button
	case WM_LBUTTONDOWN:
	{
		App->CL_Editor_Map->Current_View = App->CL_Editor_Map->VCam[V_TL];

		if (App->CL_Editor_Map->Selected_Window != Enums::Selected_Map_View_TL)
		{
			App->CL_Editor_Map->Set_Selected_View(Enums::Selected_Map_View_TL);
		}

		if (App->flag_3D_Started == true)
		{
			/*POINT p;
			GetCursorPos(&p);

			App->CL_X_Shapes_3D->CursorPosX = p.x;
			App->CL_X_Shapes_3D->CursorPosY = p.y;

			auto& listener = App->CL_X_Shapes_3D->RenderListener;
			listener->Pl_Cent500X = p.x;
			listener->Pl_Cent500Y = p.y;

			SetCapture(App->CL_X_Shapes_3D->Render_hWnd);

			SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);*/

			//App->CL_3D_TL_View->TL_RenderListener->flag_RightMouseDown = true;

			//App->CUR = SetCursor(NULL);

			return 1;
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		if (App->flag_3D_Started == true)
		{
			/*ReleaseCapture();
			App->CL_X_Shapes_3D->RenderListener->flag_Pl_LeftMouseDown = 0;
			SetCursor(App->CUR);*/

			//App->CL_3D_TL_View->TL_RenderListener->flag_RightMouseDown = false;
			return 1;
		}

		return 1;
	}

	}

	return FALSE;
}

// *************************************************************************
// *			Set_OgreWindow_TL:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_3D_TL_View::Set_OgreWindow_TL()
{
	return;
	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)ViewGLhWnd_TL);// Render_hWnd);

	Ogre_TL_Window = App->CL_Ogre->mRoot->createRenderWindow("TL_ViewWin", 1024, 768, false, &options);


	Ogre_TL_SceneMgr = App->CL_Ogre->mRoot->createSceneManager("DefaultSceneManager", "TL_ViewWin");

	Ogre_TL_CamNode = Ogre_TL_SceneMgr->getRootSceneNode()->createChildSceneNode("TL_Camera_Node");

	Ogre_TL_Camera = Ogre_TL_SceneMgr->createCamera("TL_Camera");
	Ogre_TL_Camera->setNearClipDistance(0.1);
	Ogre_TL_Camera->setFarClipDistance(8000);

	Ogre_TL_CamNode->attachObject(Ogre_TL_Camera);
	Ogre_TL_CamNode->setPosition(Ogre::Vector3(0, 0, 20));

	vp = Ogre_TL_Window->addViewport(Ogre_TL_Camera);
	
	//Ogre_TL_Camera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
	//Ogre_TL_Camera->setOrthoWindow(vp->getActualWidth(), vp->getActualHeight());

	Ogre_TL_Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setBackgroundColour(ColourValue(0.235, 0.235, 0.235));

	Ogre_TL_SceneMgr->setAmbientLight(ColourValue(1, 1, 1));

	TL_RenderListener = new CL64_3D_TL_Listener();
	App->CL_Ogre->mRoot->addFrameListener(TL_RenderListener);

	OGL_TL_Listener = new CL64_3D_TL_OGL_Listener();
	Ogre_TL_SceneMgr->addRenderQueueListener(OGL_TL_Listener);

	Set_Zoom();

}

// *************************************************************************
// *		Close_MeshWindow:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_TL_View::Close_OgreWindow(void)
{
	//App->CL_MeshViewer->flag_MV_Render_Debug = 0;

	App->CL_Ogre->mRoot->detachRenderTarget("TL_ViewWin");
	Ogre_TL_Window->destroy();
	App->CL_Ogre->mRoot->destroySceneManager(Ogre_TL_SceneMgr);

	App->CL_Ogre->mRoot->removeFrameListener(TL_RenderListener);

	delete TL_RenderListener;
	TL_RenderListener = nullptr;
}

// *************************************************************************
// *				Set_Zoom:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_TL_View::Set_Zoom(void)
{
	return;
	Ogre_TL_CamNode->setPosition(Ogre::Vector3(0, 0, 0));
	Ogre_TL_CamNode->setOrientation(Ogre::Quaternion::IDENTITY);

	float zoom = 0;

	if (App->CL_Model->flag_Model_Loaded == true)
	{
		zoom = max(App->CL_Model->S_BoundingBox[0]->Size[0].z, App->CL_Model->S_BoundingBox[0]->Size[0].x);
	}
	else
	{
		zoom = 20;
	}

	Ogre_TL_CamNode->setPosition(Ogre::Vector3(0, zoom * 2, 0));
	Ogre_TL_CamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

}

// ************************************************************************
// *			ResizeOgreWindow_TL:- Terry Mo and Hazel 2026			  *
// ************************************************************************
void CL64_3D_TL_View::ResizeOgreWindow_TL()
{
	return;
	RECT clientRect;
	GetClientRect(App->CL_Editor_Map->Top_Left_Window_Hwnd, &clientRect);

	// Set the position and size of the window
	SetWindowPos(ViewGLhWnd_TL, NULL, 0, 17, clientRect.right, clientRect.bottom - 17, SWP_NOZORDER);

	// Check if the 3D engine has started
	if (App->flag_3D_Started == true)
	{
		// Get the updated client rectangle
		RECT updatedRect;
		GetClientRect(App->CL_Editor_Map->Top_Left_Window_Hwnd, &updatedRect);

		// Ensure the height is valid and the camera is initialized
		if ((updatedRect.bottom - updatedRect.top) != 0 && App->CL_Ogre->mCamera != nullptr)
		{
			Ogre_TL_Window->windowMovedOrResized();
			Ogre_TL_Camera->setAspectRatio(static_cast<Ogre::Real>(Ogre_TL_Window->getWidth()) /
				static_cast<Ogre::Real>(Ogre_TL_Window->getHeight()));

			//App->CL_Ogre->camNode->yaw(Radian(0));
		}
	}
}
