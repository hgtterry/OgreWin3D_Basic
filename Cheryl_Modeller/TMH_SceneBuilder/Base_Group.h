/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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

typedef struct vertex_type
{
	float x, y, z;
}vertex_type;

typedef struct polygon_type { int a, b, c, Group, Brush_Index, Main_Face; }polygon_type;
typedef struct normal_type { float x, y, z; }normal_type;
typedef struct mapcoord_type { float u, v; }mapcoord_type;
typedef struct int_type { int Index; }int_type;
typedef struct float_type { float Float1; }float_type;

class Base_Group
{
public:
	Base_Group();
	~Base_Group();

	char GroupName[255];
	char MaterialName[255];
	char Text_FileName[255];			// Just Texture File Name ie Texture.bmp

	char Equity_Text_FileName[255];			// Just Texture File Name ie Texture.bmp

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
	bool Has_Alpha;

	bool Dont_Use;

	std::vector<vertex_type> vertex_Data;		// XYZ
	std::vector<polygon_type> Face_Data;		// ABC
	std::vector<normal_type> Normal_Data;		// XYZ
	std::vector<mapcoord_type> MapCord_Data;	// UV
	std::vector<int_type> FaceIndex_Data;

	std::vector<int_type> BoneIndex_Data; // Ogre
	std::vector<int_type> BA_BoneIndex_Data; // Ogre
	std::vector<int_type> BA_BoneVertexIndex_Data; // Ogre
	std::vector<float_type> BA_Weight_Data; // Ogre
	int BoneAssignMentCount; // Ogre

	// Bounding Box
	vertex_type BB_Max;
	vertex_type BB_Min;
	vertex_type Size;
	vertex_type Centre;
	float radius;

	HBITMAP Base_Bitmap;
	HTREEITEM ListView_Item;

	// Ogre Data
	char Ogre_Texture_FileName[MAX_PATH];
	char Ogre_Material[MAX_PATH];
	char Ogre_Material_File[MAX_PATH];
	char Ogre_ImGui_MatId[MAX_PATH];
	int Ogre_MipMaps;
	int Ogre_NumTextureUnits;
	bool Ogre_Texture_IsValid;

};

