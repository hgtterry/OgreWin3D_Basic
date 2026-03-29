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
	void Capture_RightMouse_Model(void);

	Ogre::SceneNode* WE_Cam_Node;

	int	Wheel_Move;
	Ogre::Vector3		mTranslateVector;
	
	POINT Mouse_point;
	int	Pl_MouseX;
	int	Pl_MouseY;
	long Pl_Cent500X;
	long Pl_Cent500Y;
	float Pl_DeltaMouse;

	float mMoveSensitivity;
	float mMoveScale;
	float mMoveSensitivityMouse;

	bool flag_LeftMouseDown;
	bool flag_RightMouseDown;

};

