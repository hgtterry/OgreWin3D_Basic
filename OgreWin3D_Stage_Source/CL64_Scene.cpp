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
	Map_Group_Count = 0;
	Object_Count = 0;
	Area_Count = 0;
	Camera_Count = 0;
	Player_Count = 0;
	Player_Location_Count = 0;
	Counters_Count = 0;
	UniqueID_Object_Counter = 0;
	UniqueID_Counters_Count = 0;
	UniqueID_Area_Count = 0;

	CurrentCamMode = 0;

	flag_Project_Resources_Created = 0;
	flag_Area_Added = 0;
	flag_Scene_Loaded = 0;
	flag_Player_Added = 0;
	flag_Camera_Added = 0;
	flag_Scene_Modified = 0;
	flag_GameMode_Running = 0;
	flag_FullScreen_Mode = 0;

	B_Area[19] = { nullptr };
	B_Locations[19] = { nullptr };
	B_Counter[19] = { nullptr };

	B_Player.reserve(20);
	B_Object.reserve(200);
	B_Camera.reserve(20);

	int Count = 0;
	while (Count < 4999)
	{
		Map_Group[Count] = nullptr;
		Count++;
	}

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
		if (B_Object[Count]->Object_Node && B_Object[Count]->Object_Ent)
		{
			B_Object[Count]->Object_Node->detachAllObjects();

			App->CL_Ogre->mSceneMgr->destroySceneNode(B_Object[Count]->Object_Node);

			App->CL_Ogre->mSceneMgr->destroyEntity(B_Object[Count]->Object_Ent);

			B_Object[Count]->Object_Node = NULL;
			B_Object[Count]->Object_Ent = NULL;
		}

		Count++;
	}


	// Remove B_Objects
	Count = 0;
	int NumObjects = B_Object.size();

	while (Count < NumObjects)
	{
		delete B_Object[Count];
		Count++;
	}

	Count = 0;
	int NumCameras = Camera_Count;

	while (Count < NumCameras)
	{
		if (App->CL_Scene->B_Camera[Count])
		{
			delete App->CL_Scene->B_Camera[Count];
		}
		
		Count++;
	}

	B_Camera.resize(0);
	B_Object.resize(0);

	Object_Count = 0;
	UniqueID_Object_Counter = 0;
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

	App->CL_Gizmos->Show_MarkerBox(false);
	//App->SBC_Markers->Arrow_Node->setVisible(false);

	//App->Set_Main_TitleBar(" ");


	if (App->CL_Scene->flag_Scene_Loaded == 1)
	{
		App->CL_Physics->Enable_Physics(0); // Look At Terry

		App->CL_Com_Player->Reset_Class(); // Look At Terry

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

	App->CL_Com_Cameras->Reset_View(); // Look At Terry

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
	flag_Camera_Added = 0;
	flag_Scene_Loaded = 0;

	Player_Location_Count = 0;
	//Locations_ID_Counter = 0;
}

// *************************************************************************
// *			Set_Scene:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Scene::Set_Scene()
{
	if (flag_Camera_Added == 0)
	{
		App->CL_Com_Cameras->Reset_View();
	}

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

	App->CL_Com_Player->Show_Player_And_Physics(true);

	App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node = 1;
	App->CL_Ogre->Bullet_Debug_Listener->btDebug_Node->setVisible(true);

	if (App->CL_Scene->flag_Area_Added == 1)
	{
		App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;
	}
	else
	{
		App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;
	}

	App->CL_ImGui->flag_Show_Demo_Options = 1;

	App->CL_Motions->Set_Player_Motion();
	
}

// *************************************************************************
// *				Game_Restart:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Scene::Game_Restart(void)
{

	App->CL_Com_Environments->Set_Environment_GameMode(0);
	App->CL_SoundMgr->SoundEngine->stopAllSounds();

	flag_GameMode_Running = 1;

	App->CL_Ogre->Bullet_Debug_Listener->flag_Render_Debug_Flag = 0;

	App->CL_Grid->Grid_SetVisible(0);
	App->CL_Grid->Hair_SetVisible(0);
	//App->CL_Gizmos->Arrow_Node->setVisible(0);

	App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;

	App->CL_Gizmos->BoxNode->setVisible(false);

	Show_Entities(false); // Hide All Visible Trigers

	SetCursorPos(App->CursorPosX, App->CursorPosY);

	if (App->CL_Build_Game->flag_Use_Front_Dlg == 0)
	{
		SetCapture(App->ViewGLhWnd);// Bernie
		App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
		App->CUR = SetCursor(NULL);
	}

	App->CL_Physics->Reset_Triggers();

	App->CL_Com_Environments->Set_Environment_GameMode(1);

	return 1;
}

// *************************************************************************
// *				Game_Mode:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Scene::Game_Mode(void)
{
	if (App->CL_Build_Game->flag_Use_Front_Dlg == 1)
	{
		if (App->CL_Dialogs->flag_Game_Start_Option == Enums::Game_Start_Full)
		{
			App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 1;
			App->CL_Keyboard->flag_Block_Keyboard = 1;
			App->flag_Block_Mouse_Buttons = 1;
			App->CL_Front_Dialog->flag_Show_Front_Dlg_Flag = 1;
		}
		else
		{
			SetCapture(App->ViewGLhWnd);
			App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
			App->CUR = SetCursor(NULL);
			App->CL_Front_Dialog->flag_Game_Running_Flag = 1;
		}
	}

	App->CL_ImGui->flag_Show_FPS = App->CL_Build_Game->flag_Show_FPS;

	flag_GameMode_Running = 1;

	//App->CL_Ogre->Bullet_Debug_Listener->Render_Debug_Flag = 0;

	App->CL_Grid->Grid_SetVisible(false);
	App->CL_Grid->Hair_SetVisible(false);
	App->CL_Gizmos->Show_MarkerBox(false);

	App->CL_Com_Environments->Set_Environment_GameMode(true);

	//App->CL_Ogre->Ogre3D_Listener->Run_Physics = 1;

	CurrentCamMode = App->CL_Ogre->Ogre3D_Listener->CameraMode;

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;

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

	if (App->CL_Build_Game->flag_Use_Front_Dlg == 0)
	{
		SetCapture(App->ViewGLhWnd);
		App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
		App->CUR = SetCursor(NULL);
	}

	App->CL_Physics->Reset_Triggers();

	App->CL_ImGui_Dialogs->flag_Show_Physics_Console = 0;
	App->CL_Com_Player->Show_Player_And_Physics(false);

	return 1;
}

// *************************************************************************
// *				Editor_Mode:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
bool CL64_Scene::Editor_Mode(void)
{
	App->CL_Front_Dialog->flag_Show_Front_Dlg_Flag = 0; // temp

	flag_GameMode_Running = 0;
	flag_FullScreen_Mode = 0;

	//App->CL_Ogre->BulletListener->Render_Debug_Flag = 1;

	App->CL_Grid->Grid_SetVisible(true);
	App->CL_Grid->Hair_SetVisible(true);

	App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
	ReleaseCapture();
	SetCursor(App->CUR);

	if (App->CL_Scene->flag_Scene_Loaded == 1)
	{
		App->CL_Com_Environments->Set_Environment_GameMode(false);

		Show_Entities(true); // Show All Visible Trigers

		App->CL_Physics->Reset_Triggers();
	}


	App->CL_Ogre->Ogre3D_Listener->CameraMode = CurrentCamMode;

	if (CurrentCamMode == Enums::Cam_Mode_Free)
	{
		App->CL_Com_Player->Show_Player_And_Physics(true);
	}
	else
	{
		App->CL_Com_Player->Show_Player_And_Physics(false);
	}

	App->CL_ImGui->flag_Show_FPS = App->CL_Build_Game->flag_Saved_Show_FPS;

	App->CL_ImGui_Dialogs->flag_Show_Physics_Console = 1;
	
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
		if (B_Object[Count]->flag_Deleted == 0)
		{
			if (B_Object[Count]->Usage == Enums::Obj_Usage_Sound)
			{
				B_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (B_Object[Count]->Usage == Enums::Obj_Usage_Message)
			{
				B_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (B_Object[Count]->Usage == Enums::Obj_Usage_Move)
			{
				B_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (B_Object[Count]->Usage == Enums::Obj_Usage_Teleport)
			{
				B_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (B_Object[Count]->Usage == Enums::Obj_Usage_Environment)
			{
				B_Object[Count]->Object_Node->setVisible(TrueFalse);
			}

			if (B_Object[Count]->Usage == Enums::Obj_Usage_EnvironEntity)
			{
				B_Object[Count]->Object_Node->setVisible(TrueFalse);
			}
		}
		Count++;
	}
	return 1;
}



