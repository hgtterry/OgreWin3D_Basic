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

#include "pch.h"
#include "CL64_App.h"

CL64_App::CL64_App(void)
{
	CL_MapEditor = nullptr;



	hInst = nullptr;
	MainHwnd = nullptr;
	Fdlg = nullptr;

	AppBackground = 0;
}

CL64_App::~CL64_App(void)
{
}

// *************************************************************************
// *			InitApp:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void CL64_App::InitApp(void)
{
	CL_MapEditor = new CL64_MapEditor();
}

// *************************************************************************
// *			SetBrushes_Fonts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_App::SetBrushes_Fonts(void)
{
	AppBackground = CreateSolidBrush(RGB(213, 222, 242));
}
