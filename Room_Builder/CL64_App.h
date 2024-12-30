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

#include "CL64_MapEditor.h"
#include "CL64_Maths.h"
#include "CL64_Box.h"
#include "CL64_Render.h"
#include "CL64_Level.h"
#include "CL64_Prefs.h"
#include "CL64_Ini_File.h"
#include "CL64_Doc.h"
#include "CL64_Utilities.h"
#include "CL64_Brush.h"
#include "CL64_FaceList.h"
#include "CL64_Face.h"
#include "CL64_BrushTemplate.h"
#include "CL64_SelBrushList.h"
#include "CL64_Array.h"
#include "CL64_SelFaceList.h"

class CL64_App
{
public:
	CL64_App(void);
	~CL64_App(void);

	CL64_MapEditor*		CL_MapEditor;
	CL64_Maths*			CL_Maths;
	CL64_Box*			CL_Box;
	CL64_Render*		CL_Render;
	CL64_Level*			CL_Level;
	CL64_Prefs*			CL_Prefs;
	CL64_Ini_File*		CL_Ini_File;
	CL64_Doc*			CL_Doc;
	CL64_Utilities*		CL_Utilities;
	CL64_Brush*			CL_Brush;
	CL64_FaceList*		CL_FaceList;
	CL64_Face*			CL_Face;
	CL64_BrushTemplate* CL_BrushTemplate;
	CL64_SelBrushList*	CL_SelBrushList;
	CL64_Array*			CL_Array;
	CL64_SelFaceList*	CL_SelFaceList;


	void InitApp(void);
	void SetBrushes_Fonts(void);
	void Flash_Window();
	void SetMainWinCentre(void) const;
	void Say_Win(const char* Message);

	HINSTANCE hInst;
	HWND MainHwnd;
	HBRUSH AppBackground;

	char RB_Directory_FullPath[MAX_PATH];
};

extern CL64_App* App;

