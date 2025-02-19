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
#include "CL64_Picking.h"

CL64_Picking::CL64_Picking(Ogre::SceneManager* sceneMgr)
{
    mSceneMgr = sceneMgr;

    mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
    if (NULL == mRaySceneQuery)
    {
        App->Say("No Ray Query");
        return;
    }

    mParticleSceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
    if (NULL == mParticleSceneQuery)
    {
        App->Say("No Ray Query");
        return;
    }

    pentity = NULL;

    Total_vertex_count = 0;
    Total_index_count = 0;
    Face_Index = 0;
    Sub_Mesh_Count = 0;
    SubMesh_Face = 0;
    flag_Selected_Ok = 0;
    ParticleFound = 0;
    flag_Hit_Player = 0;

    FaceMaterial[0] = 0;
    TextureName[0] = 0;
    ParticleName[0] = 0;
    ParticleFound = 0;
    TestName[0] = 0;
}

CL64_Picking::~CL64_Picking(void)
{
    if (mRaySceneQuery != NULL)
    {
        delete mRaySceneQuery;
    }
}

// *************************************************************************
// *		  Clear_Picking_Data:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
void CL64_Picking::Clear_Picking_Data()
{
    Total_vertex_count = 0;
    Total_index_count = 0;
    Face_Index = 0;
    Sub_Mesh_Count = 0;
    SubMesh_Face = 0;
    flag_Selected_Ok = 0;
    ParticleFound = 0;
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
    App->CL_Ogre->Ogre3D_Listener->Selected_Object_Name[0] = 0;

    Ogre::SceneNode* mNode;

    Ogre::Ray ray2 = camera->getCameraToViewportRay(tx, ty);
    if (raycast(ray2, result, target, closest_distance, queryMask))
    {
        //App->Beep_Win();

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
                    (void) sscanf((buff + 6), "%i", &IntNum);

                    App->CL_Gizmos->MarkerBox_Addjust(IntNum);

                    App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index = IntNum;
                    strcpy(App->CL_Ogre->Ogre3D_Listener->Selected_Object_Name, App->CL_Scene->B_Object[IntNum]->Object_Name);

                    App->CL_FileView->SelectItem(App->CL_Scene->B_Object[App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index]->FileViewItem);

                    return;

                }

                pdest = strstr(buff, "Area_");
                if (pdest != NULL)
                {
                    (void) sscanf((buff + 5), "%i", &IntNum);

                    App->CL_Gizmos->MarkerBox_Addjust(IntNum);

                    App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index = IntNum;
                    strcpy(App->CL_Ogre->Ogre3D_Listener->Selected_Object_Name, App->CL_Scene->B_Area[IntNum]->Area_Name);

                    App->CL_FileView->SelectItem(App->CL_Scene->B_Area[App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index]->FileViewItem);

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

// *************************************************************************
// *		        raycast:- Terry and Hazel Flanigan 2024		       	   *
// *************************************************************************
bool CL64_Picking::raycast(const Ogre::Ray& ray, Ogre::Vector3& result, Ogre::MovableObject*& target, float& closest_distance, const Ogre::uint32 queryMask)
{
    target = NULL;
    ParticleFound = 0;
    Pl_Entity_Name = "---------";
    flag_Hit_Player = 0;

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

    // at this point we have raycast to a series of different objects bounding boxes.
    // we need to test these different objects to see which is the first polygon hit.
    // there are some minor optimizations (distance based) that mean we wont have to
    // check all of the objects most of the time, but the worst case scenario is that
    // we need to test every triangle of every object.
    //Ogre::Ogre::Real closest_distance = -1.0f;
    closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult query_result = mRaySceneQuery->getLastResults();



    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        strcpy(TextureName, "None");
        // stop checking if we have found a raycast hit that is closer
        // than all remaining entities
        if ((closest_distance >= 0.0f) &&
            (closest_distance < query_result[qr_idx].distance))
        {
            //strcpy(TextureName, query_result[0].movable->getMovableType().c_str());
            break;
        }

        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != NULL) &&
            (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0) &&
            ParticleFound == 0)
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
                for (size_t i = 0; i < Total_index_count; i += 3)
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

                            Face_Index = i;

                            /*App->CL_Grid->HitVertices[0] = vertices[indices[i]];
                            App->CL_Grid->HitVertices[1] = vertices[indices[i + 1]];
                            App->CL_Grid->HitVertices[2] = vertices[indices[i + 2]];

                            App->CL_Grid->Face_Update2();

                            App->CL_Grid->HitFaceUVs[0] = TextCords[Face_Index];
                            App->CL_Grid->HitFaceUVs[1] = TextCords[Face_Index + 1];
                            App->CL_Grid->HitFaceUVs[2] = TextCords[Face_Index + 2];*/

                            //SubMesh_Face = Sub_Mesh_Indexs[Face_Index];

                            //Get_Material_Data();

                            //App->CL_Grid->FaceNode->setVisible(true);
                        }
                    }
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

    // return the result
    if (closest_distance >= 0.0f)
    {
        // raycast success
        result = closest_result;
        flag_Selected_Ok = 1;
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

    // Run through the submeshes again, adding the data into the arrays
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

    // Texture Cords UVS
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

                Sub_Mesh_Indexs[textoffsset] = i;

                textoffsset++;
            }

            vbufText->unlock();
        }
    }

}

// *************************************************************************
// *			Set_Face_UV:- Terry and Hazel Flanigan 2024		       	   *
// *************************************************************************
void CL64_Picking::Set_Face_UV()
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    const Ogre::MeshPtr mesh = ((Ogre::Entity*)pentity)->getMesh();

    // Texture Cords UVS
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

                if (textoffsset == Face_Index)
                {
                    float v = pRealText[0];

                    // pRealText[0] = 0;
                    pRealText[0] = 1 - v;
                }

                if (textoffsset == Face_Index + 1)
                {
                    float v = pRealText[0];

                    //pRealText[0] = 0;
                    pRealText[0] = 1 - v;
                }

                if (textoffsset == Face_Index + 2)
                {
                    float v = pRealText[0];

                    // pRealText[0] = 1;
                    pRealText[0] = 1 - v;
                }

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

    if (SubMesh_Face > test)
    {
        //App->Say("Sub Mesh Out of bounds");
    }
    else
    {
        strcpy(FaceMaterial, ((Ogre::Entity*)pentity)->getMesh()->getSubMesh(SubMesh_Face)->getMaterialName().c_str());
        Ogre::MaterialPtr  MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(FaceMaterial));
        strcpy(TextureName, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());
    }
}

// *************************************************************************
// *		  Ray_Test_Particles:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_Picking::Ray_Test_Particles(const Ogre::Ray& ray)
{
    bool Hit = 0;

    if (mParticleSceneQuery != NULL)
    {
        mParticleSceneQuery->clearResults();
        mParticleSceneQuery->setRay(ray);
        mParticleSceneQuery->setSortByDistance(true);
        // mRaySceneQuery->setQueryMask(Ogre::SceneManager::FX_TYPE_MASK);
        mParticleSceneQuery->setQueryTypeMask(Ogre::SceneManager::FX_TYPE_MASK);
        // execute the query, returns a vector of hits
        if (mParticleSceneQuery->execute().size() <= 0)
        {
            // raycast did not hit an objects bounding box
            flag_Selected_Ok = 0;
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
    Ogre::RaySceneQueryResult query_result = mParticleSceneQuery->getLastResults();

    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        if ((query_result[qr_idx].movable != NULL) &&
            (query_result[qr_idx].movable->getMovableType().compare("ParticleSystem") == 0))
        {
            strcpy(TextureName, "Particle");
            Ogre::ParticleSystem* Particle = static_cast<Ogre::ParticleSystem*>(query_result[qr_idx].movable);
            //App->Say(Particle->getName().c_str());
            Pl_Entity_Name = Particle->getName();
            strcpy(ParticleName, Particle->getName().c_str());

            char buff[255];
            int IntNum = 0;

            strcpy(buff, Pl_Entity_Name.c_str());

            (void) sscanf((buff + 9), "%i", &IntNum);
            //App->Say_Int(IntNum);
            App->CL_Gizmos->MarkerBox_Addjust(IntNum);

            App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index = IntNum;
            strcpy(App->CL_Ogre->Ogre3D_Listener->Selected_Object_Name, App->CL_Scene->B_Object[IntNum]->Object_Name);

            App->CL_FileView->SelectItem(App->CL_Scene->B_Object[App->CL_Ogre->Ogre3D_Listener->Selected_Entity_Index]->FileViewItem);

            Hit = 1;
        }
    }

    if (Hit == 1)
    {
        mParticleSceneQuery->clearResults();
        return true;
    }
    else
    {
        mParticleSceneQuery->clearResults();
        Pl_Entity_Name = " -------- ";
        return false;
    }
}

