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
class CL64_Ogre3D
{
public:
	CL64_Ogre3D(void);
	~CL64_Ogre3D(void);

	void Set_Export_Paths(void);
	void Set_World_Paths(void);

	void Export_To_Ogre3D();
	void Export_MaterialFile(char* MatFileName);

	void Convert_ToOgre3D(bool Create);
	void CreateMaterialFile();


	void Get_Data(int Index, int FaceIndex);
	bool DecompileTextures_TXL(char* PathAndFile);
	bool Extract_TXL_Texture(char* Name, char* Folder);
	bool LoadTextures_TXL(char* Name);
	int WriteBMP8(const char* pszFile, geBitmap* pBitmap);

	Ogre::ManualObject* Export_Manual;
	Ogre::ManualObject* World_Manual;

	char mJustName[MAX_PATH];
	char mFolder_Path[MAX_PATH];
	char mSelected_Directory[MAX_PATH];
	char mDirectory_Name[MAX_PATH];
	char mExport_Path[MAX_PATH];
	char mExport_PathAndFile_Mesh[MAX_PATH];
	char mExport_PathAndFile_Material[MAX_PATH];

	char mWorld_Mesh_JustName[MAX_PATH];
	char mWorld_File_PathAndFile[MAX_PATH];
	char mWorld_File_Path[MAX_PATH];
	char mExport_Just_Name[MAX_PATH];
	char Material_PathAndFile[MAX_PATH];

	float nx;
	float ny;
	float nz;

	float u;
	float v;

	float x;
	float y;
	float z;

	int NameIndex;
};

