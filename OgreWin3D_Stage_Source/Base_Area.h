#pragma once

class Base_Area
{
public:
	Base_Area();
	~Base_Area();

	Ogre::Entity*		Area_Ent;
	Ogre::SceneNode*	Area_Node;

	btRigidBody *Phys_Body;
	btCollisionShape *Phys_Shape;

	void Collect_Object_Data(void);

	Ogre::Vector3			Physics_Pos;
	Ogre::Vector3			Physics_Rot;
	Ogre::Vector3			Physics_Scale;

	Ogre::Quaternion		Physics_Quat;

	int Type;
	int Shape;
	int Usage;
	int Folder;
	int This_Object_UniqueID;  // Unique Number

	char Area_Name[MAX_PATH]; // Name of Area Set By User

	char Area_FileName[MAX_PATH]; // Acctual Mesh File name as in .mesh
	char Area_Path_And_FileName[MAX_PATH];
	char Area_Resource_Path[MAX_PATH];
	char Material_File[MAX_PATH]; // Acctual Material File name as in .material

	//------------------------------ Mesh
	Ogre::Vector3			Mesh_Scale;
	Ogre::Vector3			Mesh_Pos;
	Ogre::Vector3			Mesh_Center;
	Ogre::Vector3			Mesh_Rot;
	Ogre::Quaternion		Mesh_Quat;

	HTREEITEM FileViewItem;			// Holder for Area File view item [211121]

	bool flag_Physics_Valid;
	bool flag_Altered;
	bool flag_Deleted;
	bool flag_Physics_Debug_On;

};

