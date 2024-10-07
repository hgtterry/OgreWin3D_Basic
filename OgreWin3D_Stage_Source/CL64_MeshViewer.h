/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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
class CL64_MeshViewer
{
public:
	CL64_MeshViewer(void);
	~CL64_MeshViewer(void);

	void Start_MeshViewer_Dlg();

	HWND MainDlgHwnd;
	HWND MeshViewer_3D_hWnd;

	Ogre::RenderWindow* Ogre_MV_Window;
	Ogre::SceneManager* Ogre_MV_SceneMgr;
	Ogre::Camera* Ogre_MV_Camera;
	Ogre::SceneNode* Ogre_MV_CamNode;
	Ogre::Entity* Ogre_MvEnt;
	Ogre::SceneNode* Ogre_MvNode;

private:
	static LRESULT CALLBACK Proc_MeshViewer_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_MeshViewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Close_OgreWindow(void);
	bool Set_OgreWindow(void);

	void Get_Media_Folders_Actors(HWND DropHwnd);
	void Get_Files();

	HWND ListHwnd;

	char m_Current_Folder[MAX_PATH];
	char mResource_Folder[MAX_PATH];
};





