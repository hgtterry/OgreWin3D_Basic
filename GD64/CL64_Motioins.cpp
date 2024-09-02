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
#include "CL64_Motioins.h"

CL64_Motioins::CL64_Motioins(void)
{
	flag_Motion_Playing = 0;
	flag_Motion_Paused = 0;

	AnimationScale = 1;
	Animate_State = nullptr;

}

CL64_Motioins::~CL64_Motioins(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Motioins::Reset_Class(void)
{
	App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 0;
	Stop_SelectedMotion();
	flag_Motion_Playing = 0;
}

// *************************************************************************
// *			Update_Motion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Update_Motion(float deltaTime)
{
	Animate_State->addTime(deltaTime * AnimationScale);

	
	if (App->CL_Scene->Imported_Ogre_Ent)
	{
		App->CL_Scene->Imported_Ogre_Ent->_updateAnimation();
		Update_MeshData();
	}

	
	/*if (App->Cl_Ogre->RenderListener->Show_Crosshair == 1)
	{
		App->Cl_Bones->Move_BoneCrosshair();
	}*/

}

// *************************************************************************
// *			Update_MeshData:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Update_MeshData(void)
{
	App->CL_Motions->UpdateBones_Orge(false);
	App->CL_Motions->AnimationExtract_Mesh(false);
	App->CL_Scene->Set_BondingBox_Model(false);
}

// *************************************************************************
// *		Pause_SelectedMotion:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Motioins::Pause_SelectedMotion(void)
{
	if (App->CL_Scene->MotionCount > 0)
	{
		if (App->CL_Scene->Imported_Ogre_Ent)
		{
			if (flag_Motion_Playing == 1)
			{
				App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 0;
				Update_MeshData();
			}

			flag_Motion_Paused = 1;
			RedrawWindow(App->CL_TopDlg->Motions_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
}

// *************************************************************************
// *		Play_SelectedMotion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Play_SelectedMotion(void)
{
	if (App->CL_Scene->MotionCount > 0)
	{
		if (App->CL_Scene->Imported_Ogre_Ent)
		{
			Animate_State = App->CL_Scene->Imported_Ogre_Ent->getAnimationState(App->CL_TopDlg->Selected_Motion_Name);
			Animate_State->setEnabled(true);
			App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 1;

			flag_Motion_Paused = 0;
			flag_Motion_Playing = 1;

			RedrawWindow(App->CL_TopDlg->Motions_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
}

// *************************************************************************
// *		Stop_SelectedMotion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Stop_SelectedMotion(void)
{
	if (App->CL_Scene->MotionCount > 0)
	{
		if (App->CL_Scene->Imported_Ogre_Ent)
		{
			if (flag_Motion_Playing == 1)
			{
				App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 0;
				Animate_State->setEnabled(false);
				Animate_State->setTimePosition(0);
				Motion_Set_Pose();
			}

			flag_Motion_Paused = 0;
			flag_Motion_Playing = 0;
			RedrawWindow(App->CL_TopDlg->Motions_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
}

// *************************************************************************
// *			Motion_Set_Pose:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Motion_Set_Pose(void)
{
	if (App->CL_Scene->MotionCount > 0)
	{
		if (App->CL_Scene->Imported_Ogre_Ent)
		{
			App->CL_Motions->UpdateBones_Orge(true);
			App->CL_Motions->AnimationExtract_Mesh(true);
			App->CL_Scene->Set_BondingBox_Model(false);
		}
	}
}

#pragma warning(disable:4996)

// *************************************************************************
// *		   UpdateBones_Orge:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::UpdateBones_Orge(bool Reset)
{
	int Loop = 0;
	if (!App->CL_Scene->Imported_Ogre_Ent)
	{
		return;
	}

	Ogre::SkeletonInstance* skeletonInstance = App->CL_Scene->Imported_Ogre_Ent->getSkeleton();

	if (skeletonInstance)
	{
		if (Reset == 1)
		{
			skeletonInstance->reset(false);
		}

		Ogre::Skeleton::BoneIterator itor = skeletonInstance->getBoneIterator();
		while (itor.hasMoreElements())
		{
			Ogre::Bone* bone = itor.getNext();

			App->CL_Scene->S_Bones[Loop]->TranslationStart.X = bone->_getDerivedPosition().x;
			App->CL_Scene->S_Bones[Loop]->TranslationStart.Y = bone->_getDerivedPosition().y;
			App->CL_Scene->S_Bones[Loop]->TranslationStart.Z = bone->_getDerivedPosition().z;

			//App->CL_Model_Data->S_Bones[Loop]->TranslationStart.X = bone->_getDerivedOrientation().

			Loop++;
		}
	}

	return;
}

// *************************************************************************
// *							AnimationExtract_Mesh			   	 	   *
// *************************************************************************
void CL64_Motioins::AnimationExtract_Mesh(bool DefaultPose)
{
	int FaceCount = 0;
	int FaceNum = 0;
	int FaceIndexNum = 0;
	size_t vertex_count, index_count;
	Vector3* vertices;
	//	Vector3* normals;
	unsigned long* indices;

	int SubMeshCount = App->CL_Scene->Imported_Ogre_Ent->getNumSubEntities();
	int loop = 0;
	int Count = 0;

	while (Count < SubMeshCount)
	{
		Get_AnimationInstance(App->CL_Scene->Imported_Ogre_Ent->getMesh(), vertex_count, vertices, index_count, indices, Count, DefaultPose);
		Count++;
	}

	return;
}
// *************************************************************************
// *					Get_AnimationInstance Terry Bernie				   *
// *************************************************************************
void CL64_Motioins::Get_AnimationInstance(Ogre::MeshPtr mesh,
	size_t& vertex_count,
	Ogre::Vector3*& vertices,
	size_t& index_count,
	unsigned long*& indices,
	int SubMesh,
	bool DefaultPose)
{
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

	index_count = submesh->indexData->indexCount;

	// Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	indices = new unsigned long[index_count];

	Ogre::VertexData* vertex_data;

	//-------------------- Get Data
	if (DefaultPose == 0)
	{
		vertex_data = submesh->useSharedVertices ? App->CL_Scene->Imported_Ogre_Ent->_getSkelAnimVertexData() : App->CL_Scene->Imported_Ogre_Ent->getSubEntity(SubMesh)->_getSkelAnimVertexData();
	}
	else
	{
		vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
	}

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

		for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
		{
			posElem->baseVertexPointerToElement(vertex, &pReal);
			Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
			vertices[current_offset + j] = (orient * (pt * scale)) + position;
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

	int loop = 0;
	while (loop < vertex_count)
	{
		App->CL_Scene->Group[SubMesh]->vertex_Data[loop].x = vertices[loop].x;
		App->CL_Scene->Group[SubMesh]->vertex_Data[loop].y = vertices[loop].y;
		App->CL_Scene->Group[SubMesh]->vertex_Data[loop].z = vertices[loop].z;

		loop++;
	}

	delete vertices;
	delete indices;
}
