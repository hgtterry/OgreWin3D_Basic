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
#include "CL64_Converters.h"

CL64_Converters::CL64_Converters(void)
{
	Export_Manual = nullptr;
	World_Manual =	nullptr;

	World_Node =	nullptr;
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
	App->CL_Ogre->OgreListener->Ogre_Model_Loaded = 0;

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
	
	//Create_Resource_Group();
	//Debug
	/*if (World_Ent)
	{
		App->CL_Ogre->OgreListener->Ogre_Model_Loaded = 0;

		World_Node->detachAllObjects();
		App->CL_Ogre->mSceneMgr->destroySceneNode(World_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(World_Ent);
		World_Ent = nullptr;
		World_Node = nullptr;
	}*/
	//Debug
	Create_Resource_Group();
	
	World_Ent = App->CL_Ogre->mSceneMgr->createEntity(Name);
	
	remove(mWorld_File_PathAndFile);
	remove(Material_PathAndFile);

	return World_Ent;

	//App->Say("Converted");
}

// *************************************************************************
// *		Create_Resource_Group():- Terry and Hazel Flanigan 2024    	   *
// *************************************************************************
void CL64_Converters::Create_Resource_Group()
{
	if (World_Ent)
	{
		/*World_Node->detachAllObjects();

		App->CL_Ogre->mSceneMgr->destroySceneNode(World_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(World_Ent);

		World_Node = NULL;
		World_Ent = NULL;*/

		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->World_Resource_Group);

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
		//Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(App->CL_Ogre->World_Resource_Group);
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
	char GroupName[MAX_PATH];
	char GroupNum[MAX_PATH];

	int SubMeshCount = Ogre_Entity->getNumSubEntities();

	int Count = 0;
	while (Count < SubMeshCount)
	{

		App->CL_Scene->Create_Mesh_Group(Count); // Create new Mesh Group
		App->CL_Scene->Group[Count]->GroupVertCount = 0;
		App->CL_Scene->Group[Count]->MaterialIndex = -1;

		//App->CL_Model_Data->S_MeshGroup[Count]->EqIndex = Count+1;
		//App->CL_FBX->EqIndexCount++;

		//App->S_MeshGroup[Count]->MaterialIndex = mesh->mMaterialIndex;

		strcpy(App->CL_Scene->Group[Count]->Text_FileName, "No_Texture");

		_itoa(Count, GroupNum, 10);
		strcpy(GroupName, "Group_");
		strcat(GroupName, GroupNum);

		strcpy(App->CL_Scene->Group[Count]->GroupName, GroupName);

		//---------------
		Ogre::SubMesh const* subMesh = Ogre_Entity->getSubEntity(Count)->getSubMesh();
		strcpy(App->CL_Scene->Group[Count]->MaterialName, subMesh->getMaterialName().c_str());

		App->CL_Scene->Group[Count]->GroupVertCount = subMesh->vertexData->vertexCount;
		App->CL_Scene->Group[Count]->IndicesCount = subMesh->vertexData->vertexCount;
		//---------------
		
		Count++;
	}

	App->CL_Scene->TextureCount = SubMeshCount;
	App->CL_Scene->GroupCount = SubMeshCount;
}

// *************************************************************************
// *			Ogre_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Converters::Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity)
{
	App->CL_Converters->Create_MeshGroups(Ogre_Entity);
	
	int FaceCount = 0;
	int FaceNum = 0;
	int FaceIndexNum = 0;
	int mFaceIndex = 0;
	int xx = 0;
	size_t vertex_count, index_count;
	Vector3* vertices;
	Vector3* normals;
	unsigned long* indices;

	Ogre::int16* BoneIndices;	// Bone Index

	int SubMeshCount = Ogre_Entity->getNumSubEntities();

	unsigned int Vertloop = 0;
	unsigned int Faceloop = 0;
	int Count = 0;

	while (Count < SubMeshCount)
	{
		Get_SubPose_MeshInstance(Ogre_Entity->getMesh(), vertex_count, vertices, index_count, indices, Count, BoneIndices);
		int mUVTest = NewGet_SubPoseTextureUV(Ogre_Entity->getMesh(), Count);
		NewGet_SubPoseNormals(Ogre_Entity->getMesh(), vertex_count, normals, Count);

		App->CL_Scene->Group[Count]->vertex_Data.resize(index_count);
		App->CL_Scene->Group[Count]->Normal_Data.resize(index_count);
		App->CL_Scene->Group[Count]->MapCord_Data.resize(index_count);
		App->CL_Scene->Group[Count]->Face_Data.resize(index_count);
		App->CL_Scene->Group[Count]->FaceIndex_Data.resize(index_count);

		App->CL_Scene->Group[Count]->BoneIndex_Data.resize(index_count);

		FaceIndexNum = 0;
		int Faceit = 0;
		FaceCount = 0;
		Vertloop = 0;
		xx = 0;
		while (Vertloop < vertex_count) // Process Vertices
		{
			App->CL_Scene->Group[Count]->vertex_Data[Vertloop].x = vertices[Vertloop].x;
			App->CL_Scene->Group[Count]->vertex_Data[Vertloop].y = vertices[Vertloop].y;
			App->CL_Scene->Group[Count]->vertex_Data[Vertloop].z = vertices[Vertloop].z;

			App->CL_Scene->Group[Count]->BoneIndex_Data[Vertloop].Index = BoneIndices[Vertloop]; // Bone Index 

			if (mUVTest)
			{
				App->CL_Scene->Group[Count]->MapCord_Data[Vertloop].u = MeshTextureCoords[Vertloop].x;
				App->CL_Scene->Group[Count]->MapCord_Data[Vertloop].v = 1 - MeshTextureCoords[Vertloop].y;
			}

			App->CL_Scene->Group[Count]->Normal_Data[Vertloop].x = normals[Vertloop].x;
			App->CL_Scene->Group[Count]->Normal_Data[Vertloop].y = normals[Vertloop].y;
			App->CL_Scene->Group[Count]->Normal_Data[Vertloop].z = normals[Vertloop].z;

			Vertloop++;
		}

		FaceIndexNum = 0;
		Faceloop = 0;
		while (Faceloop < index_count) // Process Faces
		{
			App->CL_Scene->Group[Count]->Face_Data[FaceIndexNum].a = indices[Faceloop];
			Faceloop++;
			App->CL_Scene->Group[Count]->Face_Data[FaceIndexNum].b = indices[Faceloop];
			Faceloop++;
			App->CL_Scene->Group[Count]->Face_Data[FaceIndexNum].c = indices[Faceloop];
			Faceloop++;

			FaceIndexNum++;

			App->CL_Scene->Group[Count]->FaceIndex_Data[xx].Index = mFaceIndex;

			xx++;
			mFaceIndex++;
		}

		App->CL_Scene->Group[Count]->GroupFaceCount = FaceIndexNum;
		App->CL_Scene->Group[Count]->GroupVertCount = Vertloop;
		App->CL_Scene->Group[Count]->IndicesCount = Vertloop;

		App->CL_Scene->VerticeCount = App->CL_Scene->VerticeCount + Vertloop;
		App->CL_Scene->FaceCount = App->CL_Scene->FaceCount + FaceIndexNum;


		GetBoneAssignment(Ogre_Entity->getMesh(), Count, 0);

		Count++;
	}
	
	App->CL_Scene->Set_BondingBox_Model(true);
	App->CL_Converters->Get_SkeletonInstance(Ogre_Entity);

	return 1;
}

// *************************************************************************
// *	   Get_SubPose_MeshInstance:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Converters::Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
	size_t& vertex_count,Ogre::Vector3*& vertices,
	size_t& index_count,unsigned long*& indices,
	int SubMesh,Ogre::int16*& BoneIndices)
{
#pragma warning(disable : 4996) // Nightmare why

	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;


	const Vector3& position = Vector3::ZERO;
	const Quaternion& orient = Quaternion::IDENTITY;
	const Vector3& scale = Vector3::UNIT_SCALE;

	vertex_count = index_count = 0;
	Ogre::SubMesh* submesh = mesh->getSubMesh(SubMesh);

	vertex_count = submesh->vertexData->vertexCount;
	Ogre::SubMesh::VertexBoneAssignmentList plist = submesh->getBoneAssignments();

	index_count = submesh->indexData->indexCount;

	// Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	BoneIndices = new Ogre::int16[vertex_count]; // Create Space for Bone Indexs 
	indices = new unsigned long[index_count];

	//-------------------- Get Data
	Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

	if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
	{
		if (submesh->useSharedVertices)
		{
			added_shared = true;
			shared_offset = current_offset;
		}

		const Ogre::VertexElement* posElem =
			vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

		Ogre::HardwareVertexBufferSharedPtr vbuf =
			vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		unsigned char* vertex =
			static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

		float* pReal;

		if (mesh->hasSkeleton() == 1)
		{
			Ogre::SubMesh::BoneAssignmentIterator itor = submesh->getBoneAssignmentIterator();

			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				Ogre::VertexBoneAssignment bb = itor.getNext();
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
				BoneIndices[j] = bb.boneIndex;
			}
		}
		else
		{
			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}
		}

		vbuf->unlock();
		next_offset += vertex_data->vertexCount;
	}

	Ogre::IndexData* index_data = submesh->indexData;
	size_t numTris = index_data->indexCount / 3;
	Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

	bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

	unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

	size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;

	if (use32bitindexes)
	{
		for (size_t k = 0; k < numTris * 3; ++k)
		{
			indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
		}
	}
	else
	{
		for (size_t k = 0; k < numTris * 3; ++k)
		{
			indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
				static_cast<unsigned long>(offset);
		}
	}

	ibuf->unlock();
	current_offset = next_offset;
}

// *************************************************************************
// *			GetBoneAssignment:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Converters::GetBoneAssignment(Ogre::MeshPtr mesh, int SubMesh, HWND hDlg)
{
	int Count = 0;
	Ogre::SubMesh* mSubmesh = mesh->getSubMesh(SubMesh);

	//// Bone assignments
	if (mesh->hasSkeleton())
	{
		Ogre::SubMesh::BoneAssignmentIterator BI = mSubmesh->getBoneAssignmentIterator();

		while (BI.hasMoreElements())
		{
			BI.getNext();
			Count++;
		}
	}

	App->CL_Scene->Group[SubMesh]->BA_BoneIndex_Data.resize(Count);
	App->CL_Scene->Group[SubMesh]->BA_BoneVertexIndex_Data.resize(Count);
	App->CL_Scene->Group[SubMesh]->BA_Weight_Data.resize(Count);

	App->CL_Scene->Group[SubMesh]->BoneAssignMentCount = Count;

	Count = 0;
	if (mesh->hasSkeleton())
	{
		Ogre::SubMesh::BoneAssignmentIterator BI = mSubmesh->getBoneAssignmentIterator();

		while (BI.hasMoreElements())
		{
			Ogre::VertexBoneAssignment bb = BI.getNext();

			App->CL_Scene->Group[SubMesh]->BA_BoneIndex_Data[Count].Index = bb.boneIndex; // Bone Index 
			App->CL_Scene->Group[SubMesh]->BA_BoneVertexIndex_Data[Count].Index = bb.vertexIndex;
			App->CL_Scene->Group[SubMesh]->BA_Weight_Data[Count].Float1 = bb.weight;

			Count++;
		}
	}

	return 1;
}

// *************************************************************************
// *		NewGet_SubPoseTextureUV:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Converters::NewGet_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh)
{
	Ogre::SubMesh* submesh = mesh->getSubMesh(SubMesh);
	int m_iCoordSet = 0;

	//// Get vertex UV coordinates
	//std::vector<Vector2> MeshTextureCoords;
	{
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
		// Get last set of texture coordinates
		int i = 0;
		const VertexElement* texcoordElem;
		const VertexElement* pCurrentElement = NULL;
		do
		{
			texcoordElem = pCurrentElement;
			pCurrentElement = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES, i++);
		} while (pCurrentElement);
		m_iCoordSet = i - 2;
		if (!texcoordElem)
		{
			App->Say("no TextCords");
			return false;
		}
		HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(texcoordElem->getSource());
		unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

		float* pReal;

		MeshTextureCoords.resize(vertex_data->vertexCount);

		for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
		{
			texcoordElem->baseVertexPointerToElement(vertex, &pReal);
			MeshTextureCoords[j] = Vector2(pReal[0], pReal[1]);
		}

		vbuf->unlock();
	}

	return true;
}

// *************************************************************************
// *		NewGet_SubPoseNormals:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Converters::NewGet_SubPoseNormals(Ogre::MeshPtr mesh,size_t& vertex_count,Ogre::Vector3*& Normals,
	int SubMesh)
{

	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	const Vector3& position = Vector3::ZERO;
	const Quaternion& orient = Quaternion::IDENTITY;
	const Vector3& scale = Vector3::UNIT_SCALE;

	vertex_count = 0;

	Ogre::SubMesh* submesh = mesh->getSubMesh(SubMesh);

	vertex_count = submesh->vertexData->vertexCount;

	Normals = new Ogre::Vector3[vertex_count];

	//-------------------- Get Data
	Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

	if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
	{
		if (submesh->useSharedVertices)
		{
			added_shared = true;
			shared_offset = current_offset;
		}

		const Ogre::VertexElement* posElem =
			vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);

		Ogre::HardwareVertexBufferSharedPtr vbuf =
			vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		unsigned char* vertex =
			static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

		float* pReal;

		for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
		{
			posElem->baseVertexPointerToElement(vertex, &pReal);
			Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
			Normals[current_offset + j] = (orient * (pt * scale)) + position;
		}

		vbuf->unlock();
	}
	return 1;
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

// *************************************************************************
// *	  	Get_Ogre3D_MeshData:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Converters::Get_Ogre3D_MeshData(Ogre::Entity* Ogre_Entity)
{
	bool Edge = Ogre_Entity->hasEdgeList();
	if (Edge == 1)
	{
		App->CL_Scene->S_OgreMeshData[0]->mEdgeList = "Yes";
	}
	else
	{
		App->CL_Scene->S_OgreMeshData[0]->mEdgeList = "No";
	}

	// ---------------------------------------------------------------

	App->CL_Scene->S_OgreMeshData[0]->mMaterials.resize(0);

	int SubMeshCount = Ogre_Entity->getNumSubEntities();
	App->CL_Scene->S_OgreMeshData[0]->mSubMeshCount = SubMeshCount;


	int Count = 0;
	while (Count < SubMeshCount)
	{
		Ogre::SubMesh const* subMesh = Ogre_Entity->getSubEntity(Count)->getSubMesh();

		App->CL_Scene->S_OgreMeshData[0]->mMaterials.push_back(subMesh->getMaterialName());
		Count++;
	}
}

