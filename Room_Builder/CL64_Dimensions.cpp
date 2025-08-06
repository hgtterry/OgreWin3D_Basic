/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

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
#include "CL64_Dimensions.h"

CL64_Dimensions::CL64_Dimensions(void)
{
	flag_PosX_Selected = 1;
	flag_PosY_Selected = 0;
	flag_PosZ_Selected = 0;

	flag_ScaleX_Selected = 1;
	flag_ScaleY_Selected = 0;
	flag_ScaleZ_Selected = 0;

	flag_RotationX_Selected = 1;
	flag_RotationY_Selected = 0;
	flag_RotationZ_Selected = 0;

	Model_Pos_Delta = 1;
	Model_Scale_Delta = 0.01;
	Model_Rotation_Delta = 1;

	flag_Scale_Lock = 1;
}

CL64_Dimensions::~CL64_Dimensions(void)
{
}

// *************************************************************************
// *	Get_BoundingBox_World_Centre:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
Ogre::Vector3 CL64_Dimensions::Get_BoundingBox_World_Centre()
{
	/*if (*pBase_Shape == Enums::Shape_TriMesh)
	{
		Ogre::Vector3 Pos = pBase_Object_Node->getPosition();
		return Pos;
	}
	else
	{
	
		Ogre::Vector3 Centre = pBase_Object_Ent->getWorldBoundingBox(true).getCenter();
		return Centre;
	}*/
}

// *************************************************************************
// *			UpDate_Physics:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Dimensions::UpDate_Physics(int Index)
{
	//if (*pBase_Shape == Enums::Shape_TriMesh)
	//{

	//}
	//else
	//{
	//	{
	//		Set_Physics_Position();
	//	}
	//}

	//// Needs Looking at
	//App->CL_Scene->B_Object[Index]->flag_Altered = 1;
	//App->CL_FileView->Mark_Altered(App->CL_Scene->B_Object[Index]->FileViewItem);
	//App->CL_Level->flag_Level_is_Modified = 1;
}

// *************************************************************************
// *	  	Set_Physics_Position:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Dimensions::Set_Physics_Position()
{

	/*Ogre::Vector3 Centre = pBase_Object_Ent->getWorldBoundingBox(true).getCenter();
	pBase_Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));

	pBase_Physics_Pos->x = Centre.x;
	pBase_Physics_Pos->y = Centre.y;
	pBase_Physics_Pos->z = Centre.z;*/

}

// *************************************************************************
// *			Do_Position_New:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Dimensions::Do_Position_New()
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	auto& m_object = App->CL_Scene->B_Object[Index];

	App->CL_ImGui_Dialogs->Start_Dialog_Float_Vec3(1.00, 4, m_object->Object_Node->getPosition(), (LPSTR)"Position");

	while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 == 1)
	{
		App->CL_ImGui_Dialogs->BackGround_Render_Loop();

		m_object->Object_Node->setPosition(App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3);
		App->CL_Physics->Reset_Physics();
		App->CL_Gizmos->MarkerBox_Adjust(Index);
	}

	App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

	if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
	{
		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

		m_object->Object_Node->setPosition(App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3);
		m_object->Mesh_Pos = App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3;

		if (m_object->Shape == Enums::Shape_TriMesh)
		{
			m_object->Phys_Body->getWorldTransform().setOrigin(btVector3(m_object->Mesh_Pos.x, m_object->Mesh_Pos.y, m_object->Mesh_Pos.z));
		}
		else
		{
			App->CL_Physics->Update_Object_Physics(Index);
		}

		// TODO Test
		App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

		App->CL_Gizmos->MarkerBox_Adjust(Index);

		App->CL_Scene->B_Object[Index]->flag_Altered = 1;
		App->CL_Level->flag_Level_is_Modified = true;
		App->CL_FileView->Mark_Altered(m_object->FileViewItem);
	}
	else
	{
		App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3 = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3;
		m_object->Object_Node->setPosition(App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3);
		App->CL_Physics->Reset_Physics();
	}

}

// *************************************************************************
// *			Do_Scale_New:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Dimensions::Do_Scale_New()
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	auto& m_object = App->CL_Scene->B_Object[Index];

	App->CL_ImGui_Dialogs->Start_Dialog_Float_Vec3(0.10, 2, m_object->Object_Node->getScale(), (LPSTR)"Scale");

	while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 == 1)
	{
		App->CL_ImGui_Dialogs->BackGround_Render_Loop();

		m_object->Object_Node->setScale(App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3);
		App->CL_Physics->Reset_Physics();
	}

	App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

	if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
	{
		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

		m_object->Object_Node->setScale(App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3);
		m_object->Mesh_Scale = App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3;

		if (m_object->Shape == Enums::Shape_TriMesh)
		{
			m_object->Phys_Body->getCollisionShape()->setLocalScaling(btVector3(m_object->Mesh_Scale.x, m_object->Mesh_Scale.y, m_object->Mesh_Scale.z));
			m_object->Phys_Body->getWorldTransform().setOrigin(btVector3(m_object->Mesh_Pos.x, m_object->Mesh_Pos.y, m_object->Mesh_Pos.z));
		}
		else
		{
			App->CL_Physics->Update_Object_Physics(Index);
		}

		// TODO Test
		App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name, true);

		App->CL_Gizmos->MarkerBox_Adjust(Index);

		m_object->flag_Altered = 1;
		App->CL_Level->flag_Level_is_Modified = true;
		App->CL_FileView->Mark_Altered(m_object->FileViewItem);
	}
	else
	{
		App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3 = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3;
		m_object->Object_Node->setScale(App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3);
		App->CL_Physics->Reset_Physics();
	}
}

// *************************************************************************
// *			Do_Rotation_New:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Dimensions::Do_Rotation_New()
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	auto& m_object = App->CL_Scene->B_Object[Index];

	Ogre::Vector3 Angles;

	// Last Updated Saved Angles

	Angles.x = m_object->Object_Node->getOrientation().getPitch(false).valueDegrees();
	Angles.y = m_object->Object_Node->getOrientation().getYaw(false).valueDegrees();
	Angles.z = m_object->Object_Node->getOrientation().getRoll(false).valueDegrees();

	App->CL_ImGui_Dialogs->Start_Dialog_Float_Vec3(1.0, 4, Angles, (LPSTR)"Rotation");

	// Monitor
	while (App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 == 1)
	{
		App->CL_ImGui_Dialogs->BackGround_Render_Loop();

		if (App->CL_ImGui_Dialogs->flag_Float_Altetered == 1)
		{
			m_object->Object_Node->resetOrientation();
			m_object->Object_Node->pitch(((Ogre::Degree)App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3.x),Ogre::Node::TS_WORLD);
			m_object->Object_Node->yaw(((Ogre::Degree)App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3.y), Ogre::Node::TS_WORLD);
			m_object->Object_Node->roll(((Ogre::Degree)App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3.z), Ogre::Node::TS_WORLD);

		}

		App->CL_ImGui_Dialogs->flag_Float_Altetered = 0;
	}

	App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

	// Apply Update Rotation
	if (App->CL_ImGui_Dialogs->flag_Float_Canceld == 0)
	{
		App->CL_ImGui_Dialogs->flag_Show_Dialog_Float_Vec3 = 0;

		m_object->Mesh_Rot.x = App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3.x;
		m_object->Mesh_Rot.y = App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3.y;
		m_object->Mesh_Rot.z = App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3.z;

		// 
		m_object->Mesh_Quat.w = m_object->Object_Node->getOrientation().w;
		m_object->Mesh_Quat.x = m_object->Object_Node->getOrientation().x;
		m_object->Mesh_Quat.y = m_object->Object_Node->getOrientation().y;
		m_object->Mesh_Quat.z = m_object->Object_Node->getOrientation().z;

		if (m_object->Shape == Enums::Shape_TriMesh)
		{
			m_object->Phys_Body->getWorldTransform().setRotation(btQuaternion(m_object->Mesh_Quat.x,
				m_object->Mesh_Quat.y, m_object->Mesh_Quat.z, m_object->Mesh_Quat.w));

			m_object->Phys_Body->getWorldTransform().setOrigin(btVector3(m_object->Mesh_Pos.x, m_object->Mesh_Pos.y, m_object->Mesh_Pos.z));
		}
		else
		{
			App->CL_Physics->Update_Object_Physics(Index);
		}

		App->CL_Gizmos->MarkerBox_Adjust(Index);

		// TODO Test
		App->CL_Brush_X->Set_Brush_From_Entity_ByName(m_object->Object_Name,true);

		m_object->flag_Altered = 1;
		App->CL_Level->flag_Level_is_Modified = true;
		App->CL_FileView->Mark_Altered(m_object->FileViewItem);
	}
	else // Canceled Reset 
	{
		App->CL_ImGui_Dialogs->m_Dialog_Float_Vec3 = App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3;

		m_object->Object_Node->resetOrientation();
		m_object->Object_Node->pitch(((Ogre::Degree)App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3.x), Ogre::Node::TS_WORLD);
		m_object->Object_Node->yaw(((Ogre::Degree)App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3.y), Ogre::Node::TS_WORLD);
		m_object->Object_Node->roll(((Ogre::Degree)App->CL_ImGui_Dialogs->m_Dialog_Float_Copy_Vec3.z), Ogre::Node::TS_WORLD);

		App->CL_Physics->Reset_Physics();
	}
}

