/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Player.h"

CL64_Player::CL64_Player(void)
{
	mMoveDirection.setValue(0, 0, 0);
	Player_Count = 0;

	mWorld_Height.setValue(0, 0, 0);

	AddGravity = 0;
	Is_On_Ground = 0;
}

CL64_Player::~CL64_Player(void)
{
}

// *************************************************************************
// *	  	Create_Player_Object:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Player::Create_Player_Object(void)
{
	int Index = Player_Count;

	App->CL_Scene->B_Player[Index] = new Base_Player();

	Initialize();

	App->CL_Scene->B_Player[Index]->CameraPitch = App->CL_Ogre->mSceneMgr->createCamera("PlayerPitch");

	App->CL_Scene->B_Player[Index]->CameraPitch_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	App->CL_Scene->B_Player[Index]->CameraPitch_Node->attachObject(App->CL_Scene->B_Player[Index]->CameraPitch);
	App->CL_Scene->B_Player[Index]->CameraPitch_Node->setPosition(0, 0, 0);

	Player_Count++;

}

// *************************************************************************
// *	  			Initialize:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Player::Initialize() const
{
	Ogre::Vector3 Pos;

	int Index = Player_Count;

	Base_Player* pBase = App->CL_Scene->B_Player[Index];
	
	// ------------------- Ogre
	if (pBase->Player_Ent && pBase->Player_Node)
	{
		App->CL_Ogre->mSceneMgr->destroySceneNode(pBase->Player_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(pBase->Player_Ent);
		App->CL_Ogre->mSceneMgr->destroyCamera(pBase->CameraPitch);
		pBase->Player_Ent = nullptr;
		pBase->Player_Node = nullptr;
		pBase->CameraPitch = nullptr;
	}
	
	pBase->Player_Ent = App->CL_Ogre->mSceneMgr->createEntity("Player_1", "axes.mesh", App->CL_Ogre->App_Resource_Group);
	pBase->Player_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	pBase->Player_Node->attachObject(pBase->Player_Ent);

	pBase->Player_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	pBase->Player_Node->setVisible(false);

	Pos.x = pBase->StartPos.x;
	Pos.y = pBase->StartPos.y;
	Pos.z = pBase->StartPos.z;

	pBase->Player_Node->setPosition(Pos.x, Pos.y, Pos.z);
	
	// ------------------------ Bulet
	btVector3 pos = btVector3(Pos.x, Pos.y, Pos.z);
	
	btVector3 inertia = btVector3(0, 0, 0);
	btQuaternion rot = btQuaternion(0, 0, 0, 1);
	btDefaultMotionState* state = new btDefaultMotionState(btTransform(rot, pos));
	
	pBase->Phys_Shape = new btCapsuleShape(btScalar(pBase->Capsule_Radius), btScalar(pBase->Capsule_Height));
	pBase->Phys_Body = new btRigidBody(pBase->Capsule_Mass, state, pBase->Phys_Shape, inertia);
	
	pBase->Phys_Body->setSleepingThresholds(0.0, 0.0);
	pBase->Phys_Body->setAngularFactor(0.0);

	pBase->Phys_Body->setUserPointer(pBase->Player_Node);

	pBase->Phys_Body->setUserIndex(Enums::Usage_Player);

	int f = pBase->Phys_Body->getCollisionFlags();
	//pBase->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);
	App->CL_Bullet->dynamicsWorld->addRigidBody(pBase->Phys_Body);

	App->CL_Scene->flag_Player_Added = 1;
}

// *************************************************************************
// *			Show_Debug_Player:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Player::Show_Debug_Player(bool Show)
{
	int f = App->CL_Scene->B_Player[0]->Phys_Body->getCollisionFlags();

	if (Show == 1)
	{
		App->CL_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
	}
	else
	{
		App->CL_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f | (1 << 5));

		App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;
		App->CL_Ogre->RenderFrame(1);
		App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 1;
	}

}

// *************************************************************************
// *			Update_Player:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Player::Update_Player(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
	mWorld_Height = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin();

	Get_Height();
	
	Update_Velocity(deltaTimeStep);
}

// *************************************************************************
// *			Update_Velocity:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Player::Update_Velocity(float dt)
{
	btTransform transform;
	App->CL_Scene->B_Player[0]->Phys_Body->getMotionState()->getWorldTransform(transform);
	btMatrix3x3& basis = transform.getBasis();

	btMatrix3x3 inv = basis.transpose();

	btVector3 linearVelocity = inv * App->CL_Scene->B_Player[0]->Phys_Body->getLinearVelocity();


	if (Is_On_Ground == 1)// || mJump == 1)
	{
		btVector3 dv = mMoveDirection * ((App->CL_Scene->B_Player[0]->Ground_speed * 100)*dt);
		linearVelocity = dv;
	}
	else
	{
		if (AddGravity == 1)
		{
			linearVelocity[1] = 100;

		}
		else
		{
			linearVelocity[1] = 10;
		}
	}

	App->CL_Scene->B_Player[0]->Phys_Body->setLinearVelocity(basis * linearVelocity);

}

// *************************************************************************
// *				Get_Height:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Player::Get_Height(void)
{
	btVector3 Origin = App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin();
	btVector3 from = btVector3(Origin.getX(), Origin.getY(), Origin.getZ());
	btVector3 to = btVector3(Origin.getX(), Origin.getY() - 15, Origin.getZ());

	btCollisionWorld::ClosestRayResultCallback resultCallback(from, to);
	App->CL_Bullet->dynamicsWorld->rayTest(from, to, resultCallback);
	if (resultCallback.hasHit())
	{
		AddGravity = 0;
		Is_On_Ground = 1;
	}
	else
	{
		AddGravity = 1;
		Is_On_Ground = 0;
	}
}

// *************************************************************************
// *		Set_Player_Position:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Player::Set_Player_Position(Ogre::Vector3 Position)
{
	float px = Position.x;
	float py = Position.y;
	float pz = Position.z;

	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(btVector3(px, py, pz));

}

// *************************************************************************
// *		Set_Player_Rotation:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Player::Set_Player_Rotation(btQuaternion Rotation)
{
	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(Rotation);
}

// *************************************************************************
// *		Set_Player_GroundSpeed:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Player::Set_Player_GroundSpeed(float GroundSpeed)
{
	App->CL_Scene->B_Player[0]->Ground_speed = GroundSpeed;
}
