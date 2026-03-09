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
#include "CL64_Exporters.h"

CL64_Exporters::CL64_Exporters(void)
{
}

CL64_Exporters::~CL64_Exporters(void)
{
}

// *************************************************************************
// *	Export_Wavefront_Object_Model:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Exporters::Export_Wavefront_Object_Model(void)
{
	if (App->CL_Model->flag_Model_Loaded == false)
	{
		App->Say("No Model Loaded to Export");
		return;
	}

	App->CL_Exp_Obj->Object_Export_Dlg();

	//bool test = App->CL_Exp_Obj->Create_ObjectFile();

	//if (test == 1)
	{
		App->Say("Wavefront Object file Created successfully");
	}
}
