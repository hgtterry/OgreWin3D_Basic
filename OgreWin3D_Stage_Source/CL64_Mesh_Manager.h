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

#pragma once
class CL64_Mesh_Manager
{
public:
	CL64_Mesh_Manager(void);
	~CL64_Mesh_Manager(void);

	bool Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity, Ogre::SceneNode* Ogre_Node, int Usage);
	bool Has_Shared_Vertices;

private:

	void Create_MeshGroups(Ogre::Entity* Ogre_Entity, int Usage);

	bool Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity, int Usage);

	void Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
		size_t& vertex_count, Ogre::Vector3*& vertices,
		size_t& index_count, unsigned long*& indices,
		int SubMesh, Ogre::int16*& BoneIndices);

	int Current_Index;

	Ogre::Vector3 m_position;
	Ogre::Quaternion m_orient;
	Ogre::Vector3 m_scale;

};

