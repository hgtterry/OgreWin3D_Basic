#pragma once

typedef struct Bone_Vert { float x, y, z; }Bone_Vert;
typedef struct Translation_Type { float X, Y, Z; }Translation_Type;
typedef struct rgb_Type { int Index; }rgb_Type;
typedef struct Char_type { char Name[256]; }Char_type;

typedef struct Bone_Type {
	int Parent;
	Bone_Vert Boneverts;
	char BoneName[200];
	Translation_Type TranslationStart;
}Bone_Type;

typedef struct Ogre_SubMesh_Type {
	std::string m_SubMesh_Name_str;
	std::string m_Matrial_Name_str;
	Ogre::String m_HasSharedVertices_str;
	unsigned int VerticesCount;
	int BonesCount;
}Ogre_SubMesh_Type;

typedef struct OgreMeshData_Type {
	Ogre::String mStrSkeleton;
	Ogre::String mStrEdgeList;
	Ogre::String mStrName;
	Ogre::String mFileName_Str;
	std::vector<std::string> m_Motion_Names;
	std::vector<std::float_t> m_Motion_Length;
	std::vector<std::int16_t> m_Motion_Num_Of_Tracks;

	std::vector<Ogre_SubMesh_Type> mSubmeshes;
	int mSubMeshCount;
	Ogre::Vector3 Center;
	Ogre::Vector3 vMin;
	Ogre::Vector3 vMax;
	float Width;
	float Height;
	float Depth;
	float Area;
	float Volume;
	float Radius;
}OgreMeshData_Type;

#include "Base_Group.h"

class CL64_Mesh
{
public:
	CL64_Mesh(void);
	~CL64_Mesh(void);

	bool Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity);
	bool Ogre_Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity);
	void Ogre_Create_MeshGroups(Ogre::Entity* Ogre_Entity);

	void Ogre_Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
		size_t& vertex_count, Ogre::Vector3*& vertices,
		size_t& index_count, unsigned long*& indices,
		int SubMesh, Ogre::int16*& BoneIndices);

	bool Ogre_Get_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh);
	bool Ogre_Get_SubPoseNormals(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& Normals, int SubMesh);
	void Get_Ogre_Mesh_Data(Ogre::Entity* Ogre_Entity);
	bool Get_SkeletonInstance(Ogre::Entity* Ogre_Entity);

	void Show_Mesh_Normals();
	void Show_Mesh_Textures();
	void Show_Mesh_Faces();
	void Show_Mesh_Points();
	void Show_Mesh_BoundBox();
	void Show_Mesh_Bones();

	bool flag_Has_Shared_Vertices;

	Base_Group* Group[5000];
	Bone_Type* S_Bones[200];

	OgreMeshData_Type* S_OgreMeshData[1];

	std::vector<Ogre::Vector2> MeshTextureCoords;
};

