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
#include "CL64_Com_Area.h"

CL64_Com_Area::CL64_Com_Area()
{
}

CL64_Com_Area::~CL64_Com_Area()
{
}

// *************************************************************************
// *		Set_Area_Defaults:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_Com_Area::Set_Area_Defaults(int Index)
{
	// Actor Scale ------------------------------------------------------
	App->CL_Scene->B_Area[Index]->Mesh_Scale.x = 1;
	App->CL_Scene->B_Area[Index]->Mesh_Scale.y = 1;
	App->CL_Scene->B_Area[Index]->Mesh_Scale.z = 1;

	// Actor Pos
	App->CL_Scene->B_Area[Index]->Mesh_Pos.x = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Pos.y = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Pos.z = 0;

	// Mesh Rotation
	App->CL_Scene->B_Area[Index]->Mesh_Rot.x = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Rot.y = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Rot.z = 0;

	// Mesh Quat
	App->CL_Scene->B_Area[Index]->Mesh_Quat.w = 1;
	App->CL_Scene->B_Area[Index]->Mesh_Quat.x = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Quat.y = 0;
	App->CL_Scene->B_Area[Index]->Mesh_Quat.z = 0;

	// Physics Scale ------------------------------------------------------
	App->CL_Scene->B_Area[Index]->Physics_Scale.x = 1;
	App->CL_Scene->B_Area[Index]->Physics_Scale.y = 1;
	App->CL_Scene->B_Area[Index]->Physics_Scale.z = 1;

	// Physics Pos
	App->CL_Scene->B_Area[Index]->Physics_Pos.x = 0;
	App->CL_Scene->B_Area[Index]->Physics_Pos.y = 0;
	App->CL_Scene->B_Area[Index]->Physics_Pos.z = 0;

	// Physics Rotation
	App->CL_Scene->B_Area[Index]->Physics_Rot.x = 0;
	App->CL_Scene->B_Area[Index]->Physics_Rot.y = 0;
	App->CL_Scene->B_Area[Index]->Physics_Rot.z = 0;

	// Physics Quat
	App->CL_Scene->B_Area[Index]->Physics_Quat.w = 1;
	App->CL_Scene->B_Area[Index]->Physics_Quat.x = 0;
	App->CL_Scene->B_Area[Index]->Physics_Quat.y = 0;
	App->CL_Scene->B_Area[Index]->Physics_Quat.z = 0;
	App->CL_Scene->B_Area[Index]->Physics_Valid = 0;
	
}

// *************************************************************************
// *		Add_Aera_To_Project:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Com_Area::Add_Aera_To_Project(int Index, char* FileName, char* Resource_Location)
{
	//char B_Name[MAX_PATH];
	//char ConNum[MAX_PATH];

	//char Mesh_File[MAX_PATH];
	//strcpy(Mesh_File, FileName);

	//Base_Area* Area = App->CL_Scene->B_Area[Index];

	//strcpy_s(B_Name, "Area_");
	//_itoa(Index, ConNum, 10);
	//strcat(B_Name, ConNum);
	//strcpy(App->CL_Scene->B_Area[Index]->Area_Name, B_Name);

	//strcpy(Area->Area_FileName, FileName);
	//strcpy(Area->Area_Resource_Path, Resource_Location);

	//Area->Area_Ent = App->CL_Ogre->mSceneMgr->createEntity(App->CL_Scene->B_Area[Index]->Area_Name, Mesh_File, App->SBC_Scene->Project_Resource_Group);
	//Area->Area_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//Area->Area_Node->attachObject(Area->Area_Ent);

	//// Get Material Name
	//Ogre::String text = Area->Area_Ent->getMesh()->getSubMesh(0)->getMaterialName().c_str();
	//Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));
	//strcpy(Area->Material_File, Mat->getOrigin().c_str());

	//Area->Area_Node->setVisible(true);
	//Area->Area_Node->setPosition(Area->Mesh_Pos);
	//Area->Area_Node->setScale(Area->Mesh_Scale);
	//Area->Area_Node->setOrientation(Area->Mesh_Quat);

	//Create_Area_Trimesh(Index, Area);

	//Area->Usage = Enums::Usage_Room;

	//App->SBC_Scene->Area_Added = 1;
}
