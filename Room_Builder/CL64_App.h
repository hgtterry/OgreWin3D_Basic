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

class CL64_App
{
public:
	CL64_App(void);
	~CL64_App(void);

	CL64_MapEditor* CL_MapEditor;
	CL64_Maths*		CL_Maths;
	CL64_Box*		CL_Box;
	CL64_Render*	CL_Render;
	CL64_Level*		CL_Level;

	void InitApp(void);
	void SetBrushes_Fonts(void);
	void Flash_Window();
	void SetMainWinCentre(void) const;

	HINSTANCE hInst;
	HWND MainHwnd;
	HBRUSH AppBackground;
};

extern CL64_App* App;
