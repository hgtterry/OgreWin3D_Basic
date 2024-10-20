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
	flag_Show_Physics_Debug = 0;
	flag_Hide_All_Except = 0;
	flag_Show_Mesh_Debug = 1;
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
	float radius = (size.x < size.z) ? size.z / 2.0f : size.x / 2.0f;

	/*if (size.x > size.y && size.x > size.z) {
		radius = size.x / 2.0f;
	}
	else if (size.y > size.x && size.y > size.z) {
		radius = size.y / 2.0f;
	}
	if (size.z > size.x && size.z > size.y) {
		radius = size.z / 2.0f;
	}*/
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
// *			Rename_Object:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Object::Rename_Object(int Index)
{
	Base_Object* Object = App->CL_Scene->V_Object[Index];

	strcpy(App->CL_Dialogs->btext, "Change Object Name");
	strcpy(App->CL_Dialogs->Chr_Text, Object->Mesh_Name);

	App->CL_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CL_Dialogs->Canceled == 1)
	{
		return;
	}

	strcpy(Object->Mesh_Name, App->CL_Dialogs->Chr_Text);

	Object->Altered = 1;

	App->CL_Scene->flag_Scene_Modified = 1;
	App->CL_FileView->Mark_Altered(Object->FileViewItem);

	App->CL_FileView->Change_Item_Name(Object->FileViewItem, Object->Mesh_Name);

}

// *************************************************************************
// *	  		GetPlacement:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
Ogre::Vector3 CL64_Object::GetPlacement(int Distance)
{
	Ogre::Vector3 Placement;
	Ogre::Vector3 CamPos;
	Ogre::Quaternion CamRot;
	Ogre::Radian mYaw;
	Ogre::Vector3 mDirection;


	CamPos = App->CL_Ogre->camNode->getPosition();
	mYaw = App->CL_Ogre->camNode->getOrientation().getYaw();

	//App->CL_Ogre->PlacementCam->setPosition(CamPos);

	//CamRot = Ogre::Quaternion::IDENTITY;
	//App->CL_Ogre->PlacementCam->setOrientation(CamRot);
	//App->CL_Ogre->PlacementCam->yaw(mYaw);

	//Ogre::Vector3 TranslateVector = Ogre::Vector3::ZERO;

	////float Radius = OgreNode->getAttachedObject(0)->getBoundingRadius();
	////Radius = (Radius * 7) + 2;


	//TranslateVector.z = Distance; // Distance from Camera/Player

	//CamPos = App->CL_Ogre->mCamera->getPosition();

	//App->CL_Ogre->PlacementCam->moveRelative(TranslateVector);

	//Placement = App->CL_Ogre->PlacementCam->getPosition();
	//Placement.y = CamPos.y - 3; // = Placement.y - (float)13.5 / 2;

	// Temp
	Placement.x = CamPos.x;
	Placement.y = CamPos.y;
	Placement.z = CamPos.z;

	return Placement;
}

// *************************************************************************
// *		Hide_AllObjects_Except:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Object::Hide_AllObjects_Except(int Index, bool Show)
{
	int Count = 0;
	while (Count < App->CL_Scene->Object_Count)
	{
		App->CL_Scene->V_Object[Count]->Object_Node->setVisible(Show);
		Count++;
	}

	Count = 0;
	while (Count < App->CL_Scene->Area_Count)
	{
		App->CL_Scene->B_Area[Count]->Area_Node->setVisible(Show);
		Count++;
	}

	if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
	{
		App->CL_Scene->B_Area[Index]->Area_Node->setVisible(true);
	}
	else
	{
		App->CL_Scene->V_Object[Index]->Object_Node->setVisible(true);
	}
}

// **************************************************************************
// *	  		Delete_Object:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_Object::Delete_Object()
{
	int MeshIndex = App->CL_Properties->Current_Selected_Object;
	btRigidBody* body = App->CL_Scene->V_Object[MeshIndex]->Phys_Body;

	if (body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(body);
	}

	App->CL_FileView->DeleteItem();

	App->CL_Scene->V_Object[MeshIndex]->Deleted = 1;
	App->CL_Scene->V_Object[MeshIndex]->Object_Node->setVisible(false);


	App->CL_Scene->flag_Scene_Modified = 1;

}
