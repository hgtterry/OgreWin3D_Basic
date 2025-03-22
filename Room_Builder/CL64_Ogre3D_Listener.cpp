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
	mMoveSensitivityMouse = 50;

	flag_LeftMouseDown = 0;
	flag_RightMouseDown = 0;

	Pl_Cent500X = App->CursorPosX;
	Pl_Cent500Y = App->CursorPosY;

	Pl_DeltaMouse = 0;
	Pl_MouseX = 0;
	Pl_MouseY = 0;

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
	Ogre::ImGuiOverlay::NewFrame();
	App->CL_ImGui->ImGui_Render_Loop();

	if (CameraMode == Enums::Cam_Mode_None)
	{
		return 1;
	}

	if (CameraMode == Enums::Cam_Mode_Model)
	{
		Mode_Camera_Model(evt.timeSinceLastFrame);
	}

	if (CameraMode == Enums::Cam_Mode_Free)
	{
		Camera_Mode_Free(evt.timeSinceLastFrame);

		return 1;
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
	if (flag_LeftMouseDown == 1 && flag_RightMouseDown == 0)
	{
		Capture_LeftMouse_Model();
	}

	// Right Mouse
	if (flag_LeftMouseDown == 0 && flag_RightMouseDown == 1)
	{
		Capture_RightMouse_Model();
	}

	MoveCamera();
}

// *************************************************************************
// *		Capture_LeftMouse_Model:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Ogre3D_Listener::Capture_LeftMouse_Model(void)
{
	GetCursorPos(&Mouse_point);

	Pl_MouseX = (int(Mouse_point.x));
	Pl_MouseY = (int(Mouse_point.y));

	//// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			App->CL_Grid->GridNode->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			App->CL_Grid->HairNode->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			App->CL_Grid->GridNode->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			App->CL_Grid->HairNode->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	// Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);
			App->CL_Grid->GridNode->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			App->CL_Grid->HairNode->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
		
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
			App->CL_Grid->GridNode->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			App->CL_Grid->HairNode->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
}

// *************************************************************************
// *	Capture_RightMouse_Model:- Terry and Hazel Flanigan 2025 		   *
// *************************************************************************
void CL64_Ogre3D_Listener::Capture_RightMouse_Model(void)
{
	GetCursorPos(&Mouse_point);

	Pl_MouseX = (int(Mouse_point.x));
	Pl_MouseY = (int(Mouse_point.y));

	// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			mTranslateVector.x = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			mTranslateVector.x = -Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	// Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);

			Ogre::Real Rate;
			Rate = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);

			Ogre::Vector3 OldPos;
			OldPos = mCamNode->getPosition();

			OldPos.y -= Rate;
			mCamNode->setPosition(OldPos);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}

	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);

			Ogre::Real Rate;
			Rate = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);

			Ogre::Vector3 OldPos;
			OldPos = mCamNode->getPosition();

			OldPos.y += Rate;
			mCamNode->setPosition(OldPos);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}

	}
}

// *************************************************************************
// *		Camera_Mode_Free:- Terry and Hazel Flanigan 2025   			   *
// *************************************************************************
void CL64_Ogre3D_Listener::Camera_Mode_Free(float DeltaTime)
{
	mRotX = 0;
	mRotY = 0;
	mTranslateVector = Ogre::Vector3::ZERO;

	mMoveScale = mMoveSensitivity * DeltaTime;

	App->CL_Keyboard->Keyboard_Mode_Free(DeltaTime);

	if (flag_LeftMouseDown == 1 && flag_RightMouseDown == 0)
	{
		Capture_Left_Mouse_Free();
		SetCursorPos(App->CursorPosX, App->CursorPosY);
	}

	// Right Mouse
	if (flag_LeftMouseDown == 0 && flag_RightMouseDown == 1)
	{
		Capture_Right_Mouse_Free();
	}

	MoveCamera();
}

// *************************************************************************
// *		Capture_Left_Mouse_Free:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Ogre3D_Listener::Capture_Left_Mouse_Free(void)
{
	GetCursorPos(&Mouse_point);

	Pl_MouseX = (int(Mouse_point.x));
	Pl_MouseY = (int(Mouse_point.y));

	// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive
		if (test > 1)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			mRotX = Degree(Pl_DeltaMouse * (float)0.08);//S_Player[0]->TurnRate);
			//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 1)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			mRotX = Degree(-Pl_DeltaMouse * (float)0.08);//S_Player[0]->TurnRate);
			//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		}
	}

	// Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Positive

		if (test > 1)
		{
			Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);
			mRotY = Degree(Pl_DeltaMouse * (float)0.08);//S_Player[0]->TurnRate);
			//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		}
	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 1)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
			mRotY = Degree(-Pl_DeltaMouse * (float)0.08);//S_Player[0]->TurnRate);
			//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
		}
	}

}

// *************************************************************************
// *		Capture_Right_Mouse_Free:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Ogre3D_Listener::Capture_Right_Mouse_Free(void)
{
	GetCursorPos(&Mouse_point);

	Pl_MouseX = (int(Mouse_point.x));
	Pl_MouseY = (int(Mouse_point.y));

	//// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			mTranslateVector.x = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			mTranslateVector.x = -Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	//// Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);

			Ogre::Real Rate;
			Rate = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);

			Ogre::Vector3 OldPos;
			OldPos = mCamNode->getPosition();

			OldPos.y -= Rate;
			mCamNode->setPosition(OldPos);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}

	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);

			Ogre::Real Rate;
			Rate = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);

			Ogre::Vector3 OldPos;
			OldPos = mCamNode->getPosition();

			OldPos.y += Rate;
			mCamNode->setPosition(OldPos);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}

	}
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


