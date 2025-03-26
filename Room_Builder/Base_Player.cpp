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

#include "pch.h"
#include "CL64_App.h"
#include "Base_Player.h"


Base_Player::Base_Player()
{
	//mDummyCamera = App->CL_Ogre->mSceneMgr->createCamera("PlayerRay");

	CameraPitch =		nullptr;
	CameraPitch_Ent =	nullptr;;
	CameraPitch_Node =	nullptr;;

	Player_Ent =	nullptr;
	Player_Node =	nullptr;

	Phys_Body =		nullptr;
	Phys_Shape =	nullptr;

	mMoveDirection.setValue(0, 0, 0);

	StartPos.x = 0;
	StartPos.y = 0;
	StartPos.z = 0;

	Physics_Rotation = btQuaternion(0, 0, 1, 0);

	Forward_Timer = 0;
	Ground_speed = 70;

	Limit_Look_Up = -45;
	Limit_Look_Down = 45;

	flag_Altered = 1;
	flag_Deleted = 0;

	Capsule_Mass = 1.0;
	Capsule_Radius = 4.4;
	Capsule_Height = 17.0;

	PlayerHeight = 16.0;

	TurnRate = 0.000005;

	Compenstate = 100;

	flag_mOnGround = 1;
	flag_IsMOving = 0;
	flag_IsMOving_Back = 0;
	flag_IsMOving_Right = 0;
	flag_IsMOving_Left = 0;

	Selected_Entity_Index = 0;
	Selected_Object_Name[0] = 0;

	flag_mJump = 0;
	mJumpSpeed = 4.5;

	flag_Physics_Debug_On = 1;

	FileViewItem = nullptr;

	strcpy(Player_Name, "Player_1");
}


Base_Player::~Base_Player()
{
}


// *************************************************************************
// *	  		Rotate_FromCam:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void Base_Player::Rotate_FromCam(const Ogre::Vector3 axis, float delta, bool normalize)
{
	float test = (delta * 180) / 3.14159265359;

	btTransform xform = Phys_Body->getWorldTransform();
	btMatrix3x3 R = xform.getBasis();
	R = R * btMatrix3x3(btQuaternion(btVector3(axis[0], axis[1], axis[2]), test));

	xform.setBasis(R);
	Phys_Body->setWorldTransform(xform);
}

