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

using namespace Ogre;

class CL64_ImGui_Sytem_Listener : public Ogre::FrameListener
{
public:
	CL64_ImGui_Sytem_Listener(void);
	~CL64_ImGui_Sytem_Listener(void);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

class CL64_ImGui_System_Data
{
public:
	CL64_ImGui_System_Data();
	~CL64_ImGui_System_Data();

	void Start_Ogre_Window(void);

	void ImGui_Render_Loop(void);

	bool flag_Loop_Enabled;
	bool flag_Show_System_Data;
	bool flag_System_Viewer_Active;
	bool flag_Dark_Mode;
	bool flag_Show_Physics;

	int Selected_System_Page;

	HWND Render_hWnd;

private:
	static LRESULT CALLBACK Proc_Ogre_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Viewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Selected_Button(bool IsSelected);

	void Imgui_System_Dlg(void);
	void Set_OgreWindow(void);
	void Close_OgreWindow(void);

	void Data_Camera(void);
	void Data_Player(void);
	void Data_Model(void);
	void Data_Grids(void);
	void Data_File(void);
	void Data_Prefs(void);
	void Data_Entity(void);

	CL64_ImGui_Sytem_Listener* RenderListener;
	Ogre::RenderWindow* Ogre_MV_Window;
	Ogre::SceneManager* Ogre_MV_SceneMgr;
	Ogre::Camera* Ogre_MV_Camera;
	Ogre::SceneNode* Ogre_MV_CamNode;
	Ogre::Viewport* vp_ImGui;

};


