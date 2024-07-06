/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
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
	bool Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity);

	void Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,size_t& vertex_count, 
		Ogre::Vector3*& vertices,size_t& index_count,
		unsigned long*& indices,int SubMesh, Ogre::int16*& BoneIndices);

	bool GetBoneAssignment(Ogre::MeshPtr mesh, int SubMesh, HWND hDlg);
	bool NewGet_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh);

	bool NewGet_SubPoseNormals(Ogre::MeshPtr mesh,
		size_t& vertex_count,Ogre::Vector3*& Normals,int SubMesh);

	bool Get_SkeletonInstance(Ogre::Entity* Ogre_Entity);
	void Get_Ogre3D_MeshData(Ogre::Entity* Ogre_Entity);



	Ogre::ManualObject* Export_Manual;
	Ogre::ManualObject* World_Manual;

	Ogre::SceneNode*	World_Node;
	Ogre::Entity*		World_Ent;

	std::vector<Vector2> MeshTextureCoords;
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

