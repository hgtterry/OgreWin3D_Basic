/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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

	// Show Selected Brush and Face Outlines
	void Show_Visuals(bool Show);

	bool Get_Brush(const Brush* b);
	bool Brush_Decode_List(BrushList* BList);
	bool Brush_FaceList_Render(const Brush* b, const FaceList* pList);

	void MarkerBox_Update(float Depth, float Height, float Width);

	float RX;
	float RZ;

	int Selected_Face_Group;

	bool flag_Show_Selected_Brush;
	bool flag_Show_Selected_Face;

	bool flag_Show_Material_Faces;

	bool flag_Render_Groups;
	bool flag_Render_Brushes;
	bool flag_Render_Ogre;

	bool flag_Just_Face;

	bool flag_ShowFaces;
	bool flag_ShowPoints;
	bool flag_ShowNormals;
	bool flag_ShowBoundingBox;
	bool flag_ShowBones;

	// Brushes
	bool flag_Render_Just_Brush;
	int Selected_Brush_Index;

	int Render_Mode;

	int	mBrushCount;
	int	mSubBrushCount;

	UINT  g_BrushTexture[400];

private:

	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& skipThisInvocation);

	virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& repeatThisInvocation);

	void PreRender();
	void PostRender();
	void Render_Loop();
	void Translate(void);

	void MeshData_Render_Material_Faces(void);
	void MeshData_Render_Faces(void);
	void MeshData_Face_Groups(int Count);

	void MeshData_Render_Points(void);
	void MeshData_Points_Groups(int Count);

	void MeshData_Render_Normals(void);
	void MeshData_Normals_Groups(int Count);

	void MeshData_Render_BoundingBox(void);
	void MeshData_RenderBones();


	void Brushes_Face_Parts(int Count);
	void Brushes_Render_Faces(void);

	void Groups_Render_Textures(void);
	void Groups_Textured_Parts(int Count);

	void Groups_Render_Faces(void);
	void Groups_Faces_Parts(int Count);

	void Render_Selected_Brush();
	void Render_Selected_Face();

	bool Render_Brush(const Brush* b);
	bool Render_Sub_Brushes(BrushList* BList);
	bool Render_Faces(const FaceList* pList);
	bool Render_Face(const Face* f);

	bool Light_Activated;

};

