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
class CL64_Com_Player
{
public:

	CL64_Com_Player(void);
	~CL64_Com_Player(void);

	void Reset_Class(void);

	void Create_New_Player(const char* Name, bool From_File);
	
	void Show_Player_And_Physics(bool Show);
	void Show_Player_Physics(bool Show);
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
	int Col_Object_Trigger;

	int Round;
	float Distance;
	int Life_Time;
	float Ray_End_Gravity;

	bool flag_AddGravity;
	bool flag_Is_On_Ground;
	bool flag_Show_Physics_Debug;


	int Last_Message_Index;

private:

	void Initialize(Base_Player* p_Player) const;
	void InitializePhysics(Base_Player* p_Player, const Ogre::Vector3& Pos) const;

};

