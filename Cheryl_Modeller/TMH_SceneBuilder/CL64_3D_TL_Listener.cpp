#include "pch.h"
#include "CL64_App.h"
#include "CL64_3D_TL_Listener.h"

CL64_3D_TL_Listener::CL64_3D_TL_Listener(void)
{
	WE_Cam_Node = App->CL_3D_TL_View->Ogre_TL_CamNode;

	Wheel_Move = 0;
	mTranslateVector = Ogre::Vector3::ZERO;
	mMoveSensitivity = 50;
	mMoveScale = 0;

	mMoveSensitivityMouse = 50;

	flag_LeftMouseDown = false;
	flag_RightMouseDown = false;

	Pl_Cent500X = App->CL_3D_TL_View->CursorPosX;
	Pl_Cent500Y = App->CL_3D_TL_View->CursorPosY;

	Pl_DeltaMouse = 0;
	Pl_MouseX = 0;
	Pl_MouseY = 0;
}

CL64_3D_TL_Listener::~CL64_3D_TL_Listener(void)
{
}

// *************************************************************************
// *				frameStarted   Terry Bernie							   *
// *************************************************************************
bool CL64_3D_TL_Listener::frameStarted(const Ogre::FrameEvent& evt)
{
	return true;
}


// *************************************************************************
// *			frameRenderingQueued   Terry Bernie						   *
// *************************************************************************
bool CL64_3D_TL_Listener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	mTranslateVector = Ogre::Vector3::ZERO;
	mMoveScale = mMoveSensitivity * evt.timeSinceLastFrame;

	if (Wheel_Move < 0) // Mouse Wheel Forward
	{
		mTranslateVector.z = -mMoveScale * 2;
	}

	// back
	if (Wheel_Move > 0) // Mouse Wheel Back
	{
		mTranslateVector.z = mMoveScale * 2;

	}

	// Right Mouse
	if (flag_LeftMouseDown == 0 && flag_RightMouseDown == 1)
	{
		Capture_RightMouse_Model();
	}

	MoveCamera();


	return 1;
}

// *************************************************************************
// *				moveCamera   Terry Bernie							   *
// *************************************************************************
void CL64_3D_TL_Listener::MoveCamera(void)
{
	//WE_Cam_Node->yaw(mRotX, Ogre::Node::TS_PARENT);
	//WE_Cam_Node->pitch(mRotY);

	WE_Cam_Node->translate(mTranslateVector, Ogre::Node::TS_LOCAL); // Position Relative

	Wheel_Move = 0;
}

// *************************************************************************
// *	Capture_RightMouse_Model:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_3D_TL_Listener::Capture_RightMouse_Model(void)
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
		mTranslateVector.x = (delta < 0 ? Pl_DeltaMouse : -Pl_DeltaMouse)* (mMoveSensitivityMouse / 1000);
		SetCursorPos(App->CL_3D_TL_View->CursorPosX, App->CL_3D_TL_View->CursorPosY);
	}

	// Handle vertical movement [ Up Down ]
	delta = Pl_Cent500Y - Pl_MouseY;

	if (std::abs(delta) > 2) // Check if the movement is significant
	{
		Pl_DeltaMouse = static_cast<float>(std::abs(delta));
		Ogre::Real rate = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
		Ogre::Vector3 oldPos = WE_Cam_Node->getPosition();

		oldPos.z += (delta < 0 ? -rate : rate); // Adjust camera position based on mouse movement
		WE_Cam_Node->setPosition(oldPos);
		SetCursorPos(App->CL_3D_TL_View->CursorPosX, App->CL_3D_TL_View->CursorPosY);
	}
}
