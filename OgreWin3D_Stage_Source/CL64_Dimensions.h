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

	bool flag_Show_Dimensions;
	bool flag_Show_Position;
	bool flag_Show_Scale;
	bool flag_Show_Rotation;

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

	bool flag_PosX_Selected;
	bool flag_PosY_Selected;
	bool flag_PosZ_Selected;

	bool flag_ScaleX_Selected;
	bool flag_ScaleY_Selected;
	bool flag_ScaleZ_Selected;

	bool flag_RotationX_Selected;
	bool flag_RotationY_Selected;
	bool flag_RotationZ_Selected;

	bool flag_Scale_Lock;

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

