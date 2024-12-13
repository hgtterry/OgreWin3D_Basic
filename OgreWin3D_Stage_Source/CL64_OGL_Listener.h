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

#include "Ogre.h"

class CL64_OGL_Listener : public Ogre::RenderQueueListener
{
public:
	CL64_OGL_Listener(void);
	~CL64_OGL_Listener(void);

	float RX;
	float RZ;

	float MarkerBox_PosX;
	float MarkerBox_PosY;
	float MarkerBox_PosZ;

	float MarkerBox_Depth;
	float MarkerBox_Height;
	float MarkerBox_Width;

	float MarkerBox_Yaw;
	float MarkerBox_Pitch;
	float MarkerBox_Roll;

	int Selected_Face_Group;

	bool flag_ShowFaces;
	bool flag_ShowBoundingBox;
	bool flag_ShowPoints;
	bool flag_ShowBones;
	bool flag_ShowNormals;
	bool flag_ShowTextured;
	bool flag_Show_MarkerBox;

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

	void Marker_Render_BoundingBox(void);

	float Hair_1PosX;
	float Hair_1PosY;
	float Hair_1PosZ;

	float Hair_1RotX;
	float Hair_1RotY;
	float Hair_1RotZ;

	bool Light_Activated;
};

