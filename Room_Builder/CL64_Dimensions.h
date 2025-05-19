/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once
class CL64_Dimensions
{
public:

	CL64_Dimensions(void);
	~CL64_Dimensions(void);

	void Prepare_Dimensions(void);
	void ImGui_Dimensions(void);

	void Do_Scale_New();
	void Do_Rotation_New();
	void Do_Position_New();

	void Set_Rotation(Ogre::Vector3 Rotation, float Delta);

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

