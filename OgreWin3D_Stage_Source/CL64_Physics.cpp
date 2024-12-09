/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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
#include "CL64_Physics.h"

CL64_Physics::CL64_Physics(void)
{
}

CL64_Physics::~CL64_Physics(void)
{
}

// *************************************************************************
//				Set_Physics_New:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Physics::Set_Physics_New(int Index)
{
	// Rotation
	App->CL_Scene->B_Object[Index]->Physics_Quat = App->CL_Scene->B_Object[Index]->Object_Node->getOrientation();

	float w = App->CL_Scene->B_Object[Index]->Physics_Quat.w;
	float x = App->CL_Scene->B_Object[Index]->Physics_Quat.x;
	float y = App->CL_Scene->B_Object[Index]->Physics_Quat.y;
	float z = App->CL_Scene->B_Object[Index]->Physics_Quat.z;

	App->CL_Scene->B_Object[Index]->Phys_Body->getWorldTransform().setRotation(btQuaternion(x, y, z, w));

	// Scale
	App->CL_Scene->B_Object[Index]->Object_Node->setScale(App->CL_Scene->B_Object[Index]->Mesh_Scale);

	Ogre::Vector3 Scale = App->CL_Scene->B_Object[Index]->Object_Node->getScale();
	App->CL_Scene->B_Object[Index]->Phys_Body->getCollisionShape()->setLocalScaling(btVector3(Scale.x, Scale.y, Scale.z));

	// Position
	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	App->CL_Scene->B_Object[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));
	App->CL_Scene->B_Object[Index]->Physics_Pos = Centre;

	// All Good
	App->CL_Scene->B_Object[Index]->Physics_Valid = 1;
}

// *************************************************************************
// *				UpDate_Physics_And_Visuals Terry Flanigtan		 	   *
// *************************************************************************
void CL64_Physics::UpDate_Physics_And_Visuals(int Index)
{
	if (App->CL_Scene->B_Object[Index]->Shape == Enums::Shape_TriMesh)
	{

	}
	else
	{
		if (App->CL_Scene->B_Object[Index]->Physics_Valid == 1)
		{
			Set_Physics_Position(Index);
		}
	}


	//App->SBC_Markers->MarkerBB_Addjust(Index);

	//// Needs Looking at
	//App->CL_Scene->B_Object[Index]->Altered = 1;
	//App->SBC_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
	//App->CL_Scene->Scene_Modified = 1;
}

// *************************************************************************
// *	  				Set_Physics_Position Terry Flanigan				   *
// *************************************************************************
void CL64_Physics::Set_Physics_Position(int Index)
{
	Ogre::Vector3 Centre = App->CL_Scene->B_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	App->CL_Scene->B_Object[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));
	App->CL_Scene->B_Object[Index]->Physics_Pos = Centre;
}


// *************************************************************************
// *		 Enable_Physics:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Physics::Enable_Physics(bool Enable)
{
	int f = App->CL_Scene->B_Player[0]->Phys_Body->getCollisionFlags();

	//App->CL_Ogre->Ogre3D_Listener->Dubug_Physics_Draw = Enable;
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = Enable;
	App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = Enable;

	//App->CL_Player->ShowDebug = Enable;

	App->CL_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f ^ btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

}

// *************************************************************************
// *	  		Reset_Triggers:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Physics::Reset_Triggers(void)
{
	App->CL_Collision->DoMove = 0;

	if (App->CL_Scene->flag_GameMode_Running == 0)
	{
		App->CL_SoundMgr->SoundEngine->stopAllSounds();
	}

	Ogre::Vector3 M_Pos;
	Ogre::Vector3 P_Pos;

	int Count = 0;
	int Total = App->CL_Scene->Object_Count;

	while (Count < Total)
	{
		if (App->CL_Scene->B_Object[Count]->Deleted == 0)
		{
			// ------------------------------------- Move Entities
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Move)
			{
				int ObjectToMove = App->CL_Scene->B_Object[Count]->S_MoveType[0]->Object_To_Move_Index;

				M_Pos = App->CL_Scene->B_Object[ObjectToMove]->Mesh_Pos;
				P_Pos = App->CL_Scene->B_Object[ObjectToMove]->Physics_Pos;

				App->CL_Scene->B_Object[Count]->S_MoveType[0]->MeshPos = Ogre::Vector3(M_Pos);
				App->CL_Scene->B_Object[Count]->S_MoveType[0]->PhysicsPos = Ogre::Vector3(P_Pos);

				App->CL_Scene->B_Object[ObjectToMove]->Object_Node->setPosition(M_Pos);
				App->CL_Scene->B_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(P_Pos.x, P_Pos.y, P_Pos.z));

				App->CL_Scene->B_Object[Count]->Triggered = 0;
			}

			// ------------------------------------- Colectables
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Colectable)
			{
				App->CL_Scene->B_Object[Count]->Object_Ent->setVisible(TRUE);

				App->CL_Scene->B_Object[Count]->Object_Node->setPosition(App->CL_Scene->B_Object[Count]->Mesh_Pos);

				P_Pos = App->CL_Scene->B_Object[Count]->Physics_Pos;
				App->CL_Scene->B_Object[Count]->Phys_Body->getWorldTransform().setOrigin(btVector3(P_Pos.x, P_Pos.y, P_Pos.z));

				App->CL_Scene->B_Object[Count]->Triggered = 0;

			}

			// ------------------------------------- Messages
			if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Message)
			{
				App->CL_Scene->B_Object[Count]->Show_Message_Flag = 0;
			}
		}

		Count++;
	}

	Count = 0;
	Total = App->CL_Scene->Counters_Count;
	while (Count < Total)
	{
		App->CL_Scene->B_Counter[Count]->Counter = App->CL_Scene->B_Counter[Count]->Start_Value;
		Count++;
	}
}

// *************************************************************************
// *	  		Reset_Physics:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Physics::Reset_Physics(void)
{
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;

	float w = 1;
	float x = 0;
	float y = 0;
	float z = 0;

	int Count = 0;
	while (Count < App->CL_Scene->Object_Count)
	{
		if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Dynamic)
		{
			btVector3 zeroVector(0, 0, 0);

			x = App->CL_Scene->B_Object[Count]->Physics_Pos.x;
			y = App->CL_Scene->B_Object[Count]->Physics_Pos.y;
			z = App->CL_Scene->B_Object[Count]->Physics_Pos.z;
			btVector3 initialPosition(x, y, z);

			btTransform startTransform;
			startTransform.setIdentity();

			startTransform.setRotation(btQuaternion(App->CL_Scene->B_Object[Count]->Physics_Quat.x,
				App->CL_Scene->B_Object[Count]->Physics_Quat.y,
				App->CL_Scene->B_Object[Count]->Physics_Quat.z,
				App->CL_Scene->B_Object[Count]->Physics_Quat.w));

			startTransform.setOrigin(initialPosition);

			App->CL_Scene->B_Object[Count]->Phys_Body->clearForces();
			App->CL_Scene->B_Object[Count]->Phys_Body->setLinearVelocity(zeroVector);
			App->CL_Scene->B_Object[Count]->Phys_Body->setAngularVelocity(zeroVector);

			App->CL_Scene->B_Object[Count]->Phys_Body->setWorldTransform(startTransform);
			App->CL_Scene->B_Object[Count]->Phys_Body->getMotionState()->setWorldTransform(startTransform);
			App->CL_Scene->B_Object[Count]->Phys_Body->activate(true);

			App->CL_Scene->B_Object[Count]->Object_Node->setPosition(App->CL_Scene->B_Object[Count]->Mesh_Pos);
			App->CL_Scene->B_Object[Count]->Object_Node->setOrientation(App->CL_Scene->B_Object[Count]->Mesh_Quat);

		}

		Count++;
	}

	if (App->CL_Scene->flag_Player_Added == 1)// && GD_Reset_Player == 1)
	{
		btVector3 zeroVector(0, 0, 0);

		x = App->CL_Scene->B_Player[0]->StartPos.x;
		y = App->CL_Scene->B_Player[0]->StartPos.y;
		z = App->CL_Scene->B_Player[0]->StartPos.z;

		btVector3 initialPosition(x, y, z);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setRotation(btQuaternion(1.0f, 0.0f, 0.0f, 0.0f));
		startTransform.setOrigin(initialPosition);

		App->CL_Scene->B_Player[0]->Phys_Body->clearForces();
		App->CL_Scene->B_Player[0]->Phys_Body->setLinearVelocity(zeroVector);
		App->CL_Scene->B_Player[0]->Phys_Body->setAngularVelocity(zeroVector);

		App->CL_Scene->B_Player[0]->Phys_Body->setWorldTransform(startTransform);
		App->CL_Scene->B_Player[0]->Phys_Body->getMotionState()->setWorldTransform(startTransform);
		App->CL_Scene->B_Player[0]->Phys_Body->activate(true);

		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);

		App->CL_Player->Set_Player_Physics_Position(0);
	}

	//App->CL_Ogre->OgreListener->GD_Run_Physics = 1;
}

// *************************************************************************
// *	  		Reset_Scene:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Physics::Reset_Scene(void)
{
	int Saved = App->CL_Ogre->Ogre3D_Listener->CameraMode;
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;

	App->CL_Physics->Reset_Physics();
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;
	App->CL_Physics->Reset_Triggers();
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;

	App->CL_Ogre->camNode->setOrientation(1,0,0,0);
	App->CL_Scene->B_Player[0]->CameraPitch_Node->setOrientation(1, 0, 0, 0);

	App->CL_TopDlg->flag_Toggle_Cam_FirstMode = 1;
	App->CL_TopDlg->flag_Toggle_Cam_FreeMode = 0;
	RedrawWindow(App->CL_TopDlg->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	App->CL_Com_Environments->Set_Environment_GameMode(0);
	App->CL_Player->Show_Player_And_Physics(false);
}
