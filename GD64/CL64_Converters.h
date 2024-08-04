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
class CL64_Converters
{
public:

	CL64_Converters(void);
	~CL64_Converters(void);

	Ogre::Entity* Convert_To_Ogre3D(bool Create);

	void Set_Paths(void);
	void CreateMaterialFile();
	
	void Get_Data(int Index, int FaceIndex);

	void Create_Resource_Group();
	void CreateMaterial_Resource(char* MatName);

	// Ogre to Mesh Groups
	void Create_MeshGroups(Ogre::Entity* Ogre_Entity);
	
	bool Get_SkeletonInstance(Ogre::Entity* Ogre_Entity);
	
	Ogre::ManualObject* Export_Manual;
	Ogre::ManualObject* World_Manual;

	Ogre::SceneNode*	World_Node;
	Ogre::Entity*		World_Ent;

	std::vector<Vector3> MeshNormals;

	char mWorld_Mesh_JustName[MAX_PATH];
	char mWorld_File_PathAndFile[MAX_PATH];
	char mWorld_File_Path[MAX_PATH];
	char mExport_Just_Name[MAX_PATH];
	char Material_PathAndFile[MAX_PATH];

	float nx;
	float ny;
	float nz;

	float u;
	float v;

	float x;
	float y;
	float z;

	Ogre::String Temp_Resource_Group;

};

