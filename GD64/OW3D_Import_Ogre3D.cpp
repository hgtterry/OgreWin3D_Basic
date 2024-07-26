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
#include "OW3D_Import_Ogre3D.h"

#pragma warning(disable:4996)

OW3D_Import_Ogre3D::OW3D_Import_Ogre3D(void)
{
	flag_IsAnimated = 0;
}

OW3D_Import_Ogre3D::~OW3D_Import_Ogre3D(void)
{
}

// *************************************************************************
// *			Ogre_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool OW3D_Import_Ogre3D::Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity)
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
	App->CL_Converters->Get_Ogre3D_MeshData(Ogre_Entity);

	return 1;
}

// *************************************************************************
// *	   Get_SubPose_MeshInstance:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void OW3D_Import_Ogre3D::Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
	size_t& vertex_count, Ogre::Vector3*& vertices,
	size_t& index_count, unsigned long*& indices,
	int SubMesh, Ogre::int16*& BoneIndices)
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
// *		NewGet_SubPoseTextureUV:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool OW3D_Import_Ogre3D::NewGet_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh)
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
bool OW3D_Import_Ogre3D::NewGet_SubPoseNormals(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& Normals,
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
bool OW3D_Import_Ogre3D::GetBoneAssignment(Ogre::MeshPtr mesh, int SubMesh, HWND hDlg)
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
// *	  			Get_Motions:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void OW3D_Import_Ogre3D::Get_Motions(Ogre::Entity* Ogre_Entity)
{
	Ogre::SkeletonInstance* skeletonInstance = Ogre_Entity->getSkeleton();

	int Count = 0;
	if (skeletonInstance)
	{
		int AnimationCount = skeletonInstance->getNumAnimations();
		if (AnimationCount == 0)
		{
			flag_IsAnimated = 0;
			App->CL_Scene->MotionCount = 0;
		}
		else
		{

			flag_IsAnimated = 1;

			for (unsigned short i = 0; i < skeletonInstance->getNumAnimations(); ++i)
			{
				Ogre::Animation* animation = skeletonInstance->getAnimation(i);
				App->CL_Scene->S_OgreMeshData[0]->mMotionNames.push_back(animation->getName());

				Count = i;
			}

			App->CL_Scene->MotionCount = Count + 1;

		}
	}
	else
	{
		flag_IsAnimated = 0;
		App->CL_Scene->MotionCount = 0;
	}
}
