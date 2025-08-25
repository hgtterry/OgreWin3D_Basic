/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D Scene Builder

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
class Base_Teleporter
{
public:
	Base_Teleporter(void);
	~Base_Teleporter(void);

	// object
	char Object_Name[MAX_PATH];	// Name of Object/Entity Set By User
	char Mesh_FileName[MAX_PATH]; // Acctual Mesh File name as in .mesh
	char Material_File[MAX_PATH]; // Acctual Material File name as in .material

	int Type;
	int Shape;
	int Usage;
	/*int UsageEX;
	int Folder;*/
	int This_Object_UniqueID;  // Unique Number

	Ogre::SceneNode* Object_Node;
	Ogre::Entity* Object_Ent;
	btRigidBody* Phys_Body;
	btCollisionShape* Phys_Shape;

	//------------------------------ Mesh
	//Ogre::Vector3			Mesh_Scale;
	Ogre::Vector3			Mesh_Pos;
	//Ogre::Vector3			Mesh_Center;
	//Ogre::Vector3			Mesh_Rot;
	Ogre::Quaternion		Mesh_Quat;
	//------------------------------ Physics
	Ogre::Vector3			Physics_Pos;
	Ogre::Vector3			Physics_Rot;
	Ogre::Vector3			Physics_Scale;
	Ogre::Vector3			Physics_Size; // Box x y z ;- x = Radius y = Height
	Ogre::Quaternion		Physics_Quat;
	float					Physics_Mass;
	float					Physics_Restitution;
	bool					flag_Physics_Valid;

	// Teleport
	char Location_Name[MAX_PATH];
	Ogre::Vector3 Player_Position;
	btVector3 Physics_Position;
	btQuaternion Physics_Rotation;
	int Location_ID;
	Ogre::Real SndVolume;
	char Sound_File[MAX_PATH];
	bool flag_Play;
	int Trigger_Value;
	int Counter_ID;
	char Counter_Name[MAX_PATH];
	bool flag_Counter_Disabled;

	HTREEITEM FileViewItem;
};

