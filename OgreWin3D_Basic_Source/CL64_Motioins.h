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
class CL64_Motioins
{
public:

	CL64_Motioins(void);
	~CL64_Motioins(void);

	void Reset_Class(void);

	void Get_Motions(Ogre::Entity* Ogre_Entity);
	void Update_Motion(float deltaTime);

	void Pause_SelectedMotion(void);
	void Play_SelectedMotion(void);
	void Stop_SelectedMotion(void);
	void Motion_Set_Pose(void);

	void Update_MeshData(void);
	void UpdateBones_Orge(bool Reset);
	void AnimationExtract_Mesh(bool DefaultPose);

	void Get_AnimationInstance(Ogre::MeshPtr mesh,
		size_t& vertex_count,
		Ogre::Vector3*& vertices,
		size_t& index_count,
		unsigned long*& indices,
		int SubMesh,
		bool DefaultPose);

	Ogre::AnimationState* Animate_State;

	bool flag_Motion_Playing;
	bool flag_Motion_Paused;
	bool flag_IsAnimated;

	float AnimationScale;
};

