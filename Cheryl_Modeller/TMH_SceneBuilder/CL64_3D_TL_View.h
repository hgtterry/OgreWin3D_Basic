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

#include "CL64_3D_TL_OGL_Listener.h"

#pragma once
class CL64_3D_TL_View
{
public:
	CL64_3D_TL_View();
	~CL64_3D_TL_View();

	void Create_Top_Left_Window();
	void Set_Zoom(void);
	void ResizeOgreWindow_TL();

	HWND Render_hWnd;
	HWND ViewGLhWnd_TL;

	Ogre::RenderWindow* Ogre_TL_Window;
	Ogre::SceneManager* Ogre_TL_SceneMgr;
	Ogre::Camera* Ogre_TL_Camera;
	Ogre::SceneNode* Ogre_TL_CamNode;

	Ogre_Win_Render_Listener* RenderListener;
	CL64_3D_TL_OGL_Listener* OGL_TL_Listener;

	void Set_OgreWindow_TL();

	static LRESULT CALLBACK Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static LRESULT CALLBACK Proc_Ogre_TL(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Close_OgreWindow(void);
};

