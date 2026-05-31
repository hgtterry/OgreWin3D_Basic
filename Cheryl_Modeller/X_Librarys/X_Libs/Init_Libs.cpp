/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
#include "Init_Libs.h"

Init_Libs::Init_Libs(void)
{
	CL_CreateBox =		nullptr;
	CL_CreateCylinder = nullptr;
	CL_CreateCone =		nullptr;
	CL_CreateArch =		nullptr;

	CL_Preference =		nullptr;
}

Init_Libs::~Init_Libs(void)
{
}

// *************************************************************************
// *			Start_Libraries:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void Init_Libs::Start_Libraries()
{
	CL_CreateBox =		new CreateBoxDialog();
	CL_CreateCylinder = new CreateCylDialog();
	CL_CreateCone =		new CreateConeDialog();
	CL_CreateArch =		new CreateArchDialog();

	//CL_Preference =		new Lib_Preference();
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2026			 	   *
// *************************************************************************
char* Init_Libs::GetVersion()
{
	return (LPSTR)" Init_Libs [ 31-05-26 ] Build T ";
}

// *************************************************************************
// *			Get_Time_Stamp:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
char* Init_Libs::Get_Time_Stamp()
{
	static char buf[MAX_PATH];
	sprintf(buf, "Build Date  %s Time %s", __DATE__, __TIME__);

	return (LPSTR)buf;
}
