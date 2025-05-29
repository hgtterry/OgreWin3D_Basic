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

#pragma once
class CL64_Entities
{
public:
	CL64_Entities();
	~CL64_Entities();

	int GetIndex_By_Name(char* Name);
	void Create_Player_Brush();
	void Create_Environment_Brush(int Object_Index);

	bool Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity, Ogre::SceneNode* Ogre_Node);
	
	int Vertice_Count_Copy;
	std::vector<vertex_type> vertex_Data;			// XYZ
	T_Vec3	Verts[8];

private:

	bool Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity, Ogre::SceneNode* Ogre_Node);
	void Get_SubPose_MeshInstance(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& vertices, int SubMesh, Ogre::Node* Ogre_Node);

};

