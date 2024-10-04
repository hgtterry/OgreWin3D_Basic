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
#include "CL64_Object.h"

CL64_Object::CL64_Object(void)
{
}

CL64_Object::~CL64_Object(void)
{
}

// *************************************************************************
// *	  		GetMesh_BB_Radius:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
float CL64_Object::GetMesh_BB_Radius(SceneNode* mNode)
{
	AxisAlignedBox aab = mNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 min = aab.getMinimum() * mNode->getScale();
	Ogre::Vector3 max = aab.getMaximum() * mNode->getScale();
	Ogre::Vector3 center = aab.getCenter() * mNode->getScale();
	Ogre::Vector3 size(fabs(max.x - min.x), fabs(max.y - min.y), fabs(max.z - min.z));
	float radius = (size.x > size.z) ? size.z / 2.0f : size.x / 2.0f;

	return radius;
}

// *************************************************************************
// *	  		GetMesh_BB_Size:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
Ogre::Vector3 CL64_Object::GetMesh_BB_Size(SceneNode* mNode)
{
	AxisAlignedBox aab = mNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 min = aab.getMinimum() * mNode->getScale();
	Ogre::Vector3 max = aab.getMaximum() * mNode->getScale();
	Ogre::Vector3 center = aab.getCenter() * mNode->getScale();
	Ogre::Vector3 size(fabs(max.x - min.x), fabs(max.y - min.y), fabs(max.z - min.z));
	float radius = (size.x > size.z) ? size.z / 2.0f : size.x / 2.0f;

	return size;
}

// *************************************************************************
//			Object_Camera_Goto:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Object::Object_Camera_Goto(int Object_Index)
{
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

	Ogre::Vector3 WS;
	Ogre::Vector3 WS2;
	Ogre::Vector3 Centre;
	Ogre::Vector3 CentreNode;
	Ogre::Vector3 Size;

	int Index = Object_Index;

	if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
	{
		Centre = App->CL_Scene->B_Area[Index]->Area_Node->getAttachedObject(0)->getBoundingBox().getCenter();
		WS = App->CL_Scene->B_Area[Index]->Area_Node->convertLocalToWorldPosition(Centre);
	}
	else
	{
		//CentreNode = App->CL_Scene->V_Object[Index]->Object_Node->getPosition();

		//Centre = App->CL_Scene->V_Object[Index]->Object_Node->getAttachedObject(0)->getBoundingBox().getCenter();
		//Size = App->CL_Scene->V_Object[Index]->Object_Node->getAttachedObject(0)->getBoundingBox().getMaximum();

		////Centre.z = Centre.z + Size.z;

		//WS = App->CL_Scene->V_Object[Index]->Object_Node->convertLocalToWorldPosition(Size);
		//WS2 = App->CL_Scene->V_Object[Index]->Object_Node->convertLocalToWorldPosition(Centre);
		////WS.z = WS.z - 40;// Size.z;

		Centre = App->CL_Scene->V_Object[Index]->Object_Ent->getWorldBoundingBox(true).getCenter();
	}

	App->CL_Ogre->camNode->setPosition(Centre);
	//App->CL_Ogre->camNode->lookAt(WS2, Ogre::Node::TS_WORLD);
}
