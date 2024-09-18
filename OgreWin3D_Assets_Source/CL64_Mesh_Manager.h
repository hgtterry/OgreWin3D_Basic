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
class CL64_Mesh_Manager
{
public:

	CL64_Mesh_Manager(void);
	~CL64_Mesh_Manager(void);

	bool Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity);
	void Get_Ogre_Mesh_Data(Ogre::Entity* Ogre_Entity);

	bool Has_Shared_Vertices;

private:

	bool Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity);
	
	void Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
		size_t& vertex_count, Ogre::Vector3*& vertices,
		size_t& index_count, unsigned long*& indices,
		int SubMesh, Ogre::int16*& BoneIndices);

	bool Get_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh);

	bool Get_SubPoseNormals(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& Normals,
		int SubMesh);

	bool GetBoneAssignment(Ogre::MeshPtr mesh, int SubMesh, HWND hDlg);

	std::vector<Ogre::Vector2> MeshTextureCoords;
};

