/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
#include "CL64_3D_TR_OGL_Listener.h"

CL64_3D_TR_OGL_Listener::CL64_3D_TR_OGL_Listener()
{
}

CL64_3D_TR_OGL_Listener::~CL64_3D_TR_OGL_Listener()
{
}

// *************************************************************************
// *			renderQueueStarted:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_3D_TR_OGL_Listener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
{
}

// *************************************************************************
// *			renderQueueEnded:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_3D_TR_OGL_Listener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
{
	/*if (App->CL_Editor_Control->flag_Scene_Game_Running == true)
	{
		return;
	}*/

	if (queueGroupId != Ogre::RENDER_QUEUE_MAIN)
	{
		return;
	}

	PreRender();

	//Render_Loop();

	PostRender();
}

// *************************************************************************
// *				PreRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_3D_TR_OGL_Listener::PreRender()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity(); //Texture addressing should start out as direct.

	Ogre::RenderSystem* renderSystem = App->CL_Ogre->manObj->_getManager()->getDestinationRenderSystem();
	Ogre::Node* parentNode = App->CL_Ogre->manObj->getParentNode();

#pragma warning(disable : 4996) // Nightmare why

	renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setViewMatrix(App->CL_Ogre->mCamera->getViewMatrix());
	renderSystem->_setProjectionMatrix(App->CL_Ogre->mCamera->getProjectionMatrixWithRSDepth());

	static Ogre::Pass* clearPass = NULL;
	if (!clearPass)
	{
		Ogre::MaterialPtr clearMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");
		clearPass = clearMat->getTechnique(0)->getPass(0);
	}

	//Set a clear pass to give the renderer a clear renderstate
	//App->CL_Ogre->mSceneMgr->_setPass(clearPass, true);

	// save attribs
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

// *************************************************************************
// *				PostRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_3D_TR_OGL_Listener::PostRender()
{
	// restore original state
	glPopAttrib();

	// restore matrices
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
