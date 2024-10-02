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
#include "CL64_Scene.h"

CL64_Scene::CL64_Scene(void)
{
	// Stage
	Object_Count = 0;
	Area_Count = 0;
	Camera_Count = 0;
	Player_Location_Count = 0;
	Counters_Count = 0;
	UniqueID_Object_Counter = 0;
	UniqueID_Counters_Count = 0;
	UniqueID_Area_Count = 0;

	flag_GameMode_Running_Flag = 0;
	flag_Project_Resources_Created = 0;
	flag_Area_Added = 0;
	flag_Scene_Loaded = 0;
	flag_Player_Added = 0;
	flag_Scene_Modified = 0;

	strcpy(Texture_FolderPath, "None");
	strcpy(FileName, "None");
	strcpy(Path_FileName, "None");
	strcpy(Model_FolderPath, "None");
	strcpy(JustName, "None");

	B_Area[19] = { nullptr };
	B_Locations[19] = { nullptr };
	B_Counter[19] = { nullptr };

	B_Player.reserve(20);
	V_Object.reserve(200);
}

CL64_Scene::~CL64_Scene(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Reset_Class()
{
	//int i; // Remove Physics Objects
	//for (i = App->SBC_Bullet->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	//{
	//	btCollisionObject* obj = App->SBC_Bullet->dynamicsWorld->getCollisionObjectArray()[i];
	//	App->SBC_Bullet->dynamicsWorld->removeCollisionObject(obj);
	//	delete obj;
	//}

	int Count = 0; // Remove Ogre Objects
	while (Count < Object_Count)
	{
		if (V_Object[Count]->Object_Node && V_Object[Count]->Object_Ent)
		{
			V_Object[Count]->Object_Node->detachAllObjects();

			App->CL_Ogre->mSceneMgr->destroySceneNode(V_Object[Count]->Object_Node);

			App->CL_Ogre->mSceneMgr->destroyEntity(V_Object[Count]->Object_Ent);

			V_Object[Count]->Object_Node = NULL;
			V_Object[Count]->Object_Ent = NULL;
		}

		Count++;
	}


	// Remove V_Objects
	Count = 0;
	int NumObjects = V_Object.size();

	while (Count < NumObjects)
	{
		delete V_Object[Count];
		Count++;
	}

	V_Object.resize(0);
}

// *************************************************************************
// *			Clear_Level:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Scene::Clear_Level()
{
	//App->SBC_Gui_Environ->Reset_Class();
	App->CL_Project->Reset_Class();
	App->CL_FileView->Reset_Class();
	App->CL_TopDlg->Reset_Class(); // Look At Terry
	App->CL_Properties->Reset_Class();

	//App->SBC_Markers->BoxNode->setVisible(false);
	//App->SBC_Markers->Arrow_Node->setVisible(false);

	//App->Set_Main_TitleBar(" ");


	if (App->CL_Scene->flag_Scene_Loaded == 1)
	{
		App->CL_Physics->Enable_Physics(0); // Look At Terry

		App->CL_Player->Reset_Class(); // Look At Terry

		App->CL_Com_Area->Reset_Class();

		// Bullet Related
		int i;
		for (i = App->CL_Bullet->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = App->CL_Bullet->dynamicsWorld->getCollisionObjectArray()[i];
			App->CL_Bullet->dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}
	}


	//Scene_Modified = 0;

	Reset_Class(); // This One

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Model;

	App->CL_Camera->Reset_View(); // Look At Terry

	App->CL_Resources->Delete_Project_Resources_Group();
	flag_Project_Resources_Created = 0;

	Reset_Counters();

	App->CL_Ogre->mSceneMgr->setSkyDome(false, "OW3D/CloudySky");

	//App->SBC_FileView->SelectItem(App->SBC_FileView->FV_LevelFolder);

	//App->CL_Ogre->mSceneMgr->destroyCamera("PlayerRay");
	App->CL_Ogre->mSceneMgr->destroyAllParticleSystems();

	App->CL_Ogre->RenderFrame(8);
	return 1;
}

// *************************************************************************
// *			Reset_Counters:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Scene::Reset_Counters()
{
	Object_Count = 0;
	UniqueID_Object_Counter = 0;

	Camera_Count = 0;
	//UniqueID_Camera_Count = 0;

	Counters_Count = 0;
	UniqueID_Counters_Count = 0;

	UniqueID_Area_Count = 0;

	flag_Project_Resources_Created = 0;

	flag_Scene_Modified = 0;
	//MessageNew_Count = 0;
	//UniqueID_MessageNew_Count = 500;

	//CurrentCamMode = 0;
	//Scene_Modified = 0;

	flag_Scene_Loaded = 0;

	Player_Location_Count = 0;
	//Locations_ID_Counter = 0;
}

// *************************************************************************
// *			Set_Scene:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Set_Scene()
{
	//App->CL_Ogre->Enable_Fog(true);

	App->CL_Camera->Reset_View();

	/*App->CL_Bullet->Create_New_Trimesh(App->CL_Scene->Imported_Ogre_Ent, App->CL_Scene->Imported_Ogre_Node);*/

	//App->CL_Player->Set_Player_Position(Ogre::Vector3(0, 20, 0));
	//App->CL_Player->Set_Player_Rotation(btQuaternion(0, 0, 1, 0));
	App->CL_Player->Set_Player_GroundSpeed(70);

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;
	App->CL_Ogre->Ogre3D_Listener->Run_Physics = 1;

	App->CL_Ogre->mSceneMgr->setSkyDome(true, "OW3D/CloudySky");

	App->CL_ImGui->flag_Show_Demo_Options = 1;
}



