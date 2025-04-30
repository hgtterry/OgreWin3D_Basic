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
#include "CL64_Com_Player.h"

CL64_Com_Player::CL64_Com_Player(void)
{
	mMoveDirection.setValue(0, 0, 0);
	
	mWorld_Height.setValue(0, 0, 0);

	Col_numManifolds = 0;
	Col_Player_Index = 0;
	Col_Usage_Index = 0;
	Col_Object_Index = 0;

	Round = 0;
	Distance = 0;
	Life_Time = 0;
	Last_Message_Index = 0;

	Ray_End_Gravity = 15;

	flag_Show_Physics_Debug = 0;
	flag_AddGravity = 0;
	flag_Is_On_Ground = 0;
}

CL64_Com_Player::~CL64_Com_Player(void)
{
}

// *************************************************************************
// *	  		Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Com_Player::Reset_Class(void)
{
	int Count = 0;

	while (Count < App->CL_Editor_Com->Player_Count) // Remove Ogre Objects
	{
		if (App->CL_Editor_Com->B_Player[Count])
		{
			if (App->CL_Editor_Com->B_Player[Count]->Player_Ent && App->CL_Editor_Com->B_Player[Count]->Player_Node)
			{
				App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Editor_Com->B_Player[Count]->Player_Node);
				App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Editor_Com->B_Player[Count]->Player_Ent);
				App->CL_Ogre->mSceneMgr->destroyCamera(App->CL_Editor_Com->B_Player[Count]->CameraPitch);
				App->CL_Editor_Com->B_Player[Count]->Player_Ent = nullptr;
				App->CL_Editor_Com->B_Player[Count]->Player_Node = nullptr;
				App->CL_Editor_Com->B_Player[Count]->CameraPitch = nullptr;
			}

			delete App->CL_Editor_Com->B_Player[Count];
			App->CL_Editor_Com->B_Player[Count] = nullptr;
		}

		Count++;
	}

	App->CL_Editor_Com->Player_Count = 0;
	App->CL_Editor_Com->flag_Player_Added = 0;

}

// *************************************************************************
// *	  	Create_Player_Object:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Player::Create_Player_Object(void)
{
	int Index = App->CL_Editor_Com->Player_Count;

	App->CL_Editor_Com->B_Player[Index] = new Base_Player();
	
	Initialize();

	App->CL_Editor_Com->B_Player[Index]->CameraPitch = App->CL_Ogre->mSceneMgr->createCamera("PlayerPitch");

	App->CL_Editor_Com->B_Player[Index]->CameraPitch_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	App->CL_Editor_Com->B_Player[Index]->CameraPitch_Node->attachObject(App->CL_Editor_Com->B_Player[Index]->CameraPitch);
	App->CL_Editor_Com->B_Player[Index]->CameraPitch_Node->setPosition(0, 0, 0);

	strcpy(App->CL_Editor_Com->B_Player[0]->Player_Name, "Player_1");
	HTREEITEM Temp1 = App->CL_FileView->Add_Item(App->CL_FileView->FV_Players_Folder, (LPSTR)"Player_1", 0, true);
	App->CL_FileView->Set_FolderActive(App->CL_FileView->FV_Players_Folder);
	App->CL_FileView->SelectItem(Temp1);

	App->CL_Editor_Com->Player_Count++;

}

// *************************************************************************
// *	  			Initialize:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Player::Initialize() const
{
	Ogre::Vector3 Pos;

	int Index = App->CL_Editor_Com->Player_Count;

	Base_Player* pBase = App->CL_Editor_Com->B_Player[Index];
	
	pBase->Player_Ent = App->CL_Ogre->mSceneMgr->createEntity("Player_1", "Sinbad.mesh", App->CL_Ogre->App_Resource_Group);
	pBase->Player_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	pBase->Player_Node->attachObject(pBase->Player_Ent);
	
	pBase->Player_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	pBase->Player_Node->setVisible(false);
	pBase->Player_Node->scale(3, 3, 3);

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

	pBase->Phys_Body->setUserIndex(Enums::Obj_Usage_Player);

	int f = pBase->Phys_Body->getCollisionFlags();
	//pBase->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Editor_Com->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Editor_Com->B_Player[0]->Physics_Rotation);
	App->CL_Physics->dynamicsWorld->addRigidBody(pBase->Phys_Body);

	App->CL_Editor_Com->flag_Player_Added = 1;
	
}

// *************************************************************************
// *		Show_Player_And_Physics:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Player::Show_Player_And_Physics(bool Show)
{
	int f = App->CL_Editor_Com->B_Player[0]->Phys_Body->getCollisionFlags();

	if (Show == 1)
	{
		App->CL_Editor_Com->B_Player[0]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
		App->CL_Editor_Com->B_Player[0]->Player_Node->setVisible(true);
		//App->CL_Editor_Com->B_Player[0]->Player_Node->scale(3, 3, 3);
		flag_Show_Physics_Debug = 1;
	}
	else
	{
		App->CL_Editor_Com->B_Player[0]->Phys_Body->setCollisionFlags(f | (1 << 5));
		App->CL_Editor_Com->B_Player[0]->Player_Node->setVisible(false);
		//App->CL_Editor_Com->B_Player[0]->Player_Node->scale(0, 0, 0);
		flag_Show_Physics_Debug = 0;
	}

	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;
	App->CL_Ogre->Bullet_Debug_Listener->Clear_Debug_Render();
	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 1;

	//RedrawWindow(App->CL_Props_Dialogs->Player_Props_HWND, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *		Show_Physics:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Com_Player::Show_Physics(bool Show)
{
	int f = App->CL_Editor_Com->B_Player[0]->Phys_Body->getCollisionFlags();

	if (Show == 1)
	{
		App->CL_Editor_Com->B_Player[0]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
		flag_Show_Physics_Debug = 1;
	}
	else
	{
		App->CL_Editor_Com->B_Player[0]->Phys_Body->setCollisionFlags(f | (1 << 5));
		flag_Show_Physics_Debug = 0;
	}

	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;
	App->CL_Ogre->Bullet_Debug_Listener->Clear_Debug_Render();
	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 1;

	//RedrawWindow(App->CL_Props_Dialogs->Player_Props_HWND, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Adjust_CapsuleTerry:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Player::Adjust_Capsule(void)
{
	App->CL_Editor_Com->B_Player[0]->Phys_Shape = new btCapsuleShape(btScalar(App->CL_Editor_Com->B_Player[0]->Capsule_Radius), btScalar(App->CL_Editor_Com->B_Player[0]->Capsule_Height));
	App->CL_Editor_Com->B_Player[0]->Phys_Body->setCollisionShape(App->CL_Editor_Com->B_Player[0]->Phys_Shape);

	Ray_End_Gravity = (App->CL_Editor_Com->B_Player[0]->Capsule_Height + 12) / 2;
	Get_Height();
}

// *************************************************************************
// *			Update_Player:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Player::Update_Player(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
	mWorld_Height = App->CL_Editor_Com->B_Player[0]->Phys_Body->getWorldTransform().getOrigin();

	Get_Height();
	
	Update_Velocity(deltaTimeStep);
}

// *************************************************************************
// *			Update_Velocity:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Player::Update_Velocity(float dt)
{
	btTransform transform;
	App->CL_Editor_Com->B_Player[0]->Phys_Body->getMotionState()->getWorldTransform(transform);
	btMatrix3x3& basis = transform.getBasis();

	btMatrix3x3 inv = basis.transpose();

	btVector3 linearVelocity = inv * App->CL_Editor_Com->B_Player[0]->Phys_Body->getLinearVelocity();

	//flag_Is_On_Ground = 1;

	if (flag_Is_On_Ground == 1)// || mJump == 1)
	{
		btVector3 dv = mMoveDirection * ((App->CL_Editor_Com->B_Player[0]->Ground_speed * 100)*dt);
		linearVelocity = dv;
	}
	else
	{
		if (flag_AddGravity == 1)
		{
			linearVelocity[1] = 100;

		}
		else
		{
			linearVelocity[1] = 10;
		}
	}

	App->CL_Editor_Com->B_Player[0]->Phys_Body->setLinearVelocity(basis * linearVelocity);

}

// *************************************************************************
// *				Get_Height:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Player::Get_Height(void)
{
	btVector3 Origin = App->CL_Editor_Com->B_Player[0]->Phys_Body->getWorldTransform().getOrigin();
	btVector3 from = btVector3(Origin.getX(), Origin.getY(), Origin.getZ());
	btVector3 to = btVector3(Origin.getX(), Origin.getY() - Ray_End_Gravity, Origin.getZ());

	//App->CL_Gizmos->Crosshair_Node->setPosition(Origin.getX(), Origin.getY() - Ray_End_Gravity, Origin.getZ());

	btCollisionWorld::ClosestRayResultCallback resultCallback(from, to);
	App->CL_Physics->dynamicsWorld->rayTest(from, to, resultCallback);
	if (resultCallback.hasHit())
	{
		flag_AddGravity = 0;
		flag_Is_On_Ground = 1;
	}
	else
	{
		flag_AddGravity = 1;
		flag_Is_On_Ground = 0;
	}
}

// *************************************************************************
// *		Set_Player_Position:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Player::Set_Player_Position(Ogre::Vector3 Position)
{
	float px = Position.x;
	float py = Position.y;
	float pz = Position.z;

	App->CL_Editor_Com->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(btVector3(px, py, pz));

}

// *************************************************************************
// *	Set_Player_Physics_Position:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Player::Set_Player_Physics_Position(int Index)
{
	btTransform trans;
	App->CL_Editor_Com->B_Player[0]->Phys_Body->getMotionState()->getWorldTransform(trans);
	btQuaternion orientation = trans.getRotation();

	App->CL_Editor_Com->B_Player[0]->Player_Node->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY() + 2, trans.getOrigin().getZ()));
	App->CL_Editor_Com->B_Player[0]->Player_Node->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
	App->CL_Editor_Com->B_Player[0]->Player_Node->pitch(Ogre::Degree(180));

}

// *************************************************************************
// *		Set_Player_Rotation:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Com_Player::Set_Player_Rotation(btQuaternion Rotation)
{
	App->CL_Editor_Com->B_Player[0]->Phys_Body->getWorldTransform().setRotation(Rotation);
}

// *************************************************************************
// *		Set_Player_GroundSpeed:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Player::Set_Player_GroundSpeed(float GroundSpeed)
{
	App->CL_Editor_Com->B_Player[0]->Ground_speed = GroundSpeed;
}

// *************************************************************************
// *		Set_Player_GroundSpeed:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Player::Reset_Player()
{
	if (App->CL_Editor_Com->flag_Player_Added == 1)// && GD_Reset_Player == 1)
	{
		btVector3 zeroVector(0, 0, 0);

		float x = App->CL_Editor_Com->B_Player[0]->StartPos.x;
		float y = App->CL_Editor_Com->B_Player[0]->StartPos.y;
		float z = App->CL_Editor_Com->B_Player[0]->StartPos.z;

		btVector3 initialPosition(x, y, z);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setRotation(btQuaternion(1.0f, 0.0f, 0.0f, 0.0f));
		startTransform.setOrigin(initialPosition);

		App->CL_Editor_Com->B_Player[0]->Phys_Body->clearForces();
		App->CL_Editor_Com->B_Player[0]->Phys_Body->setLinearVelocity(zeroVector);
		App->CL_Editor_Com->B_Player[0]->Phys_Body->setAngularVelocity(zeroVector);

		App->CL_Editor_Com->B_Player[0]->Phys_Body->setWorldTransform(startTransform);
		App->CL_Editor_Com->B_Player[0]->Phys_Body->getMotionState()->setWorldTransform(startTransform);
		App->CL_Editor_Com->B_Player[0]->Phys_Body->activate(true);

		App->CL_Editor_Com->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Editor_Com->B_Player[0]->Physics_Rotation);

		Adjust_Capsule();
	}
}

// *************************************************************************
// *			Rename_Player:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Com_Player::Rename_Player(int Index)
{
	//strcpy(App->CL_Dialogs->btext, "Change Player Name");
	//strcpy(App->CL_Dialogs->Chr_Text, App->CL_Editor_Com->B_Player[0]->Player_Name);

	//App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Player);

	//if (App->CL_Dialogs->flag_Canceled == 1)
	//{
	//	return;
	//}

	//// Needs Duplicate Name test 
	//strcpy(App->CL_Editor_Com->B_Player[0]->Player_Name, App->CL_Dialogs->Chr_Text);

	//App->CL_Editor_Com->B_Player[0]->flag_Altered = 1;
	//App->CL_Editor_Com->flag_Scene_Modified = 1;
	//App->CL_FileView->Mark_Altered(App->CL_Editor_Com->B_Player[0]->FileViewItem);

	//App->CL_FileView->Change_Item_Name(App->CL_Editor_Com->B_Player[0]->FileViewItem, App->CL_Dialogs->Chr_Text);

}

// *************************************************************************
// *		 CheckNames_Player:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
int CL64_Com_Player::CheckNames_Player(char* Name)
{
	/*int Count = 0;
	int Total = App->CL_Editor_Com->Player_Count;

	while (Count < Total)
	{
		if (App->CL_Editor_Com->B_Player[0]->flag_Deleted == 0)
		{
			int Result = 1;
			Result = strcmp(App->CL_Editor_Com->B_Player[0]->Player_Name, Name);

			if (Result == 0)
			{
				return 1;
			}
		}

		Count++;
	}*/
	return 0;
}

// *************************************************************************
// *			Check_Collisions:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Player::Check_Collisions(void)
{
	int UsageIndex = 0;
	Col_Player_Index = 0;
	Col_Usage_Index = 0;
	Col_numManifolds = 0;

	/* Browse all collision pairs */
	Col_numManifolds = App->CL_Physics->dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < Col_numManifolds; i++)
	{
		btPersistentManifold* contactManifold = App->CL_Physics->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		Col_Player_Index = obA->getUserIndex();  // Should Be Player

		Col_Object_Index = obB->getUserIndex2(); // Object Index

		Col_Usage_Index = obB->getUserIndex();

		if (Col_Player_Index == Enums::Obj_Usage_Player)
		{
			if (Col_Usage_Index == 123)// && App->SBC_Scene->B_Object[Last_Message_Index]->Triggered == 1)
			{
				if (App->CL_Editor_Com->Object_Count > 0)
				{
					App->CL_Editor_Com->B_Object[Last_Message_Index]->flag_Show_Message_Flag = 0;
					App->CL_Editor_Com->B_Object[Last_Message_Index]->flag_Triggered = 0;
				}
			}
			else
			{
				// -------------------- Message Collision
				if (Col_Usage_Index == Enums::Obj_Usage_Message)
				{
					int numContacts = contactManifold->getNumContacts();
					for (int j = 0; j < numContacts; j++)
					{
						btManifoldPoint& pt = contactManifold->getContactPoint(j);

						Life_Time = pt.getLifeTime();
						Distance = pt.getDistance();
						Round = (int)Distance;

						if (Round < 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 0)
							{
								Last_Message_Index = Col_Object_Index;
								App->CL_Collision->Message_Entity(Col_Object_Index);
							}
						}
						else if (Round == 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 1)
							{
								App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Show_Message_Flag = 0;
								App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered = 0;

							}
						}
					}
				}

				// -------------------- Sound Collision
				if (Col_Usage_Index == Enums::Obj_Usage_Sound)
				{
					int numContacts = contactManifold->getNumContacts();
					for (int j = 0; j < numContacts; j++)
					{

						btManifoldPoint& pt = contactManifold->getContactPoint(j);

						Life_Time = pt.getLifeTime();
						Distance = pt.getDistance();
						Round = (int)Distance;

						if (Round < 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 0)
							{
								App->CL_Collision->Play_Sound(Col_Object_Index);
								//Last_ColisionIndex = Col_Object_Index;
							}
						}
						else if (Life_Time < 10)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 1)
							{
								App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered = 0;
							}
						}
					}
				}

				// -------------------- Move Collision
				if (Col_Usage_Index == Enums::Obj_Usage_Move)
				{

					int numContacts = contactManifold->getNumContacts();
					for (int j = 0; j < numContacts; j++)
					{
						btManifoldPoint& pt = contactManifold->getContactPoint(j);

						Life_Time = pt.getLifeTime();
						Distance = pt.getDistance();
						Round = (int)Distance;

						if (Round < 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 0)
							{

								App->CL_Collision->Move_Entity_Collision(Col_Object_Index);
							}
						}
						else if (Round == 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 1)
							{

							}
						}

					}
				}

				// -------------------- Collectable Collision
				if (Col_Usage_Index == Enums::Obj_Usage_Collectable)
				{
					int numContacts = contactManifold->getNumContacts();
					for (int j = 0; j < numContacts; j++)
					{
						if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 0)
						{
							App->CL_Collision->Do_Collectable(Col_Object_Index);
						}
					}
				}

				// -------------------- Teleport Collision
				if (Col_Usage_Index == Enums::Obj_Usage_Teleport)
				{
					int numContacts = contactManifold->getNumContacts();
					for (int j = 0; j < numContacts; j++)
					{
						App->CL_Collision->Do_Teleport(Col_Object_Index);

						btManifoldPoint& pt = contactManifold->getContactPoint(j);

						Life_Time = pt.getLifeTime();
						Distance = pt.getDistance();
						Round = (int)Distance;

						if (Round < 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 0)
							{
								App->CL_Collision->Do_Teleport(Col_Object_Index);
							}
						}
						else if (Round == 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 1)
							{

							}
						}
					}
				}

				// -------------------- EnvironEntity Collision
				if (Col_Usage_Index == Enums::Obj_Usage_EnvironEntity)
				{
					int numContacts = contactManifold->getNumContacts();
					for (int j = 0; j < numContacts; j++)
					{
						btManifoldPoint& pt = contactManifold->getContactPoint(j);

						Life_Time = pt.getLifeTime();
						Distance = pt.getDistance();
						Round = (int)Distance;

						if (Round < 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 0)
							{
								App->CL_Collision->Do_Environment(Col_Object_Index);
							}
						}
						else if (Round == 0)
						{
							if (App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered == 1)
							{
								App->CL_Editor_Com->B_Object[Col_Object_Index]->flag_Triggered = 0;
							}
						}
					}
				}

			}
		}
	}
}
