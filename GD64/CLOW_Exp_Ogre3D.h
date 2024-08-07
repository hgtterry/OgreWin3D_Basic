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
class CLOW_Exp_Ogre3D
{
public:

	CLOW_Exp_Ogre3D(void);
	~CLOW_Exp_Ogre3D(void);

	void Export_To_Ogre3D(bool Create);

private:

	void Get_Data(int Index, int FaceIndex);
	void CreateMaterialFile(char* MatFileName);
	void Set_Export_Paths(void);
	void Copy_Textures(void);

	Ogre::ManualObject* Export_Manual;
	Ogre::String Export_Resource_Group;

	float nx;
	float ny;
	float nz;

	float u;
	float v;

	float x;
	float y;
	float z;

	char mExport_Just_Name[MAX_PATH];
	char mExport_PathAndFile_Mesh[MAX_PATH];
	char mExport_PathAndFile_Material[MAX_PATH];
	char mExport_Path[MAX_PATH];
	char mDirectory_Name[MAX_PATH];
	char mSelected_Directory[MAX_PATH];
};

