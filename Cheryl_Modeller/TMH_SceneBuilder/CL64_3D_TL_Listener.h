#pragma once

using namespace Ogre;

class CL64_3D_TL_Listener : public Ogre::FrameListener
{
public:
	CL64_3D_TL_Listener(void);
	~CL64_3D_TL_Listener(void);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void MoveCamera(void);

	Ogre::SceneNode* WE_Cam_Node;

	int	Wheel_Move;
	Ogre::Vector3		mTranslateVector;
	float		mMoveScale;
	float		mMoveSensitivity;

};

