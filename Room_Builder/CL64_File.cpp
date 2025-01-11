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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_File.h"

CL64_File::CL64_File(void)
{
}

CL64_File::~CL64_File(void)
{
}

// *************************************************************************
// *	            Save:- Terry and Hazel Flanigan 2025	               *
// *************************************************************************
void CL64_File::Save()
{
    int BC = App->CL_Brush->Get_Brush_Count();
    if (BC > 0)
    {
        Save_Document();
        //App->Say("Saved", App->CL_World->mCurrent_3DT_PathAndFile);
    }
    else
    {
        App->Say("No Brushes to Save");
    }
}

// *************************************************************************
// *	          Save_Document:- Terry and Hazel Flanigan 2025	           *
// *************************************************************************
void CL64_File::Save_Document()
{
    if (Save(App->CL_Doc->mCurrent_MTF_PathAndFile) == GE_FALSE)
    {
        App->Say("Error: Unable to save file");
        return;;
    }

    App->CL_Doc->flag_IsNewDocument = 0;
    App->CL_Doc->flag_Is_Modified = false;
}

// *************************************************************************
// *				Save:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_File::Save(const char* FileName)
{
    Debug
    return 1;
}
