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

	bool Flag_Show_Selected_Brush;
	bool Flag_Show_Selected_Face;
	bool Flag_Render_Brushes;

	bool Get_Brush(const Brush* b, int Actual_Brush_Index);
	bool Brush_Decode_List(BrushList* BList, signed int SubBrush);
	bool Brush_FaceList_Create(const Brush* b, const FaceList* pList, int BrushCount, int SubBrushCount, int Actual_Brush_Index);

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

	void Groups_Render_Textures(void);
	void Groups_Textured_Parts(int Count);

	void Render_Selected_Brush();
	void Render_Selected_Face();

	bool Light_Activated;

};

