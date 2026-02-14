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

#include "Ogre_Win_Render_Listener.h"

class Sandbox
{
public:
	Sandbox(void);
	~Sandbox(void);

	void Start_Colour_Mixer();
	void Duplicate_Brush();
	void Start_Ogre_Window();
	void Set_OgreWindow();
	void Close_OgreWindow(void);

	int Selected_Item;

	HWND Render_hWnd;

	Ogre_Win_Render_Listener* RenderListener;

	Ogre::RenderWindow* Ogre_MV_Window;
	Ogre::SceneManager* Ogre_MV_SceneMgr;
	Ogre::Camera* Ogre_MV_Camera;
	Ogre::SceneNode* Ogre_MV_CamNode;


private:
	static LRESULT Proc_Colour_Mixer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Ogre_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Viewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Sliders(HWND hDlg);
	void Get_Sliders(HWND hDlg, LPARAM lParam);
	void Set_ColourBox();

	HBRUSH Actual_Colour;

	Colour_Int Colour;

	LRESULT R_pos;
	LRESULT G_pos;
	LRESULT B_pos;

	HWND hTrack_Red;
	HWND hTrack_Green;
	HWND hTrack_Blue;

	HWND Colour_Box_hWnd;
};

