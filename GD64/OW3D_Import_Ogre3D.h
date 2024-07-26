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
class OW3D_Import_Ogre3D
{
public:

	OW3D_Import_Ogre3D(void);
	~OW3D_Import_Ogre3D(void);

	void Get_Motions(Ogre::Entity* Ogre_Entity);

	bool flag_IsAnimated;
};

