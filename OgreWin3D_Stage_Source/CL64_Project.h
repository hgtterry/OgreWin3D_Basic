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

// ------------------------ Load Options
typedef struct Load_Options
{
	int Has_Area;
	int Has_Player;
	int Has_Camera;
	int Has_Objects;
	int Has_Counters;

}Load_Options;

class CL64_Project
{
public:
	CL64_Project();
	~CL64_Project();

	bool Load_Project();
	bool Load_Project_Aera();

	char m_Project_Name[MAX_PATH];
	char m_Level_Name[MAX_PATH];

	char m_Ini_Path_File_Name[MAX_PATH];
	char m_Level_File_Name[MAX_PATH];
	char m_Project_Sub_Folder[MAX_PATH];
	char m_Level_Folder_Path[MAX_PATH];

	char m_Main_Assets_Path[MAX_PATH];

private:
	void Set_Paths();
	bool Load_Get_Resource_Path();
};

