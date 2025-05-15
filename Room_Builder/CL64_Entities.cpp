/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
#include "CL64_Entities.h"
#include "Room Builder.h"
#include "Structures.cpp"

CL64_Entities::CL64_Entities()
{
	Vertice_Count_Copy = 0;
}

CL64_Entities::~CL64_Entities()
{
}
// *************************************************************************
// *						Brush_SetGroupId							   *
// *************************************************************************
//void Brush_SetGroupId(Brush* b, const int gid)
//{
//	if (b->Type == BRUSH_MULTI)
//	{
//		//BrushList_SetInt(b->BList, gid, Brush_SetGroupId);
//	}
//
//	b->GroupId = gid;
//}

// **************************************************************************
// *	  		GetIndex_By_Name:- Terry and Hazel Flanigan 2024			*
// **************************************************************************
int CL64_Entities::GetIndex_By_Name(char* Name)
{
	int Count = 0;
	int Total = App->CL_Editor_Com->Object_Count;

	while (Count < Total)
	{
		if (App->CL_Editor_Com->B_Object[Count]->flag_Deleted == 0)
		{
			int Result = 1;
			Result = strcmp(App->CL_Editor_Com->B_Object[Count]->Object_Name, Name);
			if (Result == 0)
			{
				return Count;
			}
		}

		Count++;
	}

	return -1;
}

// *************************************************************************
// *		Create_Player_Entity:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_Entities::Create_Player_Brush()
{
	/*bool test = App->CL_Brush_X->Check_if_Brush_Name_Exist((LPSTR)"Player_Main");
	if (test == 0)
	{
		return;
	}*/

	Brush* Player_Brush;

	BrushTemplate_Box* pBoxTemplate;

	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();


	pBoxTemplate->Solid = 0;
	pBoxTemplate->YSize = 25;

	pBoxTemplate->XSizeBot = 8;
	pBoxTemplate->XSizeTop = 8;
	pBoxTemplate->ZSizeBot = 8;
	pBoxTemplate->ZSizeTop = 8;

	Player_Brush = App->CL_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);

	App->CL_Brush->Brush_Bound(Player_Brush);

	App->CL_Doc->SetDefaultBrushTexInfo(Player_Brush);

	App->CL_Brush->Brush_Bound(Player_Brush);

	Brush_SetGroupId(Player_Brush, Enums::Brushs_ID_Players);

	fdocFaceScales Scales;

	Scales.DrawScale = App->CL_Level->Level_GetDrawScale(App->CL_Doc->Current_Level);
	Scales.LightmapScale = App->CL_Level->Level_GetLightmapScale(App->CL_Doc->Current_Level);
	App->CL_Brush->Brush_EnumFaces(Player_Brush, &Scales, fdocSetFaceScales);

	strcpy(Player_Brush->Name, "Player_Main");

	App->CL_Level->Level_AppendBrush(Player_Brush);

	App->CL_Doc->CurBrush = Player_Brush;
	App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);

	App->CL_Brush->Brush_SetLocked(Player_Brush, true);
}

// *************************************************************************
// *		Create_Environment_Brush:- Terry and Hazel Flanigan 2025 	   *
// *************************************************************************
void CL64_Entities::Create_Environment_Brush(int Object_Index)
{
	char m_Name[MAX_PATH];
	strcpy(m_Name, App->CL_Editor_Com->B_Object[Object_Index]->Object_Name);

	bool test = App->CL_Brush_X->Check_if_Brush_Name_Exist((LPSTR)m_Name);
	if (test == true)
	{
		Brush* b = App->CL_Brush_X->Get_Brush_By_Name(m_Name);
		if (b)
		{
			App->CL_Entities->Ogre_To_Mesh_Data(App->CL_Editor_Com->B_Object[Object_Index]->Object_Ent, App->CL_Editor_Com->B_Object[Object_Index]->Object_Node);
			App->CL_Brush_X->Set_Brush_Face_Points(b);


			b->Last_Rotation.x = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Rot.x;
			b->Last_Rotation.y = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Rot.y;
			b->Last_Rotation.z = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Rot.z;
		}

		Debug
		return;
	}

	Brush* Environ_Brush = NULL;

	BrushTemplate_Box* pBoxTemplate;

	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();

	Ogre::Vector3 size = App->CL_Com_Objects->GetMesh_BB_Size(App->CL_Editor_Com->B_Object[Object_Index]->Object_Node);


	pBoxTemplate->Solid = 0;
	pBoxTemplate->YSize = size.y;

	pBoxTemplate->XSizeBot = size.x;
	pBoxTemplate->XSizeTop = size.x;
	pBoxTemplate->ZSizeBot = size.z;
	pBoxTemplate->ZSizeTop = size.z;

	Environ_Brush = App->CL_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);

	App->CL_Brush->Brush_Bound(Environ_Brush);

	App->CL_Doc->SetDefaultBrushTexInfo(Environ_Brush);

	App->CL_Brush->Brush_Bound(Environ_Brush);

	Brush_SetGroupId(Environ_Brush, Enums::Brushs_ID_Evirons);

	fdocFaceScales Scales;

	Scales.DrawScale = App->CL_Level->Level_GetDrawScale(App->CL_Doc->Current_Level);
	Scales.LightmapScale = App->CL_Level->Level_GetLightmapScale(App->CL_Doc->Current_Level);
	App->CL_Brush->Brush_EnumFaces(Environ_Brush, &Scales, fdocSetFaceScales);

	strcpy(Environ_Brush->Name, App->CL_Editor_Com->B_Object[Object_Index]->Object_Name);

	T_Vec3 Pos;
	Pos.x = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Pos.x;
	Pos.y = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Pos.y;
	Pos.z = App->CL_Editor_Com->B_Object[Object_Index]->Mesh_Pos.z;

	App->CL_Brush->Brush_Move(Environ_Brush, &Pos);

	App->CL_Level->Level_AppendBrush(Environ_Brush);

	App->CL_Doc->CurBrush = Environ_Brush;
	App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);

	App->CL_Brush->Brush_SetLocked(Environ_Brush, true);

	App->CL_Properties_Brushes->Fill_ListBox();
}

// *************************************************************************
// *			Ogre_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Entities::Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity, Ogre::Node* Ogre_Node)
{
	Vertice_Count_Copy = 0;

	Convert_To_Mesh_Data(Ogre_Entity, Ogre_Node);

	return 1;
}

// *************************************************************************
// *		Convert_To_Mesh_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Entities::Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity, Ogre::Node* Ogre_Node)
{
	bool Has_Shared_Vertices = 0;

	int Count = 0;
	int SubMeshCount = Ogre_Entity->getNumSubEntities();

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
		int New_Vert_Count = 0;

		vertex_Data.resize(36);

		while (Count < SubMeshCount)
		{
			Get_SubPose_MeshInstance(Ogre_Entity->getMesh(), vertex_count, vertices, Count, Ogre_Node);

			FaceIndexNum = 0;
			int Faceit = 0;
			FaceCount = 0;
			Vertloop = 0;
			xx = 0;

			while (Vertloop < vertex_count) // Process Vertices
			{
				vertex_Data[New_Vert_Count].x = vertices[Vertloop].x;
				vertex_Data[New_Vert_Count].y = vertices[Vertloop].y;
				vertex_Data[New_Vert_Count].z = vertices[Vertloop].z;

				New_Vert_Count++;
				Vertloop++;
			}


			Count++;
		}

		Vertice_Count_Copy = New_Vert_Count;
	}

	for (int i = 0; i < vertex_Data.size(); i++)
	{

		for (int j = i + 1; j < vertex_Data.size();) {

			// Erase duplicates
			if (vertex_Data[j].x == vertex_Data[i].x && vertex_Data[j].y == vertex_Data[i].y && vertex_Data[j].z == vertex_Data[i].z)
			{
				vertex_Data.erase(vertex_Data.begin() + j);
			}
			else
			{
				j++;
			}
		}
	}


	// Vertices 0 to 3 are the 4 corners of the top face
	Verts[0] = { vertex_Data[0].x,vertex_Data[0].y,vertex_Data[0].z };
	Verts[1] = { vertex_Data[2].x,vertex_Data[2].y,vertex_Data[2].z };
	Verts[2] = { vertex_Data[6].x,vertex_Data[6].y,vertex_Data[6].z };
	Verts[3] = { vertex_Data[4].x,vertex_Data[4].y,vertex_Data[4].z };

	// Vertices 4 to 7 are the 4 corners of the bottom face
	Verts[4] = { vertex_Data[1].x,vertex_Data[1].y,vertex_Data[1].z };
	Verts[5] = { vertex_Data[5].x,vertex_Data[5].y,vertex_Data[5].z };
	Verts[6] = { vertex_Data[7].x,vertex_Data[7].y,vertex_Data[7].z };
	Verts[7] = { vertex_Data[3].x,vertex_Data[3].y,vertex_Data[3].z };

	return 1;
}

// *************************************************************************
// *	   Get_SubPose_MeshInstance:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Entities::Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
	size_t& vertex_count, Ogre::Vector3*& vertices, int SubMesh, Ogre::Node* Ogre_Node)
{

	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;

	const Vector3& position = Ogre_Node->getPosition();
	const Quaternion& orient = Ogre_Node->getOrientation();
	const Vector3& scale = Ogre_Node->getScale();

	Ogre::SubMesh* submesh = mesh->getSubMesh(SubMesh);

	vertex_count = submesh->vertexData->vertexCount;
	Ogre::SubMesh::VertexBoneAssignmentList plist = submesh->getBoneAssignments();

	// Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];

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

	current_offset = next_offset;
}
