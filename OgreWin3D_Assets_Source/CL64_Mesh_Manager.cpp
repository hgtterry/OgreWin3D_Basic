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
#include "CL64_Mesh_Manager.h"

CL64_Mesh_Manager::CL64_Mesh_Manager(void)
{
	flag_Has_Shared_Vertices = 0;
}

CL64_Mesh_Manager::~CL64_Mesh_Manager(void)
{
}

// *************************************************************************
// *			Ogre_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Mesh_Manager::Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity)
{
	Convert_To_Mesh_Data(Ogre_Entity);

	
	App->CL_Scene->Set_BondingBox_Model(true);
	App->CL_Converters->Get_SkeletonInstance(Ogre_Entity);
	Get_Ogre_Mesh_Data(Ogre_Entity);
	
	App->CL_ImGui->flag_Show_Model_Data = 1;
	App->CL_ImGui->flag_Show_Ogre_Data = 1;

	return 1;
}

// *************************************************************************
// *		Convert_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Mesh_Manager::Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity)
{
	flag_Has_Shared_Vertices = false;
	int SubMeshCount = Ogre_Entity->getNumSubEntities();

	// Check for shared vertices across submeshes
	for (int Count = 0; Count < SubMeshCount; ++Count)
	{
		if (Ogre_Entity->getSubEntity(Count)->getSubMesh()->useSharedVertices)
		{
			flag_Has_Shared_Vertices = true;
			break; // Exit early if shared vertices are found
		}
	}

	App->CL_Converters->Create_MeshGroups(Ogre_Entity);

	if (!flag_Has_Shared_Vertices)
	{
		size_t vertex_count = 0;
		size_t index_count = 0;

		Vector3* vertices = nullptr;
		Vector3* normals = nullptr;
		unsigned long* indices = nullptr;
		Ogre::int16* BoneIndices = nullptr; // Bone Index

		for (int Count = 0; Count < SubMeshCount; ++Count)
		{
			Get_SubPose_MeshInstance(Ogre_Entity->getMesh(), vertex_count, vertices, index_count, indices, Count, BoneIndices);
			int mUVTest = Get_SubPoseTextureUV(Ogre_Entity->getMesh(), Count);
			Get_SubPoseNormals(Ogre_Entity->getMesh(), vertex_count, normals, Count);

			// Resize data structures for the current submesh
			auto& group = App->CL_Scene->Group[Count];
			group->vertex_Data.resize(vertex_count);
			group->Normal_Data.resize(vertex_count);
			group->MapCord_Data.resize(vertex_count);
			group->Face_Data.resize(index_count);
			group->FaceIndex_Data.resize(index_count);
			group->BoneIndex_Data.resize(vertex_count);

			// Process Vertices
			for (size_t Vertloop = 0; Vertloop < vertex_count; ++Vertloop)
			{
				group->vertex_Data[Vertloop].x = vertices[Vertloop].x;
				group->vertex_Data[Vertloop].y = vertices[Vertloop].y;
				group->vertex_Data[Vertloop].z = vertices[Vertloop].z;

				group->BoneIndex_Data[Vertloop].Index = BoneIndices[Vertloop];

				if (mUVTest)
				{
					group->MapCord_Data[Vertloop].u = MeshTextureCoords[Vertloop].x;
					group->MapCord_Data[Vertloop].v = 1 - MeshTextureCoords[Vertloop].y;
				}

				group->Normal_Data[Vertloop].x = normals[Vertloop].x;
				group->Normal_Data[Vertloop].y = normals[Vertloop].y;
				group->Normal_Data[Vertloop].z = normals[Vertloop].z;
			}

			// Process Faces
			for (size_t Faceloop = 0, FaceIndexNum = 0; Faceloop < index_count; Faceloop += 3, ++FaceIndexNum)
			{
				group->Face_Data[FaceIndexNum].a = indices[Faceloop];
				group->Face_Data[FaceIndexNum].b = indices[Faceloop + 1];
				group->Face_Data[FaceIndexNum].c = indices[Faceloop + 2];
				group->FaceIndex_Data[FaceIndexNum].Index = FaceIndexNum;
			}

			group->GroupFaceCount = index_count / 3; // Assuming each face consists of 3 indices
			group->GroupVertCount = vertex_count;
			group->IndicesCount = vertex_count;

			App->CL_Scene->VerticeCount += vertex_count;
			App->CL_Scene->FaceCount += group->GroupFaceCount;

			//GetBoneAssignment(Ogre_Entity->getMesh(), Count, 0);
		}
	}

	return true;
}


// *************************************************************************
// *	    	Get_Ogre_Mesh_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Mesh_Manager::Get_Ogre_Mesh_Data(Ogre::Entity* Ogre_Entity)
{
	int Count = 0;

	bool Edge = Ogre_Entity->hasEdgeList();
	if (Edge == 1)
	{
		App->CL_Scene->S_OgreMeshData[0]->mStrEdgeList = "Yes";
	}
	else
	{
		App->CL_Scene->S_OgreMeshData[0]->mStrEdgeList = "No";
	}

	bool Skel = Ogre_Entity->hasSkeleton();
	if (Skel == 1)
	{
		App->CL_Scene->S_OgreMeshData[0]->mStrSkeleton = "Yes";
	}
	else
	{
		App->CL_Scene->S_OgreMeshData[0]->mStrSkeleton = "No";
	}
	
	// ---------------------------------------------------------------

	App->CL_Scene->S_OgreMeshData[0]->mSubmeshes.resize(0);

	int SubMeshCount = Ogre_Entity->getNumSubEntities();
	App->CL_Scene->S_OgreMeshData[0]->mSubMeshCount = SubMeshCount;

	// ------------------------------- Materials 
	Count = 0;
	int NumSubEnts = Ogre_Entity->getNumSubEntities();
	
	while (Count < NumSubEnts)
	{

		// Material
		char mMaterial[MAX_PATH];
		Ogre::SubEntity* subEnt = Ogre_Entity->getSubEntity(Count);
		strcpy(mMaterial, subEnt->getMaterialName().c_str());

		if (App->CL_Scene->Group[Count])
		{
			strcpy(App->CL_Scene->Group[Count]->Ogre_Material, mMaterial);
			char nn[25];
			char MatId[MAX_PATH];
			strcpy(MatId, mMaterial);
			strcat(MatId, "##");
			strcat(MatId, _itoa(Count, nn, 10));
			strcpy(App->CL_Scene->Group[Count]->Ogre_ImGui_MatId, MatId);

			// Texture
			char mTexture[MAX_PATH];
			Ogre::MaterialPtr MatCurent;
			MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(mMaterial));

			if (MatCurent->getNumTechniques() > 0)
			{
				int TUSCount = MatCurent->getTechnique(0)->getPass(0)->getNumTextureUnitStates();

				if (TUSCount > 0)
				{
					App->CL_Scene->Group[Count]->Ogre_NumTextureUnits = TUSCount;
					strcpy(mTexture, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());

					strcpy(App->CL_Scene->Group[Count]->Ogre_Texture_FileName, mTexture);
					App->CL_Scene->Group[Count]->Ogre_Texture_IsValid = 1;
					App->CL_Scene->Group[Count]->Ogre_MipMaps = MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getNumMipmaps();

				}
				
				strcpy(App->CL_Scene->Group[Count]->Ogre_Material_File, MatCurent->getOrigin().c_str());

			}
			else
			{
				App->CL_Scene->Group[Count]->Ogre_NumTextureUnits = 0;
				App->CL_Scene->Group[Count]->Ogre_Texture_IsValid = 0;
			}

		}

		Count++;
	}
	
	Count = 0;
	App->CL_Scene->S_OgreMeshData[0]->mStrName = Ogre_Entity->getName();

	// ------------------------------------ Sub Meshes

	Count = 0;

	App->CL_Scene->S_OgreMeshData[0]->mSubmeshes.resize(SubMeshCount);

	while (Count < SubMeshCount)
	{
		char Num[MAX_PATH];
		char strSubMesh[MAX_PATH];
		strcpy(strSubMesh, "SubMesh_");
		_itoa(Count, Num, 10);
		strcat(strSubMesh, Num);

		App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_SubMesh_Name_str = strSubMesh;

		Ogre::SubMesh const* subMesh = Ogre_Entity->getSubEntity(Count)->getSubMesh();
		if (subMesh->useSharedVertices)
		{
			App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_HasSharedVertices_str = "No";
		}
		else
		{
			App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_HasSharedVertices_str = "Yes";
		}

		App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_Matrial_Name_str = subMesh->getMaterialName();

		//App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].VerticesCount = subMesh->vertexData->vertexCount;

		App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].BonesCount = subMesh->blendIndexToBoneIndexMap.size();

		Count++;
	}

	// ------------------------------------ Bounds
	Ogre::Vector3 vMin(Ogre_Entity->getBoundingBox().getMinimum());
	Ogre::Vector3 vMax(Ogre_Entity->getBoundingBox().getMaximum());
	Ogre::Vector3 Center((vMin + vMax) * 0.5f);

	App->CL_Scene->S_OgreMeshData[0]->vMin = vMin;
	App->CL_Scene->S_OgreMeshData[0]->vMax = vMax;
	App->CL_Scene->S_OgreMeshData[0]->Center = Center;

	App->CL_Scene->S_OgreMeshData[0]->Width = (vMax - vMin).x;
	App->CL_Scene->S_OgreMeshData[0]->Height = (vMax - vMin).y;
	App->CL_Scene->S_OgreMeshData[0]->Depth = (vMax - vMin).z;
	App->CL_Scene->S_OgreMeshData[0]->Area = (vMax - vMin).x * (vMax - vMin).y;
	App->CL_Scene->S_OgreMeshData[0]->Volume = (vMax - vMin).x * (vMax - vMin).y * (vMax - vMin).z;
	App->CL_Scene->S_OgreMeshData[0]->Radius = Ogre_Entity->getBoundingRadius();
}

// *************************************************************************
// *	   Get_SubPose_MeshInstance:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Mesh_Manager::Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
	size_t& vertex_count, Ogre::Vector3*& vertices,
	size_t& index_count, unsigned long*& indices,
	int SubMesh, Ogre::int16*& BoneIndices)
{
	//#pragma warning(disable : 4996) // Nightmare why

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
			#pragma warning(disable : 4996) // Nightmare why

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
// *		Get_SubPoseTextureUV:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Mesh_Manager::Get_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh)
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
			//App->Say("no TextCords");
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
// *		Get_SubPoseNormals:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Mesh_Manager::Get_SubPoseNormals(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& Normals,
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
// *			GetBoneAssignment:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Mesh_Manager::GetBoneAssignment(Ogre::MeshPtr mesh, int SubMesh, HWND hDlg)
{

	int Count = 0;
	Ogre::SubMesh* mSubmesh = mesh->getSubMesh(SubMesh);

	//// Bone assignments
	if (mesh->hasSkeleton())
	{
		//#pragma warning(disable : 4996) // Nightmare why
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
		//#pragma warning(disable : 4996) // Nightmare why
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
