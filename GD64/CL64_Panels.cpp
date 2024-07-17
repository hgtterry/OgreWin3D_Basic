/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Panels.h"

CL64_Panels::CL64_Panels(void)
{
}

CL64_Panels::~CL64_Panels(void)
{
}

// *************************************************************************
// *				Resize_Fldg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Panels::Resize_Fldg(void)
{
	RECT rcl;

	int WidthClient = 0;
	int HeightClient;
	int NewWidth = 0;
	int NewHeight = 0;

	GetClientRect(App->MainHwnd, &rcl);

	WidthClient = rcl.right - rcl.left - 1010;
	NewWidth = 417 + WidthClient + 200;

	HeightClient = rcl.bottom - rcl.top;
	NewHeight = HeightClient - 150;

	////-----------------Ogre Window
	SetWindowPos(App->Fdlg, NULL, 2, 80, rcl.right-4, NewHeight + 65, SWP_NOZORDER);

}

// *************************************************************************
// *			Resize_TopDlg:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Panels::Resize_TopDlg(void)
{
	RECT rcl;

	GetClientRect(App->MainHwnd, &rcl);

	SetWindowPos(App->CL_TopDlg->TabsHwnd, NULL, 2,2, rcl.right-4, 76, SWP_NOZORDER);

}

// *************************************************************************
// *			Resize_OgreWin:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Panels::Resize_OgreWin(void)
{
	RECT rcl;

	int WidthClient = 0;
	int HeightClient;
	int NewWidth = 0;
	int NewHeight = 0;

	GetClientRect(App->MainHwnd, &rcl);

	WidthClient = rcl.right - rcl.left - 1010;
	NewWidth = 417 + WidthClient + 200;

	HeightClient = rcl.bottom - rcl.top;
	NewHeight = HeightClient - 150;

	//-----------------Ogre Window
	SetWindowPos(App->ViewGLhWnd, NULL, 4, 2, NewWidth + 380, NewHeight + 58, SWP_NOZORDER);

	if (App->flag_OgreStarted == 1)
	{
		RECT rect;
		GetClientRect(App->ViewGLhWnd, &rect);

		if ((rect.bottom - rect.top) != 0 && App->CL_Ogre->mCamera != 0)
		{
			App->CL_Ogre->mWindow->windowMovedOrResized();
			App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());
			App->CL_Ogre->camNode->yaw(Radian(0));
		}

	}
}

