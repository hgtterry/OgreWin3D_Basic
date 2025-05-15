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
class CL64_Brush_X
{
public:
	CL64_Brush_X();
	~CL64_Brush_X();

	void BrushList_GetUsedTextures_X(BrushList* BList, signed int* UsedTex);
	int Get_Brush_All_Faces_Count(void);
	int	BrushList_EnumLeafBrushes(const BrushList* pList, void* pVoid, BrushList_CB	CallBack);
	bool Check_if_Brush_Name_Exist(char* Name);
	void Set_Brush_Faces_Name(Brush* b);
	Brush* Get_Brush_By_Name(const char* Name);
	int Get_Brush_Index_By_Name(const char* Name);
	void Select_Brush_Editor(Brush* b);

	void Set_Brush_Faces_Index(Brush* b);
	bool Decode_Brush(const Brush* b);
	bool Set_Sub_Brush_Faces_Indexs(BrushList* BList);
	bool Set_Brush_Faces_Indexs(const FaceList* pList);
	bool Set_Face_Index(int Index, Face* f);
	void Move_Player_Brush();

	void Move_Brush_By_Name(char* Brush_Name,int Object_Index);
	void Scale_Brush_By_Name(const char* Brush_Name, int Object_Index,float SX, float SY, float SZ);

	void Rotate_Reset_Brush_By_Name(const char* Brush_Name, float SX, float SY, float SZ);
	void Rotate_Brush_By_Name(const char* Brush_Name, float SX, float SY, float SZ);

	void Set_Brush_Face_Points(Brush* pBrush);

	int Face_Count;
	int Face_Index_Set;

	T_Vec3 Last_FinalRot;

private:

	void Get_BrushData(Brush* b);
	bool Get_Brush_Info(const Brush* b);
	bool Get_Brush_ListInfo(BrushList* BList);
	bool Get_Brush_Faces_Info(const FaceList* pList);
	bool Get_Face_Data(int Index, const Face* f);

};

