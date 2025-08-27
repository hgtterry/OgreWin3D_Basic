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
#include "CL64_Collision.h"

CL64_Collision::CL64_Collision(void)
{
	ObjectIndex = 0;
	flag_DoMove = 0;

	FinalPosition = 0;
	Old_Sound_Index = -1;

	x = 0;
	y = 0;
	z = 0;

	px = 0;
	py = 0;
	pz = 0;
}

CL64_Collision::~CL64_Collision(void)
{
}

// *************************************************************************
// *				Do_Teleport:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Collision::Do_Teleport(int Index)
{
	if (App->CL_Scene->B_Object[Index]->S_Teleport[0]->flag_Counter_Disabled == 1 || App->CL_Scene->B_Object[Index]->flag_OverRide_Counter == true)
	{
		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(App->CL_Scene->B_Object[Index]->S_Teleport[0]->Physics_Position);
		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Object[Index]->S_Teleport[0]->Physics_Rotation);

		if (App->CL_Scene->B_Object[Index]->S_Teleport[0]->flag_Play == true)
		{
			char Sound[MAX_PATH];
			strcpy(Sound, App->CL_SoundMgr->Default_Folder);
			strcat(Sound, "\\Media\\Sounds\\");
			strcat(Sound, App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File);

			//App->CL_Scene->B_Object[Index]->S_Teleport[0]-> = App->CL_SoundMgr->SoundEngine->play2D(Sound, false, true, true);
			//App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File->setVolume(App->CL_Scene->B_Object[Index]->S_Teleport[0]->SndVolume);
			//App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File->setIsPaused(false);

			// Environment
			/*if (App->CL_Editor_Com->flag_GameMode_Running == 1)
			{
				App->CL_Com_Environments->Set_Environment_By_Index(0, Old_Sound_Index);
				App->CL_Com_Environments->Set_Environment_By_Index(1, Index);

				Old_Sound_Index = Index;
			}
			else*/
			{
				//App->CL_Com_Environments->Set_Environment_By_Index(0, Index);

				//Old_Sound_Index = Index;

			}
		}

		return true;
	}

	
	int Trigger_Value = App->CL_Scene->B_Object[Index]->S_Teleport[0]->Trigger_Value;
	int CounterIndex = App->CL_Scene->B_Object[Index]->S_Teleport[0]->Counter_ID;

	//if (App->CL_Scene->B_Counter[CounterIndex]->Counter >= Trigger_Value)
	{
		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(App->CL_Scene->B_Object[Index]->S_Teleport[0]->Physics_Position);
		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Object[Index]->S_Teleport[0]->Physics_Rotation);

		if (App->CL_Scene->B_Object[Index]->S_Teleport[0]->flag_Play == true)
		{
			/*char Sound[MAX_PATH];
			strcpy(Sound, App->CL_SoundMgr->Default_Folder);
			strcat(Sound, "\\Media\\Sounds\\");
			strcat(Sound, App->CL_Scene->B_Object[Index]->S_Teleport[0]->Sound_File);*/

			//App->CL_Scene->B_Object[Index]->S_Teleport[0]->SndFile = App->CL_SoundMgr->SoundEngine->play2D(Sound, false, true, true);
			//App->CL_Scene->B_Object[Index]->S_Teleport[0]->SndFile->setVolume(App->CL_Editor_Com->B_Object[Index]->S_Teleport[0]->SndVolume);
			//App->CL_Scene->B_Object[Index]->S_Teleport[0]->SndFile->setIsPaused(false);
		}

		// Environment
		/*if (App->CL_Scene->flag_GameMode_Running == 1)
		{
			App->CL_Com_Environments->Set_Environment_By_Index(0, Old_Sound_Index);
			App->CL_Com_Environments->Set_Environment_By_Index(1, Index);

			Old_Sound_Index = Index;

		}
		else
		{
			App->CL_Com_Environments->Set_Environment_By_Index(0, Index);

			Old_Sound_Index = Index;

		}*/

		return true;

	}

	return true;
}

// *************************************************************************
// *			Do_Collectable:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Collision::Do_Collectable(int Index)
{
	/*App->CL_Editor_Com->B_Object[Index]->flag_Triggered = 1;

	App->CL_Editor_Com->B_Object[Index]->Object_Ent->setVisible(0);
	App->CL_Editor_Com->B_Object[Index]->Object_Node->setPosition(100, 100, 100);
	App->CL_Editor_Com->B_Object[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(100, 100, 100));

	if (App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->flag_Play == 1)
	{
		char Sound[1024];
		strcpy(Sound, App->CL_SoundMgr->Default_Folder);
		strcat(Sound, "\\Media\\Sounds\\");
		strcat(Sound, App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->Sound_File);

		App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->SndFile = App->CL_SoundMgr->SoundEngine->play2D(Sound, false, true, true);
		App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->SndFile->setVolume(App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->SndVolume);
		App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->SndFile->setIsPaused(false);
	}

	if (App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->flag_Counter_Disabled == 0)
	{
		int CouterID = App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->Counter_ID;

		if (App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->Maths == 1)
		{
			App->CL_Editor_Com->B_Counter[CouterID]->Counter += App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->Value;
			return 1;
		}

		if (App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->Maths == 2)
		{
			App->CL_Editor_Com->B_Counter[CouterID]->Counter += -App->CL_Editor_Com->B_Object[Index]->S_Collectable[0]->Value;
			return 1;
		}
	}*/

	return 1;
}

// *************************************************************************
// *			Move_Entity:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Collision::Move_Entity_Collision(int Index)
{
	/*Ogre::Vector3 M_Pos;
	Ogre::Vector3 P_Pos;
	ObjectIndex = Index;

	int ObjectToMove = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Object_To_Move_Index;

	if (App->CL_Editor_Com->B_Object[ObjectToMove]->flag_Deleted == 0)
	{
		if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->flag_Counter_Disabled == 1 || App->CL_Editor_Com->B_Object[Index]->flag_OverRide_Counter == 1)
		{
			Set_Move_Entity(Index);
			return 1;
		}

		int Trigger_Value = App->CL_Editor_Com->B_Object[Index]->S_MoveType[0]->Trigger_Value;
		int CounterIndex = App->CL_Editor_Com->B_Object[Index]->S_MoveType[0]->Counter_ID;

		if (App->CL_Editor_Com->B_Counter[CounterIndex]->Counter >= Trigger_Value)
		{
			Set_Move_Entity(Index);
			return 1;
		}
	}*/

	return 1;
}

// *************************************************************************
// *		Set_Move_Entity:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void CL64_Collision::Set_Move_Entity(int Index)
{
	//Ogre::Vector3 M_Pos;
	//Ogre::Vector3 P_Pos;
	//ObjectIndex = Index;

	//int ObjectToMove = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Object_To_Move_Index;

	//App->CL_Editor_Com->B_Object[Index]->flag_Triggered = 1;

	//M_Pos = App->CL_Editor_Com->B_Object[ObjectToMove]->Mesh_Pos;
	//P_Pos = App->CL_Editor_Com->B_Object[ObjectToMove]->Physics_Pos;

	//App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos = Ogre::Vector3(M_Pos);
	//App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos = Ogre::Vector3(P_Pos);

	//x = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.x;
	//y = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.y;
	//z = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.z;

	//px = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.x;
	//py = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.y;
	//pz = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.z;

	//if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->WhatDirection == Enums::Axis_x)
	//{
	//	FinalPosition = x + App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Move_Distance;
	//}

	//if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->WhatDirection == Enums::Axis_y)
	//{
	//	FinalPosition = y + App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Move_Distance;
	//}

	//if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->WhatDirection == Enums::Axis_z)
	//{
	//	FinalPosition = z + App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Move_Distance;
	//}

	//if (App->CL_Editor_Com->B_Object[Index]->flag_Play_Sound == 1)
	//{
	//	char Sound[MAX_PATH];
	//	strcpy(Sound, App->CL_SoundMgr->Default_Folder);
	//	strcat(Sound, "\\Media\\Sounds\\");
	//	strcat(Sound, App->CL_Editor_Com->B_Object[ObjectIndex]->Sound_File);

	//	App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile = App->CL_SoundMgr->SoundEngine->play2D(Sound, false, true, true);
	//	App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->setVolume(App->CL_Editor_Com->B_Object[ObjectIndex]->SndVolume);
	//	App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->setIsPaused(false);
	//}

	//flag_DoMove = 1; // Trigger Ogre Listener to update
}

// *************************************************************************
// *			MoveObject_Listener:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Collision::MoveObject_Listener(Ogre::Real Time)
{
	//if (App->CL_Editor_Com->B_Object[ObjectIndex]->flag_Deleted == 0)
	//{
	//	// X Axis
	//	if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->WhatDirection == Enums::Axis_x)
	//	{

	//		if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->flag_IsNegative == false)
	//		{
	//			int ObjectToMove = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Object_To_Move_Index;

	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.x += App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;
	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.x += App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;

	//			x = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.x;
	//			px = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.x;

	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Object_Node->setPosition(x, y, z);
	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(px, py, pz));

	//			if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.x > FinalPosition)
	//			{
	//				if (App->CL_Editor_Com->B_Object[ObjectIndex]->flag_Play_Sound == 1)
	//				{
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->stop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->drop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile = NULL;
	//				}

	//				flag_DoMove = 0; // Stop Listener
	//			}

	//			return;
	//		}
	//		else
	//		{
	//			int ObjectToMove = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Object_To_Move_Index;

	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.x -= App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;
	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.x -= App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;

	//			x = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.x;
	//			px = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.x;

	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Object_Node->setPosition(x, y, z);
	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(px, py, pz));

	//			if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.x < FinalPosition)
	//			{
	//				if (App->CL_Editor_Com->B_Object[ObjectIndex]->flag_Play_Sound == 1)
	//				{
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->stop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->drop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile = NULL;
	//				}

	//				flag_DoMove = 0; // Stop Listener
	//			}

	//			return;
	//		}
	//	}

	//	// Y Axis
	//	if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->WhatDirection == Enums::Axis_y)
	//	{
	//		if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->flag_IsNegative == false)
	//		{
	//			int ObjectToMove = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Object_To_Move_Index;

	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.y += App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;
	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.y += App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;

	//			y = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.y;
	//			py = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.y;

	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Object_Node->setPosition(x, y, z);
	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(px, py, pz));

	//			if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.y > FinalPosition)
	//			{
	//				if (App->CL_Editor_Com->B_Object[ObjectIndex]->flag_Play_Sound == 1)
	//				{
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->stop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->drop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile = NULL;
	//				}

	//				flag_DoMove = 0; // Stop Listener
	//			}

	//			return;
	//		}
	//		else
	//		{
	//			int ObjectToMove = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Object_To_Move_Index;

	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.y -= App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;
	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.y -= App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;

	//			y = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.y;
	//			py = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.y;

	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Object_Node->setPosition(x, y, z);
	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(px, py, pz));

	//			if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.y < FinalPosition)
	//			{
	//				if (App->CL_Editor_Com->B_Object[ObjectIndex]->flag_Play_Sound == 1)
	//				{
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->stop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->drop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile = NULL;
	//				}

	//				flag_DoMove = 0; // Stop Listener
	//			}

	//			return;
	//		}
	//	}

	//	// Z Axis
	//	if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->WhatDirection == Enums::Axis_z)
	//	{
	//		if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->flag_IsNegative == false)
	//		{
	//			int ObjectToMove = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Object_To_Move_Index;

	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.z += App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;
	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.z += App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;

	//			z = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.z;
	//			pz = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.z;

	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Object_Node->setPosition(x, y, z);
	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(px, py, pz));

	//			if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.z > FinalPosition)
	//			{
	//				if (App->CL_Editor_Com->B_Object[ObjectIndex]->flag_Play_Sound == 1)
	//				{
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->stop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->drop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile = NULL;
	//				}

	//				flag_DoMove = 0; // Stop Listener
	//			}

	//			return;
	//		}
	//		else
	//		{
	//			int ObjectToMove = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Object_To_Move_Index;

	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.z -= App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;
	//			App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.z -= App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->Speed * Time;

	//			z = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.z;
	//			pz = App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->PhysicsPos.z;

	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Object_Node->setPosition(x, y, z);
	//			App->CL_Editor_Com->B_Object[ObjectToMove]->Phys_Body->getWorldTransform().setOrigin(btVector3(px, py, pz));

	//			if (App->CL_Editor_Com->B_Object[ObjectIndex]->S_MoveType[0]->MeshPos.z < FinalPosition)
	//			{
	//				if (App->CL_Editor_Com->B_Object[ObjectIndex]->flag_Play_Sound == 1)
	//				{
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->stop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile->drop();
	//					App->CL_Editor_Com->B_Object[ObjectIndex]->SndFile = NULL;
	//				}

	//				flag_DoMove = 0; // Stop Listener
	//			}

	//			return;
	//		}
	//	}
	//}

	return;
}

// **************************************************************************
// *				Play_Sound:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
bool CL64_Collision::Play_Sound(int Index)
{
	char buff[MAX_PATH];
	strcpy(buff, App->CL_SoundMgr->Default_Folder);
	strcat(buff, "\\Media\\Sounds\\");

	strcat(buff, App->CL_Scene->B_Object[Index]->Sound_File);

	App->CL_SoundMgr->SndFile = App->CL_SoundMgr->SoundEngine->play2D(buff, false, true, true);
	App->CL_SoundMgr->SndFile->setVolume(App->CL_Scene->B_Object[Index]->SndVolume);
	App->CL_SoundMgr->SndFile->setIsPaused(false);

	App->CL_Scene->B_Object[Index]->flag_Triggered = 1;

	return 1;
}

// *************************************************************************
// *			Message_Entity:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Collision::Message_Entity(int Index)
{
	//if (App->CL_Editor_Com->B_Object[Index]->flag_Triggered == 1) // Retrigger Yes No
	//{
	//	return 1;
	//}

	////-----------------  Do Action

	//if (App->CL_Editor_Com->B_Object[Index]->S_Message[0]->flag_Counter_Disabled == 1)
	//{
	//	App->CL_Editor_Com->B_Object[Index]->flag_Show_Message_Flag = 1;
	//	App->CL_Editor_Com->B_Object[Index]->flag_Triggered = 1;
	//	return 1;
	//}

	//int Trigger_Value = App->CL_Editor_Com->B_Object[Index]->S_Message[0]->Trigger_Value;
	//int CounterIndex = App->CL_Editor_Com->B_Object[Index]->S_Message[0]->Counter_ID;

	//if (App->CL_Editor_Com->B_Counter[CounterIndex]->Counter < Trigger_Value)
	//{
	//	App->CL_Editor_Com->B_Object[Index]->flag_Show_Message_Flag = 1;
	//	App->CL_Editor_Com->B_Object[Index]->flag_Triggered = 1;
	//	return 1;
	//}

	return 1;
}
 
// *************************************************************************
// *			Do_Environment:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Collision::Do_Environment(int Index)
{

	App->CL_Com_Environments->Stop_All_Sounds(Index);
	App->CL_Com_Environments->Set_Environment_By_Index(true, Index);
	App->CL_Com_Environments->Reset_Environment_Triggers();

	App->CL_Scene->B_Object[Index]->flag_Triggered = 1;

	return 1;
}
