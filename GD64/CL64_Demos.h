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
class CL64_Demos
{
public:

	CL64_Demos(void);
	~CL64_Demos(void);

	void Reset_Class(void);

	void Start_Demo_1(void);
	void Start_Demo_2(void);

	void Reset_View(void);

	Ogre::Entity* World_Ent;
	Ogre::SceneNode* World_Node;

	bool flag_Demo_1_Running;
	bool flag_Demo_2_Running;

};

