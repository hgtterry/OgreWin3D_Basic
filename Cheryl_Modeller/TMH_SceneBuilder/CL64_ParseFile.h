/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
class CL64_ParseFile
{
public:
	CL64_ParseFile(void);
	~CL64_ParseFile(void);

//private:

	BrushList* BrushList_CreateFromFile(bool SubBrush);
	Brush* Brush_CreateFromFile(bool SubBrush);
	FaceList* FaceList_CreateFromFile();
	Face* Face_CreateFromFile();

	bool Get_Version(char* Buffer);
	bool Get_TextureLib(char* Buffer);

	bool Get_String(const char* Should_Be, char* Chr_return);
	bool Get_Int(const char* Should_Be, int* Int_return);
	bool Get_Float(const char* Should_Be, float* Float_return);
	bool Get_Vector3(const char* Should_Be, T_Vec3* Vec3_return);
	bool Get_Vector2(const char* Should_Be, Ogre::Vector2* Vec2_return);
	bool Get_Matrix3d(const char* Should_Be, Matrix3d* Matrix3d_return);
	bool Get_Text_Info(const char* Should_Be, float* ret_Rotate, Ogre::Vector2* ret_Shift, Ogre::Vector2* ret_Scale, char* Chr_Texture);

	char str_buff_1[MAX_PATH];
	char str_buff_2[MAX_PATH];

	char Brush_Name[MAX_PATH];

	float Tag_Float;
	int Tag_Int;
	Ogre::Vector3 Tag_Vector3;

};

