#include "pch.h"
#include "CL64_App.h"
#include "Shapes_Render_Listener.h"

Shapes_Render_Listener::Shapes_Render_Listener(void)
{
	WE_Cam_Node = App->CL_X_Shapes_3D->Ogre_MV_CamNode;
	mMoveSensitivity = 50;
	Wheel_Move = 0;

	Pl_pt.x = 0;
	Pl_pt.y = 0;

	Pl_Cent500X = App->CursorPosX;
	Pl_Cent500Y = App->CursorPosY;

	Pl_DeltaMouse = 0;

	mMoveSensitivityMouse = 50;

	flag_Pl_RightMouseDown = false;
	flag_Pl_LeftMouseDown = false;
}

Shapes_Render_Listener::~Shapes_Render_Listener(void)
{
}

// *************************************************************************
// *				frameStarted   Terry Bernie							   *
// *************************************************************************
bool Shapes_Render_Listener::frameStarted(const Ogre::FrameEvent& evt)
{
	return true;
}


// *************************************************************************
// *			frameRenderingQueued   Terry Bernie						   *
// *************************************************************************
bool Shapes_Render_Listener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	ModelMode(evt.timeSinceLastFrame);

	return 1;
}

// *************************************************************************
// *							ModelMode   							   *
// *************************************************************************
void Shapes_Render_Listener::ModelMode(float DeltaTime)
{
	//*mRotX = 0;
	mRotY = 0;
	mTranslateVector = Ogre::Vector3::ZERO;
	mMoveScale = mMoveSensitivity * DeltaTime;

	//------------------------------------------------
	if (Wheel_Move < 0) // Mouse Wheel Forward
	{
		mTranslateVector.z = -mMoveScale * 30;
	}

	// back
	if (Wheel_Move > 0) // Mouse Wheel Back
	{
		mTranslateVector.z = mMoveScale * 30;
	}

	if (GetAsyncKeyState(87) < 0) // W Key
	{
		mTranslateVector.z = -mMoveScale;
	}

	if (GetAsyncKeyState(83) < 0) // S Key	
	{
		mTranslateVector.z = mMoveScale;
	}

	// Right
	if (GetAsyncKeyState(65) < 0)
	{
		mTranslateVector.x = mMoveScale;
	}
	// Left
	if (GetAsyncKeyState(68) < 0)
	{
		mTranslateVector.x = -mMoveScale;
	}

	// Left Mouse
	if (flag_Pl_LeftMouseDown == 1 && flag_Pl_RightMouseDown == 0)
	{
		Capture_LeftMouse_Model();
	}

	// Right Mouse
	if (flag_Pl_LeftMouseDown == 0 && flag_Pl_RightMouseDown == 1)
	{
		Capture_RightMouse_Model();
	}

	MoveCamera();
}

// *************************************************************************
// *				moveCamera   Terry Bernie							   *
// *************************************************************************
void Shapes_Render_Listener::MoveCamera(void)
{
	WE_Cam_Node->yaw(mRotX, Ogre::Node::TS_PARENT);
	WE_Cam_Node->pitch(mRotY);
	WE_Cam_Node->translate(mTranslateVector, Ogre::Node::TS_LOCAL); // Position Relative
	Wheel_Move = 0;
}

// *************************************************************************
// *					Capture_LeftMouse_Model							   *
// *************************************************************************
bool Shapes_Render_Listener::Capture_LeftMouse_Model(void)
{
	//if (App->CL_MeshViewer->MV_btDebug_Node && App->CL_MeshViewer->Ogre_MvNode)
	{
		GetCursorPos(&Pl_pt);

		Pl_MouseX = (int(Pl_pt.x));
		Pl_MouseY = (int(Pl_pt.y));

		//// Left Right
		if (Pl_MouseX < Pl_Cent500X)
		{
			long test = Pl_Cent500X - Pl_MouseX; // Positive

			if (test > 2)
			{
				Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
				App->CL_X_Shapes_3D->OGL_RenderListener->RZ = App->CL_X_Shapes_3D->OGL_RenderListener->RZ - (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);
				SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);
			}
		}
		else if (Pl_MouseX > Pl_Cent500X)
		{
			long test = Pl_MouseX - Pl_Cent500X; // Positive

			if (test > 2)
			{
				Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
				App->CL_X_Shapes_3D->OGL_RenderListener->RZ = App->CL_X_Shapes_3D->OGL_RenderListener->RZ + (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);
				SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);
			}
		}

		// Up Down
		if (Pl_MouseY < Pl_Cent500Y)
		{
			long test = Pl_Cent500Y - Pl_MouseY; // Positive

			if (test > 2)
			{
				Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);
				App->CL_X_Shapes_3D->OGL_RenderListener->RX = App->CL_X_Shapes_3D->OGL_RenderListener->RX - (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);
				SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);
			}
		}
		else if (Pl_MouseY > Pl_Cent500Y)
		{
			long test = Pl_MouseY - Pl_Cent500Y; // Positive

			if (test > 2)
			{
				Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
				App->CL_X_Shapes_3D->OGL_RenderListener->RX = App->CL_X_Shapes_3D->OGL_RenderListener->RX + (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);
				SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);
			}
		}
	}

	return 1;
}

// *************************************************************************
// *					Capture_RightMouse_Model Terry					   *
// *************************************************************************
bool Shapes_Render_Listener::Capture_RightMouse_Model(void)
{
	return 1;
	GetCursorPos(&Pl_pt);

	Pl_MouseX = (int(Pl_pt.x));
	Pl_MouseY = (int(Pl_pt.y));

	// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			mTranslateVector.x = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
			SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			mTranslateVector.x = -Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
			SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);
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
			OldPos = WE_Cam_Node->getPosition();

			OldPos.y -= Rate;
			WE_Cam_Node->setPosition(OldPos);
			SetCursorPos(App->CL_X_Shapes_3D->CursorPosX, App->CL_X_Shapes_3D->CursorPosY);
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
			OldPos = WE_Cam_Node->getPosition();

			OldPos.y += Rate;
			WE_Cam_Node->setPosition(OldPos);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}

	}

	return 1;
}
