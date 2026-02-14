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

#include "CLP_3D_Listener.h"
#include "OgreImGuiOverlay.h"
#include <OgreImGuiInputListener.h>
#include "OgreTrays.h"
#include "SdkSample.h"
#include "CL64_OGL_Listener.h"

using namespace Ogre;
using namespace OgreBites;

class CLP_3D_Setup : public SdkSample, public RenderTargetListener
{
public:
	CLP_3D_Setup();
	~CLP_3D_Setup();

	// Setup --------------------------------
	void Init_3D(void);
	void Initialize_3D_Components();

	bool Render_Loop_3D(void);

	void Log_Message_To_File(char* Message);
	void RenderFrame(int How_Many);

	void Camera_Reset_Zero();

	void Processes_Start_Time(void);
	void Processes_Get_Time(void);
	void Get_OpenGL_Version(char* buff);

	char Return_Chr[MAX_PATH];
	Ogre::String mResourcePath;

	Ogre::String App_Resource_Group;		// App Resource Group
	Ogre::String World_Resource_Group;
	Ogre::String Export_Resource_Group;
	Ogre::String Texture_Resource_Group;

	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* camNode;

	Ogre::ManualObject* manObj;
	Ogre::SceneNode* ModelNode;

	Ogre::OverlaySystem* mOverlaySystem;
	Ogre::Viewport* vp;

	CLP_3D_Listener*		Listener_3D;
	CL64_OGL_Listener*		OGL_Listener;
	
	Ogre::Timer FPStimer;
	Ogre::Timer Processes_timer;

	int FPSLock; // FPS Lock Setable (60 FPS Default)
	int m_Processes_Time;

	HWND RenderHwnd;

private:
	bool Init_3D_CreateRoot(void);
	bool Init_Load_Resources(void);
	bool Init_Configure(void);
	bool Init_ChooseSceneManager(void);
	bool Init_CreateCamera(void);
	bool Init_CreateViewports(void);
	bool Init_Resources(void);
	bool Init_CreateFrameListener(void);

	bool ReverseBackSlash(char* buf);
};

