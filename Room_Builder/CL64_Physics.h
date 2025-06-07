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
class CL64_Physics
{
public:

	CL64_Physics(void);
	~CL64_Physics(void);

	void Reset_Class();
	void Init_Bullet();
	void ShutDown_Bullet();
	void Clear_Trimesh();
	void Show_Debug_Area(bool Show);
	void Show_Debug_Objects(bool Show);
	bool Create_New_Trimesh(Ogre::Entity* Entity, Ogre::SceneNode* Node);
	void Reset_Physics(void);
	void Reset_Player(void);
	void Reset_Scene(bool resetPlayer);
	void Set_Physics_Dimensions(int Object_Index);

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape*> collisionShapes;

	btBvhTriangleMeshShape* mShape;
	btDefaultMotionState* myMotionState;
	btRigidBody* Area_Phys_Body;
	btTriangleMesh* triMesh;

	bool flag_GD_Physics_On;
	bool flag_Physics_Dlg_Active;
	bool flag_TriMesh_Created;
	bool flag_Debug_All;

};


