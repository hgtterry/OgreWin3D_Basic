/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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

#include "unzip.h"
#include "zip.h"

class CL64_Utilities
{
public:
	CL64_Utilities(void);
	~CL64_Utilities(void);

	bool Check_File_Exist(char* Full_Path);
	void Get_FileName_FromPath(char* pString, char* FileName);
	void Delete_Folder_Contents(char* Folder);
	void UnZip_Test();
	void UnZip_Test_2();

	ZRESULT RemoveFileFromZip(const TCHAR* zipfn, const TCHAR* zename);
	ZRESULT AddFileToZip(const TCHAR* zipfn, const TCHAR* zename, const TCHAR* zefn);

	char JustFileName[MAX_PATH];
};

