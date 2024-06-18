/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Demos.h"

CL64_Demos::CL64_Demos(void)
{
}

CL64_Demos::~CL64_Demos(void)
{
}

// *************************************************************************
// *				Demo_1:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Demos::Demo_1(void)
{
	App->CL_Grid->Grid_SetVisible(0);
	App->CL_Grid->ShowGridFlag = 0;

	App->CL_Grid->ShowHair = 0;
	App->CL_Grid->Hair_SetVisible(0);

	App->CL_Ogre->flag_Hide_Test_Cube = 0;
	App->CL_Ogre->Show_Test_Cube();

	App->CL_Camera->Reset_View();
	App->CL_Ogre->OgreListener->CameraMode = Enums::Cam_Mode_Free;


	char Path_And_File[MAX_PATH];

	strcpy(Path_And_File, App->GD_Directory_FullPath);
	strcat(Path_And_File, "\\Media\\Rooms\\StartRoom.obj");
	
	strcpy(App->CL_File_IO->Model_FileName,"StartRoom.obj");
	strcpy(App->CL_File_IO->Model_Path_FileName, Path_And_File);

	App->CL_Assimp->SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
	App->CL_Importers->Assimp_Loader(false, "", "");


	Ogre::Entity* World_Ent = nullptr;
	Ogre::SceneNode* World_Node = nullptr;


	World_Ent = App->CL_Converters->Convert_ToOgre3D(1);
	World_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	World_Node->attachObject(World_Ent);

	World_Node->setPosition(0, 0, 0);
	World_Node->setVisible(true);
	World_Node->setScale(1, 1, 1);

	App->CL_Scene->Model_Type = Enums::LoadedFile_None;

}

// *************************************************************************
// *				Demo_2:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Demos::Demo_2(void)
{
	App->Say("Demo 2");
}
