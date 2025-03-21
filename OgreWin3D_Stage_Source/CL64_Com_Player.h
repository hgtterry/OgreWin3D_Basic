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

#pragma once
class CL64_Com_Player
{
public:

	CL64_Com_Player(void);
	~CL64_Com_Player(void);

	void Reset_Class(void);

	void Create_Player_Object(void);
	void Initialize() const;

	void Show_Player_And_Physics(bool Show);
	void Show_Physics(bool Show);
	void Adjust_Capsule(void);

	void Update_Player(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);
	void Update_Velocity(float dt);
	void Get_Height(void);

	void Set_Player_Physics_Position(int Index);
	void Set_Player_Position(Ogre::Vector3 Position);
	void Set_Player_Rotation(btQuaternion Rotation);

	void Set_Player_GroundSpeed(float GroundSpeed);
	void Reset_Player();
	void Rename_Player(int Index);
	int CheckNames_Player(char* Name);

	void Check_Collisions(void);

	Ogre::Vector3 Current_Position;
	btVector3 Physics_Position;
	btQuaternion Physics_Rotation;

	btVector3 mMoveDirection;
	btVector3 mWorld_Height;

	int Col_numManifolds;
	int Col_Player_Index;
	int Col_Usage_Index;
	int Col_Object_Index;

	int Round;
	float Distance;
	int Life_Time;
	float Ray_End_Gravity;

	bool flag_AddGravity;
	bool flag_Is_On_Ground;
	bool flag_Show_Physics_Debug;


	int Last_Message_Index;

};

