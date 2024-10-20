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
class CL64_Dimensions
{
public:

	CL64_Dimensions(void);
	~CL64_Dimensions(void);

	void Prepare_Dimensions(void);
	void ImGui_Dimensions(void);

	bool Show_Dimensions;

	bool Show_Position;
	bool Show_Scale;
	bool Show_Rotation;

protected:

	void ImGui_Position(void);
	void Set_Position(Ogre::Vector3 Pos);

	void ImGui_Scale(void);
	void Set_Scale(Ogre::Vector3 Scale);

	void ImGui_Rotation(void);
	void Set_Rotation(Ogre::Vector3 Rotation, float Delta);

	Ogre::Vector3 Get_BoundingBox_World_Centre();
	void UpDate_Physics(int Index);
	void Set_Physics_Position();

	bool PosX_Selected;
	bool PosY_Selected;
	bool PosZ_Selected;

	bool ScaleX_Selected;
	bool ScaleY_Selected;
	bool ScaleZ_Selected;

	bool RotationX_Selected;
	bool RotationY_Selected;
	bool RotationZ_Selected;

	bool Scale_Lock;

	float Model_Pos_Delta;
	float Model_Scale_Delta;
	float Model_Rotation_Delta;

	// ---------------------------------------- Pointers
	char* pBase_Mesh_Name;

	int* pBase_Shape;

	Ogre::Entity* pBase_Object_Ent;
	Ogre::SceneNode* pBase_Object_Node;
	Ogre::Vector3* pBase_Mesh_Pos;
	Ogre::Vector3* pBase_Mesh_Scale;
	Ogre::Vector3* pBase_Mesh_Rot;
	Ogre::Quaternion* pBase_Mesh_Quat;

	btRigidBody* pBase_Phys_Body;
	Ogre::Vector3* pBase_Physics_Pos;
	Ogre::Vector3* pBase_Physics_Scale;
	Ogre::Vector3* pBase_Physics_Rot;
	Ogre::Quaternion* pBase_Physics_Quat;
};

