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
class CL64_Grid
{

public:
	CL64_Grid(void);
	~CL64_Grid(void);

	void Reset_Class(void) const;
	void Grid_Update(bool Create);
	void Grid_SetVisible(bool Option);

	void Hair_Update(bool Create);
	void Hair_SetVisible(bool Option);

	Ogre::ManualObject* GridManual;
	Ogre::SceneNode* GridNode;

	Ogre::ManualObject* HairManual;
	Ogre::SceneNode* HairNode;

	int XAxis_min;
	int	XAxis_max;
	int YAxis_min;
	int	YAxis_max;
	int ZAxis_min;
	int	ZAxis_max;

	int Division_X;
	int Division_Y;
	int Division_Z;

	int Scale_X;
	int Scale_Y;
	int Scale_Z;

	Ogre::ColourValue ColourMain;
	Ogre::ColourValue ColourDivision;

	int HairExtend;
	Ogre::ColourValue ColourHairZ;
	Ogre::ColourValue ColourHairX;
	Ogre::ColourValue ColourHairY;

	bool ShowGridFlag;
	bool ShowDivisions;
	bool ShowHair;
};

