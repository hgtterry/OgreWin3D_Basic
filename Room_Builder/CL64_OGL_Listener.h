/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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

#include "Ogre.h"

class CL64_OGL_Listener : public Ogre::RenderQueueListener
{
public:
	CL64_OGL_Listener(void);
	~CL64_OGL_Listener(void);

	float RX;
	float RZ;

	int Selected_Face_Group;

	bool Flag_ShowFaces;
	bool Flag_ShowBoundingBox;
	bool Flag_ShowPoints;
	bool Flag_ShowBones;
	bool Flag_ShowNormals;
	bool Flag_ShowTextured;

	bool flag_ShowOnlySubFaces;

private:

	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& skipThisInvocation);

	virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& repeatThisInvocation);

	void PreRender();
	void PostRender();
	void Render_Loop();
	void Translate(void);

	bool MeshData_Render_Textures(void);
	bool MeshData_Textured_Groups(int Count);

	void MeshData_Render_Faces(void);
	void MeshData_Face_Groups(int Count);

	void MeshData_Render_Points(void);
	void MeshData_Points_Groups(int Count);

	void MeshData_Render_Normals(void);
	void MeshData_Normals_Groups(int Count);

	void MeshData_RenderBones();

	void MeshData_Render_BoundingBox(void);

	void RenderCrossHair(void);

	float Hair_1PosX;
	float Hair_1PosY;
	float Hair_1PosZ;

	float Hair_1RotX;
	float Hair_1RotY;
	float Hair_1RotZ;

	bool Light_Activated;
};

