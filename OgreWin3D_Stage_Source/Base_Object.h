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

typedef struct Move_Type {
	char Object_Name[MAX_PATH];	// Name of Object to Move
	float Newpos;
	float Speed;			// 
	int WhatDirection;		//
	int Object_To_Move_Index;		// Can Change
	bool flag_IsNegative = 0;	// Positive or Negative distance to move
	bool flag_Triggered;			// Entity 
	bool flag_Re_Trigger;
	float Move_Distance;	// Distance to Move negative or positive
	Ogre::Vector3 MeshPos;
	Ogre::Vector3 PhysicsPos;
	int Trigger_Value;
	int Counter_ID;
	char Counter_Name[MAX_PATH];
	bool flag_Counter_Disabled;

}Move_Type;

typedef struct Teleport_type
{
	char Location_Name[MAX_PATH];
	Ogre::Vector3 Player_Position;
	btVector3 Physics_Position;
	btQuaternion Physics_Rotation;
	int Location_ID;
	irrklang::ISound* SndFile;
	Ogre::Real SndVolume;
	char Sound_File[MAX_PATH];
	bool flag_Play;
	int Trigger_Value;
	int Counter_ID;
	char Counter_Name[MAX_PATH];
	bool flag_Counter_Disabled;

}Teleport_type;

typedef struct Message_type
{
	int Trigger_Value;
	int Counter_ID;
	char Counter_Name[MAX_PATH];
	bool flag_Counter_Disabled;
	float Message_PosX;
	float Message_PosY;
	char Message_Text[MAX_PATH];
	bool flag_PosXCentre_Flag;
	bool flag_PosYCentre_Flag;
	Ogre::Vector4 Text_Colour;
	Ogre::Vector4 BackGround_Colour;
	bool flag_Show_BackGround;

}Message_type;

typedef struct Collectable_type
{
	int Counter_ID;
	irrklang::ISound* SndFile;
	Ogre::Real SndVolume;
	char Sound_File[MAX_PATH];
	char Counter_Name[MAX_PATH];
	bool flag_Counter_Disabled;
	bool flag_Play;
	int Maths; // [ 0 = None ]  [ 1 = Add ]  [ 2 = Subtract ]
	int Value; 

}Collectable_type;

typedef struct Environ_type
{
	int Environment_ID;
	char Environment_Name[MAX_PATH];

	bool flag_Environ_Enabled;

	//--------------- Sound
	irrklang::ISound* SndFile;
	Ogre::Real SndVolume;
	char Sound_File[MAX_PATH];
	bool flag_Play;
	bool flag_Loop;

	//--------------- Light
	Ogre::Vector3 AmbientColour;
	Ogre::Vector3 Light_Position;

	//--------------- Sky
	bool flag_Enabled;
	int type;
	char Material[MAX_PATH];
	float Curvature;
	float Tiling;
	float Distance;

	//--------------- Fog
	int Fog_On;
	int Fog_Mode;
	Ogre::Vector3 Fog_Colour;
	float Fog_Start;
	float Fog_End;
	float Fog_Density;

}Environ_type;

// --------------------------------- Particle
typedef struct Particle_type
{
	Ogre::ParticleSystem* Particle;
	float SpeedFactor;
	char ParticleScript[MAX_PATH];

}Particle_type;

// --------------------------------- Light
typedef struct Light_type
{
	Light* light;

}Light_type;

#pragma once
class Base_Object
{
public:

	Base_Object();
	~Base_Object();

	bool Init_Object(void);
	void Set_ImGui_Panel_Name(void);
	void Render_ImGui_Panel(void);

	Ogre::SceneNode		*Object_Node;
	Ogre::Entity		*Object_Ent;
	btRigidBody			*Phys_Body;
	btCollisionShape	*Phys_Shape;

	char Object_Name[MAX_PATH];	// Name of Object/Entity Set By User

	char Mesh_FileName[MAX_PATH]; // Acctual Mesh File name as in .mesh
	char Mesh_Resource_Path[MAX_PATH];
	char Material_File[MAX_PATH]; // Acctual Material File name as in .material

	//------------------------------ Description
	int Type;
	int Shape;
	int Usage;
	int UsageEX;
	int Folder;
	int This_Object_UniqueID;  // Unique Number

	//------------------------------ Mesh
	Ogre::Vector3			Mesh_Scale;
	Ogre::Vector3			Mesh_Pos;
	Ogre::Vector3			Mesh_Center;
	Ogre::Vector3			Mesh_Rot;
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

	//------------------------------ ListView

	HTREEITEM FileViewItem;

	//------------------------------ Bounding Box
	Ogre::Real Mesh_BB_Width;
	Ogre::Real Mesh_BB_Height;
	Ogre::Real Mesh_BB_Depth;
	Ogre::Quaternion Mesh_BB_Quat;
	Ogre::Vector3 Mesh_BB_Center;

	bool flag_Deleted;
	bool flag_Altered;
	bool flag_Physics_Debug_On;
	bool flag_Dimensions_Locked;

	//------------------------------ Entity
	bool Collision;
	bool flag_Triggered;

	//-----Sound Entity
	bool flag_HasSound;
	bool flag_Play_Sound;
	char Sound_File[MAX_PATH];
	char Sound_Path[MAX_PATH];
	irrklang::ISound* SndFile;
	Ogre::Real SndVolume;

	Move_Type* S_MoveType[1];
	Teleport_type* S_Teleport[1];
	Collectable_type* S_Collectable[1];
	Message_type*	S_Message[1];
	Environ_type*	S_Environ[1];
	Particle_type*	S_Particle[1];
	Light_type*		S_Light[1];

	bool flag_OverRide_Counter;

	bool flag_Show_Message_Flag;

	char ImGui_Panel_Name[MAX_PATH];
};

