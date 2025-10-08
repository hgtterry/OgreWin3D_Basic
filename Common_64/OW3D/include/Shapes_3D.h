#pragma once

#include "Shapes_Render_Listener.h"
#include "OGL_Listener.h"

class Shapes_3D
{
public:
	Shapes_3D(void);
	~Shapes_3D(void);

	void Set_OgreWindow();
	void Close_OgreWindow(void);
	void Set_Camera(float Zoom);

	HWND Render_hWnd;

	Ogre::RenderWindow* Ogre_MV_Window;
	Ogre::SceneManager* Ogre_MV_SceneMgr;
	Ogre::Camera* Ogre_MV_Camera;
	Ogre::SceneNode* Ogre_MV_CamNode;
	Ogre::Entity* Ogre_MvEnt;
	Ogre::SceneNode* Ogre_MvNode;

	Ogre::ManualObject* OGL_Obj;
	Ogre::SceneNode* OGL_Node;

	float Start_Zoom;

	int CursorPosX;
	int CursorPosY;

	OGL_Listener* OGL_RenderListener;
	Shapes_Render_Listener* RenderListener;

	static LRESULT CALLBACK Proc_Box_Viewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

