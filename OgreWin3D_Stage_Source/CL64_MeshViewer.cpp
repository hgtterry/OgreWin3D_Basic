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
#include "CL64_MeshViewer.h"

CL64_MeshViewer::CL64_MeshViewer(void)
{
	MainDlgHwnd =			nullptr;
	MeshViewer_3D_hWnd =	nullptr;
	ListHwnd =				nullptr;
	CB_hWnd =				nullptr;

	// Ogre
	Ogre_MV_Window =		nullptr;
	Ogre_MV_SceneMgr =		nullptr;
	Ogre_MV_Camera =		nullptr;
	Ogre_MV_CamNode =		nullptr;
	Ogre_MvEnt =			nullptr;
	Ogre_MvNode =			nullptr;

	MV_Resource_Group = "MV_Resource_Group";

	m_Just_Folder[0] = 0;
	m_Resource_Folder_Full[0] = 0;
	Selected_MeshFile[0] = 0;

}

CL64_MeshViewer::~CL64_MeshViewer(void)
{
}

// *************************************************************************
// *	  	Start_MeshViewer_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Start_MeshViewer_Dlg()
{
	CreateDialog(App->hInst, (LPCTSTR)IDD_MESHVIEWER, App->Fdlg, (DLGPROC)Proc_MeshViewer_Dlg);
	
}

// *************************************************************************
// *			Proc_MeshViewer:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_MeshViewer::Proc_MeshViewer_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_FOLDERS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LISTFILES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SELECTEDNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_MeshViewer->MainDlgHwnd = hDlg;
		App->CL_MeshViewer->ListHwnd = GetDlgItem(hDlg, IDC_LISTFILES);
		App->CL_MeshViewer->CB_hWnd = GetDlgItem(hDlg, IDC_CB_FOLDERS);

		App->CL_MeshViewer->MeshViewer_3D_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MESHVIEWER_3D, hDlg, (DLGPROC)Proc_MeshViewer_3D);
		App->CL_MeshViewer->Set_OgreWindow();

		App->CL_MeshViewer->Get_Stock_Folders(App->CL_MeshViewer->CB_hWnd);
		App->CL_MeshViewer->Add_Resources();
		App->CL_MeshViewer->Get_Mesh_Files();

		App->CL_MeshViewer->Show_Mesh(App->CL_MeshViewer->Selected_MeshFile);
		
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_SELECTEDNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}
		return FALSE;
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

		if (some_item->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_MeshViewer->Close_OgreWindow();
			App->CL_MeshViewer->Delete_Resources_Group();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_MeshViewer->Close_OgreWindow();
			App->CL_MeshViewer->Delete_Resources_Group();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	return FALSE;
}

// *************************************************************************
// *		Close_MeshWindow:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_MeshViewer::Close_OgreWindow(void)
{
	//App->CL_Ogre->mRoot->removeFrameListener(RenderListener);

	App->CL_Ogre->mRoot->detachRenderTarget("MeshViewWin");
	Ogre_MV_Window->destroy();
	App->CL_Ogre->mRoot->destroySceneManager(Ogre_MV_SceneMgr);
}

// *************************************************************************
// *			OgreWindow:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
bool CL64_MeshViewer::Set_OgreWindow(void)
{

	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)MeshViewer_3D_hWnd);

	Ogre_MV_Window = App->CL_Ogre->mRoot->createRenderWindow("MeshViewWin", 1024, 768, false, &options);

	Ogre_MV_SceneMgr = App->CL_Ogre->mRoot->createSceneManager("DefaultSceneManager", "MeshViewGD");

	Ogre_MV_CamNode = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode("Camera_Node");

	Ogre_MV_Camera = Ogre_MV_SceneMgr->createCamera("CameraMV");
	Ogre_MV_Camera->setNearClipDistance(0.1);
	Ogre_MV_Camera->setFarClipDistance(8000);

	Ogre_MV_CamNode->attachObject(Ogre_MV_Camera);
	Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, 0));

	Ogre::Viewport* vp = Ogre_MV_Window->addViewport(Ogre_MV_Camera);
	Ogre_MV_Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	Ogre_MV_SceneMgr->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	//App->CL_Ogre->RenderFrame(20);
	
	////-------------------------------------------- 

	Ogre_MvEnt = Ogre_MV_SceneMgr->createEntity("MVTest2", "Sinbad.mesh", App->CL_Ogre->App_Resource_Group);
	Ogre_MvNode = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre_MvNode->attachObject(Ogre_MvEnt);
	Ogre_MvNode->setVisible(true);

	// add a bright light above the scene
	/*Light* light = mSceneMgrMeshView->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-10, 40, 20);
	light->setSpecularColour(ColourValue::White);*/

	Ogre::Vector3 Centre = Ogre_MvEnt->getBoundingBox().getCenter();
	Ogre::Real Radius = Ogre_MvEnt->getBoundingRadius();

	Ogre_MV_CamNode->setPosition(0, Centre.y, Radius * 2.5);
	Ogre_MV_CamNode->lookAt(Ogre::Vector3(0, Centre.y, 0), Ogre::Node::TS_WORLD);
	
	//Grid_Update(1);

	/*RenderListener = new SB_MeshView_Listener();

	App->CL_Ogre->mRoot->addFrameListener(RenderListener);*/

	//Reset_Camera();

	// Debug Physics Shape
	/*btDebug_Manual = mSceneMgrMeshView->createManualObject("MVManual");
	btDebug_Manual->setRenderQueueGroup(RENDER_QUEUE_MAX);
	btDebug_Manual->setDynamic(true);
	btDebug_Manual->estimateVertexCount(2000);
	btDebug_Manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1, 1);
	btDebug_Manual->end();
	btDebug_Node = mSceneMgrMeshView->getRootSceneNode()->createChildSceneNode();
	btDebug_Node->attachObject(btDebug_Manual);*/

	return 1;
}

// *************************************************************************
// *		Proc_MeshViewer_3D:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
LRESULT CALLBACK CL64_MeshViewer::Proc_MeshViewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG: // Bernie as the dialog is created
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		//if (App->flag_OgreStarted == 0)
		{
			return (LONG)App->BlackBrush;
		}
	}

	//case WM_MOUSEWHEEL:
	//{
	//	if (App->SBC_MeshViewer->RenderListener->Pl_LeftMouseDown == 0)
	//	{
	//		{
	//			int zDelta = (short)HIWORD(wParam);    // wheel rotation

	//			if (zDelta > 0)
	//			{
	//				App->SBC_MeshViewer->RenderListener->Wheel_Move = -1;
	//			}
	//			else if (zDelta < 0)
	//			{
	//				App->SBC_MeshViewer->RenderListener->Wheel_Move = 1;
	//			}
	//			return 1;
	//		}
	//	}

	//}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		//SetFocus(App->SBC_MeshViewer->MeshView_3D_hWnd);

		break;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		//if (App->OgreStarted == 1)
		//{
		//	SetCapture(App->SBC_MeshViewer->MeshView_3D_hWnd);// Bernie
		//	SetCursorPos(App->CursorPosX, App->CursorPosY);
		//	App->SBC_MeshViewer->RenderListener->Pl_RightMouseDown = 1;
		//	App->CUR = SetCursor(NULL);
		//	return 1;
		//}

		return 1;
	}
	case WM_RBUTTONUP:
	{
		/*if (App->OgreStarted == 1)
		{
			ReleaseCapture();
			App->SBC_MeshViewer->RenderListener->Pl_RightMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}*/

		return 1;
	}
	// Left Mouse Button
	case WM_LBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		//if (App->OgreStarted == 1)
		//{

		//	SetCapture(App->SBC_MeshViewer->MeshView_3D_hWnd);// Bernie
		//	SetCursorPos(App->CursorPosX, App->CursorPosY);

		//	App->SBC_MeshViewer->RenderListener->Pl_LeftMouseDown = 1;

		//	App->CUR = SetCursor(NULL);

		//	return 1;
		//}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		/*if (App->OgreStarted == 1)
		{
			ReleaseCapture();
			App->SBC_MeshViewer->RenderListener->Pl_LeftMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}*/

		return 1;
	}
	
	}

	return FALSE;
}

// *************************************************************************
// *			Add_Resources:- Terry and Hazel Flanigan 2024 		 	   *
// *************************************************************************
bool CL64_MeshViewer::Add_Resources()
{
	bool Test = Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(m_Resource_Folder_Full, MV_Resource_Group);

	if (Test == 0)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(m_Resource_Folder_Full, "FileSystem", MV_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(MV_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(MV_Resource_Group);

	}

	App->CL_Resources->mSelected_Resource_Group = "MV_Resource_Group";

	return 1;
}

// *************************************************************************
// *		Delete_Resources_Group:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
bool CL64_MeshViewer::Delete_Resources_Group()
{

	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(MV_Resource_Group);

	App->CL_Resources->mSelected_Resource_Group = "App_Resource_Group";
	return 1;
}

// *************************************************************************
// *			Get_Stock_Folders:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_MeshViewer::Get_Stock_Folders(HWND DropHwnd)
{
	char Path[MAX_PATH]{ 0 };

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(Path, App->GD_Directory_FullPath);
	strcat(Path, "\\Stock\\*.*");

	hFind = FindFirstFile(Path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				{

				}
				else
				{
					SendMessage(DropHwnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)FindFileData.cFileName);
				}

			}
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}

	SendMessage(DropHwnd, CB_SETCURSEL, 0, 0);

	int Index = SendMessage(DropHwnd, CB_GETCURSEL, 0, 0);
	SendMessage(CB_hWnd, CB_GETLBTEXT, Index, (LPARAM)m_Just_Folder);

	strcpy(m_Resource_Folder_Full, App->GD_Directory_FullPath);
	strcat(m_Resource_Folder_Full, "\\Stock\\");
	strcat(m_Resource_Folder_Full, m_Just_Folder);
	strcat(m_Resource_Folder_Full, "\\");

}

// *************************************************************************
// *			Get_Mesh_Files:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_MeshViewer::Get_Mesh_Files()
{
	SendMessage(ListHwnd, LB_RESETCONTENT, 0, 0);

	char Path[MAX_PATH]{ 0 };

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(Path, m_Resource_Folder_Full);
	strcat(Path, "*.*");

	hFind = FindFirstFile(Path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.dwFileAttributes)
			{
				if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				{

				}
				else
				{
					if (_stricmp(FindFileData.cFileName + strlen(FindFileData.cFileName) - 5, ".mesh") == 0)
					{
						SendMessage(ListHwnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)FindFileData.cFileName);
					}
				}

			}
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}

	char buff[MAX_PATH]{ 0 };
	SendDlgItemMessage(MainDlgHwnd, IDC_LISTFILES, LB_GETTEXT, (WPARAM)0, (LPARAM)buff);
	SetDlgItemText(MainDlgHwnd, IDC_SELECTEDNAME, buff);

	strcpy(Selected_MeshFile, buff);

	//App->SBC_MeshViewer->Update_Mesh(App->SBC_MeshViewer->Selected_MeshFile);
}

// *************************************************************************
// *			Update_Mesh:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void CL64_MeshViewer::Show_Mesh(char* MeshFile)
{
	if (Ogre_MvEnt && Ogre_MvNode)
	{
		Ogre_MvNode->detachAllObjects();
		Ogre_MV_SceneMgr->destroySceneNode(Ogre_MvNode);
		Ogre_MV_SceneMgr->destroyEntity(Ogre_MvEnt);
		Ogre_MvEnt = NULL;
		Ogre_MvNode = NULL;
	}

	Ogre_MvEnt = Ogre_MV_SceneMgr->createEntity("MV", MeshFile, MV_Resource_Group);
	Ogre_MvNode = Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre_MvNode->attachObject(Ogre_MvEnt);
	Ogre_MvNode->setPosition(0, 0, 0);

	//if (App->SBC_MeshViewer->View_Zoomed_Flag == 1)
	{
		Ogre::Vector3 Centre = Ogre_MvEnt->getBoundingBox().getCenter();
		Ogre::Real Radius = Ogre_MvEnt->getBoundingRadius();

		Ogre_MV_CamNode->setPosition(0, Centre.y, Radius * 2.5);
		Ogre_MV_CamNode->lookAt(Ogre::Vector3(0, Centre.y, 0), Ogre::Node::TS_WORLD);
	}

}
