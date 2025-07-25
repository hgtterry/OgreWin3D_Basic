/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
class CL64_Camera
{
public:
	CL64_Camera(void);
	~CL64_Camera(void);

	void Track_Camera(void);
	void Camera_Reset_Zero(void);

	void Camera_Textured(void);
	void Camera_Wired(void);
	void Camera_Brushes(void);

	void Camera_Speed_Normal(void);
	void Camera_Speed_Slow(void);
	void Camera_Speed_Very_Slow(void);
	void Camera_Speed_Fast(void);
	void Camera_Set_Menu_Clear(void);

	void SetCameraMode_FirstPerson();
	void SetCameraMode_Free();

	void Reset_View_Editor(void);
	void Camera_Save_Location(void);

	bool flag_First_Person;
	bool flag_Free;

	Ogre::Vector3 Saved_Cam_Pos;
	Ogre::Quaternion Saved_Rotation;
};

