/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Ogre.h"

#include "SdkSample.h"

CL64_Ogre::CL64_Ogre(void)
{
	RenderHwnd = nullptr;

	mRoot =		nullptr;
	mWindow =	nullptr;
	mSceneMgr = nullptr;
	mCamera =	nullptr;
	camNode =	nullptr;

	OgreListener = nullptr;

	manObj = nullptr;
	ModelNode = nullptr;

	OgreEntity = nullptr;
	OgreNode = nullptr;

	mResourcePath = "";
	App_Resource_Group = "App_Resource_Group";
	World_Resource_Group = "World_Resource_Group";

	flag_Show_Test_Mesh = 1;
	flag_Show_Trays = 1;
}

CL64_Ogre::~CL64_Ogre(void)
{
}

// *************************************************************************
// *				InitOgre:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Ogre::InitOgre(void)
{
	OgreCreateRoot();
	SetUpResources();
	Configure();
	chooseSceneManager();
	CreateCamera();
	createViewports();
	Initialise_Resources();

	createFrameListener();

	manObj = mSceneMgr->createManualObject("sampleArea");
	ModelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ModelNode->attachObject(manObj);

	// Set Up Grid Functions
	App->CL_Grid->Grid_Update(1);
	App->CL_Grid->Hair_Update(1);

	OgreEntity = mSceneMgr->createEntity("Sinbad.mesh");
	OgreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	OgreNode->attachObject(OgreEntity);

	Show_Test_Mesh();

	mTrayMgr = new OgreBites::TrayManager("InterfaceName", mWindow);

	mTrayMgr->showAll();
	mTrayMgr->showFrameStats(OgreBites::TL_TOPLEFT);
	mTrayMgr->hideCursor();
	

	App->CL_ImGui->Init_ImGui();

	App->CL_Player->Create_Player_Object();

	//Ogre::Entity* Poo = mSceneMgr->createEntity("C:\\Users\\Terry\\Desktop\\Models\\Obj_Models\\crate.obj");
	
}

// *************************************************************************
// *		OgreCreateRoot:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Ogre::OgreCreateRoot(void)
{
	Ogre::String pluginsPath;
	pluginsPath = mResourcePath + "plugins.cfg";

	if (App->Debug_App == 1)
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "GD64_Ogre.log");
		Ogre::LogManager::getSingleton().createLog(mResourcePath + "App.log");
		Ogre::LogManager::getSingleton().setMinLogLevel(Ogre::LogMessageLevel::LML_NORMAL);

		App->CL_Ogre->Log_Message_To_File((LPSTR)"Initialising");
	}
	else
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "");
	}

	App->CL_Ogre->Log_Message_To_File((LPSTR)"OgreCreateRoot");

	return 1;
}

// **************************************************************************
// *			SetUpResources:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
bool CL64_Ogre::SetUpResources(void)
{

	Ogre::String File;
	char Copy[MAX_PATH];

	strcpy(Copy, App->GD_Directory_FullPath);

	ReverseBackSlash(Copy);
	File = Return_Chr;

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App_Resource_Group);

	////-------------------------------- Zip Files
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/Sinbad.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/SdkTrays.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/Cube.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/GDCore.zip", "Zip", App_Resource_Group);
	
	App->CL_Ogre->Log_Message_To_File((LPSTR)"SetUpResources");

	return 1;
}

// *************************************************************************
// *				Configure:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre::Configure(void)
{
	try
	{
		Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
		if (!(rs->getName() == "OpenGL Rendering Subsystem"))
		{
			App->Say("getRenderSystemByName Error1");
			return false; //No RenderSystem found
		}

		mRoot->setRenderSystem(rs);
	}
	catch (...)
	{
		App->Say("getRenderSystemByName Error2");
	}
	
	mWindow = mRoot->initialise(false);
	Ogre::NameValuePairList options;

	options["vsync"] = true;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)RenderHwnd);

	try
	{
		mWindow = mRoot->createRenderWindow("Main RenderWindow", 1024, 768, false, &options);
	}
	catch (...)
	{
		App->Say("createRenderWindow Error1");
	}

	//App->CL_Panels->Width = mWindow->getWidth();
	//App->CL_Panels->Height = mWindow->getHeight();
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Configure");

	return true;
}

// *************************************************************************
// *		chooseSceneManager:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager"); // Temp
	
	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	// add a bright light above the scene
	//Light* light = mSceneMgr->createLight();
	//light->setType(Light::LT_SPOTLIGHT);
	//light->setPosition(0, 0, 0);
	//light->setSpecularColour(ColourValue::White);

	App->CL_Ogre->Log_Message_To_File((LPSTR)"chooseSceneManager");

	return 1;
}

// *************************************************************************
// *			CreateCamera:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre::CreateCamera(void)
{
	camNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	mCamera = mSceneMgr->createCamera("MainCamera");
	mCamera->setNearClipDistance(Ogre::Real(0.1));
	mCamera->setFarClipDistance(Ogre::Real(8000));

	camNode->attachObject(mCamera);

	camNode->setPosition(Ogre::Vector3(0, 5, 15));
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	App->CL_Ogre->Log_Message_To_File((LPSTR)"createCamera");
	
	return 1;
}

// *************************************************************************
// *		createViewports:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Ogre::createViewports(void)
{
	mWindow->removeAllViewports();
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);

	vp->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));

	App->CL_Ogre->Log_Message_To_File((LPSTR)"createViewports");

	return 1;
}

// *************************************************************************
// *		Initialise_Resources:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre::Initialise_Resources(void)
{
	// Initialize, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	App->CL_Ogre->Log_Message_To_File((LPSTR)"Initialise_Resources");

	return 1;
}

// *************************************************************************
// *			ReverseBackSlash:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre::ReverseBackSlash(char* buf)
{
	strcpy(Return_Chr, "");
	char bufnew[256];
	strcpy(bufnew, "");
	char seps[] = "\\";
	char* token;
	bool tt = 0;

	token = strtok(buf, seps);
	while (token != NULL)
	{
		tt = 1;
		strcat(bufnew, token);
		strcat(bufnew, "/");
		token = strtok(NULL, seps);
	}

	if (tt == 1)
	{
		int len = strlen(bufnew);
		bufnew[len - 1] = 0;
		strcpy(Return_Chr, bufnew);
	}
	else
	{
		tt = 0;
		strcpy(Return_Chr, buf);
	}
	return 1;
}

// *************************************************************************
// *		createFrameListener:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre::createFrameListener(void)
{

	Ogre::String RenderSystemName = mSceneMgr->getDestinationRenderSystem()->getName();

	if (RenderSystemName == "OpenGL Rendering Subsystem")
	{
		OgreListener = new CL64_OgreListener();
		mRoot->addFrameListener(OgreListener);

		RenderListener = new CL64_OGL_Listener();
		mSceneMgr->addRenderQueueListener(RenderListener);

		BulletListener = new CL64_DebugBulletWorld();

		mSceneMgr->addRenderQueueListener(BulletListener);

		BulletListener->setDebugMode(BulletListener->getDebugMode()
			| btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
		App->CL_Bullet->dynamicsWorld->setDebugDrawer(BulletListener);

	}
	else
	{
		App->Say("OpenGL Rendering Subsystem not allocated ");
		return FALSE;
	}

	App->CL_Ogre->Log_Message_To_File((LPSTR)"createFrameListener");

	return TRUE;
}

// *************************************************************************
// *			Log_Message_To_File:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Ogre::Log_Message_To_File(char* Message)
{
	if (App->Debug_App == 1)
	{
		Ogre::LogManager::getSingleton().setDefaultLog(Ogre::LogManager::getSingleton().getLog("App.log"));
		Ogre::LogManager::getSingleton().logMessage(Message);
		Ogre::LogManager::getSingleton().setDefaultLog(Ogre::LogManager::getSingleton().getLog("GD64_Ogre.log"));

	}
}

// *************************************************************************
// *			Clear_ErrorLog:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Ogre::Clear_ErrorLog()
{
	FILE* fp = NULL;

	char Path[MAX_PATH];
	strcpy(Path, App->GD_Directory_FullPath);
	strcat(Path, "\\");
	strcat(Path, "App.log");

	fp = fopen(Path, "w+");
	if (!fp)
	{
		App->Say("Cant Find File");
		return;
	}

	fprintf(fp, "Error File");

	fclose(fp);

	_unlink(Path);
}

// *************************************************************************
// *			Show_Test_Mesh:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Ogre::Show_Test_Mesh(void)
{
	if (flag_Show_Test_Mesh == 1)
	{
		OgreNode->setVisible(true);
	}
	else
	{
		OgreNode->setVisible(false);
	}

}

// *************************************************************************
// *			Show_Trays:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Ogre::Show_Trays(void)
{
	if (flag_Show_Trays == 1)
	{
		mTrayMgr->showAll();
		mTrayMgr->hideCursor();
	}
	else
	{
		mTrayMgr->hideAll();
	}

}

// *************************************************************************
// *			RenderFrame:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Ogre::RenderFrame(void)
{
	Ogre::Root::getSingletonPtr()->renderOneFrame();
}

// *************************************************************************
// *		Ogre_Render_Loop:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre::Ogre_Render_Loop(void)
{
	mRoot->clearEventTimes();
	
	while (true)
	{
		MSG  msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (mWindow->isClosed()) return false;

		//if (FPStimer.getMicroseconds() > Fps_Tick) // FPSLock)
		//{
		//	if (Block_RenderingQueued == 0)
		//	{

		if (!mRoot->_fireFrameStarted())
		{
			return false;
		}

		if (!mRoot->_updateAllRenderTargets())
		{
			return false;
		}

		if (!mRoot->_fireFrameEnded())
		{
			return false;
		}

		//		FPStimer.reset();

		//	}
		//}
	}

	return 1;
}

