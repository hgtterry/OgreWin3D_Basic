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

#include "CL64_Ogre3D_Listener.h"
#include "OgreTrays.h"
#include "SdkSample.h"

using namespace Ogre;
using namespace OgreBites;

class CL64_Ogre : public SdkSample, public RenderTargetListener
{
public:
	CL64_Ogre();
	~CL64_Ogre();

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

	void Log_Message_To_File(char* Message);
	bool ReverseBackSlash(char* buf);
	void RenderFrame(int How_Many);

	char Return_Chr[MAX_PATH];
	Ogre::String mResourcePath;

	Ogre::String App_Resource_Group;		// App Resource Group

	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* camNode;

	Ogre::OverlaySystem* mOverlaySystem;

	CL64_Ogre3D_Listener* Ogre3D_Listener;

	HWND RenderHwnd;
};
