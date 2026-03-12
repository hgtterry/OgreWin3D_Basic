/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
#include "CL64_Motioins.h"

CL64_Motioins::CL64_Motioins(void)
{
	flag_Motion_Playing = false;
	flag_Motion_Paused = false;
	flag_IsAnimated = false;

	AnimationScale = 1;
	Animate_State = nullptr;

	Selected_Motion_Name[0] = 0;
}

CL64_Motioins::~CL64_Motioins(void)
{
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Motioins::Reset_Class(void)
{
	App->CL_Ogre->Listener_3D->flag_Animate_Ogre = false;
	Stop_SelectedMotion();
	flag_Motion_Playing = false;
}

// *************************************************************************
// *	  			Get_Motions:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Get_Motions(Ogre::Entity* Ogre_Entity)
{
	Ogre::SkeletonInstance* skeletonInstance = Ogre_Entity->getSkeleton();

	App->CL_Model->MotionCount = 0;

	int Count = 0;
	if (skeletonInstance)
	{
		int AnimationCount = skeletonInstance->getNumAnimations();
		if (AnimationCount == 0)
		{
			flag_IsAnimated = false;
			App->CL_Model->MotionCount = 0;
			App->CL_Properties_Motions->Update_Motions_Combo();
		}
		else
		{

			flag_IsAnimated = true;

			for (unsigned short i = 0; i < skeletonInstance->getNumAnimations(); ++i)
			{
				Ogre::Animation* animation = skeletonInstance->getAnimation(i);
				App->CL_Mesh->S_OgreMeshData[0]->m_Motion_Names.push_back(animation->getName());
				App->CL_Mesh->S_OgreMeshData[0]->m_Motion_Length.push_back(animation->getLength());
				App->CL_Mesh->S_OgreMeshData[0]->m_Motion_Num_Of_Tracks.push_back(animation->getNumNodeTracks());
				
				//animation->getNumNodeTracks();
				
				Count = i;
			}

			App->CL_Model->MotionCount = Count + 1;

			App->CL_ImGui->listMotionItems_Ogre[App->CL_ImGui->PreviouseMotion_Ogre] = false;
			App->CL_ImGui->listMotionItems_Ogre[0] = true;
			App->CL_ImGui->PreviouseMotion_Ogre = 0;

			App->CL_Properties_Motions->Update_Motions_Combo();
		}
	}
	else
	{
		flag_IsAnimated = false;
		App->CL_Model->MotionCount = 0;
		App->CL_Properties_Motions->Update_Motions_Combo();
	}
}

// *************************************************************************
// *			Update_Motion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Update_Motion(float deltaTime)
{
	if (App->CL_Model->Imported_Ogre_Ent)
	{
		if (Animate_State)
		{
			Animate_State->addTime(deltaTime * AnimationScale);

			App->CL_Model->Imported_Ogre_Ent->_updateAnimation();
			Update_MeshData();
		}
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
	UpdateBones_Orge(false);
	AnimationExtract_Mesh(false);
	App->CL_Model->Set_BondingBox_Model(false);
}

// *************************************************************************
// *		Pause_SelectedMotion:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Motioins::Pause_SelectedMotion(void)
{
	if (App->CL_Model->MotionCount > 0)
	{
		if (App->CL_Model->Imported_Ogre_Ent)
		{
			if (flag_Motion_Playing == 1)
			{
				App->CL_Ogre->Listener_3D->flag_Animate_Ogre = false;
				Update_MeshData();
			}

			flag_Motion_Paused = true;
			//RedrawWindow(App->CL_TopDlg->Motions_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
}

// *************************************************************************
// *		Play_SelectedMotion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Play_SelectedMotion(void)
{
	Animate_State = nullptr;

	if (App->CL_Model->MotionCount > 0)
	{
		if (App->CL_Model->Imported_Ogre_Ent)
		{
			Animate_State = App->CL_Model->Imported_Ogre_Ent->getAnimationState(Selected_Motion_Name);

			if (Animate_State)
			{
				Animate_State->setEnabled(true);
				App->CL_Ogre->Listener_3D->flag_Animate_Ogre = true;

				flag_Motion_Paused = false;
				flag_Motion_Playing = true;
			}

			RedrawWindow(App->CL_Properties_Motions->Motions_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
}

// *************************************************************************
// *		Stop_SelectedMotion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Stop_SelectedMotion(void)
{
	if (App->CL_Model->MotionCount > 0)
	{
		if (App->CL_Model->Imported_Ogre_Ent)
		{
			if (flag_Motion_Playing == 1)
			{
				App->CL_Ogre->Listener_3D->flag_Animate_Ogre = 0;
				Animate_State->setEnabled(false);
				Animate_State->setTimePosition(0);
				Motion_Set_Pose();
			}

			flag_Motion_Paused = false;
			flag_Motion_Playing = false;
			RedrawWindow(App->CL_Properties_Motions->Motions_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
}

// *************************************************************************
// *			Motion_Set_Pose:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motioins::Motion_Set_Pose(void)
{
	if (App->CL_Model->MotionCount > 0)
	{
		if (App->CL_Model->Imported_Ogre_Ent)
		{
			UpdateBones_Orge(true);
			AnimationExtract_Mesh(true);
			App->CL_Model->Set_BondingBox_Model(false);
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
	if (!App->CL_Model->Imported_Ogre_Ent)
	{
		return;
	}

	Ogre::SkeletonInstance* skeletonInstance = App->CL_Model->Imported_Ogre_Ent->getSkeleton();

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

			App->CL_Mesh->S_Bones[Loop]->TranslationStart.X = bone->_getDerivedPosition().x;
			App->CL_Mesh->S_Bones[Loop]->TranslationStart.Y = bone->_getDerivedPosition().y;
			App->CL_Mesh->S_Bones[Loop]->TranslationStart.Z = bone->_getDerivedPosition().z;

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

	int SubMeshCount = App->CL_Model->Imported_Ogre_Ent->getNumSubEntities();
	int loop = 0;
	int Count = 0;

	while (Count < SubMeshCount)
	{
		Get_AnimationInstance(App->CL_Model->Imported_Ogre_Ent->getMesh(), vertex_count, vertices, index_count, indices, Count, DefaultPose);
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
		vertex_data = submesh->useSharedVertices ? App->CL_Model->Imported_Ogre_Ent->_getSkelAnimVertexData() : App->CL_Model->Imported_Ogre_Ent->getSubEntity(SubMesh)->_getSkelAnimVertexData();
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
		App->CL_Mesh->Group[SubMesh]->vertex_Data[loop].x = vertices[loop].x;
		App->CL_Mesh->Group[SubMesh]->vertex_Data[loop].y = vertices[loop].y;
		App->CL_Mesh->Group[SubMesh]->vertex_Data[loop].z = vertices[loop].z;

		loop++;
	}

	delete vertices;
	delete indices;
}

