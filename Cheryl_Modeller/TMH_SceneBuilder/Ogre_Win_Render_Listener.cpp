#include "pch.h"
#include "CL64_App.h"
#include "Ogre_Win_Render_Listener.h"

Ogre_Win_Render_Listener::Ogre_Win_Render_Listener(void)
{
	WE_Cam_Node = App->CL_3D_TL_View->Ogre_TL_CamNode;

	Wheel_Move = 0;
	mTranslateVector = Ogre::Vector3::ZERO;
	mMoveSensitivity = 50;
}

Ogre_Win_Render_Listener::~Ogre_Win_Render_Listener(void)
{
}

// *************************************************************************
// *				frameStarted   Terry Bernie							   *
// *************************************************************************
bool Ogre_Win_Render_Listener::frameStarted(const Ogre::FrameEvent& evt)
{
	return true;
}


// *************************************************************************
// *			frameRenderingQueued   Terry Bernie						   *
// *************************************************************************
bool Ogre_Win_Render_Listener::frameRenderingQueued(const Ogre::FrameEvent& evt)
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

	MoveCamera();

	
	return 1;
}

// *************************************************************************
// *				moveCamera   Terry Bernie							   *
// *************************************************************************
void Ogre_Win_Render_Listener::MoveCamera(void)
{
	//WE_Cam_Node->yaw(mRotX, Ogre::Node::TS_PARENT);
	//WE_Cam_Node->pitch(mRotY);

	WE_Cam_Node->translate(mTranslateVector, Ogre::Node::TS_LOCAL); // Position Relative

	Wheel_Move = 0;
}
