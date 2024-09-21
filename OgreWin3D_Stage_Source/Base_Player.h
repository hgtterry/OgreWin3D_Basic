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
class Base_Player
{
public:
	Base_Player();
	~Base_Player();

	void Rotate_FromCam(const Ogre::Vector3 axis, float delta, bool normalize);

	Ogre::Camera*		CameraPitch;
	Ogre::Entity*		CameraPitch_Ent;
	Ogre::SceneNode*	CameraPitch_Node;

	Ogre::Entity*		Player_Ent;
	Ogre::SceneNode*	Player_Node;

	Ogre::Camera* mDummyCamera;
	Vector3		mDummyTranslateVector;

	Ogre::String Pl_Entity_Name;
	char Selected_Object_Name[MAX_PATH];
	int	Selected_Entity_Index;
	float Compenstate;


	btRigidBody *Phys_Body;
	btCollisionShape *Phys_Shape;

	Ogre::Vector3 StartPos;
	btQuaternion Physics_Rotation;

	bool mJump;
	btVector3 mJumpDir;
	float mJumpSpeed;

	float Forward_Timer;
	float Ground_speed;

	float TurnRate;

	float Capsule_Mass;
	float Capsule_Radius;
	float Capsule_Height;

	float Limit_Look_Up;
	float Limit_Look_Down;
	float PlayerHeight;
	bool Altered;
	bool mOnGround;
	bool IsMOving;
	bool IsMOving_Back;
	bool IsMOving_Right;
	bool IsMOving_Left;
	bool Physics_Debug_On;

	HTREEITEM FileViewItem;

	char Player_Name[255]; // Players Name must be unique [171221]


	btVector3 mMoveDirection;

	btScalar mSpeedDamping = 10.1;
	btScalar mMaxLinearVelocity2 = pow(15 / 3.6, 2);
	btScalar mWalkAccel = 25.0;
};

