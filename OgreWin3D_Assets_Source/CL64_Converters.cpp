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
#include "CL64_Converters.h"

#pragma warning(disable:4996)

CL64_Converters::CL64_Converters(void)
{
	Export_Manual = nullptr;
	World_Manual =	nullptr;

	World_Ent =		nullptr;

	mWorld_Mesh_JustName[0] = 0;
	mWorld_File_PathAndFile[0] = 0;
	mWorld_File_Path[0] = 0;
	mExport_Just_Name[0] = 0;

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;

	Temp_Resource_Group = "Temp_Resource_Group";
}

CL64_Converters::~CL64_Converters(void)
{
}

// *************************************************************************
// *			Set_Paths:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Converters::Set_Paths(void)
{
	strcpy(mWorld_File_Path, App->CL_Scene->Model_FolderPath);

	strcpy(mWorld_File_PathAndFile, App->CL_Scene->Model_FolderPath);
	strcat(mWorld_File_PathAndFile, App->CL_Scene->JustName);
	strcat(mWorld_File_PathAndFile, ".mesh");

	strcpy(mWorld_Mesh_JustName, App->CL_Scene->JustName);
	strcpy(mExport_Just_Name, App->CL_Scene->JustName);
	
	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;
}

// *************************************************************************
// *	  		Convert_To_Ogre3D:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
Ogre::Entity* CL64_Converters::Convert_To_Ogre3D(bool Create)
{
	if (World_Ent)
	{
		App->CL_Ogre->mSceneMgr->destroyEntity(World_Ent);
		World_Ent = nullptr;

	}

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(Temp_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(Temp_Resource_Group);
	}

	
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Temp_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(Temp_Resource_Group);
	
	Set_Paths();

	CreateMaterialFile();
	
	App->CL_Scene->Ogre_Face_Count = 0;
	
	if (Create == 1)
	{
		World_Manual = App->CL_Ogre->mSceneMgr->createManualObject();
		World_Manual->setRenderQueueGroup(2);
	}
	
	int A = 0;
	int B = 0;
	int C = 0;

	World_Manual->setDynamic(false);
	World_Manual->setCastShadows(false);

	World_Manual->estimateVertexCount(App->CL_Scene->VerticeCount);
	World_Manual->estimateIndexCount(App->CL_Scene->FaceCount);

	char MaterialNumber[255];
	char MatName[255];

	int GroupCountTotal = App->CL_Scene->GroupCount;
	int Count = 0;
	int FaceCount = 0;
	int FaceIndex = 0;
	int TotalFaces = 0;

	while (Count < GroupCountTotal)
	{
		_itoa(Count, MaterialNumber, 10);
		strcpy(MatName, mWorld_Mesh_JustName);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		CreateMaterial_Resource(MatName);
		
		World_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST, Temp_Resource_Group);
		
		FaceCount = 0;
		FaceIndex = 0;

		TotalFaces = TotalFaces + App->CL_Scene->Group[Count]->GroupFaceCount;

		while (FaceCount < App->CL_Scene->Group[Count]->GroupFaceCount)
		{
			A = App->CL_Scene->Group[Count]->Face_Data[FaceCount].a;
			B = App->CL_Scene->Group[Count]->Face_Data[FaceCount].b;
			C = App->CL_Scene->Group[Count]->Face_Data[FaceCount].c;

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
	
	
	App->CL_Scene->Ogre_Face_Count = TotalFaces;

	if (World_Manual->getNumSections() == 0)
	{
		App->Say("Can not create Ogre Sections");
	}

	Ogre::MeshPtr mesh = World_Manual->convertToMesh("TestMesh", Temp_Resource_Group);

	mesh->setAutoBuildEdgeLists(true);
	mesh->buildEdgeList();
	
	App->CL_Ogre->mSceneMgr->destroyManualObject(World_Manual);

	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();
	ms->exportMesh(mesh.get(), mWorld_File_PathAndFile);
	delete(ms);

	char Name[MAX_PATH];
	strcpy(Name, mWorld_Mesh_JustName);
	strcat(Name, ".mesh");
	
	Create_Resource_Group();
	
	World_Ent = App->CL_Ogre->mSceneMgr->createEntity(Name);
	
	remove(mWorld_File_PathAndFile);
	remove(Material_PathAndFile);
	
	// Needs Looking At
	App->CL_Mesh_Manager->Get_Ogre_Mesh_Data(World_Ent);

	return World_Ent;
}

// *************************************************************************
// *		Create_Resource_Group():- Terry and Hazel Flanigan 2024    	   *
// *************************************************************************
void CL64_Converters::Create_Resource_Group()
{
	if (World_Ent)
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->World_Resource_Group);

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->World_Resource_Group);

	}
	else
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->World_Resource_Group);
	}
	
}

// *************************************************************************
// *		CreateMaterial_Resource:- Terry and Hazel Flanigan 2024    	   *
// *************************************************************************
void CL64_Converters::CreateMaterial_Resource(char* MatName)
{

	MaterialManager* omatMgr = MaterialManager::getSingletonPtr();

	auto status = omatMgr->createOrRetrieve(MatName, Temp_Resource_Group);

}

// *************************************************************************
// *		CreateMaterialFile:- Terry and Hazel Flanigan 2024		   	   *
// *************************************************************************
void CL64_Converters::CreateMaterialFile()
{
	Material_PathAndFile[0] = 0;

	strcpy(Material_PathAndFile, mWorld_File_Path);
	strcat(Material_PathAndFile, mWorld_Mesh_JustName);
	strcat(Material_PathAndFile, ".material");

	char MatName[255];
	char File[255];
	char MaterialNumber[255];

	Ogre::String OMatFileName = Material_PathAndFile;
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

		Ogre::MaterialPtr ogremat = matMgrSgl.create(OMatName, Temp_Resource_Group);


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
// *			Get_Data:- Terry and Hazel Flanigan 2024  			   	   *
// *************************************************************************
void CL64_Converters::Get_Data(int Index, int FaceIndex)
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
// *			Create_MeshGroups:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Converters::Create_MeshGroups(Ogre::Entity* Ogre_Entity)
{
	const int SubMeshCount = Ogre_Entity->getNumSubEntities();

	for (int Count = 0; Count < SubMeshCount; ++Count)
	{
		App->CL_Scene->Create_Mesh_Group(Count); // Create new Mesh Group
		auto& meshGroup = App->CL_Scene->Group[Count];

		// Initialize mesh group properties
		meshGroup->GroupVertCount = 0;
		meshGroup->MaterialIndex = -1;
		strcpy(meshGroup->Text_FileName, "No_Texture");

		// Construct group name
		snprintf(meshGroup->GroupName, sizeof(meshGroup->GroupName), "Group_%d", Count);

		// Retrieve subMesh and set material name
		const Ogre::SubMesh* subMesh = Ogre_Entity->getSubEntity(Count)->getSubMesh();
		strcpy(meshGroup->MaterialName, subMesh->getMaterialName().c_str());

		// Set vertex and indices count if shared vertices flag is false
		if (App->CL_Mesh_Manager->flag_Has_Shared_Vertices == false)
		{
			meshGroup->GroupVertCount = subMesh->vertexData->vertexCount;
			meshGroup->IndicesCount = subMesh->vertexData->vertexCount;
		}
	}

	// Update texture and group counts
	App->CL_Scene->TextureCount = SubMeshCount;
	App->CL_Scene->GroupCount = SubMeshCount;
}

// *************************************************************************
// *		Get_SkeletonInstance:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Converters::Get_SkeletonInstance(Ogre::Entity* Ogre_Entity)
{

	int Loop = 0;
	if (!Ogre_Entity)
	{
		return 0;
	}

	Ogre::SkeletonInstance* skeletonInstance = Ogre_Entity->getSkeleton();

	if (skeletonInstance)
	{
		App->CL_Scene->BoneCount = skeletonInstance->getNumBones();

		Ogre::Skeleton::BoneIterator itor = skeletonInstance->getBoneIterator();
		while (itor.hasMoreElements())
		{
			Ogre::Bone* bone = itor.getNext();

			App->CL_Scene->S_Bones[Loop] = new Bone_Type;

			Ogre::Node* Parent = bone->getParent();
			if (Parent == NULL)
			{
				App->CL_Scene->S_Bones[Loop]->Parent = -1;
			}
			else
			{
				Ogre::String ParentName = Parent->getName();
				Ogre::Bone* Parentbone = skeletonInstance->getBone(ParentName);
				App->CL_Scene->S_Bones[Loop]->Parent = Parentbone->getHandle();
			}

			strcpy(App->CL_Scene->S_Bones[Loop]->BoneName, bone->getName().c_str());

			App->CL_Scene->S_Bones[Loop]->TranslationStart.X = bone->_getDerivedPosition().x;
			App->CL_Scene->S_Bones[Loop]->TranslationStart.Y = bone->_getDerivedPosition().y;
			App->CL_Scene->S_Bones[Loop]->TranslationStart.Z = bone->_getDerivedPosition().z;

			Loop++;
		}
	}
	return 1;
}

