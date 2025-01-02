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
#include "CL64_Ogre3D_Listener.h"

CL64_Ogre3D_Listener::CL64_Ogre3D_Listener()
{
	mCamNode = App->CL_Ogre->camNode;

	Wheel = 0;
	mMoveSensitivity = 50;
	mTranslateVector = Ogre::Vector3::ZERO;
	mMoveScale = 0;

	mRotX = 0;
	mRotY = 0;

	CameraMode = Enums::Cam_Mode_Model;

	flag_StopOgre = 0;
}

CL64_Ogre3D_Listener::~CL64_Ogre3D_Listener()
{
}

// *************************************************************************
// *			frameStarted:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Ogre3D_Listener::frameStarted(const FrameEvent& evt)
{

	return true;
}

// *************************************************************************
// *		frameRenderingQueued:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Ogre3D_Listener::frameRenderingQueued(const FrameEvent& evt)
{
	if (CameraMode == Enums::Cam_Mode_None)
	{
		return 1;
	}

	if (CameraMode == Enums::Cam_Mode_Model)
	{
		Mode_Camera_Model(evt.timeSinceLastFrame);
	}

	return 1;
}

// *************************************************************************
// *			frameEnded:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_Ogre3D_Listener::frameEnded(const FrameEvent& evt)
{
	if (flag_StopOgre == 1)
	{
		return false;
	}

	return true;
}

// *************************************************************************
// *		Mode_Camera_Model:- Terry and Hazel Flanigan 2025  			   *
// *************************************************************************
void CL64_Ogre3D_Listener::Mode_Camera_Model(float DeltaTime)
{
	mRotX = 0;
	mRotY = 0;

	mTranslateVector = Ogre::Vector3::ZERO;

	mMoveScale = mMoveSensitivity * DeltaTime;

	App->CL_Keyboard->Keyboard_Mode_Model(DeltaTime);

	// Left Mouse
	/*if (flag_LeftMouseDown == 1 && flag_RightMouseDown == 0)
	{
		Capture_LeftMouse_Model();
	}*/

	// Right Mouse
	//if (flag_LeftMouseDown == 0 && flag_RightMouseDown == 1)
	//{
	//	//Capture_RightMouse_Model();
	//}

	MoveCamera();
}

// *************************************************************************
// *			MoveCamera:- Terry and Hazel Flanigan 2025 				   *
// *************************************************************************
void CL64_Ogre3D_Listener::MoveCamera(void)
{
	mCamNode->yaw(mRotX, Ogre::Node::TS_PARENT);
	mCamNode->pitch(mRotY);
	mCamNode->translate(mTranslateVector, Ogre::Node::TS_LOCAL); // Position Relative
	Wheel = 0;
}
