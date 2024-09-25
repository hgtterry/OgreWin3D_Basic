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
#include "CL64_Physics.h"

CL64_Physics::CL64_Physics(void)
{
}

CL64_Physics::~CL64_Physics(void)
{
}

// *************************************************************************
//					Set_Physics:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Physics::Set_Physics(int Index)
{
	App->CL_Scene->V_Object[Index]->Physics_Quat = App->CL_Scene->V_Object[Index]->Object_Node->getOrientation();

	float w = App->CL_Scene->V_Object[Index]->Physics_Quat.w;
	float x = App->CL_Scene->V_Object[Index]->Physics_Quat.x;
	float y = App->CL_Scene->V_Object[Index]->Physics_Quat.y;
	float z = App->CL_Scene->V_Object[Index]->Physics_Quat.z;
	App->CL_Scene->V_Object[Index]->Phys_Body->getWorldTransform().setRotation(btQuaternion(x, y, z, w));

	App->CL_Scene->V_Object[Index]->Object_Node->setScale(App->CL_Scene->V_Object[Index]->Mesh_Scale);

	Ogre::Vector3 Scale = App->CL_Scene->V_Object[Index]->Object_Node->getScale();
	App->CL_Scene->V_Object[Index]->Phys_Body->getCollisionShape()->setLocalScaling(btVector3(Scale.x, Scale.y, Scale.z));

	UpDate_Physics_And_Visuals(Index);

	App->CL_Scene->V_Object[Index]->Physics_Valid = 1;

}

// *************************************************************************
// *				UpDate_Physics_And_Visuals Terry Flanigtan		 	   *
// *************************************************************************
void CL64_Physics::UpDate_Physics_And_Visuals(int Index)
{
	if (App->CL_Scene->V_Object[Index]->Shape == Enums::Shape_TriMesh)
	{

	}
	else
	{
		//if (App->CL_Scene->V_Object[Index]->Physics_Valid == 1)
		{
			Set_Physics_Position(Index);
		}
	}


	//App->SBC_Markers->MarkerBB_Addjust(Index);

	//// Needs Looking at
	//App->CL_Scene->V_Object[Index]->Altered = 1;
	//App->SBC_FileView->Mark_Altered(App->CL_Scene->V_Object[Index]->FileViewItem);
	//App->CL_Scene->Scene_Modified = 1;
}

// *************************************************************************
// *	  				Set_Physics_Position Terry Flanigan				   *
// *************************************************************************
void CL64_Physics::Set_Physics_Position(int Index)
{
	//AxisAlignedBox worldAAB = App->CL_Scene->V_Object[Index]->Object_Ent->getWorldBoundingBox();// getBoundingBox();
	////worldAAB.transformAffine(App->CL_Scene->V_Object[Index]->Object_Node->_getFullTransform());
	//Ogre::Vector3 Centre = worldAAB.getCenter();

	//Ogre::Vector3 Centre = App->CL_Scene->V_Object[Index]->Object_Ent->getWorldBoundingBox().getCenter();
	Ogre::AxisAlignedBox bb = App->CL_Scene->V_Object[Index]->Object_Node->_getWorldAABB();
	Ogre::Vector3 Centre = bb.getCenter();

	App->CL_Scene->V_Object[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));
	App->CL_Scene->V_Object[Index]->Physics_Pos = Centre;
}
