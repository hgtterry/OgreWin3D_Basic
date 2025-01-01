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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Ogre.h"

CL64_Ogre::CL64_Ogre()
{
	mResourcePath = "";
	Return_Chr[0] = 0;

	App_Resource_Group = "App_Resource_Group";

	RenderHwnd = nullptr;

}

CL64_Ogre::~CL64_Ogre()
{
}

void CL64_Ogre::Init_Ogre(void)
{
	Init_OgreCreateRoot();
	Init_Load_Resources();
	Init_Configure();
	Init_ChooseSceneManager();
	Init_CreateCamera();
	Init_CreateViewports();
	Init_Resources();

	Init_CreateFrameListener();

	/*manObj = mSceneMgr->createManualObject("sampleArea");
	ModelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ModelNode->attachObject(manObj);*/

	// Set Up Grid Functions
	/*App->CL_Grid->Grid_Update(1);
	App->CL_Grid->Hair_Update(1);*/
	/*App->CL_Gizmos->Set_Gizmos();
	App->CL_ImGui->Init_ImGui();*/
}

// *************************************************************************
// *		Init_OgreCreateRoot:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre::Init_OgreCreateRoot(void)
{
	Ogre::String pluginsPath;
	pluginsPath = mResourcePath + "plugins.cfg";

	if (App->flag_Debug_App == 1)
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "GD64_Ogre.log");
		Ogre::LogManager::getSingleton().createLog(mResourcePath + "App.log");
		Ogre::LogManager::getSingleton().setMinLogLevel(Ogre::LogMessageLevel::LML_NORMAL);

#pragma warning(disable : 4996) // Nightmare why
		Ogre::LogManager::getSingleton().setLogDetail(Ogre::LoggingLevel::LL_BOREME);

		App->CL_Ogre->Log_Message_To_File((LPSTR)"Initialising");
	}
	else
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "");
	}

	App->CL_Ogre->Log_Message_To_File((LPSTR)" // -------------------------  Init Ogre");
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Ogre Root Created");

	return 1;
}

// **************************************************************************
// *			Load_Resources:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
bool CL64_Ogre::Init_Load_Resources(void)
{

	Ogre::String File;
	char Copy[MAX_PATH];

	strcpy(Copy, App->RB_Directory_FullPath);

	ReverseBackSlash(Copy);
	File = Return_Chr;

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App_Resource_Group);

	////-------------------------------- Zip Files
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/Sinbad.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/SdkTrays.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/GDCore.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/Entitys.zip", "Zip", App_Resource_Group);

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/Files", "FileSystem", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/Files/New_Particles", "FileSystem", App_Resource_Group);
	/*if (flag_Use_RTSS == 1)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/RTShaderLib",
			"FileSystem", App_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/RTShaderLib/GLSL",
			"FileSystem", App_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/RTShaderLib/GLSLES",
			"FileSystem", App_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/RTShaderLib/HLSL",
			"FileSystem", App_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/RTShaderLib/Cg",
			"FileSystem", App_Resource_Group);
	}*/

	App->CL_Ogre->Log_Message_To_File((LPSTR)"SetUpResources");

	return 1;
}

// *************************************************************************
// *			Init_Resources:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre::Init_Resources(void)
{
	// Initialize, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	App->CL_Ogre->Log_Message_To_File((LPSTR)"Initialise_Resources");

	return 1;
}

// *************************************************************************
// *			Init_Configure:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre::Init_Configure(void)
{
	try
	{
		Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
		if (!(rs->getName() == "OpenGL Rendering Subsystem"))
		{
			App->Say("getRenderSystemByName Error1", (LPSTR)"");
			return false; //No RenderSystem found
		}

		mRoot->setRenderSystem(rs);
	}
	catch (...)
	{
		App->Say("getRenderSystemByName Error2", (LPSTR)"");
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
		App->Say("createRenderWindow Error1",(LPSTR)"");
	}

	//App->CL_Panels->Width = mWindow->getWidth();
	//App->CL_Panels->Height = mWindow->getHeight();

	App->CL_Ogre->Log_Message_To_File((LPSTR)"Configure");

	return true;
}

// *************************************************************************
// *		Init_ChooseSceneManager:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre::Init_ChooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager"); // Temp

	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	App->CL_Ogre->Log_Message_To_File((LPSTR)"chooseSceneManager");

	return 1;
}

// *************************************************************************
// *		Init_CreateCamera:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre::Init_CreateCamera(void)
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
// *		Init_CreateViewports:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre::Init_CreateViewports(void)
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
// *	Init_CreateFrameListener:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre::Init_CreateFrameListener(void)
{

	Ogre::String RenderSystemName = mSceneMgr->getDestinationRenderSystem()->getName();

	if (RenderSystemName == "OpenGL Rendering Subsystem")
	{
		//Ogre3D_Listener = new CL64_Ogre3D_Listener();
		//mRoot->addFrameListener(Ogre3D_Listener);

		/*OGL_Listener = new CL64_OGL_Listener();
		mSceneMgr->addRenderQueueListener(OGL_Listener);

		Bullet_Debug_Listener = new CL64_Bullet_Debug_World();
		mSceneMgr->addRenderQueueListener(Bullet_Debug_Listener);

		Bullet_Debug_Listener->setDebugMode(Bullet_Debug_Listener->getDebugMode()
			| btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
		App->CL_Bullet->dynamicsWorld->setDebugDrawer(Bullet_Debug_Listener);*/

	}
	else
	{
		App->Say("OpenGL Rendering Subsystem not allocated ", (LPSTR)"");
		return FALSE;
	}

	/*Resource__Listener = new CL64_ResourceListener();
	Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(Resource__Listener);*/

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
