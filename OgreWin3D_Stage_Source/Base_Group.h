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

class Base_Group
{
public:
	Base_Group();
	~Base_Group();

	char GroupName[MAX_PATH];
	char MaterialName[MAX_PATH];
	char Text_FileName[MAX_PATH]; // Just Texture File Name ie Texture.bmp

	char Equity_Text_FileName[255];	// Just Texture File Name ie Texture.bmp

	char Texture_FolderPath[MAX_PATH];
	char Texture_PathFileName[MAX_PATH];

	int GroupVertCount;	// Group Vertice Count
	int GroupFaceCount;
	int IndicesCount; // Ogre
	int MaterialIndex;
	int Soil_TextureIndex;

	int Depth;
	long Height;
	long Width;
	
	bool Bitmap_Loaded;

	std::vector<vertex_type> vertex_Data;			// XYZ
	std::vector<polygon_type> Face_Data;			// ABC
	std::vector<normal_type> Normal_Data;			// XYZ
	std::vector<mapcoord_type> MapCord_Data;		// UV
	std::vector<int_type> FaceIndex_Data;

	std::vector<int_type> BoneIndex_Data;			// Ogre
	std::vector<int_type> BA_BoneIndex_Data;		// Ogre
	std::vector<int_type> BA_BoneVertexIndex_Data;	// Ogre
	std::vector<float_type> BA_Weight_Data;			// Ogre
	int BoneAssignMentCount;						// Ogre

	HBITMAP Base_Bitmap;
	HTREEITEM ListView_Item;

	// Ogre Data
	char Ogre_TextureName[MAX_PATH];
	char Ogre_Material[MAX_PATH];
	char Ogre_Material_File[MAX_PATH];
	char Ogre_ImGui_MatId[MAX_PATH];
	int Ogre_MipMaps;
	int Ogre_NumTextureUnits;
	bool Ogre_Texture_IsValid;
};

