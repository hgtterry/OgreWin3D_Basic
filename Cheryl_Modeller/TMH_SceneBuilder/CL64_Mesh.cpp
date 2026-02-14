#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Mesh.h"

CL64_Mesh::CL64_Mesh(void)
{
	flag_Has_Shared_Vertices = false;

	int Count = 0;
	while (Count < 4999)
	{
		Group[Count] = nullptr;
		Count++;
	}

	Count = 0;
	while (Count < 199)
	{
		S_Bones[Count] = nullptr;
		Count++;
	}

	S_OgreMeshData[0] = new OgreMeshData_Type;

	S_OgreMeshData[0]->mStrName = "No Model Loaded";
	S_OgreMeshData[0]->mFileName_Str = "No Model Loaded";
}

CL64_Mesh::~CL64_Mesh(void)
{
}

// *************************************************************************
// *			Ogre_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Mesh::Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity)
{
	Ogre_Convert_To_Mesh_Data(Ogre_Entity);

	App->CL_Model->Set_BondingBox_Model(true);


	Get_SkeletonInstance(Ogre_Entity);
	Get_Ogre_Mesh_Data(Ogre_Entity);

	//App->CL_ImGui->flag_Show_Model_Data = 1;
	//App->CL_ImGui->flag_Show_Ogre_Data = 1;

	return 1;
}

// *************************************************************************
// *	Ogre_Convert_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Mesh::Ogre_Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity)
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

	Ogre_Create_MeshGroups(Ogre_Entity);

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
			Ogre_Get_SubPose_MeshInstance(Ogre_Entity->getMesh(), vertex_count, vertices, index_count, indices, Count, BoneIndices);
			int mUVTest = Ogre_Get_SubPoseTextureUV(Ogre_Entity->getMesh(), Count);
			Ogre_Get_SubPoseNormals(Ogre_Entity->getMesh(), vertex_count, normals, Count);

			// Resize data structures for the current submesh
			auto& group = App->CL_Mesh->Group[Count];
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

			App->CL_Model->VerticeCount += vertex_count;
			App->CL_Model->FaceCount += group->GroupFaceCount;

			//GetBoneAssignment(Ogre_Entity->getMesh(), Count, 0);
		}
	}

	return true;
}

// *************************************************************************
// *		Ogre_Create_MeshGroups:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Mesh::Ogre_Create_MeshGroups(Ogre::Entity* Ogre_Entity)
{
	const int SubMeshCount = Ogre_Entity->getNumSubEntities();

	for (int Count = 0; Count < SubMeshCount; ++Count)
	{
		App->CL_Scene->Create_Mesh_Group(Count); // Create new Mesh Group
		auto& meshGroup = App->CL_Mesh->Group[Count];

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
		if (flag_Has_Shared_Vertices == false)
		{
			meshGroup->GroupVertCount = subMesh->vertexData->vertexCount;
			meshGroup->IndicesCount = subMesh->vertexData->vertexCount;
		}
	}

	// Update texture and group counts
	App->CL_Model->TextureCount = SubMeshCount;
	App->CL_Model->GroupCount = SubMeshCount;
}

// *************************************************************************
// *	   Get_SubPose_MeshInstance:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Mesh::Ogre_Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
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
bool CL64_Mesh::Ogre_Get_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh)
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
bool CL64_Mesh::Ogre_Get_SubPoseNormals(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& Normals,int SubMesh)
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
// *	    	Get_Ogre_Mesh_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Mesh::Get_Ogre_Mesh_Data(Ogre::Entity* Ogre_Entity)
{
	auto& Ogre_Data = App->CL_Mesh->S_OgreMeshData[0]; // App->CL_Mesh->S_OgreMeshData[0]

	Ogre_Data->mFileName_Str = App->CL_Model->Loaded_FileName;

	int Count = 0;

	bool Edge = Ogre_Entity->hasEdgeList();
	if (Edge == 1)
	{
		Ogre_Data->mStrEdgeList = "Yes";
	}
	else
	{
		Ogre_Data->mStrEdgeList = "No";
	}

	bool Skel = Ogre_Entity->hasSkeleton();
	if (Skel == 1)
	{
		Ogre_Data->mStrSkeleton = "Yes";
	}
	else
	{
		Ogre_Data->mStrSkeleton = "No";
	}

	// ---------------------------------------------------------------

	Ogre_Data->mSubmeshes.resize(0);

	int SubMeshCount = Ogre_Entity->getNumSubEntities();
	Ogre_Data->mSubMeshCount = SubMeshCount;

	// ------------------------------- Materials 
	Count = 0;
	int NumSubEnts = Ogre_Entity->getNumSubEntities();

	while (Count < NumSubEnts)
	{

		// Material
		char mMaterial[MAX_PATH];
		Ogre::SubEntity* subEnt = Ogre_Entity->getSubEntity(Count);
		strcpy(mMaterial, subEnt->getMaterialName().c_str());

		if (App->CL_Mesh->Group[Count])
		{
			strcpy(App->CL_Mesh->Group[Count]->Ogre_Material, mMaterial);
			char nn[25];
			char MatId[MAX_PATH];
			strcpy(MatId, mMaterial);
			strcat(MatId, "##");
			strcat(MatId, _itoa(Count, nn, 10));
			strcpy(App->CL_Mesh->Group[Count]->Ogre_ImGui_MatId, MatId);

			// Texture
			char mTexture[MAX_PATH];
			Ogre::MaterialPtr MatCurent;
			MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(mMaterial));

			if (MatCurent->getNumTechniques() > 0)
			{
				int TUSCount = MatCurent->getTechnique(0)->getPass(0)->getNumTextureUnitStates();

				if (TUSCount > 0)
				{
					App->CL_Mesh->Group[Count]->Ogre_NumTextureUnits = TUSCount;
					strcpy(mTexture, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());

					strcpy(App->CL_Mesh->Group[Count]->Ogre_Texture_FileName, mTexture);
					App->CL_Mesh->Group[Count]->Ogre_Texture_IsValid = 1;
					App->CL_Mesh->Group[Count]->Ogre_MipMaps = MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getNumMipmaps();

				}

				strcpy(App->CL_Mesh->Group[Count]->Ogre_Material_File, MatCurent->getOrigin().c_str());

			}
			else
			{
				App->CL_Mesh->Group[Count]->Ogre_NumTextureUnits = 0;
				App->CL_Mesh->Group[Count]->Ogre_Texture_IsValid = 0;
			}

		}

		Count++;
	}

	Count = 0;
	Ogre_Data->mStrName = Ogre_Entity->getName();

	// ------------------------------------ Sub Meshes

	Count = 0;

	Ogre_Data->mSubmeshes.resize(SubMeshCount);

	while (Count < SubMeshCount)
	{
		char Num[MAX_PATH];
		char strSubMesh[MAX_PATH];
		strcpy(strSubMesh, "SubMesh_");
		_itoa(Count, Num, 10);
		strcat(strSubMesh, Num);

		Ogre_Data->mSubmeshes[Count].m_SubMesh_Name_str = strSubMesh;

		Ogre::SubMesh const* subMesh = Ogre_Entity->getSubEntity(Count)->getSubMesh();
		if (subMesh->useSharedVertices)
		{
			Ogre_Data->mSubmeshes[Count].m_HasSharedVertices_str = "No";
		}
		else
		{
			Ogre_Data->mSubmeshes[Count].m_HasSharedVertices_str = "Yes";
		}

		Ogre_Data->mSubmeshes[Count].m_Matrial_Name_str = subMesh->getMaterialName();

		Ogre_Data->mSubmeshes[Count].VerticesCount = subMesh->vertexData->vertexCount;

		Ogre_Data->mSubmeshes[Count].BonesCount = subMesh->blendIndexToBoneIndexMap.size();

		Count++;
	}

	// ------------------------------------ Bounds
	Ogre::Vector3 vMin(Ogre_Entity->getBoundingBox().getMinimum());
	Ogre::Vector3 vMax(Ogre_Entity->getBoundingBox().getMaximum());
	Ogre::Vector3 Center((vMin + vMax) * 0.5f);

	Ogre_Data->vMin = vMin;
	Ogre_Data->vMax = vMax;
	Ogre_Data->Center = Center;

	Ogre_Data->Width = (vMax - vMin).x;
	Ogre_Data->Height = (vMax - vMin).y;
	Ogre_Data->Depth = (vMax - vMin).z;
	Ogre_Data->Area = (vMax - vMin).x * (vMax - vMin).y;
	Ogre_Data->Volume = (vMax - vMin).x * (vMax - vMin).y * (vMax - vMin).z;
	Ogre_Data->Radius = Ogre_Entity->getBoundingRadius();
}

// *************************************************************************
// *	    Show_Mesh_Normals:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Mesh::Show_Mesh_Normals()
{
	if (App->CL_Model->flag_Model_Loaded == true)
	{
		HWND Temp = GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BTSHOWNORMALS);

		if (App->CL_Ogre->OGL_Listener->flag_ShowNormals == true)
		{
			App->CL_Ogre->OGL_Listener->flag_ShowNormals = false;
			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOff_Bmp);
		}
		else
		{
			App->CL_Ogre->OGL_Listener->flag_ShowNormals = true;
			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOn_Bmp);
		}
	}
}

// *************************************************************************
// *		Get_SkeletonInstance:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Mesh::Get_SkeletonInstance(Ogre::Entity* Ogre_Entity)
{

	int Loop = 0;
	if (!Ogre_Entity)
	{
		return 0;
	}

	Ogre::SkeletonInstance* skeletonInstance = Ogre_Entity->getSkeleton();

	if (skeletonInstance)
	{
		App->CL_Model->BoneCount = skeletonInstance->getNumBones();

		Ogre::Skeleton::BoneIterator itor = skeletonInstance->getBoneIterator();
		while (itor.hasMoreElements())
		{
			Ogre::Bone* bone = itor.getNext();

			App->CL_Mesh->S_Bones[Loop] = new Bone_Type;

			Ogre::Node* Parent = bone->getParent();
			if (Parent == NULL)
			{
				App->CL_Mesh->S_Bones[Loop]->Parent = -1;
			}
			else
			{
				Ogre::String ParentName = Parent->getName();
				Ogre::Bone* Parentbone = skeletonInstance->getBone(ParentName);
				App->CL_Mesh->S_Bones[Loop]->Parent = Parentbone->getHandle();
			}

			strcpy(App->CL_Mesh->S_Bones[Loop]->BoneName, bone->getName().c_str());

			App->CL_Mesh->S_Bones[Loop]->TranslationStart.X = bone->_getDerivedPosition().x;
			App->CL_Mesh->S_Bones[Loop]->TranslationStart.Y = bone->_getDerivedPosition().y;
			App->CL_Mesh->S_Bones[Loop]->TranslationStart.Z = bone->_getDerivedPosition().z;

			Loop++;
		}
	}
	return 1;
}

// *************************************************************************
// *	    Show_Mesh_Textures:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Mesh::Show_Mesh_Textures()
{
	if (App->CL_Model->flag_Model_Loaded == true)
	{
		HWND Temp = GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BTSHOWTEXTURES);

		if (App->CL_Model->Model_Type == Enums::Model_Type_Ogre3D)
		{
			if (App->CL_Model->Imported_Ogre_Ent->getVisible() == true)
			{
				App->CL_Model->Imported_Ogre_Node->setVisible(false);
				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOff_Bmp);
			}
			else
			{
				App->CL_Model->Imported_Ogre_Node->setVisible(true);
				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);
			}
		}

		return;
	}
}

// *************************************************************************
// *	    	Show_Mesh_Faces:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Mesh::Show_Mesh_Faces()
{
	if (App->CL_Model->flag_Model_Loaded == true)
	{
		HWND Temp = GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_TBSHOWFACES);

		if (App->CL_Ogre->OGL_Listener->flag_ShowFaces == true)
		{
			App->CL_Ogre->OGL_Listener->flag_ShowFaces = false;
			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);
		}
		else
		{
			App->CL_Ogre->OGL_Listener->flag_ShowFaces = true;
			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOn_Bmp);
		}
	}
}

// *************************************************************************
// *	    	Show_Mesh_Points:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Mesh::Show_Mesh_Points()
{
	if (App->CL_Model->flag_Model_Loaded == true)
	{
		HWND Temp = GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BTSHOWPOINTS);

		if (App->CL_Ogre->OGL_Listener->flag_ShowPoints == true)
		{
			App->CL_Ogre->OGL_Listener->flag_ShowPoints = false;

			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOff_Bmp);
		}
		else
		{
			App->CL_Ogre->OGL_Listener->flag_ShowPoints = true;

			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOn_Bmp);
		}
	}
}

// *************************************************************************
// *	    	Show_Mesh_BoundBox:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_Mesh::Show_Mesh_BoundBox()
{
	if (App->CL_Model->flag_Model_Loaded == true)
	{
		HWND Temp = GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_TBBOUNDBOX);

		if (App->CL_Ogre->OGL_Listener->flag_ShowBoundingBox == true)
		{
			App->CL_Ogre->OGL_Listener->flag_ShowBoundingBox = false;
			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);
		}
		else
		{
			App->CL_Ogre->OGL_Listener->flag_ShowBoundingBox = true;
			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOn_Bmp);
		}
	}
}

// *************************************************************************
// *	    	Show_Mesh_Bones:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_Mesh::Show_Mesh_Bones()
{
	if (App->CL_Model->flag_Model_Loaded == true)
	{
		HWND Temp = GetDlgItem(App->CL_Top_Tabs->TopTabs_Dlg_hWnd, IDC_BTSHOWBONES);

		if (App->CL_Ogre->OGL_Listener->flag_ShowBones == true)
		{
			App->CL_Ogre->OGL_Listener->flag_ShowBones = false;

			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);
		}
		else
		{
			App->CL_Ogre->OGL_Listener->flag_ShowBones = true;

			SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOn_Bmp);
		}
	}
}
