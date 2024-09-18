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
class CL64_Keyboard
{
public:

	CL64_Keyboard(void);
	~CL64_Keyboard(void);

	void Keyboard_Mode_First(float deltaTime);
	void Keyboard_Mode_Model(float deltaTime);
	void Keyboard_Mode_Free(float deltaTime);

	Ogre::Real Rate;
	Ogre::Vector3 OldPos;
};

