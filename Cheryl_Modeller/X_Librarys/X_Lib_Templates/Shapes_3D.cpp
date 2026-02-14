#include "pch.h"
#include "CL64_App.h"
#include "Shapes_3D.h"

Shapes_3D::Shapes_3D(void)
{
	RenderListener = nullptr;
	OGL_RenderListener = nullptr;

	Render_hWnd = nullptr;

	Start_Zoom = 300;

	CursorPosX = 500;
	CursorPosY = 500;

	// 3D
	Ogre_MV_Window = nullptr;
	Ogre_MV_SceneMgr = nullptr;
	Ogre_MV_Camera = nullptr;
	Ogre_MV_CamNode = nullptr;
	Ogre_MvEnt = nullptr;
	Ogre_MvNode = nullptr;

	OGL_Obj = nullptr;
	OGL_Node = nullptr;

}

Shapes_3D::~Shapes_3D(void)
{
}

// *************************************************************************
// *			SetOgreWindow:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void Shapes_3D::Set_OgreWindow()
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
	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, Start_Zoom));

	Ogre::Viewport* vp = Ogre_MV_Window->addViewport(Ogre_MV_Camera);
	Ogre_MV_Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	Ogre_MV_SceneMgr->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	RenderListener = new Shapes_Render_Listener();
	App->CL_Ogre->mRoot->addFrameListener(RenderListener);

	//App->CL_X_CreateBoxDialog->Test_Cube = App->CL_X_BrushTemplate->BrushTemplate_CreateBox(App->CL_X_CreateBoxDialog->pBoxTemplate);
	//CreateNewTemplateBrush(Test_Cube);

	OGL_RenderListener = new OGL_Listener();
	Ogre_MV_SceneMgr->addRenderQueueListener(OGL_RenderListener);

	//App->CL_Ogre->OGL_Listener->flag_Show_Selected_Brush = true;

	// Create manual object and attach it to the scene
	OGL_Obj = Ogre_MV_SceneMgr->createManualObject("3D_Area");
	OGL_Node = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode();
	OGL_Node->attachObject(OGL_Obj);

	//App->CL_MeshViewer->flag_MV_Render_Debug = 1;
}

// *************************************************************************
// *		Close_MeshWindow:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void Shapes_3D::Close_OgreWindow(void)
{
	//App->CL_MeshViewer->flag_MV_Render_Debug = 0;

	App->CL_Ogre->mRoot->detachRenderTarget("MeshViewWin");
	Ogre_MV_Window->destroy();
	App->CL_Ogre->mRoot->destroySceneManager(Ogre_MV_SceneMgr);

	App->CL_Ogre->mRoot->removeFrameListener(RenderListener);

	delete RenderListener;
	RenderListener = nullptr;

	delete OGL_RenderListener;
	OGL_RenderListener = nullptr;
}

// *************************************************************************
// *				Set_Camera:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void Shapes_3D::Set_Camera(float Zoom)
{
	OGL_RenderListener->RX = 0;
	OGL_RenderListener->RZ = 0;

	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, Zoom));
}

// *************************************************************************
// *		Proc_Box_Viewer_3D:- Terry and Hazel Flanigan 2025 			   *
// *************************************************************************
LRESULT CALLBACK Shapes_3D::Proc_Box_Viewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
		SetFocus(App->CL_X_Shapes_3D->Render_hWnd);
		break;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		//if (App->flag_3D_Started == true)
		{
			POINT cursorPosition;
			GetCursorPos(&cursorPosition);
			App->CL_X_Shapes_3D->CursorPosX = cursorPosition.x;
			App->CL_X_Shapes_3D->CursorPosY = cursorPosition.y;

			auto& listener = App->CL_X_Shapes_3D->RenderListener;
			listener->Pl_Cent500X = cursorPosition.x;
			listener->Pl_Cent500Y = cursorPosition.y;

			SetCapture(App->CL_X_Shapes_3D->Render_hWnd);
			SetCursorPos(cursorPosition.x, cursorPosition.y);

			App->CL_X_Shapes_3D->RenderListener->flag_Pl_RightMouseDown = true;
			App->CUR = SetCursor(NULL);

			return 1;
		}

		return 1;
	}
	case WM_RBUTTONUP:
	{
		//if (App->flag_3D_Started == true)
		{
			ReleaseCapture();
			App->CL_X_Shapes_3D->RenderListener->flag_Pl_RightMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}

		return 1;
	}
	// Left Mouse Button
	case WM_LBUTTONDOWN:
	{
		//if (App->flag_3D_Started == true)
		{
			POINT p;
			GetCursorPos(&p);

			App->CL_X_Shapes_3D->CursorPosX = p.x;
			App->CL_X_Shapes_3D->CursorPosY = p.y;

			auto& listener = App->CL_X_Shapes_3D->RenderListener;
			listener->Pl_Cent500X = p.x;
			listener->Pl_Cent500Y = p.y;

			SetCapture(App->CL_X_Shapes_3D->Render_hWnd);

			SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);

			App->CL_X_Shapes_3D->RenderListener->flag_Pl_LeftMouseDown = 1;

			App->CUR = SetCursor(NULL);

			return 1;
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		//if (App->flag_3D_Started == true)
		{
			ReleaseCapture();
			App->CL_X_Shapes_3D->RenderListener->flag_Pl_LeftMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}

		return 1;
	}

	}

	return FALSE;
}
