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

#include "Base_Player.h"
#include "Base_Group.h"

typedef struct AABB_Type {
	vertex_type BB_Max[1];
	vertex_type BB_Min[1];
	vertex_type Size[1];
	vertex_type Centre[1];
	float radius;
} AABB_Type;

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

typedef struct OgreMeshData_Type {

	Ogre::String mEdgeList;
	std::vector<std::string> mMaterials;
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

class CL64_Scene
{
public:

	CL64_Scene(void);
	~CL64_Scene(void);

	void Reset_Class(void);

	void Create_Mesh_Group(int Index);
	void Clear_Model_And_Reset(void);
	void Set_Paths(void);
	void Reset_Main_Entity(void);
	void Main_Entity_Set_Default(void);
	void Set_BondingBox_Model(bool Create);

	Ogre::Entity* Main_Ent;
	Ogre::SceneNode* Main_Node;

	Base_Group* Group[100];
	AABB_Type* S_BoundingBox[1];
	Bone_Type* S_Bones[200];
	OgreMeshData_Type* S_OgreMeshData[1];

	char FileName[MAX_PATH];
	char Path_FileName[MAX_PATH];
	char Model_FolderPath[MAX_PATH];
	char Texture_FolderPath[MAX_PATH];
	char JustName[MAX_PATH];

	// Internal
	int GroupCount;
	int TextureCount;
	int MotionCount;
	int VerticeCount;
	int FaceCount;
	int Model_Type;
	int BoneCount;

	int Scene_Mode;

	// Ogre
	int Ogre_Face_Count;

	bool Player_Added;
	bool Model_Loaded;

	std::vector<Base_Player*> B_Player;
};

