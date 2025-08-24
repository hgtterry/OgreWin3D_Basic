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

	flag_LeftMouseDown = false;
	flag_RightMouseDown = false;

	Pl_Cent500X = App->CursorPosX;
	Pl_Cent500Y = App->CursorPosY;

	Pl_DeltaMouse = 0;
	Pl_MouseX = 0;
	Pl_MouseY = 0;

	mRotX = 0;
	mRotY = 0;

	CameraMode = Enums::Cam_Mode_Model;

	flag_StopOgre = false;
	flag_Run_Physics = false;
}

CL64_Ogre3D_Listener::~CL64_Ogre3D_Listener()
{
}

// *************************************************************************
// *			frameStarted:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Ogre3D_Listener::frameStarted(const FrameEvent& evt)
{
	Update_Game_Logic(evt.timeSinceLastFrame);

	return true;
}

// *************************************************************************
// *		frameRenderingQueued:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_Ogre3D_Listener::frameRenderingQueued(const FrameEvent& evt)
{
	Ogre::ImGuiOverlay::NewFrame();
	App->CL_ImGui->ImGui_Render_Loop();

	
	if (App->CL_Physics->flag_TriMesh_Created == 1)
	{
		App->CL_Physics->dynamicsWorld->debugDrawWorld();
	}


	if (CameraMode == Enums::Cam_Mode_None)
	{
		return 1;
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

		App->CL_Com_Player->Update_Player(NULL, evt.timeSinceLastFrame);

		if (flag_LeftMouseDown == 1 && flag_RightMouseDown == 0)
		{
			Capture_Mouse_FirstPerson(evt.timeSinceLastFrame);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}

		return 1;
	}

	if (CameraMode == Enums::Cam_Mode_Model)
	{
		Camera_Mode_Model(evt.timeSinceLastFrame);
		return 1;
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
// *			Update_Game_Logic:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Ogre3D_Listener::Update_Game_Logic(float DeltaTime)
{
	/*Get_View_Height_Width();

	int Count = 0;
	while (Count < App->CL_Scene->Counters_Count)
	{
		if (App->CL_Scene->B_Counter)
		{
			if (App->CL_Scene->B_Counter[Count]->flag_Show_Panel_Flag == 1)
			{
				App->CL_Scene->B_Counter[Count]->Render_ImGui_Panel();
			}
		}

		Count++;
	}*/

	/*Count = 0;
	while (Count < App->CL_Scene->Object_Count)
	{
		if (App->CL_Scene->B_Object[Count]->Usage == Enums::Obj_Usage_Message)
		{
			if (App->CL_Scene->B_Object[Count]->flag_Show_Message_Flag == 1)
			{
				App->CL_Scene->B_Object[Count]->Render_ImGui_Panel();
			}
		}

		Count++;
	}*/

	//Count = 0;

	if (flag_Run_Physics == 1 && App->flag_OgreStarted == 1)
	{
		App->CL_Physics->dynamicsWorld->stepSimulation(DeltaTime * 2);// Bullet_Step);

		for (int j = App->CL_Physics->dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{

			btCollisionObject* obj = App->CL_Physics->dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;

			if (body && body->getMotionState()) //&& Block == 0)
			{
				int UI = body->getUserIndex();
				int Index = body->getUserIndex2();

				if (UI == Enums::Obj_Usage_Dynamic)
				{

					body->getMotionState()->getWorldTransform(trans);
					btQuaternion orientation = trans.getRotation();

					float x = obj->getWorldTransform().getOrigin().getX();
					float y = obj->getWorldTransform().getOrigin().getY();
					float z = obj->getWorldTransform().getOrigin().getZ();

					if (Index > -1)
					{
						if (App->CL_Scene->B_Object[Index]->Object_Node)
						{
							App->CL_Scene->B_Object[Index]->Object_Node->setPosition(x, y, z);
							App->CL_Scene->B_Object[Index]->Object_Node->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

							Ogre::Vector3 WC = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
							Ogre::Vector3 NewPos = Ogre::Vector3(x, y, z) - WC;

							App->CL_Scene->B_Object[Index]->Object_Node->setPosition((Ogre::Vector3(x, y, z)) + NewPos);
						}
					}
				}
			}
		}
	}

	if (flag_Run_Physics == 1 && App->CL_Scene->flag_Player_Added == 1)
	{
		btTransform trans;
		App->CL_Scene->B_Player[0]->Phys_Body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();

		App->CL_Scene->B_Player[0]->Player_Node->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY() + 2, trans.getOrigin().getZ()));
		App->CL_Scene->B_Player[0]->Player_Node->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
		App->CL_Scene->B_Player[0]->Player_Node->pitch(Ogre::Degree(180));
	}
}

// *************************************************************************
// *	Capture_Mouse_FirstPerson_World:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
void CL64_Ogre3D_Listener::Capture_Mouse_FirstPerson(float DeltaTime)
{
	/*if (flag_Block_Mouse == 1)
	{
		return 0;
	}*/

	GetCursorPos(&Mouse_point);

	Pl_MouseX = (int(Mouse_point.x));
	Pl_MouseY = (int(Mouse_point.y));

	auto& m_Player = App->CL_Scene->B_Player[0];

	// Check Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Rotate Left
		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);

			float Delta2 = DeltaTime * 150;
			float mTurn = (m_Player->TurnRate * Pl_DeltaMouse) * Delta2;

			m_Player->Rotate_FromCam(Ogre::Vector3(0, -1, 0), mTurn, false);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Rotate Right

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);

			float Delta2 = DeltaTime * 150;
			float mTurn = (m_Player->TurnRate * Pl_DeltaMouse) * Delta2;

			m_Player->Rotate_FromCam(Ogre::Vector3(0, 1, 0), mTurn, false);
		}
	}

	// Check Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Look Up

		if (test > 1)
		{
			float Limit = m_Player->CameraPitch_Node->getOrientation().getPitch().valueDegrees();

			if (Limit < m_Player->Limit_Look_Up)
			{
				m_Player->CameraPitch_Node->setOrientation(Ogre::Quaternion::IDENTITY);
				Ogre::Radian pi = Degree(m_Player->Limit_Look_Up);
				m_Player->CameraPitch_Node->pitch(pi);
			}
			else
			{
				Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);
				Ogre::Radian pp = Degree(-Pl_DeltaMouse * DeltaTime) * 1;
				m_Player->CameraPitch_Node->pitch(pp);
			}
		}
	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Look Down

		if (test > 1)
		{
			float Limit = m_Player->CameraPitch_Node->getOrientation().getPitch().valueDegrees();

			if (Limit > m_Player->Limit_Look_Down)
			{
				m_Player->CameraPitch_Node->setOrientation(Ogre::Quaternion::IDENTITY);
				Ogre::Radian pi = Degree(m_Player->Limit_Look_Down);
				m_Player->CameraPitch_Node->pitch(pi);
			}
			else
			{
				Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
				Ogre::Radian pp = Degree(Pl_DeltaMouse * DeltaTime) * 1;
				m_Player->CameraPitch_Node->pitch(pp);
			}
		}
	}
}

// *************************************************************************
// *		Camera_Mode_Model:- Terry and Hazel Flanigan 2025  			   *
// *************************************************************************
void CL64_Ogre3D_Listener::Camera_Mode_Model(float DeltaTime)
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

	Pl_MouseX = static_cast<int>(Mouse_point.x);
	Pl_MouseY = static_cast<int>(Mouse_point.y);

	// Calculate mouse movement delta
	auto calculateMouseDelta = [&](int mousePos, int centerPos)
		{
			return static_cast<float>(mousePos - centerPos);
		};

	// Handle horizontal movement[Left Right]
	if (std::abs(Pl_MouseX - Pl_Cent500X) > 2)
	{
		Pl_DeltaMouse = calculateMouseDelta(Pl_MouseX, Pl_Cent500X);
		float yawAmount = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2;

		App->CL_Gizmos->Grid_Node->yaw(Ogre::Degree(yawAmount), Ogre::Node::TS_LOCAL);
		App->CL_Gizmos->Hair_Node->yaw(Ogre::Degree(yawAmount), Ogre::Node::TS_LOCAL);

		SetCursorPos(App->CursorPosX, App->CursorPosY);
	}

	// Handle vertical movement [ Up Down ]
	if (std::abs(Pl_MouseY - Pl_Cent500Y) > 2)
	{
		Pl_DeltaMouse = calculateMouseDelta(Pl_MouseY, Pl_Cent500Y);
		float pitchAmount = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2;

		App->CL_Gizmos->Grid_Node->pitch(Ogre::Degree(pitchAmount), Ogre::Node::TS_PARENT);
		App->CL_Gizmos->Hair_Node->pitch(Ogre::Degree(pitchAmount), Ogre::Node::TS_PARENT);

		SetCursorPos(App->CursorPosX, App->CursorPosY);
	}
}

// *************************************************************************
// *	Capture_RightMouse_Model:- Terry and Hazel Flanigan 2025 		   *
// *************************************************************************
void CL64_Ogre3D_Listener::Capture_RightMouse_Model(void)
{
	GetCursorPos(&Mouse_point);

	Pl_MouseX = static_cast<int>(Mouse_point.x);
	Pl_MouseY = static_cast<int>(Mouse_point.y);

	// Calculate mouse movement delta
	auto calculateMouseDelta = [&](int mousePos, int centerPos)
		{
			return static_cast<float>(mousePos - centerPos);
		};

	// Handle horizontal movement [ Left Right ]
	long delta = Pl_MouseX - Pl_Cent500X;

	if (std::abs(delta) > 2) // Check if the movement is significant
	{
		Pl_DeltaMouse = static_cast<float>(std::abs(delta));
		mTranslateVector.x = (delta < 0 ? Pl_DeltaMouse : -Pl_DeltaMouse) * (mMoveSensitivityMouse / 1000);
		SetCursorPos(App->CursorPosX, App->CursorPosY);
	}

	// Handle vertical movement [ Up Down ]
	delta = Pl_Cent500Y - Pl_MouseY;

	if (std::abs(delta) > 2) // Check if the movement is significant
	{
		Pl_DeltaMouse = static_cast<float>(std::abs(delta));
		Ogre::Real rate = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
		Ogre::Vector3 oldPos = mCamNode->getPosition();

		oldPos.y += (delta < 0 ? rate : -rate); // Adjust camera position based on mouse movement
		mCamNode->setPosition(oldPos);
		SetCursorPos(App->CursorPosX, App->CursorPosY);
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
// *			MoveCamera:- Terry and Hazel Flanigan 2025 				   *
// *************************************************************************
void CL64_Ogre3D_Listener::MoveCamera(void)
{
	mCamNode->yaw(mRotX, Ogre::Node::TS_PARENT);
	mCamNode->pitch(mRotY);
	mCamNode->translate(mTranslateVector, Ogre::Node::TS_LOCAL); // Position Relative
	Wheel = 0;
}


