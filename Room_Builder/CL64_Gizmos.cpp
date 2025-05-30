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
#include "CL64_Gizmos.h"

CL64_Gizmos::CL64_Gizmos(void)
{
	Crosshair_Ent = nullptr;
	Crosshair_Node = nullptr;

	RedAxis_Ent = nullptr;
	RedAxis_Node = nullptr;

	BlueAxis_Ent = nullptr;
	BlueAxis_Node = nullptr;

	GreenAxis_Ent = nullptr;
	GreenAxis_Node = nullptr;

	Last_Selected_Object = 0;

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
	MarkerBox_Setup();
	Load_PickSight();
	Load_Crosshair();
	Load_All_Axis();
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
// *	  		MarkerBB_Addjust:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Gizmos::MarkerBox_Adjust(int index)
{
	// Retrieve the object from the scene
	Base_Object* object = App->CL_Scene->B_Object[index];

	// Get the position and orientation of the object's node
	Ogre::Vector3 position = object->Object_Node->getPosition();
	Ogre::Quaternion rotation = object->Object_Node->getOrientation();

	// Get the bounding box size of the object's mesh
	Ogre::Vector3 size = App->CL_Com_Objects->GetMeshBoundingBoxSize(object->Object_Node);

	// Update the marker box dimensions
	MarkerBox_Update(size.x / 2, size.y / 2, size.z / 2);

	// Calculate the center of the bounding box and convert to world space
	Ogre::Vector3 center = object->Object_Node->getAttachedObject(0)->getBoundingBox().getCenter();
	Ogre::Vector3 worldSpacePosition = object->Object_Node->convertLocalToWorldPosition(center);

	// Set the position and orientation of the box node
	BoxNode->setPosition(worldSpacePosition);
	BoxNode->setOrientation(rotation);
	BoxNode->setVisible(true);

	// App->SBC_Markers->Move_Arrow(worldSpacePosition);
}

// *************************************************************************
// *		  	MarkerBox_Update:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Gizmos::MarkerBox_Update(float Depth, float Height, float Width)
{
	BoxManual->beginUpdate(0);
	BoxManual->colour(0, 1, 0, 1);

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

// *************************************************************************
// *	  		Load_Crosshair:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Gizmos::Load_Crosshair()
{
	Crosshair_Ent = App->CL_Ogre->mSceneMgr->createEntity("Crosshair", "axes.mesh", App->CL_Ogre->App_Resource_Group);
	Crosshair_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Crosshair_Node->attachObject(Crosshair_Ent);

	Crosshair_Node->setPosition(0, 0, 0);
	Crosshair_Node->setScale(1, 1, 1);
	Crosshair_Node->setVisible(false);
}

// **************************************************************************
// *	  			Load_All_Axis:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Gizmos::Load_All_Axis()
{
	//Load_Blue_Rot_Axis();
	Load_Red_Axis();
	Load_Green_Axis();
	Load_Blue_Axis();
}

// **************************************************************************
// *	  			Load_Red_Axis:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Gizmos::Load_Red_Axis()
{
	RedAxis_Ent = App->CL_Ogre->mSceneMgr->createEntity("Red_Axis", "RedAxes.mesh", App->CL_Ogre->App_Resource_Group);
	RedAxis_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	RedAxis_Node->attachObject(RedAxis_Ent);

	RedAxis_Node->setVisible(false);
	RedAxis_Node->setPosition(0, 0, 0);
	RedAxis_Node->setScale(1, 1, 2);
}

// **************************************************************************
// *	  			Load_Green_Axis:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Gizmos::Load_Green_Axis()
{
	GreenAxis_Ent = App->CL_Ogre->mSceneMgr->createEntity("Green_Axis", "GreenAxes.mesh", App->CL_Ogre->App_Resource_Group);
	GreenAxis_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	GreenAxis_Node->attachObject(GreenAxis_Ent);

	GreenAxis_Node->setVisible(false);
	GreenAxis_Node->setPosition(0, 0, 0);
	GreenAxis_Node->setScale(1, 2, 1);
}

// **************************************************************************
// *	  			Load_Blue_Axis:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Gizmos::Load_Blue_Axis()
{
	BlueAxis_Ent = App->CL_Ogre->mSceneMgr->createEntity("Blue_Axis", "BlueAxes.mesh", App->CL_Ogre->App_Resource_Group);
	BlueAxis_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	BlueAxis_Node->attachObject(BlueAxis_Ent);

	BlueAxis_Node->setVisible(false);
	BlueAxis_Node->setPosition(0, 0, 0);
	BlueAxis_Node->setScale(2, 1, 1);
}

// **************************************************************************
// *	  	Update_Red_Axis_Marker:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Gizmos::Update_Red_Axis_Marker(int Index)
{
	Ogre::Vector3 Centre = App->CL_Com_Objects->Get_BoundingBox_World_Centre(Index);

	RedAxis_Node->setPosition(Centre);
	RedAxis_Node->setVisible(true);
}

// **************************************************************************
// *	  	Update_Blue_Axis_Marker:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Gizmos::Update_Green_Axis_Marker(int Index)
{
	Ogre::Vector3 Centre = App->CL_Com_Objects->Get_BoundingBox_World_Centre(Index);

	GreenAxis_Node->setPosition(Centre);
	GreenAxis_Node->setVisible(true);
}

// **************************************************************************
// *	  	Update_Blue_Axis_Marker:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Gizmos::Update_Blue_Axis_Marker(int Index)
{
	Ogre::Vector3 Centre = App->CL_Com_Objects->Get_BoundingBox_World_Centre(Index);

	BlueAxis_Node->setPosition(Centre);
	BlueAxis_Node->setVisible(true);
}

// **************************************************************************
// *			Hide_Axis_Marker:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
void CL64_Gizmos::Hide_Axis_Marker()
{
	BlueAxis_Node->setVisible(false);
	RedAxis_Node->setVisible(false);
	GreenAxis_Node->setVisible(false);
}

// **************************************************************************
// *			highlight:- Terry and Hazel Flanigan 2024					*
// **************************************************************************
void CL64_Gizmos::highlight(Ogre::Entity* entity)
{
	unsigned short count = entity->getNumSubEntities();

	const Ogre::String file_name = "sdk_text_box.png";
	const Ogre::String rim_material_name = "_rim";

	for (unsigned short i = 0; i < count; ++i)
	{
		Ogre::SubEntity* subentity = entity->getSubEntity(i);

		const Ogre::String& old_material_name = subentity->getMaterialName();
		Ogre::String new_material_name = old_material_name + rim_material_name;

		Ogre::MaterialPtr new_material = MaterialManager::getSingleton().getByName(new_material_name);

		if (new_material == nullptr)
		{
			MaterialPtr old_material = MaterialManager::getSingleton().getByName(old_material_name);
			new_material = old_material->clone(new_material_name);

			Pass* pass = new_material->getTechnique(0)->getPass(0);
			Ogre::TextureUnitState* texture = pass->createTextureUnitState();
			texture->setTextureName(file_name);
			//texture->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
			texture->setColourOperationEx(Ogre::LBX_ADD, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);
			texture->setColourOpMultipassFallback(Ogre::SBF_ONE, Ogre::SBF_ONE);
			//texture->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_NORMAL);
		}

		subentity->setMaterial(new_material);
	}
}

// **************************************************************************
// *			unhighlight:- Terry and Hazel Flanigan 2024					*
// **************************************************************************
void CL64_Gizmos::unhighlight(Ogre::Entity* entity)
{
	unsigned short count = entity->getNumSubEntities();

	for (unsigned short i = 0; i < count; ++i)
	{
		Ogre::SubEntity* subentity = entity->getSubEntity(i);
		Ogre::SubMesh* submesh = subentity->getSubMesh();

		const Ogre::String& old_material_name = submesh->getMaterialName();
		const Ogre::String& new_material_name = subentity->getMaterialName();

		// if the entity is already using the original material then we're done. 
		if (0 == _stricmp(old_material_name.c_str(), new_material_name.c_str()))
			continue;

		// otherwise restore the original material name.
		subentity->setMaterialName(old_material_name);

	}
}