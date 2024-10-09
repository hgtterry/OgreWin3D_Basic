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
	App->CL_Scene->V_Object[Index]->Physics_Quat = App->CL_Scene->V_Object[Index]->Object_Node->getOrientation();

	float w = App->CL_Scene->V_Object[Index]->Physics_Quat.w;
	float x = App->CL_Scene->V_Object[Index]->Physics_Quat.x;
	float y = App->CL_Scene->V_Object[Index]->Physics_Quat.y;
	float z = App->CL_Scene->V_Object[Index]->Physics_Quat.z;

	App->CL_Scene->V_Object[Index]->Phys_Body->getWorldTransform().setRotation(btQuaternion(x, y, z, w));

	// Scale
	App->CL_Scene->V_Object[Index]->Object_Node->setScale(App->CL_Scene->V_Object[Index]->Mesh_Scale);

	Ogre::Vector3 Scale = App->CL_Scene->V_Object[Index]->Object_Node->getScale();
	App->CL_Scene->V_Object[Index]->Phys_Body->getCollisionShape()->setLocalScaling(btVector3(Scale.x, Scale.y, Scale.z));

	// Position
	Ogre::Vector3 Centre = App->CL_Scene->V_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	App->CL_Scene->V_Object[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));
	App->CL_Scene->V_Object[Index]->Physics_Pos = Centre;

	// All Good
	App->CL_Scene->V_Object[Index]->Physics_Valid = 1;
}

// *************************************************************************
// *				UpDate_Physics_And_Visuals Terry Flanigtan		 	   *
// *************************************************************************
void CL64_Physics::UpDate_Physics_And_Visuals(int Index)
{
	if (App->CL_Scene->V_Object[Index]->Shape == Enums::Shape_TriMesh)
	{

	}
	else
	{
		if (App->CL_Scene->V_Object[Index]->Physics_Valid == 1)
		{
			Set_Physics_Position(Index);
		}
	}


	//App->SBC_Markers->MarkerBB_Addjust(Index);

	//// Needs Looking at
	//App->CL_Scene->V_Object[Index]->Altered = 1;
	//App->SBC_FileView->Mark_Altered(App->CL_Scene->V_Object[Index]->FileViewItem);
	//App->CL_Scene->Scene_Modified = 1;
}

// *************************************************************************
// *	  				Set_Physics_Position Terry Flanigan				   *
// *************************************************************************
void CL64_Physics::Set_Physics_Position(int Index)
{
	Ogre::Vector3 Centre = App->CL_Scene->V_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	App->CL_Scene->V_Object[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));
	App->CL_Scene->V_Object[Index]->Physics_Pos = Centre;
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

	if (App->CL_Scene->flag_GameMode_Running_Flag == 0)
	{
		App->CL_SoundMgr->SoundEngine->stopAllSounds();
	}

	Ogre::Vector3 M_Pos;
	Ogre::Vector3 P_Pos;

	int Count = 0;
	int Total = App->CL_Scene->Object_Count;

	while (Count < Total)
	{
		if (App->CL_Scene->V_Object[Count]->Deleted == 0)
		{
			// ------------------------------------- Move Entities
			if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Move)
			{
				int ObjectToMove = App->CL_Scene->V_Object[Count]->S_MoveType[0]->Object_To_Move_Index;

				M_Pos = App->CL_Scene->V_Object[ObjectToMove]->Mesh_Pos;
				P_Pos = App->CL_Scene->V_Object[ObjectToMove]->Physics_Pos;

				App->CL_Scene->V_Object[Count]->S_MoveType[0]->MeshPos = Ogre::Vector3(M_Pos);
				App->CL_Scene->V_Object[Count]->S_MoveType[0]->PhysicsPos = Ogre::Vector3(P_Pos);

				App->CL_Scene->V_Object[ObjectToMove]->Object_Node->setPosition(M_Pos);
				App->CL_Scene->V_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(P_Pos.x, P_Pos.y, P_Pos.z));

				App->CL_Scene->V_Object[Count]->Triggered = 0;
			}

			// ------------------------------------- Colectables
			if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Colectable)
			{
				App->CL_Scene->V_Object[Count]->Object_Ent->setVisible(TRUE);

				App->CL_Scene->V_Object[Count]->Object_Node->setPosition(App->CL_Scene->V_Object[Count]->Mesh_Pos);

				P_Pos = App->CL_Scene->V_Object[Count]->Physics_Pos;
				App->CL_Scene->V_Object[Count]->Phys_Body->getWorldTransform().setOrigin(btVector3(P_Pos.x, P_Pos.y, P_Pos.z));

				App->CL_Scene->V_Object[Count]->Triggered = 0;

			}

			// ------------------------------------- Messages
			if (App->CL_Scene->V_Object[Count]->Usage == Enums::Stage_Usage_Message)
			{
				App->CL_Scene->V_Object[Count]->Show_Message_Flag = 0;
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
