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

#pragma once

typedef struct vertex_type
{
	float x, y, z;
}vertex_type;

typedef struct polygon_type { int a, b, c, Group; }polygon_type;
typedef struct normal_type { float x, y, z; }normal_type;
typedef struct mapcoord_type { float u, v; }mapcoord_type;
typedef struct int_type { int Index; }int_type;
typedef struct float_type { float Float1; }float_type;

typedef struct Sub_Mesh_type
{
	std::vector<vertex_type> vertex_Data;			// XYZ
	std::vector<polygon_type> Face_Data;			// ABC
	std::vector<normal_type> Normal_Data;			// XYZ
	std::vector<mapcoord_type> MapCord_Data;		// UV
	std::vector<int_type> FaceIndex_Data;

	int Vertice_Count;
	int Face_Count;
	int IndicesCount;

	char chr_SubMesh_Name[MAX_PATH];

}Sub_Mesh_type;

class Base_Group
{
public:
	Base_Group();
	~Base_Group();

	char GroupName[MAX_PATH];

	int Sub_Group_Count;
	int GroupVertCount;	// Group Vertice Count
	int GroupFaceCount;
	int IndicesCount; // Ogre
	
	Sub_Mesh_type* B_Sub_Mesh[100];
};

