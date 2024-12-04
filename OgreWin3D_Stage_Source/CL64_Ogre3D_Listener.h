/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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

#include "Ogre.h"
#include "CL64_App.h"

using namespace Ogre;

class CL64_Ogre3D_Listener : public FrameListener
{
public:
	CL64_Ogre3D_Listener(void);
	~CL64_Ogre3D_Listener(void);

	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);
	bool frameRenderingQueued(const FrameEvent& evt);
	void MoveCamera(void);
	void Get_View_Height_Width(void);
	void Update_Game_Logic(float DeltaTime);

	// Model Mode
	void Mode_Camera_Model(float DeltaTime);
	bool Capture_LeftMouse_Model(void);
	bool Capture_RightMouse_Model(void);

	// Free Mode
	void Mode_Camera_Free(float DeltaTime);
	bool Capture_Left_Mouse_Free(void);
	bool Capture_Right_Mouse_Free(void);
	bool Capture_Mouse_FirstPerson(float DeltaTime);
	
	Ogre::Camera* mCam;
	Ogre::SceneNode* mCamNode;

	POINT		Mouse_point;
	//Ogre::Overlay* mDebugOverlay;

	Radian		mRotX;
	Radian		mRotY;
	Vector3		mTranslateVector;

	float		mMoveScale;
	float		mMoveSensitivity; 
	float		mMoveSensitivityMouse;
	
	int	View_Height; // Ogre window Height
	int	View_Width;	 // Ogre window WIdth

	int CameraMode;

	int	Wheel;

	int	Pl_MouseX;
	int	Pl_MouseY;
	
	int Bullet_Step;

	long Pl_Cent500X;
	long Pl_Cent500Y;

	float Pl_DeltaMouse;

	bool flag_LeftMouseDown;
	bool flag_RightMouseDown;

	bool flag_Run_Physics;
	bool flag_StopOgre;
	bool flag_Block_Mouse;

	bool flag_Ogre_Model_Loaded;

	bool flag_Animate_Ogre;

	bool flag_Selection_Mode;
	int	Selected_Entity_Index;
	char Selected_Object_Name[MAX_PATH];

};

