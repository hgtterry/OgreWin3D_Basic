/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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

	//Ogre::Camera* mDummyCamera;
	//Vector3		mDummyTranslateVector;

	Ogre::String Pl_Entity_Name;
	char Selected_Object_Name[MAX_PATH];
	int	Selected_Entity_Index;
	float Compenstate;


	btRigidBody *Phys_Body;
	btCollisionShape *Phys_Shape;

	Ogre::Vector3 StartPos;
	btQuaternion Physics_Rotation;

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

	bool flag_mJump;
	bool flag_Altered;
	bool flag_Deleted;
	bool flag_mOnGround;
	bool flag_IsMOving;
	bool flag_IsMOving_Back;
	bool flag_IsMOving_Right;
	bool flag_IsMOving_Left;
	bool flag_Physics_Debug_On;

	HTREEITEM FileViewItem;

	char Player_Name[255]; // Players Name must be unique [171221]


	btVector3 mMoveDirection;

	btScalar mSpeedDamping = 10.1;
	btScalar mMaxLinearVelocity2 = pow(15 / 3.6, 2);
	btScalar mWalkAccel = 25.0;
};

