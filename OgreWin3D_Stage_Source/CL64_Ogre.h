/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
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

using namespace Ogre;
using namespace OgreBites;

class CL64_Ogre : public SdkSample, public RenderTargetListener
{
public:

	CL64_Ogre(void);
	~CL64_Ogre(void);

	// Setup --------------------------------
	void InitOgre(void);
	bool OgreCreateRoot(void);
	bool SetUpResources(void);
	bool Configure(void);
	bool chooseSceneManager(void);
	bool CreateCamera(void);
	bool createViewports(void);
	bool Initialise_Resources(void);
	bool createFrameListener(void);
	// --------------------------------------

	void Log_Message_To_File(char* Message);
	void Clear_ErrorLog();

	void Show_Test_Mesh(bool Show);
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


	Ogre::OverlaySystem* mOverlaySystem;
	
	Ogre::ManualObject* manObj;
	Ogre::SceneNode* ModelNode;

	char Return_Chr[MAX_PATH];

	int FPSLock; // FPS Lock Setable (60 FPS Default)

	bool flag_Show_Test_Mesh;	// is Test Mesh Visable
	bool flag_Show_Fog;			// Is the fog Enabled
	bool flag_Test_Mesh_Active;
};

