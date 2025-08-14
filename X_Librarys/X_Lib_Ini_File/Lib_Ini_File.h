/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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

#define BASE_BINARY			2
#define BASE_OCTAL			8
#define BASE_DECIMAL		10
#define BASE_HEXADECIMAL	16

class Lib_Ini_File
{
public:
	Lib_Ini_File(void);
	~Lib_Ini_File(void);

	char* GetVersion();

	void Clean_Up(void);
	void SetPathName(LPCTSTR lpPathName);

	int GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase) const;
	DWORD GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault = NULL) const;

	static int __ValidateBase(int nBase);
	LPTSTR __GetStringDynamic(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault = NULL) const;

	LPTSTR m_pszPathName;
};

