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

	CurrentCamMode = 0;

	flag_GameMode_Running_Flag = 0;
	flag_Project_Resources_Created = 0;
	flag_Area_Added = 0;
	flag_Scene_Loaded = 0;
	flag_Player_Added = 0;
	flag_Scene_Modified = 0;

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
	App->CL_Gui_Environment->Reset_Class();
	App->CL_Project->Reset_Class();
	App->CL_FileView->Reset_Class();

//#pragma message(WRNG "Look At Terry")
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


	flag_Scene_Modified = 0;

	Reset_Class(); // This One

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Model;

	App->CL_Camera->Reset_View(); // Look At Terry

	App->CL_Resources->Delete_Project_Resources_Group();
	flag_Project_Resources_Created = 0;

	Reset_Counters();

	App->CL_Ogre->mSceneMgr->setSkyDome(false, "OW3D/CloudySky");

	//App->CL_FileView->SelectItem(App->CL_FileView->FV_LevelFolder);

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
	flag_Scene_Modified = 0;

	flag_Scene_Loaded = 0;

	Player_Location_Count = 0;
	//Locations_ID_Counter = 0;
}

// *************************************************************************
// *			Set_Scene:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Set_Scene()
{
	App->CL_Camera->Reset_View();
	App->CL_Player->Set_Player_GroundSpeed(70);

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;
	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;

	//App->CL_Ogre->mSceneMgr->setSkyDome(true, "OW3D/CloudySky");

	App->CL_ImGui->flag_Show_Demo_Options = 1;
}

// *************************************************************************
// *				Game_Mode:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Scene::Game_Mode(void)
{
	/*if (App->SBC_Front_Dlg->Use_Front_Dlg_Flag == 1)
	{
		App->CL_Ogre->OgreListener->Block_Mouse = 1;
		App->SBC_Keyboard->Block_Keyboard = 1;
		App->Block_Mouse_Buttons = 1;
		App->SBC_Front_Dlg->Show_Front_Dlg_Flag = 1;
	}*/

	flag_GameMode_Running_Flag = 1;

	//App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;

	App->CL_Grid->Grid_SetVisible(false);
	App->CL_Grid->Hair_SetVisible(false);

	//App->SBC_Markers->Arrow_Node->setVisible(0);

	App->CL_Com_Environments->GameMode(true);

	//App->CL_Ogre->Ogre3D_Listener->Run_Physics = 1;

	CurrentCamMode = App->CL_Ogre->Ogre3D_Listener->CameraMode;
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;


	//App->SBC_Markers->BoxNode->setVisible(false);

	Show_Entities(false); // Hide All Visible Trigers

	SetCursorPos(App->CursorPosX, App->CursorPosY);
	
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(App->ViewGLhWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos(App->ViewGLhWnd, NULL, 0, 0, cx, cy, SWP_NOZORDER);
	SetParent(App->ViewGLhWnd, NULL);

	App->CL_Ogre->mWindow->resize(cx, cy);

	App->CL_Ogre->mWindow->windowMovedOrResized();
	App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());

	Root::getSingletonPtr()->renderOneFrame();

	//if (App->SBC_Front_Dlg->Use_Front_Dlg_Flag == 0)
	//{
		SetCapture(App->ViewGLhWnd);// Bernie
		App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
		App->CUR = SetCursor(NULL);
	//}

	App->CL_Physics->Reset_Triggers();

	//App->SBC_Gui_Dialogs->Show_Physics_Console = 0;

	return 1;
}

// *************************************************************************
// *				Editor_Mode:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
bool CL64_Scene::Editor_Mode(void)
{
	flag_GameMode_Running_Flag = 0;
	//FullScreenMode_Flag = 0;

	//App->CL_Ogre->BulletListener->Render_Debug_Flag = 1;

	App->CL_Grid->Grid_SetVisible(true);
	App->CL_Grid->Hair_SetVisible(true);

	App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
	ReleaseCapture();
	SetCursor(App->CUR);

	if (App->CL_Scene->flag_Scene_Loaded == 1)
	{
		App->CL_Com_Environments->GameMode(false);

		Show_Entities(true); // Show All Visible Trigers

		App->CL_Physics->Reset_Triggers();
	}


	App->CL_Ogre->Ogre3D_Listener->CameraMode = CurrentCamMode;

	//App->CL_Vm_ImGui->Show_FPS = App->SBC_Dialogs->Saved_DoFPS;

	//App->SBC_Gui_Dialogs->Show_Physics_Console = 1;

	return 1;
}

// *************************************************************************
// *			Show_Entities:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
bool CL64_Scene::Show_Entities(bool TrueFalse)
{
	int Count = 0;
	while (Count < Object_Count)
	{
		if (V_Object[Count]->Deleted == 0)
		{
			if (V_Object[Count]->Usage == Enums::Stage_Usage_Sound)
			{
				V_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (V_Object[Count]->Usage == Enums::Stage_Usage_Message)
			{
				V_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (V_Object[Count]->Usage == Enums::Stage_Usage_Move)
			{
				V_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (V_Object[Count]->Usage == Enums::Stage_Usage_Teleport)
			{
				V_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (V_Object[Count]->Usage == Enums::Stage_Usage_Environment)
			{
				V_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (V_Object[Count]->Usage == Enums::Stage_Usage_EnvironEntity)
			{
				V_Object[Count]->Object_Node->setVisible(TrueFalse);
			}
		}
		Count++;
	}
	return 1;
}



