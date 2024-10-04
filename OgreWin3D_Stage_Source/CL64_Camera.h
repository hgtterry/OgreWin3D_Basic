/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#pragma once
class CL64_Camera
{
public:

	CL64_Camera(void);
	~CL64_Camera(void);

	void Reset_View(void);
	void Set_Camera_Position(float X,float Y,float Z);
	void Set_Camera_Rotation(float Pitch, float Yaw, float Roll);
	void Camera_Goto_Object(int Object_Index);

};

