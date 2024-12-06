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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Gizmos.h"

CL64_Gizmos::CL64_Gizmos(void)
{
	BoxManual = NULL;
	BoxNode = NULL;
	mPickSight = NULL;
}

CL64_Gizmos::~CL64_Gizmos(void)
{
}

// **************************************************************************
// *	  			Set_Gizmos:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_Gizmos::Set_Gizmos()
{
	/*Load_PickSight();
	Load_Arrow();
	Load_Target_Hit();
	Load_All_Axis();*/
	MarkerBox_Setup();
	Load_PickSight();
}

// *************************************************************************
// *	  		MarkerBox_Setup:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Gizmos::MarkerBox_Setup(void)
{
	float BoxDepth = 2.5;
	float BoxHeight = 2.5;
	float BoxWidth = 2.5;

	BoxManual = App->CL_Ogre->mSceneMgr->createManualObject("BB_Box");
	BoxManual->begin("Template/Alpha_Blend_GD64", RenderOperation::OT_LINE_STRIP, App->CL_Ogre->App_Resource_Group);

	BoxManual->colour(0, 1, 0);

	BoxManual->position(-BoxWidth, -BoxHeight, BoxDepth);
	BoxManual->position(BoxWidth, -BoxHeight, BoxDepth);
	BoxManual->position(BoxWidth, BoxHeight, BoxDepth);
	BoxManual->position(-BoxWidth, BoxHeight, BoxDepth);

	BoxManual->position(-BoxWidth, -BoxHeight, -BoxDepth);
	BoxManual->position(BoxWidth, -BoxHeight, -BoxDepth);
	BoxManual->position(BoxWidth, BoxHeight, -BoxDepth);
	BoxManual->position(-BoxWidth, BoxHeight, -BoxDepth);

	BoxManual->index(0);
	BoxManual->index(1);
	BoxManual->index(2);
	BoxManual->index(3);
	BoxManual->index(0);

	BoxManual->index(4);
	BoxManual->index(5);
	BoxManual->index(6);
	BoxManual->index(7);
	BoxManual->index(4);

	BoxManual->index(5);
	BoxManual->index(1);

	BoxManual->index(2);
	BoxManual->index(6);

	BoxManual->index(7);
	BoxManual->index(3);

	BoxManual->end();
	BoxNode = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	BoxNode->attachObject(BoxManual);

	BoxNode->setPosition(0, 0, 0);
	BoxNode->setVisible(false);

}

// *************************************************************************
// *		  	MarkerBox_Update:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Gizmos::MarkerBox_Update(float Depth, float Height, float Width)
{
	BoxManual->beginUpdate(0);
	BoxManual->colour(0, 1, 0, 0.5);

	BoxManual->position(-Depth, -Height, Width);
	BoxManual->position(Depth, -Height, Width);
	BoxManual->position(Depth, Height, Width);
	BoxManual->position(-Depth, Height, Width);
	BoxManual->position(-Depth, -Height, -Width);
	BoxManual->position(Depth, -Height, -Width);
	BoxManual->position(Depth, Height, -Width);
	BoxManual->position(-Depth, Height, -Width);

	BoxManual->index(0);
	BoxManual->index(1);
	BoxManual->index(2);
	BoxManual->index(3);
	BoxManual->index(0);
	BoxManual->index(4);
	BoxManual->index(5);
	BoxManual->index(6);
	BoxManual->index(7);
	BoxManual->index(4);
	BoxManual->index(5);
	BoxManual->index(1);
	BoxManual->index(2);
	BoxManual->index(6);
	BoxManual->index(7);
	BoxManual->index(3);

	BoxManual->end();

}

// *************************************************************************
// *	  		MarkerBB_Addjust:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Gizmos::MarkerBox_Addjust(int Index)
{

	Base_Object* Object = App->CL_Scene->B_Object[Index];

	Ogre::Vector3 Position = Object->Object_Node->getPosition();
	Ogre::Quaternion Rot = Object->Object_Node->getOrientation();

	Ogre::Vector3 Size = App->CL_Com_Objects->GetMesh_BB_Size(Object->Object_Node);

	float Rot_Yaw = Object->Object_Node->getOrientation().getYaw().valueDegrees();
	float Rot_Pitch = Object->Object_Node->getOrientation().getPitch().valueDegrees();
	float Rot_Roll = Object->Object_Node->getOrientation().getRoll().valueDegrees();

	/*App->CL_Ogre->OGL_Listener->MarkerBox_Yaw = Rot_Yaw;
	App->CL_Ogre->OGL_Listener->MarkerBox_Roll = Rot_Pitch;
	App->CL_Ogre->OGL_Listener->MarkerBox_Pitch = Rot_Roll;

	App->CL_Ogre->OGL_Listener->MarkerBox_Depth = Size.z / 2;
	App->CL_Ogre->OGL_Listener->MarkerBox_Height = Size.y / 2;
	App->CL_Ogre->OGL_Listener->MarkerBox_Width = Size.x / 2;*/
	
	MarkerBox_Update(Size.x / 2, Size.y / 2, Size.z / 2);

	Ogre::Vector3 Centre = Object->Object_Node->getAttachedObject(0)->getBoundingBox().getCenter();
	Ogre::Vector3 WS = Object->Object_Node->convertLocalToWorldPosition(Centre);

	/*App->CL_Ogre->OGL_Listener->MarkerBox_PosX = WS.x;
	App->CL_Ogre->OGL_Listener->MarkerBox_PosY = WS.y;
	App->CL_Ogre->OGL_Listener->MarkerBox_PosZ = WS.z;

	App->CL_Ogre->OGL_Listener->Flag_Show_MarkerBox = 1;*/


	BoxNode->setPosition(WS);
	BoxNode->setOrientation(Rot);
	BoxNode->setVisible(true);

	//App->SBC_Markers->Move_Arrow(WS);
}

// *************************************************************************
// *		  	Show_MarkerBox:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Gizmos::Show_MarkerBox(bool Show)
{
	if (Show == 1)
	{
		BoxNode->setVisible(true);
	}
	else
	{
		BoxNode->setVisible(false);
	}

}

// *************************************************************************
// *			Load_PickSight:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Gizmos::Load_PickSight(void)
{
	mPickSight = OverlayManager::getSingleton().getByName("MyOverlays/PicksightOverlay");
	mPickSight->hide();

	//Sight_Entity = App->CL_Ogre->mSceneMgr->createEntity("Arrow23", "Gizmo.mesh", App->CL_Ogre->App_Resource_Group);
	//Sight_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//Sight_Node->attachObject(Sight_Entity);


	////mPickSight->add3D(Sight_Node);

	//Sight_Node->setPosition(0, 0, 0);
	//Sight_Node->setVisible(true);
	//Sight_Node->setScale(7, 7, 7);
}
