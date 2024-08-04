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
class CLOW_Imp_Ogre3D
{
public:

	CLOW_Imp_Ogre3D(void);
	~CLOW_Imp_Ogre3D(void);

	void Reset_Class(void);

	bool Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity);

	void Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
		size_t& vertex_count, Ogre::Vector3*& vertices,
		size_t& index_count, unsigned long*& indices,
		int SubMesh, Ogre::int16*& BoneIndices);

	bool GetBoneAssignment(Ogre::MeshPtr mesh, int SubMesh, HWND hDlg);
	bool NewGet_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh);

	bool NewGet_SubPoseNormals(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& Normals,
		int SubMesh);

	void Get_Ogre_Mesh_Data(Ogre::Entity* Ogre_Entity);
	void Get_Motions(Ogre::Entity* Ogre_Entity);

	std::vector<Ogre::Vector2> MeshTextureCoords;

	bool flag_IsAnimated;
	bool flag_Ogre_Model_Loaded;
};

