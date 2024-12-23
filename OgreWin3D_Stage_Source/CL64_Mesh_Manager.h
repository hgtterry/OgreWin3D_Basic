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

	bool Ogre_To_Mesh_Data(Ogre::Entity* Ogre_Entity);
	void Get_Ogre_Mesh_Data(Ogre::Entity* Ogre_Entity);

	bool Has_Shared_Vertices;

private:

	bool Convert_To_Mesh_Data(Ogre::Entity* Ogre_Entity);

	void Get_SubPose_MeshInstance(Ogre::MeshPtr mesh,
		size_t& vertex_count, Ogre::Vector3*& vertices,
		size_t& index_count, unsigned long*& indices,
		int SubMesh, Ogre::int16*& BoneIndices);

	bool Get_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh);

	bool Get_SubPoseNormals(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& Normals,
		int SubMesh);

	bool GetBoneAssignment(Ogre::MeshPtr mesh, int SubMesh, HWND hDlg);

	std::vector<Ogre::Vector2> MeshTextureCoords;

};

