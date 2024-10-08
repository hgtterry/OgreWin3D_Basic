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
#include "CL64_Assimp.h"

CL64_Assimp::CL64_Assimp(void)
{
	Options.SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
	Options.Model_Type = Enums::Model_Type_None;
	
	mTotalVertices = 0;
}

CL64_Assimp::~CL64_Assimp(void)
{
}

// *************************************************************************
// *			  logInfo:- Terry and Hazel Flanigan 2024		  	 	   *
// *************************************************************************
void CL64_Assimp::logInfo(std::string logString)
{
	Assimp::DefaultLogger::get()->info(logString.c_str());
}

// *************************************************************************
// *			  logDebug:- Terry and Hazel Flanigan 2024		  	 	   *
// *************************************************************************
void CL64_Assimp::logDebug(const char* logString)
{
	Assimp::DefaultLogger::get()->debug(logString);
}

// *************************************************************************
// *				LoadFile:- Terry and Hazel Flanigan 2024	  	 	   *
// *************************************************************************
bool CL64_Assimp::LoadFile(const char* pFile)
{
	//uint32 flags = aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords | aiProcess_FlipUVs;
	//flags &= ~(aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace); // optimize for fast loading

	//flags |= aiProcessPreset_TargetRealtime_Quality;

	//if ((flags & (aiProcess_GenSmoothNormals | aiProcess_GenNormals)) != aiProcess_GenNormals)
	//	flags &= ~aiProcess_GenNormals; // prefer smooth normals

	const aiScene* scene = aiImportFile(pFile, Options.SelectedPreset);

	if (!scene)
	{
		char buf[1024];
		strcpy(buf, aiGetErrorString());
		//App->Say_Win(buf);
		return false;
	}
	else
	{

		GetBasicInfo(scene);

		Create_MeshGroups(scene);

		Get_Group_VertCount(scene);

		StoreMeshData(scene);

		LoadTextures();

		App->CL_Scene->Set_BondingBox_Model(1); // Create

		if (Options.Model_Type == Enums::Model_Type_3ds)
		{
			App->CL_Dimensions->Centre_Model_Mid_Assimp();
			App->CL_Dimensions->Rotate_Z_Assimp(90);
			App->CL_Dimensions->Centre_Model_Mid_Assimp();
		}

	}

	aiReleaseImport(scene);

	return 1;
}

// *************************************************************************
// *			GetBasicInfo:- Terry and Hazel Flanigan 2023	  	 	   *
// *************************************************************************
void CL64_Assimp::GetBasicInfo(const aiScene * pScene)
{

	bool test = pScene->HasMeshes();
	if (test == 1)
	{
		App->CL_Scene->GroupCount = pScene->mNumMeshes;
	}

	test = pScene->HasMaterials();
	if (test == 1)
	{
		int Mat = 0;

		Mat = pScene->mNumMaterials;

		if (Mat > 0)
		{
			App->CL_Scene->TextureCount = pScene->mNumMaterials;
		}
		else
		{
			App->CL_Scene->TextureCount  = -1;
		}
	}

	test = pScene->HasAnimations();
	if (test == 1)
	{
		App->CL_Scene->MotionCount = pScene->mNumAnimations;
	}
}

// *************************************************************************
// *		Create_MeshGroups_ByIndex:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Assimp::Create_MeshGroups(const aiScene* pScene)
{
	int Count = 0;

	char GroupName[255];
	char MaterialName[255];
	char GroupNum[255];

	int mGroupCount = App->CL_Scene->GroupCount;

	while (Count < mGroupCount)
	{
		aiMesh* mesh = pScene->mMeshes[Count];

		App->CL_Scene->Create_Mesh_Group(Count);

		_itoa(Count, GroupNum, 10);
		strcpy(GroupName, "Group_");
		strcat(GroupName, GroupNum);
		strcpy(App->CL_Scene->Group[Count]->GroupName, GroupName);

		strcpy(MaterialName, "Material_");
		strcat(MaterialName, GroupNum);
		strcpy(App->CL_Scene->Group[Count]->MaterialName, MaterialName);

		////---------------

		App->CL_Scene->Group[Count]->GroupVertCount = 0;
		App->CL_Scene->Group[Count]->MaterialIndex = -1;

		App->CL_Scene->Group[Count]->MaterialIndex = Count;

		strcpy(App->CL_Scene->Group[Count]->Text_FileName, "No_Texture");

		// Get Texture Path/Name
		strcpy(App->CL_Scene->Group[Count]->Texture_FolderPath, App->CL_Scene->Texture_FolderPath); // Back Slash remains

		aiString texPath;
		aiMaterial* mtl = pScene->mMaterials[mesh->mMaterialIndex];

		strcpy(App->CL_Scene->Group[Count]->MaterialName, mtl->GetName().C_Str());

		if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath))
		{
			strcpy(App->CL_Scene->Group[Count]->Text_FileName, texPath.C_Str());
			strcpy(App->CL_Scene->Group[Count]->Equity_Text_FileName, texPath.C_Str());
		}
		else
		{
			strcpy(App->CL_Scene->Group[Count]->Text_FileName, "No_Texture");
			App->CL_Scene->Group[Count]->MaterialIndex = -1;
		}

		Count++;
	}

}

// *************************************************************************
// *		Get_Group_VertCount:- Terry and Hazel Flanigan 2024		   	   *
// *************************************************************************
void CL64_Assimp::Get_Group_VertCount(const aiScene* pScene)
{
	int Count = 0;
	unsigned int t = 0;
	unsigned int i = 0;
	int VC = 0;
	int mTotalVertices = 0;
	int mTotalFaces = 0;
	
	int mGroupCount = App->CL_Scene->GroupCount;

	while (Count < mGroupCount)
	{
		aiMesh* mesh = pScene->mMeshes[Count];

		VC = 0;
		t = 0;

		mTotalFaces = mTotalFaces + mesh->mNumFaces;

		App->CL_Scene->Group[Count]->GroupFaceCount = mesh->mNumFaces;

		while (t < mesh->mNumFaces)
		{
			aiFace* face = &mesh->mFaces[t];

			i = 0;
			while (i < face->mNumIndices)
			{
				VC++;
				i++;
			}

			t++;
		}

		App->CL_Scene->Group[Count]->vertex_Data.resize(VC);
		App->CL_Scene->Group[Count]->Normal_Data.resize(VC);
		App->CL_Scene->Group[Count]->MapCord_Data.resize(VC);
		App->CL_Scene->Group[Count]->Face_Data.resize(VC);

		mTotalVertices = mTotalVertices + mesh->mNumVertices;
		Count++;
	}

	App->CL_Scene->VerticeCount = mTotalVertices;
	App->CL_Scene->FaceCount = mTotalFaces;

}

// *************************************************************************
// *			StoreMeshData:- Terry and Hazel Flanigan 2024		   	   *
// *************************************************************************
void CL64_Assimp::StoreMeshData(const aiScene* pScene)
{
	int FaceNum = 0;
	int FaceIndexNum = 0;
	mTotalVertices = 0;
	int GroupCount = 0;
	unsigned int t = 0;
	unsigned int i = 0;
	int VC = 0;

	int mGroupCount = App->CL_Scene->GroupCount;

	while (GroupCount < mGroupCount)
	{
		aiMesh* mesh = pScene->mMeshes[GroupCount];

		VC = 0;
		t = 0;

		App->CL_Scene->Group[GroupCount]->GroupFaceCount = mesh->mNumFaces;
		App->CL_Scene->Group[GroupCount]->FaceIndex_Data.resize(mesh->mNumFaces * 3);
		App->CL_Scene->Group[GroupCount]->Face_Data.resize(mesh->mNumFaces);

		while (t < mesh->mNumFaces)
		{
			aiFace* face = &mesh->mFaces[t];

			i = 0;
			while (i < face->mNumIndices) // Triangulated Each face has 3 Indices
			{
				int vertexIndex = face->mIndices[i];

				App->CL_Scene->Group[GroupCount]->vertex_Data[vertexIndex].x = mesh->mVertices[vertexIndex].x;
				App->CL_Scene->Group[GroupCount]->vertex_Data[vertexIndex].y = mesh->mVertices[vertexIndex].y;
				App->CL_Scene->Group[GroupCount]->vertex_Data[vertexIndex].z = mesh->mVertices[vertexIndex].z;


				if (mesh->HasNormals())
				{
					App->CL_Scene->Group[GroupCount]->Normal_Data[vertexIndex].x = mesh->mNormals[vertexIndex].x;
					App->CL_Scene->Group[GroupCount]->Normal_Data[vertexIndex].y = mesh->mNormals[vertexIndex].y;
					App->CL_Scene->Group[GroupCount]->Normal_Data[vertexIndex].z = mesh->mNormals[vertexIndex].z;
				}

				//		App->CL_Scene_Data->S_MeshGroup[Count]->FaceIndices[VC] = FaceNum;

				if (mesh->HasTextureCoords(0))
				{
					App->CL_Scene->Group[GroupCount]->MapCord_Data[vertexIndex].u = mesh->mTextureCoords[0][vertexIndex].x;
					App->CL_Scene->Group[GroupCount]->MapCord_Data[vertexIndex].v = 1 - mesh->mTextureCoords[0][vertexIndex].y;
				}

				VC++;
				i++;
				FaceNum++;
			}

			App->CL_Scene->Group[GroupCount]->Face_Data[t].a = face->mIndices[0];
			App->CL_Scene->Group[GroupCount]->Face_Data[t].b = face->mIndices[1];
			App->CL_Scene->Group[GroupCount]->Face_Data[t].c = face->mIndices[2];

			App->CL_Scene->Group[GroupCount]->FaceIndex_Data[t].Index = FaceIndexNum;

			FaceIndexNum++;

			t++;
		}

		App->CL_Scene->Group[GroupCount]->GroupVertCount = mesh->mNumVertices;

		mTotalVertices = mTotalVertices + mesh->mNumVertices;
		GroupCount++;
	}
}

// *************************************************************************
// *			LoadTextures:- Terry and Hazel Flanigan 2024	 	 	   *
// *************************************************************************
void CL64_Assimp::LoadTextures()
{
	App->CL_Textures->Load_Textures_Assimp();
}
