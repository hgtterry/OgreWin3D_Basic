/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#pragma once
class CL64_Bullet
{
public:

	CL64_Bullet(void);
	~CL64_Bullet(void);

	void Init_Bullet();
	void ShutDown_Bullet();
	void Clear_Trimesh();
	void Show_Debug_Area(bool Show);
	void Show_Debug_Objects(bool Show);

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape*> collisionShapes;

	btBvhTriangleMeshShape* mShape;
	btDefaultMotionState* myMotionState;
	btRigidBody* Phys_Body;
	btTriangleMesh* triMesh;

	bool GD_Physics_On;
	bool Physics_Dlg_Active;
	bool flag_TriMesh_Created;
	bool flag_Debug_All;
	bool Create_New_Trimesh(Ogre::Entity* Entity, Ogre::SceneNode* Node);
};


