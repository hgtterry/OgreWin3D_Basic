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
class CL64_Picking
{
public:

	CL64_Picking(Ogre::SceneManager* sceneMgr);
	~CL64_Picking(void);

	void Mouse_Pick_Entity();
	void Set_Face_UV();
	void Clear_Picking_Data();
	bool Ray_Test_Particles(const Ogre::Ray& ray);
	bool Hit_Player;

	Ogre::String Pl_Entity_Name;
	float closest_distance;

	Ogre::Vector3* vertices;
	Ogre::Vector2* TextCords;
	Ogre::uint32* indices;

	Ogre::uint32* Sub_Mesh_Indexs;

	int SubMesh_Face;

	char TestName[MAX_PATH];
	char FaceMaterial[MAX_PATH];
	char TextureName[MAX_PATH];
	char ParticleName[MAX_PATH];
	bool ParticleFound;
	size_t Total_vertex_count;
	size_t Total_index_count;

	int Face_Index;
	int Sub_Mesh_Count;

	bool Selected_Ok;

private:
	bool raycast(const Ogre::Ray& ray, Ogre::Vector3& result, Ogre::MovableObject*& target, float& closest_distance, const Ogre::uint32 queryMask);
	void GetMeshInformation(const Ogre::MeshPtr mesh, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
	void Get_Material_Data();

	Ogre::SceneManager* mSceneMgr;
	Ogre::RaySceneQuery* mRaySceneQuery;
	Ogre::RaySceneQuery* mParticleSceneQuery;

	Ogre::MovableObject* pentity;
};

