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
#include "CL64_Picking.h"
#include "Structures.cpp"

CL64_Picking::CL64_Picking(void)
{
    mRaySceneQuery = NULL;

    pentity = NULL;
    Pl_Entity_Name.clear();

    flag_Hit_Player = 0;
    closest_distance = 0;
    Total_vertex_count = 0;
    Total_index_count = 0;
    Face_Index = 0;
    Sub_Mesh_Count = 0;
    m_SubMesh = 0;
    Face_Count = 0;
    flag_Selected_Ok = 0;
   
    Face_Hit = 0;
    Actual_Face = 0;
    FaceMaterial[0] = 0;
    TextureName[0] = 0;
    TestName[0] = 0;

    m_Texture_FileName[0] = 0;

    Local_Face = 0;
}

CL64_Picking::~CL64_Picking(void)
{
}

// *************************************************************************
// *			Init_Picking:- Terry and Hazel Flanigan 2025   		  	   *
// *************************************************************************
void CL64_Picking::Init_Picking()
{
    mRaySceneQuery = App->CL_Ogre->mSceneMgr->createRayQuery(Ogre::Ray());
    if (mRaySceneQuery == NULL)
    {
        App->Say("No Ray Query");
        return;
    }
}

// *************************************************************************
// *		  Clear_Picking_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
void CL64_Picking::Clear_Picking_Data()
{
    Pl_Entity_Name.clear();

    closest_distance = 0;
    flag_Hit_Player = 0;
    Total_vertex_count = 0;
    Total_index_count = 0;
    Face_Index = 0;
    Sub_Mesh_Count = 0;
    m_SubMesh = 0;
    flag_Selected_Ok = 0;

    Local_Face = 0;

    strcpy(m_Texture_FileName, "No_Texture");
}

// *************************************************************************
// *		Mouse_Pick_Entity:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Picking::Mouse_Pick_Entity()
{
    Clear_Picking_Data();

    Ogre::RenderWindow* rw = App->CL_Ogre->mWindow;
    Ogre::Camera* camera = App->CL_Ogre->mCamera;

    ImGuiIO& io = ImGui::GetIO();

    Ogre::Real tx = io.MousePos.x / (Ogre::Real)rw->getWidth();
    Ogre::Real ty = io.MousePos.y / (Ogre::Real)rw->getHeight();

    Pl_Entity_Name = "-----";

    Ogre::uint32 queryMask = -1;
    Ogre::Vector3 result = Ogre::Vector3(0, 0, 0);
    Ogre::MovableObject* target = NULL;
    closest_distance = 0;

    Pl_Entity_Name = " ";
    //App->CL_Ogre->Ogre3D_Listener->Selected_Object_Name[0] = 0;

    Ogre::SceneNode* mNode;

    Ogre::Ray ray2 = camera->getCameraToViewportRay(tx, ty);
    if (raycast(ray2, result, target, closest_distance, queryMask))
    {
        
        mNode = pentity->getParentSceneNode();

        if (flag_Hit_Player == 0)
        {
            Pl_Entity_Name = pentity->getName();
        }
        else
        {
            Pl_Entity_Name = "No Selection";
        }

        char buff[255];
        strcpy(buff, Pl_Entity_Name.c_str());

        bool test = Ogre::StringUtil::match("Plane0", Pl_Entity_Name, true);
        if (test == 1)
        {
            Pl_Entity_Name = "---------";
        }
        else
        {
            bool test = Ogre::StringUtil::match("Player_1", Pl_Entity_Name, true);
            if (test == 1)
            {
                Pl_Entity_Name = "Player_1";
                return;
            }
            else
            {
                char* pdest;
                int IntNum = 0;

                strcpy(buff, Pl_Entity_Name.c_str());
                pdest = strstr(buff, "GDEnt_");
                if (pdest != NULL)
                {
                    (void)sscanf((buff + 6), "%i", &IntNum);

                    /*App->CL_Gizmos->MarkerBox_Addjust(IntNum);

                    App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index = IntNum;
                    strcpy(App->CL_Ogre->Ogre3D_Listener->Selected_Object_Name, App->CL_Scene->B_Object[IntNum]->Object_Name);

                    App->CL_FileView->SelectItem(App->CL_Scene->B_Object[App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index]->FileViewItem);*/

                    return;

                }

                pdest = strstr(buff, "Area_");
                if (pdest != NULL)
                {
                   /* (void)sscanf((buff + 5), "%i", &IntNum);

                    App->CL_Gizmos->MarkerBox_Addjust(IntNum);

                    App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index = IntNum;
                    strcpy(App->CL_Ogre->Ogre3D_Listener->Selected_Object_Name, App->CL_Scene->B_Area[IntNum]->Area_Name);

                    App->CL_FileView->SelectItem(App->CL_Scene->B_Area[App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index]->FileViewItem);*/

                    return;
                }

            }
        }

    }
    else
    {
        Pl_Entity_Name = "---------";
        strcpy(TextureName, "None 2");
        flag_Selected_Ok = 0;
    }
}

#pragma warning( disable: 6386 )

// *************************************************************************
// *		        raycast:- Terry and Hazel Flanigan 2024		       	   *
// *************************************************************************
bool CL64_Picking::raycast(const Ogre::Ray& ray, Ogre::Vector3& result, Ogre::MovableObject*& target, float& closest_distance, const Ogre::uint32 queryMask)
{
    target = NULL;
    Pl_Entity_Name = "---------";
    flag_Hit_Player = 0;
    Actual_Face = 0;
    Face_Hit = 0;

    if (mRaySceneQuery != NULL)
    {
        mRaySceneQuery->setRay(ray);
        mRaySceneQuery->setSortByDistance(true);

        mRaySceneQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
        // execute the query, returns a vector of hits
        if (mRaySceneQuery->execute().size() <= 0)
        {
            // raycast did not hit an objects bounding box
            flag_Selected_Ok = 0;
            Pl_Entity_Name = "---------";
            return (false);
        }
    }
    else
    {
        App->Say("No Ray Query");
        flag_Selected_Ok = 0;
        return (false);
    }

    closest_distance = -1.0f;

    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult query_result = mRaySceneQuery->getLastResults();

    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        if ((closest_distance >= 0.0f) &&
            (closest_distance < query_result[qr_idx].distance))
        {
            strcpy(TextureName, query_result[0].movable->getMovableType().c_str());
            break;
        }

        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != NULL) &&
            (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
        {
            // get the entity to check
            strcpy(TextureName, "Entity");

            pentity = static_cast<Ogre::MovableObject*>(query_result[qr_idx].movable);
            strcpy(TestName, (LPSTR)(Ogre::Entity*)pentity->getName().c_str());

            // Test for player
            int test = strcmp(TestName, "Player_1");
            if (test == 0)
            {
                flag_Hit_Player = 1;
            }
            else
            {
                flag_Hit_Player = 0;
            }


            if (flag_Hit_Player == 0)
            {
                // get the mesh information
                GetMeshInformation(((Ogre::Entity*)pentity)->getMesh(),
                    pentity->getParentNode()->_getDerivedPosition(),
                    pentity->getParentNode()->_getDerivedOrientation(),
                    pentity->getParentNode()->_getDerivedScale());

                // test for hitting individual triangles on the mesh
                bool new_closest_found = false;
                for (int i = 0; i < Total_index_count; i += 3)
                {
                    // check for a hit against this triangle
                    std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
                        vertices[indices[i + 1]], vertices[indices[i + 2]], true, false);

                    // if it was a hit check if its the closest
                    if (hit.first)
                    {
                        if ((closest_distance < 0.0f) ||
                            (hit.second < closest_distance))
                        {
                            // this is the closest so far, save it off
                            closest_distance = hit.second;
                            new_closest_found = true;


                            //Face_Hit = (Actual_Face / 3) - (Face_Index / 3);

                            Face_Index = i;

                            App->CL_Grid->HitVertices[0] = vertices[indices[i]];
                            App->CL_Grid->HitVertices[1] = vertices[indices[i + 1]];
                            App->CL_Grid->HitVertices[2] = vertices[indices[i + 2]];

                            App->CL_Grid->Face_Update2();
                            
                            m_SubMesh = Sub_Mesh_Indexs[Face_Index];

                            Local_Face = Get_Local_Face(m_SubMesh);

                            Face_Hit = Get_Global_Face(m_SubMesh) + Actual_Face;

                            App->CL_Grid->FaceNode->setVisible(true);
                        }
                    }

                    Actual_Face++;// = 3;
                }

                // free the verticies and indicies memory
                delete[] vertices;
                delete[] indices;
                delete[] TextCords;
                delete[] Sub_Mesh_Indexs;

                // if we found a new closest raycast for this object, update the
                // closest_result before moving on to the next object.
                if (new_closest_found)
                {
                    target = pentity;

                    Sub_Mesh_Count = ((Ogre::Entity*)pentity)->getMesh()->getNumSubMeshes();

                    closest_result = ray.getPoint(closest_distance);
                }
            }
        }

    }

    if (closest_distance >= 0.0f)
    {
        // raycast success
        result = closest_result;
        flag_Selected_Ok = 1;

        Get_Material_Data();

        Ogre::Vector3 Pick_Vecs;
        Ogre::Vector3 Face_Vecs;

       // Pick_Vecs.x = App->CL_Grid->HitVertices[0].x;
       // Pick_Vecs.y = App->CL_Grid->HitVertices[0].y;
       // Pick_Vecs.z = App->CL_Grid->HitVertices[0].z;

        int A = 0;
        int B = 0;
        int C = 0;

        int Count = 0;
        return (true);
    }
    else
    {
        // raycast failed
        flag_Selected_Ok = 0;
        Pl_Entity_Name = "---------";
        return (false);
    }
}

// *************************************************************************
// *		    Get_Local_Face:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
int CL64_Picking::Get_Local_Face(int SelectedGroup)
{
    Ogre::MeshPtr mesh = ((Ogre::Entity*)pentity)->getMesh();

    //bool added_shared = false;
    int TotalFaces = 0;
    int Count = 0;
    //int SubMeshes = mesh->getNumSubMeshes();

    while (Count < SelectedGroup)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(Count);
        TotalFaces += submesh->indexData->indexCount;

        Count++;
    }

    int Result = (Face_Index / 3) - (TotalFaces / 3);

    return Result;
}

// *************************************************************************
// *		    Get_Global_Face:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
int CL64_Picking::Get_Global_Face(int SelectedGroup)
{
    Ogre::MeshPtr mesh = ((Ogre::Entity*)pentity)->getMesh();

    //bool added_shared = false;
    int TotalFaces = 0;
    int Count = 0;
    //int SubMeshes = mesh->getNumSubMeshes();

    while (Count < SelectedGroup)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(Count);
        TotalFaces += submesh->indexData->indexCount;

        Count++;
    }

    int Result = (TotalFaces / 3);

    return Result;
}

// *************************************************************************
// *	        Get_Face:- Terry and Hazel Flanigan 2024		      	   *
// *************************************************************************
void CL64_Picking::Get_Face()
{
    Face_Count = 0;

    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);
    if (NumSelBrushes > 0)
    {
        Brush* pBrush;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, 0);
        Get_Brush_Info(pBrush);

       // App->Say_Int(Face_Count);
    }
}

// *************************************************************************
// *	  	Get_Brush_Info:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_Picking::Get_Brush_Info(const Brush* b)
{
    if (b->Type == BRUSH_MULTI)
    {
        return Get_Brush_ListInfo(b->BList);
    }
    if (b->Type == BRUSH_LEAF)
    {
        return Get_Brush_Faces_Info(b->Faces);
    }
    if (b->Type == BRUSH_CSG)
    {
        App->Say("BRUSH_CSG");
    }

    return 1;
}

// *************************************************************************
// *	  Get_Brush_Faces_Info:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Picking::Get_Brush_Faces_Info(const FaceList* pList)
{
    int i;
   
    if (pList->NumFaces < 0)
    {
    }
    else
    {
        for (i = 0; i < pList->NumFaces; i++)
        {
            if (!Get_Face_Data(i, pList->Faces[i])) return 0;
        }
    }

    return 1;
}

// *************************************************************************
// *	  	Get_Brush_ListInfo:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Picking::Get_Brush_ListInfo(BrushList* BList)
{
    Brush* pBrush;
    BrushIterator bi;
    int Count;

    Count = App->CL_Brush->BrushList_Count(BList, (BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF | BRUSH_COUNT_NORECURSE));
    if (Count < 0)
    {
        return 0;
    }
    else
    {
    }

    pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);
    while (pBrush != NULL)
    {
        Get_Brush_Info(pBrush);
        pBrush = App->CL_Brush->BrushList_GetNext(&bi);
    }

    return 1;
}

// *************************************************************************
// *		  Get_Face_Data:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_Picking::Get_Face_Data(int Index, const Face* f)
{
    Face_Count++;

    char buf[MAX_PATH];

    int  i = 0;

    const TexInfo_Vectors* TVecs = App->CL_Face->Face_GetTextureVecs(f);
    T_Vec3 uVec, vVec;
    float U, V;

    int txSize, tySize;

    App->CL_Face->Face_GetTextureSize(f, &txSize, &tySize);

    // make sure that the texture size is set correctly (division!)
    if (txSize == 0)
        txSize = 32;
    if (tySize == 0)
        tySize = 32;

    App->CL_Maths->Vector3_Scale(&TVecs->uVec, 1.f / (float)txSize, &uVec);
    App->CL_Maths->Vector3_Scale(&TVecs->vVec, -1.f / (float)tySize, &vVec);

    const T_Vec3* verts = App->CL_Face->Face_GetPoints(f);

    int j = 0;
    for (j = 0; j < f->NumPoints; j++)
    {
        U = App->CL_Maths->Vector3_DotProduct(&(verts[j]), &uVec);
        V = App->CL_Maths->Vector3_DotProduct(&(verts[j]), &vVec);
        U += (TVecs->uOffset / txSize);
        V -= (TVecs->vOffset / tySize);

        // sprintf(buf, "UV %.3f %.3f", U, V);
        // SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
    }

    strcpy(buf, App->CL_Face->Face_GetTextureName(f));

    return 1;
}

// *************************************************************************
// *		GetMeshInformation:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_Picking::GetMeshInformation(const Ogre::MeshPtr mesh, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    Total_vertex_count = Total_index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        // We only need to add the shared vertices once
        if (submesh->useSharedVertices)
        {
            if (!added_shared)
            {
                Total_vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            Total_vertex_count += submesh->vertexData->vertexCount;
        }

        // Add the indices
        Total_index_count += submesh->indexData->indexCount;
    }

    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[Total_vertex_count];
    indices = new Ogre::uint32[Total_index_count];
    TextCords = new Ogre::Vector2[Total_vertex_count];
    Sub_Mesh_Indexs = new Ogre::uint32[Total_index_count];

    added_shared = false;

    // ----------------- Vertices
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {

        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

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

            for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }

        // ----------------- Indices
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        Ogre::uint32* pLong = static_cast<Ogre::uint32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


        size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;

        if (use32bitindexes)
        {
            for (size_t k = 0; k < numTris * 3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<Ogre::uint32>(offset);
            }
        }
        else
        {
            for (size_t k = 0; k < numTris * 3; ++k)
            {
                indices[index_offset++] = static_cast<Ogre::uint32>(pShort[k]) +
                    static_cast<Ogre::uint32>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }

    // // ----------------- Texture Chords
    int textoffsset = 0;

    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if (submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* texElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);

            Ogre::HardwareVertexBufferSharedPtr vbufText =
                vertex_data->vertexBufferBinding->getBuffer(texElem->getSource());

            byte* vertexText = (byte*)vbufText->lock(Ogre::HardwareBuffer::HBL_NORMAL);
            float* pRealText;

            for (ulong j = 0; j < vertex_data->vertexCount; ++j, vertexText += vbufText->getVertexSize())
            {
                texElem->baseVertexPointerToElement(vertexText, &pRealText);

                TextCords[textoffsset].x = pRealText[0];
                TextCords[textoffsset].y = pRealText[1];

                Sub_Mesh_Indexs[textoffsset] = i; // Track which Submesh

                textoffsset++;
            }

            vbufText->unlock();
        }
    }
}

// *************************************************************************
// *		  Get_Material_Data:- Terry and Hazel Flanigan 2024		   	   *
// *************************************************************************
void CL64_Picking::Get_Material_Data()
{
    int test = ((Ogre::Entity*)pentity)->getMesh()->getNumSubMeshes();

    if (m_SubMesh > test)
    {
        strcpy(m_Texture_FileName, "No_Texture");
    }
    else
    {
        strcpy(FaceMaterial, ((Ogre::Entity*)pentity)->getMesh()->getSubMesh(m_SubMesh)->getMaterialName().c_str());
        Ogre::MaterialPtr  MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(FaceMaterial));
        strcpy(m_Texture_FileName, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());

    } 
}
