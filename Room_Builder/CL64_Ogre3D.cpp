/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_Ogre3D.h"

CL64_Ogre3D::CL64_Ogre3D(void)
{
	Export_Manual = nullptr;
	World_Manual = nullptr;

	mJustName[0] = 0;
	mFolder_Path[0] = 0;
	mSelected_Directory[0] = 0;
	mDirectory_Name[0] = 0;
	mExport_Path[0] = 0;
	mExport_PathAndFile_Mesh[0] = 0;
	mExport_PathAndFile_Material[0] = 0;

	mWorld_Mesh_JustName[0] = 0;
	mWorld_File_PathAndFile[0] = 0;
	mWorld_File_Path[0] = 0;
	mExport_Just_Name[0] = 0;
	Material_PathAndFile[0] = 0;

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;

	NameIndex = 0;
}

CL64_Ogre3D::~CL64_Ogre3D(void)
{
}

// *************************************************************************
// *		Set_World_Paths:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
void CL64_Ogre3D::Set_World_Paths(void)
{
	char Num[100];
	_itoa(NameIndex, Num, 10);

	strcpy(mWorld_Mesh_JustName, "World");
	strcat(mWorld_Mesh_JustName, Num);

	strcpy(mWorld_File_PathAndFile, App->RB_Directory_FullPath);
	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, "Data");
	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, "World_Test");

	strcpy(mWorld_File_Path, mWorld_File_PathAndFile);

	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, mWorld_Mesh_JustName);
	strcat(mWorld_File_PathAndFile, ".mesh");

	strcpy(mExport_Just_Name, mWorld_Mesh_JustName);

	NameIndex++;
	
	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;
}

// *************************************************************************
// *		Set_Export_Paths:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CL64_Ogre3D::Set_Export_Paths(void)
{
	strcpy(mSelected_Directory, App->CL_Export->mFolder_Path);
	strcpy(mDirectory_Name, App->CL_Export->mDirectory_Name);
	strcpy(mExport_Just_Name, App->CL_Export->mJustName);

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

// *************************************************************************
// *	  		Export_To_Ogre3D:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Ogre3D::Export_To_Ogre3D()
{
	Set_Export_Paths();

	CreateDirectory(mExport_Path, NULL);

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Ogre->Export_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->Export_Resource_Group);
	}

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->Export_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->Export_Resource_Group);

	Export_MaterialFile(mExport_PathAndFile_Material);
	DecompileTextures_TXL(mExport_Path);

	Export_Manual = App->CL_Ogre->mSceneMgr->createManualObject("OgreManual2");
	
	int A = 0;
	int B = 0;
	int C = 0;

	Export_Manual->setDynamic(false);
	Export_Manual->setCastShadows(false);

	Export_Manual->estimateVertexCount(App->CL_Model->VerticeCount);
	Export_Manual->estimateIndexCount(App->CL_Model->FaceCount);

	char MaterialNumber[MAX_PATH];
	char MatName[MAX_PATH];

	int GroupCountTotal = App->CL_Model->GroupCount;
	int Count = 0;
	int FaceCount = 0;
	int FaceIndex = 0;

	while (Count < GroupCountTotal)
	{
		_itoa(Count, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		Export_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST,App->CL_Ogre->Export_Resource_Group);

		FaceCount = 0;
		FaceIndex = 0;

		while (FaceCount < App->CL_Model->Group[Count]->GroupFaceCount)
		{
			A = App->CL_Model->Group[Count]->Face_Data[FaceCount].a;
			B = App->CL_Model->Group[Count]->Face_Data[FaceCount].b;
			C = App->CL_Model->Group[Count]->Face_Data[FaceCount].c;

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

	Ogre::MeshPtr mesh = Export_Manual->convertToMesh("TestMesh",App->CL_Ogre->Export_Resource_Group);

	mesh->setAutoBuildEdgeLists(true);

	if (App->CL_Export->flag_Build_Edge_List == 1)
	{
		mesh->buildEdgeList();
	}
	
	App->CL_Ogre->mSceneMgr->destroyManualObject(Export_Manual);

	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();

	// Needs to be a Case
	if (App->CL_Export->mOgre_Version == Enums::Export_Ogre_Latest)
	{
		ms->exportMesh(mesh.get(), mExport_PathAndFile_Mesh, Ogre::MeshVersion::MESH_VERSION_LATEST);
	}

	if (App->CL_Export->mOgre_Version == Enums::Export_Ogre_18)
	{
		ms->exportMesh(mesh.get(), mExport_PathAndFile_Mesh, Ogre::MeshVersion::MESH_VERSION_1_8);
	}

	if (App->CL_Export->mOgre_Version == Enums::Export_Ogre_17)
	{
		ms->exportMesh(mesh.get(), mExport_PathAndFile_Mesh, Ogre::MeshVersion::MESH_VERSION_1_7);
	}

	delete(ms);

	char File[MAX_PATH];
	strcpy(File, mExport_Just_Name);
	strcat(File, ".mesh");
	App->Say(File,(LPSTR)"Exported");

}

// *************************************************************************
// *		Export_MaterialFile:- Terry and Hazel Flanigan 2025		   	   *
// *************************************************************************
void CL64_Ogre3D::Export_MaterialFile(char* MatFileName)
{
	char MatName[255];
	char File[255];
	char MaterialNumber[255];

	Ogre::String OMatFileName = MatFileName;
	Ogre::String OFile;
	Ogre::String OMatName;

	int numMaterials = App->CL_Model->GroupCount;

	Ogre::MaterialManager& matMgrSgl = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialSerializer matSer;

	for (int i = 0; i < numMaterials; ++i)
	{
		_itoa(i, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		strcpy(File, App->CL_Model->Group[i]->Text_FileName);

		OMatName = MatName;
		OFile = File;

		Ogre::MaterialPtr ogremat = matMgrSgl.create(OMatName,App->CL_Ogre->Export_Resource_Group);


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
// *	  		Convert_ToOgre3D:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Ogre3D::Convert_ToOgre3D(bool Create)
{
	//Do_Timer

	if (App->CL_Mesh_Mgr->World_Ent)
	{
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Mesh_Mgr->World_Ent);
		App->CL_Mesh_Mgr->World_Ent = nullptr;
	}

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Ogre->World_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->World_Resource_Group);
	}

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->World_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->World_Resource_Group);

	Set_World_Paths();

	CreateMaterialFile();

	if (Create == 1)
	{
		World_Manual = App->CL_Ogre->mSceneMgr->createManualObject("OgreManual2");
		World_Manual->setRenderQueueGroup(2);
	}

	int A = 0;
	int B = 0;
	int C = 0;

	World_Manual->setDynamic(false);
	World_Manual->setCastShadows(false);

	World_Manual->estimateVertexCount(App->CL_Model->VerticeCount);
	World_Manual->estimateIndexCount(App->CL_Model->FaceCount);

	char MaterialNumber[255];
	char MatName[255];

	int GroupCountTotal = App->CL_Model->GroupCount;
	int Count = 0;
	int FaceCount = 0;
	int FaceIndex = 0;
	int TotalFaces = 0;

	if (App->CL_Model->GroupCount == 0)
	{
		App->Say("No Groups");
		return;
	}

	while (Count < GroupCountTotal)
	{
		_itoa(Count, MaterialNumber, 10);
		strcpy(MatName, mWorld_Mesh_JustName);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		World_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST, App->CL_Ogre->World_Resource_Group);

		FaceCount = 0;
		FaceIndex = 0;

		TotalFaces = TotalFaces + App->CL_Model->Group[Count]->GroupFaceCount;
		if (App->CL_Model->Group[Count]->GroupFaceCount == 0)
		{
			//App->Say("No Faces"); hgtterry Debug
		}

		while (FaceCount < App->CL_Model->Group[Count]->GroupFaceCount)
		{
			A = App->CL_Model->Group[Count]->Face_Data[FaceCount].a;
			B = App->CL_Model->Group[Count]->Face_Data[FaceCount].b;
			C = App->CL_Model->Group[Count]->Face_Data[FaceCount].c;

			// --------------------------------------------------

			Get_Data(Count, A);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, B);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, C);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);

			FaceIndex++;
			FaceCount++;
		}

		World_Manual->end();

		Count++;
	}

	if (World_Manual->getNumSections() == 0)
	{
		App->Say("Can not create Ogre Sections");
		App->CL_Ogre->mSceneMgr->destroyManualObject(World_Manual);
		return;
	}

	
	Ogre::MeshPtr mesh = World_Manual->convertToMesh("TestMesh", App->CL_Ogre->World_Resource_Group);

	mesh->setAutoBuildEdgeLists(true);
	mesh->buildEdgeList();

	App->CL_Ogre->mSceneMgr->destroyManualObject(World_Manual);

	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();
	ms->exportMesh(mesh.get(), mWorld_File_PathAndFile);
	delete(ms);

	char OutputFolder[MAX_PATH];
	strcpy(OutputFolder, mWorld_File_Path);
	strcat(OutputFolder, "\\");

	//DecompileTextures_TXL(OutputFolder);

	char Name[MAX_PATH];
	strcpy(Name, mWorld_Mesh_JustName);
	strcat(Name, ".mesh");

	char BufPath[MAX_PATH];
	strcpy(BufPath, App->RB_Directory_FullPath);
	strcat(BufPath, "\\Data\\Room_Builder\\Default.zip");

	if (App->CL_Mesh_Mgr->World_Ent)
	{
		App->CL_Mesh_Mgr->World_Node->detachAllObjects();

		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Mesh_Mgr->World_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Mesh_Mgr->World_Ent);

		App->CL_Mesh_Mgr->World_Node = NULL;
		App->CL_Mesh_Mgr->World_Ent = NULL;

		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->World_Resource_Group);

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(BufPath, "Zip", App->CL_Ogre->World_Resource_Group);

		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->World_Resource_Group);
		
	}
	else
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(BufPath, "Zip", App->CL_Ogre->World_Resource_Group);
	}

	
	App->CL_Mesh_Mgr->World_Ent = App->CL_Ogre->mSceneMgr->createEntity(Name);
	App->CL_Mesh_Mgr->World_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();

	App->CL_Mesh_Mgr->World_Node->attachObject(App->CL_Mesh_Mgr->World_Ent);

	App->CL_Mesh_Mgr->World_Node->setPosition(0, 0, 0);
	App->CL_Mesh_Mgr->World_Node->setVisible(true);
	App->CL_Mesh_Mgr->World_Node->setScale(1, 1, 1);
	
	remove(mWorld_File_PathAndFile);
	remove(Material_PathAndFile);

	//Get_Timer
}

// *************************************************************************
// *		CreateMaterialFile:- Terry and Hazel Flanigan 2025		   	   *
// *************************************************************************
void CL64_Ogre3D::CreateMaterialFile()
{
	Material_PathAndFile[0] = 0;

	strcpy(Material_PathAndFile, mWorld_File_Path);
	strcat(Material_PathAndFile, "\\");
	strcat(Material_PathAndFile, mWorld_Mesh_JustName);
	strcat(Material_PathAndFile, ".material");

	char MatName[255];
	char File[255];
	char MaterialNumber[255];

	Ogre::String OMatFileName = Material_PathAndFile;
	Ogre::String OFile;
	Ogre::String OMatName;

	int numMaterials = App->CL_Model->GroupCount;

	Ogre::MaterialManager& matMgrSgl = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialSerializer matSer;

	for (int i = 0; i < numMaterials; ++i)
	{
		_itoa(i, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		strcpy(File, App->CL_Model->Group[i]->Text_FileName);
		//App->Say_Win(File);

		OMatName = MatName;
		OFile = File;

		Ogre::MaterialPtr ogremat = matMgrSgl.create(OMatName, App->CL_Ogre->World_Resource_Group);


		if (0 < strlen(File))
		{
			ogremat->getTechnique(0)->getPass(0)->createTextureUnitState(File);

			if (_stricmp(File + strlen(File) - 4, ".TGA") == 0)
			{
				ogremat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER, 128);
			}

			if (_stricmp(File + strlen(File) - 4, ".png") == 0)
			{
				ogremat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER, 128);
			}

			if (_stricmp(File + strlen(File) - 4, ".dds") == 0)
			{
				//Debug
				//ogremat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER, 128);
				//ogremat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(true);
			}
		}

		matSer.queueForExport(ogremat);
	}

	matSer.exportQueued(OMatFileName);
}

// *************************************************************************
// *			Get_Data:- Terry and Hazel Flanigan 2025  			   	   *
// *************************************************************************
void CL64_Ogre3D::Get_Data(int Index, int FaceIndex)
{
	x = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].x;
	y = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].y;
	z = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].z;

	u = App->CL_Model->Group[Index]->MapCord_Data[FaceIndex].u;
	v = App->CL_Model->Group[Index]->MapCord_Data[FaceIndex].v;

	nx = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].x;
	ny = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].y;
	nz = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].z;
}

// *************************************************************************
// *		DecompileTextures_TXL:- Terry and Hazel Flanigan 2025  	   	   *
// *************************************************************************
bool CL64_Ogre3D::DecompileTextures_TXL(char* PathAndFile)
{
	char buf[MAX_PATH];

	int GroupCount = 0;
	int GroupCountTotal = App->CL_Model->GroupCount;

	while (GroupCount < GroupCountTotal)
	{
		strcpy(buf, App->CL_Model->Group[GroupCount]->Text_FileName);
		Export_Texture(buf, PathAndFile);

		GroupCount++;
	}

	return 1;
}

// *************************************************************************
// *			Export_Texture:- Terry and Hazel Flanigan 2025    	 	   *
// *************************************************************************
bool CL64_Ogre3D::Export_Texture(char* Name, char* Folder)
{
	Ogre::String mFileString;
	
	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	for (i = RFI->begin(); i != iend; ++i)
	{
		if (i->filename == Name)
		{
			mFileString.clear();

			Ogre::DataStreamPtr ff = i->archive->open(i->filename);

			mFileString = ff->getAsString();

			char mFileName[MAX_PATH];
			strcpy(mFileName, Folder);
			strcat(mFileName, Name);

			std::ofstream outFile;
			outFile.open(mFileName, std::ios::binary);
			outFile << mFileString;
			outFile.close();

			mFileString.clear();
			return 1;
		}
	}

	return 1;
}

