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
class CL64_Picking
{
public:
	CL64_Picking(void);
	~CL64_Picking(void);

	Ogre::RaySceneQuery* mRaySceneQuery;
	Ogre::MovableObject* target;

	void Init_Picking();
	void Mouse_Pick_Entity(bool JustTexture);
	int Get_Local_Face(int SelectedGroup);

	Ogre::MovableObject* pentity;
	Ogre::String Pl_Entity_Name;

	float closest_distance;

	char TestName[MAX_PATH];
	char FaceMaterial[MAX_PATH];
	char TextureName[MAX_PATH];
	char m_Texture_FileName[MAX_PATH];

	int m_Main_Face;
	int Face_Index;
	int Sub_Mesh_Count;
	int m_SubMesh;
	
	int Local_Face;
	int Group_Index;
	int Brush_Index;

	bool flag_Selected_Ok;
	bool flag_Hit_Player;

	size_t Total_vertex_count;
	size_t Total_index_count;

private:

	void Clear_Picking_Data();
	bool raycast(const Ogre::Ray& ray, Ogre::Vector3& result, Ogre::MovableObject*& target, float& closest_distance, const Ogre::uint32 queryMask);
	void GetMeshInformation(const Ogre::MeshPtr mesh, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
	
	void Get_Material_Data();
	int  Get_Group_Index();
	int Get_Brush_Index();

	void Get_Face();
	bool Get_Brush_Info(const Brush* b);
	bool Get_Brush_Faces_Info(const FaceList* pList);
	bool Get_Brush_ListInfo(BrushList* BList);
	bool Get_Face_Data(int Index, const Face* f);

	bool flag_Just_Texture;

	Ogre::Vector3* vertices;
	Ogre::Vector2* TextCords;
	Ogre::uint32* indices;
	Ogre::uint32* Sub_Mesh_Indexs;

};

