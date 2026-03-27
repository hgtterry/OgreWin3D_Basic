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
#include "CL64_3D_View_Top.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

CL64_3D_View_Top::CL64_3D_View_Top()
{
}

CL64_3D_View_Top::~CL64_3D_View_Top()
{
}

// *************************************************************************
// *	  		Set_View_Top:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_View_Top::Set_View_Top()
{
    if (App->CL_Model->flag_Model_Loaded == true)
    {
        App->CL_Mesh->Show_Mesh_Textures();
        App->CL_Mesh->Show_Mesh_Faces();

        App->CL_Camera->Reset_View_and_Zoom();

        App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 0, 0));
        App->CL_Ogre->camNode->setOrientation(Ogre::Quaternion::IDENTITY);

        float zoom = max(App->CL_Model->S_BoundingBox[0]->Size[0].z, App->CL_Model->S_BoundingBox[0]->Size[0].x);

        App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, zoom * 2, 0));
        App->CL_Ogre->camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
    }

	Set_OgreWindow();
}

// *************************************************************************
// *			SetOgreWindow:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_3D_View_Top::Set_OgreWindow()
{

	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)App->CL_Editor_Map->Bottom_Left_Window_Hwnd);

	App->CL_Sandbox->Ogre_MV_Window = App->CL_Ogre->mRoot->createRenderWindow("MeshViewWin", 1024, 768, false, &options);

	App->CL_Sandbox->Ogre_MV_SceneMgr = App->CL_Ogre->mRoot->createSceneManager("DefaultSceneManager", "MeshViewGD");

	App->CL_Sandbox->Ogre_MV_CamNode = App->CL_Sandbox->Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode("Camera_Node");

	App->CL_Sandbox->Ogre_MV_Camera = App->CL_Sandbox->Ogre_MV_SceneMgr->createCamera("CameraMV");
	App->CL_Sandbox->Ogre_MV_Camera->setNearClipDistance(0.1);
	App->CL_Sandbox->Ogre_MV_Camera->setFarClipDistance(8000);

	App->CL_Sandbox->Ogre_MV_CamNode->attachObject(App->CL_Sandbox->Ogre_MV_Camera);
	App->CL_Sandbox->Ogre_MV_CamNode->setPosition(Ogre::Vector3(0, 0, 100));

	Ogre::Viewport* vp = App->CL_Sandbox->Ogre_MV_Window->addViewport(App->CL_Sandbox->Ogre_MV_Camera);
	App->CL_Sandbox->Ogre_MV_Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	App->CL_Sandbox->Ogre_MV_SceneMgr->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	//App->CL_ImGui->Init_ImGui();

	//App->CL_Sandbox->RenderListener = new Ogre_Win_Render_Listener();
	//App->CL_Ogre->mRoot->addFrameListener(App->CL_Sandbox->RenderListener);

	Ogre::Entity* Ogre_Ent;
	Ogre::SceneNode* Ogre_Node;

	Ogre_Ent = App->CL_Sandbox->Ogre_MV_SceneMgr->createEntity("Imported_Entity", "Sinbad.mesh", App->CL_Ogre->App_Resource_Group);
	Ogre_Node = App->CL_Sandbox->Ogre_MV_SceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre_Node->attachObject(Ogre_Ent);

	Ogre_Node->setVisible(true);
	Ogre_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	//Ogre_Node->setPosition(0, 0, 200);
	Ogre_Node->setScale(1, 1, 1);

	Ogre_Node->setPosition(Ogre::Vector3(0, 0, 300));
	//Ogre_Node->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	//Debug
	//App->CL_Ogre->mRoot->_setCurrentSceneManager(App->CL_Sandbox->Ogre_MV_SceneMgr);
	//App->CL_ImGui->Init_ImGui();
}
