/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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

	char Selected_Motion_Name[MAX_PATH];

	bool flag_Motion_Playing;
	bool flag_Motion_Paused;
	bool flag_IsAnimated;

	float AnimationScale;
};

