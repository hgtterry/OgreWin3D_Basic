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
#include "CL64_Com_Objects.h"

CL64_Com_Objects::CL64_Com_Objects(void)
{
	flag_Show_Physics_Debug = 0;
	flag_Hide_All_Except = 0;
	flag_Show_Mesh_Debug = 1;
	flag_Dont_Clear_Objects = 0;
}

CL64_Com_Objects::~CL64_Com_Objects(void)
{
}

// *************************************************************************
// *	  		GetMesh_BB_Radius:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
float CL64_Com_Objects::GetMesh_BB_Radius(SceneNode* mNode)
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
Ogre::Vector3 CL64_Com_Objects::GetMeshBoundingBoxSize(SceneNode* mNode)
{
	Ogre::AxisAlignedBox myAAB = mNode->getAttachedObject(0)->getBoundingBox();  // the local AAB of the entity
	Ogre::Quaternion q = mNode->getOrientation();  // the orientation of the node (a quarternion which tells us how much it has been rotated)
	Ogre::Vector3 p = mNode->getPosition();  // the position of the node where it is now

	Ogre::Vector3 rotatedNearLeftTopCorner = q * myAAB.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_TOP) + p;


	// Retrieve the attached object's bounding box
	AxisAlignedBox aab = mNode->getAttachedObject(0)->getBoundingBox();

	AxisAlignedBox::Corners cc = mNode->getAttachedObject(0)->getBoundingBox().getAllCorners();
	
	// Scale the minimum and maximum points of the bounding box
	Ogre::Vector3 scaledMin = aab.getMinimum() * mNode->getScale();
	Ogre::Vector3 scaledMax = aab.getMaximum() * mNode->getScale();

	// Calculate the size of the bounding box
	Ogre::Vector3 size = Ogre::Vector3
	(
		fabs(scaledMax.x - scaledMin.x),
		fabs(scaledMax.y - scaledMin.y),
		fabs(scaledMax.z - scaledMin.z)
	);

	// Calculate the radius based on the size dimensions
	float radius = (size.x > size.z) ? size.z / 2.0f : size.x / 2.0f;

	return size; // Return the calculated size
}

// *************************************************************************
// *	  		GetPlacement:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
Ogre::Vector3 CL64_Com_Objects::GetPlacement(int Distance)
{
	Ogre::Vector3 Placement;
	Ogre::Vector3 CamPos;
	Ogre::Quaternion CamRot;
	Ogre::Radian mYaw;
	Ogre::Vector3 mDirection;

	CamPos = App->CL_Ogre->camNode->getPosition();
	mYaw = App->CL_Ogre->camNode->getOrientation().getYaw();

	// Temp
	Placement.x = CamPos.x;
	Placement.y = CamPos.y;
	Placement.z = CamPos.z;

	return Placement;
}

// *************************************************************************
// *		Hide_AllObjects_Except:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Objects::Hide_AllObjects_Except(int Index, bool Show)
{
	int Count = 0;
	while (Count < App->CL_Scene->Object_Count)
	{
		App->CL_Scene->B_Object[Count]->Object_Node->setVisible(Show);
		Count++;
	}

	/*Count = 0;
	while (Count < App->CL_Scene->Area_Count)
	{
		App->CL_Scene->B_Area[Count]->Area_Node->setVisible(Show);
		Count++;
	}*/

	/*if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
	{
		App->CL_Scene->B_Area[Index]->Area_Node->setVisible(true);
	}
	else*/
	{
		App->CL_Scene->B_Object[Index]->Object_Node->setVisible(true);
	}
}

// **************************************************************************
// *	  		Delete_Object:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_Com_Objects::Delete_Object()
{
	/*int MeshIndex = App->CL_Properties->Current_Selected_Object;
	btRigidBody* body = App->CL_Editor->B_Object[MeshIndex]->Phys_Body;

	if (body)
	{
		App->CL_Bullet->dynamicsWorld->removeCollisionObject(body);
	}

	App->CL_FileView->DeleteItem();

	App->CL_Editor->B_Object[MeshIndex]->flag_Deleted = 1;
	App->CL_Editor->B_Object[MeshIndex]->Object_Node->setVisible(false);


	App->CL_Editor->flag_Scene_Modified = 1;*/

}

// **************************************************************************
// *	  		GetIndex_By_Name:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
int CL64_Com_Objects::GetIndex_By_Name(char* Name)
{
	int Count = 0;
	int Total = App->CL_Scene->Object_Count;

	while (Count < Total)
	{
		if (App->CL_Scene->B_Object[Count]->flag_Deleted == 0)
		{
			int Result = 1;
			Result = strcmp(App->CL_Scene->B_Object[Count]->Object_Name, Name);
			if (Result == 0)
			{
				return Count;
			}
		}

		Count++;
	}

	return -1;
}

// *************************************************************************
// *		Clear_Modified_Objects:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Com_Objects::Clear_Modified_Objects()
{
	//if (flag_Dont_Clear_Objects == 0)
	//{
	//	// ---------------- Areas
	//	int Count = 0;
	//	while (Count < App->CL_Editor->Area_Count)
	//	{
	//		if (App->CL_Editor->B_Area[Count]->flag_Altered == 1)
	//		{
	//			App->CL_Editor->B_Area[Count]->flag_Altered = 0;
	//			App->CL_FileView->Mark_Clear(App->CL_Editor->B_Area[Count]->FileViewItem);
	//		}

	//		Count++;
	//	}

	//	// ---------------- Players
	//	Count = 0;
	//	while (Count < App->CL_Editor->Player_Count)
	//	{
	//		if (App->CL_Editor->B_Player[Count]->flag_Altered == 1)
	//		{
	//			App->CL_Editor->B_Player[Count]->flag_Altered = 0;
	//			App->CL_FileView->Mark_Clear(App->CL_Editor->B_Player[Count]->FileViewItem);
	//		}

	//		Count++;
	//	}

	//	// ---------------- Cameras
	//	Count = 0;
	//	while (Count < App->CL_Editor->Camera_Count)
	//	{
	//		if (App->CL_Editor->B_Camera[Count]->flag_Altered == 1)
	//		{
	//			App->CL_Editor->B_Camera[Count]->flag_Altered = 0;
	//			App->CL_FileView->Mark_Clear(App->CL_Editor->B_Camera[Count]->FileViewItem);
	//		}

	//		Count++;
	//	}

	//	// ---------------- Objects
	//	Count = 0;
	//	while (Count < App->CL_Editor->Object_Count)
	//	{
	//		if (App->CL_Editor->B_Object[Count]->flag_Altered == 1)
	//		{
	//			App->CL_Editor->B_Object[Count]->flag_Altered = 0;
	//			App->CL_FileView->Mark_Clear(App->CL_Editor->B_Object[Count]->FileViewItem);
	//		}

	//		Count++;
	//	}

	//	// ---------------- Display Counters
	//	Count = 0;
	//	while (Count < App->CL_Editor->Counters_Count)
	//	{
	//		if (App->CL_Editor->B_Counter[Count]->flag_Altered == 1)
	//		{
	//			App->CL_Editor->B_Counter[Count]->flag_Altered = 0;
	//			App->CL_FileView->Mark_Clear(App->CL_Editor->B_Counter[Count]->FileViewItem);
	//		}

	//		Count++;
	//	}

	//	// ---------------- Folders
	//	if (App->CL_Editor->Object_Count > 0)
	//	{
	//		App->CL_FileView->Mark_Clear_Folder(App->CL_FileView->FV_Objects_Folder);
	//		App->CL_FileView->Mark_Clear_Folder(App->CL_FileView->FV_Message_Trigger_Folder);
	//		App->CL_FileView->Mark_Clear_Folder(App->CL_FileView->FV_Move_Folder);
	//		App->CL_FileView->Mark_Clear_Folder(App->CL_FileView->FV_Sounds_Folder);
	//	}

	//	App->CL_Editor->flag_Scene_Modified = 0;

		//EnableMenuItem(App->mMenu, ID_FILE_SAVEPROJECTALL, MF_GRAYED);
	//}
}

// *************************************************************************
// *		 CheckNames_Objects:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
int CL64_Com_Objects::CheckNames_Objects(const char* name) 
{
	int totalObjects = App->CL_Scene->Object_Count;

	for (int count = 0; count < totalObjects; ++count) 
	{
		// Check if the object is not deleted
		if (App->CL_Scene->B_Object[count]->flag_Deleted == 0) 
		{
			// Compare the object name with the provided name
			if (strcmp(App->CL_Scene->B_Object[count]->Object_Name, name) == 0) 
			{
				return 1; // Name found
			}
		}
	}
	return 0; // Name not found
}

// *************************************************************************
// *	  Get_Adjusted_Object_Count:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
int CL64_Com_Objects::Get_Adjusted_Object_Count() const
{
	int newCount = 0; // Initialize the count of non-deleted objects
	int total = App->CL_Scene->Object_Count; // Get the total number of objects

	// Iterate through all objects in the scene
	for (int count = 0; count < total; ++count)
	{
		// Increment newCount if the object is not marked as deleted
		if (App->CL_Scene->B_Object[count]->flag_Deleted == 0)
		{
			++newCount;
		}
	}

	return newCount; // Return the count of non-deleted objects
}

// *************************************************************************
//		Get_BoundingBox_World_Centre:- Terry and Hazel Flanigan 2025	   *
// *************************************************************************
Ogre::Vector3 CL64_Com_Objects::Get_BoundingBox_World_Centre(int objectIndex)
{
	// Retrieve the object shape
	auto objectShape = App->CL_Scene->B_Object[objectIndex]->Shape;

	// Check if the shape is a TriMesh
	if (objectShape == Enums::Shape_TriMesh)
	{
		// Get the position of the object node
		return App->CL_Scene->B_Object[objectIndex]->Object_Node->getPosition();
	}
	else
	{
		// Get the center of the world bounding box for other shapes
		return App->CL_Scene->B_Object[objectIndex]->Object_Ent->getWorldBoundingBox(true).getCenter();
	}
}

// *************************************************************************
// *			Show_Entities:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
void CL64_Com_Objects::Show_Entities(bool Enable)
{
	for (int Count = 0; Count < App->CL_Scene->Object_Count; ++Count)
	{
		auto& currentObject = App->CL_Scene->B_Object[Count];

		if (currentObject->flag_Deleted == 0)
		{
			switch (currentObject->Usage)
			{
			case Enums::Obj_Usage_Sound:
			case Enums::Obj_Usage_Message:
			case Enums::Obj_Usage_Move:
			case Enums::Obj_Usage_Teleport:
			case Enums::Obj_Usage_Environment:
			case Enums::Obj_Usage_EnvironEntity:
				currentObject->Object_Node->setVisible(Enable);
				break;
			default:
				break;
			}
		}
	}
}
