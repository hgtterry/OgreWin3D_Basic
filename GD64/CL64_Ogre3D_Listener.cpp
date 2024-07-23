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
#include "CL64_Ogre3D_Listener.h"



CL64_Ogre3D_Listener::CL64_Ogre3D_Listener(void)
{
	mCam = App->CL_Ogre->mCamera;
	mCamNode = App->CL_Ogre->camNode;

	CameraMode = Enums::Cam_Mode_Model;

	mRotX = 0;;
	mRotY = 0;

	mTranslateVector = Ogre::Vector3::ZERO;
	
	mMoveScale = 0;
	mMoveSensitivity = 5;
	mMoveSensitivityMouse = 20;

	Pl_LeftMouseDown = 0;
	Pl_RightMouseDown = 0;

	Pl_Cent500X = App->CursorPosX;
	Pl_Cent500Y = App->CursorPosY;
	Pl_DeltaMouse = 0;
	Pl_MouseX = 0;
	Pl_MouseY = 0;

	View_Height = 0;
	View_Width = 0;

	Wheel = 0;
	StopOgre = 0;

	Ogre_Model_Loaded = 0;

	Run_Physics = 0;
	Bullet_Step = 2;
}

CL64_Ogre3D_Listener::~CL64_Ogre3D_Listener(void)
{
}

// *************************************************************************
// *			frameStarted:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre3D_Listener::frameStarted(const FrameEvent& evt)
{
	Get_View_Height_Width();

	if (Run_Physics == 1)
	{
		App->CL_Bullet->dynamicsWorld->stepSimulation(evt.timeSinceLastFrame * Bullet_Step); //suppose you have 60 frames per second
	}

	return true;
}

// *************************************************************************
// *		frameRenderingQueued:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre3D_Listener::frameRenderingQueued(const FrameEvent& evt)
{

	App->CL_Ogre->mTrayMgr->frameRendered(evt);

	Ogre::ImGuiOverlay::NewFrame();
	App->CL_ImGui->ImGui_Render_Loop();

	if (CameraMode == Enums::Cam_Mode_None)
	{
		return 1;
	}

	if (Run_Physics == 1 && App->CL_Scene->flag_Player_Added == 1)
	{
		btTransform trans;
		App->CL_Scene->B_Player[0]->Phys_Body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();

		App->CL_Scene->B_Player[0]->Player_Node->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		App->CL_Scene->B_Player[0]->Player_Node->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
		App->CL_Scene->B_Player[0]->Player_Node->pitch(Ogre::Degree(180));
	}
	
	if (CameraMode == Enums::Cam_Mode_First)
	{
		Ogre::Vector3 Pos;
		Ogre::Radian mmPitch;
		Ogre::Radian mYaw;

		Pos = App->CL_Scene->B_Player[0]->Player_Node->getPosition();

		mmPitch = App->CL_Scene->B_Player[0]->CameraPitch_Node->getOrientation().getPitch();
		mYaw = App->CL_Scene->B_Player[0]->Player_Node->getOrientation().getYaw();
		Pos.y = Pos.y + App->CL_Scene->B_Player[0]->PlayerHeight;

		App->CL_Ogre->camNode->setPosition(Pos);
		App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion(1, 0, 0, 0));
		App->CL_Ogre->camNode->yaw(mYaw);
		App->CL_Ogre->camNode->pitch(mmPitch);
		App->CL_Ogre->camNode->yaw(Ogre::Degree(180));

		App->CL_Keyboard->Keyboard_Mode_First(evt.timeSinceLastFrame);

		App->CL_Player->Update_Player(NULL, evt.timeSinceLastFrame);

		if (Pl_LeftMouseDown == 1 && Pl_RightMouseDown == 0)
		{
			Capture_Mouse_FirstPerson(evt.timeSinceLastFrame);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	
	if (CameraMode == Enums::Cam_Mode_Model)
	{
		Mode_Camera_Model(evt.timeSinceLastFrame);
	}
	
	if (CameraMode == Enums::Cam_Mode_Free)
	{
		Mode_Camera_Free(evt.timeSinceLastFrame);
	}
	
	App->CL_Bullet->dynamicsWorld->debugDrawWorld();

	return 1;
}

// *************************************************************************
// *			frameEnded:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Ogre3D_Listener::frameEnded(const FrameEvent& evt)
{

	if (StopOgre == 1)
	{
		return false;
	}

	return true;
}

// *************************************************************************
// *			MoveCamera:- Terry and Hazel Flanigan 2023 				   *
// *************************************************************************
void CL64_Ogre3D_Listener::MoveCamera(void)
{
	mCamNode->yaw(mRotX, Ogre::Node::TS_PARENT);
	mCamNode->pitch(mRotY);
	mCamNode->translate(mTranslateVector, Ogre::Node::TS_LOCAL); // Position Relative
	Wheel = 0;
}

// *************************************************************************
// *		Get_View_Height_Width:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Ogre3D_Listener::Get_View_Height_Width(void)
{
	Ogre::Viewport* vp = App->CL_Ogre->mWindow->getViewport(0);

	View_Width = vp->getActualWidth();
	View_Height = vp->getActualHeight();
}

// *************************************************************************
// *		Mode_Camera_Model:- Terry and Hazel Flanigan 2024  			   *
// *************************************************************************
void CL64_Ogre3D_Listener::Mode_Camera_Model(float DeltaTime)
{
	//App->CL_Ogre->m_imgui.render();

	mRotX = 0;
	mRotY = 0;

	mTranslateVector = Ogre::Vector3::ZERO;

	mMoveScale = mMoveSensitivity * DeltaTime;

	App->CL_Keyboard->Keyboard_Mode_Model(DeltaTime);

	// Left Mouse
	if (Pl_LeftMouseDown == 1 && Pl_RightMouseDown == 0)
	{
		Capture_LeftMouse_Model();
	}

	// Right Mouse
	if (Pl_LeftMouseDown == 0 && Pl_RightMouseDown == 1)
	{
		Capture_RightMouse_Model();
	}

	MoveCamera();
}

// *************************************************************************
// *		Capture_LeftMouse_Model:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre3D_Listener::Capture_LeftMouse_Model(void)
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
			App->CL_Ogre->OGL_Listener->RZ = App->CL_Ogre->OGL_Listener->RZ - (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);

			if (App->CL_Ogre->flag_TestMesh_Deleted == 0)
			{
				App->CL_Ogre->TestMesh_Node->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			}

			if (Ogre_Model_Loaded == 1)
			{
				App->CL_Scene->Imported_Ogre_Node->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			}

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
			App->CL_Ogre->OGL_Listener->RZ = App->CL_Ogre->OGL_Listener->RZ + (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);

			if (App->CL_Ogre->flag_TestMesh_Deleted == 0)
			{
				App->CL_Ogre->TestMesh_Node->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			}

			if (Ogre_Model_Loaded == 1)
			{
				App->CL_Scene->Imported_Ogre_Node->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			}

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
			App->CL_Ogre->OGL_Listener->RX = App->CL_Ogre->OGL_Listener->RX - (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);

			if (App->CL_Ogre->flag_TestMesh_Deleted == 0)
			{
				App->CL_Ogre->TestMesh_Node->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			}

			if (Ogre_Model_Loaded == 1)
			{
				App->CL_Scene->Imported_Ogre_Node->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			}

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
			App->CL_Ogre->OGL_Listener->RX = App->CL_Ogre->OGL_Listener->RX + (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);

			if (App->CL_Ogre->flag_TestMesh_Deleted == 0)
			{
				App->CL_Ogre->TestMesh_Node->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			}

			if (Ogre_Model_Loaded == 1)
			{
				App->CL_Scene->Imported_Ogre_Node->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			}

			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	return 1;
}

// *************************************************************************
// *	Capture_RightMouse_Model:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
bool CL64_Ogre3D_Listener::Capture_RightMouse_Model(void)
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

	return 1;
}

// *************************************************************************
// *		Mode_Camera_Free:- Terry and Hazel Flanigan 2023   			   *
// *************************************************************************
void CL64_Ogre3D_Listener::Mode_Camera_Free(float DeltaTime)
{
	mRotX = 0;
	mRotY = 0;
	mTranslateVector = Ogre::Vector3::ZERO;

	mMoveScale = mMoveSensitivity * DeltaTime;

	App->CL_Keyboard->Keyboard_Mode_Free(DeltaTime);

	if (Pl_LeftMouseDown == 1 && Pl_RightMouseDown == 0)
	{
		Capture_Left_Mouse_Free();
		SetCursorPos(App->CursorPosX, App->CursorPosY);
	}

	// Right Mouse
	if (Pl_LeftMouseDown == 0 && Pl_RightMouseDown == 1)
	{
		Capture_Right_Mouse_Free();
	}

	MoveCamera();
}

// *************************************************************************
// *		Capture_Left_Mouse_Free:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre3D_Listener::Capture_Left_Mouse_Free(void)
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
			mRotX = Degree(Pl_DeltaMouse * (float)0.03);//S_Player[0]->TurnRate);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			mRotX = Degree(-Pl_DeltaMouse * (float)0.03);//S_Player[0]->TurnRate);
		}
	}

	// Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);
			mRotY = Degree(Pl_DeltaMouse * (float)0.03);//S_Player[0]->TurnRate);
		}
	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
			mRotY = Degree(-Pl_DeltaMouse * (float)0.03);//S_Player[0]->TurnRate);
		}
	}
	
	return 1;
}

// *************************************************************************
// *		Capture_Right_Mouse_Free:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Ogre3D_Listener::Capture_Right_Mouse_Free(void)
{
	//if (CameraMode == Enums::CamDetached)
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

	return 1;
}

// *************************************************************************
// *	Capture_Mouse_FirstPerson_World:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_Ogre3D_Listener::Capture_Mouse_FirstPerson(float DeltaTime)
{
	/*if (Block_Mouse == 1)
	{
		return 0;
	}*/

	GetCursorPos(&Mouse_point);

	Pl_MouseX = (int(Mouse_point.x));
	Pl_MouseY = (int(Mouse_point.y));

	// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Rotate Left
		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);

			float Delta2 = DeltaTime * 150;
			float mTurn = (App->CL_Scene->B_Player[0]->TurnRate * Pl_DeltaMouse) * Delta2;

			App->CL_Scene->B_Player[0]->Rotate_FromCam(Ogre::Vector3(0, -1, 0), mTurn, false);

		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Rotate Right

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);

			float Delta2 = DeltaTime * 150;
			float mTurn = (App->CL_Scene->B_Player[0]->TurnRate * Pl_DeltaMouse) * Delta2;

			App->CL_Scene->B_Player[0]->Rotate_FromCam(Ogre::Vector3(0, 1, 0), mTurn, false);
		}
	}

	//Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Look Up

		if (test > 1)
		{
			if (App->CL_Scene->B_Player[0]->CameraPitch_Node->getOrientation().getPitch().valueDegrees() < App->CL_Scene->B_Player[0]->Limit_Look_Up)
			{

			}
			else
			{
				Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);
				Ogre::Radian pp = Degree(-Pl_DeltaMouse * DeltaTime) * 1;
				App->CL_Scene->B_Player[0]->CameraPitch_Node->pitch(pp);
			}
		}

	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Look Down

		if (test > 1)
		{
			if (App->CL_Scene->B_Player[0]->CameraPitch_Node->getOrientation().getPitch().valueDegrees() > App->CL_Scene->B_Player[0]->Limit_Look_Down)
			{

			}
			else
			{
				Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
				Ogre::Radian pp = Degree(Pl_DeltaMouse * DeltaTime) * 1;
				App->CL_Scene->B_Player[0]->CameraPitch_Node->pitch(pp);
			}
		}
	}

	return 1;
}

