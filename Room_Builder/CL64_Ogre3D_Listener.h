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

#pragma once

using namespace Ogre;

class CL64_Ogre3D_Listener : public FrameListener
{
public:
	CL64_Ogre3D_Listener();
	~CL64_Ogre3D_Listener();

	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);
	bool frameRenderingQueued(const FrameEvent& evt);

	void Update_Game_Logic(float DeltaTime);

	bool Capture_Mouse_FirstPerson(float DeltaTime);

	void Camera_Mode_Model(float DeltaTime);
	void Capture_LeftMouse_Model(void);
	void Capture_RightMouse_Model(void);

	void Camera_Mode_Free(float DeltaTime);
	void Capture_Left_Mouse_Free(void);
	void Capture_Right_Mouse_Free(void);

	void MoveCamera(void);


	POINT Mouse_point;
	int	Pl_MouseX;
	int	Pl_MouseY;
	long Pl_Cent500X;
	long Pl_Cent500Y;
	float Pl_DeltaMouse;
	

	Ogre::SceneNode* mCamNode;
	float mMoveSensitivity;
	float mMoveScale;
	float mMoveSensitivityMouse;

	bool flag_LeftMouseDown;
	bool flag_RightMouseDown;

	int CameraMode;
	int	Wheel;

	Radian		mRotX;
	Radian		mRotY;

	Vector3	mTranslateVector;

	bool flag_StopOgre;
	bool flag_Run_Physics;
};

