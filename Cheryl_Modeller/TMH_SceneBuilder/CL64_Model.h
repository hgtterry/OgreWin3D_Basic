#pragma once

typedef struct AABB_Type {
	vertex_type BB_Max[1];
	vertex_type BB_Min[1];
	vertex_type Size[1];
	vertex_type Centre[1];
	float radius;
} AABB_Type;

class CL64_Model
{
public:
	CL64_Model(void);
	~CL64_Model(void);

	void Set_Paths(void);
	void Set_BondingBox_Model(bool Create);
	void Clear_Model();

	char Loaded_PathFileName[MAX_PATH];
	char Loaded_FileName[MAX_PATH];
	char Model_FolderPath[MAX_PATH];
	char Model_Just_Name[MAX_PATH];
	
	int Model_Type;

	// Internal
	int GroupCount;
	int TextureCount;
	int MotionCount;
	int VerticeCount;
	int FaceCount;
	int BoneCount;

	bool flag_Model_Loaded;

	Ogre::Entity* Imported_Ogre_Ent;
	Ogre::SceneNode* Imported_Ogre_Node;

	AABB_Type* S_BoundingBox[1];
};

