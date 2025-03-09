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

typedef	struct	Face_Data
{
	char	Brush_Name[MAX_PATH];
}	Face_Data;

typedef struct tag_Level3 Level3;

class CL64_Mesh_Mgr
{
public:
	CL64_Mesh_Mgr();
	~CL64_Mesh_Mgr();

	bool Update_World();
	void Brush_Build_List(int ExpSelected);
	bool Brush_Build_Level_Brushes(Level3* pLevel, const char* Filename, BrushList* BList, int ExpSelected, signed int ExpLights, int GroupID);
	bool Brush_Decode_List(BrushList* BList, signed int SubBrush);
	bool Brush_Create(const Brush* b, int Actual_Brush_Index);
	bool Brush_FaceList_Create(const Brush* b, const FaceList* pList, int BrushCount, int SubBrushCount, int Actual_Brush_Index);

	bool WE_Convert_All_Texture_Groups();
	int WE_Get_Vertice_Count(int TextureID);
	bool WE_Convert_To_Texture_Group(int TextureID);

	void Delete_Brush_List();
	void Delete_Group_Brushes();

	int Get_Adjusted_Index(int RealIndex);

	void Create_V_Face(int Index);

	//bool AddTexture_GL(geVFile* BaseFile, const char* TextureName, int GroupIndex);

	int ActualFaceCount;
	int m_Total_Faces;

	Ogre::SceneNode* World_Node;
	Ogre::Entity* World_Ent;

	int	mBrushCount;
	int	mSubBrushCount;
	int mTextureCount;
	int Global_Faces_Index;
	int Actual_Brush_Index;
	int mAdjusedIndex_Store[500];
	int IsTextureAlpha[20];
	int mBrush_Index;

	signed int UsedTextures[500];

	char Actual_mBrush_Name[MAX_PATH];
	int v_Face_Data_Count;
	Face_Data* v_Face_Data[1000];

	char mBrush_Name[MAX_PATH];
	char TextureName2[20][MAX_PATH];
};

