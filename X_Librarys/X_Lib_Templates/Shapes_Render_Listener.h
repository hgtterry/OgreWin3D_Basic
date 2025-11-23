#pragma once
#include <Windows.h>
#include <OgreFrameListener.h>
#include <OgreMath.h>
#include <OgrePrerequisites.h>
#include <OgreSceneNode.h>

class Shapes_Render_Listener : public Ogre::FrameListener
{
public:
	Shapes_Render_Listener(void);
	~Shapes_Render_Listener(void);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	void ModelMode(float DeltaTime);
	void MoveCamera(void);

	bool Capture_LeftMouse_Model(void);
	bool Capture_RightMouse_Model(void);

	Ogre::SceneNode* WE_Cam_Node;

	int			Wheel_Move;
	Ogre::Radian		mRotX;
	Ogre::Radian		mRotY;
	Ogre::Vector3		mTranslateVector;
	float		mMoveScale;
	float		mMoveSensitivity;

	bool flag_Pl_RightMouseDown;
	bool flag_Pl_LeftMouseDown;

	float		mMoveSensitivityMouse;

	POINT		Pl_pt;
	long		Pl_Cent500X;
	long		Pl_Cent500Y;

	float		Pl_DeltaMouse;
	int			Pl_MouseX;
	int			Pl_MouseY;
};

