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

	Ogre3D_Listener = nullptr;

	manObj = nullptr;
	ModelNode = nullptr;

	mResourcePath = "";
	App_Resource_Group = "App_Resource_Group";
	World_Resource_Group = "World_Resource_Group";

	flag_Show_Test_Mesh = 1;
	flag_Show_Trays = 1;
	flag_Show_Fog = 0;
	flag_Test_Mesh_Active = 0;

	FPSLock = 16666; // Default 60 FPS

	FPStimer.reset();
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

	App->CL_Scene->Imported_Ogre_Ent = mSceneMgr->createEntity("Test_Mesh", "Sinbad.mesh");
	App->CL_Scene->Imported_Ogre_Node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	App->CL_Scene->Imported_Ogre_Node->attachObject(App->CL_Scene->Imported_Ogre_Ent);

	App->CL_Scene->S_OgreMeshData[0]->mFileName_Str = "Sinbad.mesh";

	App->CL_Import_Ogre3D->Ogre_To_Mesh_Data(App->CL_Scene->Imported_Ogre_Ent);
	App->CL_Import_Ogre3D->Get_Motions(App->CL_Scene->Imported_Ogre_Ent);
	App->CL_Import_Ogre3D->flag_Ogre_Model_Loaded = 1;

	App->CL_Scene->Scene_Mode = Enums::Scene_Mode_TestMesh;
	App->CL_Ogre->Ogre3D_Listener->Ogre_Model_Loaded = 1;
	App->CL_Scene->flag_Model_Loaded = 1;

	flag_Test_Mesh_Active = 1;

	mTrayMgr = new OgreBites::TrayManager("InterfaceName", mWindow);
	mTrayMgr->showAll();
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	mTrayMgr->hideCursor();
	
	App->CL_ImGui->Init_ImGui();

	App->CL_Player->Create_Player_Object();

}

// *************************************************************************
// *		OgreCreateRoot:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Ogre::OgreCreateRoot(void)
{
	Ogre::String pluginsPath;
	pluginsPath = mResourcePath + "plugins.cfg";

	if (App->flag_Debug_App == 1)
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "GD64_Ogre.log");
		Ogre::LogManager::getSingleton().createLog(mResourcePath + "App.log");
		Ogre::LogManager::getSingleton().setMinLogLevel(Ogre::LogMessageLevel::LML_TRIVIAL);

#pragma warning(disable : 4996) // Nightmare why
		Ogre::LogManager::getSingleton().setLogDetail(Ogre::LoggingLevel::LL_BOREME);

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

	//options["vsync"] = false;

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

	Ogre::ColourValue fadeColour(0.5, 0.5, 0.5);
	mWindow->getViewport(0)->setBackgroundColour(fadeColour);
	
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
		Ogre3D_Listener = new CL64_Ogre3D_Listener();
		mRoot->addFrameListener(Ogre3D_Listener);

		OGL_Listener = new CL64_OGL_Listener();
		mSceneMgr->addRenderQueueListener(OGL_Listener);

		Bullet_Debug_Listener = new CL64_Bullet_Debug_World();
		mSceneMgr->addRenderQueueListener(Bullet_Debug_Listener);

		Bullet_Debug_Listener->setDebugMode(Bullet_Debug_Listener->getDebugMode()
			| btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
		App->CL_Bullet->dynamicsWorld->setDebugDrawer(Bullet_Debug_Listener);

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
	if (App->flag_Debug_App == 1)
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
void CL64_Ogre::Show_Test_Mesh(bool Show)
{
	if (flag_Test_Mesh_Active == 1 && App->CL_Scene->Imported_Ogre_Ent)
	{
		if (Show == 1)
		{
			App->CL_Scene->Imported_Ogre_Ent->setVisible(true);
			flag_Show_Test_Mesh = 1;
		}
		else
		{
			App->CL_Scene->Imported_Ogre_Ent->setVisible(false);
			flag_Show_Test_Mesh = 0;
		}
	}
}

// *************************************************************************
// *			Show_Trays:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Ogre::Show_Trays(bool Enable)
{
	if (Enable == 1)
	{
		mTrayMgr->showAll();
		mTrayMgr->hideCursor();
		flag_Show_Trays = 1;
	}
	else
	{
		mTrayMgr->hideAll();
		flag_Show_Trays = 0;
	}
}

// *************************************************************************
// *			Delete_TestMesh:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Ogre::Delete_TestMesh(void)
{
	
	if (App->CL_Ogre->flag_Test_Mesh_Active == 1)
	{
		App->CL_TopDlg->Enable_TestMesh_Button(false);
		App->CL_Ogre->flag_Test_Mesh_Active = 0;
	}

}

// *************************************************************************
// *			RenderFrame:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Ogre::RenderFrame(int How_Many)
{
	int Count = 0;
	while (Count < How_Many)
	{
		Ogre::Root::getSingletonPtr()->renderOneFrame();
		Count++;
	}

}

// *************************************************************************
// *			Enable_Fog:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Ogre::Enable_Fog(bool Enable)
{
	if (Enable == 1)
	{
		App->CL_Ogre->mSceneMgr->setFog(FOG_LINEAR, ColourValue(1, 1, 1), 0, 100, 1000);
		flag_Show_Fog = 1;
	}
	else
	{
		App->CL_Ogre->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7, 0.7, 0.8), 0, 100, 1000);
		flag_Show_Fog = 0;
	}

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

		if (FPStimer.getMicroseconds() > FPSLock)
		{
			FPStimer.reset();

			//if (Block_RenderingQueued == 0)
			//{

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

			//}
		}
	}

	return 1;
}

