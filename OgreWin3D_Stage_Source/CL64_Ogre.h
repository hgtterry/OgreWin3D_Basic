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

#include "CL64_Ogre3D_Listener.h"
#include "CL64_OGL_Listener.h"
#include "OgreOverlaySystem.h"
#include "OgreImGuiOverlay.h"
#include "OgreAdvancedRenderControls.h"
#include "OgreTrays.h"
#include "SdkSample.h"
#include "OgreImGuiOverlay.h"
#include <OgreImGuiInputListener.h>

#include "CL64_Bullet_Debug_World.h"
#include "CL64_ResourceListener.h"

using namespace Ogre;
using namespace OgreBites;

class CL64_Ogre : public SdkSample, public RenderTargetListener
{
public:

	CL64_Ogre(void);
	~CL64_Ogre(void);

	// Setup --------------------------------
	void Init_Ogre(void);
	bool Init_OgreCreateRoot(void);
	bool Init_Load_Resources(void);
	bool Init_Configure(void);
	bool Init_ChooseSceneManager(void);
	bool Init_CreateCamera(void);
	bool Init_CreateViewports(void);
	bool Init_Resources(void);
	bool Init_CreateFrameListener(void);
	// --------------------------------------

	bool ExitFullScreen();
	void Log_Message_To_File(char* Message);
	void Clear_ErrorLog();

	void Delete_TestMesh(void);
	void Enable_Fog(bool Enable);

	// Renders a Single Frame
	// How_Many = How any times you want it to render a Frame
	void RenderFrame(int How_Many);

	// Custom Render Loop returns to where it was called from
	bool Ogre_Render_Loop(void);

	bool ReverseBackSlash(char* buf);
	
	HWND RenderHwnd;

	Ogre::Timer FPStimer;

	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* camNode;

	Ogre::String mResourcePath;

	Ogre::String App_Resource_Group;		// App Resource Group
	Ogre::String World_Resource_Group;

	CL64_Ogre3D_Listener*		Ogre3D_Listener;
	CL64_OGL_Listener*			OGL_Listener;
	CL64_Bullet_Debug_World*	Bullet_Debug_Listener;
	CL64_ResourceListener*		Resource__Listener;

	Ogre::OverlaySystem* mOverlaySystem;
	
	Ogre::ManualObject* manObj;
	Ogre::SceneNode* ModelNode;

	char Return_Chr[MAX_PATH];

	int FPSLock; // FPS Lock Setable (60 FPS Default)

	bool flag_Show_Test_Mesh;	// is Test Mesh Visable
	bool flag_Show_Fog;			// Is the fog Enabled
	bool flag_Test_Mesh_Active;
	bool flag_Block_Rendering;
};

