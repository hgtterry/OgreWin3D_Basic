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

#include "Ogre.h"

#pragma once
class CL64_MeshView_Listener : public FrameListener
{
public:
	CL64_MeshView_Listener();
	~CL64_MeshView_Listener();

	bool frameRenderingQueued(const FrameEvent& evt);
	bool frameStarted(const FrameEvent& evt); // Comes From Directly From Oger Render Listener [081221]

	int		Wheel_Move;
	bool	flag_Pl_LeftMouseDown;	// Triger Left Mouse Presed [081221]
	bool	flag_Pl_RightMouseDown;	// Triger Right Mouse Presed [081221]

	bool	flag_Show_Model_Data; // Show Model Data ImGui [081221]

	int		View_Height;	// Height of Render Window [081221]
	int		View_Width;		// Width of Render Window [081221]

	float		mMoveSensitivity;
	float		mMoveSensitivityMouse;

	Ogre::Camera* WE_Cam;
	Ogre::SceneNode* WE_Cam_Node;

	long		Pl_Cent500X;
	long		Pl_Cent500Y;

protected:

	void ModelMode(float DeltaTime);
	void MoveCamera(void);

	bool Capture_LeftMouse_Model(void);
	bool Capture_RightMouse_Model(void);

	Radian		mRotX;
	Radian		mRotY;
	Vector3		mTranslateVector;

	float		mMoveScale;


	float		Pl_DeltaMouse;
	int			Pl_MouseX;
	int			Pl_MouseY;

	POINT		Pl_pt;
};

