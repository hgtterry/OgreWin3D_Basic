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

class CL64_File
{
public:
	CL64_File(void);
	~CL64_File(void);

	void Start_Save(bool useSaveDialog);
	void Start_Load(bool useOpenDialog);

	void Set_Editor();

	char Read_Buffer[MAX_PATH];
	char PathFileName_3dt[MAX_PATH];
	char FileName_3dt[MAX_PATH];

	bool flag_loading;

	FILE* fp;
private:

	void Save_Document();
	bool Save(const char* FileName);

	bool Open_3dt_File();
	bool Load_File(const char* FileName);
	void Set_Player();
	void Check_Missing_Brushes();

	signed int BrushList_Write(BrushList* BList, FILE* ofile);
	signed int Brush_Write(const Brush* b, FILE* ofile);
	signed int	FaceList_Write(const FaceList* pList, FILE* f);
	signed int Face_Write(const Face* f, FILE* wf);
};

