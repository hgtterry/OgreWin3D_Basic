/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Demos.h"

CL64_Demos::CL64_Demos(void)
{
	World_Ent = NULL;
	World_Node = NULL;

	flag_Demo_1_Running = 0;
	flag_Demo_2_Running = 0;

}

CL64_Demos::~CL64_Demos(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Demos::Reset_Class(void)
{
	flag_Demo_1_Running = 0;
	flag_Demo_2_Running = 0;
}

// *************************************************************************
// *			Start_Demo_1:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Demos::Start_Demo_1(void)
{
	App->CL_Scene->Clear_Scene_And_Reset();

	App->CL_Camera->Reset_View();

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;

	char Path_And_File[MAX_PATH];

	strcpy(Path_And_File, App->GD_Directory_FullPath);
	strcat(Path_And_File, "\\Media\\Rooms\\StartRoom.obj");
	
	strcpy(App->CL_File_IO->Model_FileName,"StartRoom.obj");
	strcpy(App->CL_File_IO->Model_Path_FileName, Path_And_File);

	App->CL_Assimp->SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
	App->CL_Importers->Assimp_Loader(false, "", "");

	World_Ent = App->CL_Converters->Convert_To_Ogre3D(1);
	
	World_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	World_Node->attachObject(World_Ent);

	World_Node->setPosition(0, 0, 0);
	World_Node->setOrientation(Ogre::Quaternion::IDENTITY);

	World_Node->setVisible(true);
	World_Node->setScale(1, 1, 1);

	App->CL_Player->Set_Player_Position(Ogre::Vector3(0,0,0));
	App->CL_Player->Set_Player_Rotation(btQuaternion(1, 0, 0, 0));

	App->CL_Scene->Model_Type = Enums::LoadedFile_None;
	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;

	App->CL_Bullet->Create_New_Trimesh(World_Ent, World_Node);
	App->CL_Ogre->Ogre3D_Listener->Run_Physics = 1;

	App->CL_TopDlg->flag_Toggle_PhysicaDebug_Node = 0;
	App->CL_Ogre->Bullet_Debug_Listener->btDebug_Node->setVisible(false);

	App->CL_Ogre->Show_Trays(false);

	App->CL_ImGui->flag_Show_Demo_Options = 1;

	App->CL_Grid->Enable_Grid_And_Hair(false);

	flag_Demo_1_Running = 1;
}

// *************************************************************************
// *			Start_Demo_2:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Demos::Start_Demo_2(void)
{
	App->CL_Scene->Clear_Scene_And_Reset();

	App->CL_Camera->Reset_View();

	App->CL_Grid->Enable_Grid_And_Hair(false);

	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;


	char FolderPath[MAX_PATH];
	strcpy(FolderPath,App->GD_Directory_FullPath);
	strcat(FolderPath, "\\Media\\Demo2\\RF_Level\\New_Room.mesh");

	strcpy(App->CL_File_IO->Model_Path_FileName, FolderPath);
	strcpy(App->CL_File_IO->Model_FileName,"New_Room.mesh");

	App->CL_Importers->Load_Ogre_Model(false);
	

	App->CL_Camera->Set_Camera_Rotation(0, 90, 0);
	App->CL_Camera->Set_Camera_Position(-237, -120, 800);

	App->CL_Player->Set_Player_Position(Ogre::Vector3( -237, -120, 800));
	App->CL_Player->Set_Player_Rotation(btQuaternion(0.7071068, 0, 0.7071068, 0));
	App->CL_Player->Set_Player_GroundSpeed(70);


	App->CL_Bullet->Create_New_Trimesh(App->CL_Scene->Imported_Ogre_Ent, App->CL_Scene->Imported_Ogre_Node);


	App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_First;
	App->CL_Ogre->Ogre3D_Listener->Run_Physics = 1;
	
	App->CL_Ogre->Show_Trays(false);

	App->CL_ImGui->flag_Show_Demo_Options = 1;

	App->CL_Grid->Enable_Grid_And_Hair(false);

	flag_Demo_2_Running = 1;
}

// *************************************************************************
// *				Reset_View:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Demos::Reset_View(void)
{
	if (flag_Demo_1_Running == 1)
	{
		App->CL_Camera->Reset_View();

		App->CL_Player->Set_Player_Position(Ogre::Vector3(0, 0, 0));
		App->CL_Player->Set_Player_Rotation(btQuaternion(1, 0, 0, 0));

		App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);
		App->CL_Scene->B_Player[0]->CameraPitch_Node->setOrientation(Ogre::Quaternion::IDENTITY);
	}

	if (flag_Demo_2_Running == 1)
	{
		App->CL_Camera->Set_Camera_Rotation(0, 90, 0);
		App->CL_Camera->Set_Camera_Position(-237, -120, 800);

		App->CL_Player->Set_Player_Position(Ogre::Vector3(-237, -120, 800));
		App->CL_Player->Set_Player_Rotation(btQuaternion(0.7071068, 0, 0.7071068, 0));
	}

	//float x = App->CL_Scene->B_Player[0]->StartPos.x;
	//float y = App->CL_Scene->B_Player[0]->StartPos.y;
	//float z = App->CL_Scene->B_Player[0]->StartPos.z;

	//App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(btVector3(x, y, z));
	//App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);

	//App->CL_Scene->B_Player[0]->CameraPitch_Node->setOrientation(Ogre::Quaternion::IDENTITY);

}
