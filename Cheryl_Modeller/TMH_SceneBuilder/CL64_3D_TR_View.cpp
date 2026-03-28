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
#include "CL64_3D_TR_View.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

CL64_3D_TR_View::CL64_3D_TR_View()
{
	Render_hWnd = nullptr;
	RenderListener = nullptr;
	OGL_TR_Listener = nullptr;

	Ogre_MV_Window = nullptr;
	Ogre_MV_SceneMgr = nullptr;
	Ogre_MV_Camera = nullptr;
	Ogre_MV_CamNode = nullptr;
}

CL64_3D_TR_View::~CL64_3D_TR_View()
{
}

// *************************************************************************
// *	  		Set_View_Top:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_TR_View::Set_View_Top()
{
    if (App->CL_Model->flag_Model_Loaded == true)
    {
        App->CL_Mesh->Show_Mesh_Textures();
        App->CL_Mesh->Show_Mesh_Faces();

        App->CL_Camera->Reset_View_and_Zoom();

        App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 0, 0));
        App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);

        float zoom = max(App->CL_Model->S_BoundingBox[0]->Size[0].z, App->CL_Model->S_BoundingBox[0]->Size[0].x);

        App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, zoom * 2, 0));
        App->CL_Ogre->camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

		Start_Ogre_Window();
    }
}

// *************************************************************************
// *			Start_Ogre_Window:- Terry Mo and Hazel  2026			   *
// *************************************************************************
void CL64_3D_TR_View::Start_Ogre_Window()
{
	CreateDialog(App->hInst, (LPCTSTR)IDD_OGRE_WINDOW, App->MainHwnd, (DLGPROC)Proc_Ogre_Dialog);
}

// *************************************************************************
// *		  Proc_Ogre_Dialog:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_3D_TR_View::Proc_Ogre_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		App->CL_3D_TR_View->Render_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_OGRE_CANVAS, hDlg, (DLGPROC)Proc_Viewer_3D);
		App->CL_3D_TR_View->Set_OgreWindow();
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
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

	case WM_MOUSEWHEEL:
	{
		int zDelta = (short)HIWORD(wParam);    // wheel rotation

		if (zDelta > 0)
		{
			App->CL_3D_TR_View->RenderListener->Wheel_Move = -1;
		}
		else if (zDelta < 0)
		{
			App->CL_3D_TR_View->RenderListener->Wheel_Move = 1;
		}

		return 1;
	}

	case WM_COMMAND:
	{
		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_3D_TR_View->Close_OgreWindow();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_3D_TR_View->Close_OgreWindow();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *		Proc_Viewer_3D:- Terry and Hazel Flanigan 2026 				   *
// *************************************************************************
LRESULT CALLBACK CL64_3D_TR_View::Proc_Viewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
void CL64_3D_TR_View::Set_OgreWindow()
{

	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)Render_hWnd);

	Ogre_MV_Window = App->CL_Ogre->mRoot->createRenderWindow("MeshViewWin22", 1024, 768, false, &options);

	Ogre_MV_SceneMgr = App->CL_Ogre->mRoot->createSceneManager("DefaultSceneManager", "MeshViewGD22");

	Ogre_MV_CamNode = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode("Camera_Node22");

	Ogre_MV_Camera = Ogre_MV_SceneMgr->createCamera("CameraMV22");
	Ogre_MV_Camera->setNearClipDistance(0.1);
	Ogre_MV_Camera->setFarClipDistance(8000);

	Ogre_MV_CamNode->attachObject(Ogre_MV_Camera);
	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, 20));

	Ogre::Viewport* vp = Ogre_MV_Window->addViewport(Ogre_MV_Camera);
	Ogre_MV_Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	Ogre_MV_SceneMgr->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	//Ogre_MV_Camera->setRenderQueueGroup(100);

	RenderListener = new Ogre_Win_Render_Listener();
	App->CL_Ogre->mRoot->addFrameListener(RenderListener);

	OGL_TR_Listener = new CL64_3D_TR_OGL_Listener();
	Ogre_MV_SceneMgr->addRenderQueueListener(OGL_TR_Listener);


	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, 0));
	Ogre_MV_CamNode->setOrientation(Ogre::Quaternion::IDENTITY);

	float zoom = max(App->CL_Model->S_BoundingBox[0]->Size[0].z, App->CL_Model->S_BoundingBox[0]->Size[0].x);

	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, zoom * 2, 0));
	Ogre_MV_CamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	/*Ogre::Entity* Ogre_Ent;
	Ogre::SceneNode* Ogre_Node;

	Ogre_Ent = Ogre_MV_SceneMgr->createEntity("Imported_Entity", "Sinbad.mesh", App->CL_Ogre->App_Resource_Group);
	Ogre_Node = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre_Node->attachObject(Ogre_Ent);

	Ogre_Node->setVisible(true);
	Ogre_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	Ogre_Node->setPosition(0, 0, 0);
	Ogre_Node->setScale(1, 1, 1);*/

}

// *************************************************************************
// *		Close_MeshWindow:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_3D_TR_View::Close_OgreWindow(void)
{
	//App->CL_MeshViewer->flag_MV_Render_Debug = 0;

	App->CL_Ogre->mRoot->detachRenderTarget("MeshViewWin22");
	Ogre_MV_Window->destroy();
	App->CL_Ogre->mRoot->destroySceneManager(Ogre_MV_SceneMgr);

	App->CL_Ogre->mRoot->removeFrameListener(RenderListener);

	delete RenderListener;
	RenderListener = nullptr;
}

