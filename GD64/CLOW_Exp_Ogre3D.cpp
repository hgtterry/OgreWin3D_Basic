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
#include "CLOW_Exp_Ogre3D.h"

CLOW_Exp_Ogre3D::CLOW_Exp_Ogre3D(void)
{
	Export_Manual = NULL;
	Export_Resource_Group = "Export_Resource_Group";

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;

	mExport_Just_Name[0] = 0;
	mExport_PathAndFile_Mesh[0] = 0;
	mExport_PathAndFile_Material[0] = 0;;
	mExport_Path[0] = 0;
	mSelected_Directory[0] = 0;
	mDirectory_Name[0] = 0;

}

CLOW_Exp_Ogre3D::~CLOW_Exp_Ogre3D(void)
{
}

// *************************************************************************
// *	  		Export_To_Ogre3D:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CLOW_Exp_Ogre3D::Export_To_Ogre3D(bool Create)
{
	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(Export_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(Export_Resource_Group);
	}


	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Export_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(Export_Resource_Group);

	
	Set_Export_Paths();

	CreateDirectory(mExport_Path, NULL);

	CreateMaterialFile(mExport_PathAndFile_Material);
	
	//App->Say_Win(mExport_PathAndFile_Mesh);

	if (Create == 1)
	{
		Export_Manual = App->CL_Ogre->mSceneMgr->createManualObject("OgreManual2");
		Export_Manual->setRenderQueueGroup(2);
	}

	int A = 0;
	int B = 0;
	int C = 0;

	Export_Manual->setDynamic(false);
	Export_Manual->setCastShadows(false);

	Export_Manual->estimateVertexCount(App->CL_Scene->VerticeCount);
	Export_Manual->estimateIndexCount(App->CL_Scene->FaceCount);

	char MaterialNumber[255];
	char MatName[255];

	int GroupCountTotal = App->CL_Scene->GroupCount;
	int Count = 0;
	int FaceCount = 0;
	int FaceIndex = 0;

	while (Count < GroupCountTotal)
	{
		_itoa(Count, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		Export_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST, Export_Resource_Group);

		FaceCount = 0;
		FaceIndex = 0;

		while (FaceCount < App->CL_Scene->Group[Count]->GroupFaceCount)
		{
			A = App->CL_Scene->Group[Count]->Face_Data[FaceCount].a;
			B = App->CL_Scene->Group[Count]->Face_Data[FaceCount].b;
			C = App->CL_Scene->Group[Count]->Face_Data[FaceCount].c;

			// --------------------------------------------------

			Get_Data(Count, A);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, B);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, C);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);

			FaceIndex++;
			FaceCount++;
		}

		Export_Manual->end();

		Count++;
	}

	
	if (Export_Manual->getNumSections() == 0)
	{
		App->Say("Can not create Ogre Sections");
		return;
	}

	Ogre::MeshPtr mesh = Export_Manual->convertToMesh("TestMesh", Export_Resource_Group);
	
	mesh->setAutoBuildEdgeLists(true);
	mesh->buildEdgeList();

	App->CL_Ogre->mSceneMgr->destroyManualObject(Export_Manual);
	
	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();
	ms->exportMesh(mesh.get(), mExport_PathAndFile_Mesh);
	delete(ms);

	//DecompileTextures_TXL(mExport_Path);

	//CreateMaterialFile(mExport_PathAndFile_Material);
}

// *************************************************************************
// *			Get_Data:- Terry and Hazel Flanigan 2024  			   	   *
// *************************************************************************
void CLOW_Exp_Ogre3D::Get_Data(int Index, int FaceIndex)
{
	x = App->CL_Scene->Group[Index]->vertex_Data[FaceIndex].x;
	y = App->CL_Scene->Group[Index]->vertex_Data[FaceIndex].y;
	z = App->CL_Scene->Group[Index]->vertex_Data[FaceIndex].z;

	u = App->CL_Scene->Group[Index]->MapCord_Data[FaceIndex].u;
	v = App->CL_Scene->Group[Index]->MapCord_Data[FaceIndex].v;

	nx = App->CL_Scene->Group[Index]->Normal_Data[FaceIndex].x;
	ny = App->CL_Scene->Group[Index]->Normal_Data[FaceIndex].y;
	nz = App->CL_Scene->Group[Index]->Normal_Data[FaceIndex].z;
}

// *************************************************************************
// *		CreateMaterialFile:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
void CLOW_Exp_Ogre3D::CreateMaterialFile(char* MatFileName)
{
	char MatName[255];
	char File[255];
	char MaterialNumber[255];

	Ogre::String OMatFileName = MatFileName;
	Ogre::String OFile;
	Ogre::String OMatName;

	int numMaterials = App->CL_Scene->GroupCount;

	Ogre::MaterialManager& matMgrSgl = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialSerializer matSer;

	for (int i = 0; i < numMaterials; ++i)
	{
		_itoa(i, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		strcpy(File, App->CL_Scene->Group[i]->Text_FileName);

		OMatName = MatName;
		OFile = File;

		Ogre::MaterialPtr ogremat = matMgrSgl.create(OMatName, Export_Resource_Group);
			
		if (0 < strlen(File))
		{
			ogremat->getTechnique(0)->getPass(0)->createTextureUnitState(File);

			if (_stricmp(File + strlen(File) - 4, ".TGA") == 0)
			{
				ogremat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER, 128);
			}
		}

		matSer.queueForExport(ogremat);
	}

	matSer.exportQueued(OMatFileName);
}

// *************************************************************************
// *		Set_Export_Paths:- Terry and Hazel Flanigan 2023		 	   *
// *************************************************************************
void CLOW_Exp_Ogre3D::Set_Export_Paths(void)
{
	char ExportFolder[MAX_PATH];

	strcpy(mSelected_Directory, App->CL_Exporters->mFolder_Path);
	strcpy(mDirectory_Name, App->CL_Exporters->mDirectory_Name);
	strcpy(mExport_Just_Name, App->CL_Exporters->mJustName);

	strcpy(mExport_Path, mSelected_Directory);
	strcat(mExport_Path, "\\");
	strcat(mExport_Path, mDirectory_Name);
	strcat(mExport_Path, "\\");

	strcpy(mExport_PathAndFile_Mesh, mExport_Path);
	strcat(mExport_PathAndFile_Mesh, mExport_Just_Name);
	strcat(mExport_PathAndFile_Mesh, ".mesh");

	strcpy(mExport_PathAndFile_Material, mExport_Path);
	strcat(mExport_PathAndFile_Material, mExport_Just_Name);
	strcat(mExport_PathAndFile_Material, ".material");

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;

}
