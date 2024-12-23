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
#include "CL64_Mesh_Manager.h"

CL64_Mesh_Manager::CL64_Mesh_Manager(void)
{
	Has_Shared_Vertices = 0;
	Current_Index = 0;
}

CL64_Mesh_Manager::~CL64_Mesh_Manager(void)
{
}

// *************************************************************************
// *			Create_MeshGroups:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Mesh_Manager::Create_MeshGroups(Ogre::Entity* Ogre_Entity)
{
	// -------------------------------------------- Create Group
	int Index = App->CL_Scene->Map_Group_Count;
	Current_Index = App->CL_Scene->Map_Group_Count;

	if (App->CL_Scene->Map_Group[Index] != nullptr)
	{
		delete App->CL_Scene->Map_Group[Index];
		App->CL_Scene->Map_Group[Index] = nullptr;
	}

	App->CL_Scene->Map_Group[Index] = new Base_Group();
	App->CL_Scene->Map_Group[Index]->GroupVertCount = 0;
	App->CL_Scene->Map_Group[Index]->IndicesCount = 0;
	App->CL_Scene->Map_Group[Index]->GroupFaceCount = 0;

	char Sub_Mesh_Name[MAX_PATH];
	char Sub_Mesh_Num[MAX_PATH];

	int SubMeshCount = Ogre_Entity->getNumSubEntities();

	int Count = 0;
	while (Count < SubMeshCount)
	{
		App->CL_Scene->Map_Group[Index]->B_Sub_Mesh[Count] = new Sub_Mesh_type;
		App->CL_Scene->Map_Group[Index]->B_Sub_Mesh[Count]->Face_Count = 0;
		App->CL_Scene->Map_Group[Index]->B_Sub_Mesh[Count]->IndicesCount = 0;
		App->CL_Scene->Map_Group[Index]->B_Sub_Mesh[Count]->Vertice_Count = 0;

		_itoa(Count, Sub_Mesh_Num, 10);
		strcpy(Sub_Mesh_Name, "Sub_Mesh_");
		strcat(Sub_Mesh_Name, Sub_Mesh_Num);

		strcpy(App->CL_Scene->Map_Group[Index]->B_Sub_Mesh[Count]->chr_SubMesh_Name, Sub_Mesh_Name);

		/*Ogre::SubMesh const* subMesh = Ogre_Entity->getSubEntity(Count)->getSubMesh();
		strcpy(App->CL_Scene->Group[Count]->MaterialName, subMesh->getMaterialName().c_str());

		if (App->CL_Mesh_Manager->Has_Shared_Vertices == 0)
		{
			App->CL_Scene->Group[Count]->GroupVertCount = subMesh->vertexData->vertexCount;
			App->CL_Scene->Group[Count]->IndicesCount = subMesh->vertexData->vertexCount;
		}*/

		Count++;
	}

	App->CL_Scene->Map_Group[Index]->Sub_Group_Count = Count;

	App->CL_Scene->Map_Group_Count++;
}

// *************************************************************************
// *			Ogre_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Mesh_Manager::Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity)
{


	Convert_To_Mesh_Data(Ogre_Entity);


	//App->CL_Scene->Set_BondingBox_Model(true);
	//App->CL_Converters->Get_SkeletonInstance(Ogre_Entity);
	//Get_Ogre_Mesh_Data(Ogre_Entity);

	//App->CL_ImGui->flag_Show_Model_Data = 1;
	//App->CL_ImGui->flag_Show_Ogre_Data = 1;

	return 1;
}

// *************************************************************************
// *		Convert_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Mesh_Manager::Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity)
{
	Has_Shared_Vertices = 0;

	int Count = 0;
	int SubMeshCount = Ogre_Entity->getNumSubEntities();

	while (Count < SubMeshCount)
	{
		bool Sahred = Ogre_Entity->getSubEntity(Count)->getSubMesh()->useSharedVertices;

		if (Sahred == 1)
		{
			Has_Shared_Vertices = 1;
		}

		Count++;
	}

	Create_MeshGroups(Ogre_Entity);

	if (Has_Shared_Vertices == 0)
	{
		Has_Shared_Vertices = 0;

		int FaceCount = 0;
		int FaceNum = 0;
		int FaceIndexNum = 0;
		int mFaceIndex = 0;
		int xx = 0;
		size_t vertex_count = 0;
		size_t  index_count = 0;

		Vector3* vertices = { 0 };

		Vector3* normals = { 0 };
		unsigned long* indices = 0;

		Ogre::int16* BoneIndices = 0;	// Bone Index

		int SubMeshCount = Ogre_Entity->getNumSubEntities();

		unsigned int Vertloop = 0;
		unsigned int Faceloop = 0;
		int Count = 0;


		while (Count < SubMeshCount)
		{
			Get_SubPose_MeshInstance(Ogre_Entity->getMesh(), vertex_count, vertices, index_count, indices, Count, BoneIndices);

			//int mUVTest = Get_SubPoseTextureUV(Ogre_Entity->getMesh(), Count);
			//Get_SubPoseNormals(Ogre_Entity->getMesh(), vertex_count, normals, Count);


			App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->vertex_Data.resize(index_count);
			App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->Face_Data.resize(index_count);
			App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->FaceIndex_Data.resize(index_count);

			FaceIndexNum = 0;
			int Faceit = 0;
			FaceCount = 0;
			Vertloop = 0;
			xx = 0;

			while (Vertloop < vertex_count) // Process Vertices
			{
				App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->vertex_Data[Vertloop].x = vertices[Vertloop].x;
				App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->vertex_Data[Vertloop].y = vertices[Vertloop].y;
				App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->vertex_Data[Vertloop].z = vertices[Vertloop].z;

				Vertloop++;
			}

			FaceIndexNum = 0;
			Faceloop = 0;
			while (Faceloop < index_count) // Process Faces
			{
				App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->Face_Data[FaceIndexNum].a = indices[Faceloop];
				Faceloop++;
				App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->Face_Data[FaceIndexNum].b = indices[Faceloop];
				Faceloop++;
				App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->Face_Data[FaceIndexNum].c = indices[Faceloop];
				Faceloop++;

				FaceIndexNum++;

				App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->FaceIndex_Data[xx].Index = mFaceIndex;
				
				xx++;
				mFaceIndex++;
			}

			App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->Face_Count = FaceIndexNum;
			App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->Vertice_Count = Vertloop;
			App->CL_Scene->Map_Group[Current_Index]->B_Sub_Mesh[Count]->IndicesCount = Vertloop;

			//App->CL_Scene->VerticeCount = App->CL_Scene->VerticeCount + Vertloop;
			//App->CL_Scene->FaceCount = App->CL_Scene->FaceCount + FaceIndexNum;

			Count++;
		}

	}

	return 1;
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
		//App->CL_Scene->S_OgreMeshData[0]->mStrEdgeList = "Yes";
	}
	else
	{
		//App->CL_Scene->S_OgreMeshData[0]->mStrEdgeList = "No";
	}

	bool Skel = Ogre_Entity->hasSkeleton();
	if (Skel == 1)
	{
		//App->CL_Scene->S_OgreMeshData[0]->mStrSkeleton = "Yes";
	}
	else
	{
		//App->CL_Scene->S_OgreMeshData[0]->mStrSkeleton = "No";
	}

	// ---------------------------------------------------------------

	//App->CL_Scene->S_OgreMeshData[0]->mSubmeshes.resize(0);

	int SubMeshCount = Ogre_Entity->getNumSubEntities();
	//App->CL_Scene->S_OgreMeshData[0]->mSubMeshCount = SubMeshCount;

	// ------------------------------- Materials 
	Count = 0;
	int NumSubEnts = Ogre_Entity->getNumSubEntities();

	while (Count < NumSubEnts)
	{

		// Material
		char mMaterial[MAX_PATH];
		Ogre::SubEntity* subEnt = Ogre_Entity->getSubEntity(Count);
		strcpy(mMaterial, subEnt->getMaterialName().c_str());

		//if (App->CL_Scene->Group[Count])
		//{
		//	strcpy(App->CL_Scene->Group[Count]->Ogre_Material, mMaterial);
		//	char nn[25];
		//	char MatId[MAX_PATH];
		//	strcpy(MatId, mMaterial);
		//	strcat(MatId, "##");
		//	strcat(MatId, _itoa(Count, nn, 10));
		//	strcpy(App->CL_Scene->Group[Count]->Ogre_ImGui_MatId, MatId);

		//	// Texture
		//	char mTexture[MAX_PATH];
		//	Ogre::MaterialPtr MatCurent;
		//	MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(mMaterial));

		//	if (MatCurent->getNumTechniques() > 0)
		//	{
		//		int TUSCount = MatCurent->getTechnique(0)->getPass(0)->getNumTextureUnitStates();

		//		if (TUSCount > 0)
		//		{
		//			App->CL_Scene->Group[Count]->Ogre_NumTextureUnits = TUSCount;
		//			strcpy(mTexture, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());

		//			strcpy(App->CL_Scene->Group[Count]->Ogre_Texture_FileName, mTexture);
		//			App->CL_Scene->Group[Count]->Ogre_Texture_IsValid = 1;
		//			App->CL_Scene->Group[Count]->Ogre_MipMaps = MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getNumMipmaps();

		//		}

		//		strcpy(App->CL_Scene->Group[Count]->Ogre_Material_File, MatCurent->getOrigin().c_str());

		//	}
		//	else
		//	{
		//		App->CL_Scene->Group[Count]->Ogre_NumTextureUnits = 0;
		//		App->CL_Scene->Group[Count]->Ogre_Texture_IsValid = 0;
		//	}

		//}

		Count++;
	}

	Count = 0;
	//App->CL_Scene->S_OgreMeshData[0]->mStrName = Ogre_Entity->getName();

	// ------------------------------------ Sub Meshes

	Count = 0;

	//App->CL_Scene->S_OgreMeshData[0]->mSubmeshes.resize(SubMeshCount);

	//while (Count < SubMeshCount)
	//{
	//	char Num[MAX_PATH];
	//	char strSubMesh[MAX_PATH];
	//	strcpy(strSubMesh, "SubMesh_");
	//	_itoa(Count, Num, 10);
	//	strcat(strSubMesh, Num);

	//	App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_SubMesh_Name_str = strSubMesh;

	//	Ogre::SubMesh const* subMesh = Ogre_Entity->getSubEntity(Count)->getSubMesh();
	//	if (subMesh->useSharedVertices)
	//	{
	//		App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_HasSharedVertices_str = "No";
	//	}
	//	else
	//	{
	//		App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_HasSharedVertices_str = "Yes";
	//	}

	//	App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].m_Matrial_Name_str = subMesh->getMaterialName();

	//	//App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].VerticesCount = subMesh->vertexData->vertexCount;

	//	App->CL_Scene->S_OgreMeshData[0]->mSubmeshes[Count].BonesCount = subMesh->blendIndexToBoneIndexMap.size();

	//	Count++;
	//}

	// ------------------------------------ Bounds
	Ogre::Vector3 vMin(Ogre_Entity->getBoundingBox().getMinimum());
	Ogre::Vector3 vMax(Ogre_Entity->getBoundingBox().getMaximum());
	Ogre::Vector3 Center((vMin + vMax) * 0.5f);

	/*App->CL_Scene->S_OgreMeshData[0]->vMin = vMin;
	App->CL_Scene->S_OgreMeshData[0]->vMax = vMax;
	App->CL_Scene->S_OgreMeshData[0]->Center = Center;

	App->CL_Scene->S_OgreMeshData[0]->Width = (vMax - vMin).x;
	App->CL_Scene->S_OgreMeshData[0]->Height = (vMax - vMin).y;
	App->CL_Scene->S_OgreMeshData[0]->Depth = (vMax - vMin).z;
	App->CL_Scene->S_OgreMeshData[0]->Area = (vMax - vMin).x * (vMax - vMin).y;
	App->CL_Scene->S_OgreMeshData[0]->Volume = (vMax - vMin).x * (vMax - vMin).y * (vMax - vMin).z;
	App->CL_Scene->S_OgreMeshData[0]->Radius = Ogre_Entity->getBoundingRadius();*/
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

	//App->CL_Scene->Group[SubMesh]->BA_BoneIndex_Data.resize(Count);
	//App->CL_Scene->Group[SubMesh]->BA_BoneVertexIndex_Data.resize(Count);
	//App->CL_Scene->Group[SubMesh]->BA_Weight_Data.resize(Count);

	//App->CL_Scene->Group[SubMesh]->BoneAssignMentCount = Count;

	Count = 0;
	if (mesh->hasSkeleton())
	{
		//#pragma warning(disable : 4996) // Nightmare why
		Ogre::SubMesh::BoneAssignmentIterator BI = mSubmesh->getBoneAssignmentIterator();

		while (BI.hasMoreElements())
		{
			Ogre::VertexBoneAssignment bb = BI.getNext();

			//App->CL_Scene->Group[SubMesh]->BA_BoneIndex_Data[Count].Index = bb.boneIndex; // Bone Index 
			//App->CL_Scene->Group[SubMesh]->BA_BoneVertexIndex_Data[Count].Index = bb.vertexIndex;
			//App->CL_Scene->Group[SubMesh]->BA_Weight_Data[Count].Float1 = bb.weight;

			Count++;
		}
	}

	return 1;
}
