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

#include "pch.h"
#include "Base_Camera.h"

Base_Camera::Base_Camera()
{
	Init_Camera();
}

Base_Camera::~Base_Camera()
{
	
}

// *************************************************************************
// *	  		Init_Camera:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void Base_Camera::Init_Camera(void)
{
	CamPos.x = 0;
	CamPos.y = 0;
	CamPos.z = 0;

	LookAt.x = 0;
	LookAt.y = 30;
	LookAt.z = 0;

	flag_Altered = 1;

	This_Object_UniqueID = 0;

	Cam_Quat.IDENTITY;

	Camera_Name[0] = 0;

	FileViewItem = nullptr;

}
